import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################                                                                                         
# Required additional modules !!CHECK ALL UP TO DATE
# Electron conversion veto calculator:
#   in CMSSW src: git clone https://github.com/ajgilbert/ICAnalysis-ElectronConversionCalculator.git ICAnalysis/ElectronConversionCalculator
# Electron cut based ID:
#   in CMSSW src: git cms-merge-topic ikrav:egm_id_phys14
################################################################                                                                                           

################################################################                                                                                         
# Define some strings                                                                                                                          
################################################################                                                                                           
electronLabel = cms.InputTag("slimmedElectrons")
muonLabel = cms.InputTag("slimmedMuons")
photonLabel = cms.InputTag("slimmedPhotons")
tauLabel = cms.InputTag("slimmedTaus")
vtxLabel = cms.InputTag("offlineSlimmedPrimaryVertices")

################################################################                                                                                         
# Setup and Read Options                                                                                                                          
################################################################                                                                                           
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
opts.register('file', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "input file")
opts.register('globalTag', '76X_mcRun2_asymptotic_v12', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('isData', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as data?")
opts.register('release', '76XMINIAOD', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")
#opts.register('isNLO', False, parser.VarParsing.multiplicity.singleton,
#    parser.VarParsing.varType.bool, "Store sign of weight?")
opts.register('doHT', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Store HT?")

opts.parseArguments()
infile      = opts.file
if not infile: infile = "file:/tmp/file.root"
isData      = opts.isData
tag         = opts.globalTag
release     = opts.release
#isNLO       = opts.isNLO
if not isData:
  doHT     = opts.doHT
else:
  doHT     = 0

if not release in ["76XMINIAOD"]:
  print 'Release not recognised, exiting!'
  sys.exit(1)
print 'release     : '+release
print 'isData      : '+str(isData)
print 'globalTag   : '+tag
print 'doHT        : '+str(doHT)


################################################################                                                                                          
# Standard setup                                                                                                                                           
################################################################                                                                                          
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")#!!
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")#!!
process.load("Configuration.StandardSequences.MagneticField_cff")

process.TFileService = cms.Service("TFileService",
  fileName = cms.string("EventTree.root"),
  closeFileFast = cms.untracked.bool(True)
)

################################################################                                                                                        
# Message Logging, summary, and number of events                                                                                                          
################################################################                                                                                          
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(10)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.options   = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)

################################################################ 
# Input files and global tags
################################################################ 

process.load("CondCore.DBCommon.CondDBCommon_cfi")
from CondCore.DBCommon.CondDBSetup_cfi import *
                                         
if not isData:                         
  process.source = cms.Source("PoolSource", fileNames =
       #                     cms.untracked.vstring('file:/vols/cms04/pjd12/testminiaodfiles/58D548B0-AB6F-E411-B468-3417EBE34D1A.root') 
                              #cms.untracked.vstring('root://xrootd.unl.edu//store/mc/RunIISpring15DR74/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/048FB1EE-33FD-E411-A2BA-0025905A6094.root')
                              #cms.untracked.vstring('root://xrootd.unl.edu//store/mc/RunIISpring15DR74/WW_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v1/10000/006ABB57-1207-E511-A99E-0002C94CD150.root')
 #AMM - reminiaod v2 - 13/10/2015
                              #cms.untracked.vstring('file:/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_6/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/04F50A91-B46F-E511-A2A3-002618943923.root')
                              cms.untracked.vstring('file:mc_vbfh_test_Fall15.root')
                              #cms.untracked.vstring('file:/vols/cms04/pjd12/testminiaodfiles/1E0D8712-D722-E511-B7CF-008CFA051614.root')

                              
                              )
else:
  process.source = cms.Source("PoolSource", fileNames =
       #                     cms.untracked.vstring('file:/vols/cms04/pjd12/testminiaodfiles/58D548B0-AB6F-E411-B468-3417EBE34D1A.root') 
                              #cms.untracked.vstring('root://xrootd.unl.edu//store/data/Run2015B/MET/MINIAOD/17Jul2015-v1/30000/64C3BF26-C32E-E511-8430-002618943982.root')
                              #cms.untracked.vstring('root://xrootd.unl.edu//store/data/Run2015D/MET/MINIAOD/PromptReco-v3/000/256/584/00000/C0ED0230-855D-E511-85C6-02163E0170AD.root')
#AMM - reminiaod v2 - 13/10/2015
                              #cms.untracked.vstring('file:/afs/cern.ch/work/a/amagnan/CMSSW_7_4_14/src/datatest_oct5.root')
                              cms.untracked.vstring('file:datafile_test_miniaodv2_16dec15.root')
                              #cms.untracked.vstring('file:/afs/cern.ch/work/a/amagnan/CMSSW_7_4_14/src/datatest_prv4.root')                    
                              )
process.GlobalTag.globaltag = cms.string(tag)

#'root://xrootd-cms.infn.it///store/mc/Phys14DR/VBF_HToInv_M-125_13TeV_powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/58D548B0-AB6F-E411-B468-3417EBE34D1A.root')
# 72X MC: file=root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/AODSIM/PU40bx25_PHYS14_25_V1-v1/00000/00E63918-3A70-E411-A246-7845C4FC35F3.root globalTag=START72_V1::All                                                                                                               
# 72XMINIAOD MC: file=root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/36224FE2-0571-E411-9664-00266CFAE30C.root globalTag=START72_V1::All                                                                                                   
#load JEC not in global tag
# process.jec = cms.ESSource("PoolDBESSource",
#       DBParameters = cms.PSet(
#         messageLevel = cms.untracked.int32(0)
#         ),
#       timetype = cms.string('runnumber'),
#       toGet = cms.VPSet(
#       cms.PSet(
#             record = cms.string('JetCorrectionsRecord'),
#             tag    = cms.string('JetCorrectorParametersCollection_Summer15_25nsV5_DATA_AK4PFchs'),
#             label  = cms.untracked.string('AK4PFchs')
#             ),
#       #..................................................
#       ## here you add as many jet types as you need
#       ## note that the tag name is specific for the particular sqlite file 
#       ), 
#       connect = cms.string('sqlite:Summer15_25nsV5_DATA.db')
#      # uncomment above tag lines and this comment to use MC JEC
#      # connect = cms.string('sqlite:Summer12_V7_MC.db')
# )
# ## add an es_prefer statement to resolve a possible conflict from simultaneous connection to a global tag
# process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')

 
import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors


################################################################
# Declare sequences
################################################################
process.ic76XSequence = cms.Sequence()
process.icMiniAODSequence = cms.Sequence()


################################################################
# Object selection
################################################################
#process.selectedVertices = cms.EDFilter("VertexRefSelector",
#  src = cms.InputTag("offlineSlimmedPrimaryVertices"),
#  cut = cms.string("ndof >= 4 & abs(z) <= 24 & abs(position.Rho) <= 2")
#)

process.selectedElectrons = cms.EDFilter("PATElectronRefSelector",
  src = electronLabel,
  cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
)

process.selectedPFMuons = cms.EDFilter("PATMuonRefSelector",
  src = muonLabel,
  cut = cms.string("pt > 3.0 & abs(eta) < 2.6")
)

process.selectedPFTaus = cms.EDFilter("PATTauRefSelector",
  src = tauLabel,
  cut = cms.string('pt > 18.0 & abs(eta) < 2.6 & tauID("decayModeFindingNewDMs") > 0.5')
)


#process.selectedPF = cms.EDFilter("PATPackedCandidateSelector",#
#  src = cms.InputTag("packedPFCandidates"),
#  cut = cms.string("fromPV > 1&&charge!=0&&pt>0.9")
#)

process.icSelectionSequence = cms.Sequence(
  #process.selectedPF+
  #process.selectedVertices+
  process.selectedElectrons+
  process.selectedPFMuons+
  process.selectedPFTaus
)


################################################################                                                                                            
# PF sequence for lepton isolation                                             
#!!andrew custom code to redo iso, look into taking default miniaod values by making value map
#used as input for CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff
################################################################                                                                                          

process.load("CommonTools.ParticleFlow.pfParticleSelection_cff")

if release in ['76XMINIAOD']:
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

################################################################                                                                                          
# Vertices                                                                                                                                                 
################################################################                                                                                           
process.icVertexProducer = producers.icVertexProducer.clone(
  branch  = cms.string("vertices"),
  input = cms.InputTag("offlineSlimmedPrimaryVertices"),
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

process.icElectronSequence = cms.Sequence()

#ICElectronConversionCalculator NOT final, but at least have a running version for now
process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
    input       = electronLabel,
    beamspot    = cms.InputTag("offlineBeamSpot"),
    conversions = cms.InputTag("reducedEgamma:reducedConversions")
    )

process.load("RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi")
process.electronMVAValueMapProducer.src = electronLabel
process.electronMVAValueMapProducer.srcMiniAOD = electronLabel

process.icElectronSequence+=cms.Sequence(
#   process.egmGsfElectronIDSequence+
   process.electronMVAValueMapProducer
   )

#Electron PF iso sequence:
process.load("CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff")
#  process.elPFIsoValueCharged03PFIdPFIso    = process.icElectronCharged.clone()
#process.elPFIsoValueCharged03PFIdPFIso    = process.elPFIsoValueCharged03PFId.clone()
process.elPFIsoValueChargedAll03PFIdPFIso = process.elPFIsoValueChargedAll03PFId.clone()
#process.elPFIsoValueGamma03PFIdPFIso      = process.elPFIsoValueGamma03PFId.clone()
#process.elPFIsoValueNeutral03PFIdPFIso    = process.elPFIsoValueNeutral03PFId.clone()
#process.elPFIsoValuePU03PFIdPFIso         = process.elPFIsoValuePU03PFId.clone()
#  process.elPFIsoDepositCharged.src     = electronLabel
process.elPFIsoDepositChargedAll.src  = electronLabel
#  process.elPFIsoDepositNeutral.src     = electronLabel
#  process.elPFIsoDepositGamma.src       = electronLabel
#  process.elPFIsoDepositPU.src          = electronLabel
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

if release in ['76XMINIAOD']:
  process.elPFIsoDepositGamma.ExtractorPSet.ComponentName = cms.string("CandViewExtractor")

process.icElectronSequence += cms.Sequence(
#      process.electronPFIsolationDepositsSequence+
    process.elPFIsoDepositChargedAll+
    process.electronPFIsolationValuesSequence
    )

process.elPFIsoValueChargedAll03PFIdPFIso.deposits[0].vetos = (
    cms.vstring('EcalEndcaps:ConeVeto(0.015)','EcalBarrel:ConeVeto(0.01)'))

if release in ['76XMINIAOD']:
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
     mvaTrigSpring15       = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15Trig25nsV1Values")
  ),
  includeClusterIso        = cms.bool(True),
  includePFIso03           = cms.bool(True),
  includePFIso04           = cms.bool(True)
)


if release in ['76X']:
  process.icElectronProducer.includeClusterIso = cms.bool(False)

process.icElectronSequence += cms.Sequence(
  process.icElectronConversionCalculator+
  process.icElectronProducer
)



################################################################ 
# Muons
################################################################

process.icMuonSequence = cms.Sequence()

#redo isolation calculation
process.load("CommonTools.ParticleFlow.Isolation.pfMuonIsolation_cff")
process.load("CommonTools.ParticleFlow.deltaBetaWeights_cff")


process.muPFIsoDepositCharged.src     = muonLabel
process.muPFIsoDepositChargedAll.src  = muonLabel
process.muPFIsoDepositNeutral.src     = muonLabel
process.muPFIsoDepositNeutral.ExtractorPSet.inputCandView = cms.InputTag("pfWeightedNeutralHadrons")
process.muPFIsoDepositGamma.src       = muonLabel
process.muPFIsoDepositGamma.ExtractorPSet.inputCandView = cms.InputTag("pfWeightedPhotons")
process.muPFIsoDepositPU.src          = muonLabel



process.muPFIsoValueChargedAll03PFIso = process.muPFIsoValueChargedAll03.clone()
process.muPFIsoValueCharged03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muonLabel,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("charged_iso") 
)    
process.muPFIsoValueGamma03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muonLabel,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("photon_iso") 
)    
process.muPFIsoValueNeutral03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muonLabel,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("neutral_iso") 
)    
process.muPFIsoValuePU03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muonLabel,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("pu_iso") 
)    
process.muPFIsoValueNeutral04PFWeights = process.muPFIsoValueNeutral04.clone()
process.muPFIsoValueGamma04PFWeights = process.muPFIsoValueGamma04.clone()
process.muPFIsoValueNeutral03PFWeights = process.muPFIsoValueNeutral03.clone()
process.muPFIsoValueGamma03PFWeights = process.muPFIsoValueGamma03.clone()

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
  input        = muonLabel,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("charged_iso") 
)    
process.muPFIsoValueGamma04PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muonLabel,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("photon_iso") 
)    
process.muPFIsoValueNeutral04PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muonLabel,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("neutral_iso") 
)    
process.muPFIsoValuePU04PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muonLabel,
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

