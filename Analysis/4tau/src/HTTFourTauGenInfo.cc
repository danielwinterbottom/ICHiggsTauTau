#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTFourTauGenInfo.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <boost/functional/hash.hpp>
#include "boost/algorithm/string.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTConfig.h"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"



namespace ic {


  HTTFourTauGenInfo::HTTFourTauGenInfo(std::string const& name) : ModuleBase(name), channel_(channel::mt) {
    ditau_label_ = "ditau";
    ngenjets_=false;
  }

  HTTFourTauGenInfo::~HTTFourTauGenInfo() {
    ;
  }

  int HTTFourTauGenInfo::PreAnalysis() {
    return 0;
  }

  int HTTFourTauGenInfo::Execute(TreeEvent *event) {


    std::vector<CompositeCandidate *> const& fourtau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
    CompositeCandidate const* fourtau = fourtau_vec.at(0);

    std::vector<Candidate *> lepton1;
    std::vector<Candidate *> lepton2;
    std::vector<Candidate *> lepton3;
    std::vector<Candidate *> lepton4;
    lepton1.push_back(fourtau->GetCandidate("lepton1"));
    lepton2.push_back(fourtau->GetCandidate("lepton2")); 
    lepton3.push_back(fourtau->GetCandidate("lepton3"));
    lepton4.push_back(fourtau->GetCandidate("lepton4"));

    std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<GenParticle *> sel_particles;
    std::vector<GenParticle *> sel_particles_noptcut;
    std::vector<GenParticle *> undecayed_taus;
   
    double gen_match_undecayed_1_pt = -1;
    double gen_match_undecayed_2_pt = -1;
    double gen_match_undecayed_3_pt = -1;
    double gen_match_undecayed_4_pt = -1;
    double gen_match_undecayed_1_eta = -1;
    double gen_match_undecayed_2_eta = -1; 
    double gen_match_undecayed_3_eta = -1;
    double gen_match_undecayed_4_eta = -1;
    double gen_met=0.;
    GenParticle *gen_match_undecayed_1_;
    GenParticle *gen_match_undecayed_2_;
    GenParticle *gen_match_undecayed_3_;
    GenParticle *gen_match_undecayed_4_;


    CompositeCandidate *diZ = new CompositeCandidate();
    std::vector<int> Z_decays_pdgid;

    ROOT::Math::PtEtaPhiEVector neutrinos; 
 
    for (unsigned i=0; i < particles.size(); ++i){
      std::vector<bool> status_flags_start = particles[i]->statusFlags();
      if ( ((abs(particles[i]->pdgid()) == 11 )||(abs(particles[i]->pdgid()) == 13 /*&& particles[i]->status()==1*/)) && particles[i]->pt() > 8. && (status_flags_start[IsPrompt] || status_flags_start[IsDirectPromptTauDecayProduct] /*|| status_flags_start[IsDirectHadronDecayProduct]*/)){
        sel_particles.push_back(particles[i]);
      }
      if ( ((abs(particles[i]->pdgid()) == 11 )||(abs(particles[i]->pdgid()) == 13)) && (status_flags_start[IsPrompt] || status_flags_start[IsDirectPromptTauDecayProduct])){
        sel_particles_noptcut.push_back(particles[i]);
      }
      if ( ((abs(particles[i]->pdgid()) == 12 )||(abs(particles[i]->pdgid()) == 14 /*&& particles[i]->status()==1*/)||(abs(particles[i]->pdgid()) == 16)) && particles[i]->pt() > 8. && (status_flags_start[IsPrompt] || status_flags_start[IsDirectPromptTauDecayProduct] /*|| status_flags_start[IsDirectHadronDecayProduct]*/)) neutrinos+=particles[i]->vector();
      if(status_flags_start[IsPrompt] && status_flags_start[IsLastCopy] && abs(particles[i]->pdgid()) == 15) undecayed_taus.push_back(particles[i]);

      // Get parent bosons per tau
      if(status_flags_start[IsFirstCopy] && abs(particles[i]->pdgid()) == 15) {
        for (unsigned j=1; j<5; ++j){
          if (DR(fourtau->GetCandidate("lepton"+std::to_string(j)),particles[i])< 0.05 && !event->Exists("pdgid_mother_"+std::to_string(j))) {
            event->Add("pdgid_mother_"+std::to_string(j),particles[particles[i]->mothers()[0]]->pdgid()); 
            //std::cout << particles[particles[i]->mothers()[0]]->pdgid() << " " << particles[particles[i]->mothers()[0]]->vector().M() << std::endl;
          }
        }
      }

      // Check if parent is a Z and add it
      int j1 = 1;
      if(status_flags_start[IsFirstCopy] && particles[i]->pdgid() == 23 && j1<3) { 
        diZ->AddCandidate("Z_"+std::to_string(i),particles[i]);
        j1 = j1 + 1;
      }
      // Find the decay products of the Z
      int j2 = 1;
      if(status_flags_start[IsLastCopy] && particles[i]->pdgid() == 23 && j2<3) {
        Z_decays_pdgid.push_back(abs(particles[particles[i]->daughters()[0]]->pdgid()));
        j2 = j2 + 1;
      }
    }

    //std::cout << "--------------------------------------------" << std::endl;

    if (Z_decays_pdgid.size() == 2) {
      event->Add("diZ_pt",diZ->pt());
      event->Add("diZ_mass",diZ->M());
      event->Add("diZ_same_decay",Z_decays_pdgid[0] == Z_decays_pdgid[1]);
    }

    gen_met=neutrinos.Pt();

    event->Add("gen_met",gen_met);
    Met const* mets = NULL;
    mets = event->GetPtr<Met>("pfMET");
    double fake_met = (mets->vector() - neutrinos).Pt();
    event->Add("fake_met",fake_met);

    if(undecayed_taus.size()>0){
      gen_match_undecayed_1_pt = undecayed_taus[0]->pt();
      gen_match_undecayed_1_eta = undecayed_taus[0]->eta();
      gen_match_undecayed_1_=undecayed_taus[0];
      event->Add("gen_match_undecayed_1", gen_match_undecayed_1_);
    }
    if(undecayed_taus.size()>1){
      gen_match_undecayed_2_pt = undecayed_taus[1]->pt();
      gen_match_undecayed_2_eta = undecayed_taus[1]->eta();
      gen_match_undecayed_2_=undecayed_taus[1];
      event->Add("gen_match_undecayed_2", gen_match_undecayed_2_);
    }
    if(undecayed_taus.size()>2){
      gen_match_undecayed_3_pt = undecayed_taus[2]->pt();
      gen_match_undecayed_3_eta = undecayed_taus[2]->eta();
      gen_match_undecayed_3_=undecayed_taus[2];
      event->Add("gen_match_undecayed_3", gen_match_undecayed_3_);
    }
    if(undecayed_taus.size()>3){
      gen_match_undecayed_4_pt = undecayed_taus[3]->pt();
      gen_match_undecayed_4_eta = undecayed_taus[3]->eta();
      gen_match_undecayed_4_=undecayed_taus[3];
      event->Add("gen_match_undecayed_4", gen_match_undecayed_4_);
    }

    int tauFlag1 = 0;
    int tauFlag2 = 0;
    int tauFlag3 = 0;
    int tauFlag4 = 0;

    std::vector<GenJet> gen_taus = BuildTauJets(particles, false,true);
    std::vector<GenJet *> gen_taus_ptr;
    std::vector<GenJet *> gen_taus_ptr_noptcut;
    for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);
    for (auto & x : gen_taus) gen_taus_ptr_noptcut.push_back(&x);
    ic::erase_if(gen_taus_ptr, !boost::bind(MinPtMaxEta, _1, 15.0, 999.));
 
 
    std::vector<std::pair<Candidate*, GenParticle*> > lepton_match_1 = MatchByDR(lepton1, sel_particles, 0.2, true, true);
    std::vector<std::pair<Candidate*, GenJet*> > tau_match_1  = MatchByDR(lepton1, gen_taus_ptr, 0.2, true, true);

