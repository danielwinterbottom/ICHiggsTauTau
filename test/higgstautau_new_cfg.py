import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
opts.register('file', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "input file")
opts.register('globalTag', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
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
infile      = opts.file
tag         = opts.globalTag
isData      = opts.isData
release     = opts.release
isEmbedded  = opts.isEmbedded
isTandP     = opts.isTandP
isZStudy    = opts.isZStudy

if not release in ["42X", "53X", "70X", "70XMINIAOD"]:
  print 'Release not recognised, exiting!'
  sys.exit(1)
print 'release     : '+release
print 'isData      : '+str(isData)
print 'isEmbedded  : '+str(isEmbedded)
print 'isTandP     : '+str(isTandP)
print 'isZStudy    : '+str(isZStudy)

################################################################
# Standard setup
################################################################
process.load("FWCore.MessageLogger.MessageLogger_cfi")
if release in ['42X']:
  process.load("Configuration.StandardSequences.Geometry_cff")
if release in ['53X', "70X", "70XMINIAOD"]:
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
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(infile))
process.GlobalTag.globaltag = cms.string(tag)

# 42X Data: root://eoscms//eos/cms/store/user/agilbert/samples/TauPlusX-2011A-Run166512-42X.root, GR_R_42_V25::All
# 42X MC: root://eoscms//eos/cms/store/user/agilbert/samples/DYJetsToLL-Fall11-42X.root, START42_V17::All
# 53X Data: root://eoscms//eos/cms/store/user/agilbert/samples/TauPlusX-2012D.root, FT_53_V21_AN4::All
# 53X MC: root://eoscms//eos/cms/store/user/agilbert/samples/DYJetsToLL-Summer12-53X-Sample.root, START53_V22::All
# 70X MC: root://cms-xrd-global.cern.ch//store/mc/Spring14dr/GluGluToHToTauTau_M-125_13TeV-powheg-pythia6/AODSIM/PU_S14_POSTLS170_V6-v1/00000/008D295A-49ED-E311-9F6D-7845C4F92ECD.root, PLS170_V7AN1::All

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
if release in ['42X']:
  from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
  process.kt6PFJets = kt4PFJets.clone(
    rParam = cms.double(0.6),
    doAreaFastjet = cms.bool(True),
    doRhoFastjet = cms.bool(True)
  )
  process.load("RecoJets.JetProducers.ak5PFJets_cfi")
  process.ak5PFJets.doAreaFastjet = cms.bool(True)
  process.ic42XSequence += process.ak5PFJets
  process.ic42XSequence += process.kt6PFJets

################################################################
# Object Selection
################################################################
process.selectedVertices = cms.EDFilter("VertexRefSelector",
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("ndof >= 4 & abs(z) <= 24 & abs(position.Rho) <= 2")
)

process.selectedElectrons = cms.EDFilter("GsfElectronRefSelector",
  src = cms.InputTag("gsfElectrons"),
  cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
)
if release in ['70X']:
  process.selectedElectrons.src = cms.InputTag("gedGsfElectrons")

process.selectedPFMuons = cms.EDFilter("GenericPFCandidateSelector",
  src = cms.InputTag("particleFlow"),
  cut = cms.string("pt > 3.0 & abs(eta) < 2.6 & abs(pdgId) == 13")
)

