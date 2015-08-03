#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include "boost/program_options.hpp"
#include "TSystem.h"
#include "Utilities/interface/json.h"
// #include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Utilities/interface/JsonTools.h"
#include "Utilities/interface/FnRootTools.h"
#include "Utilities/interface/FnPredicates.h"
#include "Core/interface/AnalysisBase.h"
#include "Modules/interface/SimpleFilter.h"
#include "Modules/interface/CompositeProducer.h"
#include "HiggsTauTau/interface/HTTConfig.h"
#include "HiggsTauTau/interface/Phys14Plots.h"
#include "HiggsTauTau/interface/ToyTauStudy.h"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include "Modules/interface/JetEnergyCorrections.h"

namespace po = boost::program_options;
using std::string;
using std::vector;

using ic::AnalysisBase;
using std::vector;
using std::string;
using ic::Electron;

int main(int argc, char* argv[]) {
  // vector<string> cfgs;
  // vector<string> jsons;

//   po::options_description config("config");
//   config.add_options()(
//       "cfg", po::value<vector<string>>(&cfgs)->multitoken()->required(),
//       "json config files")(
//       "json", po::value<vector<string>>(&jsons)->multitoken(),
//       "json fragments");
//   po::variables_map vm;
//   po::store(po::command_line_parser(argc, argv).options(config).run(), vm);
//   po::notify(vm);

//   Json::Value js_init = ic::ExtractJsonFromFile(cfgs[0]);
//   for (unsigned i = 1; i < cfgs.size(); ++i) {
//     std::cout << ">> Updating config with file " << cfgs[i] << ":\n";
//     Json::Value extra = ic::ExtractJsonFromFile(cfgs[i]);
//     std::cout << extra;
//     ic::UpdateJson(js_init, extra);
//   }
//   for (unsigned i = 0; i < jsons.size(); ++i) {
//     Json::Value extra;
//     Json::Reader reader(Json::Features::all());
//     reader.parse(jsons[i], extra);
//     std::cout << ">> Updating config with fragment:\n";
//     std::cout << extra;
//     ic::UpdateJson(js_init, extra);
// }

  Json::Value const js = ic::MergedJson(argc, argv);

  // Create ROOT output fileservice
  fwlite::TFileService* fs =
      new fwlite::TFileService((js["job"]["output_folder"].asString() +
                                js["job"]["output_name"].asString()).c_str());

  /*
    This should really be shortened into one inline function in the AnalysisBase
    declaration. GetPrefixedFilelist(prefix, filelist)
  */
  vector<string> files = ic::ParseFileLines(js["job"]["filelist"].asString());
  for (auto & f : files) f = js["job"]["file_prefix"].asString() + f;

  AnalysisBase analysis("HiggsTauTau", files, "icEventProducer/EventTree",
                        js["job"]["max_events"].asInt64());
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);
  // analysis.DoSkimming("./skim/");
  analysis.CalculateTimings(js["job"]["timings"].asBool());

  bool do_XToTauTau = js["do_XToTauTau"].asBool();
  bool do_QCDFakes = js["do_QCDFakes"].asBool();
  bool apply_JEC = js["apply_JEC"].asBool();
  bool do_ToyTaus = js["do_ToyTaus"].asBool();

  string jec_payload = js["jec_payload"].asString();
  auto jetEnergyCorr =
      ic::JetEnergyCorrections<ic::PFJet>("JetEnergyCorrections")
          .set_input_label("pfJetsPFlow")
          .set_is_data(false)
          .set_use_new_mode(true)
          .set_l1_file("input/jec/" + jec_payload + "_L1FastJet_AK5PF.txt")
          .set_l2_file("input/jec/" + jec_payload + "_L2Relative_AK5PF.txt")
          .set_l3_file("input/jec/" + jec_payload + "_L3Absolute_AK5PF.txt")
          .set_res_file("input/jec/" + jec_payload + "_L2L3Residual_AK5PF.txt");

  auto httGenEvent = ic::HTTGenEvent("HttGenEvent")
      .set_genparticle_label("genParticles")
      .set_is_pythia8(js.get("is_pythia8", false).asBool());

  auto phys14Plots =
      ic::Phys14Plots("Phys14Plots")
          .set_fs(fs)
          .set_do_real_th_studies(do_XToTauTau)
          .set_do_fake_th_studies(do_QCDFakes)
          .set_jets_label(js.get("jets_label", "pfJetsPFlow").asString());

  auto recTauStudy = ic::ToyTauStudy("RecTauStudy")
                         .set_fs(fs)
                         .set_do_real_th_studies(do_XToTauTau)
                         .set_do_fake_th_studies(do_QCDFakes)
                         .set_taus_label("taus")
                         .set_pfcands_label("pfCandidates");

  auto toyTauStudy = ic::ToyTauStudy("ToyTauStudy")
                         .set_fs(fs)
                         .set_do_real_th_studies(do_XToTauTau)
                         .set_do_fake_th_studies(do_QCDFakes)
                         .set_taus_label("toyTaus")
                         .set_pfcands_label("genPFCandidates");

  if (apply_JEC) analysis.AddModule(&jetEnergyCorr);
  if (do_XToTauTau) analysis.AddModule(&httGenEvent);
  if (do_ToyTaus) {
    analysis.AddModule(&recTauStudy);
    analysis.AddModule(&toyTauStudy);
  } else {
    analysis.AddModule(&phys14Plots);    
  }

  analysis.RunAnalysis();

  delete fs;

  return 0;
}
