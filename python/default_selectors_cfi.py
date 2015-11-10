import FWCore.ParameterSet.Config as cms


selectedVertices = cms.EDFilter("VertexRefSelector",
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("ndof >= 4 & abs(z) <= 24 & abs(position.Rho) <= 2")
)

selectedElectrons = cms.EDFilter("GsfElectronRefSelector",
  src = cms.InputTag("gsfElectrons"),
  cut = cms.string("pt > 20 & abs(eta) < 2.5")
)

selectedMuons = cms.EDFilter("MuonRefSelector",
  src = cms.InputTag("muons"),
  cut = cms.string("pt > 20 & abs(eta) < 2.1")
)

selectedPFMuons = cms.EDFilter("GenericPFCandidateSelector",
  src = cms.InputTag("particleFlow"),
  cut = cms.string("pt > 20 & abs(eta) < 2.1 & abs(pdgId) == 13")
)

selectedCaloJets = cms.EDFilter("CaloJetRefSelector",
  src = cms.InputTag("ak5CaloJets"),
  cut = cms.string("abs(eta) < 4.7")
)

selectedJPTJets = cms.EDFilter("JPTJetRefSelector",
  src = cms.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
  cut = cms.string("pt > 30 & abs(eta) < 4.7")
)

selectedPFJets = cms.EDFilter("PFJetRefSelector",
  src = cms.InputTag("ak5PFJets"),
  cut = cms.string("abs(eta) < 4.7")
)

selectedPFTaus = cms.EDFilter("PFTauRefSelector",
  src = cms.InputTag("hpsPFTauProducer"),
  cut = cms.string("pt > 18 & decayMode > -0.5")
)

#process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
prunedGenParticles = cms.EDProducer("ICGenParticlePruner53X",
  src = cms.InputTag("genParticles"),
  select = cms.vstring(
    "drop  *",
    "keep status == 3 || status == 22 || status == 23",  # all status 3
    "keep abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15",  # all electrons and muons
    "keep abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16",  # keep neutrinos
    "keep++ abs(pdgId) == 15",                                        # but keep keep taus with decays
    "keep (4 <= abs(pdgId) <= 5)", # keep heavy flavour quarks for parton-based jet flavour
    "keep (400 <= abs(pdgId) < 600) || (4000 <= abs(pdgId) < 6000)", # keep b and c hadrons for hadron-based jet flavour
    "keep abs(pdgId) = 10411 || abs(pdgId) = 10421 || abs(pdgId) = 10413 || abs(pdgId) = 10423 || abs(pdgId) = 20413 || abs(pdgId) = 20423 || abs(pdgId) = 10431 || abs(pdgId) = 10433 || abs(pdgId) = 20433", # additional c hadrons for jet fragmentation studies
    "keep abs(pdgId) = 10511 || abs(pdgId) = 10521 || abs(pdgId) = 10513 || abs(pdgId) = 10523 || abs(pdgId) = 20513 || abs(pdgId) = 20523 || abs(pdgId) = 10531 || abs(pdgId) = 10533 || abs(pdgId) = 20533 || abs(pdgId) = 10541 || abs(pdgId) = 10543 || abs(pdgId) = 20543" # additional b hadrons for jet fragmentation studies
  )
)
