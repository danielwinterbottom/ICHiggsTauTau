#ifndef ICHiggsTauTau_Module_GenLevelStudy_h
#define ICHiggsTauTau_Module_GenLevelStudy_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include <string>


namespace ic {

class GenLevelStudy : public ModuleBase {
 private:

 	DynamicHistoSet *hset;



  CLASS_MEMBER(GenLevelStudy, fwlite::TFileService*, fs)
  CLASS_MEMBER(GenLevelStudy, bool, make_plots)
  CLASS_MEMBER(GenLevelStudy, bool, check_match)
  CLASS_MEMBER(GenLevelStudy, std::string, ditau_label)
  CLASS_MEMBER(GenLevelStudy, std::string, met_label)
  CLASS_MEMBER(GenLevelStudy, int, count_match_any)
  CLASS_MEMBER(GenLevelStudy, int, count_b1)
  CLASS_MEMBER(GenLevelStudy, int, count_b2)
  //CLASS_MEMBER(GenLevelStudy, int, count_t1)
  //CLASS_MEMBER(GenLevelStudy, int, count_t2)
  CLASS_MEMBER(GenLevelStudy, int, count_3)
  CLASS_MEMBER(GenLevelStudy, int, count_4)
  CLASS_MEMBER(GenLevelStudy, int, nev)

  bool os_;
  double mt_1_;
  bool match_b_1_;
  bool match_b_2_;
  bool match_t_1_;
  bool match_t_2_;
  bool match_any_jet_1_;
  bool match_any_jet_2_;

 public:
  GenLevelStudy(std::string const& name);
  virtual ~GenLevelStudy();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}


#endif
