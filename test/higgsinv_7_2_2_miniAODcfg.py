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
  input = cms.untracked.int32(1000)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.options   = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)

################################################################                                                                                       
# Input files and global tags                                                                                                                            
################################################################                                                                                         
process.source = cms.Source("PoolSource", fileNames =
                            cms.untracked.vstring('file:/vols/cms04/pjd12/testminiaodfiles/58D548B0-AB6F-E411-B468-3417EBE34D1A.root') 
)
process.GlobalTag.globaltag = cms.string(tag)

#'root://xrootd-cms.infn.it///store/mc/Phys14DR/VBF_HToInv_M-125_13TeV_powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/58D548B0-AB6F-E411-B468-3417EBE34D1A.root')
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

cms.EDFilter("CandPtrSelector",
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
#!!CHECK AGAINST https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Recipe_for_regular_users_for_min
################################################################                                                                                            

'''
# START POG ELECTRON ID SECTION

# Set up everything that is needed to compute electron IDs and
# add the ValueMaps with ID decisions into the event data stream

# Load tools and function definitions
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *

process.load("RecoEgamma.ElectronIdentification.egmGsfElectronIDs_cfi")
# overwrite a default parameter: for miniAOD, the collection name is a slimmed one
process.egmGsfElectronIDs.physicsObjectSrc = cms.InputTag('slimmedElectrons')

from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry
process.egmGsfElectronIDSequence = cms.Sequence(process.egmGsfElectronIDs)

# Define which IDs we want to produce
# Each of these two example IDs contains all four standard 
# cut-based ID working points (only two WP of the PU20bx25 are actually used here).
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_PHYS14_PU20bx25_V1_miniAOD_cff']
#Add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

# Do not forget to add the egmGsfElectronIDSequence to the path,
# as in the example below!

#
# END POG ELECTRON ID SECTION
'''


process.icElectronSequence = cms.Sequence()

#this module kept in src/ICAnalysis/ElectronConversionCalculator/ and is implementation of run 1 egamma conversion check !!make sure up to date
#produces value map of whether electron passes conversion veto
process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionFromPatCalculator', 
    input       = electronLabel
)

#load standard pf electron isolation calculation and make some modifications i.e. add cone veto
process.load("CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff")
process.elPFIsoValueCharged03PFIdPFIso    = process.elPFIsoValueCharged03PFId.clone()
process.elPFIsoValueChargedAll03PFIdPFIso = process.elPFIsoValueChargedAll03PFId.clone()
process.elPFIsoValueGamma03PFIdPFIso      = process.elPFIsoValueGamma03PFId.clone()
process.elPFIsoValueNeutral03PFIdPFIso    = process.elPFIsoValueNeutral03PFId.clone()
process.elPFIsoValuePU03PFIdPFIso         = process.elPFIsoValuePU03PFId.clone()
process.electronPFIsolationValuesSequence = cms.Sequence(
  process.elPFIsoValueCharged03PFIdPFIso+
  process.elPFIsoValueChargedAll03PFIdPFIso+
  process.elPFIsoValueGamma03PFIdPFIso+
  process.elPFIsoValueNeutral03PFIdPFIso+
  process.elPFIsoValuePU03PFIdPFIso
  )
process.elPFIsoDepositCharged.src     = electronLabel
process.elPFIsoDepositChargedAll.src  = electronLabel
process.elPFIsoDepositNeutral.src     = electronLabel
process.elPFIsoDepositGamma.src       = electronLabel
process.elPFIsoDepositPU.src          = electronLabel
process.elPFIsoDepositGamma.ExtractorPSet.ComponentName = cms.string("CandViewExtractor")
process.icElectronSequence += cms.Sequence(
#  process.egmGsfElectronIDSequence+
  process.electronPFIsolationDepositsSequence+
  process.electronPFIsolationValuesSequence
  )

process.elPFIsoValueGamma03PFIdPFIso.deposits[0].vetos = (
    cms.vstring('EcalEndcaps:ConeVeto(0.08)','EcalBarrel:ConeVeto(0.08)'))
process.elPFIsoValueNeutral03PFIdPFIso.deposits[0].vetos = (
    cms.vstring())
