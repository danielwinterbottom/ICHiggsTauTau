from WMCore.Configuration import Configuration
prod ='161031'     #!!TO BE UPDATED ON EACH PROCESSING
config = Configuration()

config.section_('General')
config.section_('Data')
config.section_('JobType')
config.section_('User')
config.section_('Site')

config.General.transferOutputs = True
config.General.workArea=prod+'/MC'

config.Data.outLFNDirBase='/store/user/rdimaria/'+prod+'_MC/'

config.JobType.psetName ='/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/test/higgsinv_8_0_20_miniAODcfg.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']

config.Data.unitsPerJob  = 30000
config.Data.splitting    = 'EventAwareLumiBased'
#config.Data.inputDataset = True # Allow CRAB to run over (the valid files of) the input dataset given in Data.inputDataset even if its status in DBS is not VALID. Defaults to False.
#config.Data.inputDataset = 'DUMMY'
#config.Data.unitsPerJob = 1
#config.Data.splitting = 'FileBased'

config.JobType.pyCfgParams = ['isData=0','doHT=0','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1']   #!!TO BE CHECKED ON EACH PROCESSING

config.Data.publication = False
config.Site.whitelist = ['T2_UK_London_IC','T2_CH_CERN','T2_FR_GRIF_LLR','T2_UK_SGrid_Bristol','T2_DE_DESY','T2_IT_Bari','T2_BE_IIHE','T2_US_UCSD','T2_US_MIT','T2_US_Wisconsin','T2_US_Florida','T2_IT_Rome','T2_FR_IPHC','T2_US_Purdue','T2_IT_Legnaro','T2_FR_GRIF_IRFU','T1_IT_CNAF','T2_BR_SPRACE','T1_UK_RAL','T1_RU_JINR','T2_EE_Estonia','T2_CN_Beijing','T2_RU_JINR','T2_KR_KNU','T1_US_FNAL','T2_IT_Pisa','T2_UK_London_Brunel','T1_DE_KIT','T1_ES_PIC','T1_FR_CCIN2P3','T2_ES_CIEMAT','T2_DE_RWTH','T2_CH_CSCS','T2_BR_UERJ','T2_BE_UCL','T2_IN_TIFR','T2_HU_Budapest','T2_FI_HIP','T2_ES_IFCA','T2_UA_KIPT','T2_TW_NCHC','T2_RU_IHEP','T2_PT_NCG_Lisbon','T2_PL_Swierk','T2_US_Nebraska','T2_US_Caltech','T2_UK_SGrid_RALPP']
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

    # VBF_HToInvisible_
    tasks.append(('Powheg-VBFHtoinv-mH110','/VBF_HToInvisible_M110_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH125','/VBF_HToInvisible_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH150','/VBF_HToInvisible_M150_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH200','/VBF_HToInvisible_M200_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH300','/VBF_HToInvisible_M300_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH400','/VBF_HToInvisible_M400_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH500','/VBF_HToInvisible_M500_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH600','/VBF_HToInvisible_M600_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH800','/VBF_HToInvisible_M800_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-VBFHtoinv-mH1000','/VBF_HToInvisible_M1000_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('Powheg-VBFHtoinv-mH800','/VBF_HToInvisible_M800_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))

    # GluGlu_HToInvisible_
    tasks.append(('Powheg-GluGluHtoinv-mH110','/GluGlu_HToInvisible_M110_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH125','/GluGlu_HToInvisible_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH150','/GluGlu_HToInvisible_M150_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH200','/GluGlu_HToInvisible_M200_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH300','/GluGlu_HToInvisible_M300_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH400','/GluGlu_HToInvisible_M400_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v2/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH500','/GluGlu_HToInvisible_M500_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH600','/GluGlu_HToInvisible_M600_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH800','/GluGlu_HToInvisible_M800_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('Powheg-GluGluHtoinv-mH1000','/GluGlu_HToInvisible_M1000_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('Powheg-GluGluHtoinv-mH125','/GluGlu_HToInvisible_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('Powheg-GluGluHtoinv-mH800','/GluGlu_HToInvisible_M800_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('Powheg-GluGluHtoinv-mH1000','/GluGlu_HToInvisible_M1000_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))

    # VV_
    tasks.append(('WW-pythia8','/WW_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('WZ-pythia8','/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('ZZ-pythia8','/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))

    # Top
    tasks.append(('SingleTBar-tW-powheg-inc','/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('SingleT-tW-powheg-inc','/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v2/MINIAODSIM'))
    tasks.append(('SingleTBar-t-powheg-lep','/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('SingleT-t-powheg-lep','/ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('SingleT-s-amcatnlo-lep','/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('TT-powheg-ext3','/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext3-v2/MINIAODSIM'))
    tasks.append(('TT-powheg-ext4','/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext4-v1/MINIAODSIM'))
    tasks.append(('TTJets-mg','/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('SingleTBar-tW-powheg-inc','/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('SingleTBar-tW-powheg-inc-ext1','/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('SingleT-tW-powheg-inc','/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('SingleT-tW-powheg-inc-ext1','/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('SingleT-s-amcatnlo-lep','/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))



    # VJetsToLNu
    tasks.append(('WJetsToLNu-mg','/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-nlo','/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))

    tasks.append(('WJetsToLNu-mg-ht100to200','/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v3/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht100to200-ext1','/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht200to400','/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht200to400-ext1','/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht400to600','/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht400to600-ext1','/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht600to800','/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht600to800-ext1','/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht800to1200','/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v2/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht800to1200-ext1','/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht1200to2500','/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht1200to2500-ext1','/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht2500toInf','/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-mg-ht2500toInf-ext1','/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))

    tasks.append(('WJetsToLNu-Pt100to250','/WJetsToLNu_Pt-100To250_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-Pt250to400','/WJetsToLNu_Pt-250To400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-Pt400to600','/WJetsToLNu_Pt-400To600_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu-Pt600toInf','/WJetsToLNu_Pt-600ToInf_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))

## Moriond ##    # PROD # /WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM
## Moriond ##    ## NEW ## /WJetsToLNu_Wpt-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
## Moriond ##    ## NEW ## /WJetsToLNu_Wpt-200toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
## Moriond ##    tasks.append(('WJetsToLNu-mg','/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-nlo','/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht70to100','/WJetsToLNu_HT-70To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht100to200','/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht200to400','/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht400to600','/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht600to800','/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht1200to2500','/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht2500toInf','/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht100to200-ext1','/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht200to400-ext1','/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht400to600-ext1','/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht600to800-ext1','/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht800to1200-ext1','/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht1200to2500-ext1','/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht2500toInf-ext1','/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht100to200-ext2','/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-mg-ht200to400-ext2','/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-Pt100to250','/WJetsToLNu_Pt-100To250_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-Pt250to400','/WJetsToLNu_Pt-250To400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-Pt400to600','/WJetsToLNu_Pt-400To600_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-Pt600toInf','/WJetsToLNu_Pt-600ToInf_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-Pt100to250-ext1','/WJetsToLNu_Pt-100To250_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-Pt250to400-ext1','/WJetsToLNu_Pt-250To400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-Pt400to600-ext1','/WJetsToLNu_Pt-400To600_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('WJetsToLNu-Pt600toInf-ext1','/WJetsToLNu_Pt-600ToInf_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))

    # DYJetsToLL
    tasks.append(('DYJetsToLL-mg-m5to50','/DYJetsToLL_M-5to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m5to50-ht100to200','/DYJetsToLL_M-5to50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m5to50-ht100to200-ext1','/DYJetsToLL_M-5to50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m5to50-ht200to400','/DYJetsToLL_M-5to50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m5to50-ht200to400-ext1','/DYJetsToLL_M-5to50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m5to50-ht400to600','/DYJetsToLL_M-5to50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m5to50-ht400to600-ext1','/DYJetsToLL_M-5to50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m5to50-ht600toInf','/DYJetsToLL_M-5to50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m5to50-ht600toInf-ext1','/DYJetsToLL_M-5to50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('DYJetsToLL-mg-m5to50-ht100to200','/DYJetsToLL_M-5to50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('DYJetsToLL-mg-m5to50-ht200to400','/DYJetsToLL_M-5to50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('DYJetsToLL-mg-m5to50-ht400to600','/DYJetsToLL_M-5to50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('DYJetsToLL-mg-m5to50-ht600toInf','/DYJetsToLL_M-5to50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('DYJetsToLL-mg-m5to50-ht100to200-ext1','/DYJetsToLL_M-5to50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('DYJetsToLL-mg-m5to50-ht200to400-ext1','/DYJetsToLL_M-5to50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('DYJetsToLL-mg-m5to50-ht400to600-ext1','/DYJetsToLL_M-5to50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))
## Moriond ##    tasks.append(('DYJetsToLL-mg-m5to50-ht600toInf-ext1','/DYJetsToLL_M-5to50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'))

    tasks.append(('DYJetsToLL-nlo-m10to50','/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m10to50','/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))

    tasks.append(('DYJetsToLL-nlo-m50','/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50','/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht100to200','/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht100to200-ext1','/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht200to400','/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht200to400-ext1','/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht400to600','/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht400to600-ext1','/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht600toInf','/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht600toInf-ext1','/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht600to800','/DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht800to1200','/DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht1200to2500','/DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DYJetsToLL-mg-m50-ht2500toInf','/DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))

    tasks.append(('DY1JetsToLL-mg-m50','/DY1JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DY2JetsToLL-mg-m50','/DY2JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DY3JetsToLL-mg-m50','/DY3JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('DY4JetsToLL-mg-m50','/DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))

    tasks.append(('DYJetsToLL-mg-Zpt150','/DYJetsToLL_M-50_Zpt-150toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))

    # ZJetsToNuNu
    tasks.append(('ZJetsToNuNu-mg-ht100to200','/ZJetsToNuNu_HT-100To200_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('ZJetsToNuNu-mg-ht100to200-ext1','/ZJetsToNuNu_HT-100To200_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('ZJetsToNuNu-mg-ht200to400','/ZJetsToNuNu_HT-200To400_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('ZJetsToNuNu-mg-ht200to400-ext1','/ZJetsToNuNu_HT-200To400_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('ZJetsToNuNu-mg-ht400to600','/ZJetsToNuNu_HT-400To600_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('ZJetsToNuNu-mg-ht400to600-ext1','/ZJetsToNuNu_HT-400To600_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('ZJetsToNuNu-mg-ht600to800','/ZJetsToNuNu_HT-600To800_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('ZJetsToNuNu-mg-ht800to1200','/ZJetsToNuNu_HT-800To1200_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v3/MINIAODSIM'))
    tasks.append(('ZJetsToNuNu-mg-ht1200to2500','/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('ZJetsToNuNu-mg-ht1200to2500-ext1','/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('ZJetsToNuNu-mg-ht2500toInf','/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))

    # EWK*VTo* (VBF V+2Jets)
    tasks.append(('EWKZ2Jets_ZToLL','/EWKZ2Jets_ZToLL_M-50_13TeV-madgraph-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('EWKWPlus2Jets_WToLNu','/EWKWPlus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('EWKWMinus2Jets_WToLNu','/EWKWMinus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v2/MINIAODSIM'))
    tasks.append(('EWKZ2Jets_ZToNuNu','/EWKZ2Jets_ZToNuNu_13TeV-madgraph-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))

    # GJets (Photon+Jets )
    tasks.append(('GJets-mg-ht40to100','/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('GJets-mg-ht100to200','/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v4/MINIAODSIM'))
    tasks.append(('GJets-mg-ht200to400','/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('GJets-mg-ht400to600','/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('GJets-mg-ht600toInf','/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))

    # QCD
    tasks.append(('QCD-mg-ht100to200','/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht200to300','/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht200to300-ext1','/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht300to500','/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht300to500-ext1','/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht500to700','/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht500to700-ext1','/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht700to1000','/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht700to1000-ext1','/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht1000to1500','/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v2/MINIAODSIM'))
    tasks.append(('QCD-mg-ht1000to1500-ext1','/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht1500to2000','/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v3/MINIAODSIM'))
    tasks.append(('QCD-mg-ht1500to2000-ext1','/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht2000toInf','/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))
    tasks.append(('QCD-mg-ht2000toInf-ext1','/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM'))
    #dataset dataset=/QCD_Inclusive*/*RunIISpring16MiniAODv2*/* status=*
    tasks.append(('QCD-mg-VBFFilter','/QCD_Inclusive_VBFFilter_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_VBFPostMGFilter_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))


    ### reHLT
    #tasks.append(('Powheg-VBFHtoinv-mH110-reHLT','/VBF_HToInvisible_M110_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    #tasks.append(('Powheg-VBFHtoinv-mH125-reHLT','/VBF_HToInvisible_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    #tasks.append(('Powheg-VBFHtoinv-mH150-reHLT','/VBF_HToInvisible_M150_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    #tasks.append(('Powheg-VBFHtoinv-mH200-reHLT','/VBF_HToInvisible_M200_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    #tasks.append(('Powheg-VBFHtoinv-mH300-reHLT','/VBF_HToInvisible_M300_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    #tasks.append(('Powheg-VBFHtoinv-mH400-reHLT','/VBF_HToInvisible_M400_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    #tasks.append(('Powheg-VBFHtoinv-mH500-reHLT','/VBF_HToInvisible_M500_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    #tasks.append(('Powheg-VBFHtoinv-mH600-reHLT','/VBF_HToInvisible_M600_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    #tasks.append(('Powheg-VBFHtoinv-mH800-reHLT','/VBF_HToInvisible_M800_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))
    #tasks.append(('Powheg-VBFHtoinv-mH1000-reHLT','/VBF_HToInvisible_M1000_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM'))


    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        if "_HT" in task[1]:
            if "Invisible" in task[1]:
                config.JobType.pyCfgParams = ['isData=0','doHT=0','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1']
            else :
                config.JobType.pyCfgParams = ['isData=0','doHT=1','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1']
        else:
            config.JobType.pyCfgParams = ['isData=0','doHT=0','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1']

        if "QCD_Inclusive_VBFFilter_TuneCUETP8M1" in task[1]:
          config.JobType.pyCfgParams = ['isData=0','doHT=1','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1']
        if "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM" in task[1]:
          config.JobType.pyCfgParams = ['isData=0','doHT=1','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1']
        if "DY1JetsToLL_M-50_Tune" in task[1]:
          config.JobType.pyCfgParams = ['isData=0','doHT=1','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1']
        if "DY2JetsToLL_M-50_Tune" in task[1]:
          config.JobType.pyCfgParams = ['isData=0','doHT=1','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1']
        if "DY3JetsToLL_M-50_Tune" in task[1]:
          config.JobType.pyCfgParams = ['isData=0','doHT=1','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1']
        if "DY4JetsToLL_M-50_Tune" in task[1]:
          config.JobType.pyCfgParams = ['isData=0','doHT=1','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1']
        if "DYJetsToLL_M-50_Zpt" in task[1]:
          config.JobType.pyCfgParams = ['isData=0','doHT=1','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1']
        if "WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM" in task[1]:
          config.JobType.pyCfgParams = ['isData=0','doHT=1','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1']

        if "reHLT" in task[1]:
          config.JobType.pyCfgParams = ['isData=0','isReHLT=1','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1']
        submit(config)