    std::vector<std::pair<Candidate*, GenParticle*> > lepton_match_2 = MatchByDR(lepton2, sel_particles, 0.2, true, true);
    std::vector<std::pair<Candidate*, GenJet*> > tau_match_2  = MatchByDR(lepton2, gen_taus_ptr, 0.2, true, true);

    std::vector<std::pair<Candidate*, GenParticle*> > lepton_match_3 = MatchByDR(lepton3, sel_particles, 0.2, true, true);
    std::vector<std::pair<Candidate*, GenJet*> > tau_match_3  = MatchByDR(lepton3, gen_taus_ptr, 0.2, true, true);

    std::vector<std::pair<Candidate*, GenParticle*> > lepton_match_4 = MatchByDR(lepton4, sel_particles, 0.2, true, true);
    std::vector<std::pair<Candidate*, GenJet*> > tau_match_4  = MatchByDR(lepton4, gen_taus_ptr, 0.2, true, true);

   double gen_nu_p_1=-9999.;
   double gen_nu_p_2=-9999.;
   double gen_nu_p_3=-9999.;
   double gen_nu_p_4=-9999.;

   double gen_nu_phi_1=-9999.;
   double gen_nu_phi_2=-9999.;
   double gen_nu_phi_3=-9999.;
   double gen_nu_phi_4=-9999.;

