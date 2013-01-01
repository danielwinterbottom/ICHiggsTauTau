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
    CONFIG=scripts/mc_Moriond_2012.cfg
else
    echo "Process as MSSM..."
    CONFIG=scripts/moriond_mc_2012.cfg
fi
echo $CONFIG

FILELIST=filelists/Dec26/MC_53X
PATHS=(
'VBF_HToTauTau_M-125'
)
 for i in "${PATHS[@]}"
 do

   ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_mt_skim_filelist.dat --channel=mt --output_folder=./ --svfit_mode=0 --output_name="$i"_mt_2012.root --make_sync_ntuple=true --do_skim=false --skim_path=/vols/cms04/ajg06/Dec26/MC_53X/VBF_HToTauTau_M-125/mt_skim/
 #./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_mutauSkim_IC_filelist.dat --channel=em --output_folder=./ --svfit_mode=0 --output_name=MC_TEST_2012.root
 #./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_mutauSkim_IC_filelist.dat --channel=em --output_folder=./ --svfit_mode=0 --output_name=MC_TEST_2012.root

 done
