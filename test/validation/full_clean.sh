#!/bin/sh
set -x
set -e
set -u

cd ${CMSSW_BASE}/src
scram build clean

cd ${CMSSW_BASE}/src/UserCode/ICHiggsTauTau/Analysis

PACKAGES=(
 'make clean_all'
)

for i in "${PACKAGES[@]}"
do
 eval ${i}
done
