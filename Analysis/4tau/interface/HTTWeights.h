#ifndef ICHiggsTauTau_Module_HTTWeights_h
#define ICHiggsTauTau_Module_HTTWeights_h

#include "UserCode/ICHiggsTauTau/interface/TH2DAsymErr.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagWeight.h"
#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTConfig.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "RooWorkspace.h"
#include "RooFunctor.h"
#include "Utilities/interface/FnRootTools.h"

#include <string>


namespace ic {

class HTTWeights : public ModuleBase {
 private:
  CLASS_MEMBER(HTTWeights, ic::channel, channel)
  CLASS_MEMBER(HTTWeights, ic::mc, mc)
  CLASS_MEMBER(HTTWeights, ic::era, era)
  CLASS_MEMBER(HTTWeights, ic::strategy, strategy)
  CLASS_MEMBER(HTTWeights, bool, do_trg_weights)
  CLASS_MEMBER(HTTWeights, bool, trg_applied_in_mc)
  CLASS_MEMBER(HTTWeights, bool, do_tracking_eff)
  CLASS_MEMBER(HTTWeights, std::string, gen_tau_collection)
  CLASS_MEMBER(HTTWeights, std::string, jets_label)
  CLASS_MEMBER(HTTWeights, std::string, ditau_label)
  CLASS_MEMBER(HTTWeights, bool, do_tau_id_sf)
  CLASS_MEMBER(HTTWeights, std::string, scalefactor_file)
  CLASS_MEMBER(HTTWeights, bool, do_etau_fakerate)
  CLASS_MEMBER(HTTWeights, bool, do_mtau_fakerate)
  CLASS_MEMBER(HTTWeights, bool, do_ditau_trg)
  CLASS_MEMBER(HTTWeights, bool, do_etaucross_trg) 
  CLASS_MEMBER(HTTWeights, bool, do_singlee_trg) 
  CLASS_MEMBER(HTTWeights, bool, do_mtaucross_trg)
  CLASS_MEMBER(HTTWeights, bool, do_singlem_trg) 
  CLASS_MEMBER(HTTWeights, bool, do_emucross_trg)

  std::shared_ptr<RooWorkspace> w_;
  std::map<std::string, std::shared_ptr<RooFunctor>> fns_;

 public:
  HTTWeights(std::string const& name);
  virtual ~HTTWeights();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#undef MEMBER_NP

#endif
