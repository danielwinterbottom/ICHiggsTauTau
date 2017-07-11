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

namespace ic {
HGCALTest::HGCALTest(std::string const& name) : ModuleBase(name) {}

HGCALTest::~HGCALTest() { ; }

int HGCALTest::PreAnalysis() { return 0; }

int HGCALTest::Execute(TreeEvent* event) {
  auto const& rechits = BuildRecHitCollection(event);
  auto const& simclusters = BuildSimClusterCollection(event);
  ROOT::Math::PtEtaPhiEVector sc_sum_p;
  ROOT::Math::PtEtaPhiEVector sc_sum_m;
  ROOT::Math::PtEtaPhiEVector rh_sum_p;
  ROOT::Math::PtEtaPhiEVector rh_sum_m;
  for (unsigned i = 0; i < rechits.size(); ++i) {
    if (rechits[i]->eta() > 0.) {
      rh_sum_p += rechits[i]->vector();
    } else {
      rh_sum_m += rechits[i]->vector();
    }
  }
  for (unsigned i = 0; i < simclusters.size(); ++i) {
    if (simclusters[i]->eta() > 0.) {
      sc_sum_p += simclusters[i]->vector();
    } else {
      sc_sum_m += simclusters[i]->vector();
    }
  }
  std::cout << "Simcluster sums:\n";
  std::cout << sc_sum_p << "\n";
  std::cout << sc_sum_m << "\n";
  std::cout << "RecHit sums:\n";
  std::cout << rh_sum_p << "\n";
  std::cout << rh_sum_m << "\n";
  return 0;
}

int HGCALTest::PostAnalysis() { return 0; }

std::vector<RecHit*> const& BuildRecHitCollection(TreeEvent* event) {
  auto const* rechit_pt = event->GetPtr<std::vector<float>>("rechit_pt");
  auto const* rechit_eta = event->GetPtr<std::vector<float>>("rechit_eta");
  auto const* rechit_phi = event->GetPtr<std::vector<float>>("rechit_phi");
  auto const* rechit_e = event->GetPtr<std::vector<float>>("rechit_energy");

  event->Add("rechits_storage", std::vector<RecHit>());
  auto& rechit_store = event->Get<std::vector<RecHit>>("rechits_storage");
  event->Add("rechits", std::vector<RecHit*>());
  auto& rechits = event->Get<std::vector<RecHit*>>("rechits");

  unsigned n = rechit_e->size();
  rechit_store.resize(n);
  rechits.resize(n);

  std::hash<RecHit*> hasher;

  for (unsigned i = 0; i < n; ++i) {
    ic::RecHit& dest = rechit_store[i];
    dest.set_pt((*rechit_pt)[i]);
    dest.set_eta((*rechit_eta)[i]);
    dest.set_phi((*rechit_phi)[i]);
    dest.set_energy((*rechit_e)[i]);
    dest.set_id(hasher(&dest));
    rechits[i] = &dest;
  }
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

}