process.selectedPFTaus = cms.EDFilter("PFTauRefSelector",
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
  branch  = cms.string("vertices"),
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

process.icHttElecIsoCheck = cms.EDProducer('ICHttElecIsoCheck',
    input         = cms.InputTag("gsfElectrons"),
    pfChargedAll  = cms.InputTag("pfAllChargedParticles")
)

process.icHttMuonOverlapCheck = cms.EDProducer('ICHttMuonOverlapCheck',
    input = cms.InputTag("gsfElectrons"),
    muons = cms.InputTag("muons")
)

if release in ['70X']:
  process.icHttElecIsoCheck.input = cms.InputTag("gedGsfElectrons")
  process.icHttMuonOverlapCheck.input = cms.InputTag("gedGsfElectrons")

process.load("EgammaAnalysis.ElectronTools.electronIdMVAProducer_cfi")
process.electronIdMVASequence = cms.Sequence(
  process.mvaTrigV0+
  process.mvaNonTrigV0
)

from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso
if release in ['42X', '53X']:
  process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons')
if release in ['70X']:
  process.eleIsoSequence = setupPFElectronIso(process, 'gedGsfElectrons')
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
  branch                    = cms.string("electrons"),
  input                     = cms.InputTag("selectedElectrons"),
  includeConversionMatches  = cms.bool(True),
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
  branch                    = cms.string("muonsPFlow"),
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

# Parton flavour
# --------------
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

# Jet energy corrections
# ----------------------
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

pfJECS = cms.PSet(
  L1FastJet  = cms.string("ak5PFL1Fastjet"),
  L2Relative = cms.string("ak5PFL2Relative"),
  L3Absolute = cms.string("ak5PFL3Absolute")
)
if isData: pfJECS.append(
  L2L3Residual = cms.string("ak5PFResidual")
)

# b-tagging
# ---------
process.load("RecoJets.JetAssociationProducers.ak5JTA_cff")
from RecoJets.JetAssociationProducers.ak5JTA_cff import ak5JetTracksAssociatorAtVertex
process.load("RecoBTag.Configuration.RecoBTag_cff")
import RecoBTag.Configuration.RecoBTag_cff as btag
process.jetTracksAssociatorAtVertexAK5PF = ak5JetTracksAssociatorAtVertex.clone(
  jets = cms.InputTag("ak5PFJets")
)

if isEmbedded:
  process.jetTracksAssociatorAtVertexAK5PF.tracks = cms.InputTag("tmfTracks")

process.impactParameterTagInfosAK5PF = btag.impactParameterTagInfos.clone(
  jetTracks = cms.InputTag('jetTracksAssociatorAtVertexAK5PF')
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
  tagInfos = cms.VInputTag('impactParameterTagInfosAK5PF', 'secondaryVertexTagInfosAK5PF')
)

process.btaggingSequenceAK5PF = cms.Sequence(
  process.jetTracksAssociatorAtVertexAK5PF
  +process.impactParameterTagInfosAK5PF
  +process.secondaryVertexTagInfosAK5PF
  +process.simpleSecondaryVertexHighEffBJetTagsAK5PF
  +process.simpleSecondaryVertexHighPurBJetTagsAK5PF
  +process.combinedSecondaryVertexBJetTagsAK5PF
 )

# Pileup ID
# ---------
from RecoJets.JetProducers.PileupJetID_cfi import *
stdalgos = cms.VPSet()
if release in ['53X']:
  stdalgos = cms.VPSet(full_53x,cutbased,PhilV1)
if release in ['42X']:
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
      includeJetFlavour         = cms.bool(True),
      inputJetFlavour           = cms.InputTag("icPFJetFlavourCalculator"),
      applyJECs                 = cms.bool(True),
      includeJECs               = cms.bool(False),
      JECs                      = pfJECS,
      applyCutAfterJECs         = cms.bool(True),
      cutAfterJECs              = cms.string("pt > 15.0"),
      inputSVInfo               = cms.InputTag(""),
      requestSVInfo             = cms.bool(False),
      BTagDiscriminators        = cms.PSet(
        simpleSecondaryVertexHighEffBJetTags = cms.InputTag("simpleSecondaryVertexHighEffBJetTagsAK5PF"),
        simpleSecondaryVertexHighPurBJetTags = cms.InputTag("simpleSecondaryVertexHighPurBJetTagsAK5PF"),
        combinedSecondaryVertexBJetTags      = cms.InputTag("combinedSecondaryVertexBJetTagsAK5PF")
      )
    ),
    destConfig = cms.PSet(
      includePileupID       = cms.bool(True),
      inputPileupID         = cms.InputTag("puJetMva", "fullDiscriminant"),
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
# MVA MET and PF MET
################################################################
if release in ['53X', '70X']:
  process.load("ICAnalysis.MVAMETPairProducer.mvaPFMET_cff_leptons_53X_Dec2012")
else:
  process.load("ICAnalysis.MVAMETPairProducer.mvaPFMET_cff_leptons_42X_Dec2012")
  process.pfMEtMVA.srcLeptons = cms.VInputTag("isomuons", "isoelectrons", "isotaus")
  process.pfMEtMVA.useOld42  = cms.bool(False)

if isData:
  process.calibratedAK5PFJetsForPFMEtMVA.correctors = cms.vstring("ak5PFL1FastL2L3Residual")
else:
  process.calibratedAK5PFJetsForPFMEtMVA.correctors = cms.vstring("ak5PFL1FastL2L3")

process.mvaMetPairsMT = process.mvaMetPairs.clone(
  srcLeg1   = cms.InputTag('selectedPFMuons'),
  srcLeg2   = cms.InputTag('selectedPFTaus'),
  leg1Pt    = cms.double(7.0),
  leg1Eta   = cms.double(2.6),
  leg2Pt    = cms.double(18.0),
  leg2Eta   = cms.double(2.6),
  minDeltaR = cms.double(0.49)
)

process.mvaMetPairsET = process.mvaMetPairs.clone(
  srcLeg1   = cms.InputTag('selectedElectrons'),
  srcLeg2   = cms.InputTag('selectedPFTaus'),
  leg1Pt    = cms.double(10.0),
  leg1Eta   = cms.double(2.6),
  leg2Pt    = cms.double(18.0),
  leg2Eta   = cms.double(2.6),
  minDeltaR = cms.double(0.49)
)

process.mvaMetPairsEM = process.mvaMetPairs.clone(
  srcLeg1   = cms.InputTag('selectedElectrons'),
  srcLeg2   = cms.InputTag('selectedPFMuons'),
  leg1Pt    = cms.double(9.5),
  leg1Eta   = cms.double(2.6),
  leg2Pt    = cms.double(9.5),
  leg2Eta   = cms.double(2.6),
  minDeltaR = cms.double(0.29)
)

process.pfMEtMVAsequence += process.mvaMetPairsMT
process.pfMEtMVAsequence += process.mvaMetPairsET
process.pfMEtMVAsequence += process.mvaMetPairsEM

process.icMvaMetConcatenate = cms.EDProducer("ICPFMETConcatenate",
  concatenate = cms.VInputTag(
    cms.InputTag("mvaMetPairsMT"),
    cms.InputTag("mvaMetPairsET"),
    cms.InputTag("mvaMetPairsEM")
  )
)

process.icMvaMetIDConcatenate = cms.EDProducer("ICIDConcatenate",
  concatenate = cms.VInputTag(
    cms.InputTag("mvaMetPairsMT", "MVAMetId"),
    cms.InputTag("mvaMetPairsET", "MVAMetId"),
    cms.InputTag("mvaMetPairsEM", "MVAMetId")
  )
)

process.icMvaMetProducer = producers.icMetProducer.clone(
  branch  = cms.string("pfMVAMetVector"),
  input   = cms.InputTag("icMvaMetConcatenate"),
  includeCustomID = cms.bool(True),
  inputCustomID = cms.InputTag("icMvaMetIDConcatenate")
)

process.icPfMetProducer = producers.icSingleMetProducer.clone(
  branch  = cms.string("pfMet"),
  input   = cms.InputTag("pfMet")
)

process.icMvaMetSequence = cms.Sequence(
  process.pfMEtMVAsequence+
  process.icMvaMetConcatenate+
  process.icMvaMetIDConcatenate+
  process.icMvaMetProducer+
  process.icPfMetProducer
)

################################################################
# Simulation only: GenParticles, GenJets, PileupInfo
################################################################
process.icGenSequence = cms.Sequence()

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.prunedGenParticles = cms.EDProducer("ICGenParticlePruner",
  src = cms.InputTag("genParticles", "", "SIM"),
  select = cms.vstring(
    "drop  *",
    "keep status == 3 || status == 22 || status == 23",  # all status 3
    "keep abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15",  # all charged leptons
    "keep abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16",  # all neutrinos
    # "keep++ abs(pdgId) == 15",  # keep full tau decay chain
    "keep (4 <= abs(pdgId) <= 5)", # keep heavy flavour quarks
    "keep (400 <= abs(pdgId) < 600) || (4000 <= abs(pdgId) < 6000)", # keep b and c hadrons
    "keep abs(pdgId) = 10411 || abs(pdgId) = 10421 || abs(pdgId) = 10413 || abs(pdgId) = 10423 || abs(pdgId) = 20413 || abs(pdgId) = 20423 || abs(pdgId) = 10431 || abs(pdgId) = 10433 || abs(pdgId) = 20433", # additional c hadrons for jet fragmentation studies
    "keep abs(pdgId) = 10511 || abs(pdgId) = 10521 || abs(pdgId) = 10513 || abs(pdgId) = 10523 || abs(pdgId) = 20513 || abs(pdgId) = 20523 || abs(pdgId) = 10531 || abs(pdgId) = 10533 || abs(pdgId) = 20533 || abs(pdgId) = 10541 || abs(pdgId) = 10543 || abs(pdgId) = 20543" # additional b hadrons for jet fragmentation studies
  )
)
if release in ['42X']:
  process.prunedGenParticles.src = cms.InputTag("genParticles","","HLT")

process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  input   = cms.InputTag("prunedGenParticles"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True)
)

