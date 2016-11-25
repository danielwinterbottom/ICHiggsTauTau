#ifndef HiggsTauTau_TheoryWeights_h
#define HiggsTauTau_TheoryWeights_h

#include <string>
#include <limits>
#include "boost/lexical_cast.hpp"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "Objects/interface/EventInfo.hh"
#include "TH1F.h"
#include "TH2F.h"

namespace ic {

/**
 * Simple struct to record a distribution under many PDF variations
 *
 * Given an existing TH1F creates a TH2F with the same x-binning, and as many
 * y bins as PDF variations. A range of integer PDF indices is also given, and
 * this is used to look-up the relevant event weights from the EventInfo
 * object. At the end of event processing the `PostProcess` method can be
 * called to calculate the standard deviation in each histogram. A new 1D
 * histogram is created (`uncert`) with nominal bin content and the std.
 * deviation as the error.
 */
struct TheorySystHist {
  TH2F *grid;   // store multiple versions of a TH1F in a TH2F
  TH1F *uncert;  // will hold the final uncertainty calculation
  TH1F *src;
  TH2F *corr;
  unsigned start_pdf;
  unsigned end_pdf;
  std::string set_name;
  std::vector<std::string> pdf_strs;

  TheorySystHist() {;}

  TheorySystHist(TH1F *h_src, TFileDirectory *dir, unsigned start, unsigned end,
             std::string name)
      : src(h_src), start_pdf(start), end_pdf(end), set_name(name) {
    unsigned n_pdfs = (end - start) + 1;
    for (unsigned i = start_pdf; i <= end_pdf; ++i) {
      pdf_strs.push_back(boost::lexical_cast<std::string>(i));
    }
    TString base_name = TString(src->GetName()) + "_" + TString(set_name);
    int n = src->GetNbinsX();
    double min = src->GetXaxis()->GetXmin();
    double max = src->GetXaxis()->GetXmax();
    grid   = dir->make<TH2F>(base_name + "_grid", "", n, min, max, n_pdfs, -0.5,
                           double(n_pdfs) - 0.5);
    uncert = dir->make<TH1F>(base_name + "_uncert", "", n, min, max);
    corr   = dir->make<TH2F>(base_name + "_corr", "", n, min, max, n, min, max);
  }

  void Fill(double val, double wt, EventInfo const* info) {
    int bin = grid->GetXaxis()->FindBin(val);
    for (unsigned i = 0; i < pdf_strs.size(); ++i) {
      double new_wt = wt * info->weight(pdf_strs[i]);
      grid->SetBinContent(bin, i+1, grid->GetBinContent(bin, i+1) + new_wt);
    }
  }

  void PostProcessNormal() {
    unsigned npdfs = pdf_strs.size();
    for (int bin = 1; bin <= src->GetNbinsX(); ++bin) {
      double sum2 = 0.;
      for (unsigned pdf = 0; pdf < npdfs; ++pdf) {
        double diff = grid->GetBinContent(bin, pdf+1) - src->GetBinContent(bin);
        sum2 += (diff * diff);
      }
      double err = sum2 > 0. ? std::sqrt(sum2 / double(npdfs - 1)) : 0.;
      uncert->SetBinContent(bin, src->GetBinContent(bin));
      uncert->SetBinError(bin, err);
    }
    for (int i = 1; i <= src->GetNbinsX(); ++i) {
      for (int j = i; j <= src->GetNbinsX(); ++j) {
        double sum = 0.;
        for (unsigned pdf = 0; pdf < npdfs; ++pdf) {
          double diff_i = grid->GetBinContent(i, pdf+1) - src->GetBinContent(i);
          double diff_j = grid->GetBinContent(j, pdf+1) - src->GetBinContent(j);
          sum += (diff_i * diff_j);
        }
        double ei = uncert->GetBinError(i);
        double ej = uncert->GetBinError(j);
        double corrv = sum / (double(npdfs - 1) * ei * ej);
        corr->SetBinContent(i, j, corrv);
        corr->SetBinContent(j, i, corrv);
      }
    }
  }

  void PostProcessEnvelope() {
    for (int bin = 1; bin <= src->GetNbinsX(); ++bin) {
      double min = std::numeric_limits<double>::max();
      double max = std::numeric_limits<double>::lowest();
      for (unsigned pdf = 0; pdf < pdf_strs.size(); ++pdf) {
        double val = grid->GetBinContent(bin, pdf+1);
        if (val > max) max = val;
        if (val < min) min = val;
      }
      double err = (max - min) / 2.;
      uncert->SetBinContent(bin, src->GetBinContent(bin));
      uncert->SetBinError(bin, err);
    }
  }
};

class TheoryWeights : public ModuleBase {
 private:
  TH1F *h_m_ll;
  TheorySystHist hpdf_m_ll;
  TheorySystHist hscale_m_ll;
  TH1F *h_pt_ll;
  TheorySystHist hpdf_pt_ll;
  TheorySystHist hscale_pt_ll;
  TH1F *h_n_jets;
  TheorySystHist hpdf_n_jets;
  TheorySystHist hscale_n_jets;
  TH1F *h_pt_1;
  TH1F *h_pt_2;
  TH1F *h_eta_1;
  TH1F *h_eta_2;
  CLASS_MEMBER(TheoryWeights, fwlite::TFileService*, fs)

 public:
  TheoryWeights(std::string const& name);
  virtual ~TheoryWeights();
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};
}

#endif
