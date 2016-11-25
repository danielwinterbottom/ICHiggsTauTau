#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MuonTandP.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"


namespace ic {

  MuonTandP::MuonTandP(std::string const& name) : ModuleBase(name) {
  }

  MuonTandP::~MuonTandP() {
    //delete fs_;
  }

  int MuonTandP::PreAnalysis() {
    fs_ = new fwlite::TFileService("MuonTandP_"+output_name_+".root");
    hists_ = new DynamicHistoSet(fs_->mkdir("/"));
    hists_->Create("tt", hist_bins_, min_mass_, max_mass_);
    hists_->Create("tf", hist_bins_, min_mass_, max_mass_);
    return 0;
  }

  int MuonTandP::Execute(TreeEvent *event) {
    // Make a copy of the electron pair collection
    std::vector<CompositeCandidate> & vec = 
      event->Get<std::vector<CompositeCandidate> >(input_name_);

    // Remove pairs outside mass range or with same charge
    //ic::erase_if(vec,!boost::bind(PairMassInRange, _1, min_mass_, max_mass_));
    //ic::erase_if(vec,!boost::bind(PairOppCharge, _1));

    for (unsigned i = 0; i < vec.size(); ++i) {
      Muon *muon1 = dynamic_cast<Muon *>(vec[i].At(0));
      Muon *muon2 = dynamic_cast<Muon *>(vec[i].At(1));
      bool muon1_tag = id_predicate_(muon1) && iso_predicate_(muon1);
      bool muon2_tag = id_predicate_(muon2) && iso_predicate_(muon2);

      if (muon1_tag && muon2_tag) {
        hists_->Fill("tt",vec[i].vector().M());
      }
      if ((muon1_tag && !muon2_tag) || (!muon1_tag && muon2_tag)) {
        hists_->Fill("tf", vec[i].vector().M());
      }
    }
    return 0;
  }

  int MuonTandP::PostAnalysis() {
    return 0;
  }

  void MuonTandP::PrintInfo() {
    ;
  }
}
