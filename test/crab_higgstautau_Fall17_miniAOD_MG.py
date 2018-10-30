from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Oct15_MC_94X'
config.section_('JobType')
config.JobType.psetName = 'higgstautau_cfg_94X_Apr18.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['release=94XMINIAOD','isData=0', 'globalTag=94X_mc2017_realistic_v15', 'doHT=1', 'LHEWeights=True','includenpNLO=True']
config.section_('Data')
config.Data.unitsPerJob = 100000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.outLFNDirBase='/store/user/adow/Oct15_MC_94X/'
config.Data.allowNonValidInputDataset = True
config.section_('User')
config.section_('Site')
config.Site.storageSite = 'T2_UK_London_IC'
config.JobType.allowUndistributedCMSSW = True
config.Data.inputDBS = 'phys03'

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

    tasks.append(('GluGluToPseudoscalarHToTauTau_M125_amcatnloFXFX', '/GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/dwinterb-GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-MiniAOD-v2-5f646ecd4e1c7a39ab0ed099ff55ceb9/USER'))
    tasks.append(('GluGluToMaxmixHToTauTau_M125_amcatnloFXFX', '/GluGluToMaxmixHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/dwinterb-GluGluToMaxmixHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-MiniAOD-v2-5f646ecd4e1c7a39ab0ed099ff55ceb9/USER'))

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        config.Data.unitsPerJob = 100000
        config.Data.splitting = 'EventAwareLumiBased'
        config.Data.userInputFiles = None

        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



