#!/bin/bash
#

export vbf_cat_num=$1
export onejet_cat_num=$2
export dirname1=$3

export test=$(ls $dirname1/ | grep combinedVBFSorted.txt)

if [ $vbf_cat_num == 2 -a "$test" != "combinedVBFSorted.txt" ]; then
  echo "Combining VBF categories.."
  ./scripts/postprocessOptimizeCategories.sh $dirname1"/2Jet/2Cat/tight/"
  ./scripts/postprocessOptimizeCategories.sh $dirname1"/2Jet/2Cat/loose/"
  export filename1=$dirname1"/2Jet/2Cat/loose/optimizeOutputSorted.txt"
  export filename2=$dirname1"/2Jet/2Cat/tight/optimizeOutputSorted.txt"
  IFS=$'\n'

  while read line1; do
      export mva_1=$(echo $line1 | cut -d" " -f3 | tr -d '[[:space:]]')
      export mt_1=$(echo $line1 | cut -d" " -f5 | tr -d '[[:space:]]')
      export vbf_1=$(echo $line1 | cut -d "(" -f2 | cut -d ")" -f1 | sed -e 's/^[[:space:]]*//' | sed -e 's/[[:space:]]*$//')
    while read line2; do
      
      if ["$vbf_1" == "$(echo $line2 | cut -d" " -f7- | sed -e 's/^[[:space:]]*//' | sed -e 's/[[:space:]]*$//')" ]; then if [ "$mva_1" == "$(echo $line2 | cut -d" " -f3 | tr -d '[[:space:]]')" ]; then if [ "$mt_1" == "$(echo $line2 | cut -d" " -f5 | tr -d '[[:space:]]')" ]; then
        export val1=$(echo $line1 | cut -d" " -f1 | tr -d '[[:space:]]')
        export val2=$(echo $line2 | cut -d" " -f1 | tr -d '[[:space:]]')
        export val_tot=$(echo "sqrt(($val1*$val1)+($val2*$val2))" | bc)
        export output_line=$(echo $line1 | cut -d " " -f7-| sed -e 's/^[[:space:]]*//' | sed -e 's/[[:space:]]*$//')
        echo $val_tot" && "$output_line >> $dirname1/combinedVBF.txt
        break
      fi; fi; fi

      done < $filename2
  done < $filename1
  
  sort -n -r $dirname1/combinedVBF.txt > $dirname1/combinedVBFSorted.txt
  
  echo "Finished combining VBF categories.."
elif [ $vbf_cat_num == 1 -a "$test" != "combinedVBFSorted.txt" ]; then
  ./scripts/postprocessOptimizeCategories.sh $dirname1"/2Jet/1Cat"
  cp $dirname1"/2Jet/1Cat/optimizeOutput.txt" $dirname1/combinedVBF.txt
  cp $dirname1"/2Jet/1Cat/optimizeOutputSorted.txt" $dirname1/combinedVBFSorted.txt
fi

export test=$(ls $dirname1/ | grep combined1JetSorted.txt)

if [ $onejet_cat_num == 2 -a "$test" != "combined1JetSorted.txt" ]; then
  echo "Combining 1Jet categories.."
  echo $(date +"%Y_%m_%d_%H_%M_%S")
  ./scripts/postprocessOptimizeCategories.sh $dirname1"/1Jet/2Cat/tight/"
  ./scripts/postprocessOptimizeCategories.sh $dirname1"/1Jet/2Cat/loose/"
  export filename1=$dirname1"/1Jet/2Cat/loose/optimizeOutput.txt"
  export filename2=$dirname1"/1Jet/2Cat/tight/optimizeOutput.txt"
  IFS=$'\n'

  while read line1; do
    export mva_1=$(echo $line1 | cut -d" " -f3 | tr -d '[[:space:]]')
    export mt_1=$(echo $line1 | cut -d" " -f5 | tr -d '[[:space:]]')
    export onejet_1=$(echo $line1 | cut -d "(" -f2 | cut -d ")" -f1 | sed -e 's/^[[:space:]]*//' | sed -e 's/[[:space:]]*$//')
    export vbf_1=$(echo $line1 | cut -d "(" -f3 | cut -d ")" -f1 | sed -e 's/^[[:space:]]*//' | sed -e 's/[[:space:]]*$//')
    while read line2; do
      
      if [ "$vbf_1" == "$(echo $line2 | cut -d "(" -f2 | cut -d ")" -f1 | sed -e 's/^[[:space:]]*//' | sed -e 's/[[:space:]]*$//')" ]; then if [ "$onejet_1" == "$(echo $line2 | cut -d" " -f7- | cut -d "!" -f1 | rev | cut -d" " -f3- | rev | sed -e 's/^[[:space:]]*//' | sed -e 's/[[:space:]]*$//')" ];then if [ "$mva_1" == "$(echo $line2 | cut -d" " -f3 | tr -d '[[:space:]]')" ]; then if [ "$mt_1" == "$(echo $line2 | cut -d" " -f5 | tr -d '[[:space:]]')" ]; then
        export val1=$(echo $line1 | cut -d" " -f1 | tr -d '[[:space:]]')
        export val2=$(echo $line2 | cut -d" " -f1 | tr -d '[[:space:]]')
        export val_tot=$(echo "sqrt(($val1*$val1)+($val2*$val2))" | bc)
        export output_line=$(echo $line1 | cut -d " " -f7-| sed -e 's/^[[:space:]]*//' | sed -e 's/[[:space:]]*$//')
        echo $val_tot" && "$output_line >> $dirname1/combined1Jet.txt
        break
      fi; fi; fi; fi

      done < $filename2
  done < $filename1
  
  sort -n -r $dirname1/combined1Jet.txt > $dirname1/combined1JetSorted.txt
  
  echo "Finished combining 1Jet categories.."
  echo $(date +"%Y_%m_%d_%H_%M_%S")
