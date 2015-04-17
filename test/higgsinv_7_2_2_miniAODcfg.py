import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################                                                                                         
# Setup and Read Options                                                                                                                          
################################################################                                                                                           
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
opts.register('file', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "input file")
opts.register('globalTag', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('isData', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as data?")
opts.register('release', '', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")

opts.parseArguments()
infile      = opts.file
if not infile: infile = "file:/tmp/file.root"
tag         = opts.globalTag
isData      = opts.isData
release     = opts.release

if not release in ["72X", "72XMINIAOD"]:
  print 'Release not recognised, exiting!'
  sys.exit(1)
print 'release     : '+release
print 'isData      : '+str(isData)
print 'globalTag   : '+tag


################################################################                                                                                          
# Standard setup                                                                                                                                           
################################################################                                                                                          
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.TFileService = cms.Service("TFileService",
  fileName = cms.string("EventTree.root"),
  closeFileFast = cms.untracked.bool(True)
)

################################################################                                                                                        
# Message Logging, summary, and number of events                                                                                                          
################################################################                                                                                          
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(100)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 50

process.options   = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)

################################################################                                                                                       
# Input files and global tags                                                                                                                            
################################################################                                                                                         
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(infile))
process.GlobalTag.globaltag = cms.string(tag)

# 72X MC: file=root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/AODSIM/PU40bx25_PHYS14_25_V1-v1/00000/00E63918-3A70-E411-A246-7845C4FC35F3.root globalTag=START72_V1::All                                                                                                               
# 72XMINIAOD MC: file=root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/36224FE2-0571-E411-9664-00266CFAE30C.root globalTag=START72_V1::All                                                                                                    
import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors


################################################################                                                                                       
# Declare sequences                                                                                                                           
################################################################                                                                                         
process.ic72XSequence = cms.Sequence()
process.icMiniAODSequence = cms.Sequence()


################################################################                                                                                       
# Object selection                                                                                                                            
################################################################                                                                                         
process.selectedVertices = cms.EDFilter("VertexRefSelector",
  src = cms.InputTag("offlineSlimmedPrimaryVertices"),
  cut = cms.string("ndof >= 4 & abs(z) <= 24 & abs(position.Rho) <= 2")
)

process.selectedElectrons = cms.EDFilter("PATElectronRefSelector",
  src = cms.InputTag("slimmedElectrons"),
  cut = cms.string("pt > 5.0 & abs(eta) < 2.6")
)

process.selectedPFMuons = cms.EDFilter("PATMuonRefSelector",
  src = cms.InputTag("slimmedMuons"),
  cut = cms.string("pt > 3.0 & abs(eta) < 2.6")
)

process.selectedPFTaus = cms.EDFilter("PATTauRefSelector",
  src = cms.InputTag("slimmedTaus"),
  cut = cms.string('pt > 18.0 & abs(eta) < 2.6 & tauID("decayModeFinding") > 0.5')
)

process.icSelectionSequence = cms.Sequence(
  process.selectedVertices+
  process.selectedElectrons+
  process.selectedPFMuons+
  process.selectedPFTaus
)


################################################################                                                                                            
# PF sequence for lepton isolation                                             
#!!andrew custom code to redo iso, look into taking default miniaod values by  
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

################################################################                                                                                          
# Vertices                                                                                                                                                 
################################################################                                                                                           
process.icVertexProducer = producers.icVertexProducer.clone(
  branch  = cms.string("vertices"),
  input = cms.InputTag("selectedVertices"),
  firstVertexOnly = cms.bool(True)
)

process.icVertexSequence = cms.Sequence(
  process.icVertexProducer
)

################################################################                                                                                           
# PFCandidates                                                                                                                                             
#!!IF WE WANT THESE WE SHOULD MAKE SOMETHING OURSELVES FROM PACKED PFCANDIDATES 
#!!ANDREW JUST FINISHED MAKING AN ICPRODUCER THAT DOES THIS 17/04/2015
################################################################                                                                                            

################################################################                                                                                            
# Tracks                                                                                                                                                   
#!!THERE IS AN EXAMPLE CFI ON THE MINIAOD TWIKI WITH HOW TO GET AN APPROXIMATION OF THE TRACKS IN MINIAOD 
################################################################                                                                                            

#!!CHECKED UP TO HERE

################################################################                                                                                           
# Electrons                                                                                                                                                 
################################################################                                                                                            
electronLabel = cms.InputTag("gedGsfElectrons")
if release in ['72XMINIAOD']: electronLabel = cms.InputTag("slimmedElectrons")

process.icElectronSequence = cms.Sequence()

process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
    input       = electronLabel,
    beamspot    = cms.InputTag("offlineBeamSpot"),
    conversions = cms.InputTag("allConversions")
)
if release in ['70XMINIAOD', '72XMINIAOD']:
  process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionFromPatCalculator',
      input       = electronLabel
  )

