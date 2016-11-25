import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
opts.register('isData', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as data?")
opts.register('isEmbedded', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as embedded?")
opts.register('release', '', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")
opts.register('isTandP', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Tag and probe ntuples?")
opts.register('isZStudy', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process for Z->ee or Z->mumu?")
opts.parseArguments()
isData      = opts.isData
release     = opts.release
isEmbedded  = opts.isEmbedded
isTandP     = opts.isTandP
isZStudy    = opts.isZStudy

if isData:
    print 'Setting up for data processing...'
else:
    print 'Setting up for MC processing...'

if isEmbedded:
    print 'Setting up for embedded processing...'

if not(release == '42X' or release == '53X'):
    print 'Release not recognised, exiting!'
    sys.exit(1)
else:
    print 'Setting up for ' + release + ' release...'

if isTandP:
    print 'Setting up for tag and probe ntuples...'

if isZStudy:
    print 'Setting up for Z->ee or Z->mumu processing...'

################################################################
# Standard setup
################################################################
process.load("FWCore.MessageLogger.MessageLogger_cfi")
if not (release == '53X'):
  process.load("Configuration.StandardSequences.Geometry_cff")
else:
  process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

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
if (release == '42X' and isData):
  process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('root://eoscms//eos/cms/store/user/agilbert/samples/TauPlusX-2011A-Run166512-42X.root')
  )
  process.GlobalTag.globaltag = cms.string('GR_R_42_V25::All')

if (release == '42X' and not isData):
  process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('root://eoscms//eos/cms/store/user/agilbert/samples/DYJetsToLL-Fall11-42X.root')
  )
  process.GlobalTag.globaltag = cms.string('START42_V17::All')

if (release == '53X' and isData):
  process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('root://eoscms//eos/cms/store/user/agilbert/samples/TauPlusX-2012D.root')
  )
  process.GlobalTag.globaltag = cms.string('FT_53_V21_AN4::All')

if (release == '53X' and not isData):
  process.source = cms.Source("PoolSource",
    # fileNames = cms.untracked.vstring('root://eoscms//eos/cms/store/user/agilbert/samples/DYJetsToLL-Summer12-53X-Sample.root')
    fileNames = cms.untracked.vstring('file:/Volumes/MediaRAID/samples/VBF_HToTauTau_M-125-53X.root')
  )
  process.GlobalTag.globaltag = cms.string('START53_V22::All')


import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

################################################################
# Re-do PFTau reconstruction
################################################################
process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")

################################################################
# Need to create kt6PFJets in 42X for L1FastJet correction
################################################################
process.ic42XSequence = cms.Sequence()
if (release == '42X'):
  from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
  process.kt6PFJets = kt4PFJets.clone(
    rParam = cms.double(0.6),
    doAreaFastjet = cms.bool(True),
    doRhoFastjet = cms.bool(True)
  )
  process.ic42XSequence += process.kt6PFJets

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule("PoolOutputModule",
  fileName = cms.untracked.string('patTuple.root'),
  SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
  outputCommands = cms.untracked.vstring('drop *', *patEventContent )
  )


process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.pfTools import *
from PhysicsTools.PatAlgos.tools.tauTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.trigTools import *
from PhysicsTools.PatAlgos.tools.metTools import *

switchToPFTauHPS(process)

switchJetCollection(process,
  cms.InputTag('ak5PFJets'),
  doJTA            = True,
  doBTagging       = True,
  jetCorrLabel     = ('AK5PF', ['L1FastJet','L2Relative', 'L3Absolute','L2L3Residual'] if isData else ['L1FastJet','L2Relative', 'L3Absolute']),
  doType1MET       = False,
  genJetCollection = cms.InputTag("ak5GenJets"),
  doJetID      = False,
  jetIdLabel   = "ak5"
  )

addPfMET(process, 'PF')

if isData:
  runOnData(process)

removeCleaning(process)



# ################################################################
# # Object Selection
# ################################################################
process.selectedVertices = selectors.selectedVertices.clone()

process.selectedPatElectrons.cut = cms.string("pt > 9.5 & abs(eta) < 2.6")

process.selectedPatMuons.cut = cms.string("pt > 3.0 & abs(eta) < 2.6")

process.selectedPatTaus.cut = cms.string("pt > 18.0 & abs(eta) < 2.6 & decayMode > -0.5")

process.selectedPatJets.cut = cms.string('pt > 15.')


process.icSelectionSequence = cms.Sequence(
  process.selectedVertices
)

# ################################################################
# # Electrons
# ################################################################
process.icElectronProducer = producers.icElectronProducer.clone(
  input                     = cms.InputTag("selectedPatElectrons"),
  includeConversionMatches  = cms.bool(False),
  inputConversionMatches    = cms.InputTag("icElectronConversionCalculator"),
  includeVertexIP           = cms.bool(True),
  inputVertices             = cms.InputTag("selectedVertices"),
  includeBeamspotIP         = cms.bool(True),
  inputBeamspot             = cms.InputTag("offlineBeamSpot"),
  includeFloats = cms.PSet(
  ),
  includePFIso04           = cms.bool(False)
)

process.icElectronSequence = cms.Sequence(
  process.icElectronProducer
)

# ################################################################
# # Muons
# ################################################################
process.icMuonProducer = producers.icMuonProducer.clone(
  branch                    = cms.string("muons"),
  input                     = cms.InputTag("selectedPatMuons"),
  isPF                      = cms.bool(False),
  includeVertexIP           = cms.bool(True),
  inputVertices             = cms.InputTag("selectedVertices"),
  includeBeamspotIP         = cms.bool(True),
  inputBeamspot             = cms.InputTag("offlineBeamSpot"),
  includeFloats = cms.PSet(
  ),
  includePFIso04           = cms.bool(False)
)

process.icMuonSequence = cms.Sequence(
  process.icMuonProducer
)

# ################################################################
# # Taus
# ################################################################
process.icTauProducer = cms.EDProducer("ICPFTauFromPatProducer",
  branch                  = cms.string("taus"),
  input                   = cms.InputTag("selectedPatTaus"),
  inputVertices           = cms.InputTag("selectedVertices"),
  includeVertexIP         = cms.bool(True),
  requestTracks           = cms.bool(False),
  tauIDs = cms.PSet(
  )
)

process.icTauSequence = cms.Sequence(
  process.icTauProducer
)

# ################################################################
# # Jets
# ################################################################
process.icPFJetProducer = cms.EDProducer("ICPFJetFromPatProducer",
    branch                    = cms.string("pfJetsPFlow"),
    input                     = cms.InputTag("selectedPatJets"),
    srcConfig = cms.PSet(
      includeJetFlavour         = cms.bool(True),
      includeJECs               = cms.bool(True),
      inputSVInfo               = cms.InputTag("secondaryVertexTagInfosAOD"),
      requestSVInfo             = cms.bool(True)
    ),
    destConfig = cms.PSet(
      includePileupID    = cms.bool(False),
      inputPileupID      = cms.InputTag("puJetMva", "fullDiscriminant"),
      includeTrackBasedVars = cms.bool(True),
      inputTracks           = cms.InputTag("generalTracks"),
      inputVertices         = cms.InputTag("offlinePrimaryVertices"),
      requestTracks         = cms.bool(False)
    )
)

process.icPFJetSequence = cms.Sequence(
  process.icPFJetProducer
)

# ################################################################
# # Trigger
# ################################################################
process.icTriggerSequence = cms.Sequence()

switchOnTrigger(process, sequence = 'icTriggerSequence', outputModule = '')

process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
  input   = cms.InputTag("patTriggerEvent"),
  includeAcceptedOnly = cms.bool(True),
  saveStrings = cms.bool(False),
  splitVersion = cms.bool(True)
)

process.icTriggerSequence += (
  process.icTriggerPathProducer
)

################################################################
# EventInfo
################################################################
process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  includeJetRho       = cms.bool(True),
  inputJetRho         = cms.InputTag("kt6PFJets", "rho"),
  includeLeptonRho    = cms.bool(False),
  inputLeptonRho      = cms.InputTag("kt6PFJets", "rho"),
  includeVertexCount  = cms.bool(True),
  inputVertices       = cms.InputTag("selectedVertices"),
  includeCSCFilter    = cms.bool(False),
  inputCSCFilter      = cms.InputTag("BeamHaloSummary"),
)

process.icEventInfoSequence = cms.Sequence(
  process.icEventInfoProducer
)

################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()


process.p = cms.Path(
  process.recoTauClassicHPSSequence+
  process.patDefaultSequence+
  process.icSelectionSequence+
  # process.pfParticleSelectionSequence+
  # process.icVertexSequence+
  process.icElectronSequence+
  process.icMuonSequence+
  process.icTauProducer+
  process.icPFJetSequence+
  # process.icGenSequence+
  process.icTriggerSequence+
  process.icEventInfoSequence+
  process.icEventProducer
)

# print process.dumpPython()