process.elPFIsoValuePU03PFIdPFIso.deposits[0].vetos = (
    cms.vstring())
process.elPFIsoValueCharged03PFIdPFIso.deposits[0].vetos = (
    cms.vstring('EcalEndcaps:ConeVeto(0.015)'))
process.elPFIsoValueChargedAll03PFIdPFIso.deposits[0].vetos = (
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

#redo isolation calculation
process.load("CommonTools.ParticleFlow.Isolation.pfMuonIsolation_cff")
process.muPFIsoValueCharged03PFIso    = process.muPFIsoValueCharged03.clone()
process.muPFIsoValueChargedAll03PFIso = process.muPFIsoValueChargedAll03.clone()
process.muPFIsoValueGamma03PFIso      = process.muPFIsoValueGamma03.clone()
process.muPFIsoValueNeutral03PFIso    = process.muPFIsoValueNeutral03.clone()
process.muPFIsoValuePU03PFIso         = process.muPFIsoValuePU03.clone()
process.muonPFIsolationValuesSequence = cms.Sequence(
    process.muPFIsoValueCharged03PFIso+
    process.muPFIsoValueChargedAll03PFIso+
    process.muPFIsoValueGamma03PFIso+
    process.muPFIsoValueNeutral03PFIso+
    process.muPFIsoValuePU03PFIso
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
  input                     = cms.InputTag("selectedPFMuons"),
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

################################################################                                                                                            
#!! Taus                                                                                                                                                   
#!! need to be checked
################################################################                                                                                            
import UserCode.ICHiggsTauTau.tau_discriminators_cfi as tauIDs

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
#  Photons
#!!Need to include rho as described in event info section
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
#  Jets                                                                                                                                                    
# !!Currently cannot rerun PU jet ID MVA on user reclustered jets, will be fixed in 7_4_X
# !!Need to implement PF jet ID
# !!All copied from Adinda's config needs checking
#################################################################

from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets

#Make ak4 CHS jets:
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.pfchs=cms.EDFilter("CandPtrSelector",src=cms.InputTag("packedPFCandidates"),cut=cms.string("fromPV"))
process.ak4PFJetsCHS = ak4PFJets.clone(src='pfchs',doAreaFastjet=True)

#make ak4 non-CHS jets:
process.ak4PFJets = ak4PFJets.clone(src='packedPFCandidates',doAreaFastjet=True)

#Get slimmedJets direct from miniAOD
process.selectedSlimmedJetsAK4 = cms.EDFilter("PATJetRefSelector",
                                              src = cms.InputTag("slimmedJets"),
                                              cut = cms.string("pt > 15")
                                              )

process.kt6PFJets = kt4PFJets.clone(
  src = 'packedPFCandidates',
  rParam = cms.double(0.6),
  doAreaFastjet = cms.bool(True),
  doRhoFastjet = cms.bool(True)
  )

# Parton flavour
# --------------
process.jetPartons = cms.EDProducer("PartonSelector",
                                    src = cms.InputTag("genParticles"),
                                    withLeptons = cms.bool(False)
                                    )
process.jetPartons.src = cms.InputTag("prunedGenParticles")

process.pfJetPartonMatches = cms.EDProducer("JetPartonMatcher",
                                            jets = cms.InputTag("ak4PFJetsCHS"),
                                            coneSizeToAssociate = cms.double(0.3),
                                            partons = cms.InputTag("jetPartons")
                                            )

process.pfJetFlavourAssociation = cms.EDProducer("JetFlavourIdentifier",
                                                 srcByReference = cms.InputTag("pfJetPartonMatches"),
                                                 physicsDefinition = cms.bool(False)
                                                 )

process.icPFJetFlavourCalculator = cms.EDProducer('ICJetFlavourCalculator',
                                                  input       = cms.InputTag("ak4PFJetsCHS"),
                                                  flavourMap  = cms.InputTag("pfJetFlavourAssociation")
                                                  )


# Jet energy corrections
# ----------------------
process.ak4PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
                                        srcRho = cms.InputTag("kt6PFJets", "rho"),
                                        algorithm = cms.string('AK4PFchs'),
                                        level = cms.string('L1FastJet')
                                        )
process.ak4PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
                                         algorithm = cms.string('AK4PFchs'),
                                         level = cms.string('L2Relative')
                                         )
