#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTGenMatchFilter.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <boost/functional/hash.hpp>
#include "boost/algorithm/string.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"

namespace ic {

  HTTGenMatchFilter::HTTGenMatchFilter(std::string const& name) : ModuleBase(name) {
    input_vec_label_  = "taus";
    output_vec_label_ = "genmatched_taus";
    gen_match_        = 6;
  }

  HTTGenMatchFilter::~HTTGenMatchFilter() {
    ;
  }

  int HTTGenMatchFilter::PreAnalysis() {
    return 0;
  }

  int HTTGenMatchFilter::Execute(TreeEvent *event) {
    
    std::vector<Candidate*> input_vec = event->GetPtrVec<CompositeCandidate>(input_vec_label_);
    std::vector<Candidate*> output_vec = event->GetPtrVec<CompositeCandidate>(output_vec_label_);

    std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<GenParticle *> sel_particles;
    for (unsigned i=0; i < particles.size(); ++i){
      std::vector<bool> status_flags_start = particles[i]->statusFlags();
      if ( ((abs(particles[i]->pdgid()) == 11 )||(abs(particles[i]->pdgid()) == 13)) && particles[i]->pt() > 8. && (status_flags_start[IsPrompt] || status_flags_start[IsDirectPromptTauDecayProduct])){
        sel_particles.push_back(particles[i]);
      }
    }
    
    std::vector<GenJet> gen_taus = BuildTauJets(particles, false,true);
    std::vector<GenJet *> gen_taus_ptr;
    for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);
    ic::erase_if(gen_taus_ptr, !boost::bind(MinPtMaxEta, _1, 15.0, 999.));
    
    for(unsigned i=0; i<input_vec.size(); ++i){
      std::vector<Candidate *> lepton;
      lepton.push_back(input_vec[i]);
      
      std::vector<std::pair<Candidate*, GenParticle*> > lepton_match = MatchByDR(lepton, sel_particles, 0.2, true, true);
      std::vector<std::pair<Candidate*, GenJet*> > tau_match  = MatchByDR(lepton, gen_taus_ptr, 0.2, true, true);
      
      mcorigin gen_match_1 = mcorigin::fake;
      mcorigin gen_match_2 = mcorigin::fake;
      int leptonsize = lepton_match.size();
      int tausize = tau_match.size();
      
      if(leptonsize!=0&&tausize!=0){
        DR(lepton_match.at(0).first,lepton_match.at(0).second) < DR(tau_match.at(0).first,tau_match.at(0).second) ? tausize=0 : leptonsize = 0;
      }
      
      if(leptonsize==0&&tausize==0) gen_match_1 = mcorigin::fake;
      if(leptonsize!=0) {
        std::vector<bool> status_flags = lepton_match.at(0).second->statusFlags();
        
        if(status_flags[IsPrompt]){
          if(abs(lepton_match.at(0).second->pdgid())==11){
            gen_match_1 = mcorigin::promptE;
           } else gen_match_1 = mcorigin::promptMu;
        }
        if(status_flags[IsDirectPromptTauDecayProduct]){
         if(abs(lepton_match.at(0).second->pdgid())==11){
           gen_match_1 = mcorigin::tauE;
          } else gen_match_1 = mcorigin::tauMu;
         }
        if(status_flags[IsDirectHadronDecayProduct]){
          if(abs(lepton_match.at(0).second->pdgid())==11){
            gen_match_1 = mcorigin::hadE;
           } else gen_match_1 = mcorigin::hadMu;
         } 
      }
      if(tausize!=0){
        gen_match_1 = mcorigin::tauHad;
      }
      
      std::cout << "mcorigin::tauHad = " << mcorigin::tauHad << std::endl;
      if(gen_match_1 == gen_match_) output_vec.push_back(input_vec[i]);
   }

    return 0;
  }
  int HTTGenMatchFilter::PostAnalysis() {
    return 0;
  }

  void HTTGenMatchFilter::PrintInfo() {
    ;
  }

}
