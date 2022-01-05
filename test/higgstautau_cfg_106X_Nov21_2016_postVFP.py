import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
#root://xrootd.unl.edu/
opts.register('file', 'root://xrootd.unl.edu//store/mc/RunIISummer16MiniAODv3/VBFHToTauTau_M125_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_94X_mcRun2_asymptotic_v3-v2/40000/C026A575-142C-E911-84FD-14187741278B.root', parser.VarParsing.multiplicity.singleton,
parser.VarParsing.varType.string, "input file")
opts.register('globalTag', '94X_mcRun2_asymptotic_v3', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('isData', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as data?")
opts.register('doHT', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Store HT and number of outgoing partons?")
opts.register('isReHLT', 1, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as reHLT sample?")
opts.register('LHEWeights', False, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "Produce LHE weights for sample")
opts.register('includenpNLO', False, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "Store npNLO for sample (number of partons for NLO sample)")
opts.register('LHETag', 'externalLHEProducer', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Input tag for LHE weights")
opts.register('tauSpinner', False, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "Compute weights using tauspinner")
opts.register('isEmbed', False, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "dataset is embedded sample")
opts.register('includeHTXS', False, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "Compute HTXS Gen level vars")

opts.parseArguments()
infile      = opts.file
if not infile: infile = "file:/tmp/file.root"
isData      = opts.isData
tag         = opts.globalTag
doLHEWeights = opts.LHEWeights
includenpNLO = opts.includenpNLO
isEmbed = opts.isEmbed
if not isData:
  doHT     = opts.doHT
  isReHLT  = opts.isReHLT
else:
  doHT     = 0
  isReHLT  = 0

print 'isData      : '+str(isData)
print 'globalTag   : '+str(tag)
print 'doHT        : '+str(doHT)
print 'isReHLT     : '+str(isReHLT)

################################################################
# Standard setup
################################################################
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.TFileService = cms.Service("TFileService",
  fileName = cms.string("EventTree.root"),
  closeFileFast = cms.untracked.bool(True)
)

################################################################
# Message Logging, summary, and number of events
################################################################
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(1000)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 50

process.options   = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)


################################################################
# Input files and global tags
################################################################
process.load("CondCore.CondDB.CondDB_cfi")
from CondCore.CondDB.CondDB_cfi import *

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(

infile
))
process.GlobalTag.globaltag = cms.string(tag)

process.options   = cms.untracked.PSet(
  FailPath=cms.untracked.vstring("FileReadError")
)

import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

process.load("PhysicsTools.PatAlgos.patSequences_cff")
################################################################
# Re-do PFTau reconstruction
################################################################
process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")

process.icMiniAODSequence = cms.Sequence()

################################################################
# Object Selection
################################################################

from RecoTauTag.RecoTau.TauDiscriminatorTools import noPrediscriminants
process.load('RecoTauTag.Configuration.loadRecoTauTagMVAsFromPrepDB_cfi')
from RecoTauTag.RecoTau.PATTauDiscriminationByMVAIsolationRun2_cff import *
from RecoTauTag.RecoTau.PATTauDiscriminationAgainstElectronMVA6_cfi import *

updatedTauName = "slimmedTausNewID" #name of pat::Tau collection with new tau-Ids
import RecoTauTag.RecoTau.tools.runTauIdMVA as tauIdConfig
tauIdEmbedder = tauIdConfig.TauIDEmbedder(process, cms, debug = False,
                    updatedTauName = updatedTauName,
                    toKeep = ["2017v2", "newDM2017v2", "2016v1", "newDM2016v1",
                            "deepTau2017v2p1",
                            ])
tauIdEmbedder.runTauID()

process.selectedElectrons = cms.EDFilter("PATElectronRefSelector",
    src = cms.InputTag("slimmedElectrons"),
    cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
    )
process.selectedMuons = cms.EDFilter("PATMuonRefSelector",
    src = cms.InputTag("slimmedMuons"),
    cut = cms.string("pt > 3 & abs(eta) < 2.6")
    )
process.selectedTaus = cms.EDFilter("PATTauRefSelector",
    src = cms.InputTag("slimmedTausNewID"),
    cut = cms.string('pt > 18.0 & abs(eta) < 2.6 & tauID("decayModeFindingNewDMs") > 0.5')
    )


process.icSelectionSequence = cms.Sequence()


process.icSelectionSequence += cms.Sequence(
  process.selectedElectrons+
  process.selectedMuons+
  process.rerunMvaIsolationSequence+
  getattr(process,updatedTauName)+
  process.selectedTaus
)

################################################################
# PF sequence for lepton isolation
################################################################
process.load("CommonTools.ParticleFlow.pfParticleSelection_cff")

process.pfPileUp = cms.EDFilter("CandPtrSelector",
    src = cms.InputTag("packedPFCandidates"),
    cut = cms.string("fromPV <= 1")
    )
process.pfNoPileUp = cms.EDFilter("CandPtrSelector",
    src = cms.InputTag("packedPFCandidates"),
    cut = cms.string("fromPV > 1")
    )
process.pfAllNeutralHadrons = cms.EDFilter("CandPtrSelector",
    src = cms.InputTag("pfNoPileUp"),
    cut = cms.string("abs(pdgId) = 111 | abs(pdgId) = 130 | " \
        "abs(pdgId) = 310 | abs(pdgId) = 2112")
    )
process.pfAllChargedHadrons= cms.EDFilter("CandPtrSelector",
    src = cms.InputTag("pfNoPileUp"),
    cut = cms.string("abs(pdgId) = 211 | abs(pdgId) = 321 | " \
        "abs(pdgId) = 999211 | abs(pdgId) = 2212")
    )
process.pfAllPhotons= cms.EDFilter("CandPtrSelector",
    src = cms.InputTag("pfNoPileUp"),
    cut = cms.string("abs(pdgId) = 22")
    )
process.pfAllChargedParticles= cms.EDFilter("CandPtrSelector",
    src = cms.InputTag("pfNoPileUp"),
    cut = cms.string("abs(pdgId) = 211 | abs(pdgId) = 321 | " \
        "abs(pdgId) = 999211 | abs(pdgId) = 2212 | " \
        "abs(pdgId) = 11 | abs(pdgId) = 13")
    )
process.pfPileUpAllChargedParticles= cms.EDFilter("CandPtrSelector",
    src = cms.InputTag("pfPileUp"),
    cut = cms.string("abs(pdgId) = 211 | abs(pdgId) = 321 | " \
        "abs(pdgId) = 999211 | abs(pdgId) = 2212 | " \
        "abs(pdgId) = 11 | abs(pdgId) = 13")
    )
process.pfAllNeutralHadronsAndPhotons = cms.EDFilter("CandPtrSelector",
    src = cms.InputTag("pfNoPileUp"),
    cut = cms.string("abs(pdgId) = 111 | abs(pdgId) = 130 | " \
        "abs(pdgId) = 310 | abs(pdgId) = 2112 | abs(pdgId) = 22")
    )
process.pfParticleSelectionSequence = cms.Sequence(
    process.pfPileUp+
    process.pfNoPileUp+
    process.pfAllNeutralHadrons+
    process.pfAllChargedHadrons+
    process.pfAllPhotons+
    process.pfAllChargedParticles+
    process.pfPileUpAllChargedParticles+
    process.pfAllNeutralHadronsAndPhotons
    )

vtxLabel = cms.InputTag("offlineSlimmedPrimaryVertices")

################################################################
# Vertices
################################################################
process.icVertexProducer = producers.icVertexProducer.clone(
  branch  = cms.string("vertices"),
  input = vtxLabel,
  firstVertexOnly = cms.bool(True)
)

process.icGenVertexProducer = producers.icGenVertexProducer.clone(
  input = cms.InputTag("prunedGenParticles")
)


### refit PV excluding tau decay products tracks

import VertexRefit.TauRefit.AdvancedRefitVertexProducer_cfi as vertexrefit
import VertexRefit.TauRefit.MiniAODRefitVertexProducer_cfi as vertexfit

process.refitOfflineSlimmedPrimaryVerticesBSWithAllTracks = vertexfit.MiniAODRefitVertexBSProducer.clone()
process.icBSVertexProducer = producers.icVertexProducer.clone(
    branch  = cms.string("verticesBS"),
    input = cms.InputTag("refitOfflineSlimmedPrimaryVerticesBSWithAllTracks"),
    firstVertexOnly = cms.bool(True)
)

process.refitOfflineSlimmedPrimaryVertices = vertexrefit.AdvancedRefitVertexNoBSProducer.clone()
process.refitOfflineSlimmedPrimaryVertices.storeAsMap = cms.bool(True)
process.refitOfflineSlimmedPrimaryVertices.srcLeptons = cms.VInputTag("slimmedElectrons", "slimmedMuons", updatedTauName)

process.refitOfflineSlimmedPrimaryVerticesBS = vertexrefit.AdvancedRefitVertexBSProducer.clone()
process.refitOfflineSlimmedPrimaryVerticesBS.storeAsMap = cms.bool(True)
process.refitOfflineSlimmedPrimaryVerticesBS.srcLeptons = cms.VInputTag("slimmedElectrons", "slimmedMuons", updatedTauName)


process.icRefitVertexProducer = producers.icRefitVertexProducer.clone(
  branch  = cms.string("refittedVertices"),
  input = "refitOfflineSlimmedPrimaryVertices",
  firstVertexOnly = cms.bool(False)
)

process.icRefitVertexProducerBS = producers.icRefitVertexProducer.clone(
  branch  = cms.string("refittedVerticesBS"),
  input = "refitOfflineSlimmedPrimaryVerticesBS",
  firstVertexOnly = cms.bool(False)
)

process.icVertexSequence = cms.Sequence(
  process.icVertexProducer+
  process.icGenVertexProducer+
  process.refitOfflineSlimmedPrimaryVerticesBSWithAllTracks+
  process.icBSVertexProducer+
  process.refitOfflineSlimmedPrimaryVertices+
  process.refitOfflineSlimmedPrimaryVerticesBS+
  process.icRefitVertexProducer+
  process.icRefitVertexProducerBS
)

if isData or isEmbed:
  process.icVertexSequence.remove(process.icGenVertexProducer)

################################################################
# PFCandidates
################################################################
process.icPFFromPackedProducer = cms.EDProducer('ICPFFromPackedProducer',
    branch  = cms.string("pfCandidates"),
    input   = cms.InputTag("icTauProducer", "requestedPFCandidates"),
    requestTracks       = cms.bool(False),
    requestGsfTracks    = cms.bool(False),
    inputUnpackedTracks = cms.InputTag("unpackedTracksAndVertices")
    )

process.icPFSequence = cms.Sequence()
process.icPFSequence += process.icPFFromPackedProducer

################################################################
# Electrons
################################################################
electronLabel = cms.InputTag("slimmedElectrons")

process.icElectronSequence = cms.Sequence()

# electron smear and scale
from RecoEgamma.EgammaTools.EgammaPostRecoTools import setupEgammaPostRecoSeq
setupEgammaPostRecoSeq(process,
                       runEnergyCorrections=True,
                       runVID=False, #saves CPU time by not needlessly re-running VID, if you want the Fall17V2 IDs, set this to True or remove (default is True)
                       era='2016postVFP-UL')

process.icElectronSequence += cms.Sequence(
    process.egammaPostRecoSeq
    )


process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
    input       = electronLabel,
    beamspot    = cms.InputTag("offlineBeamSpot"),
    conversions = cms.InputTag("reducedEgamma:reducedConversions")
)


process.load("RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi")
process.electronMVAValueMapProducer.src = electronLabel
process.electronMVAValueMapProducer.srcMiniAOD = electronLabel

process.icElectronSequence+=cms.Sequence(
    process.electronMVAValueMapProducer
)


#Electron PF iso sequence:
process.load("CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff")
process.elPFIsoValueChargedAll03PFIdPFIso = process.elPFIsoValueChargedAll03PFId.clone()
process.elPFIsoDepositChargedAll.src  = electronLabel
process.elPFIsoValueCharged03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = electronLabel,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("charged_iso")
)
process.elPFIsoValueGamma03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = electronLabel,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("photon_iso")
)
process.elPFIsoValueNeutral03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = electronLabel,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("neutral_iso")
)
process.elPFIsoValuePU03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = electronLabel,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("pu_iso")
)

