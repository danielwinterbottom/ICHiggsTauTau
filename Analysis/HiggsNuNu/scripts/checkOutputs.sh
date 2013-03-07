#!/bin/sh

if [ -z $1 ]
then
    echo "Must specify <jobdir>"
    exit
fi

JOBDIR=$1

RESULT=0

for LOGFILE in `ls $JOBDIR/*.log`
  do
  grep -q "Processing Complete" $LOGFILE
  if (( "$?" == 1 )); then
      echo "File $LOGFILE failed !"
      let RESULT=1
  fi
done

if (( "$RESULT" == 0 )); then
    echo "-- All Clear !"
fi