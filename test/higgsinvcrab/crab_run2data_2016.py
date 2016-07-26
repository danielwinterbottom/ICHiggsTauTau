from WMCore.Configuration import Configuration
prod ='160801'     #!!TO BE UPDATED ON EACH PROCESSING
config = Configuration()

config.section_('General')
config.section_('Data')
config.section_('JobType')
config.section_('User')
config.section_('Site')

config.General.transferOutputs = True
config.General.workArea=prod+'/DATA'

config.Data.outLFNDirBase='/store/user/rdimaria/'+prod+'_DATA/'

config.JobType.psetName ='/home/hep/rd1715/CMSSW_8_0_12/src/UserCode/ICHiggsTauTau/test/higgsinv_8_0_12_miniAODcfg.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']

config.Data.unitsPerJob = 30000
config.Data.splitting = 'EventAwareLumiBased'
#config.Data.inputDataset = 'DUMMY'
#config.Data.unitsPerJob = 1
#config.Data.splitting = 'FileBased'
#config.Data.unitsPerJob = 80
#config.Data.splitting = 'LumiBased'

config.Data.publication = False
config.Site.whitelist = ['T2_UK_London_IC','T2_CH_CERN','T2_FR_GRIF_LLR','T2_UK_SGrid_Bristol','T2_DE_DESY','T2_IT_Bari','T2_BE_IIHE','T2_US_UCSD','T2_US_MIT','T2_US_Wisconsin','T2_US_Florida','T2_IT_Rome','T2_FR_IPHC','T2_US_Purdue','T2_IT_Legnaro','T2_FR_GRIF_IRFU','T1_IT_CNAF','T2_BR_SPRACE','T1_UK_RAL','T1_RU_JINR','T2_EE_Estonia','T2_CN_Beijing','T2_RU_JINR','T2_KR_KNU','T1_US_FNAL','T2_IT_Pisa','T2_UK_London_Brunel','T1_DE_KIT','T1_ES_PIC','T1_FR_CCIN2P3','T2_ES_CIEMAT','T2_DE_RWTH','T2_CH_CSCS','T2_BR_UERJ','T2_BE_UCL','T2_IN_TIFR','T2_HU_Budapest','T2_FI_HIP','T2_ES_IFCA','T2_UA_KIPT','T2_TW_NCHC','T2_RU_IHEP','T2_PT_NCG_Lisbon','T2_PL_Swierk','T2_US_Nebraska','T2_US_Caltech','T2_UK_SGrid_RALPP']
config.Site.storageSite = 'T2_UK_London_IC'
config.Data.ignoreLocality = True



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

    #!!TO BE UPDATED ON EACH PROCESSING
    json= '/home/hep/rd1715/CMSSW_8_0_12/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt'
    #firstrun='246908'
    #maxrun='260627'#!!remember to update max run

    #Entries below are of the form:
    #tasks.append((taskname,dataset name from das,lumi mask,run range))

    #!!To be checked on each processing
    rerecoparams=['isData=1','doHT=0','release=80XMINIAOD', 'globalTag=80X_dataRun2_Prompt_ICHEP16JEC_v0'] #to be frequently updated from https://twiki.cern.ch/twiki/bin/view/CMSPublic/

    #Run B
    #           (('MET-2015D-16Dec-v1'           ,'/MET/Run2015D-16Dec2015-v1/MINIAOD'         ,json,rerecoparams,firstrun+'-'+maxrun))
    #           (('SingleMuon-2015D-16Dec-v1'    ,'/SingleMuon/Run2015D-16Dec2015-v1/MINIAOD'  ,json,rerecoparams,firstrun+'-'+maxrun))
    #           (('SinglePhoton-2015D-16Dec-v1'  ,'/SinglePhoton/Run2015D-16Dec2015-v1/MINIAOD',json,rerecoparams,firstrun+'-'+maxrun))
    tasks.append(('MET-2016B-PromptReco-v1'       ,'/MET/Run2016B-PromptReco-v1/MINIAOD'       ,json,rerecoparams,                   ))
    tasks.append(('MET-2016B-PromptReco-v2'       ,'/MET/Run2016B-PromptReco-v2/MINIAOD'       ,json,rerecoparams,                   ))
    tasks.append(('MET-2016C-PromptReco-v2'       ,'/MET/Run2016C-PromptReco-v2/MINIAOD'       ,json,rerecoparams,                   ))
    tasks.append(('MET-2016D-PromptReco-v2'       ,'/MET/Run2016D-PromptReco-v2/MINIAOD'       ,json,rerecoparams,                   ))
    #tasks.append(('MET-2016E-PromptReco-v2'       ,'/MET/Run2016E-PromptReco-v2/MINIAOD'       ,json,rerecoparams,                   ))
    tasks.append(('HTMHT-2016B-PromptReco-v1'     ,'/HTMHT/Run2016B-PromptReco-v1/MINIAOD'     ,json,rerecoparams,                   ))
    tasks.append(('HTMHT-2016B-PromptReco-v2'     ,'/HTMHT/Run2016B-PromptReco-v2/MINIAOD'     ,json,rerecoparams,                   ))
    tasks.append(('HTMHT-2016C-PromptReco-v2'     ,'/HTMHT/Run2016C-PromptReco-v2/MINIAOD'     ,json,rerecoparams,                   ))
    tasks.append(('HTMHT-2016D-PromptReco-v2'     ,'/HTMHT/Run2016D-PromptReco-v2/MINIAOD'     ,json,rerecoparams,                   ))  
    #tasks.append(('HTMHT-2016E-PromptReco-v2'     ,'/HTMHT/Run2016E-PromptReco-v2/MINIAOD'     ,json,rerecoparams,                   ))
    tasks.append(('SingleMuon-2016B-PromptReco-v1','/SingleMuon/Run2016B-PromptReco-v1/MINIAOD',json,rerecoparams,                   ))
    tasks.append(('SingleMuon-2016B-PromptReco-v2','/SingleMuon/Run2016B-PromptReco-v2/MINIAOD',json,rerecoparams,                   ))
    tasks.append(('SingleMuon-2016C-PromptReco-v2','/SingleMuon/Run2016C-PromptReco-v2/MINIAOD',json,rerecoparams,                   ))
    tasks.append(('SingleMuon-2016D-PromptReco-v2','/SingleMuon/Run2016D-PromptReco-v2/MINIAOD',json,rerecoparams,                   ))
    #tasks.append(('SingleMuon-2016E-PromptReco-v2','/SingleMuon/Run2016E-PromptReco-v2/MINIAOD',json,rerecoparams,                   ))

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        config.Data.lumiMask = task[2]
        config.JobType.pyCfgParams = task[3] ##changed from 3 to 2 since commented config.Data.lumiMask

        #config.Data.runRange= task[4]       ##commented to run on the whole dataset
        submit(config)
