
if (( "$#" != "3" ))
then
    echo "<0=et,mt, 1=em, 2=mtmet> <0=normal, 1=shift down, 2=shift up> <0=short signal, 1 = all signal>"
    exit
fi

OPTION=$1
DOTSCALE=$2
DOSIGNAL=$3

DATASET=Moriond
#DATASET=HCP

CONFIG=scripts/mc_"$DATASET"_2012.cfg
echo "Using config: $CONFIG"

FILELIST=filelists/Dec2/MC_53X


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


PATHS=(
'WJetsToLNuSoup'
'TTJets'
'TT'
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
  'TTJets'
  # 'TT'
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
      'SUSYGluGluToHToTauTau_M-80'
      'SUSYGluGluToHToTauTau_M-90'
      'SUSYGluGluToHToTauTau_M-100'
      'SUSYGluGluToHToTauTau_M-110'
      'SUSYGluGluToHToTauTau_M-120'
      'SUSYGluGluToHToTauTau_M-130'
      'SUSYGluGluToHToTauTau_M-140'
      'SUSYGluGluToHToTauTau_M-160'
      'SUSYGluGluToHToTauTau_M-180'
      'SUSYGluGluToHToTauTau_M-200'
      'SUSYGluGluToHToTauTau_M-250'
      'SUSYGluGluToHToTauTau_M-300'
      'SUSYGluGluToHToTauTau_M-350'
      'SUSYGluGluToHToTauTau_M-400'
      'SUSYGluGluToHToTauTau_M-450'
      'SUSYGluGluToHToTauTau_M-500'
      'SUSYGluGluToHToTauTau_M-600'
      'SUSYGluGluToHToTauTau_M-700'
      'SUSYGluGluToHToTauTau_M-800'
      'SUSYGluGluToHToTauTau_M-900'
      'SUSYGluGluToHToTauTau_M-1000'
      'SUSYBBHToTauTau_M-80'
      'SUSYBBHToTauTau_M-90'
      'SUSYBBHToTauTau_M-100'
      'SUSYBBHToTauTau_M-110'
      'SUSYBBHToTauTau_M-120'
      'SUSYBBHToTauTau_M-130'
      'SUSYBBHToTauTau_M-140'
      'SUSYBBHToTauTau_M-160'
      'SUSYBBHToTauTau_M-180'
      'SUSYBBHToTauTau_M-200'
      'SUSYBBHToTauTau_M-250'
      'SUSYBBHToTauTau_M-300'
      'SUSYBBHToTauTau_M-350'
      'SUSYBBHToTauTau_M-400'
      'SUSYBBHToTauTau_M-450'
      'SUSYBBHToTauTau_M-500'
      'SUSYBBHToTauTau_M-600'
      'SUSYBBHToTauTau_M-700'
      'SUSYBBHToTauTau_M-800'
      'SUSYBBHToTauTau_M-900'
      'SUSYBBHToTauTau_M-1000'
    )
else
    PATHS+=(
      'GluGluToHToTauTau_M-125' 
      'VBF_HToTauTau_M-125' 
      'WH_ZH_TTH_HToTauTau_M-125' 
      'SUSYGluGluToHToTauTau_M-160'
      'SUSYBBHToTauTau_M-160'
    )
fi


