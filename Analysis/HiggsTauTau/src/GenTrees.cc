#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/GenTrees.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

#include "TMVA/Reader.h"
#include "TVector3.h"
#include "boost/format.hpp"
#include "TMath.h"
#include "TLorentzVector.h"


namespace ic {

  GenTrees::GenTrees(std::string const& name) : ModuleBase(name), 
      channel_(channel::et){ 
      fs_ = NULL; }

  GenTrees::~GenTrees() {
    ;
  }

  int GenTrees::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "GenTrees" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    

    if (fs_ ) {
      outtree_ = fs_->make<TTree>("ntuple","ntuple");
      outtree_->Branch("event",             &event_);
      outtree_->Branch("wt",                &wt_);
      outtree_->Branch("wt_sign",                &wt_sign_);
      outtree_->Branch("zptgen",               &zptgen_);
      outtree_->Branch("zmassgen",               &zmassgen_);
      outtree_->Branch("zptlhe",               &zptlhe_);
      outtree_->Branch("zmasslhe",               &zmasslhe_);
      outtree_->Branch("vispt",  &vispT_);
      outtree_->Branch("genpt",  &genpT_);
      outtree_->Branch("visM",  &visM_);
      outtree_->Branch("genM",  &genM_);
      outtree_->Branch("npnlo", &npnlo_);
      outtree_->Branch("pdgid_1",   &pdgid_1_);
      outtree_->Branch("pdgid_2",   &pdgid_2_);
      outtree_->Branch("pdgid_gen_1",   &pdgid_gen_1_);
      outtree_->Branch("pdgid_gen_2",   &pdgid_gen_2_);
      outtree_->Branch("nlhepartons", &nlhepartons_);
      outtree_->Branch("ngenjets", &ngenjets_);
      outtree_->Branch("gen_jet_pt_1", &gen_jet_pt_1_);
      outtree_->Branch("gen_jet_eta_1", &gen_jet_eta_1_);
      outtree_->Branch("gen_jet_pt_2", &gen_jet_pt_2_);
      outtree_->Branch("gen_jet_eta_2", &gen_jet_eta_2_);
      outtree_->Branch("pt_1", &pt_1_);
      outtree_->Branch("eta_1", &eta_1_);
      outtree_->Branch("pt_2", &pt_2_);
      outtree_->Branch("eta_2", &eta_2_);
      outtree_->Branch("lhe_pt_1", &lhe_pt_1_);
      outtree_->Branch("lhe_eta_1", &lhe_eta_1_);
      outtree_->Branch("lhe_pt_2", &lhe_pt_2_);
      outtree_->Branch("lhe_eta_2", &lhe_eta_2_);
      outtree_->Branch("m_bb",&m_bb_);
      outtree_->Branch("m_jj",&m_jj_);
    }
   return 0;
  }


  int GenTrees::Execute(TreeEvent *event) {

/*    std::vector<GenParticle *> const& gen_particles = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<GenParticle *> sel_gen_particles;
    for (unsigned i=0; i < gen_particles.size(); ++i){
      std::vector<bool> status_flags_start = gen_particles[i]->statusFlags();
    //  std::cout<<status_flags_start[IsPrompt]<<std::endl;
      if ( ((abs(gen_particles[i]->pdgid()) == 11 )||(abs(gen_particles[i]->pdgid()) == 13 )) && status_flags_start[IsLastCopy] && (status_flags_start[IsPrompt] || status_flags_start[IsDirectPromptTauDecayProduct] )){
        sel_gen_particles.push_back(gen_particles[i]);
      }
    } 

    std::vector<GenJet> gen_taus = BuildTauJets(gen_particles, false,true);
    std::vector<GenJet *> gen_taus_ptr;
    for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);

    if(sel_gen_particles.size()==2){
       zmassgen_ = (sel_gen_particles[0]->vector()+sel_gen_particles[1]->vector()).M();
       zptgen_ = (sel_gen_particles[0]->vector()+sel_gen_particles[1]->vector()).pt();
       pdgid_gen_1_ = sel_gen_particles[0]->pdgid();
       pt_1_ = sel_gen_particles[0]->pt();
       eta_1_ = sel_gen_particles[0]->eta();
       pdgid_gen_2_ = sel_gen_particles[1]->pdgid();
       pt_2_ = sel_gen_particles[1]->pt();
       eta_2_ = sel_gen_particles[1]->eta();
    } else if(sel_gen_particles.size()==1 && gen_taus_ptr.size()==1){
       zmassgen_ = (sel_gen_particles[0]->vector()+gen_taus_ptr[0]->vector()).M();
       zptgen_ = (sel_gen_particles[0]->vector()+gen_taus_ptr[0]->vector()).pt();
       pdgid_gen_1_ = sel_gen_particles[0]->pdgid();
       pt_1_ = sel_gen_particles[0]->pt();
       eta_1_ = sel_gen_particles[0]->eta();
       pdgid_gen_2_ = 15;
       pt_2_ = gen_taus_ptr[0]->pt();
       eta_2_ = gen_taus_ptr[0]->eta();
    } else if (gen_taus_ptr.size()==2){
       zmassgen_ = (gen_taus_ptr[0]->vector()+gen_taus_ptr[1]->vector()).M();
       zptgen_ = (gen_taus_ptr[0]->vector()+gen_taus_ptr[1]->vector()).pt();
       pdgid_gen_1_=15;
       pt_1_ = gen_taus_ptr[0]->pt();
       eta_1_ = gen_taus_ptr[0]->eta();
       pdgid_gen_2_=15;
       pt_2_ = gen_taus_ptr[1]->pt();
       eta_2_ = gen_taus_ptr[1]->eta();
    } */

    std::vector<GenParticle *> parts = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<GenParticle *> sel_gen_parts;
    std::vector<GenParticle *> sel_gen_parts_nu;
    std::vector<GenParticle *> sel_vis_parts;

/*   std::cout<<"New event"<<std::endl;
   for (unsigned i=0; i<parts.size();++i){
      std::vector<bool> status_flags = parts[i]->statusFlags();
      unsigned status = abs(parts[i]->status());
      unsigned id = abs(parts[i]->pdgid());
      if ( (id ==12 ||id==14||id==16) && status==1 &&status_flags[IsPrompt]){
         std::cout<<"Neutrino ID "<<id<<", pT = ," <<parts[i]->pt()<<std::endl;
      }
   }*/
    
    for(unsigned i = 0; i < parts.size(); ++i){
      std::vector<bool> status_flags = parts[i]->statusFlags();
      unsigned id = abs(parts[i]->pdgid());
      unsigned status = abs(parts[i]->status());
      if (id>=11 && id <=16 && status_flags[IsHardProcess] ) { 
          sel_gen_parts.push_back(parts[i]);
      }

      //Modification for Z->ll only
      //if ( (id >= 11 && id <= 16 && status_flags[FromHardProcess] && status==1) || status_flags[IsDirectHardProcessTauDecayProduct]) sel_gen_parts.push_back(parts[i]);
   /*   if ( ( (id == 11 || id == 13) && status_flags[IsPrompt] && status==1) || (status_flags[IsDecayedLeptonHadron] && status_flags[IsPrompt] && id == 15)) sel_gen_parts.push_back(parts[i]);
   std::sort(sel_gen_parts.begin(),sel_gen_parts.end(),bind(&Candidate::pt,_1) > bind(&Candidate::pt, _2));*/
      //Mods for W->ln only
    /*  if( ( (id == 11 || 13) && status_flags[IsPrompt] && status==1) || (status_flags[IsDecayedLeptonHadron] && status_flags[IsPrompt] && id==15)) sel_gen_parts.push_back(parts[i]);
      if ( ( (id ==12||id==14||id==16) && status_flags[FromHardProcess] && status==1)) sel_gen_parts_nu.push_back(parts[i]);
      std::sort(sel_gen_parts.begin(),sel_gen_parts.end(),bind(&Candidate::pt,_1) > bind(&Candidate::pt, _2));
      std::sort(sel_gen_parts_nu.begin(),sel_gen_parts_nu.end(),bind(&Candidate::pt,_1) > bind(&Candidate::pt, _2));*/
      if ( ( (id == 11 || id == 13 || id == 15) && status_flags[FromHardProcess] && status==1) || (status_flags[IsDirectHardProcessTauDecayProduct] && !(id==12||id==14||id==16))) sel_vis_parts.push_back(parts[i]);
   }

   
  ROOT::Math::PtEtaPhiEVector gen_boson;
  ROOT::Math::PtEtaPhiEVector vis_boson;
   //Mods for z->ll
/*   for( unsigned i = 0; i < 2 ; ++i){
     gen_boson += sel_gen_parts[i]->vector();
   }*/
   //Mods for W->ln
/*    gen_boson += sel_gen_parts[0]->vector();
    gen_boson += sel_gen_parts_nu[0]->vector(); */

   for( unsigned i = 0; i < sel_gen_parts.size() ; ++i){
     gen_boson += sel_gen_parts[i]->vector();
   }



   for( unsigned i = 0; i < sel_vis_parts.size() ; ++i){
     vis_boson += sel_vis_parts[i]->vector();
   }

    genpT_ = gen_boson.pt();
    genM_ = gen_boson.M();

    vispT_ = vis_boson.pt();
    visM_ = vis_boson.M();


    std::vector<GenParticle *> const& lhe_particles = event->GetPtrVec<GenParticle>("lheParticles");
    std::vector<GenParticle *> sel_lhe_particles;
    std::vector<GenParticle *> sel_lhe_partons;
    for (unsigned i=0; i < lhe_particles.size(); ++i){
       if(lhe_particles[i]->status() != 1) continue;
       unsigned id = abs(lhe_particles[i]->pdgid());
       if( id >= 11 && id <= 16){
       //if( id == 11 || id == 13 || id == 15){
        sel_lhe_particles.push_back(lhe_particles[i]);
      } else if ((id>=1 && id <=6) || id==21) {
        sel_lhe_partons.push_back(lhe_particles[i]);
      }
    }

   if(sel_lhe_particles.size()!=2) std::cout<<"More than 2 leptons!" <<std::endl;
   lhe_pt_1_ = sel_lhe_particles[0]->pt();
   lhe_pt_2_ = sel_lhe_particles[1]->pt();
   lhe_eta_1_ = sel_lhe_particles[0]->eta();
   lhe_eta_2_ = sel_lhe_particles[1]->eta();
   pdgid_1_ = sel_lhe_particles[0]->pdgid();
   pdgid_2_ = sel_lhe_particles[1]->pdgid();
   zmasslhe_ = (sel_lhe_particles[0]->vector()+sel_lhe_particles[1]->vector()).M();
   zptlhe_ = (sel_lhe_particles[0]->vector()+sel_lhe_particles[1]->vector()).pt();
   std::cout<<"ZPTLHE " <<zptlhe_<<std::endl;
   nlhepartons_ = sel_lhe_partons.size();

   std::vector<GenJet *>  gen_jets = event->GetPtrVec<GenJet>("genJets");
   ic::erase_if(gen_jets,!boost::bind(MinPtMaxEta,_1,20.0,4.7));
   std::vector<GenJet *> gen_bjets;
   for (unsigned i=0;i<gen_jets.size();++i){
     if(abs(gen_jets[i]->flavour())==5) gen_bjets.push_back(gen_jets[i]);
   }
   std::sort(gen_jets.begin(),gen_jets.end(),bind(&Candidate::pt,_1) > bind(&Candidate::pt, _2));
   std::sort(gen_bjets.begin(),gen_bjets.end(),bind(&Candidate::pt,_1) > bind(&Candidate::pt, _2));
   ngenjets_ = gen_jets.size();
   gen_jet_pt_1_=-999;
   gen_jet_pt_2_=-999;
   gen_jet_eta_1_=-999;
   gen_jet_eta_2_=-999;
   if (ngenjets_ >0 ){
     gen_jet_pt_1_ = gen_jets[0]->pt();
     gen_jet_eta_1_ = gen_jets[0]->eta();
     if (ngenjets_ >1){
       gen_jet_pt_2_ = gen_jets[1]->pt();
       gen_jet_eta_2_ = gen_jets[1]->eta();
     }
   }
   m_jj_ = -1;
   m_bb_ = -1;
   if(ngenjets_ > 1) m_jj_ = (gen_jets[0]->vector()+gen_jets[1]->vector()).M();
   if(gen_bjets.size() > 1) m_bb_ = (gen_bjets[0]->vector()+gen_bjets[1]->vector()).M();
   

   EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
   event_ = (unsigned long long) eventInfo->event();
   wt_ = eventInfo->weight("wt_mc");
   if(wt_ > 0) wt_sign_ = 1;
   else wt_sign_ = -1;
   npnlo_ = eventInfo->npnlo();

   outtree_->Fill();
   return 0;
   }

  int GenTrees::PostAnalysis() {
    return 0;
  }

  void GenTrees::PrintInfo() {
    ;
  }
}



