#!/bin/sh
DOCERN=0
DOSUBMIT=1
#JETTYPE="ak4SlimmedJetsPuppi"
JETTYPE="pfJetsPFlow"
#MYEXEC=JetMETvalidation
MYEXEC=LightTreeMakerFromMiniAOD
PRODUCTION=160213
PRODUSER=amagnan
JPTCUTVAL=70
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

echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT

CONFIG=scripts/DefaultLightTreeConfig_mc.cfg
INPUTPARAMS="filelists/$PRODUCTION/Params${PRODUCTION}.dat"


for SYST in central JESUP JESDOWN JERBETTER JERWORSE UESUP UESDOWN #ELEEFFUP ELEEFFDOWN MUEFFUP MUEFFDOWN #NOTE TO RUN JER DOSMEAR MUST BE SET TO TRUE IN THE CONFIG
  do
  SYSTOPTIONS="--dojessyst=false --dojersyst=false"
  JOBDIRPREFIX=jobs_lighttree_160216
  JOBDIR=$JOBDIRPREFIX/
  OUTPUTPREFIX=/vols/cms02/magnan/Hinvisible/RunIILT/output_lighttree_160216
  #OUTPUTPREFIX=output_lighttree_160121
  OUTPUTDIR=$OUTPUTPREFIX/

  if [ "$SYST" != "central" ]
  then
      JOBDIR=$JOBDIRPREFIX/$SYST/
      OUTPUTDIR=$OUTPUTPREFIX/$SYST/
  fi
  
  if [ "$SYST" = "JESUP" ]
      then
      SYSTOPTIONS="--dojessyst=true --jesupordown=true --dojersyst=false"
  fi
  
  if [ "$SYST" = "JESDOWN" ]
      then
      SYSTOPTIONS="--dojessyst=true --jesupordown=false --dojersyst=false"
  fi
  
  if [ "$SYST" = "JERBETTER" ]
      then
      SYSTOPTIONS="--dojessyst=false --dojersyst=true --jerbetterorworse=true"
  fi
  
  if [ "$SYST" = "JERWORSE" ]
      then
      SYSTOPTIONS="--dojessyst=false --dojersyst=true --jerbetterorworse=false"
  fi

  if [ "$SYST" = "UESUP" ]
	then
	SYSTOPTIONS="--douessyst=true --uesupordown=true"
  fi

  if [ "$SYST" = "UESDOWN" ]
	then
	SYSTOPTIONS="--douessyst=true --uesupordown=false"
  fi

  if [ "$SYST" = "ELEEFFUP" ]
	then
	SYSTOPTIONS="--doidisoerr=true --doidisoerrmuore=false --doidisoerrupordown=true"
  fi

  if [ "$SYST" = "ELEEFFDOWN" ]
	then
	SYSTOPTIONS="--doidisoerr=true --doidisoerrmuore=false --doidisoerrupordown=false"
  fi

  if [ "$SYST" = "MUEFFUP" ]
	then
	SYSTOPTIONS="--doidisoerr=true --doidisoerrmuore=true --doidisoerrupordown=true"
  fi

  if [ "$SYST" = "MUEFFDOWN" ]
  then
      SYSTOPTIONS="--doidisoerr=true --doidisoerrmuore=true --doidisoerrupordown=false"
  fi

  echo "Config file: $CONFIG"
  
  mkdir -p $JOBDIR
  mkdir -p $OUTPUTDIR

  cp $CONFIG $OUTPUTDIR

  for QUEUEDIR in short medium #long
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
	    JOBQUEUE="1nh"
	fi
    fi
    export JOBSUBMIT=$JOBSCRIPT" "$JOBQUEUE
    echo "Using job-submission: " $JOBSUBMIT
    
    
