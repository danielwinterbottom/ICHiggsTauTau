# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step1 --fileout file:SMP-PhaseIITDRSpring17MiniAOD-00011.root --mc --eventcontent MINIAODSIM --runUnscheduled --datatier MINIAODSIM --conditions 91X_upgrade2023_realistic_v3 --step PAT --nThreads 4 --geometry Extended2023D17 --era Phase2C2_timing --python_filename SMP-PhaseIITDRSpring17MiniAOD-00011_1_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 2880
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('PAT',eras.Phase2C2_timing)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('PhysicsTools.PatAlgos.slimming.metFilterPaths_cff')
process.load('Configuration.StandardSequences.PATMC_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/a/adewit/private/test.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step1 nevts:2880'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.MINIAODSIMoutput = cms.OutputModule("PoolOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(4),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('MINIAODSIM'),
        filterName = cms.untracked.string('')
    ),
    dropMetaData = cms.untracked.string('ALL'),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    fastCloning = cms.untracked.bool(False),
    fileName = cms.untracked.string('file:out_miniAOD_prod.root'),
    outputCommands = process.MINIAODSIMEventContent.outputCommands,
    overrideInputFileSplitLevels = cms.untracked.bool(True)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '91X_upgrade2023_realistic_v3', '')

# Path and EndPath definitions
process.Flag_trackingFailureFilter = cms.Path(process.goodVertices+process.trackingFailureFilter)
process.Flag_goodVertices = cms.Path(process.primaryVertexFilter)
process.Flag_CSCTightHaloFilter = cms.Path(process.CSCTightHaloFilter)
process.Flag_trkPOGFilters = cms.Path(~process.logErrorTooManyClusters)
process.Flag_HcalStripHaloFilter = cms.Path(process.HcalStripHaloFilter)
process.Flag_trkPOG_logErrorTooManyClusters = cms.Path(~process.logErrorTooManyClusters)
process.Flag_EcalDeadCellTriggerPrimitiveFilter = cms.Path(process.EcalDeadCellTriggerPrimitiveFilter)
process.Flag_ecalLaserCorrFilter = cms.Path(process.ecalLaserCorrFilter)
process.Flag_globalSuperTightHalo2016Filter = cms.Path(process.globalSuperTightHalo2016Filter)
process.Flag_eeBadScFilter = cms.Path()
process.Flag_METFilters = cms.Path(process.metFilters)
process.Flag_chargedHadronTrackResolutionFilter = cms.Path(process.chargedHadronTrackResolutionFilter)
process.Flag_globalTightHalo2016Filter = cms.Path(process.globalTightHalo2016Filter)
process.Flag_CSCTightHaloTrkMuUnvetoFilter = cms.Path(process.CSCTightHaloTrkMuUnvetoFilter)
process.Flag_HBHENoiseIsoFilter = cms.Path()
process.Flag_BadChargedCandidateSummer16Filter = cms.Path(process.BadChargedCandidateSummer16Filter)
process.Flag_hcalLaserEventFilter = cms.Path(process.hcalLaserEventFilter)
process.Flag_BadPFMuonFilter = cms.Path(process.BadPFMuonFilter)
process.Flag_HBHENoiseFilter = cms.Path()
process.Flag_trkPOG_toomanystripclus53X = cms.Path()
process.Flag_EcalDeadCellBoundaryEnergyFilter = cms.Path(process.EcalDeadCellBoundaryEnergyFilter)
process.Flag_BadChargedCandidateFilter = cms.Path(process.BadChargedCandidateFilter)
process.Flag_trkPOG_manystripclus53X = cms.Path()
process.Flag_BadPFMuonSummer16Filter = cms.Path(process.BadPFMuonSummer16Filter)
process.Flag_muonBadTrackFilter = cms.Path(process.muonBadTrackFilter)
process.Flag_CSCTightHalo2015Filter = cms.Path(process.CSCTightHalo2015Filter)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.MINIAODSIMoutput_step = cms.EndPath(process.MINIAODSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.Flag_HBHENoiseFilter,process.Flag_HBHENoiseIsoFilter,process.Flag_CSCTightHaloFilter,process.Flag_CSCTightHaloTrkMuUnvetoFilter,process.Flag_CSCTightHalo2015Filter,process.Flag_globalTightHalo2016Filter,process.Flag_globalSuperTightHalo2016Filter,process.Flag_HcalStripHaloFilter,process.Flag_hcalLaserEventFilter,process.Flag_EcalDeadCellTriggerPrimitiveFilter,process.Flag_EcalDeadCellBoundaryEnergyFilter,process.Flag_goodVertices,process.Flag_eeBadScFilter,process.Flag_ecalLaserCorrFilter,process.Flag_trkPOGFilters,process.Flag_chargedHadronTrackResolutionFilter,process.Flag_muonBadTrackFilter,process.Flag_BadChargedCandidateFilter,process.Flag_BadPFMuonFilter,process.Flag_BadChargedCandidateSummer16Filter,process.Flag_BadPFMuonSummer16Filter,process.Flag_trkPOG_manystripclus53X,process.Flag_trkPOG_toomanystripclus53X,process.Flag_trkPOG_logErrorTooManyClusters,process.Flag_METFilters,process.endjob_step,process.MINIAODSIMoutput_step)


