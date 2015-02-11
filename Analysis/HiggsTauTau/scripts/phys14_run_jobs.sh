#!/bin/sh
set -x

PROD=Feb5

SAMPLES=(
  'MC_53X_VBF_HToTauTau_M-125'
  # 'MC_70X_VBF_HToTauTau_M-125_PU20bx25'
  # 'MC_70X_VBF_HToTauTau_M-125_PU40bx50'
  'MC_72X_PU40bx25_VBF_HToTauTau_M-125'
  # 'MC_70XMINI_VBF_HToTauTau_M-125_PU20bx25'
  # 'MC_72XMINI_VBF_HToTauTau_M-125_PU20bx25'
)

for i in "${SAMPLES[@]}"
do
./bin/Phys14 --cfg scripts/${PROD}.json --json '{ "job": {
      "filelist"    : "filelists/'${PROD}'_'${i}'.dat",
      "output_name" : "'$i'.root"},
    "do_XToTauTau" : true,
    "do_QCDFakes" : false
  }'
done


# SAMPLES=(
#   'MC_53X_QCD_Pt-15to3000'
#   'MC_72X_PU20bx25_QCD_Pt-15to3000'
# )

# JECS=(
#   'START53_V22'
#   'PHYS14_25_V2'
# )


# for (( i=0; i<${#SAMPLES[@]}; i++ ));
# do
#   ./bin/Phys14 --cfg scripts/${PROD}.json --json '{ "job": {
#       "filelist"      : "filelists/'${PROD}'_'${SAMPLES[$i]}'.dat",
#       "output_name"   : "'${SAMPLES[$i]}'_NoJEC.root"},
#       "do_XToTauTau"  : false,
#       "do_QCDFakes"   : true,
#       "apply_JEC"     : false,
#       "jec_payload"   : "'${JECS[$i]}'"
#     }'
# done

# for (( i=0; i<${#SAMPLES[@]}; i++ ));
# do
#   ./bin/Phys14 --cfg scripts/${PROD}.json --json '{ "job": {
#       "filelist"      : "filelists/'${PROD}'_'${SAMPLES[$i]}'.dat",
#       "output_name"   : "'${SAMPLES[$i]}'_WithJEC.root"},
#       "do_XToTauTau"  : false,
#       "do_QCDFakes"   : true,
#       "apply_JEC"     : true,
#       "jec_payload"   : "'${JECS[$i]}'"
#     }'
# done
