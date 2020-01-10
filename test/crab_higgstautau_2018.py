from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsernameFromSiteDB

config = config()

config.General.transferOutputs = True
config.General.workArea='Jan06_Data_102X_2018'

config.JobType.psetName = 'higgstautau_cfg_102X_Aug19_2018.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['isData=1','globalTag=102X_dataRun2_v12']
#config.JobType.maxMemoryMB = 2000
# config.JobType.numCores = 4
config.JobType.allowUndistributedCMSSW = True

config.Data.unitsPerJob = 100000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.outLFNDirBase='/store/user/{}/{}/'.format(getUsernameFromSiteDB(), config.General.workArea)
config.Data.allowNonValidInputDataset = True

config.Site.whitelist   = ['T2_*','T1_*','T3_*']
config.Data.ignoreLocality= True
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
        except HTTPException as hte:
            print hte.headers
        except ClientException as cle:
            print cle

    #############################################################################################
    ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
    #############################################################################################

    tasks=list()

    tasks.append(('SingleMuonA', '/SingleMuon/Run2018A-17Sep2018-v2/MINIAOD'))
    tasks.append(('SingleMuonB', '/SingleMuon/Run2018B-17Sep2018-v1/MINIAOD'))
    tasks.append(('SingleMuonC', '/SingleMuon/Run2018C-17Sep2018-v1/MINIAOD'))
    tasks.append(('SingleMuonD', '/SingleMuon/Run2018D-PromptReco-v2/MINIAOD'))
    tasks.append(('SingleMuonD_ReReco', '/SingleMuon/Run2018D-22Jan2019-v2/MINIAOD'))

    tasks.append(('EGammaA',     '/EGamma/Run2018A-17Sep2018-v2/MINIAOD'))
    tasks.append(('EGammaB',     '/EGamma/Run2018B-17Sep2018-v1/MINIAOD'))
    tasks.append(('EGammaC',     '/EGamma/Run2018C-17Sep2018-v1/MINIAOD'))
    tasks.append(('EGammaD',     '/EGamma/Run2018D-PromptReco-v2/MINIAOD'))
    tasks.append(('EGammaD_ReReco',     '/EGamma/Run2018D-22Jan2019-v2/MINIAOD'))

    tasks.append(('TauA',        '/Tau/Run2018A-17Sep2018-v1/MINIAOD'))
    tasks.append(('TauB',        '/Tau/Run2018B-17Sep2018-v1/MINIAOD'))
    tasks.append(('TauC',        '/Tau/Run2018C-17Sep2018-v1/MINIAOD'))
    tasks.append(('TauD',        '/Tau/Run2018D-PromptReco-v2/MINIAOD'))

    tasks.append(('MuonEGA',     '/MuonEG/Run2018A-17Sep2018-v1/MINIAOD'))
    tasks.append(('MuonEGB',     '/MuonEG/Run2018B-17Sep2018-v1/MINIAOD'))
    tasks.append(('MuonEGC',     '/MuonEG/Run2018C-17Sep2018-v1/MINIAOD'))
    tasks.append(('MuonEGD',     '/MuonEG/Run2018D-PromptReco-v2/MINIAOD'))

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]

        if "17Sep2018" in task[1] or "22Jan2019" in task[1]:
            config.JobType.pyCfgParams = ['isData=1','globalTag=102X_dataRun2_v12']
        else:
            config.JobType.pyCfgParams = ['isData=1','globalTag=102X_dataRun2_Prompt_v15']


        #config.Data.lumiMask = "May23_Data_102X/crab_{}/results/notFinishedLumis.json".format(task[0])

        print(config)
        submit(config)

