from CRABClient.UserUtilities import config

config = config()

config.General.transferOutputs = True
config.General.workArea='Jan06_MC_102X_2016'

config.JobType.psetName = 'higgstautau_cfg_102X_Aug19_Leg2016.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.maxMemoryMB = 2500
cfgParams = ['release=102XMINIAOD','isData=0' ,'doHT=1' , 'globalTag=102X_mcRun2_asymptotic_v7']
config.JobType.allowUndistributedCMSSW = True

#config.Data.unitsPerJob = 100000
#config.Data.splitting = 'EventAwareLumiBased'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.Data.outLFNDirBase='/store/user/mhassans/{}/'.format(config.General.workArea)
config.Data.allowNonValidInputDataset = True
# config.Data.inputDBS = 'phys03'
#config.Data.ignoreLocality= True
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

    tasks.append(('WJetsToLNu-LO', '/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('WJetsToLNu-LO-ext', '/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v2/MINIAODSIM'))
    tasks.append(('W1JetsToLNu-LO', '/W1JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('W2JetsToLNu-LO', '/W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('W2JetsToLNu-LO-ext', '/W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'))
    tasks.append(('W3JetsToLNu-LO', '/W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('W3JetsToLNu-LO-ext', '/W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'))
    tasks.append(('W4JetsToLNu-LO', '/W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('W4JetsToLNu-LO-ext1', '/W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'))
    tasks.append(('W4JetsToLNu-LO-ext2', '/W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v1/MINIAODSIM'))
    tasks.append(('WGToLNuG-ext1', '/WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1/MINIAODSIM'))
    tasks.append(('WGToLNuG-ext2', '/WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v1/MINIAODSIM'))
    tasks.append(('WGToLNuG-ext3', '/WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext3-v1/MINIAODSIM'))
    tasks.append(('WGstarToLNuMuMu', '/WGstarToLNuMuMu_012Jets_13TeV-madgraph/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('WGstarToLNuEE', '/WGstarToLNuEE_012Jets_13TeV-madgraph/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL', '/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-LO-ext1', '/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'))
    tasks.append(('DYJetsToLL-LO-ext2', '/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v2/MINIAODSIM'))
    tasks.append(('DY1JetsToLL-LO', '/DY1JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('DY2JetsToLL-LO', '/DY2JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('DY3JetsToLL-LO', '/DY3JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('DY4JetsToLL-LO', '/DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('DYJetsToLL_M-10-50-LO', '/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('VVTo2L2Nu', '/VVTo2L2Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('VVTo2L2Nu-ext1', '/VVTo2L2Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'))
    tasks.append(('ZZTo2L2Q', '/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('WWTo1L1Nu2Q', '/WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    #tasks.append(('WWToLNuQQ', '/WWToLNuQQ_13TeV-powheg/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    #tasks.append(('WWToLNuQQ-ext', '/WWToLNuQQ_13TeV-powheg/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'))
    tasks.append(('WZTo2L2Q', '/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('WZTo1L3Nu', '/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('WZTo1L1Nu2Q', '/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('WZJToLLLNu', '/WZJToLLLNu_TuneCUETP8M1_13TeV-amcnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    #tasks.append(('ZZTo4L', '/ZZTo4L_13TeV_powheg_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    #tasks.append(('QCDMuEnrichedPt15', '/QCD_Pt-20toInf_MuEnrichedPt15_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('Tbar-tW', '/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1/MINIAODSIM'))
    tasks.append(('T-tW', '/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1/MINIAODSIM'))
    tasks.append(('Tbar-t', '/ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('T-t', '/ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('TT', '/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu', '/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-ext', '/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v1/MINIAODSIM'))
    tasks.append(('EWKWMinus2Jets_WToLNu', '/EWKWMinus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('EWKWMinus2Jets_WToLNu-ext1', '/EWKWMinus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'))
    tasks.append(('EWKWMinus2Jets_WToLNu-ext2', '/EWKWMinus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v2/MINIAODSIM'))
    tasks.append(('EWKWPlus2Jets_WToLNu', '/EWKWPlus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('EWKWPlus2Jets_WToLNu-ext1', '/EWKWPlus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'))
    tasks.append(('EWKWPlus2Jets_WToLNu-ext2', '/EWKWPlus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v2/MINIAODSIM'))
    tasks.append(('EWKZ2Jets_ZToLL', '/EWKZ2Jets_ZToLL_M-50_13TeV-madgraph-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v3/MINIAODSIM'))
    tasks.append(('EWKZ2Jets_ZToLL-ext1', '/EWKZ2Jets_ZToLL_M-50_13TeV-madgraph-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'))
    tasks.append(('EWKZ2Jets_ZToLL-ext2', '/EWKZ2Jets_ZToLL_M-50_13TeV-madgraph-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v2/MINIAODSIM'))
    tasks.append(('EWKZ2Jets_ZToNuNu', '/EWKZ2Jets_ZToNuNu_13TeV-madgraph-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('EWKZ2Jets_ZToNuNu-ext1', '/EWKZ2Jets_ZToNuNu_13TeV-madgraph-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'))
    tasks.append(('EWKZ2Jets_ZToNuNu-ext2', '/EWKZ2Jets_ZToNuNu_13TeV-madgraph-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v2/MINIAODSIM'))
    tasks.append(('ZZTo4L-amcat', '/ZZTo4L_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'))
    tasks.append(('GluGluToHToTauTau_M-125', '/GluGluHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v3/MINIAODSIM'))
    tasks.append(('VBFHToTauTau_M-125', '/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('WplusHToTauTau_M-125', '/WplusHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('WminusHToTauTau_M-125', '/WminusHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('ZHToTauTau_M-125', '/ZHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('GluGluHToWWTo2L2Nu_M-125', '/GluGluHToWWTo2L2Nu_M125_13TeV_powheg_JHUgen_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('VBFHToWWTo2L2Nu_M-125', '/VBFHToWWTo2L2Nu_M125_13TeV_powheg_JHUgen_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))


    tasks.append(('GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX','/GluGluToHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8_tauola/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('GluGluToHToTauTau_M125_amcatnloFXFX','/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_tauola/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX','/GluGluToMaxmixHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8_tauola/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('GluGluToMaxmixHToTauTau_M125_amcatnloFXFX','/GluGluToMaxmixHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_tauola/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX','/GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8_tauola/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('GluGluToPseudoscalarHToTauTau_M125_amcatnloFXFX','/GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_tauola/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))

    tasks.append(('GluGluHToTauTauUncorrelatedDecay_Filtered','/GluGluHToTauTauUncorrelatedDecay_Filtered_M125_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('GluGluHToTauTauUncorrelatedDecay','/GluGluHToTauTauUncorrelatedDecay_M125_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('VBFHToTauTauUncorrelatedDecay_Filtered','/VBFHToTauTauUncorrelatedDecay_Filtered_M125_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('WminusHToTauTauUncorrelatedDecay_Filtered','/WminusHToTauTauUncorrelatedDecay_Filtered_M125_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('WminusHToTauTauUncorrelatedDecay','/WminusHToTauTauUncorrelatedDecay_M125_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('WplusHToTauTauUncorrelatedDecay_Filtered','/WplusHToTauTauUncorrelatedDecay_Filtered_M125_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('WplusHToTauTauUncorrelatedDecay','/WplusHToTauTauUncorrelatedDecay_M125_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('ZHToTauTauUncorrelatedDecay_Filtered','/ZHToTauTauUncorrelatedDecay_Filtered_M125_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('ZHToTauTauUncorrelatedDecay','/ZHToTauTauUncorrelatedDecay_M125_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0MToTauTauPlusOneJets_Filtered','/JJH0MToTauTauPlusOneJets_Filtered_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0MToTauTauPlusOneJets','/JJH0MToTauTauPlusOneJets_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0MToTauTauPlusTwoJets_Filtered','/JJH0MToTauTauPlusTwoJets_Filtered_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0MToTauTauPlusTwoJets','/JJH0MToTauTauPlusTwoJets_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0MToTauTauPlusZeroJets_Filtered','/JJH0MToTauTauPlusZeroJets_Filtered_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0MToTauTauPlusZeroJets','/JJH0MToTauTauPlusZeroJets_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0Mf05ph0ToTauTauPlusOneJets_Filtered','/JJH0Mf05ph0ToTauTauPlusOneJets_Filtered_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0Mf05ph0ToTauTauPlusOneJets','/JJH0Mf05ph0ToTauTauPlusOneJets_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0Mf05ph0ToTauTauPlusTwoJets_Filtered','/JJH0Mf05ph0ToTauTauPlusTwoJets_Filtered_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0Mf05ph0ToTauTauPlusTwoJets','/JJH0Mf05ph0ToTauTauPlusTwoJets_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0Mf05ph0ToTauTauPlusZeroJets_Filtered','/JJH0Mf05ph0ToTauTauPlusZeroJets_Filtered_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0Mf05ph0ToTauTauPlusZeroJets','/JJH0Mf05ph0ToTauTauPlusZeroJets_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0PMToTauTauPlusOneJets_Filtered','/JJH0PMToTauTauPlusOneJets_Filtered_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0PMToTauTauPlusOneJets','/JJH0PMToTauTauPlusOneJets_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0PMToTauTauPlusTwoJets_Filtered','/JJH0PMToTauTauPlusTwoJets_Filtered_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JH0PMToTauTauPlusTwoJets','/JJH0PMToTauTauPlusTwoJets_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJH0PMToTauTauPlusZeroJets_Filtered','/JJH0PMToTauTauPlusZeroJets_Filtered_M125_TuneCUETP8M1_13TeV-mcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))

    tasks.append(('JJHiggs0MToTauTau','/JJHiggs0MToTauTau_M-125_13TeV-JHUGenV6_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('JJHiggs0Mf05ph0ToTauTau','/JJHiggs0Mf05ph0ToTauTau_M-125_13TeV-JHUGenV6_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('VBFHiggs0L1_HToTauTau','/VBFHiggs0L1_HToTauTau_M-125_13TeV-JHUGenV6/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v4/MINIAODSIM'))
    tasks.append(('VBFHiggs0L1f05ph0_HToTauTau','/VBFHiggs0L1f05ph0_HToTauTau_M-125_13TeV-JHUGenV6/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('VBFHiggs0M_HToTauTau','/VBFHiggs0M_HToTauTau_M-125_13TeV-JHUGenV6/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('VBFHiggs0Mf05ph0_HToTauTau','/VBFHiggs0Mf05ph0_HToTauTau_M-125_13TeV-JHUGenV6/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('VBFHiggs0PH_HToTauTau','/VBFHiggs0PH_HToTauTau_M-125_13TeV-JHUGenV6/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('VBFHiggs0PHf05ph0_HToTauTau','/VBFHiggs0PHf05ph0_HToTauTau_M-125_13TeV-JHUGenV6/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('VBFHiggs0PM_HToTauTau','/VBFHiggs0PM_HToTauTau_M-125_13TeV-JHUGenV6/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('WHiggs0L1_HToTauTau','/WHiggs0L1_HToTauTau_M-125_13TeV-JHUgenV6/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('ZHiggs0L1fZH05ph0_HToTauTau','/ZHiggs0L1fZH05ph0_HToTauTau_M-125_13TeV-JHUgenV6/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'))
    tasks.append(('ttHiggs0MToTauTau','/ttHiggs0MToTauTau_M-125_13TeV-JHUGenV7_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('ttHiggs0Mf05ph0ToTauTau','/ttHiggs0Mf05ph0ToTauTau_M-125_13TeV-JHUGenV7_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))
    tasks.append(('ttHiggs0PMToTauTau','/ttHiggs0PMToTauTau_M-125_13TeV-JHUGenV7_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'))

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]

        if ("HToTauTau" in task[0] or 'JJH' in task[0]) and 'JHUGen' not in task[1]:
            if 'mcatnloFXFX' in task[0]:
                config.JobType.pyCfgParams = cfgParams + ['LHEWeights=True','includenpNLO=True']
            else:
                config.JobType.pyCfgParams = cfgParams + ['LHEWeights=True','tauSpinner=True']
        else:
            config.JobType.pyCfgParams = cfgParams

        print config.Data.unitsPerJob
        print config.Data.splitting

        print(config)
        submit(config)