process.tauGenParticles = cms.EDProducer("ICGenParticlePruner",
  src = cms.InputTag("genParticles", "", "SIM"),
  select = cms.vstring(
    "drop  *",
    "keep++ abs(pdgId) == 15"  # keep full tau decay chain
  )
)
if release in ['42X']:
  process.tauGenParticles.src = cms.InputTag("genParticles","","HLT")

process.icTauGenParticleProducer = producers.icGenParticleProducer.clone(
  branch  = cms.string("genParticlesTaus"),
  input   = cms.InputTag("tauGenParticles"),
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
    process.tauGenParticles+
    process.icTauGenParticleProducer+
    process.genParticlesForJets+
    process.ak5GenJetsNoNuBSM+
    process.selectedGenJets+
    process.icGenJetProducer+
    process.icPileupInfoProducer
  )

################################################################
# Embedding
################################################################
process.icEmbeddingSequence = cms.Sequence()

process.icEmbeddedGenParticleProducer = producers.icGenParticleProducer.clone(
  branch  = cms.string("genParticlesEmbedded"),
  input   = cms.InputTag("genParticles", "", "EmbeddedRECO"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True)
)

if isEmbedded:
  process.icEmbeddingSequence += process.icEmbeddedGenParticleProducer

################################################################
# Trigger
################################################################
process.icTriggerSequence = cms.Sequence()

