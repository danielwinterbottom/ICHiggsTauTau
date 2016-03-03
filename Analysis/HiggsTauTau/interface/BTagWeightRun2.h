#ifndef ICHiggsTauTau_HiggsTauTau_BTagWeight_h
#define ICHiggsTauTau_HiggsTauTau_BTagWeight_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include <vector>
#include <string>
#include <math.h>
#include <iostream>
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagCalibrationStandalone.h"
#include "TRandom3.h"

namespace ic {


class BTagWeightRun2 : public ModuleBase {
  private :
   CLASS_MEMBER(BTagWeightRun2, std::string, jet_label)
   CLASS_MEMBER(BTagWeightRun2, TH2F*,bbtag_eff)
   CLASS_MEMBER(BTagWeightRun2, TH2F*,cbtag_eff)
   CLASS_MEMBER(BTagWeightRun2, TH2F*,othbtag_eff)

  TRandom3  *rand;
  BTagCalibration* calib;
  BTagCalibrationReader* reader_incl;
  BTagCalibrationReader* reader_mujets;
  BTagCalibrationReader* reader_iterativefit;

 public:
  BTagWeightRun2(std::string const& name);
  virtual ~BTagWeightRun2();

  std::map<std::size_t, bool> ReTag(std::vector<PFJet *> const& jets) const;
  double SFCSVShape(std::vector<PFJet *> const& jets) const;
  double GetEff(unsigned flav, double pt, double eta) const;
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();


};


}

#endif
