if [ -z $2 ]
then
    echo "Must specify <script input> <script output>"
    exit
fi


INPUT=$1
OUTPUT=$2

echo "Generating job $OUTPUT"

echo "#!/bin/bash" &> $OUTPUT
echo "export X509_USER_PROXY=$HOME/.globus/user_proxy.pem" >> $OUTPUT
echo "source /usr/share/Modules/init/bash" >> $OUTPUT
echo "module use -a /afs/desy.de/group/cms/modulefiles/; module load cmssw" >> $OUTPUT
echo "cd $PWD" >> $OUTPUT
echo "export SCRAM_ARCH=$SCRAM_ARCH" >> $OUTPUT
echo "eval \`scramv1 runtime -sh\`" >> $OUTPUT
echo "eval $INPUT" >> $OUTPUT
chmod +x $OUTPUT
