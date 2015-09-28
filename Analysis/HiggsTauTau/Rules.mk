SUBDIRS 	:= LegacySVFit HHKinFit SVFitRun2
LIB_DEPS 	:= Core Utilities Modules Objects HiggsTauTau/LegacySVFit HiggsTauTau/HHKinFit HiggsTauTau/SVFitRun2
LIB_EXTRA := -lCondFormatsJetMETObjects -lboost_serialization 

