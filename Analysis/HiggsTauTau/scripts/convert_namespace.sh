make -j4

#SKIMMING

SOURCE_PATH=/Volumes/Storage/Sept11/Data_42X

PATHS=( 'TauPlusX-2011A-May10ReReco-v1'
        'TauPlusX-2011A-PromptReco-v4'
        'TauPlusX-2011A-05Aug2011-v1'
        'TauPlusX-2011A-03Oct2011-v1'
        'TauPlusX-2011B-PromptReco-v1'
        'MuEG-2011A-May10ReReco-v1'
        'MuEG-2011A-PromptReco-v4'
        'MuEG-2011A-05Aug2011-v1'
        'MuEG-2011A-03Oct2011-v1'
        'MuEG-2011B-PromptReco-v1'
        'Embedded-ETau-2011A-May10ReReco-v1'
        'Embedded-ETau-2011A-PromptReco-v4'
        'Embedded-ETau-2011A-05Aug2011-v1'
        'Embedded-ETau-2011A-03Oct2011-v1'
        'Embedded-ETau-2011B-PromptReco-v1'
        'Embedded-MTau-2011A-May10ReReco-v1'
        'Embedded-MTau-2011A-PromptReco-v4'
        'Embedded-MTau-2011A-05Aug2011-v1'
        'Embedded-MTau-2011A-03Oct2011-v1'
        'Embedded-MTau-2011B-PromptReco-v1'
        'Embedded-EM-2011A-May10ReReco-v1'
        'Embedded-EM-2011A-PromptReco-v4'
        'Embedded-EM-2011A-05Aug2011-v1'
        'Embedded-EM-2011A-03Oct2011-v1'
        'Embedded-EM-2011B-PromptReco-v1'
     )

for i in "${PATHS[@]}"
do
    cd $SOURCE_PATH
    find $i/electauSkim/IC*.root &> "$i"/electauSkim/IC_filelist.dat
    find $i/mutauSkim/IC*.root &> "$i"/mutauSkim/IC_filelist.dat
    find $i/elecmuSkim/IC*.root &> "$i"/elecmuSkim/IC_filelist.dat
    cd -
    cp $SOURCE_PATH/"$i"/electauSkim/IC_filelist.dat filelists/Sept11/Data_42X/"$i"_electauSkim_IC_filelist.dat
    cp $SOURCE_PATH/"$i"/mutauSkim/IC_filelist.dat filelists/Sept11/Data_42X/"$i"_mutauSkim_IC_filelist.dat
    cp $SOURCE_PATH/"$i"/elecmuSkim/IC_filelist.dat filelists/Sept11/Data_42X/"$i"_elecmuSkim_IC_filelist.dat

#  ./bin/Conversion $SOURCE_PATH/$i/electauSkim/filelist.dat
#  ./bin/Conversion $SOURCE_PATH/$i/mutauSkim/filelist.dat
#  ./bin/Conversion $SOURCE_PATH/$i/elecmuSkim/filelist.dat
done

# SOURCE_PATH=/Volumes/Storage/Sept11/MC_42X

