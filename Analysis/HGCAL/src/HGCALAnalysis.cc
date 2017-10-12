#include <algorithm>
#include <map>
#include <functional>
#include "TMath.h"
#include "TGraph2D.h"
#include "TPolyMarker3D.h"
#include "boost/functional/hash.hpp"
#include "boost/lexical_cast.hpp"
#include "RooRealVar.h"
#include "HGCAL/interface/HGCALAnalysis.h"
#include "HGCAL/interface/HTTGenEvent.h"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"
#include "Utilities/interface/FnRootTools.h"
#include "fastjet/ClusterSequence.hh"

namespace ic {
HGCALTest::HGCALTest(std::string const& name) : ModuleBase(name) {}

HGCALTest::~HGCALTest() { ; }

int HGCALTest::PreAnalysis() {
  t_jets_ = fs_->make<TTree>("jets", "jets");
  t_jets_->Branch("jet_pt",          &jet_pt_);
  t_jets_->Branch("jet_eta",          &jet_eta_);
  t_jets_->Branch("jet_phi",          &jet_phi_);
  t_jets_->Branch("jet_e",          &jet_e_);
  t_jets_->Branch("gen_matched",          &gen_matched_);
  t_jets_->Branch("genjet_pt",          &genjet_pt_);
  t_jets_->Branch("genjet_eta",          &genjet_eta_);
  t_jets_->Branch("genjet_phi",          &genjet_phi_);
  t_jets_->Branch("genjet_e",          &genjet_e_);
  t_jets_->Branch("gen_nonu_matched",          &gen_nonu_matched_);
  t_jets_->Branch("genjet_nonu_pt",          &genjet_nonu_pt_);
  t_jets_->Branch("genjet_nonu_eta",          &genjet_nonu_eta_);
  t_jets_->Branch("genjet_nonu_phi",          &genjet_nonu_phi_);
  t_jets_->Branch("genjet_nonu_e",          &genjet_nonu_e_);
  t_jets_->Branch("efrac_r0p02",          &efrac_r0p02_);
  t_jets_->Branch("efrac_r0p04",          &efrac_r0p04_);
  t_jets_->Branch("efrac_r0p06",          &efrac_r0p06_);
  t_jets_->Branch("efrac_r0p08",          &efrac_r0p08_);
  t_jets_->Branch("efrac_r0p10",          &efrac_r0p10_);
  t_jets_->Branch("isDM_r0p02",          &isDM_r0p02_);
  t_jets_->Branch("isDM_r0p04",          &isDM_r0p04_);
  t_jets_->Branch("isDM_r0p06",          &isDM_r0p06_);
  t_jets_->Branch("isDM_r0p08",          &isDM_r0p08_);
  t_jets_->Branch("isDM_r0p10",          &isDM_r0p10_);
  t_jets_->Branch("is_hadtau",          &is_hadtau_);
  t_jets_->Branch("tau_dm",          &tau_dm_);

  t_taus_ = fs_->make<TTree>("taus", "taus");
  t_taus_->Branch("evt",                    &tt_evt_);
  t_taus_->Branch("pm",                    &tt_pm_);
  t_taus_->Branch("dm",                    &tt_dm_);
  t_taus_->Branch("reached_ee",           &tt_reached_ee_);
  t_taus_->Branch("pion_reached_ee",       &tt_pion_reached_ee_);
  t_taus_->Branch("tau_pt",                &tt_tau_pt_);
  t_taus_->Branch("tau_eta",               &tt_tau_eta_);
  t_taus_->Branch("vis_tau_pt",            &tt_vis_tau_pt_);
  t_taus_->Branch("vis_tau_eta",           &tt_vis_tau_eta_);
  t_taus_->Branch("dm1_pho1_e",            &tt_dm1_pho1_e_);
  t_taus_->Branch("dm1_pho2_e",            &tt_dm1_pho2_e_);
  t_taus_->Branch("dm1_pion_e",            &tt_dm1_pion_e_);
  t_taus_->Branch("pho_dxy_l1",            &tt_pho_dxy_l1_);
  t_taus_->Branch("prods_dr",            &tt_prods_dr_);
  t_taus_->Branch("rec_prongs",            &tt_rec_prongs_);
  t_taus_->Branch("jet_mass",            &tt_jet_mass_);
  t_taus_->Branch("mass",            &tt_mass_);
  return 0;
}

int HGCALTest::Execute(TreeEvent* event) {
  unsigned v = 0;

  int long evt = *event->GetPtr<int long>("event");

  // Start by building object collections from flat trees
  auto const& all_rechits = BuildRecHitCollection(event);
  auto const& all_genparts = BuildGenParticleCollection(event);
  auto const& all_simparts = BuildSimParticleCollection(event);
  // auto const& simclusters = BuildSimClusterCollection(event);

  unsigned nlayers = hgcal::lastLayer - hgcal::firstLayer + 1;

  for (auto pm : {-1, +1}) {
    std::string pm_str = "";
    if (pm == -1) pm_str = "m";
    if (pm == +1) pm_str = "p";

    auto rechits = ic::copy_keep_if(all_rechits, [&](RecHit const* r) {
      return r->eta() * float(pm) > 0.;
    });
    auto genparts = ic::copy_keep_if(all_genparts, [&](GenParticle const* r) {
      return r->eta() * float(pm) > 0.;
    });
    auto simparts = ic::copy_keep_if(all_simparts, [&](SimParticle const* r) {
      return r->eta() * float(pm) > 0.;
    });

    //******************** Build some derived collections

    // Noise filtering for RecHits
    auto filtered_rechits = ic::copy_keep_if(rechits, [](RecHit const* r) {
      return std::get<1>(RecHitAboveThreshold(*r, 3., true));
    });

    // All final state GenParticles
    auto genparts_final = ic::copy_keep_if(genparts, [](ic::GenParticle *p) {
      return p->status() == 1;
    });

    // All final state GenParticles excluding neutrinos
    auto genparts_visible = ic::copy_keep_if(genparts_final, [](ic::GenParticle *p) {
      unsigned pdgid = std::abs(p->pdgid());
      return pdgid != 12 && pdgid != 14 && pdgid != 16;
    });

    // All tau particles
    auto genparts_taus = ic::copy_keep_if(genparts, [](ic::GenParticle *p) {
      return std::abs(p->pdgid()) == 15;
    });

    // Build information about the tau decays
    std::vector<ic::TauInfo> geninfo_taus;
    for (auto const& tau : genparts_taus) {
      geninfo_taus.push_back(HTTGenEvent::BuildTauInfo(tau, genparts, true));
    }

    // Have to build the visible GenJets manually
    std::vector<GenJet> geninfo_taujets(geninfo_taus.size());
    for (unsigned i = 0; i < geninfo_taus.size(); ++i) {
      auto& tinfo = geninfo_taus[i];
      ROOT::Math::PtEtaPhiEVector vec;
      std::vector<std::size_t> id_vec;
      for (unsigned k = 0; k < tinfo.all_vis.size(); ++k) {
        if (abs(tinfo.all_vis[k]->pdgid()) == 12 ||
            abs(tinfo.all_vis[k]->pdgid()) == 14 ||
            abs(tinfo.all_vis[k]->pdgid()) == 16)
          continue;
        vec += tinfo.all_vis[k]->vector();
        geninfo_taujets[i].set_charge(geninfo_taujets[i].charge() +
                                 tinfo.all_vis[k]->charge());
        id_vec.push_back(tinfo.all_vis[k]->id());
      }
      geninfo_taujets[i].set_vector(vec);
      geninfo_taujets[i].set_constituents(id_vec);
      tinfo.vis_jet = &geninfo_taujets[i];
    }

    // Jets clustering rechits in all layers
    fastjet::JetDefinition fj_jet_def(fastjet::antikt_algorithm, 0.1);
    auto jets = ClusterJets(filtered_rechits, fj_jet_def);
    // std::vector<ic::CompositeCandidate *> jets_p;
    // for (auto & j : jets) jets_p.push_back(&j);
    // std::vector<std::vector<ic::CompositeCandidate>> subjets(jets.size());

    std::vector<ic::CompositeCandidate *> selected_jets;
    for (auto & j : jets) {
      if (j.pt() > 15.) {
        selected_jets.push_back(&j);
      }
    }

    typedef std::vector<RecHit *> RecHitVec;
    typedef std::vector<RecHitVec> RecHitLayersVec;
    typedef std::vector<ic::CompositeCandidate> ClusteredJetVec;
    typedef std::vector<ClusteredJetVec> ClusteredJetLayersVec;

    std::vector<RecHitLayersVec> jet_hits_in_layers(selected_jets.size());
    std::vector<ClusteredJetLayersVec> layer_subjets(selected_jets.size());
    std::vector<RecHitLayersVec> signif_rhs(nlayers);
    RecHitVec signif_rhs_singles;

    for (unsigned i = 0; i < selected_jets.size(); ++i) {
      auto components = jets[i].AsVector();
      // Arrange the components into layers
      RecHitLayersVec hits_in_layers(nlayers);
      ClusteredJetLayersVec jets_in_layers(nlayers);
      for (unsigned j = 0; j < components.size(); ++j) {
        RecHit *rh = dynamic_cast<RecHit*>(components[j]);
        unsigned layer_idx = rh->layer() - hgcal::firstLayer;
        if (layer_idx < nlayers) {
          hits_in_layers[layer_idx].push_back(rh);
        }
      }
      fastjet::JetDefinition fj_jet_def(fastjet::antikt_algorithm, 0.02);
      for (unsigned l = 0; l < nlayers; ++l) {
        jets_in_layers[l] = ClusterJets(hits_in_layers[l], fj_jet_def);
      }
      jet_hits_in_layers[i] = hits_in_layers;
      layer_subjets[i] = jets_in_layers;
      // subjets[i] = ClusterJets(components, fj_jet_def);
    }


    // Testing the significant hit algo
    bool nverb = false;
    // if (evt == 15 && pm == -1) {
    //   nverb = true;
    // } else {
    //   nverb = false;
    // }

    if (nverb && jet_hits_in_layers.size()) {
      for (unsigned l = 0; l < jet_hits_in_layers[0].size(); ++l) {
        // std::cout << "Layer " << l << "\n";
        for (unsigned r = 0; r < jet_hits_in_layers[0][l].size(); ++r) {
          // auto rh = jet_hits_in_layers[0][l][r];
          // std::cout << rh->position() << "\t" << rh->thickness() << "\n";
        }
      }
    }


    for (unsigned i = 0; i < selected_jets.size(); ++i) {
      for (unsigned l = 0; l < jet_hits_in_layers[i].size(); ++l) {
        if (nverb) std::cout << "> Jet " << i << " " << selected_jets[i]->vector() << ", layer = " << l << ", hits = " << jet_hits_in_layers[i][l].size() << "\n";
        auto const& hits = jet_hits_in_layers[i][l];
        double spacing = 1.3;
        if (hits.size() && hits[0]->thickness() > 400) spacing = 6.0;
        auto matches = MatchByDxy(hits, hits, spacing, false, false);
        std::map<RecHit *, std::set<RecHit *>> rh_map;
        for (auto const& pair : matches) {
          if (pair.first == pair.second) continue;
          rh_map[pair.first].insert(pair.second);
        }
        for (auto const& neighbours : rh_map) {
          if (nverb) std::cout << neighbours.first->position() << ": ";
          if (nverb) std::cout << neighbours.second.size();

          unsigned n_tot = neighbours.second.size();
          unsigned n_low = 0;
          for (auto const& rh : neighbours.second) {
            if (rh->energy() <= neighbours.first->energy()) ++n_low;
            if (nverb) std::cout << Dxy(neighbours.first, rh) << " ";
          }
          if (nverb) std::cout << n_low << "/" << n_tot;
          if (nverb) std::cout << "\n";
          if (n_tot == n_low && n_tot >= 5) {
            std::vector<RecHit*> cluster;
            cluster.push_back(neighbours.first);
            for (auto rh : neighbours.second) cluster.push_back(rh);
            signif_rhs[l].push_back(cluster);
            signif_rhs_singles.push_back(neighbours.first);
          }
        }

      }
    }


    // Now we have to identify the cluster centers
    double cluster_finder_dr = 0.01;
    std::set<RecHit *> rh_set(signif_rhs_singles.begin(), signif_rhs_singles.end());
    std::vector<std::vector<RecHit *>> found_clusters;
    std::vector<Candidate> found_clusters_p4;
    while (rh_set.size() > 0) {
      auto rh = *(rh_set.begin());
      std::set<RecHit*> result;
      RecursiveMatchByDR(rh, std::vector<RecHit*>(rh_set.begin(), rh_set.end()), result, cluster_finder_dr);
      for (auto rh_result : result) {
        rh_set.erase(rh_result);
      }
      if (result.size() > 1) {
        found_clusters.push_back(std::vector<RecHit*>(result.begin(), result.end()));
        // take the simple average for now
        double eta_sum = 0;
        double phi_sum = 0;
        for (auto rh : found_clusters.back()) {
          eta_sum += rh->eta();
          phi_sum += rh->phi();
        }
        found_clusters_p4.push_back(Candidate());
        found_clusters_p4.back().set_eta(eta_sum / found_clusters.back().size());
        found_clusters_p4.back().set_phi(phi_sum / found_clusters.back().size());
      }
    }

    if (v > 0) {
      std::cout << "Significant RecHits:\n";
      // for (auto rh : signif_rhs_singles) {
      //   std::cout << " >> " << rh->position() << "\n";
      // }
      std::cout << "Clustered:\n";
      for (unsigned i = 0; i < found_clusters.size(); ++i) {
        double e_tot = 0.;
        std::cout << i << ": ";
        for (auto rh : found_clusters[i]) {
          e_tot += rh->energy();
          std::cout << rh->vector() << "  ";
        }
        std::cout << "energy = " << e_tot << ", eta = " << found_clusters_p4[i].eta() << ", phi = " << found_clusters_p4[i].phi() << "\n";
        // auto full_matches = ExtractSecond(MatchByDR(std::vector<Candidate*>{&found_clusters_p4[i]}, selected_jets[0]->AsVector(), cluster_finder_dr, false, false));
        // ROOT::Math::PtEtaPhiEVector cone_collected;
        // for (auto cand : full_matches) cone_collected += cand->vector();
        // std::cout << "  " << cone_collected << "\n";
      }
    }

    std::vector<double> energy_e(found_clusters_p4.size(), 0.);
    std::vector<double> energy_h(found_clusters_p4.size(), 0.);
    std::vector<double> energy_z(found_clusters_p4.size(), 0.);
    ROOT::Math::PtEtaPhiEVector summed_clusters;

    if (selected_jets.size() > 0) {
      auto jet_rhs = selected_jets[0]->AsVector();
      for (unsigned r = 0; r < jet_rhs.size(); ++r) {
        std::vector<double> weights(found_clusters_p4.size());
        double weights_sum = 0.;
        for (unsigned c = 0; c < found_clusters_p4.size(); ++c) {
          weights[c] = std::exp(-1. * DR(jet_rhs[r], &found_clusters_p4[c]) / cluster_finder_dr);
          weights_sum += weights[c];
        }
        for (unsigned c = 0; c < found_clusters_p4.size(); ++c) {
          weights[c] /= weights_sum;
          double e_to_add = jet_rhs[r]->energy() * weights[c];
          found_clusters_p4[c].set_energy(found_clusters_p4[c].energy() + e_to_add);
          if (dynamic_cast<RecHit*>(jet_rhs[r])->layer() <= 28) {
            energy_e[c] += e_to_add;
          } else {
            energy_h[c] += e_to_add;
          }
          energy_z[c] += dynamic_cast<RecHit*>(jet_rhs[r])->z() * e_to_add;
        }
      }
      for (unsigned c = 0; c < found_clusters_p4.size(); ++c) {
        energy_z[c] = energy_z[c] / found_clusters_p4[c].energy();
        found_clusters_p4[c].set_pt(1);
        found_clusters_p4[c].set_pt(found_clusters_p4[c].energy() * std::sin(found_clusters_p4[c].vector().theta()));
        summed_clusters += found_clusters_p4[c].vector();
      }
      if (v > 0) {
        std::cout << "Energy assigned:\n";
        for (unsigned c = 0; c < found_clusters_p4.size(); ++c) {
          std::cout << " >> " << found_clusters_p4[c].vector()
                    << " h/e = " << energy_h[c] / energy_e[c] << " <z> = "
                    << energy_z[c] << "\n";
        }
      }
    }




    //******************** Printing
    if (v > 0) {
      std::cout << "******** Event = " << evt << " (" << pm << ")\n";
      std::cout << "**** GenParticles\n";
      for (unsigned i = 0; i < genparts.size(); ++i) {
        genparts[i]->Print();
      }
      std::cout << "**** TauInfo (n = " << geninfo_taus.size() << ")\n";
      for (unsigned i = 0; i < geninfo_taus.size(); ++i) {
        geninfo_taus[i].Print();
      }
      std::cout << "**** SimParticles\n";
      for (unsigned i = 0; i < simparts.size(); ++i) {
        simparts[i]->Print();
      }
      std::cout << "**** Jets\n";
      for (unsigned i = 0; i < jets.size(); ++i) {
        if (jets[i].pt() > 10.) {
          std::cout << jets[i].vector() << "\n";
          // std::cout << "****** Subjets\n";
          // for (unsigned j = 0; j < subjets[i].size(); ++j) {
          //   std::cout << "    " << subjets[i][j].vector() << "\n";
          // }
          // std::cout << "****** Layer subjets\n";
          // for (unsigned l = 0; l < nlayers; ++l) {
          //   std::cout << "  " << l << ": ";
          //   for (unsigned j = 0; j < layer_subjets[i][l].size(); ++j) {
          //     std::cout << layer_subjets[i][l][j].vector() << " ";
          //   }
          //   std::cout << "\n";
          // }

        }
      }
    }



    static int event_counter = 0;
    bool save = false;
    if (event_counter < 0) {
      save = true;
    }

    unsigned good_pions = 0;
    unsigned good_photons = 0;

    std::vector<TGraph2D> graphs;
    std::vector<TGraph2D> jet_graphs;
    std::vector<TGraph2D> cluster_graphs;
    std::vector<TGraph2D> signif_graph;
    std::vector<TGraph2D> signif_cluster_graph;

    if (save) {
      graphs = PlotRecHitsInLayers(filtered_rechits, TString::Format("%s_rechits", pm_str.c_str()));
      if (layer_subjets.size()) {
        jet_graphs = PlotJetRecHitsInLayers(layer_subjets[0], TString::Format("%s_jets", pm_str.c_str()));
      }
      cluster_graphs = PlotRecHitListsInLayers(signif_rhs, TString::Format("%s_clusters", pm_str.c_str()));
      signif_graph = PlotRecHits(signif_rhs_singles, TString::Format("%s_signif_rechits", pm_str.c_str()));
      for (unsigned i = 0; i < found_clusters.size(); ++i) {
        signif_cluster_graph.push_back(PlotRecHits(found_clusters[i], TString::Format("%s_signif_rechit_clusters_%i", pm_str.c_str(), i))[0]);
      }
    }

    std::vector<TPolyMarker3D> pion_markers(nlayers);
    std::vector<TPolyMarker3D> photon_markers(nlayers);
    TPolyMarker3D pion_etaphi_markers;
    TPolyMarker3D photon_etaphi_markers;

    if (save) {
      for (unsigned i = 0; i < nlayers; ++i) {
        pion_markers[i].SetName(
            TString::Format("%s_pions_%i", pm_str.c_str(), hgcal::firstLayer + i));
        photon_markers[i].SetName(
            TString::Format("%s_photons_%i", pm_str.c_str(), hgcal::firstLayer + i));
      }
      pion_etaphi_markers.SetName(TString::Format("%s_pions", pm_str.c_str()));
      photon_etaphi_markers.SetName(TString::Format("%s_photons", pm_str.c_str()));
    }

    TString infostr;
    std::vector<SimParticle*> sim_photons;
    for (unsigned i = 0; i < simparts.size(); ++i) {
      auto const& sim = *(simparts[i]);
      if ((std::abs(sim.pid()) == 211 || std::abs(sim.pid()) == 321) &&
          sim.gen() != -1) {
        if (sim.reached_ee()) ++good_pions;
        infostr += TString::Format("%g,%g,%g:", sim.energy(), sim.eta(), sim.phi());
        if (save) {
          for (unsigned j = 0; j < sim.layer_positions().size(); ++j) {
            pion_markers[j].SetNextPoint(sim.layer_positions()[j].x(),
                                         sim.layer_positions()[j].y(), 0);
          }
          pion_etaphi_markers.SetNextPoint(sim.eta(), sim.phi(), 0.);
        }
      }

      if ((std::abs(sim.pid()) == 22) && sim.gen() != -1) {
        sim_photons.push_back(simparts[i]);
        if (sim.reached_ee()) ++good_photons;
        infostr += TString::Format("%g,%g,%g:", sim.energy(), sim.eta(), sim.phi());
        if (save) {
          for (unsigned j = 0; j < sim.layer_positions().size(); ++j) {
            photon_markers[j].SetNextPoint(sim.layer_positions()[j].x(),
                                           sim.layer_positions()[j].y(), 0);
          }
          photon_etaphi_markers.SetNextPoint(sim.eta(), sim.phi(), 0.);

        }
      }
    }

    for (unsigned i = 0; i < geninfo_taus.size(); ++i) {
      auto& tinfo = geninfo_taus[i];

      tt_dm_ = -1;
      tt_reached_ee_ = 0;
      tt_pion_reached_ee_ = 0;
      tt_tau_pt_ = 0.;
      tt_tau_eta_ = 0.;
      tt_vis_tau_pt_ = 0.;
      tt_vis_tau_eta_ = 0.;
      tt_dm1_pho1_e_ = 0.;
      tt_dm1_pho2_e_ = 0.;
      tt_dm1_pion_e_ = 0.;
      tt_pho_dxy_l1_ = 0.;
      tt_prods_dr_ = 0.;
      tt_evt_ = evt;
      tt_pm_ = pm;
      tt_rec_prongs_ = 0;
      tt_mass_ = 0.;
      tt_jet_mass_ = 0.;
      if (tinfo.hadronic_mode >= 0) {
        if (save) {
          graphs[0].SetTitle(
              TString::Format("#tau_{h} mode %i:", tinfo.hadronic_mode) + infostr);
        }
        if (tinfo.pi_charged.size() == good_pions) {
          tt_pion_reached_ee_ = 1;
        }
        if (tinfo.pi_charged.size() == good_pions &&
            (tinfo.pi_neutral.size() == good_photons / 2)) {
          tt_reached_ee_ = 1;
        }

        tt_dm_ = tinfo.hadronic_mode;
        tt_tau_pt_ = tinfo.tau_st2_post_fsr->pt();
        tt_tau_eta_ = tinfo.tau_st2_post_fsr->eta();
        tt_vis_tau_pt_ = tinfo.vis_jet->pt();
        tt_vis_tau_eta_ = tinfo.vis_jet->eta();
        if (tt_dm_ == 1) {
          tt_prods_dr_ = DR(tinfo.pi_charged.at(0), tinfo.pi_neutral.at(0));
        }

        if (tt_reached_ee_ && tt_dm_ == 1) {
          double dx = sim_photons[0]->layer_positions()[0].x() -
                      sim_photons[1]->layer_positions()[0].x();
          double dy = sim_photons[0]->layer_positions()[0].y() -
                      sim_photons[1]->layer_positions()[0].y();
          tt_pho_dxy_l1_ = std::sqrt(dx * dx + dy * dy);
          // if (tt_reached_ee_ && tt_pho_dxy_l1_ < 1.) std::cout << evt << "\t" << (tinfo.tau_st2_post_fsr->eta() >= 0.) << "\t" << tt_pho_dxy_l1_ << "\n";
        }
      }
      tt_rec_prongs_ = found_clusters.size();
      if (selected_jets.size() >= 1) {
        tt_jet_mass_ = selected_jets[0]->M();
      }
      if (found_clusters_p4.size() >= 2) {
        tt_mass_ = summed_clusters.M();
      }
      t_taus_->Fill();
    }

    if (save) {
      auto subdir = fs_->mkdir(TString::Format("event_%li", evt).Data());
      for (auto const& gr : graphs) {
        subdir.make<TGraph2D>(gr);
      }
      for (auto const& gr : signif_graph) {
        subdir.make<TGraph2D>(gr);
      }
      // for (auto const& gr : jet_graphs) {
      //   subdir.make<TGraph2D>(gr);
      // }
      for (auto const& gr : cluster_graphs) {
        subdir.make<TGraph2D>(gr);
      }
      for (auto const& gr : signif_cluster_graph) {
        subdir.make<TGraph2D>(gr);
      }
      for (auto const& gr : pion_markers) {
        subdir.make<TPolyMarker3D>(gr);
      }
      for (auto const& gr : photon_markers) {
        subdir.make<TPolyMarker3D>(gr);
      }
      subdir.make<TPolyMarker3D>(pion_etaphi_markers);
      subdir.make<TPolyMarker3D>(photon_etaphi_markers);
    }

    event_counter += 1;
  }









  //  // print the jets
  //  // cout <<   "        pt y phi" << endl;
  //  // for (unsigned i = 0; i < jets.size(); i++) {
  //  //   cout << "jet " << i << ": "<< jets[i].pt() << " " 
  //  //                  << jets[i].rap() << " " << jets[i].phi() << endl;
  //  //   vector<PseudoJet> constituents = jets[i].constituents();
  //  //   for (unsigned j = 0; j < constituents.size(); j++) {
  //  //     cout << "    constituent " << j << "'s pt: " << constituents[j].pt()
  //  //          << endl;
  //  //   }
  //  // }

  // std::string label_p = "";
  // std::string label_m = "";


  // // choose a jet definition
  // fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, 0.4);
  // auto jets = ClusterJets(filtered_rechits, jet_def);
  // std::vector<ic::CompositeCandidate *> jets_p;
  // for (auto & j : jets) jets_p.push_back(&j);

  // auto genJets = ClusterJets(genParts_final, jet_def);
  // std::vector<ic::CompositeCandidate *> genJets_p;
  // for (auto & j : genJets) genJets_p.push_back(&j);

  // auto genJets_nonu = ClusterJets(genParts_finalNoNu, jet_def);
  // std::vector<ic::CompositeCandidate *> genJets_nonu_p;
  // for (auto & j : genJets_nonu) genJets_nonu_p.push_back(&j);


  // auto matches = ic::MatchByDR(jets_p, genJets_p, 0.4, true, true);
  // std::map<CompositeCandidate*, CompositeCandidate*> matchMap;
  // for (auto const& pair : matches) matchMap[pair.first] = pair.second;

  // auto matches_nonu = ic::MatchByDR(jets_p, genJets_nonu_p, 0.4, true, true);
  // std::map<CompositeCandidate*, CompositeCandidate*> matchMap_nonu;
  // for (auto const& pair : matches_nonu) matchMap_nonu[pair.first] = pair.second;


  // // std::cout << "Simcluster sums:\n";
  // // std::cout << sc_sum_p << "\n";
  // // std::cout << sc_sum_m << "\n";
  // // std::cout << "RecHit sums:\n";
  // // std::cout << rh_sum_p << "\n";
  // // std::cout << rh_sum_m << "\n";
  // // std::cout << "RecHit (filtered) sums:\n";
  // // std::cout << rhf_sum_p << "\n";
  // // std::cout << rhf_sum_m << "\n";

  // // std::cout << "Highest pT jets:\n";
  // unsigned print = std::min(unsigned(jets.size()), unsigned(5));
  // for (unsigned i = 0; i < print; i++) {
  //   // std::cout << tmpjet << "\n";

  //   // Do the two leading reco jets for now
  //   if (i >= 2) continue;

  //   jet_pt_ = jets[i].pt();
  //   jet_eta_ = jets[i].eta();
  //   jet_phi_ = jets[i].phi();
  //   jet_e_ = jets[i].energy();

  //   // Is the jet matched to a full genjet (neutrinos included)
  //   if (matchMap.count(jets_p[i])) {
  //     gen_matched_ = true;
  //     genjet_pt_ = matchMap[jets_p[i]]->pt();
  //     genjet_eta_ = matchMap[jets_p[i]]->eta();
  //     genjet_phi_ = matchMap[jets_p[i]]->phi();
  //     genjet_e_ = matchMap[jets_p[i]]->energy();
  //   } else {
  //     gen_matched_ = false;
  //     genjet_pt_ = -1.;
  //     genjet_eta_ = -1.;
  //     genjet_phi_ = -1.;
  //     genjet_e_ = -1.;
  //   }

  //   // Tau DM finding - false by default
  //   isDM_r0p02_ = false;
  //   isDM_r0p04_ = false;
  //   isDM_r0p06_ = false;
  //   isDM_r0p08_ = false;
  //   isDM_r0p10_ = false;

  //   if (matchMap_nonu.count(jets_p[i])) {
  //     gen_nonu_matched_ = true;
  //     genjet_nonu_pt_ = matchMap_nonu[jets_p[i]]->pt();
  //     genjet_nonu_eta_ = matchMap_nonu[jets_p[i]]->eta();
  //     genjet_nonu_phi_ = matchMap_nonu[jets_p[i]]->phi();
  //     genjet_nonu_e_ = matchMap_nonu[jets_p[i]]->energy();

  //     is_hadtau_ = false;
  //     bool found_emu = false;
  //     bool tau_matched = false;
  //     std::vector<CompositeCandidate const*> partv = {matchMap_nonu[jets_p[i]]};
  //     unsigned ncharged_0p02 = 0;
  //     unsigned ncharged_0p04 = 0;
  //     unsigned ncharged_0p06 = 0;
  //     unsigned ncharged_0p08 = 0;
  //     unsigned ncharged_0p10 = 0;
  //     int charge_0p02 = 0;
  //     int charge_0p04 = 0;
  //     int charge_0p06 = 0;
  //     int charge_0p08 = 0;
  //     int charge_0p10 = 0;

  //     // Loop through genjet constituents to figure out if this is a hadronic tau, and if it passes DM
  //     for (unsigned c = 0; c < matchMap_nonu[jets_p[i]]->AsVector().size(); ++c) {
  //       GenParticle const* part = dynamic_cast<GenParticle const*>(matchMap_nonu[jets_p[i]]->at(c));
  //       if (std::abs(part->pdgid()) == 11 || std::abs(part->pdgid()) == 13) {
  //         found_emu = true;
  //       }
  //       double dr = DR(jets_p[i], part);
  //       bool is_charged = (std::abs(part->charge()) == 1);
  //       if (is_charged && dr < 0.02) ncharged_0p02 += 1;
  //       if (is_charged && dr < 0.04) ncharged_0p04 += 1;
  //       if (is_charged && dr < 0.06) ncharged_0p06 += 1;
  //       if (is_charged && dr < 0.08) ncharged_0p08 += 1;
  //       if (is_charged && dr < 0.10) ncharged_0p10 += 1;
  //       if (is_charged && dr < 0.02) charge_0p02 += part->charge();
  //       if (is_charged && dr < 0.04) charge_0p04 += part->charge();
  //       if (is_charged && dr < 0.06) charge_0p06 += part->charge();
  //       if (is_charged && dr < 0.08) charge_0p08 += part->charge();
  //       if (is_charged && dr < 0.10) charge_0p10 += part->charge();
  //     }
  //     if (MatchByDR(partv, genparts_taus, 0.4, true, true).size()) tau_matched = true;
  //     if (!found_emu && tau_matched) is_hadtau_ = true;
  //     if ((ncharged_0p02 == 1 || ncharged_0p02 == 3) && std::abs(charge_0p02) == 1) isDM_r0p02_ = true;
  //     if ((ncharged_0p04 == 1 || ncharged_0p04 == 3) && std::abs(charge_0p04) == 1) isDM_r0p04_ = true;
  //     if ((ncharged_0p06 == 1 || ncharged_0p06 == 3) && std::abs(charge_0p06) == 1) isDM_r0p06_ = true;
  //     if ((ncharged_0p08 == 1 || ncharged_0p08 == 3) && std::abs(charge_0p08) == 1) isDM_r0p08_ = true;
  //     if ((ncharged_0p10 == 1 || ncharged_0p10 == 3) && std::abs(charge_0p10) == 1) isDM_r0p10_ = true;
  //   } else {
  //     gen_nonu_matched_ = false;
  //     genjet_nonu_pt_ = -1.;
  //     genjet_nonu_eta_ = -1.;
  //     genjet_nonu_phi_ = -1.;
  //     genjet_nonu_e_ = -1.;
  //     is_hadtau_ = false;
  //   }

  //   double e_sum_all = 0.;
  //   double e_sum_0p02 = 0.;
  //   double e_sum_0p04 = 0.;
  //   double e_sum_0p06 = 0.;
  //   double e_sum_0p08 = 0.;
  //   double e_sum_0p10 = 0.;
  //   for (unsigned c = 0; c < jets_p[i]->AsVector().size(); ++c) {
  //     double energy = jets_p[i]->At(c)->energy();
  //     e_sum_all += energy;
  //     double dr = DR(jets_p[i], jets_p[i]->At(c));
  //     if (dr < 0.02) e_sum_0p02 += energy;
  //     if (dr < 0.04) e_sum_0p04 += energy;
  //     if (dr < 0.06) e_sum_0p06 += energy;
  //     if (dr < 0.08) e_sum_0p08 += energy;
  //     if (dr < 0.10) e_sum_0p10 += energy;
  //   }
  //   efrac_r0p02_ = e_sum_0p02 / e_sum_all;
  //   efrac_r0p04_ = e_sum_0p04 / e_sum_all;
  //   efrac_r0p06_ = e_sum_0p06 / e_sum_all;
  //   efrac_r0p08_ = e_sum_0p08 / e_sum_all;
  //   efrac_r0p10_ = e_sum_0p10 / e_sum_all;

  //   // Tau reconstruction
  //   // Start by plotting jet energy fraction contained within a cone of various sizes

  //   t_jets_->Fill();
  //   // std::cout << "jet " << i << ": "<< jets[i].pt() << " "
  //   //                << jets[i].eta() << " " << jets[i].phi() << std::endl;
  // }

  // // print = std::min(unsigned(genJets.size()), unsigned(5));
  // // for (unsigned i = 0; i < print; i++) {
  // //   std::cout << "genjet " << i << ": "<< genJets[i].pt() << " "
  // //                  << genJets[i].eta() << " " << genJets[i].phi() << std::endl;
  // // }


  return 0;
}

int HGCALTest::PostAnalysis() { return 0; }


std::vector<TGraph2D> PlotRecHits(std::vector<RecHit *> rechits,
                                          TString name_prefix) {
  std::vector<TGraph2D> res;
  if (rechits.size() == 0) return res;
  std::vector<float> eta(rechits.size());
  std::vector<float> phi(rechits.size());
  std::vector<float> e(rechits.size());
  for (unsigned l = 0; l < rechits.size(); ++l) {
    eta[l] = (rechits[l]->position().eta());
    phi[l] = (rechits[l]->position().phi());
    e[l] = (rechits[l]->energy());
  }
  res.push_back(TGraph2D(eta.size(), &(eta[0]), &(phi[0]), &(e[0])));
  res.back().SetName(name_prefix);
  return res;
}

std::vector<TGraph2D> PlotJetRecHitsInLayers(std::vector<std::vector<ic::CompositeCandidate>> jets,
                                          TString name_prefix) {
  std::vector<TGraph2D> res;
  for (unsigned l = 0; l < jets.size(); ++l) {
    for (unsigned j = 0; j < jets[l].size(); ++j) {
      auto cands = jets[l][j].AsVector();
      std::vector<float> x(cands.size());
      std::vector<float> y(cands.size());
      std::vector<float> e(cands.size());
      for (unsigned c = 0; c < cands.size(); ++c) {
        auto rh = dynamic_cast<RecHit*>(cands[c]);
        x[c] = (rh->position().x());
        y[c] = (rh->position().y());
        e[c] = (rh->energy());
      }
      res.push_back(TGraph2D(x.size(), &(x[0]), &(y[0]), &(e[0])));
      res.back().SetName(TString::Format("%s_%i_%i", name_prefix.Data(), hgcal::firstLayer+l, j));
    }
  }
  return res;
}

std::vector<TGraph2D> PlotRecHitListsInLayers(std::vector<std::vector<std::vector<RecHit *>>> jets,
                                          TString name_prefix) {
  std::vector<TGraph2D> res;
  for (unsigned l = 0; l < jets.size(); ++l) {
    for (unsigned j = 0; j < jets[l].size(); ++j) {
      auto cands = jets[l][j];
      std::vector<float> x(cands.size());
      std::vector<float> y(cands.size());
      std::vector<float> e(cands.size());
      for (unsigned c = 0; c < cands.size(); ++c) {
        auto rh = dynamic_cast<RecHit*>(cands[c]);
        x[c] = (rh->position().x());
        y[c] = (rh->position().y());
        e[c] = (rh->energy());
      }
      res.push_back(TGraph2D(x.size(), &(x[0]), &(y[0]), &(e[0])));
      res.back().SetName(TString::Format("%s_%i_%i", name_prefix.Data(), hgcal::firstLayer+l, j));
    }
  }
  return res;
}


std::vector<TGraph2D> PlotRecHitsInLayers(std::vector<RecHit *> const &rechits,
                                          TString name_prefix) {
  std::vector<TGraph2D> res;
  unsigned nlayers = hgcal::lastLayer - hgcal::firstLayer + 1;
  std::vector<std::vector<float>> x(nlayers);
  std::vector<std::vector<float>> y(nlayers);
  std::vector<std::vector<float>> e(nlayers);
  for (auto const& rh : rechits) {
    unsigned layer_idx = rh->layer() - hgcal::firstLayer;
    x[layer_idx].push_back(rh->position().x());
    y[layer_idx].push_back(rh->position().y());
    e[layer_idx].push_back(rh->energy());
  }
  for (unsigned i = 0; i < nlayers; ++i) {
    res.push_back(TGraph2D(x[i].size(), &(x[i][0]), &(y[i][0]), &(e[i][0])));
    res.back().SetName(TString::Format("%s_%i", name_prefix.Data(), hgcal::firstLayer+i));
  }
  return res;
}

double sigmaNoiseMeV(int layer, int thicknessIndex) {
  return sigmaNoiseMIP(layer, thicknessIndex) * MeVperMIP(layer, thicknessIndex);
}

double sigmaNoiseMIP(int layer, int thicknessIndex) {
  if (layer > 40) {
    return hgcal::noise_MIP;
  } else {
    return hgcal::fC_per_ele * hgcal::nonAgedNoises[thicknessIndex] / hgcal::fCPerMIP[thicknessIndex];
  }
  // for BH, sigmaNoiseMIP = noise_MIP
}

double MeVperMIP(int layer, int thicknessIndex) {
    if (layer > 40) {
        return hgcal::dEdX_weights[layer];
    } else {
        return hgcal::dEdX_weights[layer] / hgcal::thicknessCorrection[thicknessIndex];
    }
}

std::tuple<double, bool> RecHitAboveThreshold(RecHit const& rHit, double ecut, bool dependSensor) {
  double sigmaNoise = 1.;
    if(dependSensor) {
      int thickIndex = -1;
      if( rHit.layer() <= hgcal::lastLayerFH ) { // # EE + FH
          float thickness = rHit.thickness();
          if(thickness>99. and thickness<101.) {
            thickIndex=0;
          } else if (thickness>199. and thickness<201.) {
            thickIndex=1;
          } else if (thickness>299. and thickness<301.) {
            thickIndex=2;
          } else {
            std::cout << "ERROR - silicon thickness has a nonsensical value\n";
          }
      }
      // determine noise for each sensor/subdetector using RecHitCalibration library
      // RecHitCalib = RecHitCalibration()
      sigmaNoise = 0.001 * sigmaNoiseMeV(rHit.layer(), thickIndex); // returns threshold for EE, FH, BH (in case of BH thickIndex does not play a role)

    }
    bool aboveTreshold = rHit.energy() >= ecut*sigmaNoise;  // this checks if rechit energy is above the threshold of ecut (times the sigma noise for the sensor, if that option is set)
    return std::make_tuple(sigmaNoise, aboveTreshold);
}

std::vector<RecHit*> const& BuildRecHitCollection(TreeEvent* event) {
  auto const* rechit_pt = event->GetPtr<std::vector<float>>("rechit_pt");
  auto const* rechit_eta = event->GetPtr<std::vector<float>>("rechit_eta");
  auto const* rechit_phi = event->GetPtr<std::vector<float>>("rechit_phi");
  auto const* rechit_e = event->GetPtr<std::vector<float>>("rechit_energy");
  auto const* rechit_x = event->GetPtr<std::vector<float>>("rechit_x");
  auto const* rechit_y = event->GetPtr<std::vector<float>>("rechit_y");
  auto const* rechit_z = event->GetPtr<std::vector<float>>("rechit_z");
  auto const* rechit_layer = event->GetPtr<std::vector<int>>("rechit_layer");
  auto const* rechit_thickness = event->GetPtr<std::vector<float>>("rechit_thickness");

  event->Add("rechits_storage", std::vector<RecHit>());
  auto& rechit_store = event->Get<std::vector<RecHit>>("rechits_storage");
  event->Add("rechits", std::vector<RecHit*>());
  auto& rechits = event->Get<std::vector<RecHit*>>("rechits");

  unsigned n = rechit_e->size();
  rechit_store.resize(n);
  rechits.resize(n);

  std::hash<RecHit*> hasher;

  int min_layer = 999;
  int max_layer = 0;

  for (unsigned i = 0; i < n; ++i) {
    ic::RecHit& dest = rechit_store[i];
    dest.set_pt((*rechit_pt)[i]);
    dest.set_eta((*rechit_eta)[i]);
    dest.set_phi((*rechit_phi)[i]);
    dest.set_energy((*rechit_e)[i]);
    dest.set_layer((*rechit_layer)[i]);
    dest.set_thickness((*rechit_thickness)[i]);
    dest.set_x((*rechit_x)[i]);
    dest.set_y((*rechit_y)[i]);
    dest.set_z((*rechit_z)[i]);
    dest.set_id(hasher(&dest));
    rechits[i] = &dest;
    if (dest.layer() < min_layer) min_layer = dest.layer();
    if (dest.layer() > max_layer) max_layer = dest.layer();
  }
  // std::cout << "Layers: " << min_layer << "/" << max_layer << "\n";
  return rechits;
  }

