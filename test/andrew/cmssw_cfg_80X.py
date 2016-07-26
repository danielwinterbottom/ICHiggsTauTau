import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing('analysis')
opts.register('file', '',
    parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, 'input file')
opts.register('globalTag', '80X_dataRun2_Prompt_ICHEP16JEC_v0', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, 'global tag')
opts.register('isData', 1, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, 'Process as data')
opts.register('release', '80XMINIAOD', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, 'Release label')
opts.register('hltPaths', 'None', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, 'Trigger object collections')
opts.parseArguments()
infile = opts.file
isData = opts.isData
tag  = opts.globalTag
release = opts.release
hltPaths = opts.hltPaths

if not release in ['80XMINIAOD']:
  print 'Release not recognised, exiting!'
  sys.exit(1)
print 'release     : '+release
print 'isData      : '+str(isData)
print 'globalTag   : '+str(tag)
print 'hltPaths    : '+str(hltPaths)

# Some other flags
doPFChargedAllIso = False
doPFWeightedMuonIso = False

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
    input = cms.untracked.int32(3000)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 500

process.options   = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
    # allowUnscheduled = cms.untracked.bool(True)
)


################################################################
# Input files and global tags
################################################################
process.load("CondCore.CondDB.CondDB_cfi")
from CondCore.CondDB.CondDB_cfi import *

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(infile)
    )

process.GlobalTag.globaltag = cms.string(tag)

import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

process.load("PhysicsTools.PatAlgos.patSequences_cff")

################################################################
# Event
################################################################

if release in ['80XMINIAOD']:
    process.selectedElectrons = cms.EDFilter("PATElectronRefSelector",
        src = cms.InputTag("slimmedElectrons"),
        cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
        )
    process.selectedMuons = cms.EDFilter("PATMuonRefSelector",
        src = cms.InputTag("slimmedMuons"),
        cut = cms.string("pt > 3 & abs(eta) < 2.6")
        )
    process.selectedTaus = cms.EDFilter("PATTauRefSelector",
        src = cms.InputTag("slimmedTaus"),
        cut = cms.string('pt > 18.0 & abs(eta) < 2.6 & tauID("decayModeFindingNewDMs") > 0.5')
        )

process.icSelectionSequence = cms.Sequence(
    process.selectedElectrons+
    process.selectedMuons+
    process.selectedTaus
)


################################################################
# PF sequence for lepton isolation
################################################################
process.pfParticleSelectionSequence = cms.Sequence()

if release in ['76X'] and doPFChargedAllIso:
    process.load("CommonTools.ParticleFlow.pfParticleSelection_cff")
    process.pfPileUp.PFCandidates = cms.InputTag("particleFlowPtrs")
    process.pfPileUpIso.PFCandidates = cms.InputTag("particleFlowPtrs")
    process.pfNoPileUp.bottomCollection = cms.InputTag("particleFlowPtrs")
    process.pfNoPileUpIso.bottomCollection = cms.InputTag("particleFlowPtrs")

if release in ['80XMINIAOD'] and doPFChargedAllIso:
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
    process.pfParticleSelectionSequence += cms.Sequence(
        process.pfPileUp+
        process.pfNoPileUp+
        process.pfAllNeutralHadrons+
        process.pfAllChargedHadrons+
        process.pfAllPhotons+
        process.pfAllChargedParticles+
        process.pfPileUpAllChargedParticles+
        process.pfAllNeutralHadronsAndPhotons
    )


################################################################
# Vertices
################################################################
vtxLabel = cms.InputTag("offlinePrimaryVertices")
if release in ['80XMINIAOD']:
    vtxLabel = cms.InputTag("offlineSlimmedPrimaryVertices")

process.icVertexProducer = producers.icVertexProducer.clone(
  branch            = cms.string("vertices"),
  input             = vtxLabel,
  firstVertexOnly   = cms.bool(True)
)

process.icVertexSequence = cms.Sequence(
  process.icVertexProducer
)


################################################################
# Electrons
################################################################
electronLabel = cms.InputTag("gedGsfElectrons")
if release in ['80XMINIAOD']:
  electronLabel = cms.InputTag("slimmedElectrons")

process.icElectronSequence = cms.Sequence()

### Add the conversion calculator
process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
    input       = electronLabel,
    beamspot    = cms.InputTag("offlineBeamSpot"),
    conversions = cms.InputTag("allConversions")
)

