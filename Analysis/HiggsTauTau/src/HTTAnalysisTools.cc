#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTAnalysisTools.h"
#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "boost/range/algorithm.hpp"
#include "boost/range/algorithm_ext.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SimpleParamParser.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "TPad.h"
#include "TROOT.h"

namespace ic {

  HTTAnalysis::HTTAnalysis(ic::channel ch, std::string year, int verbosity) :
      ch_(ch),
      year_(year),
      verbosity_(verbosity) {
    // Define some sensible defaults
    sample_names_ = {
      "Data",
      "Embedded",
      "DYJetsToTauTau",
      "DYJetsToTauTauSoup",
      "WJetsToLNuSoup",
      "WWJetsTo2L2Nu",
      "WZJetsTo2L2Q",
      "WZJetsTo3LNu",
      "ZZJetsTo2L2Nu",
      "ZZJetsTo2L2Q",
      "ZZJetsTo4L",
      "T-tW",
      "Tbar-tW",
      "TTJets",
      "TT"
    };

    if (ch_ != channel::em) {
      boost::range::push_back(sample_names_, std::vector<std::string>{
        "Special_3_Data",
        "DYJetsToLL",
        "DYJetsToLL-L",
        "DYJetsToLL-J",
        "DYJetsToLLSoup",
        "DYJetsToLL-LSoup",
        "DYJetsToLL-JSoup",
        "Special_18_DYJetsToLL-L",
        "WJetsToLNuSoup"
      });
    } else {
      boost::range::push_back(sample_names_, std::vector<std::string>{
        "Special_20_Data",
        "Special_21_Data",
        "Special_22_Data",
        "Special_23_Data",
        "Special_24_Data"
      });
    }

    lumi_ = 10000.;

  }

  void HTTAnalysis::ParseParamFile(std::string const& file) {

    SimpleParamParser parser;
    parser.ParseFile(file);
    lumi_ = parser.GetParam<double>("LUMI_DATA");
    // std::string mass_str = parser.GetParam<std::string>("SIGNAL_MASSES");
    // boost::split(signal_masses_, mass_str, boost::is_any_of(","));
    // boost::range::for_each(signal_masses_, [this] (std::string const& mass) {
    //   input_files_.push_back("GluGluToHToTauTau_M-"+mass);
    //   input_files_.push_back("VBF_HToTauTau_M-"+mass);
    //   input_files_.push_back("WH_ZH_TTH_HToTauTau_M-"+mass);
    // });
    if (verbosity_) std::cout << "Extracting sample info..." << std::endl;
    for (auto sample : sample_names_) {
      if (parser.ParamExists("XS_"+sample) && parser.ParamExists("EVT_"+sample)) {
        double evt = parser.GetParam<double>("EVT_"+sample);
        double xs = parser.GetParam<double>("XS_"+sample);
        if (xs <= 0) continue;
        sample_info_[sample] = std::make_pair(evt, xs);
        if (verbosity_) {
          std::cout << sample << " evt=" << evt << " xs= " << xs << " l_eff= " << evt/xs << " l_rel= " << (evt/(xs*lumi_)) << std::endl;
        }
      }
    }
  }


  void HTTAnalysis::ReadTrees(std::string const& folder, std::string const& prefix) {
    if (verbosity_) std::cout << "Reading input files..." << std::endl;
    for (auto name : sample_names_) {
      // The input file is folder + sample name + channel + year
      std::string input_filename = folder+"/"+name+"_"+Channel2String(ch_)+"_"+year_+".root";
      std::string label = (prefix == "") ? name : (prefix + "/" + name);
      if (verbosity_) std::cout << input_filename << " --> " << label << std::endl;
      TFile *tmp_file = new TFile(input_filename.c_str());
      if (!tmp_file) {
        std::cerr << "Warning in <HTTAnalysis::ReadTrees>: Unable to open file " << input_filename << std::endl;
        continue;
      }
      gDirectory->cd("/");
      TTree *tmp_tree = dynamic_cast<TTree*>(gDirectory->Get("ntuple"));
      if (!tmp_tree) {
        std::cerr << "Warning in <HTTAnalysis::ReadTrees>: Unable to extract TTree from file " << input_filename << std::endl;
        continue;        
      }
      tfiles_[label] = tmp_file;
      ttrees_[label] = tmp_tree;
    }
  }

  double HTTAnalysis::GetLumiScale(std::string const& sample) {
    auto it = sample_info_.find(sample);
    if (it != sample_info_.end()) {
      double evt = it->second.first;
      double xs = it->second.second;
      return (evt/(xs*lumi_));
    } else {
      return 1.0;
    }
  }
  TH1F HTTAnalysis::GetShape(std::string const& variable,
                                       std::string const& sample, 
                                       std::string const& selection, 
                                       std::string const& category, 
                                       std::string const& weight) {
    std::string full_selection;
    if (weight != "" && (selection != "" || category != "")) full_selection += "( ";

    if (selection != "")                    full_selection += ("(" + selection + ")");
    if (selection != "" && category != "")  full_selection += " && ";
    if (category != "")                     full_selection += ("(" + category + ")");
    if (weight != "" && (selection != "" || category != "")) full_selection += " ) * ";
    if (weight != "") full_selection += ("("+weight+")");
    std::cout << full_selection << std::endl;
    std::string full_variable = variable;
    if (full_variable.find("(") != full_variable.npos) {
      full_variable.insert(full_variable.find("("),">>htemp");
    }
    //std::cout << full_variable << std::endl;
    TH1::AddDirectory(true);
    ttrees_[sample]->Draw(full_variable.c_str(), full_selection.c_str(), "goff");
    TH1::AddDirectory(false);
    TH1F *htemp = (TH1F*)gDirectory->Get("htemp");
    TH1F result = (*htemp);
    gDirectory->Delete("htemp;*");
    return result;
  }

  std::pair<double, double> HTTAnalysis::GetRate(std::string const& sample, 
                                      std::string const& selection, 
                                      std::string const& category, 
                                      std::string const& weight) {

    TH1F hist = 
    std::string full_selection;
    if (weight != "" && (selection != "" || category != "")) full_selection += "( ";

    if (selection != "")                    full_selection += ("(" + selection + ")");
    if (selection != "" && category != "")  full_selection += " && ";
    if (category != "")                     full_selection += ("(" + category + ")");
    if (weight != "" && (selection != "" || category != "")) full_selection += " ) * ";
    if (weight != "") full_selection += ("("+weight+")");
    TH1::AddDirectory(true);
    ttrees_[sample]->Draw("0.5>>htemp(1,0,1)", full_selection.c_str(), "goff");
    TH1::AddDirectory(false);
    std::cout << full_selection << std::endl;
    //TH1F *htemp = (TH1F*)gDirectory->Get("htemp");
    auto result = std::make_pair(Integral(htemp), Error(htemp));
    // gDirectory->Delete("htemp;*");
    return result;
  }



}

