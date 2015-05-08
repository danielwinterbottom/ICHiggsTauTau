#ifndef ICHiggsTauTau_HiggsTauTau_HhhEMuMVA_h
#define ICHiggsTauTau_HiggsTauTau_HhhEMuMVA_h

// #include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace TMVA {
	class Reader;
}

namespace ic {

class TreeEvent;

class HhhEMuMVA : public ModuleBase {
 private:
 
 	CLASS_MEMBER(HhhEMuMVA, std::string, ditau_label)
 	CLASS_MEMBER(HhhEMuMVA, std::string, met_label)
 	CLASS_MEMBER(HhhEMuMVA, std::string, jets_label)
 	CLASS_MEMBER(HhhEMuMVA, std::string, gf_mva_file)
  // CLASS_MEMBER(HTTEMuMVA, std::string, vbf_mva_file)

 	TMVA::Reader *gf_reader_;
  // TMVA::Reader *vbf_reader_;

 	float fpzeta_;
 	float fpzetamiss_;
 	float fmet_;
 	float fpt_1_;
 	float fmt_ll_;
 	float fpt_2_;
 	float femu_dphi_;

 public:
  HhhEMuMVA(std::string const& name);
  virtual ~HhhEMuMVA();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
