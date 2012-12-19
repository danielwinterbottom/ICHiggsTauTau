import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
process = cms.Process("MAIN")
import sys

options = VarParsing.VarParsing ('analysis')

options.register ('isData',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Process as data?")

options.register ('isEmbedded',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Process as embedded?")

options.register ('release',
                  '', # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "Release label")

options.register ('isTandP',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Tag and probe ntuples?")

options.register ('isZStudy',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Process for Z->ee or Z->mumu?")

options.parseArguments()

isData = options.isData
release = options.release
isEmbedded = options.isEmbedded
isTandP = options.isTandP
isZStudy = options.isZStudy

if isData:
  print 'Setting up for data processing...'
else :
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

## Configure TFileService
process.TFileService = cms.Service("TFileService", 
  fileName = cms.string("EventTree.root"),
  closeFileFast = cms.untracked.bool(True)
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
if not (release == '53X'):
  process.load("Configuration.StandardSequences.Geometry_cff")
else:
  process.load("Configuration.Geometry.GeometryIdeal_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")


from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
process.goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
    src=cms.InputTag('offlinePrimaryVertices')
    )


from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso, setupPFMuonIso

process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons')
process.muIsoSequence = setupPFMuonIso(process, 'pfAllMuons')


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



process.pfAllMuons = cms.EDFilter("PdgIdPFCandidateSelector",     
  src = cms.InputTag("particleFlow"),     
  pdgId = cms.vint32( -13, 13)
  )

process.load("EGamma.EGammaAnalysisTools.electronIdMVAProducer_cfi")
process.mvaElectronIDSequence = cms.Sequence(process.mvaTrigV0 + process.mvaNonTrigV0)

#process.load("RecoTauTag.Configuration.updateHPSPFTaus_cff")
process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")


process.icTauProducer = cms.EDProducer('ICTauProducer',
  input = cms.InputTag("hpsPFTauProducer"),
  branchName = cms.string("taus"),
  minPt = cms.double(18.0),
  maxEta = cms.double(2.5),
  vertexCollection = cms.InputTag("goodOfflinePrimaryVertices"),
  trackCollection = cms.InputTag("generalTracks"),
  storeAndRequestTrackIds = cms.bool(False),
  tauIDs = cms.PSet(
    ### Definites
    byIsolationMVAraw                           = cms.InputTag("hpsPFTauDiscriminationByIsolationMVAraw"),
    byLooseIsolationMVA                         = cms.InputTag("hpsPFTauDiscriminationByLooseIsolationMVA"),
    againstElectronMVA2raw                      = cms.InputTag("hpsPFTauDiscriminationByMVA2rawElectronRejection"),
    againstElectronMVA2category                 = cms.InputTag("hpsPFTauDiscriminationByMVA2rawElectronRejection:category"),
    againstElectronTightMVA2                    = cms.InputTag("hpsPFTauDiscriminationByMVA2TightElectronRejection"),
    byLooseCombinedIsolationDeltaBetaCorr3Hits  = cms.InputTag("hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits"),
    byMediumCombinedIsolationDeltaBetaCorr3Hits = cms.InputTag("hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits"),
    byTightCombinedIsolationDeltaBetaCorr3Hits  = cms.InputTag("hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits"),
    againstElectronMVA3raw                      = cms.InputTag("hpsPFTauDiscriminationByMVA3rawElectronRejection"),
    againstElectronMVA3category                 = cms.InputTag("hpsPFTauDiscriminationByMVA3rawElectronRejection:category"),
    againstElectronLooseMVA3                    = cms.InputTag("hpsPFTauDiscriminationByMVA3LooseElectronRejection"),
    againstElectronMediumMVA3                   = cms.InputTag("hpsPFTauDiscriminationByMVA3MediumElectronRejection"),
    againstElectronTightMVA3                    = cms.InputTag("hpsPFTauDiscriminationByMVA3TightElectronRejection"),
    againstElectronVTightMVA3                   = cms.InputTag("hpsPFTauDiscriminationByMVA3VTightElectronRejection"),
    againstElectronDeadECAL                     = cms.InputTag("hpsPFTauDiscriminationByDeadECALElectronRejection"),
    decayModeFinding                            = cms.InputTag("hpsPFTauDiscriminationByDecayModeFinding"),
    againstElectronLoose                        = cms.InputTag("hpsPFTauDiscriminationByLooseElectronRejection"),
    againstElectronMedium                       = cms.InputTag("hpsPFTauDiscriminationByMediumElectronRejection"),
    againstElectronTight                        = cms.InputTag("hpsPFTauDiscriminationByTightElectronRejection"),
    againstElectronMVA                          = cms.InputTag("hpsPFTauDiscriminationByMVAElectronRejection"),
    againstMuonLoose                            = cms.InputTag("hpsPFTauDiscriminationByLooseMuonRejection"),
    againstMuonMedium                           = cms.InputTag("hpsPFTauDiscriminationByMediumMuonRejection"),
    againstMuonTight                            = cms.InputTag("hpsPFTauDiscriminationByTightMuonRejection")

    ## Maybe
    # byMediumIsolationMVA                        = cms.InputTag("hpsPFTauDiscriminationByMediumIsolationMVA"),
    # byTightIsolationMVA                         = cms.InputTag("hpsPFTauDiscriminationByTightIsolationMVA"),
    # againstElectronVLooseMVA2                   = cms.InputTag("hpsPFTauDiscriminationByMVA2VLooseElectronRejection"),
    # againstElectronLooseMVA2                    = cms.InputTag("hpsPFTauDiscriminationByMVA2LooseElectronRejection"),
    # againstElectronMediumMVA2                   = cms.InputTag("hpsPFTauDiscriminationByMVA2MediumElectronRejection"),
    # byVLooseIsolation                           = cms.InputTag("hpsPFTauDiscriminationByVLooseIsolation"),
    # byLooseIsolation                            = cms.InputTag("hpsPFTauDiscriminationByLooseIsolation"),
    # byMediumIsolation                           = cms.InputTag("hpsPFTauDiscriminationByMediumIsolation"),
    # byTightIsolation                            = cms.InputTag("hpsPFTauDiscriminationByTightIsolation"),
    # byVLooseIsolationDeltaBetaCorr              = cms.InputTag("hpsPFTauDiscriminationByVLooseIsolationDBSumPtCorr"),
    # byLooseIsolationDeltaBetaCorr               = cms.InputTag("hpsPFTauDiscriminationByLooseIsolationDBSumPtCorr"),
    # byMediumIsolationDeltaBetaCorr              = cms.InputTag("hpsPFTauDiscriminationByMediumIsolationDBSumPtCorr"),
    # byTightIsolationDeltaBetaCorr               = cms.InputTag("hpsPFTauDiscriminationByTightIsolationDBSumPtCorr"),
    # byVLooseCombinedIsolationDeltaBetaCorr      = cms.InputTag("hpsPFTauDiscriminationByVLooseCombinedIsolationDBSumPtCorr"),
    # byLooseCombinedIsolationDeltaBetaCorr       = cms.InputTag("hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr"),
    # byMediumCombinedIsolationDeltaBetaCorr      = cms.InputTag("hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr"),
    # byTightCombinedIsolationDeltaBetaCorr       = cms.InputTag("hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr"),
    # byCombinedIsolationDeltaBetaCorrRaw         = cms.InputTag("hpsPFTauDiscriminationByRawCombinedIsolationDBSumPtCorr"),

    # No
    )
)

process.icElectronProducer = cms.EDProducer('ICElectronProducer',
    input = cms.InputTag("gsfElectrons"),
    branchName = cms.string("electrons"),
    minPt = cms.double(9.5),
    maxEta = cms.double(2.6),
    vertexCollection = cms.InputTag("goodOfflinePrimaryVertices"),
    pfIsoPostfix = cms.string("PFIso"),
    isPF = cms.bool(False)
)

process.icMuonProducer = cms.EDProducer('ICMuonProducer',
    input = cms.InputTag("pfAllMuons"),
    branchName = cms.string("muonsPFlow"),
    minPt = cms.double(3.0),
    maxEta = cms.double(2.6),
    vertexCollection = cms.InputTag("goodOfflinePrimaryVertices"),
    pfIsoPostfix = cms.string("PFIso"),
    isPF = cms.bool(True)
)


process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByMediumIsolationMVA)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByTightIsolationMVA)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByMVA2VLooseElectronRejection)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByMVA2LooseElectronRejection)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByMVA2MediumElectronRejection)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByVLooseIsolation)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByLooseIsolation)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByMediumIsolation)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByTightIsolation)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByVLooseIsolationDBSumPtCorr)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByLooseIsolationDBSumPtCorr)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByMediumIsolationDBSumPtCorr)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByTightIsolationDBSumPtCorr)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByVLooseCombinedIsolationDBSumPtCorr)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr)
process.recoTauClassicHPSSequence.remove(process.hpsPFTauDiscriminationByRawCombinedIsolationDBSumPtCorr)


