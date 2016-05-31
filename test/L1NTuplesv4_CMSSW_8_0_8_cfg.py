# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# cmsDriver for Data: l1Ntuple -s RAW2DIGI --era=Run2_2016 --geometry=Extended2016,Extended2016Reco --customise=L1Trigger/Configuration/customiseReEmul.L1TEventSetupForHF1x1TPs --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleEMU --customise=L1Trigger/Configuration/customiseUtils.L1TTurnOffUnpackStage2GtGmtAndCalo --conditions=auto:run2_data -n 100 --data --no_exec --no_output --filein=/store/data/Run2015D/ZeroBias1/RAW/v1/000/256/843/00000/FE8AD1BB-D05E-E511-B3A7-02163E01276B.root
# cmsDriver for MC  : l1Ntuple -s RAW2DIGI --era=Run2_2016 --geometry=Extended2016,Extended2016Reco --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleEMU --customise=L1Trigger/Configuration/customiseUtils.L1TTurnOffUnpackStage2GtGmtAndCalo --conditions=80X_mcRun2_asymptotic_v6 -n 5 --mc --no_exec --no_output --filein=/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_1.root
import FWCore.ParameterSet.Config as cms

# Processing command line inputs
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')
options.register ('sample',  'mc',VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,"The Sample type: data or mc")
options.register ('max',     1000,VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int,   "The maximum number of events to process")
options.register ('sumRange',   3,VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int,   "MET Range option 3 and 5")
options.parseArguments()

# Printing the option selected
print "############ options ############ "
print "sample   = ",options.sample
print "max      = ",options.max
print "sumRange = ",options.sumRange
print ""

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RAW2DIGI',eras.Run2_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')

if options.sample=="mc":
  process.load('SimGeneral.MixingModule.mixNoPU_cfi')
  process.load('Configuration.StandardSequences.MagneticField_cff')
  process.load('Configuration.StandardSequences.RawToDigi_cff')

elif options.sample=="data":
  process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
  process.load('Configuration.StandardSequences.RawToDigi_Data_cff')

process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.max)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring()
)

if options.sample=="mc":
  process.source.fileNames = cms.untracked.vstring('/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_1.root')
elif options.sample=="data":
  process.source.fileNames = cms.untracked.vstring('/store/data/Run2015D/ZeroBias1/RAW/v1/000/256/843/00000/FE8AD1BB-D05E-E511-B3A7-02163E01276B.root')

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

if options.sample=="mc":
  process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

elif options.sample=="data":
  process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.endjob_step)

# customisation of the process.

if options.sample=="mc":
  from L1Trigger.Configuration.customiseReEmul import L1TReEmulFromRAW2015

elif options.sample=="data":
  # Automatic addition of the customisation function from L1Trigger.Configuration.customiseReEmul
  from L1Trigger.Configuration.customiseReEmul import L1TEventSetupForHF1x1TPs,L1TReEmulFromRAW2015 
  #call to customisation function L1TEventSetupForHF1x1TPs imported from L1Trigger.Configuration.customiseReEmul
  process = L1TEventSetupForHF1x1TPs(process)

#call to customisation function L1TReEmulFromRAW imported from L1Trigger.Configuration.customiseReEmul
process = L1TReEmulFromRAW2015(process)

# Automatic addition of the customisation function from L1Trigger.L1TNtuples.customiseL1Ntuple
from L1Trigger.L1TNtuples.customiseL1Ntuple import L1NtupleEMU 

#call to customisation function L1NtupleEMU imported from L1Trigger.L1TNtuples.customiseL1Ntuple
process = L1NtupleEMU(process)

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
# VBFGenParticles
################################################################
if options.sample=="mc":
  process.icVBFGenParticleProducer = producers.icVBFGenParticleProducer.clone()

################################################################
# EventInfo
################################################################
process.icL1EventInfoProducer = producers.icL1EventInfoProducer.clone()

process.icL1EventInfoSequence = cms.Sequence(
  process.icL1EventInfoProducer
)


################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()

if options.sample=="mc":
  process.icL1ObjectPath = cms.Path(
    process.icL1ObjectProducer+
    process.icVBFGenParticleProducer+
    process.icL1EventInfoSequence+
    process.icEventProducer
  )
elif options.sample=="data":
  process.icL1ObjectPath = cms.Path(
    process.icL1ObjectProducer+
    process.icL1EventInfoSequence+
    process.icEventProducer
  )

process.schedule.append(process.icL1ObjectPath)

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseUtils
from L1Trigger.Configuration.customiseUtils import L1TTurnOffUnpackStage2GtGmtAndCalo 

#call to customisation function L1TTurnOffUnpackStage2GtGmtAndCalo imported from L1Trigger.Configuration.customiseUtils
process = L1TTurnOffUnpackStage2GtGmtAndCalo(process)

# End of customisation functions

if options.sumRange==3:
  process.caloStage2Params.etSumEtaMin = cms.vint32( 1, 1, 1, 1)
  process.caloStage2Params.etSumEtaMax = cms.vint32(28,28,28,28)

elif options.sumRange==5:
  process.caloStage2Params.etSumEtaMin = cms.vint32( 1, 1, 1, 1)
  process.caloStage2Params.etSumEtaMax = cms.vint32(41,41,41,41)


