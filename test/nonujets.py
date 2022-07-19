import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
#opts.register('file', 'file:/vols/cms/dw515/minlo_hjj/CMSSW_9_2_8/src/gen.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'root://gfe02.grid.hep.ph.ic.ac.uk:1097/store/user/dwinterb/minlo_sm_lhe_v2/gen.root', parser.VarParsing.multiplicity.singleton,
#parser.VarParsing.varType.string, "input file")
opts.register('output', 'EventTree.root', parser.VarParsing.multiplicity.singleton,parser.VarParsing.varType.string, "output file")
opts.register('file', '/store/user/dwinterb/ggHjj_minlo_Jun09_SM/ggHjj_minlo_Jun09_SM/ggHjj_minlo_Jun09_SM/220609_110828/0000/output_128.root', parser.VarParsing.multiplicity.singleton,
parser.VarParsing.varType.string, "input file")

opts.register('globalTag', '106X_upgrade2018_realistic_v16_L1v1', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('LHEWeights', True, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "Produce LHE weights for sample")
opts.register('LHETag', 'externalLHEProducer', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Input tag for LHE weights")
opts.register('tauSpinner', True, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "Compute weights using tauspinner")

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

process = cms.Process('PAT',eras.Run2_2018)

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
  fileName = cms.string(opts.output),
  closeFileFast = cms.untracked.bool(True)
)

################################################################
# Message Logging, summary, and number of events
################################################################
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(-1)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 1000

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

process.icGenParticleProducerFromLHEParticles = producers.icGenParticleFromLHEParticlesProducer.clone(
   input   = cms.InputTag(opts.LHETag)
)

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
#if opts.LHETag: lheTag = opts.LHETag
#else: lheTag = 'externalLHEProducer'
lheTag = opts.LHETag

process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  includeLHEWeights   = cms.bool(doLHEWeights),
  lheProducer         = cms.InputTag(lheTag),
  lheSource           = cms.string(lheTag),
)

process.icEventInfoSequence = cms.Sequence(
  process.icEventInfoProducer
)
 

################################################################
# TauSpinner
################################################################

process.icTauSpinnerProducer = cms.EDProducer("ICTauSpinnerProducer",
    branch                  = cms.string("tauspinner"),
    input                   = cms.InputTag("genParticles"),
    theta                   = cms.string("0,0.25,0.5,-0.25,0.375")
)

if opts.tauSpinner:
    process.icTauSpinnerSequence = cms.Sequence(
        process.icTauSpinnerProducer
    )
else: process.icTauSpinnerSequence = cms.Sequence()
 
################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()


process.path = cms.Path(
    process.icVertexSequence+
    process.icGenSequence+
    process.icTauSpinnerSequence+
    process.icEventInfoSequence+
    process.icEventProducer
)


process.schedule = cms.Schedule(process.path)


