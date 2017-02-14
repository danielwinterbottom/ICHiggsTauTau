#include "HiggsTauTau/interface/HTTFutureJetPV.h"

#include <string>
#include <set>
#include <vector>
// #include "boost/format.hpp"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"

namespace ic {

HTTFutureJetPV::HTTFutureJetPV(std::string const &name) : ModuleBase(name) {
  fs_=NULL;
}

HTTFutureJetPV::~HTTFutureJetPV() { ; }

int HTTFutureJetPV::PreAnalysis() {
  if(fs_){
    outtree_ = fs_->make<TTree>("jetpvassoc","jetpvassoc");
    outtree_->Branch("jet_pt",&jet_pt);
    outtree_->Branch("jet_eta",&jet_eta);
    outtree_->Branch("genjet_pt",&genjet_pt);
    outtree_->Branch("genjet_eta",&genjet_eta);
    outtree_->Branch("jet_beta",&jet_beta);
    outtree_->Branch("jet_flav",&jet_flav);
  }
  return 0;
}

int HTTFutureJetPV::PostAnalysis() {
  return 0;
}


int HTTFutureJetPV::Execute(TreeEvent *event) {
  std::vector<GenJet*> const& genjets = event->GetPtrVec<GenJet>(genjet_label_);
  std::vector<PFJet*> const& jets = event->GetPtrVec<PFJet>(jets_label_);

  std::vector<std::pair<PFJet*, GenJet*> > jet_match  = MatchByDR(jets, genjets, 0.2, true, true);
/*  std::vector<std::pair<PFJet*,GenJet*> > sel_genjets;
  for(unsigned i=0;i<jet_match.size();++i){
    if(jet_match.at(i).first->parton_flavour()!=0&&jet_match.at(i).first->parton_flavour()!=21) sel_genjets.push_back(jet_match.at(i));
  }*/
  for(unsigned j=0;j<jet_match.size();++j){
      jet_pt=jet_match.at(j).first->pt();
      jet_eta=jet_match.at(j).first->eta();
      genjet_eta=jet_match.at(j).second->eta();
      genjet_pt=jet_match.at(j).second->pt();
      jet_beta=jet_match.at(j).first->beta();
      jet_flav=jet_match.at(j).first->parton_flavour();
      outtree_->Fill();
  }

  return 0;
}
}
