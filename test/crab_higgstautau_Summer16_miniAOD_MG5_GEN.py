from WMCore.Configuration import Configuration
from multiprocessing import Process

config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Nov01_MC_80X'
config.section_('JobType')
config.JobType.psetName = 'nonujets.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['globalTag=94X_mc2017_realistic_v14','LHEWeights=True']
config.section_('Data')
config.Data.unitsPerJob = 100000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.outLFNDirBase='/store/user/dwinterb/Nov01_MC_80X/'
config.Data.allowNonValidInputDataset = True
config.Data.inputDBS = 'phys03'
config.section_('User')
config.section_('Site')
config.Site.storageSite = 'T2_UK_London_IC'
config.JobType.allowUndistributedCMSSW = True
#config.Data.ignoreLocality      = True
#config.Site.whitelist   = ['T2_*','T1_*','T3_*']


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
    
    #tasks.append(('GluGluToMaxmixHToTauTauPlusTwoJets_M125_MG5', '/GluGluToMaxmixHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/dwinterb-GluGluToMaxmixHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_RAWSIMoutput-8a1443d17ef14ab810831af8445dae41/USER'))
    #tasks.append(('GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_MG5', '/GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/dwinterb-GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_RAWSIMoutput-16b8dfa7c74a26ff948e04e2fa797e4f/USER'))
    #tasks.append(('GluGluToHToTauTauPlusTwoJets_M125_MG5', '/GluGluToHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/dwinterb-GluGluToHToTauTauPlusTwoJets_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_RAWSIMoutput-3fddf88cffa049c8e57b67766e1f5d89/USER'))
    #tasks.append(('GluGluToHToTauTau_M125_MG5', '/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/dwinterb-GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_RAWSIMoutput-b1ea9780b33c7d29f590ee223f44e204/USER'))

    #tasks.append(('GluGluToMaxmixH_M125_MG5', '/GluGluToMaxmixHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/dwinterb-GluGluToMaxmixHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_RAWSIMoutput-e86d1e82b62375fdc554cd124751c94b/USER', 'GluGluToMaxmixHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-PUMIX'))
    #tasks.append(('GluGluToPseudoscalarH_M125_MG5', '/GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/dwinterb-GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_RAWSIMoutput-3775361e318a9cdb2769aa4cddbc5339/USER', 'GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-PUMIX'))

    #tasks.append(('GluGluToHToTauTau_M125_MG5_PSUp', '/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_PSUp/dwinterb-GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_PSUp_RAWSIMoutput-eeab8075792abc78c0341fbf1f48aed7/USER'))
    #tasks.append(('GluGluToHToTauTau_M125_MG5_PSDown', '/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_PSDown/dwinterb-GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_PSDown_RAWSIMoutput-fd6f13124705d2da220491ecc332122c/USER'))

#tasks.append(('GluGluToPseudoscalarHToTauTau_M125_MG5_2016', '/GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2016-GEN/dwinterb-GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2016-GEN_RAWSIMoutput-3fa654e8a8bdbd48dc7b69a77be7238d/USER'))

 #   tasks.append(('GluGluToHToTauTau_M125_MG5_2016', '/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2016-GEN/dwinterb-GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2016-GEN_RAWSIMoutput-49464db14bc113525fbdfe2a7aee8889/USER'))

  #  tasks.append(('GluGluToMaxmixHToTauTau_M125_MG5_2016', '/GluGluToMaxmixHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2016-GEN/dwinterb-GluGluToMaxmixHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2016-GEN_RAWSIMoutput-c288e5b87904e9278cf7f18297debdb0/USER'))

    #tasks.append(('GluGluToMaxmixHToTauTau_M125', '/GluGluToMaxmixHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/dwinterb-GluGluToMaxmixHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_RAWSIMoutput-a61070e9962bb425fb463729d64f3fc5/USER'))

    #tasks.append(('GluGluToPseudoscalarHToTauTau_M125', '/GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN/dwinterb-GluGluToPseudoscalarHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_RAWSIMoutput-7a400204b909263974ed232b04cbe2e4/USER'))


    tasks.append(('GluGluToHToTauTau_M125_amcatnloFXFX-PSDown','/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_PSDown-v3/dwinterb-GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_PSDown-v3_RAWSIMoutput-6e298ea083d0eebe56346d6870e3135a/USER'))
    
    tasks.append(('GluGluToHToTauTau_M125_amcatnloFXFX-PSUp','/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_PSUp-v2/dwinterb-GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_PSUp-v2_RAWSIMoutput-476f4a3813227a11a1f427973a8688d7/USER'))
    
    tasks.append(('GluGluToHToTauTau_M125_amcatnloFXFX-UEUp','/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_UEUp/dwinterb-GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_UEUp_RAWSIMoutput-b0deed09aed4ccfdd54e98948bcc85d6/USER'))
    
    tasks.append(('GluGluToHToTauTau_M125_amcatnloFXFX-UEDown','/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_UEDown/dwinterb-GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8_2017-GEN_UEDown_RAWSIMoutput-47424eeb9b0ce1a4728b7b244afa8ae8/USER'))


    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]
        #submit(config)

        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



