cd /home/hep/dw515/CMSSW_8_0_0_pre6/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau
source /vols/cms/grid/setup.sh
eval `scramv1 runtime -sh`
#./bin/HiggsTauTauTrigger --cfg=./scripts/Trigger_2016.cfg --filelist=TauTauv5.dat --channel=mt --is_data=0 --output_name=RateVsEffV5/mtOutput_${SGE_TASK_ID}.root --line=${SGE_TASK_ID} --l1input=InputTauMuTriggers.txt
./bin/HiggsTauTauTrigger_ZPass --cfg=./scripts/Trigger_2016.cfg --filelist=L1ZJets.dat --channel=mt --is_data=0 --output_name=RateVsEffV5/L1ZEvents_${SGE_TASK_ID}.root --line=${SGE_TASK_ID} --l1input=InputTauMuTriggers.txt --zb=1