if release in ['80XMINIAOD']:
    process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
        input       = electronLabel,
        beamspot    = cms.InputTag("offlineBeamSpot"),
        conversions = cms.InputTag("reducedEgamma:reducedConversions")
    )

process.icElectronSequence += cms.Sequence(
   process.icElectronConversionCalculator
)


### Add the ID MVA value map producer
process.load("RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi")
process.electronMVAValueMapProducer.src = electronLabel
process.electronMVAValueMapProducer.srcMiniAOD = electronLabel

process.icElectronSequence += cms.Sequence(
   process.electronMVAValueMapProducer
)


### Electron PF iso sequence:
process.load("CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff")
process.electronPFIsolationValuesSequence = cms.Sequence()

# DR = 0.3 extractors
process.elPFIsoValueCharged03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
    input    = electronLabel,
    deltaR   = cms.double(0.3),
    iso_type = cms.string("charged_iso")
)
process.elPFIsoValueGamma03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
    input    = electronLabel,
    deltaR   = cms.double(0.3),
    iso_type = cms.string("photon_iso")
)
process.elPFIsoValueNeutral03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
    input    = electronLabel,
    deltaR   = cms.double(0.3),
    iso_type = cms.string("neutral_iso")
)
process.elPFIsoValuePU03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
    input    = electronLabel,
    deltaR   = cms.double(0.3),
    iso_type = cms.string("pu_iso")
)

# DR = 0.4 extractors
process.elPFIsoValueCharged04PFIdPFIso = cms.EDProducer('ICElectronIsolation',
    input    = electronLabel,
    deltaR   = cms.double(0.4),
    iso_type = cms.string("charged_iso")
)
process.elPFIsoValueGamma04PFIdPFIso = cms.EDProducer('ICElectronIsolation',
    input    = electronLabel,
    deltaR   = cms.double(0.4),
    iso_type = cms.string("photon_iso")
)
process.elPFIsoValueNeutral04PFIdPFIso = cms.EDProducer('ICElectronIsolation',
    input    = electronLabel,
    deltaR   = cms.double(0.4),
    iso_type = cms.string("neutral_iso")
)
process.elPFIsoValuePU04PFIdPFIso = cms.EDProducer('ICElectronIsolation',
    input    = electronLabel,
    deltaR   = cms.double(0.4),
    iso_type = cms.string("pu_iso")
)

# Create chargedAll:
if doPFChargedAllIso:
    process.elPFIsoDepositChargedAll.src  = electronLabel

    process.elPFIsoValueChargedAll03PFIdPFIso = process.elPFIsoValueChargedAll03PFId.clone()
    process.elPFIsoValueChargedAll03PFIdPFIso.deposits[0].vetos = (
        cms.vstring('EcalEndcaps:ConeVeto(0.015)','EcalBarrel:ConeVeto(0.01)'))

    process.elPFIsoValueChargedAll04PFIdPFIso = process.elPFIsoValueChargedAll04PFId.clone()
    process.elPFIsoValueChargedAll04PFIdPFIso.deposits[0].vetos = (
      cms.vstring('EcalEndcaps:ConeVeto(0.015)','EcalBarrel:ConeVeto(0.01)'))

    process.electronPFIsolationValuesSequence += cms.Sequence(
        process.elPFIsoDepositChargedAll
    )
else:
    process.elPFIsoValueChargedAll03PFIdPFIso = process.elPFIsoValueCharged03PFIdPFIso.clone()
    process.elPFIsoValueChargedAll04PFIdPFIso = process.elPFIsoValueCharged04PFIdPFIso.clone()

# PF cluster iso
process.elEcalPFClusterIso = cms.EDProducer('ICElectronIsolation',
    input    = electronLabel,
    deltaR   = cms.double(0.3),
    iso_type = cms.string("ecal_pf_cluster_iso")
)
process.elHcalPFClusterIso = cms.EDProducer('ICElectronIsolation',
    input    = electronLabel,
    deltaR   = cms.double(0.3),
    iso_type = cms.string("hcal_pf_cluster_iso")
)


