import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--output_folder','-o', help= 'Name of output directory', default='Nov22')
parser.add_argument('--year','-y', help= 'Name of input year', choices=["2016","2017","2018","all"], default='all')
parser.add_argument('--data', help= 'Run data samples',  action='store_true')
parser.add_argument('--mc', help= 'Run mc samples',  action='store_true')
args = parser.parse_args()

dml = []
if args.data: dml.append("Data")
if args.mc: dml.append("MC")

if args.year == "all": yl = ["2016-preVFP","2016-postVFP","2017","2018"]
else: yl = [args.year]

cfg = {
       "2016-preVFP":"higgstautau_cfg_106X_Nov21_2016-preVFP.py",
       "2016-postVFP":"higgstautau_cfg_106X_Nov21_2016-postVFP.py",
       "2017":"higgstautau_cfg_106X_Nov21_2017.py",
       "2018":"higgstautau_cfg_106X_Nov21_2018.py"
       }

gt = {
      "MC"  :{"2016-preVFP":"106X_mcRun2_asymptotic_v13",
              "2016-postVFP":"106X_mcRun2_asymptotic_v13",
              "2017":"106X_mc2017_realistic_v6",
              "2018":"106X_upgrade2018_realistic_v11_L1v1"
              },
      "Data":{"2016-preVFP":"106X_mcRun2_asymptotic_v13",
              "2016-postVFP":"106X_mcRun2_asymptotic_v13",
              "2017":"106X_mc2017_realistic_v9",
              "2018":"106X_upgrade2018_realistic_v16_L1v1"
              }
      }

