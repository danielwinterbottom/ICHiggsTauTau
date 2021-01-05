from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsernameFromCRIC
from multiprocessing import Process
import os

config = config()

config.General.transferOutputs = True
config.General.workArea='Sep18_MC_102X_2017'

config.JobType.psetName = 'higgstautau_cfg_102X_Aug19_2017.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.maxMemoryMB = 4000
cfgParams = ['isData=0', 'globalTag=102X_mc2017_realistic_v7','doHT=0','LHEWeights=False']
config.JobType.allowUndistributedCMSSW = True

config.Data.unitsPerJob = 50000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.outLFNDirBase='/store/user/{}/{}/'.format(getUsernameFromCRIC(), config.General.workArea)
config.Data.allowNonValidInputDataset = True
# config.Data.inputDBS = 'phys03'
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

   # # ggH-LO
   # ggH_LO_masses = [80,90,100,110,120,130,140,180,200,250,300,350,400,450,600,700,800,900,1200,1400,1500,1600,1800,2000,2300,2600,2900,3200]
   # for mass in ggH_LO_masses:
   #     task_name = 'SUSYGluGluToHToTauTau_M-{}'.format(mass)
   #     file_location = '/SUSYGluGluToHToTauTau_M-{}_TuneCP5_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'.format(mass)
   #     tasks.append((task_name,file_location))

   # # bbH-LO
   # bbH_LO_masses = [80,90,100,110,120,130,140,160,180,200,250,300,350,400,450,600,700,800,900,1200,1400,1500,1600,1800,2000,2300,2600,2900,3200]
   # for mass in bbH_LO_masses:
   #     task_name = 'SUSYGluGluToBBHToTauTau_M-{}'.format(mass)
   #     file_location = '/SUSYGluGluToBBHToTauTau_M-{}_TuneCP5_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'.format(mass)
   #     tasks.append((task_name,file_location))

   # # bbH-NLO
   # bbH_NLO_masses = [80,90,110,120,125,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000,1200,1400,1600,1800,2000,2300,2600,2900,3200,3500]
   # for mass in bbH_NLO_masses:
   #     task_name = 'SUSYGluGluToBBHToTauTau_M-{}-NLO'.format(mass)
   #     file_location = '/SUSYGluGluToBBHToTauTau_M-{}_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'.format(mass)
   #     tasks.append((task_name,file_location))

    bbh_masses_ph     = [60, 80, 100, 120, 125, 130, 140, 160, 180, 200, 250, 300, 350, 400, 450, 500, 600, 700, 800, 900, 1000, 1200, 1400, 1600, 1800, 2000, 2300, 2600, 2900, 3200, 3500]
    bbh_masses_missing = [60, 350, 3500]

    for mass in bbh_masses_ph:
      if mass in bbh_masses_missing: continue
 
      tasks.append(('SUSYGluGluToBBHToTauTau_M-%(mass)s_powheg' % vars(),'/SUSYGluGluToBBHToTauTau_M%(mass)s_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM' % vars()))

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
          config.Data.unitsPerJob = 100000
          config.Data.splitting = 'EventAwareLumiBased'
          # config.Data.splitting = 'FileBased'
          # config.Data.unitsPerJob = 1
          config.Data.userInputFiles = None

        config.JobType.pyCfgParams = cfgParams

        config.Data.ignoreLocality = True
        config.Data.inputDBS = 'global'

        print config.Data.unitsPerJob
        print config.Data.splitting
        print config.JobType.pyCfgParams

        print(config)

        p = Process(target=submit, args=(config,))
        p.start()
        p.join()
