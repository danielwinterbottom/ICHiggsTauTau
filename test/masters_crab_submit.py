from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsernameFromCRIC
from multiprocessing import Process

config = config()

config.General.transferOutputs = True
config.General.workArea='May21_MC_102X_2018'

config.JobType.psetName = 'masters_gen_ntuple_cfg.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
cfgParams = []
config.JobType.allowUndistributedCMSSW = True
config.Data.unitsPerJob = 100000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.outLFNDirBase='/store/user/{}/{}/'.format(getUsernameFromCRIC(), config.General.workArea)
config.Data.allowNonValidInputDataset = True
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
        except HTTPException, hte:
            print hte.headers
        except ClientException as cle:
            print cle

    #############################################################################################
    ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
    #############################################################################################

    tasks=list()

    tasks.append(('GluGluHToTauTauUncorrelatedDecay_Filtered_2018','/GluGluHToTauTauUncorrelatedDecay_Filtered_M125_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))
    tasks.append(('VBFHToTauTauUncorrelatedDecay_Filtered_2018','/VBFHToTauTauUncorrelatedDecay_Filtered_M125_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))

    tasks.append(('GluGluHToTauTauUncorrelatedDecay_Filtered_2017','/GluGluHToTauTauUncorrelatedDecay_Filtered_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('VBFHToTauTauUncorrelatedDecay_Filtered_2017','/VBFHToTauTauUncorrelatedDecay_Filtered_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))

    tasks.append(('GluGluHToTauTauUncorrelatedDecay_Filtered_2016','/GluGluHToTauTauUncorrelatedDecay_Filtered_M125_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('VBFHToTauTauUncorrelatedDecay_Filtered_2016','/VBFHToTauTauUncorrelatedDecay_Filtered_M125_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))


    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]

        if "DY4JetsToLL-LO" in task[0]:
            # talk to DBS to get list of files in this dataset
            from dbs.apis.dbsClient import DbsApi
            dbs = DbsApi('https://cmsweb.cern.ch/dbs/prod/global/DBSReader')

            dataset = task[1]
            fileDictList=dbs.listFiles(dataset=dataset)

            print ("dataset %s has %d files" % (dataset, len(fileDictList)))

            # DBS client returns a list of dictionaries, but we want a list of Logical File Names
            lfnList = [ dic['logical_file_name'] for dic in fileDictList ]
            config.Data.userInputFiles = lfnList
            config.Data.splitting = 'FileBased'
            config.Data.unitsPerJob = 1
            config.Data.inputDataset = None
        else:
            config.Data.inputDataset = task[1]

            #config.Data.unitsPerJob = 100000
            #config.Data.splitting = 'EventAwareLumiBased'
            config.Data.splitting = 'FileBased'
            config.Data.unitsPerJob = 1

            config.Data.userInputFiles = None

        if ("HToTauTau" in task[0] or 'JJH' in task[0]) and 'JHUGen' not in task[1]:
            if 'mcatnloFXFX' in task[0]:
                config.JobType.pyCfgParams = cfgParams + ['LHEWeights=True','includenpNLO=True','includeHTXS=True']
            else:
                config.JobType.pyCfgParams = cfgParams + ['LHEWeights=True','tauSpinner=True','includeHTXS=True']
        else:
            config.JobType.pyCfgParams = cfgParams

        print(config)

        p = Process(target=submit, args=(config,))
        p.start()
        p.join()

