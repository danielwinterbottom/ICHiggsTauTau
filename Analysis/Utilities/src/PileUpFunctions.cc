#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/PileupInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/PileUpFunctions.h"
#include <string>
#include "TH1F.h"
#include "TFile.h"


    TH1* WeightCalculation(TH1* data_, TH1* mc_, bool print_weights_) {
        TH1* weights_;
        unsigned nbins_data = data_->GetNbinsX();
        unsigned nbins_mc = mc_->GetNbinsX();
        // double min_data = data_->GetBinLowEdge(1);
        // double min_mc = mc_->GetBinLowEdge(1);
        // double max_data = data_->GetBinLowEdge(nbins_data) + data_->GetBinWidth(nbins_data);
        // double max_mc = mc_->GetBinLowEdge(nbins_mc) + mc_->GetBinWidth(nbins_mc);
        unsigned nbins = nbins_data > nbins_mc ? nbins_data : nbins_mc;
        //std::cout << "----------------------------------------" << std::endl;
        //std::cout << "PreAnalysis Info for PileupReweight" << std::endl;
        //std::cout << "----------------------------------------" << std::endl;
       // if (data_ && mc_) {
       //   std::cout << "- Input data histogram has " << nbins_data << " bins in the range [" << min_data << "," << max_data << "]" << std::endl;
       //   std::cout << "- Input MC histogram has " << nbins_mc << " bins in the range [" << min_mc << "," << max_mc << "]" << std::endl;
       //   std::cout << "- Weights calculated for " << nbins << " bins" << std::endl;
        //} else {
        if(!(data_ && mc_))
        {
          std::cout << "Invalid histogram!" << std::endl;
        }
        //weights_.resize(nbins);
        data_->Scale(1./data_->Integral());
        mc_->Scale(1./mc_->Integral());
        weights_ = (TH1*)data_->Clone();
        weights_->Divide(mc_);
        for (unsigned i = 0; i < nbins; ++i) {
          if (print_weights_) std::cout << "nInt = [" << weights_->GetBinLowEdge(i+1) << "," << weights_->GetBinLowEdge(i+2) << "],\tData = " << data_->GetBinContent(i+1) << ",  MC = " << mc_->GetBinContent(i+1) << ",  Weight = " << weights_->GetBinContent(i+1) << std::endl;
        }
        return weights_;
  }

