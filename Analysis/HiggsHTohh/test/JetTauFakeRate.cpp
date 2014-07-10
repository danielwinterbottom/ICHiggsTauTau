#include <iostream>
#include <vector>
#include <string>
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/format.hpp"
#include "TSystem.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/AnalysisBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/CopyCollection.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/SimpleFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/OverlapFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsHTohh/interface/JetTauFakeRate.h"

using boost::lexical_cast;
using boost::bind;
namespace po = boost::program_options;
using std::string;
using std::vector;
using namespace ic;

int main(int argc, char* argv[]){

  // Configurable parameters
  int max_events;                 // Maximum number of events to process
  string filelist;                // The file containing a list of files to use as input
  string input_prefix;            // A prefix that will be added to the path of each input file
  string output_name;             // Name of the ouput ROOT File
  string output_folder;           // Folder to write the output in
  
  po::options_description config("Configuration");
  po::variables_map vm;
  po::notify(vm);

  config.add_options()    
      ("max_events",          po::value<int>(&max_events)-> default_value(-1))
      ("filelist",            po::value<string>(&filelist)->required())
      ("input_prefix",        po::value<string>(&input_prefix)->default_value(""))
      ("output_name",         po::value<string>(&output_name)->required())
      ("output_folder",       po::value<string>(&output_folder)->default_value(""))
    ;
    po::store(po::command_line_parser(argc, argv).
              options(config).allow_unregistered().run(), vm);
    po::notify(vm);
  
  
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "JetTauFakeRateStudy" << std::endl;
  std::cout << "-------------------------------------" << std::endl;      string param_fmt = "%-25s %-40s\n";
  
 
  // Load necessary libraries for ROOT I/O of custom classes
  gSystem->Load("libFWCoreFWLite.dylib");
  gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  AutoLibraryLoader::enable();

  // Build a vector of input files
  vector<string> files = ParseFileLines(filelist);
  for (unsigned i = 0; i < files.size(); ++i) files[i] = input_prefix + files[i];
  
  // Create ROOT output fileservice
  fwlite::TFileService *fs = new fwlite::TFileService((output_folder+output_name).c_str());
  
  //Optional configurable parameters with which you can filter the collection you are interested in.
  double jet_pt, jet_eta, tau_pt, tau_eta;
  jet_pt = 20.0;
  jet_eta = 2.3; 
  tau_pt = 20.0;
  tau_eta = 2.3; 
   
  std::cout << "** Kinematics **" << std::endl;
  std::cout << boost::format(param_fmt) % "jet_pt" % jet_pt;
  std::cout << boost::format(param_fmt) % "jet_eta" % jet_eta;
  std::cout << boost::format(param_fmt) % "tau_pt" % tau_pt;
  std::cout << boost::format(param_fmt) % "tau_eta" % tau_eta;
 
 //Defining an analysis using this class removes all the need for any for loops through the events. An "analysis" loops through all the events up to max_events and runs the modules which you define at the end of this script using analysis.AddModule
  
  ic::AnalysisBase analysis(
    "JetTauFakeRateStudy",// Analysis name
    files,                // Input files
    "icEventProducer",    // TTree path
    "EventTree",          // TTree name
    max_events);          // Max. events to process (-1 = all)
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);

  //For now, read in all the jets and taus within the pt and eta range of interest. Using Andrew's modules makes it much easier to 
  //add other things later, if we need a certain type of ID (or flavour selection) applied to the jets/taus
  
  SimpleFilter<Tau> TauFilter = SimpleFilter<Tau>("TauFilter")
  .set_input_label("taus")
  .set_predicate(bind(MinPtMaxEta, _1, tau_pt, tau_eta));
  
  SimpleFilter<PFJet> JetFilter = SimpleFilter<PFJet>("JetFilter")
  .set_input_label("pfJetsPFlow")
  //.set_predicate(bind(MinPtMaxEta, _1, jet_pt, jet_eta)&&(bind(&PFJet::pu_id_mva_loose,_1)));
	.set_predicate(bind(MinPtMaxEta,_1,jet_pt,jet_eta)&&(bind(&PFJet::charged_multiplicity_nopu,_1)>0)&&(bind(&PFJet::charged_had_energy_frac,_1)>0)&&(bind(&PFJet::neutral_had_energy_frac,_1)<0.99)&&(bind(&PFJet::charged_em_energy_frac,_1)<0.99)&&(bind(&PFJet::neutral_em_energy_frac,_1)<0.99)&&((bind(&PFJet::charged_multiplicity_nopu,_1)>1)||(bind(&PFJet::neutral_multiplicity,_1)>0)));

  JetTauFakeRate jetTauFakeRate = JetTauFakeRate("jetTauFakeRate")
  .set_write_plots(true)
  .set_write_tree(false)
	.set_fs(fs);

 //Add module here which reads in the filtered taus and jets and makes the plots/performs the fake rate study
 
  analysis.AddModule(&TauFilter); 
  analysis.AddModule(&JetFilter); 
  analysis.AddModule(&jetTauFakeRate); 


  analysis.RunAnalysis();
  delete fs;
  return 0;
}

