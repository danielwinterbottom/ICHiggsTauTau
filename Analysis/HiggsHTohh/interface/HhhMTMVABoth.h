#ifndef ICHiggsTauTau_HiggsTauTau_HhhMTMVABoth_h
#define ICHiggsTauTau_HiggsTauTau_HhhMTMVABoth_h

// #include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace TMVA {
	class Reader;
}

namespace ic {

class TreeEvent;

class HhhMTMVABoth : public ModuleBase {
 private:
 
 	CLASS_MEMBER(HhhMTMVABoth, std::string, ditau_label)
 	CLASS_MEMBER(HhhMTMVABoth, std::string, met_label)
 	CLASS_MEMBER(HhhMTMVABoth, std::string, gf_mva_file_bdt)
	CLASS_MEMBER(HhhMTMVABoth, std::string, mva_input_data)

 	TMVA::Reader *gf_reader_bdt_;
  // TMVA::Reader *vbf_reader_;

 	float fmet_;
 	float fpt_1_;
 	float fmt_1_;
 	float fpt_2_;
	float femu_dphi_;
	float fpzeta_;
	float fprebjetbcsv_1_;
	float fprebjetbcsv_2_;
	
	int		nprebjets_;

 public:
  HhhMTMVABoth(std::string const& name);
  virtual ~HhhMTMVABoth();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
