from CRABClient.UserUtilities import config
config = config()

config.General.requestName= 'May8_MINIAOD'
config.General.workArea = 'crab_projects'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'miniaod_cfg_new_recipes.py'
config.JobType.pyCfgParams = ['isData=0','release=72XMINIAOD']

config.Data.inputDataset='/GluGluToHToTauTau_M-125_13TeV-powheg-pythia6/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM'
config.Data.splitting='FileBased'
config.Data.unitsPerJob =10

config.Data.outLFNDirBase='/store/user/adewit/May8_MINIAOD/'
config.Data.publication=False
config.Data.publishDataName='Crab3_test'
config.Site.storageSite='T2_UK_London_IC'
