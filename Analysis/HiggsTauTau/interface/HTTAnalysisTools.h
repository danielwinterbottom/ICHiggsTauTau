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

// namespace po = boost::program_options;

// using namespace std;
// using namespace ic;



/*
  GetHistogram(string sample, string category, string selection, string weights)
  GetQCDHistogram
  GetWNorm(string w_sample "WJetsToLNu", string data_sample="Data", 
  vec_backgrounds "Diboson, ZTT, Top", string category, string selection,  ) {
    ... work out Diboson is just an alias for ZZJetsTo... etc
    ... iterate on this, working out that I only need to integrate on the selection and scale by xs factor
    ... work out that ZTT is just an alias for Embedded
    ... work out that we need to call GetEmbeddedNorm first

  GetEmbeddedNorm(string "Embedded", string "DYJetsToTauTauSoup", string norm_selection, norm_category, string selection, string category )
    e.g. norm_selection = "os", norm_category = "inclusive", selection="os && mt_1 < 20.", category="vbf"

  

  }

*/

namespace ic {

  double Integral(TH1F const* hist) {
    return hist->Integral(0, hist->GetNbinsX() + 1);
  }

  double Error(TH1F const* hist) {
    double err = 0.0;
    hist->IntegralAndError(0, hist->GetNbinsX() + 1, err);
    return err;
  }

  void BlindHistogram(TH1F * hist, double x_min, double x_max) {
    for (int j = 0; j < hist->GetNbinsX(); ++j) {
      double low_edge = hist->GetBinLowEdge(j+1);
      double high_edge = hist->GetBinWidth(j+1)+hist->GetBinLowEdge(j+1);
      if ((low_edge > x_min && low_edge < x_max) || (high_edge > x_min && high_edge < x_max)) {
        hist->SetBinContent(j+1,0);
        hist->SetBinError(j+1,0);
      }
    }
  }

  void SetNorm(TH1F * hist, double norm) {
    if (Integral(hist) == 0.0) {
      std::cout << "Warning, attempt to scale histogram with integral of 0.0" << std::endl;
      return;
    } else {
      hist->Scale(norm / Integral(hist));
    }
  }


  class HTTAnalysis {
    public:
      typedef std::pair<double, double> Value;

    public:
      HTTAnalysis(ic::channel ch, std::string year, int verbosity);
      void ParseParamFile(std::string const& file);
      void ReadTrees(std::string const& folder, std::string const& prefix = "");
      double GetLumiScale(std::string const& sample);

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
                              std::map<std::string, std::function<Value()>> dict
                              );

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

      static Value ValueProduct(Value const& p1, Value const& p2);
      static Value ValueDivide(Value const& p1, Value const& p2);

      Value GetRateViaWMethod(std::string const& w_sample,
                              std::string const& ratio_category,
                              std::string const& ratio_control_selection,
                              std::string const& ratio_signal_selection,
                              std::string const& data_sample,
                              std::string const& category,
                              std::string const& control_selection,
                              std::vector<std::string> const& sub_samples,
                              std::string const& weight,
                              std::map<std::string, std::function<Value()>> dict
                              );
      Value GetRateViaQCDMethod(Value const& ratio,
                              std::string const& data_sample,
                              std::string const& control_selection,
                              std::string const& category,
                              std::vector<std::string> const& sub_samples,
                              std::string const& weight,
                              std::map<std::string, std::function<Value()>> dict
                              );



    private:
      ic::channel ch_;
      std::string year_;
      std::string input_folder_;
      unsigned verbosity_;
      double lumi_;
      std::vector<std::string> sample_names_;
      std::vector<std::string> signal_masses_;
      std::map<std::string, std::pair<double, double>> sample_info_;
      std::map<std::string, TFile *> tfiles_;
      std::map<std::string, TTree *> ttrees_;

      std::string BuildCutString(std::string const& selection,
                                 std::string const& category,
                                 std::string const& weight);
      std::string BuildVarString(std::string const& variable);

  };
 
}

