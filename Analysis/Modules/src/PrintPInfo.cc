#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/PrintPInfo.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"

#include "TRandom3.h"
#include <fstream>

namespace ic {

  PrintPInfo::PrintPInfo(std::string const& name) : ModuleBase(name) {
    outname_ = "default.txt";
  }

  PrintPInfo::~PrintPInfo() {
    ;
  }

  int PrintPInfo::PreAnalysis() {
    output.open(outname_.c_str());
    return 0;
  }

  int PrintPInfo::Execute(TreeEvent *event) {
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    //unsigned run = eventInfo->run();
    int evt = eventInfo->event();
    //output << run << "     " << evt << " " << std::endl;
    output << evt << std::endl;
    return 0;
  }

  int PrintPInfo::PostAnalysis() {
    output.close();
    return 0;
  }

  void PrintPInfo::PrintInfo() {
    ;
  }


}
