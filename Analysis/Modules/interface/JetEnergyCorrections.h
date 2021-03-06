#ifndef ICHiggsTauTau_Module_JetEnergyCorrections_h
#define ICHiggsTauTau_Module_JetEnergyCorrections_h

#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include <string>
#include "boost/bind.hpp"

namespace ic {

template <class T>
class JetEnergyCorrections : public ModuleBase {
 private:
  CLASS_MEMBER(JetEnergyCorrections, bool, is_data)
  CLASS_MEMBER(JetEnergyCorrections, std::string, input_label)
  CLASS_MEMBER(JetEnergyCorrections, std::string, l1_file)
  CLASS_MEMBER(JetEnergyCorrections, std::string, l2_file)
  CLASS_MEMBER(JetEnergyCorrections, std::string, l3_file)
  CLASS_MEMBER(JetEnergyCorrections, std::string, res_file)
  CLASS_MEMBER(JetEnergyCorrections, bool, use_new_mode)
  JetCorrectorParameters *ResJetPar;
  JetCorrectorParameters *L3JetPar;
  JetCorrectorParameters *L2JetPar;
  JetCorrectorParameters *L1JetPar;
  FactorizedJetCorrector *JetCorrector;

 public:
  JetEnergyCorrections(std::string const& name);
  virtual ~JetEnergyCorrections();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

template <class T>
JetEnergyCorrections<T>::JetEnergyCorrections(std::string const &name)
    : ModuleBase(name) {
  is_data_ = true;
  input_label_ = "pfJetsPFlow";
  use_new_mode_ = false;
}

template <class T>
JetEnergyCorrections<T>::~JetEnergyCorrections() {}

template <class T>
int JetEnergyCorrections<T>::PreAnalysis() {
  std::cout << "-----------------------------------------\n";
  std::cout << "PreAnalysis Info for JetEnergyCorrections\n";
  std::cout << "-----------------------------------------\n";
  std::cout << "Re-applying corrections to input: " << input_label_ << "\n";
  std::cout << "L1 Source: " << l1_file_ << std::endl;
  std::cout << "L2 Source: " << l2_file_ << std::endl;
  std::cout << "L3 Source: " << l3_file_ << std::endl;
  if (is_data_) std::cout << "Residual Source: " << res_file_ << std::endl;
  if (is_data_) ResJetPar = new JetCorrectorParameters(res_file_);
  L3JetPar  = new JetCorrectorParameters(l3_file_);
  L2JetPar  = new JetCorrectorParameters(l2_file_);
  L1JetPar  = new JetCorrectorParameters(l1_file_);
  std::vector<JetCorrectorParameters> vPar;
  vPar.push_back(*L1JetPar);
  vPar.push_back(*L2JetPar);
  vPar.push_back(*L3JetPar);
  if (is_data_) vPar.push_back(*ResJetPar);
  JetCorrector = new FactorizedJetCorrector(vPar);
  return 0;
}

template <class T>
int JetEnergyCorrections<T>::Execute(TreeEvent *event) {
  EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
  std::vector<T *> & vec = event->GetPtrVec<T>(input_label_);

  for (unsigned i = 0; i < vec.size(); ++i) {
    T *jet = vec[i];
    double uncorr_factor = use_new_mode_
                               ? (jet->uncorrected_energy() / jet->energy())
                               : jet->GetJecFactor("Uncorrected");
    double uncorr_pt = uncorr_factor * jet->pt();
    double uncorr_energy = uncorr_factor * jet->energy();
    JetCorrector->setJetEta(jet->eta());
    JetCorrector->setJetPt(uncorr_pt);
    JetCorrector->setJetA(jet->jet_area());
    JetCorrector->setRho(eventInfo->jet_rho());
    // std::cout << "--Jet " << i << ": " << vec[i]->vector() << std::endl;
    // std::cout << "-L1FastJet: " << vec[i]->GetJecFactor("L1FastJet") << std::endl;
    // std::cout << "-L2Relative: " << vec[i]->GetJecFactor("L2Relative") << std::endl;
    // std::cout << "-L3Absolute: " << vec[i]->GetJecFactor("L3Absolute") << std::endl;
    // if (is_data_) std::cout << "-L2L3Residual: " << vec[i]->GetJecFactor("L2L3Residual") << std::endl;
    //double full_corr = JetCorrector->getCorrection();
    std::vector<float> factors = JetCorrector->getSubCorrections();
    double full_corr = factors.back();
    double new_pt = uncorr_pt * full_corr;
    double new_energy = uncorr_energy * full_corr;
    jet->set_pt(new_pt);
    jet->set_energy(new_energy);
    if (use_new_mode_) {
      jet->SetJecFactor("L1FastJet", factors[0]);
      jet->SetJecFactor("L2Relative", factors[1] / factors[0]);
      jet->SetJecFactor("L3Absolute", factors[2] / factors[1]);
      if (is_data_)
        vec[i]->SetJecFactor("L2L3Residual", factors[3] / factors[2]);
    } else {
      jet->SetJecFactor("L1FastJet", factors[0] / full_corr);
      jet->SetJecFactor("L2Relative", factors[1] / full_corr);
      jet->SetJecFactor("L3Absolute", factors[2] / full_corr);
      if (is_data_) vec[i]->SetJecFactor("L2L3Residual", factors[3] / full_corr);
    }
    // std::cout << "-Correction: " << full_corr << std::endl;
    // std::cout << "--Corrected Jet " << i << ": " << vec[i]->vector() << std::endl;
    // std::cout << "-L1FastJet: " << vec[i]->GetJecFactor("L1FastJet") << std::endl;
    // std::cout << "-L2Relative: " << vec[i]->GetJecFactor("L2Relative") << std::endl;
    // std::cout << "-L3Absolute: " << vec[i]->GetJecFactor("L3Absolute") << std::endl;
    // if (is_data_) std::cout << "-L2L3Residual: " << vec[i]->GetJecFactor("L2L3Residual") << std::endl;
  }
  return 0;
}

template <class T>
int JetEnergyCorrections<T>::PostAnalysis() {
  return 0;
}

template <class T>
void JetEnergyCorrections<T>::PrintInfo() {
  ;
}

}

#endif
