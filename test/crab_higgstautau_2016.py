from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Aug14_Data_102X_2016'
config.section_('JobType')
config.JobType.psetName = 'higgstautau_cfg_102X_Aug19_2016.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['release=102XMINIAOD','isData=1','globalTag=80X_dataRun2_2016SeptRepro_v7']
config.section_('Data')
config.Data.unitsPerJob = 100000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
#config.Data.ignoreLocality= True
config.Data.outLFNDirBase='/store/user/dwinterb/Aug14_Data_102X_2016/'
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
    
    tasks.append(('TauB', '/Tau/Run2016B-03Feb2017_ver2-v2/MINIAOD'  ))
    tasks.append(('TauC', '/Tau/Run2016C-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('TauD', '/Tau/Run2016D-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('TauE', '/Tau/Run2016E-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('TauF', '/Tau/Run2016F-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('TauG', '/Tau/Run2016G-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('TauHv2', '/Tau/Run2016H-03Feb2017_ver2-v1/MINIAOD'))
    tasks.append(('TauHv3', '/Tau/Run2016H-03Feb2017_ver3-v1/MINIAOD'))
    
    tasks.append(('SingleElectronB', '/SingleElectron/Run2016B-03Feb2017_ver2-v2/MINIAOD'  ))
    tasks.append(('SingleElectronC', '/SingleElectron/Run2016C-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('SingleElectronD', '/SingleElectron/Run2016D-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('SingleElectronE', '/SingleElectron/Run2016E-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('SingleElectronF', '/SingleElectron/Run2016F-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('SingleElectronG', '/SingleElectron/Run2016G-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('SingleElectronHv2', '/SingleElectron/Run2016H-03Feb2017_ver2-v1/MINIAOD'))
    tasks.append(('SingleElectronHv3', '/SingleElectron/Run2016H-03Feb2017_ver3-v1/MINIAOD'))
    
    tasks.append(('SingleMuonB', '/SingleMuon/Run2016B-03Feb2017_ver2-v2/MINIAOD'  ))
    tasks.append(('SingleMuonC', '/SingleMuon/Run2016C-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('SingleMuonD', '/SingleMuon/Run2016D-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('SingleMuonE', '/SingleMuon/Run2016E-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('SingleMuonF', '/SingleMuon/Run2016F-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('SingleMuonG', '/SingleMuon/Run2016G-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('SingleMuonHv2', '/SingleMuon/Run2016H-03Feb2017_ver2-v1/MINIAOD'))
    tasks.append(('SingleMuonHv3', '/SingleMuon/Run2016H-03Feb2017_ver3-v1/MINIAOD'))
    
    tasks.append(('MuonEGB', '/MuonEG/Run2016B-03Feb2017_ver2-v2/MINIAOD'  ))
    tasks.append(('MuonEGC', '/MuonEG/Run2016C-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('MuonEGD', '/MuonEG/Run2016D-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('MuonEGE', '/MuonEG/Run2016E-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('MuonEGF', '/MuonEG/Run2016F-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('MuonEGG', '/MuonEG/Run2016G-03Feb2017-v1/MINIAOD'       ))
    tasks.append(('MuonEGHv2', '/MuonEG/Run2016H-03Feb2017_ver2-v1/MINIAOD'))
    tasks.append(('MuonEGHv3', '/MuonEG/Run2016H-03Feb2017_ver3-v1/MINIAOD'))

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        
        if "Run2016H" in task[1]:
            config.JobType.pyCfgParams = ['release=102XMINIAOD','isData=1','globalTag=globalTag=80X_dataRun2_Prompt_v16']
        else:
            config.JobType.pyCfgParams = ['release=102XMINIAOD','isData=1','globalTag=80X_dataRun2_2016SeptRepro_v7']
        #submit(config)
        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



