SUBDIRS 	:= LegacySVFit HHKinFit SVFit
LIB_DEPS 	:= Core Utilities Modules Objects HiggsTauTau/LegacySVFit HiggsTauTau/HHKinFit HiggsTauTau/SVFit
LIB_EXTRA := -lCondFormatsJetMETObjects -lboost_serialization 

