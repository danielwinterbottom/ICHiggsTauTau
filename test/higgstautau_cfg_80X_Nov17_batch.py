import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
#opts.register('file', 'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_powheg_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/80000/0863B733-1A39-E611-AF47-0025905C53D8.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'root://xrootd.unl.edu//store/mc/RunIISummer16MiniAODv2/SUSYGluGluToBBHToTauTau_M-1000_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/4C466283-6BC0-E611-B3AE-001517FB25E4.root', parser.VarParsing.multiplicity.singleton,
opts.register('file', 'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/001268DD-CAEB-E511-AC7F-842B2B5C2299.root', parser.VarParsing.multiplicity.singleton,               
#opts.register('file', 'root://xrootd.unl.edu//store/data/Run2016B/SingleMuon/MINIAOD/03Feb2017_ver2-v2/100000/000C6E52-8BEC-E611-B3FF-0025905C42FE.root',parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'root://xrootd.unl.edu//store/data/Run2016F/SingleMuon/MINIAOD/PromptReco-v1/000/277/932/00000/084865EB-1859-E611-BDA7-02163E011A89.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'root://xrootd.unl.edu//store/data/Run2016H/SingleMuon/MINIAOD/PromptReco-v2/000/281/265/00000/28861171-6E82-E611-9CAF-02163E0141FA.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'root://xrootd.unl.edu//store/data/Run2016H/Tau/MINIAOD/PromptReco-v3/000/284/036/00000/36B9BD65-5B9F-E611-820B-02163E0126D3.root', parser.VarParsing.multiplicity.singleton, parser.VarParsing.varType.string, "input file")
#opts.register('file', 'root://xrootd.unl.edu//store/data/Run2016H/SingleElectron/MINIAOD/PromptReco-v3/000/284/036/00000/1CBE1DEB-589F-E611-ABBB-02163E0143B5.root', parser.VarParsing.multiplicity.singleton,
parser.VarParsing.varType.string, "input file")
opts.register('globalTag', '80X_mcRun2_asymptotic_2016_TrancheIV_v7', parser.VarParsing.multiplicity.singleton,
#opts.register('globalTag', '80X_dataRun2_2016SeptRepro_v7', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('isData', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as data?")
#opts.register('release', '7412MINIAOD', parser.VarParsing.multiplicity.singleton,
opts.register('release', '80XMINIAOD', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")
opts.register('doHT', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Store HT and number of outgoing partons?")
opts.register('isReHLT', 1, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as reHLT sample?")
opts.register('LHEWeights', False, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "Produce LHE weights for sample")
opts.register('LHETag', 'externalLHEProducer', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Input tag for LHE weights")
opts.register('MGsignalGF', False, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "Using amc@NLO for ggH signal samples")
opts.register('MGsignalVBF', False, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "Using amc@NLO for qqH signal samples")
opts.register('outfile', 'EventTree.root', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")
opts.register('maxevents', 100, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "max number of events to processs")
opts.register('firstevent', 1, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "first event to process")
opts.register('cmsswbase', '/vols/build/cms/dw515/CMSSW_8_0_26_patch1/', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "path to CMSSW_BASE")


opts.parseArguments()
infile      = opts.file
if not infile: infile = "file:/tmp/file.root"
isData      = opts.isData
tag         = opts.globalTag
release     = opts.release
doLHEWeights = opts.LHEWeights
outfile      = opts.outfile
CMSSW_BASE = opts.cmsswbase

if not isData:
  doHT     = opts.doHT
  isReHLT  = opts.isReHLT
else:
  doHT     = 0
  isReHLT  = 0
#isEmbedded  = opts.isEmbedded
#isTandP     = opts.isTandP
#isZStudy    = opts.isZStudy
#isPhys14    = opts.isPhys14

if not release in ["76X", "80XMINIAOD"]:
  print 'Release not recognised, exiting!'
  sys.exit(1)
print 'release     : '+release
print 'isData      : '+str(isData)
print 'globalTag   : '+str(tag)
print 'doHT        : '+str(doHT)
print 'isReHLT     : '+str(isReHLT)

################################################################
# Standard setup
################################################################
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.TFileService = cms.Service("TFileService",
  fileName = cms.string(outfile),
  closeFileFast = cms.untracked.bool(True)
)

################################################################
# Message Logging, summary
################################################################

process.MessageLogger.cerr.FwkReport.reportEvery = 50

process.options   = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)


################################################################
# Input files and global tags
################################################################
process.load("CondCore.CondDB.CondDB_cfi")
from CondCore.CondDB.CondDB_cfi import *
#if not isData:
#  process.jec = cms.ESSource("PoolDBESSource",
#    DBParameters = cms.PSet(
#     messageLevel = cms.untracked.int32(0)
#    ),
#    timetype = cms.string('runnumber'),
#    toGet = cms.VPSet(
#    cms.PSet(
#      record = cms.string('JetCorrectionsRecord'),
#      tag = cms.string('JetCorrectorParametersCollection_Spring16_25nsV3_MC_AK4PF'),
#      label = cms.untracked.string('AK4PF')
#      ),
#    cms.PSet(
#      record = cms.string('JetCorrectionsRecord'),
#      tag = cms.string('JetCorrectorParametersCollection_Spring16_25nsV3_MC_AK4PFchs'),
#      label = cms.untracked.string('AK4PFchs')
#      ),
#    ),
#    connect = cms.string('sqlite:Spring16_25nsV3_MC.db')
#  )
#
#  process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')

#else :
#  process.jec = cms.ESSource("PoolDBESSource",
#    DBParameters = cms.PSet(
#     messageLevel = cms.untracked.int32(0)
#    ),
#    timetype = cms.string('runnumber'),
#    toGet = cms.VPSet(
#    cms.PSet(
#      record = cms.string('JetCorrectionsRecord'),
#      tag = cms.string('JetCorrectorParametersCollection_Spring16_25nsV3_DATA_AK4PF'),
#      label = cms.untracked.string('AK4PF')
#      ),
#    cms.PSet(
#      record = cms.string('JetCorrectionsRecord'),
#      tag = cms.string('JetCorrectorParametersCollection_Spring16_25nsV3_DATA_AK4PFchs'),
#      label = cms.untracked.string('AK4PFchs')
#      ),
#    ),
#    connect = cms.string('sqlite:Spring16_25nsV3_DATA.db')
#  )
#
#  process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(opts.maxevents)
)
if opts.MGsignalGF:
  process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/3E0F474D-CDEB-E511-95F7-00266CFF0B84.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/8C5A6AC3-CCEB-E511-A9CA-008CFA110C88.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/6A7A8A59-CDEB-E511-BDAF-008CFA1113F8.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/F41847B5-CDEB-E511-B6E2-008CFA197D88.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/4C9E0884-D8EB-E511-ADE9-008CFA1979AC.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1C764EB6-BCEC-E511-B413-008CFA197900.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/F656D324-D3EB-E511-B639-0025901D4B04.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/B40D88D9-D1EB-E511-A2A3-0025901D4B04.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A63E1A89-A8EC-E511-A1E1-002590AC4B3E.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/84BE98F8-47ED-E511-BFC5-90B11C050371.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/8A963B22-48ED-E511-B39F-00266CFFCD00.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A0C5CAC8-CDEC-E511-BA64-02163E017617.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/06F09FA3-CEEC-E511-B93C-02163E017971.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/60813267-CEEC-E511-82B0-02163E015323.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A8D89882-D0EC-E511-8CD5-02163E013FA2.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/C0E65A10-CEEC-E511-9133-02163E011450.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/46AF79A2-CEEC-E511-96B9-2C600CAFEF72.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/EEFE6F3F-D0EC-E511-9270-02163E00F722.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A29B2216-D3EC-E511-A536-02163E017932.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/28BA728E-D4EC-E511-A33A-02163E013EA5.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1E9EE7E4-47ED-E511-835E-24BE05CE3E61.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/BEDC9B28-C8EB-E511-B1EC-A0369F7F9350.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/B0061A62-C2EB-E511-8C50-008CFA1982C0.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A2B7985E-C2EB-E511-B119-549F358EB748.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/964752C9-9BEC-E511-9087-A0369F7FC540.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/B0ABC374-CAEB-E511-B25C-B083FECFD4F0.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/B4C2FB8B-C9EB-E511-A854-20CF3027A5F4.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/846E2F99-CBEB-E511-A67F-20CF3027A5F4.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A653D67D-CAEB-E511-8196-002590747D9C.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/864492DA-A8EC-E511-B2FF-00259073E52A.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/D621C957-BEEB-E511-93DA-0025905C9726.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/80A63657-BEEB-E511-BF43-0025905C9740.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/8E04C057-BEEB-E511-9642-0025905C4264.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/AC95BB57-BEEB-E511-A8F7-0025905C3D3E.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1E760E58-BEEB-E511-AA09-0025905C94D0.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A2F2DA58-BEEB-E511-9A29-0025905C431C.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/122E8A58-BEEB-E511-B706-0025904C66EC.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1A09DE58-BEEB-E511-ACC9-0025904C68D8.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/30C05C58-BEEB-E511-A42E-0025905C9724.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/92EF8458-BEEB-E511-A8FA-0025904C67B6.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/4E5A7758-BEEB-E511-97A3-0025905C2CB8.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/BE51A370-C2EB-E511-8DC6-0025905C42F2.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/7E6C8BE1-92EC-E511-9D3A-0025904C68DE.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/E8DBCDBE-D6EB-E511-8CE9-001E67A3EF70.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/30EC60BF-D6EB-E511-A4D7-001E675A5244.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/80172141-D8EB-E511-BEB2-001E67A40514.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/0E339DA3-D7EB-E511-8AB0-001E675A6725.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/0C0F0C56-D7EB-E511-A92E-001E67DDC4AC.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/3660E155-D7EB-E511-AA5C-001E675A5262.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/FC9B7242-E4EB-E511-85E2-002590D4FC08.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/C6D4D6E8-D5EC-E511-B1D0-20CF307C988D.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/145E6B50-48ED-E511-8A58-002590A4C6BE.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1029D3EF-47ED-E511-9AD9-B499BAAC04AA.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/40D8B8E9-47ED-E511-B9D2-002590207C28.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/2C6A72E4-47ED-E511-93D4-0025907DC9C4.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/7A6B68EA-47ED-E511-8867-0CC47A7452D8.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/F08482EB-47ED-E511-82C6-0025905A6082.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/6288BC4A-48ED-E511-9255-02163E017910.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/62D52CF7-47ED-E511-9C82-02163E013227.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/2CB271ED-47ED-E511-807B-02163E013BDB.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/0A3113F9-47ED-E511-BD57-02163E013268.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/F2541158-48ED-E511-89E2-02163E016A52.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/C6F7EA61-48ED-E511-BD64-02163E00C092.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/401B83FA-47ED-E511-B2D1-02163E00C0AE.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/5CA3CD75-C6EB-E511-94B0-0CC47A6C1810.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/DE593557-C7EB-E511-AFF2-00304865C254.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/8C8C984F-C7EB-E511-A8ED-0CC47A6C138A.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/DCF87372-C6EB-E511-B785-0CC47A6C06C2.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/C6FF7F4C-C7EB-E511-A5E9-0CC47A6C1038.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A021D37C-CAEB-E511-BBD4-0025904A8ECE.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/7C378874-D0EC-E511-B5F2-0021283BB12A.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/CEFCE775-8AEC-E511-B9A0-001E67E6F503.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/ECB66C9C-89EC-E511-8548-002590A887AC.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/D0B1489F-8AEC-E511-9411-002590A88802.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/3007B4FA-8BEC-E511-9E58-001E673975EE.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/440CB6FD-8BEC-E511-8B28-002590A37114.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/56BA7A9B-8CEC-E511-8FD2-002590A80DFA.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1E36ED8D-98EC-E511-93B4-002590A8881E.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/606FDC25-E7EB-E511-B108-0022195578C8.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A6C30C9D-CCEC-E511-8DFE-001E6757E03C.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/32F33D48-CCEB-E511-ABD8-001F2908EC96.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/04CACA93-CDEB-E511-8F3D-001CC4A6CC9E.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/2255BE78-CEEB-E511-BF44-001CC4A63C82.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A200DC4A-CCEB-E511-8E56-001F2908CE58.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/3AB6D499-CDEB-E511-BD85-001CC4BC4FC4.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/82A38D89-ADEC-E511-99ED-44A842CF0600.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/04F02BF7-CEEB-E511-8129-0026B95CD814.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/12B4A456-CFEB-E511-A769-0026B95C1CE6.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/22422A1A-CFEB-E511-B37B-782BCB71A9BF.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/D48AC2EB-CEEB-E511-BA4E-782BCB408B63.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/0449AA5F-C0EC-E511-94C3-842B2B185C3B.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/72E4C12E-48ED-E511-90AE-008CFA197D34.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/16C4AA27-4CED-E511-8DA5-782BCB40844A.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/CA69F49C-F3EB-E511-BD46-002590D0B064.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/3EB04D89-F3EB-E511-A1E5-002590747DDC.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/2CFC8F33-F5EB-E511-93FA-0CC47A4D9A5E.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/80F56D35-F5EB-E511-BCF9-002590D0AFA8.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/062C9CFE-F5EB-E511-B2C1-002590D0AFAE.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/2C858D73-8DEC-E511-AC3D-00259073E3AE.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/E0AAAFDF-93EC-E511-83BB-0CC47A4DEDD2.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/32E38515-99EC-E511-AE41-0CC47A4DEDE0.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/3A1AFB6B-98EC-E511-AAEA-00259073E37C.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/062A3837-C4EB-E511-B6FF-0025909076D2.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/94E4A5E3-92EC-E511-9E53-002590908226.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/48B182EE-47ED-E511-906F-0CC47A4DEE2A.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/5489CAF4-47ED-E511-9974-0025905C42FE.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/F086279F-C1EB-E511-A26B-A0369F3016EC.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/00C98B3F-92EC-E511-8BE0-A0000420FE80.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/8AEA47EE-96EF-E511-96A4-001E67DDC24A.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/6A46FDE4-47ED-E511-9C32-549F35AE4F95.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/F09300EA-BDEB-E511-83A0-6CC2173BBEF0.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/0C3EA1F7-93EC-E511-B48C-6CC2173BBA30.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1A3E9A2B-BEEB-E511-8600-002481DE4668.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/AC07462E-BEEB-E511-B8FC-00269E95ACE8.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/B417F331-BEEB-E511-BBCE-0017A4771004.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/2AA7475B-C2EB-E511-97EC-00269E95B128.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/12CF8352-D3EC-E511-A9C5-002481CFE642.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/30291963-97EC-E511-B42E-0CC47A4D76C0.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/D679FF67-97EC-E511-9E7C-0CC47A4C8EE8.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/B60E7E5B-97EC-E511-B89D-0CC47A78A42E.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/7EAB8878-97EC-E511-BB24-0025905A612E.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/6031887F-97EC-E511-A184-0CC47A4C8F12.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/38E0C38A-97EC-E511-AE49-0025905A48B2.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1E624A74-97EC-E511-860E-0025905A6068.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/0A100D5E-97EC-E511-A58B-0CC47A4D7692.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/3C7C86BA-9BEC-E511-ABE4-0CC47A4D767A.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/BA8A0DC0-9BEC-E511-B394-0025905A60F4.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/C2251D34-A2EC-E511-8652-0025905A60A8.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/941EA928-A2EC-E511-9E14-0025905A60B2.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/2E6B1385-0DED-E511-9C7D-0025905A60AA.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/4472D05B-C2EB-E511-A8C1-001EC9B21754.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/5C47275E-C2EB-E511-A4A4-D4AE526A0461.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/60A057DB-90EF-E511-9706-7845C4FC3641.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/06CB6C19-C6EB-E511-AFA5-D4AE526A0A4B.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/3485BB3E-C4EB-E511-B3E4-842B2B768127.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/EAC58898-C4EB-E511-B55E-0CC47A01CAEA.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/12F1A096-C4EB-E511-A3F1-D4AE526A0A4B.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/D850AA33-C6EB-E511-A57A-D4AE526A0A4B.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/001268DD-CAEB-E511-AC7F-842B2B5C2299.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/96C6D976-CAEB-E511-8E7C-842B2B76832A.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/607A5226-33EC-E511-9A4E-C45444922BFE.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/BCD8A8D0-3BEC-E511-A1A7-0CC47A00A832.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/58D55D4E-39EC-E511-B650-0CC47A009258.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/4070B19F-39EC-E511-9E81-842B2B7682C7.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/3CBC7729-28ED-E511-8BBD-D4AE526A0B8F.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/929D9D11-29ED-E511-8E3A-0025904C7F72.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/E00937BB-49ED-E511-826B-4B2E681C7A63.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/4C3D72FB-47ED-E511-8F15-002590E3A286.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/FEF49B54-68EF-E511-BAEC-3417EBE64B91.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/56862502-66EF-E511-8AE3-3417EBE643F0.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/0EE61FFF-65EF-E511-A366-3417EBE64537.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1CF664FB-65EF-E511-8869-7845C4FC37A9.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/DE367E13-64EF-E511-859F-00A0D1EC3950.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/4A43D00C-64EF-E511-BEF6-00266CFAE890.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/121BAD12-64EF-E511-9D30-00266CF9B8B0.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/8C93C417-64EF-E511-A0CD-00266CFAE074.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/AC809D3E-6BEF-E511-A278-00266CF9B274.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/BADA5CF9-65EF-E511-B3D8-180373FFCE1E.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/88FFDBFA-65EF-E511-9CEB-7845C4FC3770.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/C0C3A430-6BEF-E511-B74A-3417EBE5280A.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/6671244F-74EF-E511-B852-00266CFAE484.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/007555B7-7EEF-E511-8937-00266CF9AE10.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/0ED37AC1-7EEF-E511-9CFA-00266CFAE890.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/70E55C86-90EF-E511-9574-00266CF9AEA4.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/38FCBC5F-CCEB-E511-9870-E9A29E484C3A.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/F456BAF6-CBEB-E511-974C-C91B1AA3EA3E.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/446B77EE-CBEB-E511-8E1D-BBD5143029BB.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/0A0AA368-CBEB-E511-A406-0D23B6AFA6B4.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/9C3D723D-CBEB-E511-900E-7D51C783F5B1.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/F2EB29EF-CBEB-E511-9203-0FD3D5BFBFF7.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/464C0C6D-CBEB-E511-AC31-BD6E61BB4F04.root',
  'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/D2D5E4C8-DFEC-E511-A220-B90B5588EECD.root'
  ),
  skipEvents = cms.untracked.uint32(opts.firstevent)  
  )
elif opts.MGsignalVBF:
  process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/64130B06-AE54-E611-A4DF-28924A35058E.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/00E98807-AE54-E611-9D79-3417EBE47C5E.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/D6C80A48-AE54-E611-89C2-0023AEEEB55F.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/88679710-B354-E611-8954-009C02AAB4C0.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/EAB61AD7-B254-E611-B834-14DDA9D4F218.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/E0383827-B954-E611-BE91-0023AEEEB538.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/F877ECA9-C654-E611-99D0-0023AEEEB3D6.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/B2DC349D-C654-E611-BA79-28924A33AF26.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/ACF36CD4-D254-E611-9037-0026B94DBDF0.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/6E375ED3-D254-E611-A76E-28924A33B062.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/8C105DA8-E954-E611-9787-0023AEEEB3D6.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/DC56240B-F254-E611-8307-0023AEEEB3D6.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/FE00D9FC-F254-E611-B48B-BC305B390A80.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/74F7F5FA-F254-E611-BCFA-0026B94DBE24.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/8C138265-0955-E611-AACB-0026B94DBDD6.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/5ED5CF71-7956-E611-A90D-0026B94DBD95.root',
  'root://xrootd.unl.edu//store/mc/RunIISpring16MiniAODv2/VBFHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/20000/2E466272-D855-E611-ACBB-0026B94DBE0A.root'
  ),
  skipEvents = cms.untracked.uint32(opts.firstevent) 
  )
