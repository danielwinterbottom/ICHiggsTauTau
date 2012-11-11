#ifndef ICHiggsTauTau_Module_HttVHCategory_h
#define ICHiggsTauTau_Module_HttVHCategory_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"

#include <string>

#define MEMBER_NP(type,name)                                                \
    private:                                                                \
      type name##_;                                                         \
    public:                                                                 \
      virtual HttVHCategory & set_##name(type const& name) {name##_ = name; return *this; }

namespace ic {

class HttVHCategory : public ModuleBase {
 private:
  double jet_pt_;
  double jet_eta_;
  double dijet_mass_min_;
  double dijet_mass_max_;
  double dijet_pt_min_;

  std::string jets_label_;
  MEMBER_NP(fwlite::TFileService*, fs)
  HttPlots *plots_;



 public:
  HttVHCategory(std::string const& name, std::string const& jets_label);
  virtual ~HttVHCategory();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  HttVHCategory & set_jet_pt(double const& jet_pt) { jet_pt_ = jet_pt; return *this; }
  HttVHCategory & set_jet_eta(double const& jet_eta) { jet_eta_ = jet_eta; return *this; }
  HttVHCategory & set_dijet_mass_min(double const& dijet_mass_min) { dijet_mass_min_ = dijet_mass_min; return *this; }
  HttVHCategory & set_dijet_mass_max(double const& dijet_mass_max) { dijet_mass_max_ = dijet_mass_max; return *this; }
  HttVHCategory & set_dijet_pt_min(double const& dijet_pt_min) { dijet_pt_min_ = dijet_pt_min; return *this; }
};

}
#undef MEMBER_NP

#endif
