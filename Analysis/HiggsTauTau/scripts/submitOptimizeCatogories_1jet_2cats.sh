#!/bin/bash
#
export channel=$1
IFS='%'
export now=$(date +"%Y_%m_%d_%H_%M")
export dirname1="1Jet/2Cat/tight/"$channel"_"$now
mkdir -p $dirname1
rm $dirname1/cutsInputTemp_$channel.txt
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

declare -a taupt_2=( 40 60 )
declare -a taupt_tt_2=( 50 70 )
declare -a HPt_2=( 100 150 )
declare -a HPt_tt_2=( 100 150 200 )
declare -a met_2=( 30 50 )
declare -a tau2pt_2=( 0 )
export line_count=0
for q in "${dEta_loose[@]}"; do for w in "${Mjj_loose[@]}"; do for e in "${HPt_loose[@]}"; do for l in "${btag[@]}"; do for z in "${jetpt[@]}"; do for i in "${leadjetpt[@]}"; do
  if [ $i -ge $z ]; then
    if [ $channel == "et" ]; then
      for k in "${HPt_2[@]}"; do for m in "${met_2[@]}"; do for j in "${taupt_2[@]}"; do for v in "${mt[@]}"; do
        export output_line="mt_1<"$v" && n_lowpt_jets>=1 && jpt_1>"$i" && pt_2>"$j" && pt_tt>"$k" && met>"$m" "$l" && !(mt_1<"$v" && n_lowpt_jets>=2 && jdeta_lowpt>"$q" && mjj_lowpt>"$w" && pt_tt>="$e" "$l" && jpt_1>"$z" && jpt_2>"$z")"
        echo $output_line >> $dirname1/cutsInputTemp_$channel.txt
        ((line_count++))
        done; done; done; done;
    elif [ $channel == "mt" ]; then
      for k in "${HPt_2[@]}"; do for j in "${taupt_2[@]}"; do for v in "${mt[@]}"; do
          export output_line="mt_1<"$v" && n_lowpt_jets>=1 && jpt_1>"$i" && pt_2>"$j" && pt_tt>"$k" "$l" && !(mt_1<"$v" && n_lowpt_jets>=2 && jdeta_lowpt>"$q" && mjj_lowpt>"$w" && pt_tt>="$e" "$l" && jpt_1>"$z" && jpt_2>"$z")"
          echo $output_line >> $dirname1/cutsInputTemp_$channel.txt
          ((line_count++))
        done; done; done;
    elif [ $channel == "tt" ]; then
      for x in "${tau2pt_2[@]}"; do for k in "${HPt_tt_2[@]}"; do for j in "${taupt_tt_2[@]}"; do
        if [ $j -ge $x ]; then
          export output_line="n_lowpt_jets>=1 && jpt_1>"$i" && pt_1>"$j" && pt_2>"$x" && pt_tt>"$k" "$l" && !(n_lowpt_jets>=2 && jdeta_lowpt>"$q" && mjj_lowpt>"$w" && pt_tt>="$e" "$l" && jpt_1>"$z") && jpt_2>"$z")"
          echo $output_line >> $dirname1/cutsInputTemp_$channel.txt
          ((line_count++))
          export output_line="n_lowpt_jets>=1 && jpt_1>"$i" && pt_1>"$j" && pt_2>"$x" && pt_tt>"$k" "$l" && !(n_lowpt_jets>=2 && jdeta_lowpt>"4" && mjj_lowpt>"650" && pt_tt>="130" "$l" && jpt_1>"$z" && jpt_2>"$z")"
          echo $output_line >> $dirname1/cutsInputTemp_$channel.txt
          ((line_count++))
        fi
      done; done; done;
    fi
  fi
done; done; done; done; done; done;

for c in "${tau_id[@]}"; do
  export mva=$c
  qsub -q hep.q -l h_rt=0:30:0 -t 1-$line_count:1 scripts/runOptimizeCatogories.sh $channel $dirname1 $mva
done
  
export dirname2="1Jet/2Cat/loose/"$channel"_"$now
mkdir -p $dirname2
rm $dirname2/cutsInputTemp_$channel.txt
declare -a dEta_loose=( 3 )
declare -a Mjj_loose=( 400 500 )
declare -a HPt_loose=( 0 )
declare -a btag=("&& n_bjets==0")
declare -a jetpt=( 30 )
declare -a leadjetpt=( 30 )
declare -a taupt=( 0 30 )
declare -a taupt_tt=( 0 50 )
declare -a HPt=( 0 50 )
declare -a HPt_tt=( 0 50 100 )
declare -a met=( 0 30 )
declare -a tau2pt=( 0 )

