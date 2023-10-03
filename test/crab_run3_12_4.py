import argparse
import os

#python3 crab_run3_12_4.py --data --mc --output_folder May0223 --year 2022-preEE
#cmsRun test/higgstautau_cfg_124X_Dec22_2018.py isData=0 globalTag=106X_upgrade2018_realistic_v15_L1v1 isEmbed=0
parser = argparse.ArgumentParser()
parser.add_argument('--output_folder','-o', help= 'Name of output directory', default='Jan24_Run3')
parser.add_argument('--year','-y', help= 'Name of input year', choices=["2022-preEE", "2022-postEE","all"], default='2022-preEE')
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

if args.year == "all": yl = ["2022-preEE", "2022-postEE"]
else: yl = [args.year]

cfg = {
       "2022-preEE":"higgstautau_cfg_124X_Jan24_2022.py",
       "2022-postEE":"higgstautau_cfg_124X_Jan24_2022.py"}

gt = {
      "MC"  :{
              "2022-preEE":"124X_mcRun3_2022_realistic_v12", # check
               "2022-postEE":"124X_mcRun3_2022_realistic_postEE_v3"
              },
      "Signal":{
                "2022-preEE":"124X_mcRun3_2022_realistic_postEE_v1", # check
                "2022-postEE":"124X_mcRun3_2022_realistic_postEE_v1"
              },
      "Data":{
              "2022-preEE":"124X_dataRun3_v15", #rereco tag C-E
              "2022-postEE":"124X_dataRun3_Prompt_v2" #"124X_dataRun3_v14" 
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
    
        if dm == "Data":
           if yr == "2022-preEE":
              # SingleMuon
              tasks.append(('SingleMuonC_rereco','/SingleMuon/Run2022C-27Jun2023-v1/MINIAOD')) # single muon and double muon merged after era C
              
              # EGamma
              tasks.append(('EGammaC_rereco','/EGamma/Run2022C-27Jun2023-v1/MINIAOD'))
              tasks.append(('EGammaD_rereco','/EGamma/Run2022D-27Jun2023-v2/MINIAOD'))
              tasks.append(('EGammaE_rereco','/EGamma/Run2022E-27Jun2023-v1/MINIAOD'))
              
              # Tau
              tasks.append(('TauC_rereco','/Tau/Run2022C-27Jun2023-v2/MINIAOD'))
              tasks.append(('TauD_rereco','/Tau/Run2022D-27Jun2023-v2/MINIAOD'))
              tasks.append(('TauE_rereco','/Tau/Run2022E-27Jun2023-v1/MINIAOD'))

              # MuonEG
              tasks.append(('MuonEGC_rereco','/MuonEG/Run2022C-27Jun2023-v1/MINIAOD'))
              tasks.append(('MuonEGD_rereco','/MuonEG/Run2022D-27Jun2023-v2/MINIAOD'))
              tasks.append(('MuonEGE_rereco','/MuonEG/Run2022E-27Jun2023-v1/MINIAOD'))
              
              # DoubleMuon
              tasks.append(('DoubleMuonC_rereco','/DoubleMuon/Run2022C-27Jun2023-v1/MINIAOD')) # single muon and double muon merged after era C

              # Muon
              tasks.append(('MuonC_rereco','/Muon/Run2022C-27Jun2023-v1/MINIAOD'))
              tasks.append(('MuonD_rereco','/Muon/Run2022D-27Jun2023-v2/MINIAOD'))
              tasks.append(('MuonE_rereco','/Muon/Run2022E-27Jun2023-v1/MINIAOD'))

           elif yr == "2022-postEE":
              # EGamma
              tasks.append(('EGammaF','/EGamma/Run2022F-PromptReco-v1/MINIAOD'))
              tasks.append(('EGammaG','/EGamma/Run2022G-PromptReco-v1/MINIAOD'))

              # # Tau
              tasks.append(('TauF','/Tau/Run2022F-PromptReco-v1/MINIAOD'))
              tasks.append(('TauG','/Tau/Run2022G-PromptReco-v1/MINIAOD'))

              # MuonEG
              tasks.append(('MuonEGF','/MuonEG/Run2022F-PromptReco-v1/MINIAOD'))
              tasks.append(('MuonEGG','/MuonEG/Run2022G-PromptReco-v1/MINIAOD'))

              # Muon
              tasks.append(('MuonF','/Muon/Run2022F-PromptReco-v1/MINIAOD'))
              tasks.append(('MuonG','/Muon/Run2022G-PromptReco-v1/MINIAOD'))

        if dm == "MC":
           if yr == "2022-preEE":
              # Drell-Yan
              tasks.append(('DYJetsToLL_M-50',' /DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Summer22MiniAODv3-forPOG_124X_mcRun3_2022_realistic_v12-v4/MINIAODSIM'))
              tasks.append(('DYTo2L_MLL-4to50',' /DYTo2L_MLL-4to50_TuneCP5_13p6TeV_pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('DYTo2L_MLL-50',' /DYTo2L_MLL-50_TuneCP5_13p6TeV_pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('DYto2TautoMuTauh_M-50',' /DYto2TautoMuTauh_M-50_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('DYto2L-4Jets_MLL-50_2J',' /DYto2L-4Jets_MLL-50_2J_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('DYto2L-4Jets_MLL-50_3J',' /DYto2L-4Jets_MLL-50_3J_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('DYto2L-4Jets_MLL-50_4J',' /DYto2L-4Jets_MLL-50_4J_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v1/MINIAODSIM'))
              tasks.append(('DYto2L-4Jets_MLL-50',' /DYto2L-4Jets_MLL-50_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              
              
              # Drell-Yan LO
              #tasks.append(('DYJetsToLL-LO_summer','/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Summer22MiniAODv3-forPOG_124X_mcRun3_2022_realistic_v12-v4/MINIAODSIM'))

              # Low mass Drell-Yan LO
              #tasks.append(('DYJetsToLL_M-10to50-LO','/DYJetsToLL_M-10to50_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Winter22MiniAOD-122X_mcRun3_2021_realistic_v9-v2/MINIAODSIM'))
             
              #Drell-Yan NLO
              #/DYJetsToLL_0J*/Run3*/MINIAODSIM -not prepared yet
              #/DYJetsToLL_1J*/Run3*/MINIAODSIM -not prepared yet
              #/DYJetsToLL_2J*/Run3*/MINIAODSIM -not prepared yet
              #/DYJetsToLL_M-50_TuneCP5_13P6TeV-amcatnloFXFX*/Run3*/MINIAODSIM -not prepared yet

              # Electroweak W and Z
              #/EWKWMinus2Jets*/Run3*/MINIAODSIM -not prepared yet
              #/EWKWPlus2Jets*/Run3*/MINIAODSIM -not prepared yet
              #/EWKZ2Jets_ZToLL*/Run3*/MINIAODSIM -not prepared yet

              # W + Jets LO
              tasks.append(('WJetsToLNu-LO','/WtoLNu-4Jets_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('W3JetsToLNu-LO','/WtoLNu-4Jets_3J_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('WJetsToLNu-2Jets','/WtoLNu-2Jets_TuneCP5_13p6TeV_amcatnloFXFX-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v3/MINIAODSIM'))
              tasks.append(('WJetsToLNu-4Jets_4J','/WtoLNu-4Jets_4J_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              # tasks.append(('W1JetsToLNu-LO','/W1JetsToLNu_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Winter22MiniAOD-122X_mcRun3_2021_realistic_v9-v2/MINIAODSIM'))
              # tasks.append(('W2JetsToLNu-LO','/W2JetsToLNu_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Winter22MiniAOD-122X_mcRun3_2021_realistic_v9-v2/MINIAODSIM'))
              # tasks.append(('W4JetsToLNu-LO','/W4JetsToLNu_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Winter22MiniAOD-122X_mcRun3_2021_realistic_v9-v2/MINIAODSIM'))
              #not with global 124X tag

              # W + Jets NLO
              #/WJetsToLNu_0J*/Run3*/MINIAODSIM -not prepared yet
              #/WJetsToLNu_1J*/Run3*/MINIAODSIM -not prepared yet
              #/WJetsToLNu_2J*/Run3*/MINIAODSIM -not prepared yet
              #/WJetsToLNu_TuneCP5_13p6TeV-amcatnloFXFX-*/Run3*/MINIAODSIM -not prepared yet

              # ttbar
              tasks.append(('TTTo2L2Nu','/TTto2L2Nu_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v4/MINIAODSIM'))
              tasks.append(('TTtoLNu2Q','/TTtoLNu2Q_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v5/MINIAODSIM'))
              tasks.append(('TTto4Q','/TTto4Q_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v4/MINIAODSIM'))
              tasks.append(('TT_ext1','/TT_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12_ext1-v3/MINIAODSIM'))
              tasks.append(('TT','/TT_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v3/MINIAODSIM'))
              #/TTToHadronic_TuneCP5_13p6TeV*/Run3*/MINIAODSIM -not prepared yet
              #/TTToSemiLeptonic*/Run3*/MINIAODSIM -not prepared yet
             

              #tasks.append(('TTToSemiLeptonic_FlatPU0to70','/TTToSemiLeptonic_TuneCP5_13p6TeV-powheg-pythia8/Run3Winter22MiniAOD-FlatPU0to70_122X_mcRun3_2021_realistic_v9-v2/MINIAODSIM'))
              #tasks.append(('TTToSemiLeptonic_PUForMUOVal','/TTToSemiLeptonic_TuneCP5_13p6TeV-powheg-pythia8/Run3Winter22MiniAOD-PUForMUOVal_122X_mcRun3_2021_realistic_v9-v2/MINIAODSIM'))
 
              # Split diboson
              #/WZTo1L3Nu_4f*/Run3*/MINIAODSIM -not prepared yet
              #/WZTo1L1Nu2Q*/Run3*/MINIAODSIM -not prepared yet
              #/WZTo2Q2L*/Run3*/MINIAODSIM -not prepared yet
              #/WZTo3LNu*/Run3*/MINIAODSIM -not prepared yet
              #/WWTo1L1Nu2Q*/Run3*/MINIAODSIM -not prepared yet
              #/WWTo2L2Nu*/Run3*/MINIAODSIM -not prepared yet
              #/ZZTo2L2Nu*/Run3*/MINIAODSIM -not prepared yet
              #/ZZTo4L*/Run3*/MINIAODSIM -not prepared yet

              # Inclusive diboson
              tasks.append(('WW','/WW_TuneCP5_13p6TeV_pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('WZ','/WZ_TuneCP5_13p6TeV_pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('ZZ','/ZZ_TuneCP5_13p6TeV_pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
 
              # Triboson
              tasks.append(('ZZZ','/ZZZ_TuneCP5_13p6TeV_amcatnlo-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('WWW_4F','/WWW_4F_TuneCP5_13p6TeV_amcatnlo-madspin-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('WWZ_4F','/WWZ_4F_TuneCP5_13p6TeV_amcatnlo-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('WZZ','/WZZ_TuneCP5_13p6TeV_amcatnlo-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))

              # Other backgrounds
              tasks.append(('TbarWplusto2L2Nu','/TbarWplusto2L2Nu_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('TbarWplustoLNu2Q','/TbarWplustoLNu2Q_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('TWminustoLNu2Q','/TWminustoLNu2Q_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('TWminusto2L2Nu','/TWminusto2L2Nu_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              tasks.append(('TbarBQ_t-channel_4FS','/TbarBQ_t-channel_4FS_TuneCP5_13p6TeV_powheg-madspin-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v4/MINIAODSIM'))
              tasks.append(('TBbarQ_t-channel_4FS','/TBbarQ_t-channel_4FS_TuneCP5_13p6TeV_powheg-madspin-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v4/MINIAODSIM'))
              #/WGToLNuG*/Run3*/MINIAODSIM -not prepared yet
              #/ST_t-channel_antitop_4f_InclusiveDecays*/Run3*/MINIAODSIM -not prepared yet
              #/ST_tW_antitop_5f_inclusiveDecays*/Run3*/MINIAODSIM -not prepared yet
              #/ST_t-channel_top_4f_InclusiveDecays*/Run3*/MINIAODSIM -not prepared yet
              #/ST_tW_top_5f_inclusiveDecays*/Run3*/MINIAODSIM -not prepared yet

              # SM Higgs
              tasks.append(('GluGluHToTauTau_M125','/GluGluHToTauTau_M-125_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v3/MINIAODSIM'))
              tasks.append(('VBFHToTauTau_M125','/VBFHToTauTau_M125_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v3/MINIAODSIM'))
              tasks.append(('DYto2TautoMuTauh_M50','/DYto2TautoMuTauh_M-50_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22MiniAODv3-124X_mcRun3_2022_realistic_v12-v2/MINIAODSIM'))
              #/WminusHToTauTau_M125*/Run3*/MINIAODSIM -not prepared yet
              #/WplusHToTauTau_M125*/Run3*/MINIAODSIM -not prepared yet
              #/ttHToTauTau_M125*/Run3*/MINIAODSIM -not prepared yet
              #/VBF_HToZZTo4L_M125*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluHToZZTo4L_M125*/Run3*/MINIAODSIM -not prepared yet

              # gg->ZZ
              #/GluGluToContinToZZTo2e2mu*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluToContinToZZTo2e2nu*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluToContinToZZTo2e2tau*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluToContinToZZTo2mu2nu*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluToContinToZZTo2mu2tau*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluToContinToZZTo4e*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluToContinToZZTo4mu*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluToContinToZZTo4tau*/Run3*/MINIAODSIM -not prepared yet


#            tasks.append(('DYToLL_M-50_PU70', '/DYToLL_M-50_TuneCP5_13p6TeV-pythia8/Run3Summer22EEMiniAODv3-Poisson70KeepRAW_124X_mcRun3_2022_realistic_postEE_v1-v1/MINIAODSIM'))
        
             #tasks.append(('DYJetsToLL_M-50_FlatPU0to70-ext', '/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Winter22MiniAOD-FlatPU0to70_122X_mcRun3_2021_realistic_v9_ext1-v2/MINIAODSIM'))
   
             #tasks.append(('DYtoLL_NoTau','DYtoLL_NoTau_CP5_13p6TeV_amcatnloFXFX-pythia8/Run3Winter22MiniAOD-122X_mcRun3_2021_realistic_v9-v2/MINIAODSIM'))

           elif yr == "2022-postEE":
              # Drell-Yan LO
              tasks.append(('DYJetsToLL-LO','/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Summer22EEMiniAODv3-forPOG_124X_mcRun3_2022_realistic_postEE_v1-v3/MINIAODSIM'))
              tasks.append(('DYto2L-4Jets_MLL-50_1J','/DYto2L-4Jets_MLL-50_1J_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('DYto2L-4Jets_MLL-50_2J','/DYto2L-4Jets_MLL-50_2J_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('DYto2L-4Jets_MLL-50_3J','/DYto2L-4Jets_MLL-50_3J_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('DYto2L-4Jets_MLL-50_4J','/DYto2L-4Jets_MLL-50_4J_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v1/MINIAODSIM'))
              tasks.append(('DYto2L-4Jets_MLL-50','/DYto2L-4Jets_MLL-50_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              #tasks.append(('DYJetsToLL-LO_winter','/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Winter22MiniAOD-122X_mcRun3_2021_realistic_v9_ext1-v1/MINIAODSIM'))
              #tasks.append(('DYJetsToLL-LO_winter_v2','/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Winter22MiniAOD-122X_mcRun3_2021_realistic_v9_ext2-v2/MINIAODSIM'))
              #tasks.append(('DYJetsToLL-LO_FlatPU0to70','/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Winter22MiniAOD-FlatPU0to70_122X_mcRun3_2021_realistic_v9_ext1-v2/MINIAODSIM'))

              # Low mass Drell-Yan LO
              #tasks.append(('DYJetsToLL_M-10to50-LO','/DYJetsToLL_M-10to50_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Winter22MiniAOD-122X_mcRun3_2021_realistic_v9-v2/MINIAODSIM'))

              #Drell-Yan NLO
              #/DYJetsToLL_0J*/Run3*/MINIAODSIM -not prepared yet
              #/DYJetsToLL_1J*/Run3*/MINIAODSIM -not prepared yet
              #/DYJetsToLL_2J*/Run3*/MINIAODSIM -not prepared yet
              #/DYJetsToLL_M-50_TuneCP5_13P6TeV-amcatnloFXFX*/Run3*/MINIAODSIM -not prepared yet

              # Electroweak W and Z
              #/EWKWMinus2Jets*/Run3*/MINIAODSIM -not prepared yet
              #/EWKWPlus2Jets*/Run3*/MINIAODSIM -not prepared yet
              #/EWKZ2Jets_ZToLL*/Run3*/MINIAODSIM -not prepared yet

              # W + Jets LO
              tasks.append(('WJetsToLNu-LO','/WtoLNu-4Jets_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('W1JetsToLNu-LO','/WtoLNu-4Jets_1J_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('W2JetsToLNu-LO','/WtoLNu-4Jets_2J_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('W3JetsToLNu-LO','//WtoLNu-4Jets_3J_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('WJetstoLNu_2Jets','/WtoLNu-2Jets_TuneCP5_13p6TeV_amcatnloFXFX-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v3/MINIAODSIM'))
              tasks.append(('WtoLNu-4Jets_4J','/WtoLNu-4Jets_4J_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              # tasks.append(('W4JetsToLNu-LO','/W4JetsToLNu_TuneCP5_13p6TeV-madgraphMLM-pythia8/Run3Winter22MiniAOD-122X_mcRun3_2021_realistic_v9-v2/MINIAODSIM'))
              
              # W + Jets NLO
              #/WJetsToLNu_0J*/Run3*/MINIAODSIM -not prepared yet
              #/WJetsToLNu_1J*/Run3*/MINIAODSIM -not prepared yet
              #/WJetsToLNu_2J*/Run3*/MINIAODSIM -not prepared yet
              #/WJetsToLNu_TuneCP5_13p6TeV-amcatnloFXFX-*/Run3*/MINIAODSIM -not prepared yet

              # ttbar
              tasks.append(('TTTo2L2Nu','/TTto2L2Nu_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('TTtoLNu2Q','/TTtoLNu2Q_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('TTto4Q','/TTto4Q_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v3/MINIAODSIM'))
              #/TTToHadronic_TuneCP5_13p6TeV*/Run3*/MINIAODSIM -not prepared yet
              #/TTToSemiLeptonic*/Run3*/MINIAODSIM -not prepared yet
              #with global 124X tag


              #tasks.append(('TTToSemiLeptonic_FlatPU0to70','/TTToSemiLeptonic_TuneCP5_13p6TeV-powheg-pythia8/Run3Winter22MiniAOD-FlatPU0to70_122X_mcRun3_2021_realistic_v9-v2/MINIAODSIM'))
              #tasks.append(('TTToSemiLeptonic_PUForMUOVal','/TTToSemiLeptonic_TuneCP5_13p6TeV-powheg-pythia8/Run3Winter22MiniAOD-PUForMUOVal_122X_mcRun3_2021_realistic_v9-v2/MINIAODSIM'))

              # Split diboson
              #/WZTo1L3Nu_4f*/Run3*/MINIAODSIM -not prepared yet
              #/WZTo1L1Nu2Q*/Run3*/MINIAODSIM -not prepared yet
              #/WZTo2Q2L*/Run3*/MINIAODSIM -not prepared yet
              #/WZTo3LNu*/Run3*/MINIAODSIM -not prepared yet
              #/WWTo1L1Nu2Q*/Run3*/MINIAODSIM -not prepared yet
              #/WWTo2L2Nu*/Run3*/MINIAODSIM -not prepared yet
              #/ZZTo2L2Nu*/Run3*/MINIAODSIM -not prepared yet
              #/ZZTo4L*/Run3*/MINIAODSIM -not prepared yet
              
              # Inclusive diboson
              tasks.append(('WW','/WW_TuneCP5_13p6TeV_pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v3/MINIAODSIM'))
              tasks.append(('WZ','/WZ_TuneCP5_13p6TeV_pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v3/MINIAODSIM'))
              tasks.append(('ZZ','/ZZ_TuneCP5_13p6TeV_pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v3/MINIAODSIM'))

              # Triboson
              tasks.append(('WWW_4F','/WWW_4F_TuneCP5_13p6TeV_amcatnlo-madspin-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('WWZ_4F','/WWZ_4F_TuneCP5_13p6TeV_amcatnlo-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('WZZ','/WZZ_TuneCP5_13p6TeV_amcatnlo-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('ZZZ','/ZZZ_TuneCP5_13p6TeV_amcatnlo-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))

              # Other backgrounds
              tasks.append(('TbarWplusto2L2Nu','/TbarWplusto2L2Nu_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('TbarWplustoLNu2Q','/TbarWplustoLNu2Q_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('TWminustoLNu2Q','/TWminustoLNu2Q_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('TWminusto2L2Nu','/TWminusto2L2Nu_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('TbarBQ_t-channel_4FS','/TbarBQ_t-channel_4FS_TuneCP5_13p6TeV_powheg-madspin-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v4/MINIAODSIM'))
              tasks.append(('TBbarQ_t-channel_4FS','/TBbarQ_t-channel_4FS_TuneCP5_13p6TeV_powheg-madspin-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v4/MINIAODSIM'))
              #/WGToLNuG*/Run3*/MINIAODSIM -not prepared yet
              #/ST_t-channel_antitop_4f_InclusiveDecays*/Run3*/MINIAODSIM -not prepared yet
              #/ST_tW_antitop_5f_inclusiveDecays*/Run3*/MINIAODSIM -not prepared yet
              #/ST_t-channel_top_4f_InclusiveDecays*/Run3*/MINIAODSIM -not prepared yet
              #/ST_tW_top_5f_inclusiveDecays*/Run3*/MINIAODSIM -not prepared yet

              # SM Higgs
              tasks.append(('GluGluHToTauTau_M125_v2','/GluGluHToTauTau_M-125_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22EEMiniAODv3-Poisson60KeepRAW_124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('GluGluHToTauTau_M125_v3','/GluGluHToTauTau_M-125_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22EEMiniAODv3-Poisson70KeepRAW_124X_mcRun3_2022_realistic_postEE_v1-v3/MINIAODSIM'))
              tasks.append(('VBFHToTauTau_M125_Poisson60KeepRAW','/VBFHToTauTau_M125_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22EEMiniAODv3-Poisson60KeepRAW_124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('VBFHToTauTau_M125_v2_Poisson70KeepRAW','/VBFHToTauTau_M125_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22EEMiniAODv3-Poisson70KeepRAW_124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              tasks.append(('DYto2TautoMuTauh_M50','/DYto2TautoMuTauh_M-50_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EEMiniAODv3-124X_mcRun3_2022_realistic_postEE_v1-v2/MINIAODSIM'))
              #/WminusHToTauTau_M125*/Run3*/MINIAODSIM -not prepared yet
              #/WplusHToTauTau_M125*/Run3*/MINIAODSIM -not prepared yet
              #/ttHToTauTau_M125*/Run3*/MINIAODSIM -not prepared yet
              #/VBF_HToZZTo4L_M125*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluHToZZTo4L_M125*/Run3*/MINIAODSIM -not prepared yet

              # gg->ZZ
              #/GluGluToContinToZZTo2e2mu*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluToContinToZZTo2e2nu*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluToContinToZZTo2e2tau*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluToContinToZZTo2mu2nu*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluToContinToZZTo2mu2tau*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluToContinToZZTo4e*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluToContinToZZTo4mu*/Run3*/MINIAODSIM -not prepared yet
              #/GluGluToContinToZZTo4tau*/Run3*/MINIAODSIM -not prepared yet



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
            elif (task[0] == 'MuonF') and yr == "2022-postEE":
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
