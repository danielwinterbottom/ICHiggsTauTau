if [ -z $2 ]
then
    echo "Must specify <BATCH QUEUE> <TASK>"
    exit
fi

INPUT=$2
QUEUE=$1
if [ -f "${INPUT}" ]; then
  echo "Submitting job $INPUT (file) to lxbatch queue $QUEUE"
  bsub  -o /dev/null -e /dev/null -q $QUEUE < $INPUT
else
  echo "Submitting job $INPUT (command) to lxbatch queue $QUEUE"
  bsub  -o /dev/null -e /dev/null -q $QUEUE $INPUT
fi
#bsub  -q $QUEUE `readlink -f $INPUT` 
