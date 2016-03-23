from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName     = 'VBFHiggsTauTau'
config.General.workArea        = 'VBFtautauL1Variables'
config.General.transferOutputs = True
config.General.transferLogs    = True

config.JobType.pluginName = 'ANALYSIS'
config.JobType.psetName   = 'higgstautau_cfg_80X_L1Upgrade.py'

config.Data.inputDataset     = '/VBFHToTauTau_M125_13TeV_powheg_pythia8/pela-Reprocessing_VBFHiggsTauTau_RAW_v3-02e742d2ba16f475bed5477b1348f921/USER'
config.Data.inputDBS         = 'phys03'
config.Data.splitting        = 'EventAwareLumiBased'
config.Data.unitsPerJob      = 25000
config.Data.outLFNDirBase    = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication      = False
config.Data.outputDatasetTag = 'L1HiggsTauTauVBF_nTuplesV1'

config.Site.whitelist   = ["T2_UK_London_IC"]
config.Site.storageSite = 'T2_UK_London_IC'
