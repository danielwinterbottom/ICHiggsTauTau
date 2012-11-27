#include <iostream>
#include <vector>
#include <string>
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "TSystem.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/AnalysisBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/CopyCollection.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/SimpleFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/OverlapFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/IDOverlapFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/CompositeProducer.h"

#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/PileupWeight.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/EmbeddedStudy.h"





using boost::lexical_cast;
using boost::bind;
namespace po = boost::program_options;

int main(int argc, char* argv[]){
  using namespace ic;

  // Configurable parameters
  int max_events;
  std::string filelist;
  std::string input_path;
  bool is_dyjets;

  // Load the config
  po::variables_map vm;
  po::options_description config("Configuration");
  config.add_options()
      ("max_events", po::value<int>(&max_events)->default_value(-1), "maximum number of events to process")
      ("is_dyjets", po::value<bool>(&is_dyjets)->default_value(false), "processing unbiased DYJetsToLL MC")
      ("input_path", po::value<std::string>(&input_path)->required(), "input_path")
      ("filelist", po::value<std::string>(&filelist)->required(), "input filelist");
  po::store(po::command_line_parser(argc, argv).
            options(config).allow_unregistered().run(), vm);
  po::notify(vm);

  
  // Load necessary libraries for ROOT I/O of custom classes
  gSystem->Load("libFWCoreFWLite.dylib");
  gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  AutoLibraryLoader::enable();
  // Build a vector of input files
  std::vector<std::string> files = ParseFileLines(filelist);
  for (unsigned i = 0; i < files.size(); ++i) {
    files[i] = input_path + files[i];
  }
  // Create ROOT output fileservice
  fwlite::TFileService *fs;
  std::string outname = is_dyjets ? "dyjets_study.root" : "embedded_study.root";
  fs = new fwlite::TFileService(outname.c_str());

  // Set-up pileup reweighting files 
  // 2011
  std::string data_pu_path = "data/pileup/Data_Pileup_2011_HCP-500bins.root";
  std::string mc_pu_path = "data/pileup/MC_Fall11_PU_S6-500bins.root";
  data_pu_path = "data/pileup/Data_Pileup_2012_HCP-600bins.root";
  mc_pu_path = "data/pileup/MC_Summer12_PU_S10-600bins.root";

  TH1D data_pu  = GetFromTFile<TH1D>(data_pu_path, "/", "pileup");
  TH1D mc_pu    = GetFromTFile<TH1D>(mc_pu_path, "/", "pileup");
  

  // Create analysis object
  ic::AnalysisBase analysis(
    "HiggsTauTau",        // Analysis name
    files,                // Input files
    "icEventProducer",    // TTree path
    "EventTree",          // TTree name
    max_events);          // Max. events to process (-1 = all)

 
  PileupWeight pileupWeight = PileupWeight
    ("PileupWeight").set_data(&data_pu).set_mc(&mc_pu).set_print_weights(false);

  EmbeddedStudy embeddedStudy = EmbeddedStudy
    ("EmbeddedStudy").set_fs(fs).set_is_dyjets(is_dyjets);
  

  analysis.AddModule(&pileupWeight);
  analysis.AddModule(&embeddedStudy);

  analysis.RunAnalysis();
  delete fs;
  return 0;
}




