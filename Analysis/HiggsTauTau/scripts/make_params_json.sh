input_xs=$1
input_evt=$2
lumi=$3

# Input xs file should have format: Sample_name xs
# Input evt file should have format: Sample_name evt

# set the inclusive sample names for DY and W - the event numbers and xs for the N jets samples will be set to the values for these samples
DY_inclusive_name_1="DYJetsToLL-LO-ext1"
DY_inclusive_name_2="DYJetsToLL-LO-ext2"
W_inclusive_name_1="WJetsToLNu-LO"
W_inclusive_name_2="WJetsToLNu-LO-ext"

while read i; do 
    sample_name=$(echo $i | cut -d" " -f1)
    xs=$(echo $i | cut -d" " -f2)
    if [ "$sample_name" == "$DY_inclusive_name_1" ]; then
      dy_xs=$xs
    elif [ "$sample_name" == "$W_inclusive_name_1" ]; then
      w_xs=$xs
    fi
done < $input_xs
dy_evt=0
w_evt=0
while read i; do 
    sample_name=$(echo $i | cut -d" " -f1)
    evt=$(echo $i | cut -d" " -f2)
    if [ "$sample_name" == "$DY_inclusive_name_1" -o "$sample_name" == "$DY_inclusive_name_2" ]; then
      dy_evt=$(bc -l <<<$dy_evt+$evt)
    elif [ "$sample_name" == "$W_inclusive_name_1" -o "$sample_name" == "$W_inclusive_name_2" ]; then
      w_evt=$(bc -l <<<$w_evt+$evt)
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