  std::vector<SimCluster *> const& BuildSimClusterCollection(TreeEvent *event) {
    auto const* simcluster_pt = event->GetPtr<std::vector<float>>("simcluster_pt");
    auto const* simcluster_eta = event->GetPtr<std::vector<float>>("simcluster_eta");
    auto const* simcluster_phi = event->GetPtr<std::vector<float>>("simcluster_phi");
    auto const* simcluster_e = event->GetPtr<std::vector<float>>("simcluster_energy");

    event->Add("simclusters_storage", std::vector<SimCluster>());
    auto & simcluster_store = event->Get<std::vector<SimCluster>>("simclusters_storage");
    event->Add("simclusters", std::vector<SimCluster *>());
    auto & simclusters = event->Get<std::vector<SimCluster *>>("simclusters");

    unsigned n = simcluster_e->size();
    simcluster_store.resize(n);
    simclusters.resize(n);

    std::hash<SimCluster*> hasher;

    for (unsigned i = 0; i < n; ++i) {
      ic::SimCluster & dest = simcluster_store[i];
      dest.set_pt((*simcluster_pt)[i]);
      dest.set_eta((*simcluster_eta)[i]);
      dest.set_phi((*simcluster_phi)[i]);
      dest.set_energy((*simcluster_e)[i]);
      dest.set_id(hasher(&dest));
      simclusters[i] = &dest;
    }
    return simclusters;
  }

