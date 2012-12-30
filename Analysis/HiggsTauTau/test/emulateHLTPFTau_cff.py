
## cone-based tau to mimic trigger

import FWCore.ParameterSet.Config as cms

from RecoJets.JetAssociationProducers.ic5PFJetTracksAssociatorAtVertex_cfi \
        import ic5PFJetTracksAssociatorAtVertex
hltAK5PFJetTracksAssociatorAtVertex = ic5PFJetTracksAssociatorAtVertex.clone()
hltAK5PFJetTracksAssociatorAtVertex.jets = "ak5PFJets"

hltPFTauTagInfo = cms.EDProducer(
    "PFRecoTauTagInfoProducer",
    tkminTrackerHitsn = cms.int32( 3 ),
    tkminPt = cms.double( 0.0 ),
    tkmaxChi2 = cms.double( 100.0 ),
    ChargedHadrCand_AssociationCone = cms.double( 0.8 ),
    ChargedHadrCand_tkminTrackerHitsn = cms.int32( 0 ),
    ChargedHadrCand_tkmaxChi2 = cms.double( 100.0 ),
    tkPVmaxDZ = cms.double( 0.4 ),
    GammaCand_EcalclusMinEt = cms.double( 0.5 ),
    tkminPixelHitsn = cms.int32( 0 ),
    PVProducer = cms.InputTag( "offlinePrimaryVertices" ),
    PFCandidateProducer = cms.InputTag( "particleFlow" ),
    ChargedHadrCand_tkminPt = cms.double( 0.0 ),
    ChargedHadrCand_tkmaxipt = cms.double( 0.2 ),
    ChargedHadrCand_tkminPixelHitsn = cms.int32( 0 ),
    UsePVconstraint = cms.bool( True ),
    NeutrHadrCand_HcalclusMinEt = cms.double( 0.5 ),
    PFJetTracksAssociatorProducer = cms.InputTag( "hltAK5PFJetTracksAssociatorAtVertex" ),
    smearedPVsigmaY = cms.double( 0.0015 ),
    smearedPVsigmaX = cms.double( 0.0015 ),
    smearedPVsigmaZ = cms.double( 0.0050 ),
    ChargedHadrCand_tkPVmaxDZ = cms.double( 0.4 ),
    tkmaxipt = cms.double( 0.2 )
    )
