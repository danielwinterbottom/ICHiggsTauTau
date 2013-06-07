#ifndef ICHiggsTauTau_Module_TauIDCategories_h
#define ICHiggsTauTau_Module_TauIDCategories_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HTTPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"

#include <string>

namespace ic {

class TauIDCategories : public ModuleBase {

 private:
  std::string jets_label_;
  CLASS_MEMBER(TauIDCategories, std::string, ditau_label)
  CLASS_MEMBER(TauIDCategories, std::string, met_label)
  CLASS_MEMBER(TauIDCategories, double, mass_shift)
  CLASS_MEMBER(TauIDCategories, ic::channel, channel)
  CLASS_MEMBER(TauIDCategories, ic::era, era)
  CLASS_MEMBER(TauIDCategories, ic::strategy, strategy)
  // CLASS_MEMBER(TauIDCategories, bool, distinguish_os)
  CLASS_MEMBER(TauIDCategories, fwlite::TFileService*, fs)

  TTree *outtree_;

  // Event Properties
  bool os_;

  double wt_;

  unsigned n_vtx_;
  double m_sv_;
  double m_vis_;
  double pt_h_;
  double pt_tt_;
  double mt_1_;
  double pzeta_;
  double pt_1_;
  double pt_2_;
  double eta_1_;
  double eta_2_;
  double tau_iso_;
  double z_2_;
  double m_2_;
  double met_;
  double met_phi_;

  int    tau_decay_mode_;

  unsigned n_jets_;
  unsigned n_lowpt_jets_;
  unsigned n_bjets_;
  unsigned n_loose_bjets_;
  unsigned n_jetsingap_; // Defined if n_jets >= 2
  double jpt_1_;     // Defined if n_jets >= 1
  double jpt_2_;     // Defined if n_jets >= 2
  double jeta_1_;    // Defined if n_jets >= 1
  double jeta_2_;    // Defined if n_jets >= 2
  double bpt_1_;     // Defined if n_bjets >= 1
  double beta_1_;    // Defined if n_bjets >= 1
  double bcsv_1_; 

  double mjj_;       // Defined if n_jets >= 2
  double jdeta_;     // Defined if n_jets >= 2

  double mjj_lowpt_;       // Defined if n_lowpt_jets >= 2
  double jdeta_lowpt_;     // Defined if n_lowpt_jets >= 2
  unsigned n_jetsingap_lowpt_; // Defined if n_lowpt_jets >= 2
    // Other VBF MVA variables?

 public:
  TauIDCategories(std::string const& name);
  virtual ~TauIDCategories();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}


#endif
