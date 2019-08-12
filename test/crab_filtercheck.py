from CRABClient.UserUtilities import config

config = config()

config.General.transferOutputs = True
config.General.workArea='FilterCheck'

config.JobType.psetName = 'higgstautau_cfg_94X_Apr18_pfCands_in102X.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
cfgParams = ['release=102XMINIAOD','isData=0', 'globalTag=102X_mc2017_realistic_v6']
config.JobType.allowUndistributedCMSSW = True

config.Data.unitsPerJob = 100000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.outLFNDirBase='/store/user/dwinterb/{}/'.format(config.General.workArea)
config.Data.allowNonValidInputDataset = True
config.Data.inputDBS = 'phys03'

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

    tasks.append(('GluGluToHToTauTau_M125_nospinner', '/GluGluHToPseudoscalarTauTau_M125_13TeV_powheg_pythia8_2017-GEN_TEST07Jan19/adow-GluGluToHToTauTauNoSpin_M125_13TeV_pythia8_2017-MINIAOD-5f646ecd4e1c7a39ab0ed099ff55ceb9/USER'))
    tasks.append(('GluGluToHToTauTau_M125_nospinner-filter', '/GluGluHToTauTau_M125_13TeV_powheg_pythia8_nospinner-filter-v2/dwinterb-GluGluHToTauTau_M125_13TeV_powheg_pythia8_nospinner-filter-v2-miniAOD-5f646ecd4e1c7a39ab0ed099ff55ceb9/USER'))

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]

        config.Data.inputDataset = task[1]
        config.Data.unitsPerJob = 100000
        config.Data.splitting = 'EventAwareLumiBased'
        config.Data.userInputFiles = None

        if "nospinner" in task[0]:
            config.JobType.pyCfgParams = cfgParams + ['LHEWeights=True','doHT=1' ,'tauSpinner=True']
        else:
            config.JobType.pyCfgParams = cfgParams
        print config.Data.unitsPerJob
        print config.Data.splitting

        print(config)
        submit(config)