process.ak4PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
                                         algorithm = cms.string('AK4PFchs'),
                                         level = cms.string('L3Absolute')
                                         )
process.ak4PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
                                       algorithm = cms.string('AK4PFchs'),
                                       level = cms.string('L2L3Residual')
                                       )

#Corrections applied to miniaod slimmedJets
pfJECS = cms.PSet(
  L1FastJet  = cms.string("ak4PFL1Fastjet"),
  L2Relative = cms.string("ak4PFL2Relative"),
  L3Absolute = cms.string("ak4PFL3Absolute")
  )
if isData: pfJECS.append(
  L2L3Residual = cms.string("ak4PFResidual")
  )

# b-tagging
# ---------
process.load("RecoJets.JetAssociationProducers.ak4JTA_cff")
from RecoJets.JetAssociationProducers.ak4JTA_cff import ak4JetTracksAssociatorAtVertex
process.load("RecoBTag.Configuration.RecoBTag_cff")
import RecoBTag.Configuration.RecoBTag_cff as btag
process.jetTracksAssociatorAtVertexAK4PF = ak4JetTracksAssociatorAtVertex.clone(
  jets = cms.InputTag("ak4PFJetsCHS")
  )

process.jetTracksAssociatorAtVertexAK4PF.tracks = cms.InputTag("unpackedTracksAndVertices")
process.jetTracksAssociatorAtVertexAK4PF.pvSrc = cms.InputTag("unpackedTracksAndVertices")

#if isEmbedded:
#  process.jetTracksAssociatorAtVertexAK5PF.tracks = cms.InputTag("tmfTracks")

process.impactParameterTagInfosAK4PF = btag.impactParameterTagInfos.clone(
  jetTracks = cms.InputTag('jetTracksAssociatorAtVertexAK4PF')
  )
process.impactParameterTagInfosAK4PF.primaryVertex = cms.InputTag("unpackedTracksAndVertices")

process.secondaryVertexTagInfosAK4PF = btag.secondaryVertexTagInfos.clone(
  trackIPTagInfos = cms.InputTag('impactParameterTagInfosAK4PF')
  )
process.simpleSecondaryVertexHighEffBJetTagsAK4PF = btag.simpleSecondaryVertexHighEffBJetTags.clone (
  tagInfos = cms.VInputTag('secondaryVertexTagInfosAK4PF')
  )
process.simpleSecondaryVertexHighPurBJetTagsAK4PF = btag.simpleSecondaryVertexHighPurBJetTags.clone (
  tagInfos = cms.VInputTag('secondaryVertexTagInfosAK4PF')
  )
process.combinedSecondaryVertexBJetTagsAK4PF = btag.combinedSecondaryVertexBJetTags.clone (
  tagInfos = cms.VInputTag('impactParameterTagInfosAK4PF', 'secondaryVertexTagInfosAK4PF')
  )

process.btaggingSequenceAK4PF = cms.Sequence(
  process.jetTracksAssociatorAtVertexAK4PF
  +process.impactParameterTagInfosAK4PF
  +process.secondaryVertexTagInfosAK4PF
  +process.simpleSecondaryVertexHighEffBJetTagsAK4PF
  +process.simpleSecondaryVertexHighPurBJetTagsAK4PF
  +process.combinedSecondaryVertexBJetTagsAK4PF
 )

# Pileup ID
# ---------
stdalgos = cms.VPSet()
from RecoJets.JetProducers.PileupJetIDParams_cfi import *
stdalgos = cms.VPSet(full_5x_chs,cutbased)

