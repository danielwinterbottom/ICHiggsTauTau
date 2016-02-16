SUBDIRS 	:= LegacySVFit HHKinFit
LIB_DEPS 	:= Core Utilities Modules Objects HiggsTauTau/LegacySVFit HiggsTauTau/HHKinFit
LIB_EXTRA := -lCondFormatsJetMETObjects -lboost_serialization 

