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
  string cfg;                                   // The configuration file
  string channel        = "";
  string selection      = "";
  string eras           = "";
  string pulls_file     = "";
  string datacard_path  = "";
  string root_file_path = "";
  bool postfit          = true;
  bool mssm             = false;
  po::options_description preconfig("Pre-Configuration");
  preconfig.add_options()("cfg", po::value<std::string>(&cfg)->required());
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
    ("help,h",  "print the help message")
    ("channel",              po::value<string>(&channel)->required(),         "[REQUIRED] channel, choose one of [et,mt,em,tt]")
    ("selection",            po::value<string>(&selection)->required(),       "[REQUIRED] categories to combine with label, e.g. \"0-jet:0+1\" combines the 0-jet low and high categories")
    ("eras",                 po::value<string>(&eras)->required(),            "[REQUIRED] data-taking periods to combine, e.g. \"7TeV,8TeV\"")
    ("datacard_path",        po::value<string>(&datacard_path)->required(),   "[REQUIRED] path to the folder containing datacard *.txt files")
    ("root_file_path",       po::value<string>(&root_file_path)->required(),  "[REQUIRED] path to the folder containing datacard *.root files")
    ("pulls_file",           po::value<string>(&pulls_file)->required(),      "[REQUIRED] path to the file containing the pulls from a maximum-likelihood fit")
    ("postfit",              po::value<bool>(&postfit)->required(),           "[REQUIRED] use the pulls file to make a post-fit plot")
    ("mssm",                 po::value<bool>(&mssm)->default_value(false),                   "input is an MSSM datacard");
  HTTPlot plot;
  config.add(plot.GenerateOptions("")); // The string here is a prefix for the options parameters
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config, true), vm);
  po::notify(vm);


  vector<string> signal_procs = {"ggH", "qqH", "VH"};
  if (mssm) signal_procs = {"ggH","bbH"};


  vector<string> v_eras;
  boost::split(v_eras, eras, boost::is_any_of(","));
  std::string era_file_label;
  for (unsigned i = 0; i < v_eras.size(); ++i) era_file_label += v_eras[i];

  pair<string,vector<string>> v_columns;
  vector<string> tmp_split;
  boost::split(tmp_split, selection, boost::is_any_of(":"));
  if (tmp_split.size() == 2) {
    vector<string> tmp_cats;
    boost::split(tmp_cats, tmp_split[1], boost::is_any_of("+"));
    v_columns = make_pair(tmp_split[0],tmp_cats);
  }

  string signal_mass = plot.draw_signal_mass();
  string tanb = plot.draw_signal_tanb();
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
  if (postfit) setup.ApplyPulls();

  HTTAnalysis::HistValueMap hmap;

  hmap["data_obs"] = make_pair(setup.GetObservedShape(), 
    make_pair(setup.GetObservedRate(), sqrt(setup.GetObservedRate())));
  HTTAnalysis::PrintValue("data_obs", hmap["data_obs"].second);

  vector<string> samples;
  if (channel != "em") {
    samples = {"ZTT","QCD","W","ZL","ZJ","ZLL","VV","TT"};
  } else {
    samples = {"Ztt","Fakes","EWK","ttbar"};
  }
  for (auto const& s : samples) {
    hmap[s] = FillHistValuePair(setup.process({s}));
    HTTAnalysis::PrintValue(s, hmap[s].second);
  }
  for (auto const& s : signal_procs) {
    hmap[s+signal_mass] = FillHistValuePair(setup.process({s}));
    HTTAnalysis::PrintValue(s+signal_mass, hmap[s+signal_mass].second);
  }

  string catstring = "";
  for (unsigned i = 0; i < v_columns.second.size(); ++i) catstring += v_columns.second.at(i);
  plot.set_plot_name(channel + "_" + catstring + "_" + era_file_label+ (postfit ? "_postfit":"_prefit"));

  if (mssm) {
    double d_mass = boost::lexical_cast<double>(signal_mass);
    double d_tanb = boost::lexical_cast<double>(tanb);
    std::cout << "*** Finding MSSM cross sections for mA = " << d_mass << " tan(beta) = " << d_tanb << std::endl;
    std::cout << "*****************************************************************************" << std::endl;

    for (unsigned i = 0; i < v_eras.size(); ++i) {
      mssm_xs_tools xs_tool;
      string file;
      if (v_eras[i] == "7TeV") {
        file = "data/scale_factors/out.mhmax_mu200_7_nnlo.tanBeta_gte1.root";
      } else if (v_eras[i] == "8TeV") {
        file = "data/scale_factors/out.mhmax_mu200_8_nnlo.tanBeta_gte1_FHv274.root";
      } else {
        continue;
      }
      xs_tool.SetInput(file.c_str());
      std::cout << "*****************************************************************************" << std::endl;
      double br =  xs_tool.Give_BR_A_tautau(d_mass, d_tanb);
      double xs_ggh = xs_tool.Give_Xsec_ggFA(d_mass, d_tanb) / 1000.;
      double xs_bbh = xs_tool.Give_Xsec_bbA5f(d_mass, d_tanb) / 1000.;
      std::cout << "Era: " << v_eras[i] << " BR: " << br << " XS(ggH): " << xs_ggh << " XS(bbH): " << xs_bbh << std::endl; 
      double ggh_era = setup.era({v_eras[i]}).process({"ggH"}).GetRate();
      double bbh_era = setup.era({v_eras[i]}).process({"bbH"}).GetRate();
      double ggh_diff = ggh_era * (br*xs_ggh - 1.);
      double bbh_diff = bbh_era * (br*xs_bbh - 1.);
      std::cout << "Scaling ggH: " << ggh_era << " ---> " << ggh_era*br*xs_ggh << endl;
      std::cout << "Scaling bbH: " << bbh_era << " ---> " << bbh_era*br*xs_bbh << endl;
      TH1F & ggh_hist = hmap["ggH"+signal_mass].first;
      TH1F & bbh_hist = hmap["bbH"+signal_mass].first;
      ggh_hist.Scale((ggh_diff+Integral(&ggh_hist))/Integral(&ggh_hist));
      bbh_hist.Scale((bbh_diff+Integral(&bbh_hist))/Integral(&bbh_hist));
    }
  }
  TH1F total_hist = setup.process({"ZTT","ZL","ZJ","ZLL","W","QCD","VV","TT","Ztt","Fakes","EWK","ttbar"}).GetShape();
  hmap["Bkg"] = make_pair(total_hist, make_pair(0.,0.));

  string channel_str;
  if (channel == "et") channel_str = "e#tau_{h}";
  if (channel == "mt") channel_str = "#mu#tau_{h}";
  if (channel == "em") channel_str = "e#mu";
  if (channel == "tt") channel_str = "#tau_{h}#tau_{h}";
  if (!postfit) channel_str += " (pre-fit)";

  ic::TextElement text(channel_str,0.04,0.22,0.86);
  ic::TextElement text2(v_columns.first,0.04,0.22,0.79);
  plot.AddTextElement(text);
  plot.AddTextElement(text2);

  if (mssm) {
    ic::TextElement text_ma("m_{A}="+signal_mass+" GeV",0.035,0.41,0.86);
    ic::TextElement text_tanb("tan#beta="+tanb,0.035,0.41,0.81);
    ic::TextElement text_scen("m^{h}_{max}",0.035,0.41,0.76);
    plot.AddTextElement(text_ma);
    plot.AddTextElement(text_tanb);
    plot.AddTextElement(text_scen);
  }

  plot.GeneratePlot(hmap);


  return 0;
}

