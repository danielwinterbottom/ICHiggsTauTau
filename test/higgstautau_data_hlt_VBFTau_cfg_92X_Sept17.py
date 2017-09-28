import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')

opts.register('file', 'root://xrootd.unl.edu//store/data/Run2017D/Tau/MINIAOD/PromptReco-v1/000/302/663/00000/50B7E026-639A-E711-A072-02163E0123DF.root', parser.VarParsing.multiplicity.singleton,
parser.VarParsing.varType.string, "input file")
opts.register('globalTag', '92X_dataRun2_Prompt_v9', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('isData', 1, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as data?")
opts.register('release', '92XMINIAOD', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")
opts.register('LHEWeights', False, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "Produce LHE weights for sample")
opts.register('LHETag', 'externalLHEProducer', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Input tag for LHE weights")
opts.register('doHT', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Store HT and number of outgoing partons?")


opts.parseArguments()
infile      = opts.file
if not infile: infile = "file:/tmp/file.root"
isData      = opts.isData
tag         = opts.globalTag
release     = opts.release
doLHEWeights = opts.LHEWeights
if not isData:
  doHT     = opts.doHT
else: doHT = 0


if not release in ["92XMINIAOD"]:
  print 'Release not recognised, exiting!'
  sys.exit(1)
print 'release     : '+release
print 'isData      : '+str(isData)
print 'globalTag   : '+str(tag)
print 'doHT        : '+str(doHT)


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
  input = cms.untracked.int32(2000)
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

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(infile))
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

################################################################
# Object Selection
################################################################

from RecoTauTag.RecoTau.TauDiscriminatorTools import noPrediscriminants
process.load('RecoTauTag.Configuration.loadRecoTauTagMVAsFromPrepDB_cfi')
from RecoTauTag.RecoTau.PATTauDiscriminationByMVAIsolationRun2_cff import *
from RecoTauTag.RecoTau.PATTauDiscriminationAgainstElectronMVA6_cfi import *

process.rerunDiscriminationByIsolationMVArun2v1raw = patDiscriminationByIsolationMVArun2v1raw.clone(
   PATTauProducer = cms.InputTag('slimmedTaus'),
   Prediscriminants = noPrediscriminants,
   loadMVAfromDB = cms.bool(True),
   mvaName = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1"), # name of the training you want to use
   mvaOpt = cms.string("DBoldDMwLT"), # option you want to use for your training (i.e., which variables are used to compute the BDT score)
   requireDecayMode = cms.bool(True),
   verbosity = cms.int32(0)
)

process.rerunDiscriminationByIsolationMVArun2v1VLoose = patDiscriminationByIsolationMVArun2v1VLoose.clone(
   PATTauProducer = cms.InputTag('slimmedTaus'),
   Prediscriminants = noPrediscriminants,
   toMultiplex = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1raw'),
   key = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1raw:category'),
   loadMVAfromDB = cms.bool(True),
   mvaOutput_normalization = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1_mvaOutput_normalization"), # normalization fo the training you want to use
   mapping = cms.VPSet(
      cms.PSet(
         category = cms.uint32(0),
         cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1_WPEff90"), # this is the name of the working point you want to use
         variable = cms.string("pt"),
      )
   )
)

# here we produce all the other working points for the training
process.rerunDiscriminationByIsolationMVArun2v1Loose = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1Loose.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1_WPEff80")
process.rerunDiscriminationByIsolationMVArun2v1Medium = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1Medium.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1_WPEff70")
process.rerunDiscriminationByIsolationMVArun2v1Tight = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1Tight.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1_WPEff60")
process.rerunDiscriminationByIsolationMVArun2v1VTight = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1VTight.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1_WPEff50")
process.rerunDiscriminationByIsolationMVArun2v1VVTight = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1VVTight.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1_WPEff40")

# this sequence has to be included in your cms.Path() before your analyzer which accesses the new variables is called.
process.rerunMvaIsolation2SeqRun2 = cms.Sequence(
   process.rerunDiscriminationByIsolationMVArun2v1raw
   *process.rerunDiscriminationByIsolationMVArun2v1VLoose
   *process.rerunDiscriminationByIsolationMVArun2v1Loose
   *process.rerunDiscriminationByIsolationMVArun2v1Medium
   *process.rerunDiscriminationByIsolationMVArun2v1Tight
   *process.rerunDiscriminationByIsolationMVArun2v1VTight
   *process.rerunDiscriminationByIsolationMVArun2v1VVTight
)


process.rerunDiscriminationAgainstElectronMVA6 = patTauDiscriminationAgainstElectronMVA6.clone(
   PATTauProducer = cms.InputTag('slimmedTaus'),
   Prediscriminants = noPrediscriminants,
   loadMVAfromDB = cms.bool(True),
   returnMVA = cms.bool(True),
   method = cms.string("BDTG"),
   mvaName_NoEleMatch_woGwoGSF_BL = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_NoEleMatch_woGwoGSF_BL"),
   mvaName_NoEleMatch_wGwoGSF_BL = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_NoEleMatch_wGwoGSF_BL"),
   mvaName_woGwGSF_BL = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_woGwGSF_BL"),
   mvaName_wGwGSF_BL = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_wGwGSF_BL"),
   mvaName_NoEleMatch_woGwoGSF_EC = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_NoEleMatch_woGwoGSF_EC"),
   mvaName_NoEleMatch_wGwoGSF_EC = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_NoEleMatch_wGwoGSF_EC"),
   mvaName_woGwGSF_EC = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_woGwGSF_EC"),
   mvaName_wGwGSF_EC = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_wGwGSF_EC"),
   minMVANoEleMatchWOgWOgsfBL = cms.double(0.0),
   minMVANoEleMatchWgWOgsfBL  = cms.double(0.0),
   minMVAWOgWgsfBL            = cms.double(0.0),
   minMVAWgWgsfBL             = cms.double(0.0),
   minMVANoEleMatchWOgWOgsfEC = cms.double(0.0),
   minMVANoEleMatchWgWOgsfEC  = cms.double(0.0),
   minMVAWOgWgsfEC            = cms.double(0.0),
   minMVAWgWgsfEC             = cms.double(0.0),
   srcElectrons = cms.InputTag('slimmedElectrons')
)

# embed new id's into new tau collection
embedID = cms.EDProducer("PATTauIDEmbedder",
   src = cms.InputTag('slimmedTaus'),
   tauIDSources = cms.PSet(
      byIsolationMVArun2v1DBoldDMwLTrawNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1raw'),
      byVLooseIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1VLoose'),
      byLooseIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1Loose'),
      byMediumIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1Medium'),
      byTightIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1Tight'),
      byVTightIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1VTight'),
      byVVTightIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1VVTight'),
      againstElectronMVA6RawNew = cms.InputTag('rerunDiscriminationAgainstElectronMVA6')
      ),
   )
setattr(process, "NewTauIDsEmbedded", embedID)

process.selectedElectrons = cms.EDFilter("PATElectronRefSelector",
    src = cms.InputTag("slimmedElectrons"),
    cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
    )
process.selectedMuons = cms.EDFilter("PATMuonRefSelector",
    src = cms.InputTag("slimmedMuons"),
    cut = cms.string("pt > 3 & abs(eta) < 2.6")
    )
process.selectedTaus = cms.EDFilter("PATTauRefSelector",
    src = cms.InputTag("NewTauIDsEmbedded"),
    cut = cms.string('pt > 18.0 & abs(eta) < 2.6 & tauID("decayModeFindingNewDMs") > 0.5')
    )


process.icSelectionSequence = cms.Sequence()

process.icSelectionSequence += cms.Sequence(
  process.rerunMvaIsolation2SeqRun2+
  process.rerunDiscriminationAgainstElectronMVA6+
  getattr(process, "NewTauIDsEmbedded")
)


process.icSelectionSequence += cms.Sequence(
  process.selectedElectrons+
  process.selectedMuons+
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


process.icVertexSequence = cms.Sequence(
  process.icVertexProducer+
  process.icGenVertexProducer
)

if isData :
  process.icVertexSequence.remove(process.icGenVertexProducer)

################################################################
# Electrons
################################################################
electronLabel = cms.InputTag("slimmedElectrons")

process.icElectronSequence = cms.Sequence()

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
  includeFloats = cms.PSet(
     mvaNonTrigSpring15    = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Values"),
     mvaTrigSpring15       = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15Trig25nsV1Values"),
     generalPurposeMVASpring16  = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring16GeneralPurposeV1Values")
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
process.muPFIsoDepositChargedAll.src  = muons
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
  requestTracks           = cms.bool(False),
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

process.icTauProducer = cms.EDProducer("ICPFTauFromPatProducer",
  branch                  = cms.string("taus"),
  input                   = cms.InputTag("selectedTaus"),
  inputVertices           = vtxLabel,
  includeVertexIP         = cms.bool(True),
  requestTracks           = cms.bool(False),
  includeTotalCharged     = cms.bool(False),
  totalChargedLabel       = cms.string('totalCharged'),
  requestPFCandidates     = cms.bool(False),
  inputPFCandidates       = cms.InputTag("packedPFCandidates"),
  isSlimmed               = cms.bool(True),
  tauIDs = cms.PSet()
)


process.icTauSequence = cms.Sequence(
  process.icTauProducer
)


# ################################################################
# # Jets
# ################################################################
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection

#rebuild ak4 chs jets as in  https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD#Jets
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')

#Reapply JECs:
if not isData:
  updateJetCollection(
    process,
    jetSource = cms.InputTag("slimmedJets"),
    labelName = "UpdatedJEC",
    jetCorrections = ("AK4PFchs", cms.vstring(['L1FastJet','L2Relative','L3Absolute']), 'None')
  )
else:
 updateJetCollection(
   process,
   jetSource = cms.InputTag("slimmedJets"),
   labelName = "UpdatedJEC",
   jetCorrections = ("AK4PFchs", cms.vstring(['L1FastJet','L2Relative','L3Absolute','L2L3Residual']), 'None')
  )

process.selectedSlimmedJetsAK4 = cms.EDFilter("PATJetRefSelector",
    src = cms.InputTag("selectedUpdatedPatJetsUpdatedJEC"),
    cut = cms.string("pt > 15")
    )

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
if isData: pfJECS = cms.PSet(
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
 # Recalculated puJetId isn't the same as miniaod stored - should investigate
#stdalgos = cms.VPSet()
#from RecoJets.JetProducers.PileupJetIDParams_cfi import *
#stdalgos = cms.VPSet(full_5x_chs,cutbased)
process.load('RecoJets.JetProducers.PileupJetID_cfi')

process.pileupJetIdCalculator.vertexes = cms.InputTag("offlineSlimmedPrimaryVertices")
process.pileupJetIdCalculator.jets = cms.InputTag("ak4PFJetsCHS")
process.pileupJetIdCalculator.rho = cms.InputTag("fixedGridRhoFastjetAll")
process.pileupJetIdEvaluator.jets = cms.InputTag("ak4PFJetsCHS")
process.pileupJetIdEvaluator.rho = cms.InputTag("fixedGridRhoFastjetAll")


process.icPFJetProducerFromPat = producers.icPFJetFromPatProducer.clone(
    branch                    = cms.string("ak4PFJetsCHS"),
    input                     = cms.InputTag("selectedSlimmedJetsAK4"),
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

process.icPFJetSequence = cms.Sequence()


process.icPFJetSequence += cms.Sequence(
   process.patJetCorrFactorsUpdatedJEC+
   process.updatedPatJetsUpdatedJEC+
   process.selectedUpdatedPatJetsUpdatedJEC+
   process.selectedSlimmedJetsAK4+
   #process.unpackedTracksAndVertices+
   process.icPFJetProducerFromPat
   )

################################################################
# PF MET
################################################################
process.load('JetMETCorrections.Configuration.JetCorrectors_cff')
process.load("RecoJets.JetProducers.ak4PFJets_cfi")

from RecoMET.METProducers.PFMET_cfi import pfMet
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
runMetCorAndUncFromMiniAOD(process,
                           isData=bool(isData),
                           )

process.icPfMetProducer = producers.icMetFromPatProducer.clone(
                         branch = cms.string("pfMetFromSlimmed"),
                         getUncorrectedMet=cms.bool(False)
                         )

process.icPfMetSequence = cms.Sequence(
  process.icPfMetProducer
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
    "drop  status > 30 && status < 70 ",                    #remove pythia8 garbage
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
  input=cms.InputTag("prunedGenParticles","","PAT"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True),
  includeStatusFlags = cms.bool(True)
)

process.icGenParticleProducerFromLHEParticles = producers.icGenParticleFromLHEParticlesProducer.clone()

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

process.ak4GenJetsNoNuBSM.src=cms.InputTag("packedGenParticles")

process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
  src = cms.InputTag("ak4GenJetsNoNuBSM"),
  cut = cms.string("pt > 10.0")
)

process.icGenJetProducer = producers.icGenJetProducer.clone(
  branch = cms.string("genJetsReclustered"),
  inputGenParticles = cms.InputTag("prunedGenParticles","","PAT"),
  isSlimmed  = cms.bool(True)
)
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
    process.icGenParticleProducer+
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

# ################################################################
# # Trigger
# ################################################################

from PhysicsTools.PatAlgos.tools.trigTools import *
process.icTriggerSequence = cms.Sequence()
process.icTriggerObjectSequence = cms.Sequence()


process.patTriggerPath = cms.Path()

switchOnTrigger(process, path = 'patTriggerPath',  outputModule = '')


process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
 branch = cms.string("triggerPaths"),
 input  = cms.InputTag("TriggerResults","","HLT"),
 inputIsStandAlone = cms.bool(True),
 inputPrescales = cms.InputTag("patTrigger")
)

if isData:
  process.icTriggerSequence += cms.Sequence(
   process.icTriggerPathProducer
  )

process.icVBFDoubleLooseChargedIsoPFTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsVBFDoubleLooseChargedIsoPFTau20"),
      hltPath = cms.string("HLT_VBF_DoubleLooseChargedIsoPFTau20_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(True)
      )
process.icVBFDoubleMediumChargedIsoPFTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsVBFDoubleMediumChargedIsoPFTau20"),
      hltPath = cms.string("HLT_VBF_DoubleMediumChargedIsoPFTau20_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(True)
      )
process.icVBFDoubleTightChargedIsoPFTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsVBFDoubleTightChargedIsoPFTau20"),
      hltPath = cms.string("HLT_VBF_DoubleTightChargedIsoPFTau20_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(True)
      )
process.icDoubleMediumChargedIsoPFTau35ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsDoubleMediumChargedIsoPFTau35"),
      hltPath = cms.string("HLT_DoubleMediumChargedIsoPFTau35_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(True)
      )

process.icTriggerObjectSequence += cms.Sequence(
    process.icVBFDoubleLooseChargedIsoPFTau20ObjectProducer +
    process.icVBFDoubleMediumChargedIsoPFTau20ObjectProducer +
    process.icVBFDoubleTightChargedIsoPFTau20ObjectProducer +
    process.icDoubleMediumChargedIsoPFTau35ObjectProducer
    )


## Need to unpack filterLabels on slimmedPatTrigger then make selectedPatTrigger
process.patTriggerUnpacker = cms.EDProducer("PATTriggerObjectStandAloneUnpacker",
   patTriggerObjectsStandAlone = cms.InputTag("slimmedPatTrigger"),
   triggerResults = cms.InputTag("TriggerResults", "", "HLT"),
   unpackFilterLabels = cms.bool(True)
)
process.selectedPatTrigger = cms.EDFilter(
 'PATTriggerObjectStandAloneSelector',
  cut = cms.string('!filterLabels.empty()'),
  src = cms.InputTag('patTriggerUnpacker')
)
process.icTriggerSequence += cms.Sequence(
    process.patTriggerUnpacker +
    process.selectedPatTrigger
    )

for name in process.icTriggerObjectSequence.moduleNames():
  mod = getattr(process, name)
  mod.inputIsStandAlone = cms.bool(True)
  mod.input = cms.InputTag("selectedPatTrigger")


################################################################
# EventInfo
################################################################
#Load the MET filters here
process.load('RecoMET.METFilters.BadPFMuonFilter_cfi')
process.load('RecoMET.METFilters.BadChargedCandidateFilter_cfi')
process.load('RecoMET.METFilters.badGlobalMuonTaggersMiniAOD_cff')
process.BadPFMuonFilter.muons = cms.InputTag("slimmedMuons")
process.BadPFMuonFilter.PFCandidates = cms.InputTag("packedPFCandidates")
process.BadPFMuonFilter.taggingMode = cms.bool(True)
process.BadChargedCandidateFilter.muons = cms.InputTag("slimmedMuons")
process.BadChargedCandidateFilter.PFCandidates = cms.InputTag("packedPFCandidates")
process.BadChargedCandidateFilter.taggingMode = cms.bool(True)

if opts.LHETag: lheTag = opts.LHETag
else: lheTag = 'externalLHEProducer'

process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  includeJetRho       = cms.bool(True),
  includeLHEWeights   = cms.bool(doLHEWeights),
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
  filters             = cms.PSet(
   badChargedHadronFilter  = cms.InputTag("BadChargedCandidateFilter"),
   badMuonFilter          = cms.InputTag("BadPFMuonFilter"),
  ),
  filtersfromtrig     = cms.vstring("Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_goodVertices","Flag_eeBadScFilter","Flag_globalTightHalo2016Filter")
)

if isData:
  process.icEventInfoProducer.filters=cms.PSet(
    badChargedHadronFilter = cms.InputTag("BadChargedCandidateFilter"),
    badMuonFilter = cms.InputTag("BadPFMuonFilter")
  )
  process.icEventInfoProducer.filtersfromtrig = cms.vstring("!Flag_badMuons","!Flag_duplicateMuons","Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_goodVertices","Flag_eeBadScFilter","Flag_globalTightHalo2016Filter")


process.icEventInfoSequence = cms.Sequence(
  process.BadPFMuonFilter+
  process.BadChargedCandidateFilter+
  process.icEventInfoProducer
)


################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()


process.p = cms.Path(
  process.icSelectionSequence+
  process.pfParticleSelectionSequence+
  process.icVertexSequence+
  process.icElectronSequence+
  process.icMuonSequence+
  process.icTauSequence+
  process.icPFJetSequence+
  process.icPfMetSequence+
  process.icGenSequence+
  process.icTriggerSequence+
  process.icTriggerObjectSequence+
  process.icEventInfoSequence+
  process.icEventProducer
)


process.schedule = cms.Schedule(process.p)

