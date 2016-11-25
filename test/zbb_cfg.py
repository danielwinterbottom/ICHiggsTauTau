import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
process = cms.Process("MAIN")
import sys

################################################################
### Read Options
################################################################
options = VarParsing.VarParsing ('analysis')
options.register ('isData',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, 
                  VarParsing.VarParsing.varType.int, "Process as data?")
options.register ('release',
                  '', # default value
                  VarParsing.VarParsing.multiplicity.singleton, 
                  VarParsing.VarParsing.varType.string, "Release label")
options.parseArguments()
isData      = options.isData
release     = options.release

if isData:
  print 'Setting up for data processing...'
else :
  print 'Setting up for MC processing...'

if not(release == '42X' or release == '53X'):
  print 'Release not recognised, exiting!'
  sys.exit(1)
else:
  print 'Setting up for ' + release + ' release...'


################################################################
### Load some standard sequences and services
################################################################
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

#PAT has a hissy fit if this OutputModule isn't defined before we try and "removeMCMatching"
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('patTuple.root'),
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),             
                               outputCommands = cms.untracked.vstring('drop *', *patEventContent )
                               )


################################################################
### Vertex Modules
################################################################
from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
process.goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
    src=cms.InputTag('offlinePrimaryVertices')
    )

################################################################
### Start settting up the PAT default sequence
################################################################
process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.pfTools import *
from PhysicsTools.PatAlgos.tools.tauTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.trigTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
switchOnTrigger(process, outputModule="")

if (release == '42X'):
  switchToPFTauHPS(process)

################################################################
### Configure PAT Jets
################################################################
addJetCollection(process,
  cms.InputTag('ak5PFJets'),
  'AK5','PF',
  doJTA=True,
  doBTagging=True,
  jetCorrLabel=('AK5PF', ['L1FastJet','L2Relative', 'L3Absolute','L2L3Residual'] if isData else ['L1FastJet','L2Relative', 'L3Absolute']),
  doType1MET=False,
  doL1Cleaning=False,
  doL1Counters=False,
  genJetCollection=cms.InputTag("ak5GenJetsNoNuBSM"),
  doJetID      = True,
  jetIdLabel   = "ak5"
  )
switchJetCollection(process,
  cms.InputTag('ak5CaloJets'),
  doJTA=False,
  doBTagging=False,
  jetCorrLabel=('AK5Calo', ['L1FastJet','L2Relative', 'L3Absolute','L2L3Residual'] if isData else ['L1FastJet','L2Relative', 'L3Absolute']),
  doType1MET=False,
  genJetCollection=cms.InputTag("ak5GenJetsNoNuBSM"),
  doJetID = True,
  jetIdLabel = "ak5"
  )
process.patJets.embedGenPartonMatch = cms.bool(False)
process.patJetsAK5PF.embedGenPartonMatch = cms.bool(False)

################################################################
### Set up METs
################################################################
addPfMET(process, 'PF')

# PAT will try and use type-1 corrected MET by default in 53X, here
# we force it to use the uncorrected pfMet
process.patMETsPF.metSource  = cms.InputTag("pfMet")

if isData:
  #process.pfJetMETcorr.jetCorrLabel = cms.string("ak5PFL1FastL2L3Residual")
  process.patMETsPF.addGenMET = cms.bool(False)
else:
  #process.pfJetMETcorr.jetCorrLabel = cms.string("ak5PFL1FastL2L3")
  process.patMETsPF.addGenMET = cms.bool(True)

################################################################
### Configure kt6PFJets for lepton isolation
################################################################
process.load("JetMETCorrections.METPUSubtraction.mvaPFMET_leptons_42X_cff")

process.load('RecoJets.JetProducers.kt4PFJets_cfi')
process.kt6PFJetsForLeptons = process.kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJetsForLeptons.Rho_EtaMax = cms.double(2.5)


################################################################
### PAT Selections
################################################################
process.selectedPatTaus.cut = 'pt > 18. & abs(eta) < 2.6 & tauID("decayModeFinding") > 0.5'
process.selectedPatJetsAK5PF.cut = 'pt > 15. & abs(eta) < 100.'


