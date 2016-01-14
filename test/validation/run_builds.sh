#!/bin/sh
set -x
set -e
set -u

if (( "$#" != "7" ))
then
    echo "Wrong args!"
    exit
fi

BUILD_DIR=${1}
ENV_SCRIPT=${2}
CMSSW_VERSION=${3}
CMSSW_REPO=${4}
CMSSW_CHECKOUT=${5}
PULL_REQ=${6}
SCRIPT=${7}

MY_CMSSW_PKG="UserCode/ICHiggsTauTau"
MY_PKG_GIT="https://github.com/ajgilbert/ICHiggsTauTau.git"

echo ${PWD}
STARTDIR=${PWD}
cd ${BUILD_DIR}

set +u
set +x
source ${ENV_SCRIPT}
set -u
set -x

if [ ! -d ${CMSSW_VERSION} ]; then
  scramv1 project CMSSW ${CMSSW_VERSION}
  cd ${CMSSW_VERSION}/src
  set +x
  eval `scramv1 runtime -sh`
  set -x
  git cms-addpkg -q FWCore/Version
  cd ${BUILD_DIR}
fi

cd ${CMSSW_VERSION}/src
set +x
eval `scramv1 runtime -sh`
set -x

git fetch -q https://github.com/${CMSSW_REPO}/cmssw.git ${CMSSW_CHECKOUT} && git checkout -f -q FETCH_HEAD
git cms-sparse-checkout ${CMSSW_VERSION} HEAD
git read-tree -mu HEAD

mkdir -p ${MY_CMSSW_PKG} && cd ${MY_CMSSW_PKG}
git init
git config remote.origin.url ${MY_PKG_GIT}
git fetch ${MY_PKG_GIT} refs/pull/${PULL_REQ}/merge
git checkout -f -q FETCH_HEAD
git submodule init && git submodule update
git gc

cd ${CMSSW_BASE}/src/${MY_CMSSW_PKG}

eval ${SCRIPT}



