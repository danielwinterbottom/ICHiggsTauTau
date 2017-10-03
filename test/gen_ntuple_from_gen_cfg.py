# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step1 --fileout file:HIG-RunIISummer16MiniAODv2-00187.root --mc --eventcontent MINIAODSIM --runUnscheduled --datatier MINIAODSIM --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v6 --step PAT --nThreads 4 --era Run2_2016 --python_filename HIG-RunIISummer16MiniAODv2-00187_1_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring
import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing('python')
options.register('output','',VarParsing.multiplicity.singleton,VarParsing.varType.string,"type parameter")
options.register('input','',VarParsing.multiplicity.singleton,VarParsing.varType.string,"type parameter")

options.parseArguments()
#outputfilename = "file:%s_%s.root"%(options.output.replace(".root",""),options.jobnum)
outputfilename = "%s"%(options.output)
inputfilename = "%s"%(options.input)

from Configuration.StandardSequences.Eras import eras

process = cms.Process('PAT',eras.Run2_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('PhysicsTools.PatAlgos.slimming.metFilterPaths_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.TFileService = cms.Service("TFileService",
        fileName = cms.string(outputfilename),
            closeFileFast = cms.untracked.bool(True)
            )


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 50

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(inputfilename),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step1 nevts:1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.MINIAODSIMoutput = cms.OutputModule("PoolOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(4),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('MINIAODSIM'),
        filterName = cms.untracked.string('')
    ),
    dropMetaData = cms.untracked.string('ALL'),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    fastCloning = cms.untracked.bool(False),
    fileName = cms.untracked.string('file:HIG-RunIISummer16MiniAODv2-00187.root'),
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_prunedGenParticles_*_*',
        'keep *_ak4GenJetsNoNu_*_*'
    ),
    overrideInputFileSplitLevels = cms.untracked.bool(True)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_TrancheIV_v8', '')

# Path and EndPath definitions
process.Flag_trackingFailureFilter = cms.Path(process.goodVertices+process.trackingFailureFilter)
process.Flag_goodVertices = cms.Path(process.primaryVertexFilter)
process.Flag_CSCTightHaloFilter = cms.Path(process.CSCTightHaloFilter)
process.Flag_trkPOGFilters = cms.Path(process.trkPOGFilters)
process.Flag_trkPOG_logErrorTooManyClusters = cms.Path(~process.logErrorTooManyClusters)
process.Flag_EcalDeadCellTriggerPrimitiveFilter = cms.Path(process.EcalDeadCellTriggerPrimitiveFilter)
process.Flag_ecalLaserCorrFilter = cms.Path(process.ecalLaserCorrFilter)
process.Flag_globalSuperTightHalo2016Filter = cms.Path(process.globalSuperTightHalo2016Filter)
process.Flag_eeBadScFilter = cms.Path(process.eeBadScFilter)
process.Flag_METFilters = cms.Path(process.metFilters)
process.Flag_chargedHadronTrackResolutionFilter = cms.Path(process.chargedHadronTrackResolutionFilter)
process.Flag_globalTightHalo2016Filter = cms.Path(process.globalTightHalo2016Filter)
process.Flag_CSCTightHaloTrkMuUnvetoFilter = cms.Path(process.CSCTightHaloTrkMuUnvetoFilter)
process.Flag_HBHENoiseIsoFilter = cms.Path(process.HBHENoiseFilterResultProducer+process.HBHENoiseIsoFilter)
process.Flag_hcalLaserEventFilter = cms.Path(process.hcalLaserEventFilter)
process.Flag_HBHENoiseFilter = cms.Path(process.HBHENoiseFilterResultProducer+process.HBHENoiseFilter)
process.Flag_trkPOG_toomanystripclus53X = cms.Path(~process.toomanystripclus53X)
process.Flag_EcalDeadCellBoundaryEnergyFilter = cms.Path(process.EcalDeadCellBoundaryEnergyFilter)
process.Flag_trkPOG_manystripclus53X = cms.Path(~process.manystripclus53X)
process.Flag_HcalStripHaloFilter = cms.Path(process.HcalStripHaloFilter)
process.Flag_muonBadTrackFilter = cms.Path(process.muonBadTrackFilter)
process.Flag_CSCTightHalo2015Filter = cms.Path(process.CSCTightHalo2015Filter)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.MINIAODSIMoutput_step = cms.EndPath(process.MINIAODSIMoutput)


