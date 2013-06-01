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
#include "TEfficiency.h"


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

  void HTTAnalysis::AddSMSignalSamples(std::vector<std::string> masses) {
    for (auto m : masses) {
      sample_names_.push_back("GluGluToHToTauTau_M-"+m);
      sample_names_.push_back("VBF_HToTauTau_M-"+m);
      sample_names_.push_back("WH_ZH_TTH_HToTauTau_M-"+m);
    }
  }

  void HTTAnalysis::AddMSSMSignalSamples(std::vector<std::string> masses) {
    for (auto m : masses) {
      sample_names_.push_back("SUSYGluGluToHToTauTau_M-"+m);
      sample_names_.push_back("SUSYBBHToTauTau_M-"+m);
    }
  };


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
      return ((xs*lumi_)/evt);
    } else {
      return 1.0;
    }
  }

  std::string HTTAnalysis::BuildCutString(std::string const& selection,
      std::string const& category,
      std::string const& weight) {
    std::string full_selection;
    if (weight != "" && (selection != "" || category != "")) full_selection += "( ";

    if (selection != "")                    full_selection += ("(" + selection + ")");
    if (selection != "" && category != "")  full_selection += " && ";
    if (category != "")                     full_selection += ("(" + category + ")");
    if (weight != "" && (selection != "" || category != "")) full_selection += " ) * ";
    if (weight != "") full_selection += ("("+weight+")");
    return full_selection;                                      
  }

  std::string HTTAnalysis::BuildVarString(std::string const& variable) {
    std::string full_variable = variable;
    if (full_variable.find("(") != full_variable.npos) {
      full_variable.insert(full_variable.find("("),">>htemp");
    }
    return full_variable;
  }

  TH1F HTTAnalysis::GetShape(std::string const& variable,
                                       std::string const& sample, 
                                       std::string const& selection, 
                                       std::string const& category, 
                                       std::string const& weight) {
    TH1::SetDefaultSumw2(true);
    std::string full_variable = BuildVarString(variable);
    std::string full_selection = BuildCutString(selection, category, weight);
    // std::cout << full_selection << std::endl;
    // std::cout << full_variable << std::endl;
    TH1::AddDirectory(true);
    ttrees_[sample]->Draw(full_variable.c_str(), full_selection.c_str(), "goff");
    TH1::AddDirectory(false);
    TH1F *htemp = (TH1F*)gDirectory->Get("htemp");
    TH1F result = (*htemp);
    gDirectory->Delete("htemp;*");
    auto rate = GetRate(sample, selection, category, weight);
    SetNorm(&result, rate.first);
    return result;
  }

  TH1F HTTAnalysis::GetLumiScaledShape(std::string const& variable,
                                       std::string const& sample, 
                                       std::string const& selection, 
                                       std::string const& category, 
                                       std::string const& weight) {
    TH1F result = GetShape(variable, sample, selection, category, weight);
    result.Scale(GetLumiScale(sample));
    return result;
  }

  TH1F HTTAnalysis::GetLumiScaledShape(std::string const& variable,
                                       std::vector<std::string> const& samples, 
                                       std::string const& selection, 
                                       std::string const& category, 
                                       std::string const& weight) {

    TH1F result = GetLumiScaledShape(variable, samples.at(0), selection, category, weight);
    if (samples.size() > 1) {
      for (unsigned i = 1; i < samples.size(); ++i) {
        TH1F tmp = GetLumiScaledShape(variable, samples.at(i), selection, category, weight);
        result.Add(&tmp);
      }
    }
    return result;
  }


  std::pair<double, double> HTTAnalysis::GetRate(std::string const& sample, 
                                      std::string const& selection, 
                                      std::string const& category, 
                                      std::string const& weight) {
    if (verbosity_ > 1) std::cout << "--GetRate-- Sample:\"" << sample << "\" Selection:\"" << selection << "\" Category:\"" 
      << category << "\" Weight:\"" << weight << "\"" << std::endl;
    std::string full_selection = BuildCutString(selection, category, weight);
    TH1::AddDirectory(true);
    ttrees_[sample]->Draw("0.5>>htemp(1,0,1)", full_selection.c_str(), "goff");
    TH1::AddDirectory(false);
    TH1F *htemp = (TH1F*)gDirectory->Get("htemp");
    auto result = std::make_pair(Integral(htemp), Error(htemp));
    gDirectory->Delete("htemp;*");
    return result;
  }

  std::pair<double, double> HTTAnalysis::GetLumiScaledRate(std::string const& sample, 
                                      std::string const& selection, 
                                      std::string const& category, 
                                      std::string const& weight) {
    auto result = GetRate(sample, selection, category, weight);
    double sf = GetLumiScale(sample);
    result.first *= sf;
    result.second *= sf;
    return result;
  }
  std::pair<double, double> HTTAnalysis::GetLumiScaledRate(std::vector<std::string> const& samples, 
                                      std::string const& selection, 
                                      std::string const& category, 
                                      std::string const& weight) {
    auto result = GetLumiScaledRate(samples.at(0), selection, category, weight);
    double err_sqr = result.second * result.second;
    if (samples.size() > 1) {
      for (unsigned i = 1; i < samples.size(); ++i) {
        auto tmp = GetLumiScaledRate(samples.at(i), selection, category, weight);
        result.first += tmp.first;
        err_sqr += (tmp.second * tmp.second);
      }
    }
    result.second = sqrt(err_sqr);
    return result;
  }

  std::pair<double, double> HTTAnalysis::SampleEfficiency(std::string const& sample, 
                          std::string const& ref_selection, 
                          std::string const& ref_category,
                          std::string const& target_selection, 
                          std::string const& target_category,  
                          std::string const& weight) {
    auto num = GetRate(sample, target_selection, target_category, weight);
    auto den = GetRate(sample, ref_selection, ref_category, weight);
    // std::cout << "num: " << num.first << "\t" << num.second << std::endl;
    double num_eff = std::pow(num.first / num.second, 2.0) ;
    // std::cout << "Effective numerator = " << num_eff << std::endl;
    unsigned num_eff_rounded = unsigned(num_eff+0.5);
    // std::cout << "Effective numerator (rounded) = " << num_eff_rounded << std::endl;
    // std::cout << "den: " << den.first << "\t" << den.second << std::endl;
    double den_eff = std::pow(den.first / den.second, 2.0) ;
    // std::cout << "Effective denominator = " << den_eff << std::endl;
    unsigned den_eff_rounded = unsigned(den_eff+0.5);
    // std::cout << "Effective denominator (rounded) = " << den_eff_rounded << std::endl;
    double eff = num_eff / den_eff;
    TEfficiency teff;
    double eff_err = teff.ClopperPearson(den_eff_rounded,num_eff_rounded,0.683 ,1)-(eff);
    auto result = std::make_pair(eff, eff_err);
    return result;
  }

  HTTAnalysis::Value HTTAnalysis::SampleRatio(std::string const& sample, 
                          std::string const& ref_selection, 
                          std::string const& ref_category,
                          std::string const& target_selection, 
                          std::string const& target_category,  
                          std::string const& weight) {
    Value num = GetRate(sample, target_selection, target_category, weight);
    Value den = GetRate(sample, ref_selection, ref_category, weight);
    return ValueDivide(num, den);
  }

  HTTAnalysis::Value HTTAnalysis::GetRateViaRefEfficiency(std::string const& target_sample, 
                          std::string const& ref_sample,
                          std::string const& ref_selection, 
                          std::string const& ref_category,
                          std::string const& target_selection, 
                          std::string const& target_category,  
                          std::string const& weight) {
    if (verbosity_) {
      std::cout << "--GetRateViaRefEfficiency--" << std::endl;
      std::cout << "-Target: sample=" << target_sample << " selection=\"" << target_selection << "\" category=\"" << target_category << "\"" << std::endl;
      std::cout << "-Reference: sample=" << ref_sample << " selection=\"" << ref_selection << "\" category=\"" << ref_category << "\"" << std::endl;
    }
    auto ref_rate = GetLumiScaledRate(ref_sample, ref_selection, ref_category, weight);
    PrintValue("-RefRate", ref_rate);
    auto target_eff = SampleEfficiency(target_sample, ref_selection, ref_category, target_selection, target_category, weight);
    PrintValue("-TargetEff", target_eff);
    return ValueProduct(ref_rate, target_eff);
  }

  HTTAnalysis::Value HTTAnalysis::GetRateViaWMethod(std::string const& w_sample,
                          std::string const& ratio_category,
                          std::string const& ratio_control_selection,
                          std::string const& ratio_signal_selection,
                          std::string const& data_sample,
                          std::string const& category,
                          std::string const& control_selection,
                          std::vector<std::string> const& sub_samples,
                          std::string const& weight,
                          std::map<std::string, std::function<Value()>> dict
                          ) {
    Value ratio = SampleRatio(w_sample, ratio_control_selection, ratio_category, ratio_signal_selection, ratio_category, weight);
    Value data_control = GetRate(data_sample, control_selection, category, weight);
    Value total_bkg;
    for (unsigned i = 0; i < sub_samples.size(); ++i) {
      Value bkr;
      if (dict.count(sub_samples[i])) {
        bkr = ((*dict.find(sub_samples[i])).second)(); // find and evaluate function
      } else {
        bkr = GetLumiScaledRate(sub_samples[i], control_selection, category, weight);
      }
      double new_err = std::sqrt((total_bkg.second * total_bkg.second) + (bkr.second * bkr.second));
      total_bkg.first += bkr.first;
      total_bkg.second = new_err;
    }
    double w_control_err = std::sqrt((total_bkg.second * total_bkg.second) + (data_control.second * data_control.second));
    Value w_control(data_control.first - total_bkg.first, w_control_err);
    Value w_signal = ValueProduct(w_control, ratio);
    return w_signal;
  }

  HTTAnalysis::Value HTTAnalysis::GetRateViaQCDMethod(HTTAnalysis::Value const& ratio,
                          std::string const& data_sample,
                          std::string const& control_selection,
                          std::string const& category,
                          std::vector<std::string> const& sub_samples,
                          std::string const& weight,
                          std::map<std::string, std::function<Value()>> dict
                          ) {
    Value data_control = GetRate(data_sample, control_selection, category, weight);
    Value total_bkg;
    for (unsigned i = 0; i < sub_samples.size(); ++i) {
      Value bkr;
      if (dict.count(sub_samples[i])) {
        bkr = ((*dict.find(sub_samples[i])).second)(); // find and evaluate function
      } else {
        bkr = GetLumiScaledRate(sub_samples[i], control_selection, category, weight);
      }
      double new_err = std::sqrt((total_bkg.second * total_bkg.second) + (bkr.second * bkr.second));
      total_bkg.first += bkr.first;
      total_bkg.second = new_err;
    }
    double qcd_control_err = std::sqrt((total_bkg.second * total_bkg.second) + (data_control.second * data_control.second));
    Value qcd_control(data_control.first - total_bkg.first, qcd_control_err);
    Value qcd_signal = ValueProduct(qcd_control, ratio);
    return qcd_signal;
  }

  TH1F HTTAnalysis::GetShapeViaQCDMethod(std::string const& variable,
                          std::string const& data_sample,
                          std::string const& selection,
                          std::string const& category,
                          std::vector<std::string> const& sub_samples,
                          std::string const& weight,
                          std::map<std::string, std::function<Value()>> dict
                          ) {
    TH1F result = GetLumiScaledShape(variable, data_sample, selection, category, weight);
    for (unsigned i = 0; i < sub_samples.size(); ++i) {
      if (dict.count(sub_samples[i])) {
        Value bkr_rate = ((*dict.find(sub_samples[i])).second)(); // find and evaluate function
        TH1F tmp = GetShape(variable, sub_samples.at(i), selection, category, weight);
        SetNorm(&tmp, bkr_rate.first);
        result.Add(&tmp, -1.);
      } else {
        TH1F tmp = GetLumiScaledShape(variable, sub_samples[i], selection, category, weight);
        result.Add(&tmp, -1.);
      }
    }
    return result;
  }

  HTTAnalysis::Value HTTAnalysis::ValueProduct(Value const& p1, Value const& p2) {
    double f = p1.first * p2.first;
    double a_sqrd = std::pow(p1.second / p1.first, 2.0);
    double b_sqrd = std::pow(p2.second / p2.first, 2.0);
    double f_err = f * sqrt( a_sqrd + b_sqrd );
    return std::make_pair(f, f_err);
  }
  HTTAnalysis::Value HTTAnalysis::ValueDivide(Value const& p1, Value const& p2) {
    double f = p1.first / p2.first;
    double a_sqrd = std::pow(p1.second / p1.first, 2.0);
    double b_sqrd = std::pow(p2.second / p2.first, 2.0);
    double f_err = f * sqrt( a_sqrd + b_sqrd );
    return std::make_pair(f, f_err);
  }
  HTTAnalysis::Value HTTAnalysis::ValueAdd(Value const& p1, Value const& p2) {
    double f = p1.first + p2.first;
    double f_err = sqrt( p1.second*p1.second + p2.second*p2.second );
    return std::make_pair(f, f_err);
  }
  HTTAnalysis::Value HTTAnalysis::ValueSubtract(Value const& p1, Value const& p2) {
    double f = p1.first - p2.first;
    double f_err = sqrt( p1.second*p1.second + p2.second*p2.second );
    return std::make_pair(f, f_err);
  }

  void HTTAnalysis::PrintValue(std::string const& label, Value const& val) {
    std::cout << (boost::format("%-12s %-10.2f +/-   %-10.2f  (%.4f)") % (label+":") % val.first % val.second % (val.second/val.first)) << std::endl;
  }

}

