#ifndef ICHiggsTauTau_CombineTools_CombineHarvester_h
#define ICHiggsTauTau_CombineTools_CombineHarvester_h
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <map>
#include <list>
#include <cmath>
#include <set>
#include <functional>
#include "TFile.h"
#include "TH1.h"
#include "CombineTools/interface/Process.h"
#include "CombineTools/interface/Nuisance.h"
#include "CombineTools/interface/Parameter.h"
#include "CombineTools/interface/Observation.h"
#include "CombineTools/interface/HelperFunctions.h"

namespace ch {

class CombineHarvester {
 public:
  // Low-level
  CombineHarvester();
  ~CombineHarvester();
  CombineHarvester(CombineHarvester const& other);
  CombineHarvester(CombineHarvester&& other);
  CombineHarvester& operator=(CombineHarvester other);
  CombineHarvester shallow_copy();
  CombineHarvester& PrintAll();

  void SetParameters(std::vector<ch::Parameter> params);
  void UpdateParameters(std::vector<ch::Parameter> params);
  std::vector<ch::Parameter> GetParameters() const;

  // Datacard interaction
  int ParseDatacard(std::string const& filename,
      std::string const& analysis,
      std::string const& era,
      std::string const& channel,
      int bin_id,
      std::string const& mass);

  void WriteDatacard(std::string const& name, std::string const& root_file);

  // Filtering
  CombineHarvester& bin(bool cond, std::vector<std::string> const& vec);
  CombineHarvester& bin_id(bool cond, std::vector<int> const& vec);
  CombineHarvester& process(bool cond, std::vector<std::string> const& vec);
  CombineHarvester& process_id(bool cond, std::vector<int> const& vec);
  CombineHarvester& analysis(bool cond, std::vector<std::string> const& vec);
  CombineHarvester& era(bool cond, std::vector<std::string> const& vec);
  CombineHarvester& channel(bool cond, std::vector<std::string> const& vec);
  CombineHarvester& mass(bool cond, std::vector<std::string> const& vec);
  CombineHarvester& nus_name(bool cond, std::vector<std::string> const& vec);
  CombineHarvester& nus_type(bool cond, std::vector<std::string> const& vec);
  CombineHarvester& signals();
  CombineHarvester& backgrounds();

  // Set generation
  template<typename T>
  std::set<T> GenerateSetFromProcs(std::function<T(ch::Process const*)> func);

  template<typename T>
  std::set<T> GenerateSetFromObs(std::function<T(ch::Observation const*)> func);

  template<typename T>
  std::set<T> GenerateSetFromNus(std::function<T(ch::Nuisance const*)> func);

  // Yields & Shapes
  double GetRate();
  double GetObservedRate();
  double GetUncertainty();
  double GetUncertainty(RooFitResult const* fit, unsigned n_samples);
  TH1F GetShape();
  TH1F GetShapeWithUncertainty();
  TH1F GetShapeWithUncertainty(RooFitResult const* fit, unsigned n_samples);
  TH1F GetObservedShape();

  // void Validate();

    // int ParsePulls(std::string const& filename);
    // void ApplyPulls(bool use_b_only = false);
    // void WeightSoverB();
    // inline void AddProcess(Process proc) { processes_.push_back(proc); }
    // void VariableRebin(std::vector<double> bins);

    // double GetObservedRate();
    // TGraphAsymmErrors GetObservedShapeErrors();
    // std::set<std::string> GetNuisanceSet();
    // bool HasProcess(std::string const& process) const;
    // void ScaleProcessByEra(std::string const& process, std::string const& era, double scale);
 private:
  std::vector<std::shared_ptr<Observation>> obs_;
  std::vector<std::shared_ptr<Process>> procs_;
  std::vector<std::shared_ptr<Nuisance>> nus_;
  std::map<std::string, std::shared_ptr<Parameter>> params_;

  struct HistMapping {
    std::string process;
    std::string category;
    std::unique_ptr<TFile> file;
    std::string pattern;
    std::string syst_pattern;
  };

  typedef std::vector<std::pair<std::string, std::string>> StrPairVec;
  typedef std::vector<std::string> StrVec;
  StrPairVec GenerateShapeMapAttempts(std::string process,
      std::string category);
  TH1 * GetHistFromFile(std::vector<HistMapping> const& mappings,
      std::string const& bin,
      std::string const& process,
      std::string const& mass,
      std::string const& nuisance,
      unsigned type);

  void WriteHistToFile(
      TH1 const* hist,
      TFile * file,
      std::vector<HistMapping> const& mappings,
      std::string const& bin,
      std::string const& process,
      std::string const& mass,
      std::string const& nuisance,
      unsigned type);

  friend void swap(CombineHarvester& first, CombineHarvester& second);
  typedef std::vector<std::vector<Nuisance const*>> 
    ProcNusMap;
  ProcNusMap GenerateProcNusMap();

  double GetRateInternal(ProcNusMap const& lookup,
    std::string const& single_nus = "");

  TH1F GetShapeInternal(ProcNusMap const& lookup,
    std::string const& single_nus = "");

  inline double smoothStepFunc(double x) const { 
    if (std::fabs(x) >= 1.0/*_smoothRegion*/) return x > 0 ? +1 : -1;
    double xnorm = x/1.0;/*_smoothRegion*/
    double xnorm2 = xnorm*xnorm;
    return 0.125 * xnorm * (xnorm2 * (3.*xnorm2 - 10.) + 15);
  }

  TH1F ShapeDiff(double x, TH1 const* nom, TH1 const* low, TH1 const* high);

};

template<typename T> 
std::set<T> CombineHarvester::GenerateSetFromProcs(std::function<T (ch::Process const*)> func) {
  std::set<T> ret;
  for (auto const& item : procs_) ret.insert(func(item.get()));
  return ret;
} 

template<typename T> 
std::set<T> CombineHarvester::GenerateSetFromObs(std::function<T (ch::Observation const*)> func) {
  std::set<T> ret;
  for (auto const& item : obs_) ret.insert(func(item.get()));
  return ret;
} 

template<typename T> 
std::set<T> CombineHarvester::GenerateSetFromNus(std::function<T (ch::Nuisance const*)> func) {
  std::set<T> ret;
  for (auto const& item : nus_) ret.insert(func(item.get()));
  return ret;
} 


}

#endif