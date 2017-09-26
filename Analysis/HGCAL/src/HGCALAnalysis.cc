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
  return 0;
}

int HGCALTest::Execute(TreeEvent* event) {
  int long evt = *event->GetPtr<int long>("event");
  auto const& rechits = BuildRecHitCollection(event);
  auto const& genParticles = BuildGenParticleCollection(event);
  auto const& simParticles = BuildSimParticleCollection(event);

  // for (unsigned i = 0; i < genParticles.size(); ++i) {
  //   genParticles[i]->Print();
  // }
  auto genParts_final = ic::copy_keep_if(genParticles, [](ic::GenParticle *p) {
    return p->status() == 1;
  });

  auto genParts_finalNoNu = ic::copy_keep_if(genParts_final, [](ic::GenParticle *p) {
    unsigned pdgid = std::abs(p->pdgid());
    return pdgid != 12 && pdgid != 14 && pdgid != 16;
  });

  auto genParts_taus = ic::copy_keep_if(genParticles, [](ic::GenParticle *p) {
    return std::abs(p->pdgid()) == 15;
  });

  std::cout << ">> Found " << genParts_taus.size() << " taus in event " << evt << "\n";
  std::vector<ic::TauInfo> genInfo_taus;
  for (auto const& tau : genParts_taus) {
    genInfo_taus.push_back(HTTGenEvent::BuildTauInfo(tau, genParticles, true));
    std::cout << "************* Tau Info *************\n";
    genInfo_taus.back().Print();
    std::cout << "************************************\n";
  }

  for (unsigned i = 0; i < simParticles.size(); ++i) {
    simParticles[i]->Print();
  }


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

  std::string label_p = "";
  std::string label_m = "";

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


  auto matches = ic::MatchByDR(jets_p, genJets_p, 0.4, true, true);
  std::map<CompositeCandidate*, CompositeCandidate*> matchMap;
  for (auto const& pair : matches) matchMap[pair.first] = pair.second;

  auto matches_nonu = ic::MatchByDR(jets_p, genJets_nonu_p, 0.4, true, true);
  std::map<CompositeCandidate*, CompositeCandidate*> matchMap_nonu;
  for (auto const& pair : matches_nonu) matchMap_nonu[pair.first] = pair.second;


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

    // Do the two leading reco jets for now
    if (i >= 2) continue;

    jet_pt_ = jets[i].pt();
    jet_eta_ = jets[i].eta();
    jet_phi_ = jets[i].phi();
    jet_e_ = jets[i].energy();

    // Is the jet matched to a full genjet (neutrinos included)
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

    // Tau DM finding - false by default
    isDM_r0p02_ = false;
    isDM_r0p04_ = false;
    isDM_r0p06_ = false;
    isDM_r0p08_ = false;
    isDM_r0p10_ = false;

    if (matchMap_nonu.count(jets_p[i])) {
      gen_nonu_matched_ = true;
      genjet_nonu_pt_ = matchMap_nonu[jets_p[i]]->pt();
      genjet_nonu_eta_ = matchMap_nonu[jets_p[i]]->eta();
      genjet_nonu_phi_ = matchMap_nonu[jets_p[i]]->phi();
      genjet_nonu_e_ = matchMap_nonu[jets_p[i]]->energy();

      is_hadtau_ = false;
      bool found_emu = false;
      bool tau_matched = false;
      std::vector<CompositeCandidate const*> partv = {matchMap_nonu[jets_p[i]]};
      unsigned ncharged_0p02 = 0;
      unsigned ncharged_0p04 = 0;
      unsigned ncharged_0p06 = 0;
      unsigned ncharged_0p08 = 0;
      unsigned ncharged_0p10 = 0;
      int charge_0p02 = 0;
      int charge_0p04 = 0;
      int charge_0p06 = 0;
      int charge_0p08 = 0;
      int charge_0p10 = 0;

      // Loop through genjet constituents to figure out if this is a hadronic tau, and if it passes DM
      for (unsigned c = 0; c < matchMap_nonu[jets_p[i]]->AsVector().size(); ++c) {
        GenParticle const* part = dynamic_cast<GenParticle const*>(matchMap_nonu[jets_p[i]]->at(c));
        if (std::abs(part->pdgid()) == 11 || std::abs(part->pdgid()) == 13) {
          found_emu = true;
        }
        double dr = DR(jets_p[i], part);
        bool is_charged = (std::abs(part->charge()) == 1);
        if (is_charged && dr < 0.02) ncharged_0p02 += 1;
        if (is_charged && dr < 0.04) ncharged_0p04 += 1;
        if (is_charged && dr < 0.06) ncharged_0p06 += 1;
        if (is_charged && dr < 0.08) ncharged_0p08 += 1;
        if (is_charged && dr < 0.10) ncharged_0p10 += 1;
        if (is_charged && dr < 0.02) charge_0p02 += part->charge();
        if (is_charged && dr < 0.04) charge_0p04 += part->charge();
        if (is_charged && dr < 0.06) charge_0p06 += part->charge();
        if (is_charged && dr < 0.08) charge_0p08 += part->charge();
        if (is_charged && dr < 0.10) charge_0p10 += part->charge();
      }
      if (MatchByDR(partv, genParts_taus, 0.4, true, true).size()) tau_matched = true;
      if (!found_emu && tau_matched) is_hadtau_ = true;
      if ((ncharged_0p02 == 1 || ncharged_0p02 == 3) && std::abs(charge_0p02) == 1) isDM_r0p02_ = true;
      if ((ncharged_0p04 == 1 || ncharged_0p04 == 3) && std::abs(charge_0p04) == 1) isDM_r0p04_ = true;
      if ((ncharged_0p06 == 1 || ncharged_0p06 == 3) && std::abs(charge_0p06) == 1) isDM_r0p06_ = true;
      if ((ncharged_0p08 == 1 || ncharged_0p08 == 3) && std::abs(charge_0p08) == 1) isDM_r0p08_ = true;
      if ((ncharged_0p10 == 1 || ncharged_0p10 == 3) && std::abs(charge_0p10) == 1) isDM_r0p10_ = true;
    } else {
      gen_nonu_matched_ = false;
      genjet_nonu_pt_ = -1.;
      genjet_nonu_eta_ = -1.;
      genjet_nonu_phi_ = -1.;
      genjet_nonu_e_ = -1.;
      is_hadtau_ = false;
    }

    double e_sum_all = 0.;
    double e_sum_0p02 = 0.;
    double e_sum_0p04 = 0.;
    double e_sum_0p06 = 0.;
    double e_sum_0p08 = 0.;
    double e_sum_0p10 = 0.;
    for (unsigned c = 0; c < jets_p[i]->AsVector().size(); ++c) {
      double energy = jets_p[i]->At(c)->energy();
      e_sum_all += energy;
      double dr = DR(jets_p[i], jets_p[i]->At(c));
      if (dr < 0.02) e_sum_0p02 += energy;
      if (dr < 0.04) e_sum_0p04 += energy;
      if (dr < 0.06) e_sum_0p06 += energy;
      if (dr < 0.08) e_sum_0p08 += energy;
      if (dr < 0.10) e_sum_0p10 += energy;
    }
    efrac_r0p02_ = e_sum_0p02 / e_sum_all;
    efrac_r0p04_ = e_sum_0p04 / e_sum_all;
    efrac_r0p06_ = e_sum_0p06 / e_sum_all;
    efrac_r0p08_ = e_sum_0p08 / e_sum_all;
    efrac_r0p10_ = e_sum_0p10 / e_sum_all;

    // Tau reconstruction
    // Start by plotting jet energy fraction contained within a cone of various sizes

    t_jets_->Fill();
    // std::cout << "jet " << i << ": "<< jets[i].pt() << " "
    //                << jets[i].eta() << " " << jets[i].phi() << std::endl;
  }

  // print = std::min(unsigned(genJets.size()), unsigned(5));
  // for (unsigned i = 0; i < print; i++) {
  //   std::cout << "genjet " << i << ": "<< genJets[i].pt() << " "
  //                  << genJets[i].eta() << " " << genJets[i].phi() << std::endl;
  // }


  static int event_counter = 0;

  if (event_counter < 100) {
    auto p_rechits = ic::copy_keep_if(filtered_rechits, [](RecHit const* r) {
      return r->eta() > 0;
    });
    auto m_rechits = ic::copy_keep_if(filtered_rechits, [](RecHit const* r) {
      return !(r->eta() > 0);
    });

    unsigned p_good_pions = 0;
    unsigned m_good_pions = 0;
    unsigned p_good_photons = 0;
    unsigned m_good_photons = 0;

    auto p_graphs = PlotRecHitsInLayers(p_rechits, "p_rechits");
    auto m_graphs = PlotRecHitsInLayers(m_rechits, "m_rechits");

    unsigned nlayers = hgcal::lastLayer - hgcal::firstLayer + 1;
    std::vector<TPolyMarker3D> p_pion_markers(nlayers);
    std::vector<TPolyMarker3D> m_pion_markers(nlayers);
    std::vector<TPolyMarker3D> p_photon_markers(nlayers);
    std::vector<TPolyMarker3D> m_photon_markers(nlayers);
    for (unsigned i = 0; i < nlayers; ++i) {
      p_pion_markers[i].SetName(TString::Format("%s_%i", "p_pions", hgcal::firstLayer+i));
      m_pion_markers[i].SetName(TString::Format("%s_%i", "m_pions", hgcal::firstLayer+i));
      p_photon_markers[i].SetName(TString::Format("%s_%i", "p_photons", hgcal::firstLayer+i));
      m_photon_markers[i].SetName(TString::Format("%s_%i", "m_photons", hgcal::firstLayer+i));
    }
    TString p_infostr;
    TString m_infostr;
    for (unsigned i = 0; i < simParticles.size(); ++i) {
      auto const& sim = *(simParticles[i]);
      if ((std::abs(sim.pid()) == 211 || std::abs(sim.pid()) == 321) && sim.reached_ee() && sim.gen() != -1) {
        std::vector<TPolyMarker3D> & pion_markers = sim.eta() > 0. ? p_pion_markers : m_pion_markers;
        if (sim.eta() > 0.) {
          ++p_good_pions;
          p_infostr += TString::Format("%g:", sim.energy());
        } else {
          ++m_good_pions;
          m_infostr += TString::Format("%g:", sim.energy());

        }
        for (unsigned j = 0; j < sim.layer_positions().size(); ++j) {
          pion_markers[j].SetNextPoint(sim.layer_positions()[j].x(), sim.layer_positions()[j].y(), 0);
        }
      }

      if ((std::abs(sim.pid()) == 22) && sim.reached_ee() && sim.gen() != -1) {
        std::vector<TPolyMarker3D> & photon_markers = sim.eta() > 0. ? p_photon_markers : m_photon_markers;
        if (sim.eta() > 0.) {
          ++p_good_photons;
          p_infostr += TString::Format("%g:", sim.energy());
        } else {
          ++m_good_photons;
          m_infostr += TString::Format("%g:", sim.energy());
        }
        for (unsigned j = 0; j < sim.layer_positions().size(); ++j) {
          photon_markers[j].SetNextPoint(sim.layer_positions()[j].x(), sim.layer_positions()[j].y(), 0);
        }
      }
    }

    for (auto const& tinfo : genInfo_taus) {
      if (tinfo.hadronic_mode >= 0) {
        if (tinfo.tau_st2_post_fsr->eta() >= 0.) {
          p_graphs[0].SetTitle(TString::Format("#tau_{h} mode %i:", tinfo.hadronic_mode) + p_infostr);
          if (tinfo.pi_charged.size() == p_good_pions && (tinfo.pi_neutral.size() == p_good_photons / 2)) {
            std::cout << "GOOD HADRONIC TAU (P): " << tinfo.hadronic_mode << "\n";
          }
        } else {
          m_graphs[0].SetTitle(TString::Format("#tau_{h} mode %i:", tinfo.hadronic_mode) + m_infostr);
          if (tinfo.pi_charged.size() == m_good_pions && (tinfo.pi_neutral.size() == m_good_photons / 2)) {
            std::cout << "GOOD HADRONIC TAU (M): " << tinfo.hadronic_mode << "\n";
          }
        }
      }
    }

    auto subdir = fs_->mkdir(TString::Format("event_%li", evt).Data());
    for (auto const& gr : p_graphs) {
      subdir.make<TGraph2D>(gr);
    }
    for (auto const& gr : m_graphs) {
      subdir.make<TGraph2D>(gr);
    }
    for (auto const& gr : p_pion_markers) {
      subdir.make<TPolyMarker3D>(gr);
    }
    for (auto const& gr : m_pion_markers) {
      subdir.make<TPolyMarker3D>(gr);
    }
    for (auto const& gr : p_photon_markers) {
      subdir.make<TPolyMarker3D>(gr);
    }
    for (auto const& gr : m_photon_markers) {
      subdir.make<TPolyMarker3D>(gr);
    }
  }

  event_counter += 1;
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


}

