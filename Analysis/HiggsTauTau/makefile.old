# name of the library
LIBNAME = HiggsTauTau
# HiggsTauTau also depends on the Core, Utilities and Modules libraries
USERLIBS += -L$(CMSSW_BASE)/src/UserCode/ICHiggsTauTau/Analysis/Core/lib/ -lICHiggsTauTauCore
USERLIBS += -L$(CMSSW_BASE)/src/UserCode/ICHiggsTauTau/Analysis/Utilities/lib/ -lICHiggsTauTauUtilities
USERLIBS += -L$(CMSSW_BASE)/src/UserCode/ICHiggsTauTau/Analysis/Modules/lib/ -lICHiggsTauTauModules

LIBDEPENDS += $(CMSSW_BASE)/src/UserCode/ICHiggsTauTau/Analysis/Core/lib/libICHiggsTauTauCore.so
LIBDEPENDS += $(CMSSW_BASE)/src/UserCode/ICHiggsTauTau/Analysis/Utilities/lib/libICHiggsTauTauUtilities.so
LIBDEPENDS += $(CMSSW_BASE)/src/UserCode/ICHiggsTauTau/Analysis/Modules/lib/libICHiggsTauTauModules.so



#Necessary to use shell built-in commands
SHELL=bash

# Define include paths
USERINCLUDES += -I$(ROOTSYS)/include/
USERINCLUDES += -I$(ROOFITSYS)/include/
USERINCLUDES += -I$(CMS_PATH)/$(SCRAM_ARCH)/external/boost/1.47.0/include/
USERINCLUDES += -I$(CMSSW_BASE)/src/ 
USERINCLUDES += -I$(CMSSW_RELEASE_BASE)/src/

# Define libraries to link
USERLIBS += $(shell root-config --glibs) -lGenVector -lTreePlayer -lTMVA
USERLIBS += -L$(ROOFITSYS)/lib/ -lRooFit -lRooFitCore
USERLIBS += -L$(CMS_PATH)/$(SCRAM_ARCH)/external/boost/1.47.0/lib/ -lboost_regex -lboost_program_options -lboost_filesystem
USERLIBS += -L$(CMSSW_BASE)/lib/$(SCRAM_ARCH) -lUserCodeICHiggsTauTau -lTauAnalysisCandidateTools -lCondFormatsJetMETObjects
USERLIBS += -L$(CMSSW_RELEASE_BASE)/lib/$(SCRAM_ARCH) -lFWCoreFWLite -lPhysicsToolsFWLite -lCommonToolsUtils
#CXXFLAGS = -Wall -W -Wno-unused-function -Wno-parentheses -Wno-char-subscripts -Wno-unused-parameter -O2 
CXXFLAGS = -Wall -W -O2 -std=c++0x -Wno-deprecated-declarations -Wno-unused-parameter
LDFLAGS = -shared -Wall -W 

# If possible we'll use the clang compiler, it's faster and gives more helpful error messages
# If it's not available, then fallback to gcc.
CXX=g++
LD=g++
# CLANGPATH := $(shell type -p clang++)
# ifeq ($(CLANGPATH),)
# $(warning clang++ not found, reverting to g++!)
# CXX=g++
# LD=g++
# endif

CXXFLAGS += $(USERINCLUDES)
LIBS += $(USERLIBS)

# A list of directories
BASEDIR = $(shell pwd)
LIBDIR = $(BASEDIR)/lib
EXEDIR = $(BASEDIR)/bin
SRCDIR = $(BASEDIR)/src
OBJDIR = $(BASEDIR)/obj
TESTDIR = $(BASEDIR)/test
BASEMENTDIR = $(BASEDIR)/basement
DOCDIR= $(BASEDIR)/docs
OBJ_EXT=o
TEST_EXT=cpp

# Build a list of srcs and bins to build
SRCS=$(wildcard $(BASEDIR)/src/*.cc)
EXES=$(wildcard $(BASEDIR)/test/*.cpp)
OLDEXES=$(wildcard $(BASEDIR)/basement/*.cpp)
OBJS=$(subst $(SRCDIR), $(OBJDIR),$(subst cc,$(OBJ_EXT),$(SRCS)))
BINS=$(subst $(TESTDIR), $(EXEDIR),$(subst .$(TEST_EXT),,$(EXES)))
OLDBINS=$(subst $(BASEMENTDIR), $(EXEDIR),$(subst .$(TEST_EXT),,$(OLDEXES)))


all: buildlibs
	@$(MAKE) --no-print-directory lib $(BINS)

basement: buildlibs
	@$(MAKE) --no-print-directory lib $(OLDBINS)

buildlibs:
	$(MAKE) -C ../

docs: all
	doxygen Doxyfile
	
$(EXEDIR)/%:  $(TESTDIR)/%.cpp $(LIBDIR)/lib$(LIBNAME).so $(wildcard $(BASEDIR)/interface/*.h) 
	$(CXX) -o $@ $(CXXFLAGS) $< $(LIBS) -L$(LIBDIR) -l$(LIBNAME)

$(EXEDIR)/%:  $(BASEMENTDIR)/%.cpp $(LIBDIR)/lib$(LIBNAME).so $(wildcard $(BASEDIR)/interface/*.h) 
	$(CXX) -o $@ $(CXXFLAGS) $< $(LIBS) -L$(LIBDIR) -l$(LIBNAME)


$(OBJDIR)/%.$(OBJ_EXT):  $(SRCDIR)/%.cc $(BASEDIR)/interface/%.h
	$(CXX) $(CXXFLAGS) -fPIC -c $<  -o $@

$(LIBDIR)/lib$(LIBNAME).so:  $(OBJS) $(LIBDEPENDS) 
	$(LD) $(LDFLAGS) -o $(LIBDIR)/lib$(LIBNAME).so $(OBJS) $(LIBS)
	
lib: $(LIBDIR)/lib$(LIBNAME).so

# info:
# 	@echo "LIBS: " $(LIBS)
# 	@echo "CXXFLAGS: " $(CXXFLAGS)
# 	@echo "Source files: " $(SRCS) 
# 	@echo "Object files: " $(OBJS)
# 	@echo "Executables:  " $(TARGETS)

clean:
	rm -rf $(OBJS) $(LIBDIR)/lib$(LIBNAME).so $(BINS)

