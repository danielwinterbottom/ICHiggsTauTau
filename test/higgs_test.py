import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from Configuration.StandardSequences.Eras import eras
import os
import sys
import commands

options = VarParsing.VarParsing ('analysis')

options.register ('era',    'stage2',  VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,  "The data taking Era: stage1 or stage2")
options.register ('sample', 'data',    VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,  "The Sample type: data or mc")
options.register ('output', 'DEFAULT', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,  "The output file name")
options.register ('max',    '',        VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int,     "The maximum number of events to process")

options.max      = 10000
options.sample   = "mc"

options.parseArguments()

print "sample = ",options.sample

if (options.era == 'stage1'):
    print "INFO: runnings L1T Stage-1 (2015) Re-Emulation"
    process = cms.Process("AnalysisDecays", eras.Run2_25ns)
elif (options.era == 'stage2'):
    print "INFO: runnings L1T Stage-2 (2016) Re-Emulation"    
    process = cms.Process("AnalysisDecays", eras.Run2_2016)
else:
    print "ERROR: unknown era:  ", options.era, "\n"
    exit(0)

if (options.output == 'DEFAULT'):
    if (eras.stage1L1Trigger.isChosen()):
        options.output ='l1t_stage1.root'
    if (eras.stage2L1Trigger.isChosen()):
        options.output ='l1t_stage2.root'
print "INFO: output: ", options.output

print "INFO: max   : ", options.max

################################################################
### Messages
################################################################
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

################################################################
### Aligment and condition
################################################################
# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')

# PostLS1 geometry used
process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2015_cff')
############################
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag

#   For stage-1, we are re-emulating L1T based on the conditions in the GT, so
#   best for now to use MC GT, even when running over a data file, and just
#   ignore the main DT TP emulator warnings...  (In future we'll override only
#   L1T emulator related conditions, so you can use a data GT)
if (eras.stage1L1Trigger.isChosen()):
  
  if options.sample=="mc":
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

  elif options.sample=="data":
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

# Note:  For stage-2, all conditions are overriden by local config file.  Use data tag: 

if (eras.stage2L1Trigger.isChosen()):

  if options.sample=="mc":
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '') 

  elif options.sample=="data":
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
    process.GlobalTag.toGet = cms.VPSet(
        cms.PSet(record = cms.string("DTCCBConfigRcd"),
                 tag = cms.string("DTCCBConfig_NOSingleL_V05_mc"),
                 connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
                 ),
#        cms.PSet(record = cms.string("DTKeyedConfigListRcd"),
#                 tag = cms.string("DTKeyedConfigContainer_NOSingleL_V05_mc"),
#                 connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
#                 ),
        cms.PSet(record = cms.string("DTT0Rcd"),
                 tag = cms.string("DTt0_STARTUP_V02_mc"),
                 connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
                 ),
        cms.PSet(record = cms.string("DTTPGParametersRcd"),
                 tag = cms.string("DTTPGParameters_V01_mc"),
                 connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
                 ),
        )

#### Sim L1 Emulator Sequence:
process.load('Configuration.StandardSequences.RawToDigi_cff')
#process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('L1Trigger.Configuration.L1TReEmulateFromRAW_cff')

process.source = cms.Source("PoolSource",
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring(
     #'file:/afs/cern.ch/work/p/pela/private/sample/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer15GS-MCRUN2_71_V1-v1/GEN-SIM/8E765C66-7658-E511-A378-00215E2EAEDE.root'
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_1.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_10.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_100.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_101.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_103.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_104.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_105.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_106.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_107.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_108.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_109.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_11.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_110.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_111.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_112.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_113.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_114.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_115.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_116.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_117.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_118.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_119.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_12.root'
    ),
    #inputCommands = cms.untracked.vstring('keep *', 
        #'drop *_genParticles_*_*', 
        #'drop *_genParticlesForJets_*_*', 
        #'drop *_kt4GenJets_*_*', 
        #'drop *_kt6GenJets_*_*', 
        #'drop *_iterativeCone5GenJets_*_*', 
        #'drop *_ak4GenJets_*_*', 
        #'drop *_ak7GenJets_*_*', 
        #'drop *_ak8GenJets_*_*', 
        #'drop *_ak4GenJetsNoNu_*_*', 
        #'drop *_ak8GenJetsNoNu_*_*', 
        #'drop *_genCandidatesForMET_*_*', 
        #'drop *_genParticlesForMETAllVisible_*_*', 
        #'drop *_genMetCalo_*_*', 
        #'drop *_genMetCaloAndNonPrompt_*_*', 
        #'drop *_genMetTrue_*_*', 
        #'drop *_genMetIC5GenJs_*_*'),
    secondaryFileNames = cms.untracked.vstring()
)

