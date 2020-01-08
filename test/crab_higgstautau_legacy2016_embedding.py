from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsernameFromSiteDB

config = config()

config.General.transferOutputs = True
config.General.workArea='Oct2_MC_102X_2016'

config.JobType.psetName = 'higgstautau_cfg_102X_Aug19_Leg2016.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.maxMemoryMB = 5000
config.JobType.pyCfgParams = ['release=102XMINIAOD','isEmbed=1','isData=0','doHT=0', 'globalTag=102X_dataRun2_v12']
config.JobType.allowUndistributedCMSSW = True

config.Data.unitsPerJob = 10000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.ignoreLocality= True
config.Data.outLFNDirBase='/store/user/{}/{}/'.format(getUsernameFromSiteDB(), config.General.workArea)
config.Data.allowNonValidInputDataset = True
config.Data.inputDBS = 'phys03'

config.Site.storageSite = 'T2_UK_London_IC'
config.Site.whitelist   = ['T2_*','T1_*','T3_*']

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

    tasks.append(('EmbeddingMuTauB', '/EmbeddingRun2016B/MuTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingMuTauC', '/EmbeddingRun2016C/MuTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingMuTauD', '/EmbeddingRun2016D/MuTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingMuTauE', '/EmbeddingRun2016E/MuTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingMuTauF', '/EmbeddingRun2016F/MuTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingMuTauG', '/EmbeddingRun2016G/MuTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingMuTauH', '/EmbeddingRun2016H/MuTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    
    tasks.append(('EmbeddingElTauB', '/EmbeddingRun2016B/ElTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElTauC', '/EmbeddingRun2016C/ElTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElTauD', '/EmbeddingRun2016D/ElTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElTauE', '/EmbeddingRun2016E/ElTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElTauF', '/EmbeddingRun2016F/ElTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElTauG', '/EmbeddingRun2016G/ElTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElTauH', '/EmbeddingRun2016H/ElTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    
    tasks.append(('EmbeddingElMuB', '/EmbeddingRun2016B/ElMuFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElMuC', '/EmbeddingRun2016C/ElMuFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElMuD', '/EmbeddingRun2016D/ElMuFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElMuE', '/EmbeddingRun2016E/ElMuFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElMuF', '/EmbeddingRun2016F/ElMuFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElMuG', '/EmbeddingRun2016G/ElMuFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElMuH', '/EmbeddingRun2016H/ElMuFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    
    tasks.append(('EmbeddingTauTauB', '/EmbeddingRun2016B/TauTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingTauTauC', '/EmbeddingRun2016C/TauTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingTauTauD', '/EmbeddingRun2016D/TauTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingTauTauE', '/EmbeddingRun2016E/TauTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingTauTauF', '/EmbeddingRun2016F/TauTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingTauTauG', '/EmbeddingRun2016G/TauTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingTauTauH', '/EmbeddingRun2016H/TauTauFinalState-inputDoubleMu_94X_Legacy_miniAOD-v5/USER')) 

    tasks.append(('EmbeddingElElB', '/EmbeddingRun2016B/ElectronEmbedding-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElElC', '/EmbeddingRun2016C/ElectronEmbedding-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElElD', '/EmbeddingRun2016D/ElectronEmbedding-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElElE', '/EmbeddingRun2016E/ElectronEmbedding-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElElF', '/EmbeddingRun2016F/ElectronEmbedding-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElElG', '/EmbeddingRun2016G/ElectronEmbedding-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElElH', '/EmbeddingRun2016H/ElectronEmbedding-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))

    tasks.append(('EmbeddingMuMuB', '/EmbeddingRun2016B/MuonEmbedding-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingMuMuC', '/EmbeddingRun2016C/MuonEmbedding-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingMuMuD', '/EmbeddingRun2016D/MuonEmbedding-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingMuMuE', '/EmbeddingRun2016E/MuonEmbedding-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingMuMuF', '/EmbeddingRun2016F/MuonEmbedding-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingMuMuG', '/EmbeddingRun2016G/MuonEmbedding-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))
    tasks.append(('EmbeddingMuMuH', '/EmbeddingRun2016H/MuonEmbedding-inputDoubleMu_94X_Legacy_miniAOD-v5/USER'))


    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        
        print(config)
        submit(config)