if [ $OPTION == 0 ]
then
for j in "${TSCALE[@]}"
do
  echo "DYJetsToTauTau"
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=1 --filelist=$FILELIST/DYJetsToLL_et_skim_filelist.dat --channel=et --output_name=DYJetsToTauTau_et_2012.root >> mc_2012_et.log &
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=1 --filelist=$FILELIST/DYJetsToLL_mt_skim_filelist.dat --channel=mt --output_name=DYJetsToTauTau_mt_2012.root >> mc_2012_mt.log &
  wait

  echo "DYJetsToLL"
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=2 --filelist=$FILELIST/DYJetsToLL_et_skim_filelist.dat --channel=et --output_name=DYJetsToLL_et_2012.root >> mc_2012_et.log &
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=2 --filelist=$FILELIST/DYJetsToLL_mt_skim_filelist.dat --channel=mt --output_name=DYJetsToLL_mt_2012.root >> mc_2012_mt.log &
  wait

  echo "DYJetsToLL-L"
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=2 --filelist=$FILELIST/DYJetsToLL_et_skim_filelist.dat --channel=et \
  --svfit_override=DYJetsToLL_et_2012.root --faked_tau_selector=1 --output_name=DYJetsToLL-L_et_2012.root >> mc_2012_et.log &
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=2 --filelist=$FILELIST/DYJetsToLL_mt_skim_filelist.dat --channel=mt \
  --svfit_override=DYJetsToLL_mt_2012.root --faked_tau_selector=1 --output_name=DYJetsToLL-L_mt_2012.root >> mc_2012_mt.log &
  wait
  echo "DYJetsToLL-J"
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=2 --filelist=$FILELIST/DYJetsToLL_et_skim_filelist.dat --channel=et \
  --svfit_override=DYJetsToLL_et_2012.root --faked_tau_selector=2 --output_name=DYJetsToLL-J_et_2012.root >> mc_2012_et.log &
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=2 --filelist=$FILELIST/DYJetsToLL_mt_skim_filelist.dat --channel=mt \
  --svfit_override=DYJetsToLL_mt_2012.root --faked_tau_selector=2 --output_name=DYJetsToLL-J_mt_2012.root >> mc_2012_mt.log &
  wait
done
fi

if [ $OPTION == 1 ]
then
for j in "${TSCALE[@]}"
do
  echo "DYJetsToTauTau"
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=1 --filelist=$FILELIST/DYJetsToLL_mtmet_skim_filelist.dat --channel=mtmet --output_name=DYJetsToTauTau_mtmet_2012.root >> mc_2012_mtmet.log &
  wait

  echo "DYJetsToLL"
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=2 --filelist=$FILELIST/DYJetsToLL_mtmet_skim_filelist.dat --channel=mtmet --output_name=DYJetsToLL_mtmet_2012.root >> mc_2012_mtmet.log &
  wait

  echo "DYJetsToLL-L"
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=2 --filelist=$FILELIST/DYJetsToLL_mtmet_skim_filelist.dat --channel=mtmet \
  --svfit_override=DYJetsToLL_mtmet_2012.root --faked_tau_selector=1 --output_name=DYJetsToLL-L_mtmet_2012.root >> mc_2012_mtmet.log &
  wait
  echo "DYJetsToLL-J"
  ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --ztautau_mode=2 --filelist=$FILELIST/DYJetsToLL_mtmet_skim_filelist.dat --channel=mtmet \
  --svfit_override=DYJetsToLL_mtmet_2012.root --faked_tau_selector=2 --output_name=DYJetsToLL-J_mtmet_2012.root >> mc_2012_mtmet.log &
  wait
done
fi

if [ $OPTION == 2 ]
then
  echo "DYJetsToTauTau"
  ./bin/HiggsTauTau --cfg=$CONFIG --ztautau_mode=1 --filelist=$FILELIST/DYJetsToLL_em_skim_filelist.dat --channel=em --output_name=DYJetsToTauTau_em_2012.root >> mc_2012_em.log &
  wait
fi


 for i in "${PATHS[@]}"
 do
   echo "$i"
   for j in "${TSCALE[@]}"
   do

     if [ $OPTION == 0 ]
       then
       ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_et_skim_filelist.dat --channel=et --output_name="$i"_et_2012.root >> mc_2012_et.log &
       ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_mt_skim_filelist.dat --channel=mt --output_name="$i"_mt_2012.root >> mc_2012_mt.log &
     fi
     if [ $OPTION == 1 ]
       then
       ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_mtmet_skim_filelist.dat --channel=mtmet --output_name="$i"_mtmet_2012.root >> mc_2012_mtmet.log &
     fi
     if [ $OPTION == 2 ]
       then
       ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_em_skim_filelist.dat --channel=em --output_name="$i"_em_2012.root >> mc_2012_em.log &
     fi
     wait
   done
 done









