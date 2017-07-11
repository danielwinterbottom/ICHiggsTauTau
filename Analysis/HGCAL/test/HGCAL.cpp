#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Core/interface/AnalysisBase.h"
#include "Objects/interface/Muon.hh"
#include "Objects/interface/Unhash.h"
#include "Utilities/interface/JsonTools.h"
#include "Utilities/interface/FnRootTools.h"
#include "Modules/interface/GenericModule.h"
#include "HGCAL/interface/HGCALAnalysis.h"

using std::string;
using std::vector;
using std::set;

// Stolen from:
// http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
bool exists(const std::string& name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

class Sequence {
 private:
  std::vector<std::shared_ptr<ic::ModuleBase>> seq;

 public:
  typedef std::vector<std::shared_ptr<ic::ModuleBase>> ModuleSequence;
  Sequence() = default;
  ~Sequence() = default;

  template<class T>
  void BuildModule(T const& mod) {
     seq.push_back(std::shared_ptr<ic::ModuleBase>(new T(mod)));
  }

  void InsertSequence(std::string name, ic::AnalysisBase & ana) {
    for (auto m : seq) {
      ana.AddModule(name, m.get());
    }
  }
};


int main(int argc, char* argv[]) {
  Json::Value js = ic::MergedJson(argc, argv);

  vector<string> files = {
    "root://eoscms.cern.ch//eos/cms/store/cmst3/group/hgcal/CMG_studies/Production/Pythia8PtGun_agilbert_JetPt30_20170710/NTUP/partGun_PDGid1_x100_Pt30.0To30.0_NTUP_1.root"
  };

  ic::AnalysisBase analysis("HGCAL", files, "ana/hgc", 5);
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);

  Sequence main_seq;
  main_seq.BuildModule(ic::HGCALTest("HGCALTest"));

  main_seq.InsertSequence("Main", analysis);

  analysis.RunAnalysis();
}
