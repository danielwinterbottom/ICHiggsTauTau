if [ -z $3 ]
then
    echo "Must specify <SKIM PATH> <SAMPLE> <FILELIST>"
    exit
fi

set -e


XSKIMPATH=$1
XSAMPLE=$2
XFILELIST=$3
XSTARTDIR=$PWD

#source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc46-opt/root/bin/thisroot.sh
source /vols/sl5_exp_software/cms/slc5_amd64_gcc462/lcg/root/5.34.01-cms9/bin/thisroot.sh
cd $XSKIMPATH/$XSAMPLE
hadd -O TempTree.root EventTree*
rm EventTree*.root
mv TempTree.root EventTree_All.root
cd $XSKIMPATH
find $XSAMPLE/EventTree*.root &> $XSTARTDIR/$XFILELIST
cd $STARTDIR