hltPFTaus = cms.EDProducer(
    "PFRecoTauProducer",
    Rphi = cms.double( 2.0 ),
    LeadTrack_minPt = cms.double( 0.0 ),
    PVProducer = cms.InputTag( "offlinePrimaryVertices" ),
    ECALSignalConeSizeFormula = cms.string( "0.18" ),
    #ECALSignalConeSizeFormula = cms.string( "0.2" ), #for test, FIXME
    TrackerIsolConeMetric = cms.string( "DR" ),
    TrackerSignalConeMetric = cms.string( "DR" ),
    EcalStripSumE_deltaPhiOverQ_minValue = cms.double( 0.0 ),
    smearedPVsigmaX = cms.double( 0.0015 ),
    smearedPVsigmaY = cms.double( 0.0015 ),
    MatchingConeMetric = cms.string( "DR" ),
    TrackerSignalConeSizeFormula = cms.string( "0.18" ),
    #TrackerSignalConeSizeFormula = cms.string( "0.2" ), #for test, FIXME
    MatchingConeSizeFormula = cms.string( "0.2" ),
    TrackerIsolConeSize_min = cms.double( 0.0 ),
    MatchingConeSize_min = cms.double( 0.0 ),
    ElectronPreIDProducer = cms.InputTag( "elecpreid" ),
    ChargedHadrCandLeadChargedHadrCand_tksmaxDZ = cms.double( 0.4 ),
    TrackerIsolConeSize_max = cms.double( 0.6 ),
    TrackerSignalConeSize_max = cms.double( 0.2 ),
    HCALIsolConeMetric = cms.string( "DR" ),
    AddEllipseGammas = cms.bool( False ),
    maximumForElectrionPreIDOutput = cms.double( 0.0 ),
    TrackerSignalConeSize_min = cms.double( 0.0 ),
    JetPtMin = cms.double( 0.0 ),
    HCALIsolConeSizeFormula = cms.string( "0.5" ),
    AreaMetric_recoElements_maxabsEta = cms.double( 2.5 ),
    HCALIsolConeSize_max = cms.double( 0.6 ),
    Track_IsolAnnulus_minNhits = cms.uint32( 0 ),
    HCALSignalConeMetric = cms.string( "DR" ),
    ElecPreIDLeadTkMatch_maxDR = cms.double( 0.015 ),
    PFTauTagInfoProducer = cms.InputTag( "hltPFTauTagInfo" ),
    ECALIsolConeMetric = cms.string( "DR" ),
    ECALIsolConeSizeFormula = cms.string( "0.5" ),
    UseChargedHadrCandLeadChargedHadrCand_tksDZconstraint = cms.bool( False ),
    Algorithm = cms.string( "ConeBased" ),
    ECALIsolConeSize_max = cms.double( 0.6 ),
    ECALSignalConeMetric = cms.string( "DR" ),
    EcalStripSumE_deltaPhiOverQ_maxValue = cms.double( 0.0 ),
    HCALSignalConeSize_max = cms.double( 0.6 ),
    ECALSignalConeSize_min = cms.double( 0.0 ),
    EcalStripSumE_minClusEnergy = cms.double( 0.0 ),
    EcalStripSumE_deltaEta = cms.double( 0.0 ),
    TrackerIsolConeSizeFormula = cms.string( "0.45" ),
    #TrackerIsolConeSizeFormula = cms.string( "0.4" ), #for test, FIXME
    LeadPFCand_minPt = cms.double( 0.0 ),
    HCALSignalConeSize_min = cms.double( 0.0 ),
    ECALSignalConeSize_max = cms.double( 0.6 ),
    HCALSignalConeSizeFormula = cms.string( "0.1" ),
    putNeutralHadronsInP4 = cms.bool( False ),
    TrackLeadTrack_maxDZ = cms.double( 0.4 ),
    ChargedHadrCand_IsolAnnulus_minNhits = cms.uint32( 0 ),
    ECALIsolConeSize_min = cms.double( 0.0 ),
    UseTrackLeadTrackDZconstraint = cms.bool( False ),
    MaxEtInEllipse = cms.double( 2.0 ),
    DataType = cms.string( "AOD" ),
    smearedPVsigmaZ = cms.double( 0.0050 ),
    MatchingConeSize_max = cms.double( 0.6 ),
    HCALIsolConeSize_min = cms.double( 0.0 ),
    doOneProngTwoStrips = cms.bool( True ),
    minimumSignalCone = cms.double( 0.0 ),
    leadPionThreshold = cms.double( 1.0 ),
    gammaIsolationConeSize = cms.double( 0.5 ),
    neutrHadrIsolationConeSize = cms.double( 0.5 ),
    candOverlapCriterion = cms.string( "None" ),
    stripEtaAssociationDistance = cms.double( 0.05 ),
    oneProngTwoStripsPi0MassWindow = cms.vdouble( 0.0, 0.0 ),
    doThreeProng = cms.bool( True ),
    doOneProngStrip = cms.bool( True ),
    coneSizeFormula = cms.string( "2.8/ET" ),
    oneProngStripMassWindow = cms.vdouble( 0.0, 0.0 ),
    maximumSignalCone = cms.double( 1.8 ),
    coneMetric = cms.string( "DR" ),
    emMergingAlgorithm = cms.string( "None" ),
    chargeHadrIsolationConeSize = cms.double( 0.5 ),
    doOneProng = cms.bool( True ),
    useIsolationAnnulus = cms.bool( False ),
    threeProngMassWindow = cms.vdouble( 0.0, 0.0 ),
    tauPtThreshold = cms.double( 0.0 ),
    stripPhiAssociationDistance = cms.double( 0.2 ),
    stripCandidatesPdgIds = cms.vint32( 22, 11 ),
    stripPtThreshold = cms.double( 0.5 ),
    matchingCone = cms.double( 0.2 ),
    oneProngTwoStripsMassWindow = cms.vdouble( 0.0, 0.0 )
    )
hltPFTauTrackFindingDiscriminator = cms.EDProducer(
    "PFRecoTauDiscriminationByLeadingObjectPtCut",
    MinPtLeadingObject = cms.double( 0.0 ),
    Prediscriminants = cms.PSet(  BooleanOperator = cms.string( "and" ) ),
    UseOnlyChargedHadrons = cms.bool( True ),
    PFTauProducer = cms.InputTag( "hltPFTaus" )
    )
