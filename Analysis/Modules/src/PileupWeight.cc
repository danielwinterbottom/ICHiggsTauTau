#include "Modules/interface/PileupWeight.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/PileupInfo.hh"

namespace ic {

  PileupWeight::PileupWeight(std::string const& name) : ModuleBase(name) {
    is_valid_       = false;
    data_           = nullptr;
    mc_             = nullptr;
    print_weights_  = false;
    label_          = "pileup";
    use_sampled_interactions_ = false;
  }

  PileupWeight::PileupWeight(std::string const& name,
			     std::string const& label) : ModuleBase(name) {
    is_valid_       = false;
    data_           = nullptr;
    mc_             = nullptr;
    print_weights_  = false;
    label_          = label;
    use_sampled_interactions_ = false;
  }

  PileupWeight::~PileupWeight() {
    ;
  }

  int PileupWeight::PreAnalysis() {
    unsigned nbins_data = data_->GetNbinsX();
    unsigned nbins_mc = mc_->GetNbinsX();
    double min_data = data_->GetBinLowEdge(1);
    double min_mc = mc_->GetBinLowEdge(1);
    double max_data = data_->GetBinLowEdge(nbins_data) + data_->GetBinWidth(nbins_data);
    double max_mc = mc_->GetBinLowEdge(nbins_mc) + mc_->GetBinWidth(nbins_mc);
    unsigned nbins = nbins_data > nbins_mc ? nbins_data : nbins_mc;
    double int_data = data_->Integral();
    double int_mc = mc_->Integral();
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for PileupReweight" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    if (data_ && mc_) {
      std::cout << "* Input data histogram has " << nbins_data << " bins in the range [" << min_data << "," << max_data << "], integral = " << int_data << std::endl;
      std::cout << "* Input MC histogram has " << nbins_mc << " bins in the range [" << min_mc << "," << max_mc << "], integral = " << int_mc << std::endl;
      std::cout << "* Weights calculated for " << nbins << " bins" << std::endl;
      std::cout << "* Weight label in EventInfo is " << label_ << std::endl;
    } else {
      std::cout << "Invalid histogram!" << std::endl;
      return 1;
    }
    //data_->Sumw2();
    //mc_->Sumw2();
    data_->Scale(1./int_data);
    mc_->Scale(1./int_mc);
    weights_ = (TH1*)data_->Clone();
    weights_->Divide(mc_);
    for (unsigned i = 0; i < nbins; ++i) {
      if (print_weights_) std::cout << "nInt = [" << weights_->GetBinLowEdge(i+1) << "," << weights_->GetBinLowEdge(i+2) << "[,\tData = " << data_->GetBinContent(i+1) << ",  MC = " << mc_->GetBinContent(i+1) << ",  Weight = " << weights_->GetBinContent(i+1) << std::endl;
    }
    return 0;
  }

  int PileupWeight::Execute(TreeEvent *event) {
    EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");
    std::vector<PileupInfo *> const& puInfo = event->GetPtrVec<PileupInfo>("pileupInfo");
    float true_int = -1;
    for (unsigned i = 0; i < puInfo.size(); ++i) {
      if (puInfo[i]->bunch_crossing() == 0) {
        true_int = use_sampled_interactions_ ?
            puInfo[i]->num_interactions() : puInfo[i]->true_num_interactions();
      }
    }
    if (true_int < 0) {
      std::cout << "Warning: In-time true_num_interactions not found!" << std::endl;
      return 0;
    }
    //double weight = 0.0;
    int found_bin = weights_->FindBin(true_int);
    double weight = 1.0;
    if (found_bin >= 1 && found_bin <= weights_->GetNbinsX()) {
      weight = weights_->GetBinContent(found_bin);
    }
    eventInfo->set_weight(label_, weight);
    return 0;
  }
  int PileupWeight::PostAnalysis() {
    return 0;
  }

  void PileupWeight::PrintInfo() {
    ;
  }
}
