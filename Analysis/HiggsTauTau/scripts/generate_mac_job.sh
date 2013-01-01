INPUT=$1
OUTPUT=$2

echo "cd $PWD" &> $OUTPUT
echo "source /Users/Andrew/Code/scripts/set_cmssw_env.sh" >> $OUTPUT
echo "eval \`scramv1 runtime -sh\`" >> $OUTPUT
echo "eval $INPUT" >> $OUTPUT
chmod +x $OUTPUT
