/*
  functionmacro.C : provide Higgs pT reweighting factor for the given mass and tan(beta)
  Don't change ! (unless, you need your own defined function)

  20 April 2016
 */


#include "TMath.h"
#include "TFile.h"
#include "TH1.h"
#include <iostream>
#include <vector>
#include "TROOT.h"
#include "TGraphErrors.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"

namespace nloweights {

extern TGraphErrors* func[27][60];
extern std::vector<int> marray;

void ReadFile();
float returnNLOweight(Int_t mass, Int_t tanb, Double_t pt);
}


namespace ic {

class NLOWeighting : public ModuleBase {
 private:
  std::string jets_label_;
  CLASS_MEMBER(NLOWeighting, fwlite::TFileService*, fs)
  TTree *tout;
  double nlo_wt_ = 0.0;
  double nlo_pt_ = 0.0;

 public:
  NLOWeighting(std::string const& name);
  virtual ~NLOWeighting();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

