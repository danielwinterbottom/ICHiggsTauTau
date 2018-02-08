#ifndef ICHiggsTauTau_HiggsTauTau_MELATest_h
#define ICHiggsTauTau_HiggsTauTau_MELATest_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "boost/filesystem.hpp"
#include "boost/unordered_map.hpp"
#include "boost/functional/hash.hpp"


#include <string>
#include <fstream>
#include <tuple>
#include "TLorentzVector.h"

namespace ic {

class MELATest : public ModuleBase {
 private:

  typedef std::tuple<unsigned, unsigned, unsigned> tri_unsigned;

  TFile *out_file_;
  TTree *out_tree_;
  uint64_t out_event_;
  uint64_t out_lumi_;
  uint64_t out_run_;

  JetCorrectionUncertainty *uncert_;
  JetCorrectorParameters *params_;  
 
  double Hpx_;
  double Hpy_;
  double Hpz_;
  double HE_;

  std::vector<double> jpx_;  
  std::vector<double> jpy_;
  std::vector<double> jpz_;
  std::vector<double> jE_;
  unsigned n_jets_;

  boost::filesystem::path total_path_;
  
  std::map<tri_unsigned, std::pair<float,float> > mela_map;

 
  CLASS_MEMBER(MELATest, ic::channel, channel)
  CLASS_MEMBER(MELATest, unsigned, run_mode)
  CLASS_MEMBER(MELATest, std::string, outname)
  CLASS_MEMBER(MELATest, unsigned, split)
  CLASS_MEMBER(MELATest, std::string, fullpath)
  CLASS_MEMBER(MELATest, bool, read_all)
  CLASS_MEMBER(MELATest, std::string, met_label)
  CLASS_MEMBER(MELATest, std::string, jets_label)
  CLASS_MEMBER(MELATest, std::string, dilepton_label)
  CLASS_MEMBER(MELATest, bool, use_svfit_vec)
  CLASS_MEMBER(MELATest, std::string, add_name)
  CLASS_MEMBER(MELATest, std::string, jes_uncert_file)
  CLASS_MEMBER(MELATest, std::string, jes_uncert_set)
  CLASS_MEMBER(MELATest, unsigned, jes_shift_mode)


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
