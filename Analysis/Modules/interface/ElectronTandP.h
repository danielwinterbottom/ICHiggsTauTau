#ifndef ICHiggsTauTau_Module_ElectronTandP_h
#define ICHiggsTauTau_Module_ElectronTandP_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "boost/function.hpp"

#include <string>

namespace ic {


class ElectronTandP : public ModuleBase {
 private:
  std::string input_name_;
  fwlite::TFileService *fs_;
  DynamicHistoSet *hists_;

  std::string output_name_;
  double min_mass_;
  double max_mass_;
  unsigned hist_bins_;
  boost::function<bool (Electron const*)> id_predicate_;
  boost::function<bool (Electron const*)> iso_predicate_;


 
 public:

  ElectronTandP(std::string const& name);
  virtual ~ElectronTandP();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  ElectronTandP & set_input_name(std::string const& input_name) {
    input_name_ = input_name;
    return *this;
  }
  ElectronTandP & set_output_name(std::string const& output_name) {
    output_name_ = output_name;
    return *this;
  }
  ElectronTandP & set_min_mass(double const& min_mass) {
    min_mass_ = min_mass;
    return *this;
  }
  ElectronTandP & set_max_mass(double const& max_mass) {
    max_mass_ = max_mass;
    return *this;
  }
  ElectronTandP & set_hist_bins(unsigned const& hist_bins) {
    hist_bins_ = hist_bins;
    return *this;
  }
  ElectronTandP & set_id_predicate(boost::function<bool (Electron const*)> id_predicate) {
    id_predicate_ = id_predicate;
    return *this;
  }
  ElectronTandP & set_iso_predicate(boost::function<bool (Electron const*)> iso_predicate) {
    iso_predicate_ = iso_predicate;
    return *this;
  }

};

}

#endif
