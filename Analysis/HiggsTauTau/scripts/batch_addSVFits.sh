#!/bin/bash

dir=/vols/build/cms/dw515/test_crash/CMSSW_8_0_25/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/

cd /vols/build/cms/dw515/JER/CMSSW_10_2_18/src/
export SCRAM_ARCH=slc6_amd64_gcc481
eval `scramv1 runtime -sh`
cd $dir
source $dir/scripts/setup_libs.sh

ulimit -c 0

inputNumber=$(printf "%04d\n" $((SGE_TASK_ID-1)))

python $dir/scripts/addSVFits.py --intree $input --svfit_path $svfit_path --path $path --tag $tag --channel $channel --year $year &> jobs/${input}_${channel}_${year}_svfit.log

