#!/bin/sh

if [ "$#" -ne "3" ]; then
    echo "Usage: $0 <production name> <MC or DATA> <failed word: FAILED SUBMITTED QUEUED ..."
    exit 0
fi

PROD=$1
DATAMC=$2
FAILEDWORD=$3

echo "Getting jobs in COMPLETED status"

for crabdir in `ls $PROD/$DATAMC/`;
do 
    SAMPLE=${crabdir:5}
    echo " -- processing sample $SAMPLE"
    ls $PROD/$DATAMC/$crabdir/PROCESSED
    if (( "$?" != 0 )); then
	crab status $PROD/$DATAMC/$crabdir/ | tee logfile
	grep "Task status" logfile | grep "COMPLETED"
	if (( "$?" == 0 )); then
	    grep "Jobs status" logfile | grep -e "finished *100.0%"
	    if (( "$?" == 0 )); then
		../../Analysis/HiggsNuNu/scripts/run2_filelist_from_crab.py $PROD/$DATAMC/$crabdir/
		ls ${PROD}_${DATAMC}_$SAMPLE.dat
		if (( "$?" == 0 )); then
		    echo "Sample Processed, nFiles=" `cat ${PROD}_${DATAMC}_$SAMPLE.dat | wc -l`" "`cat ${PROD}_${DATAMC}_${SAMPLE}_report.dat` > $PROD/$DATAMC/$crabdir/PROCESSED
		else
		    echo " -- Failure to retrieve filelist for task $PROD/$DATAMC/$crabdir."
		fi
	    else 
		echo " -- Jobs not in 100% finished state:"
		grep -A 10 "Jobs status" logfile | grep "%"
		echo " Would you like to resubmit ? If not, will make filelist. Choice: Y or N. Nothing means skip."
		read doresubmit
		if [ "$doresubmit" = "Y" ]; then
		    crab resubmit $PROD/$DATAMC/$crabdir
		elif [ "$doresubmit" = "N" ]; then
		    echo "Getting filelists anyway:"
		    ../../Analysis/HiggsNuNu/scripts/run2_filelist_from_crab.py $PROD/$DATAMC/$crabdir/
		    ls ${PROD}_${DATAMC}_$SAMPLE.dat
		    if (( "$?" == 0 )); then
			echo "Sample Failed but Processed, nFiles=" `cat ${PROD}_${DATAMC}_$SAMPLE.dat | wc -l`" "`cat ${PROD}_${DATAMC}_${SAMPLE}_report.dat` > $PROD/$DATAMC/$crabdir/PROCESSED
		    else
			echo " -- Failure to retrieve filelist for task $PROD/$DATAMC/$crabdir."
		    fi
		else
		    echo "Skipping."
		fi
	    fi
	else
	    grep "Task status" logfile | grep $FAILEDWORD
	    if (( "$?" == 0 )); then
		grep -A 10 "Jobs status" logfile | grep "%"
		echo " Would you like to resubmit ? If not, will make filelist. Choice: Y or N. Nothing means skip."
		read doresubmit
		if [ "$doresubmit" = "Y" ]; then
		    crab resubmit $PROD/$DATAMC/$crabdir
		elif [ "$doresubmit" = "N" ]; then
		    echo "Getting filelists anyway:"
		    ../../Analysis/HiggsNuNu/scripts/run2_filelist_from_crab.py $PROD/$DATAMC/$crabdir/
		    ls ${PROD}_${DATAMC}_$SAMPLE.dat
		    if (( "$?" == 0 )); then
			echo "Sample Failed but Processed, nFiles=" `cat ${PROD}_${DATAMC}_$SAMPLE.dat | wc -l`" "`cat ${PROD}_${DATAMC}_${SAMPLE}_report.dat` > $PROD/$DATAMC/$crabdir/PROCESSED
		    else
			echo " -- Failure to retrieve filelist for task $PROD/$DATAMC/$crabdir."
		    fi
		else
		    echo "Skipping."
		fi
	    else
		echo " -- Task $PROD/$DATAMC/$crabdir not in COMPLETED or $FAILEDWORD status, skipping."
	    fi
	fi
    else
	echo " -- Task $PROD/$DATAMC/$crabdir already processed, skipping."
    fi

done


#Jobs status:                    finished      100.0% (23/23)
#Task status:



#  LocalWords:  DATAMC
