#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTPlotTools.h"
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
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plotting.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plotting_Style.h"
#include "TPad.h"
#include "TROOT.h"
#include "TColor.h"
#include "TEfficiency.h"


namespace ic {

  HTTPlot::HTTPlot() : config_("Config") {
    bkg_schemes_["all"] = {
      PlotBkgComponent("qcd","QCD"                  ,{"QCD","Fakes"}      , kMagenta-10),
      PlotBkgComponent("top","t#bar{t}"             ,{"TT","ttbar"}       , kBlue   - 8),
      PlotBkgComponent("ewk","Electroweak"          ,{"W","VV","EWK"}     , kRed    + 2),
      PlotBkgComponent("zll","Z#rightarrowll"       ,{"ZL","ZJ","ZLL"}    , kAzure  + 2),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau" ,{"ZTT","Ztt"}        , kOrange - 4)
    };
    bkg_schemes_["et_default"] = {
      PlotBkgComponent("qcd","QCD"                  ,{"QCD"}      ,TColor::GetColor(250,202,255)),
      PlotBkgComponent("top","t#bar{t}"             ,{"TT"}       ,TColor::GetColor(155,152,204)),
      PlotBkgComponent("ewk","Electroweak"          ,{"W","VV"}   ,TColor::GetColor(222, 90,106)),
      PlotBkgComponent("zll","Z#rightarrowee"       ,{"ZL","ZJ"}  ,TColor::GetColor(100,182,232)),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau" ,{"ZTT"}      ,TColor::GetColor(248,206,104))
    };
    //For start of run 2 before data
    bkg_schemes_["et_no_qcd"] = {
      PlotBkgComponent("top","t#bar{t}"             ,{"TT"}       ,TColor::GetColor(155,152,204)),
      PlotBkgComponent("ewk","Electroweak"          ,{"W","VV"}   ,TColor::GetColor(222, 90,106)),
      PlotBkgComponent("zll","Z#rightarrowee"       ,{"ZL","ZJ"}  ,TColor::GetColor(100,182,232)),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau" ,{"ZTT"}      ,TColor::GetColor(248,206,104))
    };
    bkg_schemes_["et_zll"] = {
      PlotBkgComponent("qcd","QCD"                  ,{"QCD"}      , kMagenta-10),
      PlotBkgComponent("top","t#bar{t}"             ,{"TT"}       , kBlue   - 8),
      PlotBkgComponent("ewk","Electroweak"          ,{"W","VV"}   , kRed    + 2),
      PlotBkgComponent("zll","Z#rightarrowee"       ,{"ZLL"}      , kAzure  + 2),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau" ,{"ZTT"}      , kOrange - 4)
    };
    bkg_schemes_["zee_default"] = {
      PlotBkgComponent("top","t#bar{t}"             ,{"TT"}       ,TColor::GetColor(155,152,204)),
      PlotBkgComponent("ewk","Electroweak"          ,{"W","VV"}   ,TColor::GetColor(222, 90,106)),
      PlotBkgComponent("zll","Z#rightarrowee"       ,{"ZLL"}  ,TColor::GetColor(100,182,232))
    };

    bkg_schemes_["et_with_zj"] = {
      PlotBkgComponent("qcd","QCD"                      ,{"QCD"}      , kMagenta-10),
      PlotBkgComponent("top","t#bar{t}"                 ,{"TT"}       , kBlue   - 8),
      PlotBkgComponent("ewk","Electroweak"              ,{"W","VV"}   , kRed    + 2),
      PlotBkgComponent("zl","Z#rightarrowee (lepton)"   ,{"ZL"}       , kAzure  + 2),
      PlotBkgComponent("zj","Z#rightarrowee (jet)"      ,{"ZJ"}       , kGreen  + 2),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau"     ,{"ZTT"}      , kOrange - 4)
    };
    bkg_schemes_["mt_default"] = {
      PlotBkgComponent("qcd","QCD",                   {"QCD"},      TColor::GetColor(250,202,255)),
      PlotBkgComponent("top","t#bar{t}",              {"TT"},       TColor::GetColor(155,152,204)),
      PlotBkgComponent("ewk","Electroweak",           {"W","VV","ZL","ZJ"},   TColor::GetColor(222, 90,106)),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau",  {"ZTT"} ,     TColor::GetColor(248,206,104))
    };
    bkg_schemes_["em_default"] = {
      PlotBkgComponent("qcd","Misidentified e/#mu"           ,{"QCD"}            ,TColor::GetColor(250,202,255)),
      PlotBkgComponent("ewk","Electroweak"          ,{"VV"}              ,TColor::GetColor(222, 90,106)),
      PlotBkgComponent("top","t#bar{t}"             ,{"TT"}            ,TColor::GetColor(155,152,204)),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau" ,{"ZTT"}              ,TColor::GetColor(248,206,104))
    };
    //For start of run 2 before data
    bkg_schemes_["em_no_qcd"] = {
      PlotBkgComponent("ewk","Electroweak"          ,{"VV"}              ,TColor::GetColor(222, 90,106)),
      PlotBkgComponent("top","t#bar{t}"             ,{"TT"}            ,TColor::GetColor(155,152,204)),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau" ,{"ZTT"}              ,TColor::GetColor(248,206,104))
    };
    bkg_schemes_["em_with_hww"] = {
      PlotBkgComponent("qcd","Misidentified e/#mu"  ,{"QCD"}            ,TColor::GetColor(250,202,255)),
      PlotBkgComponent("ewk","Electroweak"          ,{"VV"}              ,TColor::GetColor(222, 90,106)),
      PlotBkgComponent("top","t#bar{t}"             ,{"TT"}            ,TColor::GetColor(155,152,204)),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau" ,{"ZTT"}              ,TColor::GetColor(248,206,104)),
      PlotBkgComponent("hww","SM H(125 GeV)#rightarrow WW" ,{"ggH_hww125","qqH_hww125"}, TColor::GetColor(36,139,2))
    };
    bkg_schemes_["mt_with_zmm"] = {
      PlotBkgComponent("qcd","QCD",                   {"QCD"},      TColor::GetColor(250,202,255)),
      PlotBkgComponent("top","t#bar{t}",              {"TT"},       TColor::GetColor(155,152,204)),
      PlotBkgComponent("ewk","Electroweak",           {"W","VV"},   TColor::GetColor(222, 90,106)),
      PlotBkgComponent("zll","Z#rightarrow#mu#mu",    {"ZL","ZJ"},  TColor::GetColor(100,182,232)),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau",  {"ZTT"} ,     TColor::GetColor(248,206,104))
    };
    //For start of run 2 before data
    bkg_schemes_["mt_no_qcd"] = {
      PlotBkgComponent("top","t#bar{t}",              {"TT"},       TColor::GetColor(155,152,204)),
      PlotBkgComponent("ewk","Electroweak",           {"W","VV"},   TColor::GetColor(222, 90,106)),
      PlotBkgComponent("zll","Z#rightarrow#mu#mu",    {"ZL","ZJ"},  TColor::GetColor(100,182,232)),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau",  {"ZTT"} ,     TColor::GetColor(248,206,104))
    };
    bkg_schemes_["zmm_default"] = {
      PlotBkgComponent("top","t#bar{t}",              {"TT"},       TColor::GetColor(155,152,204)),
      PlotBkgComponent("ewk","Electroweak",           {"W","VV"},   TColor::GetColor(222, 90,106)),
      PlotBkgComponent("zll","Z#rightarrow#mu#mu",    {"ZLL"},  TColor::GetColor(100,182,232))
    };
    //For start of run 2 before data
    //For start of run 2 before data
    bkg_schemes_["tt_no_qcd"] = {
      PlotBkgComponent("top","t#bar{t}",              {"TT"},       TColor::GetColor(155,152,204)),
      PlotBkgComponent("ewk","Electroweak",           {"W","VV","ZL","ZJ"},   TColor::GetColor(222, 90,106)),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau",  {"ZTT"} ,     TColor::GetColor(248,206,104))
    };
    bkg_schemes_["tt_default"] = {
      PlotBkgComponent("qcd","QCD",                   {"QCD"},      TColor::GetColor(250,202,255)),
      PlotBkgComponent("top","t#bar{t}",              {"TT"},       TColor::GetColor(155,152,204)),
      PlotBkgComponent("ewk","Electroweak",           {"W","VV","ZL","ZJ"},   TColor::GetColor(222, 90,106)),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau",  {"ZTT"} ,     TColor::GetColor(248,206,104))
    };
    bkg_schemes_["mt_with_zj"] = {
      PlotBkgComponent("qcd","QCD",{"QCD"},kMagenta-10),
      PlotBkgComponent("top","t#bar{t}",{"TT"},kBlue   - 8),
      PlotBkgComponent("ewk","Electroweak",{"W","VV"},kRed    + 2),
      PlotBkgComponent("zl","Z#rightarrow#mu#mu (lepton)",{"ZL"},kAzure  + 2),
      PlotBkgComponent("zj","Z#rightarrow#mu#mu (jet)",{"ZJ"},kGreen  + 2),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau",{"ZTT"},kOrange - 4)
    };
    bkg_schemes_["mt_with_zmm_zll"] = {
      PlotBkgComponent("qcd","QCD",{"QCD"},kMagenta-10),
      PlotBkgComponent("top","t#bar{t}",{"TT"},kBlue   - 8),
      PlotBkgComponent("ewk","Electroweak",{"W","VV"},kRed    + 2),
      PlotBkgComponent("zll","Z#rightarrow#mu#mu",{"ZLL"},kAzure  + 2),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau",{"ZTT"},kOrange - 4)
    };
    bkg_schemes_["mt_big_split"] = {
      PlotBkgComponent("qcd","QCD",                   {"QCD"},      TColor::GetColor(250,202,255)),
      PlotBkgComponent("top","t#bar{t}",              {"TT"},       TColor::GetColor(155,152,204)),
      PlotBkgComponent("ewk","W+jets",                {"W"},        TColor::GetColor(222, 90,106)),
      PlotBkgComponent("dib","Diboson",               {"VV"},       kGreen    +2),
      PlotBkgComponent("zll","Z#rightarrow#mu#mu",    {"ZL","ZJ"},  TColor::GetColor(100,182,232)),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau",  {"ZTT"} ,     TColor::GetColor(248,206,104))
    };

    bkg_schemes_["tau_modes"] = {
      PlotBkgComponent("qcd","QCD"            ,{"QCD"}                ,TColor::GetColor(250,202,255)),
      PlotBkgComponent("top","t#bar{t}"       ,{"TT"}                 ,TColor::GetColor(155,152,204)),
      PlotBkgComponent("ewk","Electroweak"    ,{"W","VV"}             ,TColor::GetColor(222,90,106)),
      PlotBkgComponent("zll", "Z#rightarrow#mu#mu", {"ZL","ZJ"}        ,TColor::GetColor(100,182,232)),
      PlotBkgComponent("ztt1","Z#rightarrow#tau#tau   1 #pi^{#pm} no photons"   ,{"ZTT-1P0PZ"}  ,kOrange + 1),
      PlotBkgComponent("ztt2","Z#rightarrow#tau#tau   1 #pi^{#pm} + photons"    ,{"ZTT-1P1PZ"}  ,kOrange - 0),
      PlotBkgComponent("ztt3","Z#rightarrow#tau#tau   3 #pi^{#pm}"             ,{"ZTT-3P"}     ,17)
    };
  }
  void HTTPlot::SetMCStackStyle(TH1* hist_ptr, unsigned color) {
    hist_ptr->SetFillColor(color);
    hist_ptr->SetFillStyle(1001);
    hist_ptr->SetLineColor(1);
    hist_ptr->SetLineStyle(1);
    hist_ptr->SetLineWidth(2);
    hist_ptr->SetMarkerColor(1);
    hist_ptr->SetMarkerStyle(21);
    hist_ptr->SetMarkerSize(0.8);
    hist_ptr->SetStats(false);
    return;
  }
  void HTTPlot::SetSignalStyle(TH1* hist_ptr, unsigned color) {
    hist_ptr->SetFillStyle(1001);
    gStyle->SetLineStyleString(11,"20 10");
    hist_ptr->SetLineStyle(11);
    hist_ptr->SetFillColor(0);
    hist_ptr->SetLineColor(color);
    hist_ptr->SetLineWidth(3);
    return;
  }
  void HTTPlot::SetDataStyle(TH1* hist_ptr) {
    hist_ptr->SetMarkerColor(1);
    hist_ptr->SetLineColor(1);
    hist_ptr->SetFillColor(1);
    hist_ptr->SetFillStyle(0);
    hist_ptr->SetLineWidth(2);
    hist_ptr->SetMarkerStyle(20);
    hist_ptr->SetMarkerSize(1.1);
    return;
  }