hltPFTauLooseIsolationDiscriminator = cms.EDProducer(
    "PFRecoTauDiscriminationByIsolation",
    PFTauProducer = cms.InputTag( "hltPFTaus" ),
    qualityCuts = cms.PSet(
        isolationQualityCuts = cms.PSet(
            minTrackHits = cms.uint32( 8 ),
            minTrackPt = cms.double( 1.5 ),
            maxTrackChi2 = cms.double( 100.0 ),
            minTrackPixelHits = cms.uint32( 3 ),
            minGammaEt = cms.double( 1.5 ),
            useTracksInsteadOfPFHadrons = cms.bool( False ),
            maxDeltaZ = cms.double( 0.2 ),
            maxTransverseImpactParameter = cms.double( 0.05 )
            ),
        signalQualityCuts = cms.PSet(
            minTrackPt = cms.double( 0.0 ),
            maxTrackChi2 = cms.double( 1000.0 ),
            useTracksInsteadOfPFHadrons = cms.bool( False ),
            minGammaEt = cms.double( 0.5 ),
            minTrackPixelHits = cms.uint32( 0 ),
            minTrackHits = cms.uint32( 3 ),
            maxDeltaZ = cms.double( 0.5 ),
            maxTransverseImpactParameter = cms.double( 0.2 )
            ),
        makeHisto = cms.bool(False),
        vxAssocQualityCuts = cms.PSet(
            minTrackVertexWeight = cms.double(-1),
            minTrackPt = cms.double(0.5),
            maxTrackChi2 = cms.double(100.0),
            minTrackPixelHits = cms.uint32(0),
            minGammaEt = cms.double(0.5),
            minTrackHits = cms.uint32(3),
            maxTransverseImpactParameter = cms.double(0.03)
        ),
        vertexTrackFiltering = cms.bool(False),
        recoverLeadingTrk = cms.bool(False),
        primaryVertexSrc = cms.InputTag( "offlinePrimaryVertices" ),
        pvFindingAlgo = cms.string( "highestPtInEvent" )
        ),
    maximumSumPtCut = cms.double( 6.0 ),
    deltaBetaPUTrackPtCutOverride = cms.double( 0.5 ),
    isoConeSizeForDeltaBeta = cms.double( 0.3 ),
    vertexSrc = cms.InputTag( "NotUsed" ),
    applySumPtCut = cms.bool( False ),
    rhoConeSize = cms.double( 0.5 ),
    ApplyDiscriminationByTrackerIsolation = cms.bool( True ),
    rhoProducer = cms.InputTag( 'kt6PFJets','rho' ),
    deltaBetaFactor = cms.string( "0.38" ),
    relativeSumPtCut = cms.double( 0.0 ),
    Prediscriminants = cms.PSet(
        BooleanOperator = cms.string( "and" ),
        leadTrack = cms.PSet(
            cut = cms.double( 0.5 ),
            Producer = cms.InputTag( "hltPFTauTrackFindingDiscriminator" ),
        ),
    ),
    applyOccupancyCut = cms.bool( True ),
    applyDeltaBetaCorrection = cms.bool( False ),
    applyRelativeSumPtCut = cms.bool( False ),
    maximumOccupancy = cms.uint32( 0 ),
    rhoUEOffsetCorrection = cms.double( 1.0 ),
    ApplyDiscriminationByECALIsolation = cms.bool( False ),
    storeRawSumPt = cms.bool( False ),
    applyRhoCorrection = cms.bool( False ),
    customOuterCone = cms.double( -1.0 ),
    particleFlowSrc = cms.InputTag( "particleFlow" )
    )