process.electronPFIsolationValuesSequence = cms.Sequence(
      process.elPFIsoValueCharged03PFIdPFIso+
      process.elPFIsoValueChargedAll03PFIdPFIso+
      process.elPFIsoValueGamma03PFIdPFIso+
      process.elPFIsoValueNeutral03PFIdPFIso+
      process.elPFIsoValuePU03PFIdPFIso
      )

process.elPFIsoDepositGamma.ExtractorPSet.ComponentName = cms.string("CandViewExtractor")
process.icElectronSequence += cms.Sequence(
    process.elPFIsoDepositChargedAll+
    process.electronPFIsolationValuesSequence
    )
process.elPFIsoValueChargedAll03PFIdPFIso.deposits[0].vetos = (
    cms.vstring('EcalEndcaps:ConeVeto(0.015)','EcalBarrel:ConeVeto(0.01)'))

process.elPFIsoValueCharged04PFIdPFIso = cms.EDProducer('ICElectronIsolation',
  input        = electronLabel,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("charged_iso")
)
process.elPFIsoValueGamma04PFIdPFIso = cms.EDProducer('ICElectronIsolation',
  input        = electronLabel,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("photon_iso")
)
process.elPFIsoValueNeutral04PFIdPFIso = cms.EDProducer('ICElectronIsolation',
  input        = electronLabel,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("neutral_iso")
)
process.elPFIsoValuePU04PFIdPFIso = cms.EDProducer('ICElectronIsolation',
  input        = electronLabel,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("pu_iso")
)

process.elEcalPFClusterIso = cms.EDProducer('ICElectronIsolation',
  input        = electronLabel,
  deltaR       = cms.double(0.3),
  iso_type     = cms.string("ecal_pf_cluster_iso")
)

process.elHcalPFClusterIso = cms.EDProducer('ICElectronIsolation',
  input        = electronLabel,
  deltaR       = cms.double(0.3),
  iso_type     = cms.string("hcal_pf_cluster_iso")
)


process.elPFIsoValueChargedAll04PFIdPFIso = process.elPFIsoValueChargedAll04PFId.clone()
process.elPFIsoValueChargedAll04PFIdPFIso.deposits[0].vetos = (
  cms.vstring('EcalEndcaps:ConeVeto(0.015)','EcalBarrel:ConeVeto(0.01)'))

