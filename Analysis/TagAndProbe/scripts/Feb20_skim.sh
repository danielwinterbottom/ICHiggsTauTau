JOBWRAPPER=./scripts/generate_job.sh
JOBSUBMIT=true
#JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"


# CONFIG=data/cfg_files/2012ElectronSkim.cfg
 echo $CONFIG
 FILELIST=filelists/Feb20_Data_53X
 SKIMPATH=$SSD/Feb20/Data_53X
 PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/Feb20/Data_53X/


#  PATHS=(
#       'SingleElectron-2012A-13Jul2012-v1'
#       'SingleElectron-2012A-recover-06Aug2012-v1'
#       'SingleElectron-2012B-13Jul2012-v1'
#       'SingleElectron-2012C-24Aug2012-v1'
#       'SingleElectron-2012C-PromptReco-v2'
#       'SingleElectron-2012C-EcalRecover-11Dec2012-v1'
#       'SingleElectron-2012D-PromptReco-v1'
#       )
#  for i in "${PATHS[@]}"
#  do
#      echo "$i"
# 
#      mkdir -p $SKIMPATH/$i/tandp_e_skim/
#      
#      JOB="$i"_tandp_e_skim
#      $JOBWRAPPER "./bin/TagAndProbe --is_data=1 --is_elec=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat \
#      --do_skim=true --skim_path=$SKIMPATH/$i/tandp_e_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
#      ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/tandp_e_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
#      $JOBSUBMIT jobs/$JOB.sh
#  done
 
 CONFIG=data/cfg_files/2012MuonSkim.cfg
 
 PATHS=(
       'SingleMu-2012A-13Jul2012-v1'
#       'SingleMu-2012A-recover-06Aug2012-v1'
#       'SingleMu-2012B-13Jul2012-v1'
#       'SingleMu-2012C-24Aug2012-v1'
#       'SingleMu-2012C-PromptReco-v2'
#       'SingleMu-2012C-EcalRecover-11Dec2012-v1'
#       'SingleMu-2012D-PromptReco-v1'
       )
  for i in "${PATHS[@]}"
  do
      echo "$i"
 
      mkdir -p $SKIMPATH/$i/tandp_mu_skim/
      
      JOB="$i"_tandp_mu_skim
      $JOBWRAPPER "./bin/TagAndProbe --is_data=1 --is_elec=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat \
      --do_skim=true --skim_path=$SKIMPATH/$i/tandp_mu_skim/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
      ./scripts/hadd_and_filelist.sh $SKIMPATH "$i"/tandp_mu_skim "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
      $JOBSUBMIT jobs/$JOB.sh
  done
 