else:
  process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
  infile
  ))
  
process.GlobalTag.globaltag = cms.string(tag)

process.options   = cms.untracked.PSet(
  FailPath=cms.untracked.vstring("FileReadError")
)

import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

process.load("PhysicsTools.PatAlgos.patSequences_cff")
################################################################
# Re-do PFTau reconstruction
################################################################
process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")
#switchToPFTauHPS(process)

process.icMiniAODSequence = cms.Sequence()

################################################################
# Object Selection
################################################################

from RecoTauTag.RecoTau.TauDiscriminatorTools import noPrediscriminants
process.load('RecoTauTag.Configuration.loadRecoTauTagMVAsFromPrepDB_cfi')
from RecoTauTag.RecoTau.PATTauDiscriminationByMVAIsolationRun2_cff import *
from RecoTauTag.RecoTau.PATTauDiscriminationAgainstElectronMVA6_cfi import *

process.rerunDiscriminationByIsolationMVArun2v1raw = patDiscriminationByIsolationMVArun2v1raw.clone(
   PATTauProducer = cms.InputTag('slimmedTaus'),
   Prediscriminants = noPrediscriminants,
   loadMVAfromDB = cms.bool(True),
   mvaName = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1"), # name of the training you want to use
   mvaOpt = cms.string("DBoldDMwLT"), # option you want to use for your training (i.e., which variables are used to compute the BDT score)
   requireDecayMode = cms.bool(True),
   verbosity = cms.int32(0)
)

process.rerunDiscriminationByIsolationMVArun2v1VLoose = patDiscriminationByIsolationMVArun2v1VLoose.clone(
   PATTauProducer = cms.InputTag('slimmedTaus'),    
   Prediscriminants = noPrediscriminants,
   toMultiplex = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1raw'),
   key = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1raw:category'),
   loadMVAfromDB = cms.bool(True),
   mvaOutput_normalization = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1_mvaOutput_normalization"), # normalization fo the training you want to use
   mapping = cms.VPSet(
      cms.PSet(
         category = cms.uint32(0),
         cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1_WPEff90"), # this is the name of the working point you want to use
         variable = cms.string("pt"),
      )
   )
)

# here we produce all the other working points for the training
process.rerunDiscriminationByIsolationMVArun2v1Loose = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1Loose.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1_WPEff80")
process.rerunDiscriminationByIsolationMVArun2v1Medium = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1Medium.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1_WPEff70")
process.rerunDiscriminationByIsolationMVArun2v1Tight = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1Tight.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1_WPEff60")
process.rerunDiscriminationByIsolationMVArun2v1VTight = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1VTight.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1_WPEff50")
process.rerunDiscriminationByIsolationMVArun2v1VVTight = process.rerunDiscriminationByIsolationMVArun2v1VLoose.clone()
process.rerunDiscriminationByIsolationMVArun2v1VVTight.mapping[0].cut = cms.string("RecoTauTag_tauIdMVAIsoDBoldDMwLT2016v1_WPEff40")

# this sequence has to be included in your cms.Path() before your analyzer which accesses the new variables is called.
process.rerunMvaIsolation2SeqRun2 = cms.Sequence(
   process.rerunDiscriminationByIsolationMVArun2v1raw
   *process.rerunDiscriminationByIsolationMVArun2v1VLoose
   *process.rerunDiscriminationByIsolationMVArun2v1Loose
   *process.rerunDiscriminationByIsolationMVArun2v1Medium
   *process.rerunDiscriminationByIsolationMVArun2v1Tight
   *process.rerunDiscriminationByIsolationMVArun2v1VTight
   *process.rerunDiscriminationByIsolationMVArun2v1VVTight
)


process.rerunDiscriminationAgainstElectronMVA6 = patTauDiscriminationAgainstElectronMVA6.clone(
   PATTauProducer = cms.InputTag('slimmedTaus'),
   Prediscriminants = noPrediscriminants,
   #Prediscriminants = requireLeadTrack,
   loadMVAfromDB = cms.bool(True),
   returnMVA = cms.bool(True),
   method = cms.string("BDTG"),
   mvaName_NoEleMatch_woGwoGSF_BL = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_NoEleMatch_woGwoGSF_BL"),
   mvaName_NoEleMatch_wGwoGSF_BL = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_NoEleMatch_wGwoGSF_BL"),
   mvaName_woGwGSF_BL = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_woGwGSF_BL"),
   mvaName_wGwGSF_BL = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_wGwGSF_BL"),
   mvaName_NoEleMatch_woGwoGSF_EC = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_NoEleMatch_woGwoGSF_EC"),
   mvaName_NoEleMatch_wGwoGSF_EC = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_NoEleMatch_wGwoGSF_EC"),
   mvaName_woGwGSF_EC = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_woGwGSF_EC"),
   mvaName_wGwGSF_EC = cms.string("RecoTauTag_antiElectronMVA6v1_gbr_wGwGSF_EC"),
   minMVANoEleMatchWOgWOgsfBL = cms.double(0.0),
   minMVANoEleMatchWgWOgsfBL  = cms.double(0.0),
   minMVAWOgWgsfBL            = cms.double(0.0),
   minMVAWgWgsfBL             = cms.double(0.0),
   minMVANoEleMatchWOgWOgsfEC = cms.double(0.0),
   minMVANoEleMatchWgWOgsfEC  = cms.double(0.0),
   minMVAWOgWgsfEC            = cms.double(0.0),
   minMVAWgWgsfEC             = cms.double(0.0),
   srcElectrons = cms.InputTag('slimmedElectrons')
)

