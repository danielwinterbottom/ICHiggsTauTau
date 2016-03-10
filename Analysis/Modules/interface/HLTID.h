#ifndef ICHiggsTauTau_Module_HLTID_h
#define ICHiggsTauTau_Module_HLTID_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include <string>
#include <cmath>

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TLorentzVector.h"


namespace ic {

  class HLTID : public ModuleBase {
  private:
    
    std::string HLT1_label_;
    std::string HLT2_label_;
    std::string HLT3_label_;
    std::string HLT4_label_;
    std::string HLT5_label_;
    std::string HLT6_label_;
    std::string HLT7_label_;
    std::string HLT8_label_;
    std::string HLT9_label_;
    std::string HLT10_label_;
    std::string HLT11_label_;
    std::string HLT12_label_;
    std::string HLT13_label_;
    std::string HLT14_label_;
    std::string HLT15_label_;
    std::string genParticles_label_; 
 

    
    std::string output_name_;
    std::string output_name2_ = "HLT_";

    TFile *fOut;
    
    TH1D  *h_HLTTriggerID;
    TH1D  *h_HLTTriggerID_em;
    TH1D  *h_HLTTriggerID_et;
    TH1D  *h_HLTTriggerID_mt;
    TH1D  *h_HLTTriggerID_tt;
    
    unsigned int n_genParticles_;


  public:

    HLTID(std::string const& name, std::string output_name);
    virtual ~HLTID();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
  };

}//namespace


#endif
