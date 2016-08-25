#!/bin/bash
#
export channel=$1
IFS='%'
export now=$(date +"%Y_%m_%d_%H_%M")
export dirname="1Jet/1Cat/"$channel"_"$now
mkdir -p $dirname
rm $dirname/cutsInputTemp_$channel.txt
declare -a mt=( 20 30 40 )
if [ $channel == "tt" ]; then
  declare -a tau_id=( "loose" "medium" "tight" "vtight" )
else
  declare -a tau_id=( "loose" "medium" "tight" "vtight" )
fi
declare -a dEta_loose=( 3 )
declare -a Mjj_loose=( 400 500 )
declare -a HPt_loose=( 0 )
declare -a btag=("&& n_bjets==0")
declare -a jetpt=( 30 )
declare -a leadjetpt=( 30 )

declare -a taupt_2=( 0 40 60 )
declare -a taupt_tt_2=( 0 50 70 )
declare -a HPt_2=( 0 50 100 150)
declare -a HPt_tt_2=( 0 50 100 150 200 )
declare -a met_2=( 0 30 50 )
declare -a tau2pt_2=( 0 )

#declare -a dEta_loose=( 3 )
#declare -a Mjj_loose=( 400 450 )
#declare -a HPt_loose=( 0 )
#declare -a btag=("&& n_bjets==0")
#declare -a jetpt=( 30 )
#declare -a leadjetpt=( 30 )
#
#declare -a taupt_2=( 0 30 40 50 60 )
#declare -a taupt_tt_2=( 0 30 40 50 60 70 )
#declare -a HPt_2=( 0 20 40 50 60 70 80 90 100 110 120)
#declare -a HPt_tt_2=( 0 50 100 150 200 )
#declare -a met_2=( 0 30 40 50 )
#declare -a tau2pt_2=( 0 30 40 50 60. )
export line_count=0
for q in "${dEta_loose[@]}"; do for w in "${Mjj_loose[@]}"; do for e in "${HPt_loose[@]}"; do for l in "${btag[@]}"; do for z in "${jetpt[@]}"; do for i in "${leadjetpt[@]}"; do
  if [ $i -ge $z ]; then
    if [ $channel == "et" ]; then
      for k in "${HPt_2[@]}"; do for m in "${met_2[@]}"; do for j in "${taupt_2[@]}"; do for v in "${mt[@]}"; do
        export output_line="mt_1<"$v" && n_lowpt_jets>=1 && jpt_1>"$i" && pt_2>"$j" && pt_tt>"$k" && met>"$m" "$l" && !(mt_1<"$v" && n_lowpt_jets>=2 && jdeta_lowpt>"$q" && mjj_lowpt>"$w" && pt_tt>="$e" "$l" && jpt_1>"$z" && jpt_2>"$z")"
        echo $output_line >> $dirname/cutsInputTemp_$channel.txt
        ((line_count++))
        done; done; done; done;
    elif [ $channel == "mt" ]; then
      for k in "${HPt_2[@]}"; do for j in "${taupt_2[@]}"; do for v in "${mt[@]}"; do
          export output_line="mt_1<"$v" && n_lowpt_jets>=1 && jpt_1>"$i" && pt_2>"$j" && pt_tt>"$k" "$l" && !(mt_1<"$v" && n_lowpt_jets>=2 && jdeta_lowpt>"$q" && mjj_lowpt>"$w" && pt_tt>="$e" "$l" && jpt_1>"$z" && jpt_2>"$z")"
          echo $output_line >> $dirname/cutsInputTemp_$channel.txt
          ((line_count++))
        done; done; done;
    elif [ $channel == "tt" ]; then
      for x in "${tau2pt_2[@]}"; do for k in "${HPt_tt_2[@]}"; do for j in "${taupt_tt_2[@]}"; do
        if [ $j -ge $x ]; then
          export output_line="n_lowpt_jets>=1 && jpt_1>"$i" && pt_1>"$j" && pt_2>"$x" && pt_tt>"$k" "$l" && !(n_lowpt_jets>=2 && jdeta_lowpt>"$q" && mjj_lowpt>"$w" && pt_tt>="$e" "$l" && jpt_1>"$z" && jpt_2>"$z")"
          echo $output_line >> $dirname/cutsInputTemp_$channel.txt
          ((line_count++))
        fi
      done; done; done;
    fi
  fi
done; done; done; done; done; done;

for c in "${tau_id[@]}"; do
  export mva=$c
  qsub -q hep.q -l h_rt=0:30:0 -t 1-$line_count:1 scripts/runOptimizeCatogories.sh $channel $dirname $mva
done
