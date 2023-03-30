import argparse
import os

#python3 crab_4tau_12_4.py --data --mc --output_folder Dec22 --year 2018
#cmsRun test/higgstautau_cfg_124X_Dec22_2018.py isData=0 globalTag=106X_upgrade2018_realistic_v15_L1v1 isEmbed=0
parser = argparse.ArgumentParser()
parser.add_argument('--output_folder','-o', help= 'Name of output directory', default='Jan24_Run3')
parser.add_argument('--year','-y', help= 'Name of input year', choices=["2022","all"], default='2022')
parser.add_argument('--data', help= 'Run data samples',  action='store_true')
parser.add_argument('--mc', help= 'Run mc samples',  action='store_true')
parser.add_argument('--signal', help= 'Run signal samples',  action='store_true')
parser.add_argument('--recovery', help= 'Do recovery jobs, make sure you run crab report on the samples you want to recover first',  action='store_true')
parser.add_argument('--only_run', help= 'Only run this file', default=None)
args = parser.parse_args()

dml = []
if args.data: dml.append("Data")
if args.mc: dml.append("MC")
if args.signal: dml.append("Signal")

if args.year == "all": yl = ["2022"]
else: yl = [args.year]

cfg = {
       "2022":"higgstautau_cfg_124X_Jan24_2022.py"
       }

gt = {
      "MC"  :{
              "2022":"124X_mcRun3_2022_realistic_postEE_v1" # check
              },
      "Signal":{
                "2022":"124X_mcRun3_2022_realistic_postEE_v1" # check
              },
      "Data":{
              "2022":"124X_mcRun3_2022_realistic_postEE_v1" # needs updating 
              }
      }

for dm in dml:
  for yr in yl:

    from CRABClient.UserUtilities import config
    from CRABClient.UserUtilities import getUsernameFromCRIC
    from multiprocessing import Process

    print("Processing {} for {}".format(yr, dm))    

    config = config()
    
    config.General.transferOutputs = True
    if not args.recovery:
      config.General.workArea='{}_{}_124X_{}'.format(args.output_folder,dm,yr)
    else:
      config.General.workArea='{}_{}_124X_{}_recovery'.format(args.output_folder,dm,yr)

    config.JobType.psetName = cfg[yr]
    config.JobType.pluginName = 'Analysis'
    config.JobType.outputFiles = ['EventTree.root']
    config.JobType.maxMemoryMB = 4000
    cfgParams = ['globalTag={}'.format(gt[dm][yr])]
    if dm == "Data": 
      cfgParams.append('isData=1')
    else: 
      cfgParams.append('isData=0')
    if dm == "Signal": config.Data.inputDBS = 'phys03'

    config.JobType.allowUndistributedCMSSW = True
    config.Data.outLFNDirBase='/store/user/{}/{}/'.format(getUsernameFromCRIC(), config.General.workArea)
    config.Data.publication = False
    config.Data.allowNonValidInputDataset = True
    config.Data.ignoreLocality = True
    
    config.Site.whitelist   = ['T2_*','T1_*','T3_*']
    config.Site.storageSite = 'T2_UK_London_IC'
    
    if __name__ == '__main__':
    
        from CRABAPI.RawCommand import crabCommand
        from http.client import HTTPException
        from CRABClient.ClientExceptions import ClientException
    
        # We want to put all the CRAB project directories from the tasks we submit here into one common directory.
        # That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).
    
        def submit(config):
            try:
                crabCommand('submit', config = config, dryrun = False)
            except HTTPException as hte:
                print(hte.headers)
            except ClientException as cle:
                print(cle)
    
        #############################################################################################
        ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
        #############################################################################################
    
        tasks=list()
    
        #if dm == "Data":
        #  if yr == "2022":

        if dm == "MC":
          if yr == "2022":
#            tasks.append(('DYToLL_M-50_PU70', '/DYToLL_M-50_TuneCP5_13p6TeV-pythia8/Run3Summer22EEMiniAODv3-Poisson70KeepRAW_124X_mcRun3_2022_realistic_postEE_v1-v1/MINIAODSIM'))
            tasks.append(('DYToLL_M-50_FlatPU0to70', '/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Winter22MiniAOD-FlatPU0to70_122X_mcRun3_2021_realistic_v9-v3/MINIAODSIM'))
            tasks.append(('DYToLL_M-50_FlatPU0to70-ext', '/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Winter22MiniAOD-FlatPU0to70_122X_mcRun3_2021_realistic_v9_ext1-v2/MINIAODSIM'))

#            tasks.append(('DYJetsToLL_M-50','/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Summer22MiniAODv3-forPOG_124X_mcRun3_2022_realistic_v12-v4/MINIAODSIM'))
#            tasks.append(('DYJetsToLL_M-50_EE','/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Summer22EEMiniAODv3-forPOG_124X_mcRun3_2022_realistic_postEE_v1-v3/MINIAODSIM'))

        #if dm == "Signal":
        #  if yr == "2022":
 
        for task in tasks:

            if not (args.only_run == None or args.only_run == task[0]): continue

            print(task[0])
            config.General.requestName = task[0]
            config.Data.inputDataset = task[1]

            if args.recovery:
              os.system("crab kill {}_{}_124X_{}/crab_{}".format(args.output_folder,dm,yr,task[0]))
              os.system("crab report {}_{}_124X_{}/crab_{}".format(args.output_folder,dm,yr,task[0]))
              config.Data.lumiMask = "{}_{}_124X_{}/crab_{}/results/notFinishedLumis.json".format(args.output_folder,dm,yr,task[0])    
            config.JobType.pyCfgParams = cfgParams
    
            config.Data.userInputFiles = None
            if task[0] == 'DYJetsToLL-LO' and yr == "2017":
              config.Data.splitting = 'FileBased'
              config.Data.unitsPerJob = 4
            elif dm == "Signal":
              config.Data.splitting = 'EventAwareLumiBased'
              config.Data.unitsPerJob = 5000
            elif (task[0] == 'SingleMuonD' or task[0] == 'EGammaD') and yr == "2018":
              config.Data.splitting = 'EventAwareLumiBased'
              config.Data.unitsPerJob = 100000
            elif (((task[0] == "DYJetsToLL_0J-NLO" or task[0] == "DYJetsToLL_1J-NLO") and yr == "2016-postVFP") or ((task[0] == "DYJetsToLL-NLO" or task[0] == "DYJetsToLL_1J-NLO") and yr == "2017") or (task[0] == "DYJetsToLL-NLO" and yr == "2018")):
              config.Data.splitting = 'EventAwareLumiBased'
              config.Data.unitsPerJob = 25000
            else:
              config.Data.splitting = 'EventAwareLumiBased'
              config.Data.unitsPerJob = 50000

            if args.recovery: config.Data.unitsPerJob = 10000

            print(config)
    
            p = Process(target=submit, args=(config,))
            p.start()
            p.join()
