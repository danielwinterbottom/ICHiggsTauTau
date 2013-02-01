if (( "$#" != "2" ))
then
    echo "<INPUT TASK> <OUTPUT SCRIPT>"
    exit
fi

INPUT=$1
OUTPUT=$2

echo "Generating job $OUTPUT"

echo "cd $PWD" &> $OUTPUT
echo "source /Users/Andrew/Code/scripts/set_cmssw_env.sh" >> $OUTPUT
echo "eval \`scramv1 runtime -sh\`" >> $OUTPUT
echo "eval $INPUT" >> $OUTPUT
chmod +x $OUTPUT
