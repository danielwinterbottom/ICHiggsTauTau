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

#PAT has a hissy fit if this OutputModule isn't defined before we try and "removeMCMatching"
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('patTuple.root'),
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),             
                               outputCommands = cms.untracked.vstring('drop *', *patEventContent )
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

if (release == '42X'):
  process.patDefaultSequence.remove(process.patFixedConePFTauDiscrimination)
  process.patDefaultSequence.remove(process.patShrinkingConePFTauDiscrimination)
  process.patDefaultSequence.remove(process.patCaloTauDiscrimination)

switchToPFTauHPS(process)

from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
process.goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
    src=cms.InputTag('offlinePrimaryVertices')
    )

switchOnTrigger(process, outputModule="")


# PAT Jet configuration
if isData:
  addJetCollection(process,
    cms.InputTag('ak5PFJets'),
    'AK5','PF',
    doJTA=True,
    doBTagging=True,
    jetCorrLabel=('AK5PF', ['L1FastJet','L2Relative', 'L3Absolute','L2L3Residual']),
    doType1MET=False,
    doL1Cleaning=False,
    doL1Counters=False,
    genJetCollection=cms.InputTag("ak5GenJetsNoNuBSM"),
    doJetID      = True,
    jetIdLabel   = "ak5"
    )
else:
  addJetCollection(process,
    cms.InputTag('ak5PFJets'),
    'AK5','PF',
    doJTA=True,
    doBTagging=True,
    jetCorrLabel=('AK5PF', ['L1FastJet','L2Relative', 'L3Absolute']),
    doType1MET=False,
    doL1Cleaning=False,
    doL1Counters=False,
    genJetCollection=cms.InputTag("ak5GenJetsNoNuBSM"),
    doJetID      = True,
    jetIdLabel   = "ak5"
    )

### Some fixes for JPT jets: do L1JPTOffset on reco, then
### use these as input to PAT - only do L2L3/Residual in PAT
if not (release == '42X'):
  process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
  process.ak5JPTL1 = cms.ESProducer("JetCorrectionESChain",
      correctors = cms.vstring('ak5L1JPTOffset')
  )
  process.ak5JPTJetsL1 = cms.EDProducer("JPTJetCorrectionProducer",
      src = cms.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
      correctors = cms.vstring('ak5JPTL1')
  )
  process.load("RecoJets.Configuration.RecoJPTJets_cff")
  process.load("RecoJets.JetAssociationProducers.ak5JTA_cff")
  process.ak5JetTracksAssociatorAtVertex.useAssigned = cms.bool(True)
  process.ak5JetTracksAssociatorAtVertex.pvSrc = cms.InputTag("offlinePrimaryVertices")
  process.redoJPTSequence = cms.Sequence(
      process.ak5JTA
      +process.jetPlusTrackZSPCorJetAntiKt5
      +process.ak5JPTJetsL1
      )

if isData:
  # addJetCollection( process,
  #     cms.InputTag('ak5JPTJetsL1'),
  #     'AK5','JPT',
  #     doJTA        = True,
  #     doBTagging   = True,
  #     jetCorrLabel = ('AK5JPT', ['L2Relative','L3Absolute','L2L3Residual']),
  #     doType1MET   = False,
  #     doL1Cleaning = False,
  #     doL1Counters = False,
  #     genJetCollection = cms.InputTag("ak5GenJetsNoNuBSM"),
  #     doJetID      = True,
  #     jetIdLabel   = "ak5"
  #     )
  switchJetCollection(process,
      cms.InputTag('ak5CaloJets'),
      #'AK5','Calo',
      doJTA=False,
      doBTagging=False,
      jetCorrLabel=('AK5Calo', ['L1Offset','L2Relative', 'L3Absolute','L2L3Residual']),
      doType1MET=False,
      genJetCollection=cms.InputTag("ak5GenJetsNoNuBSM"),
      doJetID = True,
      jetIdLabel = "ak5"
      )
