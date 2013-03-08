JOBWRAPPER=./scripts/generate_job.sh
#JOBSUBMIT=true
JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"


CONFIG=scripts/Moriond_2012.cfg
echo $CONFIG
FILELIST=filelists/Feb20_Data_53X
SKIMPATH=$SSD/Feb20/Data_53X
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/Feb20/Data_53X/

 PATHS=(
      'SingleMu-2012A-13Jul2012-v1'
      'SingleMu-2012A-recover-06Aug2012-v1'
      'SingleMu-2012B-13Jul2012-v1'
      'SingleMu-2012C-24Aug2012-v1'
      'SingleMu-2012C-EcalRecover-11Dec2012-v1'
      'SingleMu-2012C-PromptReco-v2'
      'SingleMu-2012D-PromptReco-v1'
     )
  for i in "${PATHS[@]}"
  do
      echo "$i"
 
      mkdir -p $SKIMPATH/$i/tauid_skim/

      JOB="$i"_tauid_skim
      $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
      --do_skim=true --skim_path=$SKIMPATH/$i/tauid_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
      ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/tauid_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
      $JOBSUBMIT jobs/$JOB.sh
 done


#  PATHS=(
#       'Embedded-ETau-2012A-13Jul2012-v1'
#       'Embedded-ETau-2012A-recover-06Aug2012-v1'
#       'Embedded-ETau-2012B-13Jul2012-v1'
#       'Embedded-ETau-2012C-24Aug2012-v1'
#       'Embedded-ETau-2012C-PromptReco-v2'
#       'Embedded-ETau-2012D-PromptReco-v1'
#       )
#  for i in "${PATHS[@]}"
#  do
#      echo "$i"
 
#      mkdir -p $SKIMPATH/$i/et_skim/
      
#      JOB="$i"_et_skim
#      $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=et --output_folder=./ --output_name=Dummy_$JOB.root \
#      --do_skim=true --skim_path=$SKIMPATH/$i/et_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#      ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/et_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#      $JOBSUBMIT jobs/$JOB.sh
#  done
 
#  PATHS=(
#       'Embedded-MTau-2012A-13Jul2012-v1'
#       'Embedded-MTau-2012A-recover-06Aug2012-v1'
#       'Embedded-MTau-2012B-13Jul2012-v1'
#       'Embedded-MTau-2012C-24Aug2012-v1'
#       'Embedded-MTau-2012C-PromptReco-v2'
#       'Embedded-MTau-2012D-PromptReco-v1'
#       )
#  for i in "${PATHS[@]}"
#  do
#      echo "$i"
 
#      mkdir -p $SKIMPATH/$i/mt_skim/
 
#      JOB="$i"_mt_skim
#      $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
#      --do_skim=true --skim_path=$SKIMPATH/$i/mt_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#      ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/mt_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#      $JOBSUBMIT jobs/$JOB.sh
#  done

 
# CONFIG=scripts/Moriond_2012.cfg
# echo $CONFIG
# FILELIST=filelists/Dec30_MC_53X
# SKIMPATH=$SSD/Dec30/MC_53X
# PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/Dec30/MC_53X-Update/

PATHS=(
      # 'DYJetsToLL'
      # 'WJetsToLNu-v1'
      # 'WJetsToLNu-v2'
      # 'W1JetsToLNu'
      # 'W2JetsToLNu'
      # 'W3JetsToLNu'
      # 'W4JetsToLNu'
      # 'TTJets'
      # 'TT-v1'
      # 'TT-v2'
      # 'T-tW'
      # 'Tbar-tW'
      # 'WWJetsTo2L2Nu'
      # 'WZJetsTo2L2Q'
      # 'WZJetsTo3LNu'
      # 'ZZJetsTo2L2Nu'
      # 'ZZJetsTo2L2Q'
      # 'ZZJetsTo4L'
      # 'GluGluToHToTauTau_M-125'
      # 'VBF_HToTauTau_M-125'
      # 'WH_ZH_TTH_HToTauTau_M-125'
      # 'SUSYGluGluToHToTauTau_M-160'
      # 'SUSYBBHToTauTau_M-160'
      'GluGluToHToTauTau_M-90' 
      'GluGluToHToTauTau_M-95' 
      'GluGluToHToTauTau_M-100' 
      'GluGluToHToTauTau_M-105' 
      # 'GluGluToHToTauTau_M-110' 
      # 'GluGluToHToTauTau_M-115' 
      # 'GluGluToHToTauTau_M-120' 
      # 'GluGluToHToTauTau_M-130' 
      # 'GluGluToHToTauTau_M-135' 
      # 'GluGluToHToTauTau_M-140' 
      # 'GluGluToHToTauTau_M-145' 
      'GluGluToHToTauTau_M-150' 
      'GluGluToHToTauTau_M-155' 
      'GluGluToHToTauTau_M-160' 
      'VBF_HToTauTau_M-90' 
      'VBF_HToTauTau_M-95' 
      'VBF_HToTauTau_M-100' 
      'VBF_HToTauTau_M-105' 
      # 'VBF_HToTauTau_M-110' 
      # 'VBF_HToTauTau_M-115' 
      # 'VBF_HToTauTau_M-120' 
      # 'VBF_HToTauTau_M-130' 
      # 'VBF_HToTauTau_M-135' 
      # 'VBF_HToTauTau_M-140' 
      # 'VBF_HToTauTau_M-145' 
      'VBF_HToTauTau_M-150' 
      'VBF_HToTauTau_M-155' 
      'VBF_HToTauTau_M-160' 
      'WH_ZH_TTH_HToTauTau_M-90' 
      'WH_ZH_TTH_HToTauTau_M-95' 
      'WH_ZH_TTH_HToTauTau_M-100' 
      'WH_ZH_TTH_HToTauTau_M-105' 
      # 'WH_ZH_TTH_HToTauTau_M-110' 
      # 'WH_ZH_TTH_HToTauTau_M-115' 
      # 'WH_ZH_TTH_HToTauTau_M-120' 
      # 'WH_ZH_TTH_HToTauTau_M-130' 
      # 'WH_ZH_TTH_HToTauTau_M-135' 
      # 'WH_ZH_TTH_HToTauTau_M-140' 
      # 'WH_ZH_TTH_HToTauTau_M-145'
      'WH_ZH_TTH_HToTauTau_M-150' 
      'WH_ZH_TTH_HToTauTau_M-155' 
      'WH_ZH_TTH_HToTauTau_M-160' 
      # 'DY1JetsToLL'
      # 'DY2JetsToLL'
      # 'DY3JetsToLL'
      # 'DY4JetsToLL'
     )
#  for i in "${PATHS[@]}"
#  do
#      echo "$i"

#      mkdir -p $SKIMPATH/$i/et_skim/
#      mkdir -p $SKIMPATH/$i/mt_skim/
     
#      JOB="$i"_et_skim
#      $JOBWRAPPER "./bin/HiggsTauTau --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=et --output_folder=./ --output_name=Dummy_$JOB.root \
#      --do_skim=true --skim_path=$SKIMPATH/$i/et_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#      ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/et_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#      $JOBSUBMIT jobs/$JOB.sh

#      JOB="$i"_mt_skim
#      $JOBWRAPPER "./bin/HiggsTauTau --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
#      --do_skim=true --skim_path=$SKIMPATH/$i/mt_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#      ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/mt_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#      $JOBSUBMIT jobs/$JOB.sh
# done

