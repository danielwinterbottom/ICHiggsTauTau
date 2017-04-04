from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='crab_mc_HLT_VBF'
config.section_('JobType')
config.JobType.psetName = 'customise_mc_cfg.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['outputFULL.root']
#config.JobType.inputFiles = ['Spring16_25nsV3_MC.db']
#config.JobType.pyCfgParams = ['release=80XMINIAOD','isData=0','doHT=1', 'globalTag=80X_mcRun2_asymptotic_2016_TrancheIV_v8', 'isReHLT=1']
config.section_('Data')
#config.Data.inputDataset = 'DUMMY'
config.Data.unitsPerJob = 70
#config.Data.unitsPerJob = 1
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
#config.Data.ignoreLocality= True
config.Data.outLFNDirBase='/store/user/adow'
config.section_('User')
config.section_('Site')
#config.Site.whitelist = ['T2_UK_London_IC', 'T2_CH_CERN', 'T2_FR_GRIF_LLR', 'T2_UK_SGrid_Bristol', 'T3_US_FNALLPC', 'T2_DE_DESY', 'T2_IT_Bari', 'T2_BE_IIHE', 'T2_US_UCSD', 'T2_US_MIT',  'T2_US_Wisconsin', 'T2_US_Florida', 'T2_IT_Rome','T2_FR_IPHC','T2_UK_London_Brunel']
#config.Site.blacklist = ['T2_US_*']
config.Site.storageSite = 'T2_UK_London_IC'
config.JobType.allowUndistributedCMSSW = True

if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand
    from httplib import HTTPException

    # We want to put all the CRAB project directories from the tasks we submit here into one common directory.
    # That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException as hte:
            print "Failed submitting task: %s" % (hte.headers)
#        except ClientException as cle:
#            print "Failed submitting task: %s" % (cle)


    #############################################################################################
    ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
    #############################################################################################

    datasets = {}

    datasets['VBFHToTauTau'] = ('/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM',
                                '/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer16DR80-FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/GEN-SIM-RAW')

    for k, v in datasets.iteritems():
        config.General.requestName = k
        config.Data.inputDataset          = v[0]
        config.Data.secondaryInputDataset = v[1]
        print 'submitting config:'
        print config
        submit(config)

