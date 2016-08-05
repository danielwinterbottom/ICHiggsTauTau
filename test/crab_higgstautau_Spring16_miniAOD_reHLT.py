from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='July21_MC_80X'
config.section_('JobType')
config.JobType.psetName = '/afs/cern.ch/work/a/adewit/private/CMSSW_8_0_9/src/UserCode/ICHiggsTauTau/test/higgstautau_cfg_80X_Apr16.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
#config.JobType.inputFiles = ['Spring16_25nsV3_MC.db']
config.JobType.pyCfgParams = ['release=80XMINIAOD','isData=0','doHT=0', 'globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1','isReHLT=1']
config.section_('Data')
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 100000
#config.Data.unitsPerJob = 1
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
#config.Data.ignoreLocality= True
config.Data.outLFNDirBase='/store/user/adewit/July21_MC_80X/'
config.section_('User')
config.section_('Site')
config.Site.whitelist = ['T2_UK_London_IC', 'T2_CH_CERN', 'T2_FR_GRIF_LLR', 'T2_UK_SGrid_Bristol', 'T3_US_FNALLPC', 'T2_DE_DESY', 'T2_IT_Bari', 'T2_BE_IIHE', 'T2_IT_Rome','T2_FR_IPHC','T2_UK_London_Brunel']
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

#    tasks.append(('SUSYGluGluToHToTauTau_M-80','/SUSYGluGluToHToTauTau_M-80_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-90-v2','/SUSYGluGluToHToTauTau_M-90_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-100','/SUSYGluGluToHToTauTau_M-100_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-110','/SUSYGluGluToHToTauTau_M-110_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-120-v2','/SUSYGluGluToHToTauTau_M-120_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-130-v2','/SUSYGluGluToHToTauTau_M-130_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-180-v2','/SUSYGluGluToHToTauTau_M-180_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-350-v2','/SUSYGluGluToHToTauTau_M-350_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-450-v2','/SUSYGluGluToHToTauTau_M-450_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-400','/SUSYGluGluToHToTauTau_M-400_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-700-v2','/SUSYGluGluToHToTauTau_M-700_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-900','/SUSYGluGluToHToTauTau_M-900_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    #tasks.append(('SUSYGluGluToHToTauTau_M-1000-v4','/SUSYGluGluToHToTauTau_M-1000_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToHToTauTau_M-1200-v4','/SUSYGluGluToHToTauTau_M-1200_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToHToTauTau_M-1400-v3','/SUSYGluGluToHToTauTau_M-1400_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-1500-v2','/SUSYGluGluToHToTauTau_M-1500_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-1600-v2','/SUSYGluGluToHToTauTau_M-1600_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-1800','/SUSYGluGluToHToTauTau_M-1800_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToHToTauTau_M-2000-v3','/SUSYGluGluToHToTauTau_M-2000_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-2300','/SUSYGluGluToHToTauTau_M-2300_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToHToTauTau_M-2600','/SUSYGluGluToHToTauTau_M-2600_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToHToTauTau_M-2900-v3','/SUSYGluGluToHToTauTau_M-2900_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToHToTauTau_M-3200-v3','/SUSYGluGluToHToTauTau_M-3200_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-80-v2','/SUSYGluGluToBBHToTauTau_M-80_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))    
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-90-v2','/SUSYGluGluToBBHToTauTau_M-90_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))    
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-120','/SUSYGluGluToBBHToTauTau_M-120_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-130','/SUSYGluGluToBBHToTauTau_M-130_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-140-v2','/SUSYGluGluToBBHToTauTau_M-140_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-110-v2','/SUSYGluGluToBBHToTauTau_M-110_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-180-v2','/SUSYGluGluToBBHToTauTau_M-180_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-250-v2','/SUSYGluGluToBBHToTauTau_M-250_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-300-v2','/SUSYGluGluToBBHToTauTau_M-300_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-400','/SUSYGluGluToBBHToTauTau_M-400_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-450','/SUSYGluGluToBBHToTauTau_M-450_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-500-v2','/SUSYGluGluToBBHToTauTau_M-500_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-600-v2','/SUSYGluGluToBBHToTauTau_M-600_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-700-v2','/SUSYGluGluToBBHToTauTau_M-700_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-900','/SUSYGluGluToBBHToTauTau_M-900_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-1000-v3','/SUSYGluGluToBBHToTauTau_M-1000_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-1200-v2','/SUSYGluGluToBBHToTauTau_M-1200_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-1500-v2','/SUSYGluGluToBBHToTauTau_M-1500_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-1600','/SUSYGluGluToBBHToTauTau_M-1600_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v2/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-1800-v3','/SUSYGluGluToBBHToTauTau_M-1800_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-2000-v3','/SUSYGluGluToBBHToTauTau_M-2000_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-2300-v3','/SUSYGluGluToBBHToTauTau_M-2300_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    tasks.append(('SUSYGluGluToBBHToTauTau_M-2600-v3','/SUSYGluGluToBBHToTauTau_M-2600_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('SUSYGluGluToBBHToTauTau_M-2900-v2','/SUSYGluGluToBBHToTauTau_M-2900_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('GluGluToHToTauTau_M-125-v2','/GluGluHToTauTau_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
#    tasks.append(('VBFHToTauTau_M-125-v2','/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM')) 
#    tasks.append(('reHLT-TT-v2','/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext3-v1/MINIAODSIM'))

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        submit(config)



