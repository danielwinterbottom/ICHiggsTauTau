JOBWRAPPER=./scripts/generate_job.sh
#JOBSUBMIT=true
JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"


CONFIG=scripts/Paper_2012.cfg
echo $CONFIG

FILELIST=filelists/Feb20_Data_53X
SKIMPATH=$SSD/Feb20/Data_53X
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/Feb20/Data_53X/

PATHS=(
'TauPlusX-2012D-PromptReco-v1'
)
for i in "${PATHS[@]}"
do
  echo "$i"

  mkdir -p $SKIMPATH/$i/etmet_skim/
  mkdir -p $SKIMPATH/$i/mtmet_skim/
  mkdir -p $SKIMPATH/Special_2_$i/etmet_skim/
  mkdir -p $SKIMPATH/Special_2_$i/mtmet_skim/

  JOB="$i"_etmet_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=etmet --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/$i/etmet_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/etmet_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

  JOB="$i"_mtmet_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mtmet --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/$i/mtmet_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/mtmet_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

  JOB=Special_2_"$i"_etmet_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=etmet --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/Special_2_"$i"/etmet_skim/ --special_mode=2 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH Special_2_"$i"/etmet_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

  JOB=Special_2_"$i"_mtmet_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mtmet --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/Special_2_"$i"/mtmet_skim/ --special_mode=2 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH Special_2_"$i"/mtmet_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
done



PATHS=(
'Embedded-ETau-2012D-PromptReco-v1'
)
for i in "${PATHS[@]}"
do
  echo "$i"

  mkdir -p $SKIMPATH/$i/etmet_skim/

  JOB="$i"_etmet_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=etmet --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/$i/etmet_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/etmet_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
done



PATHS=(
'Embedded-MTau-2012D-PromptReco-v1'
)
for i in "${PATHS[@]}"
do
  echo "$i"

  mkdir -p $SKIMPATH/$i/mtmet_skim/

  JOB="$i"_mtmet_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mtmet --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/$i/mtmet_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/mtmet_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
done


FILELIST=filelists/Feb20_MC_53X
SKIMPATH=$SSD/Feb20/MC_53X
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/rlane/Feb20/MC_53X/

PATHS=(
'DYJetsToLL'
'DY1JetsToLL'
'DY2JetsToLL'
'DY3JetsToLL'
'DY4JetsToLL'
'WJetsToLNu-v1'
'WJetsToLNu-v2'
'W1JetsToLNu'
'W2JetsToLNu'
'W3JetsToLNu'
'W4JetsToLNu'
'TTJets'
'TT-v1'
'TT-v2'
'T-tW'
'Tbar-tW'
'WWJetsTo2L2Nu'
'WZJetsTo2L2Q'
'WZJetsTo3LNu'
'ZZJetsTo2L2Nu'
'ZZJetsTo2L2Q'
'ZZJetsTo4L'
'GluGluToHToTauTau_M-125'
'VBF_HToTauTau_M-125'
'WH_ZH_TTH_HToTauTau_M-125'
'SUSYGluGluToHToTauTau_M-160'
'SUSYBBHToTauTau_M-160'
#'GluGluToHToTauTau_M-90' 
#'GluGluToHToTauTau_M-95' 
#'GluGluToHToTauTau_M-100' 
#'GluGluToHToTauTau_M-105' 
#'GluGluToHToTauTau_M-110' 
#'GluGluToHToTauTau_M-115' 
#'GluGluToHToTauTau_M-120' 
#'GluGluToHToTauTau_M-130' 
#'GluGluToHToTauTau_M-135' 
#'GluGluToHToTauTau_M-140' 
#'GluGluToHToTauTau_M-145' 
#'GluGluToHToTauTau_M-150' 
#'GluGluToHToTauTau_M-155' 
#'GluGluToHToTauTau_M-160' 
#'VBF_HToTauTau_M-90' 
#'VBF_HToTauTau_M-95' 
#'VBF_HToTauTau_M-100' 
#'VBF_HToTauTau_M-105' 
#'VBF_HToTauTau_M-110' 
#'VBF_HToTauTau_M-115' 
#'VBF_HToTauTau_M-120' 
#'VBF_HToTauTau_M-130' 
#'VBF_HToTauTau_M-135' 
#'VBF_HToTauTau_M-140' 
#'VBF_HToTauTau_M-145' 
#'VBF_HToTauTau_M-150' 
#'VBF_HToTauTau_M-155' 
#'VBF_HToTauTau_M-160' 
#'WH_ZH_TTH_HToTauTau_M-90' 
#'WH_ZH_TTH_HToTauTau_M-95' 
#'WH_ZH_TTH_HToTauTau_M-100' 
#'WH_ZH_TTH_HToTauTau_M-105' 
#'WH_ZH_TTH_HToTauTau_M-110' 
#'WH_ZH_TTH_HToTauTau_M-115' 
#'WH_ZH_TTH_HToTauTau_M-120' 
#'WH_ZH_TTH_HToTauTau_M-130' 
#'WH_ZH_TTH_HToTauTau_M-135' 
#'WH_ZH_TTH_HToTauTau_M-140' 
#'WH_ZH_TTH_HToTauTau_M-145'
#'WH_ZH_TTH_HToTauTau_M-150' 
#'WH_ZH_TTH_HToTauTau_M-155' 
#'WH_ZH_TTH_HToTauTau_M-160' 
)
for i in "${PATHS[@]}"
do
  echo "$i"

  mkdir -p $SKIMPATH/$i/etmet_skim/
  mkdir -p $SKIMPATH/$i/mtmet_skim/

  JOB="$i"_etmet_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=etmet --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/$i/etmet_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/etmet_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

  JOB="$i"_mtmet_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mtmet --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/$i/mtmet_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/mtmet_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
done

