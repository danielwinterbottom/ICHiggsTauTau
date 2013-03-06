#!/bin/sh

## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}

export JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"


echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT

CONFIG=scripts/DefaultConfigEnuMC.cfg
echo "Config file: $CONFIG"

JOBDIR=jobs/enu/
mkdir -p $JOBDIR


#Process bkg common with HiggsTautau
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/Dec2/MC_53X/
for FILELIST in `ls filelists/Dec2_MC_53X_W*ToLNu*`
#for FILELIST in `ls filelists/Dec30_MC_53X_DY*`
  do
  echo "Processing files in "$FILELIST

  echo $FILELIST > tmp.txt
  sed "s/filelists\/Dec2_MC_53X_//" tmp.txt > tmp2.txt

  JOB=MC_`sed "s/\.dat//" tmp2.txt`

  rm tmp.txt tmp2.txt

  echo "JOB name = $JOB"

  $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
  $JOBSUBMIT $JOBDIR/$JOB.sh

done



#if (( "$#" != "2" ))
#then
#echo ""
#    exit
#fi

