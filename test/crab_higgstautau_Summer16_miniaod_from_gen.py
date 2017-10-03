from WMCore.Configuration import Configuration
from multiprocessing import Process
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea='Oct04_MC_80X'
config.section_('JobType')
config.JobType.psetName = 'gen_ntuple_from_gen_cfg.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.pyCfgParams = ['output=EventTree.root']
config.section_('Data')
config.Data.unitsPerJob = 50000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.outLFNDirBase='/store/user/dwinterb/Oct04_MC_80X/'
config.section_('User')
config.section_('Site')
config.Site.storageSite = 'T2_UK_London_IC'
config.JobType.allowUndistributedCMSSW = True
config.Data.inputDBS = 'phys03'

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

    
    #tasks.append(('bbH700_OldAcc_1', '/OldAcc_1_M-700_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-gen-prod-260917-6a48e2655320f1c4ee0bceebfe06f692/USER'))
    tasks.append(('bbH700_OldAcc_2', '/OldAcc_2_M-700_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-gen-prod-260917-85bc176885a1e4d46f1e6589f8744c20/USER'))
    tasks.append(('bbH700_OldAcc_3', '/OldAcc_3_M-700_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-gen-prod-260917-59708c9af61dc64d08956a9382fd521e/USER'))
    tasks.append(('bbH700_OldAcc_4', '/OldAcc_4_M-700_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-gen-prod-260917-ba315e886675f9d88d695b5653faef50/USER'))
    tasks.append(('bbH700_OldAcc_5', '/OldAcc_5_M-700_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-gen-prod-260917-b77b395dd8c377392edef7c537abf653/USER'))
   
    tasks.append(('bbH700_NewAcc_1', '/NewAcc_1_M-700_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-gen-prod-260917-d0a0d4088eaa82687d108deeef7bfe3d/USER'))
    tasks.append(('bbH700_NewAcc_2', '/NewAcc_2_M-700_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-gen-prod-260917-3de67ed08388789e896cdf3bc2985e32/USER'))
    tasks.append(('bbH700_NewAcc_3', '/NewAcc_3_M-700_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-gen-prod-260917-e5835917bdefb2bb72d861a4d82a7fc2/USER'))
    tasks.append(('bbH700_NewAcc_4', '/NewAcc_4_M-700_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-gen-prod-260917-cd5e6d6b0f9afe3c5cc82a573ca7480f/USER'))
    tasks.append(('bbH700_NewAcc_5', '/NewAcc_5_M-700_TuneCUETP8M1_13TeV-amcatnlo-pythia8/agilbert-gen-prod-260917-a46746efff1a2cf8298c73243db4fcab/USER'))
    

    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]

        p = Process(target=submit, args=(config,))
        p.start()
        p.join()



