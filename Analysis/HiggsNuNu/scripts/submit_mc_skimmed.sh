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

for CHANNEL in nunu #enu munu taunu mumu
  do
  JOBDIRPREFIX=jobs/
  JOBDIR=$JOBDIRPREFIX/$CHANNEL/Skim/
  OUTPUTPREFIX=output/ #oldanalysisruns/220713_taunominaltightwithsysts/output/
  OUTPUTDIR=$OUTPUTPREFIX/$CHANNEL/Skim/
      
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
    
    grep "_enu" tmp.txt
    if (( "$?" == 0 )); then
	$JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR --met_cut=130 --channel=$CHANNEL --wstream=enu &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh 0
	$JOBSUBMIT $JOBDIR/$JOB.sh $JOB
    else  
	grep "_munu" tmp.txt
	if (( "$?" == 0 )); then
	    $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR --met_cut=130 --channel=$CHANNEL --wstream=munu &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh 0
	    $JOBSUBMIT $JOBDIR/$JOB.sh $JOB
	else	
	    grep "_taunu" tmp.txt
	    if (( "$?" == 0 )); then
		$JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR --met_cut=130 --channel=$CHANNEL --wstream=taunu &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh 0
		$JOBSUBMIT $JOBDIR/$JOB.sh $JOB
	    else
		$JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR --met_cut=130 --channel=$CHANNEL &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh 0
		$JOBSUBMIT $JOBDIR/$JOB.sh $JOB
	    fi
	fi
    fi
    rm tmp.txt tmp2.txt
    
  done
	
done


#if (( "$#" != "2" ))
#then
#echo ""
#    exit
#fi

