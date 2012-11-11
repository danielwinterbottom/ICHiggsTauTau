#ifndef ICHiggsTauTau_Module_HttVbfCategory_h
#define ICHiggsTauTau_Module_HttVbfCategory_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"

#include <string>
#include "TMVA/Reader.h"
#include "TEfficiency.h"

namespace ic {

class HttVbfCategory : public ModuleBase {
 private:

  // Variables for cut-based selection
  double vbf_delta_eta_;
  double vbf_mass_;

  // Variables for mc efficiency
  double n_real_events_;
  double n_pu_events_;
  double n_real_events_pass_;
  double n_pu_events_pass_;

  // For MVA TTree output
  TFile *lOFile;
  TTree *lOTree;
  double weight_;

  double btag_jet_pt_;
  double btag_jet_eta_;


  CLASS_MEMBER(HttVbfCategory, unsigned, mode)
  CLASS_MEMBER(HttVbfCategory, fwlite::TFileService *, fs)

  CLASS_MEMBER(HttVbfCategory, double, jet_pt)
  CLASS_MEMBER(HttVbfCategory, double, jet_eta)
  CLASS_MEMBER(HttVbfCategory, double, vbf_cjv_pt)
  CLASS_MEMBER(HttVbfCategory, double, mva_cut)

  CLASS_MEMBER(HttVbfCategory, double, loose_mjj)
  CLASS_MEMBER(HttVbfCategory, double, loose_delta_eta)
  
  CLASS_MEMBER(HttVbfCategory, bool, do_mva_pu_id)
  CLASS_MEMBER(HttVbfCategory, bool, do_vbf_mva)
  CLASS_MEMBER(HttVbfCategory, bool, do_jetvtx_assoc)
  CLASS_MEMBER(HttVbfCategory, bool, do_mc_eff)
  CLASS_MEMBER(HttVbfCategory, bool, do_cjv)
  CLASS_MEMBER(HttVbfCategory, bool, make_plots)

  CLASS_MEMBER(HttVbfCategory, bool, do_btag_weight)

  
  CLASS_MEMBER(HttVbfCategory, bool, make_mva_tree)
  CLASS_MEMBER(HttVbfCategory, std::string, mva_name)
  
  CLASS_MEMBER(HttVbfCategory, std::string, jets_label)
  CLASS_MEMBER(HttVbfCategory, std::string, met_label)

  CLASS_MEMBER(HttVbfCategory, bool, use_hcp_mva)
  CLASS_MEMBER(HttVbfCategory, bool, is_2012)


  std::vector<HttPlots *> twojet_plots_;
  std::vector<HttPlots *> vbf_plots_;
  std::vector<HttPlots *> vbf_loose_plots_;


  std::vector<double> twojet_yields_;
  std::vector<double> vbf_loose_yields_;
  std::vector<double> vbf_yields_;

  TMVA::Reader *reader;
  std::vector<float> vbfvars;


 public:
  HttVbfCategory(std::string const& name, std::string const& jets_label);
  virtual ~HttVbfCategory();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  void PrintEff(std::string const& name, double const& num, double const& den) {
      TEfficiency eff;
      std::cout << name << ":\t\t" << num << "/" << den << " = " << (num/den) << "\t+/- " << (eff.ClopperPearson(den+0.5,num+0.5,0.683 ,1)-(num/den)) << std::endl;
  }

};

}



#endif
