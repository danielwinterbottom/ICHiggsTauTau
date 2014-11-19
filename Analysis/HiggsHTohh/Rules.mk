SUBDIRS 	:= 
LIB_DEPS 	:= Core Utilities Modules HiggsTauTau 
LIB_EXTRA := -lCondFormatsJetMETObjects -L$(TOP)/HiggsHTohh/HHKinFit -lHHKinFit 
