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

for METCUT in 130 0 70
  do
  for DOQCD in 0 1 2
    do
    for CHANNEL in nunu enu munu
      do
      for SYST in central #JESUP JESDOWN
        do
        SYSTOPTIONS="--dojessyst=false"
        JOBDIR=jobs/$CHANNEL/MET$METCUT/DOQCD$DOQCD/
        OUTPUTDIR=output/$CHANNEL/MET$METCUT/DOQCD$DOQCD/
	
        if [ "$SYST" = "JESUP" ]
            then
            SYSTOPTIONS="--dojessyst=true --upordown=true"
            JOBDIR=jobs/$CHANNEL/MET$METCUT/DOQCD$DOQCD/JESUP
            OUTPUTDIR=output/$CHANNEL/MET$METCUT/DOQCD$DOQCD/JESUP
        fi
	
        if [ "$SYST" = "JESDOWN" ]
            then
            SYSTOPTIONS="--dojessyst=true --upordown=false"
            JOBDIR=jobs/$CHANNEL/MET$METCUT/DOQCD$DOQCD/JESDOWN
            OUTPUTDIR=output/$CHANNEL/MET$METCUT/DOQCD$DOQCD/JESDOWN
        fi
	
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
	  $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTDIR --met_cut=$METCUT --signal_region=$DOQCD $SYSTOPTIONS --channel=$CHANNEL &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
	  $JOBSUBMIT $JOBDIR/$JOB.sh
	  
	done
	
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

