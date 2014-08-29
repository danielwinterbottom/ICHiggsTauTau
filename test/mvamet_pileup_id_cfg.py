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
opts.register('file', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "input file")
opts.register('release', '', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")
opts.parseArguments()
infile      = opts.file
isData      = opts.isData
release     = opts.release
if not(release == '42X' or release == '53X'):
    print 'Release not recognised, exiting!'
    sys.exit(1)
else:
    print 'Setting up for ' + release + ' release...'

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

process.TFileService = cms.Service("TFileService",
  fileName = cms.string("EventTree.root"),
  closeFileFast = cms.untracked.bool(True)
)

################################################################
# Message Logging, summary, and number of events
################################################################
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(500)
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
    fileNames = cms.untracked.vstring(infile if infile else 'root://eoscms//eos/cms/store/user/agilbert/samples/TauPlusX-2011A-Run166512-42X.root')
  )
  process.GlobalTag.globaltag = cms.string('GR_R_42_V25::All')

if (release == '42X' and not isData):
  process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(infile if infile else 'root://eoscms//eos/cms/store/user/agilbert/samples/DYJetsToLL-Fall11-42X.root')
  )
  process.GlobalTag.globaltag = cms.string('START42_V17::All')

if (release == '53X' and isData):
  process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(infile if infile else 'root://eoscms//eos/cms/store/user/agilbert/samples/TauPlusX-2012D.root')
  )
  process.GlobalTag.globaltag = cms.string('FT_53_V21_AN4::All')

if (release == '53X' and not isData):
  process.source = cms.Source("PoolSource",
     fileNames = cms.untracked.vstring(infile if infile else 'root://eoscms//eos/cms/store/user/agilbert/samples/DYJetsToLL-Summer12-53X-Sample.root')
  )
  process.GlobalTag.globaltag = cms.string('START53_V22::All')


import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

################################################################
# Jets
################################################################

# Pileup ID
# ---------
from RecoJets.JetProducers.PileupJetID_cfi import *
stdalgos = cms.VPSet()
if release == '53X':
  stdalgos = cms.VPSet(full_53x,cutbased,PhilV1)
else:
  stdalgos = cms.VPSet(full,cutbased,PhilV1)

process.puJetMva = cms.EDProducer('PileupJetIdProducer',
    produceJetIds = cms.bool(True),
    jetids = cms.InputTag(""),
    runMvas = cms.bool(True),
    jets = cms.InputTag("ak5PFJets"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    algos = cms.VPSet(stdalgos),
    rho     = cms.InputTag("kt6PFJets", "rho"),
    jec     = cms.string("AK5PF"),
    applyJec = cms.bool(True),
    inputIsCorrected = cms.bool(False),
    residualsFromTxt = cms.bool(False),
    residualsTxt     = cms.FileInPath("RecoJets/JetProducers/data/dummy.txt"),
)

# Producer
# --------
process.icPFJetProducer = producers.icPFJetProducer.clone(
    branch                    = cms.string("pfJetsPFlow"),
    input                     = cms.InputTag("ak5PFJets"),
    srcConfig = cms.PSet(
      includeJetFlavour         = cms.bool(False),
      inputJetFlavour           = cms.InputTag(""),
      applyJECs                 = cms.bool(False),
      includeJECs               = cms.bool(False),
      JECs                      = cms.PSet(),
      applyCutAfterJECs         = cms.bool(False),
      cutAfterJECs              = cms.string(""),
      inputSVInfo               = cms.InputTag(""),
      requestSVInfo             = cms.bool(False),
      BTagDiscriminators        = cms.PSet()
    ),
    destConfig = cms.PSet(
      includePileupID       = cms.bool(True),
      inputPileupID         = cms.InputTag("puJetMva", "fullDiscriminant"),
      includeTrackBasedVars = cms.bool(False),
      inputTracks           = cms.InputTag("generalTracks"),
      inputVertices         = cms.InputTag("offlinePrimaryVertices"),
      requestTracks         = cms.bool(False)
    )
)

process.icPFJetSequence = cms.Sequence(
  process.puJetMva+
  process.icPFJetProducer
)

################################################################
# MVA MET and PF MET
################################################################
if (release == '53X'):
  # try and load the whole thing, won't work without updated tau tags
  #process.load("ICAnalysis.MVAMETPairProducer.mvaPFMET_cff_leptons_53X_Dec2012")
  # just import the parts we need for our MVAMETPairProducer
  from ICAnalysis.MVAMETPairProducer.mvaPFMET_cff_leptons_53X_Dec2012 import mvaMetPairs
  from JetMETCorrections.METPUSubtraction.mvaPFMET_leptons_cff import calibratedAK5PFJetsForPFMEtMVA
else:
  process.load("JetMETCorrections.METPUSubtraction.mvaPFMET_leptons_42X_cff")
  from ICAnalysis.MVAMETPairProducer.mvaPFMET_cff_leptons_42X_Dec2012 import mvaMetPairs
  process.pfMEtMVA.srcLeptons = cms.VInputTag("isomuons","isoelectrons","isotaus")
  process.pfMEtMVA.useOld42  = cms.bool(False)

process.load("JetMETCorrections.Configuration.DefaultJEC_cff")
process.calibratedAK5PFJetsForPFMEtMVA = calibratedAK5PFJetsForPFMEtMVA.clone()

if isData:
  process.calibratedAK5PFJetsForPFMEtMVA.correctors = cms.vstring("ak5PFL1FastL2L3Residual")
else:
  process.calibratedAK5PFJetsForPFMEtMVA.correctors = cms.vstring("ak5PFL1FastL2L3")

process.mvaMetPairs = mvaMetPairs.clone(
  srcLeg1   = cms.InputTag('muons'),
  srcLeg2   = cms.InputTag('gsfElectrons'),
  leg1Pt    = cms.double(10.0),
  leg1Eta   = cms.double(2.6),
  leg2Pt    = cms.double(10.0),
  leg2Eta   = cms.double(2.6),
  minDeltaR = cms.double(0.49)
)

# cannot run standard MVA MET without updated tau tags
#process.pfMEtMVAsequence += process.mvaMetPairs

process.icMvaMetPairsProducer = producers.icMetProducer.clone(
  branch  = cms.string("pfMVAMetVector"),
  input   = cms.InputTag("mvaMetPairs"),
  includeCustomID = cms.bool(True),
  inputCustomID = cms.InputTag("mvaMetPairs", "MVAMetId")
)

process.icMvaMetDefaultProducer = producers.icMetProducer.clone(
  branch  = cms.string("pfMVAMet"),
  input   = cms.InputTag("pfMVAMet")
)

process.icMvaMetSequence = cms.Sequence(
  #process.pfMEtMVAsequence+
  process.calibratedAK5PFJetsForPFMEtMVA+
  process.mvaMetPairs+
  process.icMvaMetPairsProducer
  #process.icMvaMetDefaultProducer
)

################################################################
# EventInfo
################################################################
process.icEventInfoProducer = producers.icEventInfoProducer.clone()

process.icEventInfoSequence = cms.Sequence(
  process.icEventInfoProducer
)

################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()

process.p = cms.Path(
  process.icPFJetSequence+
  process.icMvaMetSequence+
  process.icEventInfoSequence+
  process.icEventProducer
)

# print process.dumpPython()
