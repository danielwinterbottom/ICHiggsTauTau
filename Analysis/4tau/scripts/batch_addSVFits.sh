#!/bin/bash

#dir=""

cd $dir
export SCRAM_ARCH=slc6_amd64_gcc481
eval `scramv1 runtime -sh`
source $dir/scripts/setup_libs.sh

ulimit -c 0

inputNumber=$(printf "%04d\n" $((SGE_TASK_ID-1)))

python $dir/scripts/addSVFits.py --intree $input --svfit_path $svfit_path --path $path --tag $tag --channel $channel --year $year &> jobs/${input}_${channel}_${year}_${extra}_svfit.log