################################################################
### Configuration of Electron ID MVA
################################################################
process.load("EgammaAnalysis.ElectronTools.electronIdMVAProducer_cfi")
process.mvaElectronIDSequence = cms.Sequence(process.mvaTrigV0 + process.mvaNonTrigV0)
if release == '53X':
  process.mvaTrigNoIPV0 = cms.EDFilter("ElectronIdMVAProducer",
      verbose = cms.untracked.bool(False),
      vertexTag = cms.InputTag('offlinePrimaryVertices'),
      electronTag = cms.InputTag('gsfElectrons'),
      reducedEBRecHitCollection = cms.InputTag('reducedEcalRecHitsEB'),
      reducedEERecHitCollection = cms.InputTag('reducedEcalRecHitsEE'),
      method = cms.string("BDT"),
      mvaWeightFile = cms.vstring(
        "EgammaAnalysis/ElectronTools/data/Electrons_BDTG_TrigNoIPV0_2012_Cat1.weights.xml",
        "EgammaAnalysis/ElectronTools/data/Electrons_BDTG_TrigNoIPV0_2012_Cat2.weights.xml",
        "EgammaAnalysis/ElectronTools/data/Electrons_BDTG_TrigNoIPV0_2012_Cat3.weights.xml",
        "EgammaAnalysis/ElectronTools/data/Electrons_BDTG_TrigNoIPV0_2012_Cat4.weights.xml",
        "EgammaAnalysis/ElectronTools/data/Electrons_BDTG_TrigNoIPV0_2012_Cat5.weights.xml",
        "EgammaAnalysis/ElectronTools/data/Electrons_BDTG_TrigNoIPV0_2012_Cat6.weights.xml",
        ),
      Trig = cms.bool(True),
      NoIP = cms.bool(True)
      )
  process.mvaElectronIDSequence += process.mvaTrigNoIPV0


################################################################
### Configuration of MVA PU Jet ID
################################################################
if (release == '53X'):
  from RecoJets.JetProducers.PileupJetID_cfi import *
  stdalgos = cms.VPSet(full_53x,cutbased,PhilV1)
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
else:
  from RecoJets.JetProducers.PileupJetID_cfi import *
  stdalgos = cms.VPSet(full,cutbased,PhilV1)
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

################################################################
### Lepton Isolation
################################################################
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

################################################################
### Final PAT config
################################################################
process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")

if isData:
  runOnData(process)

removeSpecificPATObjects(process, ['Electrons', 'Muons', 'Taus'])

removeCleaning(process)
process.pfAllMuons.src = cms.InputTag("particleFlow")

process.patJetsAK5PF.discriminatorSources = cms.VInputTag(
        cms.InputTag("simpleSecondaryVertexHighEffBJetTagsAK5PF"), 
        cms.InputTag("simpleSecondaryVertexHighPurBJetTagsAK5PF"), 
        cms.InputTag("combinedSecondaryVertexBJetTagsAK5PF"), 
       )
if (release == '42X'):
  # process.PFTau = process.recoTauClassicHPSSequence
  from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
  process.kt6PFJets = kt4PFJets.clone(
      rParam = cms.double(0.6),
      doAreaFastjet = cms.bool(True),
      doRhoFastjet = cms.bool(True)
      )
  process.patJetCorrFactorsAK5PF.rho = cms.InputTag("kt6PFJets", "rho")

  
process.btaggingAK5PF = cms.Sequence(
  process.impactParameterTagInfosAK5PF
  +process.secondaryVertexTagInfosAK5PF
  +process.simpleSecondaryVertexHighEffBJetTagsAK5PF
  +process.simpleSecondaryVertexHighPurBJetTagsAK5PF
  +process.combinedSecondaryVertexBJetTagsAK5PF
  )

################################################################
### MC specific config
################################################################
if not isData:
  process.load("RecoJets.Configuration.GenJetParticles_cff")
  process.genParticlesForJets.ignoreParticleIDs = cms.vuint32(
      1000022, 2000012, 2000014,
      2000016, 1000039, 5000039,
      4000012, 9900012, 9900014,
      9900016, 39, 12, 14, 16
      )
  process.load("RecoJets.JetProducers.ak5GenJets_cfi")
  process.ak5GenJetsNoNuBSM  =  process.ak5GenJets.clone();
  ## Configure Parton Jets
  #process.genParticlesForPartonJets = process.genParticlesForJets.clone()
  #process.genParticlesForPartonJets.partonicFinalState = True
  #process.genParticlesForPartonJets.excludeFromResonancePids = cms.vuint32(11, 12, 13, 14, 15, 16)


