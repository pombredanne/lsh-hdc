.PHONY: clean virtualenv upgrade test package dev eval_clusters roc

PYENV = . env/bin/activate;
PYTHON = $(PYENV) python
PYTHON_TIMED = $(PYENV) time python
MAC_LOG = ./tests/data/mac.json
MAC_OUT = tests/out/$(shell basename $(MAC_LOG)).out
CUSTOM_PKG_REPO=http://packages.livefyre.com/buildout/packages/

package: env
	$(PYTHON) setup.py bdist_egg
	$(PYTHON) setup.py sdist

test: env dev
	$(PYENV) nosetests --with-doctest $(NOSEARGS)

test_mr: tests/mr_cluster_mac_log.py mrjob.conf $(MAC_LOG) env dev
	mkdir -p tests/out
	rm -f $(MAC_OUT)
	$(PYTHON_TIMED) tests/mr_cluster_mac_log.py \
		-c mrjob.conf \
		-r local \
		$(MAC_LOG) > $(MAC_OUT)
	$(PYTHON) scripts/eval_clusters.py \
		--ground $(MAC_LOG) \
		--clusters $(MAC_OUT)

cluster: tests/cluster_mac_log.py tests/mac-a0.yaml $(MAC_LOG)
	$(PYTHON) tests/cluster_mac_log.py --config tests/mac-a0.yaml \
		$(MAC_LOG) > $(MAC_OUT)

det roc: scripts/eval_clusters.py
	$(PYTHON) scripts/eval_clusters.py \
		--curve $@ \
		--clusters $(MAC_OUT) \
		--ground $(MAC_LOG)

dev: env/bin/activate dev_requirements.txt
	$(PYENV) pip install --process-dependency-links -e . -r dev_requirements.txt --find-links $(CUSTOM_PKG_REPO)

clean:
	$(PYTHON) setup.py clean
	find . -type f -name "*.pyc" -exec rm {} \;

nuke: clean
	rm -rf *.egg *.egg-info env bin cover coverage.xml nosetests.xml

env virtualenv: env/bin/activate
env/bin/activate: requirements.txt setup.py
	test -f env/bin/activate || virtualenv --no-site-packages env
	ln -fs env/bin .
	$(PYENV) pip install --process-dependency-links -e . -r requirements.txt --find-links $(CUSTOM_PKG_REPO)
	touch env/bin/activate

upgrade:
	test -f env/bin/activate || virtualenv --no-site-packages env
	ln -fs env/bin .
	$(PYENV) pip install --process-dependency-links -e . -r requirements.txt --upgrade --find-links $(CUSTOM_PKG_REPO)
	touch env/bin/activate
