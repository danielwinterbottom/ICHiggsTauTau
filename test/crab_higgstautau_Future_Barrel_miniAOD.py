from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Jun22_MC_91X'
config.section_('JobType')
config.JobType.psetName = 'reRunTaus_miniAOD.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['out_miniAOD_prod.root']
#config.JobType.inputFiles = ['Spring16_25nsV3_MC.db']
#config.JobType.pyCfgParams = ['release=80XMINIAOD','isData=0','doHT=0', 'globalTag=91X_upgrade2023_realistic_v3']
config.section_('Data')
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 700
#config.Data.unitsPerJob = 100000
#config.Data.inputDBS = 'phys03'
#config.Data.unitsPerJob = 1
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = True
#config.Data.ignoreLocality= True
config.Data.allowNonValidInputDataset = True
config.Data.outLFNDirBase='/store/user/adewit/Jun22_MC_91X/'
config.section_('User')
config.section_('Site')
#config.Site.whitelist = ['T2_UK_London_IC', 'T2_CH_CERN', 'T2_FR_GRIF_LLR', 'T2_UK_SGrid_Bristol', 'T3_US_FNALLPC', 'T2_DE_DESY', 'T2_IT_Bari', 'T2_BE_IIHE', 'T2_US_UCSD', 'T2_US_MIT',  'T2_US_Wisconsin', 'T2_US_Florida', 'T2_IT_Rome','T2_FR_IPHC','T2_UK_London_Brunel']
#config.Site.whitelist = ['T2_UK_London_IC','T1_UK_RAL']
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
  
#    tasks.append(('GluGluHToTauTau_M-125-barrel-200PU-miniAOD','/GluGluHToTauTau_M125_14TeV_powheg_pythia8/PhaseIITDRSpring17DR-PU200_91X_upgrade2023_realistic_v3-v1/AODSIM'))
#    tasks.append(('GluGluHToTauTau_M-125-barrel-0PU-miniAOD','/GluGluHToTauTau_M125_14TeV_powheg_pythia8/PhaseIITDRSpring17DR-noPU_91X_upgrade2023_realistic_v3-v1/AODSIM'))
#    tasks.append(('VBFHToTauTau_M-125-barrel-200PU-miniAOD','/VBFHToTauTau_M125_14TeV_powheg_pythia8/PhaseIITDRSpring17DR-PU200_91X_upgrade2023_realistic_v3-v1/AODSIM'))
#    tasks.append(('VBFHToTauTau_M-125-barrel-0PU-miniAOD','/VBFHToTauTau_M125_14TeV_powheg_pythia8/PhaseIITDRSpring17DR-noPU_91X_upgrade2023_realistic_v3-v1/AODSIM'))
#    tasks.append(('DYJetsToLL-barrel-200PU-miniAOD-resub','/DYJetsToLL_M-50_TuneCUETP8M1_14TeV-madgraphMLM-pythia8/PhaseIITDRSpring17DR-PU200_91X_upgrade2023_realistic_v3-v1/AODSIM'))
#    tasks.append(('DYJetsToLL-barrel-0PU-miniAOD-resub','/DYJetsToLL_M-50_TuneCUETP8M1_14TeV-madgraphMLM-pythia8/PhaseIITDRSpring17DR-noPU_91X_upgrade2023_realistic_v3-v1/AODSIM'))
    #tasks.append(('ttbar-relval-miniAOD','/RelValTTbar_14TeV/CMSSW_9_1_1_patch3-91X_upgrade2023_realistic_v3_D17noPUEA3000Ultimate-v1/GEN-SIM-RECO'))
#    tasks.append(('ttbar-relval-miniAOD-1000','/RelValTTbar_14TeV/CMSSW_9_1_1_patch3-91X_upgrade2023_realistic_v3_D17noPUEA1000-v2/GEN-SIM-RECO'))
#    tasks.append(('ttbar-relval-miniAOD-300','/RelValTTbar_14TeV/CMSSW_9_1_1_patch3-91X_upgrade2023_realistic_v3_D17noPUEA300-v2/GEN-SIM-RECO'))
    tasks.append(('VBF-pu200-barrel-miniAOD','/VBFHToTauTau_M125_14TeV_powheg_pythia8/PhaseIIFall16DR82-PU200_90X_upgrade2023_realistic_v1-v1/AODSIM'))
    tasks.append(('VBF-pu0-barrel-miniAOD','/VBFHToTauTau_M125_14TeV_powheg_pythia8/PhaseIIFall16DR82-NoPU_90X_upgrade2023_realistic_v1-v1/AODSIM'))

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        #submit(config)
        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



