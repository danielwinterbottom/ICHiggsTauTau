#ifndef ICHiggsTauTau_Module_QuarkGluonDiscriminatorStudy_h
#define ICHiggsTauTau_Module_QuarkGluonDiscriminatorStudy_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include <string>


namespace ic {

class QuarkGluonDiscriminatorStudy : public ModuleBase {
 private:

 	DynamicHistoSet *hset;
 	Dynamic2DHistoSet *h2dset;



  CLASS_MEMBER(QuarkGluonDiscriminatorStudy, fwlite::TFileService*, fs)
  CLASS_MEMBER(QuarkGluonDiscriminatorStudy, bool, is_dyjets)


 public:
  QuarkGluonDiscriminatorStudy(std::string const& name);
  virtual ~QuarkGluonDiscriminatorStudy();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}


#endif
