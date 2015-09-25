from WMCore.Configuration import Configuration
prod ='Aug18'
config = Configuration()
config.section_('General')
config.section_('Data')
config.section_('JobType')
config.section_('User')
config.section_('Site')
config.General.transferOutputs = True
config.General.workArea=prod+'/MET'
config.Data.outLFNDirBase='/store/user/pdunne/'+prod+'_MET/'
#config.General.requestName = 'May13_MC'
config.JobType.psetName = '/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_6/src/UserCode/ICHiggsTauTau/test/higgsinv_7_4_6_miniAODcfg.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['isData=1', 'release=74XMINIAOD', 'globalTag=74X_dataRun2_Prompt_v2']
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 30000
config.Data.splitting = 'EventAwareLumiBased'
#config.Data.unitsPerJob = 80
#config.Data.splitting = 'LumiBased'
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

    firstrun='246908'
    maxrun='251883'#!!remember to update max run
    firstnonrerecorun='251563'

    tasks.append(('MET-2015B-PromptReco-v1','/MET/Run2015B-PromptReco-v1/MINIAOD','/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_6/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt',firstnonrerecorun+'-'+maxrun))

    tasks.append(('SingleMuon-2015B-PromptReco-v1','/SingleMuon/Run2015B-PromptReco-v1/MINIAOD','/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_6/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt',firstnonrerecorun+'-'+maxrun))

    tasks.append(('MET-2015B-17Jul2015-v1','/MET/Run2015B-17Jul2015-v1/MINIAOD','/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_6/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt',firstrun+'-'+maxrun))

    tasks.append(('SingleMuon-2015B-17Jul2015-v1','/SingleMuon/Run2015B-17Jul2015-v1/MINIAOD','/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_4_6/src/UserCode/ICHiggsTauTau/test/higgsinvcrab/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt',firstrun+'-'+maxrun))
    
    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        config.Data.lumiMask = task[2]
        config.Data.runRange= task[3]
        submit(config)
