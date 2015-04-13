#!/bin/bash

infolder=output/
outfolder=cards/
blind=false
zvvstat=18
mkdir $outfolder
for mass in 110 125 150 200 300 400
  do
  mkdir $outfolder/$mass
  ./bin/makeCountingCard -i $infolder --blind=$blind -o $outfolder/$mass/vbfhinv_${mass}_8TeV.txt -m $mass --do_latex true --do_datatop false --zvvstat $zvvstat
done