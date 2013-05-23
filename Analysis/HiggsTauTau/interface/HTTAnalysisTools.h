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


  class HTTAnalysis {

    public:
      HTTAnalysis(ic::channel ch, std::string year, int verbosity);
      void ParseParamFile(std::string const& file);
      void ReadTrees(std::string const& folder, std::string const& prefix = "");
      double GetLumiScale(std::string const& sample);

      // GetLumiScaledShape("jpt_1(100,0,100)","DYJetsToTauTauSoup","os && mt_1 < 20.", "njets>=1","wt");
      TH1F GetShape(std::string const& variable,
                              std::string const& sample, 
                              std::string const& selection, 
                              std::string const& category, 
                              std::string const& weight);
      std::pair<double, double> GetRate(std::string const& sample, 
                              std::string const& selection, 
                              std::string const& category, 
                              std::string const& weight);



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

  };
 
}

