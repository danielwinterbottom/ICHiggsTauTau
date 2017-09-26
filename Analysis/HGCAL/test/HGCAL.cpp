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

  vector<string> files;
  for (auto const& filelist : js["filelists"]) {
    auto i_files = ic::ParseFileLines(filelist.asString());
    files.insert(files.end(), i_files.begin(), i_files.end());
  }
  vector<string> do_files;
  unsigned file_offset = js.get("file_offset", 0).asUInt();
  unsigned file_step = js.get("file_step", 1).asUInt();
  for (unsigned i = file_offset; i < files.size(); i += file_step) {
    do_files.push_back(js.get("file_prefix", "").asString() + files[i]);
  }

  // vector<string> files = {
  //   "root://eoscms.cern.ch//eos/cms/store/cmst3/group/hgcal/CMG_studies/Production/Pythia8PtGun_agilbert_JetPt30_20170710/NTUP/partGun_PDGid1_x100_Pt30.0To30.0_NTUP_1.root",
  //   "root://eoscms.cern.ch//eos/cms/store/cmst3/group/hgcal/CMG_studies/Production/Pythia8PtGun_agilbert_JetPt30_20170710/NTUP/partGun_PDGid1_x100_Pt30.0To30.0_NTUP_2.root",
  //   "root://eoscms.cern.ch//eos/cms/store/cmst3/group/hgcal/CMG_studies/Production/Pythia8PtGun_agilbert_JetPt30_20170710/NTUP/partGun_PDGid1_x100_Pt30.0To30.0_NTUP_3.root",
  //   "root://eoscms.cern.ch//eos/cms/store/cmst3/group/hgcal/CMG_studies/Production/Pythia8PtGun_agilbert_JetPt30_20170710/NTUP/partGun_PDGid1_x100_Pt30.0To30.0_NTUP_4.root",
  //   "root://eoscms.cern.ch//eos/cms/store/cmst3/group/hgcal/CMG_studies/Production/Pythia8PtGun_agilbert_JetPt30_20170710/NTUP/partGun_PDGid1_x100_Pt30.0To30.0_NTUP_5.root",
  //   "root://eoscms.cern.ch//eos/cms/store/cmst3/group/hgcal/CMG_studies/Production/Pythia8PtGun_agilbert_JetPt30_20170710/NTUP/partGun_PDGid1_x100_Pt30.0To30.0_NTUP_6.root",
  //   "root://eoscms.cern.ch//eos/cms/store/cmst3/group/hgcal/CMG_studies/Production/Pythia8PtGun_agilbert_JetPt30_20170710/NTUP/partGun_PDGid1_x100_Pt30.0To30.0_NTUP_7.root",
  //   "root://eoscms.cern.ch//eos/cms/store/cmst3/group/hgcal/CMG_studies/Production/Pythia8PtGun_agilbert_JetPt30_20170710/NTUP/partGun_PDGid1_x100_Pt30.0To30.0_NTUP_8.root",
  //   "root://eoscms.cern.ch//eos/cms/store/cmst3/group/hgcal/CMG_studies/Production/Pythia8PtGun_agilbert_JetPt30_20170710/NTUP/partGun_PDGid1_x100_Pt30.0To30.0_NTUP_9.root",
  //   "root://eoscms.cern.ch//eos/cms/store/cmst3/group/hgcal/CMG_studies/Production/Pythia8PtGun_agilbert_JetPt30_20170710/NTUP/partGun_PDGid1_x100_Pt30.0To30.0_NTUP_10.root"
  // };

  std::string outputdir = "./output";
  std::map<std::string, std::shared_ptr<fwlite::TFileService>> fs;
  for (auto const& seq : {"Main"}) {
    fs[seq] = std::make_shared<fwlite::TFileService>(
        outputdir + "/" + seq + "/" + js["output_name"].asString());
    // fs[seq.asString()] = fwlite::TFileService(js["output_dir"].asString() +
    //                                           "/" + seq.asString() + "/" +
    //                                           js["output_name"].asString());
  }

  ic::AnalysisBase analysis("HGCAL", do_files, "ana/hgc", js["max_events"].asInt64());
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);



  Sequence main_seq;
  main_seq.BuildModule(ic::HGCALTest("HGCALTest")
    .set_fs(fs.at("Main").get()));

  main_seq.InsertSequence("Main", analysis);

  analysis.RunAnalysis();
}
