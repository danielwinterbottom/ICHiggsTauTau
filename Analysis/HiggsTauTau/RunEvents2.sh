cd /home/hep/dw515/CMSSW_8_0_0_pre6/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau
source /vols/cms/grid/setup.sh
eval `scramv1 runtime -sh`
./bin/HiggsTauTauTrigger --cfg=./scripts/Trigger_2016.cfg --filelist=ZeroBiasv5.dat --channel=mt --is_data=0 --output_name=RateVsEffV5/ZBmtOutput_${SGE_TASK_ID}.root --line=${SGE_TASK_ID} --zb=1 --l1input=InputTauMuTriggers.txt
#./bin/HiggsTauTauTrigger --cfg=./scripts/Trigger_2016.cfg --filelist=ZeroBiasv5.root --channel=tt --is_data=0 --output_name=RateVsEffV5/ZBttOutput_${SGE_TASK_ID}.root --line=${SGE_TASK_ID} --zb=1 --l1input=InputTauTauTriggers.txt
./bin/HiggsTauTauTrigger --cfg=./scripts/Trigger_2016.cfg --filelist=ZeroBiasv5.dat --channel=em --is_data=0 --output_name=RateVsEffV5/ZBemOutput_${SGE_TASK_ID}.root --line=${SGE_TASK_ID} --zb=1 --l1input=InputElecMuTriggers.txt
#./bin/HiggsTauTauTrigger --cfg=./scripts/Trigger_2016.cfg --filelist=ZeroBiasv5.root --channel=et --is_data=0 --output_name=RateVsEffV5/ZBetOutput_${SGE_TASK_ID}.root --line=${SGE_TASK_ID} --zb=1 --l1input=InputElecTauTriggers.txt
#lcg-cp /vols/cms02/dw515/ZeroBiasEta3/EventTreeMerged_${SGE_TASK_ID}.root srm://gfe02.grid.hep.ph.ic.ac.uk:8443/srm/managerv2?SFN=/pnfs/hep.ph.ic.ac.uk/data/cms//store/user/dwinterb/ICNtuples_L1TStage2_Merged_v3/V4PlusFixedSums/ZeroBias/EventTreeMerged_${SGE_TASK_ID}.root

#rm /vols/cms02/dw515/EventTreeMergedv2_${SGE_TASK_ID}.root

#ZeroBias_L1NTuplesV2_l1t-tsg-v4__METEtaRange3p0_v1.dat
