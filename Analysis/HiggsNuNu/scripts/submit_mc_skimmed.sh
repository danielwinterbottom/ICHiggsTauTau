#!/bin/sh

## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}

JOBSCRIPT="./scripts/submit_cern_batch_job.sh" 
JOBQUEUE="1nh"
export JOBSUBMIT=$JOBSCRIPT" "$JOBQUEUE


echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT

CONFIG=scripts/DefaultConfigMC.cfg
PRODUCTION=Apr04

for METCUT in 130 #0 130
  do
  for CHANNEL in nunu #enu munu taunu mumu
  do
    for SYST in central #JESUP JESDOWN JERBETTER JERWORSE #NOTE TO RUN JER DOSMEAR MUST BE SET TO TRUE IN THE CONFIG
      do
      SYSTOPTIONS="--dojessyst=false --dojersyst=false"
      JOBDIRPREFIX=jobs/
      JOBDIR=$JOBDIRPREFIX/$CHANNEL/MET$METCUT/Skim/
      OUTPUTPREFIX=output/ #oldanalysisruns/220713_taunominaltightwithsysts/output/
      OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/MET$METCUT/Skim/
      
      if [ "$SYST" = "JESUP" ]
	  then
	  SYSTOPTIONS="--dojessyst=true --jesupordown=true --dojersyst=false"
	  JOBDIR=$JOBDIRPREFIX/$CHANNEL/MET$METCUT/Skim/JESUP/
	  OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/MET$METCUT/Skim/JESUP/
      fi
      
      if [ "$SYST" = "JESDOWN" ]
	  then
	  SYSTOPTIONS="--dojessyst=true --jesupordown=false --dojersyst=false"
	  JOBDIR=$JOBDIRPREFIX/$CHANNEL/MET$METCUT/Skim/JESDOWN/
	  OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/MET$METCUT/Skim/JESDOWN/
      fi

      if [ "$SYST" = "JERBETTER" ]
	  then
	  SYSTOPTIONS="--dojessyst=false --dojersyst=true --jerbetterorworse=true"
	  JOBDIR=$JOBDIRPREFIX/$CHANNEL/MET$METCUT/Skim/JERBETTER/
	  OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/MET$METCUT/Skim/JERBETTER/
      fi

      if [ "$SYST" = "JERWORSE" ]
	  then
	  SYSTOPTIONS="--dojessyst=false --dojersyst=true --jerbetterorworse=false"
	  JOBDIR=$JOBDIRPREFIX/$CHANNEL/MET$METCUT/Skim/JERWORSE/
	  OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/MET$METCUT/Skim/JERWORSE/
      fi

      echo "Config file: $CONFIG"
      
      mkdir -p $JOBDIR
      mkdir -p $OUTPUTDIR
      
      PREFIX=/afs/cern.ch/work/a/amagnan/SkimFiles/Apr04/$CHANNEL/
      for FILELIST in `ls filelists/skim/$PRODUCTION/$CHANNEL/*`
  #for FILELIST in `ls filelists/skim/$PRODUCTION/$CHANNEL/EWK-W2j*_*nu.dat`
	do
	echo "Processing files in "$FILELIST
	
	echo $FILELIST
	echo $FILELIST > tmp.txt
	sed "s/filelists\/skim\/${PRODUCTION}\/$CHANNEL\///" tmp.txt > tmp2.txt
	
	JOB=MC_`sed "s/\.dat//" tmp2.txt`
    #JOB=`sed "s/\.dat//" tmp2.txt`
	
	echo "JOB name = $JOB"

	DOWSTREAM=0
	
	grep "EWK-W2j" tmp.txt
	if (( "$?" == 0 )); then
	    let DOWSTREAM=1
	fi
	grep  "JetsToLNu" tmp.txt
	if (( "$?" == 0 )); then
	    let DOWSTREAM=1
	fi
	
	if (( "$DOWSTREAM" == "1" )); then
	    for FLAVOUR in enu munu taunu
	      do
	      WJOB=$JOB"_"$FLAVOUR
	      
	      $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$WJOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT $SYSTOPTIONS --channel=$CHANNEL --wstream=$FLAVOUR &> $JOBDIR/$WJOB.log" $JOBDIR/$WJOB.sh 0
	      #$JOBSUBMIT $JOBDIR/$WJOB.sh                                                                                      
	    done
	else  
	    $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT $SYSTOPTIONS --channel=$CHANNEL &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh 0
	    #$JOBSUBMIT $JOBDIR/$JOB.sh
	fi
	
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

