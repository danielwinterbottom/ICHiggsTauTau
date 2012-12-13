CONFIG=scripts/moriond_data_2012.cfg
echo $CONFIG
#FILELIST=filelists/Dec2/Data_53X
#SKIMPATH=/Volumes/Storage/Dec2/Data_53X
#PREFIX=/internal/Dec2/Data_53X/
#PATHS=(
#     'TauPlusX-2012A-13Jul2012-v1'
#     'TauPlusX-2012A-recover-06Aug2012-v1'
#     'TauPlusX-2012B-13Jul2012-v1'
#     'TauPlusX-2012C-24Aug2012-v1'
#     'TauPlusX-2012C-PromptReco-v2'
#     'TauPlusX-2012D-PromptReco-v1'
#    )
# for i in "${PATHS[@]}"
# do
#     eval `scramv1 runtime -sh`
#     echo "$i"
#     mkdir -p $SKIMPATH/$i/et_skim/
#     mkdir -p $SKIMPATH/$i/mt_skim/
#     ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=et --output_folder=./ --output_name=Dummy1.root \
#     --do_skim=true --skim_path=$SKIMPATH/$i/et_skim/ --svfit_mode=0 --input_prefix=$PREFIX &> "$i"_et_skim.log &
#     ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=mt --output_folder=./ --output_name=Dummy2.root \
#     --do_skim=true --skim_path=$SKIMPATH/$i/mt_skim/ --svfit_mode=0 --input_prefix=$PREFIX &> "$i"_mt_skim.log &
#     wait
#     source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc46-opt/root/bin/thisroot.sh
#     cd $SKIMPATH/$i/et_skim/
#     hadd -O TempTree.root EventTree*
#     rm EventTree*.root
#     mv TempTree.root EventTree_All.root
#     cd -
#     cd $SKIMPATH/$i/mt_skim/
#     hadd -O TempTree.root EventTree*
#     rm EventTree*.root
#     mv TempTree.root EventTree_All.root
#     cd -
#done
#
#for i in "${PATHS[@]}"
#do
#     eval `scramv1 runtime -sh`
#     echo "$i"
#     mkdir -p $SKIMPATH/Special_2_$i/et_skim/
#     mkdir -p $SKIMPATH/Special_2_$i/mt_skim/
#     ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=et --output_folder=./ --output_name=Dummy1.root \
#     --do_skim=true --skim_path=$SKIMPATH/Special_2_$i/et_skim/ --svfit_mode=0 --special_mode=2 --input_prefix=$PREFIX &> Special_2_"$i"_et_skim.log &
#     ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=mt --output_folder=./ --output_name=Dummy2.root \
#     --do_skim=true --skim_path=$SKIMPATH/Special_2_$i/mt_skim/ --svfit_mode=0 --special_mode=2 --input_prefix=$PREFIX &> Special_2_"$i"_mt_skim.log &
#     wait
#     source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc46-opt/root/bin/thisroot.sh
#     cd $SKIMPATH/Special_2_$i/et_skim/
#     hadd -O TempTree.root EventTree*
#     rm EventTree*.root
#     mv TempTree.root EventTree_All.root
#     cd -
#     cd $SKIMPATH/Special_2_$i/mt_skim/
#     hadd -O TempTree.root EventTree*
#     rm EventTree*.root
#     mv TempTree.root EventTree_All.root
#     cd -
#done
#
#PATHS=(
#     'TauPlusX-2012D-PromptReco-v1'
#    )
# for i in "${PATHS[@]}"
# do
#     eval `scramv1 runtime -sh`
#     echo "$i"
#     mkdir -p $SKIMPATH/$i/mtmet_skim/
#     ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=mtmet --output_folder=./ --output_name=Dummy2.root \
#     --do_skim=true --skim_path=$SKIMPATH/$i/mtmet_skim/ --svfit_mode=0 --input_prefix=$PREFIX &> "$i"_mtmet_skim.log &
#     wait
#     source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc46-opt/root/bin/thisroot.sh
#     cd $SKIMPATH/$i/mtmet_skim/
#     hadd -O TempTree.root EventTree*
#     rm EventTree*.root
#     mv TempTree.root EventTree_All.root
#     cd -
#done
#for i in "${PATHS[@]}"
#do
#     eval `scramv1 runtime -sh`
#     echo "$i"
#     mkdir -p $SKIMPATH/Special_2_$i/mtmet_skim/
#     ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=mtmet --output_folder=./ --output_name=Dummy2.root \
#     --do_skim=true --skim_path=$SKIMPATH/Special_2_$i/mtmet_skim/ --svfit_mode=0 --special_mode=2 --input_prefix=$PREFIX &> Special_2_"$i"_mtmet_skim.log &
#     wait
#     source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc46-opt/root/bin/thisroot.sh
#     cd $SKIMPATH/Special_2_$i/mtmet_skim/
#     hadd -O TempTree.root EventTree*
#     rm EventTree*.root
#     mv TempTree.root EventTree_All.root
#     cd -
#done


