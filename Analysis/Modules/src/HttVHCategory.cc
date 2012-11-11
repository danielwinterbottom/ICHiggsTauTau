#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttVHCategory.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "TMVA/Reader.h"
#include "TVector3.h"

namespace ic {

  HttVHCategory::HttVHCategory(std::string const& name, std::string const& jets_label) : ModuleBase(name) {
    jet_pt_ = 30.0;
    jet_eta_ = 5.0;
    dijet_mass_min_ = 70.0;
    dijet_mass_max_ = 120.0;
    dijet_pt_min_ = 150.0;

    jets_label_ = jets_label;

    fs_ = NULL;
    plots_ = NULL;
  }

  HttVHCategory::~HttVHCategory() {
    ;
  }

  int HttVHCategory::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HTT VH Category" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Jet collection: [" << jets_label_ << "]" << std::endl;
    std::cout << "Jet: [Pt > " << jet_pt_ << "] [|eta| < " << jet_eta_ << "]" << std::endl;
    std::cout << "VH: [" << dijet_mass_min_ << " < j1j2 Mass < " << dijet_mass_max_ << "] [j1j2 Pt > " << dijet_pt_min_ << "]" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    if (fs_) plots_ = new HttPlots(fs_->mkdir("vh"));

    return 0;
  }

  int HttVHCategory::Execute(TreeEvent *event) {
    std::map<std::string, bool> cat_status;
    if (event->Exists("cat_status")) {
      cat_status = event->Get< std::map<std::string, bool> >("cat_status");
    }
    cat_status["vh"] = false;

    std::map<std::string, bool>::const_iterator it;
    for (it = cat_status.begin(); it != cat_status.end(); ++it) {
      if (it->first == "vbf" && it->second == true) {
        event->ForceAdd("cat_status", cat_status);
        return 2;
      }
    }
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection
    std::vector<PFJet*> btag_jets = jets;
    ic::erase_if(btag_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.5));
    ic::erase_if(btag_jets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.679);
    if (btag_jets.size() > 0) {
      event->ForceAdd("cat_status", cat_status);
      return 2;
    }


    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, jet_pt_, jet_eta_));
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));

    std::vector<PFJet*> sel_jets;
    if (jets.size() >= 2) {
      sel_jets.push_back(jets[0]);
      sel_jets.push_back(jets[1]);
    } else {
      event->ForceAdd("cat_status", cat_status);
      return 2; // Event fails, but don't stop processing
    }

    double mass = (sel_jets[0]->vector() + sel_jets[1]->vector()).M();
    double pt = (sel_jets[0]->vector() + sel_jets[1]->vector()).pt();


    if (mass > dijet_mass_min_ && mass < dijet_mass_max_ && pt > dijet_pt_min_) {
      cat_status["vbf"] = true;
    } else {
      event->ForceAdd("cat_status", cat_status);
      return 2; // Event fails, but don't stop processing
    }
    event->ForceAdd("cat_status", cat_status);
    return 0;
  }
  int HttVHCategory::PostAnalysis() {
    return 0;
  }

  void HttVHCategory::PrintInfo() {
    ;
  }
}
