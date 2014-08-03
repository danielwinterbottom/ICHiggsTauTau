import FWCore.ParameterSet.Config as cms

## [Electron]
icElectronProducer = cms.EDProducer('ICElectronProducer',
    branch                    = cms.string("electrons"),
    input                     = cms.InputTag("gsfElectrons"),
    includeR9                 = cms.bool(False),
    inputR9                   = cms.InputTag("icElectronR9Calculator"),
    includeHcalSum            = cms.bool(False),
    inputHcalSum              = cms.InputTag("icElectronHcalDepthCalculator"),
    includeConversionMatches  = cms.bool(False),
    inputConversionMatches    = cms.InputTag("icElectronConversionCalculator"),
    includeVertexIP           = cms.bool(False),
    inputVertices             = cms.InputTag("offlinePrimaryVertices"),
    includeBeamspotIP         = cms.bool(False),
    inputBeamspot             = cms.InputTag("offlineBeamSpot"),
    includeFloats = cms.PSet(
      # A named list of InputTags identifying edm::ValueMap<float>
      # objects keyed on the input GsfElectron collection. The hash
      # of the name and the float value will be stored. For example:
      #   mvaNonTrigV0    = cms.InputTag("mvaNonTrigV0")
    ),
    includePFIso03           = cms.bool(False),
    pfIso03 = cms.PSet(
      chargedAll  = cms.InputTag("elPFIsoValueChargedAll03PFIdPFIso"),
      charged     = cms.InputTag("elPFIsoValueCharged03PFIdPFIso"),
      neutral     = cms.InputTag("elPFIsoValueNeutral03PFIdPFIso"),
      gamma       = cms.InputTag("elPFIsoValueGamma03PFIdPFIso"),
      pu          = cms.InputTag("elPFIsoValuePU03PFIdPFIso")
    ),
    includePFIso04           = cms.bool(False),
    pfIso04 = cms.PSet(
      chargedAll  = cms.InputTag("elPFIsoValueChargedAll04PFIdPFIso"),
      charged     = cms.InputTag("elPFIsoValueCharged04PFIdPFIso"),
      neutral     = cms.InputTag("elPFIsoValueNeutral04PFIdPFIso"),
      gamma       = cms.InputTag("elPFIsoValueGamma04PFIdPFIso"),
      pu          = cms.InputTag("elPFIsoValuePU04PFIdPFIso")
    )
)
## [Electron]

## [ElectronHcalDepth]
icElectronHcalDepthCalculator = cms.EDProducer('ICElectronHcalDepthCalculator',
    input = cms.InputTag("gsfElectrons")
)
## [ElectronHcalDepth]

## [ElectronConversion]
icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
    input       = cms.InputTag("gsfElectrons"),
    beamspot    = cms.InputTag("offlineBeamSpot"),
    conversions = cms.InputTag("allConversions")
)
## [ElectronConversion]

## [Muon]
icMuonProducer = cms.EDProducer('ICMuonProducer',
    branch                    = cms.string("muons"),
    input                     = cms.InputTag("muons"),
    isPF                      = cms.bool(False),
    includeVertexIP           = cms.bool(False),
    inputVertices             = cms.InputTag("offlinePrimaryVertices"),
    includeBeamspotIP         = cms.bool(False),
    inputBeamspot             = cms.InputTag("offlineBeamSpot"),
    includeFloats = cms.PSet(
    ),
    includePFIso03           = cms.bool(False),
    pfIso03 = cms.PSet(
      chargedAll  = cms.InputTag("muPFIsoValueChargedAll03PFIso"),
      charged     = cms.InputTag("muPFIsoValueCharged03PFIso"),
      neutral     = cms.InputTag("muPFIsoValueNeutral03PFIso"),
      gamma       = cms.InputTag("muPFIsoValueGamma03PFIso"),
      pu          = cms.InputTag("muPFIsoValuePU03PFIso")
    ),
    includePFIso04           = cms.bool(False),
    pfIso04 = cms.PSet(
      chargedAll  = cms.InputTag("muPFIsoValueChargedAll04PFIso"),
      charged     = cms.InputTag("muPFIsoValueCharged04PFIso"),
      neutral     = cms.InputTag("muPFIsoValueNeutral04PFIso"),
      gamma       = cms.InputTag("muPFIsoValueGamma04PFIso"),
      pu          = cms.InputTag("muPFIsoValuePU04PFIso")
    )
)
## [Muon]

