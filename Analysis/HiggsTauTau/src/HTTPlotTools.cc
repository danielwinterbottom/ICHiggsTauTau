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
#include "TPad.h"
#include "TROOT.h"
#include "TEfficiency.h"


namespace ic {

  HTTPlot::HTTPlot() : config_("Config") {
    bkg_schemes_["et_default"] = {
      PlotBkgComponent("qcd","QCD"                  ,{"QCD"}      , kMagenta-10),
      PlotBkgComponent("top","t#bar{t}"             ,{"TT"}       , kBlue   - 8),
      PlotBkgComponent("ewk","electroweak"          ,{"W","VV"}   , kRed    + 2),
      PlotBkgComponent("zll","Z#rightarrowee"       ,{"ZL","ZJ"}  , kAzure  + 2),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau" ,{"ZTT"}      , kOrange - 4)
    };
    bkg_schemes_["mt_default"] = {
      PlotBkgComponent("qcd","QCD"                  ,{"QCD"}              ,kMagenta-10),
      PlotBkgComponent("top","t#bar{t}"             ,{"TT"}               ,kBlue   - 8),
      PlotBkgComponent("ewk","electroweak"          ,{"W","VV","ZL","ZJ"} ,kRed    + 2),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau" ,{"ZTT"}              ,kOrange - 4)
    };
    bkg_schemes_["em_default"] = {
      PlotBkgComponent("qcd","fakes"                ,{"Fakes"}            ,kMagenta-10),
      PlotBkgComponent("ewk","electroweak"          ,{"EWK"}              ,kRed    + 2),
      PlotBkgComponent("top","t#bar{t}"             ,{"ttbar"}            ,kBlue   - 8),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau" ,{"Ztt"}              ,kOrange - 4)
    };
    bkg_schemes_["mt_with_zmm"] = {
      PlotBkgComponent("qcd","QCD",{"QCD"},kMagenta-10),
      PlotBkgComponent("top","t#bar{t}",{"TT"},kBlue   - 8),
      PlotBkgComponent("ewk","electroweak",{"W","VV"},kRed    + 2),
      PlotBkgComponent("zll","Z#rightarrow#mu#mu",{"ZL","ZJ"},kAzure  + 2),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau",{"ZTT"},kOrange - 4)
    };
    bkg_schemes_["mt_with_zmm_zll"] = {
      PlotBkgComponent("qcd","QCD",{"QCD"},kMagenta-10),
      PlotBkgComponent("top","t#bar{t}",{"TT"},kBlue   - 8),
      PlotBkgComponent("ewk","electroweak",{"W","VV"},kRed    + 2),
      PlotBkgComponent("zll","Z#rightarrow#mu#mu",{"ZLL"},kAzure  + 2),
      PlotBkgComponent("ztt","Z#rightarrow#tau#tau",{"ZTT"},kOrange - 4)
    };
  }

  void HTTPlot::SetMCStackStyle(ic::TH1PlotElement & ele, unsigned color) {
    ele.set_fill_color(color);
    ele.set_fill_style(1001);
    ele.set_draw_fill(true);
    ele.set_draw_marker(false);
    ele.set_draw_line(false);
    ele.set_line_width(2);
    ele.set_draw_stat_error_y(false);
    ele.set_in_stack(true);
    return;
  }
  void HTTPlot::SetSignalStyle(ic::TH1PlotElement & ele, unsigned color) {
    ele.set_fill_style(1001);
    ele.set_draw_fill(true);
    ele.set_draw_marker(false);
    ele.set_draw_line(false);
    ele.set_draw_stat_error_y(false);
    ele.set_in_stack(true);
    ele.set_line_style(11);
    ele.set_fill_color(0);
    ele.set_line_color(color);
    ele.set_line_width(2);
    return;
  }
  void HTTPlot::SetDataStyle(ic::TH1PlotElement & ele) {
    ele.set_marker_color(1);
    ele.set_line_color(1);
    ele.set_fill_color(1);
    ele.set_fill_style(0);
    ele.set_draw_fill(false);
    ele.set_line_width(2);
    ele.set_draw_marker(true);
    ele.set_draw_line(true);
    ele.set_marker_style(20);
    ele.set_draw_stat_error_y(true);
    ele.set_marker_size(1.1);
    return;
  }

