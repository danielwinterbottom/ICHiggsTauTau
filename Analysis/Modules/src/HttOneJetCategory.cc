#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttOneJetCategory.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

namespace ic {

  HttOneJetCategory::HttOneJetCategory(std::string const& name, std::string const& jets_label) : ModuleBase(name), channel_(channel::et) {
    jet_pt_ = 30.0;
    jet_eta_ = 4.7;
    btag_jet_pt_ = 20.0;
    btag_jet_eta_ = 2.4;
    do_met_cut_ = true;
    met_cut_ = 15.0;
    jets_label_ = jets_label;
    met_label_ = "pfMVAMet";
    high_pt_ = false;
    fs_ = NULL;
    do_btag_weight_ = false;

    onejet_plots_.resize(4);
    onejet_yields_.resize(4);
}

  HttOneJetCategory::~HttOneJetCategory() {
    ;
  }

  int HttOneJetCategory::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HTT 1-Jet Category" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "HighPt Mode: [" << high_pt_ << "] Jets: [" << jets_label_ << "] [Pt > " << jet_pt_ << "] [|eta| < " << jet_eta_ << "]" << std::endl;
    std::cout << "b-Jet Veto: [Pt > " << btag_jet_pt_ << "] [|Eta| < " << btag_jet_eta_ << "]" << std::endl;
    if (do_met_cut_ && channel_ == channel::et) std::cout << "Applying [MET > " << met_cut_ << "]" << std::endl;
    std::string pt_cat_string = high_pt_ ? "onejet_highpt" : "onejet_lowpt";
    onejet_plots_[0] = new HttPlots(fs_->mkdir(pt_cat_string+"_os_sel"));
    onejet_plots_[1] = new HttPlots(fs_->mkdir(pt_cat_string+"_ss_sel"));
    onejet_plots_[2] = new HttPlots(fs_->mkdir(pt_cat_string+"_os_con"));
    onejet_plots_[3] = new HttPlots(fs_->mkdir(pt_cat_string+"_ss_con"));
    return 0;
  }

  int HttOneJetCategory::Execute(TreeEvent *event) {
    std::map<std::string, bool> cat_status;
    if (event->Exists("cat_status")) {
      cat_status = event->Get< std::map<std::string, bool> >("cat_status");
    }
    std::string cat_label = high_pt_ ? "onejet_highpt" : "onejet_lowpt";
    cat_status[cat_label] = false;

    std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>("emtauCandidates");
    double lep2_pt = dilepton.at(0)->GetCandidate("lepton2")->pt();
    double lep2_pt_cut = 40.0;
    if (channel_ == channel::em) lep2_pt_cut = 35.0;
    if ( (high_pt_ && lep2_pt < lep2_pt_cut) || (!high_pt_ && lep2_pt >= lep2_pt_cut) ) {
      event->ForceAdd("cat_status", cat_status);
      return 2;
    }


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
     if (channel_ == channel::et && do_met_cut_ && pfMVAMet->pt() < met_cut_) {
      event->ForceAdd("cat_status", cat_status);
      return 2;
     }


    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection
    std::vector<PFJet*> main_jets = jets;
    std::vector<PFJet*> btag_jets = jets;
    ic::erase_if(main_jets,!boost::bind(MinPtMaxEta, _1, jet_pt_, jet_eta_));
    if (main_jets.size() == 0) {
      event->ForceAdd("cat_status", cat_status);
      return 2;
    }

    ic::erase_if(btag_jets,!boost::bind(MinPtMaxEta, _1, btag_jet_pt_, btag_jet_eta_));
    ic::erase_if(btag_jets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.679);
    if (btag_jets.size() > 0) {
      event->ForceAdd("cat_status", cat_status);
      return 2;
    } else {
      if (do_btag_weight_) wt *= event->Get<double>("no_btag_weight");
    }


    if (fs_ && sel_mode < 4) {
      onejet_yields_[sel_mode] += wt;
      //onejet_plots_[sel_mode]->FillVertexPlots(vertices, wt);
      onejet_plots_[sel_mode]->FillLeptonMetPlots(*(dilepton[0]), *pfMVAMet, wt);
      onejet_plots_[sel_mode]->FillJetPlots(main_jets, wt);
      if (event->Exists("svfitMass")) {
        onejet_plots_[sel_mode]->FillSVFitMassPlot(event->Get<double>("svfitMass"),wt);
        onejet_plots_[sel_mode]->FillSVFitMassModePlots(*(dilepton[0]), event->Get<double>("svfitMass"),wt);
      }
    }
    if (fs_ && sel_mode%2 == 0) onejet_plots_[0]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    if (fs_ && sel_mode%2 == 1) onejet_plots_[1]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    if (fs_ && sel_mode == 2) onejet_plots_[2]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    if (fs_ && sel_mode == 3) onejet_plots_[3]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);


    cat_status[cat_label] = true;
    event->ForceAdd("cat_status", cat_status);
    return 0;
  }
  int HttOneJetCategory::PostAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Post-Analysis Info for HTT OneJet Category" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s\n") 
      % "Sel Mode:" % "OS-Sel" % "SS-Sel" % "OS-Con" % "SS-Con";
    std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s\n") 
      % (std::string(high_pt_ ? "onejet_highpt" : "onejet_lowpt")+":") % onejet_yields_[0] % onejet_yields_[1] % onejet_yields_[2] % onejet_yields_[3];
    return 0;
  }

  void HttOneJetCategory::PrintInfo() {
    ;
  }
}
