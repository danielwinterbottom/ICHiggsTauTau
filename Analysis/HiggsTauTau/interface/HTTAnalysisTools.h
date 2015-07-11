#ifndef ICHiggsTauTau_HiggsTauTau_HTTAnalysisTools_h
#define ICHiggsTauTau_HiggsTauTau_HTTAnalysisTools_h
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
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "RooWorkspace.h"

//! HTTAnalysisTools
/*!
  A set of classes and functions that can be used to analyse the output of
  the HiggsTauTau program. This includes creating histograms from flat
  TTrees and running the data-driven background techniques.
*/

namespace ic {

  inline double Integral(TH1F const* hist) {
    return hist->Integral(0, hist->GetNbinsX() + 1);
  }

  inline double IntegrateFloatRange(TH1F const* hist, double xmin, double xmax) {
    TAxis const* axis = hist->GetXaxis();
    int bmin = axis->FindBin(xmin);
    int bmax = axis->FindBin(xmax);
    double integral = hist->Integral(bmin, bmax);
    integral -= hist->GetBinContent(bmin)*(xmin-axis->GetBinLowEdge(bmin))/
              axis->GetBinWidth(bmin);
    integral -= hist->GetBinContent(bmax)*(axis->GetBinUpEdge(bmax)-xmax)/
              axis->GetBinWidth(bmax);
    return integral;
  }


  inline double Error(TH1F const* hist) {
    double err = 0.0;
    hist->IntegralAndError(0, hist->GetNbinsX() + 1, err);
    return err;
  }

  inline void BlindHistogram(TH1F * hist, double x_min, double x_max) {
    for (int j = 0; j < hist->GetNbinsX(); ++j) {
      double low_edge = hist->GetBinLowEdge(j+1);
      double high_edge = hist->GetBinWidth(j+1)+hist->GetBinLowEdge(j+1);
      if ((low_edge > x_min && low_edge < x_max) || (high_edge > x_min && high_edge < x_max)) {
        hist->SetBinContent(j+1,0);
        hist->SetBinError(j+1,0);
      }
    }
  }

  inline void SetNorm(TH1F * hist, double norm, bool verbose = false) {
    if (Integral(hist) == 0.0) {
      if (verbose) std::cout << "Warning, attempt to scale histogram with integral of 0.0" << std::endl;
      return;
    } else {
      hist->Scale(norm / Integral(hist));
    }
  }

  inline void FixEmptyBins(TH1F * hist, bool verbose) {
    unsigned bins = hist->GetNbinsX();
    unsigned first_populated = 0;
    unsigned last_populated = 0;
    for (unsigned i = 1; i <= bins; ++i) {
      if (hist->GetBinContent(i) > 0. && first_populated == 0) first_populated = i;
      if (hist->GetBinContent(bins-(i-1)) > 0. && last_populated == 0) last_populated = bins-(i-1);
    }
    if (last_populated <= first_populated) {
      if (verbose) std::cout << "Error: Cannot correct this distribution!" << std::endl;
      return;
    }
    if (verbose) std::cout << "First populated bin: " << first_populated << std::endl;
    if (verbose) std::cout << "Last populated bin: " << last_populated << std::endl;
    double av_weight = ( hist->Integral() / double(hist->GetEntries()));
    for (unsigned i = first_populated+1; i < last_populated; ++i) {
      if (hist->GetBinContent(i) == 0.) {
        if (verbose) std::cout << "Bin " << i << " is empty!" << std::endl;
        if (verbose) std::cout << "Set weight to 1.0 * av_weight = " << (1.0 * av_weight) << std::endl;
        hist->SetBinError(i, av_weight);    
      }
    }
  }

  inline void FixNegativeBins(TH1F * hist, bool verbose) {
    for (int j = 0; j < hist->GetNbinsX(); ++j) {
      if (hist->GetBinContent(j+1) < 0) {
        if (verbose) std::cout << "[FixNegativeBins] Bin " << j << " is negative, setting to small positive value" << std::endl;
        hist->SetBinContent(j+1,0.0000001);
      }
    }
  }

  inline void FixEmptyHist(TH1F * hist, bool verbose) {
    unsigned centre_bin = hist->GetNbinsX() / 2;
    if (hist->Integral() == 0.0) {
      if (verbose) std::cout << "[FixEmptyHist] Histogram is empty, inserting small value in central bin" << std::endl;
      hist->SetBinContent(centre_bin, 0.00001);
    }
  }

