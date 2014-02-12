#ifndef ICHiggsTauTau_CombineTools_CombineHarvester_h
#define ICHiggsTauTau_CombineTools_CombineHarvester_h
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <map>
#include <list>
#include <cmath>
#include <functional>
#include "TFile.h"
#include "TH1.h"
#include "CombineTools/interface/Process.h"
#include "CombineTools/interface/Nuisance.h"
#include "CombineTools/interface/Parameter.h"
#include "CombineTools/interface/Observation.h"
#include "CombineTools/interface/HelperFunctions.h"

/*
h.GenerateSet<string>([](ch::Parameter p){ regex_match(p, ".*_bin_.*") });
// - some people don't know/like lambdas/binding
// + Gives complete flexibility 

// A shape nuisance is split to an asymmetric lnN for the yield, and Vertica
// morphing for the unit area shapes. 

Shape algorithm:
(Build map process -> shape nuisances)
(Build map process -> all nuisances)
(Build param name -> value map)

* loop through processes
  * use (proc->shape nuis, pname->value) map to apply each shape morphing
  * use (proc->all nuis) map to apply each norm morph
  * this process histogram is then 'done', can be added to the total
  * to be returned

With "simple" error request
(build map param name -> all nuisances)
* loop through parameters
  * evaluate param up and param down    
*/

namespace ch {

class CombineHarvester {
 public:
  CombineHarvester();
  ~CombineHarvester();
  CombineHarvester(CombineHarvester const& other);
  CombineHarvester(CombineHarvester&& other);
  CombineHarvester& operator=(CombineHarvester other);

  CombineHarvester shallow_copy();

  int ParseDatacard(std::string const& filename,
      std::string const& analysis,
      std::string const& era,
      std::string const& channel,
      int bin_id,
      std::string const& mass);

  CombineHarvester& PrintAll();

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

  void Validate();

    // int ParsePulls(std::string const& filename);
    // void ApplyPulls(bool use_b_only = false);
    // void WeightSoverB();
    // inline void AddProcess(Process proc) { processes_.push_back(proc); }
    // void VariableRebin(std::vector<double> bins);
  double GetRate();
    // double GetObservedRate();
  double GetUncertainty();
  TH1F GetShape();
  TH1F GetObservedShape();
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
  friend void swap(CombineHarvester& first, CombineHarvester& second);
  typedef std::vector<std::vector<Nuisance const*>> 
    ProcNusMap;
  ProcNusMap GenerateProcNusMap();

  double GetRateInternal(ProcNusMap const& lookup,
    std::string const& single_nus = "");

  inline double smoothStepFunc(double x) const { 
    if (std::fabs(x) >= 1.0/*_smoothRegion*/) return x > 0 ? +1 : -1;
    double xnorm = x/1.0;/*_smoothRegion*/
    double xnorm2 = xnorm*xnorm;
    return 0.125 * xnorm * (xnorm2 * (3.*xnorm2 - 10.) + 15);
  }

  TH1F ShapeDiff(double x, TH1 const* nom, TH1 const* low, TH1 const* high);

};
}

#endif