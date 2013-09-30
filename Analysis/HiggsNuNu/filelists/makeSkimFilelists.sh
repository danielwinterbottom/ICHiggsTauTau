#!/bin/sh


if (( "$#" != "2" ))
    then
    echo $# $*
    echo "Input parameter needed: <skimdir1> <skimdir2>"
else

    SKIMDIR=$1
    PATHDIR=$2
    
    BASEDIR=`pwd`

    cd $SKIMDIR/$PATHDIR

    for iFile in `ls *.root`
      do
      echo $iFile > tmp
      iFileBase=`sed "s/.root//" tmp`
      echo "File base name = $iFileBase"

      mkdir -p $BASEDIR"/skim/"$PATHDIR
      echo $iFile > $BASEDIR"/skim/"$PATHDIR/$iFileBase.dat
    done

fi
