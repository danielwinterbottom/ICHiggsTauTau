#include <iostream>
#include <vector>
#include <map>

#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SimpleParamParser.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTStatTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/mssm_xs_tools.h"


namespace po = boost::program_options;

using namespace std;
using namespace ic;


struct EraInfo {
  string                era;
  string                param_file;
  SimpleParamParser     param_parser;
  double                lumi;
  map<string, double>   xs;
  mssm_xs_tools         mssm_xs;
};

struct ColInfo {
  string                label;
  vector<string>        cats_str;
  vector<int>           cats_int;
};

struct BkgInfo {
  string                label;
  vector<string>        procs;
  BkgInfo(string const& l, vector<string> const& p) {
    label = l;
    procs = p;
  }
};

int main(int argc, char* argv[]) {

  string channel        = "";
  string columns        = "";
  string eras           = "";
  string era_params     = "";
  string pulls_file     = "";
  string datacard_path  = "";
  string root_file_path = "";
  string signal_mass    = "";
  string tanb           = "";
  bool postfit          = true;
  bool no_correlations  = true;
  bool no_shapes        = true;
  bool mssm             = false;

  po::options_description config("Configuration");
  config.add_options()
    ("channel",              po::value<string>(&channel)->required(),             "channel")
    ("columns",              po::value<string>(&columns)->required(),             "labels")
    ("eras",                 po::value<string>(&eras)->required(),                "eras")
    ("era_params",           po::value<string>(&era_params)->required(),          "eras")
    ("datacard_path",        po::value<string>(&datacard_path)->required(),       "datacard_path")
    ("root_file_path",       po::value<string>(&root_file_path)->required(),      "root_file_path")
    ("pulls_file",           po::value<string>(&pulls_file)->required(),          "pulls_file")
    ("signal_mass",          po::value<string>(&signal_mass)->required(),         "signal_mass")
    ("tanb",                 po::value<string>(&tanb)->default_value("8"),        "tanb")
    ("postfit",              po::value<bool>(&postfit)->required(),               "postfit")
    ("mssm",                 po::value<bool>(&mssm)->default_value(false),           "input is an MSSM datacard")
    ("no_correlations",      po::value<bool>(&no_correlations)->default_value(true), "input is an MSSM datacard")
    ("no_shapes",            po::value<bool>(&no_shapes)->default_value(true),       "input is an MSSM datacard");
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::notify(vm);

  //---------------------------------------------------------------------------
  // Start building the properties of this table
  //---------------------------------------------------------------------------

  // Need mA and tanb as floats for the mssm
  double d_mass = boost::lexical_cast<double>(signal_mass);
  double d_tanb = boost::lexical_cast<double>(tanb);

  // Signal processes
  vector<string> signal_procs;
  if (mssm) {
    signal_procs = {"ggH","bbH"};
  } else {
    signal_procs = {"ggH", "qqH", "VH"};
  }
  unsigned n_sig = signal_procs.size();

  // Eras
  vector<string> v_eras;
  boost::split(v_eras, eras, boost::is_any_of(","));
  vector<string> v_era_params;
  boost::split(v_era_params, era_params, boost::is_any_of(","));
  if (v_eras.size() != v_era_params.size()) {
    cerr << "Must specify a parameter file for each era!" << endl;
    return 1;
  }
  unsigned n_eras = v_eras.size();
  vector<EraInfo> era_info;
  era_info.resize(n_eras);
  for (unsigned i = 0; i < n_eras; ++i) {
    EraInfo & info = era_info[i];
    info.era = v_eras[i];
    info.param_file = v_era_params[i];
    info.param_parser.ParseFile(info.param_file);
    info.lumi = info.param_parser.GetParam<double>("LUMI_DATA_"+channel);
    if (mssm) {
      if (info.era == "7TeV") info.mssm_xs.SetInput("data/scale_factors/out.mhmax_mu200_7_nnlo.tanBeta_gte1.root");
      if (info.era == "8TeV") info.mssm_xs.SetInput("data/scale_factors/out.mhmax_mu200_8_nnlo.tanBeta_gte1_FHv274.root");
      info.xs["ggH"] = info.mssm_xs.Give_BR_A_tautau(d_mass, d_tanb) * (info.mssm_xs.Give_Xsec_ggFA(d_mass, d_tanb) / 1000.);
      info.xs["bbH"] = info.mssm_xs.Give_BR_A_tautau(d_mass, d_tanb) * (info.mssm_xs.Give_Xsec_bbA5f(d_mass, d_tanb) / 1000.);
    } else {
      info.xs["ggH"]  = info.param_parser.GetParam<double>("XS_GluGluToHToTauTau_M-"+signal_mass);
      info.xs["qqH"]  = info.param_parser.GetParam<double>("XS_VBF_HToTauTau_M-"+signal_mass);
      info.xs["VH"]   = info.param_parser.GetParam<double>("XS_WH_ZH_TTH_HToTauTau_M-"+signal_mass);
    }
  }

  // Table columns 
  vector<ColInfo> col_info;
  vector<string> tmp_columns;
  boost::split(tmp_columns, columns, boost::is_any_of(","));
  col_info.resize(tmp_columns.size());
  for (unsigned i = 0; i < col_info.size(); ++i) {
    ColInfo & info = col_info[i];
    vector<string> tmp_split;
    boost::split(tmp_split, tmp_columns[i], boost::is_any_of(":"));
    info.label = tmp_split[0];
    boost::split(info.cats_str, tmp_split[1], boost::is_any_of("+"));
    for (auto const& str : info.cats_str) info.cats_int.push_back(boost::lexical_cast<int>(str));
  }
  unsigned n_cols = col_info.size();

  // Create some arrays to track to the signal efficiencies
  double signal_den[n_eras][n_sig];
  double signal_num[n_eras][n_sig][n_cols];
  double signal_eff[n_cols][n_sig];

  // We can fill the denominator already
  for (unsigned i = 0; i < n_sig; ++i) {
    for (unsigned j = 0; j < n_eras; ++j) {
      signal_den[j][i] = era_info[j].lumi * era_info[j].xs[signal_procs[i]];
    }
  }
  
  // Channel-specific background configuration
  vector<BkgInfo> bkgs;
  vector<string> total_bkg;
  if (channel == "et" || channel == "mt" || channel == "tt") {
    bkgs = {
      BkgInfo("Z$\\rightarrow \\tau\\tau$",        {"ZTT"}),
      BkgInfo("QCD",                               {"QCD"}       ),
      BkgInfo("W+jets",                            {"W"}         ),
      BkgInfo("Z+jets (l/jet faking $\\tau$)",     {"ZL","ZJ"}   ),
      BkgInfo("t$\\bar{\\rm{t}}$",                 {"TT"}        ),
      BkgInfo("Dibosons",                          {"VV"}        )
    };
    total_bkg = { "ZTT","QCD","W","ZL","ZJ","TT","VV" };
  }
  if (channel == "em") {
    bkgs = {
      BkgInfo("Z$\\rightarrow \\tau\\tau$",        {"Ztt"}       ),
      BkgInfo("QCD",                               {"Fakes"}     ),
      BkgInfo("t$\\bar{\\rm{t}}$",                 {"ttbar"}     ),
      BkgInfo("Dibosons",                          {"EWK"}       )
    };
    total_bkg = { "Ztt","Fakes","ttbar","EWK" };
  }
  if (channel == "mm") {
    bkgs = {
      BkgInfo("Z$\\rightarrow \\tau\\tau$",        {"ZTT"}              ),
      BkgInfo("Z$\\rightarrow \\mu\\mu$",          {"ZMM"}              ),
      BkgInfo("QCD",                               {"QCD"}              ),
      BkgInfo("t$\\bar{\\rm{t}}$",                 {"TTJ"}              ),
      BkgInfo("Dibosons",                          {"WJets","Dibosons"} )
    };
    total_bkg = { "ZTT","ZMM","QCD","TTJ","WJets","Dibosons" };
  }
  unsigned n_bkg = bkgs.size();

  //---------------------------------------------------------------------------
  // Now we parse the datacards and ROOT files
  //---------------------------------------------------------------------------

  HTTSetup setup;
  for (unsigned i = 0; i < n_cols; ++i) {
    for (unsigned j = 0; j < n_eras; ++j) {
      for (unsigned k = 0; k < col_info[i].cats_str.size(); ++k) {
        setup.ParseDatacard(datacard_path+"/"+"htt_"+channel+"_"+col_info[i].cats_str[k]+"_"+era_info[j].era+".txt", channel, col_info[i].cats_int[k], era_info[j].era, signal_mass);        
      }
    }
  }
  for (unsigned i = 0; i < n_eras; ++i) {
    if (!mssm) {
      setup.ParseROOTFile(root_file_path+"/"+"htt_"+channel+".input_"+era_info[i].era+".root", channel, era_info[i].era);
    } else {
      setup.ParseROOTFile(root_file_path+"/"+"htt_"+channel+".inputs-mssm-"+era_info[i].era+"-0.root", channel, era_info[i].era);
    }
  }
  setup.ParsePulls(pulls_file);
  if (postfit)           setup.ApplyPulls();
  if (no_correlations)   setup.SetIgnoreNuisanceCorrelations(true);
  if (no_shapes)         setup = setup.no_shapes();

  HTTSetup mssm_signal_setup = setup.signals();
  if (mssm) {
    for (unsigned i = 0; i < n_eras; ++i) {
      mssm_signal_setup.ParseROOTFile(root_file_path+"/"+"htt_"+channel+".inputs-mssm-"+era_info[i].era+"-0.root_"+signal_mass+"_"+(boost::format("%.1f")%d_tanb).str(), channel, era_info[i].era);
      for (unsigned j = 0; j < n_sig; ++j) {
        mssm_signal_setup.ScaleProcessByEra(signal_procs[j], era_info[i].era, d_tanb);
        setup.ScaleProcessByEra(signal_procs[j], era_info[i].era, era_info[i].xs[signal_procs[j]]);
      }
    }
  }

  double bkg_yields[n_cols][n_bkg];
  double bkg_errors[n_cols][n_bkg];
  double tot_yields[n_cols];
  double tot_errors[n_cols];
  double sig_yields[n_cols];
  double sig_errors[n_cols];
  double data_yields[n_cols];

  for (unsigned i = 0; i < n_cols; ++i) {
    data_yields[i] = setup.category_id(col_info[i].cats_int).GetObservedRate();
    if (mssm) {
      sig_yields[i] = mssm_signal_setup.category_id(col_info[i].cats_int).process(signal_procs).GetRate();
      sig_errors[i] = mssm_signal_setup.category_id(col_info[i].cats_int).process(signal_procs).GetUncertainty();      
    } else {
      sig_yields[i] = setup.category_id(col_info[i].cats_int).process(signal_procs).GetRate();
      sig_errors[i] = setup.category_id(col_info[i].cats_int).process(signal_procs).GetUncertainty();      
    }
    tot_yields[i] = setup.category_id(col_info[i].cats_int).process(total_bkg).GetRate();
    tot_errors[i] = setup.category_id(col_info[i].cats_int).process(total_bkg).GetUncertainty();
    for (unsigned j = 0; j < n_bkg; ++j) {
      bkg_yields[i][j] = setup.category_id(col_info[i].cats_int).process(bkgs[j].procs).GetRate();
      bkg_errors[i][j] = setup.category_id(col_info[i].cats_int).process(bkgs[j].procs).GetUncertainty();
    }
    for (unsigned j = 0; j < n_eras; ++j) {
      for (unsigned k = 0; k < n_sig; ++k) {
        signal_num[j][k][i] = setup.category_id(col_info[i].cats_int).process({signal_procs[k]}).era({era_info[j].era}).GetRate();
      }
    }

    for (unsigned j = 0; j < n_sig; ++j) {
      double num_total = 0;
      double den_total = 0;
      for (unsigned k = 0; k < n_eras; ++k) {
        num_total += signal_num[k][j][i];
        den_total += signal_den[k][j];
      }
      signal_eff[i][j] = num_total / den_total;
    }

  }

  cout << "\\begin{tabular}{|l|";
  for (unsigned i = 0; i < n_cols; ++i) cout << "r@{$ \\,\\,\\pm\\,\\, $}l|";
  cout << "}\n\\hline\n";
  cout << "Process";
  for (unsigned i = 0; i < n_cols; ++i) cout << " & \\multicolumn{2}{c|}{\\emph{"+col_info[i].label+"}}";
  cout << "\\\\\n";
  cout << "\\hline\n";
  string fmt = "& %-10.0f & %-10.0f";

  for (unsigned k = 0; k < bkgs.size(); ++k) {
    cout << boost::format("%-30s") % bkgs[k].label;
    for (unsigned i = 0; i < n_cols; ++i) cout << boost::format(fmt) % bkg_yields[i][k] % bkg_errors[i][k] ;
    cout << "\\\\\n";    
  }
  cout << "\\hline\n";
  cout << "\\hline\n";

  cout << boost::format("%-30s") % "Total background";
  for (unsigned i = 0; i < n_cols; ++i) cout << boost::format(fmt) % tot_yields[i] % tot_errors[i] ;
  cout << "\\\\\n";

  cout << boost::format("%-30s") % "h,H,A$\\rightarrow \\tau\\tau$";
  for (unsigned i = 0; i < n_cols; ++i) cout << boost::format(fmt) %sig_yields[i] % sig_errors[i];
  cout << "\\\\\n";

  cout << boost::format("%-30s") % "Observed";
  for (unsigned i = 0; i < n_cols; ++i) cout << boost::format("& \\multicolumn{2}{|c|}{%-10.0f}") % data_yields[i] ;
  cout << "\\\\\n";
  cout << "\\hline\n";

  if (mssm) {
    cout << "\\multicolumn{5}{c}{ } \\\\\n";
    cout << "\\multicolumn{2}{l}{Signal Eff.} &  \\multicolumn{3}{c}{ } \\\\\n";
    cout << "\\hline\n";
    cout << boost::format("%-30s") % "gg$\\rightarrow$ A";
    for (unsigned i = 0; i < n_cols; ++i) cout << boost::format("& \\multicolumn{2}{|c|}{%-10.2e}") % signal_eff[i][0];
    cout << "\\\\\n";
    cout << "\\hline\n";
    cout << boost::format("%-30s") % "bb$\\rightarrow$ A";
    for (unsigned i = 0; i < n_cols; ++i) cout << boost::format("& \\multicolumn{2}{|c|}{%-10.2e}") % signal_eff[i][1];
    cout << "\\\\\n";
    cout << "\\hline\n";
  }

/*
\multicolumn{5}{c}{ } \\
\multicolumn{2}{l}{Signal Eff.} &  \multicolumn{3}{c}{ } \\
\hline
gg$\rightarrow$ H              &   \multicolumn{2}{|c|}{3.26 $\cdot 10^{-4}$}  &   \multicolumn{2}{|c|}{2.46 $\cdot 10^{-2}$}\\
\hline
bb$\rightarrow$ H              &   \multicolumn{2}{|c|}{5.03 $\cdot 10^{-3}$}  &   \multicolumn{2}{|c|}{2.50 $\cdot 10^{-2}$}\\
\hline


*/

  cout << "\\end{tabular}\n";

  return 0;
}

