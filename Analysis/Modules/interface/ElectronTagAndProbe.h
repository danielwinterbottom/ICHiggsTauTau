#ifndef ICHiggsTauTau_Analysis_TagAndProbe_ElectronTagAndProbe_h
#define ICHiggsTauTau_Analysis_TagAndProbe_ElectronTagAndProbe_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "TH1.h"

namespace ic {

class ElectronTagAndProbe : public ModuleBase {
 private:
  
  CLASS_MEMBER(ElectronTagAndProbe, fwlite::TFileService*, fs);
  DynamicHistoSet *hists_;
  TH1* weights;

  std::string output_name_;
  int object_count, passing_count, failing_count;  
  double min_mass_;
  double max_mass_;
  int run_low_;
  int run_high_; 
  bool data_;  
  unsigned hist_bins_;
  boost::function<bool (Electron const*)> tag_predicate_;
  boost::function<bool (Electron const*)> probe_predicate_;
  boost::function<bool (Electron const*)> passprobe_predicate_;
  std::vector<int> pt_bins_;
  std::vector<double> eta_bins_;
  std::vector<int> vtx_bins_;
  std::string era_;
  std::map<int, int> prescales; 
  std::map<int, int> nrun; 
  std::ofstream outFile;
  int mode_;
  bool probe_match;// for double electron samples  
  //mode=0 -id 
  //mode=1 -iso
  //mode=2 -id+iso
  //mode=10 -id fine bins
  //mode=11 -iso fine bins
  //mode=12 -id+iso fine bins
  //mode=3 -trg

 public:
  ElectronTagAndProbe(std::string const& name);
  virtual ~ElectronTagAndProbe();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  

  ElectronTagAndProbe & set_output_name(std::string const& output_name) {
    output_name_ = output_name;
    return *this;
  }
  ElectronTagAndProbe & set_min_mass(double const& min_mass) {
    min_mass_ = min_mass;
    return *this;
  }
  ElectronTagAndProbe & set_max_mass(double const& max_mass) {
    max_mass_ = max_mass;
    return *this;
  }
  ElectronTagAndProbe & set_run_low(int const& run_low) {
    run_low_ = run_low;
    return *this;
  }
  ElectronTagAndProbe & set_run_high(int const& run_high) {
    run_high_ = run_high;
    return *this;
  }
  ElectronTagAndProbe & set_data(bool const& data) {
    data_ = data;
    return *this;
  }
  ElectronTagAndProbe & set_hist_bins(unsigned const& hist_bins) {
    hist_bins_ = hist_bins;
    return *this;
  }
  ElectronTagAndProbe & set_tag_predicate(boost::function<bool (Electron const*)> tag_predicate) {
    tag_predicate_ = tag_predicate;
    return *this;
  }
  ElectronTagAndProbe & set_probe_predicate(boost::function<bool (Electron const*)> probe_predicate) {
    probe_predicate_ = probe_predicate;
    return *this;
  }
  ElectronTagAndProbe & set_passprobe_predicate(boost::function<bool (Electron const*)> passprobe_predicate) {
    passprobe_predicate_ = passprobe_predicate;
    return *this;
  }
  ElectronTagAndProbe & set_pt_bins(const std::vector<int>& pt_bins) {
    pt_bins_ = pt_bins;
    return *this;
  }
  ElectronTagAndProbe & set_eta_bins(const std::vector<double>& eta_bins) {
    eta_bins_ = eta_bins;
    return *this;
  }
  ElectronTagAndProbe & set_vtx_bins(const std::vector<int>& vtx_bins) {
    vtx_bins_ = vtx_bins;
    return *this;
  }
  ElectronTagAndProbe & set_mode(unsigned const& mode) {
    mode_ = mode;
    return *this;
  }
  ElectronTagAndProbe & set_era(std::string const& era) {
    era_ = era;
    return *this;
  }
};

}

#endif
