import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
process = cms.Process("MAIN")
import sys

ak5dBetaCorrection=0.0772/0.1687
dBetaCorrection=ak5dBetaCorrection



################################################################
### Read Options
################################################################
options = VarParsing.VarParsing ('analysis')
options.register ('isData',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, 
                  VarParsing.VarParsing.varType.int, "Process as data?")
options.register ('isEmbedded',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, 
                  VarParsing.VarParsing.varType.int, "Process as embedded?")
options.register ('release',
                  '53X', # default value
                  VarParsing.VarParsing.multiplicity.singleton, 
                  VarParsing.VarParsing.varType.string, "Release label")
options.register ('isTandP',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, 
                  VarParsing.VarParsing.varType.int, "Tag and probe ntuples?")
options.register ('isZStudy',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, 
                  VarParsing.VarParsing.varType.int, "Process for Z->ee or Z->mumu?")
options.parseArguments()
isData      = options.isData
release     = options.release
isEmbedded  = options.isEmbedded
isTandP     = options.isTandP
isZStudy    = options.isZStudy

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


################################################################
### Load some standard sequences and services
################################################################
process.TFileService = cms.Service("TFileService", 
  fileName = cms.string("EventTree_diff.root"),
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
#addPfMET(process, 'PF')

# PAT will try and use type-1 corrected MET by default in 53X, here
# we force it to use the uncorrected pfMet
#process.patMETsPF.metSource  = cms.InputTag("pfMet")

#if isData:
  #process.pfJetMETcorr.jetCorrLabel = cms.string("ak5PFL1FastL2L3Residual")
#  process.patMETsPF.addGenMET = cms.bool(False)
#else:
  #process.pfJetMETcorr.jetCorrLabel = cms.string("ak5PFL1FastL2L3")
#  process.patMETsPF.addGenMET = cms.bool(True)

################################################################
### Configure kt6PFJets for lepton isolation
################################################################
process.load('RecoJets.JetProducers.kt4PFJets_cfi')
process.kt6PFJetsForLeptons = process.kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJetsForLeptons.Rho_EtaMax = cms.double(2.5)


################################################################
### PAT Selections
################################################################
process.selectedPatTaus.cut = 'pt > 18. & abs(eta) < 2.6 & tauID("decayModeFinding") > 0.5'
#process.selectedPatTaus.cut = 'pt>18.&abs(eta)<2.6'
process.selectedPatJetsAK5PF.cut = 'pt > 15. & abs(eta) < 100.'


################################################################
### Configuration of Electron ID MVA
################################################################
'''process.load("EgammaAnalysis.ElectronTools.electronIdMVAProducer_cfi")
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


'''
################################################################
### Additional collections for soft lepton + MET
################################################################
process.icSoftLeptonSequence = cms.Sequence( )
process.load("UserCode.ICHiggsTauTau.emulateHLTPFTau_cff")

process.icHLTTauProducer = cms.EDProducer('ICPFTauProducer',
    input = cms.InputTag("hltPFTaus"),
    branchName = cms.string("hltTaus"),
    vertexCollection = cms.InputTag("goodOfflinePrimaryVertices"),
    trackCollection = cms.InputTag("generalTracks"),
    storeAndRequestTrackIds = cms.bool(False),
    tauIDs = cms.PSet(
        decayModeFinding = cms.InputTag("hltPFTauTrackFindingDiscriminator"),
        byIsolation = cms.InputTag("hltPFTauLooseIsolationDiscriminator")
        ),
    tauIDCuts = cms.PSet(
        hltPFTauTrackFindingDiscriminator = cms.double(0.5),
        hltPFTauLooseIsolationDiscriminator = cms.double(0.5)
        ),
    minPt = cms.double(19),
    maxEta = cms.double(2.6),
    requireDecayMode = cms.bool(False)
    )
process.hltPFTauSequence = cms.Sequence(
    process.hltAK5PFJetTracksAssociatorAtVertex+
    process.hltPFTauTagInfo+process.hltPFTaus+
    process.hltPFTauTrackFindingDiscriminator+
    process.hltPFTauLooseIsolationDiscriminator
    )

'''
process.icL1ExtraMETProducer = cms.EDProducer('ICL1ExtraEtMissProducer',
  branchName = cms.untracked.string("l1extraMET"),
  inputLabel = cms.InputTag("l1extraParticles","MET","RECO"),
  minPt = cms.double(0.0),
  maxEta = cms.double(999.0)
  )

process.icL1ExtraMuonsProducer = cms.EDProducer('ICL1ExtraMuonProducer',
  branchName = cms.untracked.string("l1extraMuons"),
  inputLabel = cms.InputTag("l1extraParticles","","RECO"),
  minPt = cms.double(6.0),
  maxEta = cms.double(2.2)
  )

process.icL1ExtraEmIsolatedProducer = cms.EDProducer('ICL1ExtraEmParticleProducer',
  branchName = cms.untracked.string("l1extraEmIsolated"),
  inputLabel = cms.InputTag("l1extraParticles","Isolated","RECO"),
  minPt = cms.double(11.0),
  maxEta = cms.double(2.2)
  )
'''
## Only add HLT tau simulation in MC
# if not isData:
#   process.icSoftLeptonSequence += cms.Sequence(
#       process.hltPFTauSequence
#       +process.icHLTTauProducer
#       )

'''
process.patmetNoHF = process.patMETs.clone(
    metSource = cms.InputTag('metNoHF'),
    addMuonCorrections = cms.bool(False),
    genMETSource = cms.InputTag('genMetTrue'),
    addGenMET = cms.bool(False)
    )

process.patmetNoHFresidualCorrected = process.patMETs.clone(
    metSource = cms.InputTag('metNoHFresidualCorrected'),
    addMuonCorrections = cms.bool(False),
    genMETSource = cms.InputTag('genMetTrue'),
    addGenMET = cms.bool(False)
    )

process.load("UserCode.ICHiggsTauTau.sumCaloTowersInEtaSlices_cfi")

process.icMetNoHFProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patmetNoHF"),
    branchName = cms.untracked.string("metNoHF"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )

process.icMetNoHFCorrectedProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patmetNoHFresidualCorrected"),
    branchName = cms.untracked.string("metNoHFCorrected"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )

process.icSoftLeptonSequence += cms.Sequence(
    process.icL1ExtraMETProducer
    #+process.icL1ExtraMuonsProducer
    #+process.icL1ExtraEmIsolatedProducer
#    +process.patmetNoHF
    +process.sumCaloTowersInEtaSlicesNoHF
#    +process.metNoHFresidualCorrected
#    +process.patmetNoHFresidualCorrected
#    +process.icMetNoHFProducer
#    +process.icMetNoHFCorrectedProducer
    )
'''

################################################################
### Configuration of MVA MET
################################################################
'''
if (release == '53X'):
  process.load("JetMETCorrections.METPUSubtraction.mvaPFMET_leptons_cff")
  from UserCode.ICHiggsTauTau.mvaPFMET_cff_leptons_53X_Dec2012 import mvaMetPairs
else:
  process.load("JetMETCorrections.METPUSubtraction.mvaPFMET_leptons_42X_cff")
  from UserCode.ICHiggsTauTau.mvaPFMET_cff_leptons_42X_Dec2012 import mvaMetPairs
  process.pfMEtMVA.srcLeptons = cms.VInputTag("isomuons","isoelectrons","isotaus")
  process.pfMEtMVA.useOld42  = cms.bool(False)

process.mvaMetPairsMT = mvaMetPairs.clone(
  srcLeg1 = cms.InputTag('pfAllMuons'),
  srcLeg2 = cms.InputTag('selectedPatTaus'),
  leg1Pt = cms.double(7.0), 
  leg1Eta = cms.double(2.6),
  leg2Pt = cms.double(18.0),
  leg2Eta = cms.double(2.6),
  minDeltaR = cms.double(0.49)
  )
#process.mvaMetPairsMT.verbosity = cms.int32(1)
#process.pfMEtMVA.verbosity = cms.int32(0)

process.mvaMetPairsET = mvaMetPairs.clone(
  srcLeg1 = cms.InputTag('gsfElectrons'),
  srcLeg2 = cms.InputTag('selectedPatTaus'),
  leg1Pt = cms.double(10.0),
  leg1Eta = cms.double(2.6),
  leg2Pt = cms.double(18.0),
  leg2Eta = cms.double(2.6),
  minDeltaR = cms.double(0.49)
  )
process.mvaMetPairsEM = mvaMetPairs.clone(
  srcLeg1 = cms.InputTag('gsfElectrons'),
  srcLeg2 = cms.InputTag('pfAllMuons'),
  leg1Pt = cms.double(9.5),
  leg1Eta = cms.double(2.6),
  leg2Pt = cms.double(9.5),
  leg2Eta = cms.double(2.6),
  minDeltaR = cms.double(0.29)
  )
process.mvaMetPairsMM = mvaMetPairs.clone(
  srcLeg1 = cms.InputTag('pfAllMuons'),
  srcLeg2 = cms.InputTag('pfAllMuons'),
  leg1Pt = cms.double(18.0),
  leg1Eta = cms.double(2.6),
  leg2Pt = cms.double(9.5),
  leg2Eta = cms.double(2.6),
  minDeltaR = cms.double(0.0)
  )
process.mvaMetPairsEE = mvaMetPairs.clone(
  srcLeg1 = cms.InputTag('gsfElectrons'),
  srcLeg2 = cms.InputTag('gsfElectrons'),
  leg1Pt = cms.double(18.0),
  leg1Eta = cms.double(2.6),
  leg2Pt = cms.double(9.5),
  leg2Eta = cms.double(2.6),
  minDeltaR = cms.double(0.0)
  )

process.pfMEtMVAsequence += process.mvaMetPairsMT
process.pfMEtMVAsequence += process.mvaMetPairsET
process.pfMEtMVAsequence += process.mvaMetPairsEM
if isZStudy:
  process.pfMEtMVAsequence += process.mvaMetPairsMM
  process.pfMEtMVAsequence += process.mvaMetPairsEE

if isData:
  process.calibratedAK5PFJetsForPFMEtMVA.correctors = cms.vstring("ak5PFL1FastL2L3Residual")
else:
  process.calibratedAK5PFJetsForPFMEtMVA.correctors = cms.vstring("ak5PFL1FastL2L3")

process.patPFMetByMVA = process.patMETs.clone(
    metSource = cms.InputTag('pfMEtMVA'),
    addMuonCorrections = cms.bool(False),
    genMETSource = cms.InputTag('genMetTrue'),
    addGenMET = cms.bool(False)
)
'''
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
# massSearchReplaceAnyInputTag(process.pfParticleSelectionSequence,
#                              "offlinePrimaryVertices",
#                              "goodOfflinePrimaryVertices",
#                              verbose=True)
# massSearchReplaceAnyInputTag(process.eleIsoSequence,
#                              "offlinePrimaryVertices",
#                              "goodOfflinePrimaryVertices",
#                              verbose=True)
# massSearchReplaceAnyInputTag(process.muIsoSequence,
#                              "offlinePrimaryVertices",
#                              "goodOfflinePrimaryVertices",
#                              verbose=True)

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

removeSpecificPATObjects(process, ['Electrons', 'Muons'])

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

if isEmbedded:
  process.jetTracksAssociatorAtVertexAK5PF.tracks = cms.InputTag("tmfTracks")
  
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
        fileNames = cms.untracked.vstring(
				#'root://eoscms//eos/cms/store/user/agilbert/samples/TauPlusX-2011A-Run166512-42X.root')
				'file:/vols/cms04/amd12/testinput.root')
    )
    process.GlobalTag.globaltag = cms.string('GR_R_42_V25::All')
  else:
    process.source = cms.Source("PoolSource",
      fileNames = cms.untracked.vstring(
			#'root://eoscms//eos/cms/store/user/agilbert/samples/DYJetsToLL-Fall11-42X.root')
			'file:/vols/cms04/amd12/testinput.root')
    )
    process.GlobalTag.globaltag = cms.string('START42_V17::All')