else:
  # addJetCollection( process,
  #     cms.InputTag('ak5JPTJetsL1'),
  #     'AK5','JPT',
  #     doJTA        = True,
  #     doBTagging   = True,
  #     jetCorrLabel = ('AK5JPT', ['L2Relative', 'L3Absolute']),
  #     doType1MET   = False,
  #     doL1Cleaning = False,
  #     doL1Counters = False,
  #     genJetCollection = cms.InputTag("ak5GenJetsNoNuBSM"),
  #     doJetID      = True,
  #     jetIdLabel   = "ak5"
  #     )
  switchJetCollection(process,
      cms.InputTag('ak5CaloJets'),
      #'AK5','Calo',
      doJTA=False,
      doBTagging=False,
      jetCorrLabel=('AK5Calo', ['L1Offset','L2Relative', 'L3Absolute']),
      doType1MET=False,
      genJetCollection=cms.InputTag("ak5GenJetsNoNuBSM"),
      doJetID = True,
      jetIdLabel = "ak5"
      )

# if not (release == '42X'):
#   process.inclusiveSecondaryVertexFinderTagInfos.useExternalSV = cms.bool(False)
#   process.inclusiveSecondaryVertexFinderTagInfosAK5PF.useExternalSV = cms.bool(False)
#   process.inclusiveSecondaryVertexFinderTagInfosAOD.useExternalSV = cms.bool(False)
process.patJets.embedGenPartonMatch = cms.bool(False)
#process.patJetsAK5JPT.embedGenPartonMatch = cms.bool(False)
process.patJetsAK5PF.embedGenPartonMatch = cms.bool(False)
process.patElectrons.embedGenMatch = cms.bool(False)
process.patMuons.embedGenMatch = cms.bool(False)

if (release == '42X'):
  process.load("JetMETCorrections.Type1MET.pfMETCorrections_cff")
  process.metAnalysisSequence=cms.Sequence(process.producePFMETCorrections)
addPfMET(process, 'PF')
process.patMETsPF.metSource  = cms.InputTag("pfMet")
if isData:
  process.pfJetMETcorr.jetCorrLabel = cms.string("ak5PFL1FastL2L3Residual")
  process.patMETsPF.addGenMET = cms.bool(False)
else:
  process.pfJetMETcorr.jetCorrLabel = cms.string("ak5PFL1FastL2L3")
  process.patMETsPF.addGenMET = cms.bool(True)
### If PFMET uses type-1 corrected by default, switch back to ordinary
### PFMET, as type-1 corrected appears not to fill the covariance matrix


################################################################
### Configure kt6PFJets for lepton isolation
################################################################
process.load('RecoJets.JetProducers.kt4PFJets_cfi')
process.kt6PFJetsForLeptons = process.kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJetsForLeptons.Rho_EtaMax = cms.double(2.5)


################################################################
### PAT Selections
################################################################
process.selectedPatElectrons.cut = 'pt > 9.5 & abs(eta) < 2.6'
process.selectedPatMuons.cut = 'pt > 3. & abs(eta) < 2.6'  
process.selectedPatJets.cut = 'pt > 15. & abs(eta) < 100.'
process.selectedPatTaus.cut = 'pt > 18. & abs(eta) < 2.6 & tauID("decayModeFinding") > 0.5'
process.selectedPatJetsAK5PF.cut = 'pt > 15. & abs(eta) < 100.'
#process.selectedPatJetsAK5JPT.cut = 'pt > 15. & abs(eta) < 100.'


################################################################
### Configuration of Electron ID MVA
################################################################
process.load("EGamma.EGammaAnalysisTools.electronIdMVAProducer_cfi")
process.mvaElectronIDSequence = cms.Sequence(process.mvaTrigV0 + process.mvaNonTrigV0)
process.patElectrons.electronIDSources = cms.PSet(
    mvaTrigV0 = cms.InputTag("mvaTrigV0"),
    mvaNonTrigV0 = cms.InputTag("mvaNonTrigV0")
)


