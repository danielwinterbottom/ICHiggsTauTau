from CRABClient.UserUtilities import config

config = config()

config.General.transferOutputs = True
config.General.workArea='May24_MC_102X'

config.JobType.psetName = 'higgstautau_cfg_102X_Nov18.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.maxMemoryMB = 2500
CfgParams = ['isData=0', 'globalTag=102X_upgrade2018_realistic_v18']#previous v15
config.JobType.allowUndistributedCMSSW = True
# for deep tau need the next line
# config.JobType.scriptExe = 'submitCrabDeepTau.sh'

# config.Data.unitsPerJob = 240 # for automatic
# config.Data.splitting = 'Automatic'
config.Data.unitsPerJob = 100000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.outLFNDirBase='/store/user/adow/May24_MC_102X/'
config.Data.allowNonValidInputDataset = True
# config.Data.inputDBS = 'phys03'

config.Site.storageSite = 'T2_UK_London_IC'

# config.Debug.extraJDL = [
#     '+DESIRED_Sites="T3_UK_Opportunistic"',
#     '+JOB_CMSSite="T3_UK_Opportunistic"',
#     '+AccountingGroup="highprio.adowot"'
# ]

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

    # tasks.append(('DYJetsToLL', '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL_M-10-50-LO', '/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'))
    tasks.append(('DY1JetsToLL-LO', '/DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'))
    tasks.append(('DY2JetsToLL-LO', '/DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'))
    tasks.append(('DY3JetsToLL-LO', '/DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'))
    tasks.append(('DY4JetsToLL-LO', '/DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-LO', '/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))

    tasks.append(('EWKZ2Jets', '/EWKZ2Jets_ZToLL_M-50_TuneCP5_PSweights_13TeV-madgraph-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM')) #PSweights?
    tasks.append(('EWKWMinus2Jets', '/EWKWMinus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))
    tasks.append(('EWKWPlus2Jets', '/EWKWPlus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))

    tasks.append(('W1JetsToLNu-LO', '/W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'))
    tasks.append(('W2JetsToLNu-LO', '/W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'))
    tasks.append(('W3JetsToLNu-LO', '/W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'))
    tasks.append(('W4JetsToLNu-LO', '/W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'))
    tasks.append(('WJetsToLNu-LO', '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM')) #not complete

    tasks.append(('TTTo2L2Nu', '/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))
    tasks.append(('TTToHadronic', '/TTToHadronic_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))
    tasks.append(('TTToSemiLeptonic', '/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))

    # tasks.append(('WZTo1L1Nu2Q', '/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM')) #not found
    tasks.append(('WZTo2L2Q', '/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))
    tasks.append(('WZTo3LNu', '/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))
    tasks.append(('WZTo3LNu-ext1', '/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM'))
    tasks.append(('WZTo1L3Nu', '/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))

    tasks.append(('WWTo1L1Nu2Q', '/WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))
    # tasks.append(('WWToLNuQQ', '/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))
    tasks.append(('WWTo2L2Nu', '/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))
    # tasks.append(('WWTo4Q', '/WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM')) # don't need

    tasks.append(('ZZTo2L2Nu-ext1', '/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM'))
    tasks.append(('ZZTo2L2Nu-ext2', '/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext2-v2/MINIAODSIM'))
    tasks.append(('ZZTo2L2Q', '/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))
    tasks.append(('ZZTo4L', '/ZZTo4L_TuneCP5_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM'))
    tasks.append(('ZZTo4L-ext', '/ZZTo4L_TuneCP5_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext2-v2/MINIAODSIM'))

    # tasks.append(('WGToLNuG_01J_5f-ext1','/WGToLNuG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1/MINIAODSIM')) #don't use
    tasks.append(('WGToLNuG','/WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))
    tasks.append(('Tbar-tW-ext1', '/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1/MINIAODSIM'))
    tasks.append(('T-tW-ext1', '/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1/MINIAODSIM'))
    tasks.append(('Tbar-t', '/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))
    tasks.append(('T-t', '/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'))

    tasks.append(('GluGluHToTauTau_M-125', '/GluGluHToTauTau_M125_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'))
    tasks.append(('VBFHToTauTau_M-125-ext1', '/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1/MINIAODSIM'))
    tasks.append(('ZHToTauTau_M-125', '/ZHToTauTau_M125_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'))
    tasks.append(('WminusHToTauTau_M-125', '/WminusHToTauTau_M125_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'))
    tasks.append(('WplusHToTauTau_M-125', '/WplusHToTauTau_M125_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'))

    ########

    # CP in jets 2017 samples
    tasks.append(('GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX', '/GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX', '/GluGluToMaxmixHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX', '/GluGluToHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('GluGluToHToTauTau_M125_amcatnloFXFX', '/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    # private ones
    tasks.append(('GluGluToPseudoscalarHToTauTau_M125_amcatnloFXFX', '/GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/dwinterb-GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-MiniAOD-v2-5f646ecd4e1c7a39ab0ed099ff55ceb9/USER'))
    tasks.append(('GluGluToMaxmixHToTauTau_M125_amcatnloFXFX', '/GluGluToMaxmixHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/dwinterb-GluGluToMaxmixHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-MiniAOD-v2-5f646ecd4e1c7a39ab0ed099ff55ceb9/USER'))

    # JHU Gen for VBF, VH
    # tasks.append(('VBFHiggs0PMToTauTau', '/VBFHiggs0PMToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    # tasks.append(('VBFHiggs0MToTauTau', '/VBFHiggs0MToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    # tasks.append(('VBFHiggs0Mf05ph0ToTauTau', '/VBFHiggs0Mf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))

    # tasks.append(('ZHiggs0PMToTauTau', '/ZHiggs0PMToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    # tasks.append(('ZHiggs0MToTauTau', '/ZHiggs0MToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    # tasks.append(('ZHiggs0Mf05ph0ToTauTau', '/ZHiggs0Mf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))

    # tasks.append(('WHiggs0PMToTauTau', '/WHiggs0PMToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    # tasks.append(('WHiggs0MToTauTau', '/WHiggs0MToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3/MINIAODSIM'))
    # tasks.append(('WHiggs0Mf05ph0ToTauTau', '/WHiggs0Mf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))

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
            # config.Data.unitsPerJob = 240 # for automatic
            # config.Data.splitting = 'Automatic'

            config.Data.unitsPerJob = 100000
            config.Data.splitting = 'EventAwareLumiBased'

            # config.Data.splitting = 'FileBased'
            # config.Data.unitsPerJob = 1
            config.Data.userInputFiles = None

        if "HToTauTau" in task[0]:
            config.JobType.pyCfgParams = CfgParams + ['LHEWeights=True','doHT=1']
            if 'amcatnloFXFX' in task[0]:
                config.JobType.pyCfgParams = CfgParams + ['LHEWeights=True','includenpNLO=True','doHT=1']

        elif ("DY" in task[0] and "JetsToLL-LO" in task[0]) or ("W" in task[0] and "JetsToLNu-LO" in task[0]):
            config.JobType.pyCfgParams = CfgParams + ['doHT=1']

        elif "nospinner" in task[0]:
            config.JobType.pyCfgParams = CfgParams + ['LHEWeights=True','doHT=1' ,'tauSpinner=True']

        else:
            config.JobType.pyCfgParams = CfgParams + ['LHEWeights=False']

        print(config)
        submit(config)

