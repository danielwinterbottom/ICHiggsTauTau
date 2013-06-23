#ifndef ICHiggsTauTau_HiggsTauTau_JetEnergyUncertainty_h
#define ICHiggsTauTau_HiggsTauTau_JetEnergyUncertainty_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include <string>
#include "boost/bind.hpp"

namespace ic {

template <class T>
class JetEnergyUncertainty : public ModuleBase {
 private:
  CLASS_MEMBER(JetEnergyUncertainty, unsigned, jes_shift_mode)
  CLASS_MEMBER(JetEnergyUncertainty, std::string, input_label)
  CLASS_MEMBER(JetEnergyUncertainty, std::string, uncert_file)
  CLASS_MEMBER(JetEnergyUncertainty, std::string, uncert_set)
  JetCorrectionUncertainty *uncert_;
  JetCorrectorParameters *params_;

 public:
  JetEnergyUncertainty(std::string const& name);
  virtual ~JetEnergyUncertainty();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

template <class T>
JetEnergyUncertainty<T>::JetEnergyUncertainty(std::string const& name) : ModuleBase(name) {
  input_label_ = "pfJetsPFlow";
}

template <class T>
JetEnergyUncertainty<T>::~JetEnergyUncertainty() {
  // delete uncert_;
  // delete params_;
}

template <class T>
int JetEnergyUncertainty<T>::PreAnalysis() {
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "PreAnalysis Info for JetEnergyUncertainty" << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  std::cout << boost::format(param_fmt()) % "input_label"         % input_label_;
  std::cout << boost::format(param_fmt()) % "jes_shift_mode"      % jes_shift_mode_;
  std::cout << boost::format(param_fmt()) % "uncert_file"         % uncert_file_;
  std::cout << boost::format(param_fmt()) % "uncert_set"          % uncert_set_;
  params_ = new JetCorrectorParameters(uncert_file_, uncert_set_);
  uncert_ = new JetCorrectionUncertainty(*params_);
  return 0;
}

template <class T>
int JetEnergyUncertainty<T>::Execute(TreeEvent *event) {
  std::vector<T *> & vec = event->GetPtrVec<T>(input_label_);
  for (unsigned i = 0; i < vec.size(); ++i) {
    uncert_->setJetPt(vec[i]->pt());
    uncert_->setJetEta(vec[i]->eta());
    if (jes_shift_mode_ == 1) {
      double shift = uncert_->getUncertainty(false); //down
      vec[i]->set_vector(vec[i]->vector() * (1.0-shift));
    }
    if (jes_shift_mode_ == 2) {
      double shift = uncert_->getUncertainty(true); //up
      vec[i]->set_vector(vec[i]->vector() * (1.0+shift));
    }
  }
  return 0;
}

template <class T>
int JetEnergyUncertainty<T>::PostAnalysis() {
  return 0;
}

template <class T>
void JetEnergyUncertainty<T>::PrintInfo() {
  ;
}

}

#endif
