from WMCore.Configuration import Configuration
prod ='160213'     #!!TO BE UPDATED ON EACH PROCESSING
config = Configuration()
config.section_('General')
config.section_('Data')
config.section_('JobType')
config.section_('User')
config.section_('Site')
config.General.transferOutputs = True
config.General.workArea=prod+'/DATA'
config.Data.outLFNDirBase='/store/user/amagnan/'+prod+'_DATA/'
#config.JobType.psetName = '/vols/cms04/pjd12/invcmssws/run2ntuple/ammetwork/CMSSW_7_4_14/src/UserCode/ICHiggsTauTau/test/higgsinv_7_4_14_miniAODcfg.py' #!!NB: THIS IS A LOCAL PATH WHICH DIFFERS FOR EACH USER
config.JobType.psetName = '/afs/cern.ch/work/a/amagnan/CMSSW_7_6_3_patch2/src/UserCode/ICHiggsTauTau/test/higgsinv_7_6_3_miniAODcfg.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 30000
config.Data.splitting = 'EventAwareLumiBased'
#config.Data.unitsPerJob = 1
#config.Data.splitting = 'FileBased'
#config.Data.unitsPerJob = 80
#config.Data.splitting = 'LumiBased'
config.Data.publication = False
config.Site.whitelist = ['T2_UK_London_IC', 'T2_CH_CERN', 'T2_FR_GRIF_LLR', 'T2_UK_SGrid_Bristol', 'T3_US_FNALLPC', 'T2_DE_DESY', 'T2_IT_Bari', 'T2_BE_IIHE', 'T2_US_UCSD', 'T2_US_MIT', 'T2_IT_Pisa', 'T2_US_Wisconsin', 'T2_US_Florida', 'T2_IT_Rome','T2_FR_IPHC','T2_US_Purdue','T2_IT_Legnaro','T2_FR_GRIF_IRFU']
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
    #json='/vols/cms04/pjd12/invcmssws/run2ntuple/ammetwork/CMSSW_7_4_14/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_246908-258750_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
    #json= '/afs/cern.ch/work/a/amagnan/CMSSW_7_6_3_patch2/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt'
    json= '/afs/cern.ch/work/a/amagnan/CMSSW_7_6_3_patch2/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON.txt'
    firstrun='246908'
    maxrun='260627'#!!remember to update max run
    #lastrerecorun='258158'
    #firstnonrerecorun='258159'

    #Entries below are of the form:
    #tasks.append((taskname,dataset name from das,lumi mask,run range))

    #Run B
    #tasks.append(('MET-2015B-PromptReco-v1','/MET/Run2015B-PromptReco-v1/MINIAOD','/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_6/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt',firstnonrerecorun+'-'+maxrun))
    #tasks.append(('SingleMuon-2015B-PromptReco-v1','/SingleMuon/Run2015B-PromptReco-v1/MINIAOD','/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_6/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt',firstnonrerecorun+'-'+maxrun))
    #tasks.append(('MET-2015B-17Jul2015-v1','/MET/Run2015B-17Jul2015-v1/MINIAOD','/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_6/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt',firstrun+'-'+maxrun))
    #tasks.append(('SingleMuon-2015B-17Jul2015-v1','/SingleMuon/Run2015B-17Jul2015-v1/MINIAOD','/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_6/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt',firstrun+'-'+maxrun))

    #!!To be checked on each processing
    rerecoparams=['isData=1','doHT=0','release=76XMINIAOD', 'globalTag=76X_dataRun2_v15']
#    promptrecov4params=['isData=1', 'release=76XMINIAOD', 'globalTag=74X_dataRun2_Prompt_v4']

    #Run D
    tasks.append(('MET-2015D-16Dec-v1','/MET/Run2015D-16Dec2015-v1/MINIAOD',json,rerecoparams,firstrun+'-'+maxrun))
    tasks.append(('SingleMuon-2015D-16Dec-v1','/SingleMuon/Run2015D-16Dec2015-v1/MINIAOD',json,rerecoparams,firstrun+'-'+maxrun))
    tasks.append(('SinglePhoton-2015D-16Dec-v1','/SinglePhoton/Run2015D-16Dec2015-v1/MINIAOD',json,rerecoparams,firstrun+'-'+maxrun))

    #Prompt Reco v4
    #tasks.append(('MET-2015D-PromptReco-v4','/MET/Run2015D-PromptReco-v4/MINIAOD',json,promptrecov4params,firstnonrerecorun+'-'+maxrun))
    #tasks.append(('SingleMuon-2015D-PromptReco-v4','/SingleMuon/Run2015D-PromptReco-v4/MINIAOD',json,promptrecov4params,firstnonrerecorun+'-'+maxrun))
    #tasks.append(('SinglePhoton-2015D-PromptReco-v4','/SinglePhoton/Run2015D-PromptReco-v4/MINIAOD',json,promptrecov4params,firstnonrerecorun+'-'+maxrun))
    
    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        config.Data.lumiMask = task[2]
        config.JobType.pyCfgParams = task[3] 

        config.Data.runRange= task[4]
        submit(config)
