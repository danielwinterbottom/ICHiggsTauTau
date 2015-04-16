#!/bin/sh
set -x
set -e
set -u

cd ${CMSSW_BASE}/src
scram build -k
