from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName     = 'ZeroBias'
config.General.workArea        = 'L1NTuplesV2_l1t-tsg-v5-CMSSW_8_0_2_METEtaRange5p0_v1'
config.General.transferOutputs = True
config.General.transferLogs    = True

config.JobType.pluginName = 'ANALYSIS'
config.JobType.psetName   = 'L1NTuplesv4_CMSSW_8_0_2_cfg.py'
config.JobType.pyCfgParams = ['sample=data','sumRange=5']

config.Data.inputDataset     = '/ZeroBias/Run2015D-v1/RAW'
config.Data.inputDBS         = 'global'
config.Data.lumiMask         = 'Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt'
config.Data.runRange         = '258440,258448,259626,259721'
config.Data.splitting        = 'EventAwareLumiBased'
config.Data.unitsPerJob      = 20000
config.Data.outLFNDirBase    = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication      = False
config.Data.outputDatasetTag = 'L1NTuplesV2_l1t-tsg-v5-CMSSW_8_0_2_METEtaRange5p0_v1'

#config.Site.whitelist   = ["T2_UK_London_IC"]
config.Site.storageSite = 'T2_UK_London_IC'
