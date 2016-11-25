from CRABClient.UserUtilities import config
config = config()

config.General.requestName= 'SUSYGluGluToHToTauTau_M-160'
config.General.workArea = 'Sep09_MC_74X'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'higgstautau_cfg_74X_Aug15.py'
config.JobType.pyCfgParams = ['isData=0','release=74XMINIAOD','isNLO=0']


config.Data.inputDataset='/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM'
config.Data.splitting='EventAwareLumiBased'
config.Data.unitsPerJob =30000

config.Data.outLFNDirBase='/store/user/adewit/Sep09_MC_74X/'
config.Data.publication=False
config.Data.publishDataName='Crab3_test'
config.Site.storageSite='T2_UK_London_IC'
