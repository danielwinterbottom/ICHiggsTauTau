#include "HiggsTauTau/interface/HTTFutureTauEff.h"

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

HTTFutureTauEff::HTTFutureTauEff(std::string const &name) : ModuleBase(name) {
  fs_=NULL;
}

HTTFutureTauEff::~HTTFutureTauEff() { ; }

int HTTFutureTauEff::PreAnalysis() {
  if(fs_){
    outtree_ = fs_->make<TTree>("tauidinfo","tauidinfo");
    outtree_->Branch("tau_pt",&tau_pt);
    outtree_->Branch("tau_eta",&tau_eta);
    outtree_->Branch("tau_dm_finding",&tau_dm_finding);
    outtree_->Branch("tau_dm",&tau_dm);
    outtree_->Branch("tau_mvaiso_raw",&tau_mvaiso_raw);
    outtree_->Branch("tau_combiso",&tau_combiso);
  }
  return 0;
}

int HTTFutureTauEff::PostAnalysis() {
  return 0;
}


int HTTFutureTauEff::Execute(TreeEvent *event) {
  std::vector<GenParticle *>  const& particles = event->GetPtrVec<GenParticle>(genparticle_label_);
  std::vector<GenJet> gen_taus = BuildTauJets(particles, false,true);
  std::vector<GenJet *> gen_taus_ptr;
  for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);
  std::vector<Tau *> const& taus = event->GetPtrVec<Tau>(taus_label_); 
  ic::erase_if(gen_taus_ptr, !boost::bind(MinPtMaxEta, _1, 20.0, 999.));
  std::vector<std::pair<Tau*, GenJet*> > tau_match  = MatchByDR(taus, gen_taus_ptr, 0.2, true, true);
  
  for( unsigned i=0; i<tau_match.size();++i){
    tau_pt = tau_match.at(i).first->pt(); 
    tau_eta = tau_match.at(i).first->eta(); 
    tau_dm_finding = tau_match.at(i).first->GetTauID("decayModeFinding"); 
    tau_dm = tau_match.at(i).first->decay_mode();
    tau_mvaiso_raw = tau_match.at(i).first->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw");
    tau_combiso = tau_match.at(i).first->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    outtree_->Fill();
  }
   return 0;
}
}
