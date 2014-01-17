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
    if (fs_) h2dset = new Dynamic2DHistoSet(fs_->mkdir("quark_gluon_disc_2d"));
    hset->Create("gluon_chg_track_mult_trk", 100, 0, 100);
    hset->Create("quark_chg_track_mult_trk", 100, 0, 100);
    hset->Create("gluon_lin_rad_moment_trk", 100, 0, 1);
    hset->Create("quark_lin_rad_moment_trk", 100, 0, 1);
    hset->Create("gluon_chg_track_mult_notrk", 100, 0, 100);
    hset->Create("quark_chg_track_mult_notrk", 100, 0, 100);
    hset->Create("gluon_lin_rad_moment_notrk", 100, 0, 1);
    hset->Create("quark_lin_rad_moment_notrk", 100, 0, 1);
    hset->Create("quark_charged_frac", 100, 0, 1);
    hset->Create("gluon_charged_frac", 100, 0, 1);
    hset->Create("quark_charged_frac_trk", 100, 0, 1);
    hset->Create("gluon_charged_frac_trk", 100, 0, 1);
    hset->Create("quark_charged_frac_notrk", 100, 0, 1);
    hset->Create("gluon_charged_frac_notrk", 100, 0, 1);
    
    h2dset->Create("gluon_chg_track_vs_lin_rad_trk", 30, 0, 30, 40, 0, 0.4);
    h2dset->Create("quark_chg_track_vs_lin_rad_trk", 30, 0, 30, 40, 0, 0.4);
    h2dset->Create("gluon_chg_track_vs_lin_rad_notrk", 30, 0, 30, 40, 0, 0.4);
    h2dset->Create("quark_chg_track_vs_lin_rad_notrk", 30, 0, 30, 40, 0, 0.4);

    return 0;
  }

  int QuarkGluonDiscriminatorStudy::Execute(TreeEvent *event) {

    // EventInfo *info = event->GetPtr<EventInfo>("eventInfo");
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
    
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));

    for(unsigned i=0; i<jets.size(); i++)
    {
        double charged_frac = (jets[i]->charged_em_energy() + jets[i]->charged_had_energy())/(jets[i]->uncorrected_energy());
        if(jets[i]->parton_flavour() == 21)
        {
            if(fabs(jets[i]->eta()) < 2.4)
            {
                hset->Fill("gluon_chg_track_mult_trk", jets[i]->charged_multiplicity());
                hset->Fill("gluon_lin_rad_moment_trk", jets[i]->linear_radial_moment());
                hset->Fill("gluon_charged_frac_trk", charged_frac);
                h2dset->Fill("gluon_chg_track_vs_lin_rad_trk", jets[i]->charged_multiplicity_nopu(), jets[i]->linear_radial_moment());
            }
            else
            {
                hset->Fill("gluon_chg_track_mult_notrk", jets[i]->charged_multiplicity());
                hset->Fill("gluon_lin_rad_moment_notrk", jets[i]->linear_radial_moment());
                hset->Fill("gluon_charged_frac_notrk", charged_frac);
                h2dset->Fill("gluon_chg_track_vs_lin_rad_notrk", jets[i]->charged_multiplicity_nopu(), jets[i]->linear_radial_moment());
            }
            hset->Fill("gluon_charged_frac", charged_frac);
        }
        if(abs(jets[i]->parton_flavour()) < 4 && jets[i]->parton_flavour()!=0)
        {
            if(fabs(jets[i]->eta()) < 2.4)
            {
                hset->Fill("quark_chg_track_mult_trk", jets[i]->charged_multiplicity());
                hset->Fill("quark_lin_rad_moment_trk", jets[i]->linear_radial_moment());
                hset->Fill("quark_charged_frac_trk", charged_frac);
                h2dset->Fill("quark_chg_track_vs_lin_rad_trk", jets[i]->charged_multiplicity_nopu(), jets[i]->linear_radial_moment());
            }
            else
            {
                hset->Fill("quark_chg_track_mult_notrk", jets[i]->charged_multiplicity());
                hset->Fill("quark_lin_rad_moment_notrk", jets[i]->linear_radial_moment());
                hset->Fill("quark_charged_frac_notrk", charged_frac);
                h2dset->Fill("quark_chg_track_vs_lin_rad_notrk", jets[i]->charged_multiplicity_nopu(), jets[i]->linear_radial_moment());
            }
            hset->Fill("quark_charged_frac", charged_frac);
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
