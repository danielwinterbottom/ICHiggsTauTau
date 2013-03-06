#!/bin/sh

if [ -z $1 ]
then
    echo "Must specify <jobdir>"
    exit
fi

JOBDIR=$1

for LOGFILE in `ls $JOBDIR/*.log`
  do
  grep -q "Processing Complete" $LOGFILE
  if (( "$?" == 1 )); then
      echo "File $LOGFILE failed !"
  fi
done