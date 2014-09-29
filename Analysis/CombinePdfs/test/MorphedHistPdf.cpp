#include <string>
#include <vector>
// #include "boost/filesystem.hpp"
// #include "boost/regex.hpp"
// #include "boost/format.hpp"
#include "boost/bind.hpp"
#include "boost/lexical_cast.hpp"
// #include "boost/assign/list_of.hpp"
// #include "CombineTools/interface/CombineHarvester.h"
#include "CombineTools/interface/HelperFunctions.h"
#include "RooFitResult.h"
#include "RooHistPdf.h"
#include "RooDataHist.h"
#include "RooRealVar.h"
#include "RooRealProxy.h"
#include "RooWorkspace.h"
#include "RooHistFunc.h"
#include "RooArgList.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooMorphingPdf.h"
#include "HiggsAnalysis/CombinedLimit/interface/VerticalInterpHistPdf.h"


using boost::bind;
using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using boost::lexical_cast;
using ch::TH1F2Data;

int main() {
  /*

    Need way to automate this procedure:

    RooMorphingBuilder:
      Shape & Norm due to shape uncertainties

      BuildRooMorphing(RooWorkspace, data_obs for target, any signal for morphing)
      Need to be able to specify name of mH, x can be set automatically?



  */

  RooWorkspace ws("htt_sig", "htt_sig");

  TH1::AddDirectory(false);
  RooRealVar mh("MH", "MH", 122.5, 90, 145);

  std::string file = "data/demo/htt_mt.inputs-sm-8TeV-hcg.root";
  {
  // vector<string> masspoints = {};
  vector<string> masspoints = {"120", "125", "130", "135"};
  vector<RooDataHist> nominal_h;
  vector<vector<RooDataHist>> var_h;
  vector<RooHistPdf> nominal_p;
  vector<vector<RooHistPdf>> var_p;
  vector<RooArgList> v_arglist;
  vector<FastVerticalInterpHistPdf> v_pdfs;

  TH1F data_obs = ch::OpenFromTFile<TH1F>(file + ":muTau_0jet_high/data_obs");
  unsigned n_bins = data_obs.GetNbinsX();
  TH1F data_obs_fb = ch::OpenFromTFile<TH1F>(file + ":muTau_0jet_high/data_obs_fine_binning");

  RooRealVar mtt("CMS_th1x", "CMS_th1x", 0, static_cast<float>(data_obs_fb.GetNbinsX()));
  mtt.setBins(data_obs_fb.GetNbinsX());


  RooRealVar syst("CMS_scale_t_mutau_8TeV", "CMS_scale_t_mutau_8TeV", 0);
  RooArgList v_coeffs(syst);
  for (auto mass : masspoints) {
    nominal_h.push_back(
        TH1F2Data(ch::OpenFromTFile<TH1F>(file + ":muTau_0jet_high/qqH" + mass + "_fine_binning"),
                  mtt, "hist_qqH" + mass));
    var_h.push_back(vector<RooDataHist>());
    var_h.back().push_back(
        TH1F2Data(ch::OpenFromTFile<TH1F>(file + ":muTau_0jet_high/qqH" + mass +
                                          "_CMS_scale_t_mutau_8TeVUp_fine_binning"),
                  mtt, "hist_qqH" + mass + "Up"));
    var_h.back().push_back(
        TH1F2Data(ch::OpenFromTFile<TH1F>(file + ":muTau_0jet_high/qqH" + mass +
                                          "_CMS_scale_t_mutau_8TeVDown_fine_binning"),
                  mtt, "hist_qqH" + mass + "Down"));
  }

  for (unsigned i = 0; i < masspoints.size(); ++i) {
    nominal_p.push_back(
        RooHistPdf(("pdf_qqH" + masspoints[i]).c_str(), "", mtt, nominal_h[i]));
    var_p.push_back(vector<RooHistPdf>());
    for (unsigned j = 0; j < var_h[i].size(); ++j) {
      var_p.back().push_back(
          RooHistPdf(var_h[i][j].GetName(), "", mtt, var_h[i][j]));
    }
  }

  for (unsigned i = 0; i < masspoints.size(); ++i) {
    v_arglist.push_back(RooArgList(nominal_p[i]));
    for (unsigned j = 0; j < var_p[i].size(); ++j) {
      v_arglist.back().add(var_p[i][j]);
    }
  }

  for (unsigned i = 0; i < masspoints.size(); ++i) {
    v_pdfs.push_back(
        FastVerticalInterpHistPdf(("vpdf_" + masspoints[i]).c_str(), "", mtt,
                                  v_arglist[i], v_coeffs, 1, 0));
  }

  RooMorphingPdf morph("morph_pdf", "", mtt, mh, true, *(data_obs.GetXaxis()),
                       *(data_obs_fb.GetXaxis()));
  for (unsigned i = 0; i < masspoints.size(); ++i) {
    morph.AddPoint(lexical_cast<double>(masspoints[i]), v_pdfs[i]);
  }

  syst.setVal(0.0);
  for (unsigned i = 0; i < masspoints.size(); ++i) {
    syst.setVal(0.0);
    double d_mass = lexical_cast<double>(masspoints[i]);
    mh.setVal(d_mass);
    double x = 0.5;
    std::cout << "Mass: " << mh.getVal() << "\n";
    for (unsigned j = 0; j < n_bins; ++j) {
      mtt.setVal(x);
      std::cout << x << "\t" << morph.evaluate() << "\n";
      // std::cout << x << "\t" << v_pdfs[0].evaluate() << "\n";
      x += 1.0;
    }
  }

  for (unsigned i = 0; i < v_pdfs.size(); ++i) {
    // ws.import(v_pdfs[i]);
  }
  ws.import(morph);
  }
  ws.Print();

  TFile f("test.root", "RECREATE");
  ch::WriteToTFile(&ws, &f, "htt_sig");
  f.Close();
}
