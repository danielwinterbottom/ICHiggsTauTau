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
    fileNames = cms.untracked.vstring('file:/Volumes/HDD/DYJetsToLL.root')
  )
  process.GlobalTag.globaltag = cms.string('START53_V22::All')


import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

################################################################
# Re-do PFTau reconstruction
################################################################
process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")
if (release == '42X'): #check this goes after?
  switchToPFTauHPS(process)

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



################################################################
# Object Selection
################################################################
process.selectedVertices = selectors.selectedVertices.clone()

process.selectedElectrons = selectors.selectedElectrons.clone(
  cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
)

process.selectedPFMuons = selectors.selectedPFMuons.clone(
  cut = cms.string("pt > 3.0 & abs(eta) < 2.6 & abs(pdgId) == 13")
)

process.selectedPFTaus = selectors.selectedPFTaus.clone(
  src = cms.InputTag("hpsPFTauProducer", "", "MAIN"),
  cut = cms.string("pt > 18.0 & abs(eta) < 2.6 & decayMode > -0.5")
)

process.icSelectionSequence = cms.Sequence(
  process.selectedVertices+
  process.selectedElectrons+
  process.selectedPFMuons+
  process.selectedPFTaus
)

################################################################
# PF sequence for lepton isolation
################################################################
process.load("CommonTools.ParticleFlow.pfParticleSelection_cff")

################################################################
# Vertices
################################################################
process.icVertexProducer = producers.icVertexProducer.clone(
  input = cms.InputTag("selectedVertices"),
  firstVertexOnly = cms.bool(True)
)

process.icVertexSequence = cms.Sequence(
  process.icVertexProducer
)

################################################################
# Electrons
################################################################
process.icElectronConversionCalculator = producers.icElectronConversionCalculator.clone()
process.icHttElecIsoCheck = producers.icHttElecIsoCheck.clone()
process.icHttMuonOverlapCheck = producers.icHttMuonOverlapCheck.clone()

process.load("EgammaAnalysis.ElectronTools.electronIdMVAProducer_cfi")
process.electronIdMVASequence = cms.Sequence(
  process.mvaTrigV0+
  process.mvaNonTrigV0
)

from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso
process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons')
process.eleIsoSequence.remove(process.elPFIsoValueCharged03NoPFIdPFIso)
process.eleIsoSequence.remove(process.elPFIsoValueChargedAll03NoPFIdPFIso)
process.eleIsoSequence.remove(process.elPFIsoValueGamma03NoPFIdPFIso)
process.eleIsoSequence.remove(process.elPFIsoValueNeutral03NoPFIdPFIso)
process.eleIsoSequence.remove(process.elPFIsoValuePU03NoPFIdPFIso)
process.eleIsoSequence.remove(process.elPFIsoValueCharged04NoPFIdPFIso)
process.eleIsoSequence.remove(process.elPFIsoValueChargedAll04NoPFIdPFIso)
process.eleIsoSequence.remove(process.elPFIsoValueGamma04NoPFIdPFIso)
process.eleIsoSequence.remove(process.elPFIsoValueNeutral04NoPFIdPFIso)
process.eleIsoSequence.remove(process.elPFIsoValuePU04NoPFIdPFIso)
process.elPFIsoValueGamma04PFIdPFIso.deposits[0].vetos      = cms.vstring('EcalEndcaps:ConeVeto(0.08)','EcalBarrel:ConeVeto(0.08)')
process.elPFIsoValueNeutral04PFIdPFIso.deposits[0].vetos    = cms.vstring()
process.elPFIsoValuePU04PFIdPFIso.deposits[0].vetos         = cms.vstring()
process.elPFIsoValueCharged04PFIdPFIso.deposits[0].vetos    = cms.vstring('EcalEndcaps:ConeVeto(0.015)')
process.elPFIsoValueChargedAll04PFIdPFIso.deposits[0].vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)','EcalBarrel:ConeVeto(0.01)')

process.icElectronProducer = producers.icElectronProducer.clone(
  input                     = cms.InputTag("selectedElectrons"),
  inputConversionMatches    = cms.InputTag("icElectronConversionCalculator"),
  includeVertexIP           = cms.bool(True),
  inputVertices             = cms.InputTag("selectedVertices"),
  includeBeamspotIP         = cms.bool(True),
  inputBeamspot             = cms.InputTag("offlineBeamSpot"),
  includeFloats = cms.PSet(
    mvaTrigV0       = cms.InputTag("mvaTrigV0"),
    mvaNonTrigV0    = cms.InputTag("mvaNonTrigV0"),
    trackInIsoSum   = cms.InputTag("icHttElecIsoCheck"),
    matchedRecoMuon = cms.InputTag("icHttMuonOverlapCheck")
  ),
  includePFIso04           = cms.bool(True)
)

process.icElectronSequence = cms.Sequence(
  process.icElectronConversionCalculator+
  process.icHttElecIsoCheck+
  process.icHttMuonOverlapCheck+
  process.electronIdMVASequence+
  process.eleIsoSequence+
  process.icElectronProducer
)

