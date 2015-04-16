import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################                                                                                         
# Setup and Read Options                                                                                                                          
################################################################                                                                                           
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
opts.register('file', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "input file")
opts.register('globalTag', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('isData', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as data?")
opts.register('release', '', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")

opts.parseArguments()
infile      = opts.file
if not infile: infile = "file:/tmp/file.root"
tag         = opts.globalTag
isData      = opts.isData
release     = opts.release

if not release in ["72X", "72XMINIAOD"]:
  print 'Release not recognised, exiting!'
  sys.exit(1)
print 'release     : '+release
print 'isData      : '+str(isData)
print 'globalTag   : '+tag


################################################################                                                                                          
# Standard setup                                                                                                                                           
################################################################                                                                                          
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.TFileService = cms.Service("TFileService",
  fileName = cms.string("EventTree.root"),
  closeFileFast = cms.untracked.bool(True)
)

################################################################                                                                                        
# Message Logging, summary, and number of events                                                                                                          
################################################################                                                                                          
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(100)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 50

process.options   = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)

################################################################                                                                                       
# Input files and global tags                                                                                                                            
################################################################                                                                                         
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(infile))
process.GlobalTag.globaltag = cms.string(tag)

# 72X MC: file=root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/AODSIM/PU40bx25_PHYS14_25_V1-v1/00000/00E63918-3A70-E411-A246-7845C4FC35F3.root globalTag=START72_V1::All                                                                                                               
# 72XMINIAOD MC: file=root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/36224FE2-0571-E411-9664-00266CFAE30C.root globalTag=START72_V1::All                                                                                                    
import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors


################################################################                                                                                       
# Declare sequences                                                                                                                           
################################################################                                                                                         
process.ic72XSequence = cms.Sequence()
process.icMiniAODSequence = cms.Sequence()


################################################################                                                                                       
# Object selection                                                                                                                            
################################################################                                                                                         
process.selectedVertices = cms.EDFilter("VertexRefSelector",
  src = cms.InputTag("offlineSlimmedPrimaryVertices"),
  cut = cms.string("ndof >= 4 & abs(z) <= 24 & abs(position.Rho) <= 2")
)

process.selectedElectrons = cms.EDFilter("PATElectronRefSelector",
  src = cms.InputTag("slimmedElectrons"),
  cut = cms.string("pt > 5.0 & abs(eta) < 2.6")
)

process.selectedPFMuons = cms.EDFilter("PATMuonRefSelector",
  src = cms.InputTag("slimmedMuons"),
  cut = cms.string("pt > 3.0 & abs(eta) < 2.6")
)

process.selectedPFTaus = cms.EDFilter("PATTauRefSelector",
  src = cms.InputTag("slimmedTaus"),
  cut = cms.string('pt > 18.0 & abs(eta) < 2.6 & tauID("decayModeFinding") > 0.5')
)

process.icSelectionSequence = cms.Sequence(
  process.selectedVertices+
  process.selectedPFCandidates+
  process.selectedElectrons+
  process.selectedPFMuons+
  process.selectedPFTaus
)

#!!CHECKED UP TO HERE

