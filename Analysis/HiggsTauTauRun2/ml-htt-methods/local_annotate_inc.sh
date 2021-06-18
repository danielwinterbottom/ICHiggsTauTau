# keras for local application
for loop in {1..64}
do 
    inputNumber=$(printf "%04d\n" ${loop})
    nohup python annotate_file_inc_keras.py filelist/tmp_full_grouped/tt/x${inputNumber} IC_keras_sm4 --model_folder May28_training_tt_2016_keras_inc/ --training powheg --era 2016 --channel tt --config-training May28_training_tt_2016_keras_inc//tt_keras_training_2016_config_inc.yaml &> filelist/tmp_full_grouped/tt/${inputNumber}.log &
    # nohup python annotate_file_inc_keras_others.py filelist/tmp_full_grouped/tt/x${inputNumber} IC_Vienna_test2 --model_folder vienna_tt_keras_inc/ --training powheg --era 2016 --channel tt --config-training vienna_tt_keras_inc/tt_keras_training_2016_config_vienna_inc.yaml &> filelist/tmp_full_grouped/tt/${inputNumber}.log &
done