# embed new id's into new tau collection
embedID = cms.EDProducer("PATTauIDEmbedder",
   src = cms.InputTag('slimmedTaus'),
   tauIDSources = cms.PSet(
      byIsolationMVArun2v1DBoldDMwLTrawNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1raw'),
      byVLooseIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1VLoose'),
      byLooseIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1Loose'),
      byMediumIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1Medium'),
      byTightIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1Tight'),
      byVTightIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1VTight'),
      byVVTightIsolationMVArun2v1DBoldDMwLTNew = cms.InputTag('rerunDiscriminationByIsolationMVArun2v1VVTight'),
      againstElectronMVA6RawNew = cms.InputTag('rerunDiscriminationAgainstElectronMVA6')
      ),
   )
setattr(process, "NewTauIDsEmbedded", embedID)

process.selectedElectrons = cms.EDFilter("PATElectronRefSelector",
    src = cms.InputTag("slimmedElectrons"),
    cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
    )
process.selectedMuons = cms.EDFilter("PATMuonRefSelector",
    src = cms.InputTag("slimmedMuons"),
    cut = cms.string("pt > 3 & abs(eta) < 2.6")
    )
process.selectedTaus = cms.EDFilter("PATTauRefSelector",
    src = cms.InputTag("NewTauIDsEmbedded"),
    cut = cms.string('pt > 18.0 & abs(eta) < 2.6 & tauID("decayModeFindingNewDMs") > 0.5')
    )


process.icSelectionSequence = cms.Sequence()

process.icSelectionSequence += cms.Sequence(  
  process.rerunMvaIsolation2SeqRun2+
  process.rerunDiscriminationAgainstElectronMVA6+
  getattr(process, "NewTauIDsEmbedded")
)


process.icSelectionSequence += cms.Sequence(
  process.selectedElectrons+
  process.selectedMuons+
  process.selectedTaus
)

################################################################
# PF sequence for lepton isolation
################################################################
process.load("CommonTools.ParticleFlow.pfParticleSelection_cff")
if release in ['76X']:
  process.pfPileUp.PFCandidates = cms.InputTag("particleFlowPtrs")
  process.pfPileUpIso.PFCandidates = cms.InputTag("particleFlowPtrs")
  process.pfNoPileUp.bottomCollection = cms.InputTag("particleFlowPtrs")
  process.pfNoPileUpIso.bottomCollection = cms.InputTag("particleFlowPtrs")


if release in ['80XMINIAOD']:
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

vtxLabel = cms.InputTag("offlinePrimaryVertices")
if release in ['80XMINIAOD']:
  vtxLabel = cms.InputTag("offlineSlimmedPrimaryVertices")

################################################################
# Vertices
################################################################
process.icVertexProducer = producers.icVertexProducer.clone(
  branch  = cms.string("vertices"),
  input = vtxLabel,
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
# PFCandidates
################################################################
#process.icPFProducer = cms.EDProducer('ICPFProducer',
#  branch  = cms.string("pfCandidates"),
#  input   = cms.InputTag("selectedPFCandidates"),
#  requestTracks       = cms.bool(True),
#  requestGsfTracks    = cms.bool(True)
#)

#process.icPFSequence = cms.Sequence()
# process.icPFSequence += process.icPFProducer


################################################################
# Tracks
################################################################
#process.selectedTracks = cms.EDFilter("TrackRefSelector",
#  src = cms.InputTag("generalTracks"),
#  cut = cms.string("pt > 0.5")
#)

# We write (for phys 14 studies):
# - all tracks with pT > 5 GeV
# - tracks referenced by the PF candidates we store
# - tracks referenced by the taus we store
# - all tracks with DR < 0.5 pf the selected PF taus with pT > 0.5 GeV
#process.icMergedTracks = cms.EDProducer('ICTrackMerger',
#  merge = cms.VInputTag(
#    cms.InputTag("selectedTracks"),
#    cms.InputTag("icTauProducer", "requestedTracks")
    # cms.InputTag("icPFProducer", "requestedTracks"),
#  )
#)

#process.icTrackProducer = producers.icTrackProducer.clone(
#  branch = cms.string("tracks"),
#  input  = cms.InputTag("icMergedTracks")
#)

#process.icGsfTrackProducer = producers.icTrackProducer.clone(
#  branch = cms.string("gsfTracks"),
#  input  = cms.InputTag("icPFProducer", "requestedGsfTracks")
#)

#process.icTrackSequence = cms.Sequence()
#process.icTrackSequence += cms.Sequence(
#   process.selectedTracks+
#   process.icMergedTracks+
#   process.icTrackProducer+
#   process.icGsfTrackProducer
#)

################################################################
# Electrons
################################################################
electronLabel = cms.InputTag("gedGsfElectrons")
if release in ['80XMINIAOD']:
  electronLabel = cms.InputTag("slimmedElectrons")

process.icElectronSequence = cms.Sequence()

#ICElectronConversionCalculator NOT final, but at least have a running version for now
process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
    input       = electronLabel, 
    beamspot    = cms.InputTag("offlineBeamSpot"),
    conversions = cms.InputTag("allConversions")
)

if release in ['80XMINIAOD']:
  process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
      input       = electronLabel,
      beamspot    = cms.InputTag("offlineBeamSpot"),
      conversions = cms.InputTag("reducedEgamma:reducedConversions")
  )

process.load("RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi")
process.electronMVAValueMapProducer.src = electronLabel
process.electronMVAValueMapProducer.srcMiniAOD = electronLabel

process.icElectronSequence+=cms.Sequence(
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

if release in ['80XMINIAOD']:
  process.elPFIsoDepositGamma.ExtractorPSet.ComponentName = cms.string("CandViewExtractor")
process.icElectronSequence += cms.Sequence(
#      process.electronPFIsolationDepositsSequence+
    process.elPFIsoDepositChargedAll+
    process.electronPFIsolationValuesSequence
    )
#Htt electron vetoes:
#process.elPFIsoValueGamma03PFIdPFIso.deposits[0].vetos = (
#    cms.vstring('EcalEndcaps:ConeVeto(0.08)','EcalBarrel:ConeVeto(0.08)'))
#process.elPFIsoValueNeutral03PFIdPFIso.deposits[0].vetos = (
#    cms.vstring())
#process.elPFIsoValuePU03PFIdPFIso.deposits[0].vetos = (
#    cms.vstring())
#process.elPFIsoValueCharged03PFIdPFIso.deposits[0].vetos = (
#    cms.vstring('EcalEndcaps:ConeVeto(0.015)'))
process.elPFIsoValueChargedAll03PFIdPFIso.deposits[0].vetos = (
    cms.vstring('EcalEndcaps:ConeVeto(0.015)','EcalBarrel:ConeVeto(0.01)'))

if release in ['80XMINIAOD']:
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

if release in ['76X']:#Need to recalculate this as 04 isolation is stored in pat not reco electrons
  process.elPFIsoValueCharged04PFIdPFIso    = process.elPFIsoValueCharged04PFId.clone()
  process.elPFIsoValueGamma04PFIdPFIso      = process.elPFIsoValueGamma04PFId.clone()
  process.elPFIsoValueNeutral04PFIdPFIso    = process.elPFIsoValueNeutral04PFId.clone()
  process.elPFIsoValuePU04PFIdPFIso         = process.elPFIsoValuePU04PFId.clone()
  process.elPFIsoDepositCharged.src     = electronLabel
  process.elPFIsoDepositNeutral.src     = electronLabel
  process.elPFIsoDepositGamma.src       = electronLabel
  process.elPFIsoDepositPU.src          = electronLabel
  process.elPFIsoValueGamma04PFIdPFIso.deposits[0].vetos = (
      cms.vstring('EcalEndcaps:ConeVeto(0.08)','EcalBarrel:ConeVeto(0.08)'))
  process.elPFIsoValueNeutral04PFIdPFIso.deposits[0].vetos = (
      cms.vstring())
  process.elPFIsoValuePU04PFIdPFIso.deposits[0].vetos = (
      cms.vstring())
  process.elPFIsoValueCharged04PFIdPFIso.deposits[0].vetos = (
      cms.vstring('EcalEndcaps:ConeVeto(0.015)'))
  process.electronPFIsolationValuesSequence += cms.Sequence(
    process.elPFIsoDepositCharged+
    process.elPFIsoDepositNeutral+
    process.elPFIsoDepositGamma+
    process.elPFIsoDepositPU
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
     mvaTrigSpring15       = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15Trig25nsV1Values"),
     generalPurposeMVASpring16  = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring16GeneralPurposeV1Values")
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
if release in ['76X']: muons = cms.InputTag("muons")
if release in ['80XMINIAOD']: muons = cms.InputTag("slimmedMuons")

process.load("CommonTools.ParticleFlow.Isolation.pfMuonIsolation_cff")
process.load("CommonTools.ParticleFlow.deltaBetaWeights_cff")
#process.muPFIsoDepositCharged.src     = muons
process.muPFIsoDepositChargedAll.src  = muons #All charged isolation is not stored in reco/pat
process.muPFIsoDepositNeutral.src     = muons
process.muPFIsoDepositNeutral.ExtractorPSet.inputCandView = cms.InputTag("pfWeightedNeutralHadrons")
process.muPFIsoDepositGamma.src       = muons
process.muPFIsoDepositGamma.ExtractorPSet.inputCandView = cms.InputTag("pfWeightedPhotons")
#process.muPFIsoDepositPU.src          = muons
#process.icMuonSequence += cms.Sequence(
#    process.muonPFIsolationDepositsSequence+
#    process.muonPFIsolationValuesSequence
#    )

process.muPFIsoValueChargedAll03PFIso = process.muPFIsoValueChargedAll03.clone()
process.muPFIsoValueNeutral04PFWeights = process.muPFIsoValueNeutral04.clone()
process.muPFIsoValueGamma04PFWeights = process.muPFIsoValueGamma04.clone()
process.muPFIsoValueNeutral03PFWeights = process.muPFIsoValueNeutral03.clone()
process.muPFIsoValueGamma03PFWeights = process.muPFIsoValueGamma03.clone()
process.muPFIsoValueCharged03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("charged_iso") 
)    
process.muPFIsoValueGamma03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("photon_iso") 
)    
process.muPFIsoValueNeutral03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("neutral_iso") 
)    
process.muPFIsoValuePU03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("pu_iso") 
)    

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
  input        = muons,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("charged_iso") 
)    
process.muPFIsoValueGamma04PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("photon_iso") 
)    
process.muPFIsoValueNeutral04PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("neutral_iso") 
)    
process.muPFIsoValuePU04PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
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



