#!/bin/bash
export X509_USER_PROXY=/home/hep/ac4317/cms.proxy
source /vols/grid/cms/setup.sh
export SCRAM_ARCH=slc7_amd64_gcc700
eval `scramv1 runtime -sh`
source /vols/cms/ac4317/msci/CMSSW_10_2_19/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/setup_libs.sh
ulimit -c 0
lx03.hep.ph.ic.ac.uk
./bin/TauSpinnerMasters reco /vols/cms/ac4317/msci/MVAFILE_AllHiggs_tt_regr_10_10.root MVAFILE_AllHiggs_tt_reco_10_10_phitt.root