process.electronPFIsolationValuesSequence +=cms.Sequence(
  process.elPFIsoValueCharged04PFIdPFIso+
  process.elPFIsoValueGamma04PFIdPFIso+
  process.elPFIsoValuePU04PFIdPFIso+
  process.elPFIsoValueNeutral04PFIdPFIso+
  process.elPFIsoValueChargedAll04PFIdPFIso+
  process.elEcalPFClusterIso+
  process.elHcalPFClusterIso
)


process.icElectronProducer = producers.icElectronProducer.clone(
  branch                    = cms.string("electrons"),
  input                     = cms.InputTag("selectedElectrons"),
  includeConversionMatches  = cms.bool(True),
  inputConversionMatches    = cms.InputTag("icElectronConversionCalculator"),
  includeVertexIP           = cms.bool(True),
  inputVertices             = vtxLabel,
  includeBeamspotIP         = cms.bool(True),
  inputBeamspot             = cms.InputTag("offlineBeamSpot"),
  doSmearAndScale           = cms.bool(True),
  includeFloats = cms.PSet(
     generalPurposeMVASpring16  = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring16GeneralPurposeV1Values"),
     mvaRun2Fall17  = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Fall17NoIsoV1Values"),
     mvaRun2IsoFall17  = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Fall17IsoV1Values"),
     mvaRun2Fall17V2  = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Fall17NoIsoV2RawValues"), # need raw values for v2
     mvaRun2IsoFall17V2  = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Fall17IsoV2RawValues"),
  ),
  includeClusterIso        = cms.bool(True),
  includePFIso03           = cms.bool(True),
  includePFIso04           = cms.bool(True)
)


process.icElectronSequence += cms.Sequence(
  process.icElectronConversionCalculator+
  process.icElectronProducer
)


################################################################
# Muons
################################################################
process.icMuonSequence = cms.Sequence()
muons = cms.InputTag("slimmedMuons")

process.load("CommonTools.ParticleFlow.Isolation.pfMuonIsolation_cff")
process.load("CommonTools.ParticleFlow.deltaBetaWeights_cff")
process.muPFIsoDepositChargedAll.src  = muons #All charged isolation is not stored in reco/pat
process.muPFIsoDepositNeutral.src     = muons
process.muPFIsoDepositNeutral.ExtractorPSet.inputCandView = cms.InputTag("pfWeightedNeutralHadrons")
process.muPFIsoDepositGamma.src       = muons
process.muPFIsoDepositGamma.ExtractorPSet.inputCandView = cms.InputTag("pfWeightedPhotons")

process.muPFIsoValueChargedAll03PFIso = process.muPFIsoValueChargedAll03.clone()
process.muPFIsoValueNeutral04PFWeights = process.muPFIsoValueNeutral04.clone()
process.muPFIsoValueGamma04PFWeights = process.muPFIsoValueGamma04.clone()
process.muPFIsoValueNeutral03PFWeights = process.muPFIsoValueNeutral03.clone()
process.muPFIsoValueGamma03PFWeights = process.muPFIsoValueGamma03.clone()
process.muPFIsoValueCharged03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("charged_iso")
)
process.muPFIsoValueGamma03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("photon_iso")
)
process.muPFIsoValueNeutral03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("neutral_iso")
)
process.muPFIsoValuePU03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("pu_iso")
)

process.muonPFIsolationValuesSequence = cms.Sequence(

   process.muPFIsoValueCharged03PFIso+
   process.muPFIsoValueChargedAll03PFIso+
   process.muPFIsoValueGamma03PFIso+
   process.muPFIsoValueNeutral03PFIso+
   process.muPFIsoValuePU03PFIso+
   process.muPFIsoValueNeutral04PFWeights+
   process.muPFIsoValueGamma04PFWeights+
   process.muPFIsoValueNeutral03PFWeights+
   process.muPFIsoValueGamma03PFWeights
   )


process.muPFIsoValueCharged04PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("charged_iso")
)
process.muPFIsoValueGamma04PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("photon_iso")
)
process.muPFIsoValueNeutral04PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("neutral_iso")
)
process.muPFIsoValuePU04PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("pu_iso")
)
process.muPFIsoValueChargedAll04PFIso = process.muPFIsoValueChargedAll04.clone()
process.muonPFIsolationValuesSequence +=cms.Sequence(
  process.muPFIsoValueCharged04PFIso+
  process.muPFIsoValueGamma04PFIso+
  process.muPFIsoValuePU04PFIso+
  process.muPFIsoValueNeutral04PFIso+
  process.muPFIsoValueChargedAll04PFIso
)

process.icMuonSequence += cms.Sequence(
    process.pfDeltaBetaWeightingSequence+
    process.muPFIsoDepositChargedAll+
    process.muPFIsoDepositNeutral+
    process.muPFIsoDepositGamma+
    process.muonPFIsolationValuesSequence
    )

process.icMuonProducer = producers.icMuonProducer.clone(
  branch                    = cms.string("muons"),
  input                     = cms.InputTag("selectedMuons"),
  isPF                      = cms.bool(False),
  includeVertexIP           = cms.bool(True),
  inputVertices             = vtxLabel,
  includeBeamspotIP         = cms.bool(True),
  inputBeamspot             = cms.InputTag("offlineBeamSpot"),
  includeDoubles = cms.PSet(
   neutral_pfweighted_iso_03 = cms.InputTag("muPFIsoValueNeutral03PFWeights"),
   neutral_pfweighted_iso_04 = cms.InputTag("muPFIsoValueNeutral04PFWeights"),
   gamma_pfweighted_iso_03 = cms.InputTag("muPFIsoValueGamma03PFWeights"),
   gamma_pfweighted_iso_04 = cms.InputTag("muPFIsoValueGamma04PFWeights")
  ),
  requestTracks           = cms.bool(True),
  includePFIso03           = cms.bool(True),
  includePFIso04           = cms.bool(True),
)
process.icMuonProducer.isPF = cms.bool(False)

process.icMuonSequence += cms.Sequence(
  process.icMuonProducer
)

################################################################
# Taus
################################################################
import UserCode.ICHiggsTauTau.tau_discriminators_cfi as tauIDs

process.icTauProducer = producers.icTauProducer.clone(
  input                   = cms.InputTag("selectedTaus"),
  inputVertices           = vtxLabel,
  includeVertexIP         = cms.bool(True),
  requestTracks           = cms.bool(True),
  tauIDs = tauIDs.dynamicStripIds,
  doSVRefit             = cms.bool(True)
)

process.icTauProducer = cms.EDProducer("ICPFTauFromPatProducer",
  branch                  = cms.string("taus"),
  input                   = cms.InputTag("selectedTaus"),
  inputVertices           = vtxLabel,
  includeVertexIP         = cms.bool(True),
  requestTracks           = cms.bool(False),
  includeTotalCharged     = cms.bool(False),
  totalChargedLabel       = cms.string('totalCharged'),
  requestPFCandidates     = cms.bool(True),
  inputPFCandidates       = cms.InputTag("packedPFCandidates"),
  isSlimmed               = cms.bool(True),
  tauIDs = cms.PSet(),
  doSVRefit             = cms.bool(True)
)


process.icTauSequence = cms.Sequence(
  process.icTauProducer
)

################################################################
# photons, clusters, and superclusters
################################################################

