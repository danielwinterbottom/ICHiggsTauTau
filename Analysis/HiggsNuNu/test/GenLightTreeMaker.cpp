#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/format.hpp"
#include "boost/algorithm/string.hpp"
#include "TSystem.h"
// #include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/AnalysisBase.h"

#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/LightTreeMCGen.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvConfig.h"


using boost::lexical_cast;
using boost::bind;
namespace po = boost::program_options;
using std::string;
using std::vector;
using namespace ic;
int main(int argc, char* argv[]){
  
  // Configurable parameters
  string cfg;                     // The configuration file
  int max_events;                 // Maximum number of events to process
  string filelist;                // The file containing a list of files to use as input
  string input_prefix;            // A prefix that will be added to the path of each input file
  string output_name;             // Name of the ouput ROOT File
  string output_folder;           // Folder to write the output in

  string era_str;                 // Analysis data-taking era
  string mc_str;                  // Analysis MC production
  string prod;                    // Our prdocution string

 // Load the config
  po::options_description preconfig("Pre-Configuration");
  preconfig.add_options()("cfg", po::value<std::string>(&cfg)->required());
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
    ("max_events",          po::value<int>(&max_events)-> default_value(-1))
    ("filelist",            po::value<string>(&filelist)->required())
    ("input_prefix",        po::value<string>(&input_prefix)->default_value(""))
    ("output_name",         po::value<string>(&output_name)->required())
    ("output_folder",       po::value<string>(&output_folder)->default_value(""))
    ("era",                 po::value<string>(&era_str)->required())
    ("mc",                  po::value<string>(&mc_str)->required())
    ("prod",                po::value<string>(&prod)->required())
    ;

  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
  po::notify(vm);

  // Some options must now be re-configured based on other options
  //ic::era era           = String2Era(era_str);
  //ic::mc mc             = String2MC(mc_str);

  std::cout << "**** HiggsNuNu Analysis *****" << std::endl;
  string param_fmt = "%-25s %-40s\n";
  std::cout << boost::format(param_fmt) % "max_events" % max_events;
  std::cout << boost::format(param_fmt) % "output" % (output_folder+output_name);
  //std::cout << boost::format(param_fmt) % "era" % era_str;
  //std::cout << boost::format(param_fmt) % "mc" % mc_str;
  std::cout << boost::format(param_fmt) % "prod" % prod;

  // Load necessary libraries for ROOT I/O of custom classes
  // gSystem->Load("libFWCoreFWLite.dylib");
  // gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  // AutoLibraryLoader::enable();

  // Build a vector of input files
  vector<string> files = ParseFileLines(filelist);
  if (files.size()==0) {
    std::cout << " -- ERROR ! No input file provided, filelist " << filelist << " doesn't exist or is empty, please check." << std::endl;
    return 1;
  }
  for (unsigned i = 0; i < files.size(); ++i) files[i] = input_prefix + files[i];
  
  // Create ROOT output fileservice
  std::ofstream checkfile;
  checkfile.open((output_folder+"tmp.txt").c_str());
  if(!(checkfile.good())){
    std::cout<<"OUTPUT FOLDER DOESN'T EXIST: EXITING"<<std::endl;
    return 1;
  }
  checkfile.close();

  //rename with _wstream.root properly
  std::string fullpath = output_folder+output_name;
  fwlite::TFileService *fs = new fwlite::TFileService(fullpath.c_str());
  
  // Create analysis object
  ic::AnalysisBase analysis(
    "HiggsNuNu",        // Analysis name
    files,                // Input files
    "icEventProducer/EventTree", // TTree name
    max_events);          // Max. events to process (-1 = all)
  
  // ------------------------------------------------------------------------------------
  // Plot Modules
  // ------------------------------------------------------------------------------------  

  LightTreeMCGen lightTreeGen = LightTreeMCGen("LightTreeGen")
    .set_fs(fs);

  // ------------------------------------------------------------------------------------
  // Build Analysis Sequence
  // ------------------------------------------------------------------------------------  

  analysis.AddModule(&lightTreeGen);  

  // Run analysis
  analysis.RetryFileAfterFailure(60,5);// int <pause between attempts in seconds>, int <number of retry attempts to make> );
  analysis.StopOnFileFailure(true);
  analysis.SetTTreeCaching(true); 
  
  analysis.RunAnalysis();
  delete fs;
  return 0;

}




