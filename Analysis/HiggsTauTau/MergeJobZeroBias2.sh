#!/bin/bash
cd /home/hep/dw515/NTupleProduction/l1t-tsg-v5/CMSSW_8_0_2/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau
source /vols/cms/grid/setup.sh
eval `scramv1 runtime -sh`
./bin/MergeTreesZeroBias2 ${SGE_TASK_ID}
cd /vols/cms02/dw515/l1t-tsg-v5/ZeroBias/BothSumRanges
lcg-cp EventTree1_${SGE_TASK_ID}.root srm://gfe02.grid.hep.ph.ic.ac.uk:8443/srm/managerv2?SFN=/pnfs/hep.ph.ic.ac.uk/data/cms//store/user/dwinterb/ICNtuples_L1TStage2_Merged_v5_V2/ZeroBias/EventTree2_${SGE_TASK_ID}.root
