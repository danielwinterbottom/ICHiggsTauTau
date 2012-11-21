#include <iostream>
#include <vector>
#include <string>
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "boost/bind.hpp"
#include "TSystem.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/AnalysisBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/LumiMask.h"

using boost::lexical_cast;
namespace po = boost::program_options;


int main(int argc, char* argv[]){
  using namespace ic;

  int max_events;
  std::string filelist, input_path, input_json, output_json;

  po::options_description config("Configuration");
  po::variables_map vm;
  po::notify(vm);
  config.add_options()
      ("max_events", po::value<int>(&max_events)->default_value(-1), "maximum number of events to process")
      ("input_json", po::value<std::string>(&input_json)->required(), "json file input name")
      ("output_json", po::value<std::string>(&output_json)->required(), "json file output name")
      ("input_path", po::value<std::string>(&input_path)->default_value(""), "path to add to the input files in the file list")
      ("filelist", po::value<std::string>(&filelist)->required(), "input filelist");
  po::store(po::command_line_parser(argc, argv).
            options(config).allow_unregistered().run(), vm);
  po::notify(vm);

  std::cout << "-------------------------------------------------------------------" << std::endl;
  std::cout << "Lumi Filter" << std::endl;
  std::cout << "-------------------------------------------------------------------" << std::endl;
  std::cout << "Applying to the file list: " << filelist << std::endl;
  std::cout << "Using files at path: " << input_path << std::endl;
  std::cout << "Input json: " << input_json << std::endl;
  std::cout << "Placing output json in: " << output_json << std::endl;


  gSystem->Load("libFWCoreFWLite.dylib");
  gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  AutoLibraryLoader::enable();
  // Build a vector of input files
  std::vector<std::string> files = ParseFileLines(filelist);
  for (unsigned i = 0; i < files.size(); ++i) {
    files[i] = input_path + files[i];
  }

ic::AnalysisBase analysis(
    "LumiFilter",        // Analysis name
    files,                // Input files
    "icEventProducer",    // TTree path
    "EventTree",
    max_events);          // TTree name
        

  LumiMask lumiMask = LumiMask("LumiMask").set_produce_output_jsons(output_json).set_input_file(input_json);

  analysis.AddModule(&lumiMask);

  analysis.RunAnalysis();
  return 0;

}