  boost::program_options::options_description const& HTTPlot::GenerateOptions(std::string const& prefix) {
    namespace po = boost::program_options;
    config_.add_options()
      ((prefix+"use_htt_style").c_str(),        po::value<bool>(&use_htt_style_)->default_value(false))
      ((prefix+"plot_name").c_str(),            po::value<std::string>(&plot_name_)->default_value(""))
      ((prefix+"x_axis_label").c_str(),         po::value<std::string>(&x_axis_label_)->default_value(""))
      ((prefix+"y_axis_label").c_str(),         po::value<std::string>(&y_axis_label_)->default_value("Events"))
      ((prefix+"x_axis_bin_labels").c_str(),    po::value<std::string>(&x_axis_bin_labels_)->default_value(""))
      ((prefix+"custom_y_axis_min").c_str(),    po::value<bool>(&custom_y_axis_min_)->default_value(false))
      ((prefix+"y_axis_min").c_str(),           po::value<double>(&y_axis_min_)->default_value(0.0))
      ((prefix+"custom_x_axis_range").c_str(),  po::value<bool>(&custom_x_axis_range_)->default_value(false))
      ((prefix+"x_axis_min").c_str(),           po::value<double>(&x_axis_min_)->default_value(0.0))
      ((prefix+"x_axis_max").c_str(),           po::value<double>(&x_axis_max_)->default_value(1.0))
      ((prefix+"extra_pad").c_str(),            po::value<double>(&extra_pad_)->default_value(0.0))
      ((prefix+"background_scheme").c_str(),    po::value<std::string>(&background_scheme_)->default_value(""))
      ((prefix+"signal_scheme").c_str(),        po::value<std::string>(&signal_scheme_)->default_value(""))
      ((prefix+"draw_signal_mass").c_str(),     po::value<std::string>(&draw_signal_mass_)->default_value(""))
      ((prefix+"draw_signal_tanb").c_str(),     po::value<std::string>(&draw_signal_tanb_)->default_value(""))
      ((prefix+"cms_label").c_str(),            po::value<std::string>(&cms_label_)->default_value("CMS"))
      ((prefix+"cms_extra").c_str(),            po::value<std::string>(&cms_extra_)->default_value("Preliminary"))
      ((prefix+"lumi_label").c_str(),            po::value<std::string>(&lumi_label_)->default_value("19.7 fb^{-1} at 8 TeV"))
      ((prefix+"signal_scale").c_str(),         po::value<unsigned>(&signal_scale_)->default_value(1))
      ((prefix+"log_y").c_str(),                po::value<bool>(&log_y_)->default_value(false))
      ((prefix+"draw_ratio").c_str(),           po::value<bool>(&draw_ratio_)->default_value(false))
      ((prefix+"norm_bins").c_str(),            po::value<bool>(&norm_bins_)->default_value(false))
      ((prefix+"blind").c_str(),                po::value<bool>(&blind_)->default_value(false))
      ((prefix+"x_blind_min").c_str(),          po::value<double>(&x_blind_min_)->default_value(0))
      ((prefix+"x_blind_max").c_str(),          po::value<double>(&x_blind_max_)->default_value(0))
      ((prefix+"auto_error_band").c_str(),      po::value<double>(&auto_error_band_)->default_value(-1.0))
      ((prefix+"draw_error_band").c_str(),      po::value<bool>(&draw_error_band_)->default_value(false))
      ((prefix+"add_stat_error").c_str(),       po::value<bool>(&add_stat_error_)->default_value(true))
      ((prefix+"ratio_min").c_str(),            po::value<double>(&ratio_min_)->default_value(0.68))
      ((prefix+"ratio_max").c_str(),            po::value<double>(&ratio_max_)->default_value(1.32));

    return config_;
    // ("y_axis_min",          po::value<double>(&y_axis_min)->default_value(-10))
    // ("extra_pad",           po::value<double>(&extra_pad)->default_value(1.0))
    // ("norm_bins",           po::value<bool>(&norm_bins)->default_value(false))
    // ("signal_no_stack",     po::value<bool>(&signal_no_stack)->default_value(false))
    // ("signal_split_vbf",    po::value<bool>(&signal_split_vbf)->default_value(false))
    // ("make_datacard",       po::value<bool>(&make_datacard)->default_value(false))
    // ("swap_inclusive",      po::value<bool>(&swap_inclusive)->default_value(false))
    // ("ztt_by_decay_mode",   po::value<bool>(&ztt_by_decay_mode)->default_value(false))
    // ("add_sm_signal_as_bg", po::value<bool>(&add_sm_signal_as_bg)->default_value(false))
    // ("tau_scale_mode",      po::value<unsigned>(&tau_scale_mode)->default_value(0))
    // ("shift_backgrounds",   po::value<bool>(&shift_backgrounds)->default_value(false))
    // ("shift_tscale",        po::value<bool>(&shift_tscale)->default_value(false))
    // ("draw_signal",         po::value<bool>(&draw_signal)->default_value(true))
    // ("is_paper",            po::value<bool>(&is_paper)->default_value(false))
    // ("draw_band_on_stack",  po::value<bool>(&draw_band_on_stack)->default_value(false))
    // ("tscale_shift",        po::value<double>(&tscale_shift)->default_value(0.0))
    // ("qcd_shift",           po::value<double>(&qcd_shift)->default_value(1.0))
    // ("top_shift",           po::value<double>(&top_shift)->default_value(1.0))
    // ("ztt_shift",           po::value<double>(&ztt_shift)->default_value(1.0))
    // ("zl_shift",            po::value<double>(&zl_shift)->default_value(1.0))
    // ("zj_shift",            po::value<double>(&zj_shift)->default_value(1.0))
    // ("w_shift",             po::value<double>(&w_shift)->default_value(1.0))
    // ("vv_shift",            po::value<double>(&vv_shift)->default_value(1.0))
    
  }

