import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi import patMuons
from PhysicsTools.PatAlgos.producersLayer1.metProducer_cfi import patMETs

patMuons.addGenMatch = cms.bool(False)

goodVertex = cms.EDFilter("VertexSelector",
	src = cms.InputTag("offlinePrimaryVertices"),
	cut = cms.string("(!isFake) & ndof > 3 & abs(z) < 15 & position.Rho < 2"),
	filter = cms.bool(True)
)

goodMuons = cms.EDFilter("PATMuonSelector",
	src = cms.InputTag("patMuons"),
	cut = cms.string(
		'pt > 10 && abs(eta) < 2.5 && isGlobalMuon && isTrackerMuon '
		' && innerTrack.hitPattern.numberOfValidTrackerHits > 9 & innerTrack.hitPattern.numberOfValidPixelHits > 0'
		' && abs(dB) < 0.2 && globalTrack.normalizedChi2 < 10'
		' && globalTrack.hitPattern.numberOfValidMuonHits > 0 && numberOfMatches > 1'
	),
	filter = cms.bool(True)
)

goodMuonsPFIso = cms.EDFilter("PATMuonSelector",
	src = cms.InputTag("goodMuons"),
	cut = cms.string('(pfIsolationR04().sumChargedParticlePt + max(pfIsolationR04().sumNeutralHadronEt + pfIsolationR04().sumPhotonEt - 0.5 * pfIsolationR04().sumPUPt,0.0)) < 0.12*pt'),
        minNumber = cms.uint32(1),
        maxNumber = cms.uint32(1),                              
	filter = cms.bool(True)
)

patpfMet = cms.EDProducer(
    "PATMETProducer",
    metSource  = cms.InputTag("pfMet"),
    # muon correction
    addMuonCorrections = cms.bool(True),
    muonSource         = cms.InputTag("muons"),
    # mc matching configurables
    addGenMET    = cms.bool(False),
    genMETSource = cms.InputTag(""),
    addEfficiencies = cms.bool(False),
    efficiencies    = cms.PSet(),
    
    # resolution
    addResolutions  = cms.bool(False),
    resolutions     = cms.PSet(),
    )

WmunuCandidatesGe0IsoMuons = cms.EDProducer("CandViewShallowCloneCombiner",
    checkCharge = cms.bool(False),
    cut = cms.string(''), #charge = 0 & max(daughter(0).pt,daughter(1).pt)>20'),
    decay = cms.string("goodMuons patpfMet")
)

WmunuCandidatesGe1IsoMuons = WmunuCandidatesGe0IsoMuons.clone(
	decay = cms.string("goodMuonsPFIso patpfMet")
)

WmunuFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("WmunuCandidatesGe0IsoMuons"), # loose selection 
    #src = cms.InputTag("WmunuCandidatesGe1IsoMuons"),  # tight selection                            
    minNumber = cms.uint32(1)
)

WmunuSelectionSequence = cms.Sequence(
  goodVertex
  * patMuons
  * patpfMet
  * goodMuons
  * goodMuonsPFIso 
  * WmunuCandidatesGe0IsoMuons 
  * WmunuCandidatesGe1IsoMuons
  * WmunuFilter
)
