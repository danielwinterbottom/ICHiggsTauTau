from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName     = 'VBFHToInvisible'
config.General.workArea        = 'L1NTuplesV2_l1t-integration-v34_0-CMSSW_8_0_2_METEtaRange3p0_v1'
config.General.transferOutputs = True
config.General.transferLogs    = True

config.JobType.pluginName = 'ANALYSIS'
config.JobType.psetName   = 'L1NTuplesv4_CMSSW_8_0_2_cfg.py'
config.JobType.pyCfgParams = ['sample=mc','sumRange=3']

config.Data.inputDataset     = '/VBF_HToInvisible_M125_13TeV_powheg_pythia8/RunIIFall15DR76-25nsFlat10to25TSG_76X_mcRun2_asymptotic_v12-v1/GEN-SIM-RAW'
config.Data.inputDBS         = 'global'
config.Data.splitting        = 'EventAwareLumiBased'
config.Data.unitsPerJob      = 20000
config.Data.outLFNDirBase    = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication      = False
config.Data.outputDatasetTag = 'L1NTuplesV2_l1t-integration-v34_0-CMSSW_8_0_2_METEtaRange3p0_v1'

#config.Site.whitelist   = ["T2_UK_London_IC"]
config.Site.storageSite = 'T2_UK_London_IC'
