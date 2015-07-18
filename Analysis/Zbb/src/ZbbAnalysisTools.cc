#include "Zbb/interface/ZbbAnalysisTools.h"
#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/range/algorithm_ext.hpp"
#include "boost/filesystem.hpp"
#include "Utilities/interface/SimpleParamParser.h"
#include "Utilities/interface/FnRootTools.h"
#include "TEfficiency.h"

namespace ic {
  double Integral(TH1F const* hist) {
    return hist->Integral(0, hist->GetNbinsX() + 1);
  }

  double IntegrateFloatRange(TH1F const* hist, double xmin, double xmax) {
    TAxis const* axis = hist->GetXaxis();
    int bmin = axis->FindFixBin(xmin);
    int bmax = axis->FindFixBin(xmax);
    double integral = hist->Integral(bmin, bmax);
    integral -= hist->GetBinContent(bmin)*(xmin-axis->GetBinLowEdge(bmin))/
              axis->GetBinWidth(bmin);
    integral -= hist->GetBinContent(bmax)*(axis->GetBinUpEdge(bmax)-xmax)/
              axis->GetBinWidth(bmax);
    return integral;
  }

  double Error(TH1F const* hist) {
    double err = 0.0;
    hist->IntegralAndError(0, hist->GetNbinsX() + 1, err);
    return err;
  }

  void SetNorm(TH1F * hist, double norm, bool verbose) {
    if (Integral(hist) == 0.0) {
      if (verbose) std::cout << "Warning, attempt to scale histogram with integral of 0.0" << std::endl;
      return;
    } else {
      hist->Scale(norm / Integral(hist));
    }
  }

  ZbbAnalysis::ZbbAnalysis(int verbosity) 
      : verbosity_(verbosity)  {
    lumi_ = 1.;
    using boost::range::push_back;
    // Define some sensible defaults
    sample_names_ = {
      "Data",
      "DYJetsToLL",
      "ZZ",
      "TTJets"
    };
      alias_map_["inclusive"]  = "";
  }

  void ZbbAnalysis::AddSample(std::string const& sample) {
    sample_names_.push_back(sample);
  }

  void ZbbAnalysis::ParseParamFile(std::string const& file) {
    SimpleParamParser parser;
    parser.ParseFile(file);
    std::cout << "[ZbbAnalysis::ParseParamFile] Extracting sample info from file " << file << std::endl;
    lumi_ = parser.GetParam<double>("LUMI_DATA");
    std::cout << "[ZbbAnalysis::ParseParamFile] Integrated luminosity set to " << lumi_ << " /pb" << std::endl;
    if (verbosity_ > 1) std::cout << boost::format("%-25s %15i %15.3f %15.3f %15.3f\n") % "Sample" % "Events" % "Cross Section" % "Sample Lumi" % "Rel. Lumi";
    if (verbosity_ > 1) std::cout << "-----------------------------------------------------------------------------------------\n";
    for (auto sample : sample_names_) {
      std::string lookup = sample;
      if (sample.find("Special") != sample.npos) {
        size_t found = sample.find('_');
        found = sample.find('_',found+1);
        lookup = sample.substr(found+1);
      }
      if (parser.ParamExists("XS_"+sample) && parser.ParamExists("EVT_"+sample)) {
        double evt = parser.GetParam<double>("EVT_"+sample);
        double xs = parser.GetParam<double>("XS_"+sample);
        if (xs <= 0) continue;
        sample_info_[sample] = std::make_pair(evt, xs);
        if (verbosity_ > 1) std::cout << boost::format("%-25s %15i %15.3f %15.3f %15.3f\n") % sample % unsigned(evt+0.5) % xs % (evt/xs) % (evt/(xs*lumi_));
      }
    }
  }

