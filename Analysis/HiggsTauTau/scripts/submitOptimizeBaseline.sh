#!/bin/bash
#

export channel=$1
#IFS='%'

export now=$(date +"%Y_%m_%d_%H_%M_%S")

export dirname0=$channel"_baseline/"$now
mkdir -p $dirname0

if [ "$channel" == "tt" ]; then
  declare -a mt=( -1 )
else
  declare -a mt=({20..70..1})
fi
declare -a tau_id=( "medium" "tight" "vtight" )

export line_count=0
for i in 1 2 3; do
  if [ $i == 1 ]; then
    export output_line="n_jets>=2 && jdeta>3.5 && mjj>500 && pt_tt>=100 && n_bjets==0 && n_jetsingap==0"
  elif [ $i == 2 ]; then
    export output_line="n_jets>=1 && !(n_jets>=2 && jdeta>3.5 && mjj>500 && pt_tt>=100 && n_bjets==0 && n_jetsingap==0)"
  elif [ $i == 3 ]; then
    export output_line="n_jets==0"
  fi
  echo $output_line >> $dirname0/cutsInputTemp_$channel.txt
  ((line_count++))
done
  
for i in "${tau_id[@]}"; do for j in "${mt[@]}"; do
   qsub -q hep.q -l h_rt=0:5:0 -t 1-$line_count:1 scripts/runOptimizeCategories.sh $channel $dirname0 $i $j -m a -M ${USER}@ic.ac.uk
done; done
