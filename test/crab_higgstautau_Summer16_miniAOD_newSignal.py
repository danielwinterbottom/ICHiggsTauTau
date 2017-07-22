from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Jul22_MC_80X'
config.section_('JobType')
config.JobType.psetName = 'higgstautau_cfg_80X_Mar17.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
#config.JobType.inputFiles = ['Spring16_25nsV3_MC.db']
config.JobType.pyCfgParams = ['release=80XMINIAOD','isData=0','doHT=0', 'globalTag=80X_mcRun2_asymptotic_2016_TrancheIV_v8','isReHLT=1', 'LHEWeights=True']
config.section_('Data')
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 50000
#config.Data.unitsPerJob = 1
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
#config.Data.ignoreLocality= True
config.Data.outLFNDirBase='/store/user/dwinterb/Jul22_MC_80X/'
config.section_('User')
config.section_('Site')
#config.Site.whitelist = ['T2_UK_London_IC', 'T2_CH_CERN', 'T2_FR_GRIF_LLR', 'T2_UK_SGrid_Bristol', 'T3_US_FNALLPC', 'T2_DE_DESY', 'T2_IT_Bari', 'T2_BE_IIHE', 'T2_IT_Rome','T2_FR_IPHC','T2_UK_London_Brunel']
#config.Site.blacklist  = ['T2_US_*']
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
    
    # use option 'LHETag=source' to get LHE weights for privatly produced samples and add config.Data.inputDBS = 'phys03' above
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-80-NLO','/SUSYGluGluToBBHToTauTau_M-80_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-130-NLO','/SUSYGluGluToBBHToTauTau_M-130_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-200-NLO','/SUSYGluGluToBBHToTauTau_M-200_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-350-NLO','/SUSYGluGluToBBHToTauTau_M-350_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-700-NLO','/SUSYGluGluToBBHToTauTau_M-700_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-1200-NLO','/SUSYGluGluToBBHToTauTau_M-1200_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-1800-NLO','/SUSYGluGluToBBHToTauTau_M-1800_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-80-NLO-QshDown-','/SUSYGluGluToBBHToTauTau_M-80_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshDown/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-80-NLO-QshUp','/SUSYGluGluToBBHToTauTau_M-80_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshUp/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-130-NLO-QshDown','/SUSYGluGluToBBHToTauTau_M-130_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshDown/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-130-NLO-QshUp','/SUSYGluGluToBBHToTauTau_M-130_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshUp/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-200-NLO-QshDown','/SUSYGluGluToBBHToTauTau_M-200_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshDown/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-200-NLO-QshUp','/SUSYGluGluToBBHToTauTau_M-200_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshUp/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-350-NLO-QshDown','/SUSYGluGluToBBHToTauTau_M-350_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshDown/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-350-NLO-QshUp','/SUSYGluGluToBBHToTauTau_M-350_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshUp/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-700-NLO-QshDown','/SUSYGluGluToBBHToTauTau_M-700_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshDown/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-700-NLO-QshUp','/SUSYGluGluToBBHToTauTau_M-700_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshUp/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-1200-NLO-QshDown','/SUSYGluGluToBBHToTauTau_M-1200_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshDown/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-1200-NLO-QshUp','/SUSYGluGluToBBHToTauTau_M-1200_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshUp/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-1800-NLO-QshDown','/SUSYGluGluToBBHToTauTau_M-1800_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshDown/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-1800-NLO-QshUp','/SUSYGluGluToBBHToTauTau_M-1800_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshUp/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-3200-NLO-QshDown','/SUSYGluGluToBBHToTauTau_M-3200_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshDown/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-3200-NLO-QshUp','/SUSYGluGluToBBHToTauTau_M-3200_TuneCUETP8M1_13TeV-amcatnlo-pythia8-QshUp/agilbert-miniaod-prod-150517-28028af67189b3de7224b79195bd0e1d/USER'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-3200-NLO','/SUSYGluGluToBBHToTauTau_M-3200_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-miniaod-prod-250517-28028af67189b3de7224b79195bd0e1d/USER'))
    
    tasks.append(('SUSYGluGluToBBHToTauTau_M-1000-NLO', '/SUSYGluGluToBBHToTauTau_M-1000_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-110-NLO', '/SUSYGluGluToBBHToTauTau_M-110_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-100-NLO', '/SUSYGluGluToBBHToTauTau_M-100_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-120-NLO', '/SUSYGluGluToBBHToTauTau_M-120_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-1200-NLO', '/SUSYGluGluToBBHToTauTau_M-1200_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-1400-NLO', '/SUSYGluGluToBBHToTauTau_M-1400_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-130-NLO', '/SUSYGluGluToBBHToTauTau_M-130_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-1600-NLO', '/SUSYGluGluToBBHToTauTau_M-1600_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-140-NLO', '/SUSYGluGluToBBHToTauTau_M-140_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-1800-NLO', '/SUSYGluGluToBBHToTauTau_M-1800_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-160-NLO', '/SUSYGluGluToBBHToTauTau_M-160_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-2000-NLO', '/SUSYGluGluToBBHToTauTau_M-2000_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-180-NLO', '/SUSYGluGluToBBHToTauTau_M-180_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-2300-NLO', '/SUSYGluGluToBBHToTauTau_M-2300_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-200-NLO', '/SUSYGluGluToBBHToTauTau_M-200_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-2600-NLO', '/SUSYGluGluToBBHToTauTau_M-2600_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-250-NLO', '/SUSYGluGluToBBHToTauTau_M-250_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-3200-NLO', '/SUSYGluGluToBBHToTauTau_M-3200_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-2900-NLO', '/SUSYGluGluToBBHToTauTau_M-2900_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-400-NLO', '/SUSYGluGluToBBHToTauTau_M-400_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-350-NLO', '/SUSYGluGluToBBHToTauTau_M-350_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-500-NLO', '/SUSYGluGluToBBHToTauTau_M-500_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-450-NLO', '/SUSYGluGluToBBHToTauTau_M-450_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-700-NLO', '/SUSYGluGluToBBHToTauTau_M-700_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-600-NLO', '/SUSYGluGluToBBHToTauTau_M-600_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-80-NLO', '/SUSYGluGluToBBHToTauTau_M-80_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'  ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-800-NLO', '/SUSYGluGluToBBHToTauTau_M-800_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-90-NLO', '/SUSYGluGluToBBHToTauTau_M-90_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'  ))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-900-NLO', '/SUSYGluGluToBBHToTauTau_M-900_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' ))
    

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        #submit(config)
            
            
        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



