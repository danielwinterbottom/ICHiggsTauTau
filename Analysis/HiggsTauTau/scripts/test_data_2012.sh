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
    CONFIG=scripts/moriond_data_2012.cfg
else
    echo "Process as MSSM..."
    CONFIG=scripts/data_mssm_2012.cfg
fi
echo $CONFIG

FILELIST=filelists/Oct2/Data_53X


# Data
 echo "Data"
 ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Data_electauSkim_IC_filelist.dat  --channel=et --output_folder=./ --svfit_mode=0 --output_name=Data_ElecTau_2012.root
 ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Data_mutauSkim_IC_filelist.dat    --channel=mt --output_folder=./ --svfit_mode=0 --output_name=Data_MuTau_2012.root
 wait

