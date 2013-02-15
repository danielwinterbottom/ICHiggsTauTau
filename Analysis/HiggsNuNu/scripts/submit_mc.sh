#!/bin/sh

## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}

export JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"


echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT

CONFIG=scripts/DefaultConfigNunuMC.cfg
echo "Config file: $CONFIG"

#Process HiggsNuNu specific backgrounds

#Signal files and DYtoNuNu
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/amagnan/Dec1/VBFH120/
for FILELIST in `ls filelists/Dec1_VBFH120_* filelists/Dec1_ZJets_*`
  do
  echo "Processing files in "$FILELIST

  echo $FILELIST > tmp.txt
  sed "s/filelists\/Dec1_VBFH120_//" tmp.txt > tmp2a.txt
  sed "s/filelists\/Dec1_ZJets_//" tmp2a.txt > tmp2.txt

  JOB=MC_`sed "s/\.dat//" tmp2.txt`

  rm tmp.txt tmp2.txt

  echo "JOB name = $JOB"

  $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

done


VBFZ
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/amagnan/Dec5/VBFZ/
for FILELIST in `ls filelists/Dec5_VBFZ*`
  do
  echo "Processing files in "$FILELIST

  echo $FILELIST > tmp.txt
  sed "s/filelists\/Dec5_VBFZ_//" tmp.txt > tmp2.txt

  JOB=MC_`sed "s/\.dat//" tmp2.txt`

  rm tmp.txt tmp2.txt

  echo "JOB name = $JOB"

  $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

done

#GJets
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/amagnan/Feb1/BKG/
for FILELIST in `ls filelists/Feb1_GJets_*`
  do
  echo "Processing files in "$FILELIST

  echo $FILELIST > tmp.txt
  sed "s/filelists\/Feb1_GJets_//" tmp.txt > tmp2.txt

  JOB=MC_`sed "s/\.dat//" tmp2.txt`

  rm tmp.txt tmp2.txt

  echo "JOB name = $JOB"

  $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

done

#Process bkg common with HiggsTautau
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/Dec2/MC_53X/
for FILELIST in `ls filelists/Dec2_MC_*`
#for FILELIST in `ls filelists/Dec30_MC_53X_DY*`
  do
  echo "Processing files in "$FILELIST

  echo $FILELIST > tmp.txt
  sed "s/filelists\/Dec2_MC_53X_//" tmp.txt > tmp2.txt

  JOB=MC_`sed "s/\.dat//" tmp2.txt`

  rm tmp.txt tmp2.txt

  echo "JOB name = $JOB"

  $JOBWRAPPER "./bin/HiggsNuNu --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

done







#if (( "$#" != "2" ))
#then
#echo ""
#    exit
#fi

