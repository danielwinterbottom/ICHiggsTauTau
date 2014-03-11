#!/bin/sh
JOBWRAPPER=./scripts/generate_job.sh
JOBSUBMIT=true
#JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"
JOBSUBMIT="./scripts/submit_ts_job.sh"
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/5Mar2014/Data_42X/

SAMPLES=(
'DoubleElectron-2011A-May10ReReco-v1'
'DoubleElectron-2011A-PromptReco-v4'
'DoubleElectron-2011A-05Aug2011-v1'
'DoubleElectron-2011A-PromptReco-v6'
'DoubleMu-2011A-May10ReReco-v1'
'DoubleMu-2011A-PromptReco-v4'
'DoubleMu-2011A-05Aug2011-v1'
'DoubleMu-2011A-PromptReco-v6'
)

for i in "${SAMPLES[@]}"
do
  echo "$i"
  $JOBWRAPPER "bin/Zbb --cfg=scripts/default.cfg --filelist=filelists/5March2014_Data_42X_"$i".dat \
  --input_prefix=$PREFIX --output_name="$i.root" --is_data=true &> jobs/"$i".log" jobs/$i.sh 
  $JOBSUBMIT jobs/$i.sh
done
