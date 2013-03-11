#include <iostream>
#include <vector>
#include <map>

#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTStatTools.h"


namespace po = boost::program_options;

using namespace std;
using namespace ic;

int main(int argc, char* argv[]) {

  string channel        = "";
  string columns        = "";
  string eras           = "";
  string pulls_file     = "";
  string datacard_path  = "";
  string root_file_path = "";
  string signal_mass    = "";
  bool postfit          = true;

  po::options_description config("Configuration");
  config.add_options()
    ("channel",              po::value<string>(&channel)->required(),         "channel")
    ("columns",              po::value<string>(&columns)->required(),         "labels")
    ("eras",                 po::value<string>(&eras)->required(),            "eras")
    ("datacard_path",        po::value<string>(&datacard_path)->required(),   "datacard_path")
    ("root_file_path",       po::value<string>(&root_file_path)->required(),  "root_file_path")
    ("pulls_file",           po::value<string>(&pulls_file)->required(),      "pulls_file")
    ("signal_mass",          po::value<string>(&signal_mass)->required(),     "signal_mass")
    ("postfit",              po::value<bool>(&postfit)->required(),           "postfit");
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::notify(vm);

  vector<string> v_eras;
  boost::split(v_eras, eras, boost::is_any_of(","));

  vector<pair<string,vector<string>>> v_columns;
  vector<string> tmp_columns;
  boost::split(tmp_columns, columns, boost::is_any_of(","));
  for (unsigned i = 0; i < tmp_columns.size(); ++i) {
    vector<string> tmp_split;
    boost::split(tmp_split, tmp_columns[i], boost::is_any_of(":"));
    if (tmp_split.size() == 2) {
      vector<string> tmp_cats;
      boost::split(tmp_cats, tmp_split[1], boost::is_any_of("+"));
      v_columns.push_back(make_pair(tmp_split[0],tmp_cats));
    }
  }

  HTTSetup setup;

  for (unsigned i = 0; i < v_columns.size(); ++i) {
    for (unsigned j = 0; j < v_eras.size(); ++j) {
      for (unsigned k = 0; k < v_columns[i].second.size(); ++k) {
        string cat = v_columns[i].second[k];
        setup.ParseDatacard(datacard_path+"/"+"htt_"+channel+"_"+cat+"_"+v_eras[j]+".txt", channel, boost::lexical_cast<int>(cat), v_eras[j], signal_mass);        
      }
    }
  }

  for (unsigned i = 0; i < v_eras.size(); ++i) {
    setup.ParseROOTFile(root_file_path+"/"+"htt_"+channel+".input_"+v_eras[i]+".root", channel, v_eras[i]);
  }

  setup.ParsePulls(pulls_file);
  if (postfit) setup.ApplyPulls();

  vector<pair<double,double>> y_ztt;
  vector<pair<double,double>> y_qcd;
  vector<pair<double,double>> y_ewk;
  vector<pair<double,double>> y_top;
  vector<pair<double,double>> y_tot;
  vector<pair<double,double>> y_sig;
  vector<double>              y_dat;

  for (unsigned i = 0; i < v_columns.size(); ++i) {
    vector<int> id;
    for (unsigned j = 0; j < v_columns[i].second.size(); ++j) id.push_back(boost::lexical_cast<int>(v_columns[i].second[j]));
    y_ztt.push_back(make_pair(0.,0.));
    y_qcd.push_back(make_pair(0.,0.));
    y_ewk.push_back(make_pair(0.,0.));
    y_top.push_back(make_pair(0.,0.));
    y_tot.push_back(make_pair(0.,0.));
    y_sig.push_back(make_pair(0.,0.));
    y_dat.push_back(0.0);
    y_ztt.back().first  += setup.category_id(id).process({"ZTT","Ztt"}).GetRate();
    y_ztt.back().second += setup.category_id(id).process({"ZTT","Ztt"}).GetUncertainty();
    y_qcd.back().first  += setup.category_id(id).process({"QCD","Fakes"}).GetRate();
    y_qcd.back().second += setup.category_id(id).process({"QCD","Fakes"}).GetUncertainty();
    y_ewk.back().first  += setup.category_id(id).process({"W","ZL","ZJ","ZLL","VV","EWK"}).GetRate();
    y_ewk.back().second += setup.category_id(id).process({"W","ZL","ZJ","ZLL","VV","EWK"}).GetUncertainty();
    y_top.back().first  += setup.category_id(id).process({"TT","ttbar"}).GetRate();
    y_top.back().second += setup.category_id(id).process({"TT","ttbar"}).GetUncertainty();
    y_tot.back().first  += setup.category_id(id).process({"ZTT","ZL","ZJ","ZLL","W","QCD","VV","TT","Ztt","Fakes","EWK","ttbar"}).GetRate();
    y_tot.back().second += setup.category_id(id).process({"ZTT","ZL","ZJ","ZLL","W","QCD","VV","TT","Ztt","Fakes","EWK","ttbar"}).GetUncertainty();
    y_sig.back().first  += setup.category_id(id).process({"ggH","VH","qqH"}).GetRate();
    y_sig.back().second += setup.category_id(id).process({"ggH","VH","qqH"}).GetUncertainty();
    y_dat.back()        += setup.category_id(id).GetObservedRate();
  }


  cout << "\\begin{tabular}{|l|";
  for (unsigned i = 0; i < v_columns.size(); ++i) cout << "r@{$ \\,\\,\\pm\\,\\, $}l|";
  cout << "}\n\\hline\n";
  cout << "Process";
  for (unsigned i = 0; i < v_columns.size(); ++i) cout << " & \\multicolumn{2}{c|}{\\emph{"+v_columns[i].first+"}}";
  cout << "\\\\\n";
  cout << "\\hline\n";
  string fmt = "& %-10.0f & %-10.0f";

  cout << boost::format("%-30s") % "Z$\\rightarrow \\tau\\tau$";
  for (unsigned i = 0; i < v_columns.size(); ++i) cout << boost::format(fmt) % y_ztt[i].first % y_ztt[i].second ;
  cout << "\\\\\n";

  cout << boost::format("%-30s") % "QCD";
  for (unsigned i = 0; i < v_columns.size(); ++i) cout << boost::format(fmt) % y_qcd[i].first % y_qcd[i].second ;
  cout << "\\\\\n";

  cout << boost::format("%-30s") % "Electroweak";
  for (unsigned i = 0; i < v_columns.size(); ++i) cout << boost::format(fmt) % y_ewk[i].first % y_ewk[i].second ;
  cout << "\\\\\n";
  
  cout << boost::format("%-30s") % "t$\\bar{\\rm{t}}$";
  for (unsigned i = 0; i < v_columns.size(); ++i) cout << boost::format(fmt) % y_top[i].first % y_top[i].second ;
  cout << "\\\\\n";
  cout << "\\hline\n";
  cout << "\\hline\n";

  cout << boost::format("%-30s") % "Total background";
  for (unsigned i = 0; i < v_columns.size(); ++i) cout << boost::format(fmt) % y_tot[i].first % y_tot[i].second ;
  cout << "\\\\\n";

  cout << boost::format("%-30s") % "H$\\rightarrow \\tau\\tau$";
  for (unsigned i = 0; i < v_columns.size(); ++i) cout << boost::format(fmt) % y_sig[i].first % y_sig[i].second ;
  cout << "\\\\\n";

  cout << boost::format("%-30s") % "Observed";
  for (unsigned i = 0; i < v_columns.size(); ++i) cout << boost::format("& \\multicolumn{2}{|c|}{%-10.0f}") % y_dat[i] ;
  cout << "\\\\\n";
  cout << "\\hline\n";
  cout << "\\end{tabular}\n";

  return 0;
}

