#!/bin/bash

if [ "$#" -ne "2" ]; then
    echo "Usage: $0 <doSubmit> <do4params>"
    exit 0
fi

DOSUBMIT=$1
DO4PARAMS=$2
#infolder=output_parkedana
#outfolder=cards_parkedana/
#blind=false
infolder=output_run2ana_160216
outfolder=cards_run2ana_160217/
blind=true
zvvstat=18
mkdir -p $outfolder

for channel in qcd taunu
do
    mkdir -p $outfolder/$channel
    OUTNAME=$outfolder/$channel/vbfhinv_${channel}_8TeV.txt
    if (( "$DO4PARAMS" == "1" )); then
	OUTNAME=$outfolder/$channel/vbfhinv_${channel}_8TeV_4params.txt
    fi
    echo " ********************************"
    echo " *** Processing channel $channel"
    echo " ********************************"
    if (( "$DOSUBMIT" == "0" )); then
	echo "./bin/makeCountingCard -i $infolder --blind=$blind -o $OUTNAME -m 125 --channel $channel --do_latex true --do_datatop false --zvvstat 0 --qcdrate 0 --mcBkgOnly=true --do_run2=true --do_4params=$DO4PARAMS --minvarXcut=0.0 --histoToIntegrate=jet2_pt | tee $outfolder/$channel/card.log"
    else
	./bin/makeCountingCard -i $infolder --blind=$blind -o $OUTNAME -m 125 --channel $channel --do_latex true --do_datatop false --zvvstat 0 --qcdrate 0 --mcBkgOnly=true --do_run2=true --do_4params=$DO4PARAMS --minvarXcut=0.0 --histoToIntegrate=jet2_pt | tee $outfolder/$channel/card.log
    fi
done