FILELIST=filelists/Dec2/Embedded_53X
SKIMPATH=/Volumes/Storage/Dec2/Embedded_53X
PREFIX=/internal/Dec3/Embedded_53X/
PATHS=(
     'Embedded-ETau-2012A-13Jul2012-v1'
     'Embedded-ETau-2012A-recover-06Aug2012-v1'
     'Embedded-ETau-2012B-13Jul2012-v1'
     'Embedded-ETau-2012C-24Aug2012-v1'
     'Embedded-ETau-2012C-PromptReco-v2'
     )
for i in "${PATHS[@]}"
do
    eval `scramv1 runtime -sh`
    echo "$i"
    mkdir -p $SKIMPATH/$i/et_skim/
    ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=et --output_folder=./ --output_name=Dummy.root \
    --do_skim=true --skim_path=$SKIMPATH/$i/et_skim/ --svfit_mode=0 --input_prefix=$PREFIX &> "$i"_et_skim.log &
    wait
    source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc46-opt/root/bin/thisroot.sh
    cd $SKIMPATH/$i/et_skim/
    hadd -O TempTree.root EventTree*
    rm EventTree*.root
    mv TempTree.root EventTree_All.root
    cd -
done

PATHS=(
     'Embedded-MTau-2012A-13Jul2012-v1'
     'Embedded-MTau-2012A-recover-06Aug2012-v1'
     'Embedded-MTau-2012B-13Jul2012-v1'
     'Embedded-MTau-2012C-24Aug2012-v1'
     'Embedded-MTau-2012C-PromptReco-v2'
     )
for i in "${PATHS[@]}"
do
    eval `scramv1 runtime -sh`
    echo "$i"
    mkdir -p $SKIMPATH/$i/mt_skim/
    ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=mt --output_folder=./ --output_name=Dummy.root \
    --do_skim=true --skim_path=$SKIMPATH/$i/mt_skim/ --svfit_mode=0 --input_prefix=$PREFIX &> "$i"_mt_skim.log &
    wait
    source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc46-opt/root/bin/thisroot.sh
    cd $SKIMPATH/$i/mt_skim/
    hadd -O TempTree.root EventTree*
    rm EventTree*.root
    mv TempTree.root EventTree_All.root
    cd -
done


#CONFIG=scripts/moriond_mc_2012.cfg
#echo $CONFIG
#FILELIST=filelists/Dec2/MC_53X
#SKIMPATH=/Volumes/Storage/Dec2/MC_53X
#PREFIX=/internal/Dec2/MC_53X/
#PATHS=(
#     'DYJetsToLL'
#     'WJetsToLNu-v1'
#     'WJetsToLNu-v2-PART1'
#     'WJetsToLNu-v2-PART2'
#     'W1JetsToLNu'
#     'W2JetsToLNu'
#     'W3JetsToLNu'
#     'W4JetsToLNu'
#     'TTJets'
#     'T-tW'
#     'Tbar-tW'
#     'WWJetsTo2L2Nu'
#     'WZJetsTo2L2Q'
#     'WZJetsTo3LNu'
#     'ZZJetsTo2L2Nu'
#     'ZZJetsTo2L2Q'
#     'ZZJetsTo4L'
#     'GluGluToHToTauTau_M-125'
#     'VBF_HToTauTau_M-125'
#     'WH_ZH_TTH_HToTauTau_M-125'
#     'SUSYGluGluToHToTauTau_M-160'
#     'SUSYBBHToTauTau_M-160'
#     )
# for i in "${PATHS[@]}"
# do
#     eval `scramv1 runtime -sh`
#     echo "$i"
#     mkdir -p $SKIMPATH/$i/et_skim/
#     mkdir -p $SKIMPATH/$i/mt_skim/
#     mkdir -p $SKIMPATH/$i/mtmet_skim/
#     ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=et --output_folder=./ --output_name=Dummy1.root \
#     --do_skim=true --skim_path=$SKIMPATH/$i/et_skim/ --svfit_mode=0 --input_prefix=$PREFIX &> "$i"_et_skim.log &
#     ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=mt --output_folder=./ --output_name=Dummy2.root \
#     --do_skim=true --skim_path=$SKIMPATH/$i/mt_skim/ --svfit_mode=0 --input_prefix=$PREFIX &> "$i"_mt_skim.log &
#     ./bin/HiggsTauTau --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=mtmet --output_folder=./ --output_name=Dummy3.root \
#     --do_skim=true --skim_path=$SKIMPATH/$i/mtmet_skim/ --svfit_mode=0 --input_prefix=$PREFIX &> "$i"_mtmet_skim.log &
#     wait
#     source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc46-opt/root/bin/thisroot.sh
#     cd $SKIMPATH/$i/et_skim/
#     hadd -O TempTree.root EventTree*
#     rm EventTree*.root
#     mv TempTree.root EventTree_All.root
#     cd -
#     cd $SKIMPATH/$i/mt_skim/
#     hadd -O TempTree.root EventTree*
#     rm EventTree*.root
#     mv TempTree.root EventTree_All.root
#     cd -
#     cd $SKIMPATH/$i/mtmet_skim/
#     hadd -O TempTree.root EventTree*
#     rm EventTree*.root
#     mv TempTree.root EventTree_All.root
#     cd -
# done
#
#
