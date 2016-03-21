#include "HiggsTauTau/interface/WJetsStudy.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include <boost/functional/hash.hpp>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"
#include "boost/range/algorithm_ext/erase.hpp"

namespace ic {

WJetsStudy::WJetsStudy(std::string const& name)
    : ModuleBase(name), fs_(nullptr), genparticle_label_("genParticles") {}

WJetsStudy::~WJetsStudy() { ; }

int WJetsStudy::PreAnalysis() {
  if (fs_) {
    tree_ = fs_->make<TTree>("qcdstudy", "qcdstudy");
    lepton_.SetTree(tree_, "", "_1");
    scale_wts_.SetTree(tree_, 1001, 1009, "scale_wt_", "");
    tree_->Branch("mt_1", &mt_1_);
    tree_->Branch("wt", &wt_);
  }
  return 0;
}

int WJetsStudy::Execute(TreeEvent* event) {
  auto muons = event->GetPtrVec<ic::Muon>("muons");
  auto sel_muons = ic::copy_keep_if(muons, [](ic::Muon *m) {
    return
      m->pt()                 > 19.0    &&
      fabs(m->eta())          < 2.1     &&
      fabs(m->dxy_vertex())   < 0.045   &&
      fabs(m->dz_vertex())    < 0.2     &&
      MuonMedium(m);
  });

  if (sel_muons.size() == 0) return 1;

  auto pfmet = event->GetPtrVec<ic::Met>("pfMet");
  // auto genparticles = event->GetPtrVec<ic::GenParticle>(genparticle_label_);

  // auto gen_muon = ic::copy_keep_if(genparticles, [](ic::GenParticle* p) {
  //   return (std::abs(p->pdgid()) == 13) &&
  //          p->statusFlags()[ic::FromHardProcess] && p->status() == 1;
  // });
  // auto gen_nu = ic::copy_keep_if(genparticles, [](ic::GenParticle* p) {
  //   return (std::abs(p->pdgid()) == 14) &&
  //          p->statusFlags()[ic::FromHardProcess] && p->status() == 1;
  // });

  // if (gen_muon.size() == 0) return 1;
  // if (gen_muon.size() > 1) {
  //   std::cout << "More than one muon!\n";
  //   return 1;
  // }
  // if (gen_nu.size() != 1) {
  //   std::cout << "No muon neutrino!\n";
  //   return 1;
  // }

  lepton_.SetVals(sel_muons[0]);
  mt_1_ = ic::MT(sel_muons[0], pfmet[0]);

  auto info = event->GetPtr<ic::EventInfo>("eventInfo");
  wt_ = info->total_weight();

  scale_wts_.SetVals(info);
  /*
  0 muR=1.0 muF=1.0 <-- just the nominal
  1 muR=1.0 muF=2.0
  2 muR=1.0 muF=0.5
  3 muR=2.0 muF=1.0
  4 muR=2.0 muF=2.0
  5 muR=2.0 muF=0.5 <-- not this one
  6 muR=0.5 muF=1.0
  7 muR=0.5 muF=2.0 <-- not this one
  8 muR=0.5 muF=0.5
  */

  tree_->Fill();
  return 0;
}
int WJetsStudy::PostAnalysis() {
  std::cout << "*** " << this->ModuleName() << "\n";
  return 0;
}

void WJetsStudy::PrintInfo() {}


}
