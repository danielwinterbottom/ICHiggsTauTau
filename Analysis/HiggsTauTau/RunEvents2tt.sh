cd /home/hep/dw515/NTupleProduction/l1t-tsg-v5/CMSSW_8_0_2/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau
source /vols/grid/cms/setup.sh
eval `scramv1 runtime -sh`
./bin/HiggsTauTauTrigger --cfg=./scripts/Trigger_2016.cfg --filelist=fileinputs/ZeroBias_int_v58pt1.dat --channel=tt --is_data=0 --output_name=VBFNonSpecific/ZBttOutput_${SGE_TASK_ID}.root --line=${SGE_TASK_ID} --zb=1 --l1input=VBFNonSpecific.txt