if (release == '53X'):
  if isData:
    process.source = cms.Source("PoolSource",
      fileNames = cms.untracked.vstring(
			#'root://eoscms//eos/cms/store/user/agilbert/samples/TauPlusX-2012D.root')
			'file:/vols/cms04/amd12/testinput.root')
    )
    process.GlobalTag.globaltag = cms.string('FT_53_V21_AN4::All')
  else:
    process.source = cms.Source(
      "PoolSource",
      fileNames = cms.untracked.vstring(
        #'file:pickevents.root'
        #'root://eoscms//eos/cms/store/user/agilbert/samples/DYJetsToLL-Summer12-53X-Sample.root'
				'file:/vols/cms04/amd12/testinput.root'
        )
        #'file:/Volumes/Storage/samples/VBF_HToTauTau_M-125-53X.root'
        #'file:/Volumes/Storage/samples/embed_mutau_v1_DYJetsToLL.root'
    )
    process.GlobalTag.globaltag = cms.string('START53_V7A::All')

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
    StoreTrackIds = cms.bool(True)
    )

process.icTrackProducer = cms.EDProducer('ICTrackProducer',
		mergeLabels = cms.untracked.vstring('icPFJetProducer')
		)



'''
process.icPfMetProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patMETsPF"),
    branchName = cms.untracked.string("pfMet"),
    addGen = cms.untracked.bool(True),
    InputSig = cms.untracked.string("")
    )
if isData: process.icPfMetProducer.addGen = cms.untracked.bool(False)


process.icPfMVAMetProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patPFMetByMVA"),
    branchName = cms.untracked.string("pfMVAMet"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )

process.icPfAllPairsMVAMetProducer = cms.EDProducer('ICMetVectorProducer',
    mergeLabels = cms.untracked.vstring('mvaMetPairsMT','mvaMetPairsET','mvaMetPairsEM'),
    branchName = cms.untracked.string("pfMVAMetVector")
    )
if isZStudy:
  process.icPfAllPairsMVAMetProducer.mergeLabels = cms.untracked.vstring(
        'mvaMetPairsMT','mvaMetPairsET','mvaMetPairsEM','mvaMetPairsMM','mvaMetPairsEE')

'''
process.icTauProducer = cms.EDProducer('ICTauProducer',
    inputLabel = cms.InputTag("selectedPatTaus"),
    branchName = cms.untracked.string("taus"),
    StoreTrackIds = cms.bool(False)
    )

