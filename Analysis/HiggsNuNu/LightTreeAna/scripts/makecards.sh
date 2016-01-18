#!/bin/bash

#infolder=output_parkedana
#outfolder=cards_parkedana/
#blind=false
infolder=output_run2ana_160115
outfolder=cards_run2ana_160115/
blind=true
zvvstat=18
mkdir -p $outfolder

for channel in enu munu taunu mumu qcd topl topb
do
    mkdir -p $outfolder/$channel
    echo " ********************************"
    echo " *** Processing channel $channel"
    echo " ********************************"
    ./bin/makeCountingCard -i $infolder --blind=$blind -o $outfolder/$channel/vbfhinv_${channel}_8TeV.txt -m 125 --channel $channel --do_latex true --do_datatop false --zvvstat 0 --qcdrate 0 --mcBkgOnly=true --do_run2=true --do_4params=true
done

for mass in 125 #110 125 150 200 300 400
  do
  mkdir -p $outfolder/$mass
  ./bin/makeCountingCard -i $infolder --blind=$blind -o $outfolder/$mass/vbfhinv_${mass}_8TeV.txt -m $mass --do_latex true --do_datatop false --zvvstat $zvvstat --mcBkgOnly=true --do_run2=true --do_4params=true
done
