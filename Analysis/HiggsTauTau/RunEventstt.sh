cd /home/hep/dw515/NTupleProduction/l1t-tsg-v5/CMSSW_8_0_2/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau
source /vols/cms/grid/setup.sh
eval `scramv1 runtime -sh`
./bin/HiggsTauTauTrigger --cfg=./scripts/Trigger_2016.cfg --filelist=TauTauv5.dat --channel=tt --is_data=0 --output_name=TotalEfficiencies/ttOutput_${SGE_TASK_ID}.root --line=${SGE_TASK_ID} --l1input=InputTauTauTriggers.txt
