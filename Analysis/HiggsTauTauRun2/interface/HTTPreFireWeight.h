#ifndef ICHiggsTauTau_HiggsTauTauRun2_HTTPreFireWeight_h
#define ICHiggsTauTau_HiggsTauTauRun2_HTTPreFireWeight_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"
#include <string>
#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "TH2D.h"

namespace ic {

template <class T>
class HTTPreFireWeight : public ModuleBase {
 private:
  CLASS_MEMBER(HTTPreFireWeight, std::string, input_label)
  CLASS_MEMBER(HTTPreFireWeight, TH2F*, prefire_hist)

 public:
  HTTPreFireWeight(std::string const& name);
  virtual ~HTTPreFireWeight();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

template <class T>
HTTPreFireWeight<T>::HTTPreFireWeight(std::string const& name) : ModuleBase(name) {
  input_label_ = "ak4PFJetsCHS";
  prefire_hist_ = nullptr;
}

template <class T>
HTTPreFireWeight<T>::~HTTPreFireWeight() {
}

template <class T>
int HTTPreFireWeight<T>::PreAnalysis() {
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "PreAnalysis Info for HTTPreFireWeight" << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  std::cout << boost::format(param_fmt()) % "input_label"         % input_label_;
  
  return 0;
}

template <class T>
int HTTPreFireWeight<T>::Execute(TreeEvent *event) {
  std::vector<T *> & vec = event->GetPtrVec<T>(input_label_);
  double wt = 1., wt_up = 1., wt_down = 1.;
  for (unsigned i = 0; i < vec.size(); ++i) {
    double jetpt = vec[i]->pt();
    double jeteta = vec[i]->eta();
      if( jetpt < 20.) continue;
      if( fabs(jeteta <2.)) continue;
      if( fabs(jeteta >3.)) continue;

      int bin = prefire_hist_->FindBin(jeteta,jetpt);

      double p = prefire_hist_->GetBinContent(bin);
      double e = std::max(0.2*p, prefire_hist_->GetBinError(bin));

      wt*=(1.-p);
      wt_up*=(1.-std::min(p+e,1.));
      wt_down*=(1.-std::max(p-e,0.));
  }
  event->Add("wt_prefire", wt);
  event->Add("wt_prefire_up", wt_up/wt);
  event->Add("wt_prefire_down", wt_down/wt); 
  return 0;
}

template <class T>
int HTTPreFireWeight<T>::PostAnalysis() {
  return 0;
}

template <class T>
void HTTPreFireWeight<T>::PrintInfo() {
  ;
}

}

#endif
