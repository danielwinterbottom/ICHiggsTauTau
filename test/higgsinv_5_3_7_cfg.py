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
  process.patDefaultSequence.remove(process.patFixedConePFTauDiscrimination)
  process.patDefaultSequence.remove(process.patShrinkingConePFTauDiscrimination)
  process.patDefaultSequence.remove(process.patCaloTauDiscrimination)


################################################################
### Configure PAT Jets
################################################################
# addJetCollection(process,
#   cms.InputTag('ak5PFJets'),
#   'AK5','PF',
#   doJTA=True,
#   doBTagging=True,
#   jetCorrLabel=('AK5PF', ['L1FastJet','L2Relative', 'L3Absolute','L2L3Residual'] if isData else ['L1FastJet','L2Relative', 'L3Absolute']),
#   doType1MET=False,
#   doL1Cleaning=False,
#   doL1Counters=False,
#   genJetCollection=cms.InputTag("ak5GenJetsNoNuBSM"),
#   doJetID      = True,
#   jetIdLabel   = "ak5"
#   )
switchJetCollection(process,
  cms.InputTag('ak5PFJets'),
  doJTA=True,
  doBTagging=True,
  jetCorrLabel=('AK5PF', ['L1FastJet','L2Relative', 'L3Absolute','L2L3Residual'] if isData else ['L1FastJet','L2Relative', 'L3Absolute']),
  doType1MET=True,
  genJetCollection=cms.InputTag("ak5GenJetsNoNuBSM"),
  doJetID = True,
  jetIdLabel = "ak5"
  )
process.patJets.embedGenPartonMatch = cms.bool(False)
# process.patJetsAK5PF.embedGenPartonMatch = cms.bool(False)

################################################################
### Set up METs
################################################################
if (release == '42X'):
  process.load("JetMETCorrections.Type1MET.pfMETCorrections_cff")
  process.metAnalysisSequence=cms.Sequence(process.producePFMETCorrections)
addPfMET(process, 'PF')

# PAT will try and use type-1 corrected MET by default in 53X, here
# we force it to use the uncorrected pfMet
process.patMETs.metSource  = cms.InputTag("pfType1CorrectedMet")
process.patMETsNoCorr = process.patMETs.clone(
  metSource  = cms.InputTag("pfMet")
  )

if isData:
  process.pfJetMETcorr.jetCorrLabel = cms.string("ak5PFL1FastL2L3Residual")
  process.patMETsPF.addGenMET = cms.bool(False)
else:
  process.pfJetMETcorr.jetCorrLabel = cms.string("ak5PFL1FastL2L3")
  process.patMETsPF.addGenMET = cms.bool(True)

# process.patMETsPFType1 = process.patMETsPF.clone(
#   metSource  = cms.InputTag("pfType1CorrectedMet"),
#   addGenMET = cms.bool(False)
#   )

process.load("JetMETCorrections.Type1MET.pfMETCorrectionType0_cfi")
process.pfType1CorrectedMet.applyType0Corrections = cms.bool(False)
process.pfType1CorrectedMet.srcType1Corrections = cms.VInputTag(
      cms.InputTag('pfMETcorrType0'),
      cms.InputTag('pfJetMETcorr', 'type1')
    )

from PhysicsTools.PatUtils.tools.metUncertaintyTools import runMEtUncertainties
runMEtUncertainties(process, outputModule='')

process.patType1CorrectedPFMetElectronEnUp.src = cms.InputTag('patMETs')
process.patType1CorrectedPFMetElectronEnDown.src = cms.InputTag('patMETs')
process.icPfMetElectronEnUpProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patType1CorrectedPFMetElectronEnUp"),
    branchName = cms.untracked.string("pfMetElectronEnUp"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )
process.icPfMetElectronEnDownProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patType1CorrectedPFMetElectronEnDown"),
    branchName = cms.untracked.string("pfMetElectronEnDown"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )

