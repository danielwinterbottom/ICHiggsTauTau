#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/WJetsWeights.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "TMVA/Reader.h"
#include "TVector3.h"


namespace ic {

  WJetsWeights::WJetsWeights(std::string const& name) : ModuleBase(name) {

  }

  WJetsWeights::~WJetsWeights() {
    ;
  }

  int WJetsWeights::PreAnalysis() {
    if (fs_) {
      hists_ = new DynamicHistoSet(fs_->mkdir("parton_jets"));
      hists_->Create("njets", 10, -0.5, 9.5);
    }
    counts_.resize(6);
    return 0;
  }


  int WJetsWeights::Execute(TreeEvent *event) {
    std::vector<GenParticle*> const& parts = event->GetPtrVec<GenParticle>("genParticles");
    bool count_jets = false;
    unsigned partons = 0;
    // std::cout << "--------New Events-----------" << std::endl;
    for (unsigned i = 0; i < parts.size(); ++i) {
      // std::cout << i << "\t" << parts[i]->pdgid() << "\t" << parts[i]->status() << "\t" << parts[i]->vector() << std::endl;
      if (parts[i]->status() != 3) continue;  // Don't care about non-status 3
      unsigned id = abs(parts[i]->pdgid());
      if (count_jets) { // If we've found the W, the subsequent particles will be the partons - count them
        if (id == 1 || id == 2 || id == 3 || id == 4 || id == 5 || id == 6 || id == 21) partons++;
      }
      if (id == 24) count_jets = true;  // Start counting partons after we find the W
    }

    if (fs_) {
      hists_->Fill("njets", partons);
    }
    if (partons < 6) {
      counts_[partons]++;
    }

  return 0;

   
  }
  int WJetsWeights::PostAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Post-Analysis Info for HTT MetStudy" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    double tot = 0.0; 
    for (unsigned  i = 0; i < counts_.size(); ++i) tot += counts_[i];
    for (unsigned  i = 0; i < counts_.size(); ++i) {
      std::cout << "n" << i << ": " << counts_[i] << "\t\t" << counts_[i]/tot << std::endl;

    }
    return 0;
  }

  void WJetsWeights::PrintInfo() {
    ;
  }
}
