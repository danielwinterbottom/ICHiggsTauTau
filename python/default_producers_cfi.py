import FWCore.ParameterSet.Config as cms

## [Candidate]
icCandidateProducer = cms.EDProducer('ICCandidateProducer',
  branch  = cms.string("l1EmIsolated"),
  input   = cms.InputTag("l1extraParticles", "Isolated", "RECO")
)
## [Candidate]

## [PFCandidate]
icPFProducer = cms.EDProducer('ICPFProducer',
  branch              = cms.string("pfCandidates"),
  input               = cms.InputTag("particleFlow", "", "RECO"),
  requestTracks       = cms.bool(False),
  requestGsfTracks    = cms.bool(False),
  inputUnpackedTracks = cms.InputTag("")
)

icPFFromPackedProducer = cms.EDProducer('ICPFFromPackedProducer',
  branch              = cms.string("pfCandidates"),
  input               = cms.InputTag("packedPFCandidates", "", "PAT"),
  requestTracks       = cms.bool(False),
  requestGsfTracks    = cms.bool(False),
  inputUnpackedTracks = cms.InputTag("unpackedTracksAndVertices")
)
## [PFCandidate]

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
    ),
   includeClusterIso       = cms.bool(False),
   clusterIso = cms.PSet(
     ecal        = cms.InputTag("elEcalPFClusterIso"),
     hcal        = cms.InputTag("elHcalPFClusterIso")
   )   
)
## [Electron]

## [ElectronHcalDepth]
icElectronHcalDepthCalculator = cms.EDProducer('ICElectronHcalDepthCalculator',
    input = cms.InputTag("gsfElectrons")
)
## [ElectronHcalDepth]

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
      # A named list of InputTags identifying edm::ValueMap<float>
      # objects keyed on the input collection objects. The hash
      # of the name and the float value will be stored.
    ),
    includeDoubles = cms.PSet(
      # A named list of InputTags identifying edm::ValueMap<double>
      # objects keyed on the input collection objects. The hash
      # of the name and the double value (cast as float) will be stored.
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
    includePFIso03           = cms.bool(False),
    pfIso03 = cms.PSet(
      chargedAll  = cms.InputTag("phPFIsoValueChargedAll03PFIso"),
      charged     = cms.InputTag("phPFIsoValueCharged03PFIso"),
      neutral     = cms.InputTag("phPFIsoValueNeutral03PFIso"),
      gamma       = cms.InputTag("phPFIsoValueGamma03PFIso"),
      pu          = cms.InputTag("phPFIsoValuePU03PFIso")
    ),
    includePFIso04           = cms.bool(False),
    pfIso04 = cms.PSet(
      chargedAll  = cms.InputTag("phPFIsoValueChargedAll04PFIso"),
      charged     = cms.InputTag("phPFIsoValueCharged04PFIso"),
      neutral     = cms.InputTag("phPFIsoValueNeutral04PFIso"),
      gamma       = cms.InputTag("phPFIsoValueGamma04PFIso"),
      pu          = cms.InputTag("phPFIsoValuePU04PFIso")
    ),
    includeIsoFromPat = cms.bool(False)
)
## [Photon]

## [Tau]
icTauProducer = cms.EDProducer("ICPFTauProducer", # or "ICPFTauFromPatProducer"
  branch                  = cms.string("taus"),
  input                   = cms.InputTag("hpsPFTauProducer"),
  inputVertices           = cms.InputTag("offlinePrimaryVertices"),
  includeVertexIP         = cms.bool(False),
  requestTracks           = cms.bool(False),
  tauIDs = cms.PSet(
    # Add a list of discriminators that should be loaded from the edm::Event.
    # Each discriminator will be stored as a float and associated to the hash
    # of the label string given here (e.g. "decayModeFinding" below). Some TauID
    # PSets have been defined in python/tau_discriminators_cfi.py, and these follow
    # the labelling convention used in the default PAT sequences.
    # Note that if the ICPFTauFromPatProducer is used this list will be ignored.
    #
    # decayModeFinding = cms.InputTag("hpsPFTauDiscriminationByDecayModeFinding"),
    # etc...
  )
)
## [Tau]

