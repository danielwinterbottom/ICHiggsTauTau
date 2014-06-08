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
#include "RooWorkspace.h"
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
  CombineHarvester cp();
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
  int ParseDatacard(std::string const& filename,
      std::string parse_rule);

  void WriteDatacard(std::string const& name, std::string const& root_file);
  void WriteDatacard(std::string const& name, TFile & root_file);

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

  CombineHarvester& bin(std::vector<std::string> const& vec);
  CombineHarvester& bin_id(std::vector<int> const& vec);
  CombineHarvester& process(std::vector<std::string> const& vec);
  CombineHarvester& process_id(std::vector<int> const& vec);
  CombineHarvester& analysis(std::vector<std::string> const& vec);
  CombineHarvester& era(std::vector<std::string> const& vec);
  CombineHarvester& channel(std::vector<std::string> const& vec);
  CombineHarvester& mass(std::vector<std::string> const& vec);
  CombineHarvester& nus_name(std::vector<std::string> const& vec);
  CombineHarvester& nus_type(std::vector<std::string> const& vec);

  CombineHarvester& signals();
  CombineHarvester& backgrounds();
  CombineHarvester& histograms();
  CombineHarvester& pdfs();
  CombineHarvester& data();

  // Set generation
  template<typename T>
  std::set<T> GenerateSetFromProcs(std::function<T(ch::Process const*)> func);

  template<typename T>
  std::set<T> GenerateSetFromObs(std::function<T(ch::Observation const*)> func);

  template<typename T>
  std::set<T> GenerateSetFromNus(std::function<T(ch::Nuisance const*)> func);

  // ForEach
  template<typename Function>
  void ForEachProc(Function func);

  template<typename Function>
  void ForEachObs(Function func);

  template<typename Function>
  void ForEachNus(Function func);

  // Yields & Shapes
  double GetRate();
  double GetObservedRate();
  double GetUncertainty();
  double GetUncertainty(RooFitResult const* fit, unsigned n_samples);
  TH1F GetShape();
  TH1F GetShapeWithUncertainty();
  TH1F GetShapeWithUncertainty(RooFitResult const* fit, unsigned n_samples);
  TH1F GetObservedShape();

  // Production
  void AddObservations(std::vector<std::string> mass,
                       std::vector<std::string> analysis,
                       std::vector<std::string> era,
                       std::vector<std::string> channel,
                       std::vector<std::pair<int, std::string>> bin);

  void AddProcesses(std::vector<std::string> mass,
                    std::vector<std::string> analysis,
                    std::vector<std::string> era,
                    std::vector<std::string> channel,
                    std::vector<std::string> procs,
                    std::vector<std::pair<int, std::string>> bin,
                    bool signal);
  void ExtractShapes(std::string const& file, std::string const& rule,
                     std::string const& syst_rule);

  void AddBinByBin(double threshold, bool fixed_norm, CombineHarvester * other);

  template <class Key>
  void AddSyst(CombineHarvester* other, std::string const& name,
               std::string const& type, std::function<Key(Process*)> keygen,
               std::map<Key, double> const& valmap);

  template <class Map>
  void AddSyst(CombineHarvester* other, std::string const& name,
               std::string const& type, Map const& valmap);

  // void Validate();
  void VariableRebin(std::vector<double> bins);

 private:
  std::vector<std::shared_ptr<Observation>> obs_;
  std::vector<std::shared_ptr<Process>> procs_;
  std::vector<std::shared_ptr<Nuisance>> nus_;
  std::map<std::string, std::shared_ptr<Parameter>> params_;
  std::map<std::string, std::shared_ptr<RooWorkspace>> wspaces_;

  struct HistMapping {
    std::string process;
    std::string category;
    std::shared_ptr<TFile> file;
    std::string pattern;
    std::string syst_pattern;
    bool IsHist();
    bool IsPdf();
    bool IsData();
  };

  typedef std::vector<std::pair<std::string, std::string>> StrPairVec;
  typedef std::vector<std::string> StrVec;
  StrPairVec GenerateShapeMapAttempts(std::string process,
      std::string category);
  HistMapping ResolveMapping(std::vector<HistMapping> const& mappings,
      std::string const& bin,
      std::string const& process,
      std::string const& mass,
      std::string const& nuisance);
  TH1 * GetHistFromFile(HistMapping const& mapping,
      unsigned type);
  RooAbsPdf * GetPdfFromFile(HistMapping const& mapping);
  RooAbsData * GetDataFromFile(HistMapping const& mapping);

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
std::set<T> CombineHarvester::GenerateSetFromProcs(
    std::function<T (ch::Process const*)> func) {
  std::set<T> ret;
  for (auto const& item : procs_) ret.insert(func(item.get()));
  return ret;
}

