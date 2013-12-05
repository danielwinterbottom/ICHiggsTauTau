# Auto generated configuration file
# using: 
# Revision: 1.381.2.27 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: --data -s NONE --no_exec --conditions=FrontierConditions_GlobalTag,FT_53_V21_AN4::All --fileout=skimmed.root --eventcontent=AODSIM --python_filename=skim.py --customise=UserCode/ICHiggsTauTau/WmunuStandaloneSelectionAll.py
import FWCore.ParameterSet.Config as cms

process = cms.Process('NONE')

# import of standard configurations
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(
    'file:/afs/cern.ch/work/a/amagnan/CMSSW_5_3_11_patch6/src/UserCode/ICHiggsTauTau/python/testFile_run2012A.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.381.2.27 $'),
    annotation = cms.untracked.string('--data nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.AODSIMoutput = cms.OutputModule("PoolOutputModule",
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    outputCommands = process.AODSIMEventContent.outputCommands,
    fileName = cms.untracked.string('skimmed.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'FT_53_V21_AN4::All', '')

# Path and EndPath definitions
process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.AODSIMoutput_step)

# customisation of the process.

# Automatic addition of the customisation function from UserCode.ICHiggsTauTau.WmunuStandaloneSelectionAll
from UserCode.ICHiggsTauTau.WmunuStandaloneSelectionAll import customise 

#call to customisation function customise imported from UserCode.ICHiggsTauTau.WmunuStandaloneSelectionAll
process = customise(process)

# End of customisation functions
