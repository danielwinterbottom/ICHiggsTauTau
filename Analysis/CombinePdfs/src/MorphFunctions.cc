#include "CombinePdfs/interface/MorphFunctions.h"
#include "Utilities/interface/FnRootTools.h"
#include <iostream>
#include <set>
#include <vector>
#include <string>
#include "boost/lexical_cast.hpp"
#include "RooFitResult.h"
#include "RooRealVar.h"
#include "RooDataHist.h"

namespace ch {

void BuildRooMorphing(RooWorkspace & ws, CombineHarvester & cb,
                      std::string const& mass_var, bool verbose) {
  using std::set;
  using std::string;
  using std::vector;
  using boost::lexical_cast;

  set<string> s_masses =
      cb.GenerateSetFromProcs<string>(std::mem_fn(&Process::mass));

  set<double> d_masses;
  for (auto const& m : s_masses) d_masses.insert(lexical_cast<double>(m));

  double mass_min = *(d_masses.begin());
  double mass_max = *(--d_masses.end());

  RooRealVar mh(mass_var.c_str(), mass_var.c_str(), (mass_max + mass_min) / 2.,
                mass_min, mass_max);

  if (verbose) {
    std::cout << ">> Found " << d_masses.size() << " mass points with min "
              << mass_min << " and max " << mass_max << "\n";
    mh.Print();
  }

  set<string> bins =
      cb.GenerateSetFromProcs<string>(std::mem_fn(&Process::bin));

  for (auto const& b : bins) {
    ch::CombineHarvester tmp =
        std::move(cb.cp().bin({b}).nus_type({"shape"}));
    set<string> sigs =
      tmp.GenerateSetFromProcs<string>(std::mem_fn(&Process::process));
    for (auto s : sigs) {
      std::cout << ">> bin: " << b << " process: " << s << "\n";
      ch::CombineHarvester tmp2 = std::move(tmp.cp().process({s}));
      TH1F data_hist = tmp2.GetObservedShape();
      tmp2.PrintAll();
      RooRealVar mtt("CMS_th1x", "CMS_th1x", 0,
                     static_cast<float>(data_hist.GetNbinsX()));
      mtt.setBins(data_hist.GetNbinsX());
      RooRealVar morph_mtt(("CMS_th1x_"+b).c_str(), "", 0,
                     static_cast<float>(tmp2.GetShape().GetNbinsX()));
      morph_mtt.setConstant();
      morph_mtt.setBins(tmp2.GetShape().GetNbinsX());
      // mtt.Print();
      // morph_mtt.Print();
      vector<string> systs = ch::Set2Vec(
          tmp2.GenerateSetFromNus<string>(std::mem_fn(&Nuisance::name)));
      std::cout << ">> Found shape systematics:\n";
      RooArgList syst_list;
      for (auto const& syst: systs) {
        if (verbose) std::cout << ">>>> " << syst << "\n";
        syst_list.addClone(RooRealVar(syst.c_str(), syst.c_str(), 0));
      }
      auto v_masses = Set2Vec(s_masses);
      vector<vector<RooDataHist>> dh_vec(v_masses.size());
      vector<double> yield_vec(v_masses.size());
      vector<vector<RooHistPdf>> pdf_vec(v_masses.size());
      vector<RooArgList> v_arglist(v_masses.size());
      vector<FastVerticalInterpHistPdf> v_pdfs;

      for (unsigned m = 0; m < v_masses.size(); ++m) {
        ch::CombineHarvester tmp3 = std::move(tmp2.cp().mass({v_masses[m]}));
        TH1F shape = tmp3.GetShape();
        yield_vec[m] = tmp3.GetRate();
        string name = b + "_" + s + "_" + v_masses[m];
        dh_vec[m].push_back(ch::TH1F2Data(tmp3.GetShape(), morph_mtt,
                                              name + "_hist"));
        for (unsigned s = 0; s < systs.size(); ++s) {
          tmp3.cp().nus_name({systs[s]}).ForEachNus([&](Nuisance const* n) {
            dh_vec[m]
                .push_back(ch::TH1F2Data(*(TH1F*)(n->shape_u()), morph_mtt,
                                         name + "_" + systs[s] + "Up_hist"));
            dh_vec[m]
                .push_back(ch::TH1F2Data(*(TH1F*)(n->shape_d()), morph_mtt,
                                         name + "_" + systs[s] + "Down_hist"));
          });
        }
      }
      for (unsigned m = 0; m < v_masses.size(); ++m) {
        string name = b + "_" + s + "_" + v_masses[m];
        pdf_vec[m].push_back(
            RooHistPdf((name + "_pdf").c_str(), "", morph_mtt, dh_vec[m][0]));
        for (unsigned s = 0; s < systs.size(); ++s) {
          pdf_vec[m].push_back(RooHistPdf((name + "_" + systs[s] + "Up_pdf").c_str(),
                                          "", morph_mtt, dh_vec[m][2 * s + 1]));
          pdf_vec[m].push_back(RooHistPdf((name + "_" + systs[s] + "Down_pdf").c_str(),
                                          "", morph_mtt, dh_vec[m][2 * s + 2]));
        }
      }

      for (unsigned m = 0; m < pdf_vec.size(); ++m) {
        for (unsigned s = 0; s < pdf_vec[m].size(); ++s) {
          v_arglist[m].add(pdf_vec[m][s]);
        }
      }

      for (unsigned m = 0; m < v_arglist.size(); ++m) {
        v_pdfs.push_back(FastVerticalInterpHistPdf(
            (b + "_" + s + v_masses[m] + "_vpdf").c_str(), "", morph_mtt,
            v_arglist[m], syst_list, 1, 0));
      }

      RooMorphingPdf morph((b + "_" + s + "_mpdf").c_str(), "", mtt, mh, true, *(data_hist.GetXaxis()),
                           *(tmp2.GetShape().GetXaxis()));
      RooHttYield yield((b + "_" + s + "_mpdf_norm").c_str(), "", mh);
      for (unsigned m = 0; m < v_masses.size(); ++m) {
        morph.AddPoint(lexical_cast<double>(v_masses[m]), v_pdfs[m]);
        yield.AddPoint(lexical_cast<double>(v_masses[m]), yield_vec[m]);
      }

      ws.import(morph, RooFit::Silence());
      ws.import(yield, RooFit::Silence());
    }
  }
  // ws.Print();
}



}
