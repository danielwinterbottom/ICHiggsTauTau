from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Apr02_MC_80X_ReSub'
config.section_('JobType')
config.JobType.psetName = 'higgstautau_cfg_80X_Nov17_embedding.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['release=80XMINIAOD','isData=1', 'globalTag=80X_dataRun2_2016SeptRepro_v7']
config.section_('Data')
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 100000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.outLFNDirBase='/store/user/dwinterb/Apr02_MC_80X/'
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
    tasks.append(('EmbeddingMuTauC', '/EmbeddingRun2016C/MuTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuTauD', '/EmbeddingRun2016D/MuTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuTauE', '/EmbeddingRun2016E/MuTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    tasks.append(('EmbeddingMuTauF', '/EmbeddingRun2016F/MuTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingMuTauG', '/EmbeddingRun2016G/MuTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingMuTauH', '/EmbeddingRun2016H/MuTauFinalState-imputPromptDoubleMu_mirror_miniAOD-v2/USER'))

    #tasks.append(('EmbeddingElTauB', '/EmbeddingRun2016B/ElTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingElTauC', '/EmbeddingRun2016C/ElTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingElTauD', '/EmbeddingRun2016D/ElTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    tasks.append(('EmbeddingElTauE', '/EmbeddingRun2016E/ElTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingElTauF', '/EmbeddingRun2016F/ElTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    tasks.append(('EmbeddingElTauG', '/EmbeddingRun2016G/ElTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingElTauH', '/EmbeddingRun2016H/ElTauFinalState-imputPromptDoubleMu_mirror_miniAOD-v2/USER'))

    #tasks.append(('EmbeddingElMuB', '/EmbeddingRun2016B/ElMuFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingElMuC', '/EmbeddingRun2016C/ElMuFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingElMuD', '/EmbeddingRun2016D/ElMuFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingElMuE', '/EmbeddingRun2016E/ElMuFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingElMuF', '/EmbeddingRun2016F/ElMuFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingElMuG', '/EmbeddingRun2016G/ElMuFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingElMuH', '/EmbeddingRun2016H/ElMuFinalState-imputPromptDoubleMu_mirror_miniAOD-v4/USER'))

    #tasks.append(('EmbeddingTauTauB', '/EmbeddingRun2016B/TauTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingTauTauC', '/EmbeddingRun2016C/TauTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingTauTauD', '/EmbeddingRun2016D/TauTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingTauTauE', '/EmbeddingRun2016E/TauTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingTauTauF', '/EmbeddingRun2016F/TauTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingTauTauG', '/EmbeddingRun2016G/TauTauFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER'))
    #tasks.append(('EmbeddingTauTauH', '/EmbeddingRun2016H/TauTauFinalState-imputPromptDoubleMu_mirror_miniAOD-v3/USER'))    

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



