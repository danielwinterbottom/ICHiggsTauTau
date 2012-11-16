#include "UserCode/ICHiggsTauTau//interface/city.h"
#include "UserCode/ICHiggsTauTau/Analysis/TagAndProbe/interface/UsefulFunctions.h"
#include "TLorentzVector.h"
#include <iostream>
#include <fstream>
#include <TSystem.h>
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLatex.h"

#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooVoigtian.h"
#include "RooCBShape.h"
#include "RooBifurGauss.h"
#include "RooAddPdf.h"
#include "RooExponential.h"
#include "RooSimultaneous.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooFFTConvPdf.h"
#include "RooBreitWigner.h"

namespace ic

{
  double DR(Candidate const cand1, Candidate const cand2) {
     TLorentzVector c1, c2;
     c1.SetPtEtaPhiE(cand1.pt(),cand1.eta(),cand1.phi(),cand1.energy());
     c2.SetPtEtaPhiE(cand2.pt(),cand2.eta(),cand2.phi(),cand2.energy());
     return (c1.DeltaR(c2));
  }
  bool IsFilterMatched(Candidate const cand, std::vector<TriggerObject > const &objs, std::string const& filter, double const& max_dr) {
    std::size_t hash = CityHash64(filter);
    for (unsigned i = 0; i < objs.size(); ++i) {
      std::vector<std::size_t> const& labels = objs[i].filters();
      if (std::find(labels.begin(),labels.end(), hash) == labels.end()) continue;
      if (DR(cand, objs[i]) < max_dr) return true;
    }
    return false;
  }

  bool ElectronHTTId(Electron const& elec, unsigned const& mode) {
    //Do some cut-based pre-selection
    if (elec.has_matched_conversion()) return false;
    if (elec.gsf_tk_nhits() > 0) return false;
    bool pass_mva = false;
    double pt = elec.pt();
    double eta = fabs(elec.sc_eta());
    
    double idmva = elec.GetIdIso("mvaNonTrigV0");
    if (pt <= 20.0 && eta <= 0.8                  && idmva > 0.925) pass_mva = true;
    if (pt <= 20.0 && eta >  0.8 && eta <= 1.479  && idmva > 0.915) pass_mva = true;
    if (pt <= 20.0 && eta >  1.479                && idmva > 0.965) pass_mva = true;
    if (mode != 2) {
      if (pt >  20.0 && eta <= 0.8                  && idmva > 0.925) pass_mva = true;
      if (pt >  20.0 && eta >  0.8 && eta <= 1.479  && idmva > 0.975) pass_mva = true;
      if (pt >  20.0 && eta >  1.479                && idmva > 0.985) pass_mva = true;
    } else {
      if (pt >  20.0 && eta <= 0.8                  && idmva > 0.905) pass_mva = true;
      if (pt >  20.0 && eta >  0.8 && eta <= 1.479  && idmva > 0.955) pass_mva = true;
      if (pt >  20.0 && eta >  1.479                && idmva > 0.975) pass_mva = true;
    }
    return pass_mva;
    }


  bool MuonHTTId(Muon const& muon) {
    bool tightCut = ( 
        muon.is_global() && 
        //muon->is_tracker() &&
        //muon->it_tracker_hits() > 10 &&
        muon.it_pixel_hits() > 0 &&
        muon.gt_valid_muon_hits() > 0 &&
        muon.it_layers_with_measurement() > 5 &&
        muon.gt_normalized_chi2() < 10 &&
        muon.matched_stations() > 1
        );
    return tightCut;
  }

  int testfunction(bool test){
       if(test==true)
       {
           return 10;
       }
       else
       {
           return 20;
       }
  }

  std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
   while(std::getline(ss, item, delim)) {
         elems.push_back(item);
          }
    return elems;
  }

  double PropError(double y1, double err_y1, double y2, double err_y2){
   return ((y2/y1)*sqrt( (err_y1/y1)*(err_y1/y1) + (err_y2/y2)*(err_y2/y2) ));
  }



}
