#!/bin/bash
#
export dirname1=$1
export dirname2=$2
rm $dirname2/catogoryCombOutput.txt
rm $dirname2/catogoryCombOutputSorted.txt
IFS=$'\n'
while read line1; do    
  export tight_string=$(echo $line1 | cut -d"&" -f3- | cut -d" " -f2-)
  while read line2; do
    export tight_stringmatch=$(echo $line2 | cut -d"!" -f2- | cut -d"(" -f2- | cut -f1 -d")")
    if [ $tight_string == $tight_stringmatch ]; then
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
