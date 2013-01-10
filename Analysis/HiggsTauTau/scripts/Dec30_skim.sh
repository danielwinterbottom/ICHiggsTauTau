JOBWRAPPER=./scripts/generate_job.sh
#JOBSUBMIT=true
JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"


# CONFIG=scripts/Moriond_data_2012.cfg
# echo $CONFIG
# FILELIST=filelists/Dec30_Data_53X
# SKIMPATH=$SSD/Dec30/Data_53X
# PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/Dec30/Data_53X/
# 
# PATHS=(
#      'TauPlusX-2012A-13Jul2012-v1'
#      'TauPlusX-2012A-recover-06Aug2012-v1'
#      'TauPlusX-2012B-13Jul2012-v1'
#      'TauPlusX-2012C-24Aug2012-v1'
#      'TauPlusX-2012C-PromptReco-v2'
#      'TauPlusX-2012D-PromptReco-v1'
#     )
#  for i in "${PATHS[@]}"
#  do
#      echo "$i"
# 
#      mkdir -p $SKIMPATH/$i/et_skim/
#      mkdir -p $SKIMPATH/$i/mt_skim/
#      mkdir -p $SKIMPATH/Special_2_$i/et_skim/
#      mkdir -p $SKIMPATH/Special_2_$i/mt_skim/
#     
#      JOB="$i"_et_skim
#      $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=et --output_folder=./ --output_name=Dummy_$JOB.root \
#      --do_skim=true --skim_path=$SKIMPATH/$i/et_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#      ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/et_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#      $JOBSUBMIT jobs/$JOB.sh
# 
#      JOB="$i"_mt_skim
#      $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
#      --do_skim=true --skim_path=$SKIMPATH/$i/mt_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#      ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/mt_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#      $JOBSUBMIT jobs/$JOB.sh
# 
#      JOB=Special_2_"$i"_et_skim
#      $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=et --output_folder=./ --output_name=Dummy_$JOB.root \
#      --do_skim=true --skim_path=$SKIMPATH/Special_2_"$i"/et_skim/ --special_mode=2 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#      ./scripts/hadd_and_filelist.sh $SKIMPATH Special_2_"$i"/et_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#      $JOBSUBMIT jobs/$JOB.sh
#      
#      JOB=Special_2_"$i"_mt_skim
#      $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
#      --do_skim=true --skim_path=$SKIMPATH/Special_2_"$i"/mt_skim/ --special_mode=2 --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#      ./scripts/hadd_and_filelist.sh $SKIMPATH Special_2_"$i"/mt_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#      $JOBSUBMIT jobs/$JOB.sh
# done


# PATHS=(
#      'Embedded-ETau-2012A-13Jul2012-v1'
#      'Embedded-ETau-2012A-recover-06Aug2012-v1'
#      'Embedded-ETau-2012B-13Jul2012-v1'
#      'Embedded-ETau-2012C-24Aug2012-v1'
#      'Embedded-ETau-2012C-PromptReco-v2'
#      'Embedded-ETau-2012D-PromptReco-v1'
#      )
# for i in "${PATHS[@]}"
# do
#     echo "$i"
# 
#     mkdir -p $SKIMPATH/$i/et_skim/
#      
#     JOB="$i"_et_skim
#     $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=et --output_folder=./ --output_name=Dummy_$JOB.root \
#     --do_skim=true --skim_path=$SKIMPATH/$i/et_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#     ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/et_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#     $JOBSUBMIT jobs/$JOB.sh
# done
# 
# PATHS=(
#      'Embedded-MTau-2012A-13Jul2012-v1'
#      'Embedded-MTau-2012A-recover-06Aug2012-v1'
#      'Embedded-MTau-2012B-13Jul2012-v1'
#      'Embedded-MTau-2012C-24Aug2012-v1'
#      'Embedded-MTau-2012C-PromptReco-v2'
#      'Embedded-MTau-2012D-PromptReco-v1'
#      )
# for i in "${PATHS[@]}"
# do
#     echo "$i"
# 
#     mkdir -p $SKIMPATH/$i/mt_skim/
# 
#     JOB="$i"_mt_skim
#     $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
#     --do_skim=true --skim_path=$SKIMPATH/$i/mt_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#     ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/mt_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#     $JOBSUBMIT jobs/$JOB.sh
# done

 
CONFIG=scripts/Moriond_mc_2012.cfg
echo $CONFIG
FILELIST=filelists/Dec30_MC_53X
SKIMPATH=$SSD/Dec30/MC_53X
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/Dec30/MC_53X/

PATHS=(
     'DYJetsToLL'
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
     'GluGluToHToTauTau_M-125'
     'VBF_HToTauTau_M-125'
     'WH_ZH_TTH_HToTauTau_M-125'
     'SUSYGluGluToHToTauTau_M-160'
     'SUSYBBHToTauTau_M-160'
     )
 for i in "${PATHS[@]}"
 do
     echo "$i"

     mkdir -p $SKIMPATH/$i/et_skim/
     mkdir -p $SKIMPATH/$i/mt_skim/
     
     JOB="$i"_et_skim
     $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=et --output_folder=./ --output_name=Dummy_$JOB.root \
     --do_skim=true --skim_path=$SKIMPATH/$i/et_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
     ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/et_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
     $JOBSUBMIT jobs/$JOB.sh

     JOB="$i"_mt_skim
     $JOBWRAPPER "./bin/HiggsTauTau --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_folder=./ --output_name=Dummy_$JOB.root \
     --do_skim=true --skim_path=$SKIMPATH/$i/mt_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
     ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/mt_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
     $JOBSUBMIT jobs/$JOB.sh
done

