JOBWRAPPER=./scripts/generate_job.sh
JOBSUBMIT=true
#JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"


CONFIG=scripts/Paper_2012.cfg
echo $CONFIG

FILELIST=filelists/June6_MC_53X
SKIMPATH=$SSD/June6/MC_53X
#PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/June6/Data_53X/

#PATHS=(
#'MuEG-2012A-22Jan2013-v1'
#'MuEG-2012B-22Jan2013-v1'
#'MuEG-2012C-22Jan2013-v1'
#'MuEG-2012D-22Jan2013-v1'
#)
#for i in "${PATHS[@]}"
#do
#  echo "$i"
#
#  mkdir -p $SKIMPATH/em_skim/$i/
#  mkdir -p $SKIMPATH/em_skim/Special_20_$i/
#  mkdir -p $SKIMPATH/em_skim/Special_21_$i/
#  mkdir -p $SKIMPATH/em_skim/Special_22_$i/
#  mkdir -p $SKIMPATH/em_skim/Special_23_$i/
#  mkdir -p $SKIMPATH/em_skim/Special_24_$i/

#  JOB="$i"_em_skim
#  $JOBWRAPPER "./bin/HiggsHTohh --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=em --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/em_skim/"$i"/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH em_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh

#  JOB=Special_23_"$i"_em_skim
#  $JOBWRAPPER "./bin/HiggsHTohh --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=em --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/em_skim/Special_23_"$i"/ --special_mode=23 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH em_skim/Special_23_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
  
#  JOB=Special_24_"$i"_em_skim
#  $JOBWRAPPER "./bin/HiggsHTohh --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=em --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/em_skim/Special_24_"$i"/ --special_mode=24 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH em_skim/Special_24_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh

#  JOB=Special_20_"$i"_em_skim
#  $JOBWRAPPER "./bin/HiggsHTohh --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=em --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/em_skim/Special_20_"$i"/ --special_mode=20 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH em_skim/Special_20_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh

#  JOB=Special_21_"$i"_em_skim
#  $JOBWRAPPER "./bin/HiggsHTohh --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=em --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/em_skim/Special_21_"$i"/ --special_mode=21 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH em_skim/Special_21_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh

#  JOB=Special_22_"$i"_em_skim
#  $JOBWRAPPER "./bin/HiggsHTohh --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=em --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/em_skim/Special_22_"$i"/ --special_mode=22 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH em_skim/Special_22_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#done

#PATHS=(
#'RecHit-EM-2012A-22Jan2013-v1'
#'RecHit-EM-2012B-22Jan2013-v1'
#'RecHit-EM-2012C-22Jan2013-v1'
#'RecHit-EM-2012D-22Jan2013-v1'
#)
#for i in "${PATHS[@]}"
#do
#  echo "$i"

#  mkdir -p $SKIMPATH/em_skim/$i/

#  JOB="$i"_em_skim
#  $JOBWRAPPER "./bin/HiggsHTohh --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=em --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/em_skim/"$i"/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH em_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#done


#FILELIST=filelists/June6_MC_53X
#SKIMPATH=$SSD/June6/MC_53X

#PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/June6/MC_53X/
#PATHS=(
#'RecHit-EM-TTJets_FullLeptMGDecays'
#'GluGluToHToWWTo2LAndTau2Nu_M-125'
#'VBF_HToWWTo2LAndTau2Nu_M-125'
# )
# for i in "${PATHS[@]}"
# do
#   echo "$i"

#   mkdir -p $SKIMPATH/em_skim/$i/

#   JOB="$i"_em_skim
#   $JOBWRAPPER "./bin/HiggsHTohh --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=em --output_folder=./ --output_name=Dummy_$JOB.root \
#   --do_skim=true --skim_path=$SKIMPATH/em_skim/$i/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#   ./scripts/hadd_and_filelist.sh $SKIMPATH em_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#   $JOBSUBMIT jobs/$JOB.sh
# done

#PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/rlane/June6/MC_53X/
#JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"
#PATHS=(
#'DYJetsToLL'
#'DY1JetsToLL'
#'DY2JetsToLL'
#'DY3JetsToLL'
#'DY4JetsToLL'
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
#)
#for i in "${PATHS[@]}"
#do
#  echo "$i"
#  mkdir -p $SKIMPATH/em_skim/$i/
#  JOB="$i"_em_skim
#  $JOBWRAPPER "./bin/HiggsHTohh --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=em --output_folder=./ --output_name=Dummy_$JOB.root \
#  --do_skim=true --skim_path=$SKIMPATH/em_skim/"$i"/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#  ./scripts/hadd_and_filelist.sh $SKIMPATH em_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#  $JOBSUBMIT jobs/$JOB.sh
#done

 PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/rlane/June6/MC_53X/
# JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepshort.q"
 PATHS=(
#  'SUSYGluGluToHToTauTau_M-80'
#  'SUSYGluGluToHToTauTau_M-90'
#  'SUSYGluGluToHToTauTau_M-100'
#  'SUSYGluGluToHToTauTau_M-110'
#  'SUSYGluGluToHToTauTau_M-120'
# 'SUSYGluGluToHToTauTau_M-130'
#  'SUSYGluGluToHToTauTau_M-140'
#  'SUSYGluGluToHToTauTau_M-160'
#  'SUSYGluGluToHToTauTau_M-180'
#  'SUSYGluGluToHToTauTau_M-200'
#  'SUSYGluGluToHToTauTau_M-250'
#  'SUSYGluGluToHToTauTau_M-300'
#  'SUSYGluGluToHToTauTau_M-350'
#  'SUSYGluGluToHToTauTau_M-400'
#  'SUSYGluGluToHToTauTau_M-450'
#  'SUSYGluGluToHToTauTau_M-500'
#  'SUSYGluGluToHToTauTau_M-600'
#  'SUSYGluGluToHToTauTau_M-700'
#  'SUSYGluGluToHToTauTau_M-800'
#  'SUSYGluGluToHToTauTau_M-900'
#  'SUSYGluGluToHToTauTau_M-1000'
#  'SUSYBBHToTauTau_M-80'
#  'SUSYBBHToTauTau_M-90'
#  'SUSYBBHToTauTau_M-100'
#  'SUSYBBHToTauTau_M-110'
#  'SUSYBBHToTauTau_M-120'
#  'SUSYBBHToTauTau_M-130'
#  'SUSYBBHToTauTau_M-140'
#  'SUSYBBHToTauTau_M-160'
#  'SUSYBBHToTauTau_M-180'
#  'SUSYBBHToTauTau_M-200'
#  'SUSYBBHToTauTau_M-250'
#  'SUSYBBHToTauTau_M-300'
#  'SUSYBBHToTauTau_M-350'
#  'SUSYBBHToTauTau_M-400'
#  'SUSYBBHToTauTau_M-450'
#  'SUSYBBHToTauTau_M-500'
#  'SUSYBBHToTauTau_M-600'
#  'SUSYBBHToTauTau_M-700'
#  'SUSYBBHToTauTau_M-800'
#  'SUSYBBHToTauTau_M-900'
#  'SUSYBBHToTauTau_M-1000'
 'GluGluToHTohhTo2Tau2B_mH-260'
 'GluGluToHTohhTo2Tau2B_mH-300'
 'GluGluToHTohhTo2Tau2B_mH-350'
 'GluGluToAToZhToLLBB_mA-250'
 'GluGluToAToZhToLLBB_mA-300'
 'GluGluToAToZhToLLBB_mA-350'
 )
 for i in "${PATHS[@]}"
 do
   echo "$i"
   mkdir -p $SKIMPATH/em_skim/$i/
   JOB="$i"_em_skim
   $JOBWRAPPER "./bin/HiggsHTohh --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=em --output_folder=./ --output_name=Dummy_$JOB.root \
   --do_skim=true --skim_path=$SKIMPATH/em_skim/"$i"/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
   ./scripts/hadd_and_filelist.sh $SKIMPATH em_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
   $JOBSUBMIT jobs/$JOB.sh
 done

# PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/June6/MC_53X/
# JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepshort.q"
# PATHS=(
#  'GluGluToHToTauTau_M-90' 
#  'GluGluToHToTauTau_M-95' 
#  'GluGluToHToTauTau_M-100' 
#  'GluGluToHToTauTau_M-105' 
#  'GluGluToHToTauTau_M-110' 
#  'GluGluToHToTauTau_M-115' 
#  'GluGluToHToTauTau_M-120' 
#  'GluGluToHToTauTau_M-125'
#  'GluGluToHToTauTau_M-130' 
#  'GluGluToHToTauTau_M-135' 
#  'GluGluToHToTauTau_M-140' 
#  'GluGluToHToTauTau_M-145' 
#  'GluGluToHToTauTau_M-150' 
#  'GluGluToHToTauTau_M-155' 
#  'GluGluToHToTauTau_M-160' 
#  'VBF_HToTauTau_M-90' 
#  'VBF_HToTauTau_M-95' 
#  'VBF_HToTauTau_M-100' 
#  'VBF_HToTauTau_M-105' 
#  'VBF_HToTauTau_M-110' 
#  'VBF_HToTauTau_M-115' 
#  'VBF_HToTauTau_M-120' 
#  'VBF_HToTauTau_M-125'
#  'VBF_HToTauTau_M-130' 
#  'VBF_HToTauTau_M-135' 
#  'VBF_HToTauTau_M-140' 
#  'VBF_HToTauTau_M-145' 
#  'VBF_HToTauTau_M-150' 
#  'VBF_HToTauTau_M-155' 
#  'VBF_HToTauTau_M-160' 
#  'WH_ZH_TTH_HToTauTau_M-90' 
#  'WH_ZH_TTH_HToTauTau_M-95' 
#  'WH_ZH_TTH_HToTauTau_M-100' 
#  'WH_ZH_TTH_HToTauTau_M-105' 
#  'WH_ZH_TTH_HToTauTau_M-110' 
#  'WH_ZH_TTH_HToTauTau_M-115' 
#  'WH_ZH_TTH_HToTauTau_M-120' 
#  'WH_ZH_TTH_HToTauTau_M-125'
#  'WH_ZH_TTH_HToTauTau_M-130' 
#  'WH_ZH_TTH_HToTauTau_M-135' 
#  'WH_ZH_TTH_HToTauTau_M-140' 
#  'WH_ZH_TTH_HToTauTau_M-145'
#  'WH_ZH_TTH_HToTauTau_M-150' 
#  'WH_ZH_TTH_HToTauTau_M-155' 
#  'WH_ZH_TTH_HToTauTau_M-160'
#  'GluGluToHToWWTo2LAndTau2Nu_M-90' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-95' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-100' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-105' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-110' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-115' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-120' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-125'
#  'GluGluToHToWWTo2LAndTau2Nu_M-130' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-135' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-140' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-145' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-150' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-155' 
#  'GluGluToHToWWTo2LAndTau2Nu_M-160' 
#  'VBF_HToWWTo2LAndTau2Nu_M-90' 
#  'VBF_HToWWTo2LAndTau2Nu_M-95' 
#  'VBF_HToWWTo2LAndTau2Nu_M-100' 
#  'VBF_HToWWTo2LAndTau2Nu_M-105' 
 # 'VBF_HToWWTo2LAndTau2Nu_M-110' 
 # 'VBF_HToWWTo2LAndTau2Nu_M-115' 
 # 'VBF_HToWWTo2LAndTau2Nu_M-120' 
 # 'VBF_HToWWTo2LAndTau2Nu_M-125'
 # 'VBF_HToWWTo2LAndTau2Nu_M-130' 
 # 'VBF_HToWWTo2LAndTau2Nu_M-135' 
 # 'VBF_HToWWTo2LAndTau2Nu_M-140' 
 # 'VBF_HToWWTo2LAndTau2Nu_M-145' 
 # 'VBF_HToWWTo2LAndTau2Nu_M-150' 
 # 'VBF_HToWWTo2LAndTau2Nu_M-155' 
 # 'VBF_HToWWTo2LAndTau2Nu_M-160' 
#  'WH_ZH_TTH_HToWW_M-90' 
#  'WH_ZH_TTH_HToWW_M-95' 
#  'WH_ZH_TTH_HToWW_M-100' 
 # 'WH_ZH_TTH_HToWW_M-105' 
 # 'WH_ZH_TTH_HToWW_M-110' 
 # 'WH_ZH_TTH_HToWW_M-115' 
 # 'WH_ZH_TTH_HToWW_M-120' 
 # 'WH_ZH_TTH_HToWW_M-125'
 # 'WH_ZH_TTH_HToWW_M-130' 
 # 'WH_ZH_TTH_HToWW_M-135' 
 # 'WH_ZH_TTH_HToWW_M-140' 
 # 'WH_ZH_TTH_HToWW_M-145'
 # 'WH_ZH_TTH_HToWW_M-150' 
 # 'WH_ZH_TTH_HToWW_M-155' 
 # 'WH_ZH_TTH_HToWW_M-160'
 #)
 #for i in "${PATHS[@]}"
 #do
 #  echo "$i"
 #  mkdir -p $SKIMPATH/em_skim/$i/
 #  JOB="$i"_em_skim
 #  $JOBWRAPPER "./bin/HiggsHTohh --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=em --output_folder=./ --output_name=Dummy_$JOB.root \
 #  --do_skim=true --skim_path=$SKIMPATH/em_skim/"$i"/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
 #  ./scripts/hadd_and_filelist.sh $SKIMPATH em_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
 #  $JOBSUBMIT jobs/$JOB.sh
 #done
