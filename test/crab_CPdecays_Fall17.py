from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Dec03_GENSIM_94X'
config.section_('JobType')
config.JobType.psetName = 'nonujets.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
CfgParams = ['release=94XMINIAOD','isData=0', 'globalTag=94X_mc2017_realistic_v15']
config.section_('Data')
config.Data.unitsPerJob = 10000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
# config.Data.inputDBS = 'phys03'
config.Data.outLFNDirBase='/store/user/adow/Dec03_GENSIM_94X/'
config.Data.allowNonValidInputDataset = True
config.section_('User')
config.section_('Site')
config.Site.storageSite = 'T2_UK_London_IC'
config.Site.blacklist = ['T2_RU_*','T2_US_*']
config.JobType.allowUndistributedCMSSW = True

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

    # tasks.append(('VBFHToPseudoscalarTauTau_GEN', '/VBFHToPseudoscalarTauTau_M125_13TeV_powheg_pythia8_2017-GEN_TEST4/adow-VBFHToPseudoscalarTauTau_M125_13TeV_powheg_pythia8_2017-GEN_TEST4_RAWSIMoutput-b68554fa21e3b8151219b8bd445b4f65/USER'))
    # tasks.append(('VBFHToMaxmixTauTau_GEN', '/VBFHToMaxmixTauTau_M125_13TeV_powheg_pythia8_2017-GEN_TEST4/adow-VBFHToMaxmixTauTau_M125_13TeV_powheg_pythia8_2017-GEN_TEST4_RAWSIMoutput-b68554fa21e3b8151219b8bd445b4f65/USER'))

    # tasks.append(('GluGluHToPseudoscalarTauTau_GEN', '/GluGluHToPseudoscalarTauTau_M125_13TeV_powheg_pythia8_2017-GEN_TEST/adow-GluGluHToPseudoscalarTauTau_M125_13TeV_powheg_pythia8_2017-GEN_TEST-7bf405a5752cd2de7cd85ee9641268bf/USER'))
    # tasks.append(('GluGluHToMaxmixTauTau_GEN', '/GluGluHToMaxmixTauTau_M125_13TeV_powheg_pythia8_2017-GEN_TEST/adow-GluGluHToMaxmixTauTau_M125_13TeV_powheg_pythia8_2017-GEN_TEST-7bf405a5752cd2de7cd85ee9641268bf/USER'))

    tasks.append(('SUSYGluGluToHToTauTau_M-120', '/SUSYGluGluToHToTauTau_M-120_TuneCP5_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
 
        p = Process(target=submit, args=(config,))
        p.start()
        p.join()

