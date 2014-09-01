#!/bin/sh
CURRDIR=$PWD
cd $CMSSW_BASE
git-cms-addpkg EgammaAnalysis/ElectronTools
git-cms-addpkg RecoJets/JetProducers 
cd $CMSSW_BASE/src/EgammaAnalysis/ElectronTools/data/
cat download.url | xargs wget
cd $CMSSW_BASE/src/RecoJets/JetProducers/data/
cat download.url | xargs wget
cd CURRDIR