for dm in dml:
  for yr in yl:

    from CRABClient.UserUtilities import config
    from CRABClient.UserUtilities import getUsernameFromCRIC
    from multiprocessing import Process

    print "Processing {} for {}".format(yr, dm)    

    config = config()
    
    config.General.transferOutputs = True
    config.General.workArea='{}_{}_106X_{}'.format(args.output_folder,dm,yr)
    
    config.JobType.psetName = cfg[yr]
    config.JobType.pluginName = 'Analysis'
    config.JobType.outputFiles = ['EventTree.root']
    config.JobType.maxMemoryMB = 4000
    cfgParams = ['globalTag={}'.format(gt[dm][yr])]
    if dm == "Data": cfgParams.append('isData=1')
    else: cfgParams.append('isData=0')
    
    config.JobType.allowUndistributedCMSSW = True
    #config.Data.unitsPerJob = 1
    #config.Data.splitting = 'FileBased'
    config.Data.outLFNDirBase='/store/user/{}/{}/'.format(getUsernameFromCRIC(), config.General.workArea)
    config.Data.unitsPerJob = 50000
    config.Data.splitting = 'EventAwareLumiBased'
    config.Data.publication = False
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
            except HTTPException as hte:
                print(hte.headers)
            except ClientException as cle:
                print(cle)
    
        #############################################################################################
        ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
        #############################################################################################
    
        tasks=list()
    
        if dm == "Data":
          if yr == "2016-preVFP":
            # 2016 runs B to F had HIP problem
            # Single Muon
            tasks.append(('SingleMuonB', '/SingleMuon/Run2016B-21Feb2020_ver2_UL2016_HIPM-v1/MINIAOD'))
            tasks.append(('SingleMuonC', '/SingleMuon/Run2016C-HIPM_UL2016_MiniAODv2-v2/MINIAOD'))
            tasks.append(('SingleMuonD', '/SingleMuon/Run2016D-HIPM_UL2016_MiniAODv2-v2/MINIAOD'))
            tasks.append(('SingleMuonE', '/SingleMuon/Run2016E-HIPM_UL2016_MiniAODv2-v2/MINIAOD'))
            tasks.append(('SingleMuonF', '/SingleMuon/Run2016F-HIPM_UL2016_MiniAODv2-v2/MINIAOD'))

            # SingleElectron
            tasks.append(('SingleElectronB', '/SingleElectron/Run2016B-ver2_HIPM_UL2016_MiniAODv2-v2/MINIAOD'))
            tasks.append(('SingleElectronC', '/SingleElectron/Run2016C-HIPM_UL2016_MiniAODv2-v2/MINIAOD'))
            tasks.append(('SingleElectronD', '/SingleElectron/Run2016D-HIPM_UL2016_MiniAODv2-v2/MINIAOD'))
            tasks.append(('SingleElectronE', '/SingleElectron/Run2016E-HIPM_UL2016_MiniAODv2-v5/MINIAOD'))
            tasks.append(('SingleElectronF', '/SingleElectron/Run2016F-UL2016_MiniAODv2-v2/MINIAOD'))

            # Tau
            tasks.append(('TauB', '/Tau/Run2016B-ver2_HIPM_UL2016_MiniAODv2-v1/MINIAOD'))
            tasks.append(('TauC', '/Tau/Run2016C-HIPM_UL2016_MiniAODv2-v1/MINIAOD'))
            tasks.append(('TauD', '/Tau/Run2016D-HIPM_UL2016_MiniAODv2-v1/MINIAOD'))
            tasks.append(('TauE', '/Tau/Run2016E-HIPM_UL2016_MiniAODv2-v1/MINIAOD'))
            tasks.append(('TauF', '/Tau/Run2016F-HIPM_UL2016_MiniAODv2-v1/MINIAOD'))

            # MuonEG
            tasks.append(('MuonEGB', '/MuonEG/Run2016B-ver2_HIPM_UL2016_MiniAODv2-v2/MINIAOD'))
            tasks.append(('MuonEGC', '/MuonEG/Run2016C-HIPM_UL2016_MiniAODv2-v2/MINIAOD'))
            tasks.append(('MuonEGD', '/MuonEG/Run2016D-HIPM_UL2016_MiniAODv2-v2/MINIAOD'))
            tasks.append(('MuonEGE', '/MuonEG/Run2016E-HIPM_UL2016_MiniAODv2-v2/MINIAOD'))
            tasks.append(('MuonEGF', '/MuonEG/Run2016F-HIPM_UL2016_MiniAODv2-v2/MINIAOD'))
          
          elif yr == "2016-postVFP":
            # HIP problem fixed in 2016 runs G and H
            # SingleMuon
            tasks.append(('SingleMuonG', '/SingleMuon/Run2016G-UL2016_MiniAODv2-v2/MINIAOD'))
            tasks.append(('SingleMuonH', '/SingleMuon/Run2016H-UL2016_MiniAODv2-v2/MINIAOD'))

            # SingleElectron
            tasks.append(('SingleElectronG', '/SingleElectron/Run2016G-UL2016_MiniAODv2-v2/MINIAOD'))
            tasks.append(('SingleElectronH', '/SingleElectron/Run2016H-UL2016_MiniAODv2-v2/MINIAOD'))

            # Tau
            tasks.append(('TauG', '/Tau/Run2016G-UL2016_MiniAODv2-v1/MINIAOD'))
            tasks.append(('TauH', '/Tau/Run2016H-UL2016_MiniAODv2-v1/MINIAOD'))

            # MuonEG
            tasks.append(('MuonEGG', '/MuonEG/Run2016G-UL2016_MiniAODv2-v2/MINIAOD'))
            tasks.append(('MuonEGH', '/MuonEG/Run2016H-UL2016_MiniAODv2-v2/MINIAOD'))

          elif yr == "2017":
            # Runs B to F in 2017
            # SingleMuon - why runs G and H?
            tasks.append(('SingleMuonB', '/SingleMuon/Run2017B-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('SingleMuonC', '/SingleMuon/Run2017C-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('SingleMuonD', '/SingleMuon/Run2017D-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('SingleMuonE', '/SingleMuon/Run2017E-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('SingleMuonF', '/SingleMuon/Run2017F-UL2017_MiniAODv2-v1/MINIAOD'))
            #tasks.append(('SingleMuonG', '/SingleMuon/Run2017G-UL2017_MiniAODv2-v1/MINIAOD'))
            #tasks.append(('SingleMuonH', '/SingleMuon/Run2017H-UL2017_MiniAODv2-v1/MINIAOD'))

            # SingleElectron
            tasks.append(('SingleElectronB', '/SingleElectron/Run2017B-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('SingleElectronC', '/SingleElectron/Run2017C-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('SingleElectronD', '/SingleElectron/Run2017D-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('SingleElectronE', '/SingleElectron/Run2017E-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('SingleElectronF', '/SingleElectron/Run2017F-UL2017_MiniAODv2-v1/MINIAOD'))

            # Tau
            tasks.append(('TauB', '/Tau/Run2017B-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('TauC', '/Tau/Run2017C-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('TauD', '/Tau/Run2017D-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('TauE', '/Tau/Run2017E-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('TauF', '/Tau/Run2017F-UL2017_MiniAODv2-v1/MINIAOD'))

            # MuonEG
            tasks.append(('MuonEGB', '/MuonEG/Run2017B-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('MuonEGC', '/MuonEG/Run2017C-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('MuonEGD', '/MuonEG/Run2017D-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('MuonEGE', '/MuonEG/Run2017E-UL2017_MiniAODv2-v1/MINIAOD'))
            tasks.append(('MuonEGF', '/MuonEG/Run2017F-UL2017_MiniAODv2-v1/MINIAOD'))

          elif yr == "2018":
            # Runs A to D in 2018
            # SingleMuon
            tasks.append(('SingleMuonA', '/SingleMuon/Run2018A-UL2018_MiniAODv2-v3/MINIAOD'))
            tasks.append(('SingleMuonB', '/SingleMuon/Run2018B-UL2018_MiniAODv2-v2/MINIAOD'))
            tasks.append(('SingleMuonC', '/SingleMuon/Run2018C-UL2018_MiniAODv2-v2/MINIAOD'))
            tasks.append(('SingleMuonD', '/SingleMuon/Run2018D-UL2018_MiniAODv2-v3/MINIAOD'))

            # EGamma
            tasks.append(('EGammaA', '/EGamma/Run2018A-UL2018_MiniAODv2-v1/MINIAOD'))
            tasks.append(('EGammaB', '/EGamma/Run2018B-12Nov2019_UL2018-v2/MINIAOD'))
            tasks.append(('EGammaC', '/EGamma/Run2018C-UL2018_MiniAODv2-v1/MINIAOD'))
            tasks.append(('EGammaD', '/EGamma/Run2018D-UL2018_MiniAODv2-v2/MINIAOD'))

            # Tau
            tasks.append(('TauA', '/Tau/Run2018A-UL2018_MiniAODv2-v1/MINIAOD'))
            tasks.append(('TauB', '/Tau/Run2018B-UL2018_MiniAODv2-v2/MINIAOD'))
            tasks.append(('TauC', '/Tau/Run2018C-UL2018_MiniAODv2-v1/MINIAOD'))
            tasks.append(('TauD', '/Tau/Run2018D-UL2018_MiniAODv2-v1/MINIAOD'))

            # MuonEG
            tasks.append(('MuonEGA', '/MuonEG/Run2018A-UL2018_MiniAODv2-v1/MINIAOD'))
            tasks.append(('MuonEGB', '/MuonEG/Run2018B-UL2018_MiniAODv2-v1/MINIAOD'))
            tasks.append(('MuonEGC', '/MuonEG/Run2018C-UL2018_MiniAODv2-v1/MINIAOD'))
            tasks.append(('MuonEGD', '/MuonEG/Run2018D-UL2018_MiniAODv2-v1/MINIAOD'))

        if dm == "MC":
          if yr == "2016-preVFP":
            # Spit MC into preVFP and postVFP (HIP fix)
            # Drell-Yan LO
            tasks.append(('DYJetsToLL-LO', '/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM')
            tasks.append(('DY1JetsToLL-LO', '/DY1JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM')
            tasks.append(('DY2JetsToLL-LO', '/DY2JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM')
            tasks.append(('DY3JetsToLL-LO', '/DY3JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM')
            tasks.append(('DY4JetsToLL-LO', '/DY4JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM')

            # Low mass Drell-Yan LO
            tasks.append(('DYJetsToLL_M-10to50-LO', '/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2/MINIAODSIM'))
            tasks.append(('DY1JetsToLL_M-10to50-LO', '/DY1JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('DY2JetsToLL_M-10to50-LO', '/DY2JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('DY3JetsToLL_M-10to50-LO', '/DY3JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1/MINIAODSIM'))
            tasks.append(('DY4JetsToLL_M-10to50-LO', '/DY4JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))

            # Drell-Yan NLO
            tasks.append(('DYJetsToLL_0J-NLO', '/DYJetsToLL_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('DYJetsToLL_1J-NLO', '/DYJetsToLL_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('DYJetsToLL_2J-NLO', '/DYJetsToLL_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))

            # Electroweak W and Z
            tasks.append(('EWKWMinus2Jets_WToLNu', '/EWKWMinus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('EWKWPlus2Jets_WToLNu', '/EWKWPlus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('EWKZ2Jets_ZToLL', '/EWKZ2Jets_ZToLL_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))

            # W + Jets LO
            tasks.append(('WJetsToLNu-LO', '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM'))
            tasks.append(('W1JetsToLNu-LO', '/W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1/MINIAODSIM'))
            tasks.append(('W2JetsToLNu-LO', '/W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM'))
            tasks.append(('W3JetsToLNu-LO', '/W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1/MINIAODSIM'))
            tasks.append(('W4JetsToLNu-LO', '/W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1/MINIAODSIM'))

            # W + Jets NLO
            tasks.append(('WJetsToLNu_0J-NLO', '/WJetsToLNu_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2/MINIAODSIM'))
            tasks.append(('WJetsToLNu_1J-NLO', '/WJetsToLNu_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1/MINIAODSIM'))
            tasks.append(('WJetsToLNu_2J-NLO', '/WJetsToLNu_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))

            # ttbar
            tasks.append(('TTTo2L2Nu', '/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('TTToHadronic', '/TTToHadronic_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('TTToSemiLeptonic', '/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))

            # Split diboson - missing files commented out
            #tasks.append(('WZTo1L3Nu', ''))
            tasks.append(('WZTo1L1Nu2Q', '/WZTo1L1Nu2Q_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2/MINIAODSIM'))
            #tasks.append(('WZTo2L2Q', ''))
            tasks.append(('WZTo3LNu', '/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v3/MINIAODSIM'))
            tasks.append(('WWTo1L1Nu2Q', '/WWTo1L1Nu2Q_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2/MINIAODSIM'))
            tasks.append(('WWTo2L2Nu', '/WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('ZZTo2L2Nu', '/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('ZZTo4L', '/ZZTo4L_M-1toInf_TuneCP5_13TeV_powheg_pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))

            # Inclusive diboson
            tasks.append(('WW', '/WW_TuneCP5_13TeV-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('WZ', '/WZ_TuneCP5_13TeV-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('ZZ', '/ZZ_TuneCP5_13TeV-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))

            # Triboson
            tasks.append(('WWZ', '/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('WWZ-ext1', '/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11_ext1-v1/MINIAODSIM'))
            tasks.append(('WZZ', '/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('WZZ-ext1', '/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11_ext1-v1/MINIAODSIM'))
            tasks.append(('WWW', '/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('WWW-ext1', '/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11_ext1-v1/MINIAODSIM'))
            tasks.append(('ZZZ', '/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('ZZZ-ext1', '/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11_ext1-v1/MINIAODSIM'))

            # Other backgrounds
            tasks.append(('WGToLNuG', '/WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('Tbar-t', '/ST_t-channel_antitop_5f_InclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1/MINIAODSIM'))
            tasks.append(('Tbar-tW', '/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))
            tasks.append(('T-t', '/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v3/MINIAODSIM'))
            tasks.append(('T-tW', '/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'))

            # SM Higgs - missing files commented out
            tasks.append(('GluGluHToTauTau_M125', '/GluGluHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v4/MINIAODSIM'))
            #tasks.append(('VBFHToTauTau_M125', ''))
            #tasks.append(('WminusHToTauTau_M125', ''))
            #tasks.append(('WplusHToTauTau_M125', ''))
            tasks.append(('ZHToTauTau_M125', '/ZHToTauTau_M125_CP5_13TeV-powheg-pythia8_ext1/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM'))
            #tasks.append(('ttHToTauTau_M125', ''))

          elif yr == "2016-postVFP":
            # Spit MC into preVFP and postVFP (HIP fix)
            # Drell-Yan LO
            tasks.append(('DYJetsToLL-LO', '/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('DY1JetsToLL-LO', '/DY1JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('DY2JetsToLL-LO', '/DY2JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))
            tasks.append(('DY3JetsToLL-LO', '/DY3JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))
            tasks.append(('DY4JetsToLL-LO', '/DY4JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))

            # Low mass Drell-Yan LO
            tasks.append(('DYJetsToLL_M-10to50-LO', '/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('DY1JetsToLL_M-10to50-LO', '/DY1JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))
            tasks.append(('DY2JetsToLL_M-10to50-LO', '/DY2JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))
            tasks.append(('DY3JetsToLL_M-10to50-LO', '/DY3JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))
            tasks.append(('DY4JetsToLL_M-10to50-LO', '/DY4JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))

            # Drell-Yan NLO
            tasks.append(('DYJetsToLL_0J-NLO', '/DYJetsToLL_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('DYJetsToLL_1J-NLO', '/DYJetsToLL_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))
            tasks.append(('DYJetsToLL_2J-NLO', '/DYJetsToLL_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))

            # Electroweak W and Z
            tasks.append(('EWKWMinus2Jets_WToLNu', '/EWKWMinus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('EWKWPlus2Jets_WToLNu', '/EWKWPlus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('EWKZ2Jets_ZToLL', '/EWKZ2Jets_ZToLL_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))

            # W + Jets LO
            tasks.append(('WJetsToLNu-LO', '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('W1JetsToLNu-LO', '/W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))
            tasks.append(('W2JetsToLNu-LO', '/W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))
            tasks.append(('W3JetsToLNu-LO', '/W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))
            tasks.append(('W4JetsToLNu-LO', '/W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))

            # W + Jets NLO
            tasks.append(('WJetsToLNu_0J-NLO', '/WJetsToLNu_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('WJetsToLNu_1J-NLO', '/WJetsToLNu_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))
            tasks.append(('WJetsToLNu_2J-NLO', '/WJetsToLNu_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))
            tasks.append(('WJetsToLNu-NLO', '/WJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))

            # ttbar
            tasks.append(('TTTo2L2Nu', '/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('TTToHadronic', '/TTToHadronic_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('TTToSemiLeptonic', '/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))

            # Split diboson - missing files commented out 
            #tasks.append(('WZTo1L3Nu', ''))
            #tasks.append(('WZTo1L1Nu2Q', ''))
            tasks.append(('WZTo2Q2L', '/WZTo2Q2L_mllmin4p0_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('WZTo3LNu', '/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            #tasks.append(('WWTo1L1Nu2Q', ''))
            tasks.append(('WWTo2L2Nu', '/WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('ZZTo2L2Nu', '/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))
            tasks.append(('ZZTo4L', '/ZZTo4L_M-1toInf_TuneCP5_13TeV_powheg_pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))

            # Inclusive diboson
            tasks.append(('WW', '/WW_TuneCP5_13TeV-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('WZ', '/WZ_TuneCP5_13TeV-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('ZZ', '/ZZ_TuneCP5_13TeV-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
 
            # Triboson
            tasks.append(('WWZ', '/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('WWZ-ext1', '/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13_ext1-v2/MINIAODSIM'))
            tasks.append(('WZZ', '/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('WZZ-ext1', '/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13_ext1-v2/MINIAODSIM'))
            tasks.append(('WWW', '/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('WWW-ext1', '/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13_ext1-v2/MINIAODSIM'))
            tasks.append(('ZZZ', '/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('ZZZ-ext1', '/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13_ext1-v2/MINIAODSIM'))

            # Other backgrounds
            tasks.append(('WGToLNuG', '/WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('Tbar-t', '/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v3/MINIAODSIM'))
            tasks.append(('Tbar-tW', '/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))
            tasks.append(('T-t', '/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v3/MINIAODSIM'))
            tasks.append(('T-tW', '/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1/MINIAODSIM'))

            # SM Higgs
            tasks.append(('GluGluHToTauTau_M125', '/GluGluHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))
            tasks.append(('VBFHToTauTau_M125', '/VBFHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v3/MINIAODSIM'))
            tasks.append(('WminusHToTauTau_M125', '/WminusHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v3/MINIAODSIM'))
            tasks.append(('WplusHToTauTau_M125', '/WplusHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v3/MINIAODSIM'))
            tasks.append(('ZHToTauTau_M125', '/ZHToTauTau_M125_CP5_13TeV-powheg-pythia8_ext1/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v3/MINIAODSIM'))
            tasks.append(('ttHToTauTau_M125', '/ttHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'))

          elif yr == "2017":

            # Drell-Yan LO
            tasks.append(('DYJetsToLL-LO', '/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('DY1JetsToLL-LO', '/DY1JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('DY2JetsToLL-LO', '/DY2JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('DY3JetsToLL-LO', '/DY3JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('DY4JetsToLL-LO', '/DY4JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))

            # Low mass Drell-Yan LO
            tasks.append(('DYJetsToLL_M-10to50-LO', '/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('DY1JetsToLL_M-10to50-LO', '/DY1JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('DY2JetsToLL_M-10to50-LO', '/DY2JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('DY3JetsToLL_M-10to50-LO', '/DY3JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('DY4JetsToLL_M-10to50-LO', '/DY4JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))

            # Drell-Yan NLO
            tasks.append(('DYJetsToLL_0J-NLO', '/DYJetsToLL_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('DYJetsToLL_1J-NLO', '/DYJetsToLL_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('DYJetsToLL_2J-NLO', '/DYJetsToLL_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))

            # Electroweak W and Z
            tasks.append(('EWKWMinus2Jets_WToLNu', '/EWKWMinus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('EWKWPlus2Jets_WToLNu', '/EWKWPlus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('EWKZ2Jets_ZToLL', '/EWKZ2Jets_ZToLL_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))

            # W + Jets LO
            tasks.append(('WJetsToLNu-LO', '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('W1JetsToLNu-LO', '/W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('W2JetsToLNu-LO', '/W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('W3JetsToLNu-LO', '/W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('W4JetsToLNu-LO', '/W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))

            # W + Jets NLO
            tasks.append(('WJetsToLNu_0J-NLO', '/WJetsToLNu_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('WJetsToLNu_1J-NLO', '/WJetsToLNu_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('WJetsToLNu_2J-NLO', '/WJetsToLNu_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))

            # ttbar
            tasks.append(('TTTo2L2Nu', '/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('TTToHadronic', '/TTToHadronic_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('TTToSemiLeptonic', '/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))

            # Split diboson - missing files commented out
            #tasks.append(('WZTo1L3Nu', ''))
            #tasks.append(('WZTo1L1Nu2Q', ''))
            #tasks.append(('WZTo2L2Q', ''))
            tasks.append(('WZTo3LNu', '/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v3/MINIAODSIM'))
            #tasks.append(('WWTo1L1Nu2Q', ''))
            tasks.append(('WWTo2L2Nu', '/WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('ZZTo2L2Nu', '/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('ZZTo4L', '/ZZTo4L_TuneCP5_13TeV_powheg_pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))

            # Inclusive diboson
            tasks.append(('WW', '/WW_TuneCP5_13TeV-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('WZ', '/WZ_TuneCP5_13TeV-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('ZZ', '/ZZ_TuneCP5_13TeV-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))

            # Triboson
            tasks.append(('WWZ', '/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('WWZ-ext1', '/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6_ext1-v2/MINIAODSIM'))
            tasks.append(('WZZ', '/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('WZZ-ext1', '/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6_ext1-v2/MINIAODSIM'))
            tasks.append(('WWW', '/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('WWW-ext1', '/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6_ext1-v2/MINIAODSIM'))
            tasks.append(('ZZZ', '/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('ZZZ-ext1', '/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6_ext1-v2/MINIAODSIM'))

            # Other backgrounds
            tasks.append(('WGToLNuG', '/WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('Tbar-t', '/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('Tbar-tW', '/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('T-t', '/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('T-tW', '/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))

            # SM Higgs
            tasks.append(('GluGluHToTauTau_M125', '/GluGluHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v3/MINIAODSIM'))
            tasks.append(('VBFHToTauTau_M125', '/VBFHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('WminusHToTauTau_M125', '/WminusHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('WplusHToTauTau_M125', '/WplusHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'))
            tasks.append(('ZHToTauTau_M125', '/ZHToTauTau_M125_CP5_13TeV-powheg-pythia8_ext1/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'))
            tasks.append(('ttHToTauTau_M125', '/ttHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v3/MINIAODSIM'))

          elif yr == "2018":

            # Drell-Yan LO
            tasks.append(('DYJetsToLL-LO', '/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('DY1JetsToLL-LO', '/DY1JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('DY2JetsToLL-LO', '/DY2JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('DY3JetsToLL-LO', '/DY3JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('DY4JetsToLL-LO', '/DY4JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))

            # Low mass Drell-Yan LO
            tasks.append(('DYJetsToLL_M-10to50-LO', '/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('DY1JetsToLL_M-10to50-LO', '/DY1JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('DY2JetsToLL_M-10to50-LO', '/DY2JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('DY3JetsToLL_M-10to50-LO', '/DY3JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('DY4JetsToLL_M-10to50-LO', '/DY4JetsToLL_M-10to50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))

            # Drell-Yan NLO
            tasks.append(('DYJetsToLL_0J-NLO', '/DYJetsToLL_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('DYJetsToLL_1J-NLO', '/DYJetsToLL_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('DYJetsToLL_2J-NLO', '/DYJetsToLL_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))

            # Electroweak W and Z
            tasks.append(('EWKWMinus2Jets_WToLNu', '/EWKWMinus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('EWKWPlus2Jets_WToLNu', '/EWKWPlus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('EWKZ2Jets_ZToLL', '/EWKZ2Jets_ZToLL_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))

            # W + Jets LO
            tasks.append(('WJetsToLNu-LO', '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('W1JetsToLNu-LO', '/W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('W2JetsToLNu-LO', '/W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('W3JetsToLNu-LO', '/W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('W4JetsToLNu-LO', '/W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))

            # W + Jets NLO
            tasks.append(('WJetsToLNu_0J-NLO', '/WJetsToLNu_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('WJetsToLNu_1J-NLO', '/WJetsToLNu_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('WJetsToLNu_2J-NLO', '/WJetsToLNu_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('WJetsToLNu-NLO', '/WJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))

            # ttbar
            tasks.append(('TTTo2L2Nu', '/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('TTToHadronic', '/TTToHadronic_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('TTToSemiLeptonic', '/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))

            # Split diboson - missing files commented out
            #tasks.append(('WZTo1L3Nu', ''))
            tasks.append(('WZTo1L1Nu2Q', '/WZTo1L1Nu2Q_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            #tasks.append(('WZTo2L2Q', ''))
            tasks.append(('WZTo3LNu', '/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v3/MINIAODSIM'))
            tasks.append(('WWTo1L1Nu2Q', '/WWTo1L1Nu2Q_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('WWTo2L2Nu', '/WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('ZZTo2L2Nu', '/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('ZZTo4L', '/ZZTo4L_TuneCP5_13TeV_powheg_pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))

            # Inclusive diboson
            tasks.append(('WW', '/WW_TuneCP5_13TeV-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('WZ', '/WZ_TuneCP5_13TeV-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('ZZ', '/ZZ_TuneCP5_13TeV-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))

            # Triboson
            tasks.append(('WWZ', '/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('WWZ-ext1', '/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1_ext1-v2/MINIAODSIM'))
            tasks.append(('WZZ', '/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('WZZ-ext1', '/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1_ext1-v2/MINIAODSIM'))
            tasks.append(('WWW', '/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('WWW-ext1', '/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1_ext1-v2/MINIAODSIM'))
            tasks.append(('ZZZ', '/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('ZZZ-ext1', '/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1_ext1-v2/MINIAODSIM'))

            # Other backgrounds
            tasks.append(('WGToLNuG', '/WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('Tbar-t', '/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('Tbar-tW', '/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('T-t', '/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))
            tasks.append(('T-tW', '/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'))

            # SM Higgs
            tasks.append(('GluGluHToTauTau_M125', '/GluGluHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v3/MINIAODSIM'))
            tasks.append(('VBFHToTauTau_M125', '/VBFHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('WminusHToTauTau_M125', '/WminusHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('WplusHToTauTau_M125', '/WplusHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'))
            tasks.append(('ttHToTauTau_M125', '/ttHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v3/MINIAODSIM'))
    
        for task in tasks:
            print(task[0])
            config.General.requestName = task[0]
            config.Data.inputDataset = task[1]
    
            config.Data.splitting = 'FileBased'
            config.Data.unitsPerJob = 1
            config.JobType.pyCfgParams = cfgParams
    
            config.Data.userInputFiles = None
    
            print(config)
    
            #p = Process(target=submit, args=(config,))
            #p.start()
            #p.join()
