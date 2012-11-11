#ifndef ICHiggsTauTau_Module_HttOneJetCategory_h
#define ICHiggsTauTau_Module_HttOneJetCategory_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include <string>


namespace ic {

class HttOneJetCategory : public ModuleBase {
 private:
  double jet_pt_;
  double btag_jet_pt_;
  double btag_jet_eta_;

  std::string jets_label_;
  
  CLASS_MEMBER(HttOneJetCategory, bool, do_met_cut)
  CLASS_MEMBER(HttOneJetCategory, double, met_cut)
  CLASS_MEMBER(HttOneJetCategory, bool, high_pt)
  CLASS_MEMBER(HttOneJetCategory, fwlite::TFileService*, fs)
  CLASS_MEMBER(HttOneJetCategory, unsigned, mode)
  CLASS_MEMBER(HttOneJetCategory, std::string, met_label)

  CLASS_MEMBER(HttOneJetCategory, bool, do_btag_weight)

  CLASS_MEMBER(HttOneJetCategory, double, jet_eta);

  
  std::vector<HttPlots *> onejet_plots_;
  std::vector<double> onejet_yields_;

 public:
  HttOneJetCategory(std::string const& name, std::string const& jets_label);
  virtual ~HttOneJetCategory();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}


#endif
