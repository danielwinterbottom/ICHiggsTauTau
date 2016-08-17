from WMCore.Configuration import Configuration
prod ='160801'     #!!TO BE UPDATED ON EACH PROCESSING
config = Configuration()

config.section_('General')
config.section_('Data')
config.section_('JobType')
config.section_('User')
config.section_('Site')

config.General.transferOutputs = True
config.General.workArea=prod+'/MC'

config.Data.outLFNDirBase='/store/user/rdimaria/'+prod+'_MC/'

config.JobType.psetName ='/home/hep/rd1715/CMSSW_8_0_12/src/UserCode/ICHiggsTauTau/test/higgsinv_8_0_12_miniAODcfg.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']

config.Data.unitsPerJob  = 30000
config.Data.splitting    = 'EventAwareLumiBased'
config.Data.inputDataset = True # Allow CRAB to run over (the valid files of) the input dataset given in Data.inputDataset even if its status in DBS is not VALID. Defaults to False.
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


    ### In PRODUCTION - finished
    tasks.append(('WJetsToLNu-mg-ht100to200','/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v3/MINIAODSIM'))
    tasks.append(('QCD-mg-VBFFilter','/QCD_Inclusive_VBFFilter_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_VBFPostMGFilter_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'))

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
        
        if "DYJetsToLL_M-50_Tune" in task[1]:
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
        if "WJetsToLNu_Tune" in task[1]:
          config.JobType.pyCfgParams = ['isData=0','doHT=1','release=80XMINIAOD','globalTag=80X_mcRun2_asymptotic_2016_miniAODv2_v1']
        submit(config)