#make and store ic muon object
process.icMuonProducer = producers.icMuonProducer.clone(
  branch                    = cms.string("muons"),
  input                     = cms.InputTag("selectedPFMuons"),
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

process.icMuonSequence += cms.Sequence(
  process.icMuonProducer
)

################################################################                                                                                            
# Taus                                                                                                                                                   
################################################################                                                                                            
import UserCode.ICHiggsTauTau.tau_discriminators_cfi as tauIDs

#make and store taus
process.icTauProducer = cms.EDProducer("ICPFTauFromPatProducer",
  branch                  = cms.string("taus"),
  input                   = cms.InputTag("selectedPFTaus"),
  inputVertices           = vtxLabel,
  includeVertexIP         = cms.bool(True),
  requestTracks           = cms.bool(False),
  tauIDs = cms.PSet()
)

process.icTauSequence = cms.Sequence(
  process.icTauProducer
)

##################################################################                                                                                          
#  Photons
#################################################################
process.icPhotonSequence = cms.Sequence()

#Calculate photon conversion veto
process.icPhotonElectronConversionVetoCalculator = cms.EDProducer("ICPhotonElectronConversionVetoFromPatCalculator",
                                                        input = photonLabel
)

#produce photons using 0.3 radius isolation taken straight from slimmedPhotons
process.icPhotonProducer = producers.icPhotonProducer.clone(
                                          input = photonLabel,
                                          branch = cms.string("photons"),
                                          includeElectronVeto=cms.bool(True),
                                          inputElectronVeto=cms.InputTag("icPhotonElectronConversionVetoCalculator"),
                                          includeHadTowOverEm=cms.bool(True),
                                          includePFIso03=cms.bool(True),
                                          includeIsoFromPat=cms.bool(True) 
)

process.icPhotonSequence += cms.Sequence(
  process.icPhotonElectronConversionVetoCalculator+
  process.icPhotonProducer
)

################################################################## 
#  Jets and rho
#################################################################

from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets

#Make ak4 CHS jets:
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.pfchs=cms.EDFilter("CandPtrSelector",src=cms.InputTag("packedPFCandidates"),cut=cms.string("fromPV"))
process.ak4PFJetsCHS = ak4PFJets.clone(src='pfchs',doAreaFastjet=True)

#reapply JEC
# from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetCorrFactorsUpdated
# process.load('PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff')
# process.patJetCorrFactorsReapplyJEC = process.patJetCorrFactorsUpdated.clone(
#   src = cms.InputTag("slimmedJets"),
#   levels = ['L1FastJet', 
#         'L2Relative', 
#         'L3Absolute'],
#   payload = 'AK4PFchs' ) # Make sure to choose the appropriate levels and payload here!

# from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetsUpdated
# process.patJetsReapplyJEC = process.patJetsUpdated.clone(
#   jetSource = cms.InputTag("slimmedJets"),
#   jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJEC"))
#   )

#Also make non-chs jets:
#process.ak4PFJets = ak4PFJets.clone(src='packedPFCandidates',doAreaFastjet=True)
 
#Get slimmedJets direct from miniAOD
process.selectedSlimmedJetsAK4 = cms.EDFilter("PATJetRefSelector",
                                              src = cms.InputTag("slimmedJets"),
                                              cut = cms.string("pt > 15")
                                              )

#Get re-jec'd slimmed jets
#process.selectedReJECSlimmedJetsAK4 = cms.EDFilter("PATJetRefSelector",
#                                              src = cms.InputTag("patJetsReapplyJEC"),
#                                              cut = cms.string("pt > 15")
#                                              )

#Get slimmedJetsPuppi direct from miniAOD
process.selectedSlimmedJetsPuppiAK4 = cms.EDFilter("PATJetRefSelector",
                                              src = cms.InputTag("slimmedJetsPuppi"),
                                              cut = cms.string("pt > 15")
                                              )

# get parton flavour by matching to genjets for reclustered
#CHS
#process.jetPartonsforCHS = cms.EDProducer("PartonSelector",
 #                                   src = cms.InputTag("prunedGenParticles"),
  #                                  withLeptons = cms.bool(False)
   #                                 )

#process.pfchsJetPartonMatches = cms.EDProducer("JetPartonMatcher",
 #                                           jets = cms.InputTag("ak4PFJetsCHS"),
  #                                          coneSizeToAssociate = cms.double(0.3),
   #                                         partons = cms.InputTag("jetPartonsforCHS")
    #                                        )

#process.pfchsJetFlavourAssociation = cms.EDProducer("JetFlavourIdentifier",
                                                 #srcByReference = cms.InputTag("pfchsJetPartonMatches"),
                                                 #physicsDefinition = cms.bool(False)
                                                 #)

# select hadrons and partons for the jet flavour
process.selectedHadronsAndPartons = cms.EDProducer('HadronAndPartonSelector',
    src = cms.InputTag("generator"),
    particles = cms.InputTag("prunedGenParticles"),
    partonMode = cms.string("Auto")
)

process.pfchsJetFlavourAssociation = cms.EDProducer("JetFlavourClustering",
    jets                     = cms.InputTag("ak4PFJetsCHS"),
    bHadrons                 = cms.InputTag("selectedHadronsAndPartons","bHadrons"),
    cHadrons                 = cms.InputTag("selectedHadronsAndPartons","cHadrons"),
    partons                  = cms.InputTag("selectedHadronsAndPartons","algorithmicPartons"),
    leptons                  = cms.InputTag("selectedHadronsAndPartons","leptons"),
    jetAlgorithm             = cms.string("AntiKt"),
    rParam                   = cms.double(0.4),
    ghostRescaling           = cms.double(1e-18),
    hadronFlavourHasPriority = cms.bool(False)
)

process.icPFchsJetFlavourCalculator = cms.EDProducer('ICJetFlavourCalculator',
    input       = cms.InputTag("ak4PFJetsCHS"),
    flavourMap  = cms.InputTag("pfchsJetFlavourAssociation")
  )

# Jet energy corrections for reclustered
#CHS

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
pfchsJECS = cms.PSet(
  L1FastJet  = cms.string("ak4PFL1FastjetCHS"),
  L2Relative = cms.string("ak4PFL2RelativeCHS"),
  L3Absolute = cms.string("ak4PFL3AbsoluteCHS")
)
if isData: pfchsJECS = cms.PSet(
  L1FastJet  = cms.string("ak4PFL1FastjetCHS"),
  L2Relative = cms.string("ak4PFL2RelativeCHS"),
  L3Absolute = cms.string("ak4PFL3AbsoluteCHS"),
  L2L3Residual = cms.string("ak4PFResidualCHS")
)

# b-tagging for reclustered
process.load("RecoJets.JetAssociationProducers.ak4JTA_cff")
from RecoJets.JetAssociationProducers.ak4JTA_cff import ak4JetTracksAssociatorAtVertex
process.load("RecoBTag.Configuration.RecoBTag_cff")
import RecoBTag.Configuration.RecoBTag_cff as btag
process.jetTracksAssociatorAtVertexAK4PFCHS = ak4JetTracksAssociatorAtVertex.clone(
  jets = cms.InputTag("ak4PFJetsCHS"),
  tracks = cms.InputTag("unpackedTracksAndVertices"),
  pvSrc = cms.InputTag("unpackedTracksAndVertices")
  )

process.impactParameterTagInfosAK4PFCHS = btag.pfImpactParameterTagInfos.clone(
  #jetTracks = cms.InputTag('jetTracksAssociatorAtVertexAK4PF'),
  computeGhostTrack=cms.bool(False),
  candidates=cms.InputTag("packedPFCandidates"),
  primaryVertex = cms.InputTag("unpackedTracksAndVertices")
  )

process.secondaryVertexTagInfosAK4PFCHS = btag.pfSecondaryVertexTagInfos.clone(
  trackIPTagInfos = cms.InputTag('impactParameterTagInfosAK4PFCHS')
  )
process.simpleSecondaryVertexHighEffBJetTagsAK4PFCHS = btag.pfSimpleSecondaryVertexHighEffBJetTags.clone (
  tagInfos = cms.VInputTag('secondaryVertexTagInfosAK4PFCHS')
  )
process.simpleSecondaryVertexHighPurBJetTagsAK4PFCHS = btag.pfSimpleSecondaryVertexHighPurBJetTags.clone (
  tagInfos = cms.VInputTag('secondaryVertexTagInfosAK4PFCHS')
  )
process.combinedSecondaryVertexBJetTagsAK4PFCHS = btag.pfCombinedSecondaryVertexBJetTags.clone (
  tagInfos = cms.VInputTag('impactParameterTagInfosAK4PFCHS', 'secondaryVertexTagInfosAK4PFCHS')
  )


process.btaggingSequenceAK4PFCHS = cms.Sequence(
  process.jetTracksAssociatorAtVertexAK4PFCHS
  +process.impactParameterTagInfosAK4PFCHS
  +process.secondaryVertexTagInfosAK4PFCHS
  +process.simpleSecondaryVertexHighEffBJetTagsAK4PFCHS
  +process.simpleSecondaryVertexHighPurBJetTagsAK4PFCHS
  +process.combinedSecondaryVertexBJetTagsAK4PFCHS
 )

process.btaggingSequenceAK4PFCHS += cms.Sequence(
  process.pfImpactParameterTagInfos+
  process.pfSecondaryVertexTagInfos+
  process.pfCombinedSecondaryVertexV2BJetTags
)

if release in ['76XMINIAOD']:
  process.pfImpactParameterTagInfos.primaryVertex = cms.InputTag("offlineSlimmedPrimaryVertices")
  process.pfImpactParameterTagInfos.candidates = cms.InputTag("packedPFCandidates")

# Pileup ID !!DOESN'T WORK IN AK4PF AT THE MOMENT LOOK INTO IF WE USE THAT
stdalgoschs = cms.VPSet()
from RecoJets.JetProducers.PileupJetIDParams_cfi import *
stdalgoschs = cms.VPSet(full_5x_chs,cutbased)

process.puJetMvaCHS = cms.EDProducer('PileupJetIdProducer',
                                  produceJetIds = cms.bool(True),
                                  jetids = cms.InputTag(""),
                                  runMvas = cms.bool(True),
                                  #jets = cms.InputTag("slimmedJets"),
                                  jets = cms.InputTag("ak4PFJetsCHS"),
                                  vertexes = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                  algos = cms.VPSet(stdalgoschs),
                                  rho     = cms.InputTag("fixedGridRhoFastjetAll"),
                                  jec     = cms.string("AK4PFchs"),
                                  applyJec = cms.bool(True),
                                  inputIsCorrected = cms.bool(False),
                                  residualsFromTxt = cms.bool(False),
                                  residualsTxt = cms.FileInPath("RecoJets/JetProducers/BuildFile.xml")
                                  )

# Produce and store reclustered CHS

jetsrcconfig=cms.PSet(
  includeJetFlavour         = cms.bool(True),
  inputJetFlavour           = cms.InputTag("icPFchsJetFlavourCalculator"),
  applyJECs                 = cms.bool(True),
  includeJECs               = cms.bool(True),
  JECs                      = pfchsJECS,
  applyCutAfterJECs         = cms.bool(True),
  cutAfterJECs              = cms.string("pt > 15.0"),
  inputSVInfo               = cms.InputTag(""),
  requestSVInfo             = cms.bool(False),
  BTagDiscriminators        = cms.PSet(
    simpleSecondaryVertexHighEffBJetTags = cms.InputTag("simpleSecondaryVertexHighEffBJetTagsAK4PFCHS"),
    simpleSecondaryVertexHighPurBJetTags = cms.InputTag("simpleSecondaryVertexHighPurBJetTagsAK4PFCHS"),
    combinedSecondaryVertexBJetTags      = cms.InputTag("combinedSecondaryVertexBJetTagsAK4PFCHS"),
    pfCombinedSecondaryVertexV2BJetTags = cms.InputTag("pfCombinedSecondaryVertexV2BJetTags")
    )
  )

if isData:
  jetsrcconfig.includeJetFlavour = cms.bool(False)

process.icPFJetProducer = producers.icPFJetProducer.clone(
  branch                    = cms.string("pfJetsPFlowreclustered"),
  input                     = cms.InputTag("ak4PFJetsCHS"),
  srcConfig = jetsrcconfig,
  destConfig = cms.PSet(
    includePileupID       = cms.bool(True), 
    inputPileupID         = cms.InputTag("puJetMvaCHS", "fullDiscriminant"),
    includeTrackBasedVars = cms.bool(False),
    inputTracks           = cms.InputTag("generalTracks"),
    inputVertices         = vtxLabel,
    requestTracks         = cms.bool(False)
    )
  )

#Produce and store jets from MiniAOD with updated JEC
# process.icReJECPFJetProducerFromPat = producers.icPFJetFromPatProducer.clone(
#   branch                    = cms.string("pfJetsPFlowReJEC"),
#   input                     = cms.InputTag("selectedReJECSlimmedJetsAK4"),
#   srcConfig = cms.PSet(
#     isSlimmed               = cms.bool(True),
#     includeJetFlavour       = cms.bool(True),
#     includeJECs             = cms.bool(False),
#     inputSVInfo             = cms.InputTag(""),
#     requestSVInfo           = cms.bool(False)
#     ),
#   destConfig = cms.PSet(
#     includePileupID         = cms.bool(True),
#     inputPileupID           = cms.InputTag("puJetMvaCHS", "fullDiscriminant"),
#     includeTrackBasedVars   = cms.bool(False),
#     inputTracks             = cms.InputTag("unpackedTracksAndVertices"),#!!check this and line below
#     inputVertices           = cms.InputTag("unpackedTracksAndVertices"),#!!
#     requestTracks           = cms.bool(False)
#     )
#   )

#Produce and store jets taken straight from miniAOD
process.icPFJetProducerFromPat = producers.icPFJetFromPatProducer.clone(
  branch                    = cms.string("pfJetsPFlow"),
  input                     = cms.InputTag("selectedSlimmedJetsAK4"),
  srcConfig = cms.PSet(
    isSlimmed               = cms.bool(True),
    includeJetFlavour       = cms.bool(True),
    includeJECs             = cms.bool(True),
    JECs                    = pfchsJECS,
    inputSVInfo             = cms.InputTag(""),
    requestSVInfo           = cms.bool(False)
    ),
  destConfig = cms.PSet(
    includePileupID         = cms.bool(True),
    inputPileupID           = cms.InputTag("puJetMvaCHS", "fullDiscriminant"),
    includeTrackBasedVars   = cms.bool(False),
    inputTracks             = cms.InputTag("unpackedTracksAndVertices"),#!!check this and line below
    inputVertices           = cms.InputTag("unpackedTracksAndVertices"),#!!
    requestTracks           = cms.bool(False)
    )
  )

process.icPFJetProducerFromPatPuppi = producers.icPFJetFromPatProducer.clone(
  branch                    = cms.string("ak4SlimmedJetsPuppi"),
  input                     = cms.InputTag("selectedSlimmedJetsPuppiAK4"),
  srcConfig = cms.PSet(
    isSlimmed               = cms.bool(True),
    includeJetFlavour       = cms.bool(True),
    includeJECs             = cms.bool(True),
    inputSVInfo             = cms.InputTag(""),
    requestSVInfo           = cms.bool(False)
    ),
  destConfig = cms.PSet(
    includePileupID         = cms.bool(False),
    inputPileupID           = cms.InputTag("puJetMvaCHS", "fullDiscriminant"),
    includeTrackBasedVars   = cms.bool(False),
    inputTracks             = cms.InputTag("unpackedTracksAndVertices"),#!!check this and line below
    inputVertices           = cms.InputTag("unpackedTracksAndVertices"),#!!
    requestTracks           = cms.bool(False)
    )
  )

process.icPFJetSequence = cms.Sequence()

process.icPFJetSequence += cms.Sequence(
  process.pfchs+
#  process.patJetCorrFactorsReapplyJEC +
#  process.patJetsReapplyJEC+
#  process.selectedReJECSlimmedJetsAK4+
  process.selectedSlimmedJetsAK4+
  process.selectedSlimmedJetsPuppiAK4+
  process.unpackedTracksAndVertices+
  process.ak4PFL1FastjetCHS+
  process.ak4PFL2RelativeCHS+
  process.ak4PFL3AbsoluteCHS+
  process.ak4PFResidualCHS+
  process.ak4PFJetsCHS+
  process.puJetMvaCHS
)

if not isData:
    process.icPFJetSequence.remove(process.ak4PFResidualCHS)

#  process.ak4PFJets+
if not isData:
  process.icPFJetSequence += cms.Sequence(
    #process.jetPartonsforCHS+
    #process.pfchsJetPartonMatches+
    process.selectedHadronsAndPartons+
    process.pfchsJetFlavourAssociation+
    process.icPFchsJetFlavourCalculator
    )
process.icPFJetSequence += cms.Sequence(
  process.btaggingSequenceAK4PFCHS+
  process.icPFJetProducer+ #Not from slimmed jets!
  process.icPFJetProducerFromPat+
  #process.icReJECPFJetProducerFromPat+
  process.icPFJetProducerFromPatPuppi
  )


##################################################################                                                                                          
#  MET                                                                                                                                                    
#!!currently type 1 met only, no type0pc+type1 recipe and also haven't implemented mva met
#################################################################

#Make uncorrected pfMet from packedPF
from RecoMET.METProducers.PFMET_cfi import pfMet

process.icPfMet = pfMet.clone(src = "packedPFCandidates")
process.icPfMet.calculateSignificance = False # this can't be easily implemented on packed PF candidates at the moment

process.icuncorrectedPfMetProducer = producers.icMetProducer.clone(
                                                    input = cms.InputTag("icPfMet"),
                                                    branch = cms.string("uncorrectedpfMet"),
                                                    includeCustomID = cms.bool(False),
                                                    inputCustomID = cms.InputTag(""),
                                                    )


#setup met significance calculator for
process.load("RecoMET/METProducers.METSignificance_cfi")
process.load("RecoMET/METProducers.METSignificanceParams_cfi")

#!!clone met sig producer with different name and input for met uncertainties

#get genmet
process.icGenMetProducer = producers.icMetFromPatProducer.clone(
  input = cms.InputTag("slimmedMETs"),
  branch = cms.string("genMetCollection"),
  includeCustomID = cms.bool(False),
  inputCustomID = cms.InputTag(""),
  doGenMet = cms.bool(True)
  )

#get type 1 met straight from miniAOD !!update to take in output of met significance calculator
process.ictype1PfMetProducer = producers.icMetFromPatProducer.clone(
  input = cms.InputTag("slimmedMETs"),
  branch = cms.string("pfMetType1Collection"),
  includeCustomID = cms.bool(False),
  inputCustomID = cms.InputTag(""),
  includeExternalMetsigMethod2 = cms.bool(True),
  includeMetCorrections = cms.bool(True),
  includeMetUncertainties = cms.bool(True)
  )

#puppi met, need to update significance....
process.icPfMetPuppiProducer = producers.icMetFromPatProducer.clone(
  input = cms.InputTag("slimmedMETsPuppi"),
  branch = cms.string("pfMetPuppiCollection"),
  includeCustomID = cms.bool(False),
  inputCustomID = cms.InputTag(""),
  includeMetCorrections = cms.bool(False),
  includeMetUncertainties = cms.bool(False)
  )

# process.ictype1PfMetProducermetsigoutofbox = producers.icMetProducer.clone(
#                                                     input = cms.InputTag("slimmedMETs"),
#                                                     branch = cms.string("pfMetType1metsigoutofbox"),
#                                                     includeCustomID = cms.bool(False),
#                                                     inputCustomID = cms.InputTag(""),
#                                                     includeExternalMetsig = cms.bool(False)
#                                                     )

process.icMetSequence = cms.Sequence(
  process.METSignificance+
  process.icPfMet+
  process.icuncorrectedPfMetProducer+
  process.ictype1PfMetProducer+
  process.icPfMetPuppiProducer
  #+process.icMvaMetSequence
  #process.ictype1PfMetProducermetsigoutofbox
  #process.correctionTermsPfMetType1Type2+ #!!needs particle flow, need to find appropriate bit and change to packed version
  #process.correctionTermsPfMetType0PFCandidate + #!!currently causing errors
  #process.pfMetT0pcT1+
  #process.icPfMetT0pcT1Producer
)

#!!MET UNCERTAINTIES
#!!MET FILTERS


################################################################                                                                                            
#!! Simulation only: GenParticles, GenJets, PileupInfo                                                                                                        
################################################################                                                                                            
process.icGenSequence = cms.Sequence()

#process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
#process.icPrunedGenParticles = cms.EDProducer("GenParticlePruner",
#  src = cms.InputTag("prunedGenParticles","","PAT"),
#  select = cms.vstring(
#    "drop  *",
#    "keep status == 3 || status == 22 || status == 23",  # all status 3
#    "keep abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15",  # all charged leptons
#    "keep abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16",  # all neutrinos
#    "keep++ abs(pdgId) == 15",  # keep full tau decay chain
#    "keep (4 <= abs(pdgId) <= 6)", # keep heavy flavour quarks !!note extended to top quarks as well
#    "keep (21 <= abs(pdgId) <= 25)", # keep bosons
#    "keep (400 <= abs(pdgId) < 600) || (4000 <= abs(pdgId) < 6000)", # keep b and c hadrons
#    "keep abs(pdgId) = 10411 || abs(pdgId) = 10421 || abs(pdgId) = 10413 || abs(pdgId) = 10423 || abs(pdgId) = 20413 || abs(pdgId) = 20423 || abs(pdgId) = 10431 || abs(pdgId) = 10433 || abs(pdgId) = 20433", # additional c hadrons for jet fragmentation studies
#    "keep abs(pdgId) = 10511 || abs(pdgId) = 10521 || abs(pdgId) = 10513 || abs(pdgId) = 10523 || abs(pdgId) = 20513 || abs(pdgId) = 20523 || abs(pdgId) = 10531 || abs(pdgId) = 10533 || abs(pdgId) = 20533 || abs(pdgId) = 10541 || abs(pdgId) = 10543 || abs(pdgId) = 20543" # additional b hadrons for jet fragmentation studies
#  )
#)

#process.prunedGenParticlesTaus = cms.EDProducer("ICGenParticlePruner53X",
#  src = cms.InputTag("prunedGenParticles","","PAT"),
#  select = cms.vstring(
#    "drop  *",
#    "keep++ abs(pdgId) == 15",  # keep full tau decay chain
#  )
#)

#Store ALL pruned gen particles
process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  input   = cms.InputTag("prunedGenParticles","","PAT"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True),
  includeStatusFlags = cms.bool(True)
)

