#!/bin/sh

if [ -z $2 ]
then
    echo "Must specify <script input> <script output> <optional:GridSetup>"
    exit
fi

INPUT=$1
OUTPUT=$2
GRIDSETUP=1

if (( "$#" == "3" ))
    then
    echo "Option grid setup: $3"
    let GRIDSETUP=$3
fi

echo "cd $PWD" &> $OUTPUT
if (( "$GRIDSETUP" != "0" )); then
    echo "Grid setup is enabled."
    echo "source /vols/cms/grid/setup.sh" >> $OUTPUT
else 
    echo "Grid setup is disabled."
fi
echo "export SCRAM_ARCH=$SCRAM_ARCH" >> $OUTPUT
echo "eval \`scramv1 runtime -sh\`" >> $OUTPUT
echo "source $PWD/scripts/setup_libs.sh" >> $OUTPUT
echo "eval $INPUT" >> $OUTPUT
chmod +x $OUTPUT
