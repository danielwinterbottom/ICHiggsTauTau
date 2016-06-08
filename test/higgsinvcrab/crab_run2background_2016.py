from WMCore.Configuration import Configuration
prod='160607'     #!!TO BE UPDATED ON EACH PROCESSING
config = Configuration()

config.section_('General')
config.section_('Data')
config.section_('JobType')
config.section_('User')
config.section_('Site')

config.General.transferOutputs = True
config.General.workArea=prod+'/MC'

config.Data.outLFNDirBase='/store/user/rdimaria/'+prod+'_MC/'

config.JobType.psetName ='/home/hep/rd1715/CMSSW_8_0_6/src/UserCode/ICHiggsTauTau/test/higgsinv_8_0_6_miniAODcfg.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']

config.Data.unitsPerJob = 30000
config.Data.splitting = 'EventAwareLumiBased'
#config.Data.inputDataset = 'DUMMY'
#config.Data.unitsPerJob = 1
#config.Data.splitting = 'FileBased'

config.JobType.pyCfgParams = ['isData=0','doHT=0','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v0']   #!!TO BE CHECKED ON EACH PROCESSING

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

    #Signal nominal 25ns running conditions
    tasks.append(('Powheg-VBFHtoinv-mH110-v1','/VBF_HToInvisible_M110_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH110-v2','/VBF_HToInvisible_M110_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM')
    tasks.append(('Powheg-VBFHtoinv-mH125-v1','/VBF_HToInvisible_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH125-v2','/VBF_HToInvisible_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH150-v1','/VBF_HToInvisible_M150_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH150-v2','/VBF_HToInvisible_M150_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH200-v1','/VBF_HToInvisible_M200_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH200-v2','/VBF_HToInvisible_M200_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH300-v1','/VBF_HToInvisible_M300_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH300-v2','/VBF_HToInvisible_M300_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH400-v1','/VBF_HToInvisible_M400_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH400-v2','/VBF_HToInvisible_M400_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH500-v1','/VBF_HToInvisible_M500_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    
    tasks.append(('Powheg-VBFHtoinv-mH600-v1','/VBF_HToInvisible_M600_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH600-v2','/VBF_HToInvisible_M600_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))


    tasks.append(('Powheg-GluGluHtoinv-mH110-v1','/GluGlu_HToInvisible_M110_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH110-v2','/GluGlu_HToInvisible_M110_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))    
    tasks.append(('Powheg-GluGluHtoinv-mH125-v1','/GluGlu_HToInvisible_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH125-v2','/GluGlu_HToInvisible_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))    
    tasks.append(('Powheg-GluGluHtoinv-mH150-v1','/GluGlu_HToInvisible_M150_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH150-v2','/GluGlu_HToInvisible_M150_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))    
    tasks.append(('Powheg-GluGluHtoinv-mH200-v1','/GluGlu_HToInvisible_M200_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH200-v2','/GluGlu_HToInvisible_M200_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))    
    tasks.append(('Powheg-GluGluHtoinv-mH300-v1','/GluGlu_HToInvisible_M300_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH300-v2','/GluGlu_HToInvisible_M300_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))    
    tasks.append(('Powheg-GluGluHtoinv-mH400-v1','/GluGlu_HToInvisible_M400_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH400-v2','/GluGlu_HToInvisible_M400_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v2/MINIAODSIM'))    
    tasks.append(('Powheg-GluGluHtoinv-mH500-v1','/GluGlu_HToInvisible_M500_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH500-v2','/GluGlu_HToInvisible_M500_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))    
    tasks.append(('Powheg-GluGluHtoinv-mH600-v1','/GluGlu_HToInvisible_M600_13TeV_powheg_pythia8/RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH600-v2','/GluGlu_HToInvisible_M600_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))


    #VV
    tasks.append(('WW-pythia8-v1','/WW_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('WW-pythia8-v2','/WW_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('WZ-pythia8-v1','/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('WZ-pythia8-v2','/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('ZZ-pythia8-v1','/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v2/MINIAODSIM'))
    tasks.append(('ZZ-pythia8-v2','/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))

    #Top
    tasks.append(('SingleTBar-tW-powheg-inc-v1','/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('SingleTBar-tW-powheg-inc-v2','/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))              
    tasks.append(('SingleT-tW-powheg-inc-v1','/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))              
    tasks.append(('SingleT-tW-powheg-inc-v2','/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v2/MINIAODSIM'))              
    #tasks.append(('SingleT-t-powheg-lep-v1',''))              
    #tasks.append(('SingleT-t-powheg-lep-v2',''))
    #/ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))
    tasks.append(('SingleTBar-t-powheg-lep-v1','/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('SingleTBar-t-powheg-lep-v2','/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('SingleT-s-amcatnlo-lep-v1','/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('SingleT-s-amcatnlo-lep-v2','/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
                  
                  
    /TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext3-v1/MINIAODSIM             
      
             
    /TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext3-v1/MINIAODSIM   
    /TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext4-v1/MINIAODSIM   
  
       
       
        
        
        
        
                  
                  
                  
    tasks.append(('TT-powheg','/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext3-v1/MINIAODSIM'))
    tasks.append(('TTJets-mg','/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))


    #V+jets
    tasks.append(('WJetsToLNu-nlo','/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht100to200','/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht200to400','/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht400to600','/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht600toInf','/WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))


    tasks.append(('DYJetsToLL-mg-m5to50-ht100to200','/DYJetsToLL_M-5to50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m5to50-ht200to400','/DYJetsToLL_M-5to50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m5to50-ht400to600','/DYJetsToLL_M-5to50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m5to50-ht600toInf','/DYJetsToLL_M-5to50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))

    ###tasks.append(('DYJetsToLL-mg-m5to50','/DYJetsToLL_M-5to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'))

    tasks.append(('DYJetsToLL-nlo-m10to50','/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-nlo-m50','/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht100to200','/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht200to400','/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht400to600','/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht600toInf','/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))

    #Z->nunu
    tasks.append(('ZJetsToNuNu-mg-ht100to200','/ZJetsToNuNu_HT-100To200_13TeV-madgraph/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))
    tasks.append(('ZJetsToNuNu-mg-ht200to400','/ZJetsToNuNu_HT-200To400_13TeV-madgraph/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))
    tasks.append(('ZJetsToNuNu-mg-ht400to600','/ZJetsToNuNu_HT-400To600_13TeV-madgraph/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))
    tasks.append(('ZJetsToNuNu-mg-ht600toInf','/ZJetsToNuNu_HT-600ToInf_13TeV-madgraph/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'))

    
    
    
    
    
    
    
    
    
    
    
    #VBF V+2j
    tasks.append(('EWKZ2Jets_ZToLL','/EWKZ2Jets_ZToLL_M-50_13TeV-madgraph-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v2/MINIAODSIM'))
    tasks.append(('EWKWPlus2Jets_WToLNu-v1','/EWKWPlus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('EWKWPlus2Jets_WToLNu-v2','/EWKWPlus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('EWKWMinus2Jets_WToLNu-v1','/EWKWMinus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('EWKWMinus2Jets_WToLNu-v2','/EWKWMinus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v2/MINIAODSIM'))
    tasks.append(('EWKZ2Jets_ZToNuNu-v1','/EWKZ2Jets_ZToNuNu_13TeV-madgraph-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))              
    tasks.append(('EWKZ2Jets_ZToNuNu-v2','/EWKZ2Jets_ZToNuNu_13TeV-madgraph-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))              


    #photon+jets
    tasks.append(('GJets-mg-ht40to100-v1','/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v2/MINIAODSIM'))
    tasks.append(('GJets-mg-ht40to100-v2','/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('GJets-mg-ht100to200-v1','/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v2/MINIAODSIM'))
    tasks.append(('GJets-mg-ht100to200-v2','/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v4/MINIAODSIM'))
    tasks.append(('GJets-mg-ht200to400-v1','/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('GJets-mg-ht200to400-v2','/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('GJets-mg-ht400to600-v1','/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('GJets-mg-ht400to600-v2','/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('GJets-mg-ht600toInf-v1','/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('GJets-mg-ht600toInf-v2','/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    
    
    #QCD
    tasks.append(('QCD-mg-ht100to200','/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))

    tasks.append(('QCD-mg-ht200to300-v1-1','/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht200to300-v1-2','/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/MINIAODSIM'))

    tasks.append(('QCD-mg-ht300to500-v1-1','/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht300to500-v1-2','/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht300to500-v2-1','/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht300to500-v2-2','/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))

    tasks.append(('QCD-mg-ht500to700-v1-1','/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht500to700-v1-2','/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht500to700-v2','/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))

    tasks.append(('QCD-mg-ht700to1000-v1-1','/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht700to1000-v1-2','/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht700to1000-v2-1','/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht700to1000-v2-2','/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))

    tasks.append(('QCD-mg-ht1000to1500-v1-1','/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht1000to1500-v1-2','/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht1000to1500-v2-1','/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v2/MINIAODSIM'))
    tasks.append(('QCD-mg-ht1000to1500-v2-2','/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    
    tasks.append(('QCD-mg-ht1500to2000-v1-1','/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht1500to2000-v1-2','/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht1500to2000-v2-1','/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v3/MINIAODSIM'))
    tasks.append(('QCD-mg-ht1500to2000-v2-2','/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))

    tasks.append(('QCD-mg-ht2000toInf-v1-1','/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht2000toInf-v1-2','/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v2/MINIAODSIM'))
    tasks.append(('QCD-mg-ht2000toInf-v2-1','/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht2000toInf-v2-2','/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))




    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        if "_HT" in task[1]:
            if "Invisible" in task[1]:
                config.JobType.pyCfgParams = ['isData=0','doHT=0','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v0']
            else :
                config.JobType.pyCfgParams = ['isData=0','doHT=1','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v0']
        else:
            config.JobType.pyCfgParams = ['isData=0','doHT=0','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v0']
        submit(config)


