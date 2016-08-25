#!/bin/bash
#
export channel=$1
IFS='%'
export now=$(date +"%Y_%m_%d_%H_%M")
export dirname="2Jet/1Cat/"$channel"_"$now
mkdir -p $dirname
rm $dirname/cutsInputTemp_$channel.txt
#declare -a dEta=( 3 3.5 4 )
#declare -a Mjj=( 400 450 500 550 600 650 700 750 800 )
#declare -a HPt=( 0 10 20 30 40 50 60 70 80 90 100 110 120 130 )
#declare -a btag=("&& n_bjets==0")
#declare -a jetpt=("&& jpt_1>20. && jpt_2>20." "&& jpt_1>30. && jpt_2>30.")
#declare -a tau1pt=( 0 50 60 70 )
#declare -a tau2pt=( 0 50 )
declare -a mt=( 20 30 40 )
if [ $channel == "tt" ]; then
  declare -a tau_id=( "loose" "medium" "tight" "vtight" )
else
  declare -a tau_id=( "loose" "medium" "tight" "vtight" )
fi
declare -a dEta=( 3 4 )
declare -a Mjj=( 400 500 600 700 800 )
declare -a HPt=( 0 50 100 150 )
declare -a btag=("&& n_bjets==0")
declare -a jetpt=( "&& jpt_1>30. && jpt_2>30." )
declare -a tau1pt=( 0 )
declare -a tau2pt=( 0 )
export line_count=0
for i in "${dEta[@]}"; do for j in "${Mjj[@]}"; do for k in "${HPt[@]}"; do for l in "${btag[@]}"; do for z in "${jetpt[@]}"; do 
  if [ $channel == "tt" ]; then
    for m in "${tau1pt[@]}"; do for n in "${tau2pt[@]}"; do
      if [ $m -ge $n ]; then
        export output_line="n_lowpt_jets>=2 && jdeta_lowpt>"$i" && mjj_lowpt>"$j" && pt_tt>="$k" "$l" "$z" && pt_1>"$m" && pt_2>"$n
        echo $output_line >> $dirname/cutsInputTemp_$channel.txt
        ((line_count++))
      fi
    done; done;
  else
    for v in "${mt[@]}"; do
      export output_line="mt_1<"$v" && n_lowpt_jets>=2 && jdeta_lowpt>"$i" && mjj_lowpt>"$j" && pt_tt>="$k" "$l" "$z
      echo $output_line >> $dirname/cutsInputTemp_$channel.txt
      ((line_count++))
    done;
  fi
done; done; done; done; done;

for c in "${tau_id[@]}"; do
  export mva=$c
  qsub -q hep.q -l h_rt=0:30:0 -t 1-$line_count:1 scripts/runOptimizeCatogories.sh $channel $dirname $mva
done
