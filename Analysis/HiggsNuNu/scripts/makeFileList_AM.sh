#!/bin/sh

if [ "$#" -neq 3 ]
then
echo $*
echo "Input parameter needed: <path to srm rootdir> <multicrab dir> "
else
BASEDIR=$1
CRABDIR=$2
srmls srm://gfe02.grid.hep.ph.ic.ac.uk:8443/srm/managerv2?SFN=/pnfs/hep.ph.ic.ac.uk/data/cms/store/user/amagnan/$BASEDIR/$CRABDIR/ > tmp.out
awk '{print $2}' tmp.out | sed "s/\/pnfs\/hep.ph.ic.ac.uk\/data\/cms\/store\/user\/amagnan\/$BASEDIR\/$VERSION\///" > filelist.dat
rm tmp.out
fi