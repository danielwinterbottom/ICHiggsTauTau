#ifndef ICHiggsTauTau_Module_HTTFourTauSelector_h
#define ICHiggsTauTau_Module_HTTFourTauSelector_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTConfig.h"


#include <string>

namespace ic {
  
  bool SortBySumPt(CompositeCandidate const* c1, CompositeCandidate const* c2);
  bool SortByPt(Candidate const* c1, Candidate const* c2);
  bool SortByIsoET(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, EventInfo const* eventInfo, std::string tau_idiso_name);
  bool SortByIsoMT(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, std::string tau_idiso_name);
  bool SortByIsoEM(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, EventInfo const* eventInfo);
  bool SortByIsoTT(CompositeCandidate const* c1, CompositeCandidate const* c2, std::string tau_idiso_name);
  bool SortByIsoETTT(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, EventInfo const* eventInfo, std::string tau_idiso_name);
  bool SortByIsoEETT(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, EventInfo const* eventInfo, std::string tau_idiso_name);
  bool SortByIsoMTTT(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, std::string tau_idiso_name);
  bool SortByIsoMMTT(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, std::string tau_idiso_name);
  bool SortByIsoTTTT(CompositeCandidate const* c1, CompositeCandidate const* c2, std::string tau_idiso_name);
  bool SortByIsoEMTT(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, EventInfo const* eventInfo,  std::string tau_idiso_name);


class HTTFourTauSelector : public ModuleBase {
 private:
  CLASS_MEMBER(HTTFourTauSelector, ic::channel, channel)
  CLASS_MEMBER(HTTFourTauSelector, std::string, pair_label)
  CLASS_MEMBER(HTTFourTauSelector, ic::strategy, strategy) 
  CLASS_MEMBER(HTTFourTauSelector, bool, use_most_isolated)
  CLASS_MEMBER(HTTFourTauSelector, bool, use_charge_preference)
  CLASS_MEMBER(HTTFourTauSelector, std::string, allowed_tau_modes)
  CLASS_MEMBER(HTTFourTauSelector, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTFourTauSelector, ic::mc, mc)
  CLASS_MEMBER(HTTFourTauSelector, unsigned, metcl_mode)
  CLASS_MEMBER(HTTFourTauSelector, unsigned, metuncl_mode)
  std::vector<Dynamic2DHistoSet *> hists_;
  std::set<int> tau_mode_set_;

 public:
  HTTFourTauSelector(std::string const& name);
  void CorrectMETForShift(ic::Met * met, ROOT::Math::PxPyPzEVector const& shift);
  virtual ~HTTFourTauSelector();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  std::string tau_idiso_name_;
  

};

}

#endif