process.patType1CorrectedPFMetMuonEnUp.src = cms.InputTag('patMETs')
process.patType1CorrectedPFMetMuonEnDown.src = cms.InputTag('patMETs')
process.icPfMetMuonEnUpProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patType1CorrectedPFMetMuonEnUp"),
    branchName = cms.untracked.string("pfMetMuonEnUp"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )
process.icPfMetMuonEnDownProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patType1CorrectedPFMetMuonEnDown"),
    branchName = cms.untracked.string("pfMetMuonEnDown"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )

process.patType1CorrectedPFMetTauEnUp.src = cms.InputTag('patMETs')
process.patType1CorrectedPFMetTauEnDown.src = cms.InputTag('patMETs')
process.icPfMetTauEnUpProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patType1CorrectedPFMetTauEnUp"),
    branchName = cms.untracked.string("pfMetTauEnUp"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )
process.icPfMetTauEnDownProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patType1CorrectedPFMetTauEnDown"),
    branchName = cms.untracked.string("pfMetTauEnDown"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )

process.patType1CorrectedPFMetJetResUp.src = cms.InputTag('patMETs')
process.patType1CorrectedPFMetJetResDown.src = cms.InputTag('patMETs')
process.icPfMetJetResUpProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patType1CorrectedPFMetJetResUp"),
    branchName = cms.untracked.string("pfMetJetResUp"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )
process.icPfMetJetResDownProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patType1CorrectedPFMetJetResDown"),
    branchName = cms.untracked.string("pfMetJetResDown"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )

process.patType1CorrectedPFMetJetEnUp.src = cms.InputTag('patMETs')
process.patType1CorrectedPFMetJetEnDown.src = cms.InputTag('patMETs')
process.icPfMetJetEnUpProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patType1CorrectedPFMetJetEnUp"),
    branchName = cms.untracked.string("pfMetJetEnUp"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )
process.icPfMetJetEnDownProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patType1CorrectedPFMetJetEnDown"),
    branchName = cms.untracked.string("pfMetJetEnDown"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )

process.patType1CorrectedPFMetUnclusteredEnUp.src = cms.InputTag('patMETs')
process.patType1CorrectedPFMetUnclusteredEnDown.src = cms.InputTag('patMETs')
process.icPfMetUnclusteredEnUpProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patType1CorrectedPFMetUnclusteredEnUp"),
    branchName = cms.untracked.string("pfMetUnclusteredEnUp"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )
process.icPfMetUnclusteredEnDownProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patType1CorrectedPFMetUnclusteredEnDown"),
    branchName = cms.untracked.string("pfMetUnclusteredEnDown"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )

process.icMetUncertaintySequence = cms.Sequence(
  process.icPfMetElectronEnDownProducer
  +process.icPfMetElectronEnUpProducer
  +process.icPfMetMuonEnDownProducer
  +process.icPfMetMuonEnUpProducer
  +process.icPfMetTauEnDownProducer
  +process.icPfMetTauEnUpProducer
  +process.icPfMetJetResDownProducer
  +process.icPfMetJetResUpProducer
  +process.icPfMetJetEnDownProducer
  +process.icPfMetJetEnUpProducer
  +process.icPfMetUnclusteredEnDownProducer
  +process.icPfMetUnclusteredEnUpProducer
  )


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
process.selectedAndFilteredPatJets = process.selectedPatJets.clone(
  cut = 'pt > 15. & abs(eta) < 100.'
  )

################################################################
### Configuration of Electron ID MVA
################################################################
process.load("EGamma.EGammaAnalysisTools.electronIdMVAProducer_cfi")
process.mvaElectronIDSequence = cms.Sequence(process.mvaTrigV0 + process.mvaNonTrigV0)


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

## Only add HLT tau simulation in MC
if not isData:
  process.icSoftLeptonSequence += cms.Sequence(
      process.hltPFTauSequence
      +process.icHLTTauProducer
      )

