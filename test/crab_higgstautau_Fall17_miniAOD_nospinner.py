from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='spinner_check'
config.section_('JobType')
config.JobType.psetName = 'higgstautau_cfg_94X_Apr18_pfCands.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
#config.JobType.maxMemoryMB = 3000
CfgParams = ['release=94XMINIAOD','isData=0', 'globalTag=94X_mc2017_realistic_v17']
config.section_('Data')
config.Data.unitsPerJob = 720 # for automatic splitting
config.Data.splitting = 'Automatic'
# config.Data.unitsPerJob = 100000
# config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.outLFNDirBase='/store/user/dwinterb/spinner_check/'
config.Data.allowNonValidInputDataset = True
config.Data.inputDBS = 'phys03'
config.section_('User')
config.section_('Site')
config.Site.storageSite = 'T2_UK_London_IC'
config.JobType.allowUndistributedCMSSW = True
# for deep tau need the next line
config.JobType.scriptExe = 'submitCrabDeepTau.sh'

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

    # tests
    # tasks.append(('GluGluToHToTauTauPseudoscalarDecay_M125_amcatnloFXFX', '/GluGluToHToTauTauPseudoscalarDecay_M125_13TeV_amcatnloFXFX_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    # tasks.append(('SUSYGluGluToHToTauTau_M-120', '/SUSYGluGluToHToTauTau_M-120_TuneCP5_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))

    # CP private production
    #tasks.append(('GluGluToHToTauTau_M125_nospinner', '/GluGluHToPseudoscalarTauTau_M125_13TeV_powheg_pythia8_2017-GEN_TEST07Jan19/adow-GluGluToHToTauTauNoSpin_M125_13TeV_pythia8_2017-MINIAOD-5f646ecd4e1c7a39ab0ed099ff55ceb9/USER'))
    #tasks.append(('GluGluToHToTauTau_M125_nospinner-filter', ' 	/GluGluHToTauTau_M125_13TeV_powheg_pythia8_nospinner-filter-v2/dwinterb-GluGluHToTauTau_M125_13TeV_powheg_pythia8_nospinner-filter-v2-miniAOD-5f646ecd4e1c7a39ab0ed099ff55ceb9/USER'))
    #tasks.append(('VBFHToTauTau_M125_nospinner-2017', '/VBFHToMaxmixTauTau_M125_13TeV_powheg_pythia8_2017-GEN_TEST07Jan19/adow-VBFHToTauTauNoSpin_M125_13TeV_pythia8_2017-MINIAOD-5f646ecd4e1c7a39ab0ed099ff55ceb9/USER'))
    tasks.append(('VBFHToTauTau_M125_nospinner','/VBFHToTauTau_M125_13TeV_powheg_pythia8_nospinner-filter-v2/dwinterb-VBFHToTauTau_M125_13TeV_powheg_pythia8_nospinner-filter-v2-miniAOD-5f646ecd4e1c7a39ab0ed099ff55ceb9/USER'))
    tasks.append(('VBFHToTauTau_M125_PS','/VBFHToTauTau_M125_13TeV_powheg_pythia8_PS-filter-v3/dwinterb-VBFHToTauTau_M125_13TeV_powheg_pythia8_PS-filter-v3-miniAOD-5f646ecd4e1c7a39ab0ed099ff55ceb9/USER'))
    tasks.append(('VBFHToTauTau_M125_SM','/VBFHToTauTau_M125_13TeV_powheg_pythia8_SM-filter-v2/dwinterb-VBFHToTauTau_M125_13TeV_powheg_pythia8_SM-filter-v2-miniAOD-5f646ecd4e1c7a39ab0ed099ff55ceb9/USER'))

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
 
        config.JobType.pyCfgParams = CfgParams + ['LHEWeights=True','doHT=1' ,'tauSpinner=True']
        print config.Data.unitsPerJob
        print config.Data.splitting

        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



