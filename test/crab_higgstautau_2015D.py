from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Oct30_Data_74X'
#config.General.requestName = 'May13_MC'
config.section_('JobType')
config.JobType.psetName = '/afs/cern.ch/work/a/adewit/private/CMSSW_7_4_12/src/UserCode/ICHiggsTauTau/test/higgstautau_cfg_74X_Sep15.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
#config.JobType.inputFiles = ['Summer15_V5_MC.db']
config.JobType.pyCfgParams = ['release=7412MINIAOD','isData=1','isNLO=0', 'globalTag=74X_dataRun2_Prompt_v4']
config.section_('Data')
#config.Data.inputDataset = 'DUMMY'
#config.Data.unitsPerJob = 30000 
config.Data.unitsPerJob = 1
config.Data.splitting = 'FileBased'
config.Data.publication = False
config.Data.ignoreLocality=False
config.Data.outLFNDirBase='/store/user/adewit/Oct30_Data_74X/'
config.section_('User')
config.section_('Site')
config.Site.whitelist = ['T2_UK_London_IC', 'T2_CH_CERN', 'T2_FR_GRIF_LLR', 'T2_UK_SGrid_Bristol', 'T3_US_FNALLPC', 'T2_DE_DESY', 'T2_IT_Bari', 'T2_BE_IIHE', 'T2_US_UCSD', 'T2_US_MIT', 'T2_IT_Pisa', 'T2_US_Wisconsin', 'T2_US_Florida', 'T2_IT_Rome','T2_FR_IPHC']
config.Site.storageSite = 'T2_UK_London_IC'

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

    #tasks.append(('Tau-2015B-17Jul','/Tau/Run2015B-17Jul2015-v1/MINIAOD')) 
    #tasks.append(('SingleMuon-2015B-17Jul','/SingleMuon/Run2015B-17Jul2015-v1/MINIAOD'))
    #tasks.append(('SingleElectron-2015B-17Jul','/SingleElectron/Run2015B-17Jul2015-v1/MINIAOD'))
    #tasks.append(('MuonEG-2015B-17Jul','/MuonEG/Run2015B-17Jul2015-v1/MINIAOD'))
    tasks.append(('Tau-2015D-promptv4','/Tau/Run2015D-PromptReco-v4/MINIAOD'))
    tasks.append(('SingleMuon-2015D-promptv4','/SingleMuon/Run2015D-PromptReco-v4/MINIAOD'))
    tasks.append(('SingleElectron-2015D-promptv4','/SingleElectron/Run2015D-PromptReco-v4/MINIAOD'))
    tasks.append(('MuonEG-2015D-promptv4','/MuonEG/Run2015D-PromptReco-v4/MINIAOD'))
    tasks.append(('Tau-2015D-Oct05','/Tau/Run2015D-05Oct2015-v1/MINIAOD'))
    tasks.append(('SingleMuon-2015D-Oct05','/SingleMuon/Run2015D-05Oct2015-v1/MINIAOD'))
    tasks.append(('SingleElectron-2015D-Oct05','/SingleElectron/Run2015D-05Oct2015-v1/MINIAOD'))
    tasks.append(('MuonEG-2015D-Oct05','/MuonEG/Run2015D-05Oct2015-v2/MINIAOD'))


    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        submit(config)



