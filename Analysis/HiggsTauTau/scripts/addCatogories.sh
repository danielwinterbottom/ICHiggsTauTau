#!/bin/bash
#
export jet_num=$1
export cat_num=$2
export dirname1=$3
export dirname2=$4
rm $dirname2/catogoryCombOutput.txt
rm $dirname2/catogoryCombOutputSorted.txt
IFS=$'\n'
while read line1; do
  if [ $jet_num == 1 -a $cat_num == 2 ]; then
    export vbf_1=$(echo $line1 | cut -d"!" -f2- | cut -d"(" -f2- | cut -f1 -d")")
    export tight_string=$(echo $line1 | cut -d"&" -f5- | cut -d"!" -f1 | rev | cut -c5- | rev | cut -d" " -f2-)
  else
    export tight_string=$(echo $line1 | cut -d"&" -f5- | cut -d" " -f2-)
    export vbf_1="1"
  fi
  export id_1=$(echo echo $line1 | cut -d"&" -f3- | cut -d" " -f2- | cut -f1 -d" ")
  while read line2; do
    export id_2=$(echo echo $line2 | cut -d"&" -f3- | cut -d" " -f2- | cut -f1 -d" ")
    export tight_stringmatch=$(echo $line2 | cut -d"!" -f1- | cut -d"(" -f2- | cut -f1 -d")")
    if [ $jet_num == 1 -a $cat_num == 2 ]; then
      export vbf_2=$(echo $line2 | cut -d"!" -f3- | cut -d"(" -f2- | cut -f1 -d")")
    else
      export vbf_2="1"
    fi
    #echo $vbf_1
    #echo $vbf_2
    #echo $tight_string
    #echo $tight_stringmatch
    #echo $id_1
    #echo $id_2
    #echo "  "
    if [ "$tight_string" == "$tight_stringmatch" -a "$vbf_1" == "$vbf_2" -a "$id_1" == "$id_2" ]; then
      export tight_val=$(echo $line1 | cut -f1 -d"&" | xargs | bc -l)
      export loose_val=$(echo $line2 | cut -f1 -d"&" | xargs | bc -l)
      export tot_merit=$(echo "sqrt(($loose_val*$loose_val)+($tight_val*$tight_val))" | bc)
      export line2_output=$(echo $line2 | cut -d"&" -f3- | cut -d" " -f2-)
      export output_line=$tot_merit"  "$loose_val"  "$tight_val"  "$line2_output
      echo $output_line >> $dirname2/catogoryCombOutput.txt
    fi
  done < $dirname2/optimizeOutputSorted.txt
done < $dirname1/optimizeOutputSorted.txt
sort -n -r $dirname2/catogoryCombOutput.txt > $dirname2/catogoryCombOutputSorted.txt