  inline void InflateErrors(TH1F * hist, double factor) {
    for (int j = 1; j <= hist->GetNbinsX(); ++j) {
      if (hist->GetBinContent(j) > 0) {
        double extra = factor * hist->GetBinContent(j);
        double err = hist->GetBinError(j);
        double new_err = std::sqrt(err*err + extra*extra);
        hist->SetBinError(j, new_err);
      }
    }
  }

  class HTTAnalysis {
    public:
      typedef std::pair<double, double> Value;
      typedef std::pair<TH1F, Value> HistValuePair;
      typedef std::map<std::string, HistValuePair> HistValueMap;
      typedef std::map<std::string, std::function<std::pair<double,double>()>> ValueFnMap;
    
    public:
      //! Constructor specifying channel, year and the level of verbosity
      HTTAnalysis(ic::channel ch, std::string year, int verbosity);

      //! Add a single sample to the list of files that will be opened
      void AddSample(std::string const& sample);

      //! Read a parameter file for cross section and luminosity information
      /*! Argument \p file is the full path to the parameter file. 
      */
      void ParseParamFile(std::string const& file);

      //! For each value in \p masses adds the three SM signal samples to the list of samples to load
      /*! Example usage:

              analysis.AddSMSignalSamples({"105","110","115","120"}});

          Here we use a C++11 initializer list to declare the vector in-place.
          NB. This method does no validation on the input strings, it simply adds the sample names
          to the internal list. It should therefore be called before #ReadTrees.
      */
      void AddSMSignalSamples(std::vector<std::string> masses);
      void AddSMHbbSignalSamples(std::vector<std::string> masses);
      
      void AddHWWSignalSamples(std::vector<std::string> masses);
      
      //! For each value in \p masses adds the two MSSM signal samples to the list of samples to load
      /*! \sa AddSMSignalSamples
      */
      void AddMSSMSignalSamples(std::vector<std::string> masses);
      void AddHhhSignalSamples(std::vector<std::string> masses, bool extra_signal_profile=false);
      void AddMSSMbbHSignalSamples(std::vector<std::string> masses);
      void AddHighMassSignalSamples(std::vector<std::string> masses);

      //! Read a parameter file for cross section and luminosity information
      /*! Argument \p file is the full path to the parameter file. 
      */
      void ReadTrees(std::string const& folder, std::string const& fallback_folder = "");

      double GetLumiScale(std::string const& sample);
      double GetLumiScaleFixedXS(std::string const& sample, double xs);
      void FillHistoMap(HistValueMap & hmap, unsigned method,
                        std::string var,
                        std::string sel,
                        std::string cat,
                        std::string wt,
                        std::string postfix);
      HistValuePair GenerateData(unsigned method, std::string var, std::string sel, std::string cat, std::string wt);
      HistValuePair GenerateZTT(unsigned method, std::string var, std::string sel, std::string cat, std::string wt);
      HistValuePair GenerateZL(unsigned method, std::string var, std::string sel, std::string cat, std::string wt);
      HistValuePair GenerateZJ(unsigned method, std::string var, std::string sel, std::string cat, std::string wt);
      HistValuePair GenerateTOP(unsigned method, std::string var, std::string sel, std::string cat, std::string wt);
      HistValuePair GenerateVV(unsigned method, std::string var, std::string sel, std::string cat, std::string wt);
      HistValuePair GenerateW(unsigned method, std::string var, std::string sel, std::string cat, std::string wt);
      HistValuePair GenerateQCD(unsigned method, std::string var, std::string sel, std::string cat, std::string wt);
      HistValuePair GenerateSignal(std::string sample, std::string var, std::string sel, std::string cat, std::string wt, double xs = -1.0);
      
