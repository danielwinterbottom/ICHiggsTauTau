#!/bin/sh

if (( "$#" != "3" ))
then
echo $# $*
echo "Input parameter needed: <basedir1> <basedir2> <basedir3> "
else
BASEDIR=$1
VERSIONDIR=$2
CRABDIR=$3
srmls srm://gfe02.grid.hep.ph.ic.ac.uk:8443/srm/managerv2?SFN=/pnfs/hep.ph.ic.ac.uk/data/cms/store/user/amagnan/$BASEDIR/$VERSIONDIR/$CRABDIR > tmp1.out
awk '{print $2}' tmp1.out | grep "EventTree" > tmp2.out 
OUTFILE=$BASEDIR"_"$VERSIONDIR"_"$CRABDIR".dat"
sed "s/\/pnfs\/hep.ph.ic.ac.uk\/data\/cms\/store\/user\/amagnan\/$BASEDIR\/$VERSIONDIR\///" tmp2.out > filelists/$OUTFILE
rm tmp1.out tmp2.out
fi