process.icMuonProducer = producers.icMuonProducer.clone(
  branch                    = cms.string("muons"),
  input                     = cms.InputTag("selectedMuons"),
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
if release in ['80XMINIAOD']: process.icMuonProducer.isPF = cms.bool(False)

process.icMuonSequence += cms.Sequence(
  process.icMuonProducer
)

################################################################
# Taus
################################################################
import UserCode.ICHiggsTauTau.tau_discriminators_cfi as tauIDs

process.icTauProducer = producers.icTauProducer.clone(
  input                   = cms.InputTag("selectedTaus"),
  inputVertices           = vtxLabel,
  includeVertexIP         = cms.bool(True),
  requestTracks           = cms.bool(True),
  tauIDs = tauIDs.dynamicStripIds
)

if release in ['80XMINIAOD']:
  process.icTauProducer = cms.EDProducer("ICPFTauFromPatProducer",
    branch                  = cms.string("taus"),
    input                   = cms.InputTag("selectedTaus"),
    inputVertices           = vtxLabel,
    includeVertexIP         = cms.bool(True),
    requestTracks           = cms.bool(False),
    includeTotalCharged     = cms.bool(False),
    totalChargedLabel       = cms.string('totalCharged'),
    requestPFCandidates     = cms.bool(False),
    inputPFCandidates       = cms.InputTag("packedPFCandidates"),
    isSlimmed               = cms.bool(True),
    tauIDs = cms.PSet()
  )


process.icTauSequence = cms.Sequence(
  process.icTauProducer 
)

# ################################################################
# # Jets
# ################################################################
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
if release in ['80XMINIAOD']:
  #rebuild ak4 chs jets as in  https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD#Jets
  process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
#  process.pfchs=cms.EDFilter("CandPtrSelector",src=cms.InputTag("packedPFCandidates"),cut=cms.string("fromPV"))
#  process.ak4PFJetsCHS = ak4PFJets.clone(src='pfchs',doAreaFastjet=True)

  #Also make non-chs jets:
#  process.ak4PFJets = ak4PFJets.clone(src='packedPFCandidates',doAreaFastjet=True)
  
  #Reapply JECs:
  if not isData:
    updateJetCollection(
      process,
      jetSource = cms.InputTag("slimmedJets"),
      labelName = "UpdatedJEC",
      jetCorrections = ("AK4PFchs", cms.vstring(['L1FastJet','L2Relative','L3Absolute']), 'None')
    )
  else:
   updateJetCollection(
     process,
     jetSource = cms.InputTag("slimmedJets"),
     labelName = "UpdatedJEC",
     jetCorrections = ("AK4PFchs", cms.vstring(['L1FastJet','L2Relative','L3Absolute','L2L3Residual']), 'None')
    )
 
  process.selectedSlimmedJetsAK4 = cms.EDFilter("PATJetRefSelector",
      src = cms.InputTag("selectedUpdatedPatJetsUpdatedJEC"),
      cut = cms.string("pt > 15")
      )



#if release in ['80XMINIAOD']:
#   process.kt6PFJets = kt4PFJets.clone(
#       src = 'packedPFCandidates',
#       rParam = cms.double(0.6),
#       doAreaFastjet = cms.bool(True),
#       doRhoFastjet = cms.bool(True)
#       )

 # Parton flavour
 # --------------
#process.jetPartons = cms.EDProducer("PartonSelector",
#     src = cms.InputTag("genParticles"),
#     withLeptons = cms.bool(False)
#)

#process.pfJetPartonMatches = cms.EDProducer("JetPartonMatcher",
#     jets = cms.InputTag("ak4PFJetsCHS"),
#     coneSizeToAssociate = cms.double(0.3),
#     partons = cms.InputTag("jetPartons")
#)

#process.jetPartons = cms.EDProducer('HadronAndPartonSelector',
#  src = cms.InputTag("generator"),
#  particles = cms.InputTag("genParticles","","HLT"),
#  partonMode = cms.string("Auto")
#)

#process.pfJetFlavourAssociation = cms.EDProducer("JetFlavourClustering",
#     jets = cms.InputTag("ak4PFJetsCHS"),
#     bHadrons = cms.InputTag("jetPartons","bHadrons"),
#     cHadrons = cms.InputTag("jetPartons","cHadrons"),
#     partons = cms.InputTag("jetPartons","algorithmicPartons"),
#     leptons = cms.InputTag("jetPartons","leptons"),
#     jetAlgorithm = cms.string("AntiKt"),
#     rParam = cms.double(0.4),
#     ghostRescaling = cms.double(1e-18),
#     hadronFlavourHasPriority = cms.bool(False)
#)


#process.icPFJetFlavourCalculator = cms.EDProducer('ICJetFlavourCalculator',
#     input       = cms.InputTag("ak4PFJetsCHS"),
#     flavourMap  = cms.InputTag("pfJetFlavourAssociation")
#)
#

#if release in ['80XMINIAOD']:
#   process.jetPartons.particles = cms.InputTag("prunedGenParticles","","PAT")



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
pfJECS = cms.PSet(
  L1FastJet  = cms.string("ak4PFL1FastjetCHS"),
  L2Relative = cms.string("ak4PFL2RelativeCHS"),
  L3Absolute = cms.string("ak4PFL3AbsoluteCHS")
)
if isData: pfJECS = cms.PSet(
  L1FastJet  = cms.string("ak4PFL1FastjetCHS"),
  L2Relative = cms.string("ak4PFL2RelativeCHS"),
  L3Absolute = cms.string("ak4PFL3AbsoluteCHS"),
  L2L3Residual = cms.string("ak4PFResidualCHS")
)

 # b-tagging
 # ---------
process.load("RecoJets.JetAssociationProducers.ak4JTA_cff")
from RecoJets.JetAssociationProducers.ak4JTA_cff import ak4JetTracksAssociatorAtVertex
process.load("RecoBTag.Configuration.RecoBTag_cff")
import RecoBTag.Configuration.RecoBTag_cff as btag
#process.jetTracksAssociatorAtVertexAK4PFCHS = ak4JetTracksAssociatorAtVertex.clone(
#  jets = cms.InputTag("ak4PFJetsCHS")
#)

#if release in ['80XMINIAOD']:
#  process.jetTracksAssociatorAtVertexAK4PFCHS.tracks = cms.InputTag("unpackedTracksAndVertices")
#  process.jetTracksAssociatorAtVertexAK4PFCHS.pvSrc = cms.InputTag("unpackedTracksAndVertices")

#if isEmbedded:
#  process.jetTracksAssociatorAtVertexAK5PF.tracks = cms.InputTag("tmfTracks")

#process.impactParameterTagInfosAK4PFCHS = btag.pfImpactParameterTagInfos.clone(
  #jetTracks = cms.InputTag('jetTracksAssociatorAtVertexAK4PFCHS')
#)
#if release in ['80XMINIAOD']:
#  process.impactParameterTagInfosAK4PFCHS.computeGhostTrack=cms.bool(False)
#  process.impactParameterTagInfosAK4PFCHS.candidates=cms.InputTag("packedPFCandidates")
  #process.impactParameterTagInfosAK4PFCHS.primaryVertex = cms.InputTag("offlineSlimmedPrimaryVertices")
#  process.impactParameterTagInfosAK4PFCHS.primaryVertex = cms.InputTag("unpackedTracksAndVertices")

#process.secondaryVertexTagInfosAK4PFCHS = btag.pfSecondaryVertexTagInfos.clone(
#  trackIPTagInfos = cms.InputTag('impactParameterTagInfosAK4PFCHS')
#)
##Btag discriminators need to be properly updated 
#process.simpleSecondaryVertexHighEffBJetTagsAK4PFCHS = btag.pfSimpleSecondaryVertexHighEffBJetTags.clone (
#  tagInfos = cms.VInputTag('secondaryVertexTagInfosAK4PFCHS')
#)
#process.simpleSecondaryVertexHighPurBJetTagsAK4PFCHS = btag.pfSimpleSecondaryVertexHighPurBJetTags.clone (
#  tagInfos = cms.VInputTag('secondaryVertexTagInfosAK4PFCHS')
#)
#process.combinedSecondaryVertexBJetTagsAK4PFCHS = btag.pfCombinedSecondaryVertexV2BJetTags.clone (
#  tagInfos = cms.VInputTag('impactParameterTagInfosAK4PFCHS', 'secondaryVertexTagInfosAK4PFCHS')
#)


#process.btaggingSequenceAK4PF = cms.Sequence(
#  process.jetTracksAssociatorAtVertexAK4PFCHS
#  +process.impactParameterTagInfosAK4PFCHS
#  +process.secondaryVertexTagInfosAK4PFCHS
#  +process.simpleSecondaryVertexHighEffBJetTagsAK4PFCHS
#  +process.simpleSecondaryVertexHighPurBJetTagsAK4PFCHS
#  +process.combinedSecondaryVertexBJetTagsAK4PFCHS
# )

#if release in ['74X',]:
#process.btaggingSequenceAK4PF += cms.Sequence(
#  process.pfImpactParameterTagInfos+
#  process.pfSecondaryVertexTagInfos+
#  process.pfCombinedSecondaryVertexV2BJetTags
#)

if release in ['80XMINIAOD']:
  process.pfImpactParameterTagInfos.primaryVertex = cms.InputTag("offlineSlimmedPrimaryVertices")
  process.pfImpactParameterTagInfos.candidates = cms.InputTag("packedPFCandidates")

 # Pileup ID
 # ---------
 # Recalculated puJetId isn't the same as miniaod stored - should investigate 
#stdalgos = cms.VPSet()
#from RecoJets.JetProducers.PileupJetIDParams_cfi import *
#stdalgos = cms.VPSet(full_5x_chs,cutbased)
process.load('RecoJets.JetProducers.PileupJetID_cfi')

if release in ['80XMINIAOD']:
  process.pileupJetIdCalculator.vertexes = cms.InputTag("offlineSlimmedPrimaryVertices")
process.pileupJetIdCalculator.jets = cms.InputTag("ak4PFJetsCHS")
process.pileupJetIdCalculator.rho = cms.InputTag("fixedGridRhoFastjetAll")
process.pileupJetIdEvaluator.jets = cms.InputTag("ak4PFJetsCHS")
process.pileupJetIdEvaluator.rho = cms.InputTag("fixedGridRhoFastjetAll")

#process.puJetMvaRe = cms.EDProducer('PileupJetIdProducer',
#    produceJetIds = cms.bool(True),
#    jetids = cms.InputTag(""),
#    runMvas = cms.bool(True),
    #jets = cms.InputTag("slimmedJets"),
#    jets = cms.InputTag("ak4PFJetsCHS"),
#    vertexes = cms.InputTag("offlinePrimaryVertices"),
#    vertexes = cms.InputTag("unpackedTracksAndVertices"),
#    algos = cms.VPSet(stdalgos),
#    rho     = cms.InputTag("kt6PFJets", "rho"),
#    rho     = cms.InputTag("fixedGridRhoFastjetAll"),
#    jec     = cms.string("AK4PFchs"),
#    applyJec = cms.bool(True),
#    inputIsCorrected = cms.bool(False),
#    residualsFromTxt = cms.bool(False),
#    residualsTxt     = cms.FileInPath("RecoJets/JetProducers/data/dummy.txt"),
#)

#if release in ['80XMINIAOD']:
#  process.puJetMvaRe.vertexes = cms.InputTag("offlineSlimmedPrimaryVertices")

#if release in ['76X', '80XMINIAOD']:
#  process.puJetMvaRe.residualsTxt = cms.FileInPath("RecoJets/JetProducers/BuildFile.xml")



 # Producer
 # --------
#process.icPFJetProducer = producers.icPFJetProducer.clone(
#    branch                    = cms.string("ak4PFJetsCHS"),
#    input                     = cms.InputTag("ak4PFJetsCHS"),
#    srcConfig = cms.PSet(
#      includeJetFlavour         = cms.bool(True),
#      inputJetFlavour           = cms.InputTag("icPFJetFlavourCalculator"),
#      applyJECs                 = cms.bool(True),
#      includeJECs               = cms.bool(True),
#      JECs                      = pfJECS,
#      applyCutAfterJECs         = cms.bool(True),
#      cutAfterJECs              = cms.string("pt > 15.0"),
#      inputSVInfo               = cms.InputTag(""),
#      requestSVInfo             = cms.bool(False),
#      BTagDiscriminators        = cms.PSet(
#        simpleSecondaryVertexHighEffBJetTags = cms.InputTag("simpleSecondaryVertexHighEffBJetTagsAK4PFCHS"),
#        simpleSecondaryVertexHighPurBJetTags = cms.InputTag("simpleSecondaryVertexHighPurBJetTagsAK4PFCHS"),
#        combinedSecondaryVertexBJetTags      = cms.InputTag("combinedSecondaryVertexBJetTagsAK4PFCHS"),
#        #include this discriminator again under a different name to be compatible with miniAOD naming conventions
#        combinedInclusiveSecondaryVertexV2BJetTags      = cms.InputTag("combinedSecondaryVertexBJetTagsAK4PFCHS")
#      )
#    ),
#    destConfig = cms.PSet(
#      includePileupID       = cms.bool(True), #rerunning the pu MVA on the jet collection created in miniAOD is possible in newer CMSSW versions but not yet in 72
#      inputPileupID         = cms.InputTag("pileupJetIdEvaluator", "fullDiscriminant"),
#      includeTrackBasedVars = cms.bool(False),
#      inputTracks           = cms.InputTag("generalTracks"),
#      inputVertices         = vtxLabel,
#      requestTracks         = cms.bool(False)
#    )
#)

#if isData:
#  process.icPFJetProducer.srcConfig.includeJetFlavour = cms.bool(False)


if release in ['80XMINIAOD']:
#  process.icPFJetProducer.branch=cms.string("ak4PFJetsCHSReclustered")
#  process.icPFJetProducer.destConfig.includePileupID=cms.bool(False)
#  process.icPFJetProducer.destConfig.inputVertices=cms.InputTag("unpackedTracksAndVertices")

# if isPhys14:
#process.selectedPFJets = cms.EDFilter("PFJetRefSelector",
#    src = cms.InputTag("ak4PFJetsCHS"),
#    cut = cms.string("pt > 15")
#    )



  process.icPFJetProducerFromPat = producers.icPFJetFromPatProducer.clone(
      branch                    = cms.string("ak4PFJetsCHS"),
      input                     = cms.InputTag("selectedSlimmedJetsAK4"),
      srcConfig = cms.PSet(
        isSlimmed               = cms.bool(True),
        slimmedPileupIDLabel    = cms.string('pileupJetId:fullDiscriminant'),
        includeJetFlavour       = cms.bool(True),
        includeJECs             = cms.bool(True),
        inputSVInfo             = cms.InputTag(""),
        requestSVInfo           = cms.bool(False)
      ),
     destConfig = cms.PSet(
       includePileupID         = cms.bool(True),
       inputPileupID           = cms.InputTag("puJetMva", "fullDiscriminant"),
       includeTrackBasedVars   = cms.bool(False),
       inputTracks             = cms.InputTag("unpackedTracksAndVertices"),
       inputVertices           = cms.InputTag("unpackedTracksAndVertices"),
       requestTracks           = cms.bool(False)
      )
  )

process.icPFJetSequence = cms.Sequence()


# # 74X: error loading mva file
# if release in ['42X', '53X', '70X']:
#   process.icPFJetSequence += cms.Sequence(
#     process.puJetMva
#   )
if release in ['80XMINIAOD']:
  process.icPFJetSequence += cms.Sequence(
#     process.pfchs+
     process.patJetCorrFactorsUpdatedJEC+
     process.updatedPatJetsUpdatedJEC+
     process.selectedUpdatedPatJetsUpdatedJEC+
     process.selectedSlimmedJetsAK4+
     process.unpackedTracksAndVertices+
     process.icPFJetProducerFromPat
     )
if release in ['76X']:
  process.icPFJetSequence += cms.Sequence(
    process.pfNoPileUpJMESequence
    )
#if release in ['76X', '80XMINIAOD']:
#  process.icPFJetProducer.srcConfig.BTagDiscriminators = cms.PSet(
#  pfCombinedSecondaryVertexV2BJetTags = cms.InputTag("pfCombinedSecondaryVertexV2BJetTags")
#  )
#  process.icPFJetSequence += cms.Sequence(
#    process.ak4PFL1FastjetCHS+
#    process.ak4PFL2RelativeCHS+
#    process.ak4PFL3AbsoluteCHS+
#    process.ak4PFResidualCHS+
#    process.ak4PFJetsCHS+
#    process.pileupJetIdCalculator+
#    process.pileupJetIdEvaluator+ 
#    process.jetPartons+
#      process.pfJetPartonMatches+
#    process.pfJetFlavourAssociation+
#    process.icPFJetFlavourCalculator+
#    process.btaggingSequenceAK4PF+
#    process.icPFJetProducer #Not from slimmed jets!
#    )

#  if not isData:
#    process.icPFJetSequence.remove(process.ak4PFResidualCHS)

#  if isData:
#    process.icPFJetSequence.remove(process.jetPartons) 
#    process.icPFJetSequence.remove(process.pfJetFlavourAssociation) 
#    process.icPFJetSequence.remove(process.icPFJetFlavourCalculator)
#if release in ['74XMINIAOD','7412MINIAOD']:
#  process.icPFJetSequence.remove(process.puJetMva) #This works for jets built from PackedCandidates in CMSSW74X but not yet in 72


# ################################################################
# # MVA MET and PF MET
# ################################################################
process.load('JetMETCorrections.Configuration.JetCorrectors_cff')
#process.load('RecoMET.METPUSubtraction.mvaPFMET_cff')
process.load("RecoJets.JetProducers.ak4PFJets_cfi")

from RecoMET.METProducers.PFMET_cfi import pfMet
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
runMetCorAndUncFromMiniAOD(process,
                           isData=bool(isData),
                           )


process.pfMetRe = pfMet.clone(src = "particleFlow")

if release in ['80XMINIAOD']:
  process.pfMetRe = pfMet.clone(src = "packedPFCandidates")
  process.pfMetRe.calculateSignificance = False # this can't be easily implemented on packed PF candidates at the moment

if release in ['76X']:
  process.icPfMetProducer = producers.icMetProducer.clone(
                            input = cms.InputTag("pfMetRe"),
                            branch = cms.string("pfMet"),
                            includeCustomID = cms.bool(False),
                            inputCustomID = cms.InputTag("")
                            )

if release in ['80XMINIAOD']:
  process.icRecorrectedPfMetProducer = producers.icMetFromPatProducer.clone(
                           input=cms.InputTag("patpfMETT1"),
                           branch = cms.string("pfMet"),
                           getUncorrectedMet=cms.bool(False)
                           )
  process.icPfMetProducer = producers.icMetFromPatProducer.clone(
                           branch = cms.string("pfMetFromSlimmed"),
                           getUncorrectedMet=cms.bool(False),
                           includeMetUncertainties=cms.bool(True)
                           )
  process.icPuppiMetProducer = producers.icMetFromPatProducer.clone(
                           input=cms.InputTag("slimmedMETsPuppi"),
                           branch = cms.string("puppiMet"),
                           getUncorrectedMet=cms.bool(False)
                           )



process.icPfMetSequence = cms.Sequence(
  process.pfMetRe+
  process.icPfMetProducer
)

if release in ['80XMINIAOD']:
  process.icPfMetSequence.remove(process.pfMetRe)
  process.icPfMetSequence+=cms.Sequence(process.icPuppiMetProducer)
  #process.icPfMetSequence+=cms.Sequence(process.icRecorrectedPfMetProducer)

#from RecoMET.METPUSubtraction.MVAMETConfiguration_cff import runMVAMET
#runMVAMET(process, jetCollectionPF='selectedUpdatedPatJetsUpdatedJEC')
#process.MVAMET.debug = cms.bool(False)
#process.MVAMET.srcLeptons = cms.VInputTag("slimmedMuons","slimmedElectrons","slimmedTaus")
#process.MVAMET.requireOS = cms.bool(False)


#process.icPfMVAMetProducer = cms.EDProducer('ICPFMetFromPatProducer',
#  input = cms.InputTag("MVAMET","MVAMET"),
#  branch = cms.string("pfMVAMetVector"),
#  includeCustomID = cms.bool(False),
#  includeUserCandID = cms.bool(True),
#  includeExternalMetsig = cms.bool(False),
#  includeMetUncertainties = cms.bool(False),
#  metuncertainties = cms.vstring(),
#  metcorrections = cms.vstring(),
#  includeExternalMetsigMethod2 = cms.bool(False),
#  includeMetCorrections = cms.bool(False),
#  doGenMet = cms.bool(False),
#  metsig = cms.PSet(
#    metsig = cms.InputTag("METSignificance","METSignificance"),
#    metsigcov00 = cms.InputTag("METSignificance", "CovarianceMatrix00"),
#    metsigcov01 = cms.InputTag("METSignificance", "CovarianceMatrix01"),
#    metsigcov10 = cms.InputTag("METSignificance", "CovarianceMatrix10"),
#    metsigcov11 = cms.InputTag("METSignificance", "CovarianceMatrix11")
#    ),
#  metsig_method2 = cms.PSet(
#    metsig      = cms.InputTag("METSignificance","METSignificance"),
#    metsigcov = cms.InputTag("METSignificance","METCovariance")
#    ),
#  inputCustomID = cms.InputTag("")
#  )
#

#process.icMvaMetSequence = cms.Sequence(
#  process.tauDecayProducts+
#  process.egmGsfElectronIDs+
#  process.electronMVAValueMapProducer+
#  process.electronRegressionValueMapProducer+
#  process.photonIDValueMapProducer+
#  process.photonMVAValueMapProducer+
#  process.slimmedElectronsTight+
#  process.slimmedMuonsTight+
#  process.slimmedTausLoose+
#  process.slimmedTausLooseCleaned+
#  process.selectedUpdatedPatJetsUpdatedJECCleaned+
#  process.pfNeutrals+
#  process.neutralInJets+
#  process.pfChargedPV+
#  process.pfChs+
#  process.pfChargedPU+
#  process.pfMETCands+
#  process.pfTrackMETCands+
#  process.pfNoPUMETCands+
#  process.pfPUMETCands+
#  process.pfChargedPUMETCands+
#  process.pfNeutralPUMETCands+
#  process.pfNeutralPVMETCands+
#  process.pfNeutralUnclusteredMETCands+
#  process.pfPUCorrectedMETCands+
#  process.ak4PFCHSL1FastjetCorrector+
#  process.ak4PFCHSL2RelativeCorrector+
#  process.ak4PFCHSL3AbsoluteCorrector+
#  process.ak4PFCHSL1FastL2L3Corrector+
#  process.ak4PFCHSResidualCorrector+
#  process.ak4PFCHSL1FastL2L3ResidualCorrector+
#  process.ak4JetsForpfMET+
#  process.ak4JetsForpfTrackMET+
#  process.ak4JetsForpfPUMET+
#  process.ak4JetsForpfChargedPUMET+
#  process.ak4JetsForpfNeutralPUMET+
#  process.ak4JetsForpfNeutralPVMET+
#  process.ak4JetsForpfNeutralUnclusteredMET+
#  process.ak4JetsForpfPUCorrectedMET+
#  process.ak4JetsForpfNoPUMET+
#  process.corrpfMET+
#  process.corrpfTrackMET+
#  process.corrpfPUMET+
#  process.corrpfChargedPUMET+
#  process.corrpfNeutralPVMET+
#  process.corrpfNeutralUnclusteredMET+
#  process.corrpfNeutralPUMET+
#  process.corrpfPUCorrectedMET+
#  process.corrpfNoPUMET+
#  process.pfMET+
#  process.pfMETT1+
#  process.patpfMET+
#  process.pfTrackMET+
#  process.patpfTrackMET+
#  process.pfTrackMETT1+
#  process.patpfTrackMETT1+
#  process.pfPUCorrectedMET+
#  process.pfPUMET+
#  process.pfChargedPUMET+
#  process.pfNeutralPUMET+
#  process.patpfPUMET+
#  process.patpfChargedPUMET+
#  process.patpfNeutralPUMET+
#  process.pfNeutralPVMET+
#  process.pfNeutralUnclusteredMET+
#  process.patpfNeutralPVMET+
#  process.patpfNeutralUnclusteredMET+
#  process.patpfPUCorrectedMET+
#  process.pfPUMETT1+
#  process.patpfPUMETT1+
#  process.pfChargedPUMETT1+
#  process.pfNeutralPUMETT1+
#  process.patpfChargedPUMETT1+
#  process.patpfNeutralPUMETT1+
#  process.pfPUCorrectedMETT1+
#  process.patpfPUCorrectedMETT1+
#  process.pfNoPUMET+
 # process.patpfNoPUMET+
 # process.pfNoPUMETT1+
 # process.patpfNoPUMETT1+
 # process.patpfMETT1+
 # process.allDecayProducts+
 # process.tauPFMET+
 # process.tauMET+
 # process.tausSignificance+
 # process.MVAMET
 # #process.icPfMVAMetProducer
 # )

#if not isData:
#  process.icMvaMetSequence.remove(process.ak4PFCHSResidualCorrector)
#  process.icMvaMetSequence.remove(process.ak4PFCHSL1FastL2L3ResidualCorrector)


#from ICAnalysis.MVAMETPairProducer.mvaPFMET_cff_leptons_74X import mvaMetPairs

#if release in ['80XMINIAOD']:
#  process.ak4PFJets.src = cms.InputTag("packedPFCandidates")

#process.ak4PFJets.doAreaFastjet = cms.bool(True)

#from JetMETCorrections.Configuration.DefaultJEC_cff import ak4PFJetsL1FastL2L3
#from JetMETCorrections.Configuration.DefaultJEC_cff import ak4PFJetsL1FastL2L3

#process.mvaMetPairsTT = mvaMetPairs.clone(
# srcPFCandidates = cms.InputTag("packedPFCandidates"),
# srcVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
# srcLeptons = cms.VInputTag(
#   cms.InputTag("slimmedTaus","",""),
#   cms.InputTag("slimmedTaus","",""),
#  ),
#  permuteLeptons = cms.bool(True)
#)


#process.mvaMetPairsMT = mvaMetPairs.clone(
# srcPFCandidates = cms.InputTag("packedPFCandidates"),
# srcVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
# srcLeptons = cms.VInputTag(
#   cms.InputTag("slimmedMuons","",""),
#   cms.InputTag("slimmedTaus","",""),
#  ),
#  permuteLeptons = cms.bool(True)
#)


#process.mvaMetPairsET = mvaMetPairs.clone(
# srcPFCandidates = cms.InputTag("packedPFCandidates"),
# srcVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
# srcLeptons = cms.VInputTag(
#   cms.InputTag("slimmedElectrons","",""),
#   cms.InputTag("slimmedTaus","",""),
#  ),
#  permuteLeptons = cms.bool(True)
#  )
#process.mvaMetPairsEM = mvaMetPairs.clone(
# srcPFCandidates = cms.InputTag("packedPFCandidates"),
# srcVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
# srcLeptons = cms.VInputTag(
#   cms.InputTag("slimmedElectrons","",""),
#   cms.InputTag("slimmedMuons","",""),
#  ),
#  permuteLeptons = cms.bool(True)
#  )
#process.puJetIdForPFMVAMEt.rho = cms.InputTag("fixedGridRhoFastjetAll")

#if release in ['80XMINIAOD']:
#  process.pfMVAMEt.srcPFCandidates = cms.InputTag("packedPFCandidates")
#  process.pfMVAMEt.srcVertices = cms.InputTag("offlineSlimmedPrimaryVertices")
##process.pfMVAMEt.srcLeptons = cms.VInputTag("selectedElectrons","selectedMuons","selectedTaus")
#  process.pfMVAMEt.srcLeptons = cms.VInputTag("slimmedElectrons","slimmedMuons","slimmedTaus")
#  process.puJetIdForPFMVAMEt.jec = cms.string("AK4PF")
#  process.puJetIdForPFMVAMEt.vertexes = cms.InputTag("offlineSlimmedPrimaryVertices")
#  process.mvaMetPairsEM.srcPFCandidates = cms.InputTag('packedPFCandidates')
#  process.mvaMetPairsEM.srcVertices = cms.InputTag('offlineSlimmedPrimaryVertices')
#  process.mvaMetPairsEM.srcRho = cms.InputTag('fixedGridRhoFastjetAll')
#  process.mvaMetPairsET.srcPFCandidates = cms.InputTag('packedPFCandidates')
#  process.mvaMetPairsET.srcVertices = cms.InputTag('offlineSlimmedPrimaryVertices')
#  process.mvaMetPairsET.srcRho = cms.InputTag('fixedGridRhoFastjetAll')
#  process.mvaMetPairsMT.srcPFCandidates = cms.InputTag('packedPFCandidates')
#  process.mvaMetPairsMT.srcVertices = cms.InputTag('offlineSlimmedPrimaryVertices')
#  process.mvaMetPairsMT.srcRho = cms.InputTag('fixedGridRhoFastjetAll')
#  process.mvaMetPairsTT.srcPFCandidates = cms.InputTag('packedPFCandidates')
#  process.mvaMetPairsTT.srcVertices = cms.InputTag('offlineSlimmedPrimaryVertices')
#  process.mvaMetPairsTT.srcRho = cms.InputTag('fixedGridRhoFastjetAll')
#


#if release in ['76X']:
#  process.pfMVAMEt.srcLeptons = cms.VInputTag("selectedElectrons","selectedMuons","selectedTaus")
#  process.puJetIdForPFMVAMEt.jec = cms.string("AK4PF")
#  process.mvaMetPairsEM.srcLeptons = cms.VInputTag(
#     cms.InputTag("selectedElectrons","",""),
#     cms.InputTag("selectedMuons","",""),
#  )
#  process.mvaMetPairsEM.srcVertices = cms.InputTag("offlinePrimaryVertices")
#  process.mvaMetPairsEM.srcPFCandidates = cms.InputTag("particleFlow")
#  process.mvaMetPairsET.srcLeptons = cms.VInputTag(
#     cms.InputTag("selectedElectrons","",""),
#     cms.InputTag("selectedTaus","",""),
#  )
#  process.mvaMetPairsET.srcVertices = cms.InputTag("offlinePrimaryVertices")
#  process.mvaMetPairsET.srcPFCandidates = cms.InputTag("particleFlow")
#  process.mvaMetPairsMT.srcLeptons = cms.VInputTag(
#     cms.InputTag("selectedMuons","",""),
#     cms.InputTag("selectedTaus","",""),
#  )
#  process.mvaMetPairsMT.srcVertices = cms.InputTag("offlinePrimaryVertices")
#  process.mvaMetPairsMT.srcPFCandidates = cms.InputTag("particleFlow")
#  process.mvaMetPairsTT.srcLeptons = cms.VInputTag(
#     cms.InputTag("selectedTaus","",""),
#     cms.InputTag("selectedTaus","",""),
#  )
#  process.mvaMetPairsTT.srcVertices = cms.InputTag("offlinePrimaryVertices")
#  process.mvaMetPairsTT.srcPFCandidates = cms.InputTag("particleFlow")
#
#process.icMvaMetConcatenate = cms.EDProducer("ICPATMETConcatenate",
#   concatenate = cms.VInputTag(
#     cms.InputTag("mvaMetPairsTT"),
#     cms.InputTag("mvaMetPairsMT"),
#     cms.InputTag("mvaMetPairsET"),
#     cms.InputTag("mvaMetPairsEM")
#   )
#)
#process.icMvaMetIDConcatenate = cms.EDProducer("ICIDConcatenate",
#   concatenate = cms.VInputTag(
#     cms.InputTag("mvaMetPairsTT", "MVAMetId"),
#     cms.InputTag("mvaMetPairsMT", "MVAMetId"),
#     cms.InputTag("mvaMetPairsET", "MVAMetId"),
#     cms.InputTag("mvaMetPairsEM", "MVAMetId")
#   )
#)
#process.icMvaMetPairsProducer = producers.icMetProducer.clone(
#   branch  = cms.string("pfMVAMetVector"),
#   input   = cms.InputTag("icMvaMetConcatenate"),
#   includeCustomID = cms.bool(True),
#   inputCustomID = cms.InputTag("icMvaMetIDConcatenate")
#)


#process.icMvaMetSequence = cms.Sequence(
#   process.ak4PFL1FastL2L3ResidualCorrectorChain+
#   process.ak4PFL1FastL2L3CorrectorChain+
#   process.ak4PFJets+
#   process.calibratedAK4PFJetsForPFMVAMEt+
#   process.puJetIdForPFMVAMEt+
#   process.mvaMetPairsTT+
#   process.mvaMetPairsMT+
#   process.mvaMetPairsET+
#   process.mvaMetPairsEM+
#   process.icMvaMetConcatenate+
#   process.icMvaMetIDConcatenate+
#   process.icMvaMetPairsProducer+ 
#   process.pfMVAMEt
#   )
#
#if isData:
#  process.icMvaMetSequence.remove(process.ak4PFL1FastL2L3CorrectorChain)
#  process.calibratedAK4PFJetsForPFMVAMEt.correctors=cms.VInputTag("ak4PFL1FastL2L3ResidualCorrector")
#
#else :
#  process.icMvaMetSequence.remove(process.ak4PFL1FastL2L3ResidualCorrectorChain)
#
#process.icPfMVAMetProducer = cms.EDProducer('ICPFMetProducer',
#  input = cms.InputTag("pfMVAMEt"),
#  branch = cms.string("pfMVAMet"),
#  includeCustomID = cms.bool(False),
#  includeExternalMetsig = cms.bool(False),
#  includeMetUncertainties = cms.bool(False),
#  metuncertainties = cms.vstring(),
#  includeExternalMetsigMethod2 = cms.bool(False),
#  doGenMet = cms.bool(False),
#  metsig = cms.PSet(
#    metsig = cms.InputTag("METSignificance","METSignificance"),
#    metsigcov00 = cms.InputTag("METSignificance", "CovarianceMatrix00"),
#    metsigcov01 = cms.InputTag("METSignificance", "CovarianceMatrix01"),
#    metsigcov10 = cms.InputTag("METSignificance", "CovarianceMatrix10"),
#    metsigcov11 = cms.InputTag("METSignificance", "CovarianceMatrix11")
#    ),
#  metsig_method2 = cms.PSet(
#    metsig      = cms.InputTag("METSignificance","METSignificance"),
#    metsigcov = cms.InputTag("METSignificance","METCovariance")
#    ),
#  inputCustomID = cms.InputTag("MVAMetID")
#  )


#process.icMvaMetSequence += cms.Sequence(
#  process.icPfMVAMetProducer
#  )

################################################################
# Simulation only: GenParticles, GenJets, PileupInfo
################################################################
process.icGenSequence = cms.Sequence()

#if release in ['74X']:
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.prunedGenParticles = cms.EDProducer("GenParticlePruner",
    src = cms.InputTag("genParticles","","HLT"),
    select = cms.vstring(
        "drop  *", # this is the default
        "++keep abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15", # keep leptons, with history
        "keep abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16",   # keep neutrinos
        "drop   status == 2",                                              # drop the shower part of the history
        "+keep pdgId == 22 && status == 1 && (pt > 10 || isPromptFinalState())", # keep gamma above 10 GeV (or all prompt) and its first parent
        "+keep abs(pdgId) == 11 && status == 1 && (pt > 3 || isPromptFinalState())", # keep first parent of electrons above 3 GeV (or prompt)
        "keep++ abs(pdgId) == 15",                                         # but keep keep taus with decays
	"drop  status > 30 && status < 70 ", 				   #remove pythia8 garbage
	"drop  pdgId == 21 && pt < 5",                                    #remove pythia8 garbage
        "drop   status == 2 && abs(pdgId) == 21",                          # but remove again gluons in the inheritance chain
        "keep abs(pdgId) == 23 || abs(pdgId) == 24 || abs(pdgId) == 25 || abs(pdgId) == 6 || abs(pdgId) == 37 ",   # keep VIP(articles)s
        "keep abs(pdgId) == 310 && abs(eta) < 2.5 && pt > 1 ",                                                     # keep K0
# keep heavy flavour quarks for parton-based jet flavour
	"keep (4 <= abs(pdgId) <= 5) & (status = 2 || status = 11 || status = 71 || status = 72)",
# keep light-flavour quarks and gluons for parton-based jet flavour
	"keep (1 <= abs(pdgId) <= 3 || pdgId = 21) & (status = 2 || status = 11 || status = 71 || status = 72) && pt>5", 
# keep b and c hadrons for hadron-based jet flavour
	"keep (400 < abs(pdgId) < 600) || (4000 < abs(pdgId) < 6000)",
# additional c hadrons for jet fragmentation studies
	"keep abs(pdgId) = 10411 || abs(pdgId) = 10421 || abs(pdgId) = 10413 || abs(pdgId) = 10423 || abs(pdgId) = 20413 || abs(pdgId) = 20423 || abs(pdgId) = 10431 || abs(pdgId) = 10433 || abs(pdgId) = 20433", 
# additional b hadrons for jet fragmentation studies
	"keep abs(pdgId) = 10511 || abs(pdgId) = 10521 || abs(pdgId) = 10513 || abs(pdgId) = 10523 || abs(pdgId) = 20513 || abs(pdgId) = 20523 || abs(pdgId) = 10531 || abs(pdgId) = 10533 || abs(pdgId) = 20533 || abs(pdgId) = 10541 || abs(pdgId) = 10543 || abs(pdgId) = 20543", 
#keep SUSY particles
	"keep (1000001 <= abs(pdgId) <= 1000039 ) || ( 2000001 <= abs(pdgId) <= 2000015)",
# keep protons 
        "keep pdgId = 2212",
        "keep status == 3 || ( 21 <= status <= 29) || ( 11 <= status <= 19)",  #keep event summary (status=3 for pythia6, 21 <= status <= 29 for pythia8)
        "keep isHardProcess() || fromHardProcessFinalState() || fromHardProcessDecayed() || fromHardProcessBeforeFSR() || (statusFlags().fromHardProcess() && statusFlags().isLastCopy())",  #keep event summary based on status flags
    )
)
 

#process.prunedGenParticlesTaus = cms.EDProducer("ICGenParticlePruner53X",
#  src = cms.InputTag("genParticles", "", "HLT"),
#  select = cms.vstring(
#    "drop  *",
#    "keep++ abs(pdgId) == 15",  # keep full tau decay chain
#  )
#)

process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  input   = cms.InputTag("prunedGenParticles"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True),
  includeStatusFlags = cms.bool(True)
)


