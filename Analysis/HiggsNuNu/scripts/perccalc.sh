#!/bin/bash
for files in `ls ../output/nunu/MET130/JESDOWN/MC_*`
  do
  echo $files > tmp.txt
  sample=`sed "s/..\/output\/nunu\/MET130\///" tmp.txt`
  rm tmp.txt
  echo -n $sample
  ./perccalc $files
done

