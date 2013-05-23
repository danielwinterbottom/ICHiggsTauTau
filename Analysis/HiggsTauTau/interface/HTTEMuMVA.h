#ifndef ICHiggsTauTau_HiggsTauTau_HTTEmuMVA_h
#define ICHiggsTauTau_HiggsTauTau_HTTEmuMVA_h

// #include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace TMVA {
	class Reader;
}

namespace ic {

class TreeEvent;

class HTTEmuMVA : public ModuleBase {
 private:
 
 	CLASS_MEMBER(HTTEmuMVA, std::string, ditau_label)
 	CLASS_MEMBER(HTTEmuMVA, std::string, met_label)
 	CLASS_MEMBER(HTTEmuMVA, std::string, gf_mva_file)
 	CLASS_MEMBER(HTTEmuMVA, std::string, vbf_mva_file)

 	TMVA::Reader *gf_reader_;
 	TMVA::Reader *vbf_reader_;

 	float pzetavis_;
 	float pzetamiss_;
 	float dphi_;
 	float mvamet_;
 	float csv_;
 	float el_dxy_;
 	float mu_dxy_;

 public:
  HTTEmuMVA(std::string const& name);
  virtual ~HTTEmuMVA();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
