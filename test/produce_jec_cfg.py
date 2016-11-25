import FWCore.ParameterSet.Config as cms
process = cms.Process("jectxt")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.source = cms.Source("EmptySource")

#Get from global tag
# define your favorite global tag
#process.GlobalTag.globaltag = ''
process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v4'

#Get from local sqlite file
process.load("CondCore.DBCommon.CondDBCommon_cfi")
from CondCore.DBCommon.CondDBSetup_cfi import *
process.jec = cms.ESSource("PoolDBESSource",
      DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0)
        ),
      timetype = cms.string('runnumber'),
      toGet = cms.VPSet(
      cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Summer15_25nsV6_DATA_AK4PFCHS'),
            # tag    = cms.string('JetCorrectorParametersCollection_Summer12_V3_MC_AK5PF'),
            label  = cms.untracked.string('AK4PF')
            ),
      ..................................................
      ## here you add as many jet types as you need
      ## note that the tag name is specific for the particular sqlite file 
      ), 
      connect = cms.string('sqlite:Summer15_25nsV6_DATA.db')
     # uncomment above tag lines and this comment to use MC JEC
     # connect = cms.string('sqlite:Summer12_V7_MC.db')
)
## add an es_prefer statement to resolve a possible conflict from simultaneous connection to a global tag
process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')

#get the correction in txt file
process.readAK4PF    = cms.EDAnalyzer('JetCorrectorDBReader',  
      # below is the communication to the database 
      payloadName    = cms.untracked.string('AK4PF'),
      # this is used ONLY for the name of the printed txt files. You can use any name that you like, 
      # but it is recommended to use the GT name that you retrieved the files from.
      globalTag      = cms.untracked.string('74X_dataRun2_Prompt_v4'),  
      #globalTag      = cms.untracked.string('START53_V15'),  
      printScreen    = cms.untracked.bool(False),
      createTextFile = cms.untracked.bool(True)
)
# process.readAK4Calo = process.readAK4PF.clone(payloadName = 'AK4Calo')
# process.readAK4JPT = process.readAK4PF.clone(payloadName = 'AK4JPT')
process.p = cms.Path(process.readAK4PF)
