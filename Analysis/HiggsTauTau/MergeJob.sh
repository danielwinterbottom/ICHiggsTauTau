#!/bin/bash

cd /home/hep/dw515/CMSSW_8_0_0_pre6/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/
source /vols/cms/grid/setup.sh
eval `scramv1 runtime -sh`
#./bin/MergeTrees ${SGE_TASK_ID}
srmrm srm://gfe02.grid.hep.ph.ic.ac.uk:8443/srm/managerv2?SFN=/pnfs/hep.ph.ic.ac.uk/data/cms//store/user/dwinterb/ICNtuples_L1TStage2_Merged_v1/EventTreeMerged_${SGE_TASK_ID}.root
#lcg-cp EventTreeMerged/EventTreeMerged_${SGE_TASK_ID}.root srm://gfe02.grid.hep.ph.ic.ac.uk:8443/srm/managerv2?SFN=/pnfs/hep.ph.ic.ac.uk/data/cms//store/user/dwinterb/ICNtuples_L1TStage2_Merged_v1/EventTreeMerged_${SGE_TASK_ID}.root
#rm EventTreeMerged/EventTreeMerged_${SGE_TASK_ID}.root