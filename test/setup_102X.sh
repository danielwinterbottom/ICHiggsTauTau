#!/bin/bash
set -e # exit on errors

### Basic CMSSW setup ###
export SCRAM_ARCH=slc6_amd64_gcc700
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh

CORES=`grep -c ^processor /proc/cpuinfo`

scramv1 project CMSSW_10_2_10
cd CMSSW_10_2_10/src
eval `scramv1 runtime -sh`

git cms-init

### Add-packages ###

### Merge-topics ###

# EGamma-POG
# https://twiki.cern.ch/twiki/bin/view/CMS/EgammaMiniAODV2
# The 2018 MiniAOD includes the Fall17V2 IDs for electrons but not for photons.
git cms-merge-topic cms-egamma:EgammaID_1023 # if you want the V2 IDs, otherwise skip
git cms-merge-topic cms-egamma:EgammaPostRecoTools # just adds in an extra file to have a setup function to make things easier
scram b -j $CORES

git clone git@github.com:cms-egamma/EgammaAnalysis-ElectronTools.git EgammaAnalysis/ElectronTools/data
cd EgammaAnalysis/ElectronTools/data
git checkout ScalesSmearing2018_Dev
cd -
git cms-merge-topic cms-egamma:EgammaPostRecoTools_dev
scram b -j $CORES

# TAU-POG
# DNN Tau IDs and new antiEle-Disc trainings included in offical CMSSW

# Prefiring merged into official CMSSW
# https://twiki.cern.ch/twiki/bin/view/CMS/L1ECALPrefiringWeightRecipe#Recipe_details_10_2_X_X_10

### Analysis group related software (ntuplizer, skimming, private MiniAOD, etc.) ###


# IC related packages
git clone git@github.com:danielwinterbottom/ICHiggsTauTau.git UserCode/ICHiggsTauTau
git remote add adow git@github.com:albertdow/ICHiggsTauTau.git
git fetch adow
git checkout -b 2017_cpdecays adow/2017_cpdecays

scram b -j $CORES