  void ZbbAnalysis::ReadTrees(std::string const& folder, std::string const& fallback_folder) {
    std::cout << "[ZbbAnalysis::ReadTrees] Reading input files..." << std::endl;
    std::vector<std::string> result_summary;
    for (auto name : sample_names_) {
      // The input file is folder + sample name
      std::string input_filename = folder+"/"+name+".root";
      std::string label = name;
      TFile *tmp_file = nullptr;
      if (boost::filesystem::exists(input_filename)) tmp_file = TFile::Open(input_filename.c_str());
      if (!tmp_file && fallback_folder != "") {
        if (verbosity_ > 1) std::cout << "[ZbbAnalysis::ReadTrees] " << input_filename << " not found, trying fallback folder" << std::endl;
        input_filename = fallback_folder+"/"+name+".root";
        if (boost::filesystem::exists(input_filename)) tmp_file = TFile::Open(input_filename.c_str());
      }
      if (!tmp_file) {
        std::cout << "[ZbbAnalysis::ReadTrees] Warning: " << input_filename << " cannot be opened" << std::endl;
        continue;
      }
      if (verbosity_ > 1) result_summary.push_back((boost::format("%-70s %s %-30s\n") % input_filename % "-->" % label).str());
      gDirectory->cd("/");
      TTree *tmp_tree = dynamic_cast<TTree*>(gDirectory->Get("ntuple"));
      if (!tmp_tree) {
        std::cerr << "[ZbbAnalysis::ReadTrees] Warning: Unable to extract TTree from file " << input_filename << std::endl;
        continue;        
      }
      tmp_tree->SetEstimate(1000);
      tfiles_[label] = tmp_file;
      ttrees_[label] = tmp_tree;
    }
    for (auto str : result_summary) std::cout << str;
  }

  double ZbbAnalysis::GetLumiScale(std::string const& sample) {
    auto it = sample_info_.find(sample);
    if (it != sample_info_.end()) {
      double evt = it->second.first;
      double xs = it->second.second;
      return ((xs*lumi_)/evt);
    } else {
      return 1.0;
    }
  }

  double ZbbAnalysis::GetLumiScaleFixedXS(std::string const& sample, double xs) {
      auto it = sample_info_.find(sample);
    if (it != sample_info_.end()) {
      double evt = it->second.first;;
      return ((xs*lumi_)/evt);
    } else {
      return 1.0;
    }
  }
 
  void ZbbAnalysis::FillHistoMap(HistValueMap & hmap,
                        std::string var,
                        std::string sel,
                        std::string cat,
                        std::string wt,
                        std::string postfix) {
    Value total_bkr;
    // Data
    if (postfix == "") {
      auto data_norm = this->GetRate("Data", sel, cat, wt);
      TH1F data_hist = this->GetShape(var, "Data", sel, cat, wt);
      SetNorm(&data_hist, data_norm.first);
      auto data_pair = std::make_pair(data_hist, data_norm);
      PrintValue("data_obs"+postfix, data_pair.second);
      hmap["data_obs"+postfix] = data_pair;
    }

    // Z+l
    auto zl_norm = this->GetLumiScaledRate("DYJetsToLL", sel+" && zflav==0", cat, wt);
    TH1F zl_hist = this->GetShape(var, "DYJetsToLL", sel+" && zflav==0", cat, wt);
    SetNorm(&zl_hist, zl_norm.first);
    TH1F total_hist = zl_hist;
    auto zl_pair = std::make_pair(zl_hist, zl_norm);
    PrintValue("Zl"+postfix, zl_pair.second);
    total_bkr = ValueAdd(total_bkr, zl_pair.second);
    hmap["Zl"+postfix] = zl_pair;

    // Z+c
    auto zc_norm = this->GetLumiScaledRate("DYJetsToLL", sel+" && zflav==1", cat, wt);
    TH1F zc_hist = this->GetShape(var, "DYJetsToLL", sel+" && zflav==1", cat, wt);
    SetNorm(&zc_hist, zc_norm.first);
    total_hist.Add(&zc_hist);
    auto zc_pair = std::make_pair(zc_hist, zc_norm);
    PrintValue("Zc"+postfix, zc_pair.second);
    total_bkr = ValueAdd(total_bkr, zc_pair.second);
    hmap["Zc"+postfix] = zc_pair;

    // Z+b
    auto zb_norm = this->GetLumiScaledRate("DYJetsToLL", sel+" && zflav==2", cat, wt);
    TH1F zb_hist = this->GetShape(var, "DYJetsToLL", sel+" && zflav==2", cat, wt);
    SetNorm(&zb_hist, zb_norm.first);
    total_hist.Add(&zb_hist);
    auto zb_pair = std::make_pair(zb_hist, zb_norm);
    PrintValue("Zb"+postfix, zb_pair.second);
    total_bkr = ValueAdd(total_bkr, zb_pair.second);
    hmap["Zb"+postfix] = zb_pair;

    // ttbar
    auto tt_norm = this->GetLumiScaledRate("TTJets", sel, cat, wt);
    TH1F tt_hist = this->GetShape(var, "TTJets", sel, cat, wt);
    SetNorm(&tt_hist, tt_norm.first);
    total_hist.Add(&tt_hist);
    auto tt_pair = std::make_pair(tt_hist, tt_norm);
    PrintValue("TT"+postfix, tt_pair.second);
    total_bkr = ValueAdd(total_bkr, tt_pair.second);
    hmap["TT"+postfix] = tt_pair;
    
    auto zz_norm = this->GetLumiScaledRate("ZZ", sel, cat, wt);
    TH1F zz_hist = this->GetShape(var, "ZZ", sel, cat, wt);
    SetNorm(&zz_hist, zz_norm.first);
    total_hist.Add(&zz_hist);
    auto zz_pair = std::make_pair(zz_hist, zz_norm);
    PrintValue("ZZ"+postfix, zz_pair.second);
    total_bkr = ValueAdd(total_bkr, zz_pair.second);
    hmap["ZZ"+postfix] = zz_pair;

    // Print the total background yield
    PrintValue("Total"+postfix, total_bkr);
    auto total_pair = std::make_pair(total_hist, total_bkr);
    hmap["Bkg"+postfix] = total_pair;
    return;
  }