declare -a taupt_2=( 40 60 )
declare -a taupt_tt_2=( 50 70 )
declare -a HPt_2=( 100 150 )
declare -a HPt_tt_2=( 100 150 200 )
declare -a met_2=( 30 50 )
declare -a tau2pt_2=( 0 )
export line_count=0
for q in "${dEta_loose[@]}"; do for w in "${Mjj_loose[@]}"; do for e in "${HPt_loose[@]}"; do for l in "${btag[@]}"; do for z in "${jetpt[@]}"; do for i in "${leadjetpt[@]}"; do
  if [ $i -ge $z ]; then
    if [ $channel == "et" ]; then
      for k in "${HPt_2[@]}"; do for m in "${met_2[@]}"; do for j in "${taupt_2[@]}"; do for k2 in "${HPt[@]}"; do for m2 in "${met[@]}"; do for j2 in "${taupt[@]}"; do for v in "${mt[@]}"; do
        if [ $k \> $k2 -a $j \> $j2 -a $m \> $m2 ]; then
          export output_line="mt_1<"$v" && n_lowpt_jets>=1 && jpt_1>"$i" && pt_tt>"$k2" && pt_2>"$j2" && met>"$m" && !(mt_1<"$v" && n_lowpt_jets>=1 && jpt_1>"$i" && pt_2>"$j" && pt_tt>"$k" && met>"$m" "$l") && !(mt_1<"$v" && n_lowpt_jets>=2 && jdeta_lowpt>"$q" && mjj_lowpt>"$w" && pt_tt>="$e" "$l" && jpt_1>"$z" && jpt_2>"$z")"
          echo $output_line >> $dirname2/cutsInputTemp_$channel.txt
          ((line_count++))
        fi
        done; done; done; done; done; done; done;
    elif [ $channel == "mt" ]; then
      for k in "${HPt_2[@]}"; do for j in "${taupt_2[@]}"; do for k2 in "${HPt[@]}"; do for j2 in "${taupt[@]}"; do for v in "${mt[@]}"; do
        if [ $k \> $k2 -a $j \> $j2 ]; then
          export output_line="mt_1<"$v" && n_lowpt_jets>=1 && jpt_1>"$i" && pt_tt>"$k2" && pt_2>"$j2" && !(mt_1<"$v" && n_lowpt_jets>=1 && jpt_1>"$i" && pt_2>"$j" && pt_tt>"$k" "$l") && !(mt_1<"$v" && n_lowpt_jets>=2 && jdeta_lowpt>"$q" && mjj_lowpt>"$w" && pt_tt>="$e" "$l" && jpt_1>"$z" && jpt_2>"$z")"
          echo $output_line >> $dirname2/cutsInputTemp_$channel.txt
          ((line_count++))
        fi
        done; done; done; done; done;
    elif [ $channel == "tt" ]; then
      for x in "${tau2pt_2[@]}"; do for k in "${HPt_tt_2[@]}"; do for j in "${taupt_tt_2[@]}"; do for c2 in "${tau2pt[@]}"; do for k2 in "${HPt_tt[@]}"; do for j2 in "${taupt_tt[@]}"; do 
        if [ $j2 -ge $c2 -a $j -ge $x -a $k \> $k2 -a $j \> $j2 -a $x -ge $c2 ]; then
          export output_line="n_lowpt_jets>=1 && jpt_1>"$i" && pt_tt>"$k2" && pt_1>"$j2" && pt_2>"$x" && !(n_lowpt_jets>=1 && jpt_1>"$i" && pt_1>"$j" && pt_2>"$x" && pt_tt>"$k" "$l") && !(n_lowpt_jets>=2 && jdeta_lowpt>"$q" && mjj_lowpt>"$w" && pt_tt>="$e" "$l" && jpt_1>"$z" && jpt_2>"$z")"
          echo $output_line >> $dirname2/cutsInputTemp_$channel.txt
          ((line_count++))
          export output_line="n_lowpt_jets>=1 && jpt_1>"$i" && pt_1>"$j2" && pt_2>"$c2" && pt_tt>"$k2" "$l" && !(n_lowpt_jets>=1 && jpt_1>"$i" && pt_1>"$j" && pt_2>"$x" && pt_tt>"$k" "$l") && !( n_lowpt_jets>=2 && jdeta_lowpt>"4" && mjj_lowpt>"650" && pt_tt>="130" "$l" && jpt_1>"$z" && jpt_2>"$z")"
          echo $output_line >> $dirname2/cutsInputTemp_$channel.txt
          ((line_count++))
        fi
      done; done; done; done; done; done;
    fi
  fi
done; done; done; done; done; done; 

for c in "${tau_id[@]}"; do
  export mva=$c
  qsub -q hep.q -l h_rt=0:30:0 -t 1-$line_count:1 scripts/runOptimizeCatogories.sh $channel $dirname2 $mva
done
