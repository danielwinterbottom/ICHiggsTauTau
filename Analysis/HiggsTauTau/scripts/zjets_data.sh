#make -j4

CONFIG=scripts/zjets_data.cfg
echo $CONFIG
FILELIST=filelists/Nov26/Data_53X
PATHS=(
    'Data_Moriond'
    )
 for i in "${PATHS[@]}"
 do
     echo "$i"
     ./bin/ZJets --cfg=$CONFIG --filelist=$FILELIST/"$i"_zee_skim_filelist.dat --channel=zee --output_name="$i"_zee_NewJEC_2012.root
     ./bin/ZJets --cfg=$CONFIG --filelist=$FILELIST/"$i"_zmm_skim_filelist.dat --channel=zmm --output_name="$i"_zmm_NewJEC_2012.root
     wait
 done


 CONFIG=scripts/zjets_mc.cfg
 echo $CONFIG
 FILELIST=filelists/Dec2/MC_53X
 PATHS=(
     'DYJetsToLL'
     'TTJets'
     )
  for i in "${PATHS[@]}"
  do
      echo "$i"
      ./bin/ZJets --cfg=$CONFIG --filelist=$FILELIST/"$i"_zee_skim_filelist.dat --channel=zee --output_name="$i"_zee_NewJEC_2012.root
      ./bin/ZJets --cfg=$CONFIG --filelist=$FILELIST/"$i"_zmm_skim_filelist.dat --channel=zmm --output_name="$i"_zmm_NewJEC_2012.root
      wait
  done




