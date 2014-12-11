#!/bin/sh
set -x

PROD=Dec7

SAMPLES=(
  'MC_53X_VBF_HToTauTau_M-125'
  'MC_70X_VBF_HToTauTau_M-125_PU20bx25'
  'MC_70X_VBF_HToTauTau_M-125_PU40bx50'
  'MC_72X_VBF_HToTauTau_M-125_PU20bx25'
  # 'MC_70XMINI_VBF_HToTauTau_M-125_PU20bx25'
  # 'MC_72XMINI_VBF_HToTauTau_M-125_PU20bx25'
)

for i in "${SAMPLES[@]}"
do
./bin/Phys14 --cfg scripts/${PROD}.json --json '{ "job": {
    "filelist"    : "filelists/'${PROD}'_'${i}'.dat",
    "output_name" : "'$i'.root"}
  }'
done