process.schedule.associate(process.patTask)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# customisation of the process.

# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)

# End of customisation functions
#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)

# customisation of the process.
process.patJets.discriminatorSources = cms.VInputTag(
        cms.InputTag("pfJetBProbabilityBJetTags"),
        cms.InputTag("pfJetProbabilityBJetTags"),
        cms.InputTag("pfTrackCountingHighEffBJetTags"),
        cms.InputTag("pfSimpleSecondaryVertexHighEffBJetTags"),
        cms.InputTag("pfSimpleInclusiveSecondaryVertexHighEffBJetTags"),
        cms.InputTag("pfCombinedSecondaryVertexV2BJetTags"),
        cms.InputTag("pfCombinedInclusiveSecondaryVertexV2BJetTags"),
        cms.InputTag("softPFMuonBJetTags"),
        cms.InputTag("softPFElectronBJetTags"),
        cms.InputTag("pfCombinedMVAV2BJetTags"),
       # CTagging
        cms.InputTag('pfCombinedCvsLJetTags'),
        cms.InputTag('pfCombinedCvsBJetTags')
        )

# Automatic addition of the customisation function from PhysicsTools.PatAlgos.slimming.miniAOD_tools
from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllMC 

#call to customisation function miniAOD_customizeAllMC imported from PhysicsTools.PatAlgos.slimming.miniAOD_tools
process = miniAOD_customizeAllMC(process)

###execfile('modify_tau.py')
maxJetEta = 4

print 'Production of MiniAOD+PFTau, maxEvents:',process.maxEvents.input.value(), ',maxJetEta',maxJetEta

process.combinatoricRecoTaus.maxJetAbsEta = maxJetEta

# Modified output to produce and store modified and original PFTaus
for prod in process.RecoTauTagAOD.outputCommands:
     process.MINIAODSIMoutput.outputCommands.append(prod)

# Build Run-1 fixed size strips
import RecoTauTag.RecoTau.RecoTauPiZeroBuilderPlugins_cfi as piZeroBuilders
process.ak4PFJetsLegacyHPSPiZeros.builders = cms.VPSet(piZeroBuilders.modStrips)

from RecoTauTag.Configuration.HPSPFTaus_cff import*

# Do not build null taus (default in CMSSW >= 910
process.combinatoricRecoTaus.buildNullTaus = False
process.combinatoricRecoTaus.outputSelection = cms.string("leadPFChargedHadrCand().isNonnull()")

