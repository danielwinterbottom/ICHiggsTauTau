#!/bin/sh
DOCERN=1
DOSUBMIT=1

## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}

#export JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"

GRIDSETUP=1
if [ "$DOCERN" = "0" ]
    then
    JOBSCRIPT="./scripts/submit_ic_batch_job.sh"
else
    JOBSCRIPT="./scripts/submit_cern_batch_job.sh"
    GRIDSETUP=0
fi
export JOBSUBMIT=$JOBSCRIPT" "$JOBQUEUE


echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT

CONFIG=scripts/DefaultRun1Config.cfg
QUEUEDIR=short #medium #medium long

JOBDIRPREFIX=jobs_parkedana
JOBDIR=$JOBDIRPREFIX/
OUTPUTPREFIX=output_parkedana
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
	JOBQUEUE="1nh"
    fi
    
fi
export JOBSUBMIT=$JOBSCRIPT" "$JOBQUEUE
echo "Using job-submission: " $JOBSUBMIT


echo "JOB name = $JOB"
for syst in JESUP JESDOWN JERBETTER JERWORSE UESUP UESDOWN ELEEFFUP ELEEFFDOWN MUEFFUP MUEFFDOWN PUUP PUDOWN
  do
  mkdir -p $JOBDIR$syst
  mkdir -p $OUTPUTDIR$syst
  for channels in nunu #enu munu taunu mumu #top gamma #qcd
    do
    JOB=$channels
    OUTPUTNAME="$channels.root"
    if [ "$syst" = "" ]
	then
	$JOBWRAPPER "./bin/LTAnalysis --cfg=$CONFIG --channel=$channels -o $OUTPUTDIR$syst/$OUTPUTNAME | tee $JOBDIR$syst/$JOB.log" $JOBDIR$syst/$JOB.sh $GRIDSETUP
    else
	#if [ "$channels" = "nunu" ]
	   # then
	    #for other systs only run nunu
	    $JOBWRAPPER "./bin/LTAnalysis --cfg=$CONFIG --channel=$channels --syst=$syst -o $OUTPUTDIR$syst/$OUTPUTNAME | tee $JOBDIR$syst/$JOB.log" $JOBDIR$syst/$JOB.sh $GRIDSETUP
	#fi
    fi
    if [ "$syst" = "" ]
	then
	if [ "$DOSUBMIT" = "1" ]; then 
	    $JOBSUBMIT $JOBDIR$syst/$JOB.sh
	else 
	    echo "$JOBSUBMIT $JOBDIR$syst/$JOB.sh"
	fi
    else
	#if [ "$channels" = "nunu" ]
	    #then
	    if [ "$DOSUBMIT" = "1" ]; then 
		$JOBSUBMIT $JOBDIR$syst/$JOB.sh
	    else
		echo "$JOBSUBMIT $JOBDIR$syst/$JOB.sh"
	    fi
	#fi
    fi
  done
done