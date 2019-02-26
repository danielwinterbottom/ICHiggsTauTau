# Get DNN training files
git clone https://github.com/cms-tau-pog/RecoTauTag-TrainingFiles -b master $CMSSW_BASE/src/RecoTauTag/TrainingFiles/data
# Execute the CMSSW job:
cmsRun -j FrameworkJobReport.xml -p PSet.py
