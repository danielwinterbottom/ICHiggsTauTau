#ifndef ICHiggsTauTau_CombineTools_CombineHarvester_h
#define ICHiggsTauTau_CombineTools_CombineHarvester_h
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "TFile.h"
#include "TH1.h"
namespace ch { class Observation; }
namespace ch { class Process; }
namespace ch { class Nuisance; }

/*

A good interface is still:

CombineHarvester h;
... load datacards ...

h.process{"ZTT","W"}.era("7TeV").GetRate();
Well defined copy semantics using defaults
classes hold shared_ptrs to TH1 


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

  

From a generic datacard only get:

Observation:
  bin
  rate
  {mass=125, era=7TeV, channel=et, bin_id=7}

Process:
  bin
  process_id
  process
  rate

Nuisance
  bin
  process_id
  process
  name
  type
  value_u
  value_d

Parameter
  name
  value
  err_u
  err_d

When parsing datacards 


*/

namespace ch {

class CombineHarvester {
 public:
  CombineHarvester();
  ~CombineHarvester();
  CombineHarvester(CombineHarvester const& other);
  CombineHarvester(CombineHarvester&& other);
  CombineHarvester& operator=(CombineHarvester other);

  int ParseDatacard(std::string const& filename,
      std::string const& analysis,
      std::string const& era,
      std::string const& channel,
      int bin_id,
      std::string const& mass);

  CombineHarvester & PrintAll();
    // int ParseDatacard(std::string const& filename,  
    //   std::string mass, 
    //   std::string era,
    //   std::string const& channel, 
    //   int category_id);
    // int ParseROOTFile(std::string const& filename, std::string const& channel, std::string era);
    // int ParseDatacard(std::string const& filename);
    // int ParseDatacard(std::string const& filename, std::string mass);
    // int ParseROOTFile(std::string const& filename);
    // int ParsePulls(std::string const& filename);
    // void ApplyPulls(bool use_b_only = false);
    // void WeightSoverB();
    // inline void AddProcess(Process proc) { processes_.push_back(proc); }
    // void VariableRebin(std::vector<double> bins);
    // CombineHarvester process(std::vector<std::string> const& process) const;
    // CombineHarvester era(std::vector<std::string> const& era) const;
    // CombineHarvester key_match(CategoryKey const & keyval) const;
    // CombineHarvester category_id(std::vector<int> const& id) const;
    // CombineHarvester no_shapes() const;
    // CombineHarvester backgrounds() const;
    // CombineHarvester nuisance(std::vector<std::string> const& nuisance) const;
    // CombineHarvester nuisance_pred(std::function<bool(Nuisance const&)> fn) const;
    // double GetRate();
    // double GetObservedRate();
    // double GetUncertainty();
    // inline void SetIgnoreNuisanceCorrelations(bool const& val) {ignore_nuisance_correlations_ = val; }
    // TH1F   GetShape();
    // TH1F   GetObservedShape();
    // TGraphAsymmErrors GetObservedShapeErrors();
    // std::set<std::string> GetNuisanceSet();
    // bool HasProcess(std::string const& process) const;
    // void ScaleProcessByEra(std::string const& process, std::string const& era, double scale);
    // std::pair<double, int> GetPullsChi2(bool splusb) const;
 private:
  std::vector<std::shared_ptr<Observation>> obs_;
  std::vector<std::shared_ptr<Process>> procs_;
  std::vector<std::shared_ptr<Nuisance>> nus_;
    // std::map<std::string, std::shared_ptr<Parameter>> params_; 
    // std::vector<Pull> pulls_;
    // bool ignore_nuisance_correlations_;
  struct HistMapping {
    std::string process;
    std::string category;
    std::unique_ptr<TFile> file;
    std::string pattern;
    std::string syst_pattern;
  };
  typedef std::vector<std::pair<std::string, std::string>> StrPairVec;
  StrPairVec GenerateShapeMapAttempts(std::string process, 
      std::string category);
  TH1 * GetHistFromFile(std::vector<HistMapping> const& mappings,
      std::string const& bin,
      std::string const& process,
      std::string const& mass,
      std::string const& nuisance,
      unsigned type);
  friend void swap(CombineHarvester& first, CombineHarvester& second);
};
}

//   struct Nuisance {
//     Category      category;

//     std::string   nuisance;
//     std::string   process;
//     int           process_id;
//     std::string   type;
//     double        value_u;
//     double        value_d;
//     bool          asymm;
//     TH1*          shape;
//     TH1*          shape_u;
//     TH1*          shape_d;

//     friend std::ostream& operator<< (std::ostream &out, Nuisance &val);
//     static std::ostream& PrintHeader(std::ostream &out);
//   };

//   struct Parameter {
//     std::string name;
//     double      value;
//     double      error_u;
//     double      error_d;

//     friend std::ostream& operator<< (std::ostream &out, Parameter &val);
//     static std::ostream& PrintHeader(std::ostream &out);
//   };

  // struct Observation {
  //   Category            category;

  //   std::string         process;
  //   double              rate;
  //   TH1*                shape;
  //   TGraphAsymmErrors*  errors;

  //   friend std::ostream& operator<< (std::ostream &out, Observation &val);
  //   static std::ostream& PrintHeader(std::ostream &out);
  // };

  // struct Process {
  //   Category      category;

  //   std::string   process;
  //   int           process_id;
  //   double        rate;
  //   TH1*          shape;

  //   friend std::ostream& operator<< (std::ostream &out, Process &val);
  //   static std::ostream& PrintHeader(std::ostream &out);
  // };

// struct Pull {
//  std::string name;
//  double      prefit;
//  double      prefit_err;
//  double      bonly;
//  double      bonly_err;
//  double      splusb;
//  double      splusb_err;
//  double      rho;
//  void Print() const;
// };

  /*

    CombineHarvester h;
    h.ParseDatacard("datacard.txt");

    CombineHarvester p = h; // copies all underlying
    CombineHarvester p = h.processes{"VH"}; // h.processes makes a "shallow" copy

    Have a private copy constructor and assignment operator




  */



// void PullsFromFile(std::string const& filename, std::vector<ic::Pull> & pullvec, bool verbose);

// bool BvsSBComparator(Pull const& pull1, Pull const& pull2);

// TGraphAsymmErrors BuildPoissonErrors(TH1F const& hist);

// }

#endif