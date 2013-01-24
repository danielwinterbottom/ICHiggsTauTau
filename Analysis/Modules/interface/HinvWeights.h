#ifndef ICHiggsTauTau_Module_HinvWeights_h
#define ICHiggsTauTau_Module_HinvWeights_h

#include "UserCode/ICHiggsTauTau/interface/TH2DAsymErr.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagWeight.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include <string>


namespace ic {

class HinvWeights : public ModuleBase {
 private:
  CLASS_MEMBER(HinvWeights, ic::mc, mc)
  CLASS_MEMBER(HinvWeights, ic::era, era)
  CLASS_MEMBER(HinvWeights, bool, do_trg_weights)
  CLASS_MEMBER(HinvWeights, bool, trg_applied_in_mc)
  CLASS_MEMBER(HinvWeights, bool, do_w_soup)
  CLASS_MEMBER(HinvWeights, bool, do_dy_soup)

  double f0_,f1_,f2_,f3_,f4_,n_inc_,n1_,n2_,n3_,n4_,w0_,w1_,w2_,w3_,w4_;
  double zf0_,zf1_,zf2_,zf3_,zf4_,zn_inc_,zn1_,zn2_,zn3_,zn4_,zw0_,zw1_,zw2_,zw3_,zw4_;


 public:
  HinvWeights(std::string const& name);
  virtual ~HinvWeights();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  double Efficiency(double m, double m0, double sigma, double alpha, double n, double norm);
  void SetWTargetFractions(double f0, double f1, double f2, double f3, double f4);
  void SetWInputYields(double n_inc, double n1, double n2, double n3, double n4);
  void SetDYTargetFractions(double zf0, double zf1, double zf2, double zf3, double zf4);
  void SetDYInputYields(double zn_inc, double zn1, double zn2, double zn3, double zn4);

};

}

#undef MEMBER_NP

#endif
