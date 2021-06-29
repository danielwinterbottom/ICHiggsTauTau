from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsernameFromCRIC
from multiprocessing import Process

config = config()

config.General.transferOutputs = True
config.General.workArea='Jun28_MC_102X_2018'

config.JobType.psetName = 'higgstautau_cfg_102X_Aug19_2018.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.maxMemoryMB = 4000
cfgParams = ['isData=0', 'globalTag=102X_upgrade2018_realistic_v20','doHT=0']
config.JobType.allowUndistributedCMSSW = True
#config.Data.unitsPerJob = 1
#config.Data.splitting = 'FileBased'
config.Data.outLFNDirBase='/store/user/{}/{}/'.format(getUsernameFromCRIC(), config.General.workArea)
config.Data.unitsPerJob = 50000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.allowNonValidInputDataset = True
config.Data.inputDBS = 'phys03'
config.Data.ignoreLocality = True


config.Site.whitelist   = ['T2_*','T1_*','T3_*']
config.Site.storageSite = 'T2_UK_London_IC'

if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand
    from httplib import HTTPException
    from CRABClient.ClientExceptions import ClientException

    # We want to put all the CRAB project directories from the tasks we submit here into one common directory.
    # That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException as hte:
            print(hte.headers)
        except ClientException as cle:
            print(cle)

    #############################################################################################
    ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
    #############################################################################################

    tasks=list()

    tasks.append(('VectorLQToTauTau_betaRd33_0_mU2_gU1', '/betaRd33_0_mU2_gU1_GENSIM_2018/guttley-betaRd33_0_mU2_gU1_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU2_gU2', '/betaRd33_0_mU2_gU2_GENSIM_2018/guttley-betaRd33_0_mU2_gU2_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU2_gU3', '/betaRd33_0_mU2_gU3_GENSIM_2018/guttley-betaRd33_0_mU2_gU3_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU3_gU1', '/betaRd33_0_mU3_gU1_GENSIM_2018/guttley-betaRd33_0_mU3_gU1_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU3_gU2', '/betaRd33_0_mU3_gU2_GENSIM_2018/guttley-betaRd33_0_mU3_gU2_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU3_gU3', '/betaRd33_0_mU3_gU3_GENSIM_2018/guttley-betaRd33_0_mU3_gU3_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU4_gU1', '/betaRd33_0_mU4_gU1_GENSIM_2018/guttley-betaRd33_0_mU4_gU1_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU4_gU2', '/betaRd33_0_mU4_gU2_GENSIM_2018/guttley-betaRd33_0_mU4_gU2_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU4_gU3', '/betaRd33_0_mU4_gU3_GENSIM_2018/guttley-betaRd33_0_mU4_gU3_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_minus1_mU2_gU1', '/betaRd33_minus1_mU2_gU1_GENSIM_2018/guttley-betaRd33_minus1_mU2_gU1_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_minus1_mU2_gU2', '/betaRd33_minus1_mU2_gU2_GENSIM_2018/guttley-betaRd33_minus1_mU2_gU2_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_minus1_mU2_gU3', '/betaRd33_minus1_mU2_gU3_GENSIM_2018/guttley-betaRd33_minus1_mU2_gU3_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_minus1_mU3_gU1', '/betaRd33_minus1_mU3_gU1_GENSIM_2018/guttley-betaRd33_minus1_mU3_gU1_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_minus1_mU3_gU2', '/betaRd33_minus1_mU3_gU2_GENSIM_2018/guttley-betaRd33_minus1_mU3_gU2_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_minus1_mU3_gU3', '/betaRd33_minus1_mU3_gU3_GENSIM_2018/guttley-betaRd33_minus1_mU3_gU3_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_minus1_mU4_gU1', '/betaRd33_minus1_mU4_gU1_GENSIM_2018/guttley-betaRd33_minus1_mU4_gU1_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_minus1_mU4_gU2', '/betaRd33_minus1_mU4_gU2_GENSIM_2018/guttley-betaRd33_minus1_mU4_gU2_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_minus1_mU4_gU3', '/betaRd33_minus1_mU4_gU3_GENSIM_2018/guttley-betaRd33_minus1_mU4_gU3_2018_MINIAOD-0bd58594e6ade05f64e0c3a8301c3139/USER'))

    for task in tasks:
        print(task[0])
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]

        config.Data.splitting = 'FileBased'
        config.Data.unitsPerJob = 1
        config.JobType.pyCfgParams = cfgParams

        config.Data.userInputFiles = None

        print(config)

        p = Process(target=submit, args=(config,))
        p.start()
        p.join()