      void FillSMSignal(HistValueMap & hmap, 
                        std::vector<std::string> const& masses,
                        std::string const& var,
                        std::string const& sel,
                        std::string const& cat,
                        std::string const& wt,
                        std::string const& infix,
                        std::string const& postfix,
                        double fixed_xs = -1.0);
      void FillSMHbbSignal(HistValueMap & hmap, 
                        std::vector<std::string> const& masses,
                        std::string const& var,
                        std::string const& sel,
                        std::string const& cat,
                        std::string const& wt,
                        std::string const& infix,
                        std::string const& postfix,
                        double fixed_xs = -1.0);
      void InterpolateSMSignal(HistValueMap & hmap, 
                        std::vector<std::string> const& masses,
                        std::string const& var,
                        std::string const& var_final,
                        std::string const& sel,
                        std::string const& cat,
                        std::string const& wt,
                        std::string const& infix,
                        std::string const& postfix,
                        double interpolate,
                        double fixed_xs = -1.0);
      void FillHWWSignal(HistValueMap & hmap, 
                        std::vector<std::string> const& masses,
                        std::string const& var,
                        std::string const& sel,
                        std::string const& cat,
                        std::string const& wt,
                        std::string const& infix,
                        std::string const& postfix,
                        double fixed_xs = -1.0);
      void FillMSSMSignal(HistValueMap & hmap, 
                        std::vector<std::string> const& masses,
                        std::string const& var,
                        std::string const& sel,
                        std::string const& cat,
                        std::string const& wt,
                        std::string const& infix,
                        std::string const& postfix,
                        double fixed_xs = -1.0);
      void FillHhhSignal(HistValueMap & hmap, 
                        std::vector<std::string> const& masses,
                        std::string const& var,
                        std::string const& sel,
                        std::string const& cat,
                        std::string const& wt,
                        std::string const& infix,
                        std::string const& postfix,
                        double fixed_xs = -1.0,
                        bool extra_signal_profile = false);
      void FillMSSMbbHSignal(HistValueMap & hmap, 
                        std::vector<std::string> const& masses,
                        std::string const& var,
                        std::string const& sel,
                        std::string const& cat,
                        std::string const& wt,
                        std::string const& infix,
                        std::string const& postfix,
                        double fixed_xs = -1.0);
      void FillHighMassSignal(HistValueMap & hmap, 
                        std::vector<std::string> const& masses,
                        std::string const& var,
                        std::string const& sel,
                        std::string const& cat,
                        std::string const& wt,
                        std::string const& infix,
                        std::string const& postfix,
                        double fixed_xs = -1.0);

      //! Generate a histogram for a specific samples
      /*! \param variable A string containing the name of a histogram branch
      */
      TH1F GetShape(std::string const& variable,
                              std::string const& sample, 
                              std::string const& selection, 
                              std::string const& category, 
                              std::string const& weight);
      TH1F GetLumiScaledShape(std::string const& variable,
                              std::string const& sample, 
                              std::string const& selection, 
                              std::string const& category, 
                              std::string const& weight);
      TH1F GetLumiScaledShape(std::string const& variable,
                              std::vector<std::string> const& samples, 
                              std::string const& selection, 
                              std::string const& category, 
                              std::string const& weight);

      TH1F GetShapeViaQCDMethod(std::string const& variable,
                              std::string const& data_sample,
                              std::string const& selection,
                              std::string const& category,
                              std::vector<std::string> const& sub_samples,
                              std::string const& weight,
                              std::map<std::string, std::function<Value()>> dict);
      TH1F GetShapeViaFakesMethod(std::string const& var,
                                  std::string const& sel,
                                  std::string const& cat,
                                  std::string const& wt);
      Value GetRate(std::string const& sample, 
                    std::string const& selection, 
                    std::string const& category, 
                    std::string const& weight);
      Value GetLumiScaledRate(std::string const& sample, 
                              std::string const& selection,
                              std::string const& category,
                              std::string const& weight);
      Value GetLumiScaledRate(std::vector<std::string> const& samples, 
                              std::string const& selection,
                              std::string const& category,
                              std::string const& weight);
      // Sample efficiency is only correctly defined if the
      // "target" selection and category gives a strict subset
      // of the events in the "reference" selection and category
      Value SampleEfficiency(std::string const& sample, 
                              std::string const& ref_selection, 
                              std::string const& ref_category,
                              std::string const& target_selection, 
                              std::string const& target_category,  
                              std::string const& weight);
      // Sample ratio is only correctly defined if the events defined
      // by "target" and "ref" and mutually exclusive (and therefore 
      // can be treated as statistically independent populations)
      Value SampleRatio(std::string const& sample, 
                              std::string const& ref_selection, 
                              std::string const& ref_category,
                              std::string const& target_selection, 
                              std::string const& target_category,  
                              std::string const& weight);

