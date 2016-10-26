#ifndef ICHiggsTauTau_Module_HinvPrint_h
#define ICHiggsTauTau_Module_HinvPrint_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "TTree.h"
#include "TFile.h"


#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

namespace ic {

  struct RunLumiEvent {
    unsigned run;
    unsigned lumi;
    unsigned evt;

    bool operator==(const RunLumiEvent & rhs){
      if (run == rhs.run && lumi == rhs.lumi && evt == rhs.evt) return true;
      return false;
    }

    bool operator<(const RunLumiEvent & rhs) const{
      if (run < rhs.run) return true;
      else if (run == rhs.run){
        if (lumi < rhs.lumi) return true;
        else if (lumi == rhs.lumi){
          if (evt < rhs.evt) return true;
        }
      }
      return false;
    }


  };

class HinvPrint : public ModuleBase {
 private:
  std::set<RunLumiEvent> events_;
  std::ofstream foutList_;
  bool runLumiEvt_;
  bool filter_;
  bool is_data_;

 public:
  HinvPrint(std::string const& name, bool is_data, bool filter=false, bool runLumiEvt=false);
  virtual ~HinvPrint();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  void PrintEvent(unsigned run, unsigned lumi, unsigned evt);
};

}

#endif