process.icPi0SuperClusterProducer = cms.EDProducer('ICPi0SuperClusterProducer',
  branch  = cms.string("superClusters"),
  input   = cms.InputTag("reducedEgamma", "reducedSuperClusters"),
  input_EERecHits = cms.InputTag("reducedEgamma", "reducedEERecHits"),
  input_EBRecHits = cms.InputTag("reducedEgamma", "reducedEBRecHits"),
  input_taus = cms.InputTag("slimmedTaus")
)

process.icPhotonProducer = producers.icPhotonProducer.clone(
    branch                  = cms.string("photons"),
    input                   = cms.InputTag("slimmedPhotons"),
    includeHadTowOverEm     = cms.bool(True),
    includePFIso03           = cms.bool(True),
    pfIso03 = cms.PSet(
      chargedAll  = cms.InputTag("phPFIsoValueChargedAll03PFIso"),
      charged     = cms.InputTag("phPFIsoValueCharged03PFIso"),
      neutral     = cms.InputTag("phPFIsoValueNeutral03PFIso"),
      gamma       = cms.InputTag("phPFIsoValueGamma03PFIso"),
      pu          = cms.InputTag("phPFIsoValuePU03PFIso")
    ),
    includeIsoFromPat = cms.bool(True)
)


process.icPhotonSequence = cms.Sequence(
  process.icPhotonProducer+
  process.icPi0SuperClusterProducer
)


# ################################################################
# # Jets
# ################################################################
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
  #rebuild ak4 chs jets as in  https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD#Jets
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')


#Reapply JECs:
if not (isData or isEmbed):
  updateJetCollection(
    process,
    jetSource = cms.InputTag("slimmedJets"),
    labelName = "UpdatedJEC",
    jetCorrections = ("AK4PFchs", cms.vstring(['L1FastJet','L2Relative','L3Absolute']), 'None'),
    pvSource = cms.InputTag('offlineSlimmedPrimaryVertices'),
    svSource = cms.InputTag('slimmedSecondaryVertices'),
    btagDiscriminators = [
      'pfDeepFlavourJetTags:probb',
      'pfDeepFlavourJetTags:probbb',
      'pfDeepFlavourJetTags:problepb',
      'pfDeepFlavourJetTags:probc',
      'pfDeepFlavourJetTags:probuds',
      'pfDeepFlavourJetTags:probg'
    ],
  )
else:
 updateJetCollection(
   process,
   jetSource = cms.InputTag("slimmedJets"),
   labelName = "UpdatedJEC",
   jetCorrections = ("AK4PFchs", cms.vstring(['L1FastJet','L2Relative','L3Absolute','L2L3Residual']), 'None'),
   pvSource = cms.InputTag('offlineSlimmedPrimaryVertices'),
   svSource = cms.InputTag('slimmedSecondaryVertices'),
   btagDiscriminators = [
     'pfDeepFlavourJetTags:probb',
     'pfDeepFlavourJetTags:probbb',
     'pfDeepFlavourJetTags:problepb',
     'pfDeepFlavourJetTags:probc',
     'pfDeepFlavourJetTags:probuds',
     'pfDeepFlavourJetTags:probg'
   ],
  )

process.selectedSlimmedJetsAK4 = cms.EDFilter("PATJetRefSelector",
    src = cms.InputTag("selectedUpdatedPatJetsUpdatedJEC"),
    cut = cms.string("pt > 15")
    )

 # ----------------------
 # Jet energy corrections
 # ----------------------
process.ak4PFL1FastjetCHS = cms.EDProducer("L1FastjetCorrectorProducer",
#    srcRho = cms.InputTag("kt6PFJets", "rho"),
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1FastJet')
)
process.ak4PFL2RelativeCHS = cms.EDProducer("LXXXCorrectorProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2Relative')
)
process.ak4PFL3AbsoluteCHS = cms.EDProducer("LXXXCorrectorProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L3Absolute')
)
process.ak4PFResidualCHS = cms.EDProducer("LXXXCorrectorProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2L3Residual')
)

#Corrections applied to miniaod slimmedJets
pfJECS = cms.PSet(
  L1FastJet  = cms.string("ak4PFL1FastjetCHS"),
  L2Relative = cms.string("ak4PFL2RelativeCHS"),
  L3Absolute = cms.string("ak4PFL3AbsoluteCHS")
)
if isData or isEmbed: pfJECS = cms.PSet(
  L1FastJet  = cms.string("ak4PFL1FastjetCHS"),
  L2Relative = cms.string("ak4PFL2RelativeCHS"),
  L3Absolute = cms.string("ak4PFL3AbsoluteCHS"),
  L2L3Residual = cms.string("ak4PFResidualCHS")
)


 # b-tagging
 # ---------
process.load("RecoJets.JetAssociationProducers.ak4JTA_cff")
from RecoJets.JetAssociationProducers.ak4JTA_cff import ak4JetTracksAssociatorAtVertex
process.load("RecoBTag.Configuration.RecoBTag_cff")
import RecoBTag.Configuration.RecoBTag_cff as btag

process.pfImpactParameterTagInfos.primaryVertex = cms.InputTag("offlineSlimmedPrimaryVertices")
process.pfImpactParameterTagInfos.candidates = cms.InputTag("packedPFCandidates")

 # Pileup ID
 # ---------
process.load('RecoJets.JetProducers.PileupJetID_cfi')

process.pileupJetIdCalculator.vertexes = cms.InputTag("offlineSlimmedPrimaryVertices")
process.pileupJetIdCalculator.jets = cms.InputTag("ak4PFJetsCHS")
process.pileupJetIdCalculator.rho = cms.InputTag("fixedGridRhoFastjetAll")
process.pileupJetIdEvaluator.jets = cms.InputTag("ak4PFJetsCHS")
process.pileupJetIdEvaluator.rho = cms.InputTag("fixedGridRhoFastjetAll")

 # JER
 # --------
process.load('Configuration.StandardSequences.Services_cff')
process.load("JetMETCorrections.Modules.JetResolutionESProducer_cfi")

process.slimmedJetsSmeared = cms.EDProducer('SmearedPATJetProducer',
        src = cms.InputTag('selectedUpdatedPatJetsUpdatedJEC'),
        enabled = cms.bool(True),
        rho = cms.InputTag("fixedGridRhoFastjetAll"),
        algo = cms.string('AK4PFchs'),
        algopt = cms.string('AK4PFchs_pt'),
        #resolutionFile = cms.FileInPath('Autumn18_V7_MC_PtResolution_AK4PFchs.txt'),
        #scaleFactorFile = cms.FileInPath('combined_SFs_uncertSources.txt'),

        genJets = cms.InputTag('slimmedGenJets'),
        dRMax = cms.double(0.2),
        dPtMaxFactor = cms.double(3),

        debug = cms.untracked.bool(False),
        # Systematic variation
        # 0: Nominal
        # -1: -1 sigma (down variation)
        # 1: +1 sigma (up variation)
        variation = cms.int32(0),  # If not specified, default to 0
)

process.slimmedJetsSmearedDown = process.slimmedJetsSmeared.clone(variation=cms.int32(-1))
process.slimmedJetsSmearedUp = process.slimmedJetsSmeared.clone(variation=cms.int32(1))