   double gen_nu_eta_1=-9999.;
   double gen_nu_eta_2=-9999.;    
   double gen_nu_eta_3=-9999.;
   double gen_nu_eta_4=-9999.;

   double gen_vis_p_1=-9999.;
   double gen_vis_p_2=-9999.;
   double gen_vis_p_3=-9999.;
   double gen_vis_p_4=-9999.;

   double gen_vis_phi_1=-9999.;
   double gen_vis_phi_2=-9999.;
   double gen_vis_phi_3=-9999.;
   double gen_vis_phi_4=-9999.;

   double gen_vis_eta_1=-9999.;
   double gen_vis_eta_2=-9999.;
   double gen_vis_eta_3=-9999.;
   double gen_vis_eta_4=-9999.;

   double gen_vis_E_1=-9999.;
   double gen_vis_E_2=-9999.;
   double gen_vis_E_3=-9999.;
   double gen_vis_E_4=-9999.;


   mcorigin gen_match_1 = mcorigin::fake;
   mcorigin gen_match_2 = mcorigin::fake;
   mcorigin gen_match_3 = mcorigin::fake;
   mcorigin gen_match_4 = mcorigin::fake;

   int leptonsize_1 = lepton_match_1.size();
   int tausize_1 = tau_match_1.size();
   int leptonsize_2 = lepton_match_2.size();
   int tausize_2 = tau_match_2.size();
   int leptonsize_3 = lepton_match_3.size();
   int tausize_3 = tau_match_3.size();
   int leptonsize_4 = lepton_match_4.size();
   int tausize_4 = tau_match_4.size();

   double gen_match_1_pt = -1;
   double gen_match_2_pt = -1;
   double gen_match_3_pt = -1;
   double gen_match_4_pt = -1;

