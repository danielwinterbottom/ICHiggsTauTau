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
g++ -g -std=c++0x -isystem $ROOTSYS/include `root-config --glibs` -I$CMS_PATH/$SCRAM_ARCH/external/boost/1.47.0/include -L$CMS_PATH/$SCRAM_ARCH/external/boost/1.47.0/lib -lboost_regex -lboost_program_options doDatacard.cpp 
./a.out
rm a.out
