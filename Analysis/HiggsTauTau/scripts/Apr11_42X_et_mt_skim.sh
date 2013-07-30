JOBWRAPPER=./scripts/generate_job.sh
JOBSUBMIT=true
JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepshort.q"


CONFIG=scripts/Paper_2011.cfg
echo $CONFIG

#FILELIST=filelists/Apr11_Data_42X
#SKIMPATH=$SSD/Apr11/Data_42X
#PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/Apr11/Data_42X/
#
#PATHS=(
#   'TauPlusX-2011A-May10ReReco-v1'
#   'TauPlusX-2011A-PromptReco-v4'
#   'TauPlusX-2011A-05Aug2011-v1'
#   'TauPlusX-2011A-03Oct2011-v1'
#   'TauPlusX-2011B-PromptReco-v1'
#)
#for i in "${PATHS[@]}"
#do
#  echo "$i"
#
#  mkdir -p $SKIMPATH/et_skim/$i/
#  mkdir -p $SKIMPATH/mt_skim/$i/
#  mkdir -p $SKIMPATH/et_skim/Special_2_$i/
#  mkdir -p $SKIMPATH/mt_skim/Special_2_$i/
#  mkdir -p $SKIMPATH/et_skim/Special_4_$i/
#  mkdir -p $SKIMPATH/mt_skim/Special_4_$i/
#
#  JOB="$i"_et_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=et --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/et_skim/$i/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH et_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#
#  JOB="$i"_mt_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/mt_skim/$i/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH mt_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#
#  JOB=Special_2_"$i"_et_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=et --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/et_skim/Special_2_"$i"/ --special_mode=2 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH et_skim/Special_2_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#
#  JOB=Special_2_"$i"_mt_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/mt_skim/Special_2_"$i"/ --special_mode=2 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH mt_skim/Special_2_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#  
#  JOB=Special_4_"$i"_et_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=et --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/et_skim/Special_4_"$i"/ --special_mode=4 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH et_skim/Special_4_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#
#  JOB=Special_4_"$i"_mt_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/mt_skim/Special_4_"$i"/ --special_mode=4 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH mt_skim/Special_4_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#done
#
#
#
#PATHS=(
#   'Embedded-ETau-2011A-May10ReReco-v1'
#   'Embedded-ETau-2011A-PromptReco-v4'
#   'Embedded-ETau-2011A-05Aug2011-v1'
#   'Embedded-ETau-2011A-03Oct2011-v1'
#   'Embedded-ETau-2011B-PromptReco-v1'
#)
#for i in "${PATHS[@]}"
#do
#  echo "$i"
#
#  mkdir -p $SKIMPATH/et_skim/$i/
#
#  JOB="$i"_et_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=et --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/et_skim/$i/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH et_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#done
#
#
#
#PATHS=(
#   'Embedded-MTau-2011A-May10ReReco-v1'
#   'Embedded-MTau-2011A-PromptReco-v4'
#   'Embedded-MTau-2011A-05Aug2011-v1'
#   'Embedded-MTau-2011A-03Oct2011-v1'
#   'Embedded-MTau-2011B-PromptReco-v1'
#)
#for i in "${PATHS[@]}"
#do
#  echo "$i"
#
#  mkdir -p $SKIMPATH/mt_skim/$i/
#
#  JOB="$i"_mt_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/mt_skim/$i/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH mt_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#done


FILELIST=filelists/Apr11_MC_42X
SKIMPATH=$SSD/Apr11/MC_42X
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/Apr11/MC_42X/