import UserCode.ICHiggsTauTau.default_producers_cfi as producers

process.icGenParticleProducer = producers.icGenParticleProducer.clone(
    input               = cms.InputTag('prunedGenParticles', '', 'PAT'),
    includeMothers      = cms.bool(True),
    includeDaughters    = cms.bool(True),
    includeStatusFlags  = cms.bool(True)
    )

from PhysicsTools.JetMCAlgos.HadronAndPartonSelector_cfi import selectedHadronsAndPartons
process.selectedHadronsAndPartons = selectedHadronsAndPartons.clone(
        particles = cms.InputTag('prunedGenParticles', '', 'PAT') 
        )

from PhysicsTools.JetMCAlgos.AK4PFJetsMCFlavourInfos_cfi import ak4JetFlavourInfos
process.genJetFlavourInfos = ak4JetFlavourInfos.clone(
        jets = cms.InputTag("ak4GenJetsNoNu") 
        )

process.MessageLogger.categories += cms.vstring('JetPtMismatch', 'MissingJetConstituent', 'JetPtMismatchAtLowPt')
process.MessageLogger.cerr.JetPtMismatch = cms.untracked.PSet(limit = cms.untracked.int32(0))
process.MessageLogger.cerr.MissingJetConstituent = cms.untracked.PSet(limit = cms.untracked.int32(0))
process.MessageLogger.cerr.JetPtMismatchAtLowPt = cms.untracked.PSet(limit = cms.untracked.int32(0))

process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
    src = cms.InputTag("ak4GenJetsNoNu"),
    cut = cms.string("pt > 10.0")
    )

process.icGenJetFlavourCalculator = cms.EDProducer('ICJetFlavourCalculator',
        input       = cms.InputTag("ak4GenJetsNoNu"),
        flavourMap  = cms.InputTag("genJetFlavourInfos")
        )

process.icGenJetProducer = producers.icGenJetProducer.clone(
    branch              = cms.string("genJets"),
    input               = cms.InputTag("selectedGenJets"),
    inputGenParticles   = cms.InputTag("genParticles"),
    requestGenParticles = cms.bool(False),
    includeFlavourInfo  = cms.bool(True),
    inputFlavourInfo    = cms.InputTag("icGenJetFlavourCalculator"),
    isSlimmed           = cms.bool(False)
    )

process.icEventInfoProducer = producers.icEventInfoProducer.clone(
    lheProducer         = cms.InputTag("externalLHEProducer"),
    includeLHEWeights   = cms.bool(False)
    )

process.icEventProducer = producers.icEventProducer.clone()

process.p = cms.Path(
   process.selectedHadronsAndPartons+
   process.genJetFlavourInfos+
   process.icGenJetFlavourCalculator+
   process.icGenParticleProducer+
   process.selectedGenJets+
   process.icGenJetProducer+
   process.icEventInfoProducer+
   process.icEventProducer
)

# Schedule definition
process.schedule = cms.Schedule(process.p)

#Setup FWK for multithreaded
#process.options.numberOfThreads=cms.untracked.uint32(1)
#process.options.numberOfStreams=cms.untracked.uint32(0)

# customisation of the process.

# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)

# End of customisation functions
#do not add changes to your config after this point (unless you know what you are doing)
#from FWCore.ParameterSet.Utilities import convertToUnscheduled
#process=convertToUnscheduled(process)
process.load('Configuration.StandardSequences.PATMC_cff')
process.load('PhysicsTools.HepMCCandAlgos.genParticles_cfi')
process.load('RecoJets.Configuration.GenJetParticles_cff')
process.load('RecoJets.Configuration.RecoGenJets_cff')

process.genParticles.src = cms.InputTag("generator")
#from FWCore.ParameterSet.Utilities import cleanUnscheduled
#process=cleanUnscheduled(process)

# customisation of the process.

# Automatic addition of the customisation function from PhysicsTools.PatAlgos.slimming.miniAOD_tools
from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllMC 

#call to customisation function miniAOD_customizeAllMC imported from PhysicsTools.PatAlgos.slimming.miniAOD_tools
process = miniAOD_customizeAllMC(process)

# End of customisation functions