  std::vector<ic::GenParticle *> const &BuildGenParticleCollection(TreeEvent *event) {
    auto const* gen_pt = event->GetPtr<std::vector<float>>("gen_pt");
    auto const* gen_eta = event->GetPtr<std::vector<float>>("gen_eta");
    auto const* gen_phi = event->GetPtr<std::vector<float>>("gen_phi");
    auto const* gen_e = event->GetPtr<std::vector<float>>("gen_energy");
    auto const* gen_charge = event->GetPtr<std::vector<int>>("gen_charge");
    auto const* gen_pdgid = event->GetPtr<std::vector<int>>("gen_pdgid");
    auto const* gen_status = event->GetPtr<std::vector<int>>("gen_status");
    auto const* gen_daughters = event->GetPtr<std::vector<std::vector<int>>>("gen_daughters");

    event->Add("genParticles_storage", std::vector<ic::GenParticle>());
    auto & gen_store = event->Get<std::vector<ic::GenParticle>>("genParticles_storage");
    event->Add("genParticles", std::vector<ic::GenParticle *>());
    auto & gens = event->Get<std::vector<ic::GenParticle *>>("genParticles");

    unsigned n = gen_e->size();
    gen_store.resize(n);
    gens.resize(n);

    std::hash<ic::GenParticle*> hasher;

    for (unsigned i = 0; i < n; ++i) {
      ic::GenParticle & dest = gen_store[i];
      dest.set_index(i);
      dest.set_pt((*gen_pt)[i]);
      dest.set_eta((*gen_eta)[i]);
      dest.set_phi((*gen_phi)[i]);
      dest.set_energy((*gen_e)[i]);
      dest.set_charge((*gen_charge)[i]);
      dest.set_pdgid((*gen_pdgid)[i]);
      dest.set_status((*gen_status)[i]);
      dest.set_id(hasher(&dest));
      dest.set_daughters((*gen_daughters)[i]);
      gens[i] = &dest;
    }
    return gens;
  }