## [PhotonHadTowerOverEm]
icPhotonHadTowerOverEmCalculator = cms.EDProducer('ICPhotonHadTowerOverEmCalculator',
    input = cms.InputTag("photons")
)
## [PhotonHadTowerOverEm]

## [PhotonVetoCalculator]
icPhotonElectronVetoCalculator = cms.EDProducer('ICPhotonElectronVetoCalculator',
    input       = cms.InputTag("photons"),
    electrons   = cms.InputTag("gsfElectrons"),
    beamspot    = cms.InputTag("offlineBeamSpot"),
    conversions = cms.InputTag("allConversions")
)
## [PhotonVetoCalculator]

## [Photon]
icPhotonProducer = cms.EDProducer('ICPhotonProducer',
    branch                  = cms.string("photons"),
    input                   = cms.InputTag("photons"),
    includeElectronVeto     = cms.bool(False),
    inputElectronVeto       = cms.InputTag("icPhotonElectronVetoCalculator"),
    includeHadTowOverEm     = cms.bool(False),
    inputHadTowOverEm       = cms.InputTag("icPhotonHadTowerOverEmCalculator"),
    includePFIso            = cms.bool(False),
    pfIso = cms.PSet(
      charged     = cms.InputTag(""),
      neutral     = cms.InputTag(""),
      gamma       = cms.InputTag("")
    )
)
## [Photon]

## [Tau]
icTauProducer = cms.EDProducer("ICPFTauProducer",
  branch                  = cms.string("taus"),
  input                   = cms.InputTag("hpsPFTauProducer"),
  inputVertices           = cms.InputTag("offlinePrimaryVertices"),
  includeVertexIP         = cms.bool(False),
  requestTracks           = cms.bool(False),
  tauIDs = cms.PSet(
    decayModeFinding = cms.InputTag("hpsPFTauDiscriminationByDecayModeFinding")
  )
)
## [Tau]

## [Met]
icMetProducer = cms.EDProducer('ICMetProducer',
  branch  = cms.string("pfMet"),
  input   = cms.InputTag("pfMet"),
  includeCustomID = cms.bool(False),
  inputCustomID = cms.InputTag("")
)
## [Met]

## [SingleMet]
icSingleMetProducer = cms.EDProducer('ICSingleMetProducer',
  branch  = cms.string("genMet"),
  input   = cms.InputTag("genMetTrue")
)
## [SingleMet]

## [CaloJet]
icCaloJetProducer = cms.EDProducer('ICCaloJetProducer',
    branch                    = cms.string("caloJets"),
    input                     = cms.InputTag("ak5CaloJets"),
    includeJetFlavour         = cms.bool(False),
    inputJetFlavour           = cms.InputTag("icCaloJetFlavourCalculator"),
    applyJECs                 = cms.bool(False),
    includeJECs               = cms.bool(False),
    JECs                      = cms.PSet(
      L1FastJet  = cms.string("ak5CaloL1Fastjet"),
      L2Relative = cms.string("ak5CaloL2Relative"),
      L3Absolute = cms.string("ak5CaloL3Absolute")
    ),
    applyCutAfterJECs         = cms.bool(False),
    cutAfterJECs              = cms.string(""),
    inputSVInfo               = cms.InputTag(""),
    requestSVInfo             = cms.bool(False),
    BTagDiscriminators        = cms.PSet(),
    specificConfig = cms.PSet(
      includeJetID        = cms.bool(False),
      inputJetID          = cms.InputTag("ak5JetID"),
      includeTowerCounts  = cms.bool(False)
    )
)
## [CaloJet]