#process.icGenParticleTauProducer = producers.icGenParticleProducer.clone(
#  input   = cms.InputTag("prunedGenParticlesTaus"),
#  branch = cms.string("genParticlesTaus"),
#  includeMothers = cms.bool(True),
#  includeDaughters = cms.bool(True),
#  includeStatusFlags = cms.bool(True)
#)


process.load("RecoJets.JetProducers.ak4GenJets_cfi")
process.ak4GenJetsNoNuBSM  =  process.ak4GenJets.clone()
process.ak4GenJetsNoNuBSM.src=cms.InputTag("packedGenParticles") #This still contains nus in 72, should be fixed in 74

process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
  src = cms.InputTag("ak4GenJetsNoNuBSM"),
  cut = cms.string("pt > 10.0")
)

process.icGenJetProducer = producers.icGenJetProducer.clone(
  branch = cms.string("genJetsReclustered"),
  input   = cms.InputTag("selectedGenJets"),
  inputGenParticles = cms.InputTag("prunedGenParticles","","PAT"),
  requestGenParticles = cms.bool(False),
  isSlimmed  = cms.bool(True)#set true because based on gen jets made from packedcandidates
)

process.icGenJetProducerFromSlimmed = producers.icGenJetProducer.clone(
  branch = cms.string("genJets"),
  input = cms.InputTag("slimmedGenJets"),
  inputGenParticles=cms.InputTag("genParticles"),
  requestGenParticles = cms.bool(False),
  isSlimmed = cms.bool(True)
  ) 

