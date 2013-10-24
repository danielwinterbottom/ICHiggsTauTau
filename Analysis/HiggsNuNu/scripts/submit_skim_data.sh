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
PRODUCTION=Oct21

for METCUT in 130
  do
  for CHANNEL in nunu
    do
    JOBDIRPREFIX=jobs
    JOBDIR=$JOBDIRPREFIX/skim/$CHANNEL/MET$METCUT/
    OUTPUTPREFIX=output
    OUTPUTDIR=$OUTPUTPREFIX/skim/$CHANNEL/MET$METCUT/
    SKIMPREFIX=/vols/ssd00/cms/amagnan/trigskims/$PRODUCTION/
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
      
      #PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/$PRODUCTION/MET/
      PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/amagnan/$PRODUCTION/METembedded/
      
      for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_METembedded_*`
	do
	echo "Processing files in "$FILELIST
	
	echo $FILELIST > tmp.txt
	sed "s/filelists\/${PRODUCTION}\/$QUEUEDIR\/${PRODUCTION}_METembedded_MET/METembedded/" tmp.txt > tmp2.txt
	
	FILESTR=`sed "s/\.dat//" tmp2.txt`
	mkdir -p $SKIMDIR/$FILESTR/

	JOB=DataEmbedded_`sed "s/\.dat//" tmp2.txt`
	
	rm tmp.txt tmp2.txt
	
	echo "JOB name = $JOB"
	
	$JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT --channel=$CHANNEL --do_skim=1 --skim_path=$SKIMDIR/$FILESTR/ &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
	$JOBSUBMIT $JOBDIR/$JOB.sh
	
	
      done
      
    done
    
  done
  
done

#if (( "$#" != "2" ))
#then
#echo ""
#    exit
#fi

