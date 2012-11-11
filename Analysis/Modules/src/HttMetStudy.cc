#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttMetStudy.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

#include "TMVA/Reader.h"
#include "TVector3.h"

namespace ic {

  HttMetStudy::HttMetStudy(std::string const& name) : ModuleBase(name) {
    dilepton_label_ = "emtauCandidates";
    met_label_ = "pfMVAMet";
    mode_ = 0;
    fs_ = NULL;

    metstudy_plots_.resize(4);

    total_yields_.resize(4);
    justpair_yields_.resize(4);
    hists_.resize(4);

  }

  HttMetStudy::~HttMetStudy() {
    ;
  }

  int HttMetStudy::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HTT MetStudy" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    if (fs_) {
      std::cout << "Mode: " << mode_ << std::endl;
      std::cout << "Dilepton Label: " << dilepton_label_ << std::endl;
      std::cout << "MET Label: " << met_label_ << std::endl;

      metstudy_plots_[0] = new HttPlots(fs_->mkdir("metstudy_os_sel"));
      metstudy_plots_[1] = new HttPlots(fs_->mkdir("metstudy_ss_sel"));
      metstudy_plots_[2] = new HttPlots(fs_->mkdir("metstudy_os_con"));
      metstudy_plots_[3] = new HttPlots(fs_->mkdir("metstudy_ss_con"));

      hists_[0] = new DynamicHistoSet(fs_->mkdir("metstudy_os_sel_extra"));
      hists_[1] = new DynamicHistoSet(fs_->mkdir("metstudy_ss_sel_extra"));
      hists_[2] = new DynamicHistoSet(fs_->mkdir("metstudy_os_con_extra"));
      hists_[3] = new DynamicHistoSet(fs_->mkdir("metstudy_ss_con_extra"));

