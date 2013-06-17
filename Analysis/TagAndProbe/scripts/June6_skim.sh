JOBWRAPPER=./scripts/generate_job.sh
JOBSUBMIT=true
#JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"


 CONFIG=data/cfg_files/2012ElectronSkim.cfg
 echo $CONFIG
 FILELIST=filelists/June6_Data_53X
 SKIMPATH=$SSD/June6/Data_53X
 PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/rlane/June6/Data_53X/


  PATHS=(
       'SingleElectron-2012A-22Jan2013-v1'
       'SingleElectron-2012B-22Jan2013-v1'
       'SingleElectron-2012C-22Jan2013-v1'
       'SingleElectron-2012D-22Jan2013-v1'
       )
  for i in "${PATHS[@]}"
  do
      echo "$i"
 
      mkdir -p $SKIMPATH/tandp_e_skim/$i
      
      JOB="$i"_tandp_e_skim
      $JOBWRAPPER "./bin/TagAndProbe --is_data=1 --is_elec=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat \
      --do_skim=true --skim_path=$SKIMPATH/tandp_e_skim/"$i"/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
      ./scripts/hadd_and_filelist.sh $SKIMPATH tandp_e_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
      $JOBSUBMIT jobs/$JOB.sh
  done
 
 CONFIG=data/cfg_files/2012MuonSkim.cfg
 echo $CONFIG
 PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/June6/Data_53X/
 
 PATHS=(
       'SingleMu-2012A-22Jan2013-v1'
       'SingleMu-2012B-22Jan2013-v1'
       'SingleMu-2012C-22Jan2013-v1'
       'SingleMu-2012D-22Jan2013-v1'
       )
  for i in "${PATHS[@]}"
  do
      echo "$i"
 
      mkdir -p $SKIMPATH/tandp_mu_skim/$i
      
      JOB="$i"_tandp_mu_skim
      $JOBWRAPPER "./bin/TagAndProbe --is_data=1 --is_elec=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat \
      --do_skim=true --skim_path=$SKIMPATH/tandp_mu_skim/"$i"/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
      ./scripts/hadd_and_filelist.sh $SKIMPATH tandp_mu_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
      $JOBSUBMIT jobs/$JOB.sh
  done
 
 CONFIG=data/cfg_files/2012ElectronSkim.cfg
 echo $CONFIG
 PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/rlane/June6/Data_53X/
 
 PATHS=(
       'DYJetsToLL'
       )
  for i in "${PATHS[@]}"
  do
      echo "$i"
 
      mkdir -p $SKIMPATH/tandp_e_skim/$i
      
      JOB="$i"_tandp_e_skim
      $JOBWRAPPER "./bin/TagAndProbe --is_data=0 --is_elec=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat \
      --do_skim=true --skim_path=$SKIMPATH/tandp_e_skim/"$i"/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
      ./scripts/hadd_and_filelist.sh $SKIMPATH tandp_e_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
      $JOBSUBMIT jobs/$JOB.sh
  done
 
 CONFIG=data/cfg_files/2012MuonSkim.cfg
 echo $CONFIG
 PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/rlane/June6/Data_53X/
 
 PATHS=(
       'DYJetsToLL'
       )
  for i in "${PATHS[@]}"
  do
      echo "$i"
 
      mkdir -p $SKIMPATH/tandp_mu_skim/$i
      
      JOB="$i"_tandp_mu_skim
      $JOBWRAPPER "./bin/TagAndProbe --is_data=0 --is_elec=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat \
      --do_skim=true --skim_path=$SKIMPATH/tandp_mu_skim/"$i"/ --input_prefix=$PREFIX &> jobs/$JOB.log ; \
      ./scripts/hadd_and_filelist.sh $SKIMPATH tandp_mu_skim/"$i" "$FILELIST"_"$JOB".dat" jobs/$JOB.sh
      $JOBSUBMIT jobs/$JOB.sh
  done
 
