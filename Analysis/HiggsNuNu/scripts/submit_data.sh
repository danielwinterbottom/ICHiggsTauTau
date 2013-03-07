#!/bin/sh

## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}

export JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"


echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT

CONFIG=scripts/DefaultConfig.cfg
METCUT=130
DOQCD=0

for CHANNEL in nunu enu munu
  do
  
  JOBDIR=jobs/$CHANNEL/MET$METCUT/DOQCD$DOQCD/
  OUTPUTDIR=output/$CHANNEL/MET$METCUT/DOQCD$DOQCD/

  echo "Config file: $CONFIG"
  mkdir -p $JOBDIR
  mkdir -p $OUTPUTDIR

  PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/amagnan/Dec1/MET/
  for FILELIST in `ls filelists/Dec1_MET_*`
#for FILELIST in `ls filelists/Dec1_MET_MET-2012C-11Dec2012-v1.dat`
    do
    echo "Processing files in "$FILELIST

    echo $FILELIST > tmp.txt
    sed "s/filelists\/Dec1_MET_//" tmp.txt > tmp2.txt
    
    JOB=Data_`sed "s/\.dat//" tmp2.txt`

    rm tmp.txt tmp2.txt

    echo "JOB name = $JOB"

    $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT --do_qcd_region=$DOQCD --channel=$CHANNEL &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
    $JOBSUBMIT $JOBDIR/$JOB.sh

  done

  
  PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/Dec1/MET/

  for FILELIST in `ls filelists/MET-*`
    do
    echo "Processing files in "$FILELIST
    
    echo $FILELIST > tmp.txt
    sed "s/filelists\///" tmp.txt > tmp2.txt
    
    JOB=Data_`sed "s/\.dat//" tmp2.txt`
    
    rm tmp.txt tmp2.txt
    
    echo "JOB name = $JOB"
    
    $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR --met_cut=$METCUT --do_qcd_region=$DOQCD --channel=$CHANNEL &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
    $JOBSUBMIT $JOBDIR/$JOB.sh
    
  done

done


#if (( "$#" != "2" ))
#then
#echo ""
#    exit
#fi

