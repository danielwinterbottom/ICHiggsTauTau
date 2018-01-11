#ifndef ICHiggsTauTau_HiggsTauTau_MELATest_h
#define ICHiggsTauTau_HiggsTauTau_MELATest_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "boost/filesystem.hpp"
#include "boost/unordered_map.hpp"
#include "boost/functional/hash.hpp"


#include <string>
#include <fstream>
#include <tuple>

namespace ic {

class MELATest : public ModuleBase {
 private:

  TFile *out_file_;
  TTree *out_tree_;
  uint64_t out_event_;
  uint64_t out_lumi_;
  uint64_t out_run_;
  
  double Hpx_;
  double Hpy_;
  double Hpz_;
  double HE_;
  double jpx_1_;
  double jpy_1_;
  double jpz_1_;
  double jE_1_; 
  double jpx_2_;
  double jpy_2_;
  double jpz_2_;
  double jE_2_; 

  boost::filesystem::path total_path_;
 
  CLASS_MEMBER(MELATest, ic::channel, channel)
  CLASS_MEMBER(MELATest, ic::strategy, strategy)
  CLASS_MEMBER(MELATest, unsigned, run_mode)
  CLASS_MEMBER(MELATest, std::string, outname)
  CLASS_MEMBER(MELATest, unsigned, split)
  CLASS_MEMBER(MELATest, std::string, fullpath)
  CLASS_MEMBER(MELATest, bool, from_grid)
  CLASS_MEMBER(MELATest, bool, read_all)
  CLASS_MEMBER(MELATest, std::string, met_label)
  CLASS_MEMBER(MELATest, std::string, jets_label)
  CLASS_MEMBER(MELATest, std::string, dilepton_label)
  CLASS_MEMBER(MELATest, bool, use_svfit_vec)


  unsigned file_counter_;
  unsigned event_counter_;
  std::string outputadd_;


 public:
  MELATest(std::string const& name);
  virtual ~MELATest();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
