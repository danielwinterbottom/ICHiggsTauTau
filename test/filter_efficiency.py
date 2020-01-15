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
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/7C213504-0B84-514A-8F62-1FA4326BAC84.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/535F4D09-CCF5-3A4D-B419-C033241C09AA.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/D8CE1CB5-207A-B24D-80A2-9999B66F5DD6.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/5085F99B-7410-7F48-B667-D7CF69161880.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/7AD302C4-C11E-4648-9B5A-5B4913415F86.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/7392190F-6866-B44E-B111-F9DF94CED1A8.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/5641CAAA-396D-2A44-8BA1-4C535B115E48.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/471321B3-0B92-F645-9BD5-BCFA5C1B8AEB.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/1720B124-631E-9D4B-B16F-F5F28CF34F35.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/288F8B5B-4427-D744-ACF2-1B732452ADB5.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/9456E395-2D32-D841-A2C0-D8CB397B39A7.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/27ED3224-A74E-7E4A-B3A6-48A7FC938BE6.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/AE4D42E5-FF82-054A-9383-4336F53F22D9.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/4AEB7484-9486-684E-9B79-B941DC8CE842.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/FDF0531F-1AA7-FA44-92CC-308F8E7BD1EC.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/0C6DE980-DFDD-5342-A5FE-5F3FADCC0F69.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/5B3B31EA-B4F6-6A47-B131-8A8CEA6C54B5.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/0E53CB4F-BF3E-DD48-A340-6F7E32F80A48.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/1058278B-A609-B44B-9880-0B4C2C095C11.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/3049489F-1226-314F-9001-49CDA27196F1.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/4EF6464B-1121-294F-8A15-3131EB9EBA66.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/B57CAD7C-AA62-1B43-BAD0-72A1BF2F19F9.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/5EFA1075-7A75-0E4A-B5C2-CF39D2327B45.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/9D31245E-87E6-3B41-BA11-1C264D37F5C0.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/507D67D6-C0FF-D545-8697-411F1DB2179F.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/EEE5EF7B-72CD-9F42-977D-02CE582498DA.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/2AAA5301-1123-9F42-9468-FC8C78F5FCF5.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/44F3B1F2-42F8-B441-8AE4-BB7FF6D889C2.root",
        "root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/240000/298C01FF-9C5C-2748-9594-E5F869F6E414.root",

    ),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)


process.dummy2 = cms.EDAnalyzer("GenXSecAnalyzer")



# Path and EndPath definitions
process.ana = cms.Path(process.dummy2)
# Schedule definition
process.schedule = cms.Schedule(process.ana)
