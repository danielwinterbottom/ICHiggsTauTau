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

for METCUT in 130
# 0 70
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
            JOBDIR=jobs/$CHANNEL/MET$METCUT/DOQCD$DOQCD/JESUP/
            OUTPUTDIR=output/$CHANNEL/MET$METCUT/DOQCD$DOQCD/JESUP/
        fi
	
        if [ "$SYST" = "JESDOWN" ]
            then
            SYSTOPTIONS="--dojessyst=true --upordown=false"
            JOBDIR=jobs/$CHANNEL/MET$METCUT/DOQCD$DOQCD/JESDOWN/
            OUTPUTDIR=output/$CHANNEL/MET$METCUT/DOQCD$DOQCD/JESDOWN/
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
	  
	  PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/Mar20/MET/
	  
	  for FILELIST in `ls filelists/$QUEUEDIR/Mar20_MET_*`
	    do
	    echo "Processing files in "$FILELIST
	    
	    echo $FILELIST > tmp.txt
	    sed "s/filelists\/$QUEUEDIR\/Mar20_MET_//" tmp.txt > tmp2.txt
	    
	    JOB=Data_`sed "s/\.dat//" tmp2.txt`
	    
	    rm tmp.txt tmp2.txt
	    
	    echo "JOB name = $JOB"
	    
	    $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT --signal_region=$DOQCD $SYSTOPTIONS --channel=$CHANNEL &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
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

