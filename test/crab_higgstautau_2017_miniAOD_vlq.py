from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsernameFromCRIC
from multiprocessing import Process

config = config()

config.General.transferOutputs = True
config.General.workArea='Aug17_MC_102X_2017'

config.JobType.psetName = 'higgstautau_cfg_102X_Aug19_2017.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.maxMemoryMB = 4000
cfgParams = ['isData=0', 'globalTag=102X_mc2017_realistic_v7','doHT=0','LHEWeights=1']
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
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched', '/betaRd33_0_mU1000_gU1_matched_GENSIM_2017/guttley-betaRd33_0_mU1000_gU1_matched_2017_MINIAOD-b63beb1ae05c0e254c43785544367ee5/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_xqcut_down', '/betaRd33_0_mU1000_gU1_matched_xqcut_down_GENSIM_2017/guttley-betaRd33_0_mU1000_gU1_matched_xqcut_down_2017_MINIAOD-b63beb1ae05c0e254c43785544367ee5/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_xqcut_up', '/betaRd33_0_mU1000_gU1_matched_xqcut_up_GENSIM_2017/guttley-betaRd33_0_mU1000_gU1_matched_xqcut_up_2017_MINIAOD-b63beb1ae05c0e254c43785544367ee5/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU1000_gU1', '/betaRd33_0_mU1000_gU1_GENSIM_2017/guttley-betaRd33_0_mU1000_gU1_2017_MINIAOD-b63beb1ae05c0e254c43785544367ee5/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU2000_gU1_matched', '/betaRd33_0_mU2000_gU1_matched_GENSIM_2017/guttley-betaRd33_0_mU2000_gU1_matched_2017_MINIAOD-b63beb1ae05c0e254c43785544367ee5/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU3000_gU1_matched', '/betaRd33_0_mU3000_gU1_matched_GENSIM_2017/guttley-betaRd33_0_mU3000_gU1_matched_2017_MINIAOD-b63beb1ae05c0e254c43785544367ee5/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU4000_gU1_matched', '/betaRd33_0_mU4000_gU1_matched_GENSIM_2017/guttley-betaRd33_0_mU4000_gU1_matched_2017_MINIAOD-b63beb1ae05c0e254c43785544367ee5/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU5000_gU1_matched', '/betaRd33_0_mU5000_gU1_matched_GENSIM_2017/guttley-betaRd33_0_mU5000_gU1_matched_2017_MINIAOD-b63beb1ae05c0e254c43785544367ee5/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_0_mU500_gU1_matched', '/betaRd33_0_mU500_gU1_matched_GENSIM_2017/guttley-betaRd33_0_mU500_gU1_matched_2017_MINIAOD-b63beb1ae05c0e254c43785544367ee5/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_minus1_mU1000_gU1_matched', '/betaRd33_minus1_mU1000_gU1_matched_GENSIM_2017/guttley-betaRd33_minus1_mU1000_gU1_matched_2017_MINIAOD-b63beb1ae05c0e254c43785544367ee5/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_minus1_mU2000_gU1_matched', '/betaRd33_minus1_mU2000_gU1_matched_GENSIM_2017/guttley-betaRd33_minus1_mU2000_gU1_matched_2017_MINIAOD-b63beb1ae05c0e254c43785544367ee5/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_minus1_mU3000_gU1_matched', '/betaRd33_minus1_mU3000_gU1_matched_GENSIM_2017/guttley-betaRd33_minus1_mU3000_gU1_matched_2017_MINIAOD-b63beb1ae05c0e254c43785544367ee5/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_minus1_mU4000_gU1_matched', '/betaRd33_minus1_mU4000_gU1_matched_GENSIM_2017/guttley-betaRd33_minus1_mU4000_gU1_matched_2017_MINIAOD-b63beb1ae05c0e254c43785544367ee5/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_minus1_mU5000_gU1_matched', '/betaRd33_minus1_mU5000_gU1_matched_GENSIM_2017/guttley-betaRd33_minus1_mU5000_gU1_matched_2017_MINIAOD-b63beb1ae05c0e254c43785544367ee5/USER'))
    tasks.append(('VectorLQToTauTau_betaRd33_minus1_mU500_gU1_matched', '/betaRd33_minus1_mU500_gU1_matched_GENSIM_2017/guttley-betaRd33_minus1_mU500_gU1_matched_2017_MINIAOD-b63beb1ae05c0e254c43785544367ee5/USER'))

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