process.icGenParticleProducerFromLHEParticles = producers.icGenParticleFromLHEParticlesProducer.clone(
  input = cms.InputTag(opts.LHETag)
)

if release in ['80XMINIAOD']:
  process.icGenParticleProducer.input=cms.InputTag("prunedGenParticles","","PAT") #Store ALL pruned gen particles


process.load("RecoJets.Configuration.GenJetParticles_cff")
process.genParticlesForJets.ignoreParticleIDs = cms.vuint32(
  1000022, 2000012, 2000014,
  2000016, 1000039, 5000039,
  4000012, 9900012, 9900014,
  9900016, 39, 12, 14, 16
)
if release in ['80XMINIAOD']:
  # But of course this won't work because genParticlesForJets(InputGenJetsParticleSelector)
  # requires a vector<GenParticle> input. There's no alternative filter for the PackedGenParticle
  # type at the moment. Probably we could make our own generic cut-string selector, but
  # not in this package
  process.genParticlesForJets.src = cms.InputTag("packedGenParticles")

process.load("RecoJets.JetProducers.ak4GenJets_cfi")
process.ak4GenJetsNoNuBSM  =  process.ak4GenJets.clone()

if release in ['80XMINIAOD']:
  process.ak4GenJetsNoNuBSM.src=cms.InputTag("packedGenParticles") #This still contains nus in 72, should be fixed in 74