process.patmetNoHF = process.patMETs.clone(
    metSource = cms.InputTag('metNoHF'),
    addMuonCorrections = cms.bool(False),
    genMETSource = cms.InputTag('genMetTrue'),
    addGenMET = cms.bool(False)
    )
process.icMetNoHFProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patmetNoHF"),
    branchName = cms.untracked.string("metNoHF"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )
process.icSoftLeptonSequence += cms.Sequence(
    process.icL1ExtraMETProducer
    +process.icL1ExtraMuonsProducer
    +process.icL1ExtraEmIsolatedProducer
    +process.patmetNoHF
    +process.icMetNoHFProducer
    )


################################################################
### Configuration of MVA MET
################################################################
if (release == '53X'):
    process.load("JetMETCorrections.METPUSubtraction.mvaPFMET_leptons_cff")
    from UserCode.ICHiggsTauTau.mvaPFMET_cff_leptons_53X_Dec2012 import mvaMetPairs
else:
    process.load("RecoMET.METProducers.mvaPFMET_cff_leptons")
    process.load("UserCode.ICHiggsTauTau.mvaPFMET_cff_leptons")

process.mvaMetPairsMT = mvaMetPairs.clone(
  srcLeg1 = cms.InputTag('pfAllMuons'),
  srcLeg2 = cms.InputTag('selectedPatTaus'),
  leg1Pt = cms.double(7.0), 
  leg1Eta = cms.double(2.6),
  leg2Pt = cms.double(18.0),
  leg2Eta = cms.double(2.6),
  minDeltaR = cms.double(0.49)
  )
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

if (release == '42X'):
  process.pfMEtMVA.inputFileNames = cms.PSet(
      DPhi = cms.FileInPath('pharris/MVAMet/data/gbrmetphi_42.root'),
      CovU2 = cms.FileInPath('pharris/MVAMet/data/gbrmetu2cov_42.root'),
      U = cms.FileInPath('pharris/MVAMet/data/gbrmet_42.root'),
      CovU1 = cms.FileInPath('pharris/MVAMet/data/gbrmetu1cov_42.root')
      )
  process.pfMEtAllPairsMVA.inputFileNames = cms.PSet(
      DPhi = cms.FileInPath('pharris/MVAMet/data/gbrmetphi_42.root'),
      CovU2 = cms.FileInPath('pharris/MVAMet/data/gbrmetu2cov_42.root'),
      U = cms.FileInPath('pharris/MVAMet/data/gbrmet_42.root'),
      CovU1 = cms.FileInPath('pharris/MVAMet/data/gbrmetu1cov_42.root')
      )

process.patPFMetByMVA = process.patMETs.clone(
    metSource = cms.InputTag('pfMEtMVA'),
    addMuonCorrections = cms.bool(False),
    genMETSource = cms.InputTag('genMetTrue'),
    addGenMET = cms.bool(False)
)


################################################################
### MET Filters-curretly from Sasha's code
#################################################################
## The iso-based HBHE noise filter
process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')


### The CSC beam halo tight filter  - IMPLEMENTED DIRECTLY in ICEventInfoProducer
#process.load('RecoMET.METAnalyzers.CSCHaloFilter_cfi')

### The HCAL laser filter - CAN BE IMPLEMENTED OFFLINE USING A SIMPLE LIST OF EVENTS TO VETO
#process.load("RecoMET.METFilters.hcalLaserEventFilter_cfi")
#process.MyhcalLaserEventFilter = process.hcalLaserEventFilter.clone()
#process.MyhcalLaserEventFilter.taggingMode  = cms.bool(True)

## The ECAL dead cell trigger primitive filter
process.load('RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi')
process.EcalDeadCellTriggerPrimitiveFilter.taggingMode  = cms.bool(True)

## The EE bad SuperCrystal filter
process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.eeBadScFilter.taggingMode  = cms.bool(True)
#

