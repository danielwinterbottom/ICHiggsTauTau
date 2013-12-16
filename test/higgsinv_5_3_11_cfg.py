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
                  '53X', # default value
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

if isData:
  removeMCMatching(process,['All'])
  runOnData(process)

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
process.selectedPatJets.cut = cms.string("pt>10. && abs(eta)<5.")
#print "cutting selected pat jets"


###############
#assorted things
##############
process.selectedPatMuons.cut = cms.string("isGlobalMuon && pt>10. && abs(eta)<2.5")
process.selectedPatElectrons.cut = cms.string("pt>10. && abs(eta)<2.5")

################################################################
### Set up METs
################################################################
from PhysicsTools.PatAlgos.tools.metTools import addPfMET
addPfMET(process, 'PF')

process.patMETsNoCorr = process.patMETs.clone(
    metSource  = cms.InputTag("pfMet")
      )

process.load("JetMETCorrections.Type1MET.pfMETCorrectionType0_cfi")
process.pfType1CorrectedMet.applyType0Corrections = cms.bool(False) #set to false because this module does track based corrections
process.pfType1CorrectedMet.srcType1Corrections = cms.VInputTag( #this section does pf based type0 corrections as recommended by workbookmetanalysis
  cms.InputTag('pfMETcorrType0'),
  cms.InputTag('pfJetMETcorr', 'type1')
  )

# Get PFMET from runMEtUncertainties
from PhysicsTools.PatUtils.tools.metUncertaintyTools import runMEtUncertainties
process.load("JetMETCorrections.Type1MET.pfMETsysShiftCorrections_cfi")

if isData:
  print "not doing runmetunc for data"
else:
  runMEtUncertainties(process,
                      electronCollection = cms.InputTag('cleanPatElectrons'),
                      photonCollection = '',
                      muonCollection = 'cleanPatMuons',
                      tauCollection = '',
                      jetCollection = cms.InputTag('selectedPatJets'),
                      jetCorrLabel = 'L3Absolute',
                      doSmearJets = True,
                      makeType1corrPFMEt = True,
                      makeType1p2corrPFMEt = False,
                      makePFMEtByMVA = False,
                      makeNoPileUpPFMEt = False,
                      doApplyType0corr = True,
                      #sysShiftCorrParameter = process.pfMEtSysShiftCorrParameters_2012runAvsNvtx_mc,
                      doApplySysShiftCorr = False,
                      )
  process.producePatPFMETCorrections.replace(process.patPFJetMETtype2Corr,process.patPFJetMETtype2Corr + process.type0PFMEtCorrectionPFCandToVertexAssociation + process.patPFMETtype0Corr)
  process.icJetsmearedcentralJets = cms.EDProducer('ICPatJetCandidateProducer',
                                                   
                                                   branchName = cms.untracked.string("jetsmearedcentralJets"),
                                                   
                                                   inputLabel = cms.InputTag("smearedPatJets"),
                                                   
                                                   minPt = cms.double(25.0),
                                                   
                                                   maxEta = cms.double(5.0)
                                                   
                                                   )
  process.icJetsmearedresupJets = cms.EDProducer('ICPatJetCandidateProducer',
                                                 
                                                 branchName = cms.untracked.string("jetsmearedresupJets"),
                                                 
                                                 inputLabel = cms.InputTag("smearedPatJetsResUp"),
                                                 
                                                 minPt = cms.double(25.0),
                                                 
                                                 maxEta = cms.double(5.0)
                                                 
                                                 )
  process.icJetsmearedresdownJets = cms.EDProducer('ICPatJetCandidateProducer',
                                                   
                                                   branchName = cms.untracked.string("jetsmearedresdownJets"),
                                                   
                                                   inputLabel = cms.InputTag("smearedPatJetsResDown"),
                                                   
                                                   minPt = cms.double(25.0),
                                                   
                                                   maxEta = cms.double(5.0)

                                                   )
  process.icpattype1correctedPfMetProducer = cms.EDProducer('ICMetProducer',
                                                   inputLabel = cms.InputTag("patType1CorrectedPFMet"),
                                                   branchName = cms.untracked.string("pattype1correctedpfMet"),
                                                   addGen = cms.untracked.bool(False),
                                                   InputSig = cms.untracked.string("")
                                                   )
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
    process.icPfMetJetResDownProducer
    +process.icPfMetJetResUpProducer
    +process.icJetsmearedcentralJets
    +process.icJetsmearedresupJets
    +process.icJetsmearedresdownJets
    +process.icPfMetJetEnDownProducer
    +process.icPfMetJetEnUpProducer
    +process.icPfMetUnclusteredEnDownProducer
    +process.icPfMetUnclusteredEnUpProducer
    +process.icpattype1correctedPfMetProducer
    )
  
  # Fix Type0 correction module
  process.patPFMETtype0Corr.correction.par3 = cms.double(0.909209)
  process.patPFMETtype0Corr.correction.par2 = cms.double(0.0303531)
  process.patPFMETtype0Corr.correction.par1 = cms.double(-0.703151)
  process.patPFMETtype0Corr.correction.par0 = cms.double(0.0)
                                                                            

