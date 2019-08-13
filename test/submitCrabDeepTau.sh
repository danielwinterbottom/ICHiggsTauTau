# Get DNN training files
git clone https://github.com/cms-tau-pog/RecoTauTag-TrainingFiles -b master $CMSSW_BASE/src/RecoTauTag/TrainingFiles/data
# Execute the CMSSW job:
cmsRun -j FrameworkJobReport.xml -p PSet.py

#wget https://github.com/cms-tau-pog/RecoTauTag-TrainingFiles/raw/DeepTau2017v2/DeepTauId/deepTau_2017v2p6_e6_core.pb -P RecoTauTag/TrainingFiles/data/DeepTauId
#wget https://github.com/cms-tau-pog/RecoTauTag-TrainingFiles/raw/DeepTau2017v2/DeepTauId/deepTau_2017v2p6_e6_inner.pb -P RecoTauTag/TrainingFiles/data/DeepTauId
#wget https://github.com/cms-tau-pog/RecoTauTag-TrainingFiles/raw/DeepTau2017v2/DeepTauId/deepTau_2017v2p6_e6_outer.pb -P RecoTauTag/TrainingFiles/data/DeepTauId