process.icVertexProducer = cms.EDProducer('ICVertexProducer',
    inputLabel = cms.InputTag("goodOfflinePrimaryVertices"),
    branchName = cms.untracked.string("vertices"),
    trackPtThreshold = cms.untracked.double(3.0),
    StoreTrackIds = cms.bool(False),
    FirstVertexOnly = cms.bool(True)
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
'''
process.icGenParticleProducer = cms.EDProducer('ICGenParticleProducer',
    branchName = cms.untracked.string("genParticles"),
    inputLabel = cms.InputTag("genParticles","","SIM"),
    mergeLabels = cms.untracked.vstring(
				'icGenJetProducer'
        ),
    storeMothers = cms.untracked.bool(False),
    storeDaughters = cms.untracked.bool(False),
    addAllStatus1 = cms.untracked.bool(True),
    addAllStatus1Regex = cms.untracked.vstring('11','-11','13','-13'),
    addAllStatus1PtThreshold = cms.untracked.double(8.0),
    addAllStatus2 = cms.untracked.bool(False),
    addAllStatus2Regex = cms.untracked.vstring('5.*','-5.*','4.*','-4.*'),
    addAllStatus2PtThreshold = cms.untracked.double(0.0),
    addAllStatus3 = cms.untracked.bool(True),
    addAllStatus3Regex = cms.untracked.vstring('.*'),
    addAllStatus3PtThreshold = cms.untracked.double(0.0)
)
if release == '42X':
  process.icGenParticleProducer.inputLabel = cms.InputTag("genParticles","","HLT")


process.icEmbeddedGenParticleProducer = cms.EDProducer('ICGenParticleProducer',
    branchName = cms.untracked.string("genParticlesEmbedded"),
    inputLabel = cms.InputTag("genParticles","","EmbeddedRECO"),
    mergeLabels = cms.untracked.vstring(
        ),
    storeMothers = cms.untracked.bool(True),
    storeDaughters = cms.untracked.bool(True),
    addAllStatus1 = cms.untracked.bool(True),
    addAllStatus1Regex = cms.untracked.vstring('.*'),
    addAllStatus1PtThreshold = cms.untracked.double(0.0),
    addAllStatus2 = cms.untracked.bool(True),
    addAllStatus2Regex = cms.untracked.vstring('.*'),
    addAllStatus2PtThreshold = cms.untracked.double(0.0),
    addAllStatus3 = cms.untracked.bool(True),
    addAllStatus3Regex = cms.untracked.vstring('.*'),
    addAllStatus3PtThreshold = cms.untracked.double(0.0)
)
'''

'''
process.icGenTauProductProducer = cms.EDProducer('ICGenTauProductProducer',
  inputLabel = cms.InputTag("genParticles","","SIM"),
  )

process.icTauGenParticleProducer = cms.EDProducer('ICGenParticleProducer',
    branchName = cms.untracked.string("genParticlesTaus"),
    inputLabel = cms.InputTag("genParticles","","SIM"),
    mergeLabels = cms.untracked.vstring('icGenTauProductProducer'),
    storeMothers = cms.untracked.bool(True),
    storeDaughters = cms.untracked.bool(True),
    addAllStatus1 = cms.untracked.bool(False),
    addAllStatus1Regex = cms.untracked.vstring('.*'),
    addAllStatus1PtThreshold = cms.untracked.double(0.0),
    addAllStatus2 = cms.untracked.bool(False),
    addAllStatus2Regex = cms.untracked.vstring('.*'),
    addAllStatus2PtThreshold = cms.untracked.double(0.0),
    addAllStatus3 = cms.untracked.bool(False),
    addAllStatus3Regex = cms.untracked.vstring('.*'),
    addAllStatus3PtThreshold = cms.untracked.double(0.0)
)
if release == '42X':
  process.icGenTauProductProducer.inputLabel = cms.InputTag("genParticles","","HLT")
  process.icTauGenParticleProducer.inputLabel = cms.InputTag("genParticles","","HLT")
	'''

process.icPileupInfoProducer = cms.EDProducer('ICPileupInfoProducer')

process.icEventInfoProducer = cms.EDProducer('ICEventInfoProducer',
    jetsRhoLabel = cms.string("kt6PFJets"),
    leptonRhoLabel = cms.string("kt6PFJetsForLeptons"),
    vertexLabel = cms.string('goodOfflinePrimaryVertices')
    )

#process.icTriggerPathProducer = cms.EDProducer('ICTriggerPathProducer')

process.icSequence = cms.Sequence()


process.icSequence += cms.Sequence(
  #process.icElectronProducer
  #+process.icMuonProducer
  process.icPFJetProducer
	+process.icTrackProducer
 # +process.icPfMetProducer
 # +process.icPfMVAMetProducer
 # +process.icPfAllPairsMVAMetProducer
  +process.icTauProducer
  +process.icVertexProducer
  +process.icEventInfoProducer
  )

process.icDataSequence = cms.Sequence(
 # process.icTriggerPathProducer
  )
if isData: process.icSequence += process.icDataSequence

process.icMCSequence = cms.Sequence(
  process.icPileupInfoProducer
  +process.icGenJetProducer
  #+process.icGenParticleProducer
#  +process.icGenTauProductProducer
#  +process.icTauGenParticleProducer
  )
if not isData: process.icSequence += process.icMCSequence

#process.icEmbeddedSequence = cms.Sequence(
#  process.icEmbeddedGenParticleProducer
#  )
#if isEmbedded: process.icSequence += process.icEmbeddedSequence

#if release == '53X': process.icSequence += process.icSoftLeptonSequence

'''
process.icTriggerSequence = cms.Sequence()
notTp = not isTandP
################################################################
## Define 2011 et,mt,em physics triggers for data
################################################################
if (release == '42X' and isData): 
  process.icEle15LooseTau15ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle15LooseTau15"),
      hltPath = cms.untracked.string("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icEle15TightTau20ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle15TightTau20"),
      hltPath = cms.untracked.string("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icEle15LooseTau20ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle15LooseTau20"),
      hltPath = cms.untracked.string("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icEle18MediumTau20ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle18MediumTau20"),
      hltPath = cms.untracked.string("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icEle20MediumTau20ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle20MediumTau20"),
      hltPath = cms.untracked.string("HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icIsoMu12LooseTau10ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsIsoMu12LooseTau10"),
      hltPath = cms.untracked.string("HLT_IsoMu12_LooseIsoPFTau10_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icIsoMu15LooseTau15ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsIsoMu15LooseTau15"),
      hltPath = cms.untracked.string("HLT_IsoMu15_LooseIsoPFTau15_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icIsoMu15LooseTau20ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsIsoMu15LooseTau20"),
      hltPath = cms.untracked.string("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icMu8Ele17ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsMu8Ele17"),
      hltPath = cms.untracked.string("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icMu17Ele8ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsMu17Ele8"),
      hltPath = cms.untracked.string("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icMu8Ele17IdLObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsMu8Ele17IdL"),
      hltPath = cms.untracked.string("HLT_Mu8_Ele17_CaloIdL_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icMu17Ele8IdLObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsMu17Ele8IdL"),
      hltPath = cms.untracked.string("HLT_Mu17_Ele8_CaloIdL_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
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

################################################################
## Define 2011 et,mt,em physics triggers for mc
################################################################
if (release == '42X' and not isData):
  process.icIsoMu15LooseTau15ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsIsoMu15LooseTau15"),
      hltPath = cms.untracked.string("HLT_IsoMu15_LooseIsoPFTau15_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icEle18MediumTau20ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle18MediumTau20"),
      hltPath = cms.untracked.string("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icMu8Ele17ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsMu8Ele17"),
      hltPath = cms.untracked.string("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icMu17Ele8ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsMu17Ele8"),
      hltPath = cms.untracked.string("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icTriggerSequence += (
    process.icIsoMu15LooseTau15ObjectProducer
    +process.icEle18MediumTau20ObjectProducer 
    +process.icMu8Ele17ObjectProducer
    +process.icMu17Ele8ObjectProducer
    )

################################################################
## Define 2012 et,mt,em,mtmet physics triggers for data
################################################################
if (release == '53X' and isData):
  process.icEle20RhoLooseTau20ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle20RhoLooseTau20"),
      hltPath = cms.untracked.string("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icEle22WP90RhoLooseTau20ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle22WP90RhoLooseTau20"),
      hltPath = cms.untracked.string("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icIsoMu18LooseTau20ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsIsoMu18LooseTau20"),
      hltPath = cms.untracked.string("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icIsoMu17LooseTau20ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsIsoMu17LooseTau20"),
      hltPath = cms.untracked.string("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icMu8Ele17ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsMu8Ele17"),
      hltPath = cms.untracked.string("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icMu17Ele8ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsMu17Ele8"),
      hltPath = cms.untracked.string("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icIsoMu8LooseTau20L1ETM26ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsIsoMu8LooseTau20L1ETM26"),
      hltPath = cms.untracked.string("HLT_IsoMu8_eta2p1_LooseIsoPFTau20_L1ETM26_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icIsoMu8LooseTau20ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsIsoMu8LooseTau20"),
      hltPath = cms.untracked.string("HLT_IsoMu8_eta2p1_LooseIsoPFTau20_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icEle13LooseTau20L1ETM36ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle13LooseTau20L1ETM36"),
      hltPath = cms.untracked.string("HLT_Ele13_eta2p1_WP90Rho_LooseIsoPFTau20_L1ETM36_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icEle13LooseTau20ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle13LooseTau20"),
      hltPath = cms.untracked.string("HLT_Ele13_eta2p1_WP90Rho_LooseIsoPFTau20_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icTriggerSequence += (
    process.icEle20RhoLooseTau20ObjectProducer
    +process.icEle22WP90RhoLooseTau20ObjectProducer
    +process.icIsoMu18LooseTau20ObjectProducer
    +process.icIsoMu17LooseTau20ObjectProducer
    +process.icMu8Ele17ObjectProducer
    +process.icMu17Ele8ObjectProducer
    +process.icIsoMu8LooseTau20L1ETM26ObjectProducer
    +process.icIsoMu8LooseTau20ObjectProducer
    +process.icEle13LooseTau20L1ETM36ObjectProducer
    +process.icEle13LooseTau20ObjectProducer
    )

################################################################
## Define 2012 et,mt,em,mtmet physics triggers for mc
################################################################
if ((release == '53X') and (not isData)):
  process.icEle22WP90RhoLooseTau20ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle22WP90RhoLooseTau20"),
      hltPath = cms.untracked.string("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icIsoMu17LooseTau20ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsIsoMu17LooseTau20"),
      hltPath = cms.untracked.string("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp) 
      )    
  process.icMu8Ele17ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsMu8Ele17"),
      hltPath = cms.untracked.string("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icMu17Ele8ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsMu17Ele8"),
      hltPath = cms.untracked.string("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icMu8ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsMu8"),
      hltPath = cms.untracked.string("HLT_Mu8_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icEle8ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle8"),
      hltPath = cms.untracked.string("HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icIsoMu24ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsIsoMu24"),
      hltPath = cms.untracked.string("HLT_IsoMu24_eta2p1_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
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


################################################################
## Define 2011 t+p triggers for data
################################################################
if (release == '42X' and isData and isTandP): 
  process.icIsoMu24ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsIsoMu24"),
      hltPath = cms.untracked.string("HLT_IsoMu24_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )    
  process.icEle17Ele8ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle17Ele8"),
      hltPath = cms.untracked.string("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass30_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )    
  process.icEle17SC8ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle17SC8"),
      hltPath = cms.untracked.string("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )
  process.icTriggerSequence += (
    process.icIsoMu24ObjectProducer
    +process.icEle17Ele8ObjectProducer
    +process.icEle17SC8ObjectProducer
    )

################################################################
## Define 2012 t+p triggers for data
################################################################
if (release == '53X' and isData and isTandP):
  process.icIsoMu24ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsIsoMu24"),
      hltPath = cms.untracked.string("HLT_IsoMu24_eta2p1_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )  
  process.icMu17TkMu8ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsMu17TkMu8"),
      hltPath = cms.untracked.string("HLT_Mu17_TkMu8_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )  
  process.icEle27ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle27"),
      hltPath = cms.untracked.string("HLT_Ele27_WP80_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )
  process.icEle17Ele8Mass50ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle17Ele8Mass50"),
      hltPath = cms.untracked.string("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass50_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )
  process.icEle20SC4Mass50ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle20SC4Mass50"),
      hltPath = cms.untracked.string("HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )
  process.icTriggerSequence += (
    process.icIsoMu24ObjectProducer
    +process.icMu17TkMu8ObjectProducer
    +process.icEle27ObjectProducer
    +process.icEle17Ele8Mass50ObjectProducer
    +process.icEle20SC4Mass50ObjectProducer
    )
'''
################################################################
## Define 2012 mm,ee physics triggers for data
################################################################
'''
if (release == '53X' and isData and isZStudy):
  process.icEle17Ele8ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle17Ele8"),
      hltPath = cms.untracked.string("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icMu17Mu8ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsMu17Mu8"),
      hltPath = cms.untracked.string("HLT_Mu17_Mu8_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icTriggerSequence += (
    process.icEle17Ele8ObjectProducer
    +process.icMu17Mu8ObjectProducer
    )
'''
################################################################
## Define 2012 mm,ee physics triggers for mc
################################################################
'''
if (release == '53X' and (not isData) and isZStudy):
  process.icEle17Ele8ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsEle17Ele8"),
      hltPath = cms.untracked.string("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icMu17Mu8ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsMu17Mu8"),
      hltPath = cms.untracked.string("HLT_Mu17_Mu8_v"),
      StoreOnlyIfFired = cms.untracked.bool(notTp)
      )
  process.icTriggerSequence += (
    process.icEle17Ele8ObjectProducer
    +process.icMu17Mu8ObjectProducer
    )


process.icSequence += process.icTriggerSequence
''' 
process.icEventProducer = cms.EDProducer('ICEventProducer')
process.icSequence += process.icEventProducer

process.extra42XSequence = cms.Sequence()
if release == '42X':
  process.ak5PFJets.doAreaFastjet = cms.bool(True)
  process.extra42XSequence += (process.ak5PFJets
                              +process.kt6PFJets
                              +process.recoTauClassicHPSSequence)

process.extra53XSequence = cms.Sequence()
if release == '53X':
  process.extra53XSequence += (process.recoTauCommonSequence
                              +process.ak5PFJetsLegacyHPSPiZeros
                              +process.combinatoricRecoTaus
                              +process.produceHPSPFTaus)

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
#  +process.mvaElectronIDSequence
  +process.patDefaultSequence
  +process.puJetMva
#  +process.pfMEtMVAsequence
#  +process.patPFMetByMVA
  +process.icSequence
#	+hpsPFTauDiscriminationByIsolationSeq
  )

#print process.dumpPython()