process.electronPFIsolationValuesSequence += cms.Sequence(
    process.elPFIsoValueCharged03PFIdPFIso+
    process.elPFIsoValueChargedAll03PFIdPFIso+
    process.elPFIsoValueGamma03PFIdPFIso+
    process.elPFIsoValueNeutral03PFIdPFIso+
    process.elPFIsoValuePU03PFIdPFIso+
    process.elPFIsoValueCharged04PFIdPFIso+
    process.elPFIsoValueChargedAll04PFIdPFIso+
    process.elPFIsoValueGamma04PFIdPFIso+
    process.elPFIsoValueNeutral04PFIdPFIso+
    process.elPFIsoValuePU04PFIdPFIso+
    process.elEcalPFClusterIso+
    process.elHcalPFClusterIso
)

process.icElectronSequence += cms.Sequence(
    process.electronPFIsolationValuesSequence
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
    includeFloats = cms.PSet(
       mvaNonTrigSpring15    = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Values"),
       mvaTrigSpring15       = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15Trig25nsV1Values")
    ),
    includeClusterIso        = cms.bool(True),
    includePFIso03           = cms.bool(True),
    includePFIso04           = cms.bool(True)
)

process.icElectronSequence += cms.Sequence(
  process.icElectronProducer
)


################################################################
# Muons
################################################################
muonLabel = cms.InputTag("muons")
if release in ['80XMINIAOD']:
    muonLabel = cms.InputTag("slimmedMuons")

process.icMuonSequence = cms.Sequence()

### Muon PF iso sequence:
process.load("CommonTools.ParticleFlow.Isolation.pfMuonIsolation_cff")
process.load("CommonTools.ParticleFlow.deltaBetaWeights_cff")
process.muonPFIsolationValuesSequence = cms.Sequence()

# DR = 0.3 extractors
process.muPFIsoValueCharged03PFIso = cms.EDProducer('ICMuonIsolation',
    input     = muonLabel,
    deltaR    = cms.double(0.3),
    iso_type  = cms.string("charged_iso")
)
process.muPFIsoValueGamma03PFIso = cms.EDProducer('ICMuonIsolation',
    input    = muonLabel,
    deltaR   = cms.double(0.3),
    iso_type = cms.string("photon_iso")
)
process.muPFIsoValueNeutral03PFIso = cms.EDProducer('ICMuonIsolation',
    input    = muonLabel,
    deltaR   = cms.double(0.3),
    iso_type = cms.string("neutral_iso")
)
process.muPFIsoValuePU03PFIso = cms.EDProducer('ICMuonIsolation',
    input    = muonLabel,
    deltaR   = cms.double(0.3),
    iso_type = cms.string("pu_iso")
)

# DR = 0.4 extractors
process.muPFIsoValueCharged04PFIso = cms.EDProducer('ICMuonIsolation',
    input    = muonLabel,
    deltaR   = cms.double(0.4),
    iso_type = cms.string("charged_iso")
)
process.muPFIsoValueGamma04PFIso = cms.EDProducer('ICMuonIsolation',
    input    = muonLabel,
    deltaR   = cms.double(0.4),
    iso_type = cms.string("photon_iso")
)
process.muPFIsoValueNeutral04PFIso = cms.EDProducer('ICMuonIsolation',
    input    = muonLabel,
    deltaR   = cms.double(0.4),
    iso_type = cms.string("neutral_iso")
)
process.muPFIsoValuePU04PFIso = cms.EDProducer('ICMuonIsolation',
    input    = muonLabel,
    deltaR   = cms.double(0.4),
    iso_type = cms.string("pu_iso")
)

# Create chargedAll:
if doPFChargedAllIso:
    process.muPFIsoDepositChargedAll.src  = muonLabel
    process.muPFIsoValueChargedAll03PFIso = process.muPFIsoValueChargedAll03.clone()
    process.muPFIsoValueChargedAll04PFIso = process.muPFIsoValueChargedAll04.clone()
    process.muonPFIsolationValuesSequence += cms.Sequence(
        process.muPFIsoDepositChargedAll
    )
else:
    process.muPFIsoValueChargedAll03PFIso = process.muPFIsoValueCharged03PFIso.clone()
    process.muPFIsoValueChargedAll04PFIso = process.muPFIsoValueCharged04PFIso.clone()


