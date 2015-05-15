#ifndef ICHiggsTauTau_HiggsTauTau_HhhMTMVACategory_h
#define ICHiggsTauTau_HiggsTauTau_HhhMTMVACategory_h

// #include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace TMVA {
	class Reader;
}

namespace ic {

class TreeEvent;

class HhhMTMVACategory : public ModuleBase {
 private:
 
 	CLASS_MEMBER(HhhMTMVACategory, std::string, ditau_label)
 	CLASS_MEMBER(HhhMTMVACategory, std::string, met_label)
 	CLASS_MEMBER(HhhMTMVACategory, std::string, jets_label)
 	CLASS_MEMBER(HhhMTMVACategory, std::string, gf_mva_file)
  CLASS_MEMBER(HhhMTMVACategory, std::string, gf_mva_file_2)

 	TMVA::Reader *gf_reader_;
	TMVA::Reader *gf_reader_2_;
  // TMVA::Reader *vbf_reader_;

 	float fpzeta_;
 	float fmet_;
 	float fpt_1_;
 	float fpt_2_;
	float fmt_1_;
 	float femu_dphi_;
	float fprebjetbcsv_1_;
	float fprebjetbcsv_2_;
	float fmt_2_;
	float fprebjetpt_1_;
	float fprebjetpt_2_;
	float fprebjetpt_bb_;
	float fprebjet_dR_;
	float fmutau_dR_;
	float fpt_tt_;
	int nprebjets_;

 public:
  HhhMTMVACategory(std::string const& name);
  virtual ~HhhMTMVACategory();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
