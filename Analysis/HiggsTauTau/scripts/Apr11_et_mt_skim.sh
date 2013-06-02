JOBWRAPPER=./scripts/generate_job.sh
#JOBSUBMIT=true
JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"


CONFIG=scripts/Paper_2012.cfg
echo $CONFIG

FILELIST=filelists/Apr11_Data_53X
SKIMPATH=$SSD/Apr11/Data_53X
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/Apr11/Data_53X/

PATHS=(
'TauPlusX-2012A-13Jul2012-v1'
'TauPlusX-2012A-recover-06Aug2012-v1'
'TauPlusX-2012B-13Jul2012-v1'
'TauPlusX-2012C-24Aug2012-v1'
'TauPlusX-2012C-PromptReco-v2'
'TauPlusX-2012C-EcalRecover-11Dec2012-v1'
'TauPlusX-2012D-PromptReco-v1'
#'TauPlusX-2012A-22Jan2013-v1'
#'TauPlusX-2012B-22Jan2013-v1'
#'TauPlusX-2012C-22Jan2013-v1'
#'TauPlusX-2012D-22Jan2013-v1'
)
for i in "${PATHS[@]}"
do
  echo "$i"

  mkdir -p $SKIMPATH/et_skim/$i/
  mkdir -p $SKIMPATH/mt_skim/$i/
  mkdir -p $SKIMPATH/et_skim/Special_2_$i/
  mkdir -p $SKIMPATH/mt_skim/Special_2_$i/

  JOB="$i"_et_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=et --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/et_skim/$i/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH et_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

  JOB="$i"_mt_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/mt_skim/$i/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH mt_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

  JOB=Special_2_"$i"_et_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=et --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/et_skim/Special_2_"$i"/ --special_mode=2 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH et_skim/Special_2_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

  JOB=Special_2_"$i"_mt_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/mt_skim/Special_2_"$i"/ --special_mode=2 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH mt_skim/Special_2_"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
done



PATHS=(
'Embedded-ETau-2012A-13Jul2012-v1'
'Embedded-ETau-2012A-recover-06Aug2012-v1'
'Embedded-ETau-2012B-13Jul2012-v1'
'Embedded-ETau-2012C-24Aug2012-v1'
'Embedded-ETau-2012C-PromptReco-v2'
'Embedded-ETau-2012D-PromptReco-v1'
#'RecHit-ETau-2012A-22Jan2013-v1'
#'RecHit-ETau-2012B-22Jan2013-v1'
#'RecHit-ETau-2012C-22Jan2013-v1'
#'Embedded-ETau-2012C-22Jan2013-v1'
#'RecHit-ETau-2012D-22Jan2013-v1'
)
for i in "${PATHS[@]}"
do
  echo "$i"

  mkdir -p $SKIMPATH/et_skim/$i/

  JOB="$i"_et_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=et --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/et_skim/$i/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH et_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
done



PATHS=(
'Embedded-MTau-2012A-13Jul2012-v1'
'Embedded-MTau-2012A-recover-06Aug2012-v1'
'Embedded-MTau-2012B-13Jul2012-v1'
'Embedded-MTau-2012C-24Aug2012-v1'
'Embedded-MTau-2012C-PromptReco-v2'
'Embedded-MTau-2012D-PromptReco-v1'
#'RecHit-MTau-2012A-22Jan2013-v1'
#'RecHit-MTau-2012B-22Jan2013-v1'
#'RecHit-MTau-2012C-22Jan2013-v1'
#'Embedded-MTau-2012C-22Jan2013-v1'
#'RecHit-MTau-2012D-22Jan2013-v1'
)
for i in "${PATHS[@]}"
do
  echo "$i"

  mkdir -p $SKIMPATH/mt_skim/$i/

  JOB="$i"_mt_skim
  $JOBWRAPPER "./bin/HiggsTauTau --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
  --do_skim=true --skim_path=$SKIMPATH/mt_skim/$i/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
  ./scripts/hadd_and_filelist.sh $SKIMPATH mt_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
done


FILELIST=filelists/Apr11_MC_53X
SKIMPATH=$SSD/Apr11/MC_53X
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/Apr11/MC_53X/

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
#'SUSYGluGluToHToTauTau_M-80'
#'SUSYGluGluToHToTauTau_M-90'
#'SUSYGluGluToHToTauTau_M-100'
#'SUSYGluGluToHToTauTau_M-110'
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
#'SUSYBBHToTauTau_M-80'
#'SUSYBBHToTauTau_M-90'
#'SUSYBBHToTauTau_M-100'
#'SUSYBBHToTauTau_M-110'
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