PATHS=(
#'DYJetsToLL'
#'WJetsToLNu'
#'W1JetsToLNu'
#'W2JetsToLNu'
#'W3JetsToLNu'
#'W4JetsToLNu'
#'TTJets-v1'
#'TTJets-v2'
#'T-tW'
#'Tbar-tW'
#'WWJetsTo2L2Nu'
#'WZJetsTo2L2Q'
#'WZJetsTo3LNu'
#'ZZJetsTo2L2Nu'
#'ZZJetsTo2L2Q'
#'ZZJetsTo4L'
#'GluGluToHToTauTau_M-90' 
#'GluGluToHToTauTau_M-95' 
#'GluGluToHToTauTau_M-100' 
#'GluGluToHToTauTau_M-105' 
#'GluGluToHToTauTau_M-110' 
#'GluGluToHToTauTau_M-115' 
#'GluGluToHToTauTau_M-120' 
#'GluGluToHToTauTau_M-125'
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
#'VBF_HToTauTau_M-125'
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
#'WH_ZH_TTH_HToTauTau_M-125'
#'WH_ZH_TTH_HToTauTau_M-130' 
#'WH_ZH_TTH_HToTauTau_M-135' 
#'WH_ZH_TTH_HToTauTau_M-140' 
#'WH_ZH_TTH_HToTauTau_M-145'
#'WH_ZH_TTH_HToTauTau_M-150' 
#'WH_ZH_TTH_HToTauTau_M-155' 
#'WH_ZH_TTH_HToTauTau_M-160' 
#'SUSYGluGluToHToTauTau_M-90'
#'SUSYGluGluToHToTauTau_M-100'
#'SUSYGluGluToHToTauTau_M-120'
#'SUSYGluGluToHToTauTau_M-130'
#'SUSYGluGluToHToTauTau_M-140'
#'SUSYGluGluToHToTauTau_M-160'
#'SUSYGluGluToHToTauTau_M-180'
#'SUSYGluGluToHToTauTau_M-200'
#'SUSYGluGluToHToTauTau_M-250'
#'SUSYGluGluToHToTauTau_M-300'
#'SUSYGluGluToHToTauTau_M-350'
#'SUSYGluGluToHToTauTau_M-400'
#'SUSYGluGluToHToTauTau_M-450'
#'SUSYGluGluToHToTauTau_M-500'
#'SUSYGluGluToHToTauTau_M-600'
#'SUSYGluGluToHToTauTau_M-700'
#'SUSYGluGluToHToTauTau_M-800'
#'SUSYGluGluToHToTauTau_M-900'
#'SUSYGluGluToHToTauTau_M-1000'
#'SUSYBBHToTauTau_M-90'
#'SUSYBBHToTauTau_M-100'
#'SUSYBBHToTauTau_M-120'
#'SUSYBBHToTauTau_M-130'
#'SUSYBBHToTauTau_M-140'
#'SUSYBBHToTauTau_M-160'
#'SUSYBBHToTauTau_M-180'
#'SUSYBBHToTauTau_M-200'
#'SUSYBBHToTauTau_M-250'
#'SUSYBBHToTauTau_M-300'
#'SUSYBBHToTauTau_M-350'
#'SUSYBBHToTauTau_M-400'
#'SUSYBBHToTauTau_M-450'
#'SUSYBBHToTauTau_M-500'
#'SUSYBBHToTauTau_M-600'
#'SUSYBBHToTauTau_M-700'
#'SUSYBBHToTauTau_M-800'
#'SUSYBBHToTauTau_M-900'
#'SUSYBBHToTauTau_M-1000'
 'GluGluToHToWWTo2LAndTau2Nu_M-110'
 'VBF_HToWWTo2LAndTau2Nu_M-110'
 'GluGluToHToWWTo2LAndTau2Nu_M-115'
 'VBF_HToWWTo2LAndTau2Nu_M-115'
 'GluGluToHToWWTo2L2Nu_M-120'
 'GluGluToHToWWTo2Tau2Nu_M-120'
 'GluGluToHToWWToLNuTauNu_M-120'
 'VBF_HToWWTo2L2Nu_M-120'
 'VBF_HToWWTo2Tau2Nu_M-120'
 'VBF_HToWWToLNuTauNu_M-120'
 'GluGluToHToWWTo2LAndTau2Nu_M-125'
 'VBF_HToWWTo2LAndTau2Nu_M-125'
 'GluGluToHToWWTo2L2Nu_M-130'
 'GluGluToHToWWTo2Tau2Nu_M-130'
 'GluGluToHToWWToLNuTauNu_M-130'
 'VBF_HToWWTo2L2Nu_M-130'
 'VBF_HToWWTo2Tau2Nu_M-130'
 'VBF_HToWWToLNuTauNu_M-130'
 '/GluGluToHToWWTo2LAndTau2Nu_M-135'
 'VBF_HToWWTo2LAndTau2Nu_M-135'
 'GluGluToHToWWTo2L2Nu_M-140'
 'GluGluToHToWWTo2Tau2Nu_M-140'
 'GluGluToHToWWToLNuTauNu_M-140'
 'VBF_HToWWTo2L2Nu_M-140'
 'VBF_HToWWTo2Tau2Nu_M-140'
 'VBF_HToWWToLNuTauNu_M-140'
 'GluGluToHToWWTo2L2Nu_M-150'
 'GluGluToHToWWTo2Tau2Nu_M-150'
 'GluGluToHToWWToLNuTauNu_M-150'
 'VBF_HToWWTo2L2Nu_M-150'
 'VBF_HToWWTo2Tau2Nu_M-150'
 'VBF_HToWWToLNuTauNu_M-150'
 'GluGluToHToWWTo2L2Nu_M-160'
 'GluGluToHToWWTo2Tau2Nu_M-160'
 'GluGluToHToWWToLNuTauNu_M-160'
 'VBF_HToWWTo2L2Nu_M-160'
 'VBF_HToWWTo2Tau2Nu_M-160'
 'VBF_HToWWToLNuTauNu_M-160'
)
for i in "${PATHS[@]}"
do
  echo "$i"

  mkdir -p $SKIMPATH/et_skim/$i
  mkdir -p $SKIMPATH/mt_skim/$i

  JOB="$i"_et_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=et --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/et_skim/$i/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH et_skim/$i "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

  JOB="$i"_mt_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/mt_skim/$i/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH mt_skim/$i "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
done

#PATHS=(
# 'WJetsToLNu'
# 'W1JetsToLNu'
# 'W2JetsToLNu'
# 'W3JetsToLNu'
# 'W4JetsToLNu'
#)
#for i in "${PATHS[@]}"
#do
#  mkdir -p $SKIMPATH/et_skim/Special_5_$i/
#  mkdir -p $SKIMPATH/mt_skim/Special_5_$i/
#  echo "$i"
#  
#  JOB=Special_5_"$i"_et_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=et --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/et_skim/Special_5_"$i"/ --special_mode=5 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH et_skim/Special_5_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#
#  JOB=Special_5_"$i"_mt_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/mt_skim/Special_5_"$i"/ --special_mode=5 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH mt_skim/Special_5_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#done
