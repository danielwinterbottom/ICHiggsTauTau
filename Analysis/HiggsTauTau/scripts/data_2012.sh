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
    CONFIG=scripts/data_2012.cfg
else
    echo "Process as MSSM..."
    CONFIG=scripts/data_mssm_2012.cfg
fi
echo $CONFIG

SOURCE=/Volumes/Storage/Oct2/Data_53X
FILELIST=filelists/Oct2/Data_53X

TSCALE=(
'0'
'1'
'2'
)

# Data
 echo "Data"
 ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Data_electauSkim_IC_filelist.dat  --mode=0  --output=Data_ElecTau_2012.root  >> data_2012_ElecTau.log &
 ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Data_mutauSkim_IC_filelist.dat    --mode=1  --output=Data_MuTau_2012.root >> data_2012_MuTau.log &
 wait

# Embedded
for j in "${TSCALE[@]}"
do
  echo "Embedded"
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/Embedded_electauSkim_IC_filelist.dat  --mode=0 --is_embedded=true --output=Embedded_ElecTau_2012.root >> data_2012_ElecTau.log &
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/Embedded_mutauSkim_IC_filelist.dat    --mode=1 --is_embedded=true --output=Embedded_MuTau_2012.root >> data_2012_MuTau.log &
  wait
done

# Special Mode 3 Data
echo "Special_3_Data"
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Special_2_Data_electauSkim_IC_filelist.dat --special_mode=3 --mode=0  --output=Data_ElecTau_2012.root >> data_2012_ElecTau.log &
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Special_2_Data_mutauSkim_IC_filelist.dat --special_mode=3 --mode=1  --output=Data_MuTau_2012.root >> data_2012_MuTau.log &
wait


#Do Special Mode 6 Data
echo "Special_6_Data"
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Special_2_Data_electauSkim_IC_filelist.dat --special_mode=6 --mode=0  \
--svfit_override=Special_3_Data_ElecTau_2012.root --output=Data_ElecTau_2012.root >> data_2012_ElecTau.log &
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Special_2_Data_mutauSkim_IC_filelist.dat --special_mode=6 --mode=1  \
--svfit_override=Special_3_Data_MuTau_2012.root --output=Data_MuTau_2012.root >> data_2012_MuTau.log &
wait

# Special Mode 7 Data, can use standard SVFit
echo "Special_7_Data"
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Data_electauSkim_IC_filelist.dat --special_mode=7 --mode=0  \
--svfit_override=Data_ElecTau_2012.root --output=Data_ElecTau_2012.root >> data_2012_ElecTau.log &
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Data_mutauSkim_IC_filelist.dat --special_mode=7 --mode=1  \
--svfit_override=Data_MuTau_2012.root --output=Data_MuTau_2012.root >> data_2012_MuTau.log &
wait


