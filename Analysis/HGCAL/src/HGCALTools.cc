#include <algorithm>
#include <map>
#include <functional>
#include "TMath.h"
#include "TGraph2D.h"
#include "TPolyMarker3D.h"
#include "boost/functional/hash.hpp"
#include "boost/lexical_cast.hpp"
#include "RooRealVar.h"
#include "HGCAL/interface/HGCALTools.h"
#include "HGCAL/interface/HTTGenEvent.h"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"
#include "Utilities/interface/FnRootTools.h"
#include "fastjet/ClusterSequence.hh"

namespace ic {

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
  auto const* rechit_detid = event->GetPtr<std::vector<unsigned>>("rechit_detid");
  event->Add("rechits_storage", std::vector<RecHit>());
  auto& rechit_store = event->Get<std::vector<RecHit>>("rechits_storage");
  event->Add("rechits", std::vector<RecHit*>());
  auto& rechits = event->Get<std::vector<RecHit*>>("rechits");

  unsigned n = rechit_e->size();
  rechit_store.resize(n);
  rechits.resize(n);

  // std::hash<RecHit*> hasher;

  // int min_layer = 999;
  // int max_layer = 0;

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
    dest.set_id((*rechit_detid)[i]);
    rechits[i] = &dest;
    // if (dest.layer() < min_layer) min_layer = dest.layer();
    // if (dest.layer() > max_layer) max_layer = dest.layer();
  }
  // std::cout << "Layers: " << min_layer << "/" << max_layer << "\n";
  return rechits;
  }

  std::vector<SimCluster *> const& BuildSimClusterCollection(TreeEvent *event) {
    auto const* simcluster_pt = event->GetPtr<std::vector<float>>("simcluster_pt");
    auto const* simcluster_eta = event->GetPtr<std::vector<float>>("simcluster_eta");
    auto const* simcluster_phi = event->GetPtr<std::vector<float>>("simcluster_phi");
    auto const* simcluster_e = event->GetPtr<std::vector<float>>("simcluster_energy");
    auto const* simcluster_hits = event->GetPtr< std::vector<std::vector<uint32_t>>>("simcluster_hits");
    auto const* simcluster_fractions = event->GetPtr< std::vector<std::vector<float>>>("simcluster_fractions");

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
      dest.set_hits((*simcluster_hits)[i]);
      dest.set_fractions((*simcluster_fractions)[i]);
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

