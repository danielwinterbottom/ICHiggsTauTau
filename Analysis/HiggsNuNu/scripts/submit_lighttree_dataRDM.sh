#!/bin/sh
DOCERN=0
DOSUBMIT=1
MYEXEC=LightTreeMakerFromMiniAODRDM
PRODUCTION=161031
PRODUSER=rdimaria
JPTCUTVAL=40

## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh $DOCERN $MYEXEC $PRODUCTION"}
: ${JOBSUBMIT:="eval"}

GRIDSETUP=1

if [ "$DOCERN" = "0" ]
    then
    JOBSCRIPT="./scripts/submit_ic_batch_job.sh"
else
    JOBSCRIPT="./scripts/submit_cern_batch_job.sh"
    GRIDSETUP=0
fi
export JOBSUBMIT=$JOBSCRIPT" "$JOBQUEUE


echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT

INPUTPARAMS="filelists/$PRODUCTION/Params${PRODUCTION}.dat"
CONFIG=scripts/DefaultLightTreeConfig_data.cfg
#CONFIG=scripts/DefaultLightTreeConfig_data_forMaria.cfg


for SYST in central
  do
  SYSTOPTIONS="--dojessyst=false --dojersyst=false" 

  #JOBDIRPREFIX=/vols/cms/rd1715/HiggsToInv/jobs_lighttree_${PRODUCTION}_ICHEP
  #JOBDIRPREFIX=/vols/cms/magnan/Hinvisible/RunIILT/jobs_lighttree_170111
  JOBDIRPREFIX=/vols/cms/rd1715/HiggsToInv/jobs_lighttree_170113_ICHEP
  JOBDIR=$JOBDIRPREFIX/
  #OUTPUTPREFIX=/vols/cms/rd1715/HiggsToInv/output_lighttree_${PRODUCTION}_ICHEP
  #OUTPUTPREFIX=/vols/cms/magnan/Hinvisible/RunIILT/output_lighttree_170111
  OUTPUTPREFIX=/vols/cms/rd1715/HiggsToInv/output_lighttree_170113_ICHEP
  OUTPUTDIR=$OUTPUTPREFIX/

  if [ "$SYST" != "central" ]
  then
      JOBDIR=$JOBDIRPREFIX/$SYST/
      OUTPUTDIR=$OUTPUTPREFIX/$SYST/
  fi

  echo "Config file: $CONFIG"
  mkdir -p $JOBDIR
  mkdir -p $OUTPUTDIR

  cp $CONFIG $OUTPUTDIR

  for QUEUEDIR in medium
    do
    if [ "$DOCERN" = "0" ]
	then
	if [ "$QUEUEDIR" = "medium" ]
	    then
	    JOBQUEUE="5:59:0"
	elif [ "$QUEUEDIR" = "long" ]
	    then
                JOBQUEUE="47:59:0"
	else
	    JOBQUEUE="2:59:0"
	fi
    else
	if [ "$QUEUEDIR" = "medium" ]
	    then
	    JOBQUEUE="1nd"
	elif [ "$QUEUEDIR" = "long" ]
	    then
	    JOBQUEUE="2nd"
            else
	    JOBQUEUE="8nh"
	fi
	
    fi
    export JOBSUBMIT=$JOBSCRIPT" "$JOBQUEUE
    echo "Using job-submission: " $JOBSUBMIT

    #PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/${PRODUSER}/${PRODUCTION}_DATA
    PREFIX=root://gfe02.grid.hep.ph.ic.ac.uk:1095//store/user/${PRODUSER}/${PRODUCTION}_DATA

    if [ "$PRODUCTION" = "Dec18" ]
    then
        PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/${PRODUSER}/${PRODUCTION}/DATA
    fi

    #for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_DATA_SingleMuon*`

    #for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_DATA_SingleElectron*`
    #for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_DATA_*MET*`
    for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_DATA_*`
      do
      echo "Processing files in "$FILELIST

      echo $FILELIST > tmp.txt
      sed "s/filelists\/${PRODUCTION}\/$QUEUEDIR\/${PRODUCTION}_DATA_//" tmp.txt > tmp2.txt

      JOB=DATA_`sed "s/\.dat//" tmp2.txt`

      echo "JOB name = $JOB"

      $JOBWRAPPER $JOBDIR $OUTPUTDIR "./bin/$MYEXEC --cfg=$CONFIG --prod="$PRODUCTION" --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR  $SYSTOPTIONS --input_params=$INPUTPARAMS --jet1ptcut="$JPTCUTVAL" --jet2ptcut="$JPTCUTVAL" &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh $GRIDSETUP
      if [ "$DOSUBMIT" = "1" ]; then 
	  $JOBSUBMIT $JOBDIR/$JOB.sh
      else
	  echo "$JOBSUBMIT $JOBDIR/$JOB.sh"
      fi

      rm tmp.txt tmp2.txt

    done

  done

done
#if (( "$#" != "2" ))
#then
#echo ""
#    exit
#fi
