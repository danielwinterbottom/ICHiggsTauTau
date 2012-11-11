#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttNoBTagCategory.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

namespace ic {

  HttNoBTagCategory::HttNoBTagCategory(std::string const& name, std::string const& jets_label) : ModuleBase(name) {

    btag_jet_pt_ = 20.0;
    btag_jet_eta_ = 2.4;
    mode_ = 0;
    do_met_cut_ = true;
    met_cut_ = 15.0;
    jets_label_ = jets_label;
    met_label_ = "pfMVAMet";
    fs_ = NULL;
    do_btag_weight_ = false;

    nobtag_plots_.resize(4);
    nobtag_yields_.resize(4);
}

  HttNoBTagCategory::~HttNoBTagCategory() {
    ;
  }

  int HttNoBTagCategory::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HTT nobtag Category" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "b-Jet Veto: [Pt > " << btag_jet_pt_ << "] [|Eta| < " << btag_jet_eta_ << "]" << std::endl;
    if (do_met_cut_ && mode_ == 0) std::cout << "Applying [MET > " << met_cut_ << "]" << std::endl;
    nobtag_plots_[0] = new HttPlots(fs_->mkdir("nobtag_os_sel"));
    nobtag_plots_[1] = new HttPlots(fs_->mkdir("nobtag_ss_sel"));
    nobtag_plots_[2] = new HttPlots(fs_->mkdir("nobtag_os_con"));
    nobtag_plots_[3] = new HttPlots(fs_->mkdir("nobtag_ss_con"));
    return 0;
  }

  int HttNoBTagCategory::Execute(TreeEvent *event) {
    std::map<std::string, bool> cat_status;
    if (event->Exists("cat_status")) {
      cat_status = event->Get< std::map<std::string, bool> >("cat_status");
    }
    std::string cat_label = "nobtag";
    cat_status[cat_label] = false;

    std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>("emtauCandidates");


    std::map<std::string, bool>::const_iterator it;
    for (it = cat_status.begin(); it != cat_status.end(); ++it) {
      if ( (it->first == "vbf" && it->second == true) || (it->first == "vh" && it->second == true)) {
        event->ForceAdd("cat_status", cat_status);
        return 2;
      }
    }

     unsigned sel_mode = event->Get<unsigned>("sel_mode");
     EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
     double wt = eventInfo->total_weight();
     Met const* pfMVAMet = event->GetPtr<Met>(met_label_);
     if (mode_ == 0 && do_met_cut_ && pfMVAMet->pt() < met_cut_) {
      event->ForceAdd("cat_status", cat_status);
      return 2;
     }


    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection
    std::vector<PFJet*> main_jets = jets;
    std::vector<PFJet*> btag_jets = jets;

    ic::erase_if(btag_jets,!boost::bind(MinPtMaxEta, _1, btag_jet_pt_, btag_jet_eta_));
    ic::erase_if(btag_jets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.679);
    if (btag_jets.size() > 0) {
      event->ForceAdd("cat_status", cat_status);
      return 2;
    } else {
      if (do_btag_weight_) wt *= event->Get<double>("no_btag_weight");
    }


    if (fs_ && sel_mode < 4) {
      nobtag_yields_[sel_mode] += wt;
      //onejet_plots_[sel_mode]->FillVertexPlots(vertices, wt);
      nobtag_plots_[sel_mode]->FillLeptonMetPlots(*(dilepton[0]), *pfMVAMet, wt);
      nobtag_plots_[sel_mode]->FillJetPlots(main_jets, wt);
      if (event->Exists("svfitMass")) nobtag_plots_[sel_mode]->FillSVFitMassPlot(event->Get<double>("svfitMass"),wt);
    }
    if (fs_ && sel_mode%2 == 0) nobtag_plots_[0]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    if (fs_ && sel_mode%2 == 1) nobtag_plots_[1]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    if (fs_ && sel_mode == 2) nobtag_plots_[2]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    if (fs_ && sel_mode == 3) nobtag_plots_[3]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);


    cat_status[cat_label] = true;
    event->ForceAdd("cat_status", cat_status);
    return 0;
  }
  int HttNoBTagCategory::PostAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Post-Analysis Info for HTT nobtag Category" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s\n") 
      % "Sel Mode:" % "OS-Sel" % "SS-Sel" % "OS-Con" % "SS-Con";
    std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s\n") 
      % ("nobtag:") % nobtag_yields_[0] % nobtag_yields_[1] % nobtag_yields_[2] % nobtag_yields_[3];
    return 0;
  }

  void HttNoBTagCategory::PrintInfo() {
    ;
  }
}
