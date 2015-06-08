from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.section_('Data')
config.section_('JobType')
config.section_('User')
config.section_('Site')
config.General.transferOutputs = True
config.General.workArea='May20/MCtry3'
config.Data.outLFNDirBase='/store/user/pdunne/May20_MC/'
#config.General.requestName = 'May13_MC'
config.JobType.psetName = '/vols/cms04/pjd12/invcmssws/run2ntuple/CMSSW_7_2_3/src/UserCode/ICHiggsTauTau/test/higgsinv_7_2_2_miniAODcfg.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['isData=0', 'release=72XMINIAOD']
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 30000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Site.whitelist = ['T2_UK_London_IC', 'T2_CH_CERN', 'T2_FR_GRIF_LLR', 'T2_UK_SGrid_Bristol', 'T3_US_FNALLPC', 'T2_DE_DESY', 'T2_IT_Bari', 'T2_BE_IIHE', 'T2_US_UCSD', 'T2_US_MIT', 'T2_IT_Pisa', 'T2_US_Wisconsin', 'T2_US_Florida', 'T2_IT_Rome','T2_FR_IPHC']
config.Site.storageSite = 'T2_UK_London_IC'

if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand
    from httplib import HTTPException

    # We want to put all the CRAB project directories from the tasks we submit here into one common directory.
    # That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException, hte:
            print hte.headers

    #############################################################################################
    ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
    #############################################################################################

    tasks=list()
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


    tasks.append(('DYJetsToLL','/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'))
    tasks.append(('WJetsToLNu','/WJetsToLNu_13TeV-madgraph-pythia8-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'))
    tasks.append(('TT-pythia','/TT_Tune4C_13TeV-pythia8-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM'))
    tasks.append(('TT-madgraph','/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'))
    tasks.append(('T_tW','/T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'))
    tasks.append(('Tbar_tW','/Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'))
    tasks.append(('WZJetsTo3LNu','/WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'))





    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        submit(config)



