command="./bin/LTAnalysis"
#command="./bin/LTPromptAnalysis"
config="--cfg=scripts/DefaultConfig.cfg"
output_old="cental"
suffix_last=""
for SYST in central JERBETTER JERWORSE JESUP JESDOWN UESUP UESDOWN
do
  echo "Doing $SYST"
  if [ "$SYST" = "central" ]
      then
      suffix=""
      output="central"
  else
      suffix="$SYST"
      output="$SYST"
  fi
  sed "-i.bak" "s/$output_old.root/$output.root/" scripts/DefaultConfig.cfg
  sed "-i.bak" "s/output_lighttree\/$suffix_last/output_lighttree\/$suffix/" scripts/DefaultConfig.cfg
  $command $config 
  #eval $command $config #> $SYST.log
  suffix_last="$suffix"
  output_old="$output"
done
sed "-i.bak" "s/output_lighttree\/$suffix_last/output_lighttree\//" scripts/DefaultConfig.cfg
sed "-i.bak" "s/$output_old.root/central.root/" scripts/DefaultConfig.cfg
rm scripts/*.bak
./bin/doDatacard
