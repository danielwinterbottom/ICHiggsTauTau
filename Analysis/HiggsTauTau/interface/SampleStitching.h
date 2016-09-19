#ifndef ICHiggsTauTau_Module_SampleStitching_h
#define ICHiggsTauTau_Module_SampleStitching_h

#include <string>
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"

namespace ic {

class DYJetsStitching : public ModuleBase {
 private:
  CLASS_MEMBER(DYJetsStitching, fwlite::TFileService*, fs)

  TTree *t_gen_info_;
  int decay_;
  float mll_;
  int njets_;
  float wt_;

  double lumi_DYJetsToLL_;
  double lumi_DY1JetsToLL_;
  double lumi_DY2JetsToLL_;
  double lumi_DY3JetsToLL_;
  double lumi_DY4JetsToLL_;
  double lumi_DYJetsToTauTau_M150_;

 public:
  DYJetsStitching(std::string const& name);
  virtual ~DYJetsStitching();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  inline DYJetsStitching & Set_DYJetsToLL(unsigned evts, double xsec) {
    lumi_DYJetsToLL_ = double(evts) / xsec;
    return *this;
  }
  inline DYJetsStitching & Set_DY1JetsToLL(unsigned evts, double xsec) {
    lumi_DY1JetsToLL_ = double(evts) / xsec;
    return *this;
  }
  inline DYJetsStitching & Set_DY2JetsToLL(unsigned evts, double xsec) {
    lumi_DY2JetsToLL_ = double(evts) / xsec;
    return *this;
  }
  inline DYJetsStitching & Set_DY3JetsToLL(unsigned evts, double xsec) {
    lumi_DY3JetsToLL_ = double(evts) / xsec;
    return *this;
  }
  inline DYJetsStitching & Set_DY4JetsToLL(unsigned evts, double xsec) {
    lumi_DY4JetsToLL_ = double(evts) / xsec;
    return *this;
  }
  inline DYJetsStitching & Set_DYJetsToTauTau_M150(unsigned evts, double xsec) {
    lumi_DYJetsToTauTau_M150_ = double(evts) / xsec;
    return *this;
  }
};

class WJetsStitching : public ModuleBase {
 private:
  CLASS_MEMBER(WJetsStitching, fwlite::TFileService*, fs)

  TTree *t_gen_info_;
  int njets_;
  float wt_;

  double lumi_WJetsToLNu_;
  double lumi_W1JetsToLNu_;
  double lumi_W2JetsToLNu_;
  double lumi_W3JetsToLNu_;
  double lumi_W4JetsToLNu_;

 public:
  WJetsStitching(std::string const& name);
  virtual ~WJetsStitching();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  inline WJetsStitching & Set_WJetsToLNu(unsigned evts, double xsec) {
    lumi_WJetsToLNu_ = double(evts) / xsec;
    return *this;
  }
  inline WJetsStitching & Set_W1JetsToLNu(unsigned evts, double xsec) {
    lumi_W1JetsToLNu_ = double(evts) / xsec;
    return *this;
  }
  inline WJetsStitching & Set_W2JetsToLNu(unsigned evts, double xsec) {
    lumi_W2JetsToLNu_ = double(evts) / xsec;
    return *this;
  }
  inline WJetsStitching & Set_W3JetsToLNu(unsigned evts, double xsec) {
    lumi_W3JetsToLNu_ = double(evts) / xsec;
    return *this;
  }
  inline WJetsStitching & Set_W4JetsToLNu(unsigned evts, double xsec) {
    lumi_W4JetsToLNu_ = double(evts) / xsec;
    return *this;
  }
};


}

#endif
