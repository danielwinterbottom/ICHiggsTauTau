from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Trigger_80X'
config.section_('JobType')
config.JobType.psetName = 'higgstautau_cfg_80X_Nov17_embedding.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['release=80XMINIAOD','isData=1', 'globalTag=80X_dataRun2_2016SeptRepro_v7']
config.section_('Data')
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 80000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.outLFNDirBase='/store/user/dwinterb/Trigger_80X/'
config.section_('User')
config.section_('Site')
config.Site.storageSite = 'T2_UK_London_IC'
config.JobType.allowUndistributedCMSSW = True
config.Data.inputDBS='phys03'
config.Data.allowNonValidInputDataset = True

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
    
    #commented out samples aren't available yet
    

    #tasks.append(('EmbeddingMuTauB', '/EmbeddingRun2016B/MuTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingMuTauC', '/EmbeddingRun2016C/MuTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingMuTauD', '/EmbeddingRun2016D/MuTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingMuTauE', '/EmbeddingRun2016E/MuTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingMuTauF', '/EmbeddingRun2016F/MuTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingMuTauG', '/EmbeddingRun2016G/MuTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingMuTauH', '/EmbeddingRun2016H/MuTauFinalState-imputPromptDoubleMu_mirror_miniAOD-v2/USER'))


    tasks.append(('EmbeddingMuMuB', '/EmbeddingRun2016B/MuonEmbedding-imputSep16DoubleMu_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuMuC', '/EmbeddingRun2016C/MuonEmbedding-imputSep16DoubleMu_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuMuD', '/EmbeddingRun2016D/MuonEmbedding-imputSep16DoubleMu_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuMuE', '/EmbeddingRun2016E/MuonEmbedding-imputSep16DoubleMu_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuMuF', '/EmbeddingRun2016F/MuonEmbedding-imputSep16DoubleMu_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuMuG', '/EmbeddingRun2016G/MuonEmbedding-imputSep16DoubleMu_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuMuH', '/EmbeddingRun2016H/MuonEmbedding-imputPromptDoubleMu_miniAOD-v2/USER'))

    tasks.append(('EmbeddingElElB', '/EmbeddingRun2016B/ElectronEmbedding-imputDoubleMu_mirror_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElElC', '/EmbeddingRun2016C/ElectronEmbedding-imputDoubleMu_mirror_miniAOD-v2/USER'))
    tasks.append(('EmbeddingElElD', '/EmbeddingRun2016D/ElectronEmbedding-imputDoubleMu_mirror_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElElE', '/EmbeddingRun2016E/ElectronEmbedding-imputDoubleMu_mirror_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElElF', '/EmbeddingRun2016F/ElectronEmbedding-imputDoubleMu_mirror_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElElG', '/EmbeddingRun2016G/ElectronEmbedding-imputDoubleMu_mirror_miniAOD-v5/USER'))
    tasks.append(('EmbeddingElElH', '/EmbeddingRun2016H/ElectronEmbedding-imputDoubleMu_mirror_miniAOD-v5/USER'))


    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        if "Run2016H" in task[1]:
            config.JobType.pyCfgParams = ['release=80XMINIAOD','isData=1','globalTag=80X_dataRun2_Prompt_v16']
        else:
            config.JobType.pyCfgParams = ['release=80XMINIAOD','isData=1','globalTag=80X_dataRun2_2016SeptRepro_v7']
        #submit(config)
        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



