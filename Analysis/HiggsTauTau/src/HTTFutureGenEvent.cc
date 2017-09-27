#include "HiggsTauTau/interface/HTTFutureGenEvent.h"

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

HTTFutureGenEvent::HTTFutureGenEvent(std::string const &name) : ModuleBase(name) {
  fs_=NULL;
}

HTTFutureGenEvent::~HTTFutureGenEvent() { ; }

int HTTFutureGenEvent::PreAnalysis() {
  if(fs_){
    outtree_ = fs_->make<TTree>("geninfo","geninfo");
    outtree_->Branch("tau1_type",&tau1_type);
    outtree_->Branch("tau1_pt",&tau1_pt);
    outtree_->Branch("tau1_eta",&tau1_eta);
    outtree_->Branch("tau2_type",&tau2_type);
    outtree_->Branch("tau2_pt",&tau2_pt);
    outtree_->Branch("tau2_eta",&tau2_eta);
    outtree_->Branch("jet1_eta",&jet1_eta);
    outtree_->Branch("jet2_eta",&jet2_eta);
    outtree_->Branch("jet1_pt",&jet1_pt);
    outtree_->Branch("jet2_pt",&jet2_pt);
    outtree_->Branch("jet1_flav",&jet1_flav);
    outtree_->Branch("jet2_flav",&jet2_flav);
    outtree_->Branch("vtxdz",&vtxdz);
    outtree_->Branch("vtxdy",&vtxdy);
    outtree_->Branch("vtxdx",&vtxdx);
  }
  return 0;
}

int HTTFutureGenEvent::PostAnalysis() {
  return 0;
}


int HTTFutureGenEvent::Execute(TreeEvent *event) {
   std::vector<ic::Vertex*> vertices = event->GetPtrVec<ic::Vertex>("vertices");
   std::vector<ic::Vertex*> genvertices = event->GetPtrVec<ic::Vertex>("genVertices");
   vtxdz =fabs(vertices.at(0)->vz()-genvertices.at(0)->vz());
   vtxdy =fabs(vertices.at(0)->vy()-genvertices.at(0)->vy());
   vtxdx =fabs(vertices.at(0)->vx()-genvertices.at(0)->vx());

  std::vector<GenParticle *>  const& particles = event->GetPtrVec<GenParticle>(genparticle_label_);
  std::vector<GenParticle *> sel_particles;
  for (unsigned i=0; i < particles.size(); ++i){
    std::vector<bool> status_flags_start = particles[i]->statusFlags();
    if ( ((abs(particles[i]->pdgid()) == 11 )||(abs(particles[i]->pdgid()) == 13 )) && (status_flags_start[IsDirectPromptTauDecayProduct] )){
      sel_particles.push_back(particles[i]);
    }
  }

  std::vector<GenJet> gen_taus = BuildTauJets(particles, false,true);
  std::vector<GenJet *> gen_taus_ptr;
  for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);
  if(sel_particles.size()!=0){
    tau1_type=sel_particles[0]->pdgid();
    tau1_pt=sel_particles[0]->pt();
    tau1_eta=sel_particles[0]->eta();
    if(sel_particles.size()==2){
      tau2_type=sel_particles[1]->pdgid();
      tau2_pt=sel_particles[1]->pt();
      tau2_eta=sel_particles[1]->eta();
    } else {
      tau2_type=gen_taus_ptr[0]->pt();
      tau2_type=gen_taus_ptr[0]->eta();
      tau2_type=15;
    }
  } else {
    tau1_type=15;
    tau2_type=15;
    tau1_pt=gen_taus_ptr[0]->pt();
    tau1_eta=gen_taus_ptr[0]->eta();
    tau2_pt=gen_taus_ptr[1]->pt();
    tau2_eta=gen_taus_ptr[1]->eta();
  }

//  ic::erase_if(gen_taus_ptr, !boost::bind(MinPtMaxEta, _1, 15.0, 999.));
//

  std::vector<GenJet*> const& genjets = event->GetPtrVec<GenJet>(genjet_label_);
  std::vector<PFJet*> const& jets = event->GetPtrVec<PFJet>(jets_label_);
  std::vector<std::pair<PFJet*, GenJet*> > jet_match  = MatchByDR(jets, genjets, 0.2, true, true);
  std::vector<std::pair<PFJet*,GenJet*> > sel_genjets;
  for(unsigned i=0;i<jet_match.size();++i){
    if(jet_match.at(i).first->parton_flavour()!=0&&jet_match.at(i).first->parton_flavour()!=21&&MinDRToCollection(jet_match.at(i).first,gen_taus_ptr,0.6)) sel_genjets.push_back(jet_match.at(i));
  }
  if(sel_genjets.size()==1){
    jet1_pt=sel_genjets.at(0).first->pt(); 
    jet1_eta=sel_genjets.at(0).first->eta(); 
    jet1_flav=sel_genjets.at(0).first->parton_flavour();
  }
  if(sel_genjets.size()>1){
    if(sel_genjets.at(0).first->pt()>sel_genjets.at(1).first->pt()){
      jet1_pt=sel_genjets.at(0).first->pt();
      jet1_eta=sel_genjets.at(0).first->eta();
      jet1_flav=sel_genjets.at(0).first->parton_flavour();
      jet2_pt=sel_genjets.at(1).first->pt();
      jet2_eta=sel_genjets.at(1).first->eta();
      jet2_flav=sel_genjets.at(1).first->parton_flavour();
    }
  }
 

  outtree_->Fill();
  return 0;
}
}