process.muonPFIsolationValuesSequence += cms.Sequence(
    process.muPFIsoValueCharged03PFIso+
    process.muPFIsoValueChargedAll03PFIso+
    process.muPFIsoValueGamma03PFIso+
    process.muPFIsoValueNeutral03PFIso+
    process.muPFIsoValuePU03PFIso+
    process.muPFIsoValueCharged04PFIso+
    process.muPFIsoValueChargedAll04PFIso+
    process.muPFIsoValueGamma04PFIso+
    process.muPFIsoValuePU04PFIso+
    process.muPFIsoValueNeutral04PFIso
)

# PF-weighted iso
muonDoubles = cms.PSet()
if doPFWeightedMuonIso:
    process.muPFIsoDepositNeutral.src     = muonLabel
    process.muPFIsoDepositNeutral.ExtractorPSet.inputCandView = cms.InputTag("pfWeightedNeutralHadrons")
    process.muPFIsoDepositGamma.src       = muonLabel
    process.muPFIsoDepositGamma.ExtractorPSet.inputCandView = cms.InputTag("pfWeightedPhotons")

    process.muPFIsoValueNeutral04PFWeights = process.muPFIsoValueNeutral04.clone()
    process.muPFIsoValueGamma04PFWeights = process.muPFIsoValueGamma04.clone()
    process.muPFIsoValueNeutral03PFWeights = process.muPFIsoValueNeutral03.clone()
    process.muPFIsoValueGamma03PFWeights = process.muPFIsoValueGamma03.clone()

    process.muonPFIsolationValuesSequence += cms.Sequence(
        process.pfDeltaBetaWeightingSequence+
        process.muPFIsoDepositNeutral+
        process.muPFIsoDepositGamma+
        process.muPFIsoValueNeutral04PFWeights+
        process.muPFIsoValueGamma04PFWeights+
        process.muPFIsoValueNeutral03PFWeights+
        process.muPFIsoValueGamma03PFWeights
    )

    muonDoubles.neutral_pfweighted_iso_03 = cms.InputTag("muPFIsoValueNeutral03PFWeights")
    muonDoubles.neutral_pfweighted_iso_04 = cms.InputTag("muPFIsoValueNeutral04PFWeights")
    muonDoubles.gamma_pfweighted_iso_03 = cms.InputTag("muPFIsoValueGamma03PFWeights")
    muonDoubles.gamma_pfweighted_iso_04 = cms.InputTag("muPFIsoValueGamma04PFWeights")

process.icMuonSequence += cms.Sequence(
    process.muonPFIsolationValuesSequence
)

process.icMuonProducer = producers.icMuonProducer.clone(
    branch            = cms.string("muons"),
    input             = cms.InputTag("selectedMuons"),
    isPF              = cms.bool(False),
    includeVertexIP   = cms.bool(True),
    inputVertices     = vtxLabel,
    includeBeamspotIP = cms.bool(True),
    inputBeamspot     = cms.InputTag("offlineBeamSpot"),
    includeDoubles    = cms.PSet(muonDoubles),
    requestTracks     = cms.bool(True),
    includePFIso03    = cms.bool(True),
    includePFIso04    = cms.bool(True)
)

process.icMuonSequence += cms.Sequence(
    process.icMuonProducer
)


################################################################
# Taus
################################################################
import UserCode.ICHiggsTauTau.tau_discriminators_cfi as tauIDs

process.icTauProducer = producers.icTauProducer.clone(
    input           = cms.InputTag("selectedTaus"),
    inputVertices   = vtxLabel,
    includeVertexIP = cms.bool(True),
    requestTracks   = cms.bool(True),
    tauIDs          = tauIDs.dynamicStripIds
)

if release in ['80XMINIAOD']:
  process.icTauProducer = cms.EDProducer("ICPFTauFromPatProducer",
      branch          = cms.string("taus"),
      input           = cms.InputTag("selectedTaus"),
      inputVertices   = vtxLabel,
      includeVertexIP = cms.bool(True),
      requestTracks   = cms.bool(False),
      tauIDs          = cms.PSet()
  )

process.icTauSequence = cms.Sequence(
    process.icTauProducer
)


################################################################
# Jets
################################################################

from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection

jec_list = ['L1FastJet','L2Relative','L3Absolute']
updateJetCollection(
    process,
    jetSource = cms.InputTag("slimmedJets"),
    labelName = "UpdatedJEC",
    jetCorrections = ("AK4PFchs", cms.vstring((jec_list+['L2L3Residual']) if isData else jec_list), 'None')
)

