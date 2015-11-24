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
echo "#!/bin/sh" &> $OUTPUT
echo "BASEDIR=\`pwd\`" >> $OUTPUT
echo "echo \"Basedir: \$BASEDIR, sendir= $PWD\"" >> $OUTPUT
echo "cd $PWD" >> $OUTPUT
if (( "$GRIDSETUP" != "0" )); then
    echo "Grid setup is enabled."
    echo "source /vols/cms/grid/setup.sh" >> $OUTPUT
else 
    echo "Grid setup is disabled. Enabling EOS setup."
fi


echo "export SCRAM_ARCH=$SCRAM_ARCH" >> $OUTPUT
#echo "cmsenv" >> $OUTPUT
echo "eval \`scramv1 runtime -sh\`" >> $OUTPUT
echo "source $PWD/scripts/setup_libs.sh" >> $OUTPUT

if [ "$DOCERN" = "1" ]
    then
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
    echo "echo \"#!/bin/sh\" >  eosenv.sh" >> $OUTPUT
    echo "grep \"alias eos=\" /afs/cern.ch/project/eos/installation/cms/etc/setup.sh | sed \"s/alias /export my/\" >> eosenv.sh" >> $OUTPUT
    #echo "cat eosenv.sh" >> $OUTPUT
    echo "source ./eosenv.sh" >> $OUTPUT
    echo "echo \"myeos=\$myeos\"" >> $OUTPUT
    echo "\$myeos mkdir -p /eos/cms/store/user/amagnan/SkimFiles/$PRODUCTION/$OUTPUTDIR" >> $OUTPUT
    echo "cp -r $JOBDIR/* $PWD/$JOBDIR/." >> $OUTPUT
    echo "ls $OUTPUTDIR/*.root | head -n 1 > tmpfilename" >> $OUTPUT
    echo "filename=\`sed 's!/*/!/!g' tmpfilename\`" >> $OUTPUT
    echo "rm tmpfilename" >> $OUTPUT
    echo "echo \"Filename is \$filename\"" >> $OUTPUT
    echo "echo \"src path is: \$BASEDIR/\$filename\"" >> $OUTPUT
    echo "eosdest=/eos/cms/store/user/amagnan/SkimFiles/$PRODUCTION/\$filename" >> $OUTPUT
    echo "echo \"dest path is: \$eosdest\"" >> $OUTPUT
    echo "\$myeos cp \$BASEDIR/\$filename \$eosdest" >> $OUTPUT
    echo "if (( \"\$?\" != \"0\" )); then" >> $OUTPUT
    echo "echo \" --- Problem with copy of file .root to EOS. Keeping locally.\"" >> $OUTPUT
    echo "cp \$BASEDIR/\$filename $PWD/$OUTPUTDIR/." >> $OUTPUT
    echo "else" >> $OUTPUT
    echo "eossize=\`\$myeos ls -l \$eosdest | awk '{print \$5}'\`" >> $OUTPUT
    echo "localsize=\`ls -l \$BASEDIR/\$filename | awk '{print \$5}'\`" >> $OUTPUT
    echo "if (( \"\$eossize\" != \"\$localsize\" )); then" >> $OUTPUT
    echo "echo \" --- Copy of sim file to eos failed. Localsize = \$localsize, eossize = \$eossize. Keeping locally...\"" >> $OUTPUT
    echo "cp \$BASEDIR/\$filename $PWD/$OUTPUTDIR/." >> $OUTPUT
    echo "else" >> $OUTPUT
    echo "echo \" --- Size check done: Localsize = \$localsize, eossize = \$eossize\"" >> $OUTPUT
    echo "echo \" --- File  successfully copied to EOS: \$eosdest\"" >> $OUTPUT
    echo "fi" >> $OUTPUT
    echo "fi" >> $OUTPUT

fi

chmod +x $OUTPUT
