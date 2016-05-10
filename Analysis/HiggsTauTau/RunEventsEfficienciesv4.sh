cd /home/hep/dw515/CMSSW_8_0_0_pre6/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau
source /vols/cms/grid/setup.sh
eval `scramv1 runtime -sh`
./bin/HiggsTauTauTrigger --cfg=./scripts/Trigger_2016.cfg --filelist=TauTauv4.dat --channel=mt --is_data=0 --output_name=EfficienciesV4/mtEfficienciesv4_2.root --line=5 --zb=0 --MakeEffPlots=1 --l1input=InputTauMuTriggers.txt
#./bin/HiggsTauTauTrigger --cfg=./scripts/Trigger_2016.cfg --filelist=TauTauv34Eta3.dat --channel=tt --is_data=0 --output_name=EfficienciesV34/ttEfficienciesv34_2.root --line=1 --zb=0 --MakeEffPlots=1 --l1input=InputTauTauTriggers.txt
#./bin/HiggsTauTauTrigger --cfg=./scripts/Trigger_2016.cfg --filelist=TauTauv34Eta3.dat --channel=et --is_data=0 --output_name=EfficienciesV34/etEfficienciesv34_2.root --line=1 --zb=0 --MakeEffPlots=1 --l1input=InputElecTauTriggers.txt
#./bin/HiggsTauTauTrigger --cfg=./scripts/Trigger_2016.cfg --filelist=TauTauv34Eta3.dat --channel=em --is_data=0 --output_name=EfficienciesV34/emEfficienciesv34_2.root --line=1 --zb=0 --MakeEffPlots=1 --l1input=InputElecMuTriggers.txt
