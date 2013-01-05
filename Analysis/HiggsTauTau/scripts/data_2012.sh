: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}
echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT

CONFIG=scripts/Moriond_data_2012.cfg
echo "Config file: $CONFIG"
FILELIST=filelists/Dec30_Data_53X
echo "Filelist prefix: $FILELIST"
DATA_FILELIST=Moriond # Or HCP, DOnly for example
echo "Data filelists with label: $DATA_FILELIST"

if (( "$#" != "2" ))
then
echo "<CHANNELS: 0=et,mt 1=em 2=mtmet> <TAU ENERGY SCALE SHIFT: 0=none 1=none,up,down>"
    exit
fi

OPTION=$1
DOTSCALE=$2

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


if [ $OPTION == 0 ]
then

  # Data
  JOB=Data_et_2012
  $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_Data_"$DATA_FILELIST"_et_skim.dat  --channel=et --output_name=$JOB.root &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

  JOB=Data_mt_2012
  $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_Data_"$DATA_FILELIST"_mt_skim.dat  --channel=mt --output_name=$JOB.root &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

  # Embedded
  for j in "${TSCALE[@]}"
  do
    JOB=Embedded_et_2012
    $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist="$FILELIST"_Embedded_"$DATA_FILELIST"_et_skim.dat --channel=et \
    --is_embedded=true --output_name=$JOB.root &> jobs/$JOB-$j.log" jobs/$JOB-$j.sh
    $JOBSUBMIT jobs/$JOB-$j.sh

    JOB=Embedded_mt_2012
    $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist="$FILELIST"_Embedded_"$DATA_FILELIST"_mt_skim.dat --channel=mt \
    --is_embedded=true --output_name=$JOB.root &> jobs/$JOB-$j.log" jobs/$JOB-$j.sh
    $JOBSUBMIT jobs/$JOB-$j.sh
  done

  # Special Mode 3 Data
  JOB=Data_et_2012
  $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_Special_2_Data_"$DATA_FILELIST"_et_skim.dat  --channel=et \
  --special_mode=3 --output_name=$JOB.root &> jobs/Special_3_$JOB.log" jobs/Special_3_$JOB.sh
  $JOBSUBMIT jobs/Special_3_$JOB.sh

  JOB=Data_mt_2012
  $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_Special_2_Data_"$DATA_FILELIST"_mt_skim.dat  --channel=mt \
  --special_mode=3 --output_name=$JOB.root &> jobs/Special_3_$JOB.log" jobs/Special_3_$JOB.sh
  $JOBSUBMIT jobs/Special_3_$JOB.sh
fi


if [ $OPTION == 1 ]
then
  # Data
  JOB=Data_mtmet_2012
  $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_Data_"$DATA_FILELIST"_mtmet_skim.dat  --channel=mtmet \
  --output_name=$JOB.root &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
  
  # Special Mode 3 Data
  JOB=Data_mtmet_2012
  $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_Special_2_Data_"$DATA_FILELIST"_mtmet_skim.dat  --channel=mtmet \
  --special_mode=3 --output_name=$JOB.root &> jobs/Special_3_$JOB.log" jobs/Special_3_$JOB.sh
  $JOBSUBMIT jobs/Special_3_$JOB.sh
fi


if [ $OPTION == 2 ]
then
  # Data
  JOB=Data_em_2012
  $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_Data_"$DATA_FILELIST"_em_skim.dat  --channel=em --output_name=$JOB.root &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

  # Special Mode Data
  JOB=Data_em_2012
  $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_Special_25_Data_"$DATA_FILELIST"_em_skim.dat  --channel=em \
  --special_mode=20 --output_name=$JOB.root &> jobs/Special_20_$JOB.log" jobs/Special_20_$JOB.sh
  $JOBSUBMIT jobs/Special_20_$JOB.sh

  JOB=Data_em_2012
  $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_Special_25_Data_"$DATA_FILELIST"_em_skim.dat  --channel=em \
  --special_mode=21 --output_name=$JOB.root &> jobs/Special_21_$JOB.log" jobs/Special_21_$JOB.sh
  $JOBSUBMIT jobs/Special_21_$JOB.sh

  JOB=Data_em_2012
  $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_Special_25_Data_"$DATA_FILELIST"_em_skim.dat  --channel=em \
  --special_mode=22 --output_name=$JOB.root &> jobs/Special_22_$JOB.log" jobs/Special_22_$JOB.sh
  $JOBSUBMIT jobs/Special_22_$JOB.sh

  JOB=Data_em_2012
  $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_Special_25_Data_"$DATA_FILELIST"_em_skim.dat  --channel=em \
  --special_mode=23 --output_name=$JOB.root &> jobs/Special_23_$JOB.log" jobs/Special_23_$JOB.sh
  $JOBSUBMIT jobs/Special_23_$JOB.sh

  JOB=Data_em_2012
  $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_Special_25_Data_"$DATA_FILELIST"_em_skim.dat  --channel=em \
  --special_mode=24 --output_name=$JOB.root &> jobs/Special_24_$JOB.log" jobs/Special_24_$JOB.sh
  $JOBSUBMIT jobs/Special_24_$JOB.sh

  # Embedded
  JOB=Embedded_em_2012
  $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_Embedded_"$DATA_FILELIST"_em_skim.dat --channel=em \
  --is_embedded=true --output_name=$JOB.root &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
fi