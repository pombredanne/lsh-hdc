#!/usr/bin/env python2
__author__ = 'escherba'

import argparse
import sys
import json
import calendar
import dateutil.parser as dateutil_parser
from collections import Counter
from functools import partial
from itertools import imap, islice
from math import log
from lsh import Cluster, WordShingler
from test.utils import sort_by_length, JsonRepr, read_json_file


class Options(JsonRepr):
    """Command-line option globals
    """
    file_path = None
    output_path = None
    bands = 3
    bandwidth = 3
    shingle_size = 3
    quiet = False
    no_user_id = False
    min_cluster = 3
    head = None
    #timestamp = False
    #alias = False


def mac_get_post_id(json_obj, n):
    return json_obj[u'object'][u'post_id'] + '.' + str(n)


class MACShingler(WordShingler):

    def __init__(self, options):
        WordShingler.__init__(self, options.shingle_size)
        self.options = options

    def shingles_from_mac(self, mac_obj):
        obj = mac_obj[u'object']
        shingles = self.get_shingles(obj[u'content'])
        if not self.options.no_user_id:
            shingles.add((obj[u'user_id'],))

        #if options.timestamp:
        #    shingles.add((obj[u'timestamp'],))

        #if options.alias and u'alias' in obj:
        #    shingles.add((obj[u'alias'],))

        return shingles


def entropy(N, n):
    """

    :param N: sample count
    :param n: number of bits
    :return: (Information) entropy
    :rtype: float
    """
    n_ = float(n)
    if n_ > 0.0:
        ratio = n_ / float(N)
        return - ratio * log(ratio)
    else:
        return 0.0


def average(l):
    """Find average
    :param l: a list of numbers
    :type l: list
    :returns: average
    :rtype: float
    """
    xs = list(l)
    return float(reduce(lambda x, y: x + y, xs)) / float(len(xs))


def sumsq(l):
    """Sum of squares
    :param l: a list of numbers
    :type l: list
    :returns: sum of squares
    :rtype: float
    """
    xs = list(l)
    avg = average(xs)
    return sum((el - avg) ** 2 for el in xs)


class Summarizer:
    def add_object(self, *args, **kwargs):
        pass

    def get_summary(self):
        pass


class TimeVarianceSummarizer(Summarizer):
    def __init__(self):
        self.residual = 0.0
        self.all = []

    def add_object(self, obj):
        """

        :param obj: a list
        :type obj: list
        """
        self.residual += sumsq(obj)
        self.all.extend(obj)

    def get_summary(self):
        """

        :rtype : float
        """
        try:
            result = 1.0 - self.residual / sumsq(self.all)
        except ZeroDivisionError:
            result = None
        return result


class UncertaintySummarizer(Summarizer):
    def __init__(self):
        self.multiverse = Counter()
        self.numerator = 0.0
        self.cluster_count = 0
        self.post_count = 0

    def add_object(self, obj, cluster_size):
        """

        :param obj: a mapping from keys to counts
        :type obj: collections.Counter
        """
        self.numerator += \
            sum(imap(partial(entropy, cluster_size), obj.values()))
        self.multiverse.update(obj)
        self.cluster_count += 1
        self.post_count += cluster_size

    def get_summary(self):
        """

        :rtype : float
        """
        try:
            denominator = float(self.cluster_count) * \
                sum(imap(partial(entropy, self.post_count),
                         self.multiverse.values()))
            result = 1.0 - self.numerator / denominator
        except ZeroDivisionError:
            result = None
        return result


def print_mac_stats(clusters, options=None):
    """

    :throws ZeroDivisionError:
    :returns: Theil uncertainty index (a homogeneity measure)
    :rtype: dict
    """

    cluster_count = 0
    post_count = 0
    tag_counter = Counter()
    shingler = MACShingler(options)
    usumm = UncertaintySummarizer()
    tcoef = TimeVarianceSummarizer()

    for cluster_id, cluster in enumerate(islice(clusters, 0, options.head)):
        universe = Counter()
        times = []
        posts = cluster[u'posts']
        cluster_size = len(posts)
        if cluster_size >= options.min_cluster:
            cluster_count += 1
            for post_id, json_obj in posts.iteritems():
                try:
                    tags = json_obj[u'impermium'][1][u'4.0'][u'tags']
                except (KeyError, TypeError):
                    tags = []
                tag_counter.update(tags)
                timestamp = dateutil_parser.parse(json_obj[u'object'][u'timestamp'])
                times.append(calendar.timegm(timestamp.utctimetuple()))
                universe.update(shingler.shingles_from_mac(json_obj))

            post_count += cluster_size
            tcoef.add_object(times)
            usumm.add_object(universe, cluster_size)

    print json.dumps({
        'options': options.as_dict(),
        'stats': {
            'uncertainty_index': usumm.get_summary(),
            'time_coeff': tcoef.get_summary(),
            'num_clusters': cluster_count,
            'num_comments_in_clusters': post_count,
            'impermium_tags': tag_counter
        }
    })


