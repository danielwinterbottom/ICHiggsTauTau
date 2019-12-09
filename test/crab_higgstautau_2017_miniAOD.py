from CRABClient.UserUtilities import config

config = config()

config.General.transferOutputs = True
config.General.workArea='Sep29_MC_102X_2017'

config.JobType.psetName = 'higgstautau_cfg_102X_Aug19_2017.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
#config.JobType.maxMemoryMB = 2500
cfgParams = ['release=102XMINIAOD','isData=0', 'globalTag=102X_mc2017_realistic_v6','doHT=1']
config.JobType.allowUndistributedCMSSW = True

config.Data.unitsPerJob = 100000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.outLFNDirBase='/store/user/adow/{}/'.format(config.General.workArea)
config.Data.allowNonValidInputDataset = True
# config.Data.inputDBS = 'phys03'
config.Data.ignoreLocality = True
config.Site.whitelist   = ['T2_*','T1_*','T3_*']

config.Site.storageSite = 'T2_UK_London_IC'

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

    tasks.append(('DYJetsToLL', '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-ext', '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM'))

    tasks.append(('DYJetsToLL_M-10-50-LO', '/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL_M-10-50-LO-ext1', '/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'))

    tasks.append(('DY2JetsToLL-LO', '/DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
   
    tasks.append(('DYJetsToLL-LO', '/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017RECOSIMstep_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-LO-ext1', '/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017RECOSIMstep_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM'))
    tasks.append(('EWKZ2Jets', '/EWKZ2Jets_ZToLL_M-50_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('EWKWMinus2Jets', '/EWKWMinus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('EWKWPlus2Jets', '/EWKWPlus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('W1JetsToLNu-LO', '/W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3/MINIAODSIM'))
    tasks.append(('W2JetsToLNu-LO', '/W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v4/MINIAODSIM'))
    tasks.append(('W3JetsToLNu-LO', '/W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-LO', '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('WJetsToLNu-LO-ext', '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'))
    tasks.append(('WZTo1L1Nu2Q', '/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('WZTo2L2Q', '/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('WZTo1L3Nu', '/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8_v2/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('WWTo1L1Nu2Q', '/WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('WWTo2L2Nu', '/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('WWToLNuQQ-ext', '/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM'))
    tasks.append(('ZZTo2L2Nu', '/ZZTo2L2Nu_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('ZZTo2L2Q', '/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('ZZTo4L-ext', '/ZZTo4L_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM'))
    tasks.append(('VVTo2L2Nu', '/VVTo2L2Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('WGToLNuG_01J_5f','/WGToLNuG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('WGToLNuG','/WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('Tbar-tW', '/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('T-tW', '/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('Tbar-t', '/ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('T-t', '/ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))

    tasks.append(('GluGluHToTauTau_M-125-ext', '/GluGluHToTauTau_M125_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM'))
    tasks.append(('ZHToTauTau_M-125', '/ZHToTauTau_M125_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('WminusHToTauTau_M-125', '/WminusHToTauTau_M125_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('WplusHToTauTau_M-125', '/WplusHToTauTau_M125_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))

    tasks.append(('GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX', '/GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX', '/GluGluToMaxmixHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX', '/GluGluToHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('GluGluToHToTauTau_M125_amcatnloFXFX', '/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))

    tasks.append(('DY1JetsToLL-LO', '/DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM')) #new pmx
    tasks.append(('DY1JetsToLL-LO-ext', '/DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_v3_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM')) #new 12Apr2018_v3
    tasks.append(('TTTo2L2Nu', '/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM')) #new pmx
    tasks.append(('TTToHadronic', '/TTToHadronic_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/MINIAODSIM')) # new pmx and v2
    tasks.append(('TTToSemiLeptonic', '/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM')) # new pmx
    tasks.append(('WZTo3LNu', '/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM')) # new pmx
    tasks.append(('WWToLNuQQ', '/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM')) # new pmx
    tasks.append(('ZZTo4L', '/ZZTo4L_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM')) # new pmx
    tasks.append(('DY4JetsToLL-LO', '/DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_v2_94X_mc2017_realistic_v14-v2/MINIAODSIM')) #new 12Apr2018_v2

    tasks.append(('DY2JetsToLL-LO-ext', '/DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM')) # new pmx
    tasks.append(('DY3JetsToLL-LO-ext', '/DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM')) # new pmx

    tasks.append(('VBFHToTauTau_M-125-nospinner','/VBFHToMaxmixTauTau_M125_13TeV_powheg_pythia8_2017-GEN_TEST07Jan19/adow-StoreResults_VBFHToTauTauNoSpin_M125_13TeV_pythia8_2017_MINIAOD-v1/USER'))
    tasks.append(('GluGluToHToTauTau_M-125-nospinner','/GluGluHToPseudoscalarTauTau_M125_13TeV_powheg_pythia8_2017-GEN_TEST07Jan19/adow-StoreResults_GluGluToHToTauTauNoSpin_M125_13TeV_pythia8_2017_MINIAOD-v1/USER'))
    tasks.append(('GluGluToHToTauTau_M-125-nospinner-filter','/GluGluHToTauTau_M125_13TeV_powheg_pythia8_nospinner-filter-v2/dwinterb-StoreResults_GluGluHToTauTau_M125_13TeV_powheg_pythia8_nospinner_filter_v2_miniA-v1/USER'))
    tasks.append(('VBFHToTauTau_M-125-nospinner-filter','/VBFHToTauTau_M125_13TeV_powheg_pythia8_nospinner-filter-v2/dwinterb-StoreResults_VBFHToTauTau_M125_13TeV_powheg_pythia8_nospinner_filter_v2_miniAOD-v1/USER'))
    tasks.append(('VBFHToTauTau_M-125-PS-filter','/VBFHToTauTau_M125_13TeV_powheg_pythia8_PS-filter-v3/dwinterb-StoreResults_VBFHToTauTau_M125_13TeV_powheg_pythia8_PS_filter_v3_miniAOD-v1/USER'))
    tasks.append(('VBFHToTauTau_M-125-SM-filter','/VBFHToTauTau_M125_13TeV_powheg_pythia8_SM-filter-v2/dwinterb-StoreResults_VBFHToTauTau_M125_13TeV_powheg_pythia8_SM_filter_v2_miniAOD-v1/USER'))

    # private samples
    tasks.append(('VBFHToTauTau_M-125-MM-filter','/VBFHToTauTau_M125_13TeV_powheg_pythia8_MM-filter-v2/dwinterb-StoreResults_VBFHToTauTau_M125_13TeV_powheg_pythia8_MM_filter_v2_miniAOD-v1/USER'))
    tasks.append(('GluGluToPseudoscalarHToTauTau_M125_amcatnloFXFX', '/GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/dwinterb-StoreResults_GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017_-v1/USER'))
    tasks.append(('GluGluToMaxmixHToTauTau_M125_amcatnloFXFX', '/GluGluToMaxmixHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/dwinterb-StoreResults_GluGluToMaxmixHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017_MiniAO-v1/USER'))

    # adding these ones Sep25 2019 - to submit
    tasks.append(('DY3JetsToLL-LO', '/DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM')) # missed this one here
    tasks.append(('W4JetsToLNu-LO', '/W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/MINIAODSIM')) # new pmx
    tasks.append(('VBFHToTauTau_M-125', '/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM')) # new pmx
    tasks.append(('GluGluHToTauTau_M-125', '/GluGluHToTauTau_M125_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/MINIAODSIM')) # new pmx

    tasks.append(('VBFHToTauTauUncorrelatedDecay_Filtered','/VBFHToTauTauUncorrelatedDecay_Filtered_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('WminusHToTauTauUncorrelatedDecay_Filtered','/WminusHToTauTauUncorrelatedDecay_Filtered_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('WplusHToTauTauUncorrelatedDecay_Filtered','/WplusHToTauTauUncorrelatedDecay_Filtered_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('ZHToTauTauUncorrelatedDecay_Filtered','/ZHToTauTauUncorrelatedDecay_Filtered_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('GluGluHToTauTauUncorrelatedDecay_Filtered','/GluGluHToTauTauUncorrelatedDecay_Filtered_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('GluGluHToTauTauUncorrelatedDecay','/GluGluHToTauTauUncorrelatedDecay_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))

    tasks.append(('JJH0MToTauTauPlusOneJets_Filtered','/JJH0MToTauTauPlusOneJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('JJH0MToTauTauPlusTwoJets_Filtered','/JJH0MToTauTauPlusTwoJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('JJH0MToTauTauPlusZeroJets_Filtered','/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('JJH0Mf05ph0ToTauTauPlusOneJets_Filtered','/JJH0Mf05ph0ToTauTauPlusOneJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('JJH0Mf05ph0ToTauTauPlusZeroJets_Filtered','/JJH0Mf05ph0ToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('JJH0PMToTauTauPlusOneJets_Filtered','/JJH0PMToTauTauPlusOneJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('JJH0PMToTauTauPlusZeroJets_Filtered','/JJH0PMToTauTauPlusZeroJets_Filtered_M125_TuneCP5_13TeV-mcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))

    tasks.append(('JJHiggs0MToTauTau','/JJHiggs0MToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('JJHiggs0Mf05ph0ToTauTau','/JJHiggs0Mf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('JJHiggs0PMToTauTau','/JJHiggs0PMToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAOD-PU2017_94X_mc2017_realistic_v11-v1/MINIAODSIM'))
    tasks.append(('JJHiggs0PMToTauTau','/JJHiggs0PMToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('VBFHiggs0L1ToTauTau','/VBFHiggs0L1ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('VBFHiggs0L1ZgToTauTau','/VBFHiggs0L1ZgToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('VBFHiggs0L1Zgf05ph0ToTauTau','/VBFHiggs0L1Zgf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('VBFHiggs0L1f05ph0ToTauTau','/VBFHiggs0L1f05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('VBFHiggs0MToTauTau','/VBFHiggs0MToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('VBFHiggs0Mf05ph0ToTauTau','/VBFHiggs0Mf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAOD-PU2017_94X_mc2017_realistic_v11-v1/MINIAODSIM'))
    tasks.append(('VBFHiggs0Mf05ph0ToTauTau','/VBFHiggs0Mf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('VBFHiggs0PHToTauTau','/VBFHiggs0PHToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('VBFHiggs0PHf05ph0ToTauTau','/VBFHiggs0PHf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('VBFHiggs0PMToTauTau','/VBFHiggs0PMToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('WHiggs0L1ToTauTau','/WHiggs0L1ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('WHiggs0L1f05ph0ToTauTau','/WHiggs0L1f05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('WHiggs0MToTauTau','/WHiggs0MToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('WHiggs0MToTauTau','/WHiggs0MToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3/MINIAODSIM'))
    tasks.append(('WHiggs0Mf05ph0ToTauTau','/WHiggs0Mf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('WHiggs0PHToTauTau','/WHiggs0PHToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('WHiggs0PHToTauTau','/WHiggs0PHToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('WHiggs0PHf05ph0ToTauTau','/WHiggs0PHf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('WHiggs0PHf05ph0ToTauTau','/WHiggs0PHf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3/MINIAODSIM'))
    tasks.append(('WHiggs0PMToTauTau','/WHiggs0PMToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('ZHiggs0L1ToTauTau','/ZHiggs0L1ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('ZHiggs0L1ZgToTauTau','/ZHiggs0L1ZgToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('ZHiggs0L1ZgToTauTau','/ZHiggs0L1ZgToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3/MINIAODSIM'))
    tasks.append(('ZHiggs0L1ZgToTauTau','/ZHiggs0L1ZgToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v4/MINIAODSIM'))
    tasks.append(('ZHiggs0L1Zgf05ph0ToTauTau','/ZHiggs0L1Zgf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAOD-PU2017_94X_mc2017_realistic_v11-v1/MINIAODSIM'))
    tasks.append(('ZHiggs0L1Zgf05ph0ToTauTau','/ZHiggs0L1Zgf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('ZHiggs0L1f05ph0ToTauTau','/ZHiggs0L1f05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('ZHiggs0MToTauTau','/ZHiggs0MToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('ZHiggs0Mf05ph0ToTauTau','/ZHiggs0Mf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('ZHiggs0PHToTauTau','/ZHiggs0PHToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('ZHiggs0PHf05ph0ToTauTau','/ZHiggs0PHf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAOD-PU2017_94X_mc2017_realistic_v11-v1/MINIAODSIM'))
    tasks.append(('ZHiggs0PHf05ph0ToTauTau','/ZHiggs0PHf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('ZHiggs0PMToTauTau','/ZHiggs0PMToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAOD-PU2017_94X_mc2017_realistic_v11-v1/MINIAODSIM'))
    tasks.append(('ZHiggs0PMToTauTau','/ZHiggs0PMToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('ttHiggs0MToTauTau','/ttHiggs0MToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('ttHiggs0Mf05ph0ToTauTau','/ttHiggs0Mf05ph0ToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))
    tasks.append(('ttHiggs0PMToTauTau','/ttHiggs0PMToTauTau_M125_13TeV_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'))

    tasks.append(('GluGluHToWWTo2L2Nu_M-125', '/GluGluHToWWTo2L2Nu_M125_13TeV_powheg2_JHUGenV714_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))
    tasks.append(('VBFHToWWTo2L2Nu_M-125', '/VBFHToWWTo2L2Nu_M125_13TeV_powheg2_JHUGenV714_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'))

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
                config.JobType.pyCfgParams = cfgParams + ['LHEWeights=True','includenpNLO=True']
            else: 
                config.JobType.pyCfgParams = cfgParams + ['LHEWeights=True','tauSpinner=True']
        else: 
            config.JobType.pyCfgParams = cfgParams 

        #if 'VBFHToTauTau_M-125-MM-filter' in task[0] or 'GluGluToPseudoscalarHToTauTau_M125_amcatnloFXFX' in task[0] or 'GluGluToMaxmixHToTauTau_M125_amcatnloFXFX' in task[0]:
        #  config.Data.ignoreLocality= True
        #  config.Data.inputDBS = 'phys03'
        #else:
        config.Data.ignoreLocality = True
        config.Data.inputDBS = 'global'

        print config.Data.unitsPerJob
        print config.Data.splitting

        print(config)
        submit(config)

