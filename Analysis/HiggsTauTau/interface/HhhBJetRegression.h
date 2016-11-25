#ifndef ICHiggsTauTau_HiggsTauTau_HhhBJetRegression_h
#define ICHiggsTauTau_HiggsTauTau_HhhBJetRegression_h

// #include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace TMVA {
	class Reader;
}

namespace ic {

class TreeEvent;

class HhhBJetRegression : public ModuleBase {
 private:
 
 	CLASS_MEMBER(HhhBJetRegression, std::string, jets_label)
 	CLASS_MEMBER(HhhBJetRegression, std::string, regression_mva_file)

 	TMVA::Reader *regression_reader_;

 	float prebjet_bcsv;
 	float prebjet_pt;
 	float prebjet_eta;
 	float prebjet_chf;
 	float prebjet_phf;
 	float prebjet_nhf;
 	float prebjet_elf;
 	float prebjet_muf;
 	float prebjet_genPt;

 public:
  HhhBJetRegression(std::string const& name);
  virtual ~HhhBJetRegression();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