hltPatTaus = cms.EDProducer("PATTauProducer",
    # input
    tauSource = cms.InputTag("hltPFTaus"),

    # add user data
    userData = cms.PSet(
      # add custom classes here
      userClasses = cms.PSet(
        src = cms.VInputTag('')
      ),
      # add doubles here
      userFloats = cms.PSet(
        src = cms.VInputTag('')
      ),
      # add ints here
      userInts = cms.PSet(
        src = cms.VInputTag('')
      ),
      # add candidate ptrs here
      userCands = cms.PSet(
        src = cms.VInputTag('')
      ),
      # add "inline" functions here
      userFunctions = cms.vstring(),
      userFunctionLabels = cms.vstring()
    ),

    # jet energy corrections
    addTauJetCorrFactors = cms.bool(False),
    tauJetCorrFactorsSource = cms.VInputTag(cms.InputTag("patTauJetCorrFactors")),

    # embedding objects (for Calo- and PFTaus)
    embedLeadTrack = cms.bool(False), ## embed in AOD externally stored leading track
    embedSignalTracks = cms.bool(False), ## embed in AOD externally stored signal tracks
    embedIsolationTracks = cms.bool(False), ## embed in AOD externally stored isolation tracks
    # embedding objects (for PFTaus only)
    embedLeadPFCand = cms.bool(True), ## embed in AOD externally stored leading PFCandidate
    embedLeadPFChargedHadrCand = cms.bool(True), ## embed in AOD externally stored leading PFChargedHadron candidate
    embedLeadPFNeutralCand = cms.bool(True), ## embed in AOD externally stored leading PFNeutral Candidate
    embedSignalPFCands = cms.bool(True), ## embed in AOD externally stored signal PFCandidates
    embedSignalPFChargedHadrCands = cms.bool(True), ## embed in AOD externally stored signal PFChargedHadronCandidates
    embedSignalPFNeutralHadrCands = cms.bool(True), ## embed in AOD externally stored signal PFNeutralHadronCandidates
    embedSignalPFGammaCands = cms.bool(True), ## embed in AOD externally stored signal PFGammaCandidates
    embedIsolationPFCands = cms.bool(True), ## embed in AOD externally stored isolation PFCandidates
    embedIsolationPFChargedHadrCands = cms.bool(True), ## embed in AOD externally stored isolation PFChargedHadronCandidates
    embedIsolationPFNeutralHadrCands = cms.bool(True), ## embed in AOD externally stored isolation PFNeutralHadronCandidates
    embedIsolationPFGammaCands = cms.bool(True), ## embed in AOD externally stored isolation PFGammaCandidates

    # embed IsoDeposits
    isoDeposits = cms.PSet(),

    # user defined isolation variables the variables defined here will be accessible
    # via pat::Tau::userIsolation(IsolationKeys key) with the key as defined in
    # DataFormats/PatCandidates/interface/Isolation.h
    #
    # (set Pt thresholds for PFChargedHadrons (PFGammas) to 1.0 (1.5) GeV,
    # matching the thresholds used when computing the tau iso. discriminators
    # in RecoTauTag/RecoTau/python/PFRecoTauDiscriminationByIsolation_cfi.py)
    userIsolation = cms.PSet(),

    # tau ID (for efficiency studies)
    addTauID     = cms.bool(True),
    tauIDSources = cms.PSet(
        # configure many IDs as InputTag <someName> = <someTag>
        # you can comment out those you don't want to save some
        # disk space
        decayModeFinding = cms.InputTag("hltPFTauTrackFindingDiscriminator"),
        byIsolation = cms.InputTag("hltPFTauLooseIsolationDiscriminator"),
    ),

    # mc matching configurables
    addGenMatch      = cms.bool(False),
    embedGenMatch    = cms.bool(False),
    genParticleMatch = cms.InputTag(""),
    addGenJetMatch   = cms.bool(False),
    embedGenJetMatch = cms.bool(False),
    genJetMatch      = cms.InputTag(""),

    # efficiencies
    addEfficiencies = cms.bool(False),
    efficiencies    = cms.PSet(),

    # resolution
    addResolutions  = cms.bool(False),
    resolutions     = cms.PSet()
)
selectedHltPatTaus = cms.EDFilter(
    "PATTauSelector",
    src = cms.InputTag("hltPatTaus"),
    cut = cms.string("pt>17"),
    filter = cms.bool(False)
    )

isolatedHltPatTaus = cms.EDFilter(
    "PATTauSelector",
    src = cms.InputTag("hltPatTaus"),
    cut = cms.string("pt>20 "+
                     "&& tauID('decayModeFinding')>0.5 "+
                     "&& tauID('byIsolation')>0.5"),
    filter = cms.bool(False)
    )

hltTauCounter = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("isolatedHltPatTaus"),
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(999),
    )

hltPFTauSequence = cms.Sequence(
    hltAK5PFJetTracksAssociatorAtVertex+
    hltPFTauTagInfo+hltPFTaus+
    hltPFTauTrackFindingDiscriminator+
    hltPFTauLooseIsolationDiscriminator+
    hltPatTaus+
    selectedHltPatTaus)

hltPFTauFilterSeqence = cms.Sequence(
    isolatedHltPatTaus+
    hltTauCounter)
