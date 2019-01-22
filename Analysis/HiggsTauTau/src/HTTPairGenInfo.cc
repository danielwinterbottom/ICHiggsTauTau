#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTPairGenInfo.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <boost/functional/hash.hpp>
#include "boost/algorithm/string.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"

namespace ic {

  HTTPairGenInfo::HTTPairGenInfo(std::string const& name) : ModuleBase(name), channel_(channel::mt) {
    ditau_label_ = "ditau";
    fs_ = NULL;
    write_plots_ = false;
    hists_.resize(1);
    ngenjets_=false;
  }

  HTTPairGenInfo::~HTTPairGenInfo() {
    ;
  }

  int HTTPairGenInfo::PreAnalysis() {
  if (fs_ && write_plots_) {
      hists_[0] = new Dynamic2DHistoSet(fs_->mkdir("httpairgeninfo"));
      for (unsigned i = 0; i < hists_.size(); ++i) {
        hists_[i]->Create("relpt_vs_drlep_lead", 50, 0, 10, 20, 0, 0.5);
        hists_[i]->Create("relpt_vs_drtau_lead", 50, 0, 10, 20, 0, 0.5);
        hists_[i]->Create("relpt_vs_drlep_sublead", 50, 0, 10, 20, 0, 0.5);
        hists_[i]->Create("relpt_vs_drtau_sublead", 50, 0, 10, 20, 0, 0.5);
      }
    }

    return 0;
  }

  int HTTPairGenInfo::Execute(TreeEvent *event) {


    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
    CompositeCandidate const* ditau = ditau_vec.at(0);

    std::vector<Candidate *> leading_lepton;
    std::vector<Candidate *> subleading_lepton;
    leading_lepton.push_back(ditau->GetCandidate("lepton1"));
    subleading_lepton.push_back(ditau->GetCandidate("lepton2")); 

    std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<GenParticle *> sel_particles;
    std::vector<GenParticle *> undecayed_taus;
    
    double gen_match_undecayed_1_pt = -1;
    double gen_match_undecayed_2_pt = -1;
    double gen_match_undecayed_1_eta = -1;
    double gen_match_undecayed_2_eta = -1;
    
    for (unsigned i=0; i < particles.size(); ++i){
      std::vector<bool> status_flags_start = particles[i]->statusFlags();
      if ( ((abs(particles[i]->pdgid()) == 11 )||(abs(particles[i]->pdgid()) == 13 /*&& particles[i]->status()==1*/)) && particles[i]->pt() > 8. && (status_flags_start[IsPrompt] || status_flags_start[IsDirectPromptTauDecayProduct] /*|| status_flags_start[IsDirectHadronDecayProduct]*/)){
        sel_particles.push_back(particles[i]);
      }
      if(channel_!=channel::zmm&&status_flags_start[IsPrompt] && status_flags_start[IsLastCopy] && abs(particles[i]->pdgid()) == 15) undecayed_taus.push_back(particles[i]);
      if(channel_==channel::zmm&&status_flags_start[IsPrompt] && status_flags_start[IsLastCopy] && abs(particles[i]->pdgid()) == 13) undecayed_taus.push_back(particles[i]);
    }
    
    if(undecayed_taus.size()>0){
      gen_match_undecayed_1_pt = undecayed_taus[0]->pt();
      gen_match_undecayed_1_eta = undecayed_taus[0]->eta();
    }
    if(undecayed_taus.size()>1){
      gen_match_undecayed_2_pt = undecayed_taus[1]->pt();
      gen_match_undecayed_2_eta = undecayed_taus[1]->eta();
    }
    
    std::vector<GenJet> gen_taus = BuildTauJets(particles, false,true);
    std::vector<GenJet *> gen_taus_ptr;
    for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);
    ic::erase_if(gen_taus_ptr, !boost::bind(MinPtMaxEta, _1, 15.0, 999.));
 
 
 
    std::vector<std::pair<Candidate*, GenParticle*> > leading_lepton_match = MatchByDR(leading_lepton, sel_particles, 0.2, true, true);
    std::vector<std::pair<Candidate*, GenJet*> > leading_tau_match  = MatchByDR(leading_lepton, gen_taus_ptr, 0.2, true, true);

