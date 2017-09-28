SHELL := /bin/bash
RUNDIR := $(shell pwd)
ifndef TOP
TOP := $(shell \
       top=$(RUNDIR); \
       while [ ! -r "$$top/Rules.top" ] && [ "$$top" != "" ]; do \
           top=$${top%/*}; \
       done; \
       echo $$top)
endif

# Define some useful colours
ifeq ($(NOCOL),1)
COLOR_RE :
COLOR_GR :
COLOR_YE :
COLOR_BL :
COLOR_PU :
COLOR_CY :
NOCOLOR :=
else
COLOR_RE := \033[31m
COLOR_GR := \033[32m
COLOR_YE := \033[33m
COLOR_BL := \033[34m
COLOR_PU := \033[35m
COLOR_CY := \033[36m
NOCOLOR := \033[0m
endif

# Folder containing ancillary makefile inputs
MK := $(TOP)/mk

# Setup standard includes that will be used everywhere
# -isystem is used instead of -I where we want to suppress
# compiler warnings and dependency generation for stuff thatW
# we can't actually touch

ifndef CMSSW
	CMSSW := 1
endif

ifeq ($(CMSSW),0)
$(info ==> Standalone mode enabled - CMSSW will not be linked)
$(info ==> Directories that do not define REQUIRES_CMSSW=0 will be skipped!)
endif

ifndef BOOSTSYS
	BOOSTSYS := $(shell scram tool tag boost BOOST_BASE)
endif

ifndef PYSYS
	PYSYS := $(shell scram tool tag python INCLUDE)
endif

USERINCLUDES += -I$(TOP)
USERINCLUDES += -I$(TOP)/../../..

USERINCLUDES += -isystem $(ROOTSYS)/include
USERINCLUDES += -isystem $(ROOFITSYS)/include
USERINCLUDES += -isystem $(BOOSTSYS)/include

USERLIBS += $(shell $(ROOTSYS)/bin/root-config --glibs) -lGenVector -lTreePlayer -lTMVA
USERLIBS += -L$(ROOFITSYS)/lib -lRooFit -lRooFitCore
USERLIBS += -L$(BOOSTSYS)/lib -lboost_regex -lboost_program_options -lboost_filesystem

# Can set CMSSW=0 to disable include/linking to CMSSW
ifneq ($(CMSSW), 0)
# USERINCLUDES += -I$(CMSSW_BASE)/src
	USERINCLUDES += -isystem $(CMSSW_RELEASE_BASE)/src
# CMSSW user area
USERLIBS += -L$(CMSSW_BASE)/lib/$(SCRAM_ARCH)
USERLIBS += -L$(CMSSW_RELEASE_BASE)/lib/$(SCRAM_ARCH) -lCommonToolsUtils -lPhysicsToolsFWLite
endif
# Analysis area

USERINCLUDES += -isystem $(PYSYS)

# Special includes for running the include-what-you-use tool on OS X
# USERINCLUDES += -isystem $(CMS_PATH)/$(SCRAM_ARCH)/external/gcc/4.6.2/include/c++/4.6.2
# USERINCLUDES += -isystem $(CMS_PATH)/$(SCRAM_ARCH)/external/gcc/4.6.2/include/c++/4.6.2/x86_64-apple-darwin11.2.0

CXX=g++
LD=g++
CXXFLAGS= -Wall -Wextra -Werror -O2 -std=c++1y -g
LDFLAGS= -shared -Wall -Wextra
CXXFLAGS += $(EXTRAFLAGS)

# Extra gcc flags that will generate A LOT of warnings
# -pedantic -Weffc++

CXXFLAGS += $(USERINCLUDES)
LIBS += $(USERLIBS)

.PHONY: dir tree all clean clean_all clean_tree dist_clean env

# Default target when nothing is given on the command line.  Reasonable
# options are:
# "dir"  - updates only targets from current directory and its dependencies
# "tree" - updates targets (and their dependencies) in whole subtree
#          starting at current directory
# "all"  - updates all targets in the project
.DEFAULT_GOAL := dir

dir : dir_$(RUNDIR)
	@echo -e "==> $(COLOR_GR)Directory '$(notdir $(RUNDIR))' updated$(NOCOLOR)"

tree : tree_$(RUNDIR)
	@echo -e "==> $(COLOR_GR)Directory tree starting at '$(notdir $(RUNDIR))' updated$(NOCOLOR)"

all : all_proxy
	@echo -e "==> $(COLOR_GR)Complete directory tree updated$(NOCOLOR)"

clean : clean_$(RUNDIR)

clean_tree : clean_tree_$(RUNDIR)

#dictionary:
#	rootcint -v -f src/deltaphi_dict.cc -c interface/deltaphi.h interface/LinkDef.h
#	sed "s!interface/!UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/!" src/deltaphi_dict.h > interface/deltaphi_dict.h
#	rm src/deltaphi_dict.h
#	sed "s!deltaphi_dict.h!UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/deltaphi_dict.h!" src/deltaphi_dict.cc > tmp
#	mv tmp src/deltaphi_dict.cc
env :
	@echo -e 'export ANAPATH='$(TOP)';\nexport PYTHONPATH='$(ALL_LIB_DIRS)':$$PYTHONPATH;'

# $(d) keeps the path of "current" directory during tree traversal and
# $(dir_stack) is used for backtracking during traversal
d := $(TOP)
dir_stack :=

include $(MK)/header.mk
include $(MK)/footer.mk

# Automatic inclusion of the skel.mk at the top level - that way
# Rules.top has exactly the same structure as other Rules.mk
include $(MK)/skel.mk

$(eval $(value HEADER))
include $(TOP)/Rules.top
$(eval $(value FOOTER))

