#!/bin/sh
DOCERN=0

## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}

if [ "$DOCERN" = "0" ]
    then
    JOBSCRIPT="./scripts/submit_ic_batch_job.sh" 
else
    JOBSCRIPT="./scripts/submit_cern_batch_job.sh"
fi

echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT

CONFIG=scripts/DefaultLightTreeConfig_mc.cfg
PRODUCTION=Dec18
INPUTPARAMS="filelists/Dec18/ParamsDec18.dat"


for SYST in central JESUP JESDOWN JERBETTER JERWORSE UESUP UESDOWN ELEEFFUP ELEEFFDOWN MUEFFUP MUEFFDOWN #NOTE TO RUN JER DOSMEAR MUST BE SET TO TRUE IN THE CONFIG
  do
  SYSTOPTIONS="--dojessyst=false --dojersyst=false"
  JOBDIRPREFIX=jobs_lighttree_noskimsig
  JOBDIR=$JOBDIRPREFIX/
  OUTPUTPREFIX=output_lighttree_noskimsig
  OUTPUTDIR=$OUTPUTPREFIX/
  
  if [ "$SYST" = "JESUP" ]
      then
      SYSTOPTIONS="--dojessyst=true --jesupordown=true --dojersyst=false"
      JOBDIR=$JOBDIRPREFIX/JESUP/
      OUTPUTDIR=$OUTPUTPREFIX/JESUP/
  fi
  
  if [ "$SYST" = "JESDOWN" ]
      then
      SYSTOPTIONS="--dojessyst=true --jesupordown=false --dojersyst=false"
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
  
  for QUEUEDIR in short medium long
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
    
    
#Process HiggsNuNu specific backgrounds
#Signal files and DYtoNuNu
    PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/$PRODUCTION/MC/
    for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_MC_*`
#    for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_MC_Powheg*`
	  do
      echo "Processing files in "$FILELIST
      
      echo $FILELIST
      echo $FILELIST > tmp.txt
      sed "s/filelists\/${PRODUCTION}\/$QUEUEDIR\/${PRODUCTION}_MC_//" tmp.txt > tmp2.txt
	  
      JOB=MC_`sed "s/\.dat//" tmp2.txt`
      
      echo "JOB name = $JOB"
      
	  # grep "JetsToLL" tmp.txt
# 	  if (( "$?" != 0 )); then
# 	      continue
# 	  fi
      
      NEEDSSTREAM=0
      grep "EWK-W2j" tmp.txt
      if (( "$?" == 0 )); then
	  NEEDSSTREAM=1
	  fi
      grep  "JetsToLNu" tmp.txt
      if (( "$?" == 0 )); then
	  NEEDSSTREAM=1
      fi
      if (( "$NEEDSSTREAM" == 1 )); then
	  for FLAVOUR in enu munu taunu
	    do
	    WJOB=$JOB"_"$FLAVOUR
	    
	    $JOBWRAPPER "./bin/LightTreeMaker --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$WJOB.root --output_folder=$OUTPUTDIR $SYSTOPTIONS --input_params=$INPUTPARAMS --wstream=$FLAVOUR &> $JOBDIR/$WJOB.log" $JOBDIR/$WJOB.sh
	    $JOBSUBMIT $JOBDIR/$WJOB.sh                                                                                      
	  done
	  
      else  
	  $JOBWRAPPER "./bin/LightTreeMaker --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR $SYSTOPTIONS --input_params=$INPUTPARAMS &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
	  $JOBSUBMIT $JOBDIR/$JOB.sh
      fi
      rm tmp.txt tmp2.txt
      
    done
	
    
#Process bkg common with HiggsTautau (ONLY NEEDED IN Mar20 and Apr04)
    DOSHARED=false
    if [ "$PRODUCTION" = "Mar20" ]
	then
	DOSHARED=true
	SHAREDPREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/rlane/Feb20/MC_53X/
	FILELISTPREFIX=Feb20_MC_53X_
	else
	if [ "$PRODUCTION" = "Apr04" ]
	    then
	    DOSHARED=true
	    SHAREDPREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/$PRODUCTION/MCtaushared/
	    FILELISTPREFIX=Apr04_MCtaushared_
	fi
    fi
    
    for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${FILELISTPREFIX}*`
	  do
      if [ "$DOSHARED" = "true" ]
	  then
	  echo "Processing files in "$FILELIST
	  
	  echo $FILELIST > tmp.txt
	  
	  sed "s/filelists\/${PRODUCTION}\/$QUEUEDIR\/${FILELISTPREFIX}//" tmp.txt > tmp2.txt
	      JOB=MC_`sed "s/\.dat//" tmp2.txt`
	      
	  echo "JOB name = $JOB"
	      
# 	  grep "JetsToLL" tmp.txt
# 	  if (( "$?" != 0 )); then
# 	      #continue
# 	  fi
	      
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
		
		$JOBWRAPPER "./bin/LightTreeMaker --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$SHAREDPREFIX --output_name=$WJOB.root --output_folder=$OUTPUTDIR $SYSTOPTIONS --input_params=$INPUTPARAMS --wstream=$FLAVOUR &> $JOBDIR/$WJOB.log" $JOBDIR/$WJOB.sh
		$JOBSUBMIT $JOBDIR/$WJOB.sh
	      done
	  else  
	      $JOBWRAPPER "./bin/LightTreeMaker --cfg=$CONFIG --filelist="$FILELIST" --input_prefix=$SHAREDPREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR $SYSTOPTIONS --input_params=$INPUTPARAMS &> $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh
	      $JOBSUBMIT $JOBDIR/$JOB.sh
	  fi
	  
	  rm tmp.txt tmp2.txt
      fi
      
    done
    
  done
  
done
  


#if (( "$#" != "2" ))
#then
#echo ""
#    exit
#fi