process.icPileupInfoProducer = producers.icPileupInfoProducer.clone(
  input   = cms.InputTag("slimmedAddPileupInfo")
  )


if not isData:
  process.icGenSequence += (
    #process.icPrunedGenParticles+
    #process.prunedGenParticlesTaus+
    #process.icGenParticleTauProducer+
    process.icGenParticleProducer
  )
  process.icGenSequence += (
      process.ak4GenJetsNoNuBSM+
      process.selectedGenJets+
      process.icGenJetProducer+
      process.icGenJetProducerFromSlimmed+
      process.icGenMetProducer+
      process.icPileupInfoProducer
    )
#  if release in [ '74X']:
#    process.icGenSequence += (
#      process.ak4GenJetsNoNuBSM+
#      process.selectedGenJets+
#      process.icGenJetProducer
#      process.icPileupInfoProducer
#    )

##################################################################                                                                                          
#!! Trigger                                                                                                                                                 
#!! Need to get L1 extra
################################################################## 

from PhysicsTools.PatAlgos.tools.trigTools import *
process.icTriggerSequence = cms.Sequence()
process.icTriggerObjectSequence = cms.Sequence()

#process.patTriggerPath = cms.Path()
process.patTriggerPath = cms.Path()
if release in ['76XMINIAOD']:
  switchOnTrigger(process, path = 'patTriggerPath',  outputModule = '')
