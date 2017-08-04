SUBDIRS 	:=
LIB_DEPS 	:= Core Utilities Modules
LIB_EXTRA := -L$(shell scram tool tag fastjet FASTJET_BASE)/lib -lfastjettools -lfastjet
PKG_FLAGS := -isystem $(shell scram tool tag fastjet FASTJET_BASE)/include