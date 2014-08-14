#ifndef ICHiggsTauTau_HiggsTauTau_HhhEMuMVABoth_h
#define ICHiggsTauTau_HiggsTauTau_HhhEMuMVABoth_h

// #include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace TMVA {
	class Reader;
}

namespace ic {

class TreeEvent;

class HhhEMuMVABoth : public ModuleBase {
 private:
 
 	CLASS_MEMBER(HhhEMuMVABoth, std::string, ditau_label)
 	CLASS_MEMBER(HhhEMuMVABoth, std::string, met_label)
 	CLASS_MEMBER(HhhEMuMVABoth, std::string, gf_mva_file_bdt)
  CLASS_MEMBER(HhhEMuMVABoth, std::string, gf_mva_file_bdtg)
	CLASS_MEMBER(HhhEMuMVABoth, std::string, mva_input_data)

 	TMVA::Reader *gf_reader_bdt_;
	TMVA::Reader *gf_reader_bdtg_;
  // TMVA::Reader *vbf_reader_;

  float fpzeta_;
 	float fpzetamiss_;
 	float fmet_;
 	float fpt_1_;
 	float fmt_ll_;
 	float fpt_2_;
 	float femu_dphi_;
	float	fprebjetbcsv_2_;
	float fprebjetbcsv_1_;
	float fprebjet_deta_;
	
	int		nprebjets_;

 public:
  HhhEMuMVABoth(std::string const& name);
  virtual ~HhhEMuMVABoth();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
