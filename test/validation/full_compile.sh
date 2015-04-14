#!/bin/sh
set -x
set -e
set -u

cd ${CMSSW_BASE}/src
scram build -k

cd ${CMSSW_BASE}/src/UserCode/ICHiggsTauTau/Analysis

PACKAGES=(
 'make -k all NOCOL=1'
)

for i in "${PACKAGES[@]}"
do
 eval ${i}
done
