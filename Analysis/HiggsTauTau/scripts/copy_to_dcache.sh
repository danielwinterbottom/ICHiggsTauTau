cd /vols/cms04/ajg06/CMSSW_5_3_4/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau
export SCRAM_ARCH=slc5_amd64_gcc462
eval `scramv1 runtime -sh`
source /vols/cms04/ajg06/CMSSW_5_3_4/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/setup_libs.sh
eval python ~/sesync/sesync.py -v -c -y -k -d 6 /vols/ssd00/cms/ajg06/Dec30 srm://gfe02.grid.hep.ph.ic.ac.uk:8443/srm/managerv2?SFN=/pnfs/hep.ph.ic.ac.uk/data/cms/store/user/agilbert/Skims &> copy.log
