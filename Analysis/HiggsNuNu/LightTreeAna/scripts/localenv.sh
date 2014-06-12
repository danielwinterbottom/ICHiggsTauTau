export CMSSW_BASE=~/SOFTWARE/CMS/Hinvisible/
export ROOFITSYS=$ROOTSYS/roofit/roofit/
alias lmake='make -f LocalMakefile'
ln -s ../interface/HiggsNuNuAnalysisTools.h interface/
ln -s ../src/HiggsNuNuAnalysisTools.cc src/
ln -s LocalRules.mk Rules.top
ln -s ../../mk mk