from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger(process, sequence = 'icTriggerSequence', outputModule = '')

process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
  input   = cms.InputTag("patTriggerEvent"),
  includeAcceptedOnly = cms.bool(True),
  saveStrings = cms.bool(True),
  splitVersion = cms.bool(False)
)

if isData:
  process.icTriggerSequence += (
    process.icTriggerPathProducer
  )

notTp = not isTandP
# Define 2011 et,mt,em physics triggers for data
if (release == '42X' and isData):
  process.icEle15LooseTau15ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle15LooseTau15"),
      hltPath = cms.string("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icEle15TightTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle15TightTau20"),
      hltPath = cms.string("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icEle15LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle15LooseTau20"),
      hltPath = cms.string("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icEle18MediumTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle18MediumTau20"),
      hltPath = cms.string("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icEle20MediumTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle20MediumTau20"),
      hltPath = cms.string("HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icIsoMu12LooseTau10ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu12LooseTau10"),
      hltPath = cms.string("HLT_IsoMu12_LooseIsoPFTau10_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icIsoMu15LooseTau15ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu15LooseTau15"),
      hltPath = cms.string("HLT_IsoMu15_LooseIsoPFTau15_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icIsoMu15LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu15LooseTau20"),
      hltPath = cms.string("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu8Ele17ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu8Ele17"),
      hltPath = cms.string("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu17Ele8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17Ele8"),
      hltPath = cms.string("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu8Ele17IdLObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu8Ele17IdL"),
      hltPath = cms.string("HLT_Mu8_Ele17_CaloIdL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu17Ele8IdLObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17Ele8IdL"),
      hltPath = cms.string("HLT_Mu17_Ele8_CaloIdL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icTriggerSequence += (
    process.icEle15LooseTau15ObjectProducer
    +process.icEle15TightTau20ObjectProducer
    +process.icEle15LooseTau20ObjectProducer
    +process.icEle18MediumTau20ObjectProducer
    +process.icEle20MediumTau20ObjectProducer
    +process.icIsoMu12LooseTau10ObjectProducer
    +process.icIsoMu15LooseTau15ObjectProducer
    +process.icIsoMu15LooseTau20ObjectProducer
    +process.icMu8Ele17IdLObjectProducer
    +process.icMu17Ele8IdLObjectProducer
    +process.icMu8Ele17ObjectProducer
    +process.icMu17Ele8ObjectProducer
    )