#switchOnTrigger(process, path = 'patTriggerPath', outputModule = '')
#switchOnTrigger(process, path = 'patTrigger', outputModule = '')

process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
  branch = cms.string("triggerPaths"),
  inputIsStandAlone = cms.bool(True),
  input = cms.InputTag("TriggerResults", "", "HLT"),
  inputPrescales=cms.InputTag("patTrigger")
  )

#if isData:
process.icTriggerSequence += cms.Sequence(
  #process.patTrigger+
  #process.patTriggerEvent+
  process.icTriggerPathProducer
  )

process.icPFMET170NoiseCleanedObjectProducer = producers.icTriggerObjectProducer.clone(
  branch = cms.string("triggerObjectsPFMET170NoiseCleaned"),
  input   = cms.InputTag("patTriggerEvent"),
  hltPath = cms.string("HLT_PFMET170_NoiseCleaned"),
  inputIsStandAlone = cms.bool(False),
  storeOnlyIfFired = cms.bool(True)
  )

process.icDiPFJet40DEta3p5MJJ600PFMETNoMu140ObjectProducer = producers.icTriggerObjectProducer.clone(
  branch = cms.string("triggerObjectsDiPFJet40DEta3p5MJJ600PFMETNoMu140"),
  input   = cms.InputTag("patTriggerEvent"),
  hltPath = cms.string("HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu140"),
  inputIsStandAlone = cms.bool(False),
  storeOnlyIfFired = cms.bool(True)
  )

