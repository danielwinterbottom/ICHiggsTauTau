if [ -z $3 ]
then
    echo "Must specify <BATCH QUEUE> <TASK> <JOBNAME>"
    exit
fi

INPUT=$2
QUEUE=$1
JOBNAME=$3
echo "Submitting job $INPUT to lxplus batch queue $QUEUE"
bsub -q $QUEUE -J $JOBNAME < $INPUT
