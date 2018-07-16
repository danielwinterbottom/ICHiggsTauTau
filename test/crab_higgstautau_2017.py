from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Jun15_Data_94X'
config.section_('JobType')
config.JobType.psetName = 'higgstautau_cfg_94X_Apr18.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['release=94XMINIAOD','isData=1','doHT=0', 'globalTag=94X_dataRun2_v10']
config.section_('Data')
#config.Data.unitsPerJob = 100000
config.Data.unitsPerJob = 50000 # switch back after!
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
#config.Data.ignoreLocality= True
config.Data.outLFNDirBase='/store/user/dwinterb/Jun15_Data_94X/'
config.section_('User')
config.section_('Site')
config.Site.storageSite = 'T2_UK_London_IC'
config.JobType.allowUndistributedCMSSW = True
config.Data.allowNonValidInputDataset = True

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
    
    tasks.append(('SingleMuonB', '/SingleMuon/Run2017B-31Mar2018-v1/MINIAOD'))
    tasks.append(('SingleMuonC', '/SingleMuon/Run2017C-31Mar2018-v1/MINIAOD'))
    tasks.append(('SingleMuonD', '/SingleMuon/Run2017D-31Mar2018-v1/MINIAOD'))
    tasks.append(('SingleMuonE', '/SingleMuon/Run2017E-31Mar2018-v1/MINIAOD'))
    tasks.append(('SingleMuonF', '/SingleMuon/Run2017F-31Mar2018-v1/MINIAOD'))
    tasks.append(('SingleElectronB', '/SingleElectron/Run2017B-31Mar2018-v1/MINIAOD'))
    tasks.append(('SingleElectronC', '/SingleElectron/Run2017C-31Mar2018-v1/MINIAOD'))
    tasks.append(('SingleElectronD', '/SingleElectron/Run2017D-31Mar2018-v1/MINIAOD'))
    tasks.append(('SingleElectronE', '/SingleElectron/Run2017E-31Mar2018-v1/MINIAOD'))
    tasks.append(('SingleElectronF', '/SingleElectron/Run2017F-31Mar2018-v1/MINIAOD'))
    tasks.append(('TauB', '/Tau/Run2017B-31Mar2018-v1/MINIAOD'))
    tasks.append(('TauC', '/Tau/Run2017C-31Mar2018-v1/MINIAOD'))
    tasks.append(('TauD', '/Tau/Run2017D-31Mar2018-v1/MINIAOD'))
    tasks.append(('TauE', '/Tau/Run2017E-31Mar2018-v1/MINIAOD'))
    tasks.append(('TauF', '/Tau/Run2017F-31Mar2018-v1/MINIAOD'))
    tasks.append(('MuonEGB', '/MuonEG/Run2017B-31Mar2018-v1/MINIAOD'))
    tasks.append(('MuonEGC', '/MuonEG/Run2017C-31Mar2018-v1/MINIAOD'))
    tasks.append(('MuonEGD', '/MuonEG/Run2017D-31Mar2018-v1/MINIAOD'))
    tasks.append(('MuonEGE', '/MuonEG/Run2017E-31Mar2018-v1/MINIAOD'))
    tasks.append(('MuonEGF', '/MuonEG/Run2017F-31Mar2018-v1/MINIAOD'))   

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        
        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



