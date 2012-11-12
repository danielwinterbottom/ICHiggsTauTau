make -j4

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

SOURCE=/Volumes/Storage/Sept11/Data_42X

TSCALE=(
'0'
#'1'
#'2'
)

# Do Data
 echo "Data"
 ./bin/HiggsTauTau --cfg=$CONFIG --filelist=Data/electauSkim/IC_filelist.dat      --mode=0  --output=Data_ElecTau_2011.root
 ./bin/HiggsTauTau --cfg=$CONFIG --filelist=Data/mutauSkim/IC_filelist.dat        --mode=1  --output=Data_MuTau_2011.root
 wait

# Do Embedded
# for j in "${TSCALE[@]}"
# do
#   echo "Embedded"
#   ../bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$SOURCE/Embedded/electauSkim/filelist.dat  --mode=0 --is_embedded=true --output=Embedded_ElecTau_2011.root >> data_2011_ElecTau.log &
#   ../bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$SOURCE/Embedded/mutauSkim/filelist.dat    --mode=1 --is_embedded=true --output=Embedded_MuTau_2011.root >> data_2011_MuTau.log &
#   wait
# done

# # Do Special Mode 3 Data
# echo "Special_3_Data"
# ../bin/HiggsTauTau --cfg=$CONFIG --filelist=$SOURCE/Special_2_Data/electauSkim/filelist.dat --special_mode=3 --mode=0  --output=Data_ElecTau_2011.root >> data_2011_ElecTau.log &
# ../bin/HiggsTauTau --cfg=$CONFIG --filelist=$SOURCE/Special_2_Data/mutauSkim/filelist.dat --special_mode=3 --mode=1  --output=Data_MuTau_2011.root >> data_2011_MuTau.log &
# wait

# # # Do Special Mode 16 Data
# # echo "Special_16_Data"
# # ../bin/HiggsTauTau --cfg=$CONFIG --filelist=$SOURCE/Special_2_Data/electauSkim/filelist.dat --special_mode=16 --mode=0  --output=Data_ElecTau_2011.root >> data_2011_ElecTau.log &
# # ../bin/HiggsTauTau --cfg=$CONFIG --filelist=$SOURCE/Special_2_Data/mutauSkim/filelist.dat --special_mode=16 --mode=1  --output=Data_MuTau_2011.root >> data_2011_MuTau.log &
# # wait

# #Do Special Mode 6 Data
# echo "Special_6_Data"
# ../bin/HiggsTauTau --cfg=$CONFIG --filelist=$SOURCE/Special_2_Data/electauSkim/filelist.dat --special_mode=6 --mode=0  \
# --svfit_override=Special_3_Data_ElecTau_2011.root --output=Data_ElecTau_2011.root >> data_2011_ElecTau.log &
# ../bin/HiggsTauTau --cfg=$CONFIG --filelist=$SOURCE/Special_2_Data/mutauSkim/filelist.dat --special_mode=6 --mode=1  \
# --svfit_override=Special_3_Data_MuTau_2011.root --output=Data_MuTau_2011.root >> data_2011_MuTau.log &
# wait

# # Do Special Mode 7 Data, can use standard SVFit
# echo "Special_7_Data"
# ../bin/HiggsTauTau --cfg=$CONFIG --filelist=$SOURCE/Data/electauSkim/filelist.dat --special_mode=7 --mode=0  \
# --svfit_override=Data_ElecTau_2011.root --output=Data_ElecTau_2011.root >> data_2011_ElecTau.log &
# ../bin/HiggsTauTau --cfg=$CONFIG --filelist=$SOURCE/Data/mutauSkim/filelist.dat --special_mode=7 --mode=1  \
# --svfit_override=Data_MuTau_2011.root --output=Data_MuTau_2011.root >> data_2011_MuTau.log &
# wait

# # Do Special Mode 10 Data, can use standard SVFit
# echo "Special_10_Data"
# ../bin/HiggsTauTau --cfg=$CONFIG --filelist=$SOURCE/Special_2_Data/electauSkim/filelist.dat --special_mode=10 --mode=0  \
# --svfit_override=Special_3_Data_ElecTau_2011.root --output=Data_ElecTau_2011.root >> data_2011_ElecTau.log &
# ../bin/HiggsTauTau --cfg=$CONFIG --filelist=$SOURCE/Special_2_Data/mutauSkim/filelist.dat --special_mode=10 --mode=1  \
# --svfit_override=Special_3_Data_MuTau_2011.root --output=Data_MuTau_2011.root >> data_2011_MuTau.log &
# wait