process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
  src = cms.InputTag("ak4GenJetsNoNuBSM"),
  cut = cms.string("pt > 10.0")
)

process.icGenJetProducer = producers.icGenJetProducer.clone(
  branch  = cms.string("genJets"),
  input   = cms.InputTag("selectedGenJets"),
  inputGenParticles = cms.InputTag("genParticles"),
  requestGenParticles = cms.bool(False)
)

if release in ['80XMINIAOD']:
  process.icGenJetProducer.branch = cms.string("genJetsReclustered")
  process.icGenJetProducer.inputGenParticles = cms.InputTag("prunedGenParticles","","PAT")
  process.icGenJetProducer.isSlimmed  = cms.bool(True)
  process.icGenJetProducerFromSlimmed = producers.icGenJetProducer.clone(
    branch = cms.string("genJets"),
    input = cms.InputTag("slimmedGenJets"),
    inputGenParticles=cms.InputTag("genParticles"),
    requestGenParticles = cms.bool(False),
    isSlimmed = cms.bool(True)
  ) 

process.icPileupInfoProducer = producers.icPileupInfoProducer.clone()
if release in ['80XMINIAOD']:
  process.icPileupInfoProducer.input=cms.InputTag("slimmedAddPileupInfo")


if not isData:
  process.icGenSequence += (
    process.prunedGenParticles+
    process.icGenParticleProducer
  )
  if release in ['80XMINIAOD']:
    process.icGenSequence.remove(process.prunedGenParticles)
    process.icGenSequence += (
      process.ak4GenJetsNoNuBSM+
      process.selectedGenJets+
      process.icGenJetProducer+
      process.icGenJetProducerFromSlimmed+
      process.icPileupInfoProducer
    )
  if release in [ '76X']:
    process.icGenSequence += (
      process.genParticlesForJets+
      process.ak4GenJetsNoNuBSM+
      process.selectedGenJets+
      process.icGenJetProducer+
      process.icPileupInfoProducer
    )

  if doHT:
    process.icGenSequence += (
      process.icGenParticleProducerFromLHEParticles
    )


