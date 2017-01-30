#!/bin/bash
declare -a cat=("QCD")
declare -a channel=("em" "et" "mt" "tt")
folder="FakeEfficiencies"
rm $folder/*.root
for i in "${channel[@]}"
do
  for j in "${cat[@]}"
  do
    ./bin/HLTStudies $i $folder $j 
    if [ $i == "et" ]; then
     ./bin/HLTStudies $i $folder $j HLT_Ele27_eta2p1_WPLoose_Gsf_v
     ./bin/HLTStudies $i $folder $j HLT_Ele27_eta2p1_WPTight_Gsf_v
    fi
  done
done
