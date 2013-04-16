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

for METCUT in 0 70
  do
  for CHANNEL in nunu enu munu
    do
    for SYST in central #JESUP JESDOWN
      do
      SYSTOPTIONS="--dojessyst=false"
      JOBDIR=jobs/$CHANNEL/MET$METCUT/
      OUTPUTDIR=output/$CHANNEL/MET$METCUT/
      
      if [ "$SYST" = "JESUP" ]
	  then
	  SYSTOPTIONS="--dojessyst=true --upordown=true"
	  JOBDIR=jobs/$CHANNEL/MET$METCUT/JESUP/
	  OUTPUTDIR=output/$CHANNEL/MET$METCUT/JESUP/
      fi
      
      if [ "$SYST" = "JESDOWN" ]
	  then
	  SYSTOPTIONS="--dojessyst=true --upordown=false"
	  JOBDIR=jobs/$CHANNEL/MET$METCUT/JESDOWN/
	  OUTPUTDIR=output/$CHANNEL/MET$METCUT/JESDOWN/
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
	PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/Mar20/MC/
	for FILELIST in `ls filelists/Mar20/$QUEUEDIR/Mar20_MC_*`
	  do
	  echo "Processing files in "$FILELIST
	  
	  echo $FILELIST
	  echo $FILELIST > tmp.txt
	  sed "s/filelists\/Mar20\/$QUEUEDIR\/Mar20_MC_//" tmp.txt > tmp2.txt
	  
	  JOB=MC_`sed "s/\.dat//" tmp2.txt`
	  
	  rm tmp.txt tmp2.txt
	  
	  echo "JOB name = $JOB"
	  
	  $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT $SYSTOPTIONS --channel=$CHANNEL &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
	  $JOBSUBMIT $JOBDIR/$JOB.sh
	  
	done
	
	
#Process bkg common with HiggsTautau
	PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/rlane/Feb20/MC_53X/
	for FILELIST in `ls filelists/Mar20/$QUEUEDIR/Feb20_*`
	  do
	  echo "Processing files in "$FILELIST
	  
	  echo $FILELIST > tmp.txt
	  
	  sed "s/filelists\/Mar20\/$QUEUEDIR\/Feb20_MC_53X_//" tmp.txt > tmp2.txt
	  JOB=MC_`sed "s/\.dat//" tmp2.txt`
	  
	  echo "JOB name = $JOB"
	  
	  grep "JetsToLNu" tmp.txt
	  if (( "$?" == 0 )); then
	      for FLAVOUR in enu munu taunu
		do
		PREFIX=/vols/ssd00/cms/invskims/$FLAVOUR/Feb20/MC_53X/
		
		WJOB=$JOB"_"$FLAVOUR
		
		$JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$WJOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT $SYSTOPTIONS --channel=$CHANNEL &> $JOBDIR/$WJOB.log" $JOBDIR/$WJOB.sh
		$JOBSUBMIT $JOBDIR/$WJOB.sh
	      done
	  else 
	      PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/rlane/Feb20/MC_53X/
	      $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT $SYSTOPTIONS --channel=$CHANNEL &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
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