################################################################
### Input files
################################################################
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.max) )

# Additional output definition
# TTree output file
process.load("CommonTools.UtilAlgos.TFileService_cfi")
process.TFileService.fileName = cms.string(options.output)

################################################################
### Analysis
################################################################
process.decayAnalyzer = cms.EDProducer('DecayAnalyzer',
    verbose       = cms.untracked.bool(False),
    output_edm    = cms.untracked.bool(False),
    output_ntuple = cms.untracked.bool(True)
)

# Output definition

#process.output = cms.OutputModule("PoolOutputModule",
  #fileName = cms.untracked.string('file:DecayAnalyzer_edm.root'),
  #outputCommands = cms.untracked.vstring(
    #'drop *', 
    #'keep *_simCaloStage2Digis_*_*',    
    #'keep *_simGmtStage2Digis_*_*',
    #'drop l1tCaloClusterBXVector_simCaloStage2Digis_*_*',
    #'drop l1tCaloTowerBXVector_simCaloStage2Digis_*_*',
    #'keep *_decayAnalyzer_*_*'
  #)
#)



process.l1EventTree = cms.EDAnalyzer("L1EventTreeProducer",
                                     hltSource            = cms.InputTag("TriggerResults::HLT"),
                                     puMCFile             = cms.untracked.string(""),
                                     puDataFile           = cms.untracked.string(""),
                                     puMCHist             = cms.untracked.string(""),
                                     puDataHist           = cms.untracked.string(""),
                                     
                                     useAvgVtx            = cms.untracked.bool(True),
                                     maxAllowedWeight     = cms.untracked.double(-1)                                     
)

# Stage-1 Ntuple will not contain muons, and might (investigating) miss some Taus.  (Work underway)
process.l1UpgradeTree = cms.EDAnalyzer("L1UpgradeTreeProducer")
if (eras.stage1L1Trigger.isChosen()):
    process.l1UpgradeTree.egToken      = cms.untracked.InputTag("simCaloStage1FinalDigis")
    process.l1UpgradeTree.tauTokens    = cms.untracked.VInputTag("simCaloStage1FinalDigis:rlxTaus","simCaloStage1FinalDigis:isoTaus")
    process.l1UpgradeTree.jetToken     = cms.untracked.InputTag("simCaloStage1FinalDigis")
    process.l1UpgradeTree.muonToken    = cms.untracked.InputTag("None")
    process.l1UpgradeTree.sumToken     = cms.untracked.InputTag("simCaloStage1FinalDigis","")
    process.l1UpgradeTree.maxL1Upgrade = cms.uint32(60)
if (eras.stage2L1Trigger.isChosen()):
    process.l1UpgradeTree.egToken      = cms.untracked.InputTag("simCaloStage2Digis")
    process.l1UpgradeTree.tauTokens    = cms.untracked.VInputTag("simCaloStage2Digis")
    process.l1UpgradeTree.jetToken     = cms.untracked.InputTag("simCaloStage2Digis")
    process.l1UpgradeTree.muonToken    = cms.untracked.InputTag("simGmtStage2Digis")
    process.l1UpgradeTree.sumToken     = cms.untracked.InputTag("simCaloStage2Digis","")
    process.l1UpgradeTree.maxL1Upgrade = cms.uint32(60)


################################################################
# L1 Objects
################################################################

import UserCode.ICHiggsTauTau.default_producers_cfi as producers

process.icL1ObjectSequence = cms.Sequence()

