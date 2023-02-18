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
#include <TSpline.h>

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
  CLASS_MEMBER(HTTWeights, bool, do_ggZZ_k_fact)
  CLASS_MEMBER(HTTWeights, bool, do_qqZZ_k_fact)

  std::shared_ptr<RooWorkspace> w_;
  std::map<std::string, std::shared_ptr<RooFunctor>> fns_;

 public:
  HTTWeights(std::string const& name);
  virtual ~HTTWeights();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  TFile *fin = TFile::Open("input/k_factors/Kfactor_Collected_ggHZZ_2l2l_NNLO_NNPDF_NarrowWidth_13TeV.root");
  TSpline3* ggZZ_kf_Nominal = (TSpline3*)fin->Get("sp_kfactor_Nominal");
  TSpline3* ggZZ_kf_PDFScaleDn = (TSpline3*)fin->Get("sp_kfactor_PDFScaleDn");
  TSpline3* ggZZ_kf_PDFScaleUp = (TSpline3*)fin->Get("sp_kfactor_PDFScaleUp");
  TSpline3* ggZZ_kf_QCDScaleDn = (TSpline3*)fin->Get("sp_kfactor_QCDScaleDn");
  TSpline3* ggZZ_kf_QCDScaleUp = (TSpline3*)fin->Get("sp_kfactor_QCDScaleUp");
  TSpline3* ggZZ_kf_AsDn = (TSpline3*)fin->Get("sp_kfactor_AsDn");
  TSpline3* ggZZ_kf_AsUp = (TSpline3*)fin->Get("sp_kfactor_AsUp");
  TSpline3* ggZZ_kf_PDFReplicaDn = (TSpline3*)fin->Get("sp_kfactor_PDFReplicaDn");
  TSpline3* ggZZ_kf_PDFReplicaUp = (TSpline3*)fin->Get("sp_kfactor_PDFReplicaUp");
};

}

#undef MEMBER_NP

#endif
