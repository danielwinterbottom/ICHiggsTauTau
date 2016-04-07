from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName     = 'VBFHToTauTau'
config.General.workArea        = 'L1NTuplesV2_jetmet-update-forjoe-CMSSW_8_0_2_METEtaRange3p0_v1'
config.General.transferOutputs = True
config.General.transferLogs    = True

config.JobType.pluginName = 'ANALYSIS'
config.JobType.psetName   = 'L1NTuplesv4_CMSSW_8_0_2_cfg.py'
config.JobType.pyCfgParams = ['sample=mc','sumRange=3']

config.Data.inputDataset     = '/VBFHToTauTau_M125_13TeV_powheg_pythia8/pela-Reprocessing_VBFHiggsTauTau_RAW_v3-02e742d2ba16f475bed5477b1348f921/USER'
config.Data.inputDBS         = 'phys03'
config.Data.splitting        = 'EventAwareLumiBased'
config.Data.unitsPerJob      = 20000
config.Data.outLFNDirBase    = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication      = False
config.Data.outputDatasetTag = 'L1NTuplesV2_jetmet-update-forjoe-CMSSW_8_0_2_METEtaRange3p0_v1'

#config.Site.whitelist   = ["T2_UK_London_IC"]
config.Site.storageSite = 'T2_UK_London_IC'