   // LEPTON1
   if(leptonsize_1!=0&&tausize_1!=0){
     DR(lepton_match_1.at(0).first,lepton_match_1.at(0).second) < DR(tau_match_1.at(0).first,tau_match_1.at(0).second) ? tausize_1=0 : leptonsize_1 = 0;
   }
   if(leptonsize_1==0&&tausize_1==0) gen_match_1 = mcorigin::fake;
   if(leptonsize_1!=0) {
      std::vector<bool> status_flags = lepton_match_1.at(0).second->statusFlags();
      gen_match_1_pt = lepton_match_1.at(0).second->pt();
      if(status_flags[IsPrompt]){
        if(abs(lepton_match_1.at(0).second->pdgid())==11){
          gen_match_1 = mcorigin::promptE;
         } else gen_match_1 = mcorigin::promptMu;
       }
       if(status_flags[IsDirectPromptTauDecayProduct]){
        if(abs(lepton_match_1.at(0).second->pdgid())==11){
          gen_match_1 = mcorigin::tauE;
         } else gen_match_1 = mcorigin::tauMu;
        }
       if(status_flags[IsDirectHadronDecayProduct]){
         if(abs(lepton_match_1.at(0).second->pdgid())==11){
           gen_match_1 = mcorigin::hadE;
          } else gen_match_1 = mcorigin::hadMu;
        } 
      }
     if(tausize_1!=0){
       gen_match_1_pt = tau_match_1.at(0).second->pt();
       gen_match_1 = mcorigin::tauHad;
       tauFlag1 = tau_match_1.at(0).second->flavour();
       gen_nu_p_1=tau_match_1.at(0).second->nu_vector().P();
       gen_nu_phi_1=tau_match_1.at(0).second->nu_vector().Phi();
       gen_nu_eta_1=tau_match_1.at(0).second->nu_vector().Rapidity();

       gen_vis_p_1=tau_match_1.at(0).second->vector().P();
       gen_vis_E_1=tau_match_1.at(0).second->vector().E();
       gen_vis_phi_1=tau_match_1.at(0).second->vector().Phi();
       gen_vis_eta_1=tau_match_1.at(0).second->vector().Rapidity();
      }
   
   // LEPTON2
   if(leptonsize_2!=0&&tausize_2!=0){
     DR(lepton_match_2.at(0).first,lepton_match_2.at(0).second) < DR(tau_match_2.at(0).first,tau_match_2.at(0).second) ? tausize_2=0 : leptonsize_2 = 0;
   }
   if(leptonsize_2==0&&tausize_2==0) gen_match_2 = mcorigin::fake;
   if(leptonsize_2!=0) {
      std::vector<bool> status_flags = lepton_match_2.at(0).second->statusFlags();
      gen_match_2_pt = lepton_match_2.at(0).second->pt();
      if(status_flags[IsPrompt]){
        if(abs(lepton_match_2.at(0).second->pdgid())==11){
          gen_match_2 = mcorigin::promptE;
         } else gen_match_2 = mcorigin::promptMu;
       }
       if(status_flags[IsDirectPromptTauDecayProduct]){
        if(abs(lepton_match_2.at(0).second->pdgid())==11){
          gen_match_2 = mcorigin::tauE;
         } else gen_match_2 = mcorigin::tauMu;
        }
       if(status_flags[IsDirectHadronDecayProduct]){
         if(abs(lepton_match_2.at(0).second->pdgid())==11){
           gen_match_2 = mcorigin::hadE;
          } else gen_match_2 = mcorigin::hadMu;
        }
      }
     if(tausize_2!=0){
       gen_match_2_pt = tau_match_2.at(0).second->pt();
       gen_match_2 = mcorigin::tauHad;
       tauFlag2 = tau_match_2.at(0).second->flavour();
       gen_nu_p_2=tau_match_2.at(0).second->nu_vector().P();
       gen_nu_phi_2=tau_match_2.at(0).second->nu_vector().Phi();
       gen_nu_eta_2=tau_match_2.at(0).second->nu_vector().Rapidity();

       gen_vis_p_2=tau_match_2.at(0).second->vector().P();
       gen_vis_E_2=tau_match_2.at(0).second->vector().E();
       gen_vis_phi_2=tau_match_2.at(0).second->vector().Phi();
       gen_vis_eta_2=tau_match_2.at(0).second->vector().Rapidity();
      }
 
