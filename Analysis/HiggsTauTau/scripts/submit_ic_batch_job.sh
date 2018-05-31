if [ -z $1 ]
then
    echo "Must specify <TASK>"
    exit
fi

INPUT=$1
sed -i '/export SCRAM_ARCH/ i\
source /vols/grid/cms/setup.sh' $INPUT
echo "Submitting job $INPUT to Imperial batch queue hep.q"
qsub -e /dev/null -o /dev/null -V -cwd $INPUT
