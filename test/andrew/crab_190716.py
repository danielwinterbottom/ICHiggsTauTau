from WMCore.Configuration import Configuration
config = Configuration()

PROD='prod_190716'

config.section_('General')
config.General.workArea=PROD
#config.General.requestName = ''


config.section_('JobType')
config.JobType.psetName = '/afs/cern.ch/work/a/agilbert/CMSSW_Analysis/CMSSW_8_0_12/src/UserCode/ICHiggsTauTau/test/andrew/cmssw_cfg_80X.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
#config.JobType.inputFiles = ['Summer15_V5_MC.db']
config.JobType.pyCfgParams = ['release=80XMINIAOD','isData=1', 'globalTag=80X_dataRun2_Prompt_v8']


config.section_('Data')
#config.Data.inputDataset = ''
config.Data.unitsPerJob = 150000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
#config.Data.runRange = '271036-274240'
config.Data.ignoreLocality=False
config.Data.outLFNDirBase='/store/user/agilbert/%s' % PROD


config.section_('User')
config.User.voGroup = 'dcms'


config.section_('Site')
config.Site.storageSite = 'T2_DE_DESY'
#config.Site.whitelist = ['T2_UK_London_IC', 'T2_CH_CERN', 'T2_FR_GRIF_LLR', 'T2_UK_SGrid_Bristol', 'T3_US_FNALLPC', 'T2_DE_DESY', 'T2_IT_Bari', 'T2_BE_IIHE', 'T2_US_UCSD', 'T2_US_MIT', 'T2_IT_Pisa', 'T2_US_Wisconsin', 'T2_US_Florida', 'T2_IT_Rome','T2_FR_IPHC']

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

    pyCfgs = {
        'dataMu':  ['release=80XMINIAOD','isData=1', 'globalTag=80X_dataRun2_Prompt_ICHEP16JEC_v0', 'hltPaths=SingleMuon'],
        'dataEl':  ['release=80XMINIAOD','isData=1', 'globalTag=80X_dataRun2_Prompt_ICHEP16JEC_v0', 'hltPaths=SingleElectron'],
        'dataTau': ['release=80XMINIAOD','isData=1', 'globalTag=80X_dataRun2_Prompt_ICHEP16JEC_v0', 'hltPaths=Tau'],
        'dataEM':  ['release=80XMINIAOD','isData=1', 'globalTag=80X_dataRun2_Prompt_ICHEP16JEC_v0', 'hltPaths=MuonEG'],
        'mc':      ['release=80XMINIAOD','isData=0', 'globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1', 'hltPaths=None'],
        'mcHLT':   ['release=80XMINIAOD','isData=0', 'globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1', 'hltPaths=MCreHLT'],
        }

    tasks_data=[
      ('MuonEG-Run2016B-PromptReco-v1', '/MuonEG/Run2016B-PromptReco-v1/MINIAOD', 'dataEM'),
      ('MuonEG-Run2016B-PromptReco-v2', '/MuonEG/Run2016B-PromptReco-v2/MINIAOD', 'dataEM'),
      ('MuonEG-Run2016C-PromptReco-v2', '/MuonEG/Run2016C-PromptReco-v2/MINIAOD', 'dataEM'),
      ('MuonEG-Run2016D-PromptReco-v2', '/MuonEG/Run2016D-PromptReco-v2/MINIAOD', 'dataEM'),
      ('SingleMuon-Run2016B-PromptReco-v1', '/SingleMuon/Run2016B-PromptReco-v1/MINIAOD', 'dataMu'),
      ('SingleMuon-Run2016B-PromptReco-v2', '/SingleMuon/Run2016B-PromptReco-v2/MINIAOD', 'dataMu'),
      ('SingleMuon-Run2016C-PromptReco-v2', '/SingleMuon/Run2016C-PromptReco-v2/MINIAOD', 'dataMu'),
      ('SingleMuon-Run2016D-PromptReco-v2', '/SingleMuon/Run2016D-PromptReco-v2/MINIAOD', 'dataMu'),
      ('SingleElectron-Run2016B-PromptReco-v1', '/SingleElectron/Run2016B-PromptReco-v1/MINIAOD', 'dataEl'),
      ('SingleElectron-Run2016B-PromptReco-v2', '/SingleElectron/Run2016B-PromptReco-v2/MINIAOD', 'dataEl'),
      ('SingleElectron-Run2016C-PromptReco-v2', '/SingleElectron/Run2016C-PromptReco-v2/MINIAOD', 'dataEl'),
      ('SingleElectron-Run2016D-PromptReco-v2', '/SingleElectron/Run2016D-PromptReco-v2/MINIAOD', 'dataEl'),
      ('Tau-Run2016B-PromptReco-v1', '/Tau/Run2016B-PromptReco-v1/MINIAOD', 'dataTau'),
      ('Tau-Run2016B-PromptReco-v2', '/Tau/Run2016B-PromptReco-v2/MINIAOD', 'dataTau'),
      ('Tau-Run2016C-PromptReco-v2', '/Tau/Run2016C-PromptReco-v2/MINIAOD', 'dataTau'),
      ('Tau-Run2016D-PromptReco-v2', '/Tau/Run2016D-PromptReco-v2/MINIAOD', 'dataTau'),
    ]

    tasks_bkg=[
      ('VVTo2L2Nu',   '/VVTo2L2Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM', 'mc'),
      ('ZZTo2L2Q',    '/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM', 'mc'),
      ('ZZTo4L',      '/ZZTo4L_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM', 'mc'),
      ('WWTo1L1Nu2Q', '/WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM', 'mc'),
      ('WZTo2L2Q',    '/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM', 'mc'),
      ('WZJToLLLNu',  '/WZJToLLLNu_TuneCUETP8M1_13TeV-amcnlo-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM', 'mc'),
      ('WZTo1L3Nu',   '/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM', 'mc'),
      ('WZTo1L1Nu2Q', '/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM', 'mc'),
      ('QCD_Pt-20toInf_MuEnrichedPt15', '/QCD_Pt-20toInf_MuEnrichedPt15_TuneCUETP8M1_13TeV_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM', 'mc'),
      ('ST_tW_antitop', '/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM', 'mc'),
      ('ST_tW_top', '/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM', 'mc'),
      ('ST_t-channel_top', '/ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM', 'mc'),
      ('ST_t-channel_antitop', '/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM', 'mc'),
      #('TT', '/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext4-v1/MINIAODSIM', 'mc'),
      ('TT', '/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext3-v1/MINIAODSIM', 'mcHLT'),
      ('WJetsToLNu', '/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM', 'mc'),
      ('W1JetsToLNu', '/W1JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM', 'mc'),
      ('W2JetsToLNu', '/W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM', 'mc'),
      ('W3JetsToLNu', '/W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM', 'mc'),
      ('W4JetsToLNu', '/W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM', 'mc'),
      ('DYJetsToLL', '/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM', 'mc'),
      ('DY1JetsToLL', '/DY1JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM', 'mc'),
      ('DY2JetsToLL', '/DY2JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM', 'mc'),
      ('DY3JetsToLL', '/DY3JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM', 'mc'),
      ('DY4JetsToLL', '/DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM', 'mc'),
      ('DYJetsToLL_M-10to50', '/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM', 'mc'),
      ('EWKWMinus2Jets_WToLNu', '/EWKWMinus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v2/MINIAODSIM', 'mc'),
      ('EWKWPlus2Jets_WToLNu', '/EWKWPlus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM', 'mc'),
      ('EWKZ2Jets_ZToLL', '/EWKZ2Jets_ZToLL_M-50_13TeV-madgraph-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM', 'mc')
    ]

    tasks_sig=[
      ('GluGluHToTauTau_M125', '/GluGluHToTauTau_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      ('VBFHToTauTau_M125', '/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      ('SUSYGluGluToHToTauTau_M-90', '/SUSYGluGluToHToTauTau_M-90_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToHToTauTau_M-120', '/SUSYGluGluToHToTauTau_M-120_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToHToTauTau_M-130', '/SUSYGluGluToHToTauTau_M-130_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      ('SUSYGluGluToHToTauTau_M-180', '/SUSYGluGluToHToTauTau_M-180_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToHToTauTau_M-350', '/SUSYGluGluToHToTauTau_M-350_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToHToTauTau_M-450', '/SUSYGluGluToHToTauTau_M-450_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      ('SUSYGluGluToHToTauTau_M-700', '/SUSYGluGluToHToTauTau_M-700_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToHToTauTau_M-1000', '/SUSYGluGluToHToTauTau_M-1000_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToHToTauTau_M-1200', '/SUSYGluGluToHToTauTau_M-1200_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToHToTauTau_M-1400', '/SUSYGluGluToHToTauTau_M-1400_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      ('SUSYGluGluToHToTauTau_M-1500', '/SUSYGluGluToHToTauTau_M-1500_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToHToTauTau_M-1600', '/SUSYGluGluToHToTauTau_M-1600_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToHToTauTau_M-2000', '/SUSYGluGluToHToTauTau_M-2000_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      ('SUSYGluGluToHToTauTau_M-3200', '/SUSYGluGluToHToTauTau_M-3200_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToBBHToTauTau_M-80', '/SUSYGluGluToBBHToTauTau_M-80_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      ('SUSYGluGluToBBHToTauTau_M-90', '/SUSYGluGluToBBHToTauTau_M-90_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToBBHToTauTau_M-140', '/SUSYGluGluToBBHToTauTau_M-140_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      ('SUSYGluGluToBBHToTauTau_M-180', '/SUSYGluGluToBBHToTauTau_M-180_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToBBHToTauTau_M-300', '/SUSYGluGluToBBHToTauTau_M-300_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToBBHToTauTau_M-500', '/SUSYGluGluToBBHToTauTau_M-500_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToBBHToTauTau_M-600', '/SUSYGluGluToBBHToTauTau_M-600_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      ('SUSYGluGluToBBHToTauTau_M-700', '/SUSYGluGluToBBHToTauTau_M-700_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToBBHToTauTau_M-1000', '/SUSYGluGluToBBHToTauTau_M-1000_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      #('SUSYGluGluToBBHToTauTau_M-1200', '/SUSYGluGluToBBHToTauTau_M-1200_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT'),
      ('SUSYGluGluToBBHToTauTau_M-1500', '/SUSYGluGluToBBHToTauTau_M-1500_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM', 'mcHLT')
    ]


    for task in tasks_bkg:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        config.JobType.pyCfgParams = pyCfgs[task[2]]
        print config
        #submit(config)



