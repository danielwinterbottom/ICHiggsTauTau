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
#include "RooProduct.h"

namespace ch {

void BuildRooMorphing(RooWorkspace& ws, CombineHarvester& cb, RooAbsReal& mh,
                      bool verbose, std::string norm_postfix) {
  using std::set;
  using std::string;
  using std::vector;
  using boost::lexical_cast;

  set<string> mass_set =
      cb.GenerateSetFromProcs<string>(std::mem_fn(&Process::mass));

  vector<double> mass_vec;
  vector<string> mass_str_vec;
  for (auto const& m : mass_set) {
    mass_str_vec.push_back(m);
    // mass_vec.push_back(lexical_cast<double>(m));
  }
  std::sort(mass_str_vec.begin(), mass_str_vec.end(),
    [](string const& s1, string const& s2) {
      return lexical_cast<double>(s1) < lexical_cast<double>(s2);
    });
  for (auto const& s : mass_str_vec) {
    if (verbose) std::cout << ">> Mass point: " << s << "\n";
    mass_vec.push_back(lexical_cast<double>(s));
  }
  unsigned n = mass_vec.size();

  double mass_min = mass_vec.front();
  double mass_max = mass_vec.back();

  if (verbose) {
    std::cout << ">> Found " << mass_vec.size() << " mass points with min "
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
      if (verbose) std::cout << ">> bin: " << b << " process: " << s << "\n";
      ch::CombineHarvester tmp2 = std::move(tmp.cp().process({s}));
      TH1F data_hist = tmp2.GetObservedShape();
      // tmp2.PrintAll();
      RooRealVar mtt("CMS_th1x", "CMS_th1x", 0,
                     static_cast<float>(data_hist.GetNbinsX()));
      mtt.setBins(data_hist.GetNbinsX());
      RooRealVar morph_mtt(("CMS_th1x_"+b).c_str(), "", 0,
                     static_cast<float>(tmp2.GetShape().GetNbinsX()));
      morph_mtt.setConstant();
      morph_mtt.setBins(tmp2.GetShape().GetNbinsX());
      if (verbose) {
        mtt.Print();
        morph_mtt.Print();
      }
      vector<string> systs = ch::Set2Vec(
          tmp2.GenerateSetFromNus<string>(std::mem_fn(&Nuisance::name)));
      if (verbose) std::cout << ">> Found shape systematics:\n";
      RooArgList syst_list;
      for (auto const& syst: systs) {
        if (verbose) std::cout << ">>>> " << syst << "\n";
        syst_list.addClone(RooRealVar(syst.c_str(), syst.c_str(), 0));
      }
      vector<vector<TH1F>> h_vec(n);
      vector<TList> list_vec(n);
      vector<double> yield_vec(n);
      vector<FastVerticalInterpHistPdf2> v_pdfs;

      vector<vector<double>> k_vals_hi(systs.size(), vector<double>(n));
      vector<vector<double>> k_vals_lo(systs.size(), vector<double>(n));
      vector<RooSpline1D> k_splines_hi;
      vector<RooSpline1D> k_splines_lo;
      vector<AsymPow> k_asym;
      RooArgList k_list;

      for (unsigned m = 0; m < n; ++m) {
        ch::CombineHarvester tmp3 =
            std::move(tmp2.cp().mass({mass_str_vec[m]}));
        yield_vec[m] = tmp3.GetRate();
        h_vec[m].push_back(RebinHist(tmp3.GetShape()));
        for (unsigned s = 0; s < systs.size(); ++s) {
          tmp3.cp().nus_name({systs[s]}).ForEachNus([&](Nuisance const* n) {
            h_vec[m].push_back(RebinHist(*(TH1F*)(n->shape_u())));
            h_vec[m].push_back(RebinHist(*(TH1F*)(n->shape_d())));
            k_vals_hi[s][m] = n->value_u();
            k_vals_lo[s][m] = n->value_d();
          });
        }
      }

      std::string key = b + "_" + s;
      RooSpline1D yield((key + "_yield").c_str(), "", mh,
                        n, &(mass_vec[0]), &(yield_vec[0]), "LINEAR");
      k_list.add(yield);
      for (unsigned s = 0; s < systs.size(); ++s) {
        k_splines_hi.push_back(RooSpline1D((key + "_" + systs[s] + "_hi").c_str(), "", mh, n,
                        &(mass_vec[0]), &(k_vals_hi[s][0]), "LINEAR"));
        k_splines_lo.push_back(RooSpline1D((key + "_" + systs[s] + "_lo").c_str(), "", mh, n,
                        &(mass_vec[0]), &(k_vals_lo[s][0]), "LINEAR"));
      }
      for (unsigned s = 0; s < systs.size(); ++s) {
        k_asym.push_back(
            AsymPow((key + "_" + systs[s] + "_lnN").c_str(), "",
                    k_splines_lo[s], k_splines_hi[s],
                    *(reinterpret_cast<RooRealVar*>(syst_list.at(s)))));
      }
      for (unsigned s = 0; s < systs.size(); ++s) k_list.add(k_asym[s]);

      for (unsigned m = 0; m < n; ++m) {
        for (unsigned h = 0; h < h_vec[m].size(); ++h) {
          list_vec[m].Add(&h_vec[m][h]);
        }
      }

      for (unsigned m = 0; m < n; ++m) {
        v_pdfs.push_back(FastVerticalInterpHistPdf2(
            (b + "_" + s + mass_str_vec[m] + "_vpdf").c_str(), "", morph_mtt,
            list_vec[m], syst_list, 1, 0));
        // v_pdfs.back().Print();
      }
      RooArgList v_pdf_list;
      for (unsigned m = 0; m < n; ++m) v_pdf_list.add(v_pdfs[m]);

      RooMorphingPdf morph((b + "_" + s + "_mpdf").c_str(), "", mtt, mh,
                           v_pdf_list, mass_vec, true, *(data_hist.GetXaxis()),
                           *(tmp2.GetShape().GetXaxis()));

      RooProduct full_yield((b + "_" + s + "_mpdf" + norm_postfix).c_str(), "",
                            k_list);

      ws.import(morph, RooFit::RecycleConflictNodes());
      ws.import(full_yield, RooFit::RecycleConflictNodes());
    }
  }
  // ws.Print();
}
}
