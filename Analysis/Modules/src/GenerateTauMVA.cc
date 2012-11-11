#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/GenerateTauMVA.h"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "boost/lexical_cast.hpp"
#include <boost/algorithm/string.hpp>
#include <stdlib.h>

#include "boost/filesystem.hpp"



namespace ic {

  GenerateTauMVA::GenerateTauMVA(std::string const& name) : ModuleBase(name) {
    //Add option to specify full path

    outname_ = "svfit_test";
  }

  GenerateTauMVA::~GenerateTauMVA() {
    ;
  }

  int GenerateTauMVA::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for GenerateTauMVA" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    std::string sub(".root");
    std::string::size_type foundpos = outname_.find(sub);
    if ( foundpos != std::string::npos )
    outname_.erase(outname_.begin() + foundpos, outname_.begin() + foundpos + sub.length());
    outname_ += "_TAUMVA.txt";
    outFile.open(outname_.c_str());
  return 0;
}

int GenerateTauMVA::Execute(TreeEvent *event) {
  std::vector<Tau *> const& taus = event->GetPtrVec<Tau>("taus");
  EventInfo const* info = event->GetPtr<EventInfo>("eventInfo");
  bool has_failing = false;
  for (unsigned i = 0; i < taus.size(); ++i) {
    if (taus[i]->pt() < 19.0 || taus[i]->eta() > 2.3) continue;
    if (taus[i]->GetTauID("againstElectronTightMVA2") < 0.5) {
      has_failing = true;
      break;
    }
  }
  if (has_failing) {
    outFile << info->run() << "," << info->lumi_block() << "," << info->event() << ":";
    for (unsigned i = 0; i < taus.size(); ++i) {
      if (taus[i]->pt() < 19.0 || taus[i]->eta() > 2.3) continue;
      if (taus[i]->GetTauID("againstElectronTightMVA2") < 0.5) {
        outFile << taus[i]->pt() << ",";
        outFile << taus[i]->eta() << ",";
        outFile << taus[i]->phi() << ":";
      }
    }
  outFile << std::endl;
  }
  return 0;
}
int GenerateTauMVA::PostAnalysis() {

  if (outFile.is_open()) {
    outFile.close();
  }
  return 0;
}

void GenerateTauMVA::PrintInfo() {
  ;
}


}