################################################################
### Configuration of MVA MET
################################################################
if (release == '53X'):
    process.load("JetMETCorrections.METPUSubtraction.mvaPFMET_leptons_cff")
    #process.load("UserCode.ICHiggsTauTau.mvaPFMET_cff_leptons_53X")
    from UserCode.ICHiggsTauTau.mvaPFMET_cff_leptons_53X import mvaMetPairs
else:
    process.load("RecoMET.METProducers.mvaPFMET_cff_leptons")
    process.load("UserCode.ICHiggsTauTau.mvaPFMET_cff_leptons")

process.mvaMetPairsMT = mvaMetPairs.clone(
  srcLeg1 = cms.InputTag('selectedPatMuons'),
  srcLeg2 = cms.InputTag('selectedPatTaus'),
  leg1Pt = cms.double(15.0),
  leg1Eta = cms.double(2.6),
  leg2Pt = cms.double(18.0),
  leg2Eta = cms.double(2.6),
  minDeltaR = cms.double(0.5)
  )

process.mvaMetPairsET = mvaMetPairs.clone(
  srcLeg1 = cms.InputTag('selectedPatElectrons'),
  srcLeg2 = cms.InputTag('selectedPatTaus'),
  leg1Pt = cms.double(18.0),
  leg1Eta = cms.double(2.6),
  leg2Pt = cms.double(18.0),
  leg2Eta = cms.double(2.6),
  minDeltaR = cms.double(0.5)
  )

process.mvaMetPairsEM = mvaMetPairs.clone(
  srcLeg1 = cms.InputTag('selectedPatElectrons'),
  srcLeg2 = cms.InputTag('selectedPatMuons'),
  leg1Pt = cms.double(9.5),
  leg1Eta = cms.double(2.6),
  leg2Pt = cms.double(9.5),
  leg2Eta = cms.double(2.6),
  minDeltaR = cms.double(0.3)
  )

process.mvaMetPairsMM = mvaMetPairs.clone(
  srcLeg1 = cms.InputTag('selectedPatMuons'),
  srcLeg2 = cms.InputTag('selectedPatMuons'),
  leg1Pt = cms.double(18.0),
  leg1Eta = cms.double(2.6),
  leg2Pt = cms.double(9.5),
  leg2Eta = cms.double(2.6),
  minDeltaR = cms.double(0.0)
  )