# process.icPFJetProducerFromPat = producers.icPFJetFromPatProducer.clone(
#     branch                    = cms.string("ak4PFJetsCHS"),
#     input                     = cms.InputTag("selectedSlimmedJetsAK4"),
#     srcConfig = cms.PSet(
#       isSlimmed               = cms.bool(True),
#       slimmedPileupIDLabel    = cms.string('pileupJetId:fullDiscriminant'),
#       includeJetFlavour       = cms.bool(True),
#       includeJECs             = cms.bool(True),
#       inputSVInfo             = cms.InputTag(""),
#       requestSVInfo           = cms.bool(False)
#     ),
#    destConfig = cms.PSet(
#      includePileupID         = cms.bool(True),
#      inputPileupID           = cms.InputTag("puJetMva", "fullDiscriminant"),
#      includeTrackBasedVars   = cms.bool(False),
#      inputTracks             = cms.InputTag("unpackedTracksAndVertices"),
#      inputVertices           = cms.InputTag("unpackedTracksAndVertices"),
#      requestTracks           = cms.bool(False)
#     )
# )

process.icPFJetProducerFromPatNew = producers.icPFJetFromPatNewProducer.clone(
    branch                    = cms.string("ak4PFJetsCHS"),
    input                     = cms.InputTag("selectedSlimmedJetsAK4"),
    inputSmear                = cms.InputTag("slimmedJetsSmeared"),
    inputSmearDown            = cms.InputTag("slimmedJetsSmearedDown"),
    inputSmearUp              = cms.InputTag("slimmedJetsSmearedUp"),
    srcConfig = cms.PSet(
      isSlimmed               = cms.bool(True),
      slimmedPileupIDLabel    = cms.string('pileupJetId:fullDiscriminant'),
      includeJetFlavour       = cms.bool(True),
      includeJECs             = cms.bool(True),
      inputSVInfo             = cms.InputTag(""),
      requestSVInfo           = cms.bool(False)
    ),
   destConfig = cms.PSet(
     includePileupID         = cms.bool(True),
     inputPileupID           = cms.InputTag("puJetMva", "fullDiscriminant"),
     includeTrackBasedVars   = cms.bool(False),
     inputTracks             = cms.InputTag("unpackedTracksAndVertices"),
     inputVertices           = cms.InputTag("unpackedTracksAndVertices"),
     requestTracks           = cms.bool(False)
    )
)
if isData or isEmbed:
    process.icPFJetProducerFromPatNew.doSmear = cms.bool(False)
else:
    process.icPFJetProducerFromPatNew.doSmear = cms.bool(True)

process.icPFJetSequence = cms.Sequence()

process.icPFJetSequence += cms.Sequence(
    process.patJetCorrFactorsUpdatedJEC+
    process.updatedPatJetsUpdatedJEC+
    process.pfImpactParameterTagInfosUpdatedJEC+
    process.pfInclusiveSecondaryVertexFinderTagInfosUpdatedJEC+
    process.pfDeepCSVTagInfosUpdatedJEC+
    process.pfDeepFlavourTagInfosUpdatedJEC+
    process.pfDeepFlavourJetTagsUpdatedJEC+
    process.patJetCorrFactorsTransientCorrectedUpdatedJEC+
    process.updatedPatJetsTransientCorrectedUpdatedJEC+
    process.selectedUpdatedPatJetsUpdatedJEC+
    process.selectedSlimmedJetsAK4+
    # process.icPFJetProducerFromPat +
    process.slimmedJetsSmeared+
    process.slimmedJetsSmearedDown+
    process.slimmedJetsSmearedUp+
    process.icPFJetProducerFromPatNew
    )

# ################################################################
# # PF MET
# ################################################################
process.load('JetMETCorrections.Configuration.JetCorrectors_cff')
process.load("RecoJets.JetProducers.ak4PFJets_cfi")

from RecoMET.METProducers.PFMET_cfi import pfMet

from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
runMetCorAndUncFromMiniAOD(process,
                           isData=bool(isData) or bool(isEmbed),
                           postfix="ModifiedMET",
                           )


from PhysicsTools.PatAlgos.slimming.puppiForMET_cff import makePuppiesFromMiniAOD
makePuppiesFromMiniAOD( process, True );
runMetCorAndUncFromMiniAOD(process,
                           isData=(bool(isData) or bool(isEmbed)),
                           metType="Puppi",
                           postfix="PuppiModifiedMET",
                           jetFlavor="AK4PFPuppi",
                           )

process.icPfMetProducer = producers.icMetFromPatProducer.clone(
                         branch = cms.string("pfMetFromSlimmed"),
                         input = cms.InputTag("slimmedMETsModifiedMET"),
                         getUncorrectedMet=cms.bool(False),
                         includeMetUncertainties=cms.bool(True)
                         )


process.icPuppiMetProducer = producers.icMetFromPatProducer.clone(
                         input=cms.InputTag("slimmedMETsPuppiModifiedMET"),
                         branch = cms.string("puppiMet"),
                         getUncorrectedMet=cms.bool(False),
                         includeMetUncertainties=cms.bool(True)
                         )

process.icMetSequence = cms.Sequence(
  process.puppiMETSequence *
  process.fullPatMetSequencePuppiModifiedMET *
  process.fullPatMetSequenceModifiedMET *
  process.icPfMetProducer *
  process.icPuppiMetProducer
)

################################################################
# Simulation only: GenParticles, GenJets, PileupInfo
################################################################
process.icGenSequence = cms.Sequence()

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.prunedGenParticles = cms.EDProducer("GenParticlePruner",
    src = cms.InputTag("genParticles","","HLT"),
    select = cms.vstring(
        "drop  *", # this is the default
        "++keep abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15", # keep leptons, with history
        "keep abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16",   # keep neutrinos
        "drop   status == 2",                                              # drop the shower part of the history
        "+keep pdgId == 22 && status == 1 && (pt > 10 || isPromptFinalState())", # keep gamma above 10 GeV (or all prompt) and its first parent
        "+keep abs(pdgId) == 11 && status == 1 && (pt > 3 || isPromptFinalState())", # keep first parent of electrons above 3 GeV (or prompt)
        "keep++ abs(pdgId) == 15",                                         # but keep keep taus with decays
	"drop  status > 30 && status < 70 ", 				   #remove pythia8 garbage
	"drop  pdgId == 21 && pt < 5",                                    #remove pythia8 garbage
        "drop   status == 2 && abs(pdgId) == 21",                          # but remove again gluons in the inheritance chain
        "keep abs(pdgId) == 23 || abs(pdgId) == 24 || abs(pdgId) == 25 || abs(pdgId) == 6 || abs(pdgId) == 37 ",   # keep VIP(articles)s
        "keep abs(pdgId) == 310 && abs(eta) < 2.5 && pt > 1 ",                                                     # keep K0
# keep heavy flavour quarks for parton-based jet flavour
	"keep (4 <= abs(pdgId) <= 5) & (status = 2 || status = 11 || status = 71 || status = 72)",
# keep light-flavour quarks and gluons for parton-based jet flavour
	"keep (1 <= abs(pdgId) <= 3 || pdgId = 21) & (status = 2 || status = 11 || status = 71 || status = 72) && pt>5",
# keep b and c hadrons for hadron-based jet flavour
	"keep (400 < abs(pdgId) < 600) || (4000 < abs(pdgId) < 6000)",
# additional c hadrons for jet fragmentation studies
	"keep abs(pdgId) = 10411 || abs(pdgId) = 10421 || abs(pdgId) = 10413 || abs(pdgId) = 10423 || abs(pdgId) = 20413 || abs(pdgId) = 20423 || abs(pdgId) = 10431 || abs(pdgId) = 10433 || abs(pdgId) = 20433",
# additional b hadrons for jet fragmentation studies
	"keep abs(pdgId) = 10511 || abs(pdgId) = 10521 || abs(pdgId) = 10513 || abs(pdgId) = 10523 || abs(pdgId) = 20513 || abs(pdgId) = 20523 || abs(pdgId) = 10531 || abs(pdgId) = 10533 || abs(pdgId) = 20533 || abs(pdgId) = 10541 || abs(pdgId) = 10543 || abs(pdgId) = 20543",
#keep SUSY particles
	"keep (1000001 <= abs(pdgId) <= 1000039 ) || ( 2000001 <= abs(pdgId) <= 2000015)",
# keep protons
        "keep pdgId = 2212",
        "keep status == 3 || ( 21 <= status <= 29) || ( 11 <= status <= 19)",  #keep event summary (status=3 for pythia6, 21 <= status <= 29 for pythia8)
        "keep isHardProcess() || fromHardProcessFinalState() || fromHardProcessDecayed() || fromHardProcessBeforeFSR() || (statusFlags().fromHardProcess() && statusFlags().isLastCopy())",  #keep event summary based on status flags
    )
)



