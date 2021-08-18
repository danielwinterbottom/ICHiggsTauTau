import os
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

process.MessageLogger.cerr.FwkReport.reportEvery = 200000

file_loc = "store/user/guttley/MINIAOD/betaRd33_minus1_mU4_gU3_GENSIM_2018"
os.system("uberftp sedsk53.grid.hep.ph.ic.ac.uk \"ls {}/*/*/*/*.root\" > dcache_ls.txt".format(file_loc))
dcache_ls = open('dcache_ls.txt', 'r')
MINIAOD_files = []
for line in dcache_ls: 
  if "store" in line:
    MINIAOD_files.append("root://gfe02.grid.hep.ph.ic.ac.uk:1097/store"+line.split("store")[1].strip())
os.system("rm dcache_ls.txt")

process.source = cms.Source(
    "PoolSource",
    fileNames  = cms.untracked.vstring(MINIAOD_files),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)


process.dummy2 = cms.EDAnalyzer("GenXSecAnalyzer")



# Path and EndPath definitions
process.ana = cms.Path(process.dummy2)
# Schedule definition
process.schedule = cms.Schedule(process.ana)
