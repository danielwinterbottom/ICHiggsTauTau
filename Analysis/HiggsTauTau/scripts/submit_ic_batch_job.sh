if [ -z $2 ]
then
    echo "Must specify <BATCH QUEUE> <TASK>"
    exit
fi

INPUT=$2
QUEUE=$1
sed -i '/export SCRAM_ARCH/ i\
source /vols/grid/cms/setup.sh' $INPUT
echo "Submitting job $INPUT to Imperial batch queue $QUEUE"
qsub -q $QUEUE -cwd $INPUT
