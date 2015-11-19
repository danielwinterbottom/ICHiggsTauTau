if [ -z $3 ]
then
    if [ -z $2 ]
	then
	echo "Must specify <BATCH QUEUE> <TASK> <JOBNAME> or <BATCH QUEUE> <TASK>"
	exit
    fi
    INPUT=$2
    QUEUE=$1
    JOBNAME=$2
else
    INPUT=$2
    QUEUE=$1
    JOBNAME=$3
fi

echo "Submitting job $INPUT to lxplus batch queue $QUEUE"
bsub -q $QUEUE -J $JOBNAME < $INPUT