## [JPTJet]
icJPTJetProducer = cms.EDProducer('ICJPTJetProducer',
    branch                    = cms.string("jptJets"),
    input                     = cms.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
    includeJetFlavour         = cms.bool(False),
    inputJetFlavour           = cms.InputTag(""),
    applyJECs                 = cms.bool(False),
    includeJECs               = cms.bool(False),
    JECs                      = cms.PSet(
    ),
    applyCutAfterJECs         = cms.bool(False),
    cutAfterJECs              = cms.string(""),
    inputSVInfo               = cms.InputTag(""),
    requestSVInfo             = cms.bool(False),
    BTagDiscriminators        = cms.PSet(
    ),
    specificConfig = cms.PSet(
      includeTrackBasedVars = cms.bool(False),
      inputTracks           = cms.InputTag("generalTracks"),
      inputVertices         = cms.InputTag("offlinePrimaryVertices"),
      requestTracks         = cms.bool(False),
      includeJetID          = cms.bool(False),
      inputJetID            = cms.InputTag("ak5JetID"),
      includeTowerCounts    = cms.bool(False)
    )
)
## [JPTJet]

## [Jet]
## This example given for the reco::PFJet --> ic::PFJet version
## of the producer. The srcConfig and destConfig options for the
## other input and output choices are also given below but are
## commented out
icPFJetProducer = cms.EDProducer('ICPFJetProducer',
    branch                    = cms.string("pfJets"),
    input                     = cms.InputTag("ak5PFJets"),
    #### The srcConfig PSet when the input is a reco::Jet collection
    srcConfig = cms.PSet(
      includeJetFlavour         = cms.bool(False),
      inputJetFlavour           = cms.InputTag("icPFJetFlavourCalculator"),
      applyJECs                 = cms.bool(False),
      includeJECs               = cms.bool(False),
      JECs                      = cms.PSet(),
      applyCutAfterJECs         = cms.bool(False),
      cutAfterJECs              = cms.string(""),
      inputSVInfo               = cms.InputTag("secondaryVertexTagInfosAK5PF"),
      requestSVInfo             = cms.bool(False),
      BTagDiscriminators        = cms.PSet(),
    ),
    #### The srcConfig PSet when the input is a pat::Jet collection
    # srcConfig = cms.PSet(
    #   includeJetFlavour         = cms.bool(False),
    #   includeJECs               = cms.bool(False),
    #   inputSVInfo               = cms.InputTag("secondaryVertexTagInfosAOD"),
    #   requestSVInfo             = cms.bool(False)
    # ),
    #### The destConfig PSet when the output is an ic::PFJet collection
    destConfig = cms.PSet(
      includePileupID       = cms.bool(False),
      inputPileupID         = cms.InputTag("puJetMva", "fullDiscriminant"),
      includeTrackBasedVars = cms.bool(False),
      inputTracks           = cms.InputTag("generalTracks"),
      inputVertices         = cms.InputTag("offlinePrimaryVertices"),
      requestTracks         = cms.bool(False)
    )
    #### The destConfig PSet when the output is an ic::CaloJet collection
    # destConfig = cms.PSet(
    #   includeJetID        = cms.bool(False),
    #   inputJetID          = cms.InputTag("ak5JetID"),
    #   includeTowerCounts  = cms.bool(False)
    # )
    #### The destConfig PSet when the output is an ic::JPTJet collection
    # destConfig = cms.PSet(
    #   includeTrackBasedVars = cms.bool(False),
    #   inputTracks           = cms.InputTag("generalTracks"),
    #   inputVertices         = cms.InputTag("offlinePrimaryVertices"),
    #   requestTracks         = cms.bool(False),
    #   includeJetID          = cms.bool(False),
    #   inputJetID            = cms.InputTag("ak5JetID"),
    #   includeTowerCounts    = cms.bool(False)
    # )
    #### The destConfig PSet when the output is an ic::Jet collection (empty!)
    # destConfig = cms.PSet()
)
## [Jet]

## [Vertex]
icVertexProducer = cms.EDProducer('ICVertexProducer',
  branch  = cms.string("vertices"),
  input   = cms.InputTag("offlinePrimaryVertices"),
  firstVertexOnly = cms.bool(False),
  trackPtThreshold = cms.double(0.0),
  requestTracks = cms.bool(False)
)
## [Vertex]

