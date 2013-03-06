#!/bin/sh

## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}

export JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"


echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT

CONFIG=scripts/DefaultConfigMunuMC.cfg
echo "Config file: $CONFIG"

JOBDIR=jobs/munu/
mkdir -p $JOBDIR


#QCD from Patrick
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/Feb7/BKG/

for FILELIST in `ls filelists/QCD*`
  do
  echo "Processing files in "$FILELIST

  echo $FILELIST > tmp.txt
  sed "s/filelists\///" tmp.txt > tmp2.txt

  JOB=MC_`sed "s/\.dat//" tmp2.txt`

  rm tmp.txt tmp2.txt

  echo "JOB name = $JOB"

  $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --mc=summer12_53X --output_name=$JOB.root &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
  $JOBSUBMIT $JOBDIR/$JOB.sh

done


#if (( "$#" != "2" ))
#then
#echo ""
#    exit
#fi

