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
#include "HGCAL/interface/HGCALTools.h"
#include "HGCAL/interface/TauReco.h"

namespace ic {

HGCALObjectBuilder::HGCALObjectBuilder(std::string const& name)
    : ModuleBase(name) {}

HGCALObjectBuilder::~HGCALObjectBuilder() { ; }

int HGCALObjectBuilder::PreAnalysis() { return 0; }

int HGCALObjectBuilder::Execute(TreeEvent* event) {
  BuildRecHitCollection(event);
  BuildGenParticleCollection(event);
  BuildSimParticleCollection(event);
  BuildSimClusterCollection(event);
  return 0;
}

int HGCALObjectBuilder::PostAnalysis() { return 0; }

HGCALTest::HGCALTest(std::string const& name) : ModuleBase(name) {}

HGCALTest::~HGCALTest() { ; }

int HGCALTest::PreAnalysis() {

  t_taus_ = fs_->make<TTree>("taus", "taus");
  t_taus_evt_.AddToTree(t_taus_, "");
  t_taus_rec_.AddToTree(t_taus_, "rec_");
  t_taus_gen_.AddToTree(t_taus_, "gen_");
  t_taus_m1_gen_.AddToTree(t_taus_, "m1_");
  t_taus_m1_match_.AddToTree(t_taus_, "m1_");

  return 0;
}

int HGCALTest::Execute(TreeEvent* event) {
  unsigned v = 1;

  std::vector<std::pair<int,int>> save_events;

  save_events = {
    {41, 23},
    {42, 23},
    {45, 23}
  };

  int long evt = *event->GetPtr<int long>("event");
  int long lumi = *event->GetPtr<int long>("lumi");

  // Start by building object collections from flat trees
  auto const& all_rechits = event->GetPtrVec<RecHit>("rechits");
  auto const& all_genparts = event->GetPtrVec<GenParticle>("genParticles");
  auto const& all_simparts = event->GetPtrVec<SimParticle>("simParticles");
  auto const& all_tauinfos = event->Get<std::vector<TauReco::TauInfo>>("tauInfos");
  auto const& all_simclusters = event->GetPtrVec<SimCluster>("simclusters");

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
    auto simclusters = ic::copy_keep_if(all_simclusters, [&](SimCluster const* r) {
      return r->eta() * float(pm) > 0.;
    });

    std::vector<TauReco::TauInfo const*> tauinfos;
    for (auto & info : all_tauinfos) {
      if (info.jet.eta() * float(pm) > 0.) {
        tauinfos.push_back(&info);
      }
    }

    std::map<unsigned, RecHit*> rechit_id_map;
    for (auto r : rechits) {
      rechit_id_map[r->id()] = r;
    }

    //******************** Build some derived collections

    // Noise filtering for RecHits
    auto filtered_rechits = ic::copy_keep_if(rechits, [](RecHit const* r) {
      return std::get<1>(RecHitAboveThreshold(*r, 5., true));
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
      for (unsigned i = 0; i < tauinfos.size(); ++i) {
        tauinfos[i]->Print();
      }
    }

    static int event_counter = 0;
    bool save = false;
    for (auto to_save : save_events) {
      if (evt == to_save.first && lumi == to_save.second) save = true;
      // break;
    }
    // if (event_counter < 20) {
    //   save = true;
    // }

    unsigned good_pions = 0;
    unsigned good_photons = 0;

    std::vector<TGraph2D> graphs;
    std::vector<TGraph2D> jet_graphs;
    std::vector<TGraph2D> cluster_graphs;
    std::vector<TGraph2D> signif_graph;
    std::vector<TGraph2D> signif_cluster_graph;

    if (save) {
      graphs = PlotRecHitsInLayers(filtered_rechits, TString::Format("%s_rechits", pm_str.c_str()));
      // if (layer_subjets.size()) {
      //   jet_graphs = PlotJetRecHitsInLayers(layer_subjets[0], TString::Format("%s_jets", pm_str.c_str()));
      // }
      if (tauinfos.size() >= 1) {
        cluster_graphs = PlotRecHitListsInLayers(tauinfos[0]->selected_hit_patterns_in_layers, TString::Format("%s_clusters", pm_str.c_str()));
        signif_graph = PlotRecHits(tauinfos[0]->all_selected_central_hits, TString::Format("%s_signif_rechits", pm_str.c_str()));
        for (unsigned i = 0; i < tauinfos[0]->merged_central_hits.size(); ++i) {
          signif_cluster_graph.push_back(PlotRecHits(tauinfos[0]->merged_central_hits[i], TString::Format("%s_signif_rechit_clusters_%i", pm_str.c_str(), i))[0]);
        }
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
          if (sim.layer_positions().size() > 0) {
            photon_etaphi_markers.SetNextPoint(sim.layer_positions()[0].eta(), sim.layer_positions()[0].phi(), 0.);
          } else {
            photon_etaphi_markers.SetNextPoint(sim.eta(), sim.phi(), 0.);
          }

        }
      }
    }

    for (unsigned i = 0; i < geninfo_taus.size(); ++i) {
      auto& tinfo = geninfo_taus[i];

      if (tinfo.hadronic_mode >= 0) {
        if (save) {
          graphs[0].SetTitle(
              TString::Format("#tau_{h} mode %i:", tinfo.hadronic_mode) + infostr);
        }
      }

      t_taus_evt_.evt = evt;
      t_taus_evt_.lumi = lumi;
      t_taus_evt_.pm = pm;

      t_taus_gen_.dm = tinfo.hadronic_mode;
      t_taus_gen_.pt = tinfo.tau_st2_post_fsr->pt();
      t_taus_gen_.eta = tinfo.tau_st2_post_fsr->eta();
      t_taus_gen_.vis_pt = tinfo.vis_jet->pt();
      t_taus_gen_.vis_eta = tinfo.vis_jet->eta();



      t_taus_m1_gen_.all_reached_ee = false;
      t_taus_m1_gen_.pi_reached_ee = false;
      t_taus_m1_gen_.p1_reached_ee = false;
      t_taus_m1_gen_.p2_reached_ee = false;
      t_taus_m1_gen_.pi_e = 0.;
      t_taus_m1_gen_.p1_e = 0.;
      t_taus_m1_gen_.p2_e = 0.;
      t_taus_m1_gen_.p1_p2_dxy = 0.;
      t_taus_m1_gen_.has_gen_ele_pair = false;


      t_taus_rec_.jet_pt = 0.;
      t_taus_rec_.jet_eta = 0.;
      t_taus_rec_.jet_phi = 0.;
      t_taus_rec_.jet_e = 0.;
      t_taus_rec_.nprongs = 0;

      t_taus_m1_match_.matched = false;
      t_taus_m1_match_.nmatched_to_pi = 0.;
      t_taus_m1_match_.nmatched_to_p1 = 0.;
      t_taus_m1_match_.nmatched_to_p2 = 0.;
      t_taus_m1_match_.pi_bestmatch_e = 0.;
      t_taus_m1_match_.p1_bestmatch_e = 0.;
      t_taus_m1_match_.p2_bestmatch_e = 0.;
      t_taus_m1_match_.pi_bestmatch_dr = 0.;
      t_taus_m1_match_.p1_bestmatch_dr = 0.;
      t_taus_m1_match_.p2_bestmatch_dr = 0.;
      t_taus_m1_match_.pi_bestmatch_hfrac = 0.;
      t_taus_m1_match_.p1_bestmatch_hfrac = 0.;
      t_taus_m1_match_.p2_bestmatch_hfrac = 0.;
      if (tinfo.hadronic_mode == 1) {
        // Build a list of the sim photons that match those in the tau collection
        std::vector<SimParticle *> tau_sim_pions;
        std::vector<SimParticle *> tau_sim_photons;
        ROOT::Math::PtEtaPhiEVector all_gen_prongs;
        for (auto sim : simparts) {
          if ((std::abs(sim->pid()) == 22 || std::abs(sim->pid()) == 11) && sim->gen() != -1) {
            for (auto gen : tinfo.all_vis) {
              if (sim->gen() == gen->index()) {
                tau_sim_photons.push_back(sim);
                all_gen_prongs += sim->vector();
              }
            }
          }
          if ((std::abs(sim->pid()) == 211 || std::abs(sim->pid()) == 321) &&
          sim->gen() != -1) {
            for (auto gen : tinfo.all_vis) {
              if (sim->gen() == gen->index()) {
                tau_sim_pions.push_back(sim);
                all_gen_prongs += sim->vector();
              }
            }
          }
        }

        t_taus_m1_gen_.mass = all_gen_prongs.M();

        std::sort(tau_sim_photons.begin(), tau_sim_photons.end(),
                  [](SimParticle* s1, SimParticle* s2) {
                    return s1->energy() > s2->energy();
                  });

        if (tau_sim_pions.size() >= 1) {
          t_taus_m1_gen_.pi_reached_ee = (tau_sim_pions[0]->reached_ee() == 2);
          t_taus_m1_gen_.pi_e = tau_sim_pions[0]->energy();
        }
        if (tau_sim_photons.size() >= 1) {
          t_taus_m1_gen_.p1_reached_ee = (tau_sim_photons[0]->reached_ee() == 2);
          t_taus_m1_gen_.p1_e = tau_sim_photons[0]->energy();

        }
        if (tau_sim_photons.size() >= 2) {
          t_taus_m1_gen_.p2_reached_ee = (tau_sim_photons[1]->reached_ee() == 2);
          t_taus_m1_gen_.p2_e = tau_sim_photons[1]->energy();
        }
        if (tau_sim_photons.size() != 2) {
          t_taus_m1_gen_.has_gen_ele_pair = true;
        }

        t_taus_m1_gen_.all_reached_ee =
            (t_taus_m1_gen_.pi_reached_ee && t_taus_m1_gen_.p1_reached_ee &&
             t_taus_m1_gen_.p2_reached_ee);
        if (t_taus_m1_gen_.all_reached_ee) {
          double dx = tau_sim_photons[0]->layer_positions()[0].x() -
                      tau_sim_photons[1]->layer_positions()[0].x();
          double dy = tau_sim_photons[0]->layer_positions()[0].y() -
                      tau_sim_photons[1]->layer_positions()[0].y();
          t_taus_m1_gen_.p1_p2_dxy = std::sqrt(dx * dx + dy * dy);
        }

        if (tauinfos.size() >= 1) {
          t_taus_rec_.jet_pt = tauinfos[0]->jet.pt();
          t_taus_rec_.jet_eta = tauinfos[0]->jet.eta();
          t_taus_rec_.jet_phi = tauinfos[0]->jet.phi();
          t_taus_rec_.jet_e = tauinfos[0]->jet.energy();
          t_taus_rec_.nprongs = tauinfos[0]->prongs.size();
          t_taus_rec_.jet_mass = tauinfos[0]->jet.M();

          ROOT::Math::PtEtaPhiEVector all_prongs;
          for (auto const& prong : tauinfos[0]->prongs) all_prongs += prong.vector();

          t_taus_rec_.all_prong_mass = all_prongs.M();



          t_taus_m1_match_.matched = true;

          std::vector<SimParticle *> gen_prongs;
          std::vector<SimParticle *> gen_prongs_unmodified;
          std::vector<SimParticle> modified_pions;
          for (auto gen : tau_sim_pions) {
            modified_pions.push_back(*gen);
            if (gen->layer_positions().size() > 0) {
              modified_pions.back().set_eta(gen->layer_positions()[0].eta());
              modified_pions.back().set_phi(gen->layer_positions()[0].phi());
            }
          }
          for (auto & gen : modified_pions) gen_prongs.push_back(&gen);
          for (auto & gen : tau_sim_pions) gen_prongs_unmodified.push_back(gen);
          for (auto gen : tau_sim_photons) {
            gen_prongs.push_back(gen);
            gen_prongs_unmodified.push_back(gen);
          }

          std::vector<ProngCandidate const*> rec_prongs;
          for (auto & rec : tauinfos[0]->prongs) {
            rec_prongs.push_back(&rec);
          }
          auto matches = MatchByDR(gen_prongs, rec_prongs, 0.2, false, true);
          auto matches_simclusters = MatchByDR(gen_prongs_unmodified, simclusters, 0.2, true, true);

          std::map<Candidate *, std::vector<ProngCandidate const*>> match_map;
          for (auto match : matches) {
            match_map[match.first].push_back(match.second);
          }

          std::map<Candidate *, SimCluster*> match_map_simclusters;
          for (auto match : matches_simclusters) {
            match_map_simclusters[match.first] = match.second;
          }


          for (auto & match : match_map) {
            double true_e = match.first->energy();
            std::sort(match.second.begin(), match.second.end(), [&](ProngCandidate const* c1, ProngCandidate const* c2) {
              double diff1 = std::fabs(c1->energy() - true_e);
              double diff2 = std::fabs(c2->energy() - true_e);
              return diff1 < diff2;
            });
          }

          if (modified_pions.size() >= 1) {
            t_taus_m1_match_.nmatched_to_pi = match_map[&modified_pions[0]].size();
            if (t_taus_m1_match_.nmatched_to_pi >= 1) {
              auto bestmatch = match_map[&modified_pions[0]][0];
              t_taus_m1_match_.pi_bestmatch_e = bestmatch->energy();
              t_taus_m1_match_.pi_bestmatch_dr = DR(&modified_pions[0], bestmatch);
              t_taus_m1_match_.pi_bestmatch_hfrac = bestmatch->h_energy() / bestmatch->energy();
            }
            if (match_map_simclusters.count(tau_sim_pions[0])) {
              auto simcluster = match_map_simclusters[tau_sim_pions[0]];
              std::cout << ">> Simcluster rechits\n";
              for (unsigned h = 0; h < simcluster->hits().size(); ++h) {
                std::cout << ">>>> " << simcluster->hits()[h] << "\t" << simcluster->fractions()[h] << "\n";
                if (rechit_id_map.count(simcluster->hits()[h])) {
                  rechit_id_map[simcluster->hits()[h]]->Print();
                }
              }
            }
          }
          if (tau_sim_photons.size() >= 1) {
            t_taus_m1_match_.nmatched_to_p1 = match_map[tau_sim_photons[0]].size();
            if (t_taus_m1_match_.nmatched_to_p1 >= 1) {
              auto bestmatch = match_map[tau_sim_photons[0]][0];
              t_taus_m1_match_.p1_bestmatch_e = bestmatch->energy();
              t_taus_m1_match_.p1_bestmatch_dr = DR(tau_sim_photons[0], bestmatch);
              t_taus_m1_match_.p1_bestmatch_hfrac = bestmatch->h_energy() / bestmatch->energy();
            }

          }
          if (tau_sim_photons.size() >= 2) {
            t_taus_m1_match_.nmatched_to_p2 = match_map[tau_sim_photons[1]].size();
            if (t_taus_m1_match_.nmatched_to_p2 >= 1) {
              auto bestmatch = match_map[tau_sim_photons[1]][0];
              t_taus_m1_match_.p2_bestmatch_e = bestmatch->energy();
              t_taus_m1_match_.p2_bestmatch_dr = DR(tau_sim_photons[1], bestmatch);
              t_taus_m1_match_.p2_bestmatch_hfrac = bestmatch->h_energy() / bestmatch->energy();
            }
          }
        }
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
  return 0;
}

int HGCALTest::PostAnalysis() { return 0; }

}

