#!/bin/sh

if (( "$#" != "4" ))
    then
    echo $# $*
    echo "Input parameter needed: <userdir> <basedir1> <basedir2> <basedir3> "
else
    USERDIR=$1
    BASEDIR=$2
    VERSIONDIR=$3
    CRABDIR=$4
    
    OUTFILE=$BASEDIR"_"$VERSIONDIR"_"$CRABDIR".dat"
    rm $OUTFILE

    MYOFFSET=0
    MYCOUNT=500

    while [ "$MYCOUNT" == "500" ]
      do
      srmls -count=500 -offset=$MYOFFSET srm://gfe02.grid.hep.ph.ic.ac.uk:8443/srm/managerv2?SFN=/pnfs/hep.ph.ic.ac.uk/data/cms/store/user/$USERDIR/$BASEDIR/$VERSIONDIR/$CRABDIR > tmp1.out
      
      let MYCOUNT=`grep EventTree tmp1.out | wc -l`
      
      echo " - Retrieved " `expr $MYCOUNT + $MYOFFSET` " files from SE."
      
      let MYOFFSET=$MYOFFSET+$MYCOUNT
      
      awk '{print $2}' tmp1.out | grep "EventTree" > tmp2.out 
      sed "s/\/pnfs\/hep.ph.ic.ac.uk\/data\/cms\/store\/user\/amagnan\/$BASEDIR\/$VERSIONDIR\///" tmp2.out >> $OUTFILE
      rm tmp1.out tmp2.out


    done

fi
