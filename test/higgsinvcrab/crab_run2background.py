from WMCore.Configuration import Configuration
prod='151030'     #!!TO BE UPDATED ON EACH PROCESSING
config = Configuration()
config.section_('General')
config.section_('Data')
config.section_('JobType')
config.section_('User')
config.section_('Site')
config.General.transferOutputs = True
config.General.workArea=prod+'/MC'
config.Data.outLFNDirBase='/store/user/amagnan/'+prod+'_MC/'
#config.JobType.psetName = '/vols/cms04/pjd12/invcmssws/run2ntuple/ammetwork/CMSSW_7_4_14/src/UserCode/ICHiggsTauTau/test/higgsinv_7_4_14_miniAODcfg.py' #!!NB: THIS IS A LOCAL PATH WHICH DIFFERS FOR EACH USER
config.JobType.psetName = '/afs/cern.ch/work/a/amagnan/CMSSW_7_4_14/src/UserCode/ICHiggsTauTau/test/higgsinv_7_4_14_miniAODcfg.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['isData=0', 'release=74XMINIAOD','globalTag=74X_mcRun2_asymptotic_v2']   #!!TO BE CHECKED ON EACH PROCESSING
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 1
config.Data.splitting = 'FileBased'
#config.Data.unitsPerJob = 30000
#config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Site.whitelist = ['T2_UK_London_IC', 'T2_CH_CERN', 'T2_FR_GRIF_LLR', 'T2_UK_SGrid_Bristol', 'T3_US_FNALLPC', 'T2_DE_DESY', 'T2_IT_Bari', 'T2_BE_IIHE', 'T2_US_UCSD', 'T2_US_MIT', 'T2_IT_Pisa', 'T2_US_Wisconsin', 'T2_US_Florida', 'T2_IT_Rome','T2_FR_IPHC','T2_US_Purdue','T2_IT_Legnaro','T2_FR_GRIF_IRFU']
config.Site.storageSite = 'T2_UK_London_IC'

if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand
    from CRABClient.ClientExceptions import ClientException
    from httplib import HTTPException

    # We want to put all the CRAB project directories from the tasks we submit here into one common directory.
    # That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).

    def submit(inconfig):
        try:
            crabCommand('submit', config = inconfig)
