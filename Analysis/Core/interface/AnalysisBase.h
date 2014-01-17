#ifndef ICHiggsTauTau_Core_AnalysisBase_h
#define ICHiggsTauTau_Core_AnalysisBase_h

#include <vector>
#include <string>
#include <set>
#include <utility>
#include "Core/interface/TreeEvent.h"
namespace ic { class ModuleBase; }

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
    bool ttree_caching_;
    bool stop_on_failed_file_;
    bool retry_on_fail_;
    unsigned retry_pause_;
    unsigned retry_attempts_;
    int skim_after_module_;

  public:
    //! The standard AnalysisBase constructor constructor 
    /*! 
     */
    AnalysisBase(   std::string const& analysis_name,       /**< [in] a name for the analysis object, purely decorative. */
                    std::vector<std::string> const& input,  /**< [in] a vector of TFile paths  */
                    std::string const& tree_path,           /**< [in] path to the directory containing the TTree in the TFile */   
                    std::string const& tree_name,           /**< [in] the TTree name */  
                    unsigned const& events                  /**< [in] the number of events to process, -1 means process all events. */
                );             


    void AddModule(ic::ModuleBase *module_ptr);
    inline std::string analysis_name() { return analysis_name_; }
    inline std::string tree_path() { return tree_path_; }
    inline std::string tree_name() { return tree_name_; }
    virtual ~AnalysisBase();
    virtual int RunAnalysis();    
    virtual void DoEventSetup();  
    virtual bool PostModule(int status);
    virtual void NotifyRunEvent(int const& run, int const& event);
    virtual void NotifyEvent(int const& event);
    void DoSkimming(std::string const& skim_path) { skim_path_ = skim_path; }
    void WriteSkimHere();
    void SetTTreeCaching(bool const& value);
    void StopOnFileFailure(bool const& value);
    void RetryFileAfterFailure(unsigned pause_in_seconds, unsigned retry_attempts);
 };
}

#endif
