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

    /*
      vbf_no_cjv
      vbf_loose
    
    */
    if (ch_ != channel::em) {
      alias_map_["inclusive"]         = "";
      alias_map_["vbf"]               = "(n_jets>=2 && n_jetsingap==0 && mjj>500. && jdeta>3.5)";
      alias_map_["vbf_no_cjv"]        = "(n_jets>=2 && mjj>500. && jdeta>3.5)";
      alias_map_["vbf_loose"]         = "(n_jets>=2 && n_jetsingap==0 && mjj>200. && jdeta>2.0)";
      alias_map_["vbf_loose_jets20"]  = "(n_lowpt_jets>=2 && n_jetsingap_lowpt==0 && mjj_lowpt>200. && jdeta_lowpt>2.0)";
      alias_map_["twojet"]            = "(n_jets>=2)";
      alias_map_["1jet"]              = "(n_jets>=1)";
      alias_map_["1jet_high"]         = "!"+alias_map_["vbf"]+" && n_jets>=1 && pt_2>40. && n_bjets==0";
      if (ch_ == channel::et) alias_map_["1jet_high"] += " && met>30.";
      alias_map_["1jet_low"]          = "!"+alias_map_["vbf"]+" && n_jets>=1 && pt_2<=40. && n_bjets==0";
      if (ch_ == channel::et) alias_map_["1jet_low"] += " && met>30.";
      alias_map_["0jet_high"]         = "n_jets==0 && pt_2>40. && n_bjets==0";
      alias_map_["0jet_low"]          = "n_jets==0 && pt_2<=40. && n_bjets==0";
    } else {
      alias_map_["inclusive"]         = "";
      alias_map_["vbf"]               = "(n_jets>=2 && n_jetsingap==0 && mjj>500. && jdeta>3.5 && n_bjets==0)";
      alias_map_["vbf_no_cjv"]        = "(n_jets>=2 && mjj>500. && jdeta>3.5 && n_bjets==0)";
      alias_map_["vbf_loose"]         = "(n_jets>=2 && n_jetsingap==0 && mjj>200. && jdeta>2.0 && n_bjets==0)";
      alias_map_["vbf_loose_jets20"]  = "(n_lowpt_jets>=2 && n_jetsingap_lowpt==0 && mjj_lowpt>200. && jdeta_lowpt>2.0 && n_bjets==0)";
      alias_map_["twojet"]            = "(n_jets>=2 && n_bjets==0)";
      alias_map_["1jet"]              = "(n_jets>=1 && n_bjets==0)";
      alias_map_["1jet_high"]         = "!"+alias_map_["vbf"]+" && n_jets>=1 && pt_2>35. && n_bjets==0";
      alias_map_["1jet_low"]          = "!"+alias_map_["vbf"]+" && n_jets>=1 && pt_2<=35. && n_bjets==0";
      alias_map_["0jet_high"]         = "n_jets==0 && pt_2>35. && n_bjets==0";
      alias_map_["0jet_low"]          = "n_jets==0 && pt_2<=35. && n_bjets==0";
    }
    // alias_map_["prebtag"] = "n_jet<=1 && "
    alias_map_["btag"]          = "(n_jets<=1 && n_bjets>=1)";
    alias_map_["btag_loose"]    = "(n_jets<=1 && n_loose_bjets>=1)";
    alias_map_["nobtag"]        = "!"+alias_map_["vbf"]+" && n_bjets==0";

    alias_map_["w_extrp_os_sdb_sel"]        = "os && mt_1>70.";
    alias_map_["w_extrp_os_sig_sel"]        = "os && mt_1<20.";
    alias_map_["w_os_sdb_sel"]              = "os && mt_1>70.";
    alias_map_["vbf_w_extrp_os_sdb_sel"]    = "os && mt_1>60. && mt_1<120.";
    alias_map_["vbf_w_extrp_os_sig_sel"]    = "os && mt_1<20.";
    alias_map_["vbf_w_os_sdb_sel"]          = "os && mt_1>60. && mt_1<120.";
    alias_map_["w_shape_sel"]               = "os && mt_1<20.";
    alias_map_["qcd_sdb_sel"]               = "!os && mt_1<20.";
    alias_map_["w_extrp_ss_sdb_sel"]        = "!os && mt_1>70.";
    alias_map_["w_extrp_ss_sig_sel"]        = "!os && mt_1<20.";
    alias_map_["w_ss_sdb_sel"]              = "!os && mt_1>70.";
    alias_map_["em_qcd_sel"]                = "os";
    alias_map_["ss_sel"]                    = "!os && pzeta>-20.";

    samples_alias_map_["vv_samples"] = {
     "WWJetsTo2L2Nu", "WZJetsTo2L2Q", "WZJetsTo3LNu",
     "ZZJetsTo2L2Nu", "ZZJetsTo2L2Q", "ZZJetsTo4L",
     "T-tW", "Tbar-tW"
    };
    samples_alias_map_["w_sub_samples"] = {
     "WWJetsTo2L2Nu", "WZJetsTo2L2Q", "WZJetsTo3LNu",
     "ZZJetsTo2L2Nu", "ZZJetsTo2L2Q", "ZZJetsTo4L",
     "T-tW", "Tbar-tW"
    };
    if (year_ == "2011") {
      boost::range::push_back(samples_alias_map_["w_sub_samples"], std::vector<std::string>{
        "TTJets", "DYJetsToTauTau", "DYJetsToLL"});      
    } else {
      boost::range::push_back(samples_alias_map_["w_sub_samples"], std::vector<std::string>{
        "TTJets", "DYJetsToTauTauSoup", "DYJetsToLL-LSoup", "DYJetsToLL-JSoup"});      
    }
    samples_alias_map_["qcd_sub_samples"] = {
     "WWJetsTo2L2Nu", "WZJetsTo2L2Q", "WZJetsTo3LNu",
     "ZZJetsTo2L2Nu", "ZZJetsTo2L2Q", "ZZJetsTo4L",
     "T-tW", "Tbar-tW", "WJetsToLNuSoup"
    };
    if (year_ == "2011") {
      boost::range::push_back(samples_alias_map_["qcd_sub_samples"], std::vector<std::string>{
        "TTJets", "DYJetsToTauTauSoup", "DYJetsToLL-LSoup", "DYJetsToLL-JSoup"});      
    } else {
      boost::range::push_back(samples_alias_map_["qcd_sub_samples"], std::vector<std::string>{
        "TTJets", "DYJetsToTauTauSoup", "DYJetsToLL-LSoup", "DYJetsToLL-JSoup"});      
    }


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

  void HTTAnalysis::ReadTreesWithFallback(std::string const& folder, std::string const& fallback_folder) {
    if (verbosity_) std::cout << "Reading input files..." << std::endl;
    for (auto name : sample_names_) {
      // The input file is folder + sample name + channel + year
      std::string input_filename = folder+"/"+name+"_"+Channel2String(ch_)+"_"+year_+".root";
      std::string label = name;
      if (verbosity_) std::cout << input_filename << " --> " << label << std::endl;
      TFile *tmp_file = new TFile(input_filename.c_str());
      if (!(tmp_file->IsOpen())) {
        std::cerr << "Info in <HTTAnalysis::ReadTrees>: Trying fallback " << input_filename << std::endl;
        input_filename = fallback_folder+"/"+name+"_"+Channel2String(ch_)+"_"+year_+".root";
        if (verbosity_) std::cout << input_filename << " --> " << label << std::endl;
        tmp_file = new TFile(input_filename.c_str());
        if (!tmp_file) {
          std::cerr << "Warning in <HTTAnalysis::ReadTrees>: Unable to extract TTree from file " << input_filename << std::endl;
          continue;
        }
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

  void HTTAnalysis::FillHistoMap(HistoMap & hmap, unsigned method,
                        std::string variable,
                        std::string selection,
                        std::string category,
                        std::string weight,
                        std::string postfix) {
    std::string var = variable;
    std::string sel = selection;
    std::string cat = category;
    std::string wt = weight;
    bool use_soup = true;
    std::string soup = (year_ == "2011") ? "" : "Soup";
    if (!use_soup) soup = "";

    // Data
    auto data_norm = this->GetRate("Data", sel, cat, wt);
    TH1F data_hist = this->GetShape(var, "Data", sel, cat, wt);
    SetNorm(&data_hist, data_norm.first);
    PrintValue("data_obs"+postfix, data_norm);
    hmap["data_obs"+postfix] = std::make_pair(data_hist, data_norm);

    Value total_bkr;
    // Top
    auto top_norm = this->GetLumiScaledRate("TTJets", sel, cat, wt);
    std::string top_shape_sample = (year_ == "2011") ? "TTJets" : "TT";
    std::string top_shape_cat = cat;
    if (method == 5) {
      if (ch_ == channel::em) {
        top_shape_cat = this->ResolveAlias("vbf_no_cjv");
      } else {
        top_shape_cat = this->ResolveAlias("vbf_loose");
      }
    }
    TH1F top_hist = this->GetLumiScaledShape(var, top_shape_sample, sel, top_shape_cat, wt);
    SetNorm(&top_hist, top_norm.first);
    std::string top_map_label = (ch_ == channel::em) ? "ttbar" : "TT";
    PrintValue(top_map_label+postfix, top_norm);
    total_bkr = ValueAdd(total_bkr, top_norm);
    hmap[top_map_label+postfix] = std::make_pair(top_hist, top_norm);

    // Diboson
    std::vector<std::string> vv_samples = this->ResolveSamplesAlias("vv_samples");
    auto vv_norm = this->GetLumiScaledRate(vv_samples, sel, cat, wt);
    if (ch_ == channel::em && (method == 0 || method == 1)) {
      vv_norm = ValueProduct(vv_norm, std::make_pair(1.23, 0.0));
    }
    std::string vv_shape_cat = cat;
    if (method == 5 && ch_ != channel::em) {
      vv_shape_cat = this->ResolveAlias("vbf_loose");
    }
    TH1F vv_hist = this->GetLumiScaledShape(var, vv_samples, sel, cat, wt);
    SetNorm(&vv_hist, vv_norm.first);
    std::string vv_map_label = (ch_ == channel::em) ? "EWK" : "VV";
    PrintValue(vv_map_label+postfix, vv_norm);
    total_bkr = ValueAdd(total_bkr, vv_norm);
    hmap[vv_map_label+postfix] = std::make_pair(vv_hist, vv_norm);

    // Z->ll
    if (ch_ != channel::em) {
      Value zl_norm, zj_norm, zll_norm;
      TH1F zl_hist, zj_hist, zll_hist;
      if (method == 5) {
        zl_norm = this->GetRateViaRefEfficiency("Embedded", "DYJetsToLL-L"+soup, sel, this->ResolveAlias("twojet"), sel, cat, wt);
        zj_norm = this->GetLumiScaledRate("DYJetsToLL-J"+soup, sel, cat, wt);
        zl_hist = this->GetLumiScaledShape(var, "DYJetsToLL-L"+soup, sel, this->ResolveAlias("vbf_loose_jets20"), wt);
        zj_hist = this->GetLumiScaledShape(var, "DYJetsToLL-J"+soup, sel, this->ResolveAlias("vbf_loose"), wt);
      } else {
        std::string zll_shape_cat = cat;
        if (method == 6)  zll_shape_cat = "btag_low_loose";
        if (method == 7)  zll_shape_cat = "btag_high_loose";
        if (method == 12) zll_shape_cat = "btag_loose";
        zll_shape_cat = this->ResolveAlias(zll_shape_cat);
        zl_norm = this->GetLumiScaledRate("DYJetsToLL-L"+soup, sel, zll_shape_cat, wt);
        zl_hist = this->GetLumiScaledShape(var, "DYJetsToLL-L"+soup, sel, zll_shape_cat, wt);
        zj_norm = this->GetLumiScaledRate("DYJetsToLL-J"+soup, sel, zll_shape_cat, wt);
        zj_hist = this->GetLumiScaledShape(var, "DYJetsToLL-J"+soup, sel, zll_shape_cat, wt);
      }
      SetNorm(&zl_hist, zl_norm.first);
      SetNorm(&zj_hist, zj_norm.first);
      zll_norm = ValueAdd(zl_norm, zj_norm);
      zll_hist = zl_hist;
      zll_hist.Add(&zj_hist);
      PrintValue("ZLL"+postfix, zll_norm);
      PrintValue("ZL"+postfix, zl_norm);
      PrintValue("ZJ"+postfix, zj_norm);
      total_bkr = ValueAdd(total_bkr, zll_norm);
      hmap["ZLL"+postfix] = std::make_pair(zll_hist, zll_norm);
      hmap["ZL"+postfix]  = std::make_pair(zl_hist, zl_norm);
      hmap["ZJ"+postfix]  = std::make_pair(zj_hist, zj_norm);
    }

    // Z->tautau
    auto ztt_norm = this->GetRateViaRefEfficiency("Embedded", "DYJetsToTauTau"+soup, "os", "", sel, cat, wt);
    TH1F ztt_hist = this->GetShape(var, "Embedded", sel, cat, wt);
    SetNorm(&ztt_hist, ztt_norm.first);
    std::string ztt_map_label = (ch_ == channel::em) ? "Ztt" : "ZTT";
    PrintValue(ztt_map_label+postfix, ztt_norm);
    total_bkr = ValueAdd(total_bkr, ztt_norm);
    hmap[ztt_map_label+postfix] = std::make_pair(ztt_hist, ztt_norm);

    // W+jets
    if (ch_ != channel::em) {
      std::vector<std::string> w_sub_samples = this->ResolveSamplesAlias("w_sub_samples");
      std::string w_extrap_cat = cat;
      std::string w_extrp_sdb_sel = this->ResolveAlias("w_extrp_os_sdb_sel");
      std::string w_extrp_sig_sel = this->ResolveAlias("w_extrp_os_sig_sel");
      std::string w_sdb_sel = this->ResolveAlias("w_os_sdb_sel");
      if (method == 5) {
        w_extrap_cat    = this->ResolveAlias("vbf_loose");
        w_extrp_sdb_sel = this->ResolveAlias("vbf_w_extrp_os_sdb_sel");
        w_extrp_sig_sel = this->ResolveAlias("vbf_w_extrp_os_sig_sel");
        w_sdb_sel       = this->ResolveAlias("vbf_w_os_sdb_sel");
      }
      if (method == 6)  w_extrap_cat = this->ResolveAlias("btag_low_loose");
      if (method == 7)  w_extrap_cat = this->ResolveAlias("btag_high_loose");
      if (method == 12) w_extrap_cat = this->ResolveAlias("btag_loose");
      
      auto w_norm = this->GetRateViaWMethod("WJetsToLNuSoup", w_extrap_cat, w_extrp_sdb_sel, w_extrp_sig_sel, 
          "Data", cat, w_sdb_sel, w_sub_samples, wt, ValueFnMap());
      std::string w_shape_cat = cat;
      std::string w_shape_sel = this->ResolveAlias("w_shape_sel");
      if (method == 5)  w_shape_cat = this->ResolveAlias("vbf_loose");
      if (method == 6)  w_shape_cat = this->ResolveAlias("btag_low_loose");
      if (method == 7)  w_shape_cat = this->ResolveAlias("btag_high_loose");
      if (method == 12) w_shape_cat = this->ResolveAlias("btag_loose");
      TH1F w_hist = this->GetShape(var, "WJetsToLNuSoup", w_shape_sel, w_shape_cat, wt);
      SetNorm(&w_hist, w_norm.first);
      PrintValue("W"+postfix, w_norm);
      total_bkr = ValueAdd(total_bkr, w_norm);
      hmap["W"+postfix] = std::make_pair(w_hist, w_norm);
    }

    // QCD/Fakes
    Value qcd_norm;
    TH1F qcd_hist;
    if (ch_ != channel::em) {
      std::vector<std::string> qcd_sub_samples = this->ResolveSamplesAlias("qcd_sub_samples");
      std::vector<std::string> w_sub_samples = this->ResolveSamplesAlias("w_sub_samples");
      std::string qcd_sdb_sel = this->ResolveAlias("qcd_sdb_sel");
      std::string w_extrp_sdb_sel = this->ResolveAlias("w_extrp_ss_sdb_sel");
      std::string w_extrp_sig_sel = this->ResolveAlias("w_extrp_ss_sig_sel");
      std::string w_sdb_sel = this->ResolveAlias("w_ss_sdb_sel");
      std::string qcd_cat = cat;
      if (method == 5) qcd_cat = this->ResolveAlias("inclusive");
      qcd_norm = this->GetRateViaQCDMethod(std::make_pair(1.06,0.), "Data", qcd_sdb_sel, qcd_cat, qcd_sub_samples, wt, {
        {"WJetsToLNuSoup", [&]()->HTTAnalysis::Value {
          return this->GetRateViaWMethod("WJetsToLNuSoup", qcd_cat, w_extrp_sdb_sel, w_extrp_sig_sel, 
            "Data", qcd_cat, w_sdb_sel, w_sub_samples, wt, ValueFnMap());}
        } 
      });
      if (method == 5) {
        Value qcd_eff = this->SampleEfficiency("Special_3_Data", qcd_sdb_sel, qcd_cat, qcd_sdb_sel, cat, wt);
        qcd_norm = ValueProduct(qcd_norm, qcd_eff);
      }
      if (method == 0 || method == 8 || method == 11) {
        qcd_hist = this->GetShapeViaQCDMethod(var, "Data", qcd_sdb_sel, qcd_cat, qcd_sub_samples, wt, {
          {"WJetsToLNuSoup", [&]()->HTTAnalysis::Value {
            return this->GetRateViaWMethod("WJetsToLNuSoup", qcd_cat, w_extrp_sdb_sel, w_extrp_sig_sel, 
              "Data", qcd_cat, w_sdb_sel, w_sub_samples, wt, ValueFnMap());} 
          }
        });
      } else {
        if (method == 5)  qcd_cat = this->ResolveAlias("vbf_loose_jets20");
        if (method == 6)  qcd_cat = this->ResolveAlias("btag_low_loose");
        if (method == 7)  qcd_cat = this->ResolveAlias("btag_high_loose");
        if (method == 12) qcd_cat = this->ResolveAlias("btag_loose");        
        qcd_hist = this->GetShape(var, "Special_3_Data", qcd_sdb_sel, qcd_cat, wt);
      }
    } else {
      Value qcd_dilepton = ValueAdd( this->GetRate("Special_20_Data", this->ResolveAlias("em_qcd_sel"), "", wt),
                                     this->GetRate("Special_21_Data", this->ResolveAlias("em_qcd_sel"), "", wt));
      qcd_dilepton = ValueSubtract(qcd_dilepton, this->GetRate("Special_22_Data", this->ResolveAlias("em_qcd_sel"), "", wt));
      qcd_dilepton = ValueProduct(qcd_dilepton, std::make_pair(0.83,0.));
      Value qcd_eff = this->SampleEfficiency("Data", this->ResolveAlias("em_qcd_sel"), "", sel, cat, wt);
      if (method == 0 || method == 2) {
        qcd_norm = ValueProduct(qcd_dilepton, qcd_eff);
        qcd_hist = this->GetShape(var, "Special_23_Data", this->ResolveAlias("ss_sel"), cat, wt);
      } else if (method == 1) {
        qcd_norm = ValueProduct(qcd_dilepton, qcd_eff);
        qcd_hist = this->GetShape(var, "Special_24_Data", this->ResolveAlias("ss_sel"), cat, wt);
        TH1F fr_hist =  this->GetShape(var, "Special_20_Data", sel, cat, wt);
        TH1F fr_m_hist =  this->GetShape(var, "Special_21_Data", sel, cat, wt);
        TH1F fr_em_hist = this->GetShape(var, "Special_22_Data", sel, cat, wt);
        fr_hist.Add(&fr_m_hist,1.0);
        fr_hist.Add(&fr_em_hist,-1.0);
        qcd_hist.Scale(0.2 / Integral(&qcd_hist));
        fr_hist.Scale(0.8 / Integral(&fr_hist));
        qcd_hist.Add(&fr_hist,1.0);
      } else if (method == 3) {
        qcd_norm = ValueAdd(this->GetRate("Special_20_Data", sel, cat, wt),
                            this->GetRate("Special_21_Data", sel, cat, wt));
        qcd_norm = ValueSubtract(qcd_norm, this->GetRate("Special_22_Data", sel, cat, wt));
        qcd_hist        = this->GetShape(var, "Special_20_Data", sel, cat, wt);
        TH1F fr_m_hist  = this->GetShape(var, "Special_21_Data", sel, cat, wt);
        TH1F fr_em_hist = this->GetShape(var, "Special_22_Data", sel, cat, wt);
        qcd_hist.Add(&fr_m_hist,1.0);
        qcd_hist.Add(&fr_em_hist,-1.0);
      } else if (method == 5) {
       qcd_norm = ValueAdd(this->GetRate("Special_20_Data", sel, cat, wt),
                           this->GetRate("Special_21_Data", sel, cat, wt));
       qcd_norm = ValueSubtract(qcd_norm, this->GetRate("Special_22_Data", sel, cat, wt));
       qcd_hist        = this->GetShape(var, "Special_20_Data", sel, this->ResolveAlias("vbf_no_cjv"), wt);
       TH1F fr_m_hist  = this->GetShape(var, "Special_21_Data", sel, this->ResolveAlias("vbf_no_cjv"), wt);
       TH1F fr_em_hist = this->GetShape(var, "Special_22_Data", sel, this->ResolveAlias("vbf_no_cjv"), wt);
       qcd_hist.Add(&fr_m_hist,1.0);
       qcd_hist.Add(&fr_em_hist,-1.0); 
     } else {
      qcd_norm = ValueProduct(qcd_dilepton, qcd_eff);
      qcd_hist = this->GetShape(var, "Data", this->ResolveAlias("ss_sel"), cat, wt);
     }
    }
    SetNorm(&qcd_hist, qcd_norm.first);
    std::string qcd_map_label = (ch_ == channel::em) ? "Fakes" : "QCD";
    PrintValue(qcd_map_label+postfix, qcd_norm);
    total_bkr = ValueAdd(total_bkr, qcd_norm);
    hmap[qcd_map_label+postfix] = std::make_pair(qcd_hist, qcd_norm);

    PrintValue("Total"+postfix, total_bkr);

    return;
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
    double eff = num.first / den.first;
    TEfficiency teff;
    double eff_err = teff.ClopperPearson(den_eff_rounded,num_eff_rounded,0.683 ,1)-(num_eff/den_eff);
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
    std::cout << (boost::format("%-12s %-10.8f +/-   %-10.2f  (%.4f)") % (label+":") % val.first % val.second % (val.second/val.first)) << std::endl;
  }

  std::string HTTAnalysis::ResolveAlias(std::string const& al) {
    if (alias_map_.count(al)) {
      return alias_map_.find(al)->second;
    } else {
      std::cerr << "Warning in <HTTAnalysis::ResolveAlias>: No alias for " << al << " found" << std::endl;
      return al;
    }
  }

  std::vector<std::string> HTTAnalysis::ResolveSamplesAlias(std::string const& al) {
    if (samples_alias_map_.count(al)) {
      return samples_alias_map_.find(al)->second;
    } else {
      std::cerr << "Warning in <HTTAnalysis::ResolveSamplesAlias>: No alias for " << al << " found" << std::endl;
      return std::vector<std::string>();
    }
  }



}

