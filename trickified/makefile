ifeq (, ${TRICK_HOME})
export TRICK_HOME := $(shell trick-config --prefix)
endif

ifeq (, ${TRICK_HOME})
    $(error TRICK_HOME must be set)
endif

ifeq (, ${GUNNS_HOME})
export GUNNS_HOME := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)
endif

export TRICKIFY_ORIG_DIR = $(GUNNS_HOME)
export TRICKIFY_PYTHON_DIR := gunns_python.zip
export TRICKIFY_OBJECT_NAME := libgunns
export TRICKIFY_CXX_FLAGS := \
    -I${GUNNS_HOME}/include/gunns \
    -I${GUNNS_HOME}/include/gunns/ms-utils \
    -I${GUNNS_HOME}/include/gunns/gunns-ts-models

GIT_VERSION := $(shell git describe --tags --long | sed -E 's/-g[0-9a-f]+$$//')
VERSION_RELEASE := $(patsubst jeod_v%,%,$(GIT_VERSION))
VERSION   := $(word 1,$(subst -, ,$(VERSION_RELEASE)))
ITERATION := $(word 2,$(subst -, ,$(VERSION_RELEASE)))
OS_VERSION_MAJOR := $(shell . /etc/os-release && printf '%s' "$${VERSION_ID%%.*}")
RPM_OS_TAG ?= RHEL$(OS_VERSION_MAJOR)

INSTALL_PREFIX ?= .
INSTALL_DIR = $(INSTALL_PREFIX)/gunns_${GIT_VERSION}

all: trickify

trickify: ${GUNNS_HOME}/include/gunns
	@$(MAKE) -s -f $(TRICK_HOME)/share/trick/makefiles/trickify.mk

${GUNNS_HOME}/include/gunns:
	mkdir -p $@
	rsync -av --prune-empty-dirs --exclude='verif' ../aspects ../core ../gunns-ts-models ../ms-utils ../sims $@/

rpm: INSTALL_PREFIX := package-root/opt/er7_packages
rpm: install
	fpm -s dir -t rpm -n gunns -v $(VERSION) --iteration $(ITERATION) -C package-root --package "gunns_${GIT_VERSION}.${RPM_OS_TAG}.$(shell uname -m).rpm" opt/er7_packages/gunns_${GIT_VERSION}

install:
	mkdir -p ${INSTALL_DIR}/include/gunns
	mkdir -p ${INSTALL_DIR}/lib/gunns/python
	mkdir -p ${INSTALL_DIR}/share/gunns
	rsync -av --prune-empty-dirs --exclude='verif' --include='*/' --include='*.h' --include='*.hh' --exclude='*' --chmod=Fugo-x ../aspects ../core ../gunns-ts-models ../ms-utils ../sims ${INSTALL_DIR}/include/gunns
	rsync -av ../lib/trick_if/S_gunns_trickified.mk ${INSTALL_DIR}/share/gunns/
	rsync -av ../draw ${INSTALL_DIR}/
	rsync -av ../sims ${INSTALL_DIR}/
	rsync -av ../trickified/libgunns.* ${INSTALL_DIR}/lib64/
	rsync -av ../trickified/${TRICKIFY_PYTHON_DIR} ${INSTALL_DIR}/lib/gunns/python
	rsync -av ../trickified/build --include='*/' --include='*.i' --exclude='*'  ${INSTALL_DIR}/
	rsync -av ../trickified/build_dir.txt ${INSTALL_DIR}/

clean:
	@rm -rf build ${TRICKIFY_PYTHON_DIR} build_dir.txt ${GUNNS_HOME}/include libgunns.* gunns*.rpm package-root .trick

