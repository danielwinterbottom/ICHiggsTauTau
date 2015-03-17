#include "HiggsTauTau/interface/Phys14Plots.h"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include "UserCode/ICHiggsTauTau/interface/Track.hh"

namespace ic {

Phys14Plots::Phys14Plots(std::string const& name) : ModuleBase(name) {
  th_pt_acc   = 20.;
  th_eta_acc  = 2.3;
  do_real_th_studies_ = true;
  do_fake_th_studies_ = false;
}

Phys14Plots::~Phys14Plots() { ; }

int Phys14Plots::PreAnalysis() {
  TH1::SetDefaultSumw2(true);
  using ROOT::Math::Pi;
  if (!fs_) return 0;
  dir_ = new TFileDirectory(fs_->mkdir("phys14"));
  if (do_real_th_studies_) {
    h_n_vtx = dir_->make<TH1F>("n_vtx", "",71, -0.5, 70.5);
    h_n_it_pu = dir_->make<TH1F>("n_it_pu", "",50, -0.5, 99.5);
    h_n_ot_pu = dir_->make<TH1F>("n_ot_pu", "",75, -0.5, 149.5);

    h_gen_h_pt = dir_->make<TH1F>("gen_h_pt", "",     250, 0, 500);
    h_gen_h_eta = dir_->make<TH1F>("gen_h_eta", "",   50, -5, 5);
    h_gen_h_phi = dir_->make<TH1F>("gen_h_phi", "",   50, -Pi(), Pi());
    h_gen_h_mass = dir_->make<TH1F>("gen_h_mass", "", 100, 0, 200);
    // = dir_->make<TH1F>("", "", , , );
    h_gen_th_pt = dir_->make<TH1F>("gen_th_pt", "", 100, 0, 200);
    h_gen_th_eta = dir_->make<TH1F>("gen_th_eta", "", 50, -5, 5);
    h_gen_th_mode = dir_->make<TH1F>("gen_th_mode", "", 20, -0.5, 19.5);

    th_mt_eff_vs_pt = EfficiencyPlot1D(dir_, "th_mt_eff_vs_pt", 100, 0, 200);
    th_rf_mt_eff_vs_pt = EfficiencyPlot1D(dir_, "th_rf_mt_eff_vs_pt", 100, 0, 200);
    th_dm_rf_eff_vs_pt = EfficiencyPlot1D(dir_, "th_dm_rf_eff_vs_pt", 100, 0, 200);

    th_dm_eff_vs_pt = EfficiencyPlot1D(dir_, "th_dm_eff_vs_pt", 100, 0, 200);
    th_dm_eff_vs_eta = EfficiencyPlot1D(dir_, "th_dm_eff_vs_eta", 50, -5, 5);
    th_dm_eff_vs_nvtx = EfficiencyPlot1D(dir_, "th_dm_eff_vs_nvtx", 51, -0.5, 50.5);
    th_dm_eff_vs_it_pu = EfficiencyPlot1D(dir_, "th_dm_eff_vs_it_pu", 50, -0.5, 99.5);
    th_dm_eff_vs_ot_pu = EfficiencyPlot1D(dir_, "th_dm_eff_vs_ot_pu", 75, -0.5, 149.5);

    th0_mt_eff_vs_pt = EfficiencyPlot1D(dir_, "th0_mt_eff_vs_pt", 100, 0, 200);
    th0_rf_mt_eff_vs_pt = EfficiencyPlot1D(dir_, "th0_rf_mt_eff_vs_pt", 100, 0, 200);
    th0_dm_rf_eff_vs_pt = EfficiencyPlot1D(dir_, "th0_dm_rf_eff_vs_pt", 100, 0, 200);

    th0_dm_eff_vs_pt = EfficiencyPlot1D(dir_, "th0_dm_eff_vs_pt", 100, 0, 200);
    th0_dm_eff_vs_eta = EfficiencyPlot1D(dir_, "th0_dm_eff_vs_eta", 50, -5, 5);
    th0_dm_eff_vs_nvtx = EfficiencyPlot1D(dir_, "th0_dm_eff_vs_nvtx", 51, -0.5, 50.5);
    th0_dm_eff_vs_it_pu = EfficiencyPlot1D(dir_, "th0_dm_eff_vs_it_pu", 50, -0.5, 99.5);
    th0_dm_eff_vs_ot_pu = EfficiencyPlot1D(dir_, "th0_dm_eff_vs_ot_pu", 75, -0.5, 149.5);

    th1_mt_eff_vs_pt = EfficiencyPlot1D(dir_, "th1_mt_eff_vs_pt", 100, 0, 200);
    th1_rf_mt_eff_vs_pt = EfficiencyPlot1D(dir_, "th1_rf_mt_eff_vs_pt", 100, 0, 200);
    th1_dm_rf_eff_vs_pt = EfficiencyPlot1D(dir_, "th1_dm_rf_eff_vs_pt", 100, 0, 200);

    // Special plot of th1 matching efficiency vs pt of the charged pion instead
    // of the entire visible part
    th1_mt_eff_vs_pt_pi = EfficiencyPlot1D(dir_, "th1_mt_eff_vs_pt_pi", 100, 0, 200);
    th1_jet_eff_vs_pt = EfficiencyPlot1D(dir_, "th1_jet_eff_vs_pt", 100, 0, 200);
    th1_mt_eff_after_jet_vs_pt = EfficiencyPlot1D(dir_, "th1_mt_eff_after_jet_vs_pt", 100, 0, 200);
    th1_mt_eff_after_jet_pi15_vs_pt = EfficiencyPlot1D(dir_, "th1_mt_eff_after_jet_pi15_vs_pt", 100, 0, 200);
    th1_mt_eff_after_jet_pi20_vs_pt = EfficiencyPlot1D(dir_, "th1_mt_eff_after_jet_pi20_vs_pt", 100, 0, 200);

    th1_jet_ch_had_frac = dir_->make<TH1F>("th1_jet_ch_had_frac", "", 21, 0, 1.05);
    th1_jet_nt_had_frac = dir_->make<TH1F>("th1_jet_nt_had_frac", "", 21, 0, 1.05);
    th1_jet_photon_frac = dir_->make<TH1F>("th1_jet_photon_frac", "", 21, 0, 1.05);
    th1_jet_elec_frac = dir_->make<TH1F>("th1_jet_elec_frac", "", 21, 0, 1.05);
    th1_jet_muon_frac = dir_->make<TH1F>("th1_jet_muon_frac", "", 21, 0, 1.05);
    th1_jet_tot_frac = dir_->make<TH1F>("th1_jet_tot_frac", "", 21, 0, 1.05);

    th1_dm_eff_vs_pt = EfficiencyPlot1D(dir_, "th1_dm_eff_vs_pt", 100, 0, 200);
    th1_dm_eff_vs_eta = EfficiencyPlot1D(dir_, "th1_dm_eff_vs_eta", 50, -5, 5);
    th1_dm_eff_vs_nvtx = EfficiencyPlot1D(dir_, "th1_dm_eff_vs_nvtx", 51, -0.5, 50.5);
    th1_dm_eff_vs_it_pu = EfficiencyPlot1D(dir_, "th1_dm_eff_vs_it_pu", 50, -0.5, 99.5);
    th1_dm_eff_vs_ot_pu = EfficiencyPlot1D(dir_, "th1_dm_eff_vs_ot_pu", 75, -0.5, 149.5);

    th10_mt_eff_vs_pt = EfficiencyPlot1D(dir_, "th10_mt_eff_vs_pt", 100, 0, 200);
    th10_rf_mt_eff_vs_pt = EfficiencyPlot1D(dir_, "th10_rf_mt_eff_vs_pt", 100, 0, 200);
    th10_dm_rf_eff_vs_pt = EfficiencyPlot1D(dir_, "th10_dm_rf_eff_vs_pt", 100, 0, 200);

    th10_dm_eff_vs_pt = EfficiencyPlot1D(dir_, "th10_dm_eff_vs_pt", 100, 0, 200);
    th10_dm_eff_vs_eta = EfficiencyPlot1D(dir_, "th10_dm_eff_vs_eta", 50, -5, 5);
    th10_dm_eff_vs_nvtx = EfficiencyPlot1D(dir_, "th10_dm_eff_vs_nvtx", 51, -0.5, 50.5);
    th10_dm_eff_vs_it_pu = EfficiencyPlot1D(dir_, "th10_dm_eff_vs_it_pu", 50, -0.5, 99.5);
    th10_dm_eff_vs_ot_pu = EfficiencyPlot1D(dir_, "th10_dm_eff_vs_ot_pu", 75, -0.5, 149.5);

    h_th_mode_table =
        dir_->make<TH2F>("th_mode_table", "", 17, -1.5, 15.5, 17, -1.5, 15.5);

    h_th_mode_table_gen_den =
        dir_->make<TH2F>("th_mode_table_gen_den", "", 17, -1.5, 15.5, 17, -1.5, 15.5);

    h_th_mode_table_gen_den_rec_fid =
        dir_->make<TH2F>("th_mode_table_gen_den_rec_fid", "", 17, -1.5, 15.5, 17, -1.5, 15.5);



    h_th_pt_resp = dir_->make<TH1F>("th_pt_resp", "", 50, -2, 2);
    h_th0_pt_resp = dir_->make<TH1F>("th0_pt_resp", "", 50, -2, 2);
    h_th1_pt_resp = dir_->make<TH1F>("th1_pt_resp", "", 50, -2, 2);
    h_th10_pt_resp = dir_->make<TH1F>("th10_pt_resp", "", 50, -2, 2);

    th_pf_match_pt = PFMatchPlot(dir_, "th_pf_match_pt", 24, 6, 102);
    th_pf_match_eta = PFMatchPlot(dir_, "th_pf_match_eta", 30, -2.5, 2.5);
    th0_pf_match_pt = PFMatchPlot(dir_, "th0_pf_match_pt", 24, 6, 102);
    th0_pf_match_eta = PFMatchPlot(dir_, "th0_pf_match_eta", 30, -2.5, 2.5);
    th1_pf_match_pt = PFMatchPlot(dir_, "th1_pf_match_pt", 24, 6, 102);
    th1_pf_match_eta = PFMatchPlot(dir_, "th1_pf_match_eta", 30, -2.5, 2.5);
    th10_pf_match_pt = PFMatchPlot(dir_, "th10_pf_match_pt", 24, 6, 102);
    th10_pf_match_eta = PFMatchPlot(dir_, "th10_pf_match_eta", 30, -2.5, 2.5);

    h_trk_pt_frac_ch = dir_->make<TH1F>("trk_pt_frac_ch", "", 100, 0, 10);
    h_trk_pt_frac_em = dir_->make<TH1F>("trk_pt_frac_em", "", 100, 0, 10);
    h_th_pt_frac_ch = dir_->make<TH1F>("th_pt_frac_ch", "", 20, 0, 2);
    h_th_pt_frac_em = dir_->make<TH1F>("th_pt_frac_em", "", 20, 0, 2);

    trk_plots_matched = TrackPlots(dir_, "trk_plots_matched");
    trk_plots_ph_matched = TrackPlots(dir_, "trk_plots_ph_matched");
    trk_plots_unmatched = TrackPlots(dir_, "trk_plots_unmatched");
  }
  if (do_fake_th_studies_) {
    jet_th_fake_dm_vs_pt = EfficiencyPlot1D(dir_, "jet_th_fake_dm_vs_pt", 100, 0, 1000);
    jet_th_fake_dm_vs_eta = EfficiencyPlot1D(dir_, "jet_th_fake_dm_vs_eta", 50, -5, 5);
  }
  return 0;
}

void Phys14Plots::DoRealThStudies(TreeEvent *event) {
  auto const* event_info = event->GetPtr<EventInfo>("eventInfo");
  unsigned n_vtx = event_info->good_vertices();
  h_n_vtx->Fill(n_vtx);

  auto const& pu_info = event->GetPtrVec<PileupInfo>("pileupInfo");
  unsigned n_it = 0;
  unsigned n_ot = 0;
  for (auto info : pu_info) {
    if (info->bunch_crossing() == 0) n_it += info->num_interactions();
    if (abs(info->bunch_crossing()) == 1) n_ot += info->num_interactions();
  }
  h_n_it_pu->Fill(n_it);
  h_n_ot_pu->Fill(n_ot);

  auto const& gevt = event->Get<GenEvent_XToTauTau>("genEvent_XToTauTau");
  if (gevt.boson) {
    h_gen_h_pt->Fill(gevt.boson->pt());
    h_gen_h_eta->Fill(gevt.boson->eta());
    h_gen_h_phi->Fill(gevt.boson->phi());
    h_gen_h_mass->Fill(gevt.boson->M());
  }

  // A vector of the GenEvent infos for any hadronic tau decays
  std::vector<GenEvent_Tau const*> gen_th_vec;
  // A vector of the visible hadronic tau jets (only really needed as input to
  // the matching algorithm)
  std::vector<GenJet const*> gen_th_vis_vec;
  if (gevt.tau_0.hadronic_mode >= 0) gen_th_vec.push_back(&(gevt.tau_0));
  if (gevt.tau_1.hadronic_mode >= 0) gen_th_vec.push_back(&(gevt.tau_1));
  for (auto th : gen_th_vec) gen_th_vis_vec.push_back(th->vis_jet);

  // Get the reco tau collection
  auto reco_th_vec = event->GetPtrVec<Tau>("taus");

  // Get the reco jet collection
  auto reco_jet_vec = event->GetPtrVec<PFJet>("pfJetsPFlow");

  // Do the matching on the unbiased gen. and reco. taus
  auto gen_rec_th_matches =
      MatchByDR(gen_th_vis_vec, reco_th_vec, 0.3, true, true);

  // Do the matching on the unbiased gen. and reco. jets
  auto gen_rec_jet_matches =
      MatchByDR(gen_th_vis_vec, reco_jet_vec, 0.5, true, true);


  // We have a vector of pairs from MatchByDR, but a more useful
  // format will be a map going from the gen. to the reco. tau
  std::map<GenJet const*, Tau const*> gen_rec_th_map;
  for (auto const& x : gen_rec_th_matches) {
    gen_rec_th_map[x.first] = x.second;
  }
  // Make a similar map for the gen tau -> reco jet
  std::map<GenJet const*, PFJet const*> gen_rec_jet_map;
  for (auto const& x : gen_rec_jet_matches) {
    gen_rec_jet_map[x.first] = x.second;
  }

  // Now loop through the hadronic taus
  for (auto gen_th : gen_th_vec) {
    // Get the visible jet
    GenJet const* gen_th_vis = gen_th->vis_jet;
    h_gen_th_pt->Fill(gen_th_vis->pt());
    h_gen_th_eta->Fill(gen_th_vis->eta());
    h_gen_th_mode->Fill(gen_th->hadronic_mode);

    // Use decay mode -1 to mean not reconstructed
    int reco_mode = -1;
    Tau const* rec_th = nullptr;
    PFJet const* rec_jet = nullptr;
    if (gen_rec_th_map.count(gen_th_vis)) {
      rec_th = gen_rec_th_map[gen_th_vis];
      if (rec_th->decay_mode() >= 0) reco_mode = rec_th->decay_mode();
    }
    if (gen_rec_jet_map.count(gen_th_vis)) {
      rec_jet = gen_rec_jet_map[gen_th_vis];
    }

    // The mode table definitely seems to require the reco acceptance cuts be
    // applied. Not clear if they should also be applied at the gen level. For
    // now let's just apply it at the reco level
    if (MinPtMaxEta(gen_th_vis, th_pt_acc, th_eta_acc) && rec_th &&
        MinPtMaxEta(rec_th, th_pt_acc, th_eta_acc)) {
      h_th_mode_table->Fill(gen_th->hadronic_mode, reco_mode);
    }

    if (MinPtMaxEta(gen_th_vis, th_pt_acc, th_eta_acc)) {
      int local_reco_mode = reco_mode;
      h_th_mode_table_gen_den->Fill(gen_th->hadronic_mode, local_reco_mode);
    }

    if (MinPtMaxEta(gen_th_vis, th_pt_acc, th_eta_acc)) {
      int local_reco_mode = reco_mode;
      if (rec_th && !MinPtMaxEta(rec_th, th_pt_acc, th_eta_acc)) local_reco_mode = -1;
      h_th_mode_table_gen_den_rec_fid->Fill(gen_th->hadronic_mode, local_reco_mode);
    }


    // Now we'll do the efficiencies. Here the definition is clearer: gen
    // acceptance is in the denominator
    if (MinPtMaxEta(gen_th_vis, th_pt_acc, th_eta_acc)) {
      // We're in the denominator
      bool pass_dm = false;
      // Full matching + acceptance + DM flag
      if (rec_th && rec_th->decay_mode() >= 0. &&
          MinPtMaxEta(rec_th, th_pt_acc, th_eta_acc))
        pass_dm = true;
      // matching + acceptance flag
      bool pass_rf = rec_th && MinPtMaxEta(rec_th, th_pt_acc, th_eta_acc);

      th_mt_eff_vs_pt.Fill(gen_th_vis->pt(), bool(rec_th));
      if (rec_th) th_rf_mt_eff_vs_pt.Fill(gen_th_vis->pt(), pass_rf);
      if (pass_rf) th_dm_rf_eff_vs_pt.Fill(gen_th_vis->pt(), pass_dm);

      th_dm_eff_vs_pt.Fill(gen_th_vis->pt(), pass_dm);
      th_dm_eff_vs_eta.Fill(gen_th_vis->eta(), pass_dm);
      th_dm_eff_vs_nvtx.Fill(n_vtx, pass_dm);
      th_dm_eff_vs_it_pu.Fill(n_it, pass_dm);
      th_dm_eff_vs_ot_pu.Fill(n_ot, pass_dm);
      float pt_res = 0;
      if (pass_dm) {
        pt_res = (rec_th->pt() - gen_th_vis->pt()) / gen_th_vis->pt();
        h_th_pt_resp->Fill(pt_res);
      }


      if (gen_th->hadronic_mode == 0) {
        th0_mt_eff_vs_pt.Fill(gen_th_vis->pt(), bool(rec_th));
        if (rec_th) th0_rf_mt_eff_vs_pt.Fill(gen_th_vis->pt(), pass_rf);
        if (pass_rf) th0_dm_rf_eff_vs_pt.Fill(gen_th_vis->pt(), pass_dm);

        th0_dm_eff_vs_pt.Fill(gen_th_vis->pt(), pass_dm);
        th0_dm_eff_vs_eta.Fill(gen_th_vis->eta(), pass_dm);
        th0_dm_eff_vs_nvtx.Fill(n_vtx, pass_dm);
        th0_dm_eff_vs_it_pu.Fill(n_it, pass_dm);
        th0_dm_eff_vs_ot_pu.Fill(n_ot, pass_dm);
        if (pass_dm) {
          h_th0_pt_resp->Fill(pt_res);
        }
      }
      if (gen_th->hadronic_mode >= 1 && gen_th->hadronic_mode <= 4) {
        th1_mt_eff_vs_pt.Fill(gen_th_vis->pt(), bool(rec_th));
        if (rec_th) th1_rf_mt_eff_vs_pt.Fill(gen_th_vis->pt(), pass_rf);
        if (pass_rf) th1_dm_rf_eff_vs_pt.Fill(gen_th_vis->pt(), pass_dm);

        th1_dm_eff_vs_pt.Fill(gen_th_vis->pt(), pass_dm);
        th1_dm_eff_vs_eta.Fill(gen_th_vis->eta(), pass_dm);
        th1_dm_eff_vs_nvtx.Fill(n_vtx, pass_dm);
        th1_dm_eff_vs_it_pu.Fill(n_it, pass_dm);
        th1_dm_eff_vs_ot_pu.Fill(n_ot, pass_dm);
        if (pass_dm) {
          h_th1_pt_resp->Fill(pt_res);
        }
        // Filling some special plots
        th1_mt_eff_vs_pt_pi.Fill(gen_th->pi_charged.at(0)->pt(), bool(rec_th));
        if (rec_jet) {
          th1_jet_ch_had_frac->Fill(rec_jet->charged_had_energy_frac());
          th1_jet_nt_had_frac->Fill(rec_jet->neutral_had_energy_frac());
          th1_jet_photon_frac->Fill(rec_jet->photon_energy_frac());
          th1_jet_elec_frac->Fill(rec_jet->electron_energy_frac());
          th1_jet_muon_frac->Fill(rec_jet->muon_energy_frac());
          th1_jet_tot_frac->Fill(rec_jet->charged_had_energy_frac() +
                                 rec_jet->neutral_had_energy_frac() +
                                 rec_jet->photon_energy_frac() +
                                 rec_jet->electron_energy_frac() +
                                 rec_jet->muon_energy_frac());
        }
        double rec_jet_uncorr_pt = rec_jet
                                       ? rec_jet->pt() *
                                             rec_jet->uncorrected_energy() /
                                             rec_jet->energy()
                                       : 0.;
        bool rec_jet_acc = rec_jet && MinPtMaxEta(rec_jet, 0., 2.5) &&
            rec_jet_uncorr_pt > 14.5;
        th1_jet_eff_vs_pt.Fill(gen_th_vis->pt(), bool(rec_jet_acc));
        if (rec_jet_acc) {
          th1_mt_eff_after_jet_vs_pt.Fill(gen_th_vis->pt(), bool(rec_th));
          if (gen_th->pi_charged.at(0)->pt() > 15.) {
            th1_mt_eff_after_jet_pi15_vs_pt.Fill(gen_th_vis->pt(),
                                                 bool(rec_th));
          }
          if (gen_th->pi_charged.at(0)->pt() > 20.) {
            th1_mt_eff_after_jet_pi20_vs_pt.Fill(gen_th_vis->pt(),
                                                 bool(rec_th));
          }
        }
      }
      if (gen_th->hadronic_mode == 10) {
        th10_mt_eff_vs_pt.Fill(gen_th_vis->pt(), bool(rec_th));
        if (rec_th) th10_rf_mt_eff_vs_pt.Fill(gen_th_vis->pt(), pass_rf);
        if (pass_rf) th10_dm_rf_eff_vs_pt.Fill(gen_th_vis->pt(), pass_dm);

        th10_dm_eff_vs_pt.Fill(gen_th_vis->pt(), pass_dm);
        th10_dm_eff_vs_eta.Fill(gen_th_vis->eta(), pass_dm);
        th10_dm_eff_vs_nvtx.Fill(n_vtx, pass_dm);
        th10_dm_eff_vs_it_pu.Fill(n_it, pass_dm);
        th10_dm_eff_vs_ot_pu.Fill(n_ot, pass_dm);
        if (pass_dm) {
          h_th10_pt_resp->Fill(pt_res);
        }
      }
    }
  }

  auto tracks = event->GetPtrVec<Track>("tracks");
  auto vertices = event->GetPtrVec<Vertex>("vertices");
  auto pfcands = event->GetPtrVec<PFCandidate>("pfCandidates");
  std::map<std::size_t, PFCandidate const*> trk_pf_map;
  for (PFCandidate const* pf : pfcands) {
    if (pf->constituent_tracks().size() > 0)
      trk_pf_map[pf->constituent_tracks()[0]] = pf;
  }
  if (vertices.size() >= 1) {
    ic::erase_if(tracks, [&](ic::Track const* trk) {
      return !(QualityTrack(trk, vertices[0]) && trk->pt() > 6.);
    });
    for (auto gen_th : gen_th_vec) {
      // Make a copy of the gen pions ptr vec
      auto gen_pions = gen_th->pi_charged;
      // Match filtered tracks to charged pions
      auto trk_pion_matches = MatchByDR(tracks, gen_pions, 0.01, true, true);
      auto matched_trks = ExtractFirst(trk_pion_matches);
      auto matched_pfs = MatchByDR(matched_trks, pfcands, 0.5, true, true);
      std::map<std::size_t, PFCandidate const*> trk_pf_dr_map;
      for (auto x : matched_pfs) {
        trk_pf_dr_map[x.first->id()] = x.second;
      }
      for (Track const* trk : matched_trks) {
        PFType type = trk_pf_map.count(trk->id())
                          ? trk_pf_map.at(trk->id())->type()
                          : PFType::X;
        PFCandidate const* pf =
            trk_pf_map.count(trk->id()) ? trk_pf_map.at(trk->id()) : nullptr;
        PFCandidate const* pf_dr = trk_pf_dr_map.count(trk->id())
                                       ? trk_pf_dr_map.at(trk->id())
                                       : nullptr;
        th_pf_match_pt.Fill(trk->pt(), type);
        th_pf_match_eta.Fill(trk->eta(), type);
        if (pf) {
          if (type == PFType::gamma) {
            trk_plots_ph_matched.Fill(trk, 1.);
            trk_plots_ph_matched.FillWithPF(trk, pf_dr, 1.);
          } else {
            trk_plots_matched.Fill(trk, 1.);
            trk_plots_matched.FillWithPF(trk, pf_dr, 1.);
          }
        } else {
          trk_plots_unmatched.Fill(trk, 1, event_info);
          trk_plots_unmatched.FillWithPF(trk, pf_dr, 1.);

        }
        if (gen_th->hadronic_mode == 0) {
          th0_pf_match_pt.Fill(trk->pt(), type);
          th0_pf_match_eta.Fill(trk->eta(), type);
        } else if (gen_th->hadronic_mode >= 1 && gen_th->hadronic_mode <= 4) {
          th1_pf_match_pt.Fill(trk->pt(), type);
          th1_pf_match_eta.Fill(trk->eta(), type);
          // Make some extra plots to see what's happening here
          if (type == PFType::h) {
            h_trk_pt_frac_ch->Fill(gen_pions[0]->pt() > 0.
                                       ? (pf->pt() / gen_pions[0]->pt())
                                       : -0.5);
            h_th_pt_frac_ch->Fill(gen_pions[0]->pt() > 0.
                                      ? (pf->pt() / gen_th->vis_jet->pt())
                                      : -0.5);
          }
          if (type == PFType::gamma) {
            h_trk_pt_frac_em->Fill(gen_pions[0]->pt() > 0.
                                       ? (pf->pt() / gen_pions[0]->pt())
                                       : -0.5);
            h_th_pt_frac_em->Fill(gen_pions[0]->pt() > 0.
                                      ? (pf->pt() / gen_th->vis_jet->pt())
                                      : -0.5);
          }
        } else if (gen_th->hadronic_mode == 10) {
          th10_pf_match_pt.Fill(trk->pt(), type);
          th10_pf_match_eta.Fill(trk->eta(), type);
        }
      }
    };
  }
}

void Phys14Plots::DoFakeThStudies(TreeEvent *event) {
  auto reco_th_vec = event->GetPtrVec<Tau>("taus");
  auto reco_jet_vec = event->GetPtrVec<PFJet>("pfJetsPFlow");

  // Only look at jets with pT > 20.0 and eta < 2.3
  ic::erase_if(reco_jet_vec, [](ic::PFJet *jet) {
    return !(MinPtMaxEta(jet, 20.0, 2.3));
  });

  ic::erase_if(reco_th_vec, [](ic::Tau *tau) {
    return !(
      MinPtMaxEta(tau, 20.0, 2.3) &&
      tau->GetTauID("decayModeFindingOldDMs") > 0.5 &&
      tau->GetTauID("chargedIsoPtSum") < 2.0);
      // i.e. loose WP (3.0 for VLoose, 1.0 for Medium, 0.8 for tight)
  });

  auto jet_tau_matches = MatchByDR(reco_jet_vec, reco_th_vec, 0.5, true, true);
  std::map<Jet const*, Tau const*> jet_th_map;
  for (auto x : jet_tau_matches) jet_th_map[x.first] = x.second;

  for (auto jet : reco_jet_vec) {
    jet_th_fake_dm_vs_pt.Fill(jet->pt(), jet_th_map.count(jet));
    jet_th_fake_dm_vs_eta.Fill(jet->eta(), jet_th_map.count(jet));
  }
}

int Phys14Plots::Execute(TreeEvent* event) {
  if (do_real_th_studies_) DoRealThStudies(event);
  if (do_fake_th_studies_) DoFakeThStudies(event);
  return 0;
}

bool Phys14Plots::QualityTrack(Track const* trk, Vertex const* vtx) {
  return trk->pt() > 0.5 && trk->normalized_chi2() < 100. &&
         std::abs(trk->dxy(vtx->point())) < 0.1 &&
         std::abs(trk->dz(vtx->point())) < 0.4 && trk->hits() >= 3 &&
         trk->pixel_hits() >= 0;
}


int Phys14Plots::PostAnalysis() { return 0; }

void Phys14Plots::PrintInfo() { ; }
}
