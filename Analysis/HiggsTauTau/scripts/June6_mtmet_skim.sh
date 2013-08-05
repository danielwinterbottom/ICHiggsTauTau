JOBWRAPPER=./scripts/generate_job.sh
JOBSUBMIT=true
#JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"


CONFIG=scripts/Paper_2012.cfg
echo $CONFIG

#FILELIST=filelists/June6_Data_53X
#SKIMPATH=$SSD/Apr11/Data_53X
#PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/June6/Data_53X/
#
#PATHS=(
#'TauPlusX-2012D-22Jan2013-v1'
#)
#for i in "${PATHS[@]}"
#do
#  echo "$i"
#
#  mkdir -p $SKIMPATH/mtmet_skim/$i/
#  mkdir -p $SKIMPATH/mtmet_skim/Special_2_$i/
#  mkdir -p $SKIMPATH/mtmet_skim/Special_4_$i/
#
#  JOB="$i"_mtmet_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mtmet --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/mtmet_skim/$i/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH mtmet_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#
#  JOB=Special_2_"$i"_mtmet_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mtmet --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/mtmet_skim/Special_2_"$i"/ --special_mode=2 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH mtmet_skim/Special_2_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#  
#  JOB=Special_4_"$i"_mtmet_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mtmet --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/mtmet_skim/Special_4_"$i"/ --special_mode=4 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH mtmet_skim/Special_4_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#done
#
#PATHS=(
#'RecHit-MTau-2012D-22Jan2013-v1-Soft'
#)
#for i in "${PATHS[@]}"
#do
#  echo "$i"
#
#  mkdir -p $SKIMPATH/mtmet_skim/$i/
#
#  JOB="$i"_mtmet_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mtmet --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/mtmet_skim/$i/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH mtmet_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#done
#

FILELIST=filelists/June6_MC_53X
SKIMPATH=$SSD/June6/MC_53X
#PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/rlane/June6/MC_53X/
#
#PATHS=(
#'DYJetsToLL'
#'DY1JetsToLL'
#'DY2JetsToLL'
#'DY3JetsToLL'
#'DY4JetsToLL'
#'WJetsToLNu-v1'
#'WJetsToLNu-v2'
#'W1JetsToLNu'
#'W2JetsToLNu'
#'W3JetsToLNu'
#'W4JetsToLNu'
#'TTJets'
#'TT-v1'
#'TT-v2'
#'T-tW'
#'Tbar-tW'
#'WWJetsTo2L2Nu'
#'WZJetsTo2L2Q'
#'WZJetsTo3LNu'
#'ZZJetsTo2L2Nu'
#'ZZJetsTo2L2Q'
#'ZZJetsTo4L'
#'TTJetsHadronicExt'
#'TTJetsSemiLept'
#'TTJetsFullLept'
#'W1JetsToLNuExt'
#'W2JetsToLNuExt'
#)
#for i in "${PATHS[@]}"
#do
#  echo "$i"
#
#  mkdir -p $SKIMPATH/mtmet_skim/$i
#  
#  JOB="$i"_mtmet_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mtmet --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/mtmet_skim/$i/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH mtmet_skim/$i "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#done
#
#PATHS=(
#'WJetsToLNu-v1'
#'WJetsToLNu-v2'
#'W1JetsToLNu'
#'W2JetsToLNu'
#'W3JetsToLNu'
#'W4JetsToLNu'
#'W1JetsToLNuExt'
#'W2JetsToLNuExt'
#)
#for i in "${PATHS[@]}"
#do
#  echo "$i"
#  mkdir -p $SKIMPATH/mtmet_skim/Special_5_$i/
#
#  JOB=Special_5_"$i"_mtmet_skim
#  $JOBWRAPPER "./bin/HiggsTauTau --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mtmet --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/mtmet_skim/Special_5_"$i"/ --special_mode=5 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH mtmet_skim/Special_5_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#done

JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepshort.q"
 PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/June6/MC_53X/
 PATHS=(
# 'GluGluToHToTauTau_M-90' 
# 'GluGluToHToTauTau_M-95' 
# 'GluGluToHToTauTau_M-100' 
# 'GluGluToHToTauTau_M-105' 
# 'GluGluToHToTauTau_M-110' 
# 'GluGluToHToTauTau_M-115' 
# 'GluGluToHToTauTau_M-120' 
# 'GluGluToHToTauTau_M-125'
# 'GluGluToHToTauTau_M-130' 
# 'GluGluToHToTauTau_M-135' 
# 'GluGluToHToTauTau_M-140' 
# 'GluGluToHToTauTau_M-145' 
# 'GluGluToHToTauTau_M-150' 
# 'GluGluToHToTauTau_M-155' 
# 'GluGluToHToTauTau_M-160' 
# 'VBF_HToTauTau_M-90' 
# 'VBF_HToTauTau_M-95' 
# 'VBF_HToTauTau_M-100' 
# 'VBF_HToTauTau_M-105' 
# 'VBF_HToTauTau_M-110' 
# 'VBF_HToTauTau_M-115' 
# 'VBF_HToTauTau_M-120' 
# 'VBF_HToTauTau_M-125'
# 'VBF_HToTauTau_M-130' 
# 'VBF_HToTauTau_M-135' 
# 'VBF_HToTauTau_M-140' 
# 'VBF_HToTauTau_M-145' 
# 'VBF_HToTauTau_M-150' 
# 'VBF_HToTauTau_M-155' 
# 'VBF_HToTauTau_M-160' 
# 'WH_ZH_TTH_HToTauTau_M-90' 
# 'WH_ZH_TTH_HToTauTau_M-95' 
# 'WH_ZH_TTH_HToTauTau_M-100' 
# 'WH_ZH_TTH_HToTauTau_M-105' 
# 'WH_ZH_TTH_HToTauTau_M-110' 
# 'WH_ZH_TTH_HToTauTau_M-115' 
# 'WH_ZH_TTH_HToTauTau_M-120' 
# 'WH_ZH_TTH_HToTauTau_M-125'
# 'WH_ZH_TTH_HToTauTau_M-130' 
# 'WH_ZH_TTH_HToTauTau_M-135' 
# 'WH_ZH_TTH_HToTauTau_M-140' 
# 'WH_ZH_TTH_HToTauTau_M-145'
# 'WH_ZH_TTH_HToTauTau_M-150' 
# 'WH_ZH_TTH_HToTauTau_M-155' 
# 'WH_ZH_TTH_HToTauTau_M-160' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-90' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-95' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-100' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-105' 
  'GluGluToHToWWTo2LAndTau2Nu_M-110' 
  'GluGluToHToWWTo2LAndTau2Nu_M-115' 
  'GluGluToHToWWTo2LAndTau2Nu_M-120' 
  'GluGluToHToWWTo2LAndTau2Nu_M-125'
  'GluGluToHToWWTo2LAndTau2Nu_M-130' 
  'GluGluToHToWWTo2LAndTau2Nu_M-135' 
  'GluGluToHToWWTo2LAndTau2Nu_M-140' 
  'GluGluToHToWWTo2LAndTau2Nu_M-145' 
  'GluGluToHToWWTo2LAndTau2Nu_M-150' 
  'GluGluToHToWWTo2LAndTau2Nu_M-155' 
  'GluGluToHToWWTo2LAndTau2Nu_M-160' 
#  'VBF_HToWWTo2LAndTau2Nu_M-90' 
#  'VBF_HToWWTo2LAndTau2Nu_M-95' 
#  'VBF_HToWWTo2LAndTau2Nu_M-100' 
#  'VBF_HToWWTo2LAndTau2Nu_M-105' 
  'VBF_HToWWTo2LAndTau2Nu_M-110' 
  'VBF_HToWWTo2LAndTau2Nu_M-115' 
  'VBF_HToWWTo2LAndTau2Nu_M-120' 
  'VBF_HToWWTo2LAndTau2Nu_M-125'
  'VBF_HToWWTo2LAndTau2Nu_M-130' 
  'VBF_HToWWTo2LAndTau2Nu_M-135' 
  'VBF_HToWWTo2LAndTau2Nu_M-140' 
  'VBF_HToWWTo2LAndTau2Nu_M-145' 
  'VBF_HToWWTo2LAndTau2Nu_M-150' 
  'VBF_HToWWTo2LAndTau2Nu_M-155' 
  'VBF_HToWWTo2LAndTau2Nu_M-160' 
 )

 for i in "${PATHS[@]}"
do
  echo "$i"

  mkdir -p $SKIMPATH/mtmet_skim/$i
  
  JOB="$i"_mtmet_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mtmet --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/mtmet_skim/$i/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH mtmet_skim/$i "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
done