#process.load("RecoJets.JetProducers.ak4GenJets_cfi")
#process.ak4GenJetsNoNuBSM  =  process.ak4GenJets.clone()
#process.selectedGenJetsAK4 = cms.EDFilter("GenJetRefSelector",
#  src = cms.InputTag("ak4GenJetsNoNuBSM"),
#  cut = cms.string("pt > 10.0")
#)
#process.icGenJetProducerAK4 = producers.icGenJetProducer.clone(
#  branch  = cms.string("ak4GenJets"),
#  input   = cms.InputTag("selectedGenJetsAK4"),
#  inputGenParticles = cms.InputTag("genParticles"),
#  requestGenParticles = cms.bool(False)
#)
#process.icGenSequence += (
#    process.ak4GenJetsNoNuBSM+
#    process.selectedGenJetsAK4+
#    process.icGenJetProducerAK4
#)



# ################################################################
# # Trigger
# ################################################################
from PhysicsTools.PatAlgos.tools.trigTools import *
process.icTriggerSequence = cms.Sequence()
process.icTriggerObjectSequence = cms.Sequence()
# if release in ['42X', '53X']:
#   process.patTriggerSequence = cms.Sequence()
#   switchOnTrigger(process, sequence = 'patTriggerSequence', outputModule = '')
#   process.patTriggerPath = cms.Path(process.patTriggerSequence)
#if release in ['70X', '74X']:
#  process.patTriggerPath = cms.Path()
#  switchOnTrigger(process, path = 'patTriggerPath', outputModule = '')

#  process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
#   branch = cms.string("triggerPaths"),
#   input  = cms.InputTag("patTriggerEvent")
#  )




process.patTriggerPath = cms.Path()
if release in ['80XMINIAOD']:
  switchOnTrigger(process, path = 'patTriggerPath',  outputModule = '')
if release in ['74X']:
  switchOnTrigger(process, outputModule = '')


if release in ['76X']:
  process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
    branch = cms.string("triggerPaths"),
    input = cms.InputTag("patTriggerEvent")
  )

#  if isData:
  process.icTriggerSequence += cms.Sequence(
    process.patTrigger+
    process.patTriggerEvent+
   process.icTriggerPathProducer
  )
  
trig_name="HLT"
if opts.MGsignalVBF: trig_name = "HLT2"
if opts.MGsignalGF: trig_name = "HLT"
if release in ['80XMINIAOD']:
  process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
   branch = cms.string("triggerPaths"),
   input  = cms.InputTag("TriggerResults","",trig_name),
   inputIsStandAlone = cms.bool(True),
   inputPrescales = cms.InputTag("patTrigger")
  )

  if isReHLT:
    process.icTriggerPathProducer.input = cms.InputTag("TriggerResults","",trig_name)

  if isData:
    process.icTriggerSequence += cms.Sequence(
     #process.patTrigger+
     #process.patTriggerEvent+
     process.icTriggerPathProducer
    )

