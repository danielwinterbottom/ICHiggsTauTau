##########
# Customization of tau reco&Id for MiniAOD production
# M. Bluj 
# June 2017
# - Originally made for 90X modified for 91X to include relevant processes explicitly in the task
##########

# Maximal eta of seeding jet, |eta|=4 for Phase-2, |eta|=2.5 for Phase-1
#maxJetEta = 4
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

#

