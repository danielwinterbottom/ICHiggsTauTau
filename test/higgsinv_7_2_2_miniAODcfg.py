import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################                                                                                         
# Define some strings                                                                                                                          
################################################################                                                                                           
electronLabel = cms.InputTag("slimmedElectrons")
muonLabel = cms.InputTag("slimmedMuons")


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

if not release in ["72XMINIAOD"]:
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
  src = electronLabel,
  cut = cms.string("pt > 5.0 & abs(eta) < 2.6")
)

process.selectedPFMuons = cms.EDFilter("PATMuonRefSelector",
  src = muonLabel,
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
#!!andrew custom code to redo iso, look into taking default miniaod values by making value map
#used as input for CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff
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

################################################################                                                                                           
# Electrons                                                                                                                                                 
################################################################                                                                                            

process.icElectronSequence = cms.Sequence()

#!!CHECK IF WE SHOULD USE MVA ID


#this module kept in src/ICAnalysis/ElectronConversionCalculator/ and is implementation of run 1 egamma conversion check !!make sure up to date
#produces value map of whether electron passes conversion veto
process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionFromPatCalculator', 
    input       = electronLabel
)

#load standard pf electron isolation calculation and make some modifications i.e. add cone veto
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


#run ic producer and use above for iso
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
  includePFIso03           = cms.bool(True)
)

process.icElectronSequence += cms.Sequence(
  process.icElectronConversionCalculator+
  process.icElectronProducer
)

################################################################                                                                                            
# Muons                                                                                                                                                     
################################################################                                                                                            
process.icMuonSequence = cms.Sequence()

#!!CHECK IF WE NEED MUON ID PRODUCER

#redo isolation calculation
process.load("CommonTools.ParticleFlow.Isolation.pfMuonIsolation_cff")
process.muPFIsoValueCharged04PFIso    = process.muPFIsoValueCharged04.clone()
process.muPFIsoValueChargedAll04PFIso = process.muPFIsoValueChargedAll04.clone()
process.muPFIsoValueGamma04PFIso      = process.muPFIsoValueGamma04.clone()
process.muPFIsoValueNeutral04PFIso    = process.muPFIsoValueNeutral04.clone()
process.muPFIsoValuePU04PFIso         = process.muPFIsoValuePU04.clone()
process.muonPFIsolationValuesSequence = cms.Sequence(
    process.muPFIsoValueCharged04PFIso+
    process.muPFIsoValueChargedAll04PFIso+
    process.muPFIsoValueGamma04PFIso+
    process.muPFIsoValueNeutral04PFIso+
    process.muPFIsoValuePU04PFIso
    )
process.muPFIsoDepositCharged.src     = muonLabel
process.muPFIsoDepositChargedAll.src  = muonLabel
process.muPFIsoDepositNeutral.src     = muonLabel
process.muPFIsoDepositGamma.src       = muonLabel
process.muPFIsoDepositPU.src          = muonLabel
process.icMuonSequence += cms.Sequence(
    process.muonPFIsolationDepositsSequence+
    process.muonPFIsolationValuesSequence
    )

#make and store ic muon object
process.icMuonProducer = producers.icMuonProducer.clone(
  branch                    = cms.string("muons"),
  input                     = cms.InputTag("selectedMuons"),
  isPF                      = cms.bool(False),
  includeVertexIP           = cms.bool(True),
  inputVertices             = cms.InputTag("selectedVertices"),
  includeBeamspotIP         = cms.bool(True),
  inputBeamspot             = cms.InputTag("offlineBeamSpot"),
  includeFloats = cms.PSet(
  ),
  includePFIso03           = cms.bool(True)
)

if release in ['72XMINIAOD']: process.icMuonProducer.isPF = cms.bool(False) #!!CHECK THIS

process.icMuonSequence += cms.Sequence(
  process.icMuonProducer
)

#!!CHECKED UP TO HERE

################################################################                                                                                            
# Taus                                                                                                                                                      
################################################################                                                                                            
import UserCode.ICHiggsTauTau.tau_discriminators_cfi as tauIDs

#!!CHECK THIS MAY HAVE BEEN REPLACED BY ICPFTAUPRODUCER
process.icTauProducer = cms.EDProducer("ICPFTauFromPatProducer",
  branch                  = cms.string("taus"),
  input                   = cms.InputTag("selectedPFTaus"),
  inputVertices           = cms.InputTag("selectedVertices"),
  includeVertexIP         = cms.bool(True),
  requestTracks           = cms.bool(False),
  tauIDs = cms.PSet()
)

process.icTauSequence = cms.Sequence(
  process.icTauProducer
)

##################################################################                                                                                          
#  Jets                                                                                                                                                    
#################################################################

##################################################################                                                                                          
#  MET                                                                                                                                                    
#################################################################