################################################################
### General Config
################################################################
process.MessageLogger.cerr.FwkReport.reportEvery = 50000
process.MessageLogger.suppressError = cms.untracked.vstring( 'patTrigger','HLTConfigData' )
process.MessageLogger.suppressWarning = cms.untracked.vstring( 'patTrigger','HLTConfigData')

if (release == '42X'):
  if isData:
    process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring('root://eoscms//eos/cms/store/user/agilbert/samples/TauPlusX-2011A-Run166512-42X.root')
    )
    process.GlobalTag.globaltag = cms.string('GR_R_42_V21::All')
  else:
    process.source = cms.Source("PoolSource",
      fileNames = cms.untracked.vstring('root://eoscms//eos/cms/store/user/agilbert/samples/DYJetsToLL-Fall11-42X.root')
    )
    process.GlobalTag.globaltag = cms.string('START42_V15B::All')
if (release == '53X'):
  if isData:
    process.source = cms.Source("PoolSource",
      fileNames = cms.untracked.vstring('root://eoscms//eos/cms/store/user/agilbert/samples/TauPlusX-2012D.root')
    )
    process.GlobalTag.globaltag = cms.string('FT_53_V21_AN4::All')
  else:
    process.source = cms.Source(
      "PoolSource",
      fileNames = cms.untracked.vstring(
        #'file:pickevents.root'
        'root://eoscms//eos/cms/store/user/agilbert/samples/DYJetsToLL-Summer12-53X-Sample.root'
        )
        #'file:/Volumes/Storage/samples/VBF_HToTauTau_M-125-53X.root'
        #'file:/Volumes/Storage/samples/embed_mutau_v1_DYJetsToLL.root'
    )
    process.GlobalTag.globaltag = cms.string('START53_V22::All')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

################################################################
## Configure private modules
################################################################
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

process.icPFJetProducer = cms.EDProducer('ICPFJetProducer',
    inputLabel = cms.InputTag("selectedPatJetsAK5PF"),
    branchName = cms.untracked.string("pfJetsPFlow"),
    RequestSecondaryVertices = cms.InputTag('secondaryVertexTagInfosAK5PF'),
    StoreTrackIds = cms.bool(False)
    )

process.icPfMetProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patMETsPF"),
    branchName = cms.untracked.string("pfMet"),
    addGen = cms.untracked.bool(True),
    InputSig = cms.untracked.string("")
    )
if isData: process.icPfMetProducer.addGen = cms.untracked.bool(False)

process.icVertexProducer = cms.EDProducer('ICVertexProducer',
    inputLabel = cms.InputTag("goodOfflinePrimaryVertices"),
    branchName = cms.untracked.string("vertices"),
    trackPtThreshold = cms.untracked.double(3.0),
    StoreTrackIds = cms.bool(False),
    FirstVertexOnly = cms.bool(True)
    )

process.icSecondaryVertexProducer = cms.EDProducer('ICSecondaryVertexProducer',
    inputLabel = cms.InputTag("secondaryVertexTagInfosAK5PF"),
    branchName = cms.untracked.string("secondaryVertices"),
    trackPtThreshold = cms.untracked.double(0.0),
    StoreTrackIds = cms.bool(True),
    mergeLabels = cms.vstring(
      'icPFJetProducer'
      )
    )

process.icTrackProducer = cms.EDProducer('ICTrackProducer',
    mergeLabels = cms.untracked.vstring(
      'icSecondaryVertexProducer'
      )
    )

