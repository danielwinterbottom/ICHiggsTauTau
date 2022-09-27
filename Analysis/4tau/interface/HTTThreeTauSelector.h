#ifndef ICHiggsTauTau_Module_HTTThreeTauSelector_h
#define ICHiggsTauTau_Module_HTTThreeTauSelector_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTConfig.h"


#include <string>

namespace ic {
  bool SortBySumPt_(CompositeCandidate const* c1, CompositeCandidate const* c2);
  bool SortByPt_(Candidate const* c1, Candidate const* c2); 
  bool SortByIsoTTT(CompositeCandidate const* c1, CompositeCandidate const* c2, std::string tau_idiso_name);



class HTTThreeTauSelector : public ModuleBase {
 private:
  CLASS_MEMBER(HTTThreeTauSelector, ic::channel, channel)
  CLASS_MEMBER(HTTThreeTauSelector, std::string, pair_label)
  CLASS_MEMBER(HTTThreeTauSelector, ic::strategy, strategy) 
  CLASS_MEMBER(HTTThreeTauSelector, bool, use_most_isolated)
  CLASS_MEMBER(HTTThreeTauSelector, bool, use_charge_preference)
  CLASS_MEMBER(HTTThreeTauSelector, std::string, allowed_tau_modes)
  CLASS_MEMBER(HTTThreeTauSelector, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTThreeTauSelector, ic::mc, mc)
  CLASS_MEMBER(HTTThreeTauSelector, unsigned, metcl_mode)
  CLASS_MEMBER(HTTThreeTauSelector, unsigned, metuncl_mode)
  std::vector<Dynamic2DHistoSet *> hists_;
  std::set<int> tau_mode_set_;

 public:
  HTTThreeTauSelector(std::string const& name);
  void CorrectMETForShift(ic::Met * met, ROOT::Math::PxPyPzEVector const& shift);
  virtual ~HTTThreeTauSelector();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  std::string tau_idiso_name_;
  

};

}

#endif