      Value GetRateViaRefEfficiency(std::string const& target_sample, 
                              std::string const& ref_sample,
                              std::string const& ref_selection, 
                              std::string const& ref_category,
                              std::string const& target_selection, 
                              std::string const& target_category,  
                              std::string const& weight);
      Value GetRateViaFakesMethod(std::string const& sel,
                                  std::string const& cat,
                                  std::string const& wt);

      static Value ValueProduct(Value const& p1, Value const& p2);
      static Value ValueDivide(Value const& p1, Value const& p2);
      static Value ValueAdd(Value const& p1, Value const& p2);
      static Value ValueSubtract(Value const& p1, Value const& p2);
      static void PrintValue(std::string const& label, Value const& val);

      //! Get event rate using the W-sideband data-driven method
      /*! The behaviour of this function is as follows:
          1. Use sample \p w_sample to calculate the number of events in
             selection \p ratio_signal_sel over the number in selection
             \p ratio_control_sel for a given category \p ratio_cat. This
            gives the low/high \f$m_{T}\f$ ratio. 
          2. The selection \p control_sel and category \p cat are then used
             to calculate the rate in \p Data and subtract the contribution
             from the list of samples in \p sub_samples.  For each background
             that is subtracted the function #GetLumiScaledRate is used to 
             evaluate the rate, unless an entry in \p dict is provided in 
             which case the corresponding function will be evaluated instead.
          3. The method returns the background-subtracted data yield (2)  
             multiplied by the extrapolation factor (1) 
      */
      Value GetRateViaWMethod(std::string const& w_sample,
                              std::string const& ratio_cat,
                              std::string const& ratio_control_sel,
                              std::string const& ratio_signal_sel,
                              std::string const& data_sample,
                              std::string const& cat,
                              std::string const& control_sel,
                              std::vector<std::string> const& sub_samples,
                              std::string const& wt,
                              std::map<std::string, std::function<Value()>> dict
                              );
      Value GetRateViaWFitMethod(std::string const& w_sample,
                              std::string const& ratio_cat,
                              std::string const& ratio_control_sel,
                              std::string const& ratio_signal_sel,
                              std::string const& data_sample,
                              std::string const& cat,
                              std::string const& control_sel,
                              std::vector<std::string> const& sub_samples,
                              std::string const& wt,
                              std::map<std::string, std::function<Value()>> dict,
                              std::string const& fit_var
                              );
      Value GetRateViaQCDMethod(Value const& ratio,
                              std::string const& data_sample,
                              std::string const& control_selection,
                              std::string const& category,
                              std::vector<std::string> const& sub_samples,
                              std::string const& weight,
                              std::map<std::string, std::function<Value()>> dict
                              );
      std::string ResolveAlias(std::string const& al);
      std::vector<std::string> ResolveSamplesAlias(std::string const& al);
      void SetAlias(std::string const& al, std::string const& val);
      inline bool AliasDefined(std::string const& al) { return alias_map_.count(al); }
      inline double GetLumi() const { return lumi_; };
      double KolmogorovTest(std::string const& variable,
                              std::string const& sample1, std::string const& selection1, std::string const& category1,
                              std::string const& sample2, std::string const& selection2, std::string const& category2,
                              std::string const& weight);
      Value WTTTemplateFit(TH1F* data, TH1F* W, TH1F* TT, double mt_min, int mode);

      void SetQCDRatio(double const& ratio);
      inline void SetVerbosity(unsigned const& verbosity) { verbosity_ = verbosity; }

    private:
      ic::channel ch_;
      std::string year_;
      std::string input_folder_;
      unsigned verbosity_;
      double lumi_;
      std::string dy_soup_;
      double qcd_os_ss_factor_;
      std::vector<std::string> sample_names_;
      std::vector<std::string> signal_masses_;
      std::map<std::string, std::pair<double, double>> sample_info_;
      std::map<std::string, TFile *> tfiles_;
      std::map<std::string, TTree *> ttrees_;
      std::map<std::string, std::string> alias_map_;
      std::map<std::string, std::vector<std::string>> samples_alias_map_;

      std::string BuildCutString(std::string const& selection,
                                 std::string const& category,
                                 std::string const& weight);
      std::string BuildVarString(std::string const& variable);

  };
 
}

#endif

