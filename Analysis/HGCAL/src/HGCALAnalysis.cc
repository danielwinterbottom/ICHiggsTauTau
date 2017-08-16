#include <algorithm>
#include <map>
#include <functional>
#include "TMath.h"
#include "boost/functional/hash.hpp"
#include "boost/lexical_cast.hpp"
#include "RooRealVar.h"
#include "HGCAL/interface/HGCALAnalysis.h"
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

  return 0;
}

int HGCALTest::Execute(TreeEvent* event) {
  auto const& rechits = BuildRecHitCollection(event);
  auto const& genParticles = BuildGenParticleCollection(event);
  auto genParts_final = ic::copy_keep_if(genParticles, [](ic::GenParticle *p) {
    return p->status() == 1;
  });

  auto genParts_finalNoNu = ic::copy_keep_if(genParts_final, [](ic::GenParticle *p) {
    unsigned pdgid = std::abs(p->pdgid());
    return pdgid != 12 && pdgid != 14 && pdgid != 16;
  });

  // int long evt = *event->GetPtr<int long>("event");
  auto filtered_rechits = ic::copy_keep_if(rechits, [](RecHit const* r) {
    return std::get<1>(RecHitAboveThreshold(*r, 3., true));
  });
  auto const& simclusters = BuildSimClusterCollection(event);
  ROOT::Math::PtEtaPhiEVector sc_sum_p;
  ROOT::Math::PtEtaPhiEVector sc_sum_m;
  ROOT::Math::PtEtaPhiEVector rh_sum_p;
  ROOT::Math::PtEtaPhiEVector rh_sum_m;
  ROOT::Math::PtEtaPhiEVector rhf_sum_p;
  ROOT::Math::PtEtaPhiEVector rhf_sum_m;

  std::vector<fastjet::PseudoJet> particles;


   // print the jets
   // cout <<   "        pt y phi" << endl;
   // for (unsigned i = 0; i < jets.size(); i++) {
   //   cout << "jet " << i << ": "<< jets[i].pt() << " " 
   //                  << jets[i].rap() << " " << jets[i].phi() << endl;
   //   vector<PseudoJet> constituents = jets[i].constituents();
   //   for (unsigned j = 0; j < constituents.size(); j++) {
   //     cout << "    constituent " << j << "'s pt: " << constituents[j].pt()
   //          << endl;
   //   }
   // }


  for (unsigned i = 0; i < rechits.size(); ++i) {
    if (rechits[i]->eta() > 0.) {
      rh_sum_p += rechits[i]->vector();
    } else {
      rh_sum_m += rechits[i]->vector();
    }
  }
  for (unsigned i = 0; i < filtered_rechits.size(); ++i) {
    particles.push_back(fastjet::PseudoJet(
        filtered_rechits[i]->vector().px(),
        filtered_rechits[i]->vector().py(),
        filtered_rechits[i]->vector().pz(),
        filtered_rechits[i]->vector().energy()));

    if (filtered_rechits[i]->eta() > 0.) {
      rhf_sum_p += filtered_rechits[i]->vector();
    } else {
      rhf_sum_m += filtered_rechits[i]->vector();
    }
  }
  for (unsigned i = 0; i < simclusters.size(); ++i) {
    if (simclusters[i]->eta() > 0.) {
      sc_sum_p += simclusters[i]->vector();
    } else {
      sc_sum_m += simclusters[i]->vector();
    }
  }


  // choose a jet definition
  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, 0.4);
  auto jets = ClusterJets(filtered_rechits, jet_def);
  std::vector<ic::CompositeCandidate *> jets_p;
  for (auto & j : jets) jets_p.push_back(&j);

  auto genJets = ClusterJets(genParts_final, jet_def);
  std::vector<ic::CompositeCandidate *> genJets_p;
  for (auto & j : genJets) genJets_p.push_back(&j);

  auto genJets_nonu = ClusterJets(genParts_finalNoNu, jet_def);
  std::vector<ic::CompositeCandidate *> genJets_nonu_p;
  for (auto & j : genJets_nonu) genJets_nonu_p.push_back(&j);

  // std::cout << "Simcluster sums:\n";
  // std::cout << sc_sum_p << "\n";
  // std::cout << sc_sum_m << "\n";
  // std::cout << "RecHit sums:\n";
  // std::cout << rh_sum_p << "\n";
  // std::cout << rh_sum_m << "\n";
  // std::cout << "RecHit (filtered) sums:\n";
  // std::cout << rhf_sum_p << "\n";
  // std::cout << rhf_sum_m << "\n";

  // std::cout << "Highest pT jets:\n";
  unsigned print = std::min(unsigned(jets.size()), unsigned(5));
  for (unsigned i = 0; i < print; i++) {
    // std::cout << tmpjet << "\n";
    if (i < 2) {
      jet_pt_ = jets[i].pt();
      jet_eta_ = jets[i].eta();
      jet_phi_ = jets[i].phi();
      jet_e_ = jets[i].energy();
      auto matches = ic::MatchByDR(jets_p, genJets_p, 0.4, true, true);
      std::map<CompositeCandidate*, CompositeCandidate*> matchMap;
      for (auto const& pair : matches) matchMap[pair.first] = pair.second;
      if (matchMap.count(jets_p[i])) {
        gen_matched_ = true;
        genjet_pt_ = matchMap[jets_p[i]]->pt();
        genjet_eta_ = matchMap[jets_p[i]]->eta();
        genjet_phi_ = matchMap[jets_p[i]]->phi();
        genjet_e_ = matchMap[jets_p[i]]->energy();
      } else {
        gen_matched_ = false;
        genjet_pt_ = -1.;
        genjet_eta_ = -1.;
        genjet_phi_ = -1.;
        genjet_e_ = -1.;
      }

      auto matches_nonu = ic::MatchByDR(jets_p, genJets_nonu_p, 0.4, true, true);
      std::map<CompositeCandidate*, CompositeCandidate*> matchMap_nonu;
      for (auto const& pair : matches_nonu) matchMap_nonu[pair.first] = pair.second;
      if (matchMap_nonu.count(jets_p[i])) {
        gen_nonu_matched_ = true;
        genjet_nonu_pt_ = matchMap_nonu[jets_p[i]]->pt();
        genjet_nonu_eta_ = matchMap_nonu[jets_p[i]]->eta();
        genjet_nonu_phi_ = matchMap_nonu[jets_p[i]]->phi();
        genjet_nonu_e_ = matchMap_nonu[jets_p[i]]->energy();
      } else {
        gen_nonu_matched_ = false;
        genjet_nonu_pt_ = -1.;
        genjet_nonu_eta_ = -1.;
        genjet_nonu_phi_ = -1.;
        genjet_nonu_e_ = -1.;
      }

      t_jets_->Fill();
    }
    // std::cout << "jet " << i << ": "<< jets[i].pt() << " "
    //                << jets[i].eta() << " " << jets[i].phi() << std::endl;
  }

  // print = std::min(unsigned(genJets.size()), unsigned(5));
  // for (unsigned i = 0; i < print; i++) {
  //   std::cout << "genjet " << i << ": "<< genJets[i].pt() << " "
  //                  << genJets[i].eta() << " " << genJets[i].phi() << std::endl;
  // }



  // auto p_rechits = ic::copy_keep_if(filtered_rechits, [](RecHit const* r) {
  //   return r->eta() > 0;
  // });
  // auto m_rechits = ic::copy_keep_if(filtered_rechits, [](RecHit const* r) {
  //   return !(r->eta() > 0);
  // });

  // auto p_graphs = PlotRecHitsInLayers(p_rechits, "p_rechits");
  // auto m_graphs = PlotRecHitsInLayers(m_rechits, "m_rechits");
  // // fs

  // auto subdir = fs_->mkdir(TString::Format("event_%li", evt).Data());
  // for (auto const& gr : p_graphs) {
  //   subdir.make<TGraph2D>(gr);
  // }
  // for (auto const& gr : m_graphs) {
  //   subdir.make<TGraph2D>(gr);
  // }
  return 0;
}