  void HTTPlot::SetRatioStyle(ic::RatioPlotElement & ele, unsigned color) {
    ele.set_marker_color(color);
    ele.set_line_color(color);
    ele.set_draw_stat_error_y(true);
    ele.set_draw_line(false);
    ele.set_draw_marker(true);
    ele.set_marker_size(1.1);
    ele.set_marker_style(20);
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
      ((prefix+"extra_pad").c_str(),            po::value<double>(&extra_pad_)->default_value(1.0))
      ((prefix+"background_scheme").c_str(),    po::value<std::string>(&background_scheme_)->default_value(""))
      ((prefix+"signal_scheme").c_str(),        po::value<std::string>(&signal_scheme_)->default_value(""))
      ((prefix+"draw_signal_mass").c_str(),     po::value<std::string>(&draw_signal_mass_)->default_value(""))
      ((prefix+"draw_signal_tanb").c_str(),     po::value<std::string>(&draw_signal_tanb_)->default_value(""))
      ((prefix+"title_left").c_str(),           po::value<std::string>(&title_left_)->default_value(""))
      ((prefix+"title_right").c_str(),          po::value<std::string>(&title_right_)->default_value(""))
      ((prefix+"signal_scale").c_str(),         po::value<unsigned>(&signal_scale_)->default_value(1))
      ((prefix+"log_y").c_str(),                po::value<bool>(&log_y_)->default_value(false))
      ((prefix+"draw_ratio").c_str(),           po::value<bool>(&draw_ratio_)->default_value(false))
      ((prefix+"norm_bins").c_str(),            po::value<bool>(&norm_bins_)->default_value(false))
      ((prefix+"blind").c_str(),                po::value<bool>(&blind_)->default_value(false))
      ((prefix+"x_blind_min").c_str(),          po::value<double>(&x_blind_min_)->default_value(0))
      ((prefix+"x_blind_max").c_str(),          po::value<double>(&x_blind_max_)->default_value(0))
      ((prefix+"auto_error_band").c_str(),      po::value<double>(&auto_error_band_)->default_value(-1.0))
      ((prefix+"draw_error_band").c_str(),      po::value<bool>(&draw_error_band_)->default_value(false));

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
        (boost::lexical_cast<std::string>(signal_scale_)+"#times H("+draw_signal_mass_+")#rightarrow#tau#tau"),
        {"ggH","qqH","VH"}, kBlue+3, true)
    };
    sig_schemes_["mssm_default"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times #Phi#rightarrow#tau#tau(m_{A}="+draw_signal_mass_+",tan#beta="+draw_signal_tanb_+")"),
        {"ggH","bbH"}, kBlue+3, true)
    };
    sig_schemes_["mssm_nostack"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times #Phi#rightarrow#tau#tau(m_{A}="+draw_signal_mass_+",tan#beta="+draw_signal_tanb_+")"),
        {"ggH","bbH"}, kBlue+3, false)
    };
    sig_schemes_["sm_split_vbf"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times H("+draw_signal_mass_+")#rightarrow#tau#tau (ggH+VH)"),
        {"ggH","VH"}, kBlue+3, false),
      PlotSigComponent("vbf",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times H("+draw_signal_mass_+")#rightarrow#tau#tau (qqH)"),
        {"qqH"}, 1, false)
    };
    sig_schemes_["sm_split_ww"] = {
      PlotSigComponent("sig",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times H("+draw_signal_mass_+")#rightarrow#tau#tau"),
        {"ggH","VH","qqH"}, kBlue+3, true),
      PlotSigComponent("ww",
        (boost::lexical_cast<std::string>(signal_scale_)+"#times H("+draw_signal_mass_+")#rightarrow WW"),
        {"ggHWW","qqHWW"}, 8, true)
    };



    ic::Plot plot;
    if (use_htt_style_) plot.use_htt_style = true;

    plot.output_filename = plot_name_+".pdf";
    if (log_y_) {
      plot.output_filename = plot_name_+"_log.pdf";
      plot.y_axis_log = true;
    }
    plot.x_bin_labels_ = x_axis_bin_labels_;

    plot.extra_pad = extra_pad_;
    plot.custom_x_axis_range = custom_x_axis_range_;
    if (custom_x_axis_range_){
      plot.x_axis_min = x_axis_min_;
      plot.x_axis_max = x_axis_max_;
    }
    if (custom_y_axis_min_) {
      plot.y_axis_min = y_axis_min_;
    }
    // if (mssm_mode == 1) plot.legend_height = 0.045;
    plot.x_axis_title = x_axis_label_;
    plot.y_axis_title = y_axis_label_;

    plot.draw_ratio_hist = draw_ratio_;

    plot.title_left = title_left_;
    plot.title_right = title_right_;

    // should check if data actually exists: we might want to make a plot
    // where it doesn't
    TH1PlotElement data_plot("data", &hmap["data_obs"].first, "observed");
    if (norm_bins_) data_plot.hist_ptr()->Scale(1.0, "width");
    if (blind_) {
      TH1F *data_hist = &hmap["data_obs"].first;
      for (int j = 0; j < data_hist->GetNbinsX(); ++j) {
        double low_edge = data_hist->GetBinLowEdge(j+1);
        double high_edge = data_hist->GetBinWidth(j+1)+data_hist->GetBinLowEdge(j+1);
        if ((low_edge > x_blind_min_ && low_edge < x_blind_max_) || (high_edge > x_blind_min_ && high_edge < x_blind_max_)) {
          data_hist->SetBinContent(j+1,0);
          data_hist->SetBinError(j+1,0);
        }
      }
    }

    // should checkt the bakground scheme actually exists
    std::vector<PlotBkgComponent> bkg_scheme = bkg_schemes_[background_scheme_];
    std::vector<TH1PlotElement> bkg_elements;

    std::string bkr_list_for_ratio = "";


    for (unsigned i = 0; i < bkg_scheme.size(); ++i) {
      if (i == 0) bkr_list_for_ratio += bkg_scheme[i].name;
      if (i != 0) bkr_list_for_ratio += ("+"+bkg_scheme[i].name);
      bkg_elements.push_back(TH1PlotElement(bkg_scheme[i].name, &(hmap[bkg_scheme[i].plots[0]].first), bkg_scheme[i].legend));
      for (unsigned j = 1; j < bkg_scheme[i].plots.size(); ++j) {
        bkg_elements.back().hist_ptr()->Add(&hmap[bkg_scheme[i].plots[j]].first);
      }
      HTTPlot::SetMCStackStyle(bkg_elements.back(), bkg_scheme[i].color);
      if (norm_bins_) bkg_elements.back().hist_ptr()->Scale(1.0, "width");

      plot.AddTH1PlotElement(bkg_elements.back());
    }

    std::vector<PlotSigComponent> sig_scheme = sig_schemes_[signal_scheme_];
    std::vector<TH1PlotElement> sig_elements;
    for (unsigned i = 0; i < sig_scheme.size(); ++i) {
      sig_elements.push_back(TH1PlotElement(sig_scheme[i].name, &(hmap[sig_scheme[i].plots[0]+draw_signal_mass_].first), sig_scheme[i].legend));
      for (unsigned j = 1; j < sig_scheme[i].plots.size(); ++j) {
        sig_elements.back().hist_ptr()->Add(&hmap[sig_scheme[i].plots[j]+draw_signal_mass_].first);
      }
      sig_elements.back().hist_ptr()->Scale(signal_scale_);
      HTTPlot::SetSignalStyle(sig_elements.back(), sig_scheme[i].color);
      if (!sig_scheme[i].in_stack) {
        sig_elements.back().set_in_stack(false);
        sig_elements.back().set_line_width(3);
      }
      if (norm_bins_) sig_elements.back().hist_ptr()->Scale(1.0, "width");
      plot.AddTH1PlotElement(sig_elements.back());
    }
    TH1F error_band;
    TH1PlotElement err_element;
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
          error_band.SetBinError(i, error_band.GetBinContent(i)*auto_error_band_);
        }
      }
    err_element = TH1PlotElement("error_shape", &error_band,"bkg. uncertainty");
    err_element.set_marker_size(0);
    err_element.set_fill_color(1);
    err_element.set_fill_style(3013);
    err_element.set_line_width(1);
    err_element.set_draw_stat_error_y(true);
    err_element.set_draw_fill(true);
    err_element.set_draw_line(false);
    err_element.set_draw_marker(false);
    err_element.set_draw_options("e2");
    plot.AddTH1PlotElement(err_element);
    }

    ic::RatioPlotElement ratio("ratio","data",bkr_list_for_ratio);
    HTTPlot::SetRatioStyle(ratio,1);
    ratio.set_multi_mode(true);
    plot.ratio_y_axis_title = "Data/MC";
    plot.AddRatioPlotElement(ratio);
    plot.custom_ratio_y_axis_range = true;
    plot.ratio_y_axis_min = 0.5;
    plot.ratio_y_axis_max = 1.5;
    HTTPlot::SetDataStyle(data_plot);
    plot.AddTH1PlotElement(data_plot);
    plot.legend_height = 0.045;
    plot.GeneratePlot();



    // if (shift_backgrounds) {
    //   std::cout << "Shift ZTT: " << ztt_shift << std::endl;
    //   std::cout << "Shift QCD: " << qcd_shift << std::endl;
    //   std::cout << "Shift TOP: " << top_shift << std::endl;
    //   std::cout << "Shift ZL: "  << zl_shift << std::endl;
    //   std::cout << "Shift ZJ: "  << zj_shift << std::endl;
    //   std::cout << "Shift W: "   << w_shift << std::endl;
    //   std::cout << "Shift VV: "  << vv_shift << std::endl;
    //   top_shape.hist_ptr()->Scale(top_shift);
    //   qcd_shape.hist_ptr()->Scale(qcd_shift);
    //   w_shape.hist_ptr()  ->Scale(w_shift);
    //   vv_shape.hist_ptr() ->Scale(vv_shift);
    //   // zl_shape.hist_ptr()->Scale(zl_shift);
    //   // zj_shape.hist_ptr()->Scale(zj_shift);
    //   ztt_shape.hist_ptr()->Scale(ztt_shift);
    // }


    // if (channel == channel::mt || channel == channel::mtmet) w_shape.hist_ptr()->Add(zll_shape.hist_ptr());
    // w_shape.hist_ptr()->Add(vv_shape.hist_ptr());




    // ic::TH1PlotElement ztt_1p0pz_shape("ztt_1p0pz_shape", NULL, "Z#rightarrow#tau#tau (1 Prong 0 #pi^{0})");
    // ic::TH1PlotElement ztt_1p1pz_shape("ztt_1p1pz_shape", NULL, "Z#rightarrow#tau#tau (1 Prong 1 #pi^{0})");
    // ic::TH1PlotElement ztt_3p_shape("ztt_3p_shape", NULL, "Z#rightarrow#tau#tau (3 Prong)");



    // std::vector<ic::TH1PlotElement *> drawn_ele;
    // drawn_ele.push_back(&qcd_plot);
    // drawn_ele.push_back(&ewk_plot);
    // drawn_ele.push_back(&ztt_plot);
    // drawn_ele.push_back(&top_plot);

    // if (ztt_by_decay_mode) {
    //   ztt_1p0pz_shape.set_hist_ptr((TH1F*)(plots[Token("Embedded1P0PZ",cat,os_sel)].hist_ptr()->Clone()));
    //   ztt_1p1pz_shape.set_hist_ptr((TH1F*)(plots[Token("Embedded1P1PZ",cat,os_sel)].hist_ptr()->Clone()));
    //   ztt_3p_shape.set_hist_ptr((TH1F*)(plots[Token("Embedded3P",cat,os_sel)].hist_ptr()->Clone()));
    //   double ztt_split_tot =  Integral(ztt_1p0pz_shape.hist_ptr()) + Integral(ztt_1p1pz_shape.hist_ptr()) + Integral(ztt_3p_shape.hist_ptr());
    //   double ztt_1p0pz_frac = Integral(ztt_1p0pz_shape.hist_ptr()) / ztt_split_tot;
    //   double ztt_1p1pz_frac = Integral(ztt_1p1pz_shape.hist_ptr()) / ztt_split_tot;
    //   double ztt_3p_frac = Integral(ztt_3p_shape.hist_ptr()) / ztt_split_tot;
    //   ztt_1p0pz_shape.hist_ptr()->Scale( ztt_1p0pz_frac * Integral(ztt_shape.hist_ptr()) /  Integral(ztt_1p0pz_shape.hist_ptr()) );
    //   ztt_1p1pz_shape.hist_ptr()->Scale( ztt_1p1pz_frac * Integral(ztt_shape.hist_ptr()) /  Integral(ztt_1p1pz_shape.hist_ptr()) );
    //   ztt_3p_shape.hist_ptr()->Scale( ztt_3p_frac * Integral(ztt_shape.hist_ptr()) /  Integral(ztt_3p_shape.hist_ptr()) );
    //   SetStyle(ztt_1p0pz_shape, kOrange + 2);
    //   SetStyle(ztt_1p1pz_shape, kOrange - 0);
    //   SetStyle(ztt_3p_shape, 17);

    //   drawn_ele.push_back(&ztt_1p0pz_shape);
    //   drawn_ele.push_back(&ztt_1p1pz_shape);
    //   drawn_ele.push_back(&ztt_3p_shape);
    //   plot.legend_left = 0.54;
    // }


    // if (channel == channel::et) drawn_ele.push_back(&zll_shape);
    // if (draw_signal) drawn_ele.push_back(&signal_shape);
    // if (draw_signal && signal_split_vbf) drawn_ele.push_back(&signalvbf_shape);


    // drawn_ele.push_back(&data_plot);
    // for (unsigned i = 0; i < drawn_ele.size(); ++i) {
    //   //drawn_ele[i]->set_rebin_factor(rebin);
       // drawn_ele[i]->set_line_width(2);
       // if (norm_bins) drawn_ele[i]->hist_ptr()->Scale(1.0, "width");
    // }
    // if (signal_no_stack) {
    //   signalvbf_shape.set_line_width(3);
    //   signal_shape.set_line_width(3);
    // }


    // qcd_shape.set_legend_text("QCD");
    // if (channel == channel::em) qcd_shape.set_legend_text("fakes");
    // w_shape.set_legend_text("electroweak");
    // data_shape.set_legend_text("observed");
    // ztt_shape.set_legend_text("Z#rightarrow#tau#tau");
    // if (use_ztt_mc) ztt_shape.set_legend_text("Z#rightarrow#tau#tau (MC)");
    // zll_shape.set_legend_text("Z#rightarrowee");
    // top_shape.set_legend_text("t#bar{t}");
    // if (mssm_mode == 0) {
    //   signal_shape.set_legend_text(boost::lexical_cast<std::string>(draw_signal_factor)+"#times H("+draw_signal_mass+")#rightarrow#tau#tau");
    //   if (signal_split_vbf) signal_shape.set_legend_text(boost::lexical_cast<std::string>(draw_signal_factor)+"#times H("+draw_signal_mass+")#rightarrow#tau#tau (ggH+VH)");
    //   if (signal_split_vbf) signalvbf_shape.set_legend_text(boost::lexical_cast<std::string>(draw_signal_factor)+"#times H("+draw_signal_mass+")#rightarrow#tau#tau (qqH)");
    // } else {
    //   signal_shape.set_legend_text(boost::lexical_cast<std::string>(draw_mssm_signal_factor)+"#times #Phi#rightarrow#tau#tau(m_{A}="+draw_mssm_signal_mass+",tan#beta=8)");
    // }
    // if (channel == channel::et || channel == channel::mt || channel == channel::mtmet) {
    //   // top_shape.hist_ptr()->SetTitleSize  (0.055,"Y");
    //   // top_shape.hist_ptr()->SetTitleOffset(1.600,"Y");
    //   // top_shape.hist_ptr()->SetLabelOffset(0.014,"Y");
    //   // top_shape.hist_ptr()->SetLabelSize  (0.040,"Y");
    //   // top_shape.hist_ptr()->SetLabelFont  (42   ,"Y");
    //   // top_shape.hist_ptr()->SetTitleSize  (0.055,"X");
    //   // top_shape.hist_ptr()->SetTitleOffset(1.300,"X");
    //   // top_shape.hist_ptr()->SetLabelOffset(0.014,"X");
    //   // top_shape.hist_ptr()->SetLabelSize  (0.040,"X");
    //   // top_shape.hist_ptr()->SetLabelFont  (42   ,"X");
    //   // top_shape.hist_ptr()->SetMarkerStyle(20);
    //   // // top_shape.hist_ptr()->SetMarkerColor(color);
    //   // top_shape.hist_ptr()->SetMarkerSize (0.6);
    //   // top_shape.hist_ptr()->GetYaxis()->SetTitleFont(42);
    //   // top_shape.hist_ptr()->GetXaxis()->SetTitleFont(42);
    //   plot.AddTH1PlotElement(qcd_plot);
    //   plot.AddTH1PlotElement(top_plot);
    //   plot.AddTH1PlotElement(ewk_plot);
    // // } else if (channel == channel::em) {
    //   // plot.AddTH1PlotElement(qcd_shape);
    //   // plot.AddTH1PlotElement(w_shape);
    //   // plot.AddTH1PlotElement(top_shape);
    // // }
    // // if (channel == channel::et) plot.AddTH1PlotElement(zll_shape);
    // // if (!ztt_by_decay_mode) {
    //   plot.AddTH1PlotElement(ztt_plot);
    // } else {
    //   plot.AddTH1PlotElement(ztt_1p0pz_shape);    
    //   plot.AddTH1PlotElement(ztt_1p1pz_shape);    
    //   plot.AddTH1PlotElement(ztt_3p_shape);    
    // }


    // if (!draw_ss) {
    //   if (draw_signal) plot.AddTH1PlotElement(signal_shape);
    //   if (draw_signal && signal_split_vbf) plot.AddTH1PlotElement(signalvbf_shape);
    // }

    // if (norm_bins) plot.y_axis_title = "dN/dm_{#tau#tau} [1/GeV]";
    // plot.title_left = "CMS Preliminary " + year_label +", #sqrt{s} = " + (is_2012 ? "8":"7") +" TeV, "+ lumi_data_label;
    // if (official_style) plot.title_left = std::string("CMS Preliminary, #sqrt{s} = ") + (is_2012 ? "8":"7") + " TeV, L = "+ lumi_data_label;
    // string cat_app = show_category ? (", "+category) : "";
    // if (channel == channel::et) plot.title_right = "e#tau_{h}"+cat_app;
    // if (channel == channel::mt) plot.title_right = "#mu#tau_{h}"+cat_app;
    // if (channel == channel::mtmet) plot.title_right = "#mu#tau_{h}+MET"+cat_app;
    // if (channel == channel::em) plot.title_right = "e#mu"+cat_app;
    // if (mssm_mode == 1) plot.legend_left = 0.45;
    // if (official_style) plot.title_right = "";

    // plot.draw_ratio_hist = draw_ratio;
    // plot.draw_signif = false;

    // string background_list = "top_shape+ztt_shape+w_shape+qcd_shape";
    // if (ztt_by_decay_mode) background_list = "top_shape+ztt_3p_shape+ztt_1p1pz_shape+ztt_1p0pz_shape+w_shape+qcd_shape";
    // if (channel == channel::et) background_list += "+zll_shape";
    // ic::RatioPlotElement ratio("Mug","data_shape",background_list);
    
    // plot.band_size_fractional_ = band_size_fractional;
    // plot.draw_band_on_stack_ = draw_band_on_stack;
    // plot.samples_for_band_ = "top_shape+ztt_shape+w_shape+qcd_shape";
    // if (ztt_by_decay_mode) plot.samples_for_band_ = "top_shape+ztt_3p_shape+ztt_1p1pz_shape+ztt_1p0pz_shape+w_shape+qcd_shape";
    // if (channel == channel::et) plot.samples_for_band_ += "+zll_shape";

    // SetStyle(ratio,1);
    // ratio.set_multi_mode(true);
    // //plot.ratio_y_axis_title = "#frac{(N_{obs}-N_{exp})}{#sqrt{N_{obs}}}";
    // plot.ratio_y_axis_title = "Data/MC";
    // plot.AddRatioPlotElement(ratio);
    // plot.custom_ratio_y_axis_range = true;
    // plot.ratio_y_axis_min = 0.5;
    // plot.ratio_y_axis_max = 1.5;
    
    // if (y_axis_min > 0) plot.y_axis_min = y_axis_min;
    
    // // // plot.y_axis_min = 5.0;
    // // if (blind) {
    // //   agilbert::TextElement text1("Data in ["+boost::lexical_cast<std::string>(x_blind_min)+","+boost::lexical_cast<std::string>(x_blind_max)+"] blinded",0.03, 0.16,0.89);
    // //   plot.AddTextElement(text1);
    // // }
    // // plot.y_axis_min = 5.0;
    // if (official_style) {
    //   string ch_label;
    //   if (channel == channel::et) ch_label = "e#tau_{h}"+cat_app;
    //   if (channel == channel::mt) ch_label = "#mu#tau_{h}"+cat_app;
    //   if (channel == channel::mtmet) ch_label = "#mu#tau_{h}+MET"+cat_app;
    //   if (channel == channel::em) ch_label = "e#mu"+cat_app;
    //   ic::TextElement text1(ch_label,0.055, 0.21,0.84);
    //   plot.AddTextElement(text1);
    // }
    // ic::TextElement ss_text("Same-sign",0.0,0.19,0.89);
    // if (draw_ss) plot.AddTextElement(ss_text);

    // if (add_text.size() > 0) {
    //   std::vector<string> tmp_vec;
    //   boost::split(tmp_vec, add_text, boost::is_any_of(","));
    //   if (tmp_vec.size() == 4) {
    //     ic::TextElement tmp_text(tmp_vec[0], boost::lexical_cast<double>(tmp_vec[1]), boost::lexical_cast<double>(tmp_vec[2]) ,boost::lexical_cast<double>(tmp_vec[3]));
    //     plot.AddTextElement(tmp_text);
    //   }
    // }


  }



}

