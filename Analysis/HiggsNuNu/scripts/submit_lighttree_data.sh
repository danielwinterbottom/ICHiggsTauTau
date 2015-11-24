#!/bin/sh
DOCERN=1
DOSUBMIT=0
MYEXEC=LightTreeMakerFromMiniAOD
PRODUCTION=151113
PRODUSER=amagnan


## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh $DOCERN $MYEXEC $PRODUCTION"}
: ${JOBSUBMIT:="eval"}

#export JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"

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


for SYST in central #JESDOWN JERBETTER JERWORSE UESUP UESDOWN ELEEFFUP ELEEFFDOWN MUEFFUP MUEFFDOWN #NOTE SYSTEMATIC RUNS WILL BE SAME AS CENTRAL BUT OUTPUT WILL GO TO SYSTEMATIC SUBDIRECTORIES
  do
  SYSTOPTIONS="--dojessyst=false --dojersyst=false" 

  JOBDIRPREFIX=jobs_lighttree_301015goldenjson_l1metfix_131115
  JOBDIR=$JOBDIRPREFIX/
  OUTPUTPREFIX=output_lighttree_301015goldenjson_l1metfix_131115
  OUTPUTDIR=$OUTPUTPREFIX/
  
  if [ "$SYST" = "JESUP" ]
      then
      SYSTOPTIONS="--dojessyst=true --jesupordown=true"
      JOBDIR=$JOBDIRPREFIX/JESUP/
      OUTPUTDIR=$OUTPUTPREFIX/JESUP/
  fi
  
  if [ "$SYST" = "JESDOWN" ]
      then
      SYSTOPTIONS="--dojessyst=true --jesupordown=false"
      JOBDIR=$JOBDIRPREFIX/JESDOWN/
      OUTPUTDIR=$OUTPUTPREFIX/JESDOWN/
  fi  
  
  if [ "$SYST" = "JERBETTER" ]
      then
      SYSTOPTIONS="--dojessyst=false --dojersyst=true --jerbetterorworse=true"
      JOBDIR=$JOBDIRPREFIX/JERBETTER/
      OUTPUTDIR=$OUTPUTPREFIX/JERBETTER/
  fi  
  
  if [ "$SYST" = "JERWORSE" ]
      then
      SYSTOPTIONS="--dojessyst=false --dojersyst=true --jerbetterorworse=false"
      JOBDIR=$JOBDIRPREFIX/JERWORSE/
	  OUTPUTDIR=$OUTPUTPREFIX/JERWORSE/
  fi  
  
  if [ "$SYST" = "UESUP" ]
	then
	SYSTOPTIONS="--douessyst=true --uesupordown=true"
	JOBDIR=$JOBDIRPREFIX/UESUP/  
  	OUTPUTDIR=$OUTPUTPREFIX/UESUP/
  fi

  if [ "$SYST" = "UESDOWN" ]
	then
	SYSTOPTIONS="--douessyst=true --uesupordown=false"
	JOBDIR=$JOBDIRPREFIX/UESDOWN/
	OUTPUTDIR=$OUTPUTPREFIX/UESDOWN/
  fi

if [ "$SYST" = "ELEEFFUP" ]
        then
        SYSTOPTIONS="--doidisoerr=true --doidisoerrmuore=false --doidisoerrupordown=true"
        JOBDIR=$JOBDIRPREFIX/ELEEFFUP/
        OUTPUTDIR=$OUTPUTPREFIX/ELEEFFUP/
  fi

  if [ "$SYST" = "ELEEFFDOWN" ]
        then
        SYSTOPTIONS="--doidisoerr=true --doidisoerrmuore=false --doidisoerrupordown=false"
        JOBDIR=$JOBDIRPREFIX/ELEEFFDOWN/
        OUTPUTDIR=$OUTPUTPREFIX/ELEEFFDOWN/
  fi

  if [ "$SYST" = "MUEFFUP" ]
        then
        SYSTOPTIONS="--doidisoerr=true --doidisoerrmuore=true --doidisoerrupordown=true"
        JOBDIR=$JOBDIRPREFIX/MUEFFUP/
        OUTPUTDIR=$OUTPUTPREFIX/MUEFFUP/
  fi

  if [ "$SYST" = "MUEFFDOWN" ]
        then
        SYSTOPTIONS="--doidisoerr=true --doidisoerrmuore=true --doidisoerrupordown=false"
        JOBDIR=$JOBDIRPREFIX/MUEFFDOWN/
        OUTPUTDIR=$OUTPUTPREFIX/MUEFFDOWN/
  fi

  
  echo "Config file: $CONFIG"
  mkdir -p $JOBDIR
  mkdir -p $OUTPUTDIR

  cp $CONFIG $OUTPUTDIR

  for QUEUEDIR in medium long
    do
    if [ "$DOCERN" = "0" ]
	then
	if [ "$QUEUEDIR" = "medium" ]
	    then
	    JOBQUEUE="hepmedium.q"
	elif [ "$QUEUEDIR" = "long" ]
	    then
                JOBQUEUE="heplong.q"
	else
	    JOBQUEUE="hepshort.q"
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

    PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/${PRODUSER}/${PRODUCTION}_MET
    if [ "$PRODUCTION" = "Dec18" ]
    then
        PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/${PRODUSER}/${PRODUCTION}/MET
    fi    

    for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_MET_*` 
      do
      echo "Processing files in "$FILELIST
      
      echo $FILELIST > tmp.txt
      sed "s/filelists\/${PRODUCTION}\/$QUEUEDIR\/${PRODUCTION}_MET_//" tmp.txt > tmp2.txt
      
      JOB=MET_`sed "s/\.dat//" tmp2.txt`
      
      echo "JOB name = $JOB"
      
      $JOBWRAPPER $JOBDIR $OUTPUTDIR "./bin/$MYEXEC --cfg=$CONFIG --prod="$PRODUCTION" --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR  $SYSTOPTIONS --input_params=$INPUTPARAMS | tee $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh $GRIDSETUP
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