process.mvaMetPairsEE = mvaMetPairs.clone(
  srcLeg1 = cms.InputTag('selectedPatElectrons'),
  srcLeg2 = cms.InputTag('selectedPatElectrons'),
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

#process.pfMEtAllPairsMVA.srcLeptons =  cms.VInputTag( 'selectedPatElectrons', 'selectedPatMuons', 'selectedPatTaus' )
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
# process.patPFMetAllPairsByMVA = process.patMETs.clone(
#     metSource = cms.InputTag('mvaMetPairsMT'),
#     addMuonCorrections = cms.bool(False),
#     genMETSource = cms.InputTag('genMetTrue'),
#     addGenMET = cms.bool(False)
# )

#--------------------------------------------------------------------------------
# produce PFMET significance cov. matrix

# CV: compute PFMET significance cov. matrix for uncorrected jets
#     in order to include pile-up jets
#    (which to a significant fraction get killed by L1Fastjet corrections)
process.ak5PFJetsNotOverlappingWithLeptons = cms.EDFilter("PFJetAntiOverlapSelector",
  src = cms.InputTag('ak5PFJets'),
  srcNotToBeFiltered = cms.VInputTag('isomuons','isoelectrons','isotaus'),
  dRmin = cms.double(0.5),
  invert = cms.bool(False),
  filter = cms.bool(False)
)
from JetMETCorrections.Type1MET.pfMETCorrections_cff import pfCandsNotInJet
process.pfCandsNotInJetForPFMEtSignCovMatrix = pfCandsNotInJet.clone()
from RecoMET.METProducers.METSigParams_cfi import *
process.pfMEtSignCovMatrix = cms.EDProducer("PFMEtSignCovMatrixProducer",
  METSignificance_params,
  src = cms.VInputTag(
      'isoelectrons', 'isomuons', 'isotaus',
      'ak5PFJetsNotOverlappingWithLeptons',
      'pfCandsNotInJetForPFMEtSignCovMatrix'
  ),
  addJERcorr = cms.PSet(
      inputFileName = cms.FileInPath('PhysicsTools/PatUtils/data/pfJetResolutionMCtoDataCorrLUT.root'),
      lutName = cms.string('pfJetResolutionMCtoDataCorrLUT')
  )
)
process.prePatProductionSequence = cms.Sequence(process.ak5PFJetsNotOverlappingWithLeptons + process.pfCandsNotInJetForPFMEtSignCovMatrix + process.pfMEtSignCovMatrix)



################################################################
### Configuration of MVA PU Jet ID
################################################################
if (release == '53X'):
    process.load("RecoJets.JetProducers.pujetidsequence_cff")
    process.puJetId.jets = cms.InputTag("ak5PFJets")
    process.puJetMva.jets = cms.InputTag("ak5PFJets")
else:
    process.load("CMGTools.External.pujetidsequence_cff")
    process.puJetId.jets = cms.InputTag("selectedPatJetsAK5PF")
    process.puJetMva.jets = cms.InputTag("selectedPatJetsAK5PF")



################################################################
### General Config
################################################################
process.MessageLogger.cerr.FwkReport.reportEvery = 50000
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
        'file:/Volumes/Storage/samples/DoubleElectron-2012C-24Aug2012-v1-Sample.root'
      )
    )
    process.GlobalTag.globaltag = cms.string('GR_P_V42_AN2::All')
  else:
    process.source = cms.Source(
      "PoolSource",
      fileNames = cms.untracked.vstring(
        #'file:/Volumes/Storage/samples/VBF_HToTauTau_M-125-53X.root'
        'file:/Volumes/Storage/samples/DYJetsToLL-Summer12-53X-Sample.root'
      )
    )
    process.GlobalTag.globaltag = cms.string('START53_V10::All')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

if (release == '42X'):
  process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")

################################################################
### Final PAT config
################################################################
adaptPFMuons(process,process.patMuons)
process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons')
process.muIsoSequence = setupPFMuonIso(process, 'pfAllMuons')
adaptPFIsoMuons( process, applyPostfix(process,"patMuons",""))
process.muonMatch.src = cms.InputTag("pfAllMuons")
adaptPFIsoElectrons( process, applyPostfix(process,"patElectrons",""))

if isData:
  runOnData(process)

removeCleaning(process)
process.pfAllMuons.src = cms.InputTag("particleFlow")
process.patMuons.pfMuonSource = cms.InputTag("pfAllMuons")
process.patMuons.embedPFCandidate = cms.bool(False)
process.elPFIsoValueGamma04PFIdPFIso.deposits[0].vetos = cms.vstring('EcalEndcaps:ConeVeto(0.08)','EcalBarrel:ConeVeto(0.08)')
process.elPFIsoValueNeutral04PFIdPFIso.deposits[0].vetos = cms.vstring()
process.elPFIsoValuePU04PFIdPFIso.deposits[0].vetos = cms.vstring()
process.elPFIsoValueCharged04PFIdPFIso.deposits[0].vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)')
process.elPFIsoValueChargedAll04PFIdPFIso.deposits[0].vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)','EcalBarrel:ConeVeto(0.01)')
process.patJetsAK5PF.discriminatorSources = cms.VInputTag(
        cms.InputTag("simpleSecondaryVertexHighEffBJetTagsAK5PF"), 
        cms.InputTag("simpleSecondaryVertexHighPurBJetTagsAK5PF"), 
        cms.InputTag("combinedSecondaryVertexBJetTagsAK5PF"), 
       )
if (release == '42X'):
  process.PFTau = process.recoTauClassicHPSSequence


if (release == '42X'):
  from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
  process.kt6PFJets = kt4PFJets.clone(
      rParam = cms.double(0.6),
      doAreaFastjet = cms.bool(True),
      doRhoFastjet = cms.bool(True)
      )
  process.patJetCorrFactorsAK5PF.rho = cms.InputTag("kt6PFJets", "rho")

