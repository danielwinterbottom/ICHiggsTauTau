#ifndef ICHiggsTauTau_HiggsTauTau_GenTrees_h
#define ICHiggsTauTau_HiggsTauTau_GenTrees_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HTTPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"

#include <string>

namespace ic {

class GenTrees : public ModuleBase {
 private:
  CLASS_MEMBER(GenTrees, ic::channel, channel)
  CLASS_MEMBER(GenTrees, fwlite::TFileService*, fs)

  TTree *outtree_;

  unsigned long long event_;
  double wt_;
  int wt_sign_;
  int pdgid_1_ = 0;
  int pdgid_2_ = 0;
  int pdgid_gen_1_ = 0;
  int pdgid_gen_2_ = 0;
  int npnlo_ = 0;
  unsigned nlhepartons_;
  unsigned ngenjets_;
  double zptgen_ = -1;  
  double zmassgen_ = -1;
  double zptlhe_ = -1;  
  double zmasslhe_ = -1;
  double vispT_ = -1;
  double visM_ = -1;
  double genpT_ = -1;
  double genM_ = -1;
  double pt_1_ = 0;
  double pt_2_ = 0;
  double eta_1_ = -999;
  double eta_2_ = -999;
  double lhe_pt_1_ = 0;
  double lhe_pt_2_ = 0;
  double lhe_eta_1_ = -999;
  double lhe_eta_2_ = -999;
  double gen_jet_pt_1_ = 0;
  double gen_jet_pt_2_ = 0;
  double gen_jet_eta_1_ = -999;
  double gen_jet_eta_2_ = -999;
  double m_bb_=-1;
  double m_jj_=-1;



 public:
  GenTrees(std::string const& name);
  virtual ~GenTrees();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  };
}

#endif


