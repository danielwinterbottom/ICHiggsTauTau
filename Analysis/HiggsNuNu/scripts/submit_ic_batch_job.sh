if [ -z $2 ]
then
    echo "Must specify <BATCH QUEUE> <TASK>"
    exit
fi

INPUT=$2
QUEUE=$1
echo "Submitting job $INPUT to Imperial batch queue $QUEUE"
qsub -o /dev/null -e /dev/null -q $QUEUE -cwd $INPUT
