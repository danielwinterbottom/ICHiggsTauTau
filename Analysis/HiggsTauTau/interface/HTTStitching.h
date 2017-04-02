#ifndef ICHiggsTauTau_Module_HTTStitching_h
#define ICHiggsTauTau_Module_HTTStitching_h

#include "UserCode/ICHiggsTauTau/interface/TH2DAsymErr.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagWeight.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"

#include <string>


namespace ic {

class HTTStitching : public ModuleBase {
 private:
  CLASS_MEMBER(HTTStitching, ic::era, era)
  CLASS_MEMBER(HTTStitching, bool, do_w_soup)
  CLASS_MEMBER(HTTStitching, bool, do_w_soup_htbinned)
  CLASS_MEMBER(HTTStitching, bool, do_dy_soup)
  CLASS_MEMBER(HTTStitching, bool, do_dy_soup_high_mass)
  CLASS_MEMBER(HTTStitching, bool, do_dy_soup_htbinned)
  CLASS_MEMBER(HTTStitching, fwlite::TFileService*, fs)

  TTree *t_gen_info_;
  int t_decay_;
  float t_mll_;
  int t_njets_;
  double t_ht_;
  float t_wt_;

  double f0_,f1_,f2_,f3_,f4_,n_inc_,n1_,n2_,n3_,n4_,w0_,w1_,w2_,w3_,w4_;
  double zf0_,zf1_,zf2_,zf3_,zf4_,zn_inc_,zn1_,zn2_,zn3_,zn4_,zn_hm_,zw0_,zw1_,zw2_,zw3_,zw4_;
  double zfhm_,zfhm1_,zfhm2_,zfhm3_,zfhm4_,zw0hi_,zw1lo_,zw1hi_,zw2lo_,zw2hi_,zw3lo_,zw3hi_,zw4lo_,zw4hi_;
  double zxs0_,zxs1_,zxs2_,zxs3_,zxs4_,z_lo_nlo_corr_,zxsinc_,zxshm_;
  double wxs0_,wxs1_,wxs2_,wxs3_,wxs4_,w_lo_nlo_corr_;
  double wt_lumi_;


 public:
  HTTStitching(std::string const& name);
  virtual ~HTTStitching();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  void SetWTargetFractions(double f0, double f1, double f2, double f3, double f4);
  void SetWInputYields(double n_inc, double n1, double n2, double n3, double n4);
  void SetDYTargetFractions(double zf0, double zf1, double zf2, double zf3, double zf4);
  void SetDYInputYields(double zn_inc, double zn1, double zn2, double zn3, double zn4);
  void SetDYInputYieldsHighMass(double zn_inc, double zn1, double zn2, double zn3, double zn4, double zn_hm);
  void SetDYInputCrossSections(double zxs0, double zxs1, double zxs2, double zxs3, double zxs4);
  void SetDYInputCrossSectionsHighMass(double zxsinc, double zxs1, double zxs2, double zxs3, double zxs4, double zxshm);
  void SetWInputCrossSections(double wxs0, double wxs1, double wxs2, double wxs3, double wxs4);
};

}

#undef MEMBER_NP

#endif
