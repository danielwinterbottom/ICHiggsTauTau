#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTL1MetCorrector.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <boost/functional/hash.hpp>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"

namespace ic {

  HTTL1MetCorrector::HTTL1MetCorrector(std::string const& name) : ModuleBase(name) {
    l1_met_label_ = "l1extraMET";
    uncorrected_calo_met_label_ = "metNoHF";
    corrected_calo_met_label_ = "metNoHFCorrected";
  }

  HTTL1MetCorrector::~HTTL1MetCorrector() {
    ;
  }

  int HTTL1MetCorrector::PreAnalysis() {
    std::string param_fmt = "%-25s %-40s\n";
    std::cout << "HTTL1MetCorrector------------------------------" << std::endl;
    std::cout << boost::format(param_fmt) % "l1_met_label" % l1_met_label_;
    std::cout << boost::format(param_fmt) % "raw_calomet_label" % uncorrected_calo_met_label_;
    std::cout << boost::format(param_fmt) % "corr_calomet_label" % corrected_calo_met_label_;
    return 0;
  }

  int HTTL1MetCorrector::Execute(TreeEvent *event) {
    std::vector<Candidate *> & v_l1met = event->GetPtrVec<Candidate>(l1_met_label_);
    Candidate *l1met = v_l1met.at(0);
    Met const* raw_calo = event->GetPtr<Met>(uncorrected_calo_met_label_);
    Met const* corr_calo = event->GetPtr<Met>(corrected_calo_met_label_);
    //std::cout << "pt: " << corr_calo->pt() / raw_calo->pt() << std::endl;
    //std::cout << "ex: " << corr_calo->vector().px() / raw_calo->vector().px() << std::endl;
    //std::cout << "ey: " << corr_calo->vector().py() / raw_calo->vector().py() << std::endl;
    return 0;
  }
  int HTTL1MetCorrector::PostAnalysis() {
    return 0;
  }

  void HTTL1MetCorrector::PrintInfo() {
    ;
  }
}

