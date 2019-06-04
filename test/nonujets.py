import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
opts.register('file', 'root://xrootd.unl.edu//store/user/dwinterb/MG5MC_2016/GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2016-GEN/GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2016-GEN/180801_114835/0000/test_gen_295.root', parser.VarParsing.multiplicity.singleton,               

#opts.register('file', 'root://xrootd.unl.edu//store/user/dwinterb/MG5MC/GluGluToHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/GluGluToHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/180702_190927/0001/test_gen_1412.root', parser.VarParsing.multiplicity.singleton,

parser.VarParsing.varType.string, "input file")
opts.register('globalTag', '94X_mc2017_realistic_v14', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('LHEWeights', False, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "Produce LHE weights for sample")
opts.register('LHETag', 'externalLHEProducer', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Input tag for LHE weights")

opts.parseArguments()
infile      = opts.file
if not infile: infile = "file:/tmp/file.root"
tag         = opts.globalTag
doLHEWeights = opts.LHEWeights

print 'globalTag   : '+str(tag)

################################################################
# Standard setup
################################################################

from Configuration.StandardSequences.Eras import eras

process = cms.Process('PAT',eras.Run2_2017)

# import of standard configurations
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('PhysicsTools.PatAlgos.slimming.metFilterPaths_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.TFileService = cms.Service("TFileService",
  fileName = cms.string("EventTree.root"),
  closeFileFast = cms.untracked.bool(True)
)

################################################################
# Message Logging, summary, and number of events
################################################################
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(-1)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 50

process.options   = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step1 nevts:1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.MINIAODSIMoutput = cms.OutputModule("PoolOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(4),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('MINIAODSIM'),
        filterName = cms.untracked.string('')
    ),
    dropMetaData = cms.untracked.string('ALL'),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    fastCloning = cms.untracked.bool(False),
    fileName = cms.untracked.string('file:HIG-RunIISummer16MiniAODv2-00187.root'),
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_prunedGenParticles_*_*',
        'keep *_ak4GenJetsNoNu_*_*'
    ),
    overrideInputFileSplitLevels = cms.untracked.bool(True)
)

# Additional output definition

# Other statements
#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_TrancheIV_v8', '')

# Path and EndPath definitions
process.Flag_trackingFailureFilter = cms.Path(process.goodVertices+process.trackingFailureFilter)
process.Flag_goodVertices = cms.Path(process.primaryVertexFilter)
process.Flag_CSCTightHaloFilter = cms.Path(process.CSCTightHaloFilter)
process.Flag_trkPOGFilters = cms.Path(process.trkPOGFilters)
process.Flag_trkPOG_logErrorTooManyClusters = cms.Path(~process.logErrorTooManyClusters)
process.Flag_EcalDeadCellTriggerPrimitiveFilter = cms.Path(process.EcalDeadCellTriggerPrimitiveFilter)
process.Flag_ecalLaserCorrFilter = cms.Path(process.ecalLaserCorrFilter)
process.Flag_globalSuperTightHalo2016Filter = cms.Path(process.globalSuperTightHalo2016Filter)
process.Flag_eeBadScFilter = cms.Path(process.eeBadScFilter)
process.Flag_METFilters = cms.Path(process.metFilters)
process.Flag_chargedHadronTrackResolutionFilter = cms.Path(process.chargedHadronTrackResolutionFilter)
process.Flag_globalTightHalo2016Filter = cms.Path(process.globalTightHalo2016Filter)
process.Flag_CSCTightHaloTrkMuUnvetoFilter = cms.Path(process.CSCTightHaloTrkMuUnvetoFilter)
process.Flag_HBHENoiseIsoFilter = cms.Path(process.HBHENoiseFilterResultProducer+process.HBHENoiseIsoFilter)
process.Flag_hcalLaserEventFilter = cms.Path(process.hcalLaserEventFilter)
process.Flag_HBHENoiseFilter = cms.Path(process.HBHENoiseFilterResultProducer+process.HBHENoiseFilter)
process.Flag_trkPOG_toomanystripclus53X = cms.Path(~process.toomanystripclus53X)
process.Flag_EcalDeadCellBoundaryEnergyFilter = cms.Path(process.EcalDeadCellBoundaryEnergyFilter)
process.Flag_trkPOG_manystripclus53X = cms.Path(~process.manystripclus53X)
process.Flag_HcalStripHaloFilter = cms.Path(process.HcalStripHaloFilter)
process.Flag_muonBadTrackFilter = cms.Path(process.muonBadTrackFilter)
process.Flag_CSCTightHalo2015Filter = cms.Path(process.CSCTightHalo2015Filter)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.MINIAODSIMoutput_step = cms.EndPath(process.MINIAODSIMoutput)

################################################################
# Input files and global tags
################################################################
process.load("CondCore.CondDB.CondDB_cfi")
from CondCore.CondDB.CondDB_cfi import *

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(infile))
process.GlobalTag.globaltag = cms.string(tag)

process.options   = cms.untracked.PSet(
  FailPath=cms.untracked.vstring("FileReadError")
)

import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

process.load("PhysicsTools.PatAlgos.patSequences_cff")


process.icGenVertexProducer = producers.icGenVertexProducer.clone(
  input = cms.InputTag("genParticles")
)

process.icVertexSequence = cms.Sequence(
  process.icGenVertexProducer
)

################################################################
# Simulation only: GenParticles, GenJets, PileupInfo
################################################################
process.icGenSequence = cms.Sequence()
 
process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  input   = cms.InputTag("genParticles"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True),
  includeStatusFlags = cms.bool(True)
)

process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
  src = cms.InputTag("ak4GenJetsNoNu"),
  cut = cms.string("pt > 10.0")
)

process.icGenJetProducer = producers.icGenJetProducer.clone(
  branch  = cms.string("genJets"),
  input   = cms.InputTag("selectedGenJets"),
)

process.icGenParticleProducerFromLHEParticles = producers.icGenParticleFromLHEParticlesProducer.clone()

process.icGenSequence += (
  process.icGenParticleProducer+
  process.selectedGenJets+
  process.icGenJetProducer
)
process.icGenSequence += (
  process.icGenParticleProducerFromLHEParticles
)


#################################################################
## EventInfo
#################################################################
if opts.LHETag: lheTag = opts.LHETag
else: lheTag = 'externalLHEProducer'

process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  includeLHEWeights   = cms.bool(doLHEWeights),
  lheProducer         = cms.InputTag(lheTag),
)

process.icEventInfoSequence = cms.Sequence(
  process.icEventInfoProducer
)
  
################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()

process.p = cms.Path(
  process.icVertexSequence+
  process.icGenSequence+
  process.icEventInfoSequence+
  process.icEventProducer
)

process.schedule = cms.Schedule(process.p)


