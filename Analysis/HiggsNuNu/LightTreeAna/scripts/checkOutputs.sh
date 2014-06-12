#!/bin/sh

JOBDIR=jobs
INJOBDIR=0

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
        *)
            echo " **** ERROR:  Unrecognised option: "
            $0 -h
            exit 1;
            ;;
    esac ;
done ;


if (( "$INJOBDIR" == "0" )); then
    JOBDIR=jobs
fi

echo "Processing directory: "$JOBDIR

RESULT=0

for LOGFILE in `ls $JOBDIR/*.log`
  do
  grep -q "All modules ran and exited with status 0." $LOGFILE
  if (( "$?" == 1 )); then
      echo "File $LOGFILE failed !"
      let RESULT=1
      SHFILE=`echo $LOGFILE | sed "s/log/sh/"`
      grep -q "Error opening the file" $LOGFILE
      if (( "$?" == 0 )); then
	  echo "--> Error opening a file, need to resubmit job $SHFILE!"
	  echo "./scripts/submit_ic_batch_job.sh hepshort.q $SHFILE"
      else
	  tail -5 $LOGFILE
	  echo "--> Error: Please fix and resubmit with command:"
	  echo "./scripts/submit_ic_batch_job.sh hepmedium.q $SHFILE"
      fi
  fi
  grep -qi "nan" $LOGFILE | grep -v ".root"
  if (( "$?" == 0 )); then
      grep -qi "nan" $LOGFILE | grep -v "value"
      if (( "$?" == 0 )); then
	  echo "File $LOGFILE failed ! Found NAN value."
	  let RESULT=1
      fi
  fi
done

if (( "$RESULT" == 0 )); then
    echo "-- All Clear !"
fi

if (( "$INJOBDIR" == "1" )); then
    exit;
fi