  std::vector<SimParticle *> const &BuildSimParticleCollection(TreeEvent *event) {
    auto const* genpart_pt = event->GetPtr<std::vector<float>>("genpart_pt");
    auto const* genpart_eta = event->GetPtr<std::vector<float>>("genpart_eta");
    auto const* genpart_phi = event->GetPtr<std::vector<float>>("genpart_phi");
    auto const* genpart_e = event->GetPtr<std::vector<float>>("genpart_energy");

    auto const* genpart_dvx = event->GetPtr<std::vector<float>>("genpart_dvx");
    auto const* genpart_dvy = event->GetPtr<std::vector<float>>("genpart_dvy");
    auto const* genpart_dvz = event->GetPtr<std::vector<float>>("genpart_dvz");

    auto const* genpart_ovx = event->GetPtr<std::vector<float>>("genpart_ovx");
    auto const* genpart_ovy = event->GetPtr<std::vector<float>>("genpart_ovy");
    auto const* genpart_ovz = event->GetPtr<std::vector<float>>("genpart_ovz");

    auto const* genpart_pid = event->GetPtr<std::vector<int>>("genpart_pid");
    auto const* genpart_gen = event->GetPtr<std::vector<int>>("genpart_gen");
    auto const* genpart_mother = event->GetPtr<std::vector<int>>("genpart_mother");
    auto const* genpart_reachedEE = event->GetPtr<std::vector<int>>("genpart_reachedEE");
    auto const* genpart_fromBeamPipe = event->GetPtr<std::vector<bool>>("genpart_fromBeamPipe");


    auto const* genpart_posx = event->GetPtr<std::vector<std::vector<float>>>("genpart_posx");
    auto const* genpart_posy = event->GetPtr<std::vector<std::vector<float>>>("genpart_posy");
    auto const* genpart_posz = event->GetPtr<std::vector<std::vector<float>>>("genpart_posz");

    event->Add("simParticles_storage", std::vector<ic::SimParticle>());
    auto & sim_store = event->Get<std::vector<ic::SimParticle>>("simParticles_storage");
    event->Add("simParticles", std::vector<ic::SimParticle *>());
    auto & sims = event->Get<std::vector<ic::SimParticle *>>("simParticles");

    unsigned n = genpart_pt->size();
    sim_store.resize(n);
    sims.resize(n);

    // std::hash<ic::SimParticle*> hasher;

    for (unsigned i = 0; i < n; ++i) {
      SimParticle & dest = sim_store[i];
      dest.set_pt((*genpart_pt)[i]);
      dest.set_eta((*genpart_eta)[i]);
      dest.set_phi((*genpart_phi)[i]);
      dest.set_energy((*genpart_e)[i]);
      dest.set_charge(0);
      dest.set_index(i);

      dest.set_x((*genpart_dvx)[i]);
      dest.set_y((*genpart_dvy)[i]);
      dest.set_z((*genpart_dvz)[i]);

      dest.set_origin_x((*genpart_ovx)[i]);
      dest.set_origin_y((*genpart_ovy)[i]);
      dest.set_origin_z((*genpart_ovz)[i]);


      dest.set_pid((*genpart_pid)[i]);
      dest.set_gen((*genpart_gen)[i]);
      if (dest.gen() > 0) dest.set_gen(dest.gen() - 1);
      dest.set_mother((*genpart_mother)[i]);
      dest.set_reached_ee((*genpart_reachedEE)[i]);
      dest.set_from_beampipe((*genpart_fromBeamPipe)[i]);

      std::vector<ROOT::Math::XYZPoint> positions((*genpart_posx)[i].size());
      for (unsigned j = 0; j < (*genpart_posx)[i].size(); ++j) {
        positions[j].SetX((*genpart_posx)[i][j]);
        positions[j].SetY((*genpart_posy)[i][j]);
        positions[j].SetZ((*genpart_posz)[i][j]);
      }
      dest.set_layer_positions(positions);
      sims[i] = &dest;
    }
    return sims;
  }

  void SimParticle::Print() const {
    std::cout << (boost::format("idx: %-4i pid: %-4i gen: %-3i mother: %-3i reached_ee: %-3i from_bp: %-3i %-40s  M: %f\n") %
                  this->index() % this->pid() % this->gen() % this->mother() % this->reached_ee() % this->from_beampipe() %
                     this->vector() % this->M());
  }

  void RecursiveMatchByDR(RecHit* c1, std::vector<RecHit*> collection, std::set<RecHit*> & result, double maxDR) {
    // std::cout << result.size() << "\t" << collection.size() << "\n";
    result.insert(c1);
    std::vector<std::pair<RecHit*,RecHit*>> matches = ic::MatchByDR(std::vector<RecHit*>{c1}, collection, maxDR, false, false);
    for (auto match : matches) {
      if (!result.count(match.second)) {
        RecursiveMatchByDR(match.second, collection, result, maxDR);
      }
    }
  }



}

