#!/bin/bash
export X509_USER_PROXY=/home/hep/ktc17/cms.proxy
source /vols/grid/cms/setup.sh
export SCRAM_ARCH=slc7_amd64_gcc700
eval `scramv1 runtime -sh`
source /vols/cms/ktc17/CMSSW_10_2_19/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/setup_libs.sh
ulimit -c 0
lx03.hep.ph.ic.ac.uk
./bin/TauSpinnerMasters pseudo /vols/cms/ktc17/MVAFILE_AllHiggs_tt.root MVAFILE_AllHiggs_tt_pseudo_smeared_12.12GeV_phitt.root 12.12
