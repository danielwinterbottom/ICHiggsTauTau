#!/bin/sh

## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}

JOBSCRIPT="./scripts/submit_ic_batch_job.sh" 
JOBQUEUE="hepshort.q"
export JOBSUBMIT=$JOBSCRIPT" "$JOBQUEUE


echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT

CONFIG=scripts/DefaultConfigMC.cfg
PRODUCTION=Apr04

for METCUT in 130
  do
  for CHANNEL in nunu enu munu taunu
    do
    for SYST in central #JESUP JESDOWN JERBETTER JERWORSE #NOTE TO RUN JER DOSMEAR MUST BE SET TO TRUE IN THE CONFIG
      do
      SYSTOPTIONS="--dojessyst=false --dojersyst=false"
      JOBDIR=jobs/$CHANNEL/MET$METCUT/
      OUTPUTPREFIX=output
      OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/MET$METCUT/
      
      if [ "$SYST" = "JESUP" ]
	  then
	  SYSTOPTIONS="--dojessyst=true --jesupordown=true --dojersyst=false"
	  JOBDIR=jobs/$CHANNEL/MET$METCUT/JESUP/
	  OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/MET$METCUT/JESUP/
      fi
      
      if [ "$SYST" = "JESDOWN" ]
	  then
	  SYSTOPTIONS="--dojessyst=true --jesupordown=false --dojersyst=false"
	  JOBDIR=jobs/$CHANNEL/MET$METCUT/JESDOWN/
	  OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/MET$METCUT/JESDOWN/
      fi

      if [ "$SYST" = "JERBETTER" ]
	  then
	  SYSTOPTIONS="--dojessyst=false --dojersyst=true --jerbetterorworse=true"
	  JOBDIR=jobs/$CHANNEL/MET$METCUT/JERBETTER/
	  OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/MET$METCUT/JERBETTER/
      fi

      if [ "$SYST" = "JERWORSE" ]
	  then
	  SYSTOPTIONS="--dojessyst=false --dojersyst=true --jerbetterorworse=false"
	  JOBDIR=jobs/$CHANNEL/MET$METCUT/JERWORSE/
	  OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/MET$METCUT/JERWORSE/
      fi

      
      echo "Config file: $CONFIG"
      
      mkdir -p $JOBDIR
      mkdir -p $OUTPUTDIR
      
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
	
#Process HiggsNuNu specific backgrounds
#Signal files and DYtoNuNu
	PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/$PRODUCTION/MC/
	for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_MC_*`
	  do
	  echo "Processing files in "$FILELIST
	  
	  echo $FILELIST
	  echo $FILELIST > tmp.txt
	  sed "s/filelists\/${PRODUCTION}\/$QUEUEDIR\/${PRODUCTION}_MC_//" tmp.txt > tmp2.txt
	  
	  JOB=MC_`sed "s/\.dat//" tmp2.txt`
	  
	  rm tmp.txt tmp2.txt
	  
	  echo "JOB name = $JOB"
	  
	  $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT $SYSTOPTIONS --channel=$CHANNEL &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
	  $JOBSUBMIT $JOBDIR/$JOB.sh
	  
	done
	
	
#Process bkg common with HiggsTautau (ONLY NEEDED IN Mar20 and Apr04)
	if [ "$PRODUCTION" = "Mar20" ]
	    then
	    SHAREDPREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/rlane/Feb20/MC_53X/
	    FILELISTPREFIX=Feb20_MC_53X_
	else
	    if [ "$PRODUCTION" = "Apr04" ]
		then
		SHAREDPREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/$PRODUCTION/MCtaushared/
		FILELISTPREFIX=Apr04_MCtaushared_
	    fi
	fi
	
	for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${FILELISTPREFIX}*`
	  do
	  echo "Processing files in "$FILELIST
	  
	  echo $FILELIST > tmp.txt
	  
	  sed "s/filelists\/${PRODUCTION}\/$QUEUEDIR\/${FILELISTPREFIX}//" tmp.txt > tmp2.txt
	  JOB=MC_`sed "s/\.dat//" tmp2.txt`
	  
	  echo "JOB name = $JOB"
	  
	  grep "JetsToLNu" tmp.txt
	  if (( "$?" == 0 )); then
	      for FLAVOUR in enu munu taunu
		do
		#if [ "$PRODUCTION" = "Mar20" ]
		#    then
		#    PREFIX=/vols/ssd00/cms/invskims/$FLAVOUR/Feb20/MC_53X/
		#else
		#    PREFIX=/vols/ssd00/cms/invskims/$FLAVOUR/$PRODUCTION/
		#fi
		
		WJOB=$JOB"_"$FLAVOUR
		
		$JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$SHAREDPREFIX --output_name=$WJOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT $SYSTOPTIONS --channel=$CHANNEL --wstream=$FLAVOUR &> $JOBDIR/$WJOB.log" $JOBDIR/$WJOB.sh
		$JOBSUBMIT $JOBDIR/$WJOB.sh
	      done
	  else 
	      $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$SHAREDPREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT $SYSTOPTIONS --channel=$CHANNEL &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
	      $JOBSUBMIT $JOBDIR/$JOB.sh
	  fi
	  
	  rm tmp.txt tmp2.txt
	  
	done
	
      done
      
    done
    
  done
  
done

#if (( "$#" != "2" ))
#then
#echo ""
#    exit
#fi

