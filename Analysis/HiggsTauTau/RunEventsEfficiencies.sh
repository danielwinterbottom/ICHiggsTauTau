cd /home/hep/dw515/CMSSW_8_0_0_pre6/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau
source /vols/cms/grid/setup.sh
eval `scramv1 runtime -sh`
./bin/HiggsTauTauTrigger --cfg=./scripts/Trigger_2016.cfg --filelist=TauTauv5.dat --channel=mt --is_data=0 --output_name=EfficienciesV5/mtEfficienciesv5_2.root --line=5 --zb=0 --MakeEffPlots=1 --l1input=InputTauMuTriggers.txt
#./bin/HiggsTauTauTrigger --cfg=./scripts/Trigger_2016.cfg --filelist=TauTauv5.dat --channel=tt --is_data=0 --output_name=EfficienciesV5/ttEfficienciesv5_2.root --line=1 --zb=0 --MakeEffPlots=1 --l1input=InputTauTauTriggers.txt
#./bin/HiggsTauTauTrigger --cfg=./scripts/Trigger_2016.cfg --filelist=TauTauv5.dat --channel=et --is_data=0 --output_name=EfficienciesV5/etEfficienciesv5_2.root --line=1 --zb=0 --MakeEffPlots=1 --l1input=InputElecTauTriggers.txt
#./bin/HiggsTauTauTrigger --cfg=./scripts/Trigger_2016.cfg --filelist=TauTauv5.dat --channel=em --is_data=0 --output_name=EfficienciesV5/emEfficienciesv5_2.root --line=1 --zb=0 --MakeEffPlots=1 --l1input=InputElecMuTriggers.txt
