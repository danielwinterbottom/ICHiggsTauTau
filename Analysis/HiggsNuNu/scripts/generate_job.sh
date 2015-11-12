#!/bin/sh

if [ -z $7 ]
then
    echo "Must specify <doCERN> <execName> <production> <jobDir> <outputDir> <script input> <script output> <optional:GridSetup>"
    exit
fi
DOCERN=$1
MYEXEC=$2
PRODUCTION=$3
JOBDIR=$4
OUTPUTDIR=$5
INPUT=$6
OUTPUT=$7
GRIDSETUP=1

if (( "$#" == "8" ))
    then
    echo "Option grid setup: $8"
    let GRIDSETUP=$8
fi

echo "BASEDIR=\`pwd\`" &> $OUTPUT
echo "echo \"Basedir: \$BASEDIR, sendir= $PWD\"" >> $OUTPUT
echo "cd $PWD" >> $OUTPUT
if (( "$GRIDSETUP" != "0" )); then
    echo "Grid setup is enabled."
    echo "source /vols/cms/grid/setup.sh" >> $OUTPUT
else 
    echo "Grid setup is disabled. Enabling EOS setup."
fi


echo "export SCRAM_ARCH=$SCRAM_ARCH" >> $OUTPUT
echo "eval \`scramv1 runtime -sh\`" >> $OUTPUT
echo "source $PWD/scripts/setup_libs.sh" >> $OUTPUT

if [ "$DOCERN" = "1" ]
    then
    echo "cmsenv" >> $OUTPUT
    echo "cp -r scripts/ \$BASEDIR/" >> $OUTPUT
    echo "cp -r input/ \$BASEDIR/" >> $OUTPUT
    echo "mkdir -p \$BASEDIR/filelists \$BASEDIR/bin \$BASEDIR/$JOBDIR \$BASEDIR/$OUTPUTDIR" >> $OUTPUT
    echo "cp -r filelists/$PRODUCTION/ \$BASEDIR/filelists/" >> $OUTPUT
    echo "cp bin/$MYEXEC \$BASEDIR/bin" >> $OUTPUT
    echo "cd \$BASEDIR" >> $OUTPUT
    echo "pwd" >> $OUTPUT
    echo "ls -ltrh *" >> $OUTPUT
fi

echo "eval $INPUT" >> $OUTPUT
if [ "$DOCERN" = "1" ]
    then
    echo "grep \"alias eos=\" /afs/cern.ch/project/eos/installation/cms/etc/setup.sh | sed \"s/alias /export my/\" > eosenv.sh" >> $OUTPUT
    echo "source eosenv.sh" >> $OUTPUT
    echo "\$myeos mkdir -p /eos/cms/store/user/amagnan/SkimFiles/$PRODUCTION/$OUTPUTDIR" >> $OUTPUT
    echo "cp -r $JOBDIR/* $PWD/$JOBDIR/." >> $OUTPUT
    echo "filename=\`ls $OUTPUTDIR*.root\`" >> $OUTPUT
    echo "\$myeos cp \$BASEDIR/\$filename /eos/cms/store/user/amagnan/SkimFiles/$PRODUCTION/\$filename" >> $OUTPUT
fi

chmod +x $OUTPUT