process.selectedSlimmedJetsAK4 = cms.EDFilter("PATJetRefSelector",
    src = cms.InputTag("selectedUpdatedPatJetsUpdatedJEC"),
    cut = cms.string("pt > 15")
)

process.load("RecoJets.JetProducers.PileupJetID_cfi")
process.pileupJetIdUpdated = process.pileupJetId.clone(
    jets                = cms.InputTag("slimmedJets"),
    inputIsCorrected    = cms.bool(True),
    applyJec            = cms.bool(True),
    vertexes            = cms.InputTag("offlineSlimmedPrimaryVertices")
)

process.updatedPatJetsUpdatedJEC.userData.userFloats.src += ['pileupJetIdUpdated:fullDiscriminant']

if release in ['80XMINIAOD']:
    process.icPFJetProducerFromPat = producers.icPFJetFromPatProducer.clone(
        branch                    = cms.string("ak4PFJetsCHS"),
        input                     = cms.InputTag("selectedSlimmedJetsAK4"),
        srcConfig = producers.icPFJetFromPatProducer.srcConfig.clone(
            isSlimmed               = cms.bool(True),
            slimmedPileupIDLabel    = cms.string('pileupJetIdUpdated:fullDiscriminant'),
            includeJetFlavour       = cms.bool(True),
            includeJECs             = cms.bool(True),
            inputSVInfo             = cms.InputTag(""),
            requestSVInfo           = cms.bool(False)
        ),
       destConfig = producers.icPFJetFromPatProducer.destConfig.clone(
            includePileupID         = cms.bool(True),
            inputPileupID           = cms.InputTag("puJetMva", "fullDiscriminant"),
            includeTrackBasedVars   = cms.bool(False),
            inputTracks             = cms.InputTag("unpackedTracksAndVertices"),
            inputVertices           = cms.InputTag("unpackedTracksAndVertices"),
            requestTracks           = cms.bool(False)
        )
    )

process.icPFJetSequence = cms.Sequence()

if release in ['80XMINIAOD']:
    process.icPFJetSequence += cms.Sequence(
        process.pileupJetIdUpdated+
        process.patJetCorrFactorsUpdatedJEC+
        process.updatedPatJetsUpdatedJEC+
        process.selectedUpdatedPatJetsUpdatedJEC+
        process.selectedSlimmedJetsAK4+
        # process.unpackedTracksAndVertices+
        process.icPFJetProducerFromPat
    )


################################################################
# MET
################################################################
process.icMetSequence = cms.Sequence()

# Recreate type-1 PFMET with new JECs
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
runMetCorAndUncFromMiniAOD(process, isData=bool(isData))
# Use the newly corrected jets we produced above
process.basicJetsForMet.src = cms.InputTag('updatedPatJetsUpdatedJEC')
if not isData:
    process.patPFMet.addGenMET = cms.bool(False)

process.icPfMetProducer = producers.icMetFromPatProducer.clone(
    branch            = cms.string("pfMet"),
    input             = cms.InputTag("patPFMetT1"),
    getUncorrectedMet = cms.bool(False)
)

process.icMetSequence += cms.Sequence(
    process.pfMet+
    process.basicJetsForMet+
    process.jetSelectorForMet+
    process.cleanedPatJets+
    process.patPFMetT1T2Corr+
    process.patPFMet+
    process.patPFMetT1+
    process.icPfMetProducer
)

# process.icPfMetOriginalProducer = producers.icMetFromPatProducer.clone(
#     branch            = cms.string("pfMetOriginal"),
#     input             = cms.InputTag("slimmedMETs"),
#     getUncorrectedMet = cms.bool(False)
# )

# process.icMetSequence += cms.Sequence(
#     process.icPfMetOriginalProducer
# )

from UserCode.ICHiggsTauTau.configure_mvamet_cff import configureMVAMET
configureMVAMET(process, jetCollectionPF='selectedUpdatedPatJetsUpdatedJEC', isData=bool(isData))
process.icMetSequence += process.icMvaMetSequence

process.icPfMVAMetProducer = producers.icMetFromPatProducer.clone(
    input             = cms.InputTag("MVAMET", "MVAMET"),
    branch            = cms.string("pfMVAMetVector"),
    includeUserCandID = cms.bool(True)
)
process.icMetSequence += cms.Sequence(
    process.icPfMVAMetProducer
)


################################################################
# Tracks
################################################################