################################################################
# Muons
################################################################
from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFMuonIso
process.muIsoSequence = setupPFMuonIso(process, 'selectedPFMuons')

process.icMuonProducer = producers.icMuonProducer.clone(
  input                     = cms.InputTag("selectedPFMuons"),
  isPF                      = cms.bool(True),
  includeVertexIP           = cms.bool(True),
  inputVertices             = cms.InputTag("selectedVertices"),
  includeBeamspotIP         = cms.bool(True),
  inputBeamspot             = cms.InputTag("offlineBeamSpot"),
  includeFloats = cms.PSet(
  ),
  includePFIso04           = cms.bool(True)
)

process.icMuonSequence = cms.Sequence(
  process.muIsoSequence+
  process.icMuonProducer
)

################################################################
# Taus
################################################################
import UserCode.ICHiggsTauTau.tau_discriminators_cfi as tauIDs

process.icTauProducer = producers.icTauProducer.clone(
  input                   = cms.InputTag("selectedPFTaus"),
  inputVertices           = cms.InputTag("selectedVertices"),
  includeVertexIP         = cms.bool(True),
  requestTracks           = cms.bool(False),
  tauIDs = tauIDs.minimalHttIds
)

process.icTauSequence = cms.Sequence(
  process.icTauProducer
)

################################################################
# Jets
################################################################
import UserCode.ICHiggsTauTau.tau_discriminators_cfi as tauIDs

process.jetPartons = cms.EDProducer("PartonSelector",
    src = cms.InputTag("genParticles"),
    withLeptons = cms.bool(False)
)

process.pfJetPartonMatches = cms.EDProducer("JetPartonMatcher",
    jets = cms.InputTag("ak5PFJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("jetPartons")
)
process.pfJetFlavourAssociation = cms.EDProducer("JetFlavourIdentifier",
    srcByReference = cms.InputTag("pfJetPartonMatches"),
    physicsDefinition = cms.bool(False)
)

process.icPFJetFlavourCalculator = cms.EDProducer('ICJetFlavourCalculator',
    input       = cms.InputTag("ak5PFJets"),
    flavourMap  = cms.InputTag("pfJetFlavourAssociation")
)

process.ak5PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6PFJets", "rho"),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1FastJet')
)
process.ak5PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L2Relative')
)
process.ak5PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L3Absolute')
)
process.ak5PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L2L3Residual')
)

process.load("RecoJets.JetAssociationProducers.ak5JTA_cff")
from RecoJets.JetAssociationProducers.ak5JTA_cff import ak5JetTracksAssociatorAtVertex
process.load("RecoBTag.Configuration.RecoBTag_cff")
import RecoBTag.Configuration.RecoBTag_cff as btag
process.ak5JetTracksAssociatorAtVertexAK5PF = ak5JetTracksAssociatorAtVertex.clone(
  jets = cms.InputTag("ak5PFJets")
)
process.impactParameterTagInfosAK5PF = btag.impactParameterTagInfos.clone(
  jetTracks = cms.InputTag('ak5JetTracksAssociatorAtVertexAK5PF')
)
process.secondaryVertexTagInfosAK5PF = btag.secondaryVertexTagInfos.clone(
  trackIPTagInfos = cms.InputTag('impactParameterTagInfosAK5PF')
)
process.simpleSecondaryVertexHighEffBJetTagsAK5PF = btag.simpleSecondaryVertexHighEffBJetTags.clone (
  tagInfos = cms.VInputTag('secondaryVertexTagInfosAK5PF')
)
process.simpleSecondaryVertexHighPurBJetTagsAK5PF = btag.simpleSecondaryVertexHighPurBJetTags.clone (
  tagInfos = cms.VInputTag('secondaryVertexTagInfosAK5PF')
)
process.combinedSecondaryVertexBJetTagsAK5PF = btag.combinedSecondaryVertexBJetTags.clone (
  tagInfos = cms.VInputTag('impactParameterTagInfosAK5PF','secondaryVertexTagInfosAK5PF')
)

process.btaggingSequenceAK5PF = cms.Sequence(
  process.ak5JetTracksAssociatorAtVertexAK5PF
  +process.impactParameterTagInfosAK5PF
  +process.secondaryVertexTagInfosAK5PF
  +process.simpleSecondaryVertexHighEffBJetTagsAK5PF
  +process.simpleSecondaryVertexHighPurBJetTagsAK5PF
  +process.combinedSecondaryVertexBJetTagsAK5PF
 )

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

pfJECS = cms.PSet(
  L1FastJet  = cms.string("ak5PFL1Fastjet"),
  L2Relative = cms.string("ak5PFL2Relative"),
  L3Absolute = cms.string("ak5PFL3Absolute")
)
if isData: pfJECS.append(
  L2L3Residual = cms.string("ak5PFResidual")
)

