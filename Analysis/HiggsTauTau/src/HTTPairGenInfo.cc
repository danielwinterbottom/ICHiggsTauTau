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

  HTTPairGenInfo::HTTPairGenInfo(std::string const& name) : ModuleBase(name) {
    ditau_label_ = "ditau";
  }

  HTTPairGenInfo::~HTTPairGenInfo() {
    ;
  }

  int HTTPairGenInfo::PreAnalysis() {
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
    std::vector<GenParticle *> prompt_sel_particles;
    std::vector<GenParticle *> tau_decay_sel_particles;
    for (unsigned i=0; i < particles.size(); ++i){
      std::vector<bool> status_flags = particles[i]->statusFlags();
      if ( ((abs(particles[i]->pdgid()) == 11 && status_flags[IsPrompt])||(abs(particles[i]->pdgid()) == 13 && status_flags[IsPrompt] && particles[i]->status()==1)) && particles[i]->pt() > 8. ){
        prompt_sel_particles.push_back(particles[i]);
      }
      if ( ((abs(particles[i]->pdgid())==11 && status_flags[IsDirectPromptTauDecayProduct]) || (abs(particles[i]->pdgid()) == 13 && status_flags[IsDirectPromptTauDecayProduct] && particles[i]->status()==1)) && particles[i]->pt() > 8.){
       tau_decay_sel_particles.push_back(particles[i]);
      }
    }

    
    std::vector<GenJet> gen_taus = BuildTauJets(particles, false);
    std::vector<GenJet *> gen_taus_ptr;
    for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);
    ic::erase_if(gen_taus_ptr, !boost::bind(MinPtMaxEta, _1, 15.0, 999.));
 
 
 
    std::vector<std::pair<Candidate*, GenParticle*> > leading_prompt_match = MatchByDR(leading_lepton, prompt_sel_particles, 0.5, true, true);
    std::vector<std::pair<Candidate*, GenParticle*> > leading_tau_decay_match = MatchByDR(leading_lepton, tau_decay_sel_particles, 0.5, true, true);
    std::vector<std::pair<Candidate*, GenJet*> > leading_tau_match  = MatchByDR(leading_lepton, gen_taus_ptr, 0.5, true, true);

    std::vector<std::pair<Candidate*, GenParticle*> > subleading_prompt_match = MatchByDR(subleading_lepton, prompt_sel_particles, 0.5, true, true);
    std::vector<std::pair<Candidate*, GenParticle*> > subleading_tau_decay_match = MatchByDR(subleading_lepton, tau_decay_sel_particles, 0.5, true, true);
    std::vector<std::pair<Candidate*, GenJet*> > subleading_tau_match  = MatchByDR(subleading_lepton, gen_taus_ptr, 0.5, true, true);
    


   mcorigin gen_match_1 = mcorigin::fake;
   mcorigin gen_match_2 = mcorigin::fake;
   int promptsize = leading_prompt_match.size();
   int taudecaysize = leading_tau_decay_match.size();
   int tausize = leading_tau_match.size();
   double leading_lepton_match_pt = -1.;
   double subleading_lepton_match_pt = -1.;


   if(promptsize!=0 && taudecaysize!=0 && tausize == 0){
     DR(leading_prompt_match.at(0).first,leading_prompt_match.at(0).second) < DR(leading_tau_decay_match.at(0).first,leading_tau_decay_match.at(0).second) ? taudecaysize=0 : promptsize = 0;
   } else if (promptsize!=0 && taudecaysize==0 && tausize != 0){
     DR(leading_prompt_match.at(0).first,leading_prompt_match.at(0).second) < DR(leading_tau_match.at(0).first,leading_tau_match.at(0).second) ? tausize=0 : promptsize = 0;
  } else if (promptsize==0 && taudecaysize!=0 && tausize != 0){
     DR(leading_tau_decay_match.at(0).first,leading_tau_decay_match.at(0).second) < DR(leading_tau_match.at(0).first,leading_tau_match.at(0).second) ? tausize=0 : taudecaysize = 0;
 } else if(promptsize!=0 && taudecaysize!=0 && tausize!=0){
   if(DR(leading_tau_decay_match.at(0).first,leading_tau_decay_match.at(0).second) < DR(leading_tau_match.at(0).first,leading_tau_match.at(0).second)){
      tausize=0;
      DR(leading_tau_decay_match.at(0).first,leading_tau_decay_match.at(0).second) < DR(leading_prompt_match.at(0).first,leading_prompt_match.at(0).second) ? promptsize=0:taudecaysize=0;
    } else {
      taudecaysize = 0;
      DR(leading_tau_match.at(0).first,leading_tau_match.at(0).second) < DR(leading_prompt_match.at(0).first,leading_prompt_match.at(0).second) ? promptsize=0:tausize=0;
    } 
 }


   if(promptsize==0 && taudecaysize==0&&tausize==0) gen_match_1 = mcorigin::fake;
   if(promptsize!=0 && taudecaysize==0 && tausize ==0){
     leading_lepton_match_pt = leading_prompt_match.at(0).second->pt();
     if(abs(leading_prompt_match.at(0).second->pdgid())==11){
              gen_match_1 = mcorigin::promptE;
     } else gen_match_1 = mcorigin::promptMu;
   }
   if(promptsize==0 && taudecaysize !=0 &&tausize==0){
     leading_lepton_match_pt = leading_tau_decay_match.at(0).second->pt();
     if(abs(leading_tau_decay_match.at(0).second->pdgid())==11){
       gen_match_1 = mcorigin::tauE;
     } else gen_match_1 = mcorigin::tauMu;
   }
   if(promptsize==0 && taudecaysize==0 && tausize!=0){
    leading_lepton_match_pt = leading_tau_match.at(0).second->pt();
    gen_match_1 = mcorigin::tauHad;
   }

