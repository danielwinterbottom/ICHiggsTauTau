from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Aug14_MC_102X_2017'
config.section_('JobType')
config.JobType.psetName = 'higgstautau_cfg_102X_Aug19_2017.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['release=102XMINIAOD','isData=0', 'isEmbed=1','globalTag=102X_dataRun2_v8']
config.JobType.maxMemoryMB = 2500
config.section_('Data')
config.Data.unitsPerJob = 100000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
#config.Data.ignoreLocality= True
config.Data.outLFNDirBase='/store/user/dwinterb/Aug14_MC_102X_2017/'
config.Data.allowNonValidInputDataset = True
#config.Data.inputDBS='phys03'
config.section_('User')
config.section_('Site')
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
    
    tasks.append(('EmbeddingMuTauB', '/EmbeddingRun2017B/MuTauFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuTauC', '/EmbeddingRun2017C/MuTauFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuTauD', '/EmbeddingRun2017D/MuTauFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuTauE', '/EmbeddingRun2017E/MuTauFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuTauF', '/EmbeddingRun2017F/MuTauFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))

    tasks.append(('EmbeddingElTauB', '/EmbeddingRun2017B/ElTauFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingElTauC', '/EmbeddingRun2017C/ElTauFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingElTauD', '/EmbeddingRun2017D/ElTauFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingElTauE', '/EmbeddingRun2017E/ElTauFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingElTauF', '/EmbeddingRun2017F/ElTauFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))

    tasks.append(('EmbeddingElMuB', '/EmbeddingRun2017B/ElMuFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingElMuC', '/EmbeddingRun2017C/ElMuFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingElMuD', '/EmbeddingRun2017D/ElMuFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingElMuE', '/EmbeddingRun2017E/ElMuFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingElMuF', '/EmbeddingRun2017F/ElMuFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))

    tasks.append(('EmbeddingTauTauB', '/EmbeddingRun2017B/TauTauFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingTauTauC', '/EmbeddingRun2017C/TauTauFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingTauTauD', '/EmbeddingRun2017D/TauTauFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingTauTauE', '/EmbeddingRun2017E/TauTauFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingTauTauF', '/EmbeddingRun2017F/TauTauFinalState-inputDoubleMu_94X_miniAOD-v2/USER'))

    tasks.append(('EmbeddingElElB', '/EmbeddingRun2017B/ElectronEmbedding-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingElElC', '/EmbeddingRun2017C/ElectronEmbedding-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingElElD', '/EmbeddingRun2017D/ElectronEmbedding-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingElElE', '/EmbeddingRun2017E/ElectronEmbedding-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingElElF', '/EmbeddingRun2017F/ElectronEmbedding-inputDoubleMu_94X_miniAOD-v2/USER'))

    tasks.append(('EmbeddingMuMuB', '/EmbeddingRun2017B/MuonEmbedding-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuMuC', '/EmbeddingRun2017C/MuonEmbedding-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuMuD', '/EmbeddingRun2017D/MuonEmbedding-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuMuE', '/EmbeddingRun2017E/MuonEmbedding-inputDoubleMu_94X_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuMuF', '/EmbeddingRun2017F/MuonEmbedding-inputDoubleMu_94X_miniAOD-v2/USER'))


    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]

        print config.JobType.pyCfgParams
        print config.Data.unitsPerJob
        
        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



