# CONFIG=scripts/zjets_data.cfg
# echo $CONFIG
# FILELIST=filelists/Nov26/Data_53X
# SKIMPATH=/Volumes/Storage/Nov26/Data_53X

# PATHS=(
#     # 'DoubleElectron-2012A-13Jul2012-v1'
#     # 'DoubleElectron-2012B-13Jul2012-v1'
#     # 'DoubleElectron-2012C-24Aug2012-v1'
#     # 'DoubleElectron-2012C-PromptReco-v2'
#     # 'DoubleElectron-2012D-PromptReco-v1'
#     )
#  for i in "${PATHS[@]}"
#  do
#      eval `scramv1 runtime -sh`
#      echo "$i"
#      mkdir -p $SKIMPATH/$i/zee_skim/
#      ./bin/ZJets --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=zee --output_name="$i"_zee_2012.root \
#      --do_skim=true --skim_path=$SKIMPATH/$i/zee_skim/
#      wait
#      source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc46-opt/root/bin/thisroot.sh
#      cd $SKIMPATH/$i/zee_skim/
#      hadd -O TempTree.root EventTree*
#      rm EventTree*.root
#      mv TempTree.root EventTree_All.root
#      cd -
#  done


# PATHS=(
#     'DoubleMu-2012A-13Jul2012-v1'
#     'DoubleMu-2012B-13Jul2012-v4'
#     'DoubleMu-2012C-24Aug2012-v1'
#     'DoubleMu-2012C-PromptReco-v2'
#     'DoubleMu-2012D-PromptReco-v1'
#     )
#  for i in "${PATHS[@]}"
#  do
#      eval `scramv1 runtime -sh`
#      echo "$i"
#      mkdir -p $SKIMPATH/$i/zmm_skim/
#      ./bin/ZJets --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=zmm --output_name="$i"_zmm_2012.root \
#      --do_skim=true --skim_path=$SKIMPATH/$i/zmm_skim/
#      wait
#      source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc46-opt/root/bin/thisroot.sh
#      cd $SKIMPATH/$i/zmm_skim/
#      hadd -O TempTree.root EventTree*
#      rm EventTree*.root
#      mv TempTree.root EventTree_All.root
#      cd -
#  done

 CONFIG=scripts/zjets_mc.cfg
 echo $CONFIG
 FILELIST=filelists/Dec2/MC_53X
 SKIMPATH=/Volumes/Storage/Dec2/MC_53X
 PATHS=(
     'DYJetsToLL'
     'TTJets'
     )

 for i in "${PATHS[@]}"
 do
     eval `scramv1 runtime -sh`
     echo "$i"
     mkdir -p $SKIMPATH/$i/zee_skim/
     mkdir -p $SKIMPATH/$i/zmm_skim/
     ./bin/ZJets --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=zee --output_name="$i"_zee_2012.root \
     --do_skim=true --skim_path=$SKIMPATH/$i/zee_skim/
     ./bin/ZJets --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=zmm --output_name="$i"_zmm_2012.root \
     --do_skim=true --skim_path=$SKIMPATH/$i/zmm_skim/
     wait
     source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc46-opt/root/bin/thisroot.sh
     cd $SKIMPATH/$i/zee_skim/
     hadd -O TempTree.root EventTree*
     rm EventTree*.root
     mv TempTree.root EventTree_All.root
     cd -
     cd $SKIMPATH/$i/zmm_skim/
     hadd -O TempTree.root EventTree*
     rm EventTree*.root
     mv TempTree.root EventTree_All.root
     cd -
 done


