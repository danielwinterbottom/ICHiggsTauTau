#!/bin/bash

if [ "$#" -ne "2" ]; then
    echo "Usage: $0 <doSubmit> <do4params>"
    exit 0
fi


DOSUBMIT=$1
DO4PARAMS=$2
infolder=output_run2ana_170118/
outfolder=cards_run2ana_170118/
blind=true
#zvvstat=18
mkdir -p $outfolder

extraoptions="--do_ues=false" #--do_ggh=false --do_separate_qcdewk=false"

for channel in qcd enu munu taunu mumu ee nunu #qcd #topl topb
do
    echo " ********************************"
    echo " *** Processing channel $channel"
    echo " ********************************"

    mkdir -p $outfolder/$channel

    HistToIntegrate="dijet_M:dijet_dphi:dijet_deta"
    echo "channel $channel, HistToIntegrate: $HistToIntegrate"
    
    for mindetacut in 2.5 3.0 3.5 4.0 4.5 5.0
    do
	echo "channel $channel, mindetacut: $mindetacut"
	#exact bin boundaries
	#for maxdphicut in 3.1416 2.82744 2.51328 2.19912 1.88496 1.5708 1.25664 0.94248 0.62832
        #rounding
	for maxdphicut in 3.1416 2.8 2.5 2.1 1.8 1.5 1.2 0.9 0.6
	do

	    echo "channel $channel, maxdphicut: $maxdphicut"
 
	    for minjjcut in 701 801 901 1001 1101 1201 1301 1401
	    do
		OUTNAME=$outfolder/$channel/vbfhinv_${channel}_13TeV_${mindetacut}_${maxdphicut}_${minjjcut}.txt
		if (( "$DO4PARAMS" == "1" )); then
		    OUTNAME=$outfolder/$channel/vbfhinv_${channel}_13TeV_${mindetacut}_${maxdphicut}_${minjjcut}_4params.txt
		fi
		if (( "$DOSUBMIT" == "0" )); then
		    echo "./bin/makeCountingCard -i $infolder --blind=$blind -o $OUTNAME -m 125 --channel $channel --do_latex true --do_datatop false --zvvstat 0 --qcdrate 0 --mcBkgOnly=true --do_run2=true --do_4params=$DO4PARAMS --minvarZcut=$minjjcut --maxvarYcut=$maxdphicut --minvarXcut=$mindetacut --histoToIntegrate=$HistToIntegrate $extraoptions | tee $outfolder/$channel/card_${mindetacut}_${maxdphicut}_${minjjcut}.log"
		else
		    ./bin/makeCountingCard -i $infolder --blind=$blind -o $OUTNAME -m 125 --channel $channel --do_latex true --do_datatop false --zvvstat 0 --qcdrate 0 --mcBkgOnly=true --do_run2=true --do_4params=$DO4PARAMS --minvarZcut=$minjjcut --maxvarYcut=$maxdphicut --minvarXcut=$mindetacut --histoToIntegrate=$HistToIntegrate $extraoptions | tee $outfolder/$channel/card_${mindetacut}_${maxdphicut}_${minjjcut}.log
		fi
	    done
	done
    done

done
