## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}
echo "Using job-wrapper: " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT

CONFIG=scripts/mc_2011.cfg
echo "Config file: $CONFIG"
FILELIST=filelists/Sept11_MC_42X
echo "Filelist prefix: $FILELIST"

if (( "$#" != "3" ))
then
    echo "<CHANNELS: 0=et,mt 1=em> <TAU ENERGY SCALE SHIFT: 0=none 1=down 2=up> <SIGNAL SAMPLES 0=subset, 1=all>"
    exit
fi

OPTION=$1
DOTSCALE=$2
DOSIGNAL=$3

if [ $DOTSCALE == 1 ]
then
  TSCALE=('1')
elif  [ $DOTSCALE == 2 ]
then
  TSCALE=('2')
else
  TSCALE=('0')
fi

PATHS=(
'DYJetsToTauTau'
'DYJetsToLL'
'WJetsToLNuSoup'
'TTJets' 
'WWJetsTo2L2Nu'
'WZJetsTo2L2Q'
'WZJetsTo3LNu'
'ZZJetsTo2L2Nu'
'ZZJetsTo2L2Q'
'ZZJetsTo4L'
'T-tW'
'Tbar-tW'
)
if [ $OPTION == 2 ]
then
  PATHS=(
  'DYJetsToTauTau'
  'TTJets'
  'WWJetsTo2L2Nu'
  'WZJetsTo2L2Q'
  'WZJetsTo3LNu'
  'ZZJetsTo2L2Nu'
  'ZZJetsTo2L2Q'
  'ZZJetsTo4L'
  'T-tW'
  'Tbar-tW'
  )
fi


if [ $DOSIGNAL == 1 ]
then
    PATHS+=(
      'GluGluToHToTauTau_M-110' 
      'GluGluToHToTauTau_M-115' 
      'GluGluToHToTauTau_M-120' 
      'GluGluToHToTauTau_M-125' 
      'GluGluToHToTauTau_M-130' 
      'GluGluToHToTauTau_M-135' 
      'GluGluToHToTauTau_M-140' 
      'GluGluToHToTauTau_M-145' 
      'VBF_HToTauTau_M-110' 
      'VBF_HToTauTau_M-115' 
      'VBF_HToTauTau_M-120' 
      'VBF_HToTauTau_M-125' 
      'VBF_HToTauTau_M-130' 
      'VBF_HToTauTau_M-135' 
      'VBF_HToTauTau_M-140' 
      'VBF_HToTauTau_M-145' 
      'WH_ZH_TTH_HToTauTau_M-110' 
      'WH_ZH_TTH_HToTauTau_M-115' 
      'WH_ZH_TTH_HToTauTau_M-120' 
      'WH_ZH_TTH_HToTauTau_M-125' 
      'WH_ZH_TTH_HToTauTau_M-130' 
      'WH_ZH_TTH_HToTauTau_M-135' 
      'WH_ZH_TTH_HToTauTau_M-140' 
      'WH_ZH_TTH_HToTauTau_M-145'
      # 'SUSYGluGluToHToTauTau_M-90'
      # 'SUSYGluGluToHToTauTau_M-100'
      # 'SUSYGluGluToHToTauTau_M-120'
      # 'SUSYGluGluToHToTauTau_M-130'
      # 'SUSYGluGluToHToTauTau_M-140'
      # 'SUSYGluGluToHToTauTau_M-160'
      # 'SUSYGluGluToHToTauTau_M-180'
      # 'SUSYGluGluToHToTauTau_M-200'
      # 'SUSYGluGluToHToTauTau_M-250'
      # 'SUSYGluGluToHToTauTau_M-300'
      # 'SUSYGluGluToHToTauTau_M-350'
      # 'SUSYGluGluToHToTauTau_M-400'
      # 'SUSYGluGluToHToTauTau_M-450'
      # 'SUSYGluGluToHToTauTau_M-500'
      # 'SUSYGluGluToHToTauTau_M-600'
      # 'SUSYGluGluToHToTauTau_M-700'
      # 'SUSYGluGluToHToTauTau_M-800'
      # 'SUSYGluGluToHToTauTau_M-900'
      # 'SUSYGluGluToHToTauTau_M-1000'
      # 'SUSYBBHToTauTau_M-90'
      # 'SUSYBBHToTauTau_M-100'
      # 'SUSYBBHToTauTau_M-120'
      # 'SUSYBBHToTauTau_M-130'
      # 'SUSYBBHToTauTau_M-140'
      # 'SUSYBBHToTauTau_M-160'
      # 'SUSYBBHToTauTau_M-180'
      # 'SUSYBBHToTauTau_M-200'
      # 'SUSYBBHToTauTau_M-250'
      # 'SUSYBBHToTauTau_M-300'
      # 'SUSYBBHToTauTau_M-350'
      # 'SUSYBBHToTauTau_M-400'
      # 'SUSYBBHToTauTau_M-450'
      # 'SUSYBBHToTauTau_M-500'
      # 'SUSYBBHToTauTau_M-600'
      # 'SUSYBBHToTauTau_M-700'
      # 'SUSYBBHToTauTau_M-800'
      # 'SUSYBBHToTauTau_M-900'
      # 'SUSYBBHToTauTau_M-1000'
    )