# Clone PAT to use original collections from AOD
# Modified taus (fixed strips) will be stored as slimmedTaus 
# while original ones as slimmedTausOrg
orgProcess='RECO'
process.slimmedTausOrg = process.slimmedTaus.clone(
     src = cms.InputTag("selectedPatTausOrg")
)
process.selectedPatTausOrg = process.selectedPatTaus.clone(
     src = cms.InputTag("patTausOrg")
)
process.patTausOrg = process.patTaus.clone(
     genParticleMatch = cms.InputTag("tauMatchOrg"),
     genJetMatch = cms.InputTag("tauGenJetMatchOrg"),
     tauSource = cms.InputTag("hpsPFTauProducer"+"::"+orgProcess),
     tauTransverseImpactParameterSource = cms.InputTag("hpsPFTauTransverseImpactParameters"+"::"+orgProcess),
)
for name in process.patTausOrg.tauIDSources.parameterNames_():
     param = getattr(process.patTausOrg.tauIDSources,name)
     param.setProcessName(orgProcess)
     #print name,param
process.tauMatchOrg = process.tauMatch.clone(
     src = cms.InputTag("hpsPFTauProducer"+"::"+orgProcess)
)
process.tauGenJetMatchOrg = process.tauGenJetMatch.clone(
     src = cms.InputTag("hpsPFTauProducer"+"::"+orgProcess)
)

process.hpsPFTauProducer = hpsPFTauProducer.clone()

process.hpsPFTauProducerSansRefs = process.hpsPFTauProducerSansRefs.clone()

process.combinatoricRecoTaus = process.combinatoricRecoTaus.clone(
     piZeroSrc = cms.InputTag("ak4PFJetsLegacyHPSPiZeros")
     )


