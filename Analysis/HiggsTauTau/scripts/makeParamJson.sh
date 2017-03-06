input=$1
output=$2
lumi=$3

xs_array=($(grep '^XS_' $input | cut -d"_" -f2-))
evt_array=($(grep '^EVT_' $input | cut -d"_" -f2-))
length=${#evt_array[@]}
count=0
echo "{" > $output

for i in "${evt_array[@]}"; do
  ((count++))
  echo "  \""$(echo $i | cut -d":" -f1)"\": {" >> $output
  if [[ $i == "SingleElectron"* || $i == "SingleMuon"* || $i == "MuonEG"* || $i == "Tau"* ]]; then
    echo "    \"lumi\":"$lumi >> $output  
  else
    echo "    \"evt\":"$(echo $i | cut -d":" -f2)"," >> $output
    
    xs="1.0"
    for j in "${xs_array[@]}"; do
      if [ $(echo $i | cut -d":" -f1) == $(echo $j | cut -d":" -f1) ]; then
        xs=$(echo $j | cut -d":" -f2)
      fi
    done
    
    echo "    \"xs\":"$xs >> $output
  fi
  if [ $length == $count ]; then
    echo "  }" >> $output
  else
    echo "  }," >> $output
  fi
done;

echo "}" >> $output