# Define 2011 et,mt,em physics triggers for mc
if (release == '42X' and not isData):
  process.icIsoMu15LooseTau15ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu15LooseTau15"),
      hltPath = cms.string("HLT_IsoMu15_LooseIsoPFTau15_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icEle18MediumTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle18MediumTau20"),
      hltPath = cms.string("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu8Ele17ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu8Ele17"),
      hltPath = cms.string("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu17Ele8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17Ele8"),
      hltPath = cms.string("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icTriggerSequence += (
    process.icIsoMu15LooseTau15ObjectProducer
    +process.icEle18MediumTau20ObjectProducer
    +process.icMu8Ele17ObjectProducer
    +process.icMu17Ele8ObjectProducer
    )

# Define 2012 et,mt,em physics triggers for data
if (release == '53X' and isData):
  process.icEle20RhoLooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle20RhoLooseTau20"),
      hltPath = cms.string("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icEle22WP90RhoLooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle22WP90RhoLooseTau20"),
      hltPath = cms.string("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icIsoMu18LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu18LooseTau20"),
      hltPath = cms.string("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icIsoMu17LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu17LooseTau20"),
      hltPath = cms.string("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu8Ele17ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu8Ele17"),
      hltPath = cms.string("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu17Ele8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17Ele8"),
      hltPath = cms.string("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icTriggerSequence += (
    process.icEle20RhoLooseTau20ObjectProducer
    +process.icEle22WP90RhoLooseTau20ObjectProducer
    +process.icIsoMu18LooseTau20ObjectProducer
    +process.icIsoMu17LooseTau20ObjectProducer
    +process.icMu8Ele17ObjectProducer
    +process.icMu17Ele8ObjectProducer
    )

