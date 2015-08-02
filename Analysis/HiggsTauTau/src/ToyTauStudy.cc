#include "HiggsTauTau/interface/ToyTauStudy.h"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include "UserCode/ICHiggsTauTau/interface/Track.hh"

namespace ic {

ToyTauStudy::ToyTauStudy(std::string const& name)
    : ModuleBase(name), dir_(nullptr), iso_deps_(nullptr) {
  th_pt_acc_   = 20.;
  th_eta_acc_  = 2.3;
  do_real_th_studies_ = true;
  do_fake_th_studies_ = false;
  taus_label_ = "taus";
}

ToyTauStudy::~ToyTauStudy() { ; }

ToyTauStudy::MainPlots::MainPlots(TFileDirectory dir) {
  unsigned pt_bins = 12;
  double pt_min = 20;
  double pt_max = 500;

  // Efficiency of matching gen->reco
  rec_eff_vs_pt = EfficiencyPlot1D(dir, "rec_eff_vs_pt", pt_bins, pt_min, pt_max);
  // Efficiency  of matching gen->reco & passing DMP
  rec_dm_eff_vs_pt = EfficiencyPlot1D(dir, "rec_dm_eff_vs_pt", pt_bins, pt_min, pt_max);

  // Efficiency of (DMF & acceptance & charged iso) cuts given a gen-level tau_h
  rec_dm_chiso0p8_eff_vs_pt = EfficiencyPlot1D(dir, "rec_dm_chiso0p8_eff_vs_pt", pt_bins, pt_min, pt_max);
  rec_dm_chiso1p0_eff_vs_pt = EfficiencyPlot1D(dir, "rec_dm_chiso1p0_eff_vs_pt", pt_bins, pt_min, pt_max);
  rec_dm_chiso2p0_eff_vs_pt = EfficiencyPlot1D(dir, "rec_dm_chiso2p0_eff_vs_pt", pt_bins, pt_min, pt_max);

  // Efficiency of (DMF & acceptance & combined iso) cuts given a gen-level tau_h
  rec_dm_cmbiso0p8_eff_vs_pt = EfficiencyPlot1D(dir, "rec_dm_cmbiso0p8_eff_vs_pt", pt_bins, pt_min, pt_max);
  rec_dm_cmbiso1p0_eff_vs_pt = EfficiencyPlot1D(dir, "rec_dm_cmbiso1p0_eff_vs_pt", pt_bins, pt_min, pt_max);
  rec_dm_cmbiso2p0_eff_vs_pt = EfficiencyPlot1D(dir, "rec_dm_cmbiso2p0_eff_vs_pt", pt_bins, pt_min, pt_max);

  rec_dm_cmbiso2p0_nh2p0_eff_vs_pt = EfficiencyPlot1D(dir, "rec_dm_cmbiso2p0_nh2p0_eff_vs_pt", pt_bins, pt_min, pt_max);
  rec_dm_cmbiso2p0_nh5p0_eff_vs_pt = EfficiencyPlot1D(dir, "rec_dm_cmbiso2p0_nh5p0_eff_vs_pt", pt_bins, pt_min, pt_max);
  rec_dm_cmbiso2p0_nh10p0_eff_vs_pt = EfficiencyPlot1D(dir, "rec_dm_cmbiso2p0_nh10p0_eff_vs_pt", pt_bins, pt_min, pt_max);
  rec_dm_cmbiso2p0_nh10p0_scaled_eff_vs_pt = EfficiencyPlot1D(dir, "rec_dm_cmbiso2p0_nh10p0_scaled_eff_vs_pt", pt_bins, pt_min, pt_max);

  
  // Efficiency of (charged iso) given a reco tau_h matched to gen-level and passing (DMF & acceptance)
  rec_chiso0p8_gvn_dm_eff_vs_pt = EfficiencyPlot1D(dir, "rec_chiso0p8_gvn_dm_eff_vs_pt", pt_bins, pt_min, pt_max);
  rec_chiso1p0_gvn_dm_eff_vs_pt = EfficiencyPlot1D(dir, "rec_chiso1p0_gvn_dm_eff_vs_pt", pt_bins, pt_min, pt_max);
  rec_chiso2p0_gvn_dm_eff_vs_pt = EfficiencyPlot1D(dir, "rec_chiso2p0_gvn_dm_eff_vs_pt", pt_bins, pt_min, pt_max);

  // Efficiency of (combined iso) given a reco tau_h matched to gen-level and passing (DMF & acceptance)
  rec_cmbiso0p8_gvn_dm_eff_vs_pt = EfficiencyPlot1D(dir, "rec_cmbiso0p8_gvn_dm_eff_vs_pt", pt_bins, pt_min, pt_max);
  rec_cmbiso1p0_gvn_dm_eff_vs_pt = EfficiencyPlot1D(dir, "rec_cmbiso1p0_gvn_dm_eff_vs_pt", pt_bins, pt_min, pt_max);
  rec_cmbiso2p0_gvn_dm_eff_vs_pt = EfficiencyPlot1D(dir, "rec_cmbiso2p0_gvn_dm_eff_vs_pt", pt_bins, pt_min, pt_max);



  // Charged iso sum of reco tau_h matched to gen-level and passing (DMF & acceptance)
  ch_iso = dir.make<TH1F>("ch_iso", "", 25, 0, 5);
  // Combined iso sum of reco_tau_h matched to gen-level and passing (DMF & acceptance)
  cmb_iso = dir.make<TH1F>("cmb_iso", "", 25, 0, 5);

  // Combined iso sum of reco tau_h if (matched & DMF & acceptance & cut on neutral hadron iso)
  cmb_iso_gvn_nh_iso_2p0          = dir.make<TH1F>("cmb_iso_gvn_nh_iso_2p0", "", 25, 0, 5);
  cmb_iso_gvn_nh_iso_5p0          = dir.make<TH1F>("cmb_iso_gvn_nh_iso_5p0", "", 25, 0, 5);
  cmb_iso_gvn_nh_iso_10p0         = dir.make<TH1F>("cmb_iso_gvn_nh_iso_10p0", "", 25, 0, 5);
  cmb_iso_gvn_nh_iso_10p0_scaled  = dir.make<TH1F>("cmb_iso_gvn_nh_iso_10p0_scaled", "", 25, 0, 5);

  nh_iso_profile = dir.make<TH1F>("nh_iso_profile", "", 50, 0, 1.);
  nh_iso_profile_den = dir.make<TH1F>("nh_iso_profile_den", "", 1, 0, 1);

  pt_resp = dir.make<TH1F>("pt_resp", "", 50, 0., 2.);

  gen_pt = dir.make<TH1F>("gen_pt", "", pt_bins, pt_min, pt_max);
  gen_eta = dir.make<TH1F>("gen_eta", "", 25, -2.5, 2.5);
  rec_pt = dir.make<TH1F>("rec_pt", "", pt_bins, pt_min, pt_max);
  rec_eta = dir.make<TH1F>("rec_eta", "", 25, -2.5, 2.5);
}

void ToyTauStudy::Fill(MainPlots & plots, Candidate const* gen_th_vis, Tau const* rec_th) {
  bool pass_rec        = bool(rec_th);
  bool pass_rec_dm     = pass_rec && rec_th->GetTauID("decayModeFindingOldDMs") > 0.5;
  bool pass_rec_dm_acc = pass_rec_dm && MinPtMaxEta(rec_th, th_pt_acc_, th_eta_acc_);

  float ch_iso = pass_rec ? rec_th->GetTauID("chargedIsoPtSum") : 0.;
  // float ph_iso = pass_rec ? rec_th->GetTauID("neutralIsoPtSum") : 0.;
  // float pu_iso = pass_rec ? rec_th->GetTauID("puCorrPtSum") : 0.;
  float nh_iso = pass_rec ? rec_th->GetTauID("neutralHadronIsoPtSum") : 0.;
  float cmb_iso = pass_rec ? rec_th->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0.;
  // float recalc_cm_iso = ch_iso + std::max(ph_iso - 0.46 * pu_iso, 0.);

  double gen_pt = gen_th_vis->pt();

  float overflow = std::numeric_limits<float>::max();
  plots.ch_iso->Fill(pass_rec_dm_acc ? ch_iso : overflow);
  plots.cmb_iso->Fill(pass_rec_dm_acc ? cmb_iso : overflow);

  plots.cmb_iso_gvn_nh_iso_2p0->Fill((pass_rec_dm_acc && nh_iso < 2.) ? cmb_iso : overflow);
  plots.cmb_iso_gvn_nh_iso_5p0->Fill((pass_rec_dm_acc && nh_iso < 5.) ? cmb_iso : overflow);
  plots.cmb_iso_gvn_nh_iso_10p0->Fill((pass_rec_dm_acc && nh_iso < 10.) ? cmb_iso : overflow);
  plots.cmb_iso_gvn_nh_iso_10p0_scaled->Fill(
      (pass_rec_dm_acc && nh_iso < std::max(10., 0.1 * rec_th->pt()))
          ? cmb_iso
          : overflow);

  plots.rec_eff_vs_pt.Fill(gen_pt, pass_rec);
  plots.rec_dm_eff_vs_pt.Fill(gen_pt, pass_rec_dm_acc);
  plots.rec_dm_chiso0p8_eff_vs_pt.Fill(gen_pt, pass_rec_dm_acc && ch_iso < 0.8);
  plots.rec_dm_chiso1p0_eff_vs_pt.Fill(gen_pt, pass_rec_dm_acc && ch_iso < 1.0);
  plots.rec_dm_chiso2p0_eff_vs_pt.Fill(gen_pt, pass_rec_dm_acc && ch_iso < 2.0);
  plots.rec_dm_cmbiso0p8_eff_vs_pt.Fill(gen_pt, pass_rec_dm_acc && cmb_iso < 0.8);
  plots.rec_dm_cmbiso1p0_eff_vs_pt.Fill(gen_pt, pass_rec_dm_acc && cmb_iso < 1.0);
  plots.rec_dm_cmbiso2p0_eff_vs_pt.Fill(gen_pt, pass_rec_dm_acc && cmb_iso < 2.0);

  plots.rec_dm_cmbiso2p0_nh2p0_eff_vs_pt.Fill(gen_pt, pass_rec_dm_acc && cmb_iso < 2.0 && nh_iso < 2.0);
  plots.rec_dm_cmbiso2p0_nh5p0_eff_vs_pt.Fill(gen_pt, pass_rec_dm_acc && cmb_iso < 2.0 && nh_iso < 5.0);
  plots.rec_dm_cmbiso2p0_nh10p0_eff_vs_pt.Fill(gen_pt, pass_rec_dm_acc && cmb_iso < 2.0 && nh_iso < 10.0);
  plots.rec_dm_cmbiso2p0_nh10p0_scaled_eff_vs_pt.Fill(gen_pt, pass_rec_dm_acc && cmb_iso < 2.0 && nh_iso < std::max(10., 0.1 * rec_th->pt()));

  if (pass_rec_dm_acc) {
    plots.rec_chiso0p8_gvn_dm_eff_vs_pt.Fill(gen_pt, ch_iso < 0.8);
    plots.rec_chiso1p0_gvn_dm_eff_vs_pt.Fill(gen_pt, ch_iso < 1.0);
    plots.rec_chiso2p0_gvn_dm_eff_vs_pt.Fill(gen_pt, ch_iso < 2.0);
    plots.rec_cmbiso0p8_gvn_dm_eff_vs_pt.Fill(gen_pt, cmb_iso < 0.8);
    plots.rec_cmbiso1p0_gvn_dm_eff_vs_pt.Fill(gen_pt, cmb_iso < 1.0);
    plots.rec_cmbiso2p0_gvn_dm_eff_vs_pt.Fill(gen_pt, cmb_iso < 2.0);
  }

  if (pass_rec_dm_acc && cmb_iso < 2.0) plots.pt_resp->Fill(rec_th->pt() / gen_pt);

  plots.gen_pt->Fill(gen_th_vis->pt());
  plots.gen_eta->Fill(gen_th_vis->eta());
  if (pass_rec) {
    plots.rec_pt->Fill(rec_th->pt());
    plots.rec_eta->Fill(rec_th->eta());
  }
}

void ToyTauStudy::FillIso(MainPlots & plots, Tau const* rec_th) {
  if (!rec_th) return;
  for (auto p : iso_deps_->at(rec_th)) {
    plots.nh_iso_profile->Fill(DR(rec_th, p), p->pt());
  }
  plots.nh_iso_profile_den->Fill(0.5);
}

int ToyTauStudy::PreAnalysis() {
  TH1::SetDefaultSumw2(true);
  using ROOT::Math::Pi;
  if (!fs_) return 0;
  dir_ = new TFileDirectory(fs_->mkdir("ToyTauStudy_"+taus_label_));
  if (do_real_th_studies_) {
    mp_all_dm = MainPlots(dir_->mkdir("all_dm"));
    mp_dm0 = MainPlots(dir_->mkdir("dm0"));
    mp_dm1 = MainPlots(dir_->mkdir("dm1"));
    mp_dm10 = MainPlots(dir_->mkdir("dm10"));
  } else if (do_fake_th_studies_) {
    mp_all_dm = MainPlots(dir_->mkdir("all_dm_fake"));
  }
  return 0;
}

void ToyTauStudy::DoRealThStudies(TreeEvent *event) {
  using std::vector;
  using std::map;
  // auto const& pu_info = event->GetPtrVec<PileupInfo>("pileupInfo");
  // unsigned n_it = 0;
  // unsigned n_ot = 0;
  // for (auto info : pu_info) {
  //   if (info->bunch_crossing() == 0) n_it += info->num_interactions();
  //   if (abs(info->bunch_crossing()) == 1) n_ot += info->num_interactions();
  // }

  // auto const* event_info = event->GetPtr<EventInfo>("eventInfo");
  auto const& gevt = event->Get<GenEvent_XToTauTau>("genEvent_XToTauTau");
  // A vector of the GenEvent infos for any hadronic tau decays
  vector<GenEvent_Tau const*> gen_th_vec;
  // A vector of the visible hadronic tau jets (only really needed as input to
  // the matching algorithm)
  vector<GenJet const*> gen_th_vis_vec;
  if (gevt.tau_0.hadronic_mode >= 0) gen_th_vec.push_back(&(gevt.tau_0));
  if (gevt.tau_1.hadronic_mode >= 0) gen_th_vec.push_back(&(gevt.tau_1));
  for (auto th : gen_th_vec) gen_th_vis_vec.push_back(th->vis_jet);

  // Skip events where the taus overlap
  if (DR(gevt.tau_0.vis_jet, gevt.tau_1.vis_jet) <= 0.5) return;

  auto reco_th_vec = event->GetPtrVec<Tau>(taus_label_);

  // Do the matching on the unbiased gen. and reco. taus
  auto gen_rec_th_matches =
      MatchByDR(gen_th_vis_vec, reco_th_vec, 0.3, true, true);
  // We have a vector of pairs from MatchByDR, but a more useful
  // format will be a map going from the gen. to the reco. tau
  map<GenJet const*, Tau const*> gen_rec_th_map;
  for (auto const& x : gen_rec_th_matches) {
    gen_rec_th_map[x.first] = x.second;
  }

  // Now loop through the hadronic taus
  for (auto gen_th : gen_th_vec) {
    GenJet const* gen_th_vis = gen_th->vis_jet;
    Tau const* rec_th = nullptr;
    if (gen_rec_th_map.count(gen_th_vis)) {
      rec_th = gen_rec_th_map[gen_th_vis];
    }
    if (MinPtMaxEta(gen_th_vis, th_pt_acc_, th_eta_acc_)) {
      Fill(mp_all_dm, gen_th_vis, rec_th);
      FillIso(mp_all_dm, rec_th);
      if (gen_th->hadronic_mode == 0) {
        Fill(mp_dm0, gen_th_vis, rec_th);
      }
      if (gen_th->hadronic_mode >= 1 && gen_th->hadronic_mode <= 4) {
        Fill(mp_dm1, gen_th_vis, rec_th);
      }
      if (gen_th->hadronic_mode == 10) {
        Fill(mp_dm10, gen_th_vis, rec_th);
      }
    }
  }
}

void ToyTauStudy::DoFakeThStudies(TreeEvent *event) {
  using std::vector;
  using std::map;
  // auto const& pu_info = event->GetPtrVec<PileupInfo>("pileupInfo");
  // unsigned n_it = 0;
  // unsigned n_ot = 0;
  // for (auto info : pu_info) {
  //   if (info->bunch_crossing() == 0) n_it += info->num_interactions();
  //   if (abs(info->bunch_crossing()) == 1) n_ot += info->num_interactions();
  // }

  
  auto rec_jets = event->GetPtrVec<PFJet>("pfJetsPFlow");

  auto reco_th_vec = event->GetPtrVec<Tau>(taus_label_);

  auto jet_th_matches = MatchByDR(rec_jets, reco_th_vec, 0.5, true, true);

  map<PFJet const*, Tau const*> jet_th_map;
  for (auto const& x : jet_th_matches) {
    jet_th_map[x.first] = x.second;
  }

  for (auto jet : rec_jets) {
    Tau const* rec_th = nullptr;
    if (jet_th_map.count(jet)) {
      rec_th = jet_th_map[jet];
    }
    if (MinPtMaxEta(jet, th_pt_acc_, th_eta_acc_)) {
      Fill(mp_all_dm, jet, rec_th);
      FillIso(mp_all_dm, rec_th);
    }
  }
}

int ToyTauStudy::Execute(TreeEvent* event) {
  // Build neutral hadron iso sum first
  auto pfcands = event->GetPtrVec<PFCandidate>(pfcands_label_);
  ic::erase_if(pfcands, [](ic::PFCandidate *p) {
    return p->type() != PFType::h0;
  });
  auto taus = event->GetPtrVec<Tau>(taus_label_);
  auto pairs = MatchByDR(taus, pfcands, 1.0, false, false);
  std::map<Tau *, double> iso_sums;
  std::map<Tau const*, std::vector<ic::PFCandidate *>> iso_deps;
  for (auto t : taus) iso_sums[t] = 0.;
  for (auto t : taus) iso_deps[t] = std::vector<ic::PFCandidate *>();
  for (auto p : pairs) {
    if (DR(p.first, p.second) < 0.5) iso_sums[p.first] += p.second->pt();
    iso_deps[p.first].push_back(p.second);
  }
  iso_deps_ = &iso_deps;
  for (auto t : iso_sums) {
    t.first->SetTauID("neutralHadronIsoPtSum", t.second);
  }
  if (do_real_th_studies_) DoRealThStudies(event);
  if (do_fake_th_studies_) DoFakeThStudies(event);
  return 0;
}


int ToyTauStudy::PostAnalysis() { return 0; }

void ToyTauStudy::PrintInfo() { ; }
}
