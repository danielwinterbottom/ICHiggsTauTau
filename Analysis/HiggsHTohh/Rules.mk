SUBDIRS 	:= 
LIB_DEPS 	:= Core Utilities Modules HiggsTauTau 
LIB_EXTRA := -lCondFormatsJetMETObjects -L$(PWD)/HHKinFit -lHHKinFit 
