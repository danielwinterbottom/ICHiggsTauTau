#!/bin/bash

cd /vols/build/cms/akd116/newest/CMSSW_10_2_9/src/
export SCRAM_ARCH=slc6_amd64_gcc481
eval `scramv1 runtime -sh`
source $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/setup_libs.sh

ulimit -c 0

inputNumber=$(printf "%04d\n" $((SGE_TASK_ID-1)))

python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/addSVFits.py --intree $input --svfit_path $svfit_path --path $path --tag $tag --channel $channel --year $year &> jobs/${input}_${channel}_${year}_svfit.log

