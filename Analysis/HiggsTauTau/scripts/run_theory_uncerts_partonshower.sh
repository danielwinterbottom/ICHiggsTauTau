#!/bin/bash

declare -a channel=("em" "et" "mt" "tt")
#declare -a channel=("mt")
declare -a sig=("GluGlu" "VBF")
declare -a var=( "mjj" "n_jets" "n_jetsingap" "jdeta" "jpt_1" "jpt_2" "pt_tt" "pt_1" "pt_2" "mt_1" "n_bjets")
declare -a input=( "" "_PythiaFragment_Down" "_PythiaFragment_Up" "_herwigpp" "_amcatnlo")

for i in "${sig[@]}"
do
  for j in "${channel[@]}"
  do
    for k in "${input[@]}"
    do
      export doGen="0"
      if [ "$k" == "" ]; then
        export samp="default"
      elif [ "$k" == "_PythiaFragment_Down" ]; then
        export samp="pythia_down"
      elif [ "$k" == "_PythiaFragment_Up" ]; then
        export samp="pythia_up"
      elif [ "$k" == "_herwigpp" ]; then
        export samp="herwigpp"
      elif [ "$k" == "_amcatnlo" ]; then
        export samp="amcatnlo"
        export doGen="1"
      fi
      
      if [ $i == "GluGlu" ]; then
        export output="theory_output_gluglu_partonshower/"
        export output_name="uncertainties_gluglu_"$j"_"$k".txt"
      elif [ $i == "VBF" ]; then
        export output="theory_output_vbf_partonshower/"
        export output_name="uncertainties_vbf_"$j"_"$k".txt"
      fi
      export input_name="/vols/cms/dw515/Offline/output/PartonShower3/""$i""HToTauTau""$k""_M-125_""$j""_2016.root"
      ./bin/TheoryUncertainties --doGen=0 --doPDF=0 --doScale=0 --doAlphaS=0 --doAcceptance=1 --makePlots=0 --channel=$j --input=$input_name --outputDirname=$output --outputFilename=$output_name
      ./bin/TheoryUncertainties --do2D=1 --doGen=0 --doPDF=0 --doScale=0 --doAlphaS=0 --doAcceptance=1 --makePlots=0 --channel=$j --input=$input_name --outputDirname=$output --outputFilename=2d_$output_name
      
      #cp $output/2d_$output_name uncerts_2d/partonshower_$output_name
      #cp $output/$output_name uncerts_cats/partonshower_$output_name
       
      export output_name="gen_"$output_name
      ./bin/TheoryUncertainties --doGen=1 --doPDF=0 --doScale=0 --doAlphaS=0 --doAcceptance=1 --makePlots=0 --channel=$j --input=$input_name --outputDirname=$output --outputFilename=$output_name
      ./bin/TheoryUncertainties --do2D=1 --doGen=1 --doPDF=0 --doScale=0 --doAlphaS=0 --doAcceptance=1 --makePlots=0 --channel=$j --input=$input_name --outputDirname=$output --outputFilename=2d_$output_name
      #cp $output/2d_$output_name uncerts_2d/partonshower_$output_name
      #cp $output/$output_name uncerts_cats/partonshower_$output_name
      
      #for l in "${var[@]}"
      #do
      #  ./bin/TheoryUncertainties --doScale=0 --doPDF=0 --doAlphaS=0 --doGen=0 --doAcceptance=1 --input=$input_name --variable=$l --RecreateRenorm=0 --makePlots=1 --channel=$j --outputDirname=$output --sample=$samp
      #  ./bin/TheoryUncertainties --doScale=0 --doPDF=0 --doAlphaS=0 --doGen=1 --doAcceptance=1 --input=$input_name --variable=$l --RecreateRenorm=0 --makePlots=1 --channel=$j --outputDirname=$output --sample=$samp"_gen"
      #done
    done
  done
done
