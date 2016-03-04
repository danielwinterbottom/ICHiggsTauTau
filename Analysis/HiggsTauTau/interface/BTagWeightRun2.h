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
   CLASS_MEMBER(BTagWeightRun2, bool, do_reshape)
   CLASS_MEMBER(BTagWeightRun2, unsigned, btag_mode) 
   CLASS_MEMBER(BTagWeightRun2, unsigned, bfake_mode)
   //1 Up 2 Down (promote-demote) || 
   //3 JES Up 4 Jes Down 5 LF UP 6 LFDown  7 HFUp 8 HFDown 9HFStats1Up 10 HFStats1Down 11 HFStats2Up 12 HFStats2Down 13 LFStats1Up 14 LFStats1Down 
   //15 LFStats2Up 16 LFStats2Down 17 CFErrIUp 18CFErr1Down 19 CFErr2Up 20 CFerr2Down

  TRandom3  *rand;
  BTagCalibration* calib;
  BTagCalibrationReader* reader_incl;
  BTagCalibrationReader* reader_mujets;
  BTagCalibrationReader* reader_iterativefit;
  //Syst uncs - promote demote:
  BTagCalibrationReader* reader_inclup;
  BTagCalibrationReader* reader_incldown;
  BTagCalibrationReader* reader_mujetsup;
  BTagCalibrationReader* reader_mujetsdown;
  //Syst uncs - btagshape
  BTagCalibrationReader* reader_jesup;
  BTagCalibrationReader* reader_jesdown;
  BTagCalibrationReader* reader_lfup;
  BTagCalibrationReader* reader_lfdown;
  BTagCalibrationReader* reader_hfup;
  BTagCalibrationReader* reader_hfdown;
  BTagCalibrationReader* reader_hfstats1up;
  BTagCalibrationReader* reader_hfstats1down;
  BTagCalibrationReader* reader_hfstats2up;
  BTagCalibrationReader* reader_hfstats2down;
  BTagCalibrationReader* reader_lfstats1up;
  BTagCalibrationReader* reader_lfstats1down;
  BTagCalibrationReader* reader_lfstats2up;
  BTagCalibrationReader* reader_lfstats2down;
  BTagCalibrationReader* reader_cferr1up;
  BTagCalibrationReader* reader_cferr1down;
  BTagCalibrationReader* reader_cferr2up;
  BTagCalibrationReader* reader_cferr2down;

 public:
  BTagWeightRun2(std::string const& name);
  virtual ~BTagWeightRun2();

  std::map<std::size_t, bool> ReTag(std::vector<PFJet *> const& jets, unsigned btag_mode, unsigned bfake_mode) const;
  double SFCSVShape(std::vector<PFJet *> const& jets, unsigned btag_mode) const;
  double GetEff(unsigned flav, double pt, double eta) const;
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();


};


}

#endif
