#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTL1MetCorrector.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "boost/format.hpp"
#include "TMath.h"

namespace ic {

  HTTL1MetCorrector::HTTL1MetCorrector(std::string const& name) : ModuleBase(name) {
    l1_met_label_               = "l1extraMET";
    uncorrected_calo_met_label_ = "metNoHF";
    corrected_calo_met_label_   = "metNoHFCorrected";
    l1_met_mode_ = 0;
  }

  HTTL1MetCorrector::~HTTL1MetCorrector() {
    ;
  }

  int HTTL1MetCorrector::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTL1MetCorrector" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
    std::cout << boost::format(param_fmt()) % "l1_met_label"        % l1_met_label_;
    std::cout << boost::format(param_fmt()) % "l1_met_mode"         % l1_met_mode_;
    std::cout << boost::format(param_fmt()) % "raw_calomet_label"   % uncorrected_calo_met_label_;
    std::cout << boost::format(param_fmt()) % "corr_calomet_label"  % corrected_calo_met_label_;
    return 0;
  }

  int HTTL1MetCorrector::Execute(TreeEvent *event) {
    std::vector<Candidate *> & v_l1met = event->GetPtrVec<Candidate>(l1_met_label_);
    Candidate *l1met = v_l1met.at(0);



    double raw_l1 = l1met->pt();
    double raw_calo = event->GetPtr<Met>(uncorrected_calo_met_label_)->pt();
    double corr_calo = event->GetPtr<Met>(corrected_calo_met_label_)->pt(); 

    double R = 0.9322;
    double K = 0.6693;
    double H = (0.8828 - 1.0) + (0.0499*TMath::Log(raw_calo));
    //double R =  0.9389 - ( 0.00128 * sqrt(raw_calo) );
    //double H = -0.0623 + ( 0.02164 * sqrt(raw_calo) );
    //double K =  0.6729 - ( 0.00024 *      raw_calo  );

    double corr_l1 = raw_l1 * (corr_calo/raw_calo) * R;
    corr_l1 += H * ( (raw_l1*(corr_calo/raw_calo)*R) - (K*raw_calo) );

    double l1_ratio = corr_l1 / raw_l1;
    
    if (l1_met_mode_ == 1) l1_ratio *= 0.95;
    if (l1_met_mode_ == 2) l1_ratio *= 1.05;

    l1met->set_pt(l1met->pt() * l1_ratio);
    l1met->set_energy(l1met->energy() * l1_ratio);
    return 0;
  }
  int HTTL1MetCorrector::PostAnalysis() {
    return 0;
  }

  void HTTL1MetCorrector::PrintInfo() {
    ;
  }
}

