input_xs=$1
input_evt=$2
lumi=$3
# bash scripts/make_params_json.sh scripts/xs_inputs_2018_UL.txt scripts/evt_UL_2018.txt LUMI &> params_UL_2018.json
# Input xs file should have format: Sample_name xs
# Input evt file should have format: Sample_name evt

# set the inclusive sample names for DY and W - the event numbers and xs for the N jets samples will be set to the values for these samples
DY_inclusive_name_1="DYJetsToLL-LO"
W_inclusive_name_1="WJetsToLNu-LO"
DY_NLO_inclusive_name_1="DYJetsToLL-NLO"

while read i; do 
    sample_name=$(echo $i | cut -d" " -f1)
    xs=$(echo $i | cut -d" " -f2)
    if [ "$sample_name" == "$DY_inclusive_name_1" ]; then
      dy_xs=$xs
    elif [ "$sample_name" == "$W_inclusive_name_1" ]; then
      w_xs=$xs
    elif [ "$sample_name" == "$DY_NLO_inclusive_name_1" ]; then
      dy_nlo_xs=$xs
    fi
done < $input_xs
dy_evt=0
w_evt=0
dy_nlo_evt=0
while read i; do 
    sample_name=$(echo $i | cut -d" " -f1)
    evt=$(echo $i | cut -d" " -f2)
    if [ "$sample_name" == "$DY_inclusive_name_1" ]; then
      dy_evt=$(bc -l <<<$dy_evt+$evt)
    elif [ "$sample_name" == "$W_inclusive_name_1" ]; then
      w_evt=$(bc -l <<<$w_evt+$evt)
    elif [ "$sample_name" == "$DY_NLO_inclusive_name_1" ]; then
      dy_nlo_evt=$(bc -l <<<$dy_nlo_evt+$evt)
    fi
done < $input_evt

echo "{"

while read i; do 
    sample_name=$(echo $i | cut -d" " -f1)
    xs=$(echo $i | cut -d" " -f2)
    loop=1
    if [[ "$sample_name" == "DY"*"JetsToLL-LO"* ]]; then 
      xs=$dy_xs
      evt=$dy_evt
      loop=0
    elif [[ "$sample_name" == "W"*"JetsToLNu-LO"* ]]; then
      xs=$w_xs
      evt=$w_evt
      loop=0
    elif [[ "$sample_name" == "DY"*"NLO"* ]]; then
      xs=$dy_nlo_xs
      evt=$dy_nlo_evt
      loop=0
    fi
    if [ $loop == 1 ]; then
      while read j; do
        sample_name_to_match=$(echo $j | cut -d" " -f1)
        if [ "$sample_name" == "$sample_name_to_match" ]; then
          evt=$(echo $j | cut -d" " -f2)
          break
        fi
      done < $input_evt
    fi
  echo "  \""$sample_name"\": {"
  echo "    \"xs\": "$xs","
  echo "    \"evt\": "$evt
  echo "  },"
done < $input_xs

while read i; do
    sample_name=$(echo $i | cut -d" " -f1)
    if [[ "$sample_name" == "SUSY"* ]]; then
      evt=$(echo $i | cut -d" " -f2)
      echo "  \""$sample_name"\": {"
      echo "    \"xs\": 1.0,"
      echo "    \"evt\": "$evt
      echo "  },"
    fi
done < $input_evt

echo "  \"SingleElectron\": {"
echo "    \"lumi\": "$lumi
echo "  },"
echo "  \"SingleMuon\": {"
echo "    \"lumi\": "$lumi
echo "  },"
echo "  \"Tau\": {"
echo "    \"lumi\": "$lumi
echo "  },"
echo "  \"MuonEG\": {"
echo "    \"lumi\": "$lumi
echo "  }"

echo "}"