## [SecondaryVertex]
icSecondaryVertexProducer = cms.EDProducer('ICSecondaryVertexProducer',
  branch  = cms.string("secondaryVertices"),
  input   = cms.InputTag(""),
  trackPtThreshold = cms.double(0.0),
  requestTracks = cms.bool(False)
)
## [SecondaryVertex]

## [Track]
icTrackProducer = cms.EDProducer('ICTrackProducer',
  branch  = cms.string("tracks"),
  input   = cms.InputTag("generalTracks")
)
## [Track]

## [LightTrack]
icLightTrackProducer = cms.EDProducer('ICLightTrackProducer',
  branch  = cms.string("tracks"),
  input   = cms.InputTag("generalTracks")
)
## [LightTrack]

## [PileupInfo]
icPileupInfoProducer = cms.EDProducer('ICPileupInfoProducer',
  branch  = cms.string("pileupInfo"),
  input   = cms.InputTag("addPileupInfo")
)
## [PileupInfo]

## [GenParticle]
icGenParticleProducer = cms.EDProducer('ICGenParticleProducer',
  branch  = cms.string("genParticles"),
  input   = cms.InputTag("genParticles"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True)
)
## [GenParticle]

## [GenJet]
icGenJetProducer = cms.EDProducer('ICGenJetProducer',
  branch  = cms.string("genJets"),
  input   = cms.InputTag("ak5GenJets"),
  inputGenParticles = cms.InputTag("genParticles"),
  requestGenParticles = cms.bool(False)
)
## [GenJet]

## [SuperCluster]
icSuperClusterProducer = cms.EDProducer('ICSuperClusterProducer',
  branch  = cms.string("superClusters"),
  inputBarrel   = cms.InputTag("correctedHybridSuperClusters"),
  inputEndcap   = cms.InputTag("correctedMulti5x5SuperClustersWithPreshower")
)
## [SuperCluster]

## [Candidate]
icL1EmIsolatedProducer = cms.EDProducer('ICCandidateProducer',
  branch  = cms.string("l1EmIsolated"),
  input   = cms.InputTag("l1extraParticles", "Isolated", "RECO")
)
## [Candidate]

## [L1EtMiss]
icL1MHTProducer = cms.EDProducer('ICL1EtMissProducer',
  branch  = cms.string("l1MHT"),
  input   = cms.InputTag("l1extraParticles", "MHT", "RECO")
)
## [L1EtMiss]

## [TriggerPath]
icTriggerPathProducer = cms.EDProducer('ICTriggerPathProducer',
  branch  = cms.string("triggerPaths"),
  input   = cms.InputTag("patTriggerEvent"),
  includeAcceptedOnly = cms.bool(True),
  saveStrings = cms.bool(True),
  splitVersion = cms.bool(False)
)
## [TriggerPath]

## [TriggerObject]
icTriggerObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
  branch = cms.string("triggerObjects"),
  input   = cms.InputTag("patTriggerEvent"),
  hltPath = cms.string(""),
  storeOnlyIfFired = cms.bool(False)
)
## [TriggerObject]

## [EventInfo]
icEventInfoProducer = cms.EDProducer('ICEventInfoProducer',
  branch              = cms.string("eventInfo"),
  includeJetRho       = cms.bool(False),
  inputJetRho         = cms.InputTag("kt6PFJets", "rho"),
  includeLeptonRho    = cms.bool(False),
  inputLeptonRho      = cms.InputTag("kt6PFJets", "rho"),
  includeVertexCount  = cms.bool(False),
  inputVertices       = cms.InputTag("offlinePrimaryVertices"),
  includeCSCFilter    = cms.bool(False),
  inputCSCFilter      = cms.InputTag("BeamHaloSummary"),
  filters             = cms.PSet(),
  weights             = cms.PSet(
  ),
  genFilterWeights    = cms.PSet(
  )
)
## [EventInfo]

## [Event]
icEventProducer = cms.EDProducer('ICEventProducer')
## [Event]

