#ifndef ICHiggsTauTau_HiggsTauTau_JetEnergyUncertainty_h
#define ICHiggsTauTau_HiggsTauTau_JetEnergyUncertainty_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include <string>
#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"

namespace ic {

template <class T>
class JetEnergyUncertainty : public ModuleBase {
 private:
  CLASS_MEMBER(JetEnergyUncertainty, unsigned, jes_shift_mode)
  CLASS_MEMBER(JetEnergyUncertainty, std::string, input_label)
  CLASS_MEMBER(JetEnergyUncertainty, std::string, uncert_file)
  CLASS_MEMBER(JetEnergyUncertainty, std::string, uncert_set)
  CLASS_MEMBER(JetEnergyUncertainty, bool, sum_uncerts)
  CLASS_MEMBER(JetEnergyUncertainty, std::vector<std::string>, uncert_sets)
  CLASS_MEMBER(JetEnergyUncertainty, std::vector<double>, correlations)
  CLASS_MEMBER(JetEnergyUncertainty, unsigned, jes_corr_mode)
  CLASS_MEMBER(JetEnergyUncertainty, bool, EENoiseFix)
  JetCorrectionUncertainty *uncert_;
  JetCorrectorParameters *params_;
  std::vector<JetCorrectionUncertainty*> uncerts_;

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
  sum_uncerts_ = false;
  EENoiseFix_ = false;
  jes_corr_mode_ = 0;
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
  
  if(sum_uncerts_){
    for (unsigned j=0; j<uncert_sets_.size(); ++j){
      std::cout << "uncert sets[j]: " << uncert_sets_[j] << std::endl;
      JetCorrectorParameters params = JetCorrectorParameters(uncert_file_, uncert_sets_[j]); 
      uncerts_.push_back(new JetCorrectionUncertainty(params));
    }
  }
  
  return 0;
}

template <class T>
int JetEnergyUncertainty<T>::Execute(TreeEvent *event) {
  std::vector<T *> & vec = event->GetPtrVec<T>(input_label_);
  if(!sum_uncerts_){
    ROOT::Math::PxPyPzEVector before(0.,0.,0.,0.);
    ROOT::Math::PxPyPzEVector after(0.,0.,0.,0.);  
    for (unsigned i = 0; i < vec.size(); ++i) {
      double pt = vec[i]->pt() * (vec[i]->uncorrected_energy()/vec[i]->energy());
      bool skipJet = (EENoiseFix_ && pt<50 && fabs(vec[i]->eta())>2.65 && fabs(vec[i]->eta())<3.139);  
      if(!skipJet) before+=vec[i]->vector();
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
      if(!skipJet) after+=vec[i]->vector();

    }
    event->Add("jes_shift", after-before);
  } else if (sum_uncerts_){
    ROOT::Math::PxPyPzEVector before(0.,0.,0.,0.);
    ROOT::Math::PxPyPzEVector after(0.,0.,0.,0.);
    for (unsigned i = 0; i < vec.size(); ++i) {
      double pt = vec[i]->pt() * (vec[i]->uncorrected_energy()/vec[i]->energy());
      bool skipJet = (EENoiseFix_ && pt<50 && fabs(vec[i]->eta())>2.65 && fabs(vec[i]->eta())<3.139); 
      if(!skipJet) before+=vec[i]->vector();  
      double shift=0;
      double factor = 1.; //for correlations
      for (unsigned j=0; j<uncerts_.size(); ++j){
        uncerts_[j]->setJetPt(vec[i]->pt());
        uncerts_[j]->setJetEta(vec[i]->eta());
        if (jes_corr_mode_ == 1) {
          factor = correlations_[j];
        } else if (jes_corr_mode_ == 2){
            factor = 1. - correlations_[j];
        }

        if (jes_shift_mode_ == 1) {
          shift = sqrt(shift*shift + factor*pow(uncerts_[j]->getUncertainty(false),2)); //down
        }
        if (jes_shift_mode_ == 2) {
          shift = sqrt(shift*shift + factor*pow(uncerts_[j]->getUncertainty(true),2)); //up
        }
      }
      if (jes_shift_mode_ == 1) {
        vec[i]->set_vector(vec[i]->vector() * (1.0-shift));
      }
      if (jes_shift_mode_ == 2) {
        vec[i]->set_vector(vec[i]->vector() * (1.0+shift));
      }
      if(!skipJet) after+=vec[i]->vector();
    }
    event->Add("jes_shift", after-before);
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
