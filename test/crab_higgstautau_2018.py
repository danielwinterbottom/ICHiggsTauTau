from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Feb_Data_102X'
config.section_('JobType')
config.JobType.psetName = 'higgstautau_cfg_102X_Nov18.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['release=102XMINIAOD','isData=1','doHT=0', 'globalTag=102X_dataRun2_Prompt_v11']
config.JobType.maxMemoryMB = 3000
config.section_('Data')
config.Data.unitsPerJob = 360
config.Data.splitting = 'Automatic'
# config.Data.unitsPerJob = 100000
# config.Data.unitsPerJob = 50000 # switch back after!
# config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
#config.Data.ignoreLocality= True
config.Data.outLFNDirBase='/store/user/adow/Feb_Data_102X/'
config.section_('User')
config.section_('Site')
config.Site.storageSite = 'T2_UK_London_IC'
config.JobType.allowUndistributedCMSSW = True
config.Data.allowNonValidInputDataset = True
# for deep tau need the next line
# config.JobType.scriptExe = 'submitCrabDeepTau.sh'

if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand
    from httplib import HTTPException

    # We want to put all the CRAB project directories from the tasks we submit here into one common directory.
    # That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException, hte:
            print hte.headers

    #############################################################################################
    ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
    #############################################################################################

    tasks=list()
    
    tasks.append(('SingleMuonA', '/SingleMuon/Run2018A-17Sep2018-v2/MINIAOD'))
    tasks.append(('SingleMuonB', '/SingleMuon/Run2018B-17Sep2018-v1/MINIAOD'))
    tasks.append(('SingleMuonC', '/SingleMuon/Run2018C-17Sep2018-v1/MINIAOD'))
    tasks.append(('SingleMuonD', '/SingleMuon/Run2018D-PromptReco-v2/MINIAOD'))

    tasks.append(('EGammaA', '/EGamma/Run2018A-17Sep2018-v2/MINIAOD'))
    tasks.append(('EGammaB', '/EGamma/Run2018B-17Sep2018-v1/MINIAOD'))
    tasks.append(('EGammaC', '/EGamma/Run2018C-17Sep2018-v1/MINIAOD'))
    tasks.append(('EGammaD', '/EGamma/Run2018D-PromptReco-v2/MINIAOD'))

    tasks.append(('TauA', '/Tau/Run2018A-17Sep2018-v1/MINIAOD'))
    tasks.append(('TauB', '/Tau/Run2018B-17Sep2018-v1/MINIAOD'))
    tasks.append(('TauC', '/Tau/Run2018C-17Sep2018-v1/MINIAOD'))
    tasks.append(('TauD', '/Tau/Run2018D-PromptReco-v2/MINIAOD'))

    tasks.append(('MuonEGA', '/MuonEG/Run2017A-17Sep2018-v1/MINIAOD'))
    tasks.append(('MuonEGB', '/MuonEG/Run2017B-17Sep2018-v1/MINIAOD'))
    tasks.append(('MuonEGC', '/MuonEG/Run2017C-17Sep2018-v1/MINIAOD'))
    tasks.append(('MuonEGD', '/MuonEG/Run2017D-PromptReco-v2/MINIAOD'))

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]

        print config.Data.unitsPerJob
        print config.Data.splitting
        
        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



