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
    outtree_->Branch("jet_beta_puppi",&jet_beta_puppi);
    outtree_->Branch("jet_flav",&jet_flav);
    outtree_->Branch("vtxdz",&vtxdz);
    outtree_->Branch("vtxdx",&vtxdx);
    outtree_->Branch("vtxdy",&vtxdy);
    outtree_second_ = fs_->make<TTree>("leadsubleadjet","leadsubleadjet");
    outtree_second_->Branch("jet_pt1",&jet_pt1);
    outtree_second_->Branch("jet_eta1",&jet_eta1);
    outtree_second_->Branch("genjet_pt1",&genjet_pt1);
    outtree_second_->Branch("genjet_eta1",&genjet_eta1);
    outtree_second_->Branch("jet_beta1",&jet_beta1);
    outtree_second_->Branch("jet_flav1",&jet_flav1);
    outtree_second_->Branch("jet_pt2",&jet_pt2);
    outtree_second_->Branch("jet_eta2",&jet_eta2);
    outtree_second_->Branch("genjet_pt2",&genjet_pt2);
    outtree_second_->Branch("genjet_eta2",&genjet_eta2);
    outtree_second_->Branch("jet_beta2",&jet_beta2);
    outtree_second_->Branch("jet_flav2",&jet_flav2);
    outtree_second_->Branch("vbf_dphi",&vbf_dphi);
    outtree_second_->Branch("vtxdz",&vtxdz);
    outtree_second_->Branch("vtxdx",&vtxdx);
    outtree_second_->Branch("vtxdy",&vtxdy);
  }
  return 0;
}

int HTTFutureJetPV::PostAnalysis() {
  return 0;
}


int HTTFutureJetPV::Execute(TreeEvent *event) {
  std::vector<GenJet*> const& genjets = event->GetPtrVec<GenJet>(genjet_label_);
  std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
  std::vector<GenParticle *>  const& particles = event->GetPtrVec<GenParticle>(genparticle_label_);
  std::vector<GenJet> gen_taus = BuildTauJets(particles, false,true);
  std::vector<GenJet *> gen_taus_ptr;
  for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);
  std::vector<GenParticle *> sel_particles;
  for (unsigned i=0; i < particles.size(); ++i){
    std::vector<bool> status_flags_start = particles[i]->statusFlags();
    if ( ((abs(particles[i]->pdgid()) == 11 )||(abs(particles[i]->pdgid()) == 13 )) && (status_flags_start[IsDirectPromptTauDecayProduct] )){
      sel_particles.push_back(particles[i]);
    }
  }

  std::vector<PFJet*> quark_jets;
  for (auto *x: jets){
    if(x->parton_flavour()!=0&&x->parton_flavour()!=21&&x->pt()>20&&MinDRToCollection(x,gen_taus_ptr,0.5)&&MinDRToCollection(x,sel_particles,0.5)){
      quark_jets.push_back(x);
   }
 }

  std::vector<std::pair<PFJet*, GenJet*> > jet_match  = MatchByDR(jets, genjets, 0.1, true, true);
  std::vector<std::pair<PFJet*, GenJet*> > quark_jet_match  = MatchByDR(quark_jets, genjets, 0.1, true, true);
  std::sort(jet_match.begin(),jet_match.end(),SortByJetPt);
  std::sort(quark_jet_match.begin(),quark_jet_match.end(),SortByJetPt);
  std::vector<ic::Vertex*> vertices = event->GetPtrVec<ic::Vertex>("vertices");
  std::vector<ic::Vertex*> genvertices = event->GetPtrVec<ic::Vertex>("genVertices");
  vtxdz =fabs(vertices.at(0)->vz()-genvertices.at(0)->vz());
  vtxdy =fabs(vertices.at(0)->vy()-genvertices.at(0)->vy());
  vtxdx =fabs(vertices.at(0)->vx()-genvertices.at(0)->vx());

  
/*  std::vector<std::pair<PFJet*,GenJet*> > sel_genjets;
  for(unsigned i=0;i<jet_match.size();++i){
    if(jet_match.at(i).first->parton_flavour()!=0&&jet_match.at(i).first->parton_flavour()!=21) sel_genjets.push_back(jet_match.at(i));
  }*/
/*  for(unsigned j=0;j<jet_match.size();++j){
      if(MinDRToCollection(jet_match.at(j).first,gen_taus_ptr,0.5)&&MinDRToCollection(jet_match.at(j).first,sel_particles,0.5)){
        jet_pt=jet_match.at(j).first->pt();
        jet_eta=jet_match.at(j).first->eta();
        genjet_eta=jet_match.at(j).second->eta();
        genjet_pt=jet_match.at(j).second->pt();
        jet_beta=jet_match.at(j).first->beta();
        jet_beta_puppi=jet_match.at(j).first->beta_puppi();
        jet_flav=jet_match.at(j).first->parton_flavour();
        outtree_->Fill();
      }
  }*/

  for(unsigned j=0;j<jets.size();++j){
     std::vector<PFJet*> test_jet;
     test_jet.push_back(jets.at(j));
     std::vector<std::pair<PFJet*, GenJet*> > jet_match = MatchByDR(test_jet,genjets,0.1,true,true);
     if(jet_match.size() <1){
       jet_pt=jets.at(j)->pt();
       jet_eta=jets.at(j)->eta();
       jet_beta=jets.at(j)->beta();
       outtree_->Fill();
     }
   }

      if(quark_jet_match.size()>1){
        jet_pt1=quark_jet_match.at(0).first->pt();
        jet_eta1=quark_jet_match.at(0).first->eta();
        genjet_eta1=quark_jet_match.at(0).second->eta();
        genjet_pt1=quark_jet_match.at(0).second->pt();
        jet_beta1=quark_jet_match.at(0).first->beta();
        jet_flav1=quark_jet_match.at(0).first->parton_flavour();
        jet_pt2=quark_jet_match.at(1).first->pt();
        jet_eta2=quark_jet_match.at(1).first->eta();
        genjet_eta2=quark_jet_match.at(1).second->eta();
        genjet_pt2=quark_jet_match.at(1).second->pt();
        jet_beta2=quark_jet_match.at(1).first->beta();
        jet_flav2=quark_jet_match.at(1).first->parton_flavour();
        vbf_dphi=fabs(ROOT::Math::VectorUtil::DeltaPhi(quark_jet_match.at(0).first->vector(),quark_jet_match.at(1).first->vector()));
        outtree_second_->Fill();
     
  }


  return 0;
}

bool SortByJetPt(std::pair<ic::PFJet*,ic::GenJet*> const c1,
                                       std::pair<ic::PFJet*,ic::GenJet*> const c2) {
           return (c1.first->pt() > c2.first->pt() );
             }
}