if release in ['72X', '72XMINIAOD']:
  process.load("CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff")
  process.elPFIsoValueCharged04PFIdPFIso    = process.elPFIsoValueCharged04PFId.clone()
  process.elPFIsoValueChargedAll04PFIdPFIso = process.elPFIsoValueChargedAll04PFId.clone()
  process.elPFIsoValueGamma04PFIdPFIso      = process.elPFIsoValueGamma04PFId.clone()
  process.elPFIsoValueNeutral04PFIdPFIso    = process.elPFIsoValueNeutral04PFId.clone()
  process.elPFIsoValuePU04PFIdPFIso         = process.elPFIsoValuePU04PFId.clone()
  process.electronPFIsolationValuesSequence = cms.Sequence(
      process.elPFIsoValueCharged04PFIdPFIso+
      process.elPFIsoValueChargedAll04PFIdPFIso+
      process.elPFIsoValueGamma04PFIdPFIso+
      process.elPFIsoValueNeutral04PFIdPFIso+
      process.elPFIsoValuePU04PFIdPFIso
      )
  process.elPFIsoDepositCharged.src     = electronLabel
  process.elPFIsoDepositChargedAll.src  = electronLabel
  process.elPFIsoDepositNeutral.src     = electronLabel
  process.elPFIsoDepositGamma.src       = electronLabel
  process.elPFIsoDepositPU.src          = electronLabel
  if release in ['70XMINIAOD', '72XMINIAOD']:
    process.elPFIsoDepositGamma.ExtractorPSet.ComponentName = cms.string("CandViewExtractor")
  process.icElectronSequence += cms.Sequence(
      process.electronPFIsolationDepositsSequence+
      process.electronPFIsolationValuesSequence
      )

process.elPFIsoValueGamma04PFIdPFIso.deposits[0].vetos = (
    cms.vstring('EcalEndcaps:ConeVeto(0.08)','EcalBarrel:ConeVeto(0.08)'))
process.elPFIsoValueNeutral04PFIdPFIso.deposits[0].vetos = (
    cms.vstring())
process.elPFIsoValuePU04PFIdPFIso.deposits[0].vetos = (
    cms.vstring())
process.elPFIsoValueCharged04PFIdPFIso.deposits[0].vetos = (
    cms.vstring('EcalEndcaps:ConeVeto(0.015)'))
process.elPFIsoValueChargedAll04PFIdPFIso.deposits[0].vetos = (
    cms.vstring('EcalEndcaps:ConeVeto(0.015)','EcalBarrel:ConeVeto(0.01)'))

process.icElectronProducer = producers.icElectronProducer.clone(
  branch                    = cms.string("electrons"),
  input                     = cms.InputTag("selectedElectrons"),
  includeConversionMatches  = cms.bool(True),
  inputConversionMatches    = cms.InputTag("icElectronConversionCalculator"),
  includeVertexIP           = cms.bool(True),
  inputVertices             = cms.InputTag("selectedVertices"),
  includeBeamspotIP         = cms.bool(True),
  inputBeamspot             = cms.InputTag("offlineBeamSpot"),
  includeFloats = cms.PSet(
    # mvaTrigV0       = cms.InputTag("mvaTrigV0"),                                                                                                          
    # mvaNonTrigV0    = cms.InputTag("mvaNonTrigV0"),                                                                                                       
    # trackInIsoSum   = cms.InputTag("icHttElecIsoCheck"),                                                                                                  
    # matchedRecoMuon = cms.InputTag("icHttMuonOverlapCheck")                                                                                               
  ),
  includePFIso04           = cms.bool(True)
)

process.icElectronSequence += cms.Sequence(
  process.icElectronConversionCalculator+
  process.icElectronProducer
)
