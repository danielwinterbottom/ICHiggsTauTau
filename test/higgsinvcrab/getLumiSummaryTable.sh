#!/bin/bash

if [ "$#" -ne "2" ]; then
    echo "Usage: $0 <production name> <Dataset>"
    exit 0
fi

PROD=$1
DATASET=$2

echo $PATH | grep -q brilconda
if (( "$?" != 0 )); then
    export PATH=$HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.0.3/bin:$PATH
fi

echo "Dataset: "$DATASET
totlumi=0
for era in 16Dec-v1 #05Oct-v1 PromptReco-v4;
do
    brilcalc lumi --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/OfflineNormtagV2.json -i $PROD/DATA/crab_${DATASET}-2015D-$era/results/lumiSummary.json -u /pb > lumiSummary_${PROD}_${DATASET}_${era}.txt
     #--normtag /afs/cern.ch/user/c/cmsbril/public/normtag_json/OfflineNormtagV1.json 
    lumi=`grep -A 4 "Summary" lumiSummary_${PROD}_${DATASET}_${era}.txt | tail -n 1 | awk '{SUM += $12} END {print SUM}'`
    echo $era" & "$lumi
    totlumi=`echo "$totlumi+$lumi" | bc`
done
echo "Total & "$totlumi

#exit 0

SIGTRIG="HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu140"
CONTTRIG="HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu80"

for TRIG in $SIGTRIG $CONTTRIG;
do
    echo $TRIG
    totlumi=0
    for era in 16Dec-v1 #05Oct-v1 PromptReco-v4;
    do
	brilcalc lumi --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/OfflineNormtagV2.json -i $PROD/DATA/crab_${DATASET}-2015D-$era/results/lumiSummary.json -u /pb --hltpath ${TRIG}"*" > lumiSummary_${PROD}_${DATASET}_${era}_${TRIG}.txt
	lumi=`grep -A 10 "Summary" lumiSummary_${PROD}_${DATASET}_${era}_${TRIG}.txt | grep ${TRIG} | awk '{SUM += $12} END {print SUM}'`
	echo $era" & "$lumi
	totlumi=`echo "$totlumi+$lumi" | bc`
    done
    echo "total & "$totlumi
done
