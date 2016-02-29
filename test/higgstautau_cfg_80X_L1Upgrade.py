# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: l1Ntuple -s RAW2DIGI --era=Run2_2016 --geometry=Extended2016,Extended2016Reco --customise=L1Trigger/Configuration/customiseReEmul.L1TEventSetupForHF1x1TPs --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleEMU --customise=L1Trigger/Configuration/customiseUtils.L1TTurnOffUnpackStage2GtGmtAndCalo --conditions=auto:run2_data -n 100 --data --no_exec --no_output --filein=/store/data/Run2015D/ZeroBias1/RAW/v1/000/256/843/00000/FE8AD1BB-D05E-E511-B3A7-02163E01276B.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
import FWCore.ParameterSet.VarParsing as VarParsing

options = VarParsing.VarParsing ('analysis')

options.register ('era',    'stage2',  VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,  "The data taking Era: stage1 or stage2")
options.register ('sample', 'data',    VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,  "The Sample type: data or mc")
options.register ('output', 'DEFAULT', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,  "The output file name")
options.register ('max',    '',        VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int,     "The maximum number of events to process")

options.max      = 1000
options.sample   = "mc"

options.parseArguments()

print "sample = ",options.sample

if (options.era == 'stage1'):
    print "INFO: runnings L1T Stage-1 (2015) Re-Emulation"
    process = cms.Process("AnalysisDecays", eras.Run2_25ns)
elif (options.era == 'stage2'):
    print "INFO: runnings L1T Stage-2 (2016) Re-Emulation"    
    process = cms.Process("AnalysisDecays", eras.Run2_2016)
else:
    print "ERROR: unknown era:  ", options.era, "\n"
    exit(0)

if (options.output == 'DEFAULT'):
    if (eras.stage1L1Trigger.isChosen()):
        options.output ='L1Objects_stage1.root'
    if (eras.stage2L1Trigger.isChosen()):
        options.output ='L1Objects_stage2.root'
print "INFO: output: ", options.output

print "INFO: max   : ", options.max

process = cms.Process('RAW2DIGI',eras.Run2_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.max)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 50

# Input source
process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('/store/data/Run2015D/ZeroBias1/RAW/v1/000/256/843/00000/FE8AD1BB-D05E-E511-B3A7-02163E01276B.root'),
    fileNames = cms.untracked.vstring(
     #'file:/afs/cern.ch/work/p/pela/private/sample/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer15GS-MCRUN2_71_V1-v1/GEN-SIM/8E765C66-7658-E511-A378-00215E2EAEDE.root'
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_1.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_10.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_100.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_101.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_103.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_104.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_105.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_106.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_107.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_108.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_109.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_11.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_110.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_111.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_112.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_113.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_114.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_115.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_116.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_117.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_118.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_119.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_12.root'
    ),
    secondaryFileNames = cms.untracked.vstring()
)


################################################################
### Input files
################################################################

# Additional output definition
# TTree output file
process.load("CommonTools.UtilAlgos.TFileService_cfi")
process.TFileService.fileName = cms.string(options.output)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('l1Ntuple nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

if (eras.stage1L1Trigger.isChosen()):
  
  if options.sample=="mc":
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

  elif options.sample=="data":
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

# Note:  For stage-2, all conditions are overriden by local config file.  Use data tag: 

if (eras.stage2L1Trigger.isChosen()):

  if options.sample=="mc":
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '') 

  elif options.sample=="data":
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
    process.GlobalTag.toGet = cms.VPSet(
        cms.PSet(record = cms.string("DTCCBConfigRcd"),
                 tag = cms.string("DTCCBConfig_NOSingleL_V05_mc"),
                 connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
                 ),
#        cms.PSet(record = cms.string("DTKeyedConfigListRcd"),
#                 tag = cms.string("DTKeyedConfigContainer_NOSingleL_V05_mc"),
#                 connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
#                 ),
        cms.PSet(record = cms.string("DTT0Rcd"),
                 tag = cms.string("DTt0_STARTUP_V02_mc"),
                 connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
                 ),
        cms.PSet(record = cms.string("DTTPGParametersRcd"),
                 tag = cms.string("DTTPGParameters_V01_mc"),
                 connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
                 ),
        )

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.endjob_step)

# customisation of the process.

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseReEmul
from L1Trigger.Configuration.customiseReEmul import L1TEventSetupForHF1x1TPs,L1TReEmulFromRAW 

#call to customisation function L1TEventSetupForHF1x1TPs imported from L1Trigger.Configuration.customiseReEmul
process = L1TEventSetupForHF1x1TPs(process)

#call to customisation function L1TReEmulFromRAW imported from L1Trigger.Configuration.customiseReEmul
process = L1TReEmulFromRAW(process)

# Automatic addition of the customisation function from L1Trigger.L1TNtuples.customiseL1Ntuple
from L1Trigger.L1TNtuples.customiseL1Ntuple import L1NtupleEMU 

#call to customisation function L1NtupleEMU imported from L1Trigger.L1TNtuples.customiseL1Ntuple
#process = L1NtupleEMU(process)

################################################################
# L1 Objects
################################################################

import UserCode.ICHiggsTauTau.default_producers_cfi as producers

process.icL1ObjectProducer = producers.icL1ObjectProducer.clone(
  input                     = cms.InputTag("simCaloStage2Digis"),
  input_L1TEra       = cms.untracked.string  ("stage2"),
  inputTag_L1TEGamma = cms.untracked.InputTag("simCaloStage2Digis"),
  inputTag_L1TMuon   = cms.untracked.InputTag("simGmtStage2Digis"),
  inputTag_L1TTau    = cms.untracked.InputTag("simCaloStage2Digis"),
  inputTag_L1TJet    = cms.untracked.InputTag("simCaloStage2Digis"),
  inputTag_L1TSum    = cms.untracked.InputTag("simCaloStage2Digis","")
)

################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()

process.icL1ObjectPath = cms.Path(
  process.icL1ObjectProducer+
  process.icEventProducer
)

process.schedule.append(process.icL1ObjectPath)


# Automatic addition of the customisation function from L1Trigger.Configuration.customiseUtils
from L1Trigger.Configuration.customiseUtils import L1TTurnOffUnpackStage2GtGmtAndCalo 

#call to customisation function L1TTurnOffUnpackStage2GtGmtAndCalo imported from L1Trigger.Configuration.customiseUtils
process = L1TTurnOffUnpackStage2GtGmtAndCalo(process)

# End of customisation functions