# PATHS=( 'WJetsToLNu'
#         'W1JetsToLNu'
#         'W2JetsToLNu'
#         'W3JetsToLNu'
#         'W4JetsToLNu'
#         'TTJets-v1'
#         'TTJets-v2'
#         'T-tW'
#         'Tbar-tW'
#         'WWJetsTo2L2Nu'
#         'WZJetsTo2L2Q'
#         'WZJetsTo3LNu'
#         'ZZJetsTo2L2Nu'
#         'ZZJetsTo2L2Q'
#         'ZZJetsTo4L'
#         'GluGluToHToTauTau_M-110' 
#         'GluGluToHToTauTau_M-115' 
#         'GluGluToHToTauTau_M-120' 
#         'GluGluToHToTauTau_M-125' 
#         'GluGluToHToTauTau_M-130' 
#         'GluGluToHToTauTau_M-135' 
#         'GluGluToHToTauTau_M-140' 
#         'GluGluToHToTauTau_M-145' 
#         'VBF_HToTauTau_M-110' 
#         'VBF_HToTauTau_M-115' 
#         'VBF_HToTauTau_M-120' 
#         'VBF_HToTauTau_M-125' 
#         'VBF_HToTauTau_M-130' 
#         'VBF_HToTauTau_M-135' 
#         'VBF_HToTauTau_M-140' 
#         'VBF_HToTauTau_M-145' 
#         'WH_ZH_TTH_HToTauTau_M-110' 
#         'WH_ZH_TTH_HToTauTau_M-115' 
#         'WH_ZH_TTH_HToTauTau_M-120' 
#         'WH_ZH_TTH_HToTauTau_M-125' 
#         'WH_ZH_TTH_HToTauTau_M-130' 
#         'WH_ZH_TTH_HToTauTau_M-135' 
#         'WH_ZH_TTH_HToTauTau_M-140' 
#         'WH_ZH_TTH_HToTauTau_M-145'
#         'SUSYGluGluToHToTauTau_M-90'
#         'SUSYGluGluToHToTauTau_M-100'
#         'SUSYGluGluToHToTauTau_M-120'
#         'SUSYGluGluToHToTauTau_M-130'
#         'SUSYGluGluToHToTauTau_M-140'
#         'SUSYGluGluToHToTauTau_M-160'
#         'SUSYGluGluToHToTauTau_M-180'
#         'SUSYGluGluToHToTauTau_M-200'
#         'SUSYGluGluToHToTauTau_M-250'
#         'SUSYGluGluToHToTauTau_M-300'
#         'SUSYGluGluToHToTauTau_M-350'
#         'SUSYGluGluToHToTauTau_M-400'
#         'SUSYGluGluToHToTauTau_M-450'
#         'SUSYGluGluToHToTauTau_M-500'
#         'SUSYGluGluToHToTauTau_M-600'
#         'SUSYGluGluToHToTauTau_M-700'
#         'SUSYGluGluToHToTauTau_M-800'
#         'SUSYGluGluToHToTauTau_M-900'
#         'SUSYGluGluToHToTauTau_M-1000'
#         'SUSYBBHToTauTau_M-90'
#         'SUSYBBHToTauTau_M-100'
#         'SUSYBBHToTauTau_M-120'
#         'SUSYBBHToTauTau_M-130'
#         'SUSYBBHToTauTau_M-140'
#         'SUSYBBHToTauTau_M-160'
#         'SUSYBBHToTauTau_M-180'
#         'SUSYBBHToTauTau_M-200'
#         'SUSYBBHToTauTau_M-250'
#         'SUSYBBHToTauTau_M-300'
#         'SUSYBBHToTauTau_M-350'
#         'SUSYBBHToTauTau_M-400'
#         'SUSYBBHToTauTau_M-450'
#         'SUSYBBHToTauTau_M-500'
#         'SUSYBBHToTauTau_M-600'
#         'SUSYBBHToTauTau_M-700'
#         'SUSYBBHToTauTau_M-800'
#         'SUSYBBHToTauTau_M-900'
#         'SUSYBBHToTauTau_M-1000'
#      )

# cd $SOURCE_PATH
# for i in "${PATHS[@]}"
# do
#  find "$i"/electauSkim/IC* &> "$i"/electauSkim/IC_filelist.dat
#  find "$i"/mutauSkim/IC* &> "$i"/mutauSkim/IC_filelist.dat
#  find "$i"/elecmuSkim/IC* &> "$i"/elecmuSkim/IC_filelist.dat
#  # ./bin/Conversion $SOURCE_PATH/$i/electauSkim/filelist.dat
#  # ./bin/Conversion $SOURCE_PATH/$i/mutauSkim/filelist.dat
#  # ./bin/Conversion $SOURCE_PATH/$i/elecmuSkim/filelist.dat
# done