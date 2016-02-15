#ifndef ICHiggsTauTau_HiggsTauTau_WMuNuCategories_h
#define ICHiggsTauTau_HiggsTauTau_WMuNuCategories_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HTTPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"

#include <string>

namespace ic {

class WMuNuCategories : public ModuleBase {

 private:
  CLASS_MEMBER(WMuNuCategories, std::string, muon_label)
  CLASS_MEMBER(WMuNuCategories, std::string, met_label)
  CLASS_MEMBER(WMuNuCategories, std::string, jets_label)
  CLASS_MEMBER(WMuNuCategories, ic::channel, channel)
  CLASS_MEMBER(WMuNuCategories, ic::era, era)
  CLASS_MEMBER(WMuNuCategories, ic::strategy, strategy)
  CLASS_MEMBER(WMuNuCategories, bool, write_tree)
  CLASS_MEMBER(WMuNuCategories, bool, is_data)
  CLASS_MEMBER(WMuNuCategories, fwlite::TFileService*, fs)
 
  TTree *outtree_;
  TTree *synctree_;
  TFile *lOFile;

  struct branch_var {
      double var_double;  
      float var_float;
      branch_var& operator=(double doub){
          var_double = doub;
          var_float = static_cast<float>(doub);
          return *this;
      }
      branch_var& operator*(double sf){
          var_double*=sf;
          var_float*=sf;
          return *this;
      }
  };

  // Event Properties
  branch_var wt_;
  int run_;
  unsigned long long event_;
  int lumi_;
  float rho_;
  unsigned n_vtx_;
  branch_var mt_1_;
  float d0_1_;
  float dz_1_;
  branch_var pt_1_;
  branch_var eta_1_;
  float phi_1_;
  int q_1_;
  branch_var iso_1_;
  bool antiele_1_;
  bool antimu_1_;
  float m_1_;
  float mva_1_;
  branch_var mvamet_;
  branch_var mvamet_phi_;
  float pfmet_;
  float pfmet_phi_;
  float mvametCov00_;
  float mvametCov01_;
  float mvametCov10_;
  float mvametCov11_;
  float pfmetCov00_;
  float pfmetCov01_;
  float pfmetCov10_;
  float pfmetCov11_;


  unsigned n_jets_;
  unsigned n_prebjets_;
  unsigned n_taus_;
  unsigned n_bjets_;
  unsigned n_loose_bjets_;
  branch_var jpt_1_;     // Defined if n_jets >= 1
  branch_var jpt_2_;     // Defined if n_jets >= 2
  branch_var jeta_1_;    // Defined if n_jets >= 1
  branch_var jeta_2_;    // Defined if n_jets >= 2
  float jphi_1_;    // Defined if n_jets >= 1
  float jphi_2_;    // Defined if n_jets >= 2
  float jptraw_1_; 
  float jrawf_1_;
  float jptraw_2_; 
  float jrawf_2_;
  float jptunc_1_; 
  float jptunc_2_; 
  float jmva_1_; 
  float jmva_2_; 
  float jlrm_1_; 
  float jlrm_2_; 
  float jctm_1_; 
  float jctm_2_; 
  branch_var bpt_1_;     // Defined if n_bjets >= 1
  branch_var beta_1_;    // Defined if n_bjets >= 1
  float bphi_1_;    // Defined if n_bjets >= 1
  float bmva_1_;
  float brawf_1_;
  branch_var bcsv_1_; 
  branch_var bpt_2_;     // Defined if n_bjets >= 2
  branch_var beta_2_;    // Defined if n_bjets >= 2
  float bphi_2_;    // Defined if n_bjets >= 2
  float bmva_2_;
  float bcsv_2_; 
  float brawf_2_;
  double jet_csvpt_1_;     // Defined if n_jets >= 1
  double jet_csvEt_1_;     // Defined if n_jets >= 1
  double jet_csvpt_2_;     // Defined if n_jets >= 2
  double jet_csvpt_bb_;
  double jet_csv_dR_;
  double jet_csveta_1_;    // Defined if n_jets >= 1
  double jet_csveta_2_;    // Defined if n_jets >= 2
  double jet_csvbcsv_1_; 
  double jet_csvbcsv_2_; 

  int j1_dm_;

  branch_var mjj_;       // Defined if n_jets >= 2
  branch_var jdeta_;     // Defined if n_jets >= 2
  float jdphi_;



 public:
  WMuNuCategories(std::string const& name);
  virtual ~WMuNuCategories();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();






};

}


#endif