process.patTask.add(process.ak4PFJetTracksAssociatorAtVertex,process.recoTauAK4PFJets08Region,process.recoTauPileUpVertices,process.pfRecoTauTagInfoProducer,process.ak4PFJetsLegacyHPSPiZeros,process.ak4PFJetsRecoTauChargedHadrons,process.hpsSelectionDiscriminator,process.hpsPFTauDiscriminationByDecayModeFindingNewDMs,process.hpsPFTauDiscriminationByDecayModeFindingOldDMs,process.hpsPFTauDiscriminationByDecayModeFinding,process.hpsPFTauDiscriminationByLooseChargedIsolation,process.hpsPFTauDiscriminationByLooseIsolation,process.hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits,process.hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits,process.hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits,process.hpsPFTauDiscriminationByRawCombinedIsolationDBSumPtCorr3Hits,process.hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr3HitsdR03,process.hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr3HitsdR03,process.hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr3HitsdR03,process.hpsPFTauDiscriminationByLoosePileupWeightedIsolation3Hits,process.hpsPFTauDiscriminationByMediumPileupWeightedIsolation3Hits,process.hpsPFTauDiscriminationByTightPileupWeightedIsolation3Hits,process.hpsPFTauDiscriminationByPhotonPtSumOutsideSignalCone,process.hpsPFTauDiscriminationByRawPileupWeightedIsolation3Hits,process.hpsPFTauDiscriminationByLooseElectronRejection,process.hpsPFTauDiscriminationByMediumElectronRejection,process.hpsPFTauDiscriminationByTightElectronRejection,process.hpsPFTauDiscriminationByMVA6rawElectronRejection,process.hpsPFTauDiscriminationByMVA6VLooseElectronRejection,process.hpsPFTauDiscriminationByMVA6LooseElectronRejection,process.hpsPFTauDiscriminationByMVA6MediumElectronRejection,process.hpsPFTauDiscriminationByMVA6TightElectronRejection,process.hpsPFTauDiscriminationByMVA6VTightElectronRejection,process.hpsPFTauDiscriminationByDeadECALElectronRejection,process.hpsPFTauDiscriminationByLooseMuonRejection3,process.hpsPFTauDiscriminationByTightMuonRejection3,process.hpsPFTauPrimaryVertexProducer,process.hpsPFTauSecondaryVertexProducer,process.hpsPFTauTransverseImpactParameters,process.hpsPFTauChargedIsoPtSum,process.hpsPFTauNeutralIsoPtSum,process.hpsPFTauPUcorrPtSum,process.hpsPFTauNeutralIsoPtSumWeight,process.hpsPFTauFootprintCorrection,process.hpsPFTauPhotonPtSumOutsideSignalCone,process.hpsPFTauDiscriminationByIsolationMVArun2v1DBoldDMwLTraw,process.hpsPFTauDiscriminationByVLooseIsolationMVArun2v1DBoldDMwLT,process.hpsPFTauDiscriminationByLooseIsolationMVArun2v1DBoldDMwLT,process.hpsPFTauDiscriminationByMediumIsolationMVArun2v1DBoldDMwLT,process.hpsPFTauDiscriminationByTightIsolationMVArun2v1DBoldDMwLT,process.hpsPFTauDiscriminationByVTightIsolationMVArun2v1DBoldDMwLT,process.hpsPFTauDiscriminationByVVTightIsolationMVArun2v1DBoldDMwLT,process.hpsPFTauDiscriminationByIsolationMVArun2v1DBnewDMwLTraw,process.hpsPFTauDiscriminationByVLooseIsolationMVArun2v1DBnewDMwLT,process.hpsPFTauDiscriminationByLooseIsolationMVArun2v1DBnewDMwLT,process.hpsPFTauDiscriminationByMediumIsolationMVArun2v1DBnewDMwLT,process.hpsPFTauDiscriminationByTightIsolationMVArun2v1DBnewDMwLT,process.hpsPFTauDiscriminationByVTightIsolationMVArun2v1DBnewDMwLT,process.hpsPFTauDiscriminationByVVTightIsolationMVArun2v1DBnewDMwLT,process.hpsPFTauDiscriminationByIsolationMVArun2v1PWoldDMwLTraw,process.hpsPFTauDiscriminationByVLooseIsolationMVArun2v1PWoldDMwLT,process.hpsPFTauDiscriminationByLooseIsolationMVArun2v1PWoldDMwLT,process.hpsPFTauDiscriminationByMediumIsolationMVArun2v1PWoldDMwLT,process.hpsPFTauDiscriminationByTightIsolationMVArun2v1PWoldDMwLT,process.hpsPFTauDiscriminationByVTightIsolationMVArun2v1PWoldDMwLT,process.hpsPFTauDiscriminationByVVTightIsolationMVArun2v1PWoldDMwLT,process.hpsPFTauDiscriminationByIsolationMVArun2v1PWnewDMwLTraw,process.hpsPFTauDiscriminationByVLooseIsolationMVArun2v1PWnewDMwLT,process.hpsPFTauDiscriminationByLooseIsolationMVArun2v1PWnewDMwLT,process.hpsPFTauDiscriminationByMediumIsolationMVArun2v1PWnewDMwLT,process.hpsPFTauDiscriminationByTightIsolationMVArun2v1PWnewDMwLT,process.hpsPFTauDiscriminationByVTightIsolationMVArun2v1PWnewDMwLT,process.hpsPFTauDiscriminationByVVTightIsolationMVArun2v1PWnewDMwLT,process.hpsPFTauChargedIsoPtSumdR03,process.hpsPFTauNeutralIsoPtSumdR03,process.hpsPFTauPUcorrPtSumdR03,process.hpsPFTauNeutralIsoPtSumWeightdR03,process.hpsPFTauFootprintCorrectiondR03,process.hpsPFTauPhotonPtSumOutsideSignalConedR03,process.hpsPFTauDiscriminationByIsolationMVArun2v1DBdR03oldDMwLTraw,process.hpsPFTauDiscriminationByVLooseIsolationMVArun2v1DBdR03oldDMwLT,process.hpsPFTauDiscriminationByLooseIsolationMVArun2v1DBdR03oldDMwLT,process.hpsPFTauDiscriminationByMediumIsolationMVArun2v1DBdR03oldDMwLT,process.hpsPFTauDiscriminationByTightIsolationMVArun2v1DBdR03oldDMwLT,process.hpsPFTauDiscriminationByVTightIsolationMVArun2v1DBdR03oldDMwLT,process.hpsPFTauDiscriminationByVVTightIsolationMVArun2v1DBdR03oldDMwLT,process.hpsPFTauDiscriminationByIsolationMVArun2v1PWdR03oldDMwLTraw,process.hpsPFTauDiscriminationByVLooseIsolationMVArun2v1PWdR03oldDMwLT,process.hpsPFTauDiscriminationByLooseIsolationMVArun2v1PWdR03oldDMwLT,process.hpsPFTauDiscriminationByMediumIsolationMVArun2v1PWdR03oldDMwLT,process.hpsPFTauDiscriminationByTightIsolationMVArun2v1PWdR03oldDMwLT,process.hpsPFTauDiscriminationByVTightIsolationMVArun2v1PWdR03oldDMwLT,process.hpsPFTauDiscriminationByVVTightIsolationMVArun2v1PWdR03oldDMwLT,process.hpsPFTauChargedIsoPtSum,process.hpsPFTauNeutralIsoPtSum,process.hpsPFTauPUcorrPtSum,process.hpsPFTauNeutralIsoPtSumWeight,process.hpsPFTauFootprintCorrection,process.hpsPFTauPhotonPtSumOutsideSignalCone,process.hpsPFTauDiscriminationByRawCombinedIsolationDBSumPtCorr3Hits,process.hpsPFTauDiscriminationByLoosePileupWeightedIsolation3Hits,process.hpsPFTauDiscriminationByMediumPileupWeightedIsolation3Hits,process.hpsPFTauDiscriminationByTightPileupWeightedIsolation3Hits,process.hpsPFTauDiscriminationByRawPileupWeightedIsolation3Hits,process.hpsPFTauDiscriminationByPhotonPtSumOutsideSignalCone,process.pfPileUpIsoPFBRECO,process.pfNoPileUpIsoPFBRECO,process.pfAllNeutralHadronsPFBRECO,process.pfAllChargedHadronsPFBRECO,process.pfAllPhotonsPFBRECO,process.pfAllChargedParticlesPFBRECO,process.pfPileUpAllChargedParticlesPFBRECO,process.pfAllNeutralHadronsAndPhotonsPFBRECO,process.tauIsoDepositPFCandidates,process.tauIsoDepositPFChargedHadrons,process.tauIsoDepositPFNeutralHadrons,process.tauIsoDepositPFGammas,process.tauGenJets,process.tauGenJetsSelectorAllHadrons)

process.patTask.add(process.tauGenJetMatchOrg,process.tauMatchOrg,process.selectedPatTausOrg,process.patTausOrg,process.slimmedTausOrg,process.ak4PFJetsLegacyHPSPiZeros,process.combinatoricRecoTaus, process.hpsPFTauProducerSansRefs, process.hpsPFTauProducer)

process.MINIAODSIMoutput.outputCommands.append('keep *_slimmedTausOrg_*_*')

if process.maxEvents.input.value()>10:
     process.MessageLogger.cerr.FwkReport.reportEvery = process.maxEvents.input.value()//10
if process.maxEvents.input.value()>10000 or process.maxEvents.input.value()<0:
     process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.options = cms.untracked.PSet(
     process.options,
     wantSummary = cms.untracked.bool(True)
)
### end of modify_tau


# Customisation from command line
#associatePatAlgosToolsTask(process)

del process.patTrigger; del process.selectedPatTrigger
# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion


#dump_file = open('dump_july6.py','w')
#dump_file.write(process.dumpPython())

