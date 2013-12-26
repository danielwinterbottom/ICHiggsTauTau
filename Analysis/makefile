# When I'm using default shell on Cygwin $(shell pwd) does not give the
# same result as $(shell echo $$PWD) or $(shell echo `pwd`).  It gives
# me path with symbolic links resolved whereas the later two do not do
# that.  I prefer unresolved version and since simple $(shell pwd) looks
# nicer than other two I'm sticking with bash :).
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


MK := $(TOP)/mk



USERINCLUDES += -isystem $(ROOTSYS)/include/
USERINCLUDES += -isystem $(ROOFITSYS)/include/
USERINCLUDES += -isystem $(CMS_PATH)/$(SCRAM_ARCH)/external/boost/1.47.0/include/
USERINCLUDES += -I$(CMSSW_BASE)/src/ 
USERINCLUDES += -isystem $(CMSSW_RELEASE_BASE)/src/

# Define libraries to link
USERLIBS += $(shell root-config --glibs) -lGenVector -lTreePlayer -lTMVA
USERLIBS += -L$(ROOFITSYS)/lib/ -lRooFit -lRooFitCore
USERLIBS += -L$(CMS_PATH)/$(SCRAM_ARCH)/external/boost/1.47.0/lib/ -lboost_regex -lboost_program_options -lboost_filesystem
USERLIBS += -L$(CMSSW_BASE)/lib/$(SCRAM_ARCH) -lUserCodeICHiggsTauTau -lTauAnalysisCandidateTools
USERLIBS += -L$(CMSSW_RELEASE_BASE)/lib/$(SCRAM_ARCH) -lFWCoreFWLite -lPhysicsToolsFWLite -lCommonToolsUtils

#CXXFLAGS = -Wall -W -Wno-unused-function -Wno-parentheses -Wno-char-subscripts -Wno-unused-parameter -O2 
CXXFLAGS = -Wall -Wextra -O2 -pedantic -std=c++0x
LDFLAGS = -shared -Wall -Wextra

# -Wall -Wextra -pedantic -Weffc++

# If possible we'll use the clang compiler, it's faster and gives more helpful error messages
# If it's not available, then fallback to gcc.
CXX=g++
LD=g++
# Update 17/2/2013:  Remove support for compiling with clang as there are issues with
# C++11

# CLANGPATH := $(shell type -p clang++)
# ifeq ($(CLANGPATH),)
# $(warning clang++ not found, reverting to g++!)
# CXX=g++
# LD=g++
# endif

CXXFLAGS += $(USERINCLUDES)
LIBS += $(USERLIBS)


.PHONY: dir tree all clean clean_all clean_tree dist_clean

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

all_proxy : all
	@echo -e "==> $(COLOR_GR)Complete directory tree updated$(NOCOLOR)"


clean : clean_$(RUNDIR)
clean_tree : clean_tree_$(RUNDIR)

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


print-%:
	@echo '$*=$($*)'