process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.icTrackSequence = cms.Sequence()


process.selectedTracks = cms.EDFilter("TrackRefSelector",
 src = cms.InputTag("unpackedTracksAndVertices"),
 cut = cms.string("pt > 20")
)

process.icTrackProducer = producers.icTrackProducer.clone(
 branch = cms.string("tracks"),
 input  = cms.InputTag("selectedTracks")
)

process.icTrackSequence += cms.Sequence(
    process.unpackedTracksAndVertices+
    process.selectedTracks+
    process.icTrackProducer
)

################################################################
# Triggers
################################################################

process.icTriggerPathProducer = cms.EDProducer('ICTriggerPathProducer',
    branch                  = cms.string("triggerPaths"),
    input                   = cms.InputTag('TriggerResults', '', 'HLT' if isData else 'HLT2'),
    includeAcceptedOnly     = cms.bool(False),
    saveStrings             = cms.bool(False),
    splitVersion            = cms.bool(True),
    inputIsStandAlone       = cms.bool(True),
    inputPrescales          = cms.InputTag('patTrigger', '', 'RECO' if isData else 'PAT'),
    prescaleFallback        = cms.bool(True),
    hltProcess              = cms.string('HLT' if isData else 'HLT2')
    )

process.icTriggerObjectSequence = cms.Sequence()

paths_2016_full = [
    'HLT_IsoMu18_v',
    'HLT_IsoMu20_v',
    'HLT_IsoMu22_v',
    'HLT_IsoMu22_eta2p1_v',
    'HLT_IsoMu24_v',
    'HLT_IsoMu27_v',
    'HLT_IsoTkMu18_v',
    'HLT_IsoTkMu20_v',
    'HLT_IsoTkMu22_eta2p1_v',
    'HLT_IsoTkMu22_v',
    'HLT_IsoTkMu24_v',
    'HLT_IsoTkMu27_v',
    'HLT_IsoMu17_eta2p1_LooseIsoPFTau20_SingleL1_v',
    'HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v',
    'HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v',
    'HLT_IsoMu19_eta2p1_LooseIsoPFTau20_v',
    'HLT_IsoMu21_eta2p1_LooseIsoPFTau20_SingleL1_v',
    'HLT_Ele23_WPLoose_Gsf_v',
    'HLT_Ele24_eta2p1_WPLoose_Gsf_v',
    'HLT_Ele25_WPTight_Gsf_v',
    'HLT_Ele25_eta2p1_WPLoose_Gsf_v',
    'HLT_Ele25_eta2p1_WPTight_Gsf_v',
    'HLT_Ele27_WPLoose_Gsf_v',
    'HLT_Ele27_WPTight_Gsf_v',
    'HLT_Ele27_eta2p1_WPLoose_Gsf_v',
    'HLT_Ele27_eta2p1_WPTight_Gsf_v',
    'HLT_Ele32_eta2p1_WPTight_Gsf_v',
    'HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v',
    'HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v',
    'HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v',
    'HLT_Ele27_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v',
    'HLT_Ele32_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v',
    'HLT_DoubleMediumIsoPFTau32_Trk1_eta2p1_Reg_v',
    'HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v',
    'HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v',
    'HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v',
    'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v',
    'HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v',
    'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v',
    'HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v'
]

paths_2016_reduced_muon = [
    'HLT_IsoMu22_v',
    'HLT_IsoMu22_eta2p1_v',
    'HLT_IsoMu24_v',
    'HLT_IsoTkMu22_eta2p1_v',
    'HLT_IsoTkMu22_v',
    'HLT_IsoTkMu24_v',
    'HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v',
    'HLT_IsoMu19_eta2p1_LooseIsoPFTau20_v'
]
paths_2016_reduced_elec = [
    'HLT_Ele25_WPTight_Gsf_v',
    'HLT_Ele25_eta2p1_WPLoose_Gsf_v',
    'HLT_Ele25_eta2p1_WPTight_Gsf_v',
    'HLT_Ele27_WPTight_Gsf_v',
    'HLT_Ele27_eta2p1_WPLoose_Gsf_v',
    'HLT_Ele27_eta2p1_WPTight_Gsf_v',
    'HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v',
    'HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v',
    'HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v',
]
paths_2016_reduced_tau = [
    'HLT_DoubleMediumIsoPFTau32_Trk1_eta2p1_Reg_v',
    'HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v',
]
paths_2016_reduced_emu = [
    'HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v',
    'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v',
    'HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v',
    'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v',
    'HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v'
]