process.icPFJetProducer = producers.icPFJetProducer.clone(
    input                     = cms.InputTag("ak5PFJets"),
    includeJetFlavour         = cms.bool(True),
    inputJetFlavour           = cms.InputTag("icPFJetFlavourCalculator"),
    applyJECs                 = cms.bool(True),
    includeJECs               = cms.bool(False),
    JECs                      = pfJECS,
    applyCutAfterJECs         = cms.bool(True),
    cutAfterJECs              = cms.string("pt > 15.0"),
    BTagDiscriminators        = cms.PSet(
      simpleSecondaryVertexHighEff = cms.InputTag("simpleSecondaryVertexHighEffBJetTagsAK5PF"),
      simpleSecondaryVertexHighPur = cms.InputTag("simpleSecondaryVertexHighPurBJetTagsAK5PF"),
      combinedSecondaryVertex      = cms.InputTag("combinedSecondaryVertexBJetTagsAK5PF")
    ),
    specificConfig = cms.PSet(
      includePileupID    = cms.bool(True),
      inputPileupID      = cms.InputTag("puJetMva", "fullDiscriminant"),
      includeTrackBasedVars = cms.bool(True),
      inputTracks           = cms.InputTag("generalTracks"),
      inputVertices         = cms.InputTag("offlinePrimaryVertices"),
      requestTracks         = cms.bool(False)
    )
)

process.icPFJetSequence = cms.Sequence(
  process.jetPartons+
  process.pfJetPartonMatches+
  process.pfJetFlavourAssociation+
  process.icPFJetFlavourCalculator+
  process.btaggingSequenceAK5PF+
  process.puJetMva+
  process.icPFJetProducer
)

################################################################
# Simulation only: GenParticles, GenJets, PileupInfo
################################################################
process.icGenSequence = cms.Sequence()

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.prunedGenParticles = cms.EDProducer(
    "ICGenParticlePruner",
    src = cms.InputTag("genParticles", "", "SIM"),
    select = cms.vstring(
      "drop  *",
      "keep status == 3 || status == 22 || status == 23",  # all status 3
      "keep abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15",  # all charged leptons
      "keep abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16",  # all neutrinos
      "keep++ abs(pdgId) == 15",  # keep full tau decay chain
      "keep (4 <= abs(pdgId) <= 5)", # keep heavy flavour quarks
      "keep (400 <= abs(pdgId) < 600) || (4000 <= abs(pdgId) < 6000)", # keep b and c hadrons
      "keep abs(pdgId) = 10411 || abs(pdgId) = 10421 || abs(pdgId) = 10413 || abs(pdgId) = 10423 || abs(pdgId) = 20413 || abs(pdgId) = 20423 || abs(pdgId) = 10431 || abs(pdgId) = 10433 || abs(pdgId) = 20433", # additional c hadrons for jet fragmentation studies
      "keep abs(pdgId) = 10511 || abs(pdgId) = 10521 || abs(pdgId) = 10513 || abs(pdgId) = 10523 || abs(pdgId) = 20513 || abs(pdgId) = 20523 || abs(pdgId) = 10531 || abs(pdgId) = 10533 || abs(pdgId) = 20533 || abs(pdgId) = 10541 || abs(pdgId) = 10543 || abs(pdgId) = 20543" # additional b hadrons for jet fragmentation studies
    )
)

process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  input   = cms.InputTag("prunedGenParticles"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True)
)

process.load("RecoJets.Configuration.GenJetParticles_cff")
process.genParticlesForJets.ignoreParticleIDs = cms.vuint32(
  1000022, 2000012, 2000014,
  2000016, 1000039, 5000039,
  4000012, 9900012, 9900014,
  9900016, 39, 12, 14, 16
)
process.load("RecoJets.JetProducers.ak5GenJets_cfi")
process.ak5GenJetsNoNuBSM  =  process.ak5GenJets.clone()

process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
  src = cms.InputTag("ak5GenJetsNoNuBSM"),
  cut = cms.string("pt > 15.0")
)

process.icGenJetProducer = producers.icGenJetProducer.clone(
  branch  = cms.string("genJets"),
  input   = cms.InputTag("selectedGenJets"),
  inputGenParticles = cms.InputTag("genParticles"),
  requestGenParticles = cms.bool(False)
)

process.icPileupInfoProducer = producers.icPileupInfoProducer.clone()

if not isData:
  process.icGenSequence += (
    process.prunedGenParticles+
    process.icGenParticleProducer+
    process.genParticlesForJets+
    process.ak5GenJetsNoNuBSM+
    process.selectedGenJets+
    process.icGenJetProducer+
    process.icPileupInfoProducer
  )

################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()


process.p = cms.Path(
  process.recoTauClassicHPSSequence+
  process.icSelectionSequence+
  process.pfParticleSelectionSequence+
  process.icVertexSequence+
  process.icElectronSequence+
  process.icMuonSequence+
  process.icTauProducer+
  process.icPFJetSequence+
  process.icGenSequence+
  process.icEventProducer
)

#print process.dumpPython()