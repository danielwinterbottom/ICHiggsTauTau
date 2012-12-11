CONFIG=scripts/moriond_data_2012.cfg
echo $CONFIG
FILELIST=filelists/Dec2/Data_53X
SKIMPATH=/Volumes/Storage/Dec2/Data_53X
PREFIX=/internal/Dec2/Data_53X/
PATHS=(
     'MuEG-2012A-13Jul2012-v1'
     # 'MuEG-2012A-recover-06Aug2012-v1'
     # 'MuEG-2012B-13Jul2012-v1'
     # 'MuEG-2012C-24Aug2012-v1'
     # 'MuEG-2012C-PromptReco-v2'
     # 'MuEG-2012D-PromptReco-v1'
    )
 for i in "${PATHS[@]}"
 do
     eval `scramv1 runtime -sh`
     echo "$i"
     mkdir -p $SKIMPATH/$i/em_skim/
     mkdir -p $SKIMPATH/Special_25_$i/em_skim/
     ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=em --output_folder=./ --output_name=Dummy.root \
     --do_skim=true --skim_path=$SKIMPATH/$i/em_skim/ --svfit_mode=0 --input_prefix=$PREFIX
     
     ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=em --output_folder=./ --output_name=Dummy.root \
     --do_skim=true --skim_path=$SKIMPATH/Special_25_$i/em_skim/ --svfit_mode=0 --special_mode=25 --input_prefix=$PREFIX
     
     wait
     source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc46-opt/root/bin/thisroot.sh
     cd $SKIMPATH/$i/em_skim/
     hadd -O TempTree.root EventTree*
     rm EventTree*.root
     mv TempTree.root EventTree_All.root
     cd -
     cd $SKIMPATH/Special_25_$i/em_skim/
     hadd -O TempTree.root EventTree*
     rm EventTree*.root
     mv TempTree.root EventTree_All.root
     cd -
done

#  FILELIST=filelists/Dec2/Embedded_53X
#  SKIMPATH=/Volumes/Storage/Dec2/Embedded_53X
#  PREFIX=/internal/Dec3/Embedded_53X/
#  PATHS=(
#      'Embedded-EM-2012A-13Jul2012-v1'
#      'Embedded-EM-2012A-recover-06Aug2012-v1'
#      'Embedded-EM-2012B-13Jul2012-v1'
#      'Embedded-EM-2012C-24Aug2012-v1'
#      'Embedded-EM-2012C-PromptReco-v2'
#      )
#   for i in "${PATHS[@]}"
#   do
#       eval `scramv1 runtime -sh`
#       echo "$i"
#       mkdir -p $SKIMPATH/$i/em_skim/
#       ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=em --output_folder=./ --output_name=Dummy.root \
#       --do_skim=true --skim_path=$SKIMPATH/$i/em_skim/ --svfit_mode=0 --input_prefix=$PREFIX
#       wait
#       source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc46-opt/root/bin/thisroot.sh
#       cd $SKIMPATH/$i/em_skim/
#       hadd -O TempTree.root EventTree*
#       rm EventTree*.root
#       mv TempTree.root EventTree_All.root
#       cd -
#   done


# CONFIG=scripts/moriond_mc_2012.cfg
# echo $CONFIG
# FILELIST=filelists/Dec2/MC_53X
# SKIMPATH=/Volumes/Storage/Dec2/MC_53X
# PREFIX=/internal/Dec2/MC_53X/
# PATHS=(
#      'DYJetsToLL'
#      'TTJets'
#      'T-tW'
#      'Tbar-tW'
#      'WWJetsTo2L2Nu'
#      'WZJetsTo2L2Q'
#      'WZJetsTo3LNu'
#      'ZZJetsTo2L2Nu'
#      'ZZJetsTo2L2Q'
#      'ZZJetsTo4L'
#      'GluGluToHToTauTau_M-125'
#      'VBF_HToTauTau_M-125'
#      'WH_ZH_TTH_HToTauTau_M-125'
#      'SUSYGluGluToHToTauTau_M-160'
#      'SUSYBBHToTauTau_M-160'
#      )
#  for i in "${PATHS[@]}"
#  do
#      eval `scramv1 runtime -sh`
#      echo "$i"
#      ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=em --output_folder=./ --output_name=Dummy.root \
#      --do_skim=true --skim_path=$SKIMPATH/$i/em_skim/ --svfit_mode=0 --input_prefix=$PREFIX
#      wait
#      source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc46-opt/root/bin/thisroot.sh
#      cd $SKIMPATH/$i/em_skim/
#      hadd -O TempTree.root EventTree*
#      rm EventTree*.root
#      mv TempTree.root EventTree_All.root
#      cd -
#  done


