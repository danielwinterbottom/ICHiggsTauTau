from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName     = 'sb400_lsp395'
config.General.workArea        = 'L1NTuplesV2_l1t-tsg-v5-CMSSW_8_0_2_METEtaRange5p0_v1'
config.General.transferOutputs = True
config.General.transferLogs    = True

config.JobType.pluginName = 'ANALYSIS'
config.JobType.psetName   = 'L1NTuplesv4_CMSSW_8_0_2_cfg.py'
config.JobType.pyCfgParams = ['sample=mc','sumRange=5']

datalist = ['/store/user/lpcsusyvbf/mc/r2.2015autumn/sb400_lsp395/evts_5raw_10k.root']
config.Data.userInputFiles     = datalist
config.Data.inputDBS         = 'global'
config.Data.splitting        = 'FileBased'
config.Data.unitsPerJob      = 1
config.Data.outLFNDirBase    = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication      = False
config.Data.outputDatasetTag = 'L1NTuplesV2_l1t-tsg-v5-CMSSW_8_0_2_METEtaRange5p0_v1'

#config.Site.whitelist   = ["T2_UK_London_IC"]
config.Site.storageSite = 'T2_UK_London_IC'