   // LEPTON3
   if(leptonsize_3!=0&&tausize_3!=0){
     DR(lepton_match_3.at(0).first,lepton_match_3.at(0).second) < DR(tau_match_3.at(0).first,tau_match_3.at(0).second) ? tausize_3=0 : leptonsize_3 = 0;
   }
   if(leptonsize_3==0&&tausize_3==0) gen_match_3 = mcorigin::fake;
   if(leptonsize_3!=0) {
      std::vector<bool> status_flags = lepton_match_3.at(0).second->statusFlags();
      gen_match_3_pt = lepton_match_3.at(0).second->pt();
      if(status_flags[IsPrompt]){
        if(abs(lepton_match_3.at(0).second->pdgid())==11){
          gen_match_3 = mcorigin::promptE;
         } else gen_match_3 = mcorigin::promptMu;
       }
       if(status_flags[IsDirectPromptTauDecayProduct]){
        if(abs(lepton_match_3.at(0).second->pdgid())==11){
          gen_match_3 = mcorigin::tauE;
         } else gen_match_3 = mcorigin::tauMu;
        }
       if(status_flags[IsDirectHadronDecayProduct]){
         if(abs(lepton_match_3.at(0).second->pdgid())==11){
           gen_match_3 = mcorigin::hadE;
          } else gen_match_3 = mcorigin::hadMu;
        }
      }
     if(tausize_3!=0){
       gen_match_3_pt = tau_match_3.at(0).second->pt();
       gen_match_3 = mcorigin::tauHad;
       tauFlag3 = tau_match_3.at(0).second->flavour();
       gen_nu_p_3=tau_match_3.at(0).second->nu_vector().P();
       gen_nu_phi_3=tau_match_3.at(0).second->nu_vector().Phi();
       gen_nu_eta_3=tau_match_3.at(0).second->nu_vector().Rapidity();

       gen_vis_p_3=tau_match_3.at(0).second->vector().P();
       gen_vis_E_3=tau_match_3.at(0).second->vector().E();
       gen_vis_phi_3=tau_match_3.at(0).second->vector().Phi();
       gen_vis_eta_3=tau_match_3.at(0).second->vector().Rapidity();
      }

   // LEPTON4
   if(leptonsize_4!=0&&tausize_4!=0){
     DR(lepton_match_4.at(0).first,lepton_match_4.at(0).second) < DR(tau_match_4.at(0).first,tau_match_4.at(0).second) ? tausize_4=0 : leptonsize_4 = 0;
   }
   if(leptonsize_4==0&&tausize_4==0) gen_match_4 = mcorigin::fake;
   if(leptonsize_4!=0) {
      std::vector<bool> status_flags = lepton_match_4.at(0).second->statusFlags();
      gen_match_4_pt = lepton_match_4.at(0).second->pt();
      if(status_flags[IsPrompt]){
        if(abs(lepton_match_4.at(0).second->pdgid())==11){
          gen_match_4 = mcorigin::promptE;
         } else gen_match_4 = mcorigin::promptMu;
       }
       if(status_flags[IsDirectPromptTauDecayProduct]){
        if(abs(lepton_match_4.at(0).second->pdgid())==11){
          gen_match_4 = mcorigin::tauE;
         } else gen_match_4 = mcorigin::tauMu;
        }
       if(status_flags[IsDirectHadronDecayProduct]){
         if(abs(lepton_match_4.at(0).second->pdgid())==11){
           gen_match_4 = mcorigin::hadE;
          } else gen_match_4 = mcorigin::hadMu;
        }
      }
     if(tausize_4!=0){
       gen_match_4_pt = tau_match_4.at(0).second->pt();
       gen_match_4 = mcorigin::tauHad;
       tauFlag4 = tau_match_4.at(0).second->flavour();
       gen_nu_p_4=tau_match_4.at(0).second->nu_vector().P();
       gen_nu_phi_4=tau_match_4.at(0).second->nu_vector().Phi();
       gen_nu_eta_4=tau_match_4.at(0).second->nu_vector().Rapidity();

       gen_vis_p_4=tau_match_4.at(0).second->vector().P();
       gen_vis_E_4=tau_match_4.at(0).second->vector().E();
       gen_vis_phi_4=tau_match_4.at(0).second->vector().Phi();
       gen_vis_eta_4=tau_match_4.at(0).second->vector().Rapidity();
      }


