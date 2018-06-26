#!/bin/bash

cd $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau
export SCRAM_ARCH=slc6_amd64_gcc481
eval `scramv1 runtime -sh`
source $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/setup_libs.sh

ulimit -c 0

var_=${var//\'}

python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --no_plot --cfg=$CFG --channel=$ch --method=$cat_num --cat=$cat_str --year=$YEAR --outputfolder=$output_folder --datacard=$dc --paramfile=$PARAMS --folder=$FOLDER $BLIND --var=$var_ $extra
