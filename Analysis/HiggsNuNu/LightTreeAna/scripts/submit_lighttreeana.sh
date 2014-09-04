#!/bin/sh
DOCERN=0

## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}

#export JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"

if [ "$DOCERN" = "0" ]
    then
    JOBSCRIPT="./scripts/submit_ic_batch_job.sh"
else
    JOBSCRIPT="./scripts/submit_cern_batch_job.sh"
fi
export JOBSUBMIT=$JOBSCRIPT" "$JOBQUEUE


echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT

CONFIG=scripts/DefaultConfig.cfg
QUEUEDIR=medium #medium long

JOBDIRPREFIX=jobs
JOBDIR=$JOBDIRPREFIX/
OUTPUTPREFIX=output
OUTPUTDIR=$OUTPUTPREFIX/

OUTPUTNAME="output.root"
  
echo "Config file: $CONFIG"
mkdir -p $JOBDIR
mkdir -p $OUTPUTDIR

if [ "$DOCERN" = "0" ]
    then
    if [ "$QUEUEDIR" = "medium" ]
	then
	JOBQUEUE="hepmedium.q"
    elif [ "$QUEUEDIR" = "long" ]
	then
	JOBQUEUE="heplong.q"
    else
	JOBQUEUE="hepshort.q"
    fi
else
    if [ "$QUEUEDIR" = "medium" ]
	then
	JOBQUEUE="1nd"
    elif [ "$QUEUEDIR" = "long" ]
	then
	JOBQUEUE="2nd"
    else
	JOBQUEUE="8nh"
    fi
    
fi
export JOBSUBMIT=$JOBSCRIPT" "$JOBQUEUE
echo "Using job-submission: " $JOBSUBMIT


echo "JOB name = $JOB"
for channels in nunu munu enu taunu mumu
  do
  JOB=$channels
  OUTPUTNAME="$channels.root"
  $JOBWRAPPER "./bin/LTAnalysis --cfg=$CONFIG --channel=$channels -o $OUTPUTDIR$OUTPUTNAME &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
  $JOBSUBMIT $JOBDIR/$JOB.sh
done




