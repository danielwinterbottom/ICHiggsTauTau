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
    CONFIG=scripts/data_2011.cfg
else
    echo "Process as MSSM..."
    CONFIG=scripts/data_mssm_2011.cfg
fi
echo $CONFIG

FILELIST=filelists/Sept11/Data_42X


# Data
 echo "Data"
 ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Data_electauSkim_IC_filelist.dat  --mode=0 --outfolder=./ --do_svfit=false --output=Data_ElecTau_2011.root
 #./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Data_mutauSkim_IC_filelist.dat    --mode=1 --outfolder=./ --do_svfit=false --output=Data_MuTau_2011.root
 wait