process.icDiPFJet40DEta3p5MJJ600PFMETNoMu80ObjectProducer = producers.icTriggerObjectProducer.clone(
  branch = cms.string("triggerObjectsDiPFJet40DEta3p5MJJ600PFMETNoMu80"),
  input   = cms.InputTag("patTriggerEvent"),
  hltPath = cms.string("HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu80"),
  inputIsStandAlone = cms.bool(False),
  storeOnlyIfFired = cms.bool(True)
  )

process.icTriggerObjectSequence += cms.Sequence(
  process.icPFMET170NoiseCleanedObjectProducer+
  process.icDiPFJet40DEta3p5MJJ600PFMETNoMu140ObjectProducer+
  process.icDiPFJet40DEta3p5MJJ600PFMETNoMu80ObjectProducer
  )

for name in process.icTriggerObjectSequence.moduleNames():
  mod = getattr(process, name)
  mod.inputIsStandAlone = cms.bool(True)
  mod.input = cms.InputTag("selectedPatTrigger")


#L1 Extra information
process.icL1ExtraMETProducer = cms.EDProducer('ICL1EtMissProducer',
  branch = cms.string("l1extraMET"),
  input = cms.InputTag("l1extraParticles","MET","RECO"),
  )

process.icL1ExtraMHTProducer = cms.EDProducer('ICL1EtMissProducer',
  branch = cms.string("l1extraMHT"),
  input = cms.InputTag("l1extraParticles","MHT","RECO"),
  )

