#ifndef ICHiggsTauTau_HiggsTauTau_ZbbAnalysisTools_h
#define ICHiggsTauTau_HiggsTauTau_ZbbAnalysisTools_h
#include <vector>
#include <map>
#include "TH1.h"
#include "Utilities/interface/SimpleParamParser.h"
#include "Utilities/interface/FnRootTools.h"


namespace ic {

  double Integral(TH1F const* hist);
  double IntegrateFloatRange(TH1F const* hist, double xmin, double xmax);
  double Error(TH1F const* hist);
  void SetNorm(TH1F * hist, double norm, bool verbose = false);

  class ZbbAnalysis {
    public:
      typedef std::pair<double, double> Value;
      typedef std::pair<TH1F, Value> HistValuePair;
      typedef std::map<std::string, HistValuePair> HistValueMap;
      typedef std::map<std::string, std::function<std::pair<double,double>()>> ValueFnMap;
    
    public:
      ZbbAnalysis(int verbosity);
      void AddSample(std::string const& sample);
      void ParseParamFile(std::string const& file);
      void ReadTrees(std::string const& folder, std::string const& fallback_folder = "");

      double GetLumiScale(std::string const& sample);
      double GetLumiScaleFixedXS(std::string const& sample, double xs);
      void FillHistoMap(HistValueMap & hmap,
                        std::string var,
                        std::string sel,
                        std::string cat,
                        std::string wt,
                        std::string postfix);

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
      static Value ValueAdd(Value const& p1, Value const& p2);
      static Value ValueSubtract(Value const& p1, Value const& p2);
      static void PrintValue(std::string const& label, Value const& val);

      std::string ResolveAlias(std::string const& al);
      std::vector<std::string> ResolveSamplesAlias(std::string const& al);
      void SetAlias(std::string const& al, std::string const& val);
      inline bool AliasDefined(std::string const& al) { return alias_map_.count(al); }
      inline double GetLumi() const { return lumi_; };
      inline void SetVerbosity(unsigned const& verbosity) { verbosity_ = verbosity; }

    private:
      std::string input_folder_;
      unsigned verbosity_;
      double lumi_;
      std::vector<std::string> sample_names_;
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

