#make -j4

MSSM_MODE=$1

if [ -z $1 ]
then
    echo "No mode specified!"
    exit
fi

if [ $MSSM_MODE == 0 ]
then
    echo "Process as SM..."
    CONFIG=scripts/mc_2012.cfg
else
    echo "Process as MSSM..."
    CONFIG=scripts/data_mssm_2012.cfg
fi
echo $CONFIG

FILELIST=filelists/Oct2/MC_53X
PATHS=(
'VBF_HToTauTau_M-125'
)
 for i in "${PATHS[@]}"
 do

 ./bin/HiggsTauTau --cfg=$CONFIG --max_events=-1 --filelist=$FILELIST/"$i"_mutauSkim_IC_filelist.dat --mode=1 --outfolder=./ --do_svfit=false --output=MC_TEST_2012.root

 done