#process.icL1ExtraHTTProducer = cms.EDProducer('ICCandidateProducer',
#  branch = cms.string("l1extraHTT"),
#  input = cms.InputTag("l1extraParticles","MHT","RECO"),
#)

process.icL1ExtraMuonsProducer = cms.EDProducer('ICCandidateFromL1MuonProducer',
  branch = cms.string("l1extraMuons"),
  input = cms.InputTag("l1extraParticles","","RECO"),
  )

process.icL1ExtraEmIsolatedProducer = cms.EDProducer('ICCandidateProducer',
  branch = cms.string("l1extraEmIsolated"),
  input = cms.InputTag("l1extraParticles","Isolated","RECO"),
  )

process.icL1ExtraEmNonIsolatedProducer = cms.EDProducer('ICCandidateProducer',
  branch = cms.string("l1extraEmNonIsolated"),
  input = cms.InputTag("l1extraParticles","NonIsolated","RECO"),
  )

process.icL1ExtraCentralJetProducer = cms.EDProducer('ICCandidateProducer',
  branch = cms.string("l1extraCentralJets"),
  input = cms.InputTag("l1extraParticles","Central","RECO"),
)

process.icL1ExtraForwardJetProducer = cms.EDProducer('ICCandidateProducer',
  branch = cms.string("l1extraForwardJets"),
  input = cms.InputTag("l1extraParticles","Forward","RECO"),
)