elif [ $onejet_cat_num == 1 -a "$test" != "combined1JetSorted.txt" ]; then
  ./scripts/postprocessOptimizeCategories.sh $dirname1"/1Jet/1Cat"
  cp $dirname1"/1Jet/1Cat/optimizeOutput.txt" $dirname1/combined1Jet.txt
  cp $dirname1"/1Jet/1Cat/optimizeOutputSorted.txt" $dirname1/combined1JetSorted.txt
fi

#if [ "$onejet_cat_num" \> 0 -a "$vbf_cat_num" \> 0 ]; then
#  echo "Combining VBF and 1Jet Categories.."
#  export filename1=$dirname1/combinedVBF.txt
#  export filename2=$dirname1/combined1Jet.txt
#  while read line1; do
#    export val1=$(echo $line1 | cut -d" " -f1 | tr -d '[[:space:]]')
#    export mva_1=$(echo $line1 | cut -d" " -f3 | tr -d '[[:space:]]')
#    export mt_1=$(echo $line1 | cut -d" " -f5 | tr -d '[[:space:]]')
#    export vbf_1=$(echo $line1 | cut -d" " -f7- | cut -d "!" -f1 | rev | cut -d" " -f3- | rev | sed -e 's/^[[:space:]]*//' | sed -e 's/[[:space:]]*$//')
#    while read line2; do
#    
#    export val2=$(echo  $line2 | cut -d" " -f1 | tr -d '[[:space:]]')
#    export mva_2=$(echo $line2 | cut -d" " -f3 | tr -d '[[:space:]]')
#    export mt_2=$(echo  $line2 | cut -d" " -f5 | tr -d '[[:space:]]')
#    export vbf_2=$(echo $line2 | cut -d "(" -f3 | cut -d ")" -f1 | sed -e 's/^[[:space:]]*//' | sed -e 's/[[:space:]]*$//')
#    
#    echo $line1
#    echo $val1
#    echo $mva_1
#    echo $vbf_1
#    
#    echo $line2
#    echo $val2
#    echo $mva_2
#    echo $vbf_2
#    
#    echo "  "
#    
#    if [ "$mva_1" == "$mva_2" -a "$mt_1" == "$mt_2" -a "$onejet_1" == "$onejet_2" -a "$vbf_1" == "$vbf_2" ]; then
#      export output_lin1_1=$(echo $line1 | cut -d" " -f7-)
#      export output_line_2=$(echo $line2 | cut -d" " -f7- | cut -d "(" -f1-2 | rev | cut -d" " -f3- | rev | sed -e 's/^[[:space:]]*//' | sed -e 's/[[:space:]]*$//')
#      echo $output_line_1
#      echo $output_line_2
#      echo "  "
#    fi
#    
#    done < $filename2
#  done < $filename1
#  
#  echo "Finished combining VBF and 1Jet Categories.."
#fi

      #echo $val1"_"
      #echo $val2"_"
      #echo $mva_1"_"
      #echo $mva_2"_"
      #echo $mt_1"_"
      #echo $mt_2"_"
      #echo $onejet_1"_"
      #echo $onejet_2"_"
      #echo $vbf_1"_"
      #echo $vbf_2"_"

