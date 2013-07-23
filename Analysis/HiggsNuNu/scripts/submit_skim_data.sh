#!/bin/sh

## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}

JOBSCRIPT="./scripts/submit_ic_batch_job.sh"

echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-script: " $JOBSCRIPT

CONFIG=scripts/DefaultConfigSkimData.cfg
echo "Config file: $CONFIG"
PRODUCTION=Apr04

JOBDIR=jobs/skim/$CHANNEL/
OUTDIR=/vols/ssd00/cms/invskims//$PRODUCTION/
mkdir -p $JOBDIR
mkdir -p $OUTDIR

for QUEUEDIR in short medium
  do
  
  if [ "$QUEUEDIR" = "medium" ]
      then
      JOBQUEUE="hepmedium.q"
      export JOBSUBMIT=$JOBSCRIPT" "$JOBQUEUE
      echo "Using job-submission: " $JOBSUBMIT
        else
      JOBQUEUE="hepshort.q"
      export JOBSUBMIT=$JOBSCRIPT" "$JOBQUEUE
      echo "Using job-submission: " $JOBSUBMIT
  fi
  
  PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/$PRODUCTION/MET/
  
  for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_MET_*`
    do
    echo "Processing files in "$FILELIST
    
    echo $FILELIST > tmp.txt
    sed "s/filelists\/${PRODUCTION}\/$QUEUEDIR\/${PRODUCTION}_MET_//" tmp.txt > tmp2.txt
    LOOPOUTDIR=$OUTDIR/`sed "s/\.dat//" tmp2.txt`/
    JOB=Data_`sed "s/\.dat//" tmp2.txt`

    mkdir -p $LOOPOUTDIR

    rm tmp.txt tmp2.txt
    
    echo "JOB name = $JOB"
    
    $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --skim_path=$LOOPOUTDIR --channel=nunu &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
    $JOBSUBMIT $JOBDIR/$JOB.sh
    
  done
  
done


#if (( "$#" != "2" ))
#then
#echo ""
#    exit
#fi

