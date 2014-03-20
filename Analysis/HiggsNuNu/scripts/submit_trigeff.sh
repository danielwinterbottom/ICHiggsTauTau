#!/bin/sh

## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}

#export JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"

JOBSCRIPT="./scripts/submit_ic_batch_job.sh" 
JOBQUEUE="hepshort.q"
export JOBSUBMIT=$JOBSCRIPT" "$JOBQUEUE


echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT

CONFIG=scripts/DefaultConfig.cfg
PRODUCTION=Dec18

for CHANNEL in nunu #nunulowmet nunulowmetiglep #nunu nunulowmet nunulowmetiglep mumu nunuiglep #nunu enu munu taunu #mumu nunuiglep
    do
    for SYST in central #JESUP JESDOWN JERBETTER JERWORSE #NOTE SYSTEMATIC RUNS WILL BE SAME AS CENTRAL BUT OUTPUT WILL GO TO SYSTEMATIC SUBDIRECTORIES
      do
      SYSTOPTIONS="--dojessyst=false --dojersyst=false" 
      JOBDIRPREFIX=jobs_trigeff
      JOBDIR=$JOBDIRPREFIX/$CHANNEL/
      OUTPUTPREFIX=output_trigeff_alltrigs
      OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/
      
      if [ "$SYST" = "JESUP" ]
	  then
	  SYSTOPTIONS="--dojessyst=true --jesupordown=true"
	  JOBDIR=$JOBDIRPREFIX/$CHANNEL/JESUP/
	  OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/JESUP/
      fi
      
      if [ "$SYST" = "JESDOWN" ]
	  then
	  SYSTOPTIONS="--dojessyst=true --jesupordown=false"
	  JOBDIR=$JOBDIRPREFIX/$CHANNEL/JESDOWN/
	  OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/JESDOWN/
      fi  

      if [ "$SYST" = "JERBETTER" ]
	  then
	  SYSTOPTIONS="--dojessyst=false --dojersyst=true --jerbetterorworse=true"
	  JOBDIR=$JOBDIRPREFIX/$CHANNEL/JERBETTER/
	  OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/JERBETTER/
      fi  

      if [ "$SYST" = "JERWORSE" ]
	  then
	  SYSTOPTIONS="--dojessyst=false --dojersyst=true --jerbetterorworse=false"
	  JOBDIR=$JOBDIRPREFIX/$CHANNEL/JERWORSE/
	  OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/JERWORSE/
      fi  

      TRIGEFFOPTIONS=" --jet1ptcut=0 --jet2ptcut=0 --detajjcut=0 --jetptprecut=0 --dphi-cut=0 --met_cut=0 mjj_cut=0 "

      echo "Config file: $CONFIG"
      mkdir -p $JOBDIR
      mkdir -p $OUTPUTDIR
      
      for QUEUEDIR in singlemurun #medium #long
	do
	
	if [ "$QUEUEDIR" = "medium" ]
	    then
	    JOBQUEUE="hepmedium.q"
	    export JOBSUBMIT=$JOBSCRIPT" "$JOBQUEUE
	    echo "Using job-submission: " $JOBSUBMIT
	else
	    JOBQUEUE="hepmedium.q"
	    export JOBSUBMIT=$JOBSCRIPT" "$JOBQUEUE
	    echo "Using job-submission: " $JOBSUBMIT
	fi
	
	PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/$PRODUCTION/SingleMu/
	
	for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_SingleMu_*`
	  do
	  echo "Processing files in "$FILELIST

	  echo $FILELIST > tmp.txt
	  sed "s/filelists\/${PRODUCTION}\/$QUEUEDIR\/${PRODUCTION}_SingleMu_//" tmp.txt > tmp2.txt
	  
	  JOB=SingleMu_`sed "s/\.dat//" tmp2.txt`
	  
	  echo "JOB name = $JOB"
	  
	  $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR  $SYSTOPTIONS $TRIGEFFOPTIONS --channel=$CHANNEL --do_trigeff_tree=true &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
	  $JOBSUBMIT $JOBDIR/$JOB.sh
	    
	  
	  rm tmp.txt tmp2.txt

	done
	
      done
      
    done
      
done

#if (( "$#" != "2" ))
#then
#echo ""
#    exit
#fi

