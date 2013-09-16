#!/bin/sh


if (( "$#" != "2" ))
    then
    echo $# $*
    echo "Input parameter needed: <skimdir1> <skimdir2>"
else

    SKIMDIR=$1
    PATHDIR=$2
    
    for iFile in `ls $SKIMDIR/$PATHDIR`
      do
      echo $iFile > tmp
      iFileBase=`sed "s/.root//" tmp`
      echo "File base name = $iFileBase"
      mkdir -p "skim/"$PATHDIR
      echo $iFile > "skim/"$PATHDIR/$iFileBase.dat
    done

fi
