#!/bin/sh
DOCERN=0

if [ "$#" -ne "2" ]; then
    echo "Usage: $0 <doSubmit> <do4params>"
    exit 0
fi


DOSUBMIT=$1
DO4PARAMS=$2
infolder=output_run2ana_160222
outfolder=cards_run2ana_160222/
blind=true
zvvstat=18
mkdir -p $outfolder

extraoptions="" #--do_ggh=false --do_separate_qcdewk=false"


## Try and take the JOBWRAPPER and JOBSUBMIT commands
## from the environment if set, otherwise use these defaults
: ${JOBWRAPPER:="./scripts/generate_job.sh"}
: ${JOBSUBMIT:="eval"}

#export JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"

GRIDSETUP=1
if [ "$DOCERN" = "0" ]
    then
    JOBSCRIPT="./scripts/submit_ic_batch_job.sh"
else
    JOBSCRIPT="./scripts/submit_cern_batch_job.sh"
    GRIDSETUP=0
fi

echo "Using job-wrapper: " $JOBWRAPPER

QUEUEDIR=short #medium #medium long

if [ "$DOCERN" = "0" ]
    then
    if [ "$QUEUEDIR" = "medium" ]
	then
	JOBQUEUE="hepmedium.q"
    elif [ "$QUEUEDIR" = "long" ]
	then
	JOBQUEUE="heplong.q"
    else
	JOBQUEUE="hepshort.q"
    fi
else
    if [ "$QUEUEDIR" = "medium" ]
	then
	JOBQUEUE="1nd"
    elif [ "$QUEUEDIR" = "long" ]
	then
	JOBQUEUE="2nd"
    else
	JOBQUEUE="1nh"
    fi
    
fi
export JOBSUBMIT=$JOBSCRIPT" "$JOBQUEUE
echo "Using job-submission: " $JOBSUBMIT


for mass in 110 125 150 200 300 400 500 600
do

    mkdir -p $outfolder/$mass
    for mindphicut in 1.01 1.21 1.41 1.61 1.81 2.01 2.11 2.21 2.31 2.41 2.51 2.61 2.81
    do
        echo "-----------------------------------------------------"
        echo "---Processing mindphi>$mindphicut"
        echo "-----------------------------------------------------"

	JOB=sig_${mass}_${mindphicut}
	OUTPUT=$outfolder/$mass/$JOB.sh
	echo "cd $PWD" &> $OUTPUT
	echo "source /vols/cms/grid/setup.sh" >> $OUTPUT
	echo "source ~/CMS/cms_env_version.sh 7_6_3" >> $OUTPUT
	echo "source $PWD/scripts/setup_libs.sh" >> $OUTPUT

        for minjjcut in 801 901 1001 1101 1201 1301 1401 1501 1601 1701 1801 1901
        do
            OUTNAME=$outfolder/$mass/vbfhinv_${mass}_8TeV_${mindphicut}_${minjjcut}.txt
            if (( "$DO4PARAMS" == "1" )); then
                OUTNAME=$outfolder/$mass/vbfhinv_${mass}_8TeV_${mindphicut}_${minjjcut}_4params.txt
            fi

            echo "./bin/makeCountingCard -i $infolder --blind=$blind -o $OUTNAME -m $mass --do_latex true --do_datatop false --zvvstat $zvvstat --mcBkgOnly=true --do_run2=true --do_4params=$DO4PARAMS --minvarXcut=$minjjcut --minvarYcut=$mindphicut --histoToIntegrate=alljetsmetnomu_mindphi:dijet_M $extraoptions  &> $outfolder/$mass/card_${mindphicut}_${minjjcut}.log" >> $OUTPUT

	done
	chmod +x $OUTPUT
	    if [ "$DOSUBMIT" = "1" ]; then
		$JOBSUBMIT $OUTPUT
            else
		echo "$JOBSUBMIT $OUTPUT"
            fi
	    
    done
done