process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  input   = cms.InputTag("prunedGenParticles"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True),
  includeStatusFlags = cms.bool(True)
)


process.icGenParticleProducerFromLHEParticles = producers.icGenParticleFromLHEParticlesProducer.clone(
  input   = cms.InputTag(opts.LHETag)
)

if isEmbed:
  process.icGenParticleProducer.input=cms.InputTag("prunedGenParticles","","MERGE")
else:
  process.icGenParticleProducer.input=cms.InputTag("prunedGenParticles","","PAT")
process.load("RecoJets.Configuration.GenJetParticles_cff")
process.genParticlesForJets.ignoreParticleIDs = cms.vuint32(
  1000022, 2000012, 2000014,
  2000016, 1000039, 5000039,
  4000012, 9900012, 9900014,
  9900016, 39, 12, 14, 16
)
process.genParticlesForJets.src = cms.InputTag("packedGenParticles")

process.load("RecoJets.JetProducers.ak4GenJets_cfi")
process.ak4GenJetsNoNuBSM  =  process.ak4GenJets.clone()

process.ak4GenJetsNoNuBSM.src=cms.InputTag("packedGenParticles") #This still contains nus in 72, should be fixed in 74

process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
  src = cms.InputTag("ak4GenJetsNoNuBSM"),
  cut = cms.string("pt > 10.0")
)

process.icGenJetProducer = producers.icGenJetProducer.clone(
  branch  = cms.string("genJets"),
  input   = cms.InputTag("selectedGenJets"),
  inputGenParticles = cms.InputTag("genParticles"),
  requestGenParticles = cms.bool(False)
)

process.icGenJetProducer.branch = cms.string("genJetsReclustered")
if isEmbed:
  process.icGenJetProducer.inputGenParticles = cms.InputTag("prunedGenParticles","","MERGE")
else:
  process.icGenJetProducer.inputGenParticles = cms.InputTag("prunedGenParticles","","PAT")
process.icGenJetProducer.isSlimmed  = cms.bool(True)
process.icGenJetProducerFromSlimmed = producers.icGenJetProducer.clone(
  branch = cms.string("genJets"),
  input = cms.InputTag("slimmedGenJets"),
  inputGenParticles=cms.InputTag("genParticles"),
  requestGenParticles = cms.bool(False),
  isSlimmed = cms.bool(True)
)

process.icPileupInfoProducer = producers.icPileupInfoProducer.clone()
process.icPileupInfoProducer.input=cms.InputTag("slimmedAddPileupInfo")


if not isData:
  process.icGenSequence += (
    process.prunedGenParticles+
    process.icGenParticleProducer
  )
  process.icGenSequence.remove(process.prunedGenParticles)
  process.icGenSequence += (
    process.ak4GenJetsNoNuBSM+
    process.selectedGenJets+
    process.icGenJetProducer+
    process.icGenJetProducerFromSlimmed+
    process.icPileupInfoProducer
  )

  if doHT:
    process.icGenSequence += (
      process.icGenParticleProducerFromLHEParticles
    )
  if isEmbed:
    process.icGenSequence.remove(process.icPileupInfoProducer)
    process.icGenSequence.remove(process.icGenJetProducerFromSlimmed)


# ################################################################
# # Trigger
# ################################################################
from PhysicsTools.PatAlgos.tools.trigTools import *
process.icTriggerSequence = cms.Sequence()
process.icTriggerObjectSequence = cms.Sequence()

################################################################
# L1 Objects
################################################################

v_doBXloop = False

process.icL1EGammaProducer = cms.EDProducer('ICL1TObjectProducer<l1t::EGamma>',
  branch = cms.string("L1EGammas"),
  input = cms.InputTag("caloStage2Digis","EGamma"),
  doBXloop = cms.bool(v_doBXloop)
)

process.icL1TauProducer = cms.EDProducer('ICL1TObjectProducer<l1t::Tau>',
  branch = cms.string("L1Taus"),
  input = cms.InputTag("caloStage2Digis","Tau"),
  doBXloop = cms.bool(v_doBXloop)
)

process.icL1MuonProducer = cms.EDProducer('ICL1TObjectProducer<l1t::Muon>',
  branch = cms.string("L1Muons"),
  input = cms.InputTag("gmtStage2Digis","Muon"),
  doBXloop = cms.bool(v_doBXloop)
)

if isEmbed:
  process.icL1EGammaProducer.input = cms.InputTag("caloStage2Digis","EGamma","SIMembedding")
  process.icL1TauProducer.input = cms.InputTag("caloStage2Digis","Tau","SIMembedding")
  process.icL1MuonProducer.input = cms.InputTag("gmtStage2Digis","Muon","SIMembedding")


process.patTriggerPath = cms.Path()
switchOnTrigger(process, path = 'patTriggerPath',  outputModule = '')



process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
 branch = cms.string("triggerPaths"),
 input  = cms.InputTag("TriggerResults","","HLT"),
 inputIsStandAlone = cms.bool(True),
 inputPrescales = cms.InputTag("patTrigger")
)


if isData or isEmbed:
  process.icTriggerSequence += cms.Sequence(
   process.icTriggerPathProducer
  )

