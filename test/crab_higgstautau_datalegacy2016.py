from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Sep05_Data_102X_2016'
config.section_('JobType')
config.JobType.psetName = 'higgstautau_cfg_102X_Aug19_Leg2016.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.maxMemoryMB = 2500
#config.JobType.inputFiles = ['Spring16_25nsV3_DATA.db']
config.JobType.pyCfgParams = ['release=102XMINIAOD','isData=1','doHT=0', 'globalTag=94X_dataRun2_v10']
config.section_('Data')
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 100000
#config.Data.unitsPerJob = 1
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.ignoreLocality= False
config.Data.outLFNDirBase='/store/user/dwinterb/{}/'.format(config.General.workArea)
config.section_('User')
config.section_('Site')
#config.Site.whitelist = ['T2_UK_London_IC', 'T2_CH_CERN', 'T2_FR_GRIF_LLR', 'T2_UK_SGrid_Bristol', 'T3_US_FNALLPC', 'T2_DE_DESY', 'T2_IT_Bari', 'T2_BE_IIHE', 'T2_US_UCSD', 'T2_US_MIT', 'T2_US_Wisconsin', 'T2_US_Florida', 'T2_IT_Rome','T2_FR_IPHC','T2_UK_London_Brunel']
#config.Site.blacklist = ['T2_US_*']
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

    tasks.append(('TauB', '/Tau/Run2016B-17Jul2018_ver2-v1/MINIAOD'  ))
    tasks.append(('TauC', '/Tau/Run2016C-17Jul2018-v1/MINIAOD'  ))
    tasks.append(('TauD', '/Tau/Run2016D-17Jul2018-v1/MINIAOD'  ))
    tasks.append(('TauE', '/Tau/Run2016E-17Jul2018-v1/MINIAOD'  ))
    tasks.append(('TauF', '/Tau/Run2016F-17Jul2018-v1/MINIAOD'  ))
    tasks.append(('TauG', '/Tau/Run2016G-17Jul2018-v1/MINIAOD'  ))
    tasks.append(('TauH', '/Tau/Run2016H-17Jul2018-v1/MINIAOD'  ))
    tasks.append(('SingleElectronB', '/SingleElectron/Run2016B-17Jul2018_ver2-v1/MINIAOD'  ))
    tasks.append(('SingleElectronC', '/SingleElectron/Run2016C-17Jul2018-v1/MINIAOD'  ))
    tasks.append(('SingleElectronD', '/SingleElectron/Run2016D-17Jul2018-v1/MINIAOD'  ))
    tasks.append(('SingleElectronE', '/SingleElectron/Run2016E-17Jul2018-v1/MINIAOD'  ))
    tasks.append(('SingleElectronF', '/SingleElectron/Run2016F-17Jul2018-v1/MINIAOD'  ))
    tasks.append(('SingleElectronG', '/SingleElectron/Run2016G-17Jul2018-v1/MINIAOD'  ))
    tasks.append(('SingleElectronH', '/SingleElectron/Run2016H-17Jul2018-v1/MINIAOD'  ))
    tasks.append(('SingleMuonB', '/SingleMuon/Run2016B-17Jul2018_ver2-v1/MINIAOD'  ))
    tasks.append(('SingleMuonC', '/SingleMuon/Run2016C-17Jul2018-v1/MINIAOD'       ))
    tasks.append(('SingleMuonD', '/SingleMuon/Run2016D-17Jul2018-v1/MINIAOD'       ))
    tasks.append(('SingleMuonE', '/SingleMuon/Run2016E-17Jul2018-v1/MINIAOD'       ))
    tasks.append(('SingleMuonF', '/SingleMuon/Run2016F-17Jul2018-v1/MINIAOD'       ))
    tasks.append(('SingleMuonG', '/SingleMuon/Run2016G-17Jul2018-v1/MINIAOD'       ))
    tasks.append(('SingleMuonH', '/SingleMuon/Run2016H-17Jul2018-v1/MINIAOD'))
    
    tasks.append(('MuonEGB', '/MuonEG/Run2016B-17Jul2018_ver2-v1/MINIAOD'  ))
    tasks.append(('MuonEGC', '/MuonEG/Run2016C-17Jul2018-v1/MINIAOD'       ))
    tasks.append(('MuonEGD', '/MuonEG/Run2016D-17Jul2018-v1/MINIAOD'       ))
    tasks.append(('MuonEGE', '/MuonEG/Run2016E-17Jul2018-v2/MINIAOD'       ))
    tasks.append(('MuonEGF', '/MuonEG/Run2016F-17Jul2018-v1/MINIAOD'       ))
    tasks.append(('MuonEGG', '/MuonEG/Run2016G-17Jul2018-v1/MINIAOD'       ))
    tasks.append(('MuonEGH', '/MuonEG/Run2016H-17Jul2018-v1/MINIAOD'))

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        
        #submit(config)
        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