process.icL1ExtraSequence = cms.Sequence(
    process.icL1ExtraMETProducer
    +process.icL1ExtraMHTProducer
    +process.icL1ExtraMuonsProducer
    +process.icL1ExtraEmIsolatedProducer
    +process.icL1ExtraEmNonIsolatedProducer
    +process.icL1ExtraCentralJetProducer
    +process.icL1ExtraForwardJetProducer
    )


################################################################
# EventInfo
################################################################

#process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
#process.HBHENoiseFilterResultProducer.minZeros = cms.int32(99999)
#process.HBHENoiseFilterResultProducer.IgnoreTS4TS5ifJetInLowBVRegion=cms.bool(False) 
#process.HBHENoiseFilterResultProducer.defaultDecision = cms.string("HBHENoiseFilterResultRun2Loose")


#process.HBHEISONoiseFilterResultProducer = process.HBHENoiseFilterResultProducer.clone()
#process.HBHEISONoiseFilterResultProducer.defaultDecision = cms.string("HBHEIsoNoiseFilterResult")

process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  includeJetRho       = cms.bool(True),
  inputJetRho         = cms.InputTag("fixedGridRhoFastjetAll"),
  includeLeptonRho    = cms.bool(False),
  inputLeptonRho      = cms.InputTag("fixedGridRhoFastjetAll"),
  includeVertexCount  = cms.bool(True),
  inputVertices       = vtxLabel,
  includeCSCFilter    = cms.bool(False),
  inputCSCFilter      = cms.InputTag("BeamHaloSummary"),
  includeFiltersFromTrig = cms.bool(True),
  inputfiltersfromtrig = cms.InputTag("TriggerResults","","RECO"),
  filtersfromtrig     = cms.vstring("*"),
  #filters             = cms.PSet(
   # Flag_HBHENoiseFilter = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
   # Flag_HBHENoiseIsoFilter = cms.InputTag('HBHENoiseFilterResultProducer','HBHEIsoNoiseFilterResult'),
   # )
)


if doHT:
  process.icEventInfoProducer.includeHT = cms.bool(True)

 
process.icEventInfoSequence = cms.Sequence(
#  process.HBHENoiseFilterResultProducer+
# process.HBHEISONoiseFilterResultProducer+
  process.icEventInfoProducer
)
################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()


process.p = cms.Path(
  process.ic76XSequence+
  process.icMiniAODSequence+
  process.icSelectionSequence+
  process.pfParticleSelectionSequence+
  process.icVertexSequence+
  process.icElectronSequence+
  process.icMuonSequence+
  process.icTauSequence+
  process.icPhotonSequence+
  process.icPFJetSequence+
  process.icGenSequence+
  process.icMetSequence+
  process.icEventInfoSequence+
  process.icTriggerSequence+
  process.icTriggerObjectSequence+
  process.icL1ExtraSequence+
  process.icEventProducer
)


#process.out = cms.OutputModule("PoolOutputModule",
#                               fileName = cms.untracked.string("edmdump.root")
#                               )

#process.outpath = cms.EndPath(process.out)


#process.schedule = cms.Schedule(process.patTriggerPath, process.p)                                                                                        
process.schedule = cms.Schedule(process.p)

#make an edm output ntuple with everything in it
#process.schedule = cms.Schedule(process.p,process.outpath)



#Uncomment below for a dump of the config
#print process.dumpPython()
