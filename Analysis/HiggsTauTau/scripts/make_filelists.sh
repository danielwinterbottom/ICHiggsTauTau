# PREPATH=Nov26/Data_53X
# SOURCE_PATH=/internal/$PREPATH

# PREPATH=Nov26/Data_53X
# SOURCE_PATH=/Volumes/Storage/$PREPATH

# PATHS=( 
#     'DoubleElectron-2012A-13Jul2012-v1'
#     'DoubleElectron-2012B-13Jul2012-v1'
#     'DoubleElectron-2012C-24Aug2012-v1'
#     'DoubleElectron-2012C-PromptReco-v2'
#     'DoubleElectron-2012D-PromptReco-v1'
#     'DoubleMu-2012A-13Jul2012-v1'
#     'DoubleMu-2012B-13Jul2012-v4'
#     'DoubleMu-2012C-24Aug2012-v1'
#     'DoubleMu-2012C-PromptReco-v2'
#     'DoubleMu-2012D-PromptReco-v1'
#       )

# RETDIR=$PWD

# cd $SOURCE_PATH

# for i in "${PATHS[@]}"
# do
# # # # # Then generate new filelists
# #find $i/EventTree*.root &> $RETDIR/filelists/$PREPATH/"$i"_filelist.dat
# find $i/zee_skim/EventTree*.root &> $RETDIR/filelists/$PREPATH/"$i"_zee_skim_filelist.dat
# find $i/zmm_skim/EventTree*.root &> $RETDIR/filelists/$PREPATH/"$i"_zmm_skim_filelist.dat
# done

# cd -


PREPATH=Dec2/Data_53X
SOURCE_PATH=/internal/$PREPATH
PATHS=( 
    'MuEG-2012A-13Jul2012-v1'
    'MuEG-2012A-recover-06Aug2012-v1'
    'MuEG-2012B-13Jul2012-v1'
    'MuEG-2012C-24Aug2012-v1'
    'MuEG-2012C-PromptReco-v2'
    'MuEG-2012D-PromptReco-v1'
    'TauPlusX-2012A-13Jul2012-v1'
    'TauPlusX-2012A-recover-06Aug2012-v1'
    'TauPlusX-2012B-13Jul2012-v1'
    'TauPlusX-2012C-24Aug2012-v1'
    'TauPlusX-2012C-PromptReco-v2'
    'TauPlusX-2012D-PromptReco-v1'
)
RETDIR=$PWD
cd $SOURCE_PATH
for i in "${PATHS[@]}"
do
find $i/EventTree*.root &> $RETDIR/filelists/$PREPATH/"$i"_filelist.dat
done
cd -


PREPATH=Dec2/MC_53X
SOURCE_PATH=/internal/$PREPATH
PATHS=( 
    'DYJetsToLL'
    'WJetsToLNu-v1'
    'WJetsToLNu-v2-PART1'
    'WJetsToLNu-v2-PART2'
    'W1JetsToLNu'
    'W2JetsToLNu'
    'W3JetsToLNu'
    'W4JetsToLNu'
    'TTJets'
    'T-tW'
    'Tbar-tW'
    'WWJetsTo2L2Nu'
    'WZJetsTo2L2Q'
    'WZJetsTo3LNu'
    'ZZJetsTo2L2Nu'
    'ZZJetsTo2L2Q'
    'ZZJetsTo4L'
    'GluGluToHToTauTau_M-125'
    'VBF_HToTauTau_M-125'
    'WH_ZH_TTH_HToTauTau_M-125'
    'SUSYGluGluToHToTauTau_M-160'
    'SUSYBBHToTauTau_M-160'
)
RETDIR=$PWD
cd $SOURCE_PATH
for i in "${PATHS[@]}"
do
find $i/EventTree*.root &> $RETDIR/filelists/$PREPATH/"$i"_filelist.dat
done
cd -


PREPATH=Dec3/Embedded_53X
SOURCE_PATH=/internal/$PREPATH
PATHS=( 
    'Embedded-EM-2012A-13Jul2012-v1'
    'Embedded-EM-2012A-recover-06Aug2012-v1'
    'Embedded-EM-2012B-13Jul2012-v1'
    'Embedded-EM-2012C-24Aug2012-v1'
    'Embedded-EM-2012C-PromptReco-v2'
    'Embedded-ETau-2012A-13Jul2012-v1'
    'Embedded-ETau-2012A-recover-06Aug2012-v1'
    'Embedded-ETau-2012B-13Jul2012-v1'
    'Embedded-ETau-2012C-24Aug2012-v1'
    'Embedded-ETau-2012C-PromptReco-v2'
    'Embedded-MTau-2012A-13Jul2012-v1'
    'Embedded-MTau-2012A-recover-06Aug2012-v1'
    'Embedded-MTau-2012B-13Jul2012-v1'
    'Embedded-MTau-2012C-24Aug2012-v1'
    'Embedded-MTau-2012C-PromptReco-v2'
)
RETDIR=$PWD
cd $SOURCE_PATH
for i in "${PATHS[@]}"
do
find $i/EventTree*.root &> $RETDIR/filelists/Dec2/Embedded_53X/"$i"_filelist.dat
done
cd -




