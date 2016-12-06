#ifndef ICHiggsTauTau_Module_HTTGenMatchFilter_h
#define ICHiggsTauTau_Module_HTTGenMatchFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"


#include <string>

namespace ic {

template <class T>
class HTTGenMatchFilter : public ModuleBase {
 private:
  CLASS_MEMBER(HTTGenMatchFilter<T>, std::string, input_vec_label)
  CLASS_MEMBER(HTTGenMatchFilter<T>, std::string, output_vec_label)
  CLASS_MEMBER(HTTGenMatchFilter<T>, ic::mcorigin, gen_match)
 public:
  HTTGenMatchFilter(std::string const& name);
  virtual ~HTTGenMatchFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};
  
template <class T>
HTTGenMatchFilter<T>::HTTGenMatchFilter(std::string const& name) : ModuleBase(name), gen_match_(mcorigin::tauHad) {
  input_vec_label_  = "taus";
  output_vec_label_ = "genmatched_taus";
}

template <class T>
HTTGenMatchFilter<T>::~HTTGenMatchFilter() {
  ;
}
template <class T>
int HTTGenMatchFilter<T>::Execute(TreeEvent *event) {
  std::vector<T*> input_vec = event->GetPtrVec<T>(input_vec_label_);
  std::vector<T*> output_vec;
  
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
    std::vector<T *> lepton;
    lepton.push_back(input_vec[i]);
    std::vector<std::pair<T*, GenParticle*> > lepton_match = MatchByDR(lepton, sel_particles, 0.2, true, true);
    std::vector<std::pair<T*, GenJet*> > tau_match  = MatchByDR(lepton, gen_taus_ptr, 0.2, true, true);
    
    mcorigin gen_match_1 = mcorigin::fake;
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
    
    if(gen_match_1 == gen_match_){
      output_vec.push_back(input_vec[i]);
    }
  }
  event->Add(output_vec_label_, output_vec);

  return 0;
}
template <class T>
int HTTGenMatchFilter<T>::PreAnalysis() {
  return 0;
}
template <class T>
int HTTGenMatchFilter<T>::PostAnalysis() {
  return 0;
}
template <class T>
void HTTGenMatchFilter<T>::PrintInfo() {
  ;
}
  

}

#endif
