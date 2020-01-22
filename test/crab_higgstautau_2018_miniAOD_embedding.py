from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsernameFromSiteDB

config = config()

config.General.transferOutputs = True
config.General.workArea='Jan06_MC_102X_2018'

config.JobType.psetName = 'higgstautau_cfg_102X_Aug19_2018.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['isData=0', 'isEmbed=1','globalTag=102X_dataRun2_v12']
#config.JobType.maxMemoryMB = 2500
config.JobType.allowUndistributedCMSSW = True

#config.Data.unitsPerJob = 6
#config.Data.splitting = 'FileBased'
#config.Data.unitsPerJob = 100000
#config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.ignoreLocality= True
config.Data.outLFNDirBase='/store/user/{}/{}/'.format(getUsernameFromSiteDB(), config.General.workArea)
config.Data.allowNonValidInputDataset = True
config.Data.inputDBS='phys03'

config.Site.storageSite = 'T2_UK_London_IC'
config.Site.whitelist   = ['T2_*','T1_*','T3_*']

if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand
    from httplib import HTTPException
    from CRABClient.ClientExceptions import ClientException

    # We want to put all the CRAB project directories from the tasks we submit here into one common directory.
    # That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException, hte:
            print(hte.headers)
        except ClientException as cle:
            print(cle)

    #############################################################################################
    ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
    #############################################################################################

    tasks=list()

    
    tasks.append(('EmbeddingMuTauA', '/EmbeddingRun2018A/MuTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingMuTauB', '/EmbeddingRun2018B/MuTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingMuTauC', '/EmbeddingRun2018C/MuTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingMuTauD', '/EmbeddingRun2018D/MuTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))

    tasks.append(('EmbeddingElTauA', '/EmbeddingRun2018A/ElTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingElTauB', '/EmbeddingRun2018B/ElTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingElTauC', '/EmbeddingRun2018C/ElTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingElTauD', '/EmbeddingRun2018D/ElTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))

    tasks.append(('EmbeddingElMuA', '/EmbeddingRun2018A/ElMuFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingElMuB', '/EmbeddingRun2018B/ElMuFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingElMuC', '/EmbeddingRun2018C/ElMuFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingElMuD', '/EmbeddingRun2018D/ElMuFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))

    tasks.append(('EmbeddingTauTauA', '/EmbeddingRun2018A/TauTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingTauTauB', '/EmbeddingRun2018B/TauTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingTauTauC', '/EmbeddingRun2018C/TauTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingTauTauD', '/EmbeddingRun2018D/TauTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER'))

    tasks.append(('EmbeddingMuMuA', '/EmbeddingRun2018A/MuonEmbedding-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingMuMuB', '/EmbeddingRun2018B/MuonEmbedding-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingMuMuC', '/EmbeddingRun2018C/MuonEmbedding-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingMuMuD', '/EmbeddingRun2018D/MuonEmbedding-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingElElA', '/EmbeddingRun2018A/ElectronEmbedding-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingElElB', '/EmbeddingRun2018B/ElectronEmbedding-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingElElC', '/EmbeddingRun2018C/ElectronEmbedding-inputDoubleMu_102X_miniAOD-v1/USER'))
    tasks.append(('EmbeddingElElD', '/EmbeddingRun2018D/ElectronEmbedding-inputDoubleMu_102X_miniAOD-v1/USER'))

    for task in tasks:
        print(task[0])
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]

        if "Run2018D" in task[1]:
            config.JobType.pyCfgParams = ['isData=0', 'isEmbed=1', 'doHT=0', 'globalTag=102X_dataRun2_Prompt_v15']
            config.Data.unitsPerJob = 6
            config.Data.splitting = 'FileBased'
        else:
            config.JobType.pyCfgParams = ['isData=0', 'isEmbed=1', 'doHT=0', 'globalTag=102X_dataRun2_v12']
            config.Data.unitsPerJob = 100000
            config.Data.splitting = 'EventAwareLumiBased'

        print(config.JobType.pyCfgParams)
        print(config.Data.unitsPerJob)
        
        print(config)
        submit(config)



