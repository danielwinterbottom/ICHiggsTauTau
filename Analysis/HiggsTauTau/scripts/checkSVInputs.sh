#!/bin/bash
export output=$1
export sv_output=$2
export return=$(pwd)
cd $output
for i in *; do
  export num_matched=0
  export match1=$(echo $i | cut -d"." -f1)
  cd $return/$sv_output
  for j in *; do
    export match2=$(echo $j | cut -d"." -f1 | cut -d"_" -f2- | rev | cut -d"_" -f3- | rev)
    #echo $match2
    if [ "$match1" == "$match2" ]; then
      ((num_matched++))
      break
    fi
  done
  #if [ "$num_matched" -gt 0 ]; then
    #echo "Matched "$i" to svfit input file"
  if [ "$num_matched" == 0 ]; then
    echo $i" not matched to svfit input file!!!!!!!!!!!!!!!!!!!!"
  fi
done
cd $return
