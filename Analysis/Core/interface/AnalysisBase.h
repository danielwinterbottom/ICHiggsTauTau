#ifndef ICHiggsTauTau_Core_AnalysisBase_h
#define ICHiggsTauTau_Core_AnalysisBase_h

#include <vector>
#include <string>
#include <set>
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "boost/function.hpp"
#include "boost/bind.hpp"

namespace ic {

	class AnalysisBase {

  protected:
    std::string analysis_name_;
    std::vector<std::string> input_file_paths_;
    std::string tree_path_;
    std::string tree_name_;
    unsigned events_to_process_;
    unsigned events_processed_;
    std::vector<ic::ModuleBase *> modules_;
    std::vector<double> weighted_yields_;
    ic::TreeEvent event_;
    bool notify_on_fail_;
    bool notify_evt_on_fail_;
    std::set< std::pair<int,int> > notify_run_event_;
    std::set< int > notify_event_;
    std::string skim_path_;
    bool print_module_list_;

  public:
    AnalysisBase(   std::string const& analysis_name, 
                    std::vector<std::string> const& input,
                    std::string const& tree_path,
                    std::string const& tree_name,
                    unsigned const& events     );

    
    void AddModule(ic::ModuleBase *module_ptr);
    inline std::string analysis_name() { return analysis_name_; }
    inline std::string tree_path() { return tree_path_; }
    inline std::string tree_name() { return tree_name_; }
    virtual ~AnalysisBase();
    virtual int RunAnalysis();
    virtual void DoEventSetup();
    virtual bool PostModule(int status);
    virtual void NotifyRunEvent(int const& run, int const& event);
virtual     void NotifyEvent(int const& event);
    void DoSkimming(std::string const& skim_path) { skim_path_ = skim_path; }
 };
}

#endif
