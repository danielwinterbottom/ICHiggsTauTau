from WMCore.Configuration import Configuration
prod ='Oct09'     #!!TO BE UPDATED ON EACH PROCESSING
config = Configuration()
config.section_('General')
config.section_('Data')
config.section_('JobType')
config.section_('User')
config.section_('Site')
config.General.transferOutputs = True
config.General.workArea=prod+'/MET'
config.Data.outLFNDirBase='/store/user/pdunne/'+prod+'_MET/'
config.JobType.psetName = '/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_12/src/UserCode/ICHiggsTauTau/test/higgsinv_7_4_12_miniAODcfg.py' #!!NB: THIS IS A LOCAL PATH WHICH DIFFERS FOR EACH USER
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['isData=1', 'release=74XMINIAOD', 'globalTag=74X_dataRun2_Prompt_v2']    #!!TO BE CHECKED ON EACH PROCESSING
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 1
config.Data.splitting = 'FileBased'
#config.Data.unitsPerJob = 30000
#config.Data.splitting = 'EventAwareLumiBased'
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
    json='/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_12/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_246908-258159_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
    firstrun='246908'
    maxrun='258159'#!!remember to update max run
    lastrerecorun='258158'
    firstnonrerecorun='258159'

    #Entries below are of the form:
    #tasks.append((taskname,dataset name from das,lumi mask,run range))

    #Run B
    #tasks.append(('MET-2015B-PromptReco-v1','/MET/Run2015B-PromptReco-v1/MINIAOD','/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_6/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt',firstnonrerecorun+'-'+maxrun))
    #tasks.append(('SingleMuon-2015B-PromptReco-v1','/SingleMuon/Run2015B-PromptReco-v1/MINIAOD','/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_6/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt',firstnonrerecorun+'-'+maxrun))
    #tasks.append(('MET-2015B-17Jul2015-v1','/MET/Run2015B-17Jul2015-v1/MINIAOD','/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_6/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt',firstrun+'-'+maxrun))
    #tasks.append(('SingleMuon-2015B-17Jul2015-v1','/SingleMuon/Run2015B-17Jul2015-v1/MINIAOD','/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_6/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt',firstrun+'-'+maxrun))

    #Run D
    #!!For AM: prompt reco v3 needs replacing with the rereco in giovanni's email
    tasks.append(('MET-2015D-PromptReco-v3','/MET/Run2015D-PromptReco-v3/MINIAOD',json,firstrun+'-'+lastrerecorun))
    tasks.append(('SingleMuon-2015D-PromptReco-v3','/SingleMuon/Run2015D-PromptReco-v3/MINIAOD',json,firstrun+'-'+lastrerecorun))
    tasks.append(('SinglePhoton-2015D-PromptReco-v3','/SinglePhoton/Run2015D-PromptReco-v3/MINIAOD',json,firstrun+'-'+lastrerecorun))

    #Prompt Reco v4
    tasks.append(('MET-2015D-PromptReco-v4','/MET/Run2015D-PromptReco-v4/MINIAOD',json,firstnonrerecorun+'-'+maxrun))
    tasks.append(('SingleMuon-2015D-PromptReco-v4','/SingleMuon/Run2015D-PromptReco-v4/MINIAOD',json,firstnonrerecorun+'-'+maxrun))
    tasks.append(('SinglePhoton-2015D-PromptReco-v4','/SinglePhoton/Run2015D-PromptReco-v4/MINIAOD',json,firstnonrerecorun+'-'+maxrun))
    
    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        config.Data.lumiMask = task[2]
        config.Data.runRange= task[3]
        submit(config)
