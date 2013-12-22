# name of the library
LIBNAME = ICHiggsTauTauCore

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
USERLIBS += -L$(CMSSW_BASE)/lib/$(SCRAM_ARCH) -lUserCodeICHiggsTauTau -lTauAnalysisCandidateTools
USERLIBS += -L$(CMSSW_RELEASE_BASE)/lib/$(SCRAM_ARCH) -lFWCoreFWLite -lPhysicsToolsFWLite -lCommonToolsUtils

#CXXFLAGS = -Wall -W -Wno-unused-function -Wno-parentheses -Wno-char-subscripts -Wno-unused-parameter -O2 
CXXFLAGS = -Wall -W -O2 -std=c++0x
LDFLAGS = -shared -Wall -W 

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

# A list of directories
BASEDIR = $(shell pwd)
LIBDIR = $(BASEDIR)/lib
EXEDIR = $(BASEDIR)/bin
SRCDIR = $(BASEDIR)/src
OBJDIR = $(BASEDIR)/obj
TESTDIR = $(BASEDIR)/test
DOCDIR= $(BASEDIR)/docs
OBJ_EXT=o
TEST_EXT=cpp

# Build a list of srcs and bins to build
SRCS=$(wildcard $(BASEDIR)/src/*.cc)
EXES=$(wildcard $(BASEDIR)/test/*.cpp)
OBJS=$(subst $(SRCDIR), $(OBJDIR),$(subst cc,$(OBJ_EXT),$(SRCS)))
EXE_OBJS=$(subst $(TESTDIR), $(EXEDIR),$(subst cpp,$(OBJ_EXT),$(EXES)))
BINS=$(subst $(TESTDIR), $(EXEDIR),$(subst .$(TEST_EXT),,$(EXES)))

all:  lib $(BINS)


.SECONDARY: $(EXE_OBJS)

docs: all
	doxygen Doxyfile
	
$(EXEDIR)/%: $(EXEDIR)/%.o $(LIBDIR)/lib$(LIBNAME).so 
	$(CXX) -o $@  $< $(LIBS) -L$(LIBDIR) -l$(LIBNAME)

-include $(EXE_OBJS:.o=.d)

-include $(OBJS:.o=.d)

$(OBJDIR)/%.$(OBJ_EXT): $(SRCDIR)/%.cc
	$(CXX) $(CXXFLAGS) -fPIC -c $<  -o $@
	$(CXX) $(CXXFLAGS) -MM -MP -MT "$@" $< -o $(OBJDIR)/$*.d

$(EXEDIR)/%.$(OBJ_EXT): $(TESTDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -fPIC -c $<  -o $@
	$(CXX) $(CXXFLAGS) -MM -MP -MT "$@" $< -o $(EXEDIR)/$*.d

$(LIBDIR)/lib$(LIBNAME).so: $(OBJS)
	$(LD) $(LDFLAGS) -o $(LIBDIR)/lib$(LIBNAME).so $(OBJS) $(LIBS)
	
lib: $(LIBDIR)/lib$(LIBNAME).so


# info:
# 	@echo "LIBS: " $(LIBS)
# 	@echo "CXXFLAGS: " $(CXXFLAGS)
# 	@echo "Source files: " $(SRCS) 
# 	@echo "Object files: " $(OBJS)
# 	@echo "Executables:  " $(TARGETS)

clean:
	rm -rf $(EXEDIR)/*.o $(EXEDIR)/*.d $(OBJDIR)/*.o $(OBJDIR)/*.d $(LIBDIR)/lib$(LIBNAME).so $(BINS) 


