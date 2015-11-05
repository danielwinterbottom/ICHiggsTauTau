#!/bin/sh

if [ "$#" -ne "1" ]; then
    echo "Usage: $0 <production name>"
    exit 0
fi

PROD=$1

echo "Getting jobs in COMPLETED status"

for crabdir in `ls $PROD/MC/`; 
do 
    SAMPLE=${crabdir:5}
    echo " -- processing sample $SAMPLE"
    ls $PROD/MC/$crabdir/PROCESSED
    if (( "$?" != 0 )); then
	crab status $PROD/MC/$crabdir/ | tee logfile
	grep "Task status" logfile | grep "COMPLETED"
	if (( "$?" == 0 )); then
	    grep "Jobs status" logfile | grep -e "finished *100.0%"
	    if (( "$?" == 0 )); then
		../../Analysis/HiggsNuNu/scripts/run2_filelist_from_crab.py $PROD/MC/$crabdir/
		ls ${PROD}_MC_$SAMPLE.dat
		if (( "$?" == 0 )); then
		    echo "Sample Processed, nFiles=" `cat ${PROD}_MC_$SAMPLE.dat | wc -l`" "`cat ${PROD}_MC_${SAMPLE}_report.dat` > $PROD/MC/$crabdir/PROCESSED
		else
		    echo " -- Failure to retrieve filelist for task $PROD/MC/$crabdir."
		fi
	    else 
		echo " -- Jobs not in 100% finished state:"
		grep -A 10 "Jobs status" logfile | grep "%"
		echo " Would you like to resubmit ? If not, will make filelist. Choice: Y or N. Nothing means skip."
		read doresubmit
		if [ "$doresubmit" = "Y" ]; then
		    crab resubmit $PROD/MC/$crabdir
		elif [ "$doresubmit" = "N" ]; then
		    echo "Getting filelists anyway:"
		    ../../Analysis/HiggsNuNu/scripts/run2_filelist_from_crab.py $PROD/MC/$crabdir/
		    ls ${PROD}_MC_$SAMPLE.dat
		    if (( "$?" == 0 )); then
			echo "Sample Failed but Processed, nFiles=" `cat ${PROD}_MC_$SAMPLE.dat | wc -l`" "`cat ${PROD}_MC_${SAMPLE}_report.dat` > $PROD/MC/$crabdir/PROCESSED
		    else
			echo " -- Failure to retrieve filelist for task $PROD/MC/$crabdir."
		    fi
		else
		    echo "Skipping."
		fi
	    fi
	else
	    grep "Task status" logfile | grep "FAILED"
	    if (( "$?" == 0 )); then
		grep -A 10 "Jobs status" logfile | grep "%"
		echo " Would you like to resubmit ? If not, will make filelist. Choice: Y or N. Nothing means skip."
		read doresubmit
		if [ "$doresubmit" = "Y" ]; then
		    crab resubmit $PROD/MC/$crabdir
		elif [ "$doresubmit" = "N" ]; then
		    echo "Getting filelists anyway:"
		    ../../Analysis/HiggsNuNu/scripts/run2_filelist_from_crab.py $PROD/MC/$crabdir/
		    ls ${PROD}_MC_$SAMPLE.dat
		    if (( "$?" == 0 )); then
			echo "Sample Failed but Processed, nFiles=" `cat ${PROD}_MC_$SAMPLE.dat | wc -l`" "`cat ${PROD}_MC_${SAMPLE}_report.dat` > $PROD/MC/$crabdir/PROCESSED
		    else
			echo " -- Failure to retrieve filelist for task $PROD/MC/$crabdir."
		    fi
		else
		    echo "Skipping."
		fi
	    else
		echo " -- Task $PROD/MC/$crabdir not in COMPLETED or FAILED status, skipping."
	    fi
	fi
    else
	echo " -- Task $PROD/MC/$crabdir already processed, skipping."
    fi

done


#Jobs status:                    finished      100.0% (23/23)
#Task status:


