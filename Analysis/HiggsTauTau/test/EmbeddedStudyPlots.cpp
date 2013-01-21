#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SimpleParamParser.h"

namespace po = boost::program_options;

using namespace std;
using namespace ic;

double Integral(TH1F const* hist) {
  return hist->Integral(0, hist->GetNbinsX() + 1);
}

double Error(TH1F const* hist) {
  double err = 0.0;
  hist->IntegralAndError(0, hist->GetNbinsX() + 1, err);
  return err;
}

void SetStyle(ic::TH1PlotElement & ele, unsigned color) {
  ele.set_marker_color(color);
  ele.set_line_color(color);
  ele.set_draw_line(true);
  ele.set_line_width(1);
  ele.set_draw_fill(true);
  ele.set_draw_stat_error_y(true);
  return;
}


void SetDataStyle(ic::TH1PlotElement & ele) {
  ele.set_marker_color(1);
  ele.set_line_color(1);
  ele.set_fill_color(1);
  ele.set_fill_style(0);
  ele.set_draw_fill(false);
  ele.set_line_width(3);
  ele.set_draw_marker(true);
  ele.set_draw_line(true);
  ele.set_marker_style(20);
  ele.set_draw_stat_error_y(true);
  ele.set_marker_size(1.3);

  //ele.set_draw_normalised(true);

  //ele.set_in_stack(true);
  return;
}

void SetStyle(ic::RatioPlotElement & ele, unsigned color) {
  ele.set_marker_color(color);
  ele.set_line_color(color);
  ele.set_draw_stat_error_y(true);
  ele.set_draw_line(false);
  ele.set_draw_marker(true);
  return;
}


int main(int argc, char* argv[]){

  std::cout << argc << " arguments passed to program: " << std::endl;
  for (int i = 0; i < argc; ++i) {
    std::cout << "-- " << argv[i] << std::endl;
  }

  TFile f("embedded_study.root");

  ic::Plot unfolded_vs_ref;
  unfolded_vs_ref.output_filename = "unfolded_vs_ref.pdf";

  ic::TH1PlotElement unfolded_vs_ref__ref("unfolded_vs_ref__ref", &f, "kinematics/","mass_unfold_gen_test", "Reference Dist.");
  ic::TH1PlotElement unfolded_vs_ref__unfolded("unfolded_vs_ref__unfolded", &f, "kinematics/","mass_unfold_gen_result", "Unfolded Dist.");
  ic::TH1PlotElement unfolded_vs_ref__reco("unfolded_vs_ref__reco", &f, "kinematics/","mass_unfold_reco_test", "Reco. Dist.");

  ic::RatioPlotElement unfolded_vs_ref__ratio("Ratio","unfolded_vs_ref__unfolded","unfolded_vs_ref__ref");

  SetStyle(unfolded_vs_ref__ref, 1);
  SetStyle(unfolded_vs_ref__unfolded, 2);
  SetStyle(unfolded_vs_ref__reco, 9);

  unfolded_vs_ref.AddTH1PlotElement(unfolded_vs_ref__ref);
  unfolded_vs_ref.AddTH1PlotElement(unfolded_vs_ref__unfolded);
  unfolded_vs_ref.AddTH1PlotElement(unfolded_vs_ref__reco);

  unfolded_vs_ref.x_axis_title = "Di-Muon Mass [GeV]";

  unfolded_vs_ref.draw_ratio_hist = true;
  unfolded_vs_ref.AddRatioPlotElement(unfolded_vs_ref__ratio);
  unfolded_vs_ref.ratio_y_axis_title = "Unfolded/Ref";

  unfolded_vs_ref.GeneratePlot();


  TFile f_embed("output/HCP_2012/Embedded_MuTau_2012.root");
  TFile f_dyjet("output/HCP_2012/DYJetsToTauTau_MuTau_2012.root");
  TFile f_signal("output/HCP_2012/GluGluToHToTauTau_M-125_MuTau_2012.root");

  ic::Plot mutau_embed;
  mutau_embed.output_filename = "mutau_inclusive_embedded_vs_dyjets.pdf";

  ic::TH1PlotElement mutau_embed__embed("mutau_embed__embed", &f_embed, "onejet_highpt_os_sel/","vis_sm_fine", "Embedded");
  ic::TH1PlotElement mutau_embed__dyjet("mutau_embed__dyjet", &f_dyjet, "onejet_highpt_os_sel/","vis_sm_fine", "MC");
  ic::TH1PlotElement mutau_embed__signal("mutau_embed__signal", &f_signal, "onejet_highpt_os_sel/","vis_sm_fine", "ggH125 (5x SM)");

  ic::RatioPlotElement mutau_embed__ratio("Ratio","mutau_embed__embed","mutau_embed__dyjet");

  SetStyle(mutau_embed__embed, 1);
  SetStyle(mutau_embed__dyjet, 2);
  SetStyle(mutau_embed__signal, 38);

  mutau_embed__embed.set_draw_normalised(true);
  mutau_embed__dyjet.set_draw_normalised(true);
  mutau_embed__signal.set_scale_factor(5*0.013/mutau_embed__signal.hist_ptr()->Integral());

  mutau_embed.AddTH1PlotElement(mutau_embed__embed);
  mutau_embed.AddTH1PlotElement(mutau_embed__dyjet);
  mutau_embed.AddTH1PlotElement(mutau_embed__signal);

  mutau_embed.x_axis_title = "Visible Mass [GeV]";
  mutau_embed.y_axis_title = "Arb. Units";

  mutau_embed.draw_ratio_hist = true;
  mutau_embed.AddRatioPlotElement(mutau_embed__ratio);
  mutau_embed.ratio_y_axis_title = "Embedded/MC";

  mutau_embed.GeneratePlot();


  return 0;
}