## [Met]
icMetProducer = cms.EDProducer('ICPFMetProducer',
  branch  = cms.string("pfMet"),
  input   = cms.InputTag("pfMet"),
  includeCustomID = cms.bool(False),
  inputCustomID = cms.InputTag(""),
  doGenMet = cms.bool(False),
  includeExternalMetsig = cms.bool(False),
  includeExternalMetsigMethod2 = cms.bool(False),
  metsig = cms.PSet(
    metsig      = cms.InputTag("METSignificance","METSignificance"),
    metsigcov00 = cms.InputTag("METSignificance","CovarianceMatrix00"),
    metsigcov01 = cms.InputTag("METSignificance","CovarianceMatrix01"),
    metsigcov10 = cms.InputTag("METSignificance","CovarianceMatrix10"),
    metsigcov11 = cms.InputTag("METSignificance","CovarianceMatrix11")
    ),
  metsig_method2 = cms.PSet(
    metsig      = cms.InputTag("METSignificance","METSignificance"),
    metsigcov = cms.InputTag("METSignificance","METCovariance")
    ),
  #includeMetCorrections = cms.bool(False),
  #metcorrections = cms.vstring(
  #      'Raw','Type1','Type01','TypeXY','Type1XY','Type01XY','Type1Smear','Type01Smear','Type1SmearXY','Type01SmearXY','RawCalo'),
  #includeMetUncertainties = cms.bool(False),
  #metuncertainties = cms.vstring(
  #      'JetResUp','JetResDown','JetEnUp','JetEnDown','MuonEnUp','MuonEnDown','ElectronEnUp','ElectronEnDown','TauEnUp','TauEnDown','UnclusteredEnUp','UnclusteredEnDown','PhotonEnUp','PhotonEnDown','NoShift')
)

