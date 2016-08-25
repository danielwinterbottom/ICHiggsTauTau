#!/bin/bash
#
export channel=$1
IFS='%'
export now=$(date +"%Y_%m_%d_%H_%M")
export dirname="2Jet/2Cat/tight/"$channel"_"$now
mkdir -p $dirname
rm $dirname/cutsInputTemp_$channel.txt
declare -a mt=( 20 30 40 )
if [ $channel == "tt" ]; then
  declare -a tau_id=( "loose" "medium" "tight" "vtight" )
else
  declare -a tau_id=( "loose" "medium" "tight" "vtight" )
fi
#declare -a dEta=( 3.5 4 )
#declare -a Mjj=( 600 650 700 750 800 )
#declare -a HPt=( 70 80 90 100 110 120 130 )
#declare -a btag=("&& n_bjets==0")
#declare -a jetpt=("&& jpt_1>20. && jpt_2>20." "&& jpt_1>30. && jpt_2>30.")
declare -a dEta=( 4 )
declare -a Mjj=( 600 700 800 )
declare -a HPt=( 100 150 )
declare -a btag=("&& n_bjets==0")
declare -a jetpt=( "&& jpt_1>30. && jpt_2>30." )
export line_count=0
for i in "${dEta[@]}"; do for j in "${Mjj[@]}"; do for k in "${HPt[@]}"; do for l in "${btag[@]}"; do for z in "${jetpt[@]}"; do
  if [ $channel == "tt" ]; then
    export output_line="n_lowpt_jets>=2 && jdeta_lowpt>"$i" && mjj_lowpt>"$j" && pt_tt>="$k" "$l" "$z
    echo $output_line >> $dirname/cutsInputTemp_$channel.txt
    ((line_count++))
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

export dirname="2Jet/2Cat/loose/"$channel"_"$now
mkdir -p $dirname
rm $dirname/cutsInputTemp_$channel.txt
declare -a dEta_loose=( 3 )
declare -a Mjj_loose=( 400 500 600 )
declare -a HPt_loose=( 0 50 )
declare -a dEta=( 4 )
declare -a Mjj=( 600 700 800 )
declare -a HPt=( 100 150 )
declare -a btag=("&& n_bjets==0")
declare -a jetpt=("&& jpt_1>30. && jpt_2>30.")
export line_count=0
for q in "${dEta_loose[@]}"; do for w in "${Mjj_loose[@]}"; do for e in "${HPt_loose[@]}"; do for i in "${dEta[@]}"; do for j in "${Mjj[@]}"; do for k in "${HPt[@]}"; do for l in "${btag[@]}"; do for z in "${jetpt[@]}"; do 
  if [ $q \< $i -a $w \< $j -a $e \< $k ]; then
    if [ $channel == "tt" ]; then
      export output_line="n_lowpt_jets>=2 && jdeta_lowpt>"$q" && mjj_lowpt>"$w" && pt_tt>="$e" "$l" "$z" && !(n_lowpt_jets>=2 && jdeta_lowpt>"$i" && mjj_lowpt>"$j" && pt_tt>="$k" "$l" "$z")"
      echo $output_line >> $dirname/cutsInputTemp_$channel.txt
      ((line_count++))
    else
      for v in "${mt[@]}"; do
        export output_line="mt_1<"$v" && n_lowpt_jets>=2 && jdeta_lowpt>"$q" && mjj_lowpt>"$w" && pt_tt>="$e" "$l" "$z" && !(mt_1<"$v" && n_lowpt_jets>=2 && jdeta_lowpt>"$i" && mjj_lowpt>"$j" && pt_tt>="$k" "$l" "$z")"
        echo $output_line >> $dirname/cutsInputTemp_$channel.txt
        ((line_count++))
      done;
    fi
  fi
done; done; done; done; done; done; done; done;
for c in "${tau_id[@]}"; do
  export mva=$c
  qsub -q hep.q -l h_rt=0:30:0 -t 1-$line_count:1 scripts/runOptimizeCatogories.sh $channel $dirname $mva
done