process.icGenJetProducer = cms.EDProducer('ICGenJetProducer',
    inputLabel = cms.InputTag("ak5GenJetsNoNuBSM"),
    branchName = cms.untracked.string("genJets"),
    mergeLabels = cms.untracked.vstring(
        ),
    addAll = cms.untracked.bool(True),
    addAllPtCut = cms.untracked.double(15.0),
    addAllEtaCut = cms.untracked.double(100.0),
    storeGenParticles = cms.untracked.bool(False)
)
process.icGenParticleProducer = cms.EDProducer('ICGenParticleProducer',
    branchName = cms.untracked.string("genParticles"),
    inputLabel = cms.InputTag("genParticles","","SIM"),
    mergeLabels = cms.untracked.vstring(
        'icPFJetProducer',
        'icElectronProducer',
        'icMuonProducer'
        ),
    storeMothers = cms.untracked.bool(False),
    storeDaughters = cms.untracked.bool(False),
    addAllStatus1 = cms.untracked.bool(True),
    addAllStatus1Regex = cms.untracked.vstring('11','-11','13','-13'),
    addAllStatus1PtThreshold = cms.untracked.double(5.0),
    addAllStatus2 = cms.untracked.bool(True),
    addAllStatus2Regex = cms.untracked.vstring('5.*','-5.*','4.*','-4.*'),
    addAllStatus2PtThreshold = cms.untracked.double(0.0),
    addAllStatus3 = cms.untracked.bool(True),
    addAllStatus3Regex = cms.untracked.vstring('.*'),
    addAllStatus3PtThreshold = cms.untracked.double(0.0)
)
if release == '42X':
  process.icGenParticleProducer.inputLabel = cms.InputTag("genParticles","","HLT")


process.icPileupInfoProducer = cms.EDProducer('ICPileupInfoProducer')

process.icEventInfoProducer = cms.EDProducer('ICEventInfoProducer',
    jetsRhoLabel = cms.string("kt6PFJets"),
    leptonRhoLabel = cms.string("kt6PFJetsForLeptons"),
    vertexLabel = cms.string('goodOfflinePrimaryVertices')
    )

process.icTriggerPathProducer = cms.EDProducer('ICTriggerPathProducer')

process.icSequence = cms.Sequence()

process.icSequence += cms.Sequence(
  process.icElectronProducer
  +process.icMuonProducer
  +process.icPFJetProducer
  +process.icPfMetProducer
  +process.icVertexProducer
  +process.icSecondaryVertexProducer
  +process.icTrackProducer
  +process.icEventInfoProducer
  )

process.icDataSequence = cms.Sequence(
  process.icTriggerPathProducer
  )
if isData: process.icSequence += process.icDataSequence

process.icMCSequence = cms.Sequence(
  process.icPileupInfoProducer
  +process.icGenJetProducer
  +process.icGenParticleProducer
  )
if not isData: process.icSequence += process.icMCSequence

process.icTriggerSequence = cms.Sequence()
################################################################
## Define 2011 et,mt,em physics triggers for data
################################################################
if (release == '42X' and isData): 
  process.icDoubleMu6ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsDoubleMu6"),
      hltPath = cms.untracked.string("HLT_DoubleMu6_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )
  process.icDoubleMu7ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsDoubleMu7"),
      hltPath = cms.untracked.string("HLT_DoubleMu7_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )
  process.icDoubleMu8ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsMu13Mu8"),
      hltPath = cms.untracked.string("HLT_Mu13_Mu8_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )
  process.icEle17Ele8LObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle17Ele8L"),
      hltPath = cms.untracked.string("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )
  process.icEle17Ele8ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle17Ele8"),
      hltPath = cms.untracked.string("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )
  process.icTriggerSequence += (
    process.icDoubleMu6ObjectProducer
    +process.icDoubleMu7ObjectProducer
    +process.icDoubleMu8ObjectProducer
    +process.icEle17Ele8LObjectProducer
    +process.icEle17Ele8ObjectProducer
    )

process.icSequence += process.icTriggerSequence
 
process.icEventProducer = cms.EDProducer('ICEventProducer')
process.icSequence += process.icEventProducer

process.extra42XSequence = cms.Sequence()
if release == '42X':
  process.ak5PFJets.doAreaFastjet = cms.bool(True)
  process.extra42XSequence += (process.ak5PFJets
                              +process.kt6PFJets)

process.extra53XSequence = cms.Sequence()

process.mcSequence = cms.Sequence()
if not isData: process.mcSequence += (process.genParticlesForJets+process.ak5GenJetsNoNuBSM)

process.p = cms.Path(
  process.mcSequence
  +process.goodOfflinePrimaryVertices
  +process.pfAllMuons
  +process.extra42XSequence
  +process.extra53XSequence
  +process.kt6PFJetsForLeptons
  +process.pfParticleSelectionSequence
  +process.eleIsoSequence
  +process.muIsoSequence
  +process.mvaElectronIDSequence
  +process.patDefaultSequence
  +process.puJetMva
  +process.icSequence
  )

#print process.dumpPython()