process.puJetMva = cms.EDProducer('PileupJetIdProducer',
                                  produceJetIds = cms.bool(True),
                                  jetids = cms.InputTag(""),
                                  runMvas = cms.bool(True),
                                  #jets = cms.InputTag("slimmedJets"),
                                  jets = cms.InputTag("ak4PFJetsCHS"),
                                  vertexes = cms.InputTag("offlinePrimaryVertices"),
                                  #    vertexes = cms.InputTag("unpackedTracksAndVertices"),
                                  algos = cms.VPSet(stdalgos),
                                  rho     = cms.InputTag("kt6PFJets", "rho"),
                                  #    rho     = cms.InputTag("fixedGridRhoFastjetAll"),
                                  jec     = cms.string("AK4PFchs"),
                                  applyJec = cms.bool(True),
                                  inputIsCorrected = cms.bool(False),
                                  residualsFromTxt = cms.bool(False),
                                  residualsTxt     = cms.FileInPath("RecoJets/JetProducers/data/dummy.txt"),
                                  )

process.puJetMva.vertexes = cms.InputTag("unpackedTracksAndVertices")
process.puJetMva.rho = cms.InputTag("fixedGridRhoFastjetAll")
process.puJetMva.residualsTxt = cms.FileInPath("RecoJets/JetProducers/BuildFile.xml")

# Producer
# --------
process.icPFJetProducer = producers.icPFJetProducer.clone(
  branch                    = cms.string("pfJetsPFlow"),
  input                     = cms.InputTag("ak4PFJetsCHS"),
  srcConfig = cms.PSet(
    includeJetFlavour         = cms.bool(True),
    inputJetFlavour           = cms.InputTag("icPFJetFlavourCalculator"),
    applyJECs                 = cms.bool(True),
    includeJECs               = cms.bool(False),
    JECs                      = pfJECS,
    applyCutAfterJECs         = cms.bool(True),
    cutAfterJECs              = cms.string("pt > 15.0"),
    inputSVInfo               = cms.InputTag(""),
    requestSVInfo             = cms.bool(False),
    BTagDiscriminators        = cms.PSet(
      simpleSecondaryVertexHighEffBJetTags = cms.InputTag("simpleSecondaryVertexHighEffBJetTagsAK4PF"),
      simpleSecondaryVertexHighPurBJetTags = cms.InputTag("simpleSecondaryVertexHighPurBJetTagsAK4PF"),
      combinedSecondaryVertexBJetTags      = cms.InputTag("combinedSecondaryVertexBJetTagsAK4PF")
      )
    ),
  destConfig = cms.PSet(
    includePileupID       = cms.bool(True), #rerunning the pu MVA on the jet collection created in miniAOD is possible in newer CMSSW versions but not yet in 72
    inputPileupID         = cms.InputTag("puJetMva", "fullDiscriminant"),
    includeTrackBasedVars = cms.bool(False),
    inputTracks           = cms.InputTag("generalTracks"),
    inputVertices         = cms.InputTag("selectedVertices"),
    requestTracks         = cms.bool(False)
    )
  )


process.icPFJetProducer.destConfig.includePileupID=cms.bool(False)#!!cannot rerun run pu ID mva in 72 will be fixed for 74
process.icPFJetProducer.destConfig.inputVertices=cms.InputTag("unpackedTracksAndVertices")#!!check why this is needed

process.icPFJetProducerFromPat = producers.icPFJetFromPatProducer.clone(
  branch                    = cms.string("ak4SlimmedJets"),
  input                     = cms.InputTag("selectedSlimmedJetsAK4"),
  srcConfig = cms.PSet(
    isSlimmed               = cms.bool(True),
        includeJetFlavour       = cms.bool(True),
    includeJECs             = cms.bool(False),
    inputSVInfo             = cms.InputTag(""),
    requestSVInfo           = cms.bool(False)
    ),
  destConfig = cms.PSet(
    includePileupID         = cms.bool(True),
    inputPileupID           = cms.InputTag("puJetMva", "fullDiscriminant"),
    includeTrackBasedVars   = cms.bool(False),
    inputTracks             = cms.InputTag("unpackedTracksAndVertices"),#!!check this and line below
    inputVertices           = cms.InputTag("unpackedTracksAndVertices"),#!!
    requestTracks           = cms.bool(False)
    )
  )

process.icPFJetSequence = cms.Sequence()


process.icPFJetSequence += cms.Sequence(
  process.pfchs+
  process.selectedSlimmedJetsAK4+
  process.unpackedTracksAndVertices+
  process.icPFJetProducerFromPat
  )
