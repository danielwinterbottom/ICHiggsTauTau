from CRABClient.UserUtilities import config

config = config()

config.General.transferOutputs = True
config.General.workArea='Jun06_Data_94X'

config.JobType.psetName = 'higgstautau_cfg_94X_Apr18_pfCands_in102X.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['release=102XMINIAOD','isData=1','doHT=0', 'globalTag=102X_dataRun2_v8']
# config.JobType.maxMemoryMB = 3000
config.JobType.allowUndistributedCMSSW = True

config.Data.unitsPerJob = 100000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.outLFNDirBase='/store/user/adow/{}/'.format(config.General.workArea)
config.Data.allowNonValidInputDataset = True

config.Site.storageSite = 'T2_UK_London_IC'

if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand
    from httplib import HTTPException
    from CRABClient.ClientExceptions import ClientException

    # We want to put all the CRAB project directories from the tasks we submit here into one common directory.
    # That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException, hte:
            print hte.headers
        except ClientException as cle:
            print cle

    #############################################################################################
    ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
    #############################################################################################

    tasks=list()

    tasks.append(('SingleMuonB', '/SingleMuon/Run2017B-31Mar2018-v1/MINIAOD'))
    tasks.append(('SingleMuonC', '/SingleMuon/Run2017C-31Mar2018-v1/MINIAOD'))
    tasks.append(('SingleMuonD', '/SingleMuon/Run2017D-31Mar2018-v1/MINIAOD'))
    tasks.append(('SingleMuonE', '/SingleMuon/Run2017E-31Mar2018-v1/MINIAOD'))
    tasks.append(('SingleMuonF', '/SingleMuon/Run2017F-31Mar2018-v1/MINIAOD'))

    # tasks.append(('SingleElectronB', '/SingleElectron/Run2017B-31Mar2018-v1/MINIAOD'))
    # tasks.append(('SingleElectronC', '/SingleElectron/Run2017C-31Mar2018-v1/MINIAOD'))
    # tasks.append(('SingleElectronD', '/SingleElectron/Run2017D-31Mar2018-v1/MINIAOD'))
    # tasks.append(('SingleElectronE', '/SingleElectron/Run2017E-31Mar2018-v1/MINIAOD'))
    # tasks.append(('SingleElectronF', '/SingleElectron/Run2017F-31Mar2018-v1/MINIAOD'))

    # tasks.append(('TauB', '/Tau/Run2017B-31Mar2018-v1/MINIAOD'))
    # tasks.append(('TauC', '/Tau/Run2017C-31Mar2018-v1/MINIAOD'))
    # tasks.append(('TauD', '/Tau/Run2017D-31Mar2018-v1/MINIAOD'))
    # tasks.append(('TauE', '/Tau/Run2017E-31Mar2018-v1/MINIAOD'))
    # tasks.append(('TauF', '/Tau/Run2017F-31Mar2018-v1/MINIAOD'))

    # tasks.append(('MuonEGB', '/MuonEG/Run2017B-31Mar2018-v1/MINIAOD'))
    # tasks.append(('MuonEGC', '/MuonEG/Run2017C-31Mar2018-v1/MINIAOD'))
    # tasks.append(('MuonEGD', '/MuonEG/Run2017D-31Mar2018-v1/MINIAOD'))
    # tasks.append(('MuonEGE', '/MuonEG/Run2017E-31Mar2018-v1/MINIAOD'))
    # tasks.append(('MuonEGF', '/MuonEG/Run2017F-31Mar2018-v1/MINIAOD'))

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]

        # only to run recovery tasks with the missing lumis
        # config.Data.lumiMask = "Jan31_Data_94X/crab_{}/results/notFinishedLumis.json".format(task[0])
        # print config.Data.lumiMask

        print(config)
        submit(config)

