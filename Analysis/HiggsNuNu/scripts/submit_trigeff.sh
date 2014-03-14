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
      JOBDIRPREFIX=trigeffjobs
      JOBDIR=$JOBDIRPREFIX/$CHANNEL/
      OUTPUTPREFIX=trigeffoutput
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
	
	PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/$PRODUCTION/SingleMu/
	
	for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_SingleMu_*`
	  do
	  echo "Processing files in "$FILELIST
	  
	  echo $FILELIST > tmp.txt
	  sed "s/filelists\/${PRODUCTION}\/$QUEUEDIR\/${PRODUCTION}_SingleMu_//" tmp.txt > tmp2.txt
	  
	  #!!PUT FOR LOOP ON EACH CUT
	  nmets=14
	  mets[1]=40
	  mets[2]=60
	  mets[3]=80
	  mets[4]=100
	  mets[5]=110
	  mets[6]=120
	  mets[7]=130
	  mets[8]=140
	  mets[9]=150
	  mets[10]=160
	  mets[11]=170
	  mets[12]=180
	  mets[13]=190
	  mets[14]=200
	  for imet in $(seq 1 $nmets)
	    do
	    JOB=SingleMu_`sed "s/\.dat//" tmp2.txt`_MET${mets[$imet]}
	    
	    echo "JOB name = $JOB"

	    if [ "$imet" = "$nmets" ]
		then
		trigeffoptions=" --met_cut=${mets[$imet]} "		
	    else
		metcutmax=${mets[$(($imet+1))]}
		trigeffoptions=" --met_cut=${mets[$imet]} --met_cut_max=$metcutmax "		
	    fi



	    $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR  $SYSTOPTIONS --channel=$CHANNEL --do_trigeff=true $trigeffoptions &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
	    $JOBSUBMIT $JOBDIR/$JOB.sh
	    
	  done
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