template<typename T>
std::set<T> CombineHarvester::GenerateSetFromObs(
    std::function<T (ch::Observation const*)> func) {
  std::set<T> ret;
  for (auto const& item : obs_) ret.insert(func(item.get()));
  return ret;
}

template<typename T>
std::set<T> CombineHarvester::GenerateSetFromNus(
    std::function<T (ch::Nuisance const*)> func) {
  std::set<T> ret;
  for (auto const& item : nus_) ret.insert(func(item.get()));
  return ret;
}

template<typename Function>
void CombineHarvester::ForEachProc(Function func) {
  for (auto & item: procs_) func(item.get());
}

template<typename Function>
void CombineHarvester::ForEachObs(Function func) {
  for (auto & item: obs_) func(item.get());
}

template<typename Function>
void CombineHarvester::ForEachNus(Function func) {
  for (auto & item: nus_) func(item.get());
}

template <class Key>
void CombineHarvester::AddSyst(CombineHarvester* other, std::string const& name,
                               std::string const& type,
                               std::function<Key(Process*)> keygen,
                               std::map<Key, double> const& valmap) {
  for (unsigned i = 0; i < procs_.size(); ++i) {
    Key key = keygen(procs_[i].get());
    if (!valmap.count(key)) continue;
    std::string subbed_name = name;
    boost::replace_all(subbed_name, "$BIN", procs_[i]->bin());
    boost::replace_all(subbed_name, "$PROCESS", procs_[i]->process());
    boost::replace_all(subbed_name, "$MASS", procs_[i]->mass());
    boost::replace_all(subbed_name, "$ERA", procs_[i]->era());
    boost::replace_all(subbed_name, "$CHANNEL", procs_[i]->channel());
    boost::replace_all(subbed_name, "$ANALYSIS", procs_[i]->analysis());
    auto nus = std::make_shared<Nuisance>();
    nus->set_bin(procs_[i]->bin());
    nus->set_process(procs_[i]->process());
    nus->set_process_id(procs_[i]->process_id());
    nus->set_analysis(procs_[i]->analysis());
    nus->set_era(procs_[i]->era());
    nus->set_channel(procs_[i]->channel());
    nus->set_bin_id(procs_[i]->bin_id());
    nus->set_mass(procs_[i]->mass());
    nus->set_name(subbed_name);
    nus->set_type(type);
    if (type == "lnN") {
      nus->set_asymm(false);
      nus->set_value_u(valmap.find(key)->second);
    } else if (type == "shape") {
      nus->set_asymm(true);
      nus->set_value_u(1.0);
      nus->set_value_d(1.0);
    }
    if (other) {
      other->nus_.push_back(nus);
    } else {
      nus_.push_back(nus);
    }
  }
}


template <class Map>
void CombineHarvester::AddSyst(CombineHarvester* other, std::string const& name,
                               std::string const& type,
                               Map const& valmap) {
  for (unsigned i = 0; i < procs_.size(); ++i) {
    if (!valmap.Contains(procs_[i].get())) continue;
    std::string subbed_name = name;
    boost::replace_all(subbed_name, "$BIN", procs_[i]->bin());
    boost::replace_all(subbed_name, "$PROCESS", procs_[i]->process());
    boost::replace_all(subbed_name, "$MASS", procs_[i]->mass());
    boost::replace_all(subbed_name, "$ERA", procs_[i]->era());
    boost::replace_all(subbed_name, "$CHANNEL", procs_[i]->channel());
    boost::replace_all(subbed_name, "$ANALYSIS", procs_[i]->analysis());
    auto nus = std::make_shared<Nuisance>();
    nus->set_bin(procs_[i]->bin());
    nus->set_process(procs_[i]->process());
    nus->set_process_id(procs_[i]->process_id());
    nus->set_analysis(procs_[i]->analysis());
    nus->set_era(procs_[i]->era());
    nus->set_channel(procs_[i]->channel());
    nus->set_bin_id(procs_[i]->bin_id());
    nus->set_mass(procs_[i]->mass());
    nus->set_name(subbed_name);
    nus->set_type(type);
    if (type == "lnN") {
      nus->set_asymm(false);
      nus->set_value_u(valmap.Val(procs_[i].get()));
    } else if (type == "shape") {
      nus->set_asymm(true);
      nus->set_value_u(1.0);
      nus->set_value_d(1.0);
    }
    if (other) {
      other->nus_.push_back(nus);
    } else {
      nus_.push_back(nus);
    }
  }
}




}

#endif
