#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTEMuExtras.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"

namespace ic {

  HTTEMuExtras::HTTEMuExtras(std::string const& name) : ModuleBase(name) {
    electron_label_ = "electrons";
    apply_muon_dr_cut_ = true;
    correct_pf_isolation_ = true;
  }

  HTTEMuExtras::~HTTEMuExtras() {
    ;
  }

  int HTTEMuExtras::PreAnalysis() {
    return 0;
  }

  int HTTEMuExtras::Execute(TreeEvent *event) {
    std::vector<Electron *> & elecs = event->GetPtrVec<Electron>(electron_label_);

    if (apply_muon_dr_cut_) {
      ic::erase_if(elecs, [] (Electron * ele) {
        if (ele->HasIdIso("matchedRecoMuon")) {
          return ele->GetIdIso("matchedRecoMuon") > 0.5;
        } else {
          return false;
        }
      });
    }

    if (correct_pf_isolation_) {
      for (auto ele : elecs) {
        if (ele->HasIdIso("trackInIsoSum")) {
          float current_iso = ele->dr04_pfiso_charged_all();
          float sub_iso = ele->GetIdIso("trackInIsoSum");
          if (current_iso >= sub_iso) ele->set_dr04_pfiso_charged_all(current_iso - sub_iso);
        }
      }
    }
    
    return 0;
  }



  int HTTEMuExtras::PostAnalysis() {
    return 0;
  }

  void HTTEMuExtras::PrintInfo() {
    ;
  }

}