process.icEle12Mu23ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle12Mu23"),
      hltPath = cms.string("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle23Mu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle23Mu8"),
      hltPath = cms.string("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )



process.icEle24LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle24LooseTau20"),
      hltPath = cms.string("HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v"),
  #    hltPath = cms.string("HLT_Ele22_eta2p1_WP75_Gsf_LooseIsoPFTau20_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle24LooseTau20SingleL1ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle24LooseTau20SingleL1"),
      hltPath = cms.string("HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v"),
  #    hltPath = cms.string("HLT_Ele22_eta2p1_WP75_Gsf_LooseIsoPFTau20_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle22LooseTau20SingleL1ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle22LooseTau20SingleL1"),
      hltPath = cms.string("HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v"),
  #    hltPath = cms.string("HLT_Ele22_eta2p1_WP75_Gsf_LooseIsoPFTau20_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )


process.icEle25TightEta2p1GsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle25GsfTightEta2p1"),
      hltPath = cms.string("HLT_Ele25_eta2p1_WPTight_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icIsoMu19LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu19LooseTau20"),
      hltPath = cms.string("HLT_IsoMu19_eta2p1_LooseIsoPFTau20_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icIsoMu19LooseTau20SingleL1ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu19LooseTau20SingleL1"),
      hltPath = cms.string("HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icIsoMu24ObjectProducer = producers.icTriggerObjectProducer.clone(
    input   = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoMu24"),
    hltPath = cms.string("HLT_IsoMu24_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )

process.icIsoMu22ObjectProducer = producers.icTriggerObjectProducer.clone(
    input   = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoMu22"),
    hltPath = cms.string("HLT_IsoMu22_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )

process.icIsoMu22Eta2p1ObjectProducer = producers.icTriggerObjectProducer.clone(
    input   = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoMu22Eta2p1"),
    hltPath = cms.string("HLT_IsoMu22_eta2p1_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )

process.icIsoTkMu22ObjectProducer = producers.icTriggerObjectProducer.clone(
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoTkMu22"),
    hltPath = cms.string("HLT_IsoTkMu22_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )

process.icIsoTkMu24ObjectProducer = producers.icTriggerObjectProducer.clone(
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoTkMu24"),
    hltPath = cms.string("HLT_IsoTkMu24_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )

process.icIsoTkMu22Eta2p1ObjectProducer = producers.icTriggerObjectProducer.clone(
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoTkMu22Eta2p1"),
    hltPath = cms.string("HLT_IsoTkMu22_eta2p1_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )

process.icIsoMu21MediumTau32ObjectProducer = producers.icTriggerObjectProducer.clone(
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoMu21MediumTau32"),
    hltPath           = cms.string("HLT_IsoMu21_eta2p1_MediumIsoPFTau32_Trk1_eta2p1_Reg_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired  = cms.bool(False)
    )

process.icIsoMu19MediumTau32ObjectProducer = producers.icTriggerObjectProducer.clone(
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoMu19MediumTau32"),
    hltPath           = cms.string("HLT_IsoMu19_eta2p1_MediumIsoPFTau32_Trk1_eta2p1_Reg_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired  = cms.bool(False)
    )

process.icDoubleMediumTau32ObjectProducer = producers.icTriggerObjectProducer.clone(
   input = cms.InputTag("patTriggerEvent"),
   branch = cms.string("triggerObjectsDoubleMediumTau32"),
   hltPath = cms.string("HLT_DoubleMediumIsoPFTau32_Trk1_eta2p1_Reg_v"),
   inputIsStandAlone = cms.bool(False),
   storeOnlyIfFired = cms.bool(False)
   )

process.icDoubleMediumTau35ObjectProducer = producers.icTriggerObjectProducer.clone(
   input = cms.InputTag("patTriggerEvent"),
   branch = cms.string("triggerObjectsDoubleMediumTau35"),
   hltPath = cms.string("HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v"),
   inputIsStandAlone = cms.bool(False),
   storeOnlyIfFired = cms.bool(False)
   )

process.icDoubleMediumTau40ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsDoubleMediumTau40"),
      hltPath = cms.string("HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icSingleTau140ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsSingleTau140"),
      hltPath = cms.string("HLT_VLooseIsoPFTau140_Trk50_eta2p1_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icSingleTau120ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsSingleTau120"),
      hltPath = cms.string("HLT_VLooseIsoPFTau120_Trk50_eta2p1_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icIsoMu19MediumCombinedIsoTau32RegObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu19MediumCombinedIsoTau32Reg"),
      hltPath = cms.string("HLT_IsoMu19_eta2p1_MediumCombinedIsoPFTau32_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icIsoMu21MediumCombinedIsoTau32RegObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu21MediumCombinedIsoTau32Reg"),
      hltPath = cms.string("HLT_IsoMu21_eta2p1_MediumCombinedIsoPFTau32_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle12Mu23ObjectProducerDZ = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle12Mu23DZ"),
      hltPath = cms.string("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle23Mu8ObjectProducerDZ = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle23Mu8DZ"),
      hltPath = cms.string("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )


process.icDoubleMediumCombinedIsoTau35RegObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsDoubleMediumCombinedIsoTau35Reg"),
      hltPath = cms.string("HLT_DoubleMediumCombinedIsoPFTau35_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icDoubleMediumCombinedIsoTau40RegObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsDoubleMediumCombinedIsoTau40Reg"),
      hltPath = cms.string("HLT_DoubleMediumCombinedIsoPFTau40_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icDoubleMediumCombinedIsoTau40ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsDoubleMediumCombinedIsoTau40"),
      hltPath = cms.string("HLT_DoubleMediumCombinedIsoPFTau40_Trk1_eta2p1_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icIsoMu19erMediumCombinedIsoTau32ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu19erMediumCombinedIsoTau32"),
      hltPath = cms.string("HLT_IsoMu19_eta2p1_MediumCombinedIsoPFTau32_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icIsoMu19erMediumIsoTau32ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu19erMediumIsoTau32"),
      hltPath = cms.string("HLT_IsoMu19_eta2p1_MediumIsoPFTau32_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle23Ele12ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle23Ele12"),
      hltPath = cms.string("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle23Ele12DZObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle23Ele12DZ"),
      hltPath = cms.string("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )


process.icMu17Mu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsMu17Mu8"),
      hltPath = cms.string("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icMu17TkMu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsMu17TkMu8"),
      hltPath = cms.string("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icMu17Mu8DZObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsMu17Mu8DZ"),
      hltPath = cms.string("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icMu17TkMu8DZObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsMu17TkMu8DZ"),
      hltPath = cms.string("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icTriggerObjectSequence += cms.Sequence(
      process.icIsoMu19erMediumCombinedIsoTau32ObjectProducer+
      process.icIsoMu19erMediumIsoTau32ObjectProducer+
      process.icEle23Ele12ObjectProducer+
      process.icEle23Ele12DZObjectProducer+
      process.icMu17Mu8ObjectProducer+
      process.icMu17TkMu8ObjectProducer+
      process.icMu17Mu8DZObjectProducer+
      process.icMu17TkMu8DZObjectProducer+
      process.icEle12Mu23ObjectProducer +
      process.icEle23Mu8ObjectProducer +
      process.icEle24LooseTau20ObjectProducer +
      process.icEle24LooseTau20SingleL1ObjectProducer +
      process.icEle22LooseTau20SingleL1ObjectProducer +
      process.icEle25TightEta2p1GsfObjectProducer +
      process.icIsoMu19LooseTau20ObjectProducer +
      process.icIsoMu19LooseTau20SingleL1ObjectProducer +
      process.icIsoMu22Eta2p1ObjectProducer+
      process.icIsoMu24ObjectProducer+
      process.icIsoTkMu22ObjectProducer+
      process.icIsoTkMu24ObjectProducer+
      process.icIsoMu22ObjectProducer+
      process.icIsoTkMu22Eta2p1ObjectProducer+

      process.icDoubleMediumTau40ObjectProducer +
      process.icDoubleMediumTau35ObjectProducer +
      process.icDoubleMediumTau32ObjectProducer +
      process.icSingleTau140ObjectProducer+
      process.icSingleTau120ObjectProducer+

      process.icIsoMu19MediumTau32ObjectProducer+
      process.icIsoMu21MediumTau32ObjectProducer+

      process.icIsoMu19MediumCombinedIsoTau32RegObjectProducer+
      process.icIsoMu21MediumCombinedIsoTau32RegObjectProducer+
      process.icEle12Mu23ObjectProducerDZ+
      process.icEle23Mu8ObjectProducerDZ+
      process.icDoubleMediumCombinedIsoTau35RegObjectProducer+
      process.icDoubleMediumCombinedIsoTau40RegObjectProducer+
      process.icDoubleMediumCombinedIsoTau40ObjectProducer
      )


for name in process.icTriggerObjectSequence.moduleNames():
  mod = getattr(process, name)
  mod.inputIsStandAlone = cms.bool(True)
  mod.input = cms.InputTag("selectedPatTrigger")
  if isEmbed:
    mod.inputTriggerResults = cms.InputTag("TriggerResults","","SIMembedding")
  elif isReHLT and not isEmbed:
    mod.inputTriggerResults = cms.InputTag("TriggerResults", "","HLT")

## Need to unpack filterLabels on slimmedPatTrigger then make selectedPatTrigger
process.patTriggerUnpacker = cms.EDProducer("PATTriggerObjectStandAloneUnpacker",
    patTriggerObjectsStandAlone = cms.InputTag("slimmedPatTrigger"),
    triggerResults = cms.InputTag("TriggerResults", "", "HLT"),
    unpackFilterLabels = cms.bool(True)
)

if isEmbed: process.patTriggerUnpacker.triggerResults = cms.InputTag("TriggerResults", "", "SIMembedding")

process.selectedPatTrigger = cms.EDFilter(
    'PATTriggerObjectStandAloneSelector',
    cut = cms.string('!filterLabels.empty()'),
    src = cms.InputTag('patTriggerUnpacker')
)
process.icTriggerSequence += cms.Sequence(
    process.patTriggerUnpacker +
    process.selectedPatTrigger
)

################################################################
# EventInfo
################################################################

from RecoMET.METFilters.BadPFMuonDzFilter_cfi import BadPFMuonDzFilter

process.BadPFMuonFilterUpdateDz=BadPFMuonDzFilter.clone(
    muons = cms.InputTag("slimmedMuons"),
    vtx   = cms.InputTag("offlineSlimmedPrimaryVertices"),
    PFCandidates = cms.InputTag("packedPFCandidates"),
    minDzBestTrack = cms.double(0.5),
    taggingMode    = cms.bool(True)
)

if opts.LHETag: lheTag = opts.LHETag
else: lheTag = 'externalLHEProducer'

data_type = ""
if isData: data_type = "RECO"
elif isEmbed: data_type = "MERGE"
else: data_type = "PAT"

## add prefiring weights
from PhysicsTools.PatUtils.l1ECALPrefiringWeightProducer_cfi import l1ECALPrefiringWeightProducer
process.prefiringweight = l1ECALPrefiringWeightProducer.clone(
    DataEra = cms.string("2016BtoH"), #Use 2016BtoH for 2016
    UseJetEMPt = cms.bool(False),
    PrefiringRateSystematicUncty = cms.double(0.2),
    SkipWarnings = False)

process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  includePrefireWeights = cms.bool(not bool(isData) and not bool(isEmbed)),
  includeJetRho       = cms.bool(True),
  includeLHEWeights   = cms.bool(doLHEWeights),
  includeGenWeights   = cms.bool(doLHEWeights),
  includenpNLO        = cms.bool(includenpNLO),
  includeHTXS         = cms.bool(opts.includeHTXS),
  includeHT           = cms.bool(False),
  lheProducer         = cms.InputTag(lheTag),
  inputJetRho         = cms.InputTag("fixedGridRhoFastjetAll"),
  includeLeptonRho    = cms.bool(False),
  inputLeptonRho      = cms.InputTag("fixedGridRhoFastjetAll"),
  includeVertexCount  = cms.bool(True),
  inputVertices       = vtxLabel,
  includeCSCFilter    = cms.bool(False),
  inputCSCFilter      = cms.InputTag("BeamHaloSummary"),
  includeFiltersFromTrig = cms.bool(True),
  inputfiltersfromtrig = cms.InputTag("TriggerResults","",data_type),
  filtersfromtrig     = cms.vstring(
        "Flag_goodVertices","Flag_globalSuperTightHalo2016Filter",
        "Flag_HBHENoiseFilter", "Flag_HBHENoiseIsoFilter",
        "Flag_EcalDeadCellTriggerPrimitiveFilter", "Flag_BadPFMuonFilter",
        "Flag_BadPFMuonDzFilter","Flag_eeBadScFilter"
  )
)

if isEmbed:
  process.icEventInfoProducer.includeEmbeddingWeights = cms.bool(True)
  process.icEventInfoProducer.inputfiltersfromtrig = cms.InputTag("TriggerResults","","MERGE")

process.icEventInfoSequence = cms.Sequence(
  process.BadPFMuonFilterUpdateDz+
  process.prefiringweight+
  process.icEventInfoProducer
)


################################################################
# TauSpinner
################################################################


process.icTauSpinnerProducer = cms.EDProducer("ICTauSpinnerProducer",
  branch                  = cms.string("tauspinner"),
  input                   = cms.InputTag("prunedGenParticles"),
  theta                   = cms.string("0,0.25,0.5,-0.25,0.375")
)

if opts.tauSpinner:
  process.icTauSpinnerSequence = cms.Sequence(
    process.icTauSpinnerProducer
  )
else: process.icTauSpinnerSequence = cms.Sequence()

################################################################
# HTXS NNLOPS
################################################################

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.mergedGenParticles = cms.EDProducer("MergedGenParticleProducer",
    inputPruned = cms.InputTag("prunedGenParticles"),
    inputPacked = cms.InputTag("packedGenParticles"),
)
process.genParticles2HepMC = cms.EDProducer("GenParticles2HepMCConverter",
    genParticles = cms.InputTag("mergedGenParticles"),
    genEventInfo = cms.InputTag("generator"),
    signalParticlePdgIds = cms.vint32(25),
)
process.rivetProducerHTXS = cms.EDProducer('HTXSRivetProducer',
    HepMCCollection = cms.InputTag('genParticles2HepMC','unsmeared'),
    LHERunInfo = cms.InputTag('externalLHEProducer'),
    ProductionMode = cms.string('AUTO'),
)
process.icHtxsSequence = cms.Sequence()
if opts.includeHTXS:
    process.icHtxsSequence = cms.Sequence(
        process.mergedGenParticles *
        process.genParticles2HepMC *
        process.rivetProducerHTXS
    )

################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()



process.p = cms.Path(
  process.icMiniAODSequence+
  process.icSelectionSequence+
  process.pfParticleSelectionSequence+
  process.icElectronSequence+
  process.icVertexSequence+
  process.icMuonSequence+
  process.icTauSequence+
  process.icTauProducer+
  process.icPhotonSequence+
  process.icMetSequence+
  process.icPFJetSequence+
  process.icPFSequence+
  process.icGenSequence+
  process.icTriggerSequence+
  process.icTriggerObjectSequence+
  process.icL1EGammaProducer+
  process.icL1TauProducer+
  process.icL1MuonProducer+
  process.icTauSpinnerSequence+
  process.icHtxsSequence+
  process.icEventInfoSequence+
  process.icEventProducer
)

if not isData and not isReHLT:
  process.p.remove(process.icTriggerSequence)
  process.p.remove(process.icTriggerObjectSequence)

process.schedule = cms.Schedule(process.p)