### The ECAL laser correction filter - CAN BE IMPLEMENTED OFFLINE USING A SIMPLE LIST OF EVENTS TO VETO
#process.load('RecoMET.METFilters.ecalLaserCorrFilter_cfi')
#process.MyecalLaserCorrFilter = process.ecalLaserCorrFilter.clone()
#process.MyecalLaserCorrFilter.taggingMode  = cms.bool(True)
#
## The Good vertices collection needed by the tracking failure filter
process.goodVertices = cms.EDFilter(
 "VertexSelector",
 filter = cms.bool(False),
 src = cms.InputTag("offlinePrimaryVertices"),
 cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
) 

## The tracking failure filter
process.load('RecoMET.METFilters.trackingFailureFilter_cfi')
process.trackingFailureFilter.taggingMode  = cms.bool(True)

process.load('RecoMET.METFilters.trackingPOGFilters_cff')
## NOTE: to make tagging mode of the tracking POG filters (three of them), please do:
process.manystripclus53X.taggedMode = cms.untracked.bool(True)
process.manystripclus53X.forcedValue = cms.untracked.bool(False)
process.toomanystripclus53X.taggedMode = cms.untracked.bool(True)
process.toomanystripclus53X.forcedValue = cms.untracked.bool(False)
process.logErrorTooManyClusters.taggedMode = cms.untracked.bool(True)
process.logErrorTooManyClusters.forcedValue = cms.untracked.bool(False)
#
### total sequence
process.filterSequence = cms.Sequence(
# # process.primaryVertexFilter *
# # process.noscraping *
  process.HBHENoiseFilterResultProducer
#  process.CSCTightHaloFilter *
#  process.MyhcalLaserEventFilter *
  +process.EcalDeadCellTriggerPrimitiveFilter
  +process.goodVertices + process.trackingFailureFilter
  +process.eeBadScFilter
#  process.MyecalLaserCorrFilter
# # process.hcallasereventfilter2012 
  +process.trkPOGFilters 
) 

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
    process.load("CMGTools.External.pujetidsequence_cff")
    process.puJetId.jets = cms.InputTag("selectedPatJetsAK5PF")
    process.puJetMva.jets = cms.InputTag("selectedPatJetsAK5PF")

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

# removeSpecificPATObjects(process, ['Electrons', 'Muons'])

# removeCleaning(process)
process.pfAllMuons.src = cms.InputTag("particleFlow")

process.patJets.discriminatorSources = cms.VInputTag(
        cms.InputTag("simpleSecondaryVertexHighEffBJetTagsAOD"), 
        cms.InputTag("simpleSecondaryVertexHighPurBJetTagsAOD"), 
        cms.InputTag("combinedSecondaryVertexBJetTagsAOD"), 
       )
if (release == '42X'):
  process.PFTau = process.recoTauClassicHPSSequence
  from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
  process.kt6PFJets = kt4PFJets.clone(
      rParam = cms.double(0.6),
      doAreaFastjet = cms.bool(True),
      doRhoFastjet = cms.bool(True)
      )
  process.patJetCorrFactors.rho = cms.InputTag("kt6PFJets", "rho")

if isEmbedded:
  process.jetTracksAssociatorAtVertex.tracks = cms.InputTag("tmfTracks")
  
process.btaggingAOD = cms.Sequence(
  process.impactParameterTagInfosAOD
  +process.secondaryVertexTagInfosAOD
  +process.simpleSecondaryVertexHighEffBJetTagsAOD
  +process.simpleSecondaryVertexHighPurBJetTagsAOD
  +process.combinedSecondaryVertexBJetTagsAOD
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
      fileNames = cms.untracked.vstring('file:/Volumes/Storage/samples/TauPlusX-2011A-Run166512-42X.root')
    )
    process.GlobalTag.globaltag = cms.string('GR_R_42_V25::All')
  else:
    process.source = cms.Source("PoolSource",
      fileNames = cms.untracked.vstring('file:/Volumes/Storage/samples/NEW_SYNC_2011.root')
    )
    process.GlobalTag.globaltag = cms.string('START42_V17::All')
