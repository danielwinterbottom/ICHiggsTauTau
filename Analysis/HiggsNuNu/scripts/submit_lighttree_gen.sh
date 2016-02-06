#!/bin/sh
DOCERN=0
DOSUBMIT=1
#MYEXEC=LightTreeMakerFromMiniAOD
MYEXEC=GenLightTreeMaker
PRODUCTION=160203
PRODUSER=amagnan

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

CONFIG=scripts/DefaultGenLightTreeConfig.cfg

JOBDIRPREFIX=jobs_genlighttree_160206
JOBDIR=$JOBDIRPREFIX/
OUTPUTPREFIX=output_genlighttree_160206
OUTPUTDIR=$OUTPUTPREFIX/

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
    for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_MC_EWK*`
#    for FILELIST in `ls filelists/$PRODUCTION/$QUEUEDIR/${PRODUCTION}_MC_WJ*`
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
	
	$JOBWRAPPER $JOBDIR $OUTPUTDIR "./bin/$MYEXEC --cfg=$CONFIG --prod="$PRODUCTION" --filelist="$FILELIST" --input_prefix=$PREFIX --output_name=$JOB.root --output_folder=$OUTPUTDIR | tee $JOBDIR/$JOB.log" $JOBDIR/$JOB.sh  $GRIDSETUP
	if [ "$DOSUBMIT" = "1" ]; then 
	    $JOBSUBMIT $JOBDIR/$JOB.sh
	else
	    echo "$JOBSUBMIT $JOBDIR/$JOB.sh"
	fi                                                                                  
	rm tmp.txt tmp2.txt
	
    done
	
    
done
  


#if (( "$#" != "2" ))
#then
#echo ""
#    exit
#fi

