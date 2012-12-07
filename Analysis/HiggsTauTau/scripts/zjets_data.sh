#make -j4

CONFIG=scripts/zjets_data.cfg
echo $CONFIG

FILELIST=filelists/Nov26/Data_53X


PATHS=(
    'DoubleElectron-2012A-13Jul2012-v1'
    'DoubleElectron-2012B-13Jul2012-v1'
    'DoubleElectron-2012C-24Aug2012-v1'
    'DoubleElectron-2012C-PromptReco-v2'
    'DoubleElectron-2012D-PromptReco-v1'
    #'DoubleMu-2012A-13Jul2012-v1'
    #'DoubleMu-2012B-13Jul2012-v4'
    #'DoubleMu-2012C-24Aug2012-v1'
    #'DoubleMu-2012C-PromptReco-v2'
    #'DoubleMu-2012D-PromptReco-v1'
    )
 for i in "${PATHS[@]}"
 do
     echo "$i"
     ./bin/ZJets --cfg=$CONFIG --filelist=$FILELIST/"$i"_filelist.dat --channel=zee --output_name="$i"_zee_2012.root
     wait
 done


