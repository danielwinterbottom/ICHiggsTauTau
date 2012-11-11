#ifndef ICHiggsTauTau_Module_GenerateTauMVA_h
#define ICHiggsTauTau_Module_GenerateTauMVA_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "boost/filesystem.hpp"

#include <string>
#include <fstream>

namespace ic {

class GenerateTauMVA : public ModuleBase {
 private:
  CLASS_MEMBER(GenerateTauMVA, std::string, outname)
  std::ofstream outFile;


 public:
  GenerateTauMVA(std::string const& name);
  virtual ~GenerateTauMVA();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
