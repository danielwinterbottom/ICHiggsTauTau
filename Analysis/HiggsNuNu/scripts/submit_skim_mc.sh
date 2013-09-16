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

for METCUT in 130 #0 130
  do
  for CHANNEL in nunu #enu munu taunu mumu
    do
    
      JOBDIRPREFIX=jobs
      JOBDIR=$JOBDIRPREFIX/skim/$CHANNEL/MET$METCUT/
      OUTPUTPREFIX=output/ #oldanalysisruns/220713_taunominaltightwithsysts/output/
      OUTPUTDIR=$OUTPUTPREFIX/skim/$CHANNEL/MET$METCUT/
      SKIMPREFIX=/vols/ssd00/cms/amagnan/trigskims/
      SKIMDIR=$SKIMPREFIX/$CHANNEL/MET$METCUT/
 
      echo "Config file: $CONFIG"
      
      mkdir -p $JOBDIR
      mkdir -p $OUTPUTDIR
      mkdir -p $SKIMDIR
 
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

	  FILESTR=`sed "s/\.dat//" tmp2.txt`
	  mkdir -p $SKIMDIR/$FILESTR/

	  JOB=MC_`sed "s/\.dat//" tmp2.txt`

	  echo "JOB name = $JOB"

	  grep "EWK-W2j" tmp.txt
	  if (( "$?" == 0 )); then
	      for FLAVOUR in enu munu taunu
		do
		WJOB=$JOB"_"$FLAVOUR

		mkdir -p $SKIMDIR/$FILESTR/$FLAVOUR/

		$JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$WJOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT --channel=$CHANNEL --wstream=$FLAVOUR --do_skim=1 --skim_path=$SKIMDIR/$FILESTR/$FLAVOUR/ &> $JOBDIR/$WJOB.log" $JOBDIR/$WJOB.sh
                $JOBSUBMIT $JOBDIR/$WJOB.sh                                                                                      
	      done
	  else  
	      $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT --channel=$CHANNEL --do_skim=1 --skim_path=$SKIMDIR/$FILESTR/ &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
	      $JOBSUBMIT $JOBDIR/$JOB.sh
	  fi
	  rm tmp.txt tmp2.txt

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
	
	  FILESTR=`sed "s/\.dat//" tmp2.txt`
	  mkdir -p $SKIMDIR/$FILESTR/
	  
	  JOB=MC_`sed "s/\.dat//" tmp2.txt`
	  
	  echo "JOB name = $JOB"
	  
	  
	  grep  "JetsToLNu" tmp.txt
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
		
		mkdir -p $SKIMDIR/$FILESTR/$FLAVOUR/

		$JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$SHAREDPREFIX --output_name=$WJOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT --channel=$CHANNEL --wstream=$FLAVOUR --do_skim=1 --skim_path=$SKIMDIR/$FILESTR/$FLAVOUR/ &> $JOBDIR/$WJOB.log" $JOBDIR/$WJOB.sh
		$JOBSUBMIT $JOBDIR/$WJOB.sh
	      done
	  else  
	      $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$SHAREDPREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT --channel=$CHANNEL --do_skim=1 --skim_path=$SKIMDIR/$FILESTR/ &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
	      $JOBSUBMIT $JOBDIR/$JOB.sh
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

