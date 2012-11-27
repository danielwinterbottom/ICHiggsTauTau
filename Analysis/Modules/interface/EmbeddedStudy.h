#ifndef ICHiggsTauTau_Module_EmbeddedStudy_h
#define ICHiggsTauTau_Module_EmbeddedStudy_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include <string>


namespace ic {

class EmbeddedStudy : public ModuleBase {
 private:

 	DynamicHistoSet *hset;
 	Dynamic2DHistoSet *h2dset;



  CLASS_MEMBER(EmbeddedStudy, fwlite::TFileService*, fs)
  CLASS_MEMBER(EmbeddedStudy, bool, is_dyjets)


 public:
  EmbeddedStudy(std::string const& name);
  virtual ~EmbeddedStudy();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}


#endif
