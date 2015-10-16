.PHONY: clean coverage develop env extras package release test virtualenv

PYENV := . env/bin/activate;
PYTHON := $(PYENV) python
PIP := $(PYENV) pip
EXTRAS_REQS := dev-requirements.txt $(wildcard extras-*-requirements.txt)
DISTRIBUTE := sdist bdist_wheel

package: env
	$(PYTHON) setup.py $(DISTRIBUTE)

release: env
	$(PYTHON) setup.py $(DISTRIBUTE) upload -r livefyre

# if in local dev on Mac, `make coverage` will run tests and open
# coverage report in the browser
ifeq ($(shell uname -s), Darwin)
coverage: test
	open cover/index.html
endif

test: extras
	$(PYENV) nosetests $(NOSEARGS)
	$(PYENV) py.test README.rst

extras: env/make.extras
env/make.extras: $(EXTRAS_REQS) | env
	rm -rf env/build
	$(PYENV) for req in $?; do pip install -r $$req; done
	touch $@

nuke: clean
	rm -rf *.egg *.egg-info env bin cover coverage.xml nosetests.xml

clean:
	python setup.py clean
	rm -rf dist build
	find . -path ./env -prune -o -type f -name "*.pyc" -exec rm {} \;

develop:
	@echo "Installing for " `which pip`
	pip uninstall $(PYMODULE) || true
	python setup.py develop

ifeq ($(PIP_SYSTEM_SITE_PACKAGES),1)
VENV_OPTS="--system-site-packages"
else
VENV_OPTS="--no-site-packages"
endif

env virtualenv: env/bin/activate
env/bin/activate: requirements.txt setup.py
	test -f $@ || virtualenv $(VENV_OPTS) env
	$(PIP) install -U pip wheel
	$(PIP) install cython
	$(PIP) install -r $<
	touch $@
