#!/bin/sh
DOCERN=0
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

CONFIG=scripts/DefaultRun2Config.cfg
QUEUEDIR=short #medium #medium long

JOBDIRPREFIX=jobs_run2ana_160216 #_sig #_nomindphi
JOBDIR=$JOBDIRPREFIX/
OUTPUTPREFIX=output_run2ana_160216 #_sig #_nomindphi
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
#for syst in JESUP JESDOWN JERBETTER JERWORSE UESUP UESDOWN ELEEFFUP ELEEFFDOWN MUEFFUP MUEFFDOWN PUUP PUDOWN
#for syst in "" PUUP PUDOWN
for syst in "" PUUP PUDOWN TRIG0UP TRIG0DOWN TRIG1UP TRIG1DOWN TRIG2UP TRIG2DOWN JESUP JESDOWN JERBETTER JERWORSE UESUP UESDOWN
#for syst in JERBETTER JERWORSE
do
  mkdir -p $JOBDIR$syst
  mkdir -p $OUTPUTDIR$syst
  for channels in nunu qcd enu munu taunu mumu #nunu topl topb #top gamma #qcd
    do
    JOB=$channels
    OUTPUTNAME="$channels.root"
    MINDPHICUT="alljetsmetnomu_mindphi\>=1.0"
    if [ "$channels" = "qcd" ]; then
	MINDPHICUT="alljetsmetnomu_mindphi\<0.5"
	#MINDPHICUT="alljetsmetnomu_mindphi\>=0"
    fi
    if [ "$syst" = "" ]
	then
	$JOBWRAPPER "./bin/LTAnalysisRun2 --cfg=$CONFIG --channel=$channels -o $OUTPUTDIR$syst/$OUTPUTNAME --jetmetdphicut=$MINDPHICUT | tee $JOBDIR$syst/$JOB.log" $JOBDIR$syst/$JOB.sh $GRIDSETUP
    else
	#if [ "$channels" = "nunu" ]
	   # then
	    #for other systs only run nunu
	    $JOBWRAPPER "./bin/LTAnalysisRun2 --cfg=$CONFIG --channel=$channels --syst=$syst -o $OUTPUTDIR$syst/$OUTPUTNAME --jetmetdphicut=$MINDPHICUT | tee $JOBDIR$syst/$JOB.log" $JOBDIR$syst/$JOB.sh $GRIDSETUP
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