process.icEle12Mu23ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle12Mu23"),
      hltPath = cms.string("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle12Mu17ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle12Mu17"),
      hltPath = cms.string("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle23Mu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle23Mu8"),
      hltPath = cms.string("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
process.icEle17Mu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle17Mu8"),
      hltPath = cms.string("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle27LooseTau20SingleL1ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle27LooseTau20SingleL1"),
      hltPath = cms.string("HLT_Ele27_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v"),
  #    hltPath = cms.string("HLT_Ele22_eta2p1_WP75_Gsf_LooseIsoPFTau20_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle32LooseTau20SingleL1ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle32LooseTau20SingleL1"),
      hltPath = cms.string("HLT_Ele32_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v"),
  #    hltPath = cms.string("HLT_Ele22_eta2p1_WP75_Gsf_LooseIsoPFTau20_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )



process.icEle24LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle24LooseTau20"),
      hltPath = cms.string("HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v"),
  #    hltPath = cms.string("HLT_Ele22_eta2p1_WP75_Gsf_LooseIsoPFTau20_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle24LooseTau20SingleL1ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle24LooseTau20SingleL1"),
      hltPath = cms.string("HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v"),
  #    hltPath = cms.string("HLT_Ele22_eta2p1_WP75_Gsf_LooseIsoPFTau20_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle22LooseTau20SingleL1ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle22LooseTau20SingleL1"),
      hltPath = cms.string("HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v"),
  #    hltPath = cms.string("HLT_Ele22_eta2p1_WP75_Gsf_LooseIsoPFTau20_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )




  
process.icEle27Eta2p1LooseGsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle27GsfLooseEta2p1"),
  #    hltPath = cms.string("HLT_Ele27_eta2p1_WP75_Gsf_v"),
      hltPath = cms.string("HLT_Ele27_eta2p1_WPLoose_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle27Eta2p1TightGsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle27GsfTightEta2p1"),
  #    hltPath = cms.string("HLT_Ele27_eta2p1_WP75_Gsf_v"),
      hltPath = cms.string("HLT_Ele27_eta2p1_WPTight_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle27LooseGsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle27GsfLoose"),
  #    hltPath = cms.string("HLT_Ele27_eta2p1_WP75_Gsf_v"),
      hltPath = cms.string("HLT_Ele27_WPLoose_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle27TightGsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle27GsfTight"),
  #    hltPath = cms.string("HLT_Ele27_eta2p1_WP75_Gsf_v"),
      hltPath = cms.string("HLT_Ele27_WPTight_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )


process.icEle32TightGsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle32GsfTight"),
      hltPath = cms.string("HLT_Ele32_eta2p1_WPTight_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

  
  
process.icEle24GsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle24Gsf"),
      hltPath = cms.string("HLT_Ele24_eta2p1_WPLoose_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle25LooseEta2p1GsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle25GsfLooseEta2p1"),
      hltPath = cms.string("HLT_Ele25_eta2p1_WPLoose_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle25TightEta2p1GsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle25GsfTightEta2p1"),
      hltPath = cms.string("HLT_Ele25_eta2p1_WPTight_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle25TightGsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle25GsfTight"),
      hltPath = cms.string("HLT_Ele25_WPTight_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icIsoMu19LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu19LooseTau20"),
      hltPath = cms.string("HLT_IsoMu19_eta2p1_LooseIsoPFTau20_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icIsoMu19LooseTau20SingleL1ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu19LooseTau20SingleL1"),
      hltPath = cms.string("HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icIsoMu21LooseTau20SingleL1ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu21LooseTau20SingleL1"),
      hltPath = cms.string("HLT_IsoMu21_eta2p1_LooseIsoPFTau20_SingleL1_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )


process.icIsoMu24ObjectProducer = producers.icTriggerObjectProducer.clone(
    input   = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoMu24"),
    hltPath = cms.string("HLT_IsoMu24_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )

process.icIsoMu27ObjectProducer = producers.icTriggerObjectProducer.clone(
    input   = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoMu27"),
    hltPath = cms.string("HLT_IsoMu27_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )


  
process.icIsoMu22ObjectProducer = producers.icTriggerObjectProducer.clone(
    input   = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoMu22"),
    hltPath = cms.string("HLT_IsoMu22_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )

process.icIsoMu22Eta2p1ObjectProducer = producers.icTriggerObjectProducer.clone(
    input   = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoMu22Eta2p1"),
    hltPath = cms.string("HLT_IsoMu22_eta2p1_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )


process.icIsoMu20ObjectProducer = producers.icTriggerObjectProducer.clone(
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoMu20"),
    hltPath = cms.string("HLT_IsoMu20_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )

process.icIsoTkMu18ObjectProducer = producers.icTriggerObjectProducer.clone(
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoTkMu18"),
    hltPath = cms.string("HLT_IsoTkMu18_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )

process.icIsoTkMu20ObjectProducer = producers.icTriggerObjectProducer.clone(
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoTkMu20"),
    hltPath = cms.string("HLT_IsoTkMu20_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )

process.icIsoTkMu22ObjectProducer = producers.icTriggerObjectProducer.clone(
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoTkMu22"),
    hltPath = cms.string("HLT_IsoTkMu22_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )

process.icIsoTkMu24ObjectProducer = producers.icTriggerObjectProducer.clone(
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoTkMu24"),
    hltPath = cms.string("HLT_IsoTkMu24_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )

process.icIsoTkMu27ObjectProducer = producers.icTriggerObjectProducer.clone(
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoTkMu27"),
    hltPath = cms.string("HLT_IsoTkMu27_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )



process.icIsoTkMu22Eta2p1ObjectProducer = producers.icTriggerObjectProducer.clone(
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoTkMu22Eta2p1"),
    hltPath = cms.string("HLT_IsoTkMu22_eta2p1_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )

process.icIsoTkMu24Eta2p1ObjectProducer = producers.icTriggerObjectProducer.clone(
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoTkMu24Eta2p1"),
    hltPath = cms.string("HLT_IsoTkMu24_eta2p1_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )

process.icIsoMu21MediumTau32ObjectProducer = producers.icTriggerObjectProducer.clone(    
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoMu21MediumTau32"),
    hltPath           = cms.string("HLT_IsoMu21_eta2p1_MediumIsoPFTau32_Trk1_eta2p1_Reg_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired  = cms.bool(False)
    )

process.icIsoMu19MediumTau32ObjectProducer = producers.icTriggerObjectProducer.clone( 
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoMu19MediumTau32"),
    hltPath           = cms.string("HLT_IsoMu19_eta2p1_MediumIsoPFTau32_Trk1_eta2p1_Reg_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired  = cms.bool(False)
    ) 

process.icDoubleMediumTau32ObjectProducer = producers.icTriggerObjectProducer.clone(
   input = cms.InputTag("patTriggerEvent"),
   branch = cms.string("triggerObjectsDoubleMediumTau32"),
   hltPath = cms.string("HLT_DoubleMediumIsoPFTau32_Trk1_eta2p1_Reg_v"),
   inputIsStandAlone = cms.bool(False),
   storeOnlyIfFired = cms.bool(False)
   )

process.icDoubleMediumTau35ObjectProducer = producers.icTriggerObjectProducer.clone(
   input = cms.InputTag("patTriggerEvent"),
   branch = cms.string("triggerObjectsDoubleMediumTau35"),
   hltPath = cms.string("HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v"),
   inputIsStandAlone = cms.bool(False),
   storeOnlyIfFired = cms.bool(False)
   )

process.icDoubleMediumTau40ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsDoubleMediumTau40"),
      hltPath = cms.string("HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icSingleTau140ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsSingleTau140"),
      hltPath = cms.string("HLT_VLooseIsoPFTau140_Trk50_eta2p1_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icSingleTau120ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsSingleTau120"),
      hltPath = cms.string("HLT_VLooseIsoPFTau120_Trk50_eta2p1_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

#paths not existing in current reHLT

process.icEle20LooseTau28ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle20LooseTau28"),
      hltPath = cms.string("HLT_Ele20_eta2p1_WPLoose_Gsf_LooseIsoPFTau28_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle22LooseTau29ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle22LooseTau29"),
      hltPath = cms.string("HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau29_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle24LooseTau30ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle24LooseTau30"),
      hltPath = cms.string("HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau30_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle30TightGsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle30GsfTight"),
      hltPath = cms.string("HLT_Ele30_WPTight_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle30TightEta2p1GsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle30GsfTightEta2p1"),
      hltPath = cms.string("HLT_Ele30_eta2p1_WPTight_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle32TightGsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle32GsfTight"),
      hltPath = cms.string("HLT_Ele32_WPTight_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icIsoMu19LooseCombinedIsoTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu19LooseCombinedIsoTau20"),
      hltPath = cms.string("HLT_IsoMu19_eta2p1_LooseCombinedIsoPFTau20_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icIsoMu19MediumCombinedIsoTau32RegObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu19MediumCombinedIsoTau32Reg"),
      hltPath = cms.string("HLT_IsoMu19_eta2p1_MediumCombinedIsoPFTau32_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icIsoMu21MediumCombinedIsoTau32RegObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu21MediumCombinedIsoTau32Reg"),
      hltPath = cms.string("HLT_IsoMu21_eta2p1_MediumCombinedIsoPFTau32_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle12Mu23ObjectProducerDZ = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle12Mu23DZ"),
      hltPath = cms.string("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle8Mu23ObjectProducerDZ = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle8Mu23DZ"),
      hltPath = cms.string("HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle23Mu12ObjectProducerDZ = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle23Mu12DZ"),
      hltPath = cms.string("HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle23Mu8ObjectProducerDZ = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle23Mu8DZ"),
      hltPath = cms.string("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )


process.icDoubleMediumCombinedIsoTau35RegObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsDoubleMediumCombinedIsoTau35Reg"),
      hltPath = cms.string("HLT_DoubleMediumCombinedIsoPFTau35_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icDoubleMediumCombinedIsoTau40RegObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsDoubleMediumCombinedIsoTau40Reg"),
      hltPath = cms.string("HLT_DoubleMediumCombinedIsoPFTau40_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icDoubleMediumCombinedIsoTau40ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsDoubleMediumCombinedIsoTau40"),
      hltPath = cms.string("HLT_DoubleMediumCombinedIsoPFTau40_Trk1_eta2p1_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
process.icTriggerObjectSequence += cms.Sequence(
      process.icEle12Mu23ObjectProducer +
      process.icEle23Mu8ObjectProducer +
      process.icEle12Mu17ObjectProducer +
      process.icEle17Mu8ObjectProducer +
      process.icEle24LooseTau20ObjectProducer +
      process.icEle24LooseTau20SingleL1ObjectProducer +
      process.icEle22LooseTau20SingleL1ObjectProducer +
      process.icEle27LooseTau20SingleL1ObjectProducer +
      process.icEle32LooseTau20SingleL1ObjectProducer +
      process.icEle24GsfObjectProducer +
      process.icEle25TightGsfObjectProducer +
      process.icEle25TightEta2p1GsfObjectProducer +
      process.icEle25LooseEta2p1GsfObjectProducer +
      process.icEle27LooseGsfObjectProducer + 
      process.icEle27TightGsfObjectProducer + 
      process.icEle27Eta2p1LooseGsfObjectProducer + 
      process.icEle27Eta2p1TightGsfObjectProducer + 
      process.icEle32TightGsfObjectProducer +
      process.icIsoMu19LooseTau20ObjectProducer +
      process.icIsoMu19LooseTau20SingleL1ObjectProducer +
      process.icIsoMu21LooseTau20SingleL1ObjectProducer +
      process.icIsoMu22ObjectProducer+
      process.icIsoMu20ObjectProducer+
      process.icIsoMu22Eta2p1ObjectProducer+
      process.icIsoMu24ObjectProducer+
      process.icIsoMu27ObjectProducer+
      process.icIsoTkMu18ObjectProducer+
      process.icIsoTkMu20ObjectProducer+
      process.icIsoTkMu22ObjectProducer+
      process.icIsoTkMu24ObjectProducer+
      process.icIsoTkMu27ObjectProducer+
      
      process.icIsoTkMu22Eta2p1ObjectProducer+
      process.icIsoTkMu24Eta2p1ObjectProducer+
      
      process.icDoubleMediumTau40ObjectProducer +
      process.icDoubleMediumTau35ObjectProducer +
      process.icDoubleMediumTau32ObjectProducer +
      process.icSingleTau140ObjectProducer+
      process.icSingleTau120ObjectProducer+
      
      process.icIsoMu19MediumTau32ObjectProducer+ 
      process.icIsoMu21MediumTau32ObjectProducer+
      
      process.icEle20LooseTau28ObjectProducer+
      process.icEle22LooseTau29ObjectProducer+
      process.icEle24LooseTau30ObjectProducer+
      process.icEle30TightGsfObjectProducer+
      process.icEle30TightEta2p1GsfObjectProducer+
      process.icEle32TightGsfObjectProducer+
      process.icIsoMu19LooseCombinedIsoTau20ObjectProducer+
      process.icIsoMu19MediumCombinedIsoTau32RegObjectProducer+
      process.icIsoMu21MediumCombinedIsoTau32RegObjectProducer+
      process.icEle12Mu23ObjectProducerDZ+
      process.icEle8Mu23ObjectProducerDZ+
      process.icEle23Mu12ObjectProducerDZ+
      process.icEle23Mu8ObjectProducerDZ+
      process.icDoubleMediumCombinedIsoTau35RegObjectProducer+
      process.icDoubleMediumCombinedIsoTau40RegObjectProducer+
      process.icDoubleMediumCombinedIsoTau40ObjectProducer
      )

  
if release in ['80XMINIAOD']:
  for name in process.icTriggerObjectSequence.moduleNames():
    mod = getattr(process, name)
    mod.inputIsStandAlone = cms.bool(True)
    mod.input = cms.InputTag("selectedPatTrigger")
  if isReHLT:
    for name in process.icTriggerObjectSequence.moduleNames():
      mod = getattr(process, name)
      mod.inputTriggerResults = cms.InputTag("TriggerResults", "",trig_name)

################################################################
# EventInfo
################################################################
#Load the MET filters here
process.load('RecoMET.METFilters.BadPFMuonFilter_cfi')
process.load('RecoMET.METFilters.BadChargedCandidateFilter_cfi')
process.load('RecoMET.METFilters.badGlobalMuonTaggersMiniAOD_cff')
process.BadPFMuonFilter.muons = cms.InputTag("slimmedMuons")
process.BadPFMuonFilter.PFCandidates = cms.InputTag("packedPFCandidates")
process.BadPFMuonFilter.taggingMode = cms.bool(True)
process.BadChargedCandidateFilter.muons = cms.InputTag("slimmedMuons")
process.BadChargedCandidateFilter.PFCandidates = cms.InputTag("packedPFCandidates")
process.BadChargedCandidateFilter.taggingMode = cms.bool(True)
process.badGlobalMuonTagger.taggingMode = cms.bool(True)
process.cloneGlobalMuonTagger.taggingMode = cms.bool(True)
if opts.LHETag: lheTag = opts.LHETag
else: lheTag = 'externalLHEProducer'

process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  includeJetRho       = cms.bool(True),
  includeLHEWeights   = cms.bool(doLHEWeights),
  includeHT           = cms.bool(False),
  lheProducer         = cms.InputTag(lheTag),
  inputJetRho         = cms.InputTag("fixedGridRhoFastjetAll"),
  includeLeptonRho    = cms.bool(False),
  inputLeptonRho      = cms.InputTag("fixedGridRhoFastjetAll"),
  includeVertexCount  = cms.bool(True),
  inputVertices       = vtxLabel,
  includeCSCFilter    = cms.bool(False),
  inputCSCFilter      = cms.InputTag("BeamHaloSummary"),
  includeFiltersFromTrig = cms.bool(True),
  filters             = cms.PSet(
   badChargedHadronFilter  = cms.InputTag("BadChargedCandidateFilter"),
   badMuonFilter          = cms.InputTag("BadPFMuonFilter"),
   Flag_badMuons      = cms.InputTag("badGlobalMuonTagger"),
   Flag_duplicateMuons      = cms.InputTag("cloneGlobalMuonTagger")
  ),
  filtersfromtrig     = cms.vstring("Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_goodVertices","Flag_eeBadScFilter","Flag_globalTightHalo2016Filter") 
)

if isData:
  process.icEventInfoProducer.filters=cms.PSet(
    badChargedHadronFilter = cms.InputTag("BadChargedCandidateFilter"),
    badMuonFilter = cms.InputTag("BadPFMuonFilter")
  )
  process.icEventInfoProducer.filtersfromtrig = cms.vstring("!Flag_badMuons","!Flag_duplicateMuons","Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_goodVertices","Flag_eeBadScFilter","Flag_globalTightHalo2016Filter") 

#if doHT:
#  process.icEventInfoProducer.includeHT = cms.bool(True)
  

process.icEventInfoSequence = cms.Sequence(
  process.BadPFMuonFilter+
  process.BadChargedCandidateFilter+ 
  process.badGlobalMuonTagger+
  process.cloneGlobalMuonTagger+
  process.icEventInfoProducer
)

if isData:
  process.icEventInfoSequence.remove(process.badGlobalMuonTagger)
  process.icEventInfoSequence.remove(process.cloneGlobalMuonTagger)
  

################################################################
# MadgraphReWeighting
################################################################
proc_dir = ''
p_card = ''
p_card_sample =''
if opts.MGsignalGF:
  proc_dir = CMSSW_BASE+"src/UserCode/ICHiggsTauTau/data/ggh_2p6/SubProcesses"
  p_card = CMSSW_BASE+"src/UserCode/ICHiggsTauTau/data/ggh_2p6/Cards/param_card_cp.dat"
  p_card_sample = CMSSW_BASE+"src/UserCode/ICHiggsTauTau/data/ggh_2p6/Cards/param_card_default.dat"
if opts.MGsignalVBF:
  proc_dir = CMSSW_BASE+"src/UserCode/ICHiggsTauTau/data/vbf_2p6/SubProcesses"
  p_card = CMSSW_BASE+"src/UserCode/ICHiggsTauTau/data/vbf_2p6/Cards/param_card_cp.dat"
  p_card_sample = CMSSW_BASE+"src/UserCode/ICHiggsTauTau/data/vbf_2p6/Cards/param_card_default.dat"

process.icMadgraphWeightsProducer = cms.EDProducer("ICMadgraphWeightsProducer",
  branch                  = cms.string("madgraphWeights"),
  input                   = cms.InputTag("externalLHEProducer"),
  theta                   = cms.string("0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0"),
  theta_sample            = cms.double(0.),
  process_dir             = cms.string(proc_dir),
  param_card              = cms.string(p_card),
  param_card_sample       = cms.string(p_card_sample)
)

process.icMadgraphWeightsSequence = cms.Sequence(
  process.icMadgraphWeightsProducer 
)
if not opts.MGsignalGF and not opts.MGsignalVBF or isData: process.icMadgraphWeightsSequence.remove(process.icMadgraphWeightsProducer)  
  

################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()



process.p = cms.Path(
  process.fullPatMetSequence+
  process.icMiniAODSequence+
  process.icSelectionSequence+
  process.pfParticleSelectionSequence+
  process.icVertexSequence+
# process.icPFSequence+
  process.icElectronSequence+
  process.icMuonSequence+
  process.icTauSequence+
  process.icTauProducer+
#  process.icL1ExtraTauProducer+
  #process.icL1ExtraMETProducer+
 # process.icTrackSequence+
  process.icPFJetSequence+
  #process.icMvaMetSequence+
  process.icPfMetSequence+
  process.icGenSequence+
  process.icTriggerSequence+
  process.icTriggerObjectSequence+
  process.icEventInfoSequence+
  #process.patDefaultSequence+
  process.icMadgraphWeightsSequence+
  process.icEventProducer
)

if not isData and not isReHLT:
  process.p.remove(process.icTriggerSequence)
  process.p.remove(process.icTriggerObjectSequence)

# process.schedule = cms.Schedule(process.patTriggerPath, process.p)
process.schedule = cms.Schedule(process.p)

