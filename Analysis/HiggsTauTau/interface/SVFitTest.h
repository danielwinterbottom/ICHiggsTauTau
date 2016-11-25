#ifndef ICHiggsTauTau_HiggsTauTau_SVFitTest_h
#define ICHiggsTauTau_HiggsTauTau_SVFitTest_h

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

class SVFitTest : public ModuleBase {
 private:

   typedef std::tuple<unsigned, unsigned, unsigned> tri_unsigned;
  inline std::size_t RunLumiEvtHash(unsigned run, unsigned lumi, unsigned evt) const {
    std::size_t id = 0;
    boost::hash_combine(id, evt);
    boost::hash_combine(id, lumi);
    boost::hash_combine(id, run);
    return id;
  }

  inline std::size_t ObjectsHash(Candidate const* c1, Candidate const* c2, Met const* met) const {
    std::size_t id = 0;
    boost::hash_combine(id, c1->id());
    boost::hash_combine(id, c2->id());
    boost::hash_combine(id, met->id());
    return id;
  }
    // These variables for writing the svfit input
    TFile *out_file_;
    TTree *out_tree_;
    uint64_t out_event_;
    uint64_t out_lumi_;
    uint64_t out_run_;
    uint64_t out_objects_hash_;
    Candidate *out_cand1_;
    int dm1_ = -1;
    Candidate *out_cand2_;
    int dm2_ = -1;
    Met *out_met_;

    // These variables for reading in svfit output


    // The decay mode will be set internally depending on the channel
    // 0: leptonic-hadronic // 1: leptonic-leptonic
    unsigned decay_mode_;


  boost::filesystem::path total_path_;

  // std::map<std::string, double> mass_map;
  // We map from a combined hash of the run,ls,event to a the lepton+met hash and the mass value
  std::map<tri_unsigned, std::pair<std::size_t,double> > mass_map;
  // And a similar map from a combined hash of the run,ls,event to a the lepton+met hash, the higgs 4-vector and svfit transverse mass
  std::map<tri_unsigned, std::tuple<std::size_t,Candidate,double> > p4_map;
 
  CLASS_MEMBER(SVFitTest, ic::channel, channel)
  CLASS_MEMBER(SVFitTest, ic::strategy, strategy)

  // The module operation mode:
  // 0: Do Nothing, no mass added to event
  // 1: Create svfit inputs
  // 2: Read svfit output
  CLASS_MEMBER(SVFitTest, unsigned, run_mode)

  // When using run_mode 2, set behaviour when mass not found
  // 0: Add a dummy mass value, track number of events where this occurs
  // 1: Stop processing, throw exception
  // 2: Calculate svfit mass on the fly
  CLASS_MEMBER(SVFitTest, unsigned, fail_mode)

  // When getting the svfit mass for an event, verify that the
  // mass was calculated using the same lepton and met inputs
  CLASS_MEMBER(SVFitTest, bool, require_inputs_match)


  CLASS_MEMBER(SVFitTest, std::string, outname)
  CLASS_MEMBER(SVFitTest, unsigned, split)
  CLASS_MEMBER(SVFitTest, std::string, fullpath)
  CLASS_MEMBER(SVFitTest, std::string, met_label)
  CLASS_MEMBER(SVFitTest, std::string, dilepton_label)
  CLASS_MEMBER(SVFitTest, bool, legacy_svfit)
  CLASS_MEMBER(SVFitTest, bool, MC)
  CLASS_MEMBER(SVFitTest, bool, do_preselection)
  CLASS_MEMBER(SVFitTest, bool, from_grid)
  CLASS_MEMBER(SVFitTest, bool, read_all)
  CLASS_MEMBER(SVFitTest, bool, read_svfit_mt)
  CLASS_MEMBER(SVFitTest, bool, tau_optimisation)

  unsigned file_counter_;
  unsigned event_counter_;
  std::string outputadd_;



 public:
  SVFitTest(std::string const& name);
  virtual ~SVFitTest();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  void WriteRunScript();
};

}

#endif
