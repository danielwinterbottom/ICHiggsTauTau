#ifndef ICHiggsTauTau_Module_HTTGenMatchSelector_h
#define ICHiggsTauTau_Module_HTTGenMatchSelector_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"


#include <string>

namespace ic {

template <class T>
class HTTGenMatchSelector : public ModuleBase {
 private:
  CLASS_MEMBER(HTTGenMatchSelector<T>, std::string, input_vec_label)
  CLASS_MEMBER(HTTGenMatchSelector<T>, std::string, output_vec_label)
  CLASS_MEMBER(HTTGenMatchSelector<T>, ic::mcorigin, gen_match)
  CLASS_MEMBER(HTTGenMatchSelector<T>, bool, add_gen)
 public:
  HTTGenMatchSelector(std::string const& name);
  virtual ~HTTGenMatchSelector();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};
  
template <class T>
HTTGenMatchSelector<T>::HTTGenMatchSelector(std::string const& name) : ModuleBase(name), gen_match_(mcorigin::tauHad) {
  input_vec_label_  = "taus";
  output_vec_label_ = "genmatched_taus";
  add_gen_ = false;
}

template <class T>
HTTGenMatchSelector<T>::~HTTGenMatchSelector() {
  ;
}
template <class T>
int HTTGenMatchSelector<T>::Execute(TreeEvent *event) {
  std::vector<T*> input_vec = event->GetPtrVec<T>(input_vec_label_);
  std::vector<T*> output_vec;
  std::vector<ROOT::Math::PtEtaPhiEVector> output_vec_gen;
  
  std::vector<GenJet *> gen_taus_ptr;
  std::vector<GenParticle *> sel_particles;
  
  if(event->Exists("genHadTaus") && event->Exists("genLeps")){
    // We don't want to have to find leptons/tau jets more than once so these will be added to the event the first time this module is run and reused when called multiple times  
    gen_taus_ptr = event->GetPtrVec<GenJet>("genHadTaus");    
    sel_particles = event->GetPtrVec<GenParticle>("genLeps");
  }
  else {
    std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("genParticles");
    for (unsigned i=0; i < particles.size(); ++i){
      std::vector<bool> status_flags_start = particles[i]->statusFlags();
      if ( ((abs(particles[i]->pdgid()) == 11 )||(abs(particles[i]->pdgid()) == 13)) && particles[i]->pt() > 8. && (status_flags_start[IsPrompt] || status_flags_start[IsDirectPromptTauDecayProduct])){
        sel_particles.push_back(particles[i]);
      }
    }
    
    std::vector<GenJet> gen_taus = BuildTauJets(particles, false,true);
    for (auto & x : gen_taus) gen_taus_ptr.push_back(new GenJet(x));
    ic::erase_if(gen_taus_ptr, !boost::bind(MinPtMaxEta, _1, 15.0, 999.));
    
    event->Add("genHadTaus", gen_taus_ptr);
    event->Add("genLeps", sel_particles);
  }

  
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
      if (add_gen_) {
        if(tausize!=0) output_vec_gen.push_back(tau_match.at(0).second->vector()); 
        else if(leptonsize!=0) output_vec_gen.push_back(lepton_match.at(0).second->vector()); 
      } 
    }
  }
  event->Add(output_vec_label_, output_vec);
  if (add_gen_) event->Add(output_vec_label_+"_gen", output_vec_gen);

  return 0;
}
template <class T>
int HTTGenMatchSelector<T>::PreAnalysis() {
  return 0;
}
template <class T>
int HTTGenMatchSelector<T>::PostAnalysis() {
  return 0;
}
template <class T>
void HTTGenMatchSelector<T>::PrintInfo() {
  ;
}
  

}

#endif
