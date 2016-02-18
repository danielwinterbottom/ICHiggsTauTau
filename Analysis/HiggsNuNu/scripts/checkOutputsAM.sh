#!/bin/sh

JOBDIR=jobs_lighttree_160207/
INJOBDIR=0
PROD=160213

while [ $# -gt 0 ] ; do

    case $1 in
        -h|--help|-help)
            echo "usage: "$0"  [-d <job directory>]"
            exit 1;
            ;;
        -d)
	JOBDIR=$2
	INJOBDIR=1
            shift 2;
            ;;
	-p)
	PROD=$3
	    shift 3;
	    ;;
        *)
            echo " **** ERROR:  Unrecognised option: "
            $0 -h
            exit 1;
            ;;
    esac ;
done ;

echo "Processing directory: "$JOBDIR
echo "Production is "$PROD
RESULT=0

for LOGFILE in `ls $JOBDIR/*.log`
do
    isData=0
    WSTREAM=""
    SPLITSTREAM=""
    echo $LOGFILE | grep -q "MET_"
    if (( "$?" == 0 )); then
	let isData=1
    fi
    JOBFILE=`echo $LOGFILE | sed "s!$JOBDIR/!!" | sed "s/\.log//"`
    echo $LOGFILE | grep -q "nu.log"
    if (( "$?" == 0 )); then
	WSTREAM=`echo $LOGFILE | sed "s!$JOBDIR/.*_!_!" | sed "s/\.log//"`
	#echo "wstream = $WSTREAM"
	JOBFILE=`echo $LOGFILE | sed "s!$JOBDIR/!!" | sed "s/\.log//" | sed "s!$WSTREAM!!"`
    fi
    echo $LOGFILE | grep -q "split"
    if (( "$?" == 0 )); then
	echo $LOGFILE | grep -q "nu.log"
	if (( "$?" == 0 )); then
	    SPLITSTREAM=`echo $LOGFILE | sed "s!$JOBDIR/.*_split!_split!" | sed "s/\.log//" | sed "s!$WSTREAM!!"`
	    #echo "split string = $SPLITSTREAM"
	    JOBFILE=`echo $LOGFILE | sed "s!$JOBDIR/!!" | sed "s/\.log//" | sed "s!$WSTREAM!!" | sed "s!$SPLITSTREAM!!"`
	else
	    SPLITSTREAM=`echo $LOGFILE | sed "s!$JOBDIR/.*_split!_split!" | sed "s/\.log//"`
	    #echo "split string = $SPLITSTREAM"
	    JOBFILE=`echo $LOGFILE | sed "s!$JOBDIR/!!" | sed "s/\.log//" | sed "s!$SPLITSTREAM!!"`
	fi
    fi
    grep -q "Processing Complete" $LOGFILE
    if (( "$?" == 1 )); then
	echo "****File $LOGFILE failed !"
	let RESULT=$RESULT+1
	grep -q "Error opening the file" $LOGFILE
	if (( "$?" == 0 )); then
	    echo "--> Error opening a file, need to resubmit job $JOBDIR/${JOBFILE}${WSTREAM}.sh!"
	    echo "./scripts/submit_ic_batch_job.sh hepshort.q $JOBDIR/${JOBFILE}${WSTREAM}.sh"
	else
	    tail -5 $LOGFILE
	    echo "--> Error: Please fix and resubmit with command:"
	    echo "./scripts/submit_ic_batch_job.sh hepmedium.q $JOBDIR/${JOBFILE}${WSTREAM}.sh"
	fi
	#else
	#    echo "File $LOGFILE succeeded !"
    fi
    grep -qi "nan" $LOGFILE | grep -v ".root"
    if (( "$?" == 0 )); then
	grep -qi "nan" $LOGFILE | grep -v "value"
	if (( "$?" == 0 )); then
	    echo "****File $LOGFILE failed ! Found NAN value."
	    let RESULT=$RESULT+1
	fi
    fi

    nfilelist=`grep "EVT_$JOBFILE:" filelists/$PROD/Params$PROD.dat | grep -v "#" | sed "s!EVT_$JOBFILE:!!"`
    missed=0

    echo $LOGFILE | grep -q "split00"
    if (( "$?" == 0 )); then
	echo "adding together splitted files"
	ntotal=0
	npassed=0
	rejected=0
	for SPLITLOGFILE in `ls $JOBDIR/${JOBFILE}_split*${WSTREAM}.log`
	do
	    #echo "JOBFILE $JOBFILE adding $SPLITLOGFILE"
	    let ntotal=$ntotal+`grep "Processing Complete" $SPLITLOGFILE | sed 's!.*>>!!' | awk '{print $3}'`
	    let npassed=$npassed+`grep "LightTreeNew" $SPLITLOGFILE | awk '{print $2}'`
	    let rejected=$rejected+`echo "$ntotal-$npassed" | bc`
	done
	if (( "$isData" == "0" )); then
	    missed=`echo "$nfilelist-$ntotal" | bc`
	fi
	echo -e "-- Sample: "${JOBFILE}${WSTREAM}", Nfilelist="$nfilelist", Nprocessed="$ntotal", Npassed="$npassed", Nmissed="$missed", Nrejected="$rejected
	if (( "$missed" != "0" )); then
	    echo "****Problem with filelist EVT number! Please check."
	    let RESULT=$RESULT+1
	fi	
    fi
    echo $LOGFILE | grep -q "split"
    if (( "$?" == 0 )); then
	echo "skip"
    else
	ntotal=`grep "Processing Complete" $LOGFILE | sed 's!.*>>!!' | awk '{print $3}'`
	npassed=`grep "LightTreeNew" $LOGFILE | awk '{print $2}'`
	rejected=`echo "$ntotal-$npassed" | bc`
	if (( "$isData" == "0" )); then
	    missed=`echo "$nfilelist-$ntotal" | bc`
	fi
	echo -e "-- Sample: "${JOBFILE}${WSTREAM}", Nfilelist="$nfilelist", Nprocessed="$ntotal", Npassed="$npassed", Nmissed="$missed", Nrejected="$rejected
	if (( "$missed" != "0" )); then
	    echo "****Problem with filelist EVT number! Please check."
	    let RESULT=$RESULT+1
	fi
    fi
done

if (( "$RESULT" == 0 )); then
    echo "-- All Clear !"
else
    echo "-- Found $RESULT problematic jobs."
fi