icMetFromPatProducer = cms.EDProducer('ICPFMetFromPatProducer',
  branch  = cms.string("pfMet"),
  input   = cms.InputTag("slimmedMETs"),
  includeCustomID = cms.bool(False),
  includeUserCandID = cms.bool(False),
  inputCustomID = cms.InputTag(""),
  doGenMet = cms.bool(False),
  includeExternalMetsig = cms.bool(False),
  includeExternalMetsigMethod2 = cms.bool(False),
  getUncorrectedMet = cms.bool(False),
  metsig = cms.PSet(
    metsig      = cms.InputTag("METSignificance","METSignificance"),
    metsigcov00 = cms.InputTag("METSignificance","CovarianceMatrix00"),
    metsigcov01 = cms.InputTag("METSignificance","CovarianceMatrix01"),
    metsigcov10 = cms.InputTag("METSignificance","CovarianceMatrix10"),
    metsigcov11 = cms.InputTag("METSignificance","CovarianceMatrix11")
    ),
  metsig_method2 = cms.PSet(
    metsig      = cms.InputTag("METSignificance","METSignificance"),
    metsigcov = cms.InputTag("METSignificance","METCovariance")
    ),
  includeMetCorrections = cms.bool(False),
  metcorrections = cms.vstring(
        'Raw','Type1','Type01','TypeXY','Type1XY','Type01XY','Type1Smear','Type01Smear','Type1SmearXY','Type01SmearXY','RawCalo'),
  includeMetUncertainties = cms.bool(False),
  metuncertainties = cms.vstring(
        'JetResUp','JetResDown','JetEnUp','JetEnDown','MuonEnUp','MuonEnDown','ElectronEnUp','ElectronEnDown','TauEnUp','TauEnDown','UnclusteredEnUp','UnclusteredEnDown','PhotonEnUp','PhotonEnDown','NoShift')
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
    srcConfig = cms.PSet(
      includeJetFlavour         = cms.bool(False),
      inputJetFlavour           = cms.InputTag("icCaloJetFlavourCalculator"),
      applyJECs                 = cms.bool(False),
      includeJECs               = cms.bool(False),
      JECs                      = cms.PSet(),
      applyCutAfterJECs         = cms.bool(False),
      cutAfterJECs              = cms.string(""),
      inputSVInfo               = cms.InputTag("secondaryVertexTagInfosAK5Calo"),
      requestSVInfo             = cms.bool(False),
      BTagDiscriminators        = cms.PSet(),
    ),
    destConfig = cms.PSet(
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
    srcConfig = cms.PSet(
      includeJetFlavour         = cms.bool(False),
      inputJetFlavour           = cms.InputTag("icJPTJetFlavourCalculator"),
      applyJECs                 = cms.bool(False),
      includeJECs               = cms.bool(False),
      JECs                      = cms.PSet(),
      applyCutAfterJECs         = cms.bool(False),
      cutAfterJECs              = cms.string(""),
      inputSVInfo               = cms.InputTag("secondaryVertexTagInfosAK5JPT"),
      requestSVInfo             = cms.bool(False),
      BTagDiscriminators        = cms.PSet(),
    ),
    destConfig = cms.PSet(
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
      ### If True corrects the input jets using the list of correctors
      ### given by JECs below
      applyJECs                 = cms.bool(False),
      ### If True saves the value of each correction factor along witht he
      ### hash of the label given in the option JECs below
      includeJECs               = cms.bool(False),
      JECs                      = cms.PSet(
        ### E.g.
        #  L1FastJet  = cms.string("ak5PFL1Fastjet"),
        #  L2Relative = cms.string("ak5PFL2Relative"),
        #  L3Absolute = cms.string("ak5PFL3Absolute")
      ),
      ### If true will apply the cut specfied by 'cutAfterJECS' to the corrected
      ### input jets
      applyCutAfterJECs         = cms.bool(False),
      cutAfterJECs              = cms.string(""),
      inputSVInfo               = cms.InputTag("secondaryVertexTagInfosAK5PF"),
      requestSVInfo             = cms.bool(False),
      BTagDiscriminators        = cms.PSet(
        ### E.g.
        #  simpleSecondaryVertexHighEffBJetTags = cms.InputTag("simpleSecondaryVertexHighEffBJetTagsAK5PF"),
        #  simpleSecondaryVertexHighPurBJetTags = cms.InputTag("simpleSecondaryVertexHighPurBJetTagsAK5PF"),
        #  combinedSecondaryVertexBJetTags      = cms.InputTag("combinedSecondaryVertexBJetTagsAK5PF")
      ),
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


icPFJetFromPatProducer = cms.EDProducer('ICPFJetFromPatProducer',
    branch                    = cms.string("pfJetsFromPat"),
    input                     = cms.InputTag("ak4PFJets"),
    #### The srcConfig PSet when the input is a reco::Jet collection
    #### The srcConfig PSet when the input is a pat::Jet collection
     srcConfig = cms.PSet(
       isSlimmed                 = cms.bool(False),
       includeJetFlavour         = cms.bool(False),
       includeJECs               = cms.bool(False),
       inputSVInfo               = cms.InputTag("secondaryVertexTagInfosAOD"),
       requestSVInfo             = cms.bool(False)
     ),
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
  includeDaughters = cms.bool(True),
  includeStatusFlags = cms.bool(False)
)
## [GenParticle]

icGenVertexProducer = cms.EDProducer('ICGenVertexProducer',
  branch = cms.string("genVertices"),
  input = cms.InputTag("genParticles"),
  type = cms.string("pp")
)

## [GenJet]
icGenJetProducer = cms.EDProducer('ICGenJetProducer',
  branch  = cms.string("genJets"),
  input   = cms.InputTag("ak5GenJets"),
  inputGenParticles = cms.InputTag("genParticles"),
  requestGenParticles = cms.bool(False),
  isSlimmed = cms.bool(False)
)
## [GenJet]

## [SuperCluster]
icSuperClusterProducer = cms.EDProducer('ICSuperClusterProducer',
  branch  = cms.string("superClusters"),
  inputBarrel   = cms.InputTag("correctedHybridSuperClusters"),
  inputEndcap   = cms.InputTag("correctedMulti5x5SuperClustersWithPreshower")
)
## [SuperCluster]

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
  splitVersion = cms.bool(False),
  inputIsStandAlone = cms.bool(False),
  inputPrescales = cms.InputTag("patTrigger", "", "PAT") # only used when inputIsStandAlone is true
)
## [TriggerPath]

## [TriggerObject]
icTriggerObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
  branch = cms.string("triggerObjects"),
  input   = cms.InputTag("patTriggerEvent"),
  hltPath = cms.string(""),
  storeOnlyIfFired = cms.bool(False),
  inputIsStandAlone = cms.bool(False),
  inputTriggerResults = cms.InputTag("TriggerResults", "", "HLT")
)
## [TriggerObject]

## [EventInfo]
icEventInfoProducer = cms.EDProducer('ICEventInfoProducer',
  branch              = cms.string("eventInfo"),
  lheProducer         = cms.InputTag("externalLHEProducer"),
  includeLHEWeights   = cms.bool(False),
  includeJetRho       = cms.bool(False),
  inputJetRho         = cms.InputTag("fixedGridRhoFastjetAll"),
  includeLeptonRho    = cms.bool(False),
  inputLeptonRho      = cms.InputTag("fixedGridRhoFastjetAll"),
  includeHT           = cms.bool(False),
  includeVertexCount  = cms.bool(False),
  inputVertices       = cms.InputTag("offlinePrimaryVertices"),
  includeCSCFilter    = cms.bool(False),
  inputCSCFilter      = cms.InputTag("BeamHaloSummary"),
  filters             = cms.PSet(),
  weights             = cms.PSet(
  ),
  genFilterWeights    = cms.PSet(
  ),
  includeFiltersFromTrig = cms.bool(False),
  filtersfromtrig     = cms.vstring("Flag_CSCTightHaloFilter","Flag_hcalLaserEventFilter","Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_goodVertices","Flag_eeBadScFilter","Flag_ecalLaserCorrFilter","Flag_trkPOGFilters","Flag_trkPOG_manystripclus53X","Flag_trkPOG_toomanystripclus53X","Flag_trkPOG_logErrorTooManyClusters","Flag_METFilters"),
  inputfiltersfromtrig = cms.InputTag("TriggerResults")
)
## [EventInfo]

## [Event]
icEventProducer = cms.EDProducer('ICEventProducer')
## [Event]

