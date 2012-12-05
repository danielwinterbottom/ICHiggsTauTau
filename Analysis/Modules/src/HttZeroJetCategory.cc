#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttZeroJetCategory.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

namespace ic {

  HttZeroJetCategory::HttZeroJetCategory(std::string const& name) : ModuleBase(name), channel_(channel::et) {
    high_pt_ = false;
    fs_ = NULL;
    do_met_cut_ = true;
    met_cut_ = 15.0;
    zerojet_plots_.resize(4);
    zerojet_yields_.resize(4);  
    met_label_ = "pfMVAMet";
    jets_label_ = "pfJetsPFlow";
    veto_jet_pt_ = 30.0;
    veto_jet_eta_ = 4.7;
    do_btag_weight_ = false;
    no_pt_split_ = false;
  }

  HttZeroJetCategory::~HttZeroJetCategory() {
    ;
  }

  int HttZeroJetCategory::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HTT Zero Jet Category" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "HighPt Mode: [" << high_pt_ << "] Jet Veto: [Pt > " << veto_jet_pt_ << "] [|Eta| < " << veto_jet_eta_ << "]" << std::endl;
   if (do_met_cut_ && channel_ == channel::et) std::cout << "Applying [MET > " << met_cut_ << "]" << std::endl;
    std::string pt_cat_string = high_pt_ ? "zerojet_highpt" : "zerojet_lowpt";
    if (no_pt_split_) std::cout << "WARNING: NOT SPLITTING BY pT!" << std::endl;
    zerojet_plots_[0] = new HttPlots(fs_->mkdir(pt_cat_string+"_os_sel"));
    zerojet_plots_[1] = new HttPlots(fs_->mkdir(pt_cat_string+"_ss_sel"));
    zerojet_plots_[2] = new HttPlots(fs_->mkdir(pt_cat_string+"_os_con"));
    zerojet_plots_[3] = new HttPlots(fs_->mkdir(pt_cat_string+"_ss_con"));
    return 0;
  }

  int HttZeroJetCategory::Execute(TreeEvent *event) {
    std::map<std::string, bool> cat_status;
    if (event->Exists("cat_status")) {
      cat_status = event->Get< std::map<std::string, bool> >("cat_status");
    }
    std::string cat_label = high_pt_ ? "zerojet_highpt" : "zerojet_lowpt";
    cat_status[cat_label] = false;

    std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>("emtauCandidates");
    double lep2_pt = dilepton.at(0)->GetCandidate("lepton2")->pt();
    double lep2_pt_cut = 40.0;
    if (channel_ == channel::em) lep2_pt_cut = 35.0;
    if (!no_pt_split_) {
      if ( (high_pt_ && lep2_pt < lep2_pt_cut) || (!high_pt_ && lep2_pt >= lep2_pt_cut) ) {
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

    std::map<std::string, bool>::const_iterator it;
    for (it = cat_status.begin(); it != cat_status.end(); ++it) {
      if (it->second == true) {
        event->ForceAdd("cat_status", cat_status);
        return 2;
      }
    }

    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, veto_jet_pt_, veto_jet_eta_));
    if (jets.size() > 0) {
      event->ForceAdd("cat_status", cat_status);
      return 2;
    }

    if (do_btag_weight_) wt *= event->Get<double>("no_btag_weight");



    if (fs_ && sel_mode < 4) {
      zerojet_yields_[sel_mode] += wt;
      //onejet_plots_[sel_mode]->FillVertexPlots(vertices, wt);
      zerojet_plots_[sel_mode]->FillLeptonMetPlots(*(dilepton[0]), *pfMVAMet, wt);
      //zerojet_plots_[sel_mode]->FillJetPlots(main_jets, wt);
      if (event->Exists("svfitMass")) {
        zerojet_plots_[sel_mode]->FillSVFitMassPlot(event->Get<double>("svfitMass"),wt);
        zerojet_plots_[sel_mode]->FillSVFitMassModePlots(*(dilepton[0]), event->Get<double>("svfitMass"),wt);
      }
    }
    if (fs_ && sel_mode%2 == 0) zerojet_plots_[0]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    if (fs_ && sel_mode%2 == 1) zerojet_plots_[1]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    if (fs_ && sel_mode == 2) zerojet_plots_[2]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    if (fs_ && sel_mode == 3) zerojet_plots_[3]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);


    cat_status[cat_label] = true;
    event->ForceAdd("cat_status", cat_status);
    return 0;
  }
  int HttZeroJetCategory::PostAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Post-Analysis Info for HTT ZeroJet Category" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s\n") 
      % "Sel Mode:" % "OS-Sel" % "SS-Sel" % "OS-Con" % "SS-Con";
    std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s\n") 
      % (std::string(high_pt_ ? "zerojet_highpt" : "zerojet_lowpt")+":") % zerojet_yields_[0] % zerojet_yields_[1] % zerojet_yields_[2] % zerojet_yields_[3];
    return 0;
  }

  void HttZeroJetCategory::PrintInfo() {
    ;
  }
}
