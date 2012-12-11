#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/QuarkGluonDiscriminatorStudy.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "Math/SMatrix.h"
#include "Math/SVector.h"

namespace ic {

  QuarkGluonDiscriminatorStudy::QuarkGluonDiscriminatorStudy(std::string const& name) : ModuleBase(name) {

    fs_ = NULL;
    is_dyjets_ = false;

}

  QuarkGluonDiscriminatorStudy::~QuarkGluonDiscriminatorStudy() {
    ;
  }

  int QuarkGluonDiscriminatorStudy::PreAnalysis() {

    if (fs_) hset = new DynamicHistoSet(fs_->mkdir("quark_gluon_disc"));
    hset->Create("gluon_chg_track_mult", 100, 0, 100);
    hset->Create("quark_chg_track_mult", 100, 0, 100);
    hset->Create("gluon_lin_rad_moment", 100, 0, 1);
    hset->Create("quark_lin_rad_moment", 100, 0, 1);

    return 0;
  }

  int QuarkGluonDiscriminatorStudy::Execute(TreeEvent *event) {

    EventInfo *info = event->GetPtr<EventInfo>("eventInfo");
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow");

    for(unsigned i=0; i<jets.size(); i++)
    {
        if(jets[i]->parton_flavour() == 21)
        {
            hset->Fill("gluon_chg_track_mult", jets[i]->charged_multiplicity());
            hset->Fill("gluon_lin_rad_moment", jets[i]->linear_radial_moment());
        }
        if(fabs(jets[i]->parton_flavour()) < 7)
        {
            hset->Fill("quark_chg_track_mult", jets[i]->charged_multiplicity());
            hset->Fill("quark_lin_rad_moment", jets[i]->linear_radial_moment());
        }
    }

    return 0;
  }
  int QuarkGluonDiscriminatorStudy::PostAnalysis() {

    return 0;
  }

  void QuarkGluonDiscriminatorStudy::PrintInfo() {
    ;
  }
}
