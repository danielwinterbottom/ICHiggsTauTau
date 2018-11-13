from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Apr02_MC_80X'
config.section_('JobType')
config.JobType.psetName = 'higgstautau_cfg_80X_Nov17.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
#config.JobType.inputFiles = ['Spring16_25nsV3_MC.db']
CfgParams = ['release=80XMINIAOD','isData=0','doHT=1', 'globalTag=80X_mcRun2_asymptotic_2016_TrancheIV_v8','isReHLT=1','LHEWeights=True', 'LHETag=source']
config.section_('Data')
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 100000
#config.Data.unitsPerJob = 1
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
#config.Data.ignoreLocality= True
config.Data.outLFNDirBase='/store/user/dwinterb/Apr02_MC_80X/'
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
    
    tasks.append(('GluGluH2JetsToTauTau_M125_CPmixing_sm', '/GluGluH2JetsToTauTau_M125_13TeV_CPmixing_sm_JHU/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('GluGluH2JetsToTauTau_M125_CPmixing_maxmix','/GluGluH2JetsToTauTau_M125_13TeV_CPmixing_maxmix_JHU/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('GluGluH2JetsToTauTau_M125_CPmixing_pseudoscalar','/GluGluH2JetsToTauTau_M125_13TeV_CPmixing_pseudoscalar_JHU/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('VBFHiggs0PM_M-125','/VBFHiggs0PM_M-125_13TeV-JHUGenV6/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('VBFHiggs0Mf05ph0_M-125','/VBFHiggs0Mf05ph0_M-125_13TeV-JHUGenV6/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('VBFHiggs0M_M-125','/VBFHiggs0M_M-125_13TeV-JHUGenV6/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))

    tasks.append(('ZHiggs0PM_M-125', '/ZHiggs0PM_Undecayed_M-125_13TeV-JHUgenV6/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM'))
    tasks.append(('ZHiggs0Mf05ph0_M-125', '/ZHiggs0MfZH05ph0_Undecayed_M-125_13TeV-JHUgenV6/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM'))
    tasks.append(('ZHiggs0M_M-125', '/ZHiggs0M_Undecayed_M-125_13TeV-JHUgenV6/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM'))

    tasks.append(('WHiggs0PM_M-125', '/WHiggs0PM_Undecayed_M-125_13TeV-JHUgenV6/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM'))
    tasks.append(('WHiggs0Mf05ph0_M-125', '/WHiggs0MfWH05ph0_Undecayed_M-125_13TeV-JHUgenV6/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM'))
    tasks.append(('WHiggs0M_M-125', '/WHiggs0M_Undecayed_M-125_13TeV-JHUgenV6/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM'))


    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        #submit(config)

        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