#commented out because they aren't used by chayanit
'''
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
'''




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

if (release == '53X'):
  if isData:
    process.source = cms.Source("PoolSource",
      #fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/p/pdunne/public/TauPlusX-2012D.root')
      #fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/p/pdunne/private/CMSSW_5_3_7/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/pickevents.root')
      #fileNames = cms.untracked.vstring("/store/user/srimanob/invHiggs/22Jan13/0008F234-739C-E211-A836-002590832A48.root")
      fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/a/amagnan/CMSSW_5_3_11_patch6/src/UserCode/ICHiggsTauTau/python/embedded_dataTestFile.root')       
     )
    process.GlobalTag.globaltag = cms.string('FT_53_V21A_AN6::All')
  else:
    process.source = cms.Source(
      "PoolSource",
      fileNames = cms.untracked.vstring(
      #'file:/Volumes/Storage/samples/VBF_HToTauTau_M-125-53X.root'
      #'file:/afs/cern.ch/work/p/pdunne/public/VBF_HToTauTau_M-125-53X.root'
      'file:/afs/cern.ch/work/a/amagnan/CMSSW_5_3_11_patch6/src/UserCode/ICHiggsTauTau/python/testFile_W2j.root'       
      #'file:/afs/cern.ch/work/a/amagnan/CMSSW_5_3_11_patch6/src/UserCode/ICHiggsTauTau/python/embedded_mcW2jTestfile.root'       
      )
        #'file:/Volumes/Storage/samples/VBF_HToTauTau_M-125-53X.root'
        #'file:/Volumes/Storage/samples/embed_mutau_v1_DYJetsToLL.root'
    )
    process.GlobalTag.globaltag = cms.string('START53_V27::All')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

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

if release == '53X':
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
## Define 2012 Hinv prompt physics triggers for mc
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

################################################################
## Define 2012 Hinv parked physics triggers for mc
################################################################
if (release == '53X' and (not isData)):
  #Run B,C,D
  process.icDiJet35MJJ650VBFAllJetsObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsDiJet35MJJ650VBFAllJets"),
      hltPath = cms.untracked.string("HLT_DiJet35_MJJ650_AllJets_DEta3p5_VBF_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )
  process.icDiJet35MJJ700VBFAllJetsObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsDiJet35MJJ700VBFAllJets"),
      hltPath = cms.untracked.string("HLT_DiJet35_MJJ700_AllJets_DEta3p5_VBF_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )
  process.icDiJet35MJJ750VBFAllJetsObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsDiJet35MJJ750VBFAllJets"),
      hltPath = cms.untracked.string("HLT_DiJet35_MJJ750_AllJets_DEta3p5_VBF_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )
  
  #Run D only, may not be present in MC AOD
  process.icDiJet20MJJ650VBFAllJetsHT120ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsDiJet20MJJ650VBFAllJetsHT120"),
      hltPath = cms.untracked.string("HLT_DiJet20_MJJ650_AllJets_DEta3p5_HT120_VBF_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )
  process.icDiJet30MJJ700VBFAllJetsObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
      branchName = cms.untracked.string("triggerObjectsDiJet30MJJ700VBFAllJets"),
      hltPath = cms.untracked.string("HLT_DiJet30_MJJ700_AllJets_DEta3p5_VBF_v"),
      StoreOnlyIfFired = cms.untracked.bool(True)
      )
  
  process.icTriggerSequence += ( 
    process.icDiJet35MJJ650VBFAllJetsObjectProducer
    +process.icDiJet35MJJ700VBFAllJetsObjectProducer
    +process.icDiJet35MJJ750VBFAllJetsObjectProducer
    #RUN D ONLY
    +process.icDiJet20MJJ650VBFAllJetsHT120ObjectProducer
    +process.icDiJet30MJJ700VBFAllJetsObjectProducer
  )
  
process.icSequence += process.icTriggerSequence

if not isData:
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