      for (unsigned i = 0; i < hists_.size(); ++i) {
        hists_[i]->Create("n_mva_elecs", 5, -0.5, 4.5);
        hists_[i]->Create("n_mva_muons", 5, -0.5, 4.5);
        hists_[i]->Create("n_mva_taus", 5, -0.5, 4.5);
      }
    }
    return 0;
  }

  int HttMetStudy::Execute(TreeEvent *event) {

    // Get the stuff we need from the event
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    double wt = eventInfo->total_weight();
    std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(dilepton_label_);
    std::vector<Vertex *> const& vertices = event->GetPtrVec<Vertex>("vertices");
    Met const* pfMetMVA = event->GetPtr<Met>(met_label_);
    unsigned sel_mode = event->Get<unsigned>("sel_mode");

    std::vector<Electron *> & mva_elecs = event->GetPtrVec<Electron>("mva_elecs","electrons");
    std::vector<Electron *> mva_elecs_sel;
    std::vector<Muon *> & mva_muons = event->GetPtrVec<Muon>("mva_muons","muonsPFlow");
    std::vector<Muon *> mva_muons_sel;
    std::vector<Tau *> & mva_taus = event->GetPtrVec<Tau>("mva_taus","taus");
    std::vector<Tau *> mva_taus_sel;

    for (unsigned i = 0; i < mva_elecs.size(); ++i) {
      Electron * elec = mva_elecs[i];
      bool pass_id = (
          elec->pt() > 9.5 && fabs(elec->eta()) < 2.5
          && elec->gsf_tk_nhits() == 0
          && (elec->dr03_tk_sum_pt()/elec->pt()) < 0.2
          && (  (
                  fabs(elec->eta()) < 1.442
                  && fabs(elec->deta_sc_tk_at_vtx()) < 0.007
                  && fabs(elec->dphi_sc_tk_at_vtx()) < 0.8
                  && elec->sigma_IetaIeta() < 0.01
                  && elec->hadronic_over_em() < 0.15
                  && fabs(1./elec->sc_energy() - 1./elec->vector().P()) < 0.05
                ) || (
                  fabs(elec->eta()) > 1.566
                  && fabs(elec->deta_sc_tk_at_vtx()) < 0.007
                  && fabs(elec->dphi_sc_tk_at_vtx()) < 0.03
                  && elec->sigma_IetaIeta() < 0.03
                  && elec->hadronic_over_em() < 0.10
                  && fabs(1./elec->sc_energy() - 1./elec->vector().P()) < 0.05  
                )
            )
        );
      if (pass_id) mva_elecs_sel.push_back(elec);
    }

    for (unsigned i = 0; i < mva_muons.size(); ++i) {
      Muon * muon = mva_muons[i];
      bool pass_id = (
          muon->pt() > 9.5 && fabs(muon->eta()) < 2.5
          && muon->is_tracker()
          && muon->is_global()
          && muon->it_pixel_hits() > 0
          && muon->gt_normalized_chi2() < 10
          && muon->matched_stations() > 0
          && muon->it_tracker_hits() > 5
          && muon->gt_valid_muon_hits() > 0
          && (muon->dr03_tk_sum_pt() / muon->pt()) < 0.2
          && muon->dxy_vertex() < 2.0
        );
      if (pass_id) mva_muons_sel.push_back(muon);
    }

    for (unsigned i = 0; i < mva_taus.size(); ++i) {
      Tau * tau = mva_taus[i];
      bool pass_id = (
          tau->pt() > 19.0 && fabs(tau->eta()) < 2.3
          && tau->GetTauID("decayModeFinding") > 0.5
          //&& tau->GetTauID("byVLooseIsolation") > 0.5
          && tau->GetTauID("byIsolationMVAraw") > 0.7
          && tau->GetTauID("againstElectronLoose") > 0.5
          && tau->GetTauID("againstMuonLoose") > 0.5
        );
      if (pass_id) mva_taus_sel.push_back(tau);
    }

    ic::erase_if(mva_taus_sel, !boost::bind(MinDRToCollection<Electron*>, _1, mva_elecs_sel, 0.5));
    ic::erase_if(mva_taus_sel, !boost::bind(MinDRToCollection<Muon*>, _1, mva_muons_sel, 0.5));


    hists_[sel_mode]->Fill("n_mva_elecs", mva_elecs_sel.size(), wt);
    hists_[sel_mode]->Fill("n_mva_muons", mva_muons_sel.size(), wt);
    hists_[sel_mode]->Fill("n_mva_taus", mva_taus_sel.size(), wt);

    bool exact_overlap = false;
    if (mode_ == 0) {
      if (mva_elecs_sel.size() == 1 && mva_muons_sel.size() == 0 && mva_taus_sel.size() == 1) {
        if ( (mva_elecs_sel[0]->id() == dilepton[0]->GetCandidate("lepton1")->id()) &&
             (mva_taus_sel[0]->id() == dilepton[0]->GetCandidate("lepton2")->id()) ) exact_overlap = true;
      }
    } else if (mode_ == 1) {
      if (mva_elecs_sel.size() == 0 && mva_muons_sel.size() == 1 && mva_taus_sel.size() == 1) {
        if ( (mva_muons_sel[0]->id() == dilepton[0]->GetCandidate("lepton1")->id()) &&
             (mva_taus_sel[0]->id() == dilepton[0]->GetCandidate("lepton2")->id()) ) exact_overlap = true;
      }
    } else if (mode_ == 2) {
      if (mva_elecs_sel.size() == 1 && mva_muons_sel.size() == 1 && mva_taus_sel.size() == 0) {
        if ( (mva_elecs_sel[0]->id() == dilepton[0]->GetCandidate("lepton1")->id()) &&
             (mva_muons_sel[0]->id() == dilepton[0]->GetCandidate("lepton2")->id()) ) exact_overlap = true;
      }    
    }


    // Now define the actual selection mode
    // 0 = OS, Selection
    // 1 = SS, Selection
    // 2 = OS, Control
    // 3 = SS, Control
   
    total_yields_[sel_mode] += wt;
    if (exact_overlap) justpair_yields_[sel_mode] += wt;
    if (fs_ && exact_overlap) {
      metstudy_plots_[sel_mode]->FillVertexPlots(vertices, wt);
      metstudy_plots_[sel_mode]->FillLeptonMetPlots(*(dilepton[0]), *pfMetMVA, wt);
      if (event->Exists("svfitMass")) {
        metstudy_plots_[sel_mode]->FillSVFitMassPlot(event->Get<double>("svfitMass"),wt);
      }
    }

    
    return 0;
  }
  int HttMetStudy::PostAnalysis() {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Post-Analysis Info for HTT MetStudy" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s\n") 
          % "Sel Mode:" % "OS-Sel" % "SS-Sel" % "OS-Con" % "SS-Con";
        std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s\n") 
          % "Total:" % total_yields_[0] % total_yields_[1] % total_yields_[2] % total_yields_[3];
        std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s\n") 
          % "Exact Overlap:" % justpair_yields_[0] % justpair_yields_[1] % justpair_yields_[2] % justpair_yields_[3];
        PrintEff("OS-Sel Frac", justpair_yields_[0], total_yields_[0]);
        PrintEff("SS-Sel Frac", justpair_yields_[1], total_yields_[1]);

    return 0;
  }

  void HttMetStudy::PrintInfo() {
    ;
  }
}
