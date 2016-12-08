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

JOBDIRPREFIX=jobs_run2ana_161208_ICHEP
JOBDIR=$JOBDIRPREFIX/
OUTPUTPREFIX=output_run2ana_161208_ICHEP
OUTPUTDIR=$OUTPUTPREFIX/

OUTPUTNAME="output.root"

echo "Config file: $CONFIG"
mkdir -p $JOBDIR
mkdir -p $OUTPUTDIR

if [ "$DOCERN" = "0" ]
    then
    if [ "$QUEUEDIR" = "medium" ]
	then
	JOBQUEUE="5:59:0"
    elif [ "$QUEUEDIR" = "long" ]
	then
	JOBQUEUE="47:59:0"
    else
	JOBQUEUE="2:59:0"
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
for syst in "" #JESUP JESDOWN JERBETTER JERWORSE ELEEFFUP ELEEFFDOWN MUEFFUP MUEFFDOWN PUUP PUDOWN TRIG0UP TRIG0DOWN TRIG1UP TRIG1DOWN TRIG2UP TRIG2DOWN UESUP UESDOWN
#for syst in JESUP JESDOWN JERBETTER JERWORSE ELEEFFUP ELEEFFDOWN MUEFFUP MUEFFDOWN PUUP PUDOWN TRIG0UP TRIG0DOWN TRIG1UP TRIG1DOWN TRIG2UP TRIG2DOWN UESUP UESDOWN
do
  mkdir -p $JOBDIR$syst
  mkdir -p $OUTPUTDIR$syst
  for channels in qcd enu munu taunu mumu ee nunu #qcd #topl topb top gamma
    do
    JOB=$channels
    #executable expect strings separated by "!"
    #HISTSTRING=`awk '{FS="\t"}{ORS="!"}{print $2}' scripts/${channels}_nomindphi.hists`
    #SHAPESTRING=`awk '{ORS="!"}{print $1}' scripts/${channels}_nomindphi.hists`
    ## To produce all of the hist
    HISTSTRING=`awk '{FS="\t"}{ORS="!"}{print $2}' scripts/${channels}.hists`
    SHAPESTRING=`awk '{ORS="!"}{print $1}' scripts/${channels}.hists`
    ## To produce all of the hist for datacard
    #HISTSTRING=`awk '{FS="\t"}{ORS="!"}{print $2}' scripts/${channels}_datacard.hists`
    #SHAPESTRING=`awk '{ORS="!"}{print $1}' scripts/${channels}_datacard.hists`
    #HISTSTRING=`awk '{FS="\t"}{ORS="!"}{print $2}' scripts/${channels}_sig.hists`
    #SHAPESTRING=`awk '{ORS="!"}{print $1}' scripts/${channels}_sig.hists`
    ## To test for one hist
    #HISTSTRING=";E_{T,no-#mu}^{miss} (GeV);Events"
    #SHAPESTRING="metnomuons(25,200.,600.)"
    #HISTSTRING=";E_{T,no-#mu}^{miss} (GeV);Events!;Forward tag jet #eta;Events"
    #SHAPESTRING="metnomuons(25,200.,600.)!forward_tag_eta(25,-5.,5.)"
    #HISTSTRING=";#Delta#phi(E_{T,no-#mu}^{miss},j);Events"
    #SHAPESTRING="alljetsmetnomu_mindphi(14,2.3,3.1416)"
    echo "Making histograms: " $SHAPESTRING
    OUTPUTNAME="$channels.root"
    MINDPHICUT="alljetsmetnomu_mindphi\>=2.3"
    #MINDPHICUT="alljetsmetnomu_mindphi\>=1."
    if [ "$channels" = "taunu" ]; then
	MINDPHICUT="jetmetnomu_mindphi\>=1.0\&\&alljetsmetnomu_mindphi\<2.3"
    fi
    if [ "$channels" = "qcd" ]; then
	MINDPHICUT="alljetsmetnomu_mindphi\<0.5"
	#MINDPHICUT="alljetsmetnomu_mindphi\>=0"
    fi
    if [ "$channels" = "ee" ]; then
	MINDPHICUT="alljetsmetnoel_mindphi\>2.3"
    fi
    if [ "$channels" = "enu" ]; then
	MINDPHICUT="alljetsmetnoel_mindphi\>2.3"
    fi
    if [ "$syst" = "" ]
	then
	$JOBWRAPPER "./bin/LTAnalysisRun2_2016 --cfg=$CONFIG --channel=$channels --histTitlePar='$HISTSTRING' --shapePar='$SHAPESTRING' -o $OUTPUTDIR$syst/$OUTPUTNAME --jetmetdphicut=$MINDPHICUT &> $JOBDIR$syst/$JOB.log" $JOBDIR$syst/$JOB.sh $GRIDSETUP
    else
	#if [ "$channels" = "nunu" ]
	   # then
	    #for other systs only run nunu
    #executable expect strings separated by "!"
	#HISTSTRING="Forward tag jet #eta;Events"
	#SHAPESTRING="forward_tag_eta(25,-5.,5.)"
	#echo "Making histograms: " $SHAPESTRING

	$JOBWRAPPER "./bin/LTAnalysisRun2_2016 --cfg=$CONFIG --channel=$channels --histTitlePar='$HISTSTRING' --shapePar='$SHAPESTRING' --syst=$syst -o $OUTPUTDIR$syst/$OUTPUTNAME --jetmetdphicut=$MINDPHICUT &> $JOBDIR$syst/$JOB.log" $JOBDIR$syst/$JOB.sh $GRIDSETUP
	#fi
    fi
    if [ "$DOSUBMIT" = "1" ]; then 
	$JOBSUBMIT $JOBDIR$syst/$JOB.sh
    else 
	echo "$JOBSUBMIT $JOBDIR$syst/$JOB.sh"
    fi
  done
done
