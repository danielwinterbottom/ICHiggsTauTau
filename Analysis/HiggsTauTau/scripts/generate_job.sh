if [ -z $2 ]
then
    echo "Must specify <script input> <script output>"
    exit
fi


INPUT=$1
OUTPUT=$2

echo "Generating job $OUTPUT"

echo "cd $PWD" &> $OUTPUT
echo "export SCRAM_ARCH=slc5_amd64_gcc462" >> $OUTPUT
echo "eval \`scramv1 runtime -sh\`" >> $OUTPUT
echo "source $PWD/scripts/setup_libs.sh" >> $OUTPUT
echo "eval $INPUT" >> $OUTPUT
chmod +x $OUTPUT