# Define 2012 et,mt,em physics triggers for mc
if (release == '53X' and not isData):
  process.icEle22WP90RhoLooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle22WP90RhoLooseTau20"),
      hltPath = cms.string("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icIsoMu17LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu17LooseTau20"),
      hltPath = cms.string("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu8Ele17ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu8Ele17"),
      hltPath = cms.string("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu17Ele8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17Ele8"),
      hltPath = cms.string("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu8"),
      hltPath = cms.string("HLT_Mu8_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icEle8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle8"),
      hltPath = cms.string("HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icIsoMu24ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu24"),
      hltPath = cms.string("HLT_IsoMu24_eta2p1_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icTriggerSequence += (
    process.icEle22WP90RhoLooseTau20ObjectProducer
    +process.icIsoMu17LooseTau20ObjectProducer
    +process.icMu8Ele17ObjectProducer
    +process.icMu17Ele8ObjectProducer
    +process.icMu8ObjectProducer
    +process.icEle8ObjectProducer
    +process.icIsoMu24ObjectProducer
    )

# Define 2011 t+p triggers for data
if (release == '42X' and isData and isTandP):
  process.icIsoMu24ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu24"),
      hltPath = cms.string("HLT_IsoMu24_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icEle17Ele8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle17Ele8"),
      hltPath = cms.string("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass30_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icEle17SC8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle17SC8"),
      hltPath = cms.string("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icTriggerSequence += (
    process.icIsoMu24ObjectProducer
    +process.icEle17Ele8ObjectProducer
    +process.icEle17SC8ObjectProducer
    )

# Define 2012 t+p triggers for data
if (release == '53X' and isData and isTandP):
  process.icIsoMu24ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu24"),
      hltPath = cms.string("HLT_IsoMu24_eta2p1_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icMu17TkMu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17TkMu8"),
      hltPath = cms.string("HLT_Mu17_TkMu8_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icEle27ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle27"),
      hltPath = cms.string("HLT_Ele27_WP80_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icEle17Ele8Mass50ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle17Ele8Mass50"),
      hltPath = cms.string("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass50_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icEle20SC4Mass50ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle20SC4Mass50"),
      hltPath = cms.string("HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icTriggerSequence += (
    process.icIsoMu24ObjectProducer
    +process.icMu17TkMu8ObjectProducer
    +process.icEle27ObjectProducer
    +process.icEle17Ele8Mass50ObjectProducer
    +process.icEle20SC4Mass50ObjectProducer
    )

# Define 2012 mm,ee physics triggers for data
if (release == '53X' and isData and isZStudy):
  process.icEle17Ele8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle17Ele8"),
      hltPath = cms.string("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu17Mu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17Mu8"),
      hltPath = cms.string("HLT_Mu17_Mu8_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icTriggerSequence += (
    process.icEle17Ele8ObjectProducer
    +process.icMu17Mu8ObjectProducer
    )

# Define 2012 mm,ee physics triggers for mc
if (release == '53X' and (not isData) and isZStudy):
  process.icEle17Ele8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle17Ele8"),
      hltPath = cms.string("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu17Mu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17Mu8"),
      hltPath = cms.string("HLT_Mu17_Mu8_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icTriggerSequence += (
    process.icEle17Ele8ObjectProducer
    +process.icMu17Mu8ObjectProducer
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

if isEmbedded and release == '53X':
  process.icEventInfoProducer.genFilterWeights.append(
    embed_weight = cms.InputTag("generator", "minVisPtFilter", "EmbeddedRECO")
  )
if isEmbedded and release == '42X':
  process.icEventInfoProducer.weights.append(
    embed_weight = cms.InputTag("generator", "weight", "EmbeddedRECO")
)

process.icEventInfoSequence = cms.Sequence(
  process.icEventInfoProducer
)


################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()


process.p = cms.Path(
  process.ic42XSequence+
  process.recoTauClassicHPSSequence+
  process.icSelectionSequence+
  process.pfParticleSelectionSequence+
  process.icVertexSequence+
  process.icElectronSequence+
  process.icMuonSequence+
  process.icTauProducer+
  process.icMvaMetSequence+
  process.icPFJetSequence+
  process.icGenSequence+
  process.icTriggerSequence+
  process.icEmbeddingSequence+
  process.icEventInfoSequence+
  process.icEventProducer
)

#print process.dumpPython()