process.patTriggerSequence = cms.Sequence()

from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger(process, sequence = 'patTriggerSequence', hltProcess = '*', outputModule = '')




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

# process.load("RecoJets.JetProducers.PileupJetID_cfi")

process.puJetMva = cms.EDProducer('PileupJetIdProducer',
                         produceJetIds = cms.bool(True),
                         jetids = cms.InputTag(""),
                         runMvas = cms.bool(True),
                         jets = cms.InputTag("ak5PFJets"),
                         vertexes = cms.InputTag("offlinePrimaryVertices"),
                         algos = cms.VPSet(stdalgos),
                         rho     = cms.InputTag("kt6PFJets","rho"),
                         jec     = cms.string("AK5PF"),
                         applyJec = cms.bool(True),
                         inputIsCorrected = cms.bool(False),                                     
                         residualsFromTxt = cms.bool(False),
                         residualsTxt     = cms.FileInPath("RecoJets/JetProducers/data/dummy.txt"),
)


process.icEventProducer = cms.EDProducer('ICEventProducer')


process.p = cms.Path(
  process.goodOfflinePrimaryVertices
  +process.pfAllMuons
  +process.pfParticleSelectionSequence
  +process.eleIsoSequence
  +process.muIsoSequence
  +process.mvaElectronIDSequence
  +process.recoTauClassicHPSSequence
  +process.patTriggerSequence
  +process.btaggingSequenceAK5PF
  +process.puJetMva
  # +process.hpsPFTauDiscriminationByDecayModeFinding
  # +process.kt6PFJetsForRhoComputationVoronoi
  # +process.hpsPFTauDiscriminationByIsolationMVAraw
  # +process.hpsPFTauDiscriminationByLooseIsolationMVA
  # +process.hpsPFTauDiscriminationByMediumIsolationMVA
  # +process.hpsPFTauDiscriminationByTightIsolationMVA
  # +process.hpsPFTauDiscriminationByVLooseCombinedIsolationDBSumPtCorr
  # +process.hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr
  # +process.hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr
  # +process.hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr
  # +process.hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits
  # +process.hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits
  # +process.hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits
  # +process.hpsPFTauDiscriminationByLooseElectronRejection
  # +process.hpsPFTauDiscriminationByMediumElectronRejection
  # +process.hpsPFTauDiscriminationByTightElectronRejection
  # +process.hpsPFTauDiscriminationByMVAElectronRejection
  # +process.hpsPFTauDiscriminationByMVA2rawElectronRejection
  # +process.hpsPFTauDiscriminationByMVA2LooseElectronRejection
  # +process.hpsPFTauDiscriminationByMVA2VLooseElectronRejection
  # +process.hpsPFTauDiscriminationByMVA2MediumElectronRejection
  # +process.hpsPFTauDiscriminationByMVA2TightElectronRejection
  # +process.hpsPFTauDiscriminationByLooseMuonRejection
  # +process.hpsPFTauDiscriminationByMediumMuonRejection
  # +process.hpsPFTauDiscriminationByTightMuonRejection
  # +process.hpsPFTauDiscriminationByMVA3rawElectronRejection
  # +process.hpsPFTauDiscriminationByMVA3LooseElectronRejection
  # +process.hpsPFTauDiscriminationByMVA3VTightElectronRejection
  # +process.hpsPFTauDiscriminationByMVA3MediumElectronRejection
  # +process.hpsPFTauDiscriminationByMVA3TightElectronRejection
  # +process.hpsPFTauDiscriminationByDeadECALElectronRejection
  +process.icElectronProducer
  +process.icMuonProducer
  +process.icTauProducer
  +process.icEventProducer

 )

