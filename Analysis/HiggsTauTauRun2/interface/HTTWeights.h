#ifndef ICHiggsTauTau_Module_HTTWeights_h
#define ICHiggsTauTau_Module_HTTWeights_h

#include "UserCode/ICHiggsTauTau/interface/TH2DAsymErr.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagWeight.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"
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
  CLASS_MEMBER(HTTWeights, bool, do_etau_fakerate)
  CLASS_MEMBER(HTTWeights, bool, do_mtau_fakerate)
  CLASS_MEMBER(HTTWeights, bool, do_idiso_weights)
  CLASS_MEMBER(HTTWeights, bool, do_id_weights)
  CLASS_MEMBER(HTTWeights, bool, do_zpt_weight)
  CLASS_MEMBER(HTTWeights, bool, do_tracking_eff)
  CLASS_MEMBER(HTTWeights, bool, do_topquark_weights)
  CLASS_MEMBER(HTTWeights, bool, do_tau_fake_weights)
  CLASS_MEMBER(HTTWeights, std::string, gen_tau_collection)
  CLASS_MEMBER(HTTWeights, std::string, jets_label)
  CLASS_MEMBER(HTTWeights, std::string, ditau_label)
  CLASS_MEMBER(HTTWeights, bool, do_tau_id_weights)
  CLASS_MEMBER(HTTWeights, bool, do_tau_id_sf)
  CLASS_MEMBER(HTTWeights, bool, do_em_qcd_weights)
  CLASS_MEMBER(HTTWeights, std::string, scalefactor_file)
  CLASS_MEMBER(HTTWeights, std::string, scalefactor_file_ggh)
  CLASS_MEMBER(HTTWeights, std::string, mssm_higgspt_file)
  CLASS_MEMBER(HTTWeights, bool, do_mssm_higgspt)
  CLASS_MEMBER(HTTWeights, bool, do_quarkmass_higgspt)
  CLASS_MEMBER(HTTWeights, bool, do_ps_weights)
  CLASS_MEMBER(HTTWeights, std::string, mssm_mass)
  CLASS_MEMBER(HTTWeights, bool, is_embedded)
  CLASS_MEMBER(HTTWeights, bool, do_nnlops_weights)

  TFile *ggh_weights_;
  TGraph *ggh_mg_0jet_;
  TGraph *ggh_mg_1jet_;
  TGraph *ggh_mg_2jet_;
  TGraph *ggh_mg_3jet_;
  TGraph *ggh_ph_0jet_;
  TGraph *ggh_ph_1jet_;
  TGraph *ggh_ph_2jet_;
  TGraph *ggh_ph_3jet_;
  std::shared_ptr<RooWorkspace> w_;
  std::shared_ptr<RooWorkspace> w_ggh_;
  std::shared_ptr<RooWorkspace> mssm_w_;
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
