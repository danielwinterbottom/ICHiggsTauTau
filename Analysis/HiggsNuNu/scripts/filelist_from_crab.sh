#!/bin/bash

if [ "$#" -lt "1" ]
then
  echo "Required arguments: <CRAB TASK>"
  exit
fi

STRIPPED=`echo $1 | sed "s/\//_/g"`".dat"
STRIPPED=`echo $STRIPPED | sed "s/_.dat/.dat/"`

echo Writing filelist to $STRIPPED
find_goodfiles.py -q -c $1 | cut -d / -f 1-14 --complement &> $STRIPPED
