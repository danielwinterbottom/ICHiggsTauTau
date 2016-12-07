#!/bin/bash

declare -a channel=("em" "et" "mt" "tt")
#declare -a channel=("em" )
#declare -a var=( "mjj" "n_jets" "n_jetsingap" "jdeta" "jpt_1" "jpt_2" "pt_tt" "pt_1" "pt_2" "mt_1" "n_bjets" "mva_olddm_tight_2" "mva_olddm_vloose_2" "m_vis" )
declare -a var=("iso_1")
declare -a sig=("VBF" "GluGlu")

export count=0
export location=$(pwd)
echo cd $location > jobs/job.sh
echo source /vols/grid/cms/setup.sh >> jobs/job.sh
echo export SCRAM_ARCH=slc6_amd64_gcc481 >> jobs/job.sh
echo eval \`scramv1 runtime -sh\` >> jobs/job.sh

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
    export input="/vols/cms/dw515/Offline/output/PartonShower6/"$i"HToTauTau_M-125_"$j"_2016.root"
  
    #for k in "${var[@]}"
    #do
    #  cp jobs/job.sh jobs/job_$count.sh
    #  echo ./bin/TheoryUncertainties --doScale=1 --doPDF=1 --doAlphaS=1 --doGen=0 --input=$input --variable=$k --RecreateRenorm=0 --makePlots=1 --channel=$j --outputDirname=$output >>jobs/job_$count.sh
    #  chmod 755 jobs/job_$count.sh
    #  qsub -q hep.q -l h_rt=0:60:0 -cwd jobs/job_$count.sh
    #  ((count++))
    #done
    
    #cp jobs/job.sh jobs/job_$count.sh
    #echo ./bin/TheoryUncertainties --do2D=1 --doGen=0 --makePlots=0 --channel=$j --input=$input --outputDirname=$output --outputFilename=2d_$outputfile >>jobs/job_$count.sh
    #chmod 755 jobs/job_$count.sh
    #qsub -q hep.q -l h_rt=0:60:0 -cwd jobs/job_$count.sh
    #((count++))
    #cp jobs/job.sh jobs/job_$count.sh
    #echo ./bin/TheoryUncertainties --do2D=0 --doGen=0 --makePlots=0 --channel=$j --input=$input --outputDirname=$output --outputFilename=$outputfile >>jobs/job_$count.sh
    #chmod 755 jobs/job_$count.sh
    #qsub -q hep.q -l h_rt=0:60:0 -cwd jobs/job_$count.sh
    #((count++))
    
    cp jobs/job.sh jobs/job_$count.sh
    echo ./bin/TheoryUncertainties --do2D=1 --doGen=1 --makePlots=0 --channel=$j --input=$input --outputDirname=$output --outputFilename=2d_gen_$outputfile >>jobs/job_$count.sh
    chmod 755 jobs/job_$count.sh
    qsub -q hep.q -l h_rt=0:500:0 -cwd jobs/job_$count.sh
    ((count++))
    cp jobs/job.sh jobs/job_$count.sh
    echo ./bin/TheoryUncertainties --do2D=0 --doGen=1 --makePlots=0 --channel=$j --input=$input --outputDirname=$output --outputFilename=gen_$outputfile >>jobs/job_$count.sh
    chmod 755 jobs/job_$count.sh
    qsub -q hep.q -l h_rt=0:500:0 -cwd jobs/job_$count.sh
    ((count++))
    
  done
done
