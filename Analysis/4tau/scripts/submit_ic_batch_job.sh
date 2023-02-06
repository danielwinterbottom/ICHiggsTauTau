if [ -z $2 ]
then
    echo "Must specify <BATCH QUEUE> <TASK>"
    exit
fi

if [ -z $3 ]
then
  ERROR="/dev/null"
else
  ERROR=$3
fi

if [ -z $4 ]
then
  OUTPUT="/dev/null"
else
  ERROR=$4
fi

INPUT=$2
QUEUE=$1
sed -i '/export SCRAM_ARCH/ i\
source /vols/grid/cms/setup.sh' $INPUT
echo "Submitting job $INPUT to Imperial batch queue $QUEUE"
qsub -e $ERROR -o $OUTPUT -V -q $QUEUE -cwd $INPUT
