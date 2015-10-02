#include "HiggsTauTau/interface/NvtxWeight.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"

namespace ic {

  NvtxWeight::NvtxWeight(std::string const& name) : ModuleBase(name) {
    is_valid_       = false;
    vertex_dist_    = nullptr;
    print_weights_  = false;
    label_          = "pileup";
  }

  NvtxWeight::NvtxWeight(std::string const& name,
			     std::string const& label) : ModuleBase(name) {
    is_valid_       = false;
    vertex_dist_    = nullptr;
    print_weights_  = false;
    label_          = label;
  }

  NvtxWeight::~NvtxWeight() {
    ;
  }

  int NvtxWeight::PreAnalysis() {
    unsigned nbins_weight = vertex_dist_->GetNbinsX();
//    double min_weight = vertex_dist_->GetBinLowEdge(1);
 //   double max_weight = vertex_dist_->GetBinLowEdge(nbins_weight) + vertex_dist_->GetBinWidth(nbins_weight);
    unsigned nbins = nbins_weight;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for PileupReweight" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
      std::cout << "* Weights calculated for " << nbins << " bins" << std::endl;
      std::cout << "* Weight label in EventInfo is " << label_ << std::endl;
    weights_ = (TH1*)vertex_dist_->Clone();
    for (unsigned i = 0; i < nbins; ++i) {
      if (print_weights_) std::cout << "nInt = [" << weights_->GetBinLowEdge(i+1) << "," << weights_->GetBinLowEdge(i+2) << " Weight = " << weights_->GetBinContent(i+1) << std::endl;
    }
    return 0;
  }

  int NvtxWeight::Execute(TreeEvent *event) {
    EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");
    unsigned n_vtx = eventInfo->good_vertices();
    //double weight = 0.0;
    int found_bin = weights_->FindBin(n_vtx);
    double weight = 1.0;
    if (found_bin >= 1 && found_bin <= weights_->GetNbinsX()) {
      weight = weights_->GetBinContent(found_bin);
    }
    eventInfo->set_weight(label_, weight);
    return 0;
  }
  int NvtxWeight::PostAnalysis() {
    return 0;
  }

  void NvtxWeight::PrintInfo() {
    ;
  }
}
