#!/bin/bash

declare -a channel=("em" "et" "mt" "tt")
declare -a var=( "mjj" "n_jets" "n_jetsingap" "jdeta" "jpt_1" "jpt_2" "pt_tt" "pt_1" "pt_2" "mt_1" "n_bjets")
declare -a sig=("GluGlu" "VBF")

for i in "${sig[@]}"
do
  for j in "${channel[@]}"
  do
    if [ $i == "GluGlu" ]; then
      export output="theory_output_gluglu_"$j"/"
      export outputfile="uncertainties__gluglu_"$j".txt"
    elif [ $i == "VBF" ]; then
      export output="theory_output_vbf_"$j"/"
      export outputfile="uncertainties__vbf_"$j".txt"
    fi
    export input="/vols/cms/dw515/Offline/output/PartonShower3/"$i"HToTauTau_M-125_"$j"_2016.root"
  
    for k in "${var[@]}"
    do
      ./bin/TheoryUncertainties --doScale=1 --doPDF=1 --doAlphaS=1 --doGen=0 --input=$input --variable=$k --RecreateRenorm=0 --makePlots=1 --channel=$j --outputDirname=$output
      ./bin/TheoryUncertainties --doScale=1 --doPDF=1 --doAlphaS=1 --doGen=1 --input=$input --variable=$k --RecreateRenorm=0 --makePlots=1 --channel=$j --outputDirname=$output 
    done
    
    ./bin/TheoryUncertainties --do2D=1 --doGen=0 --makePlots=0 --channel=$j --input=$input --outputDirname=$output --outputFilename=2d_$outputfile
    ./bin/TheoryUncertainties --do2D=0 --doGen=0 --makePlots=0 --channel=$j --input=$input --outputDirname=$output --outputFilename=$outputfile
    
    ./bin/TheoryUncertainties --do2D=1 --doGen=1 --makePlots=0 --channel=$j --input=$input --outputDirname=$output --outputFilename=2d_gen_$outputfile
    ./bin/TheoryUncertainties --do2D=0 --doGen=1 --makePlots=0 --channel=$j --input=$input --outputDirname=$output --outputFilename=gen_$outputfile
    
    #cp $output/2d_$outputfile uncerts_2d/.
    #cp $output/$outputfile uncerts_cats/.
  done
done
