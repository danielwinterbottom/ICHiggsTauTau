#include <iostream>
#include <vector>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTStatTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTPlotTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/mssm_xs_tools.h"

namespace po = boost::program_options;

using namespace std;
using namespace ic;

HTTAnalysis::HistValuePair FillHistValuePair(HTTSetup setup) {
  return std::make_pair(
      setup.GetShape(), 
      std::make_pair(setup.GetRate(), setup.GetUncertainty()));
}


int main(int argc, char* argv[]){
  string channel        = "";
  string selection      = "";
  string eras           = "";
  string pulls_file     = "";
  string datacard_path  = "";
  string root_file_path = "";
  string signal_mass    = "";
  bool mssm             = false;
  bool splusb_pulls     = false;
  po::variables_map vm;
  po::options_description config("Configuration");
  config.add_options()
    ("help,h",  "print the help message")
    ("channel",              po::value<string>(&channel)->required(),         "[REQUIRED] channel, choose one of [et,mt,em,tt]")
    ("selection",            po::value<string>(&selection)->required(),       "[REQUIRED] categories to combine with label, e.g. \"0-jet:0+1\" combines the 0-jet low and high categories")
    ("eras",                 po::value<string>(&eras)->required(),            "[REQUIRED] data-taking periods to combine, e.g. \"7TeV,8TeV\"")
    ("datacard_path",        po::value<string>(&datacard_path)->required(),   "[REQUIRED] path to the folder containing datacard *.txt files")
    ("root_file_path",       po::value<string>(&root_file_path)->required(),  "[REQUIRED] path to the folder containing datacard *.root files")
    ("pulls_file",           po::value<string>(&pulls_file)->required(),      "[REQUIRED] path to the file containing the pulls from a maximum-likelihood fit")
    ("signal_mass",          po::value<string>(&signal_mass)->required(),     "[REQUIRED] path to the file containing the pulls from a maximum-likelihood fit")
    ("mssm",                 po::value<bool>(&mssm)->default_value(false),    "input is an MSSM datacard")
    ("splusb_pulls",         po::value<bool>(&splusb_pulls)->default_value(false), "Use s+b pulls instead of b-only");
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::notify(vm);


  vector<string> signal_procs = {"ggH", "qqH", "VH"};
  if (mssm) signal_procs = {"ggH","bbH"};


  vector<string> v_eras;
  boost::split(v_eras, eras, boost::is_any_of(","));

  pair<string,vector<string>> v_columns;
  vector<string> tmp_split;
  boost::split(tmp_split, selection, boost::is_any_of(":"));
  if (tmp_split.size() == 2) {
    vector<string> tmp_cats;
    boost::split(tmp_cats, tmp_split[1], boost::is_any_of("+"));
    v_columns = make_pair(tmp_split[0],tmp_cats);
  }
  std::cout << "------ Scale factors for " << channel << ", " << eras << ", " << selection << std::endl;

  // string tanb = plot.draw_signal_tanb();
  HTTSetup setup;
  for (unsigned j = 0; j < v_eras.size(); ++j) {
    for (unsigned k = 0; k < v_columns.second.size(); ++k) {
      string cat = v_columns.second[k];
      setup.ParseDatacard(datacard_path+"/"+"htt_"+channel+"_"+cat+"_"+v_eras[j]+".txt", channel, boost::lexical_cast<int>(cat), v_eras[j], signal_mass);        
    }
  }
  for (unsigned i = 0; i < v_eras.size(); ++i) {
    if (!mssm) {
      setup.ParseROOTFile(root_file_path+"/"+"htt_"+channel+".input_"+v_eras[i]+".root", channel, v_eras[i]);
    } else {
      setup.ParseROOTFile(root_file_path+"/"+"htt_"+channel+".inputs-mssm-"+v_eras[i]+"-0.root", channel, v_eras[i]);
    }
  }
  setup.ParsePulls(pulls_file);

  // HTTAnalysis::HistValueMap hmap;

  // hmap["data_obs"] = make_pair(setup.GetObservedShape(), 
  //   make_pair(setup.GetObservedRate(), sqrt(setup.GetObservedRate())));
  // HTTAnalysis::PrintValue("data_obs", hmap["data_obs"].second);

  vector<string> samples;
  if (channel != "em") {
    samples = {"ZTT","QCD","W","ZL","ZJ","VV","TT"};
  } else {
    samples = {"Ztt","Fakes","EWK","ttbar"};
  }
  vector<double> rate_prefit(samples.size(),0.);
  for (unsigned i = 0; i < samples.size(); ++i) {
    rate_prefit[i] = setup.process({samples[i]}).GetRate();
  }
  double err_band = setup.process(samples).GetUncertainty()/setup.process(samples).GetRate();
  std::cout << "Prefit error band: " << err_band << std::endl;
  setup.ApplyPulls(!splusb_pulls);
  std::cout << "--shift_backgrounds=";
  for (unsigned i = 0; i < samples.size(); ++i) {
    std::cout << samples[i] << ":" << setup.process({samples[i]}).GetRate()/rate_prefit[i];
    if (i != samples.size()-1) std::cout << ",";
  }
  std::cout << " --auto_error_band=";
  err_band = setup.process(samples).GetUncertainty()/setup.process(samples).GetRate();
  std::cout << err_band << std::endl;


 

  return 0;
}

