import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
options.parseArguments()

process = cms.Process('ANA')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic8TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:mc', '')
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.source = cms.Source(
    "PoolSource",
    fileNames  = cms.untracked.vstring(
	"root://xrootd.unl.edu//store/mc/RunIISummer20UL16MiniAODAPVv2/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/106X_mcRun2_asymptotic_preVFP_v11-v1/140000/BD2A7B20-ED6D-6B4B-B826-903A1BB85555.root",
       # "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/2AAA5301-1123-9F42-9468-FC8C78F5FCF5.root",
       # "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/44F3B1F2-42F8-B441-8AE4-BB7FF6D889C2.root",
       # "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/298C01FF-9C5C-2748-9594-E5F869F6E414.root",

    ),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)


process.dummy2 = cms.EDAnalyzer("GenXSecAnalyzer")



# Path and EndPath definitions
process.ana = cms.Path(process.dummy2)
# Schedule definition
process.schedule = cms.Schedule(process.ana)
