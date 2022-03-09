#ifndef ICHiggsTauTau_4tau_BTagWeightLegacyRun2_h
#define ICHiggsTauTau_4tau_BTagWeightLegacyRun2_h

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
#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTConfig.h"
#include "TRandom3.h"

namespace ic {


class BTagWeightLegacyRun2 : public ModuleBase {
  private :
   CLASS_MEMBER(BTagWeightLegacyRun2, ic::channel, channel)
   CLASS_MEMBER(BTagWeightLegacyRun2, ic::era, era)
   CLASS_MEMBER(BTagWeightLegacyRun2, ic::strategy, strategy)
   CLASS_MEMBER(BTagWeightLegacyRun2, std::string, jet_label)
   CLASS_MEMBER(BTagWeightLegacyRun2, TH2F*,bbtag_eff)
   CLASS_MEMBER(BTagWeightLegacyRun2, TH2F*,cbtag_eff)
   CLASS_MEMBER(BTagWeightLegacyRun2, TH2F*,othbtag_eff)
   CLASS_MEMBER(BTagWeightLegacyRun2, TH2F*,bbtag_eff_alt)
   CLASS_MEMBER(BTagWeightLegacyRun2, TH2F*,cbtag_eff_alt)
   CLASS_MEMBER(BTagWeightLegacyRun2, TH2F*,othbtag_eff_alt)
   CLASS_MEMBER(BTagWeightLegacyRun2, std::string, add_name)
   CLASS_MEMBER(BTagWeightLegacyRun2, TH2F*,bbtag_eff_cp5)
   CLASS_MEMBER(BTagWeightLegacyRun2, TH2F*,cbtag_eff_cp5)
   CLASS_MEMBER(BTagWeightLegacyRun2, TH2F*,othbtag_eff_cp5)
   CLASS_MEMBER(BTagWeightLegacyRun2, TH2F*,bbtag_eff_cp5_alt)
   CLASS_MEMBER(BTagWeightLegacyRun2, TH2F*,cbtag_eff_cp5_alt)
   CLASS_MEMBER(BTagWeightLegacyRun2, TH2F*,othbtag_eff_cp5_alt)
   CLASS_MEMBER(BTagWeightLegacyRun2, bool, do_cp5_2016)

   //1 Down 2 Up (promote-demote) || 
   //3 JES Down 4 Jes Up 5 LF Down 6 LF Up  7 HF down 8 HFUp 9HFStats1Down 10 HFStats1Up 11 HFStats2Down 12 HFStats2Up 13 LFStats1Down 14 LFStats1Up 
   //15 LFStats2Down 16 LFStats2Up 17 CFErrIDown 18CFErr1Up 19 CFErr2Down 20 CFerr2Up

  const BTagCalibration *calib;
  BTagCalibrationReader* reader_comb_tight;
  BTagCalibrationReader* reader_comb_loose;

 public:
  BTagWeightLegacyRun2(std::string const& name);
  virtual ~BTagWeightLegacyRun2();

  std::vector<double> EventReweighting(std::vector<PFJet *> const& jets) const;
  double GetEff(unsigned flav, double pt, double eta, std::string wp="tight") const;
  double GetEffCP5(unsigned flav, double pt, double eta, std::string wp="tight") const;
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();


};


}

#endif