else
    PATHS+=(
      'GluGluToHToTauTau_M-125' 
      'VBF_HToTauTau_M-125' 
      'WH_ZH_TTH_HToTauTau_M-125' 
      # 'SUSYGluGluToHToTauTau_M-160'
      # 'SUSYBBHToTauTau_M-160'
    )
fi

if [ $OPTION == 0 ]
then
  for j in "${TSCALE[@]}"
  do
    # DYJetsToLL-L
    JOB=DYJetsToLL-L_et_2011
    $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=2 --filelist="$FILELIST"_DYJetsToLL_et_skim.dat --channel=et \
    --svfit_override=DYJetsToLL_et_2011.root --faked_tau_selector=1 --output_name=$JOB.root &> jobs/$JOB-$j.log" jobs/$JOB-$j.sh
    $JOBSUBMIT jobs/$JOB-$j.sh

    JOB=DYJetsToLL-L_mt_2011
    $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=2 --filelist="$FILELIST"_DYJetsToLL_mt_skim.dat --channel=mt \
    --svfit_override=DYJetsToLL_mt_2011.root --faked_tau_selector=1 --output_name=$JOB.root &> jobs/$JOB-$j.log" jobs/$JOB-$j.sh
    $JOBSUBMIT jobs/$JOB-$j.sh

    # Special Mode 18 DYJetsToLL-L
    JOB=DYJetsToLL-L_et_2011
    $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=2 --filelist="$FILELIST"_DYJetsToLL_et_skim.dat --channel=et \
    --special_mode=18 --faked_tau_selector=1 --output_name=$JOB.root &> jobs/Special_18_$JOB-$j.log" jobs/Special_18_$JOB-$j.sh
    $JOBSUBMIT jobs/Special_18_$JOB-$j.sh

    JOB=DYJetsToLL-L_mt_2011
    $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=2 --filelist="$FILELIST"_DYJetsToLL_mt_skim.dat --channel=mt \
    --special_mode=18 --faked_tau_selector=1 --output_name=$JOB.root &> jobs/Special_18_$JOB-$j.log" jobs/Special_18_$JOB-$j.sh
    $JOBSUBMIT jobs/Special_18_$JOB-$j.sh

    # DYJetsToLL-J
    JOB=DYJetsToLL-J_et_2011
    $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=2 --filelist="$FILELIST"_DYJetsToLL_et_skim.dat --channel=et \
    --svfit_override=DYJetsToLL_et_2011.root --faked_tau_selector=2 --output_name=$JOB.root &> jobs/$JOB-$j.log" jobs/$JOB-$j.sh
    $JOBSUBMIT jobs/$JOB-$j.sh

    JOB=DYJetsToLL-J_mt_2011
    $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=2 --filelist="$FILELIST"_DYJetsToLL_mt_skim.dat --channel=mt \
    --svfit_override=DYJetsToLL_mt_2011.root --faked_tau_selector=2 --output_name=$JOB.root &> jobs/$JOB-$j.log" jobs/$JOB-$j.sh
    $JOBSUBMIT jobs/$JOB-$j.sh
  done
fi

for i in "${PATHS[@]}"
do
  for j in "${TSCALE[@]}"
  do

    if [ $OPTION == 0 ]
    then
      JOB="$i"_et_2011
      $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist="$FILELIST"_"$i"_et_skim.dat --channel=et --output_name=$JOB.root &> jobs/$JOB-$j.log" jobs/$JOB-$j.sh
      $JOBSUBMIT jobs/$JOB-$j.sh

      JOB="$i"_mt_2011
      $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist="$FILELIST"_"$i"_mt_skim.dat --channel=mt --output_name=$JOB.root &> jobs/$JOB-$j.log" jobs/$JOB-$j.sh
      $JOBSUBMIT jobs/$JOB-$j.sh
    fi
    if [ $OPTION == 1 ]
      then
      JOB="$i"_em_2011
      $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist="$FILELIST"_"$i"_em_skim.dat --channel=em --output_name=$JOB.root &> jobs/$JOB-$j.log" jobs/$JOB-$j.sh
      $JOBSUBMIT jobs/$JOB-$j.sh
    fi
  done
done