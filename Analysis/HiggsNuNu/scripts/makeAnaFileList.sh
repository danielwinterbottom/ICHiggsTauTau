#!/bin/sh                                                                                                                                             

if (( "$#" != "2" ))
    then
    echo $# $*
    echo "Input parameter needed: <inputdir> <outputfile>"
    exit
fi

MYDIR=$1
OUTPUTFILE=$2
ls $MYDIR | awk '{$2="XXX"}{$3=$1}{print $0}' > $OUTPUTFILE

sed 's/MC\_//' $OUTPUTFILE > tmp
sed 's/\.root//' tmp > $OUTPUTFILE


