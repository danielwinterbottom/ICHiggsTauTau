if [ -z $2 ]
then
    echo "Must specify <BATCH QUEUE> <TASK>"
    exit
fi

INPUT=$2
QUEUE=$1
echo "Submitting job $INPUT to NAF batch queue $QUEUE"
qsub -q $QUEUE  -l h_vmem="2G" -cwd $INPUT
