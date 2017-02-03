from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Jan24_MC_80X'
config.section_('JobType')
config.JobType.psetName = 'higgstautau_cfg_80X_Jan17.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
#config.JobType.inputFiles = ['Spring16_25nsV3_MC.db']
config.JobType.pyCfgParams = ['release=80XMINIAOD','isData=0','doHT=0', 'globalTag=80X_mcRun2_asymptotic_2016_TrancheIV_v7','isReHLT=1', 'LHEWeights=False']
config.section_('Data')
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 100000
#config.Data.unitsPerJob = 1
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
#config.Data.ignoreLocality= True
config.Data.outLFNDirBase='/store/user/dwinterb/Jan24_MC_80X/'
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

    #tasks.append(('SUSYGluGluToHToTauTau_M-80','/SUSYGluGluToHToTauTau_M-80_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-90','/SUSYGluGluToHToTauTau_M-90_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-100','/SUSYGluGluToHToTauTau_M-100_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-110','/SUSYGluGluToHToTauTau_M-110_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-120','/SUSYGluGluToHToTauTau_M-120_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-130','/SUSYGluGluToHToTauTau_M-130_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    ##tasks.append(('SUSYGluGluToHToTauTau_M-140','/SUSYGluGluToHToTauTau_M-140_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-160','/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-180','/SUSYGluGluToHToTauTau_M-180_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-200','/SUSYGluGluToHToTauTau_M-200_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-250','/SUSYGluGluToHToTauTau_M-250_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-350','/SUSYGluGluToHToTauTau_M-350_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-450','/SUSYGluGluToHToTauTau_M-450_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-400','/SUSYGluGluToHToTauTau_M-400_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-500','/SUSYGluGluToHToTauTau_M-500_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-700','/SUSYGluGluToHToTauTau_M-700_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-800','/SUSYGluGluToHToTauTau_M-800_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-900','/SUSYGluGluToHToTauTau_M-900_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-1000','/SUSYGluGluToHToTauTau_M-1000_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-1200','/SUSYGluGluToHToTauTau_M-1200_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-1400','/SUSYGluGluToHToTauTau_M-1400_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-1600','/SUSYGluGluToHToTauTau_M-1600_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-1800','/SUSYGluGluToHToTauTau_M-1800_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-2000','/SUSYGluGluToHToTauTau_M-2000_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-2300','/SUSYGluGluToHToTauTau_M-2300_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-2600','/SUSYGluGluToHToTauTau_M-2600_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-2900','/SUSYGluGluToHToTauTau_M-2900_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-3200','/SUSYGluGluToHToTauTau_M-3200_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-80','/SUSYGluGluToBBHToTauTau_M-80_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))    
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-90','/SUSYGluGluToBBHToTauTau_M-90_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))    
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-100','/SUSYGluGluToBBHToTauTau_M-100_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-110','/SUSYGluGluToBBHToTauTau_M-110_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-120','/SUSYGluGluToBBHToTauTau_M-120_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-130','/SUSYGluGluToBBHToTauTau_M-130_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-140','/SUSYGluGluToBBHToTauTau_M-140_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-160','/SUSYGluGluToBBHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-180','/SUSYGluGluToBBHToTauTau_M-180_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-200','/SUSYGluGluToBBHToTauTau_M-200_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-250','/SUSYGluGluToBBHToTauTau_M-250_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-350','/SUSYGluGluToBBHToTauTau_M-350_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-400','/SUSYGluGluToBBHToTauTau_M-400_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-450','/SUSYGluGluToBBHToTauTau_M-450_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-500','/SUSYGluGluToBBHToTauTau_M-500_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-600','/SUSYGluGluToBBHToTauTau_M-600_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-700','/SUSYGluGluToBBHToTauTau_M-700_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-800','/SUSYGluGluToBBHToTauTau_M-800_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-900','/SUSYGluGluToBBHToTauTau_M-900_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-1000','/SUSYGluGluToBBHToTauTau_M-1000_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-1200','/SUSYGluGluToBBHToTauTau_M-1200_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-1400','/SUSYGluGluToBBHToTauTau_M-1400_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-1600','/SUSYGluGluToBBHToTauTau_M-1600_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-1800','/SUSYGluGluToBBHToTauTau_M-1800_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-2000','/SUSYGluGluToBBHToTauTau_M-2000_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-2300','/SUSYGluGluToBBHToTauTau_M-2300_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-2600','/SUSYGluGluToBBHToTauTau_M-2600_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-2900','/SUSYGluGluToBBHToTauTau_M-2900_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToBBHToTauTau_M-3200','/SUSYGluGluToBBHToTauTau_M-3200_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    ##tasks.append(('GluGluToHToTauTau_M-120','/GluGluHToTauTau_M120_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('GluGluToHToTauTau_M-125','/GluGluHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    ##tasks.append(('GluGluToHToTauTau_M-130','/GluGluHToTauTau_M130_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('GluGluToHToTauTau_herwig_M-125','/GluGluHToTauTau_M125_13TeV_powheg_herwigpp/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    ##tasks.append(('VBFHToTauTau_M-120','/VBFHToTauTau_M120_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    #tasks.append(('VBFHToTauTau_M-125','/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM')) 
    ##tasks.append(('VBFHToTauTau_M-130','/VBFHToTauTau_M130_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    #tasks.append(('WplusHToTauTau_M-120','/WplusHToTauTau_M120_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('WplusHToTauTau_M-125','/WplusHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('WplusHToTauTau_M-130','/WplusHToTauTau_M130_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('WminusHToTauTau_M-120','/WminusHToTauTau_M120_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('WminusHToTauTau_M-125','/WminusHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('WminusHToTauTau_M-130','/WminusHToTauTau_M130_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('ZHToTauTau_M-120','/ZHToTauTau_M120_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('ZHToTauTau_M-125','/ZHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('ZHToTauTau_M-130','/ZHToTauTau_M130_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('TTH_M-120','/ttHJetToTT_M120_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    #tasks.append(('TTH_M-130','/ttHJetToTT_M130_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    ##tasks.append(('TTH_M-125','/ttHJetToTT_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
    

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        #submit(config)
            
        if "GluGluToHToTauTau_M-125" in task[1] or "VBFHToTauTau_M-125" in task[1]:
            config.JobType.pyCfgParams = ['release=80XMINIAOD','isData=0','doHT=0', 'globalTag=80X_mcRun2_asymptotic_2016_TrancheIV_v7','isReHLT=1', 'LHEWeights=True']
        else:
            config.JobType.pyCfgParams = ['release=80XMINIAOD','isData=0','doHT=0', 'globalTag=80X_mcRun2_asymptotic_2016_TrancheIV_v7','isReHLT=1', 'LHEWeights=False']
            
        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



