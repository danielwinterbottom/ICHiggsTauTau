INPUT=$1

echo "Submitting job $INPUT to local task-spooler queue"
ts bash -c "eval $INPUT"