################################################################
### General Config
################################################################
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.suppressError = cms.untracked.vstring( 'patTrigger',
      'HLTConfigData')
process.MessageLogger.suppressWarning = cms.untracked.vstring( 'patTrigger',
      'HLTConfigData')
## Input source
if (release == '42X'):
  if isData:
    process.source = cms.Source(
      "PoolSource",
      fileNames = cms.untracked.vstring(
        'file:/Volumes/Storage/samples/TauPlusX-2011A-Run166512-42X.root'
      )
    )
    process.GlobalTag.globaltag = cms.string('GR_R_42_V25::All')
  else:
    process.source = cms.Source(
      "PoolSource",
      fileNames = cms.untracked.vstring(
        'file:/Volumes/Storage/samples/NEW_SYNC_2011.root'
      )
    )
    process.GlobalTag.globaltag = cms.string('START42_V17::All')
if (release == '53X'):
  if isData:
    process.source = cms.Source(
      "PoolSource",
      fileNames = cms.untracked.vstring(
        #'file:/Volumes/Storage/samples/TauPlusX-2012C-PromptReco-v1-Sample.root'
        #'file:/Volumes/Storage/samples/DoubleMu-2012C-24Aug2012-v1-Sample.root'
        'file:/Volumes/Storage/samples/TauPlusX-2012D.root'
      )
    )
    process.GlobalTag.globaltag = cms.string('GR_P_V42_AN2::All')
  else:
    process.source = cms.Source(
      "PoolSource",
      fileNames = cms.untracked.vstring(
        'file:/Volumes/Storage/samples/VBF_HToTauTau_M-125-53X.root'
        #'file:/Volumes/Storage/samples/DYJetsToLL-Summer12-53X-Sample.root'
        #'file:/Volumes/Storage/samples/embed_mutau_v1_DYJetsToLL.root'
      )
    )
    process.GlobalTag.globaltag = cms.string('START53_V10::All')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )



# print process.dumpPython()