    std::vector<std::pair<Candidate*, GenParticle*> > subleading_lepton_match = MatchByDR(subleading_lepton, sel_particles, 0.2, true, true);
    std::vector<std::pair<Candidate*, GenJet*> > subleading_tau_match  = MatchByDR(subleading_lepton, gen_taus_ptr, 0.2, true, true);
    
    /* if (leading_tau_match.size()!=0) */
    /*     std::cout << "tau: " << leading_tau_match.at(0).first->eta() << std::endl; */
    /*     std::cout << "tau gen jet: " << leading_tau_match.at(0).second->eta() << std::endl; */
    /* if (subleading_tau_match.size()!=0) */
    /*     std::cout << "tau: " << subleading_tau_match.at(0).first->eta() << std::endl; */
    /*     std::cout << "tau gen jet: " << subleading_tau_match.at(0).second->eta() << std::endl; */
    


   mcorigin gen_match_1 = mcorigin::fake;
   mcorigin gen_match_2 = mcorigin::fake;
   int tauDecayFlag_1 = -1;
   int tauDecayFlag_2 = -1;
   int leptonsize = leading_lepton_match.size();
   int tausize = leading_tau_match.size();
   double gen_match_1_pt = -1;
   double gen_match_2_pt = -1;
/*   double leading_lepton_match_pt = -1.;
   double subleading_lepton_match_pt = -1.;
   double leading_lepton_match_DR = -1.;
   double subleading_lepton_match_DR = -1;*/


   if(leptonsize!=0&&tausize!=0){
     DR(leading_lepton_match.at(0).first,leading_lepton_match.at(0).second) < DR(leading_tau_match.at(0).first,leading_tau_match.at(0).second) ? tausize=0 : leptonsize = 0;
   }

   if(leptonsize==0&&tausize==0) gen_match_1 = mcorigin::fake;
   if(leptonsize!=0) {
      std::vector<bool> status_flags = leading_lepton_match.at(0).second->statusFlags();
      gen_match_1_pt = leading_lepton_match.at(0).second->pt();
//      leading_lepton_match_DR = DR(leading_lepton_match.at(0).first,leading_lepton_match.at(0).second);
      if(fs_ && write_plots_){ 
       hists_[0]->Fill("relpt_vs_drlep_lead",fabs(leading_lepton_match.at(0).second->pt()-leading_lepton_match.at(0).first->pt())/leading_lepton_match.at(0).first->pt(),DR(leading_lepton_match.at(0).first,leading_lepton_match.at(0).second));
      }
      if(status_flags[IsPrompt]){
        if(abs(leading_lepton_match.at(0).second->pdgid())==11){
          gen_match_1 = mcorigin::promptE;
         } else gen_match_1 = mcorigin::promptMu;
       }
       if(status_flags[IsDirectPromptTauDecayProduct]){
        if(abs(leading_lepton_match.at(0).second->pdgid())==11){
          gen_match_1 = mcorigin::tauE;
         } else gen_match_1 = mcorigin::tauMu;
        }
       if(status_flags[IsDirectHadronDecayProduct]){
         if(abs(leading_lepton_match.at(0).second->pdgid())==11){
           gen_match_1 = mcorigin::hadE;
          } else gen_match_1 = mcorigin::hadMu;
        } 
      }
     if(tausize!=0){
       if(fs_ && write_plots_){
       hists_[0]->Fill("relpt_vs_drtau_lead",fabs(leading_tau_match.at(0).second->pt()-leading_tau_match.at(0).first->pt())/leading_tau_match.at(0).first->pt(),DR(leading_tau_match.at(0).first,leading_tau_match.at(0).second));
       }
       gen_match_1_pt = leading_tau_match.at(0).second->pt();
  //     leading_lepton_match_DR = DR(leading_tau_match.at(0).first,leading_tau_match.at(0).second);
       gen_match_1 = mcorigin::tauHad;
       tauDecayFlag_1 = leading_tau_match.at(0).second->flavour();
      }
   
//Now for subleading lepton:
   leptonsize = subleading_lepton_match.size();
   tausize = subleading_tau_match.size();