#Process HiggsNuNu specific backgrounds
#Signal files and DYtoNuNu
#    PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/${PRODUSER}/$PRODUCTION/MC/
    PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/${PRODUSER}/${PRODUCTION}_MC
    if [ "$PRODUCTION" = "Dec18" ]
    then
	PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/${PRODUSER}/${PRODUCTION}/MC
    fi
    for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/*_MC_*`
#    for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_MC_WJ*`
#    for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_MC_Powheg*`
	  do
      echo "Processing files in "$FILELIST

      echo $FILELIST
      echo $FILELIST > tmp.txt
      
      MCOPTION="--mc=fall15_76X"
      PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/${PRODUSER}/${PRODUCTION}_MC
      grep "151030" tmp.txt
      if (( "$?" == "0" )); then
	  PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/${PRODUSER}/151030_MC
	  sed "s/filelists\/${PRODUCTION}\/$QUEUEDIR\/151030_MC_//" tmp.txt > tmp2.txt
	  MCOPTION="--mc=spring15_74X"
	  INPUTPARAMS="filelists/151030/Params151030.dat"
      else
	  sed "s/filelists\/${PRODUCTION}\/$QUEUEDIR\/${PRODUCTION}_MC_//" tmp.txt > tmp2.txt
	  MCOPTION="--mc=fall15_76X"
	  INPUTPARAMS="filelists/$PRODUCTION/Params${PRODUCTION}.dat"
      fi

      echo $PREFIX


      #sed "s/filelists\/${PRODUCTION}\/$QUEUEDIR\/${PRODUCTION}_MC_//" tmp.txt > tmp2.txt
	  
      JOB=MC_`sed "s/\.dat//" tmp2.txt`
      
      echo "JOB name = $JOB"
      
	  # grep "JetsToLL" tmp.txt
# 	  if (( "$?" != 0 )); then
# 	      continue
# 	  fi
      
      JPTCUT=$JPTCUTVAL
      grep "Htoinv" tmp.txt
      if (( "$?" == 0 )); then
	  JPTCUT=0
      fi

      NEEDSSTREAM=0
      grep "EWKW" tmp.txt
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
	    
	    $JOBWRAPPER $JOBDIR $OUTPUTDIR "./bin/$MYEXEC --cfg=$CONFIG --prod="$PRODUCTION" "$MCOPTION" --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR $SYSTOPTIONS --inputparams=$INPUTPARAMS --wstream=$FLAVOUR --jet1ptcut="$JPTCUT" --jet2ptcut="$JPTCUT" --jettype=$JETTYPE | tee $JOBDIR/$WJOB.log" $JOBDIR/$WJOB.sh $GRIDSETUP
	    if [ "$DOSUBMIT" = "1" ]; then 
		$JOBSUBMIT $JOBDIR/$WJOB.sh    
	    else
		echo "$JOBSUBMIT $JOBDIR/$WJOB.sh"
	    fi                                                                                  
	  done
	  
      else  
	  $JOBWRAPPER $JOBDIR $OUTPUTDIR "./bin/$MYEXEC --cfg=$CONFIG --prod="$PRODUCTION" "$MCOPTION" --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR $SYSTOPTIONS --inputparams=$INPUTPARAMS --jet1ptcut="$JPTCUT" --jet2ptcut="$JPTCUT" --jettype=$JETTYPE | tee $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh  $GRIDSETUP
	    if [ "$DOSUBMIT" = "1" ]; then 
		$JOBSUBMIT $JOBDIR/$JOB.sh
	    else
		echo "$JOBSUBMIT $JOBDIR/$JOB.sh"
	    fi                                                                                  
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
	    SHAREDPREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/${PRODUSER}/$PRODUCTION/MCtaushared/
	    FILELISTPREFIX=Apr04_MCtaushared_
	fi
    fi
    
    for FILELIST in filelists/$PRODUCTION/$QUEUEDIR/${FILELISTPREFIX}*
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
		
		$JOBWRAPPER $JOBDIR $OUTPUTDIR "./bin/$MYEXEC --cfg=$CONFIG --prod="$PRODUCTION" --filelist="$FILELIST" --input_prefix=$SHAREDPREFIX --output_name=$WJOB.root --output_folder=$OUTPUTDIR $SYSTOPTIONS --inputparams=$INPUTPARAMS --wstream=$FLAVOUR --jet1ptcut="$JPTCUT" --jet2ptcut="$JPTCUT" --jettype=$JETTYPE | tee $JOBDIR/$WJOB.log" $JOBDIR/$WJOB.sh $GRIDSETUP
		if [ "$DOSUBMIT" = "1" ]; then 
		    $JOBSUBMIT $JOBDIR/$WJOB.sh
		else
		    echo "$JOBSUBMIT $JOBDIR/$WJOB.sh"
		fi                                                                                  
	      done
	  else  
	      $JOBWRAPPER $JOBDIR $OUTPUTDIR "./bin/$MYEXEC --cfg=$CONFIG --prod="$PRODUCTION" --filelist="$FILELIST" --input_prefix=$SHAREDPREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR $SYSTOPTIONS --inputparams=$INPUTPARAMS --jet1ptcut="$JPTCUT" --jet2ptcut="$JPTCUT" --jettype=$JETTYPE | tee $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh $GRIDSETUP
	      if [ "$DOSUBMIT" = "1" ]; then 
		  $JOBSUBMIT $JOBDIR/$JOB.sh
	      else
		  echo "$JOBSUBMIT $JOBDIR/$JOB.sh"
	      fi
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

