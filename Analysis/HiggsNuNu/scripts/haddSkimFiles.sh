#!/bin/sh

SKIMDIR=/vols/ssd00/cms/amagnan/trigskims/Oct21/nunu/MET130/
OUTPUTDIR=/vols/ssd00/cms/amagnan/trigskims/Oct21/nunu/MET130/


FILELISTDIR=/vols/cms04/pjd12/invcmssws/CMSSW_5_3_7/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/filelists/skim/Apr04/nunu/ #/home/hep/magnan/CMS/CMSSW_5_3_11/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/filelists/skim/Apr04/nunu/
mkdir -p $FILELISTDIR

source /vols/sl5_exp_software/cms/slc5_amd64_gcc462/lcg/root/5.34.01-cms9/bin/thisroot.sh

#for iDir in `ls $SKIMDIR`
#  do

#  echo " ---- processing directory $iDir"
#  nFiles=`ls $SKIMDIR/$iDir | wc -l`
#  echo " - Found $nFiles files. Doing hadd"
#  hadd $OUTPUTDIR/$iDir.root $SKIMDIR/$iDir/*.root

#done

cd $SKIMDIR
pwd

for iW in MET-2012A-06Aug2012-v1 MET-2012A-13Jul2012-v1 MET-2012B-13Jul2012-v1 MET-2012C-11Dec2012-v1 MET-2012C-24Aug2012-v1 MET-2012C-PromptReco-v2 MET-2012D-PromptReco-v1
#`ls`
#for iW in W1JetsToLNu W2JetsToLNu WJetsToLNu-v2
#for iW in W3JetsToLNu W4JetsToLNu WJetsToLNu-v1
#for iW in EWK-W2jminus EWK-W2jplus
  do
  rm $iW".root"
done

for iW in `ls`
  do
  ls $iW/enu
  if (( "$?" == "0" )); then
      hadd -O $iW".root" $iW/enu/EventTree*
      if (( "$?" == "0" )); then
	  echo "hadd successful for directory $iW!"
	  echo $iW".root" > $FILELISTDIR/$iW".dat"
      else
	  echo "hadd failed for directory $iW! "
      fi
  else
      hadd -O $iW".root" $iW/EventTree*
      if (( "$?" == "0" )); then
	  echo "hadd successful for directory $iW!"
	  echo $iW".root" > $FILELISTDIR/$iW".dat"
      else
	  echo "hadd failed for directory $iW! "
      fi
  fi
done





#for (( itry=1; itry<=5; itry++ )); do
#	    rm $iW"_"$iF"_1.root"
#	    hadd $iW"_"$iF"_1.root" $iW/$iF/EventTree_1*

#	    if (( "$?" == "0" )); then
#		echo "hadd successful !"
#		let itry=10
#	    else
#		echo "hadd failed! Trying again..."
#	    fi
#	done
#	if (( "$itry" == "6" )); then
#	    echo "hadd failed after $itry tries..."
#	    exit
#	fi
#	mkdir $iW/$iF/tmp/

#	mv $iW/$iF/EventTree_1* $iW/$iF/tmp/
#	for (( itry=1; itry<=5; itry++ )); do
#	    rm $iW"_"$iF"_2.root"
#	    hadd $iW"_"$iF"_2.root" $iW/$iF/EventTree_2*
#	    if (( "$?" == "0" )); then
#		echo "hadd successful !"
#		let itry=10
#	    else
#		echo "hadd failed! Trying again..."
#	    fi
#	done


#scp $SKIMDIR/*.root amagnan@lxplus.cern.ch:/afs/cern.ch/work/a/amagnan/ToCopyToEOS/