   if(leptonsize!=0&&tausize!=0){
     DR(subleading_lepton_match.at(0).first,subleading_lepton_match.at(0).second) < DR(subleading_tau_match.at(0).first,subleading_tau_match.at(0).second) ? tausize=0 : leptonsize = 0;
   }

   if(leptonsize==0&&tausize==0) gen_match_2 = mcorigin::fake;
   if(leptonsize!=0) {
      std::vector<bool> status_flags = subleading_lepton_match.at(0).second->statusFlags();
      gen_match_2_pt = subleading_lepton_match.at(0).second->pt();
    //  subleading_lepton_match_DR = DR(subleading_lepton_match.at(0).first,subleading_lepton_match.at(0).second);
      if(fs_ && write_plots_){
       hists_[0]->Fill("relpt_vs_drlep_sublead",fabs(subleading_lepton_match.at(0).second->pt()-subleading_lepton_match.at(0).first->pt())/subleading_lepton_match.at(0).first->pt(),DR(subleading_lepton_match.at(0).first,subleading_lepton_match.at(0).second));
      }
      if(status_flags[IsPrompt]){
        if(abs(subleading_lepton_match.at(0).second->pdgid())==11){
          gen_match_2 = mcorigin::promptE;
         } else gen_match_2 = mcorigin::promptMu;
       }
       if(status_flags[IsDirectPromptTauDecayProduct]){
        if(abs(subleading_lepton_match.at(0).second->pdgid())==11){
          gen_match_2 = mcorigin::tauE;
         } else gen_match_2 = mcorigin::tauMu;
        }
       if(status_flags[IsDirectHadronDecayProduct]){
         if(abs(subleading_lepton_match.at(0).second->pdgid())==11){
           gen_match_2 = mcorigin::hadE;
          } else gen_match_2 = mcorigin::hadMu;
        } 
      }
     if(tausize!=0){
       if(fs_&&write_plots_){
       hists_[0]->Fill("relpt_vs_drtau_sublead",fabs(subleading_tau_match.at(0).second->pt()-subleading_tau_match.at(0).first->pt())/subleading_tau_match.at(0).first->pt(),DR(subleading_tau_match.at(0).first,subleading_tau_match.at(0).second));
       }
       gen_match_2_pt = subleading_tau_match.at(0).second->pt();
      // subleading_lepton_match_DR = DR(subleading_tau_match.at(0).first,subleading_tau_match.at(0).second);
       gen_match_2 = mcorigin::tauHad;
       tauDecayFlag_2 = subleading_tau_match.at(0).second->flavour();
      }



   event->Add("gen_match_1",gen_match_1);
   event->Add("gen_match_2",gen_match_2);
   event->Add("gen_match_1_pt", gen_match_1_pt);
   event->Add("gen_match_2_pt", gen_match_2_pt);
   event->Add("gen_match_undecayed_1_pt", gen_match_undecayed_1_pt);
   event->Add("gen_match_undecayed_2_pt", gen_match_undecayed_2_pt);
   event->Add("gen_match_undecayed_1_eta", gen_match_undecayed_1_eta);
   event->Add("gen_match_undecayed_2_eta", gen_match_undecayed_2_eta);
/*   event->Add("leading_lepton_match_pt",leading_lepton_match_pt);
   event->Add("subleading_lepton_match_pt",subleading_lepton_match_pt);
   event->Add("leading_lepton_match_DR",leading_lepton_match_DR);
   event->Add("subleading_lepton_match_DR",subleading_lepton_match_DR);*/

   // to save gen decay mode
   event->Add("tauDecayFlag_1", tauDecayFlag_1);
   event->Add("tauDecayFlag_2", tauDecayFlag_2);

