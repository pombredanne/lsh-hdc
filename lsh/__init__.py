# -*- coding: utf-8 -*-
"""
lsh.py

Algorithms based on 'Mining of Massive Datasets'
"""

import re
from unionfind import UnionFind
from collections import defaultdict
import HTMLParser


def shingle(text, n):
    return set([text[i:i + n] for i in range(len(text) - n + 1)])


class Shingler:
    def __init__(self, n, pattern=None):
        if pattern is None:
            # match any Unicode word optionally preceded by #, $, or @ characters
            #
            #pattern = ur'(?u)\w+'
            #pattern = ur'(?:\B[#@$£€¥₩฿])?(?u)\w+(?:[%\+]\B)?'
            pattern = ur'''
                        (?:                # Either URL
                        http[s]?://(?:[a-zA-Z]|[0-9]|[$-_@.&+]|[!*\(\),]|(?:%[0-9a-fA-F][0-9a-fA-F]))+
                        |                  # or
                        (?:\B[#@$£€¥₩฿])?  # (preceded by optional pound-, at-, or currency signs)
                        (?u)\w+            # a Unicode word
                        (?:[%\+]\B)?       # optionally followed by percentage or plus signs
                        )
                        '''
        self.r = re.compile(pattern, (re.VERBOSE | re.UNICODE))
        self.n = n
        self.html_parser = HTMLParser.HTMLParser()

    def normalize(self, text):
        #TODO: consider using a try/catch block
        return self.html_parser.unescape(text).lower()

    def get_shingles(self, text):
        """Get shingles (n-grams) from text

        :param text: Source text to process
        :type text: unicode
        :return: a collection of shingles found
        :rtype: set
        """
        n_ = self.n
        shingles = set()

        tokens = self.r.findall(self.normalize(text))
        if len(tokens) >= n_:
            for offset in xrange(len(tokens) - n_ + 1):
                shingles.add(tuple(tokens[offset:(offset + n_)]))
        else:
            shingles.add(tuple(tokens))
        return shingles


def jaccard_sim(x, y):
    """Jaccard similarity between two sets"""
    set_x = set(x)
    set_y = set(y)
    return float(len(set_x & set_y)) / float(len(set_x | set_y))


def get_bandwidth(n, threshold):
    """Approximates the bandwidth (number of rows in each band)
    needed to get threshold.

    Threshold t = (1/b) ** (1/r) where
    b = #bands
    r = #rows per band
    n = b * r = #elements in signature
    """

    best = n, 1
    min_err = float("inf")
    for r in range(1, n + 1):
        try:
            b = 1. / (threshold ** r)
        except ZeroDivisionError:
            return best
        err = abs(n - b * r)
        if err < min_err:
            best = r
            min_err = err
    return best


class Signature:
    """Signature Base class."""

    def __init__(self, dim):
        self.dim = dim
        self.hashes = self.hash_functions()

    def hash_functions(self):
        """Returns dim different hash functions"""
        pass

    def get_signature(self, obj):
        """Return the signature for object"""
        pass


class MinHashSignature(Signature):
    """Creates signatures for sets/tuples using minhash."""

    def hash_functions(self):
        """Return dim different hash functions"""
        def hash_factory(n):
            return lambda x: hash("salt" + str(n) + str(x) + "salt")
        return [hash_factory(_) for _ in range(self.dim)]

    def get_signature(self, s):
        """Returns minhash signature for set s -- which
        is a list of list consisting of hashings for each value and has function"""
        return [min(hash_fun(value) for value in s) for hash_fun in self.hashes]


class LSH:
    """Locality sensitive hashing.  Uses a banding approach to hash
    similar signatures to the same buckets."""
    def __init__(self, length, threshold):
        #self.length = length
        #self.threshold = threshold
        self.bandwidth = get_bandwidth(length, threshold)

    def hash(self, sig):
        """Generate hashvals for this signature

        The zip() clause converts a 1D-list to a list of b-dimensional
        tuples such that:
        [1,2,3,4,5,6] -> [(1,2), (3,4), (5,6)] if bandwidth == 2
                      -> [(1,2,3), (4,5,6)]    if bandwidth == 3
        """
        for band in zip(*(iter(sig),) * self.bandwidth):
            yield hash("salt" + str(band) + "tlas")

    '''
    def get_threshold(self):
        r = self.bandwidth
        b = self.length / r
        return (1. / b) ** (1. / r)
    '''


class Cluster:
    """Clusters sets with Jaccard similarity above threshold with high
    probability.

    Algorithm based on Rajaraman, "Mining of Massive Datasets":
    1. Generate set signature
    2. Use LSH to map similar signatures to same buckets
    3. Use UnionFind to merge buckets containing same values
    """
    def __init__(self, width=10, threshold=0.5):
        self.width = width
        self.unionfind = UnionFind()
        self.signer = MinHashSignature(width)
        self.hasher = LSH(width, threshold)
        self.hashmap = defaultdict(list)

    def add_set(self, s, label=None):
        # A label for this set
        if not label:
            label = s

        # Add to unionfind structure
        self.unionfind[label]

        # Get signature
        sig = self.signer.get_signature(s)

        # Union labels with same LSH keys
        for value in self.hasher.hash(sig):
            self.hashmap[value].append(label)
            self.unionfind.union(label, self.hashmap[value][0])

    def get_sets(self):
        return self.unionfind.sets()
