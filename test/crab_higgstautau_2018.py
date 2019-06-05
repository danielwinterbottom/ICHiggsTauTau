from CRABClient.UserUtilities import config

config = config()

config.General.transferOutputs = True
config.General.workArea='May23_Data_102X'

config.JobType.psetName = 'higgstautau_cfg_102X_Nov18.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['isData=1','doHT=0', 'globalTag=102X_dataRun2_Prompt_v13']#previous v11
config.JobType.maxMemoryMB = 2500
# config.JobType.numCores = 4
# config.JobType.scriptExe = 'submitCrabDeepTauV2.sh'
config.JobType.allowUndistributedCMSSW = True

# config.Data.unitsPerJob = 240
# config.Data.splitting = 'Automatic'
config.Data.unitsPerJob = 100000
# config.Data.unitsPerJob = 50000 # switch back after!
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
#config.Data.ignoreLocality= True
config.Data.outLFNDirBase='/store/user/adow/May23_Data_102X/'
config.Data.allowNonValidInputDataset = True

config.Site.storageSite = 'T2_UK_London_IC'

# config.Debug.extraJDL = [
#     '+DESIRED_Sites="T3_UK_Opportunistic"',
#     '+JOB_CMSSite="T3_UK_Opportunistic"',
#     '+AccountingGroup="highprio.adowot"'
# ]

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

    tasks.append(('EGammaA',     '/EGamma/Run2018A-17Sep2018-v2/MINIAOD'))
    tasks.append(('EGammaB',     '/EGamma/Run2018B-17Sep2018-v1/MINIAOD'))
    tasks.append(('EGammaC',     '/EGamma/Run2018C-17Sep2018-v1/MINIAOD'))
    tasks.append(('EGammaD',     '/EGamma/Run2018D-PromptReco-v2/MINIAOD'))

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

        if "17Sep2018" in task[1]:
            config.JobType.pyCfgParams = ['isData=1','doHT=0', 'globalTag=102X_dataRun2_Sep2018ABC_v2']
        else:
            config.JobType.pyCfgParams = ['isData=1','doHT=0', 'globalTag=102X_dataRun2_Prompt_v13']

        print config.Data.unitsPerJob
        print config.Data.splitting

        print(config)
        submit(config)