if (release == '53X'):
  if isData:
    process.source = cms.Source("PoolSource",
      fileNames = cms.untracked.vstring('file:/Volumes/Storage/samples/TauPlusX-2012D.root')
    )
    process.GlobalTag.globaltag = cms.string('GR_P_V42_AN3::All')
  else:
    process.source = cms.Source(
      "PoolSource",
      fileNames = cms.untracked.vstring(
        #'file:/Volumes/Storage/samples/VBF_HToTauTau_M-125-53X.root'
        'file:/Volumes/Storage/samples/DYJetsToLL-Summer12-53X-Sample.root'
        )
        #'file:/Volumes/Storage/samples/VBF_HToTauTau_M-125-53X.root'
        #'file:/Volumes/Storage/samples/embed_mutau_v1_DYJetsToLL.root'
    )
    process.GlobalTag.globaltag = cms.string('START53_V15::All')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

################################################################
## Configure private modules
################################################################
process.icElectronProducer = cms.EDProducer('ICElectronProducer',
    input = cms.InputTag("gsfElectrons"),
    branchName = cms.string("electrons"),
    minPt = cms.double(9.5),
    maxEta = cms.double(5.0),
    vertexCollection = cms.InputTag("goodOfflinePrimaryVertices"),
    pfIsoPostfix = cms.string("PFIso"),
    isPF = cms.bool(False)
)

process.icMuonProducer = cms.EDProducer('ICMuonProducer',
    input = cms.InputTag("pfAllMuons"),
    branchName = cms.string("muonsPFlow"),
    minPt = cms.double(3.0),
    maxEta = cms.double(5.0),
    vertexCollection = cms.InputTag("goodOfflinePrimaryVertices"),
    pfIsoPostfix = cms.string("PFIso"),
    isPF = cms.bool(True)
)

process.icPFJetProducer = cms.EDProducer('ICPFJetProducer',
    inputLabel = cms.InputTag("selectedAndFilteredPatJets"),
    branchName = cms.untracked.string("pfJetsPFlow"),
    StoreTrackIds = cms.bool(False)
    )

process.icPfMetProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patMETsNoCorr"),
    branchName = cms.untracked.string("pfMet"),
    addGen = cms.untracked.bool(True),
    InputSig = cms.untracked.string("")
    )
if isData: process.icPfMetProducer.addGen = cms.untracked.bool(False)

process.icPfMetType1Producer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patMETs"),
    branchName = cms.untracked.string("pfMetType1"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )

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

process.icPileupInfoProducer = cms.EDProducer('ICPileupInfoProducer')

process.icEventInfoProducer = cms.EDProducer('ICEventInfoProducer',
    jetsRhoLabel = cms.string("kt6PFJets"),
    leptonRhoLabel = cms.string("kt6PFJetsForLeptons"),
    vertexLabel = cms.string('goodOfflinePrimaryVertices')
    )

process.icTriggerPathProducer = cms.EDProducer('ICTriggerPathProducer')

process.icSequence = cms.Sequence()

if isData and release == '53X':
  process.icEventInfoProducer.filters = cms.PSet(
      HBHENoiseFilter                     = cms.InputTag("HBHENoiseFilterResultProducer","HBHENoiseFilterResult"),
      EcalDeadCellTriggerPrimitiveFilter  = cms.InputTag("EcalDeadCellTriggerPrimitiveFilter"),
      eeBadScFilter                       = cms.InputTag("eeBadScFilter"),
      trackingFailureFilter               = cms.InputTag("trackingFailureFilter"),
      manystripclus53X                    = cms.InputTag("!manystripclus53X"),
      toomanystripclus53X                 = cms.InputTag("!toomanystripclus53X"),
      logErrorTooManyClusters             = cms.InputTag("!logErrorTooManyClusters"),
      )
  process.icSequence += process.filterSequence

