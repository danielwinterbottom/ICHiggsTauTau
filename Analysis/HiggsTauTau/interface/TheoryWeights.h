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
struct PDFSetHist {
  TH2F *pdf_grid;
  TH1F *uncert;
  TH1F *src;
  unsigned start_pdf;
  unsigned end_pdf;
  std::string set_name;
  std::vector<std::string> pdf_strs;

  PDFSetHist() {;}

  PDFSetHist(TH1F *h_src, TFileDirectory *dir, unsigned start, unsigned end,
             std::string name)
      : src(h_src), start_pdf(start), end_pdf(end), set_name(name) {
    unsigned n_pdfs = (end - start) + 1;
    for (unsigned i = start_pdf; i <= end_pdf; ++i) {
      pdf_strs.push_back(boost::lexical_cast<std::string>(i));
    }
    pdf_grid = dir->make<TH2F>(
        TString(src->GetName()) + "_pdf_grid_" + TString(set_name), "",
        src->GetNbinsX(), src->GetXaxis()->GetXmin(), src->GetXaxis()->GetXmax(), n_pdfs, -0.5,
        double(n_pdfs) - 0.5);
    uncert = dir->make<TH1F>(
        TString(src->GetName()) + "_pdf_" + TString(set_name), "", src->GetNbinsX(),
        src->GetXaxis()->GetXmin(), src->GetXaxis()->GetXmax());
  }

  void Fill(double val, double wt, EventInfo const* info) {
    int bin = pdf_grid->GetXaxis()->FindBin(val);
    for (unsigned i = 0; i < pdf_strs.size(); ++i) {
      double new_wt = wt * info->weight(pdf_strs[i]);
      pdf_grid->SetBinContent(bin, i+1, pdf_grid->GetBinContent(bin, i+1) + new_wt);
    }
  }

  void PostProcess() {
    for (int bin = 1; bin <= src->GetNbinsX(); ++bin) {
      double sum2 = 0.;
      for (unsigned pdf = 0; pdf < pdf_strs.size(); ++pdf) {
        sum2 += std::pow(pdf_grid->GetBinContent(bin, pdf+1) - src->GetBinContent(bin), 2.);
      }
      double err = sum2 > 0. ? std::sqrt(sum2 / double(pdf_strs.size() - 1)) : 0.;
      uncert->SetBinContent(bin, src->GetBinContent(bin));
      uncert->SetBinError(bin, err);
    }
  }

  void PostProcessEnvelope() {
    for (int bin = 1; bin <= src->GetNbinsX(); ++bin) {
      double min = std::numeric_limits<double>::max();
      double max = std::numeric_limits<double>::lowest();
      for (unsigned pdf = 0; pdf < pdf_strs.size(); ++pdf) {
        double val = pdf_grid->GetBinContent(bin, pdf+1);
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
  PDFSetHist hpdf_m_ll;
  PDFSetHist hscale_m_ll;
  TH1F *h_pt_ll;
  PDFSetHist hpdf_pt_ll;
  PDFSetHist hscale_pt_ll;
  TH1F *h_n_jets;
  PDFSetHist hpdf_n_jets;
  PDFSetHist hscale_n_jets;
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