#            crabCommand('status')
        except HTTPException as hte:
            print "Failed submitting task: %s" % (hte.headers)
        except ClientException as cle:
            print "Failed submitting task: %s" % (cle)

    #############################################################################################
    ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
    #############################################################################################

    tasks=list()
    
    #Entries are in the format:
    #tasks.append((taskname,dataset name from das))

    #Miniaod v2: RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1

    #25ns
    
    #Signal nominal 25ns running conditions
    #tasks.append(('Powheg-Htoinv-mH110Asympt25ns','/VBF_HToInvisible_M110_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('Powheg-Htoinv-mH125Asympt25ns','/VBF_HToInvisible_M125_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('Powheg-Htoinv-mH150Asympt25ns','/VBF_HToInvisible_M150_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('Powheg-Htoinv-mH200Asympt25ns','/VBF_HToInvisible_M200_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('Powheg-Htoinv-mH300Asympt25ns','/VBF_HToInvisible_M300_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('Powheg-Htoinv-mH400Asympt25ns','/VBF_HToInvisible_M400_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('Powheg-Htoinv-mH500Asympt25ns','/VBF_HToInvisible_M500_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))

    #VV
    #tasks.append(('WW-pythia8','/WW_TuneCUETP8M1_13TeV-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('WZ-pythia8','/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('ZZ-pythia8','/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM')) 
    #tasks.append(('WW2L2Nu-powheg','/WWTo2L2Nu_13TeV-powheg/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('WW4Q-powheg','/WWTo4Q_13TeV-powheg/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM'))
    #tasks.append(('WWLNuQQ-powheg','/WWToLNuQQ_13TeV-powheg/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))

    #tasks.append(('WZLNuQQ-mg','/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('WZLLQQ-mg','/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('ZZ4L-powheg','/ZZTo4L_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM'))

    #Top
    #tasks.append(('SingleTBar-tW-powheg-inc','/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('SingleT-tW-powheg-inc','/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM'))
    tasks.append(('SingleT-t-powheg-lep','/ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    tasks.append(('SingleTBar-t-powheg-lep','/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('SingleT-s-amcatnlo-lep','/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('TTJets-mg','/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v3/MINIAODSIM'))
    #tasks.append(('TT-powheg','/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    
    #V+jets
    #tasks.append(('WJetsToLNu-nlo','/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('WJetsToLNu-mg','/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('WJetsToLNu-mg-ht100to200','/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('WJetsToLNu-mg-ht200to400','/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('WJetsToLNu-mg-ht400to600','/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('WJetsToLNu-mg-ht600to800','/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('WJetsToLNu-mg-ht800to1200','/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('WJetsToLNu-mg-ht1200to2500','/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('WJetsToLNu-mg-ht2500toInf','/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('WJetsToLNu-mg-ht600toInf','/WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))


    #tasks.append(('DYJetsToLL-mg-m5to50','/DYJetsToLL_M-5to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('DYJetsToLL-mg-m5to50-ht100to200','/DYJetsToLL_M-5to50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('DYJetsToLL-mg-m5to50-ht200to400','/DYJetsToLL_M-5to50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('DYJetsToLL-mg-m5to50-ht400to600','/DYJetsToLL_M-5to50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('DYJetsToLL-mg-m5to50-ht600toInf','/DYJetsToLL_M-5to50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('DYJetsToLL-nlo-m10to50','/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('DYJetsToLL-nlo-m50','/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))

    #tasks.append(('DYJetsToLL-mg-m50','/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('DYJetsToLL-mg-m50-ht100to200','/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('DYJetsToLL-mg-m50-ht200to400','/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('DYJetsToLL-mg-m50-ht400to600','/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM'))
    #tasks.append(('DYJetsToLL-mg-m50-ht600toInf','/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))

    #tasks.append(('DYJetsToLL-mg-Zpt150','/DYJetsToLL_M-50_Zpt-150toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))

    #Z->nunu
    #tasks.append(('ZJetsToNuNu-mg-ht100to200','/ZJetsToNuNu_HT-100To200_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('ZJetsToNuNu-mg-ht200to400','/ZJetsToNuNu_HT-200To400_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('ZJetsToNuNu-mg-ht400to600','/ZJetsToNuNu_HT-400To600_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('ZJetsToNuNu-mg-ht600toInf','/ZJetsToNuNu_HT-600ToInf_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM'))
    
    #photon+jets
    #tasks.append(('GJets-mg-ht40to100','/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('GJets-mg-ht100to200','/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('GJets-mg-ht200to400','/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('GJets-mg-ht400to600','/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('GJets-mg-ht600toInf','/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))

    #QCD
    #tasks.append(('QCD-mg-ht100to200','/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('QCD-mg-ht200to300','/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('QCD-mg-ht300to500','/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('QCD-mg-ht500to700','/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('QCD-mg-ht700to1000','/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('QCD-mg-ht1000to1500','/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('QCD-mg-ht1500to2000','/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
    #tasks.append(('QCD-mg-ht2000toInf','/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))


    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        if "amcatnlo" in task[1]:
            config.JobType.pyCfgParams = ['isData=0', 'release=74XMINIAOD','globalTag=74X_mcRun2_asymptotic_v2','isNLO=True']
        else:
            config.JobType.pyCfgParams = ['isData=0', 'release=74XMINIAOD','globalTag=74X_mcRun2_asymptotic_v2','isNLO=False']
        submit(config)


#    tasks.append(('DYJetsToLL-','/DYJetsToLL_M-100to200_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
#    tasks.append(('DYJetsToLL-','/DYJetsToLL_M-200to400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
#    tasks.append(('DYJetsToLL-','/DYJetsToLL_M-400to500_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
#    tasks.append(('DYJetsToLL-','/DYJetsToLL_M-500to700_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v3/MINIAODSIM'))
#    tasks.append(('DYJetsToLL-','/DYJetsToLL_M-700to800_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
#    tasks.append(('DYJetsToLL-','/DYJetsToLL_M-800to1000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
#    tasks.append(('DYJetsToLL-','/DYJetsToLL_M-1500to2000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))
#    tasks.append(('DYJetsToLL-','/DYJetsToLL_M-2000to3000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))

    #Startup running conditions signal
    #tasks.append(('Powheg-Htoinv-mH125Startupbx50','/VBF_HToInvisible_M125_13TeV_powheg_pythia8/RunIISpring15DR74-StartupFlat10to50bx50Raw_MCRUN2_74_V8-v1/MINIAODSIM'))

    #50ns
    
    #Signal nominal 25ns running conditions
    #!!NOT AVAILABLE AT T2 YET
    #tasks.append(('Powheg-Htoinv-mH110Asympt50ns','/VBF_HToInvisible_M110_13TeV_powheg_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))
    #!!NOT AVAILABLE AT T2 YET

    #tasks.append(('Powheg-Htoinv-mH300Asympt50ns','/VBF_HToInvisible_M300_13TeV_powheg_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))
    #tasks.append(('Powheg-Htoinv-mH400Asympt50ns','/VBF_HToInvisible_M300_13TeV_powheg_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))

    #VV
    #tasks.append(('WW-pythia8-50ns','/WW_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))
    #tasks.append(('WZ-pythia8-50ns','/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))
    #tasks.append(('ZZ-pythia8-50ns','/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM')) 
    #tasks.append(('WW2L2Nu-powheg-50ns','/WWTo2L2Nu_13TeV-powheg/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))
    #tasks.append(('WW4Q-powheg-50ns','/WWTo4Q_13TeV-powheg/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/MINIAODSIM'))
    #tasks.append(('WWLNuQQ-powheg-50ns','/WWToLNuQQ_13TeV-powheg/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))
    #tasks.append(('WZLNuQQ-mg-50ns','/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))
    #tasks.append(('ZZ4L-powheg-50ns','/ZZTo4L_13TeV_powheg_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))

    #QCD

    #Top
    #tasks.append(('SingleTBar-tW-powheg-inc-50ns','/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))
    #tasks.append(('SingleT-tW-powheg-inc-50ns','/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))
    #tasks.append(('SingleT-t-powheg-lep-50ns','/ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))
    #tasks.append(('SingleTBar-t-powheg-lep-50ns','/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))
    #tasks.append(('SingleT-s-amcatnlo-lep-50ns','/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))
    #tasks.append(('TTJets-mg-50ns','/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))
    #tasks.append(('TT-powheg-50ns','/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM'))
    
    #V+jets
    #tasks.append(('WJetsToLNu-50ns','/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))
    #tasks.append(('DYJetsToLL-50ns','/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9-v3/MINIAODSIM'))
    #tasks.append(('DYJetsToLL-M_10to50-50ns','/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'))




    #Old Phys14 samples
    #Signal 125 GeV
#    tasks.append(('Powheg-Htoinv-mH125PU20bx25','/VBF_HToInv_M-125_13TeV_powheg-pythia6/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM'))
#    tasks.append(('Powheg-Htoinv-mH125PU40bx25','/VBF_HToInv_M-125_13TeV_powheg-pythia6/Phys14DR-PU40bx25_PHYS14_25_V1-v2/MINIAODSIM'))
#    tasks.append(('Powheg-Htoinv-mH125PU30bx50','/VBF_HToInv_M-125_13TeV_powheg-pythia6/Phys14DR-AVE30BX50_tsg_PHYS14_ST_V1-v2/MINIAODSIM'))


    
    #QCD
#    tasks.append(('QCD-Pt-30to50-pythia8PU20bx25','/QCD_Pt-30to50_Tune4C_13TeV_pythia8/Phys14DR-AVE20BX25_tsg_castor_PHYS14_25_V3-v2/MINIAODSIM'))
#    tasks.append(('QCD-Pt-50to80-pythia8PU20bx25','/QCD_Pt-50to80_Tune4C_13TeV_pythia8/Phys14DR-AVE20BX25_tsg_castor_PHYS14_25_V3-v1/MINIAODSIM'))
#    tasks.append(('QCD-Pt-80to120-pythia8PU20bx25','/QCD_Pt-80to120_Tune4C_13TeV_pythia8/Phys14DR-AVE20BX25_tsg_castor_PHYS14_25_V3-v1/MINIAODSIM'))
#    tasks.append(('QCD-Pt-120to170-pythia8PU20bx25','/QCD_Pt-120to170_Tune4C_13TeV_pythia8/Phys14DR-AVE20BX25_tsg_castor_PHYS14_25_V3-v1/MINIAODSIM'))
#    tasks.append(('QCD-Pt-170to300-pythia8PU20bx25','/QCD_Pt-170to300_Tune4C_13TeV_pythia8/Phys14DR-AVE20BX25_tsg_castor_PHYS14_25_V3-v1/MINIAODSIM'))
#    tasks.append(('QCD-Pt-300to470-pythia8PU20bx25','/QCD_Pt-300to470_Tune4C_13TeV_pythia8/Phys14DR-AVE20BX25_tsg_castor_PHYS14_25_V3-v1/MINIAODSIM'))
#    tasks.append(('QCD-Pt-470to600-pythia8PU20bx25','/QCD_Pt-470to600_Tune4C_13TeV_pythia8/Phys14DR-AVE20BX25_tsg_castor_PHYS14_25_V3-v1/MINIAODSIM'))
#    tasks.append(('QCD-Pt-600to800-pythia8PU20bx25','/QCD_Pt-600to800_Tune4C_13TeV_pythia8/Phys14DR-AVE20BX25_tsg_castor_PHYS14_25_V3-v1/MINIAODSIM'))
#    tasks.append(('QCD-Pt-800to1000-pythia8PU20bx25','/QCD_Pt-800to1000_Tune4C_13TeV_pythia8/Phys14DR-AVE20BX25_tsg_castor_PHYS14_25_V3-v1/MINIAODSIM'))
#    tasks.append(('QCD-Pt-1000to1400-pythia8PU20bx25','/QCD_Pt-1000to1400_Tune4C_13TeV_pythia8/Phys14DR-AVE20BX25_tsg_castor_PHYS14_25_V3-v1/MINIAODSIM'))
#    tasks.append(('QCD-Pt-1400to1800-pythia8PU20bx25','/QCD_Pt-1400to1800_Tune4C_13TeV_pythia8/Phys14DR-AVE20BX25_tsg_castor_PHYS14_25_V3-v1/MINIAODSIM'))
#    #nb these qcd samples were made with a different global tag
#    tasks.append(('QCD-Pt-1800to2400-pythia8PU20bx25','/QCD_Pt-1800to2400_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_trkalmb_PHYS14_25_V1-v2/MINIAODSIM'))
#    tasks.append(('QCD-Pt-2400to3200-pythia8PU20bx25','/QCD_Pt-2400to3200_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_trkalmb_PHYS14_25_V1-v1/MINIAODSIM'))
#    tasks.append(('QCD-Pt-3200-pythia8PU20bx25','/QCD_Pt-2400to3200_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_trkalmb_PHYS14_25_V1-v1/MINIAODSIM'))

        # tasks.append(('DYJetsToLL','/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'))
        # tasks.append(('WJetsToLNu','/WJetsToLNu_13TeV-madgraph-pythia8-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'))
        # tasks.append(('TT-pythia','/TT_Tune4C_13TeV-pythia8-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM'))
        # tasks.append(('TT-madgraph','/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'))
        # tasks.append(('T_tW','/T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'))
        # tasks.append(('Tbar_tW','/Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'))
        # tasks.append(('WZJetsTo3LNu','/WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'))
