#!/bin/bash

#infolder=output_parkedana
#outfolder=cards_parkedana/
#blind=false
infolder=output_run2ana
outfolder=cards_run2ana/
blind=true
zvvstat=18
mkdir -p $outfolder

for channel in qcd enu munu taunu mumu
do
    mkdir -p $outfolder/$channel
    ./bin/makeCountingCard -i $infolder --blind=$blind -o $outfolder/$channel/vbfhinv_${channel}_8TeV.txt -m 125 --channel $channel --do_latex true --do_datatop false --zvvstat 0 --qcdrate 0 --mcBkgOnly=true --do_run2=true
done

for mass in 125 #110 125 150 200 300 400
  do
  mkdir -p $outfolder/$mass
  ./bin/makeCountingCard -i $infolder --blind=$blind -o $outfolder/$mass/vbfhinv_${mass}_8TeV.txt -m $mass --do_latex true --do_datatop false --zvvstat $zvvstat --mcBkgOnly=true --do_run2=true
done