paths_dict = {
    'None':           [],
    'SingleMuon':     (paths_2016_reduced_muon + paths_2016_reduced_tau + paths_2016_reduced_emu),
    'SingleElectron': (paths_2016_reduced_elec + paths_2016_reduced_tau + paths_2016_reduced_emu),
    'Tau':            paths_2016_reduced_tau,
    'MuonEG':         paths_2016_reduced_emu,
    'MCreHLT':        (paths_2016_reduced_muon + paths_2016_reduced_elec + paths_2016_reduced_emu + paths_2016_reduced_tau)
}

hlt_paths = paths_dict[hltPaths]

if hltPaths != 'None':
    process.icTriggerObjectSequence += cms.Sequence(
        process.icTriggerPathProducer
    )


for path in hlt_paths:
    shortname = path[4:-2]  # drop the HLT_ and _v parts
    setattr(process, 'ic_%s_ObjectProducer' % shortname, producers.icTriggerObjectProducer.clone(
        input               = cms.InputTag('selectedPatTrigger'),
        inputTriggerResults = cms.InputTag('TriggerResults', '', 'HLT' if isData else 'HLT2'),
        branch              = cms.string('triggerObjects_%s' % shortname),
        hltPath             = cms.string(path),
        inputIsStandAlone   = cms.bool(True),
        storeOnlyIfFired    = cms.bool(False)
    ))
    process.icTriggerObjectSequence += cms.Sequence(getattr(process, 'ic_%s_ObjectProducer' % shortname))


################################################################
# Gen info
################################################################
process.icGenParticleProducer = producers.icGenParticleProducer.clone(
    input               = cms.InputTag('prunedGenParticles', '', 'PAT'),
    includeMothers      = cms.bool(True),
    includeDaughters    = cms.bool(True),
    includeStatusFlags  = cms.bool(True)
)

process.icPileupInfoProducer = producers.icPileupInfoProducer.clone(
    input = cms.InputTag("slimmedAddPileupInfo")
)

process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
    src = cms.InputTag("slimmedGenJets"),
    cut = cms.string("pt > 10.0")
)

process.icGenJetProducer = producers.icGenJetProducer.clone(
    branch              = cms.string("genJets"),
    input               = cms.InputTag("selectedGenJets"),
    inputGenParticles   = cms.InputTag("genParticles"),
    requestGenParticles = cms.bool(False),
    isSlimmed           = cms.bool(True)
)

process.icGenSequence = cms.Sequence()

if not isData:
    process.icGenSequence += (
        process.icGenParticleProducer+
        process.selectedGenJets+
        process.icGenJetProducer+
        process.icPileupInfoProducer
    )


################################################################
# Event info
################################################################

process.icEventInfoProducer = producers.icEventInfoProducer.clone(
    includeHT           = cms.bool(False),
    lheProducer         = cms.InputTag("externalLHEProducer"),
    includeJetRho       = cms.bool(True),
    inputJetRho         = cms.InputTag("fixedGridRhoFastjetAll"),
    includeLeptonRho    = cms.bool(False),
    inputLeptonRho      = cms.InputTag("fixedGridRhoFastjetAll"),
    includeVertexCount  = cms.bool(True),
    inputVertices       = cms.InputTag("offlineSlimmedPrimaryVertices"),
    includeCSCFilter    = cms.bool(False),
    inputCSCFilter      = cms.InputTag("BeamHaloSummary")
)

# if not isData:
#     process.icEventInfoProducer.includeLHEWeights = cms.bool(True)

process.icEventProducer = producers.icEventProducer.clone()
process.icHashTreeProducer = cms.EDProducer('ICHashTreeProducer')

process.p = cms.Path(
    process.icSelectionSequence+
    process.pfParticleSelectionSequence+
    process.icVertexSequence+
    process.icElectronSequence+
    process.icMuonSequence+
    process.icTauSequence+
    process.icPFJetSequence+
    process.icMetSequence+
    process.icTrackSequence+
    process.icTriggerObjectSequence+
    process.icGenSequence+
    process.icEventInfoProducer+
    process.icEventProducer+
    process.icHashTreeProducer
)

process.schedule = cms.Schedule(process.p)
# print process.dumpPython()
