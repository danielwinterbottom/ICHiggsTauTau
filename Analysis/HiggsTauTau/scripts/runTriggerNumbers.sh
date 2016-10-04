#!/bin/bash
declare -a cat=("GluGlu", "VBF")
declare -a channel=("mt")
declare -a triggers1=("mt")
declare -a triggers2=("mt")
folder="Temp"
rm $folder/*.root
for i in "${channel[@]}"
do
  for j in "${cat[@]}"
  do
    for
    do
      ./bin/HLTStudies $i $folder/ $j
    done
  done
done