  void HTTPlot::GeneratePlot(HTTAnalysis::HistValueMap hmap) {

    // Probably move this somewhere else
    sig_schemes_["sm_default"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times SM H("+draw_signal_mass_+" GeV)#rightarrow#tau#tau"),
        {"ggH","qqH","VH"}, TColor::GetColor(0,18,255), true)
    };
    sig_schemes_["run2_mssm"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times SUSYGluGluH("+draw_signal_mass_+" GeV)#rightarrow#tau#tau"),
        {"ggH"}, TColor::GetColor(0,18,255), false)
    };
    sig_schemes_["run2_Hhh"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times RadionGluGluH("+draw_signal_mass_+" GeV)#rightarrowhh#rightarrowbb#tau#tau"),
        {"ggH"}, TColor::GetColor(0,18,255), false)
    };

   sig_schemes_["run2_sm"] = {
     PlotSigComponent("sig", 
       (boost::lexical_cast<std::string>(signal_scale_)+"#times GluGluH("+draw_signal_mass_+" GeV)#rightarrow#tau#tau"),
       {"ggH"}, TColor::GetColor(0,18,255), false)
    };

    sig_schemes_["sm_nomult"] = {
      PlotSigComponent("sig",
        ("SM H("+draw_signal_mass_+" GeV)#rightarrow#tau#tau"),
        {"ggH","qqH","VH"}, TColor::GetColor(0,18,255), true)
    };
    sig_schemes_["sm_nostack"] = {
      PlotSigComponent("sig",
        ("SM H("+draw_signal_mass_+" GeV)#rightarrow#tau#tau"),
        {"ggH","qqH","VH"}, TColor::GetColor(0,18,255), false)
    };
    sig_schemes_["mssm_default"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times #Phi#rightarrow#tau#tau(m_{A}="+draw_signal_mass_+",tan#beta="+draw_signal_tanb_+")"),
        {"ggH","bbH"}, TColor::GetColor(0,18,255), true)
    };
    sig_schemes_["mssm_nostack"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times #Phi#rightarrow#tau#tau(m_{A}="+draw_signal_mass_+",tan#beta="+draw_signal_tanb_+")"),
        {"ggH","bbH"}, TColor::GetColor(0,18,255), false)
    };
    sig_schemes_["mssm_nostack_sigsep"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times gg#Phi#rightarrow#tau#tau(m_{A}="+draw_signal_mass_+",tan#beta="+draw_signal_tanb_+")"),
        {"ggH"}, TColor::GetColor(0,18,255), false),
      PlotSigComponent("sig_bbH",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times bb#Phi#rightarrow#tau#tau(m_{A}="+draw_signal_mass_+",tan#beta="+draw_signal_tanb_+")"),
        {"bbH"}, kBlue+3, false)
    };
    sig_schemes_["mssm_nostack_1pb"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times gg#Phi#rightarrow#tau#tau(m_{A}="+draw_signal_mass_+", #sigma*BR = 1pb"+")"),
        {"ggH"}, TColor::GetColor(0,18,255), false),
      PlotSigComponent("sig_bbH",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times bb#Phi#rightarrow#tau#tau(m_{A}="+draw_signal_mass_+", #sigma*BR = 1pb"+")"),
        {"bbH"}, kBlue+3, false)
    };
    sig_schemes_["mssm_nopars"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times #Phi#rightarrow#tau#tau"),
        {"ggH","bbH"}, TColor::GetColor(0,18,255), true)
    };
    sig_schemes_["mssm_nostack_nopars"] = {
      PlotSigComponent("sig",
        ("MSSM h,H,A#rightarrow#tau#tau"),
        {"ggH","bbH"}, TColor::GetColor(0,18,255), false)
    };
    sig_schemes_["Hhh_nostack_AZh"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times H#rightarrowhh(m_{H}="+draw_signal_mass_+",tan#beta="+draw_signal_tanb_+")"),
        {"ggHTohhTo2Tau2B"}, kBlue+3, false),
      PlotSigComponent("sig_AZh",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times A#rightarrowZh(m_{A}="+draw_signal_mass_+",tan#beta="+draw_signal_tanb_+")"),
        {"ggAToZhToLLBB","ggAToZhToTauTauBB","ggAToZhToBBTauTau"}, kGreen+3, false)
    };
    sig_schemes_["Hhh_nostack"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times H#rightarrowhh(m_{H}="+draw_signal_mass_+",tan#beta="+draw_signal_tanb_+")"),
        {"ggHTohhTo2Tau2B"}, kBlue+3, false),
    };
    sig_schemes_["Hhh_1pb"] = {
      PlotSigComponent("sig",
        ("1pb H#rightarrowhh(m_{H}="+draw_signal_mass_+")"),
        {"ggHTohhTo2Tau2B"}, kBlue+3, false),
      PlotSigComponent("sig_AZh",
        ("1pb A#rightarrowZh(m_{A}="+draw_signal_mass_+")"),
        {"ggAToZhToLLBB","ggAToZhToTauTauBB","ggAToZhToBBTauTau"}, kGreen+3, false)
    };
    sig_schemes_["Hhh_highmass"] = {
      PlotSigComponent("sig_Radion",
        ("1pb Radion H#rightarrowhh(m_{H}="+draw_signal_mass_+")"),
        {"RadionToHH"}, kBlue+3, false),
      PlotSigComponent("sig_Graviton",
        ("1pb Graviton H#rightarrowhh(m_{H}="+draw_signal_mass_+")"),
        {"GravitonToHH"}, kGreen+3, false)
    };
    sig_schemes_["sm_split_vbf"] = {
      PlotSigComponent("sig",
        ("SM H("+draw_signal_mass_+")#rightarrow#tau#tau (ggH)"),
        {"ggH","VH"}, TColor::GetColor(0,18,255), false),
      PlotSigComponent("vbf",
        ("SM H("+draw_signal_mass_+")#rightarrow#tau#tau (VBF)"),
        {"qqH"}, 1, false)
    };
    sig_schemes_["sm_split_ww"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times H("+draw_signal_mass_+")#rightarrow#tau#tau"),
        {"ggH","VH","qqH"}, TColor::GetColor(0,18,255), true),
      PlotSigComponent("ww",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times H("+draw_signal_mass_+")#rightarrowWW"),
        {"ggH_hww","qqH_hww"}, TColor::GetColor(0,18,255), true)
    };
    sig_schemes_["sm_split_ww_nostack"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times H("+draw_signal_mass_+")#rightarrow#tau#tau"),
        {"ggH","VH","qqH"}, TColor::GetColor(0,18,255), false),
      PlotSigComponent("ww",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times H("+draw_signal_mass_+")#rightarrowWW"),
        {"ggH_hww","qqH_hww"}, TColor::GetColor(0,18,255), false)
    };

    ModTDRStyle();
    THStack thstack("stack","stack");
    
    // First collect the data histogram and blind it if necessary
    TH1F *data_hist = &hmap["data_obs"].first;
    if (norm_bins_) data_hist->Scale(1.0, "width");
    if (blind_) {
      for (int j = 0; j < data_hist->GetNbinsX(); ++j) {
        double low_edge = data_hist->GetBinLowEdge(j+1);
        double high_edge = data_hist->GetBinWidth(j+1)+data_hist->GetBinLowEdge(j+1);
        if ((low_edge > x_blind_min_ && low_edge < x_blind_max_) || (high_edge > x_blind_min_ && high_edge < x_blind_max_)) {
          data_hist->SetBinContent(j+1,0);
          data_hist->SetBinError(j+1,0);
        }
      }
    }
    
    // Setup canvas and 1/2 TPads as necessary
    TCanvas* canv = new TCanvas("c1", "c1");
    canv->cd();
  
    std::vector<TPad*> pads =
        draw_ratio_ ? TwoPadSplit(0.29, 0.00, 0.00) : OnePad();
   
    pads[0]->SetLogy(log_y_);
    // Create axes based on data hist and possible user specified axis range
    std::vector<TH1*> h = CreateAxisHists(2, data_hist, custom_x_axis_range_ ? x_axis_min_ : data_hist->GetXaxis()->GetXmin(), custom_x_axis_range_ ? x_axis_max_-0.01 : data_hist->GetXaxis()->GetXmax()-0.01);
    h[0]->Draw();
    
    //Deal with units in axis label
    std::string units="";
    size_t pos = x_axis_label_.find("[");
    if(pos!=std::string::npos) {
      units = x_axis_label_.substr(pos+1, x_axis_label_.find("]") - pos -1 );
      x_axis_label_ = x_axis_label_.substr(0, pos);
    }
    // Set second axis when necessary
    if (draw_ratio_) {
      pads[1]->cd();
      h[1]->Draw();
      h[1]->GetXaxis()->SetTitleOffset(1.0);
      SetupTwoPadSplitAsRatio(pads, "Obs/Exp", true, 0.65, 1.35);
      if(norm_bins_) {
        UnitAxes(h[1]->GetXaxis(), h[0]->GetYaxis(), x_axis_label_, units);
      } else {      
        StandardAxes(h[1]->GetXaxis(), h[0]->GetYaxis(), x_axis_label_, units);
      }
      h[1]->GetYaxis()->SetNdivisions(4);
      h[1]->GetXaxis()->SetTitleOffset(1.2);
      h[1]->GetYaxis()->SetTitleOffset(2.0);
      pads[0]->cd();
      h[0]->GetYaxis()->SetTitleOffset(2.0);
      pads[1]->SetGrid(0,1);
      //it complains if the minimum is set to 0 and you try to set log y scale
      if(log_y_) h[0]->SetMinimum(0.1);
      if(custom_y_axis_min_) h[0]->SetMinimum(y_axis_min_);
      if (x_axis_bin_labels_ != "") {
        std::vector<std::string> bin_labels;
        boost::split(bin_labels, x_axis_bin_labels_, boost::is_any_of(":"));
        if (long(bin_labels.size()) <= long(h[1]->GetNbinsX())) {
          for (unsigned binl = 0; binl < bin_labels.size(); ++binl) {
            h[1]->GetXaxis()->SetBinLabel(binl+1, bin_labels[binl].c_str());
          }
        } 
      }
    } else {
      if(norm_bins_) {
        UnitAxes(h[0]->GetXaxis(), h[0]->GetYaxis(), x_axis_label_, units);
      } else {      
        StandardAxes(h[0]->GetXaxis(), h[0]->GetYaxis(), x_axis_label_, units);
      }
      h[0]->GetXaxis()->SetTitleOffset(1.2);
      h[0]->GetYaxis()->SetTitleOffset(2.0);
      if(log_y_) h[0]->SetMinimum(0.1);
      if(custom_y_axis_min_) h[0]->SetMinimum(y_axis_min_);
      if (x_axis_bin_labels_ != "") {
        std::vector<std::string> bin_labels;
        boost::split(bin_labels, x_axis_bin_labels_, boost::is_any_of(":"));
        if (long(bin_labels.size()) <= long(h[0]->GetNbinsX())) {
          for (unsigned binl = 0; binl < bin_labels.size(); ++binl) {
            h[0]->GetXaxis()->SetBinLabel(binl+1, bin_labels[binl].c_str());
          }
        } 
      }
    }
    pads[0]->cd();
   
    // Setup legend
    TLegend *legend = PositionedLegend(0.40, 0.30, 3, 0.03);
    legend->SetTextFont(42);
    HTTPlot::SetDataStyle(data_hist);    
    legend->AddEntry(data_hist, "Observed", "ple");
    
    std::string bkr_list_for_ratio = "";
    // Produce stack from background components
    std::vector<PlotBkgComponent> bkg_scheme = bkg_schemes_[background_scheme_];
    std::vector<TH1PlotElement> bkg_elements;
    for (unsigned i = 0; i < bkg_scheme.size(); ++i) {
      if (i == 0) bkr_list_for_ratio += bkg_scheme[i].name;
      if (i != 0) bkr_list_for_ratio += ("+"+bkg_scheme[i].name);
      bool valid_element = false;
      for (unsigned j = 0; j < bkg_scheme[i].plots.size(); ++j) {
        if (hmap.count(bkg_scheme[i].plots[j])) {
          if (!valid_element) {
            bkg_elements.push_back(TH1PlotElement(bkg_scheme[i].name, &(hmap[bkg_scheme[i].plots[j]].first), bkg_scheme[i].legend));            
            valid_element = true;
          } else {
            bkg_elements.back().hist_ptr()->Add(&hmap[bkg_scheme[i].plots[j]].first);
          }
        }
      }
      if (valid_element) {
        TH1PlotElement ele = bkg_elements.back();
        HTTPlot::SetMCStackStyle(ele.hist_ptr(), bkg_scheme[i].color);
        if (norm_bins_) ele.hist_ptr()->Scale(1.0, "width");
        thstack.Add(ele.hist_ptr(),"HIST");
      }
    }
    //Loop again in opposite direction to fill the legend (looks better that way)
    for (int i = bkg_scheme.size()-1; i>=0; --i) {
        TH1PlotElement ele2 = bkg_elements[i];
        legend->AddEntry(ele2.hist_ptr(), ele2.legend_text().c_str(), "f");
    }
    
    //Draw backgrounds and data
    thstack.Draw("HISTSAME");
    canv->Update();
    
    std::vector<PlotSigComponent> sig_scheme = sig_schemes_[signal_scheme_];
    std::vector<TH1PlotElement> sig_elements;
    for (unsigned i = 0; i < sig_scheme.size(); ++i) {
      sig_elements.push_back(TH1PlotElement(sig_scheme[i].name, &(hmap[sig_scheme[i].plots[0]+draw_signal_mass_].first), sig_scheme[i].legend));
      for (unsigned j = 1; j < sig_scheme[i].plots.size(); ++j) {
        sig_elements.back().hist_ptr()->Add(&hmap[sig_scheme[i].plots[j]+draw_signal_mass_].first);
      }
      sig_elements.back().hist_ptr()->Scale(signal_scale_);
      HTTPlot::SetSignalStyle(sig_elements.back().hist_ptr(), sig_scheme[i].color);
      if (norm_bins_) sig_elements.back().hist_ptr()->Scale(1.0, "width");
      legend->AddEntry(sig_elements.back().hist_ptr(), sig_elements.back().legend_text().c_str(), "f");
      //Draw signal
      if(sig_scheme[i].in_stack) {
        thstack.Add(sig_elements.back().hist_ptr(), "HIST");
        canv->Update();
      } else {
        sig_elements.back().hist_ptr()->Draw("HISTsame");
      }
    }
    
    canv->Update();
   
    //Uncertainty band
    TH1F error_band;
    TH1F bkg_total;
    TH1PlotElement err_element;
    TH1PlotElement bkg_element;
    if (draw_error_band_) {
      if (auto_error_band_ <= 0.) {
        error_band = hmap["Bkg"].first;
        if (norm_bins_) error_band.Scale(1.0, "width");
      } else {
        error_band = *((TH1F*)bkg_elements[0].hist_ptr()->Clone());
        for (unsigned i = 1; i < bkg_elements.size(); ++i) {
          error_band.Add(bkg_elements[i].hist_ptr());
        }
        for (unsigned i = 1; i <= unsigned(error_band.GetNbinsX()); ++i) {
          if (add_stat_error_) {
            double from_frac = error_band.GetBinContent(i)*auto_error_band_;
            double from_bin = error_band.GetBinError(i);
            error_band.SetBinError(i, std::sqrt(from_frac*from_frac + from_bin*from_bin));
          } else {
            error_band.SetBinError(i, error_band.GetBinContent(i)*auto_error_band_);
          }
        }
      }
      bkg_total = error_band;
      for (unsigned i = 1; i <= unsigned(bkg_total.GetNbinsX()); ++i) {
        bkg_total.SetBinError(i, 0.0);
      }
      err_element = TH1PlotElement("error_shape", &error_band,"Bkg. uncertainty");
      bkg_element = TH1PlotElement("bkg_shape", &bkg_total,"");

      err_element.hist_ptr()->SetMarkerSize(0);
      //New transparent grey uncertainty bands
      int new_idx = CreateTransparentColor(13, 0.5);
      err_element.hist_ptr()->SetFillColor(new_idx);
      err_element.hist_ptr()->SetFillStyle(3001);
      err_element.hist_ptr()->SetLineWidth(1);
      bkg_element.hist_ptr()->SetMarkerSize(0);
      bkg_element.hist_ptr()->SetFillColor(1);
      bkg_element.hist_ptr()->SetFillStyle(0);
      bkg_element.hist_ptr()->SetLineWidth(1);
      err_element.hist_ptr()->Draw("e2same");
      legend->AddEntry(err_element.hist_ptr(), "Bkg. uncertainty" , "F" );
      canv->Update();
    } else {
      bkg_total = *((TH1F*)bkg_elements[0].hist_ptr()->Clone());
      for (unsigned i = 1; i < bkg_elements.size(); ++i) {
        bkg_total.Add(bkg_elements[i].hist_ptr());
      }
      for (unsigned i = 1; i <= unsigned(bkg_total.GetNbinsX()); ++i) {
        bkg_total.SetBinError(i, 0.0);
      }
      bkg_element = TH1PlotElement("bkg_shape", &bkg_total,"");
      bkg_element.hist_ptr()->SetMarkerSize(0);
      bkg_element.hist_ptr()->SetFillColor(1);
      bkg_element.hist_ptr()->SetFillStyle(0);
      bkg_element.hist_ptr()->SetLineWidth(1);
    }

    //Setup ratio plots
    TH1F *ratio = reinterpret_cast<TH1F*>(MakeRatioHist(data_hist, bkg_element.hist_ptr(), true, false));
    TH1F *err_ratio = NULL;
    if(draw_error_band_) err_ratio = reinterpret_cast<TH1F*>(MakeRatioHist(err_element.hist_ptr(), err_element.hist_ptr(), true, false));

    if (draw_ratio_) {
      pads[1]->cd();
      if(draw_error_band_) err_ratio->Draw("e2same");
      ratio->Draw("pe0same");
    }
    pads[0]->cd();
    //Finally draw the data
    data_hist->Draw("pesame");
    
    //Add any text elements - for now using ic::TextElement - could definitely change this to use DrawTitle function or similar
    TLatex *title_latex = new TLatex();
    title_latex->SetNDC();
    for (unsigned te = 0; te < text_.size(); ++te) {
      title_latex->SetTextSize(text_[te].size());
      title_latex->SetTextAlign(11);
      title_latex->DrawLatex(text_[te].x_pos(), text_[te].y_pos(), text_[te].text().c_str());
    } 

    //Set fraction of white space at the top of plot
    FixTopRange(pads[0], GetPadYMax(pads[0]), extra_pad_>0 ? extra_pad_ : 0.15);
    DrawCMSLogo(pads[0], cms_label_, cms_extra_, 11, 0.045, 0.035, 1.2);
    DrawTitle(pads[0], lumi_label_, 3);
    FixBoxPadding(pads[0], legend, 0.05);
    legend->Draw();
    FixOverlay();
    canv->Update();
    pads[0]->GetFrame()->Draw();
    std::string log = log_y_ ? "_log" : "";  
    canv->Print((plot_name_+log+".pdf").c_str());
    canv->Print((plot_name_+log+".png").c_str());
  }

  void HTTPlot::AddTextElement(ic::TextElement & ele) {
    text_.push_back(ele);
  }




}

