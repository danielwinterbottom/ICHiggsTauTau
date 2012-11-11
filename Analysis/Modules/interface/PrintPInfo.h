#ifndef ICHiggsTauTau_Module_PrintPInfo_h
#define ICHiggsTauTau_Module_PrintPInfo_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>
#include <fstream>
#include "TRandom3.h"


namespace ic {


class PrintPInfo : public ModuleBase {
 private:
  std::ofstream output;

  CLASS_MEMBER(PrintPInfo, std::string, outname)
  
 public:

  PrintPInfo(std::string const& name);
  virtual ~PrintPInfo();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
