#include "HiggsTauTau/interface/HTTFutureTauResolution.h"

#include <string>
#include <set>
#include <vector>
// #include "boost/format.hpp"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"

namespace ic {

HTTFutureTauResolution::HTTFutureTauResolution(std::string const &name) : ModuleBase(name) {
  fs_=NULL;
}

HTTFutureTauResolution::~HTTFutureTauResolution() { ; }

int HTTFutureTauResolution::PreAnalysis() {
  if(fs_){
    outtree_ = fs_->make<TTree>("tauidinfo","tauidinfo");
    outtree_->Branch("tau_resolution",&tau_resolution);
    outtree_->Branch("tau_dm",&tau_dm);
  }
  return 0;
}

int HTTFutureTauResolution::PostAnalysis() {
  return 0;
}


int HTTFutureTauResolution::Execute(TreeEvent *event) {
  std::vector<GenParticle *>  const& particles = event->GetPtrVec<GenParticle>(genparticle_label_);
  std::vector<GenJet> gen_taus = BuildTauJets(particles, false,true);
  std::vector<GenJet *> gen_taus_ptr;
  for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);
  std::vector<Tau *> const& taus = event->GetPtrVec<Tau>(taus_label_); 
  ic::erase_if(gen_taus_ptr, !boost::bind(MinPtMaxEta, _1, 20.0, 999.));
  std::vector<std::pair<Tau*, GenJet*> > tau_match  = MatchByDR(taus, gen_taus_ptr, 0.2, true, true);
  
  for( unsigned i=0; i<tau_match.size();++i){
    if(tau_match.at(i).first->GetTauID("decayModeFinding")&&std::abs(tau_match.at(i).first->eta()<2.3)){
      tau_resolution = tau_match.at(i).first->pt()-tau_match.at(i).second->pt(); 
      tau_dm = tau_match.at(i).first->decay_mode();
      outtree_->Fill();
    }
  }
   return 0;
}
}
