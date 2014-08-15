#ifndef ICHiggsTauTau_Core_AnalysisBase_h
#define ICHiggsTauTau_Core_AnalysisBase_h

#include <vector>
#include <string>
#include <set>
#include <utility>
#include "Core/interface/TreeEvent.h"
namespace ic {
class ModuleBase;
}

namespace ic {

class AnalysisBase {
  struct ModuleSequence {
    std::string name;
    std::vector<ic::ModuleBase*> modules;
    std::vector<uint64_t> counters;
    int skim_point;

    ModuleSequence() : name("default"), skim_point(-1) {}
    explicit ModuleSequence(std::string const& n) : name(n), skim_point(-1) {}
  };

 private:
  typedef std::vector<ic::ModuleBase*> V_Modules;
  typedef std::pair<std::string, V_Modules> SV_Modules;
  std::vector<ModuleSequence> seqs_;
  std::string analysis_name_;
  std::vector<std::string> input_files_;
  std::string tree_path_;
  int64_t events_to_process_;
  unsigned events_processed_;
  ic::TreeEvent event_;
  std::string skim_path_;
  bool print_module_list_;
  bool ttree_caching_;
  bool stop_on_failed_file_;
  bool retry_on_fail_;
  unsigned retry_pause_;
  unsigned retry_attempts_;

 public:
  AnalysisBase(std::string const& analysis_name,
               std::vector<std::string> const& input,
               std::string const& tree_path,
               int64_t const& events);

  /// Add module to the "default" sequence
  void AddModule(ic::ModuleBase* module_ptr);
  /// Add module to the named sequence (creating it if it doesn't exist)
  void AddModule(std::string const& seq_name, ic::ModuleBase* module_ptr);

  /// Returns the analysis name
  inline std::string analysis_name() { return analysis_name_; }
  inline std::string tree_path() { return tree_path_; }
  // inline std::string tree_name() { return tree_name_; }
  virtual ~AnalysisBase();
  virtual int RunAnalysis();
  virtual void DoEventSetup();
  virtual bool PostModule(int status);

  void DoSkimming(std::string const& skim_path) { skim_path_ = skim_path; }
  void WriteSkimHere();
  void WriteSkimHere(std::string const& seq_name);
  void SetTTreeCaching(bool const& value);
  void StopOnFileFailure(bool const& value);
  void RetryFileAfterFailure(unsigned pause_in_seconds,
                             unsigned retry_attempts);
};
}

#endif
