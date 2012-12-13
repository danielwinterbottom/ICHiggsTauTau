#make -j4


if (( "$#" != "2" ))
then
    echo "<0=et,mt 1=mtmet, 2=em> <0=no tscale, 1=do tscale shifts>"
    exit
fi

OPTION=$1
DOTSCALE=$2

DATASET=Moriond
#DATASET=HCP

CONFIG=scripts/data_"$DATASET"_2012.cfg
echo "Using config: $CONFIG"

FILELIST=filelists/Dec2/Data_53X


if [ $DOTSCALE == 1 ]
then
  TSCALE=(
  '0'
  '1'
  '2'
  )
else
  TSCALE=(
  '0'
  )
fi

echo "Using data filelists: $DATASET"

if [ $OPTION == 0 ]
then

# Data
 echo "Data"
 ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Data_"$DATASET"_et_skim_filelist.dat  --channel=et  --output_name=Data_et_2012.root  >> data_2012_et.log &
 ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Data_"$DATASET"_mt_skim_filelist.dat  --channel=mt  --output_name=Data_mt_2012.root >> data_2012_mt.log &
 wait

# Embedded
for j in "${TSCALE[@]}"
do
  echo "Embedded"
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/Embedded_et_skim_filelist.dat  --channel=et --is_embedded=true --output_name=Embedded_et_2012.root >> data_2012_et.log &
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/Embedded_mt_skim_filelist.dat  --channel=mt --is_embedded=true --output_name=Embedded_mt_2012.root >> data_2012_mt.log &
  wait
done

# Special Mode 3 Data
echo "Special_3_Data"
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Special_2_Data_"$DATASET"_et_skim_filelist.dat --special_mode=3 --channel=et  --output_name=Data_et_2012.root >> data_2012_et.log &
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Special_2_Data_"$DATASET"_mt_skim_filelist.dat --special_mode=3 --channel=mt  --output_name=Data_mt_2012.root >> data_2012_mt.log &
wait

fi



if [ $OPTION == 1 ]
then

# Data
echo "Data"
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Data_"$DATASET"_mtmet_skim_filelist.dat  --channel=mtmet  --output_name=Data_mtmet_2012.root >> data_2012_mtmet.log &
wait

# Special Mode 3 Data
echo "Special_3_Data"
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Special_2_Data_"$DATASET"_mtmet_skim_filelist.dat --special_mode=3 --channel=mtmet --output_name=Data_mtmet_2012.root >> data_2012_mtmet.log &
wait

fi




if [ $OPTION == 2 ]
then

# Data
echo "Data"
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Data_"$DATASET"_em_skim_filelist.dat  --channel=em  --output_name=Data_em_2012.root  >> data_2012_em.log &
wait

# Special Mode 20 Data
echo "Special_20_Data"
echo "Special_21_Data"
echo "Special_22_Data"
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Special_25_Data_"$DATASET"_em_skim_filelist.dat --special_mode=20 --channel=em  --output_name=Data_em_2012.root >> data_2012_em_0.log &
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Special_25_Data_"$DATASET"_em_skim_filelist.dat --special_mode=21 --channel=em  --output_name=Data_em_2012.root >> data_2012_em_1.log &
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Special_25_Data_"$DATASET"_em_skim_filelist.dat --special_mode=22 --channel=em  --output_name=Data_em_2012.root >> data_2012_em_2.log &
wait

# Special Mode 23 Data
echo "Special_23_Data"
echo "Special_24_Data"
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Special_25_Data_"$DATASET"_em_skim_filelist.dat --special_mode=23 --channel=em  --output_name=Data_em_2012.root >> data_2012_em_0.log &
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Special_25_Data_"$DATASET"_em_skim_filelist.dat --special_mode=24 --channel=em  --output_name=Data_em_2012.root >> data_2012_em_1.log &
wait

echo "Embedded"
./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/Embedded_em_skim_filelist.dat  --channel=em --is_embedded=true --output_name=Embedded_em_2012.root >> data_2012_em.log &
wait

fi





