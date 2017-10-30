#include <algorithm>
#include <map>
#include <functional>
#include "TMath.h"
#include "TGraph2D.h"
#include "TPolyMarker3D.h"
#include "boost/functional/hash.hpp"
#include "boost/lexical_cast.hpp"
#include "RooRealVar.h"
#include "HGCAL/interface/TauReco.h"
#include "HGCAL/interface/HTTGenEvent.h"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"
#include "Utilities/interface/FnRootTools.h"
#include "fastjet/ClusterSequence.hh"
#include "HGCAL/interface/HGCALTools.h"

/*
  Taking all pion directions as known...
  insert as prong directions

*/
namespace ic {
TauReco::TauReco(std::string const& name) : ModuleBase(name) {
   s1_rechit_threshold_ = 5.;
   s1_pu_strategy_ = 0;
   s2_jet_distance_ = 0.2;
   s2_min_jet_pt_ = 15.;
   s3_min_surrounding_hits_ = 5;
   s3_min_lower_energy_hits_ = 5;
   s3_use_hcal_dxy_ = true;
   s3_hcal_dxy_ = 10.;
   s4_hit_merge_dr_ = 0.01;
   s4_min_hits_for_prong_ = 3;
   s5_merge_strategy_ = 0;  // 0 = exp(-R/lambda) weighting, 1 = closest in DR
   s5_exp_merge_scale_ = 0.01;
}

TauReco::~TauReco() { ; }

void TauReco::Settings::Print() const {
  std::cout << " - s1_rechit_threshold: " << s1_rechit_threshold << "\n";
  std::cout << " - s1_pu_strategy: " << s1_pu_strategy << "\n";
  std::cout << " - s2_jet_distance: " << s2_jet_distance << "\n";
  std::cout << " - s2_min_jet_pt: " << s2_min_jet_pt << "\n";
  std::cout << " - s3_min_surrounding_hits: " << s3_min_surrounding_hits << "\n";
  std::cout << " - s3_min_lower_energy_hits: " << s3_min_lower_energy_hits << "\n";
  std::cout << " - s3_use_hcal_dxy: " << s3_use_hcal_dxy << "\n";
  std::cout << " - s3_hcal_dxy: " << s3_hcal_dxy << "\n";
  std::cout << " - s3_use_single_hit: " << s3_use_single_hit << "\n";
  std::cout << " - s3_single_hit_min: " << s3_single_hit_min << "\n";
  std::cout << " - s4_hit_merge_dr: " << s4_hit_merge_dr << "\n";
  std::cout << " - s4_min_hits_for_prong: " << s4_min_hits_for_prong << "\n";
  std::cout << " - s5_merge_strategy: " << s5_merge_strategy << "\n";
  std::cout << " - s5_exp_merge_scale: " << s5_exp_merge_scale << "\n";
}


int TauReco::PreAnalysis() {
  settings.s1_rechit_threshold = s1_rechit_threshold_;
  settings.s1_pu_strategy = s1_pu_strategy_;
  settings.s2_jet_distance = s2_jet_distance_;
  settings.s2_min_jet_pt = s2_min_jet_pt_;
  settings.s3_min_surrounding_hits = s3_min_surrounding_hits_;
  settings.s3_min_lower_energy_hits = s3_min_lower_energy_hits_;
  settings.s3_use_hcal_dxy = s3_use_hcal_dxy_;
  settings.s3_hcal_dxy = s3_hcal_dxy_;
  settings.s3_use_single_hit = s3_use_single_hit_;
  settings.s3_single_hit_min = s3_single_hit_min_;
  settings.s4_hit_merge_dr = s4_hit_merge_dr_;
  settings.s4_min_hits_for_prong = s4_min_hits_for_prong_;
  settings.s5_merge_strategy = s5_merge_strategy_;
  settings.s5_exp_merge_scale = s5_exp_merge_scale_;
  PrintHeader(this->ModuleName());
  settings.Print();


  unsigned nlayers = hgcal::lastLayer - hgcal::firstLayer + 1;
  TH1::AddDirectory(false);
  for (unsigned i = 0; i < nlayers; ++i) {
    if (i + hgcal::firstLayer <= hgcal::lastLayerE) {
      pu_bins.push_back(TH1F(TString::Format("bins%i", i), "", 10, 1.479, 3.0));
      pu_profiles.push_back(fs_->make<TH2F>(TString::Format("pu_profile_%i", i), "", 10, 1.479, 3.0, 100, 0, 0.2));
      // pu_profiles.push_back(TH2F(TString::Format("pu_profile_%i", i), "", 10, 1.479, 3.0, 100, 0, 0.2));
    } else {
      pu_bins.push_back(TH1F(TString::Format("bins%i", i), "", 2, std::vector<double>{1.479, 2.6, 3.0}.data()));
      pu_profiles.push_back(fs_->make<TH2F>(TString::Format("pu_profile_%i", i), "", 2, std::vector<double>{1.479, 2.6, 3.0}.data(), 100, 0, 0.4));
    }
  }
  return 0;
}

void TauReco::RunStep3(TauInfo & cand) {
  unsigned nlayers = hgcal::lastLayer - hgcal::firstLayer + 1;

  cand.hits_in_layers = RecHitPtrVecVec(nlayers);

  cand.selected_hit_patterns_in_layers = std::vector<RecHitPtrVecVec>(nlayers);
  cand.all_selected_central_hits = RecHitPtrVec();


  auto components = cand.jet.AsVector();

  // Arrange the components into layers
  for (unsigned j = 0; j < components.size(); ++j) {
    RecHit *rh = dynamic_cast<RecHit*>(components[j]);
    unsigned layer_idx = rh->layer() - hgcal::firstLayer;
    if (layer_idx < nlayers) {
      cand.hits_in_layers[layer_idx].push_back(rh);
    }
  }

  for (unsigned l = 0; l < cand.hits_in_layers.size(); ++l) {
    // if (nverb) std::cout << "> Jet " << i << " " << selected_jets[i]->vector() << ", layer = " << l << ", hits = " << jet_hits_in_layers[i][l].size() << "\n";
    auto const& hits = cand.hits_in_layers[l];

    // Estimate the inter-rechit spacing
    double spacing = 1.3;
    if (!settings.s3_use_hcal_dxy && hits.size() && hits[0]->thickness() > 400)
      spacing = settings.s3_hcal_dxy;
    if (settings.s3_use_hcal_dxy && hits.size() && hits[0]->layer() > hgcal::lastLayerE)
      spacing = settings.s3_hcal_dxy;
    auto matches = MatchByDxy(hits, hits, spacing, false, false);
    std::map<RecHit *, std::set<RecHit *>> rh_map;
    for (auto const& pair : matches) {
      if (pair.first == pair.second) continue;
      rh_map[pair.first].insert(pair.second);
    }
    std::set<RecHit *> all_hits_in_selected_patterns;
    for (auto const& neighbours : rh_map) {
      // if (nverb) std::cout << neighbours.first->position() << ": ";
      // if (nverb) std::cout << neighbours.second.size();

      int n_tot = neighbours.second.size();
      int n_low = 0;
      for (auto const& rh : neighbours.second) {
        if (rh->energy() <= neighbours.first->energy()) ++n_low;
        // if (nverb) std::cout << Dxy(neighbours.first, rh) << " ";
      }
      // if (nverb) std::cout << n_low << "/" << n_tot;
      // if (nverb) std::cout << "\n";
      if (n_tot >= settings.s3_min_surrounding_hits && n_low >= settings.s3_min_lower_energy_hits && n_tot == n_low) {
        std::vector<RecHit*> cluster;
        cluster.push_back(neighbours.first);
        all_hits_in_selected_patterns.insert(neighbours.first);
        for (auto rh : neighbours.second) cluster.push_back(rh);
        for (auto rh : neighbours.second) all_hits_in_selected_patterns.insert(rh);
        cand.selected_hit_patterns_in_layers[l].push_back(cluster);
        cand.all_selected_central_hits.push_back(neighbours.first);
      }
    }
    if (settings.s3_use_single_hit) {
      for (auto rh : hits) {
        if (!all_hits_in_selected_patterns.count(rh) && rh->energy() > settings.s3_single_hit_min) {
          std::vector<RecHit*> cluster;
          cluster.push_back(rh);
          cand.selected_hit_patterns_in_layers[l].push_back(cluster);
          cand.all_selected_central_hits.push_back(rh);
        }
      }
    }
  }
}

void TauReco::RunStep4(TauInfo & cand) {
  std::set<RecHit *> rh_set(cand.all_selected_central_hits.begin(), cand.all_selected_central_hits.end());
  cand.merged_central_hits = RecHitPtrVecVec();
  cand.prongs = std::vector<ProngCandidate>();
  while (rh_set.size() > 0) {
    auto rh = *(rh_set.begin());
    std::set<RecHit*> result;
    RecursiveMatchByDR(rh, std::vector<RecHit*>(rh_set.begin(), rh_set.end()), result, settings.s4_hit_merge_dr);
    for (auto rh_result : result) {
      rh_set.erase(rh_result);
    }
    if (result.size() >= settings.s4_min_hits_for_prong) {
      cand.merged_central_hits.push_back(std::vector<RecHit*>(result.begin(), result.end()));
      // take the simple average for now
      double eta_sum = 0;
      double phi_sum = 0;
      for (auto rh : cand.merged_central_hits.back()) {
        eta_sum += rh->eta();
        phi_sum += rh->phi();
      }
      cand.prongs.push_back(ProngCandidate());
      cand.prongs.back().set_eta(eta_sum / cand.merged_central_hits.back().size());
      cand.prongs.back().set_phi(phi_sum / cand.merged_central_hits.back().size());
    }
  }
}

void TauReco::RunStep5(TauInfo & cand) {
  auto jet_rhs = cand.jet.AsVector();
  for (unsigned r = 0; r < jet_rhs.size(); ++r) {
    std::vector<double> weights(cand.prongs.size());
    double weights_sum = 0.;
    unsigned min_dr_idx = 0;
    double min_dr = 999999.;
    for (unsigned c = 0; c < cand.prongs.size(); ++c) {
      weights[c] = 0.;
      double dr = DR(jet_rhs[r], &cand.prongs[c]);
      if (dr < min_dr) {
        min_dr = dr;
        min_dr_idx = c;
      }
      if (settings.s5_merge_strategy == 0) {
        weights[c] = std::exp(-1. * DR(jet_rhs[r], &cand.prongs[c]) / settings.s5_exp_merge_scale);
        weights_sum += weights[c];
      }
    }
    if (settings.s5_merge_strategy == 1 && cand.prongs.size() > 0) {
      weights[min_dr_idx] = 1.;
      weights_sum = 1.;
    }
    for (unsigned c = 0; c < cand.prongs.size(); ++c) {
      weights[c] /= weights_sum;
      double e_to_add = jet_rhs[r]->energy() * weights[c];
      cand.prongs[c].set_energy(cand.prongs[c].energy() + e_to_add);
      if (dynamic_cast<RecHit*>(jet_rhs[r])->layer() <= hgcal::lastLayerE) {
        cand.prongs[c].set_e_energy(cand.prongs[c].e_energy() + e_to_add);
      } else {
        cand.prongs[c].set_h_energy(cand.prongs[c].h_energy() + e_to_add);
      }
      cand.prongs[c].set_weighted_z(cand.prongs[c].weighted_z() +  dynamic_cast<RecHit*>(jet_rhs[r])->z() * e_to_add);
    }
  }
  for (unsigned c = 0; c < cand.prongs.size(); ++c) {
    cand.prongs[c].set_weighted_z(cand.prongs[c].weighted_z() / cand.prongs[c].energy());
    cand.prongs[c].set_pt(1);
    cand.prongs[c].set_pt(cand.prongs[c].energy() * std::sin(cand.prongs[c].vector().theta()));
  }
}

void TauReco::TauInfo::Print() const {
  using std::cout;
  cout << "-----------------------------\n";
  cout << "TauInfo\n";
  cout << "-----------------------------\n";
  cout << "> Jet: " << jet.vector() << ", nConstituents: " << jet.AsVector().size() << "\n";
  // cout << " > Merged central hits:\n";
  // for (unsigned i = 0; i < merged_central_hits.size(); ++i) {
  //   cout << ">> " << i << ": ";
  //   for (unsigned j = 0; j < merged_central_hits[i].size(); ++j) {
  //     cout << "[" << merged_central_hits[i][j]->vector() << ", " << merged_central_hits[i][j]->position() << "] ";
  //   }
  //   cout << "\n";
  // }
  cout << " > Prongs:\n";
  for (unsigned i = 0; i < prongs.size(); ++i) {
    std::cout << ">> " << i << ": " << prongs[i].vector()
              << ", e: " << prongs[i].e_energy()
              << ", h: " << prongs[i].h_energy()
              << ", z: " << prongs[i].weighted_z() << "\n";
  }
}


int TauReco::Execute(TreeEvent* event) {
  unsigned v = 0;
  int long evt = *event->GetPtr<int long>("event");

  // Start by building object collections from flat trees
  auto const& rechits = event->GetPtrVec<RecHit>("rechits");


  // Step 1 - rechit selection
  // -------------------------------------------------------------------------
  // Noise filtering for RecHits
  auto filtered_rechits = ic::copy_keep_if(rechits, [&](RecHit const* r) {
    return std::get<1>(RecHitAboveThreshold(*r, settings.s1_rechit_threshold, true));
  });

  unsigned nlayers = hgcal::lastLayer - hgcal::firstLayer + 1;
  // PU subtraction
  if (settings.s1_pu_strategy == 1) {
    // std::cout << "RecHits before: " << filtered_rechits.size() << "\n";
    std::vector<std::vector<std::vector<RecHit *>>> hits_in_layers(nlayers);
    std::vector<std::vector<double>> median_energy(nlayers);
    for (unsigned l = 0; l < nlayers; ++l) {
      hits_in_layers[l].resize(pu_bins[l].GetNbinsX());
      median_energy[l].resize(pu_bins[l].GetNbinsX());
    }
    for (unsigned i = 0; i < filtered_rechits.size(); ++i) {
      if (filtered_rechits[i]->layer() > hgcal::lastLayer) continue;
      unsigned layer_idx = filtered_rechits[i]->layer() - hgcal::firstLayer;

      int mynbins = pu_bins[layer_idx].GetNbinsX();
      int eta_idx = pu_bins[layer_idx].FindFixBin(std::abs(filtered_rechits[i]->eta())) - 1;
      if (layer_idx < nlayers && eta_idx >= 0 && eta_idx < mynbins) {
        hits_in_layers[layer_idx][eta_idx].push_back(filtered_rechits[i]);
      }
    }
    for (unsigned l = 0; l < nlayers; ++l) {
      for (unsigned b = 0; b < hits_in_layers[l].size(); ++b){
        std::sort(hits_in_layers[l][b].begin(), hits_in_layers[l][b].end(), [](RecHit *h1, RecHit *h2) {
          return h1->energy() < h2->energy();
        });
        if (hits_in_layers[l][b].size()) {
          median_energy[l][b] = hits_in_layers[l][b][unsigned(hits_in_layers[l][b].size()/2)]->energy();
        } else {
          median_energy[l][b] = 0.;
        }
        pu_profiles[l]->Fill(pu_profiles[l]->GetXaxis()->GetBinCenter(b+1), median_energy[l][b]);
      }
      // std::cout << "Median energy in layer " << l << " = ";
      for (unsigned b = 0; b < hits_in_layers[l].size(); ++b) {
        // std::cout << median_energy[l][b] << " (" << hits_in_layers[l][b].size() << ") ";
      }
      // std::cout << "\n";
    }
    filtered_rechits = ic::copy_keep_if(filtered_rechits, [&](RecHit * r) {
      if (r->layer() > hgcal::lastLayer) return true;
      unsigned lidx = r->layer() - hgcal::firstLayer;
      int mynbins = pu_bins[lidx].GetNbinsX();
      int eta_idx = pu_bins[lidx].FindFixBin(std::abs(r->eta())) - 1;
      if (lidx < nlayers && eta_idx >= 0 && eta_idx < mynbins) {
        double scale = std::max(r->energy() - median_energy[lidx][eta_idx], 0.) / r->energy();
        r->set_vector(r->vector() * scale);
      }
      return (r->energy() > 1E-6);
    });
    // std::cout << "RecHits after: " << filtered_rechits.size() << "\n";

  }
  // Step 2 - jet clustering and selection
  // -------------------------------------------------------------------------
  // Jets clustering rechits in all layers
  fastjet::JetDefinition fj_jet_def(fastjet::antikt_algorithm,
                                    settings.s2_jet_distance);
  auto jets = ClusterJets(filtered_rechits, fj_jet_def);

  std::vector<TauInfo> tau_infos;
  for (auto& j : jets) {
    if (j.pt() > settings.s2_min_jet_pt) {
      tau_infos.push_back(TauInfo());
      tau_infos.back().jet = j;
      tau_infos.back().used_settings = settings;
    }
  }

  // Step 3 - hit pattern selection
  // -------------------------------------------------------------------------
  for (auto & tau_info : tau_infos) {
    RunStep3(tau_info);
  }

  // Step 4 - hit merging
  // -------------------------------------------------------------------------
  for (auto & tau_info : tau_infos) {
    RunStep4(tau_info);
  }

  // Step 5 - energy assignment to merged hits
  // -------------------------------------------------------------------------
  for (auto & tau_info : tau_infos) {
    RunStep5(tau_info);
  }

  //******************** Printing
  if (v > 0) {
    std::cout << "******** Event = " << evt << "\n";
    for (auto& info : tau_infos) {
      info.Print();
    }
  }

  event->Add("tauInfos", tau_infos);

  return 0;
}

int TauReco::PostAnalysis() { return 0; }

}