if isEmbedded:
  process.jetTracksAssociatorAtVertexAK5PF.tracks = cms.InputTag("tmfTracks")
  
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
## Configure private modules
################################################################
process.icElectronProducer = cms.EDProducer('ICElectronProducer',
    inputLabel = cms.InputTag("selectedPatElectrons"),
    branchName = cms.untracked.string("electrons"),
    AddPFIsoPostfix = cms.string("03PFIdPFIso"),
    RhoJetsName = cms.string("kt6PFJetsForLeptons"),
    PFNoPURhoJetsName = cms.string("kt6PFJetsForLeptons"),
    EffAreas = cms.string("kEleEAFall11MC"),
    IsPF = cms.bool(False)
)
if isData:
  if (release == '53X'):
    process.icElectronProducer.EffAreas = cms.string("kEleEAData2012")
  if (release == '42X'):
    process.icElectronProducer.EffAreas = cms.string("kEleEAData2011")

process.icMuonProducer = cms.EDProducer('ICMuonProducer',
    inputLabel = cms.InputTag("selectedPatMuons"),
    branchName = cms.untracked.string("muonsPFlow"),
    AddPFIsoPostfix = cms.string("03PFIso"),
    RhoJetsName = cms.string("kt6PFJetsForLeptons"),
    PFNoPURhoJetsName = cms.string("kt6PFJetsForLeptons"),
    EffAreas = cms.string(""),
    IsPF = cms.bool(True)
    )

process.icCaloJetProducer = cms.EDProducer('ICCaloJetProducer',
    inputLabel = cms.InputTag("selectedPatJets"),
    branchName = cms.untracked.string("caloJets"))

process.icPFJetProducer = cms.EDProducer('ICPFJetProducer',
    inputLabel = cms.InputTag("selectedPatJetsAK5PF"),
    branchName = cms.untracked.string("pfJetsPFlow"),
    StoreTrackIds = cms.bool(False)
    )

process.icJPTJetProducer = cms.EDProducer('ICJPTJetProducer',
    inputLabel = cms.InputTag("selectedPatJetsAK5JPT"),
    branchName = cms.untracked.string("jptJets"),
    StoreTrackIds = cms.bool(False)
    )

process.icCaloMetProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patMETs"),
    branchName = cms.untracked.string("caloMet"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )

process.icPfMetProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patMETsPF"),
    branchName = cms.untracked.string("pfMet"),
    addGen = cms.untracked.bool(True),
    InputSig = cms.untracked.string("pfMEtSignCovMatrix")
    )
if isData:
  process.icPfMetProducer.addGen = cms.untracked.bool(False)

process.icPfMVAMetProducer = cms.EDProducer('ICMetProducer',
    inputLabel = cms.InputTag("patPFMetByMVA"),
    branchName = cms.untracked.string("pfMVAMet"),
    addGen = cms.untracked.bool(False),
    InputSig = cms.untracked.string("")
    )

process.icPfAllPairsMVAMetProducer = cms.EDProducer('ICMetVectorProducer',
    mergeLabels = cms.untracked.vstring(
        'mvaMetPairsMT','mvaMetPairsET','mvaMetPairsEM'
        ),
    branchName = cms.untracked.string("pfMVAMetVector")
    )

if isZStudy:
  process.icPfAllPairsMVAMetProducer.mergeLabels = cms.untracked.vstring(
        'mvaMetPairsMT','mvaMetPairsET','mvaMetPairsEM',
        'mvaMetPairsMM','mvaMetPairsEE'
        )

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

process.icTrackProducer = cms.EDProducer('ICTrackProducer',
    mergeLabels = cms.untracked.vstring(
        'icVertexProducer','icTauProducer',
        'icPFJetProducer'
        )
    )