    if(ngenjets_){
      //Get gen-jets collection, filter Higgs decay products and add Njets variable to event
      std::vector<ic::GenJet*> gen_jets = event->GetPtrVec<ic::GenJet>("genJets");
      ic::erase_if(gen_jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
      for(unsigned i=0; i<gen_jets.size(); ++i){
        ic::GenJet *genjet = gen_jets[i];
        bool MatchedToPrompt = false;
        for(unsigned j=0; j<sel_particles.size(); ++j){
         if(DRLessThan(std::make_pair(genjet, sel_particles[j]),0.5)) MatchedToPrompt = true;
        } 
        for(unsigned j=0; j<gen_taus_ptr.size(); ++j){
         if(DRLessThan(std::make_pair(genjet, gen_taus_ptr[j]),0.5)) MatchedToPrompt = true;
        }
        //remove jets that are matched to Higgs decay products
        if(MatchedToPrompt) gen_jets.erase (gen_jets.begin()+i);
      }
      unsigned ngenjets = gen_jets.size();
      double gen_sjdphi_ = -999;
      if (ngenjets >= 2) {
        if(gen_jets[0]->eta() > gen_jets[1]->eta())
          gen_sjdphi_ =  ROOT::Math::VectorUtil::DeltaPhi(gen_jets[0]->vector(), gen_jets[1]->vector());
        
        else
          gen_sjdphi_ =  ROOT::Math::VectorUtil::DeltaPhi(gen_jets[1]->vector(), gen_jets[0]->vector());
        
      }
      event->Add("ngenjets", ngenjets);
      event->Add("gen_sjdphi", gen_sjdphi_);
    }
    
    // new cp stuff
    std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");

    std::vector<std::pair<GenParticle*, GenParticle*>> rho_daughters;
    std::vector<ic::GenParticle*> gen_taus_from_particles;

    double gen_pi_pt_1 ;
    double gen_pi_eta_1;
    double gen_pi_phi_1;
    double gen_pi_E_1  ;

    double gen_pi0_pt_1 ;
    double gen_pi0_eta_1;
    double gen_pi0_phi_1;
    double gen_pi0_E_1  ;

    double gen_pi_pt_2 ;
    double gen_pi_eta_2;
    double gen_pi_phi_2;
    double gen_pi_E_2  ;

    double gen_pi0_pt_2 ;
    double gen_pi0_eta_2;
    double gen_pi0_phi_2;
    double gen_pi0_E_2  ;

    for (unsigned i = 0; i < gen_particles.size(); ++i) {
      std::pair<GenParticle*,GenParticle*> rho = std::make_pair(new GenParticle(), new GenParticle());
      unsigned count_pi = 0;
      unsigned count_pi0 = 0;

      if(std::fabs(gen_particles[i]->pdgid()) == 15 && gen_particles[i]->statusFlags()[IsLastCopy]){
        ic::GenParticle* tau = gen_particles[i];

        /* std::cout << "gen tau: " << i << std::endl; */
        /* std::cout << "gen tau p4: " << tau->vector() << std::endl; */
        for (unsigned d : tau->daughters()) {
          unsigned daughter_id = std::abs(gen_particles[d]->pdgid());
          /* std::cout << "tau daughters pdgid " << gen_particles[d]->pdgid() << std::endl; */
          if (daughter_id == 211) {
            /* std::cout << "pi 4 vector: " << gen_particles[d]->vector() << std::endl; */
            rho.first = gen_particles[d];
            ++count_pi;
          }
          else if (daughter_id == 111) {
            /* std::cout << "pi0 4 vector: " << gen_particles[d]->vector() << std::endl; */
            rho.second = gen_particles[d];
            ++count_pi0;
          }
        }
        /* std::cout << "pi count: " << count_pi << std::endl; */
        /* std::cout << "pi0 count: " << count_pi0 << std::endl; */

        if (count_pi == 1 && count_pi0 == 1 && tauDecayFlag_1==1 && tauDecayFlag_2==1){
          /* std::cout << "gen pi p4: " << rho.first->vector() << std::endl; */
          /* std::cout << "gen pi0 p4: " << rho.second->vector() << std::endl; */

          rho_daughters.push_back(rho);

          ic::Candidate* tauvis = new Candidate();
          tauvis->set_vector(rho.first->vector()+rho.second->vector());

          double leadingdR = DR(leading_lepton[0],tauvis);
          double subleadingdR = DR(subleading_lepton[0],tauvis);

          /* std::cout << "DR (leading tau, gentau): " << leadingdR << std::endl; */
          /* std::cout << "DR (subleading tau, gentau): " << subleadingdR << std::endl; */

          if (leadingdR < 0.2) {
            gen_pi_pt_1 = rho.first->vector().Pt();
            gen_pi_eta_1 = rho.first->vector().Eta();
            gen_pi_phi_1 = rho.first->vector().Phi();
            gen_pi_E_1 = rho.first->vector().E();

            gen_pi0_pt_1 = rho.second->vector().Pt();
            gen_pi0_eta_1 = rho.second->vector().Eta();
            gen_pi0_phi_1 = rho.second->vector().Phi();
            gen_pi0_E_1 = rho.second->vector().E();
            
            break;
          }
          else {
            gen_pi_pt_1  = -9999.; 
            gen_pi_eta_1 = -9999.;
            gen_pi_phi_1 = -9999.;
            gen_pi_E_1   = -9999.; 

            gen_pi0_pt_1  = -9999.;
            gen_pi0_eta_1 = -9999.;
            gen_pi0_phi_1 = -9999.;
            gen_pi0_E_1   = -9999.;
          }
          if (subleadingdR < 0.2) {
            gen_pi_pt_2 = rho.first->vector().Pt();
            gen_pi_eta_2 = rho.first->vector().Eta();
            gen_pi_phi_2 = rho.first->vector().Phi();
            gen_pi_E_2 = rho.first->vector().E();

            gen_pi0_pt_2 = rho.second->vector().Pt();
            gen_pi0_eta_2 = rho.second->vector().Eta();
            gen_pi0_phi_2 = rho.second->vector().Phi();
            gen_pi0_E_2 = rho.second->vector().E();

            break;
          }
          else {
            gen_pi_pt_1  = -9999.; 
            gen_pi_eta_1 = -9999.;
            gen_pi_phi_1 = -9999.;
            gen_pi_E_1   = -9999.; 

            gen_pi0_pt_2  = -9999.;
            gen_pi0_eta_2 = -9999.;
            gen_pi0_phi_2 = -9999.;
            gen_pi0_E_2   = -9999.;
          }
        }
      }
    }
    event->Add("gen_pi_pt_1", gen_pi_pt_1);
    event->Add("gen_pi_eta_1", gen_pi_eta_1);
    event->Add("gen_pi_phi_1", gen_pi_phi_1);
    event->Add("gen_pi_E_1", gen_pi_E_1);

    event->Add("gen_pi_pt_2", gen_pi_pt_2);
    event->Add("gen_pi_eta_2", gen_pi_eta_2);
    event->Add("gen_pi_phi_2", gen_pi_phi_2);
    event->Add("gen_pi_E_2", gen_pi_E_2);

    event->Add("gen_pi0_pt_1", gen_pi0_pt_1);
    event->Add("gen_pi0_eta_1", gen_pi0_eta_1);
    event->Add("gen_pi0_phi_1", gen_pi0_phi_1);
    event->Add("gen_pi0_E_1", gen_pi0_E_1);
    
    event->Add("gen_pi0_pt_2", gen_pi0_pt_2);
    event->Add("gen_pi0_eta_2", gen_pi0_eta_2);
    event->Add("gen_pi0_phi_2", gen_pi0_phi_2);
    event->Add("gen_pi0_E_2", gen_pi0_E_2);

    /* std::pair<bool, std::vector<ic::GenParticle*>> rho_1 = std::make_pair(false, std::vector<GenParticle*>()); */
    /* std::pair<bool, std::vector<ic::GenParticle*>> rho_2 = std::make_pair(false, std::vector<GenParticle*>()); */
    /* TLorentzVector lvec1; */
    /* TLorentzVector lvec2; */
    /* TLorentzVector lvec3; */
    /* TLorentzVector lvec4; */

    /* double genE_pi1_ = -999; */
    /* double genE_pi01_ = -999; */
    /* double genE_pi2_ = -999; */
    /* double genE_pi02_ = -999; */

    /* double genPhi_pi1_ = -999; */
    /* double genPhi_pi01_ = -999; */
    /* double genPhi_pi2_ = -999; */
    /* double genPhi_pi02_ = -999; */

    /* double genEta_pi1_ = -999; */
    /* double genEta_pi01_ = -999; */
    /* double genEta_pi2_ = -999; */
    /* double genEta_pi02_ = -999; */

    /* double gen_aco_angle_1_ = -999; */
    /* double gen_aco_angle_2_ = -999; */
    /* double gen_cp_sign_1_ = -999; */
    /* if(gen_taus_ptr.size()>=2) { */
    /*   rho_1 = GetTauRhoDaughter(particles, gen_taus_ptr[0]->constituents()); */  
    /*   rho_2 = GetTauRhoDaughter(particles, gen_taus_ptr[1]->constituents()); */  
    /*   if (rho_1.first && rho_2.first && tauDecayFlag_1==1 && tauDecayFlag_2==1) { */
    /*     lvec1 = ConvertToLorentz(rho_1.second[1]->vector()); */   
    /*     lvec2 = ConvertToLorentz(rho_2.second[1]->vector()); */
    /*     lvec3 = ConvertToLorentz(rho_1.second[0]->vector()); */   
    /*     lvec4 = ConvertToLorentz(rho_2.second[0]->vector()); */

    /*     std::cout << "gen pi_1: " << rho_1.second[1]->vector() << std::endl; */
    /*     std::cout << "gen pi0_1 : " << rho_2.second[1]->vector() << std::endl; */
    /*     std::cout << "gen pi_2 : " << rho_1.second[0]->vector() << std::endl; */
    /*     std::cout << "gen pi0_2 : " << rho_2.second[0]->vector() << std::endl; */

    /*     genE_pi1_ = rho_1.second[0]->vector().E(); */
    /*     genE_pi01_ = rho_1.second[1]->vector().E(); */
    /*     genE_pi2_ = rho_2.second[0]->vector().E(); */
    /*     genE_pi02_ = rho_2.second[1]->vector().E(); */

    /*     genPhi_pi1_ =  rho_1.second[0]->vector().Phi(); */
    /*     genPhi_pi01_ = rho_1.second[1]->vector().Phi(); */
    /*     genPhi_pi2_ =  rho_2.second[0]->vector().Phi(); */
    /*     genPhi_pi02_ = rho_2.second[1]->vector().Phi(); */

    /*     genEta_pi1_ =  rho_1.second[0]->vector().Eta(); */
    /*     genEta_pi01_ = rho_1.second[1]->vector().Eta(); */
    /*     genEta_pi2_ =  rho_2.second[0]->vector().Eta(); */
    /*     genEta_pi02_ = rho_2.second[1]->vector().Eta(); */

    /*     gen_cp_sign_1_ = YRho(rho_1.second,TVector3())*YRho(rho_2.second,TVector3()); */

    /*     gen_aco_angle_1_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,false); */    
    /*     gen_aco_angle_2_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,true); */
    /*   } */
    /* } */
    /* event->Add("genE_pi1", genE_pi1_); */
    /* event->Add("genE_pi01", genE_pi01_); */
    /* event->Add("genE_pi2", genE_pi2_); */
    /* event->Add("genE_pi02", genE_pi02_); */

    /* event->Add("genPhi_pi1", genPhi_pi1_); */
    /* event->Add("genPhi_pi01", genPhi_pi01_); */
    /* event->Add("genPhi_pi2", genPhi_pi2_); */
    /* event->Add("genPhi_pi02", genPhi_pi02_); */

    /* event->Add("genEta_pi1",  genEta_pi1_); */
    /* event->Add("genEta_pi01", genEta_pi01_); */
    /* event->Add("genEta_pi2",  genEta_pi2_); */
    /* event->Add("genEta_pi02", genEta_pi02_); */

    /* event->Add("gen_cp_sign_1", gen_cp_sign_1_); */
    /* event->Add("gen_aco_angle_1", gen_aco_angle_1_); */
    /* event->Add("gen_aco_angle_2", gen_aco_angle_2_); */

    // CP in decay
    /* std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");
    std::pair<bool, std::vector<ic::GenParticle*>> rho_daughters;
    TLorentzVector lvec1;
    TLorentzVector lvec2;
    TLorentzVector lvec3;
    TLorentzVector lvec4;*/

    return 0;
  }
  int HTTPairGenInfo::PostAnalysis() {
    return 0;
  }

  void HTTPairGenInfo::PrintInfo() {
    ;
  }

}
