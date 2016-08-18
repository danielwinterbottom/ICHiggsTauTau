#!/bin/bash
#
export channel=$1
IFS='%'
export now=$(date +"%Y_%m_%d_%H_%M")
export dirname=$channel"_"$now
mkdir -p $dirname
rm $dirname/cutsInputTemp_$channel.txt
declare -a dEta=( 3 3.5 4 )
declare -a Mjj=( 400 450 500 550 600 650 700 750 800 )
declare -a HPt=( 0 10 20 30 40 50 60 70 80 90 100 110 120 130 )
declare -a btag=("&& n_bjets==0")
declare -a jetpt=("&& jpt_1>20. && jpt_2>20." "&& jpt_1>30. && jpt_2>30.")
export line_count=1
for i in "${dEta[@]}"; do for j in "${Mjj[@]}"; do for k in "${HPt[@]}"; do for l in "${btag[@]}"; do for z in "${jetpt[@]}"; do
  export output_line="n_lowpt_jets>=2 && jdeta_lowpt>"$i" && mjj_lowpt>"$j" && pt_tt>="$k" "$l" "$z
  echo $output_line >> $dirname/cutsInputTemp_$channel.txt
  ((line_count++))
done; done; done; done; done;
qsub -q hep.q -l h_rt=0:60:0 -t 1-$line_count:1 scripts/runOptimizeCatogories.sh $channel $dirname
