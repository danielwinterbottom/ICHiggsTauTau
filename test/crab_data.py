from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName     = 'ZeroBias'
config.General.workArea        = 'ZeroBias_L1Variables'
config.General.transferOutputs = True
config.General.transferLogs    = True

config.JobType.pluginName = 'ANALYSIS'
config.JobType.psetName   = 'higgstautau_cfg_80X_L1Upgrade.py'
config.JobType.pyCfgParams = ['sample=data']

config.Data.inputDataset     = '/ZeroBias/Run2015D-v1/RAW'
config.Data.inputDBS         = 'global'
config.Data.lumiMask         = 'Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt'
config.Data.runRange         = '258440,258448,259721'
config.Data.splitting        = 'EventAwareLumiBased'
config.Data.unitsPerJob      = 20000
config.Data.outLFNDirBase    = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication      = False
config.Data.outputDatasetTag = 'L1ZeroBias_nTuplesV1'

#config.Site.whitelist   = ["T2_UK_London_IC"]
config.Site.storageSite = 'T2_UK_London_IC'
