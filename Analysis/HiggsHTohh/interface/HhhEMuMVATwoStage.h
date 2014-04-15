#ifndef ICHiggsTauTau_HiggsTauTau_HhhEMuMVATwoStage_h
#define ICHiggsTauTau_HiggsTauTau_HhhEMuMVATwoStage_h

// #include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace TMVA {
	class Reader;
}

namespace ic {

class TreeEvent;

class HhhEMuMVATwoStage : public ModuleBase {
 private:
 
 	CLASS_MEMBER(HhhEMuMVATwoStage, std::string, ditau_label)
 	CLASS_MEMBER(HhhEMuMVATwoStage, std::string, met_label)
 	CLASS_MEMBER(HhhEMuMVATwoStage, std::string, gf_mva_file)
  CLASS_MEMBER(HhhEMuMVATwoStage, std::string, gf_mva_file_2)

 	TMVA::Reader *gf_reader_;
	TMVA::Reader *gf_reader_2_;
  // TMVA::Reader *vbf_reader_;

 	float fpzeta_;
 	float fpzetamiss_;
 	float fmet_;
 	float fpt_1_;
 	float fmt_ll_;
 	float fpt_2_;
 	float femu_dphi_;

 public:
  HhhEMuMVATwoStage(std::string const& name);
  virtual ~HhhEMuMVATwoStage();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
