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
QUEUEDIR=short #medium #medium long

JOBDIRPREFIX=jobs_2015Dtrigeff_131115json_sigtrig_binned_2erfs_241115
JOBDIR=$JOBDIRPREFIX/
OUTPUTPREFIX=output_2015Dtrigeff_131115json_sigtrig_binnedfrom80_2erfs_241115
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
for syst in "" #JESUP JESDOWN JERBETTER JERWORSE UESUP UESDOWN ELEEFFUP ELEEFFDOWN MUEFFUP MUEFFDOWN PUUP PUDOWN
  do
  mkdir -p $JOBDIR$syst
  mkdir -p $OUTPUTDIR$syst
  for channels in nunu #munu enu taunu mumu top gamma #qcd
    do
    JOB=$channels
    OUTPUTNAME="$channels.root"
    if [ "$syst" = "" ]
	then
	$JOBWRAPPER "./bin/LTAnalysis --cfg=$CONFIG --channel=$channels -o $OUTPUTDIR$syst/$OUTPUTNAME &> $JOBDIR$syst/$JOB.log" $JOBDIR$syst/$JOB.sh
    else
	if [ "$channels" = "nunu" ]
	    then
	    #for other systs only run nunu
	    $JOBWRAPPER "./bin/LTAnalysis --cfg=$CONFIG --channel=$channels --syst=$syst -o $OUTPUTDIR$syst/$OUTPUTNAME &> $JOBDIR$syst/$JOB.log" $JOBDIR$syst/$JOB.sh
	fi
    fi
    if [ "$syst" = "" ]
	then
	$JOBSUBMIT $JOBDIR$syst/$JOB.sh
    else
	if [ "$channels" = "nunu" ]
	    then
	    $JOBSUBMIT $JOBDIR$syst/$JOB.sh
	fi
    fi
  done
done