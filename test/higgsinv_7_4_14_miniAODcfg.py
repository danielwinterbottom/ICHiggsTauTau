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
opts.register('globalTag', '74X_mcRun2_asymptotic_v2', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('isData', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as data?")
opts.register('release', '74XMINIAOD', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")
opts.register('isNLO', False, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "Store sign of weight?")

opts.parseArguments()
infile      = opts.file
if not infile: infile = "file:/tmp/file.root"
tag         = opts.globalTag
isData      = opts.isData
release     = opts.release
isNLO       = opts.isNLO

if not release in ["74XMINIAOD"]:
  print 'Release not recognised, exiting!'
  sys.exit(1)
print 'release     : '+release
print 'isData      : '+str(isData)
print 'globalTag   : '+tag


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
if not isData:                         
  process.source = cms.Source("PoolSource", fileNames =
       #                     cms.untracked.vstring('file:/vols/cms04/pjd12/testminiaodfiles/58D548B0-AB6F-E411-B468-3417EBE34D1A.root') 
                              #cms.untracked.vstring('root://xrootd.unl.edu//store/mc/RunIISpring15DR74/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/048FB1EE-33FD-E411-A2BA-0025905A6094.root')
                              #cms.untracked.vstring('root://xrootd.unl.edu//store/mc/RunIISpring15DR74/WW_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v1/10000/006ABB57-1207-E511-A99E-0002C94CD150.root')
 #AMM - reminiaod v2 - 13/10/2015
                              cms.untracked.vstring('file:/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_6/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/04F50A91-B46F-E511-A2A3-002618943923.root')
                              #cms.untracked.vstring('file:/afs/cern.ch/work/a/amagnan/CMSSW_7_4_14/src/ttbartest.root')
                              #cms.untracked.vstring('file:/vols/cms04/pjd12/testminiaodfiles/1E0D8712-D722-E511-B7CF-008CFA051614.root')
                              
                              )
else:
  process.source = cms.Source("PoolSource", fileNames =
       #                     cms.untracked.vstring('file:/vols/cms04/pjd12/testminiaodfiles/58D548B0-AB6F-E411-B468-3417EBE34D1A.root') 
                              #cms.untracked.vstring('root://xrootd.unl.edu//store/data/Run2015B/MET/MINIAOD/17Jul2015-v1/30000/64C3BF26-C32E-E511-8430-002618943982.root')
                              #cms.untracked.vstring('root://xrootd.unl.edu//store/data/Run2015D/MET/MINIAOD/PromptReco-v3/000/256/584/00000/C0ED0230-855D-E511-85C6-02163E0170AD.root')
#AMM - reminiaod v2 - 13/10/2015
                              cms.untracked.vstring('file:/afs/cern.ch/work/a/amagnan/CMSSW_7_4_14/src/datatest_oct5.root')
                              #cms.untracked.vstring('file:/afs/cern.ch/work/a/amagnan/CMSSW_7_4_14/src/datatest_prv4.root')                    
                              )
process.GlobalTag.globaltag = cms.string(tag)

#'root://xrootd-cms.infn.it///store/mc/Phys14DR/VBF_HToInv_M-125_13TeV_powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/58D548B0-AB6F-E411-B468-3417EBE34D1A.root')
# 72X MC: file=root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/AODSIM/PU40bx25_PHYS14_25_V1-v1/00000/00E63918-3A70-E411-A246-7845C4FC35F3.root globalTag=START72_V1::All                                                                                                               
# 72XMINIAOD MC: file=root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/36224FE2-0571-E411-9664-00266CFAE30C.root globalTag=START72_V1::All                                                                                                   
#load JEC not in global tag
# process.load("CondCore.DBCommon.CondDBCommon_cfi")
# from CondCore.DBCommon.CondDBSetup_cfi import *
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
process.ic74XSequence = cms.Sequence()
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


process.selectedPF = cms.EDFilter("PATPackedCandidateSelector",#
  src = cms.InputTag("packedPFCandidates"),
  cut = cms.string("fromPV > 1&&charge!=0&&pt>0.9")
)

process.icSelectionSequence = cms.Sequence(
  process.selectedPF+
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

process.icPFProducer = cms.EDProducer('ICPFFromPackedProducer',                                                                                                     
  branch  = cms.string("pfCandidates"),                                                                                                                   
  input   = cms.InputTag("selectedPF"),                                                                                                         
  requestTracks       = cms.bool(False),                                                                                                                    
  requestGsfTracks    = cms.bool(False),
  inputUnpackedTracks = cms.InputTag("")
)                                                                                                                                                          

process.icPFSequence = cms.Sequence()                                                                                                                     
process.icPFSequence += process.icPFProducer

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
# !!Currently cannot rerun PU jet ID MVA on user reclustered jets, will be fixed in 7_4_X
# !!Need to implement PF jet ID
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
process.ak4PFCHSL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
                                        srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
                                        algorithm = cms.string('AK4PFchs'),
                                        level = cms.string('L1FastJet')
                                        )
process.ak4PFCHSL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
                                         algorithm = cms.string('AK4PFchs'),
                                         level = cms.string('L2Relative')
                                         )
process.ak4PFCHSL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
                                         algorithm = cms.string('AK4PFchs'),
                                         level = cms.string('L3Absolute')
                                         )
process.ak4PFCHSResidual = cms.ESProducer("LXXXCorrectionESProducer",
                                       algorithm = cms.string('AK4PFchs'),
                                       level = cms.string('L2L3Residual')
                                       )

pfchsJECS = cms.PSet()
if not isData:
  pfchsJECS = cms.PSet(
    L1FastJet  = cms.string("ak4PFCHSL1Fastjet"),
    L2Relative = cms.string("ak4PFCHSL2Relative"),
    L3Absolute = cms.string("ak4PFCHSL3Absolute")
  )
else: 
  pfchsJECS = cms.PSet(
    L1FastJet  = cms.string("ak4PFCHSL1Fastjet"),
    L2Relative = cms.string("ak4PFCHSL2Relative"),
    L3Absolute = cms.string("ak4PFCHSL3Absolute"),
    L2L3Residual = cms.string("ak4PFCHSResidual")
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
                                  vertexes = cms.InputTag("unpackedTracksAndVertices"),
                                  algos = cms.VPSet(stdalgoschs),
                                  rho     = cms.InputTag("fixedGridRhoFastjetAll"),
                                  jec     = cms.string("AK4PFchs"),
                                  applyJec = cms.bool(True),
                                  inputIsCorrected = cms.bool(False),
                                  residualsFromTxt = cms.bool(False),
                                  residualsTxt = cms.FileInPath("RecoJets/JetProducers/BuildFile.xml")
                                  )

# Produce and store reclustered CHS

if not isData:
  jetsrcconfig=cms.PSet(
    includeJetFlavour         = cms.bool(True),
    inputJetFlavour           = cms.InputTag("icPFchsJetFlavourCalculator"),
    applyJECs                 = cms.bool(True),
    includeJECs               = cms.bool(False),
    JECs                      = pfchsJECS,
    applyCutAfterJECs         = cms.bool(True),
    cutAfterJECs              = cms.string("pt > 15.0"),
    inputSVInfo               = cms.InputTag(""),
    requestSVInfo             = cms.bool(False),
    BTagDiscriminators        = cms.PSet(
      simpleSecondaryVertexHighEffBJetTags = cms.InputTag("simpleSecondaryVertexHighEffBJetTagsAK4PFCHS"),
      simpleSecondaryVertexHighPurBJetTags = cms.InputTag("simpleSecondaryVertexHighPurBJetTagsAK4PFCHS"),
      combinedSecondaryVertexBJetTags      = cms.InputTag("combinedSecondaryVertexBJetTagsAK4PFCHS")
      )
    )
else:
  jetsrcconfig=cms.PSet(
    includeJetFlavour         = cms.bool(False),
    inputJetFlavour           = cms.InputTag("icPFchsJetFlavourCalculator"),
    applyJECs                 = cms.bool(True),
    includeJECs               = cms.bool(False),
    JECs                      = pfchsJECS,
    applyCutAfterJECs         = cms.bool(True),
    cutAfterJECs              = cms.string("pt > 15.0"),
    inputSVInfo               = cms.InputTag(""),
    requestSVInfo             = cms.bool(False),
    BTagDiscriminators        = cms.PSet(
      simpleSecondaryVertexHighEffBJetTags = cms.InputTag("simpleSecondaryVertexHighEffBJetTagsAK4PFCHS"),
      simpleSecondaryVertexHighPurBJetTags = cms.InputTag("simpleSecondaryVertexHighPurBJetTagsAK4PFCHS"),
      combinedSecondaryVertexBJetTags      = cms.InputTag("combinedSecondaryVertexBJetTagsAK4PFCHS")
      )
    )
process.icPFJetProducer = producers.icPFJetProducer.clone(
  branch                    = cms.string("pfJetsPFlowreclustered"),
  input                     = cms.InputTag("ak4PFJetsCHS"),
  srcConfig = jetsrcconfig,
  destConfig = cms.PSet(
    includePileupID       = cms.bool(True), 
    inputPileupID         = cms.InputTag("puJetMvaCHS", "fullDiscriminant"),
    includeTrackBasedVars = cms.bool(False),
    inputTracks           = cms.InputTag("generalTracks"),
    inputVertices=cms.InputTag("unpackedTracksAndVertices"),#!!check why this is needed
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
    includeJECs             = cms.bool(False),
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
  process.ak4PFJetsCHS+
  process.puJetMvaCHS
) 
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

#Apply met corrections !!doesn't work at the moment as modules only take AOD input
'''
process.load("JetMETCorrections.Type1MET.correctionTermsPfMetType1Type2_cff")
process.load("JetMETCorrections.Type1MET.correctionTermsPfMetType0PFCandidate_cff")
if not isData:
  process.corrPfMetType1.jetCorrLabel = cms.string("ak5PFL1FastL2L3")
else:
  process.corrPfMetType1.jetCorrLabel = cms.string("ak5PFL1FastL2L3Residual")
process.selectedVerticesForPFMEtCorrType0.src = cms.InputTag("offlineSlimmedPrimaryVertices")

process.pfMetT0pcT1 = cms.EDProducer(
    "AddCorrectionsToPFMET",
    src = cms.InputTag('icPfMet'),
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
'''

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

process.load('JetMETCorrections.Configuration.JetCorrectionProducers_cff')
process.load('RecoMET.METPUSubtraction.mvaPFMET_cff')
process.pfMVAMEt.inputFileNames = cms.PSet(
        U     = cms.FileInPath('RecoMET/METPUSubtraction/data/gbru_7_4_X_miniAOD_25NS_July2015.root'),
        DPhi  = cms.FileInPath('RecoMET/METPUSubtraction/data/gbrphi_7_4_X_miniAOD_25NS_July2015.root'),
        CovU1 = cms.FileInPath('RecoMET/METPUSubtraction/data/gbru1cov_7_4_X_miniAOD_25NS_July2015.root'),
        CovU2 = cms.FileInPath('RecoMET/METPUSubtraction/data/gbru2cov_7_4_X_miniAOD_25NS_July2015.root')
    )

process.load("RecoJets.JetProducers.ak4PFJets_cfi")

#make ak4 non-CHS jets:
process.ak4PFJets=ak4PFJets.clone()
process.ak4PFJets.src=cms.InputTag("packedPFCandidates")
process.ak4PFJets.doAreaFastjet=cms.bool(True)

from JetMETCorrections.Configuration.DefaultJEC_cff import ak4PFJetsL1FastL2L3

process.puJetIdForPFMVAMEt.rho = cms.InputTag("fixedGridRhoFastjetAll")
process.pfMVAMEt.srcPFCandidates = cms.InputTag("packedPFCandidates")
process.pfMVAMEt.srcVertices = cms.InputTag("offlineSlimmedPrimaryVertices")
process.puJetIdForPFMVAMEt.jec = cms.string("AK4PF")
process.puJetIdForPFMVAMEt.vertexes = cms.InputTag("offlineSlimmedPrimaryVertices")

process.icPfMVAMetProducer = producers.icMetProducer.clone(
  input = cms.InputTag("pfMVAMEt"),
  branch = cms.string("pfMVAMet"),
  inputCustomID = cms.InputTag("MVAMetID")
  )

process.icMvaMetSequence = cms.Sequence(
   process.ak4PFJets+
   process.calibratedAK4PFJetsForPFMVAMEt+
   process.puJetIdForPFMVAMEt+
   process.pfMVAMEt+
   process.icPfMVAMetProducer
   )

process.icMetSequence = cms.Sequence(
  process.METSignificance+
  process.icPfMet+
  process.icuncorrectedPfMetProducer+
  process.ictype1PfMetProducer+
  process.icPfMetPuppiProducer+
  process.icMvaMetSequence
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

#!!CHECK KEEPS ALL TOPS!!
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.icPrunedGenParticles = cms.EDProducer("GenParticlePruner",
  src = cms.InputTag("prunedGenParticles","","PAT"),
  select = cms.vstring(
    "drop  *",
    "keep status == 3 || status == 22 || status == 23",  # all status 3
    "keep abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15",  # all charged leptons
    "keep abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16",  # all neutrinos
    "keep++ abs(pdgId) == 15",  # keep full tau decay chain
    "keep (4 <= abs(pdgId) <= 6)", # keep heavy flavour quarks !!note extended to top quarks as well
    "keep (21 <= abs(pdgId) <= 25)", # keep bosons
    "keep (400 <= abs(pdgId) < 600) || (4000 <= abs(pdgId) < 6000)", # keep b and c hadrons
    "keep abs(pdgId) = 10411 || abs(pdgId) = 10421 || abs(pdgId) = 10413 || abs(pdgId) = 10423 || abs(pdgId) = 20413 || abs(pdgId) = 20423 || abs(pdgId) = 10431 || abs(pdgId) = 10433 || abs(pdgId) = 20433", # additional c hadrons for jet fragmentation studies
    "keep abs(pdgId) = 10511 || abs(pdgId) = 10521 || abs(pdgId) = 10513 || abs(pdgId) = 10523 || abs(pdgId) = 20513 || abs(pdgId) = 20523 || abs(pdgId) = 10531 || abs(pdgId) = 10533 || abs(pdgId) = 20533 || abs(pdgId) = 10541 || abs(pdgId) = 10543 || abs(pdgId) = 20543" # additional b hadrons for jet fragmentation studies
  )
)

process.prunedGenParticlesTaus = cms.EDProducer("ICGenParticlePruner53X",
  src = cms.InputTag("prunedGenParticles","","PAT"),
  select = cms.vstring(
    "drop  *",
    "keep++ abs(pdgId) == 15",  # keep full tau decay chain
  )
)

process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  input   = cms.InputTag("icPrunedGenParticles"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True),
  includeStatusFlags = cms.bool(True)
)

process.icGenParticleTauProducer = producers.icGenParticleProducer.clone(
  input   = cms.InputTag("prunedGenParticlesTaus"),
  branch = cms.string("genParticlesTaus"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True),
  includeStatusFlags = cms.bool(True)
)


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
  inputGenParticles = cms.InputTag("icPrunedGenParticles"),
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
    process.icPrunedGenParticles+
    process.prunedGenParticlesTaus+
    process.icGenParticleTauProducer+
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
#process.patTriggerPath = cms.Path()
process.patTriggerPath = cms.Path(
)
#switchOnTrigger(process, path = 'patTriggerPath', outputModule = '')
#switchOnTrigger(process, path = 'patTrigger', outputModule = '')

process.patTriggerSequence = cms.Sequence(
  #process.patTrigger+
  #process.patTriggerEvent
  )

process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
  branch = cms.string("triggerPaths"),
  inputIsStandAlone = cms.bool(True),
  input = cms.InputTag("TriggerResults", "", "HLT"),
  inputPrescales=cms.InputTag("patTrigger")
  )

process.icPFMET170NoiseCleanedObjectProducer = producers.icTriggerObjectProducer.clone(
  branch = cms.string("triggerObjectsPFMET170NoiseCleaned"),
  input   = cms.InputTag("patTriggerEvent"),
  hltPath = cms.string("HLT_PFMET170_NoiseCleaned_v"),
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

process.icTriggerObjectSequence = cms.Sequence(
  process.icPFMET170NoiseCleanedObjectProducer+
  process.icDiPFJet40DEta3p5MJJ600PFMETNoMu140ObjectProducer+
  process.icDiPFJet40DEta3p5MJJ600PFMETNoMu80ObjectProducer
  )

for name in process.icTriggerObjectSequence.moduleNames():
  mod = getattr(process, name)
  mod.inputIsStandAlone = cms.bool(True)
  mod.input = cms.InputTag("selectedPatTrigger", "", "")


#L1 Extra information
process.icL1ExtraMETProducer = cms.EDProducer('ICL1EtMissProducer',
  branch = cms.string("l1extraMET"),
  input = cms.InputTag("l1extraParticles","MET","RECO"),
  )

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

process.icL1ExtraHTTProducer = cms.EDProducer('ICCandidateProducer',
  branch = cms.string("l1extraHTT"),
  input = cms.InputTag("l1extraParticles","MHT","RECO"),
)

process.icL1ExtraSequence = cms.Sequence(
    process.icL1ExtraMETProducer
    +process.icL1ExtraMuonsProducer
    +process.icL1ExtraEmIsolatedProducer
    +process.icL1ExtraEmNonIsolatedProducer
    +process.icL1ExtraHTTProducer
    +process.icL1ExtraCentralJetProducer
    +process.icL1ExtraForwardJetProducer
    )


################################################################                                                                                            
# EventInfo                                                                                                                                              
################################################################                                                                                            
process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
process.HBHENoiseFilterResultProducer.minZeros = cms.int32(99999)
process.HBHENoiseFilterResultProducer.IgnoreTS4TS5ifJetInLowBVRegion=cms.bool(False) 
process.HBHENoiseFilterResultProducer.defaultDecision = cms.string("HBHENoiseFilterResultRun2Loose")

#process.HBHEISONoiseFilterResultProducer = process.HBHENoiseFilterResultProducer.clone()
#process.HBHEISONoiseFilterResultProducer.defaultDecision = cms.string("HBHEIsoNoiseFilterResult")

process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  isNlo               = isNLO,
  includeJetRho       = cms.bool(True),
  inputJetRho         = cms.InputTag("fixedGridRhoFastjetAll"),
  includeLeptonRho    = cms.bool(False),
  inputLeptonRho      = cms.InputTag("fixedGridRhoFastjetAll"),
  includeVertexCount  = cms.bool(True),
  inputVertices       = cms.InputTag("selectedVertices"),
  includeCSCFilter    = cms.bool(False),
  inputCSCFilter      = cms.InputTag("BeamHaloSummary"),
  includeFiltersFromTrig = cms.bool(True),
  filters             = cms.PSet(
    Flag_HBHENoiseFilter = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
    Flag_HBHENoiseIsoFilter = cms.InputTag('HBHENoiseFilterResultProducer','HBHEIsoNoiseFilterResult'),
    )
)

process.icEventInfoSequence = cms.Sequence(
  process.HBHENoiseFilterResultProducer+
# process.HBHEISONoiseFilterResultProducer+
  process.icEventInfoProducer
)
################################################################                                                                                            
# Event                                                                                                                                                     
################################################################                                                                                            
process.icEventProducer = producers.icEventProducer.clone()


process.p = cms.Path(
  process.ic74XSequence+
  process.icMiniAODSequence+
  process.icSelectionSequence+
  process.pfParticleSelectionSequence+
  process.icVertexSequence+
  process.icPFSequence+
  process.icElectronSequence+
  process.icMuonSequence+
  process.icTauSequence+
  process.icPhotonSequence+
  process.icPFJetSequence+
  process.icGenSequence+
  process.icMetSequence+
  process.icEventInfoSequence+
  process.patTriggerSequence+
  process.icTriggerPathProducer+
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