   if(gen_match_1 == mcorigin::tauHad) event->Add("gen_tau_1", new ic::GenJet(*(tau_match_1.at(0).second)));
   if(gen_match_2 == mcorigin::tauHad) event->Add("gen_tau_2", new ic::GenJet(*(tau_match_2.at(0).second)));
   if(gen_match_3 == mcorigin::tauHad) event->Add("gen_tau_3", new ic::GenJet(*(tau_match_3.at(0).second)));
   if(gen_match_4 == mcorigin::tauHad) event->Add("gen_tau_4", new ic::GenJet(*(tau_match_4.at(0).second)));



   event->Add("gen_match_1",gen_match_1);
   event->Add("gen_match_2",gen_match_2);
   event->Add("gen_match_3",gen_match_3);
   event->Add("gen_match_4",gen_match_4);

   event->Add("gen_match_1_pt", gen_match_1_pt);
   event->Add("gen_match_2_pt", gen_match_2_pt);
   event->Add("gen_match_3_pt", gen_match_3_pt);
   event->Add("gen_match_4_pt", gen_match_4_pt);

   event->Add("gen_match_undecayed_1_pt", gen_match_undecayed_1_pt);
   event->Add("gen_match_undecayed_2_pt", gen_match_undecayed_2_pt);
   event->Add("gen_match_undecayed_3_pt", gen_match_undecayed_3_pt);
   event->Add("gen_match_undecayed_4_pt", gen_match_undecayed_4_pt);

   event->Add("gen_match_undecayed_1_eta", gen_match_undecayed_1_eta);
   event->Add("gen_match_undecayed_2_eta", gen_match_undecayed_2_eta);
   event->Add("gen_match_undecayed_3_eta", gen_match_undecayed_3_eta);
   event->Add("gen_match_undecayed_4_eta", gen_match_undecayed_4_eta);

   event->Add("tauFlag1", tauFlag1);
   event->Add("tauFlag2", tauFlag2);
   event->Add("tauFlag3", tauFlag3);
   event->Add("tauFlag4", tauFlag4);

   event->Add("gen_nu_p_1",gen_nu_p_1);
   event->Add("gen_nu_phi_1",gen_nu_phi_1);
   event->Add("gen_nu_eta_1",gen_nu_eta_1);

   event->Add("gen_nu_p_2",gen_nu_p_2);
   event->Add("gen_nu_phi_2",gen_nu_phi_2);
   event->Add("gen_nu_eta_2",gen_nu_eta_2);

   event->Add("gen_nu_p_3",gen_nu_p_3);
   event->Add("gen_nu_phi_3",gen_nu_phi_3);
   event->Add("gen_nu_eta_3",gen_nu_eta_3);

   event->Add("gen_nu_p_4",gen_nu_p_4);
   event->Add("gen_nu_phi_4",gen_nu_phi_4);
   event->Add("gen_nu_eta_4",gen_nu_eta_4);

   event->Add("gen_vis_p_1",  gen_vis_p_1);
   event->Add("gen_vis_E_1",  gen_vis_E_1);
   event->Add("gen_vis_phi_1",gen_vis_phi_1);
   event->Add("gen_vis_eta_1",gen_vis_eta_1);

   event->Add("gen_vis_p_2",  gen_vis_p_2);
   event->Add("gen_vis_E_2",  gen_vis_E_2);
   event->Add("gen_vis_phi_2",gen_vis_phi_2);
   event->Add("gen_vis_eta_2",gen_vis_eta_2);