//Now for subleading lepton:
   promptsize = subleading_prompt_match.size();
   taudecaysize = subleading_tau_decay_match.size();
   tausize = subleading_tau_match.size();

   if(promptsize!=0 && taudecaysize!=0 && tausize == 0){
     DR(subleading_prompt_match.at(0).first,subleading_prompt_match.at(0).second) < DR(subleading_tau_decay_match.at(0).first,subleading_tau_decay_match.at(0).second) ? taudecaysize=0 : promptsize = 0;
   } else if (promptsize!=0 && taudecaysize==0 && tausize != 0){
     DR(subleading_prompt_match.at(0).first,subleading_prompt_match.at(0).second) < DR(subleading_tau_match.at(0).first,subleading_tau_match.at(0).second) ? tausize=0 : promptsize = 0;
  } else if (promptsize==0 && taudecaysize!=0 && tausize != 0){
     DR(subleading_tau_decay_match.at(0).first,subleading_tau_decay_match.at(0).second) < DR(subleading_tau_match.at(0).first,subleading_tau_match.at(0).second) ? tausize=0 : taudecaysize = 0;
 } else if(promptsize!=0 && taudecaysize!=0 && tausize!=0){
   if(DR(subleading_tau_decay_match.at(0).first,subleading_tau_decay_match.at(0).second) < DR(subleading_tau_match.at(0).first,subleading_tau_match.at(0).second)){
      tausize=0;
      DR(subleading_tau_decay_match.at(0).first,subleading_tau_decay_match.at(0).second) < DR(subleading_prompt_match.at(0).first,subleading_prompt_match.at(0).second) ? promptsize=0:taudecaysize=0;
    } else {
      taudecaysize = 0;
      DR(subleading_tau_match.at(0).first,subleading_tau_match.at(0).second) < DR(subleading_prompt_match.at(0).first,subleading_prompt_match.at(0).second) ? promptsize=0:tausize=0;
    } 
 }
   if(promptsize==0 && taudecaysize==0&&tausize==0) gen_match_2 = mcorigin::fake;
  else if(promptsize!=0 && taudecaysize==0 && tausize ==0){
     subleading_lepton_match_pt = subleading_prompt_match.at(0).second->pt();
     if(abs(subleading_prompt_match.at(0).second->pdgid())==11){
       gen_match_2 = mcorigin::promptE;
     } else  gen_match_2 = mcorigin::promptMu;
   }
   else if(promptsize==0 && taudecaysize !=0 &&tausize==0){
     subleading_lepton_match_pt = subleading_tau_decay_match.at(0).second->pt();
     if(abs(subleading_tau_decay_match.at(0).second->pdgid())==11){
       gen_match_2 = mcorigin::tauE;
     } else gen_match_2 = mcorigin::tauMu;
   }
   else if(promptsize==0 && taudecaysize==0 && tausize!=0){
    subleading_lepton_match_pt = subleading_tau_match.at(0).second->pt();
    gen_match_2 = mcorigin::tauHad;
  }

   event->Add("gen_match_1",gen_match_1);
   event->Add("gen_match_2",gen_match_2);
   event->Add("leading_lepton_match_pt",leading_lepton_match_pt);
   event->Add("subleading_lepton_match_pt",subleading_lepton_match_pt);

    return 0;
  }
  int HTTPairGenInfo::PostAnalysis() {
    return 0;
  }

  void HTTPairGenInfo::PrintInfo() {
    ;
  }

}
