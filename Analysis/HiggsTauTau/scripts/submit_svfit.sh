## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}

echo "Using job-wrapper:    " $JOBWRAPPER
echo "Using job-submission: " $JOBSUBMIT


if (( "$#" != "1" ))
then
  echo "Usage: submit_svfit.sh [svfit folder]"
  exit
fi

FOLDER=$1

find $FOLDER -name *input.root -exec $JOBWRAPPER "./bin/SVFitTest {}" {}.sh \;