process.icL1ObjectProducer = producers.icL1ObjectProducer.clone(
  branch                    = cms.string("Candidates"),
  input                     = cms.InputTag("simCaloStage2Digis"),
  input_L1TEra       = cms.untracked.string  ("stage2"),
  inputTag_L1TEGamma = cms.untracked.InputTag("simCaloStage2Digis"),
  inputTag_L1TMuon   = cms.untracked.InputTag("simGmtStage2Digis"),
  inputTag_L1TTau    = cms.untracked.InputTag("simCaloStage2Digis"),
  inputTag_L1TJet    = cms.untracked.InputTag("simCaloStage2Digis"),
  inputTag_L1TSum    = cms.untracked.InputTag("simCaloStage2Digis",""),
  maxL1Upgrade = cms.uint32(1000)
)

process.icL1ObjectSequence += cms.Sequence(
  process.icL1ObjectProducer
)

################################################################
### Sequence
################################################################
process.analysis = cms.Path(
  process.RawToDigi*        
  process.L1TReEmulateFromRAW* 
  process.icL1ObjectSequence 
#  process.l1EventTree*
#  process.l1UpgradeTree*
#  process.decayAnalyzer
)
#process.output_step = cms.EndPath(process.output)

# Re-emulating, so don't unpack L1T output, might not even exist...
# Also, remove uneeded unpackers for speed.
if (eras.stage2L1Trigger.isChosen()):
    process.analysis.remove(process.gmtStage2Digis)
    process.analysis.remove(process.caloStage2Digis)
    process.analysis.remove(process.gtStage2Digis)
    process.analysis.remove(process.siPixelDigis)
    process.analysis.remove(process.siStripDigis)
    process.analysis.remove(process.castorDigis)
    process.analysis.remove(process.scalersRawToDigi)
    process.analysis.remove(process.tcdsDigis)
if (eras.stage1L1Trigger.isChosen()):
    process.analysis.remove(process.caloStage1Digis)
    process.analysis.remove(process.caloStage1FinalDigis)
    process.analysis.remove(process.gtDigis)
    process.analysis.remove(process.siPixelDigis)
    process.analysis.remove(process.siStripDigis)
    process.analysis.remove(process.castorDigis)
    process.analysis.remove(process.scalersRawToDigi)
    process.analysis.remove(process.tcdsDigis)







# enable debug message logging for our modules
#process.MessageLogger.categories.append('L1TCaloEvents')
#process.MessageLogger.categories.append('L1TGlobalEvents')
#process.MessageLogger.categories.append('l1t|Global')
#process.MessageLogger.suppressInfo = cms.untracked.vstring('Geometry', 'AfterSource')



                               
#)

# Stage-1 Ntuple will not contain muons, and might (investigating) miss some Taus.  (Work underway)
#process.l1UpgradeTree = cms.EDAnalyzer("L1UpgradeTreeProducer")
#if (eras.stage1L1Trigger.isChosen()):
    #process.l1UpgradeTree.egToken      = cms.untracked.InputTag ("simCaloStage1FinalDigis")
    #process.l1UpgradeTree.tauTokens    = cms.untracked.VInputTag("simCaloStage1FinalDigis:rlxTaus","simCaloStage1FinalDigis:isoTaus")
    #process.l1UpgradeTree.jetToken     = cms.untracked.InputTag ("simCaloStage1FinalDigis")
    #process.l1UpgradeTree.muonToken    = cms.untracked.InputTag ("None")
    #process.l1UpgradeTree.sumToken     = cms.untracked.InputTag ("simCaloStage1FinalDigis","")
    #process.l1UpgradeTree.maxL1Upgrade = cms.uint32(60)
#if (eras.stage2L1Trigger.isChosen()):
    #process.l1UpgradeTree.egToken      = cms.untracked.InputTag ("simCaloStage2Digis")
    #process.l1UpgradeTree.tauTokens    = cms.untracked.VInputTag("simCaloStage2Digis")
    #process.l1UpgradeTree.jetToken     = cms.untracked.InputTag ("simCaloStage2Digis")
    #process.l1UpgradeTree.muonToken    = cms.untracked.InputTag ("simGmtStage2Digis")
    #process.l1UpgradeTree.sumToken     = cms.untracked.InputTag ("simCaloStage2Digis","")
    #process.l1UpgradeTree.maxL1Upgrade = cms.uint32(60)