# process.icPFJetProducer.srcConfig.BTagDiscriminators = cms.PSet()
process.icPFJetSequence += cms.Sequence(
  process.ak4PFJetsCHS+
  process.puJetMva+ 
  process.jetPartons+
  process.pfJetPartonMatches+
  process.pfJetFlavourAssociation+
  process.icPFJetFlavourCalculator+
  process.btaggingSequenceAK4PF+
  process.icPFJetProducer #Not from slimmed jets!
  )


process.icPFJetSequence.remove(process.puJetMva) #!!This works for jets built from PackedCandidates in CMSSW74X but not yet in 72




# process.ak4PFJets = ak4PFJets.clone(src = 'packedPFCandidates')

# process.icPFJetSequence = cms.Sequence(
#   process.ak4PFJets
# )

##################################################################                                                                                          
#  MET                                                                                                                                                    
#!!currently type 1 met only, no type0pc+type1 recipe and also haven't implemented mva met
#################################################################

#Make uncorrected pfMet from packedPF
from RecoMET.METProducers.PFMET_cfi import pfMet

process.pfMet = pfMet.clone(src = "packedPFCandidates")
process.pfMet.calculateSignificance = False # this can't be easily implemented on packed PF candidates at the moment

process.icuncorrectedPfMetProducer = cms.EDProducer('ICMetProducer',
                                                    input = cms.InputTag("pfMet"),
                                                    branch = cms.string("uncorrectedpfMet"),
                                                    includeCustomID = cms.bool(False),
                                                    inputCustomID = cms.InputTag(""),
                                                    )

#Apply met corrections
process.load("JetMETCorrections.Type1MET.correctionTermsPfMetType1Type2_cff")
process.load("JetMETCorrections.Type1MET.correctionTermsPfMetType0PFCandidate_cff")
if not isData:
  process.corrPfMetType1.jetCorrLabel = cms.string("ak5PFL1FastL2L3")
else:
  process.corrPfMetType1.jetCorrLabel = cms.string("ak5PFL1FastL2L3Residual")
process.selectedVerticesForPFMEtCorrType0.src = cms.InputTag("offlineSlimmedPrimaryVertices")

process.pfMetT0pcT1 = cms.EDProducer(
    "AddCorrectionsToPFMET",
    src = cms.InputTag('pfMet'),
    srcCorrections = cms.VInputTag(
        cms.InputTag('corrPfMetType0PfCand'),
        cms.InputTag('corrPfMetType1', 'type1')
    ),
)

process.icPfMetT0pcT1Producer = cms.EDProducer('ICMetProducer',
                                                    input = cms.InputTag("pfMetT0pcT1"),
                                                    branch = cms.string("pfMetType1"),
                                                    includeCustomID = cms.bool(False),
                                                    inputCustomID = cms.InputTag(""),
                                                    )

process.icMetSequence = cms.Sequence(
  process.pfMet+
  process.icuncorrectedPfMetProducer
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
'''
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
'''
##################################################################                                                                                          
#!! Trigger                                                                                                                                                 
################################################################## 

################################################################                                                                                            
#!! EventInfo                                                                                                                                              
#!!needs kt6 jets to be working so not included in path for the moment
#!!make sure we keep from RecoJets.JetProducers.fixedGridRhoProducer_cfi import fixedGridRhoAll for photons
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
  process.icPhotonSequence+
  #process.icTrackSequence+
  process.icPFJetSequence+                                                                                                                                
  #process.icGenSequence+
  #process.icMetSequence+
  # process.icTriggerSequence+                                                                                                                              
  #need met sequence
#  process.icEventInfoSequence+
  process.icEventProducer
)


# process.out = cms.OutputModule("PoolOutputModule",
#                 fileName = cms.untracked.string("edmdump.root")
#         )

# process.outpath = cms.EndPath(process.out)


#process.schedule = cms.Schedule(process.patTriggerPath, process.p)                                                                                        
process.schedule = cms.Schedule(process.p)

#make an edm output ntuple with everything in it
#process.schedule = cms.Schedule(process.p,process.outpath)



#Uncomment below for a dump of the config
#print process.dumpPython()
