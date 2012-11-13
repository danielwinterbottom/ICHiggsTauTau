make -j4

MSSM_MODE=$1

if [ -z $1 ]
then
    echo "No mode specified!"
    exit
fi

if [ $MSSM_MODE == 0 ]
then
    echo "Process as SM..."
    CONFIG=scripts/mc_2011.cfg
else
    echo "Process short signal..."
    CONFIG=scripts/mc_2011.cfg
fi
echo $CONFIG

SOURCE=/Volumes/Storage/Sept11/MC_42X
FILELIST=filelists/Sept11/MC_42X

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
if [ $MSSM_MODE == 0 ]
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
      'SUSYGluGluToHToTauTau_M-90'
      'SUSYGluGluToHToTauTau_M-100'
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
      'SUSYBBHToTauTau_M-90'
      'SUSYBBHToTauTau_M-100'
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


TSCALE=(
'0'
'1'
'2'
)

 for i in "${PATHS[@]}"
 do
   for j in "${TSCALE[@]}"
   do
     echo "$i"
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_electauSkim_IC_filelist.dat --mode=0 --output="$i"_ElecTau_2011.root >> mc_2011_ElecTau_"$j".log &
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_mutauSkim_IC_filelist.dat --mode=1 --output="$i"_MuTau_2011.root >> mc_2011_MuTau_"$j".log &
   done
   wait
 done

 # Do standard DYJetsToLL-L, can use DYJetsToLL SVFit
 echo "DYJetsToLL-L"
 for j in "${TSCALE[@]}"
 do
   ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/DYJetsToLL_electauSkim_IC_filelist.dat --mode=0 \
   --svfit_override=DYJetsToLL_ElecTau_2011.root --faked_tau_selector=1 --output=DYJetsToLL-L_ElecTau_2011.root >> mc_2011_ElecTau_"$j".log &
   ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/DYJetsToLL_mutauSkim_IC_filelist.dat --mode=1 \
   --svfit_override=DYJetsToLL_MuTau_2011.root --faked_tau_selector=1 --output=DYJetsToLL-L_MuTau_2011.root >> mc_2011_MuTau_"$j".log &
 done
 wait


 # Do standard DYJetsToLL-J, can use DYJetsToLL SVFit
 echo "DYJetsToLL-J"
 for j in "${TSCALE[@]}"
 do
   ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/DYJetsToLL_electauSkim_IC_filelist.dat --mode=0 \
   --svfit_override=DYJetsToLL_ElecTau_2011.root --faked_tau_selector=2 --output=DYJetsToLL-J_ElecTau_2011.root >> mc_2011_ElecTau_"$j".log &
   ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/DYJetsToLL_mutauSkim_IC_filelist.dat --mode=1 \
   --svfit_override=DYJetsToLL_MuTau_2011.root --faked_tau_selector=2 --output=DYJetsToLL-J_MuTau_2011.root >> mc_2011_MuTau_"$j".log &
 done
 wait


 # Do Special Mode 7 DYJetsToTauTau, can use DYJetsToTauTau SVFit
 echo "DYJetsToTauTau"
 for j in "${TSCALE[@]}"
 do
   ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/DYJetsToTauTau_electauSkim_IC_filelist.dat --mode=0 \
   --special_mode=7 --svfit_override=DYJetsToTauTau_ElecTau_2011.root --output=DYJetsToTauTau_ElecTau_2011.root >> mc_2011_ElecTau_"$j".log &
   ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/DYJetsToTauTau_mutauSkim_IC_filelist.dat --mode=1 \
   --special_mode=7 --svfit_override=DYJetsToTauTau_MuTau_2011.root --output=DYJetsToTauTau_MuTau_2011.root >> mc_2011_MuTau_"$j".log &
 done
 wait


 # Do Special Mode 7 WJetsToLNuSoup, can use standard SVFit
 echo "Special_7_WJetsToLNuSoup"
 for j in "${TSCALE[@]}"
 do
   ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/WJetsToLNuSoup_electauSkim_IC_filelist.dat --mode=0 \
   --special_mode=7 --svfit_override=WJetsToLNuSoup_ElecTau_2011.root --output=WJetsToLNuSoup_ElecTau_2011.root >> mc_2011_ElecTau_"$j".log &
   ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/WJetsToLNuSoup_mutauSkim_IC_filelist.dat --mode=1 \
   --special_mode=7 --svfit_override=WJetsToLNuSoup_MuTau_2011.root --output=WJetsToLNuSoup_MuTau_2011.root >> mc_2011_MuTau_"$j".log &
 done
 wait


 # Do Special Mode 7 TTJets, can use standard SVFit
 echo "Special_7_TTJets"
 for j in "${TSCALE[@]}"
 do
   ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/TTJets_electauSkim_IC_filelist.dat --mode=0 \
   --special_mode=7 --svfit_override=TTJets_ElecTau_2011.root --output=TTJets_ElecTau_2011.root >> mc_2011_ElecTau_"$j".log &
   ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/TTJets_mutauSkim_IC_filelist.dat --mode=1 \
   --special_mode=7 --svfit_override=TTJets_MuTau_2011.root --output=TTJets_MuTau_2011.root >> mc_2011_MuTau_"$j".log &
 done
 wait


 PATHS=(
 'WWJetsTo2L2Nu'
 'WZJetsTo2L2Q'
 'WZJetsTo3LNu'
 'ZZJetsTo2L2Nu'
 'ZZJetsTo2L2Q'
 'ZZJetsTo4L'
 'T-tW'
 'Tbar-tW'
 )
 for i in "${PATHS[@]}"
 do
 echo "Special_$i"
   for j in "${TSCALE[@]}"
   do
     # Do Special Mode 7 Diboson, can use standard SVfit
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_electauSkim_IC_filelist.dat --mode=0 \
       --special_mode=7 --svfit_override="$i"_ElecTau_2011.root --output="$i"_ElecTau_2011.root >> mc_2011_ElecTau_"$j".log &
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_mutauSkim_IC_filelist.dat --mode=1 \
       --special_mode=7 --svfit_override="$i"_MuTau_2011.root --output="$i"_MuTau_2011.root >> mc_2011_MuTau_"$j".log &
   done
   wait
 done

 PATHS=(
 'DYJetsToLL'
 )

 for i in "${PATHS[@]}"
 do
   echo "$i"
   for j in "${TSCALE[@]}"
   do
     # Do Special Mode 7 DYJetsToLL, can use standard SVFit
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_electauSkim_IC_filelist.dat --mode=0 \
     --special_mode=7 --svfit_override="$i"_ElecTau_2011.root --output="$i"_ElecTau_2011.root >> mc_2011_ElecTau_"$j".log &
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_mutauSkim_IC_filelist.dat --mode=1 \
     --special_mode=7 --svfit_override="$i"_MuTau_2011.root --output="$i"_MuTau_2011.root >> mc_2011_MuTau_"$j".log &
     # Do Special Mode 7 DYJetsToLL-L and DYJetsToLL-J, can use standard SVFit
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_electauSkim_IC_filelist.dat --mode=0 \
     --special_mode=7 --svfit_override="$i"_ElecTau_2011.root --faked_tau_selector=1 --output="$i"-L_ElecTau_2011.root >> mc_2011_ElecTau_"$j".log &
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_mutauSkim_IC_filelist.dat --mode=1 \
     --special_mode=7 --svfit_override="$i"_MuTau_2011.root --faked_tau_selector=1 --output="$i"-L_MuTau_2011.root >> mc_2011_MuTau_"$j".log &
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_electauSkim_IC_filelist.dat --mode=0 \
     --special_mode=7 --svfit_override="$i"_ElecTau_2011.root --faked_tau_selector=2 --output="$i"-J_ElecTau_2011.root >> mc_2011_ElecTau_"$j".log &
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_mutauSkim_IC_filelist.dat --mode=1 \
     --special_mode=7 --svfit_override="$i"_MuTau_2011.root --faked_tau_selector=2 --output="$i"-J_MuTau_2011.root >> mc_2011_MuTau_"$j".log &
     wait
     #### NEW STUFF ####
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_electauSkim_IC_filelist.dat --mode=0 \
     --special_mode=18  --faked_tau_selector=1 --output="$i"-L_ElecTau_2011.root >> mc_2011_ElecTau_"$j".log &
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_mutauSkim_IC_filelist.dat --mode=1 \
     --special_mode=18  --faked_tau_selector=1 --output="$i"-L_MuTau_2011.root >> mc_2011_MuTau_"$j".log &
     wait
     # Do Special Mode 9 DYJetsToLL, can use standard SVFit
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_electauSkim_IC_filelist.dat --mode=0 \
     --special_mode=9 --svfit_override="$i"_ElecTau_2011.root --output="$i"_ElecTau_2011.root >> mc_2011_ElecTau_"$j".log &
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_mutauSkim_IC_filelist.dat --mode=1 \
     --special_mode=9 --svfit_override="$i"_MuTau_2011.root --output="$i"_MuTau_2011.root >> mc_2011_MuTau_"$j".log &
     # Do Special Mode 9 DYJetsToLL-L and DYJetsToLL-J, can use standard SVFit
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_electauSkim_IC_filelist.dat --mode=0 \
     --special_mode=9 --svfit_override="$i"_ElecTau_2011.root --faked_tau_selector=1 --output="$i"-L_ElecTau_2011.root >> mc_2011_ElecTau_"$j".log &
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_mutauSkim_IC_filelist.dat --mode=1 \
     --special_mode=9 --svfit_override="$i"_MuTau_2011.root --faked_tau_selector=1 --output="$i"-L_MuTau_2011.root >> mc_2011_MuTau_"$j".log &
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_electauSkim_IC_filelist.dat --mode=0 \
     --special_mode=9 --svfit_override="$i"_ElecTau_2011.root --faked_tau_selector=2 --output="$i"-J_ElecTau_2011.root >> mc_2011_ElecTau_"$j".log &
     ./bin/HiggsTauTau --cfg=$CONFIG --tau_scale_mode=$j --filelist=$FILELIST/"$i"_mutauSkim_IC_filelist.dat --mode=1 \
     --special_mode=9 --svfit_override="$i"_MuTau_2011.root --faked_tau_selector=2 --output="$i"-J_MuTau_2011.root >> mc_2011_MuTau_"$j".log &
     wait
   done
 done









