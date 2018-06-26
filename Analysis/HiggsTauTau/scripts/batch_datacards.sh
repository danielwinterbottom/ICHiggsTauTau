#!/bin/bash

cd /vols/build/cms/akd116/MLStudies/latest/CMSSW_8_0_25/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau
export SCRAM_ARCH=slc6_amd64_gcc481
eval `scramv1 runtime -sh`
source /vols/build/cms/akd116/MLStudies/latest/CMSSW_8_0_25/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/setup_libs.sh

ulimit -c 0

var_=${var//\'}

python /vols/build/cms/akd116/MLStudies/latest/CMSSW_8_0_25/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --no_plot --cfg=$CFG --channel=$ch --method=$cat_num --cat=$cat_str --year=$YEAR --outputfolder=$output_folder --datacard=$dc --paramfile=$PARAMS --folder=$FOLDER $BLIND --var=$var_ $extra
