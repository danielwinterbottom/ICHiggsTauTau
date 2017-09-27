from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Jun22_MC_91X'
config.section_('JobType')
config.JobType.psetName = 'higgstautau_cfg_future_Jun17.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
#config.JobType.inputFiles = ['Spring16_25nsV3_MC.db']
config.JobType.pyCfgParams = ['release=80XMINIAOD','isData=0','doHT=0', 'globalTag=91X_upgrade2023_realistic_v3']
config.section_('Data')
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 10000
#config.Data.unitsPerJob = 100000
config.Data.inputDBS = 'phys03'
#config.Data.unitsPerJob = 1
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.ignoreLocality= True
config.Data.allowNonValidInputDataset = True
config.Data.outLFNDirBase='/store/user/adewit/Jun22_MC_91X/'
config.section_('User')
config.section_('Site')
#config.Site.whitelist = ['T2_UK_London_IC', 'T2_CH_CERN', 'T2_FR_GRIF_LLR', 'T2_UK_SGrid_Bristol', 'T3_US_FNALLPC', 'T2_DE_DESY', 'T2_IT_Bari', 'T2_BE_IIHE', 'T2_US_UCSD', 'T2_US_MIT',  'T2_US_Wisconsin', 'T2_US_Florida', 'T2_IT_Rome','T2_FR_IPHC','T2_UK_London_Brunel']
config.Site.whitelist = ['T2_UK_London_IC','T1_UK_RAL','T2_CH_CERN','T2_UK_SGrid_Bristol','T2_DE_*']
#config.Site.blacklist = ['T2_US_*']
config.Site.storageSite = 'T2_UK_London_IC'
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
  
#    tasks.append(('VBFHToTauTau_M-125-Summer16','/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
#    tasks.append(('GluGluToHToTauTau_M-125-Summer16','/GluGluHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
#    tasks.append(('DYJetsToLL-LO-ext1-Summer16','/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/MINIAODSIM'))
    #tasks.append(('VBFHToTauTau_M-125-barrel-200PU-resub','/VBFHToTauTau_M125_14TeV_powheg_pythia8/PhaseIITDRSpring17MiniAOD-PU200_91X_upgrade2023_realistic_v3-v1/MINIAODSIM'))
    #tasks.append(('VBFHToTauTau_M-125-barrel-0PU-resub','/VBFHToTauTau_M125_14TeV_powheg_pythia8/PhaseIITDRSpring17MiniAOD-noPU_91X_upgrade2023_realistic_v3-v1/MINIAODSIM'))
#    tasks.append(('GluGluHToTauTau_M-125-barrel-200PU-resub2','/GluGluHToTauTau_M125_14TeV_powheg_pythia8/PhaseIITDRSpring17MiniAOD-PU200_91X_upgrade2023_realistic_v3-v1/MINIAODSIM'))
    #tasks.append(('GluGluHToTauTau_M-125-barrel-0PU-resub','/GluGluHToTauTau_M125_14TeV_powheg_pythia8/PhaseIITDRSpring17MiniAOD-noPU_91X_upgrade2023_realistic_v3-v1/MINIAODSIM'))
    #4tasks.append(('DYJetsToLL-barrel-200PU-resub2','/DYJetsToLL_M-50_TuneCUETP8M1_14TeV-madgraphMLM-pythia8/PhaseIITDRSpring17MiniAOD-PU200_91X_upgrade2023_realistic_v3-v1/MINIAODSIM'))
    #tasks.append(('DYJetsToLL-barrel-0PU-resub','/DYJetsToLL_M-50_TuneCUETP8M1_14TeV-madgraphMLM-pythia8/PhaseIITDRSpring17MiniAOD-noPU_91X_upgrade2023_realistic_v3-v1/MINIAODSIM'))
#    tasks.append(('VBFHToTauTau_M-125-barrel-0PU-reminiaod','/VBFHToTauTau_M125_14TeV_powheg_pythia8/adewit-crab_VBFHToTauTau_M-125-barrel-0PU-miniAOD-f98c32fb8d06a506275dc076089204ac/USER'))
#    tasks.append(('VBFHToTauTau_M-125-barrel-200PU-reminiaod','/VBFHToTauTau_M125_14TeV_powheg_pythia8/adewit-crab_VBFHToTauTau_M-125-barrel-200PU-miniAOD-f98c32fb8d06a506275dc076089204ac/USER'))
#    tasks.append(('GluGluHToTauTau_M-125-barrel-0PU-reminiaod','/GluGluHToTauTau_M125_14TeV_powheg_pythia8/adewit-crab_GluGluHToTauTau_M-125-barrel-0PU-miniAOD-f98c32fb8d06a506275dc076089204ac/USER'))
    #tasks.append(('GluGluHToTauTau_M-125-barrel-200PU-reminiaod-puppiiso-2','/GluGluHToTauTau_M125_14TeV_powheg_pythia8/adewit-crab_GluGluHToTauTau_M-125-barrel-200PU-miniAOD-f98c32fb8d06a506275dc076089204ac/USER'))
    tasks.append(('VBF-tracker-pu200','/VBFHToTauTau_M125_14TeV_powheg_pythia8/adewit-crab_VBF-pu200-barrel-miniAOD-f98c32fb8d06a506275dc076089204ac/USER'))
    tasks.append(('VBF-tracker-pu0','/VBFHToTauTau_M125_14TeV_powheg_pythia8/adewit-crab_VBF-pu0-barrel-miniAOD-f98c32fb8d06a506275dc076089204ac/USER'))
#    tasks.append(('DYJetsToLL-barrel-0PU-reminiaod','/DYJetsToLL_M-50_TuneCUETP8M1_14TeV-madgraphMLM-pythia8/adewit-crab_DYJetsToLL-barrel-0PU-miniAOD-resub-f98c32fb8d06a506275dc076089204ac/USER'))
#    tasks.append(('DYJetsToLL-barrel-200PU-reminiaod','/DYJetsToLL_M-50_TuneCUETP8M1_14TeV-madgraphMLM-pythia8/adewit-crab_DYJetsToLL-barrel-200PU-miniAOD-resub-f98c32fb8d06a506275dc076089204ac/USER'))
#    tasks.append(('TTbar-relval-300-reminiaod','/RelValTTbar_14TeV/adewit-crab_ttbar-relval-miniAOD-300-f98c32fb8d06a506275dc076089204ac/USER'))
#    tasks.append(('TTbar-relval-1000-reminiaod','/RelValTTbar_14TeV/adewit-crab_ttbar-relval-miniAOD-1000-f98c32fb8d06a506275dc076089204ac/USER'))
#    tasks.append(('TTbar-relval-3000-reminiaod','/RelValTTbar_14TeV/adewit-crab_ttbar-relval-miniAOD-f98c32fb8d06a506275dc076089204ac/USER'))

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        #submit(config)
        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