process.icSuperClusterProducer = cms.EDProducer('ICSuperClusterProducer',
    branchName = cms.untracked.string("superClusters"),
    minEnergy = cms.untracked.double(15.0)
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
    mergeLabels = cms.untracked.vstring(
        'icPFJetProducer',
        #'icJPTJetProducer',
        #'icGenJetProducer',
        'icElectronProducer',
        'icMuonProducer'
        ),
    storeMothers = cms.untracked.bool(False),
    storeDaughters = cms.untracked.bool(False),
    addAllStatus1 = cms.untracked.bool(False),
    addAllStatus1Regex = cms.untracked.vstring('.*'),
    addAllStatus1PtThreshold = cms.untracked.double(10.0),
    addAllStatus2 = cms.untracked.bool(False),
    addAllStatus2Regex = cms.untracked.vstring('5.*','-5.*','4.*','-4.*'),
    addAllStatus2PtThreshold = cms.untracked.double(0.0),
    addAllStatus3 = cms.untracked.bool(True),
    addAllStatus3Regex = cms.untracked.vstring('.*'),
    addAllStatus3PtThreshold = cms.untracked.double(0.0)
)

process.icPileupInfoProducer = cms.EDProducer('ICPileupInfoProducer')

process.icEventInfoProducer = cms.EDProducer('ICEventInfoProducer',
    jetsRhoLabel = cms.string("kt6PFJets"),
    leptonRhoLabel = cms.string("kt6PFJetsForLeptons"),
    vertexLabel = cms.string('goodOfflinePrimaryVertices')
    )

process.icTriggerPathProducer = cms.EDProducer('ICTriggerPathProducer')

process.icSequence = cms.Sequence(
  process.icElectronProducer
  +process.icMuonProducer
  +process.icPFJetProducer
  +process.icPfMetProducer
  +process.icPfMVAMetProducer
  +process.icPfAllPairsMVAMetProducer
  +process.icTauProducer
  +process.icVertexProducer
  +process.icEventInfoProducer
  )

process.icDataSequence = cms.Sequence(
  process.icTriggerPathProducer
  )

process.icMCSequence = cms.Sequence(
  process.icPileupInfoProducer
  +process.icGenJetProducer
  +process.icGenParticleProducer
  )

process.icEmbeddedSequence = cms.Sequence(
  process.icGenParticleProducer
  )

if isData:
  process.icSequence += process.icDataSequence

if not (isData):
  process.icSequence += process.icMCSequence

if isEmbedded:
  process.icSequence += process.icEmbeddedSequence

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
## Define 2012 et,mt,em physics triggers for data
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
  process.icTriggerSequence += (
    process.icEle20RhoLooseTau20ObjectProducer
    +process.icEle22WP90RhoLooseTau20ObjectProducer
    +process.icIsoMu18LooseTau20ObjectProducer
    +process.icIsoMu17LooseTau20ObjectProducer
    +process.icMu8Ele17ObjectProducer
    +process.icMu17Ele8ObjectProducer
    )

################################################################
## Define 2012 et,mt,em physics triggers for mc
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
  process.icTriggerSequence += (
    process.icEle22WP90RhoLooseTau20ObjectProducer
    +process.icIsoMu17LooseTau20ObjectProducer
    +process.icMu8Ele17ObjectProducer
    +process.icMu17Ele8ObjectProducer
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
## Define 2011 mm,ee physics triggers for data
################################################################

################################################################
## Define 2011 mm,ee physics triggers for mc
################################################################

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


process.icSequence += process.icTriggerSequence
 
process.icEventProducer = cms.EDProducer('ICEventProducer')
process.icSequence += process.icEventProducer

process.extra42XSequence = cms.Sequence()
if release == '42X':
  process.extra42XSequence += (process.PFTau+process.ak5PFJets+process.kt6PFJets)

process.extra53XSequence = cms.Sequence()
if release == '53X':
  process.extra53XSequence += (process.softElectronCands)

process.mcSequence = cms.Sequence()
if not isData:
  process.mcSequence += (process.genParticlesForJets+process.ak5GenJetsNoNuBSM)

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
  +process.puJetIdSqeuence
  +process.pfMEtMVAsequence
  +process.prePatProductionSequence
  +process.patPFMetByMVA
  +process.icSequence
  )