process.icSequence += cms.Sequence(
  process.icElectronProducer
  +process.icMuonProducer
  +process.icPFJetProducer
  +process.patMETsNoCorr
  +process.icPfMetProducer
  +process.icPfMVAMetProducer
  +process.icPfAllPairsMVAMetProducer
  # +process.patMETsPFType1
  +process.icPfMetType1Producer
  +process.icTauProducer
  +process.icVertexProducer
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
  +process.icGenTauProductProducer
  +process.icTauGenParticleProducer
  )
if not isData: process.icSequence += process.icMCSequence

process.icEmbeddedSequence = cms.Sequence(
  process.icEmbeddedGenParticleProducer
  )
if isEmbedded: process.icSequence += process.icEmbeddedSequence

process.icSequence += process.icSoftLeptonSequence

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

################################################################
## Define 2012 mm,ee physics triggers for data
################################################################
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

################################################################
## Define 2012 mm,ee physics triggers for mc
################################################################
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

################################################################
## Define 2012 Hinv physics triggers for mc
################################################################
if (release == '53X' and (not isData)):
  process.icDiPFJet40PFMETnoMu65MJJ600VBFLeadingJetsObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsDiPFJet40PFMETnoMu65MJJ600VBFLeadingJets"), 
      hltPath = cms.untracked.string("HLT_DiPFJet40_PFMETnoMu65_MJJ600VBF_LeadingJets_v"), 
      StoreOnlyIfFired = cms.untracked.bool(True) 
      ) 
  process.icDiPFJet40PFMETnoMu65MJJ800VBFAllJetsObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsDiPFJet40PFMETnoMu65MJJ800VBFAllJets"), 
      hltPath = cms.untracked.string("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v"), 
      StoreOnlyIfFired = cms.untracked.bool(True) 
      ) 
  process.icDiPFJetAve80ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsDiPFJetAve80"), 
      hltPath = cms.untracked.string("HLT_DiPFJetAve80_v"), 
      StoreOnlyIfFired = cms.untracked.bool(True) 
      ) 
  process.icDiPFJetAve40ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsDiPFJetAve40"), 
      hltPath = cms.untracked.string("HLT_DiPFJetAve40_v"), 
      StoreOnlyIfFired = cms.untracked.bool(True) 
      ) 
  process.icL1ETM40ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsL1ETM40"), 
      hltPath = cms.untracked.string("HLT_L1ETM40"), 
      StoreOnlyIfFired = cms.untracked.bool(True) 
      )
  process.icTriggerSequence += ( 
    process.icDiPFJet40PFMETnoMu65MJJ600VBFLeadingJetsObjectProducer 
    +process.icDiPFJet40PFMETnoMu65MJJ800VBFAllJetsObjectProducer 
    +process.icDiPFJetAve80ObjectProducer 
    +process.icDiPFJetAve40ObjectProducer 
    +process.icL1ETM40ObjectProducer 
    )

process.icSequence += process.icTriggerSequence

process.icSequence += process.icMetUncertaintySequence
 
process.icEventProducer = cms.EDProducer('ICEventProducer')
process.icSequence += process.icEventProducer

process.extra42XSequence = cms.Sequence()
if release == '42X':
  process.extra42XSequence += (process.PFTau+process.ak5PFJets+process.kt6PFJets)

process.extra53XSequence = cms.Sequence()
if release == '53X':
  process.extra53XSequence += (process.recoTauCommonSequence+process.ak5PFJetsLegacyHPSPiZeros+
                               process.combinatoricRecoTaus+process.produceHPSPFTaus)

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
  +process.type0PFMEtCorrection
  +process.patDefaultSequence
  +process.selectedAndFilteredPatJets
  +process.puJetMva
  +process.pfMEtMVAsequence
  +process.patPFMetByMVA
  +process.icSequence
  )

#print process.dumpPython()