int HGCALTest::PostAnalysis() { return 0; }


std::vector<TGraph2D> PlotRecHitsInLayers(std::vector<RecHit *> const &rechits,
                                          TString name_prefix) {
  std::vector<TGraph2D> res;
  unsigned nlayers = hgcal::lastLayer - hgcal::firstLayer + 1;
  std::vector<std::vector<float>> x(nlayers);
  std::vector<std::vector<float>> y(nlayers);
  std::vector<std::vector<float>> e(nlayers);
  for (auto const& rh : rechits) {
    unsigned layer_idx = rh->layer() - hgcal::firstLayer;
    x[layer_idx].push_back(rh->position().phi());
    y[layer_idx].push_back(rh->position().eta());
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
    auto const* gen_pdgid = event->GetPtr<std::vector<int>>("gen_pdgid");
    auto const* gen_status = event->GetPtr<std::vector<int>>("gen_status");

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
      dest.set_pt((*gen_pt)[i]);
      dest.set_eta((*gen_eta)[i]);
      dest.set_phi((*gen_phi)[i]);
      dest.set_energy((*gen_e)[i]);
      dest.set_pdgid((*gen_pdgid)[i]);
      dest.set_status((*gen_status)[i]);
      dest.set_id(hasher(&dest));
      gens[i] = &dest;
    }
    return gens;
  }
}
