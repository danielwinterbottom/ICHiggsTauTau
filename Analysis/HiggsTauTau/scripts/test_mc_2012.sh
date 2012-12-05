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
    CONFIG=scripts/moriond_mc_2012.cfg
else
    echo "Process as MSSM..."
    CONFIG=scripts/moriond_mc_2012.cfg
fi
echo $CONFIG

FILELIST=filelists/Oct2/MC_53X
PATHS=(
'VBF_HToTauTau_M-125'
)
 for i in "${PATHS[@]}"
 do

 ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_mutauSkim_IC_filelist.dat --channel=em --output_folder=./ --svfit_mode=0 --output_name=MC_TEST_2012.root

 done