  std::string ZbbAnalysis::BuildCutString(std::string const& selection,
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

  std::string ZbbAnalysis::BuildVarString(std::string const& variable) {
    std::string full_variable = variable;
    if (full_variable.find_last_of("(") != full_variable.npos 
        && full_variable.find("[") == full_variable.npos
        && full_variable.find("]") == full_variable.npos) {
      full_variable.insert(full_variable.find_last_of("("),">>htemp");
    }
    return full_variable;
  }


  TH1F ZbbAnalysis::GetShape(std::string const& variable,
                                       std::string const& sample, 
                                       std::string const& selection, 
                                       std::string const& category, 
                                       std::string const& weight) {
    TH1::SetDefaultSumw2(true);
    std::string full_variable = BuildVarString(variable);
    std::size_t begin_var = full_variable.find("[");
    std::size_t end_var   = full_variable.find("]");
    TH1F *htemp = nullptr;
    if (begin_var != full_variable.npos && end_var != full_variable.npos) {
      std::string binning = full_variable.substr(begin_var+1, end_var-begin_var-1);
      std::vector<std::string> string_vec;
      boost::split(string_vec, binning, boost::is_any_of(","));
      std::vector<double> bin_vec;
      for (auto str : string_vec) bin_vec.push_back(boost::lexical_cast<double>(str));
      TH1::AddDirectory(true);
      htemp = new TH1F("htemp","htemp", bin_vec.size()-1, &(bin_vec[0]));
      TH1::AddDirectory(false);
      full_variable.erase(begin_var, full_variable.npos);
      full_variable += ">>htemp";
    }
    std::string full_selection = BuildCutString(selection, category, weight);
    // std::cout << full_selection << std::endl;
    // std::cout << full_variable << std::endl;
    TH1::AddDirectory(true);
    ttrees_[sample]->Draw(full_variable.c_str(), full_selection.c_str(), "goff");
    TH1::AddDirectory(false);
    htemp = (TH1F*)gDirectory->Get("htemp");
    TH1F result = (*htemp);
    gDirectory->Delete("htemp;*");
    auto rate = GetRate(sample, selection, category, weight);
    SetNorm(&result, rate.first);
    return result;
  }

  TH1F ZbbAnalysis::GetLumiScaledShape(std::string const& variable,
                                       std::string const& sample, 
                                       std::string const& selection, 
                                       std::string const& category, 
                                       std::string const& weight) {
    TH1F result = GetShape(variable, sample, selection, category, weight);
    result.Scale(GetLumiScale(sample));
    return result;
  }

  TH1F ZbbAnalysis::GetLumiScaledShape(std::string const& variable,
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

  std::pair<double, double> ZbbAnalysis::GetRate(std::string const& sample, 
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

  std::pair<double, double> ZbbAnalysis::GetLumiScaledRate(std::string const& sample, 
                                      std::string const& selection, 
                                      std::string const& category, 
                                      std::string const& weight) {
    auto result = GetRate(sample, selection, category, weight);
    double sf = GetLumiScale(sample);
    result.first *= sf;
    result.second *= sf;
    return result;
  }
  std::pair<double, double> ZbbAnalysis::GetLumiScaledRate(std::vector<std::string> const& samples, 
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

  std::pair<double, double> ZbbAnalysis::SampleEfficiency(std::string const& sample, 
                          std::string const& ref_selection, 
                          std::string const& ref_category,
                          std::string const& target_selection, 
                          std::string const& target_category,  
                          std::string const& weight) {
    auto num = GetRate(sample, target_selection, target_category, weight);
    auto den = GetRate(sample, ref_selection, ref_category, weight);
    double num_eff = std::pow(num.first / num.second, 2.0) ;
    unsigned num_eff_rounded = unsigned(num_eff+0.5);
    double den_eff = std::pow(den.first / den.second, 2.0) ;
    unsigned den_eff_rounded = unsigned(den_eff+0.5);
    double eff = num.first / den.first;
    TEfficiency teff;
    double eff_err_up   = teff.ClopperPearson(den_eff_rounded,num_eff_rounded,0.683,1)-(num_eff/den_eff);
    double eff_err_down = (num_eff/den_eff)-teff.ClopperPearson(den_eff_rounded,num_eff_rounded,0.683,0);
    double eff_err = (eff_err_up/(num_eff/den_eff)) * eff;
    if (num.first == 0.0) {
      std::cout << "[ZbbAnalysis::SampleEfficiency] Numerator is zero, setting error to zero" << std::endl;
      eff_err = 0.0;
    }
    auto result = std::make_pair(eff, eff_err);
    if (verbosity_ > 0) {
      std::cout << "[ZbbAnalysis::SampleEfficiency]" << std::endl;
      std::cout << "Numerator:   " << boost::format("%s,'%s','%s','%s'\n") % sample % target_selection
                % target_category % weight;
      std::cout << "Denominator: " << boost::format("%s,'%s','%s','%s'\n") % sample % ref_selection
                % ref_category % weight;
      PrintValue("Numerator",num);
      PrintValue("Denominator",den);
      std::cout << "Effective Numerator:   " << num_eff_rounded << std::endl;
      std::cout << "Effective Denominator: " << den_eff_rounded << std::endl;
      std::cout << "Error down (relative): " << eff_err_down/(num_eff/den_eff) << std::endl;
      std::cout << "Error up   (relative): " << eff_err_up/(num_eff/den_eff) << std::endl;
    }
    return result;
  }

  ZbbAnalysis::Value ZbbAnalysis::SampleRatio(std::string const& sample, 
                          std::string const& ref_selection, 
                          std::string const& ref_category,
                          std::string const& target_selection, 
                          std::string const& target_category,  
                          std::string const& weight) {
    Value num = GetRate(sample, target_selection, target_category, weight);
    Value den = GetRate(sample, ref_selection, ref_category, weight);
    return ValueDivide(num, den);
  }

  ZbbAnalysis::Value ZbbAnalysis::GetRateViaRefEfficiency(std::string const& target_sample, 
                          std::string const& ref_sample,
                          std::string const& ref_selection, 
                          std::string const& ref_category,
                          std::string const& target_selection, 
                          std::string const& target_category,  
                          std::string const& weight) {
    if (verbosity_) {
      std::cout << "[ZbbAnalysis::GetRateViaRefEfficiency]\n";
      std::cout << "ReferenceRate:   " << boost::format("%s,'%s','%s','%s'\n") % ref_sample % ref_selection 
                % ref_category % weight;
      std::cout << "Efficiency:      " << boost::format("%s,'%s','%s','%s'\n") %target_sample % target_selection 
                % target_category % weight;
    }
    auto ref_rate = GetLumiScaledRate(ref_sample, ref_selection, ref_category, weight);
    if (verbosity_) PrintValue("ReferenceRate", ref_rate);
    auto target_eff = SampleEfficiency(target_sample, ref_selection, ref_category, target_selection, target_category, weight);
    if (verbosity_) PrintValue("Efficiency", target_eff);
    return ValueProduct(ref_rate, target_eff);
  }

  ZbbAnalysis::Value ZbbAnalysis::ValueProduct(Value const& p1, Value const& p2) {
    if (p1.first == 0.0 || p2.first == 0.0) {
      //if (verbosity_ > 0) std::cout << "[ZbbAnalysis::ValueProduct] At least one value is zero, returning 0.0 +/- 0.0" << std::endl;
      return std::make_pair(0.0, 0.0);
    }
    double f = p1.first * p2.first;
    double a_sqrd = std::pow(p1.second / p1.first, 2.0);
    double b_sqrd = std::pow(p2.second / p2.first, 2.0);
    double f_err = f * sqrt( a_sqrd + b_sqrd );
    return std::make_pair(f, f_err);
  }
  ZbbAnalysis::Value ZbbAnalysis::ValueDivide(Value const& p1, Value const& p2) {
    if (p1.first == 0.0 && p2.first == 0.0) {
      std::cout << "[ZbbAnalysis::ValueDivide] Numerator and denominator both zero, returning 0.0 +/- 0.0" << std::endl;
      return std::make_pair(0.0, 0.0);
    }
    if (p1.first == 0.0) {
      //if (verbosity_ > 0) std::cout << "[ZbbAnalysis::ValueDivide] Numerator is zero, returning 0.0 +/- 0.0" << std::endl;
      return std::make_pair(0.0, 0.0);
    }
    if (p2.first == 0.0) {
      std::cout << "[ZbbAnalysis::ValueDivide] Denominator is zero, returning 0.0 +/- 0.0" << std::endl;
      return std::make_pair(0.0, 0.0);
    }
    double f = p1.first / p2.first;
    double a_sqrd = std::pow(p1.second / p1.first, 2.0);
    double b_sqrd = std::pow(p2.second / p2.first, 2.0);
    double f_err = f * sqrt( a_sqrd + b_sqrd );
    return std::make_pair(f, f_err);
  }
  ZbbAnalysis::Value ZbbAnalysis::ValueAdd(Value const& p1, Value const& p2) {
    double f = p1.first + p2.first;
    double f_err = sqrt( p1.second*p1.second + p2.second*p2.second );
    return std::make_pair(f, f_err);
  }
  ZbbAnalysis::Value ZbbAnalysis::ValueSubtract(Value const& p1, Value const& p2) {
    double f = p1.first - p2.first;
    double f_err = sqrt( p1.second*p1.second + p2.second*p2.second );
    return std::make_pair(f, f_err);
  }

  void ZbbAnalysis::PrintValue(std::string const& label, Value const& val) {
    std::cout << (boost::format("%-45s %10.3f +/-   %10.3f  (%.4f)") % (label+":") % val.first % val.second % (val.second/val.first)) << std::endl;
  }

  std::string ZbbAnalysis::ResolveAlias(std::string const& al) {
    if (alias_map_.count(al)) {
      return alias_map_.find(al)->second;
    } else {
      std::cerr << "Warning in <ZbbAnalysis::ResolveAlias>: No alias for " << al << " found" << std::endl;
      return al;
    }
  }

  std::vector<std::string> ZbbAnalysis::ResolveSamplesAlias(std::string const& al) {
    if (samples_alias_map_.count(al)) {
      return samples_alias_map_.find(al)->second;
    } else {
      std::cerr << "Warning in <ZbbAnalysis::ResolveSamplesAlias>: No alias for " << al << " found" << std::endl;
      return std::vector<std::string>();
    }
  }

  void ZbbAnalysis::SetAlias(std::string const& al, std::string const& val) {
    alias_map_[al] = val;
  }
}

