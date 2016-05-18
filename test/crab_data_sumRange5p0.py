from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName     = 'ZeroBias'
config.General.workArea        = 'L1NTuplesV2_l1t-tsg-v5-CMSSW_8_0_2_METEtaRange5p0_v1'
config.General.transferOutputs = True
config.General.transferLogs    = True

config.JobType.pluginName = 'ANALYSIS'
config.JobType.psetName   = 'L1NTuplesv4_CMSSW_8_0_2_cfg.py'
config.JobType.pyCfgParams = ['sample=data','sumRange=5']

#config.Data.inputDataset     = '/ZeroBias/Run2015D-v1/RAW'
config.Data.inputDBS         = 'global'
config.Data.lumiMask         = 'Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt'
config.Data.runRange         = '259721'
config.Data.splitting        = 'EventAwareLumiBased'
config.Data.unitsPerJob      = 20000
config.Data.outLFNDirBase    = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication      = False
config.Data.outputDatasetTag = 'L1NTuplesV2_l1t-tsg-v5-CMSSW_8_0_2_METEtaRange5p0_v1'

#config.Site.whitelist   = ["T2_UK_London_IC"]
config.Site.storageSite = 'T2_UK_London_IC'


##########################

#from WMCore.Configuration import Configuration
#config = Configuration()
#config.General.transferOutputs = True
#config.General.workArea='L1NTuplesV2_l1t-tsg-v5-CMSSW_8_0_2_METEtaRange3p0_v1'
#config.General.requestName = 'ZeroBias'
#config.JobType.psetName = 'L1NTuplesv4_CMSSW_8_0_2_cfg.py'
#config.JobType.pluginName = 'ANALYSIS'
#config.JobType.outputFiles = ['EventTree.root']
#config.JobType.pyCfgParams = ['sample=data','sumRange=3']
#
##config.Data.inputDataset     = '/ZeroBias/Run2015D-v1/RAW'
#config.Data.inputDBS         = 'global'
#config.Data.lumiMask         = 'Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt'
#config.Data.runRange         = '259721'
#config.Data.splitting        = 'EventAwareLumiBased'
#config.Data.unitsPerJob      = 20000
#config.Data.outLFNDirBase    = '/store/user/%s/' % (getUsernameFromSiteDB())
#config.Data.publication      = False
#config.Data.outputDatasetTag = 'L1NTuplesV2_l1t-tsg-v5-CMSSW_8_0_2_METEtaRange3p0_v1'
#
##config.Site.whitelist   = ["T2_UK_London_IC"]
#config.Site.storageSite = 'T2_UK_London_IC'

#########################


#if __name__ == '__main__':

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

tasks.append(('ZeroBias0','/ZeroBias/Run2015D-v1/RAW')) 
tasks.append(('ZeroBias1','/ZeroBias1/Run2015D-v1/RAW'))
tasks.append(('ZeroBias2','/ZeroBias2/Run2015D-v1/RAW'))
tasks.append(('ZeroBias3','/ZeroBias3/Run2015D-v1/RAW'))
tasks.append(('ZeroBias4','/ZeroBias4/Run2015D-v1/RAW'))
tasks.append(('ZeroBias5','/ZeroBias5/Run2015D-v1/RAW'))
tasks.append(('ZeroBias6','/ZeroBias6/Run2015D-v1/RAW'))
tasks.append(('ZeroBias7','/ZeroBias7/Run2015D-v1/RAW'))
tasks.append(('ZeroBias8','/ZeroBias8/Run2015D-v1/RAW'))

for task in tasks:
    print task[0]
    config.General.requestName = task[0]
    config.Data.inputDataset = task[1]
    submit(config)