   event->Add("gen_vis_p_3",  gen_vis_p_3);
   event->Add("gen_vis_E_3",  gen_vis_E_3);
   event->Add("gen_vis_phi_3",gen_vis_phi_3);
   event->Add("gen_vis_eta_3",gen_vis_eta_3);

   event->Add("gen_vis_p_4",  gen_vis_p_4);
   event->Add("gen_vis_E_4",  gen_vis_E_4);
   event->Add("gen_vis_phi_4",gen_vis_phi_4);
   event->Add("gen_vis_eta_4",gen_vis_eta_4);

    if(ngenjets_){
      //Get gen-jets collection, filter Higgs decay products and add Njets variable to event
      std::vector<ic::GenJet*> gen_jets = event->GetPtrVec<ic::GenJet>("genJets");

      // erase jets matched to our taus or leptons
      for(unsigned i=0; i<gen_jets.size(); ++i){
        ic::GenJet *genjet = gen_jets[i];
        bool MatchedToPrompt = false;
        for(unsigned j=0; j<sel_particles_noptcut.size(); ++j){
         if(DRLessThan(std::make_pair(genjet, sel_particles_noptcut[j]),0.5)) MatchedToPrompt = true;
        }
        for(unsigned j=0; j<gen_taus_ptr_noptcut.size(); ++j){
         if(DRLessThan(std::make_pair(genjet, gen_taus_ptr_noptcut[j]),0.5)) MatchedToPrompt = true;
        }
        //remove jets that are matched to Higgs decay products
        if(MatchedToPrompt) gen_jets.erase (gen_jets.begin()+i);
      }

      double genjetpt_1=-1., genjetpt_2=-1.;
      double genjeteta_1=-1., genjeteta_2=-1.;
      if(gen_jets.size()>0) {
        genjetpt_1 = gen_jets[0]->pt();
        genjeteta_1 = fabs(gen_jets[0]->eta());
      }
      if(gen_jets.size()>1) {
        genjetpt_2 = gen_jets[1]->pt();
        genjeteta_2 = fabs(gen_jets[1]->eta());
      }
      event->Add("genjetpt_1", genjetpt_1);
      event->Add("genjetpt_2", genjetpt_2);
      event->Add("genjeteta_1", genjeteta_1);
      event->Add("genjeteta_2", genjeteta_2);

      ic::erase_if(gen_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 5.0));
      unsigned ngenjets20 = gen_jets.size();
      event->Add("ngenjets20", ngenjets20);   
      ic::erase_if(gen_jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
      double gen_sjdphi_ = -9999; 
      double gen_mjj_=-9999;
      double gen_deta_=-9999;
      if (gen_jets.size() >= 2) {
        gen_mjj_ = (gen_jets[0]->vector()+gen_jets[1]->vector()).M();
        gen_deta_ = fabs(gen_jets[0]->eta() - gen_jets[1]->eta());
        if(gen_jets[0]->eta() > gen_jets[1]->eta())
          gen_sjdphi_ =  ROOT::Math::VectorUtil::DeltaPhi(gen_jets[0]->vector(), gen_jets[1]->vector());
        
        else
          gen_sjdphi_ =  ROOT::Math::VectorUtil::DeltaPhi(gen_jets[1]->vector(), gen_jets[0]->vector());
        
      }
      unsigned ngenjets = gen_jets.size();
      event->Add("ngenjets", ngenjets);
      event->Add("gen_sjdphi", gen_sjdphi_);
      event->Add("gen_mjj", gen_mjj_);
      event->Add("gen_jdeta", gen_deta_);
 
    }

    return 0;
  }
  int HTTFourTauGenInfo::PostAnalysis() {
    return 0;
  }

  void HTTFourTauGenInfo::PrintInfo() {
    ;
  }

}