################################################################                                                                                            
# Simulation only: GenParticles, GenJets, PileupInfo                                                                                                        
################################################################                                                                                            
process.icGenSequence = cms.Sequence()

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.prunedGenParticles = cms.EDProducer("ICGenParticlePruner",
  src = cms.InputTag("genParticles", "", "SIM"),
  select = cms.vstring(
    "drop  *",
    "keep status == 3 || status == 22 || status == 23",  # all status 3                                                                                     
    "keep abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15",  # all charged leptons                                                                 
    "keep abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16",  # all neutrinos                                                                       
    "keep++ abs(pdgId) == 15",  # keep full tau decay chain                                                                                                 
    "keep (4 <= abs(pdgId) <= 5)", # keep heavy flavour quarks                                                                                              
    "keep (400 <= abs(pdgId) < 600) || (4000 <= abs(pdgId) < 6000)", # keep b and c hadrons                                                                 
    "keep abs(pdgId) = 10411 || abs(pdgId) = 10421 || abs(pdgId) = 10413 || abs(pdgId) = 10423 || abs(pdgId) = 20413 || abs(pdgId) = 20423 || abs(pdgId) = 10431 || abs(pdgId) = 10433 || abs(pdgId) = 20433", # additional c hadrons for jet fragmentation studies                                                     
    "keep abs(pdgId) = 10511 || abs(pdgId) = 10521 || abs(pdgId) = 10513 || abs(pdgId) = 10523 || abs(pdgId) = 20513 || abs(pdgId) = 20523 || abs(pdgId) = 10531 || abs(pdgId) = 10533 || abs(pdgId) = 20533 || abs(pdgId) = 10541 || abs(pdgId) = 10543 || abs(pdgId) = 20543" # additional b hadrons for jet fragmentation studies                                                                                                                                                 
  )
)
if release in ['72X']:
  process.prunedGenParticles.src = cms.InputTag("genParticles","","HLT")
if release in ['72XMINIAOD']:
  process.prunedGenParticles.src = cms.InputTag("prunedGenParticles", "", "PAT")

process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  input   = cms.InputTag("prunedGenParticles"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True)
)

process.load("RecoJets.Configuration.GenJetParticles_cff")
process.genParticlesForJets.ignoreParticleIDs = cms.vuint32(
  1000022, 2000012, 2000014,
  2000016, 1000039, 5000039,
  4000012, 9900012, 9900014,
  9900016, 39, 12, 14, 16
)
if release in ['72XMINIAOD']:
  # But of course this won't work because genParticlesForJets(InputGenJetsParticleSelector)                                                                 
  # requires a vector<GenParticle> input. There's no alternative filter for the PackedGenParticle                                                           
  # type at the moment. Probably we could make our own generic cut-string selector, but                                                                     
  # not in this package                                                                                                                                     
  process.genParticlesForJets.src = cms.InputTag("packedGenParticles")

process.load("RecoJets.JetProducers.ak5GenJets_cfi")
process.ak5GenJetsNoNuBSM  =  process.ak5GenJets.clone()

process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
  src = cms.InputTag("ak5GenJetsNoNuBSM"),
  cut = cms.string("pt > 10.0")
)

process.icGenJetProducer = producers.icGenJetProducer.clone(
  branch  = cms.string("genJets"),
  input   = cms.InputTag("selectedGenJets"),
  inputGenParticles = cms.InputTag("genParticles"),
  requestGenParticles = cms.bool(False)
)

process.icPileupInfoProducer = producers.icPileupInfoProducer.clone()

if not isData:
  process.icGenSequence += (
    process.prunedGenParticles+
    process.icGenParticleProducer+
    process.icPileupInfoProducer
  )
  if release in [ '72X']:
    process.icGenSequence += (
      process.genParticlesForJets+
      process.ak5GenJetsNoNuBSM+
      process.selectedGenJets+
      process.icGenJetProducer
    )

process.load("RecoJets.JetProducers.ak4GenJets_cfi")
process.ak4GenJetsNoNuBSM  =  process.ak4GenJets.clone()
process.selectedGenJetsAK4 = cms.EDFilter("GenJetRefSelector",
  src = cms.InputTag("ak4GenJetsNoNuBSM"),
  cut = cms.string("pt > 10.0")
)
process.icGenJetProducerAK4 = producers.icGenJetProducer.clone(
  branch  = cms.string("ak4GenJets"),
  input   = cms.InputTag("selectedGenJetsAK4"),
  inputGenParticles = cms.InputTag("genParticles"),
  requestGenParticles = cms.bool(False)
)
process.icGenSequence += (
    process.ak4GenJetsNoNuBSM+
    process.selectedGenJetsAK4+
    process.icGenJetProducerAK4
)

##################################################################                                                                                          
# Trigger                                                                                                                                                 
################################################################## 

################################################################                                                                                            
# EventInfo                                                                                                                                                 
################################################################                                                                                            
process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  includeJetRho       = cms.bool(True),
  inputJetRho         = cms.InputTag("kt6PFJets", "rho"),
  includeLeptonRho    = cms.bool(False),
  inputLeptonRho      = cms.InputTag("kt6PFJets", "rho"),
  includeVertexCount  = cms.bool(True),
  inputVertices       = cms.InputTag("selectedVertices"),
  includeCSCFilter    = cms.bool(False),
  inputCSCFilter      = cms.InputTag("BeamHaloSummary"),
)

process.icEventInfoSequence = cms.Sequence(
  process.icEventInfoProducer
)
################################################################                                                                                            
# Event                                                                                                                                                     
################################################################                                                                                            
process.icEventProducer = producers.icEventProducer.clone()


process.p = cms.Path(
  process.ic72XSequence+
  process.icMiniAODSequence+
  process.icSelectionSequence+
  process.pfParticleSelectionSequence+
  process.icVertexSequence+
  #process.icPFSequence+
  process.icElectronSequence+
  process.icMuonSequence+
  process.icTauProducer+
  #process.icTrackSequence+
  # process.icPFJetSequence+                                                                                                                                
  process.icGenSequence+
  # process.icTriggerSequence+                                                                                                                              
  #need met sequence
  process.icEventInfoSequence+
  process.icEventProducer
)

# process.schedule = cms.Schedule(process.patTriggerPath, process.p)                                                                                        
process.schedule = cms.Schedule(process.p)

#Uncomment below for a dump of the config
print process.dumpPython()
