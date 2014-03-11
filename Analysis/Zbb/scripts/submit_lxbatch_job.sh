if [ -z $2 ]
then
    echo "Must specify <BATCH QUEUE> <TASK>"
    exit
fi

INPUT=$2
QUEUE=$1
if [ -f "${INPUT}" ]; then
  echo "Submitting job $INPUT (file) to lxbatch queue $QUEUE"
  # Do not re-direct output to /dev/null using the -o option.
  # This has been found to create issues with the job environment
  # being initialised. Instead a file <job id>.log will be created
  # in the submission directory. This is acheived through the special
  # placeholder "%J", which bsub will replace with the job id
  # automatically. Also, as lxbatch does not always locate the
  # directory you submitted from, just passing the relative path
  # to the job shell script may fail. Therefore the readlink command is
  # used to determine the full path to the input file you specify.
  bsub -o $PWD/%J.log -q $QUEUE `readlink -f $INPUT`
else
  echo "Submitting job $INPUT (command) to lxbatch queue $QUEUE"
  bsub -o $PWD/%J.log -q $QUEUE $INPUT
fi
