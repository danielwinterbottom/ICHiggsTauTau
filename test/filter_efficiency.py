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
#root://xrootd.unl.edu/
        "/store/mc/Run3Summer22MiniAODv3/DYto2L-4Jets_MLL-50_2J_TuneCP5_13p6TeV_madgraphMLM-pythia8/MINIAODSIM/124X_mcRun3_2022_realistic_v12-v2/50000/99aa30d5-989c-4890-87be-d991cf02e24e.root",
        "/store/mc/Run3Summer22MiniAODv3/DYto2L-4Jets_MLL-50_2J_TuneCP5_13p6TeV_madgraphMLM-pythia8/MINIAODSIM/124X_mcRun3_2022_realistic_v12-v2/50000/fd790dbf-0207-4054-96d2-fc82856a4c17.root",
        "/store/mc/Run3Summer22MiniAODv3/DYto2L-4Jets_MLL-50_2J_TuneCP5_13p6TeV_madgraphMLM-pythia8/MINIAODSIM/124X_mcRun3_2022_realistic_v12-v2/50000/b5d73fb1-fcaa-47d3-a9bf-3067921383c2.root",

    ),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)


process.dummy2 = cms.EDAnalyzer("GenXSecAnalyzer")



# Path and EndPath definitions
process.ana = cms.Path(process.dummy2)
# Schedule definition
process.schedule = cms.Schedule(process.ana)
