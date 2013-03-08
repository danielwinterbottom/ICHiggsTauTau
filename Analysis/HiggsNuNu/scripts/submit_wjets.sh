#!/bin/sh

## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}

export JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"


echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT

CONFIG=scripts/DefaultConfigMC.cfg
echo "Config file: $CONFIG"

for METCUT in 0 70 130
  do
  for DOQCD in 0 1
    do
    for CHANNEL in nunu enu munu
      do
    
      JOBDIR=jobs/$CHANNEL/MET$METCUT/DOQCD$DOQCD/
      OUTDIR=output/$CHANNEL/MET$METCUT/DOQCD$DOQCD/
      mkdir -p $JOBDIR
      mkdir -p $OUTDIR
      
      
#Process W+jets skimmed files
      for FLAVOUR in enu munu taunu
	do
	PREFIX=/vols/ssd00/cms/invskims/$FLAVOUR/Dec2/MC_53X/
	for FILELIST in `ls filelists/Dec2_MC_53X_W*ToLNu*`
	  do
	  echo "Processing files in "$FILELIST
	  
	  echo $FILELIST > tmp.txt
	  sed "s/filelists\/Dec2_MC_53X_//" tmp.txt > tmp2.txt
	  
	  JOB=MC_`sed "s/\.dat//" tmp2.txt`_$FLAVOUR
	  
	  rm tmp.txt tmp2.txt
	  
	  echo "JOB name = $JOB"
	  echo "OUTPUT dir = $OUTDIR"
	  $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTDIR --met_cut=$METCUT --do_qcd_region=$DOQCD --channel=$CHANNEL &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
	  $JOBSUBMIT $JOBDIR/$JOB.sh
	  
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

