#include "HiggsTauTau/interface/HTTSync.h"
#include "TVector3.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"

namespace ic {

HTTSync::HTTSync(std::string const& name, std::string const& output_name,
                 ic::channel channel)
    : ModuleBase(name), channel_(channel) {
  output_name_ = output_name;
  jet_eta_ = 4.7;
  is_embedded_ = false;
  select_sel_mode_ = -1;
  select_category_ = "";
}

HTTSync::~HTTSync() { ; }

int HTTSync::PreAnalysis() {
  lOFile = new TFile(output_name_.c_str(), "RECREATE");
  lOFile->cd();
  // Tree should be named "TauCheck" to aid scripts which
  // make comparisons between sync trees
  lOTree = new TTree("TauCheck", "TauCheck");

  // The sync tree is filled for all events passing the di-lepton
  // selections in each channel. This includes vertex selection,
  // trigger, ID, isolation, di-lepton and extra lepton vetoes.
  // Topological (e.g. mT) and opposite-charge requirements are
  // not applied.

  // Note: not all of the following variables were in the original
  // list of sync tree variables, and not all are necessary/used in
  // the legacy htt anaylsis

  // Lepton properties are numbered as follows for each channel:
  // electron     (1)  + tau        (2)
  // muon         (1)  + tau        (2)
  // electron     (1)  + muon       (2)
  // high pT tau  (1)  + low pT tau (2)

  // Run
  lOTree->Branch("run", &lRun, "lRun/I");
  // Lumi
  lOTree->Branch("lumi", &lLumi, "lLumi/I");
  // Event
  lOTree->Branch("evt", &lEvt, "lEvt/I");

  // Number of primary vertices passing good vertex selection
  lOTree->Branch("npv", &lNPV, "lNPV/I");
  // Number of in-time pileup interactions (used for pileup reweighting)
  lOTree->Branch("npu", &lNPU, "lNPU/I");
  // The rho used for jet energy corrections
  lOTree->Branch("rho", &lRho, "lRho/F");

  // The lumi scaling factor for mc * additional weights
  // (not filled in IC trees!)
  lOTree->Branch("mcweight", &lMCWeight, "lMCWeight/F");
  // Pileup weight
  lOTree->Branch("puweight", &lPUWeight, "lPUWeight/F");

  // Tag-and-probe weights for leptons
  // Total trigger weight for lepton 1
  lOTree->Branch("trigweight_1", &lTrigweight_1, "trigweight_1/F");
  // Total trigger weight for lepton 2
  lOTree->Branch("trigweight_2", &lTrigweight_2, "trigweight_2/F");
  // Total ID weight for lepton 1
  lOTree->Branch("idweight_1", &lIdweight_1, "idweight_1/F");
  // Total ID weight for lepton 2
  lOTree->Branch("idweight_2", &lIdweight_2, "idweight_2/F");
  // Total iso weight for lepton 1
  lOTree->Branch("isoweight_1", &lIsoweight_1, "isoweight_1/F");
  // Total iso weight for lepton 2
  lOTree->Branch("isoweight_2", &lIsoweight_2, "isoweight_2/F");
  // Product of all trigger, ID and iso weights
  lOTree->Branch("effweight", &lEffWeight, "lEffWeight/F");
  // Jet->tau fake rate weight (pT-dependent)
  lOTree->Branch("fakeweight", &lFakeWeight, "lFakeWeight/F");
  // Product of all embedded weights, but only for rechit samples
  lOTree->Branch("embeddedWeight", &lEmbeddedWeight, "lEmbeddedWeight/F");
  // Higgs pt weights (for ggh samples)
  lOTree->Branch("signalWeight", &lSignalWeight, "lSignalWeight/F");
  // Total combined event weight (excluding lumi weighting)
  // NB: may contain weights not included in the above
  lOTree->Branch("weight", &lWeight, "lWeight/F");

  // Visible di-tau mass
  lOTree->Branch("mvis", &lMVis, "lMVis/F");
  // SVFit di-tau mass
  lOTree->Branch("m_sv", &lMSV, "lMSV/F");
  // SVFit di-tau pt (only for Markov-Chain SVFit)
  lOTree->Branch("pt_sv", &lPtSV, "lPtSV/F");
  // SVFit di-tau eta (only for Markov-Chain SVFit)
  lOTree->Branch("eta_sv", &lEtaSV, "lEtaSV/F");
  // SVFit di-tau phi (only for Markov-Chain SVFit)
  lOTree->Branch("phi_sv", &lPhiSV, "lPhiSV/F");
  // High energy scale SVFit mass (not filled)
  lOTree->Branch("m_sv_Up", &lMSVUp, "lMSVUp/F");
  // Low energy scale SVFit mass (not filled)
  lOTree->Branch("m_sv_Down", &lMSVDown, "lMSVDown/F");

  // Lepton 1 properties
  // pt (including effect of any energy scale corrections)
  lOTree->Branch("pt_1", &lPt1, "lPt1/F");
  // phi
  lOTree->Branch("phi_1", &lPhi1, "lPhi1/F");
  // eta
  lOTree->Branch("eta_1", &lEta1, "lEta1/F");
  // mass
  lOTree->Branch("m_1", &lM1, "lM1/F");
  // charge
  lOTree->Branch("q_1", &lq1, "lq1/I");
  // delta-beta corrected isolation (relative or absolute as appropriate)
  // If lepton 1 is a tau, this is the value of byIsolationMVAraw,
  // which is no longer used in the analysis, but retained for legacy
  // reasons
  lOTree->Branch("iso_1", &lIso1, "lIso1/F");
  // If an electron, the output of the ID MVA, zero otherwise
  lOTree->Branch("mva_1", &lMVA1, "lMMVA1/F");
  // Transverse (x-y) impact parameter w.r.t to the primary vertex
  lOTree->Branch("d0_1", &lD01, "lD0/F");
  // Longitudinal (z) impact parameter w.r.t to the primary vertex
  lOTree->Branch("dZ_1", &lDZ1, "lDZ/F");
  // Whether lepton passes ID selection (always true in IC ntuples)
  lOTree->Branch("passid_1", &lPassId1, "lPassId1/B");
  // Whether lepton passes iso selection (always true in IC ntuples)
  lOTree->Branch("passiso_1", &lPassIso1, "lPassIso1/B");
  // Transverse mass of lepton 1 and MVA MET
  lOTree->Branch("mt_1", &lMt1, "lMt1/F");

  // Lepton 2 properties
  // pt (including effect of any energy scale corrections)
  lOTree->Branch("pt_2", &lPt2, "lPt2/F");
  // phi
  lOTree->Branch("phi_2", &lPhi2, "lPhi2/F");
  // eta
  lOTree->Branch("eta_2", &lEta2, "lEta2/F");
  // mass
  lOTree->Branch("m_2", &lM2, "lM2/F");
  // charge
  lOTree->Branch("q_2", &lq2, "lq2/I");
  // delta-beta corrected isolation (relative or absolute as appropriate)
  // If lepton 2 is a tau, this is the value of byIsolationMVAraw,
  // which is no longer used in the analysis, but retained for legacy
  // reasons
  lOTree->Branch("iso_2", &lIso2, "lIso2/F");
  // Transverse (x-y) impact parameter w.r.t to the primary vertex
  lOTree->Branch("d0_2", &lD02, "lD02/F");
  // Longitudinal (z) impact parameter w.r.t to the primary vertex
  lOTree->Branch("dZ_2", &lDZ2, "lDZ2/F");
  // If an electron, the output of the ID MVA, zero otherwise
  lOTree->Branch("mva_2", &lMVA2, "lMMVA2/F");
  // Whether lepton passes ID selection (always true in IC ntuples)
  lOTree->Branch("passid_2", &lPassId2, "lPassId2/B");
  // Whether lepton passes iso selection (always true in IC ntuples)
  lOTree->Branch("passiso_2", &lPassIso2, "lPassIso2/B");
  // Transverse mass of lepton 2 and MVA MET
  lOTree->Branch("mt_2", &lMt2, "lMt2/F");

  // Variables defined when lepton 2 is a tau
  // raw value of the 3hits delta-beta isolation
  lOTree->Branch("byCombinedIsolationDeltaBetaCorrRaw3Hits_2", &l3Hits_2,
                 "byCombinedIsolationDeltaBetaCorrRaw3Hits_2/F");
  // raw value of the anti-electron MVA3 output
  lOTree->Branch("againstElectronMVA3raw_2", &lagainstElectronMVA3raw_2,
                 "againstElectronMVA3raw_2/F");
  // raw value of the MVA2 isolation
  lOTree->Branch("byIsolationMVA2raw_2", &lbyIsolationMVA2raw_2,
                 "byIsolationMVA2raw_2/F");
  // output of againstMuonLoose2
  lOTree->Branch("againstMuonLoose2_2", &lagainstMuonLoose2_2,
                 "againstMuonLoose2_2/F");
  // output of againstMuonMedium2
  lOTree->Branch("againstMuonMedium2_2", &lagainstMuonMedium2_2,
                 "againstMuonMedium2_2/F");
  // output of againstMuonTight2
  lOTree->Branch("againstMuonTight2_2", &lagainstMuonTight2_2,
                 "againstMuonTight2_2/F");

  // Uncorrected PF MET (not used in analysis)
  lOTree->Branch("met", &lMet, "lMet/F");
  // Uncorrected PF MET phi (not used in analysis)
  lOTree->Branch("metphi", &lMetPhi, "lMetPhi/F");
  // Elements of the PF MET covariance matrix (not used in analysis)
  lOTree->Branch("metcov00", &lMetCov00, "lMetCov00/F");
  lOTree->Branch("metcov01", &lMetCov01, "lMetCov01/F");
  lOTree->Branch("metcov10", &lMetCov10, "lMetCov10/F");
  lOTree->Branch("metcov11", &lMetCov11, "lMetCov11/F");

  // Following variables used for soft muon + tau + MET analysis
  // L1 MET
  lOTree->Branch("l1met", &lL1Met, "lL1Met/F");
  // L1 MET phi
  lOTree->Branch("l1metphi", &lL1MetPhi, "lL1MetPhi/F");
  // Corrected L1 MET
  lOTree->Branch("l1metcorr", &lL1MetCorr, "lL1Metcorr/F");
  // Calo MET (NoHF)
  lOTree->Branch("calomet", &lCaloMet, "lCaloMet/F");
  // Calo MET phi (NoHF)
  lOTree->Branch("calometphi", &lCaloMetPhi, "lCaloMetPhi/F");
  // Corrected calo MET (NoHF)
  lOTree->Branch("calometcorr", &lCaloMetCorr, "lCaloMetCorr/F");
  // Corrected calo MET phi (NoHF)
  lOTree->Branch("calometphicorr", &lCaloMetPhiCorr, "lCaloMetPhiCorr/F");

  // MVA MET
  lOTree->Branch("mvamet", &lMVAMet, "lMet/F");
  // MVA MET phi
  lOTree->Branch("mvametphi", &lMVAMetPhi, "lMetPhi/F");
  // Elements of the MVA MET covariance matrix
  lOTree->Branch("mvacov00", &lMVACov00, "lMVACov00/F");
  lOTree->Branch("mvacov01", &lMVACov01, "lMVACov01/F");
  lOTree->Branch("mvacov10", &lMVACov10, "lMVACov10/F");
  lOTree->Branch("mvacov11", &lMVACov11, "lMVACov11/F");

  // pt of the di-tau + MET system
  lOTree->Branch("pt_tt", &lPtTT, "lPtTT/F");

  // Visible pzeta
  lOTree->Branch("pzetavis", &lPZetaVis, "lPZetaVis/F");
  // MET pzeta
  lOTree->Branch("pzetamiss", &lPZetaMiss, "lPZetaMiss/F");
  // ttbar-rejection MVA output (emu channel only)
  lOTree->Branch("mva_gf", &em_gf_mva_, "MVAGF/F");
  // ttbar-rejection MVA output for VBF (deprecated)
  lOTree->Branch("mva_vbf", &em_vbf_mva_, "MVAVBF/F");

  // Jet properties
  // The following properties are for the leading (1) and sub-leading (2) jets
  // with pt > 30, |eta| < 4.7 after jet energy corrections, PF jet ID and
  // pileup jet ID are applied. Jets overlapping with either selected lepton
  // are not counted

  // Number of jets passing above selection
  lOTree->Branch("njets", &lNJets, "lNJets/I");
  // Number of jets passing above selection but with
  // pt > 20 instead of pt > 30
  lOTree->Branch("njetspt20", &lNJetsPt20, "lNJetsPt20/I");

  // Leading Jet
  // pt
  lOTree->Branch("jpt_1", &lJPt1, "lJPt1/F");
  // eta
  lOTree->Branch("jeta_1", &lJEta1, "lJEta1/F");
  // phi
  lOTree->Branch("jphi_1", &lJPhi1, "lJPhi1/F");
  // raw pt (before JEC)
  lOTree->Branch("jptraw_1", &lJPtRaw1, "lJPtRaw1/F");
  // pt uncertainty relative to corrected pt (not in IC ntuples)
  lOTree->Branch("jptunc_1", &lJPtUnc1, "lJPtUnc1/F");
  // Pileup ID MVA output
  lOTree->Branch("jmva_1", &lJMVA1, "lJMVA1/F");
  // Linear radial moment (not used in htt analysis)
  lOTree->Branch("jlrm_1", &lLRM1, "lLRM1/F");
  // Charged track multiplicity (not used in htt analysis)
  lOTree->Branch("jctm_1", &lCTM1, "lCTM1/I");
  // True if jet passes MVA pileup ID (deprecated, do not use)
  lOTree->Branch("jpass_1", &lJPass1, "lJPass1/B");

  // Sub-leading Jet
  // pt
  lOTree->Branch("jpt_2", &lJPt2, "lJPt2/F");
  // eta
  lOTree->Branch("jeta_2", &lJEta2, "lJEta2/F");
  // phi
  lOTree->Branch("jphi_2", &lJPhi2, "lJPhi2/F");
  // raw pt (before JEC)
  lOTree->Branch("jptraw_2", &lJPtRaw2, "lJPtRaw2/F");
  // pt uncertainty relative to corrected pt (not in IC ntuples)
  lOTree->Branch("jptunc_2", &lJPtUnc2, "lJPtUnc2/F");
  // Pileup ID MVA output
  lOTree->Branch("jmva_2", &lJMVA2, "lJMVA2/F");
  // Linear radial moment (not used in htt analysis)
  lOTree->Branch("jlrm_2", &lLRM2, "lLRM2/F");
  // Charged track multiplicity (not used in htt analysis)
  lOTree->Branch("jctm_2", &lCTM2, "lCTM2/I");
  // True if jet passes MVA pileup ID (deprecated, do not use)
  lOTree->Branch("jpass_2", &lJPass2, "lJPass2/B");

  // Di-jet properties
  // Calculated with leading and sub-leading jets when njets >= 2
  // di-jet mass
  lOTree->Branch("mjj", &lMJJ, "lMJJ/F");
  // absolute difference in eta
  lOTree->Branch("jdeta", &lJDEta, "lJDEta/F");
  // number of jets, passing above selections, in pseudorapidity gap
  // between jets
  lOTree->Branch("njetingap", &lNJetInGap, "lNJetInGap/I");

  // B-Tagged Jet properties
  // The following properties are for the leading (in pt) CSV medium b-tagged
  // jet with pt > 20, |eta| < 2.4 after jet energy corrections, PF jet ID and
  // pileup jet ID are applied. Jets overlapping with either selected lepton
  // are not counted

  // Number of b-tagging jets passing above selections
  lOTree->Branch("nbtag", &lNBTag, "lNBTag/I");
  // pt
  lOTree->Branch("bpt", &lBTagPt, "lBTagPt/F");
  // eta
  lOTree->Branch("beta", &lBTagEta, "lBTagEta/F");
  // phi
  lOTree->Branch("bphi", &lBTagPhi, "lBTagPhi/F");

  // Variables that go into the VBF MVA
  // Deprecated since HCP 2012
  lOTree->Branch("mva", &lMVA, "lMVA/F");
  // Delta Phi between two leading jets
  lOTree->Branch("jdphi", &lJDPhi, "lJDPhi/F");
  // Pt of the di jet system
  lOTree->Branch("dijetpt", &lDiJetPt, "lDiJetPt/F");
  // Phi of the di jet system
  lOTree->Branch("dijetphi", &lDiJetPhi, "lDiJetPhi/F");
  // Phi of the di jet system - Higgs system phi
  lOTree->Branch("hdijetphi", &lHDJetPhi, "lHDJetPhi/F");
  // TMath::Min(eta_vis - jeta,eta_vis,jeta2);
  lOTree->Branch("visjeteta", &lVisJetEta, "lVisJetEta/F");
  // Pt Vis
  lOTree->Branch("ptvis", &lPtVis, "lPtVis/F");

  return 0;
}

int HTTSync::Execute(TreeEvent *event) {
  if (select_category_ != "") {
    if (event->Exists("cat_status")) {
      std::map<std::string, bool> cat_status =
          event->Get<std::map<std::string, bool>>("cat_status");
      if (cat_status.count(select_category_) > 0) {
        if (!(cat_status[select_category_])) return 0;
      } else {
        return 0;
      }
    }
  }

  if (select_sel_mode_ >= 0) {
    unsigned sel_mode  = event->Get<unsigned>("sel_mode");
    if (int(sel_mode) != select_sel_mode_) return 0;
  }

  EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
  std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
  std::vector<Tau*> taus = event->GetPtrVec<Tau>("taus");
  ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 0.0, jet_eta_));
  std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));

  std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>("emtauCandidates");

  Met const* pfMet = event->GetPtr<Met>("pfMet");
  Met const* pfMetMVA = event->GetPtr<Met>("pfMVAMet");
  Met const* selectedMet = event->GetPtr<Met>(met_label_);

  std::vector<PileupInfo *> puInfo;
  float true_int = -1;

  if (event->Exists("pileupInfo")) {
    puInfo = event->GetPtrVec<PileupInfo>("pileupInfo");
    for (unsigned i = 0; i < puInfo.size(); ++i) {
      if (puInfo[i]->bunch_crossing() == 0)
        true_int = puInfo[i]->true_num_interactions();
    }
  }

  lRun = eventInfo->run();
  lLumi = eventInfo->lumi_block();
  lEvt = eventInfo->event();

  //Event Variables
  lNPV = eventInfo->good_vertices();
  lNPU = true_int;
  lRho = eventInfo->jet_rho();

  //Event Weights
  lMCWeight = 0.0;
  if (!is_embedded_ && event->Exists("pileupInfo")) lPUWeight = eventInfo->weight("pileup");
  if (event->Exists("trigweight_1")) lTrigweight_1 = event->Get<double>("trigweight_1");
  if (event->Exists("trigweight_2")) lTrigweight_2 = event->Get<double>("trigweight_2");
  if (event->Exists("idweight_1")) lIdweight_1 = event->Get<double>("idweight_1");
  if (event->Exists("idweight_2")) lIdweight_2 = event->Get<double>("idweight_2");
  if (event->Exists("isoweight_1")) lIsoweight_1 = event->Get<double>("isoweight_1");
  if (event->Exists("isoweight_2")) lIsoweight_2 = event->Get<double>("isoweight_2");
  if (eventInfo->weight_defined("lepton")) lEffWeight = eventInfo->weight("lepton");
  if (eventInfo->weight_defined("tau_fake_weight")) lFakeWeight = eventInfo->weight("tau_fake_weight");
  if (eventInfo->weight_defined("tau_mode_scale")) lEffWeight *= eventInfo->weight("tau_mode_scale");
  lWeight = eventInfo->total_weight();

  if (eventInfo->weight_defined("tauspinner")) {
    lEmbeddedWeight = eventInfo->weight("tauspinner") *
      eventInfo->weight("zmm_eff") *
      //eventInfo->weight("muon_rad") *
      eventInfo->weight("kin_weight1") *
      eventInfo->weight("kin_weight2") *
      eventInfo->weight("kin_weight3") *
      eventInfo->weight("embed_weight");
   } else {
    lEmbeddedWeight = 0.;
   }
  if (eventInfo->weight_defined("ggh")) {
    lSignalWeight = eventInfo->weight("ggh");
   } else {
    lSignalWeight = 0.;
   }

  ///First lepton :  muon for mu Tau, electron for e Tau, electron for e mu, Leading (in pT) Tau for Tau Tau
  Candidate* lepton = dilepton.at(0)->GetCandidate("lepton1");
  Candidate* tau = dilepton.at(0)->GetCandidate("lepton2");

  lPt1 = lepton->pt();
  lPhi1 = lepton->phi();
  lEta1 = lepton->eta();
  lM1 = lepton->vector().M();
  lq1 = lepton->charge();
  lPt2 = tau->pt();
  lPhi2 = tau->phi();
  lEta2 = tau->eta();
  lM2 = tau->vector().M();
  lq2 = tau->charge();

  lMVis = dilepton.at(0)->M();

  if (event->Exists("svfitMass")) {
    lMSV = event->Get<double>("svfitMass");
  } else {
    lMSV = -999.;
  }
  if (event->Exists("svfitHiggs")) {
    Candidate const& higgs = event->Get<Candidate>("svfitHiggs");
    lPtSV = higgs.pt();
    lEtaSV = higgs.eta();
    lPhiSV = higgs.phi();
  } else {
    lPtSV = -999.;
    lEtaSV = -999.;
    lPhiSV = -999.;
  }
  lPtTT = (dilepton.at(0)->vector() + selectedMet->vector()).pt();

  if (channel_ == channel::et || channel_ == channel::etmet) {
    Electron* elec = dynamic_cast<Electron*>(lepton);
    double iso =  elec->dr04_pfiso_charged_all()
                  + std::max(elec->dr04_pfiso_neutral() + elec->dr04_pfiso_gamma() - 0.5 * elec->dr04_pfiso_pu(), 0.0);
    iso = iso / elec->pt();
    lIso1 = iso;
    lMVA1 = elec->GetIdIso("mvaNonTrigV0");
    lD01 = elec->dxy_vertex();
    lDZ1 = elec->dz_vertex();
    lPassId1 = true;
    lPassIso1 = true;
    lMt1 = MT(elec, selectedMet);

    Tau* htau = dynamic_cast<Tau*>(tau);
    lIso2 = htau->GetTauID("byIsolationMVAraw");
    lMVA2 = htau->GetTauID("againstElectronMVA");
    l3Hits_2 = htau->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? htau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
    lagainstElectronMVA3raw_2 = htau->HasTauID("againstElectronMVA3raw") ? htau->GetTauID("againstElectronMVA3raw") : 0. ;
    lbyIsolationMVA2raw_2 = htau->HasTauID("byIsolationMVA2raw") ? htau->GetTauID("byIsolationMVA2raw") : 0. ;
    lagainstMuonLoose2_2 = htau->HasTauID("againstMuonLoose2") ? htau->GetTauID("againstMuonLoose2") : 0. ;
    lagainstMuonMedium2_2 = htau->HasTauID("againstMuonMedium2") ? htau->GetTauID("againstMuonMedium2") : 0. ;
    lagainstMuonTight2_2 = htau->HasTauID("againstMuonTight2") ? htau->GetTauID("againstMuonTight2") : 0. ;
    lPassId2 = true;
    lPassIso2 = true;
    lMt2 = MT(htau, selectedMet);

  } else if (channel_ == channel::mt || channel_ == channel::mtmet) {
    Muon* muon = dynamic_cast<Muon*>(lepton);
    double iso =  muon->dr04_pfiso_charged_all()
                  + std::max(muon->dr04_pfiso_neutral() + muon->dr04_pfiso_gamma() - 0.5 * muon->dr04_pfiso_pu(), 0.0);
    iso = iso / muon->pt();
    lIso1 = iso;
    lMVA1 = 0.0;
    lD01 = muon->dxy_vertex();
    lDZ1 = muon->dz_vertex();
    lPassId1 = true;
    lPassIso1 = true;
    lMt1 = MT(muon, selectedMet);
    Tau* htau = dynamic_cast<Tau*>(tau);
    lIso2 = htau->GetTauID("byIsolationMVAraw");
    lMVA2 = htau->GetTauID("againstElectronMVA");
    l3Hits_2 = htau->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? htau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
    lagainstElectronMVA3raw_2 = htau->HasTauID("againstElectronMVA3raw") ? htau->GetTauID("againstElectronMVA3raw") : 0. ;
    lbyIsolationMVA2raw_2 = htau->HasTauID("byIsolationMVA2raw") ? htau->GetTauID("byIsolationMVA2raw") : 0. ;
    lagainstMuonLoose2_2 = htau->HasTauID("againstMuonLoose2") ? htau->GetTauID("againstMuonLoose2") : 0. ;
    lagainstMuonMedium2_2 = htau->HasTauID("againstMuonMedium2") ? htau->GetTauID("againstMuonMedium2") : 0. ;
    lagainstMuonTight2_2 = htau->HasTauID("againstMuonTight2") ? htau->GetTauID("againstMuonTight2") : 0. ;
    lPassId2 = true;
    lPassIso2 = true;
    lMt2 = MT(htau, selectedMet);
  } else if (channel_ == channel::em) {
    if (lepton->charge() == tau->charge()) return 0;
    Electron* elec = dynamic_cast<Electron*>(lepton);
    double iso =  elec->dr04_pfiso_charged_all()
                  + std::max(elec->dr04_pfiso_neutral() + elec->dr04_pfiso_gamma() - 0.5 * elec->dr04_pfiso_pu(), 0.0);
    iso = iso / elec->pt();
    lIso1 = iso;
    lMVA1 = elec->GetIdIso("mvaNonTrigV0");
    lD01 = elec->dxy_vertex();
    lDZ1 = elec->dz_vertex();
    lPassId1 = true;
    lPassIso1 = true;
    lMt1 = MT(elec, selectedMet);

    Muon* muon = dynamic_cast<Muon*>(tau);
    double isom =  muon->dr04_pfiso_charged_all()
                  + std::max(muon->dr04_pfiso_neutral() + muon->dr04_pfiso_gamma() - 0.5 * muon->dr04_pfiso_pu(), 0.0);
    isom = isom / muon->pt();
    lIso2 = isom;
    lD02 = muon->dxy_vertex();
    lDZ2 = muon->dz_vertex();
    lMVA2 = 0.0;
    lPassId2 = true;
    lPassIso2 = true;
    lMt2 = MT(muon, selectedMet);
    em_gf_mva_ = event->Exists("em_gf_mva") ? event->Get<double>("em_gf_mva") : 0.;
    em_vbf_mva_ = event->Exists("em_vbf_mva") ? event->Get<double>("em_vbf_mva") : 0.;
  }

  //Met related variables
  lMet = pfMet->pt();
  lMetPhi = pfMet->phi();
  lMVAMet = pfMetMVA->pt();
  lMVAMetPhi = pfMetMVA->phi();
  lPZetaVis = PZetaVis(dilepton.at(0));
  lPZetaMiss = PZeta(dilepton.at(0), pfMetMVA, 0.0);
  //MET covariance matrices
  lMetCov00 = pfMet->xx_sig();
  lMetCov01 = pfMet->xy_sig();
  lMetCov10 = pfMet->yx_sig();
  lMetCov11 = pfMet->yy_sig();
  //MVAMet covariance matrices
  lMVACov00 = pfMetMVA->xx_sig();
  lMVACov01 = pfMetMVA->xy_sig();
  lMVACov10 = pfMetMVA->yx_sig();
  lMVACov11 = pfMetMVA->yy_sig();

  if (channel_ == channel::mtmet) {
    std::vector<Candidate *> & v_l1met = event->GetPtrVec<Candidate>("l1extraMET");
    Candidate *l1met = v_l1met.at(0);
    lL1Met = l1met->pt();
    lL1MetPhi = l1met->phi();
    lL1MetCorr = l1met->pt();
    Met const* calo_raw = event->GetPtr<Met>("metNoHF");
    Met const* calo_corr = event->GetPtr<Met>("metNoHFCorrected");
    lCaloMet = calo_raw->pt();
    lCaloMetPhi = calo_raw->phi();
    lCaloMetCorr = calo_corr->pt();
    lCaloMetPhiCorr = calo_corr->phi();
  } else {
    lL1Met = 0.;
    lL1MetPhi = 0.;
    lL1MetCorr = 0.;
    lCaloMetPhi = 0.;
    lCaloMetCorr = 0.;
    lCaloMetPhiCorr = 0.;
  }

  //First Jet   : leading jet after applying Jet energy corrections (excluding hadronic Tau)
  if (jets.size() >= 1) {
    lJPt1 = jets[0]->pt();
    lJEta1 = jets[0]->eta();
    lJPhi1 = jets[0]->phi();
    lJPtRaw1 = jets[0]->uncorrected_energy() * (jets[0]->pt() / jets[0]->energy());
    lJPtUnc1 = 0.0;
    lJMVA1 = jets[0]->pu_id_mva_value();
    lJPass1 = jets[0]->pu_id_mva_loose();
    lLRM1 = jets[0]->linear_radial_moment();
    lCTM1 = jets[0]->charged_multiplicity_nopu();
  } else {
    lJPt1 = -9999;
    lJEta1 = -9999;
    lJPhi1 = -9999;
    lJPtRaw1 = -9999;
    lJPtUnc1 = -9999;
    lJMVA1 = -9999;
    lJPass1 = false;
    lLRM1 = -9999;
    lCTM1 = -9999;
  }

  //Second Jet  : 2nd leading jet (in pt) afer applying Jet energy corrections (excluding Tau)
  if (jets.size() >= 2) {
    lJPt2 = jets[1]->pt();
    lJEta2 = jets[1]->eta();
    lJPhi2 = jets[1]->phi();
    lJPtRaw2 = jets[1]->uncorrected_energy() * (jets[1]->pt() / jets[1]->energy());
    lJPtUnc2 = 0.0;
    lJMVA2 = jets[1]->pu_id_mva_value();
    lJPass2 = jets[1]->pu_id_mva_loose();
    lLRM2 = jets[1]->linear_radial_moment();
    lCTM2 = jets[1]->charged_multiplicity_nopu();
  } else {
    lJPt2 = -9999;
    lJEta2 = -9999;
    lJPhi2 = -9999;
    lJPtRaw2 = -9999;
    lJPtUnc2 = -9999;
    lJMVA2 = -9999;
    lJPass2 = false;
    lLRM2 = -9999;
    lCTM2 = -9999;
  }

  std::vector<PFJet *> btag_jets = jets;
  ic::erase_if(btag_jets, !boost::bind(MinPtMaxEta, _1, 20, 2.4));
  if (event->Exists("retag_result")) {
    auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result");
    ic::erase_if(btag_jets, !boost::bind(IsReBTagged, _1, retag_result));
  } else {
    ic::erase_if(btag_jets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.679);
  }

  if (btag_jets.size() >= 1) {
  //B Tagged Jet : leading btagged jet (in pt) passing btag wp (pt > 20 + cvs medium)
  lBTagPt = btag_jets[0]->pt();
  lBTagEta = btag_jets[0]->eta();
  lBTagPhi = btag_jets[0]->phi();
  } else {
    lBTagPt = 0;
    lBTagEta = 0;
    lBTagPhi = 0;
  }

  //Di Jet kinematic variables for VBF selection ==> Two leading pT Jets
  if (jets.size() >= 2 && jets[0]->pt() > 20.0 && jets[1]->pt() > 20.0) {
    lMJJ = (jets[0]->vector() + jets[1]->vector()).M();
    lJDEta = fabs(jets[0]->eta() - jets[1]->eta());
    std::vector<PFJet*> nonvbf_jets = jets;
    std::vector<PFJet *> vbf_jets;
    vbf_jets.push_back(jets[0]);
    vbf_jets.push_back(jets[1]);
    double eta_high = (vbf_jets[0]->eta() > vbf_jets[1]->eta()) ? vbf_jets[0]->eta() : vbf_jets[1]->eta();
    double eta_low = (vbf_jets[0]->eta() > vbf_jets[1]->eta()) ? vbf_jets[1]->eta() : vbf_jets[0]->eta();
    ic::erase_if(nonvbf_jets, boost::bind(FoundIdInCollection<PFJet, PFJet>, _1, vbf_jets));
    std::vector<PFJet*> cjv_jets;
    for (unsigned i = 0; i < nonvbf_jets.size(); ++i) {
      if (nonvbf_jets[i]->pt() > 30.0 &&
          nonvbf_jets[i]->eta() > eta_low &&
          nonvbf_jets[i]->eta() < eta_high) cjv_jets.push_back(nonvbf_jets[i]);
    }
    lNJetInGap = cjv_jets.size();

    //
    // Compute MVA inputs
    //
    float dPhi = fabs(ROOT::Math::VectorUtil::DeltaPhi(jets[0]->vector(), jets[1]->vector()));

    // Di-tau transverse vector
    TVector3 vTau, vMu, vMET, vDiTau, vDiTauVis;
    vTau.SetPtEtaPhi(tau->pt(), tau->eta(), tau->phi());
    vMu.SetPtEtaPhi(lepton->pt(), lepton->eta(), lepton->phi());
    vMET.SetPtEtaPhi(selectedMet->pt(), 0, selectedMet->phi());
    vDiTau = vTau + vMu + vMET;
    vDiTauVis = vTau + vMu;

    // Di-jet transverse vector
    TVector3 vJet1, vJet2, vDiJet;
    vJet1.SetPtEtaPhi(vbf_jets[0]->pt(), vbf_jets[0]->eta(), vbf_jets[0]->phi());
    vJet2.SetPtEtaPhi(vbf_jets[1]->pt(), vbf_jets[1]->eta(), vbf_jets[1]->phi());
    vDiJet = vJet1 + vJet2;
    //float dijet_pt = vDiJet.Pt();
    //float dijet_phi = vDiJet.Phi();
    float dPhi_hj = fabs(ROOT::Math::VectorUtil::DeltaPhi(vDiTau, vDiJet));

    // Lorenzo's variables
    double C1 = std::min(fabs(vDiTauVis.Eta() - vbf_jets[0]->eta()), fabs(vDiTauVis.Eta() - vbf_jets[1]->eta()));
    double C2 = vDiTauVis.Pt();

    // Evaluate MVA
    // float mvaValue = reader->EvaluateMVA(vbfvars, "BDTG");
    float mvaValue = 0.0;
    lMVA = mvaValue;
    lJDPhi = dPhi;
    lDiJetPt = vDiJet.Pt();
    lDiJetPhi = vDiJet.Phi();
    lHDJetPhi = dPhi_hj;
    lVisJetEta = C1;
    lPtVis =C2;
  } else {
    lMJJ = -999.;
    lJDEta = -999.;
    lNJetInGap = 0;
    lMVA = -999.;
    lJDPhi = -999.;
    lDiJetPt = -999.;
    lDiJetPhi = -999.;
    lHDJetPhi = -999.;
    lVisJetEta = -999.;
    lPtVis = -999.;
  }

  //Variables that go o the VBF MVA

  //number of btags passing btag id ( pt > 20 )
  lNBTag = btag_jets.size();

  //number of jets passing jet id ( pt > 30 )
  ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 20, jet_eta_));
  lNJetsPt20 = jets.size();
  ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30, jet_eta_));
  lNJets = jets.size();
  lOTree->Fill();
  return 0;
}

int HTTSync::PostAnalysis() {
  lOFile->cd();
  lOTree->Write();
  lOFile->Close();
  return 0;
}

void HTTSync::PrintInfo() { ; }
}