def cluster_from_mac_log(options):

    def output_clusters(unfiltered_sets, data):
        if options.output_path:
            fh = open(options.output_path, 'w')
        else:
            fh = None
        for cluster_id, cluster in enumerate(sort_by_length(unfiltered_sets)):
            parcel = {
                "cluster_id": cluster_id,
                "length": len(cluster),
                "posts": {post_id: data[post_id] for post_id in cluster}
            }
            if fh:
                print >>fh, json.dumps(parcel)
            yield parcel

    cluster_builder = Cluster(bands=options.bands,
                              bandwidth=options.bandwidth)
    shingler = MACShingler(options)

    data = {}
    with open(options.file_path) as mac_log:
        for line_num, line in enumerate(islice(mac_log, 0, options.head)):
            if (not options.quiet) and (not line_num % 10000):
                sys.stderr.write("Processing line " + str(line_num) + "\n")
            json_obj = json.loads(line)
            post_id = mac_get_post_id(json_obj, line_num)
            cluster_builder.add_set(shingler.shingles_from_mac(json_obj), post_id)
            data[post_id] = json_obj

    clusters = cluster_builder.get_clusters()
    transformed_clusers = output_clusters(clusters, data)
    print_mac_stats(transformed_clusers,
                    options=options)


def get_options(args):
    """Merge options with defaults

    :param args: command-line arguments parsed by argparse
    :type args: argparse.Namespace
    :returns: an instance of Options class
    :rtype : instance
    """
    options = Options()
    options.assign(args)
    return options


def cluster_mac_log(args):
    """Cluster entrypoint

    Processes a MAC log
    :param args: command-line arguments parsed by argparse
    :type args: argparse.Namespace
    """
    cluster_from_mac_log(get_options(args))


def summarize_mac_log(args):
    """Summary entrypoint

    Summarize an intermediate
    :param args: command-line arguments parsed by argparse
    :type args: argparse.Namespace
    """
    options = get_options(args)
    print_mac_stats(read_json_file(options.file_path),
                    options=options)


if __name__ == '__main__':
    # A sample bash-script illustrating how to run this
    #
    # python process_MAC_log.py \
    #     --shingle_size 4 \
    #     --quiet \
    #     --file data/detail.log.1 > /dev/null
    #
    parser = argparse.ArgumentParser(description='Perform clustering.')

    # add common arguments up here
    parser.add_argument('--quiet', action='store_true',
                        help='whether to be quiet', required=False)
    parser.add_argument('--min_cluster', type=int, dest='min_cluster', default=3,
                        help='minimum cluster size for quality evaluation', required=False)
    parser.add_argument('--head', type=int, dest='head', default=None,
                        help='how many lines from file to process (all if not set)', required=False)
    parser.add_argument('--file', type=str, dest='file_path', required=True,
                        help='Path to log file to process (required)')

    # for specialized functionality, use subparsers
    subparsers = parser.add_subparsers()

    # subparser: cluster
    parser_cluster = subparsers.add_parser('cluster', help='cluster a MAC log file and produce an intermediate')
    parser_cluster.add_argument('--shingle_size', type=int, dest='shingle_size', default=3,
                                help='shingle length (in tokens)', required=False)
    parser_cluster.add_argument('--bands', type=int, dest='bands', default=3,
                                help='number of bands', required=False)
    parser_cluster.add_argument('--bandwidth', type=int, dest='bandwidth', default=3,
                                help='rows per band', required=False)
    parser_cluster.add_argument('--no_user_id', action='store_true',
                                help='exclude user_id field', required=False)
    parser_cluster.add_argument('--output', type=str, dest='output_path', required=False,
                                help='Path to output')
    parser_cluster.set_defaults(func=cluster_mac_log)

    # subparser: summary
    parser_summary = subparsers.add_parser('summary', help='summary an intermediate')
    parser_summary.set_defaults(func=summarize_mac_log)

    # standard arg processing...
    args = parser.parse_args()
    args.func(args)
