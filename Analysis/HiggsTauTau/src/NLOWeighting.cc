/*
  functionmacro.C : provide Higgs pT reweighting factor for the given mass and tan(beta)
  Don't change ! (unless, you need your own defined function)

  20 April 2016
 */
#include "TMath.h"
#include "TFile.h"
#include "TH1.h"
#include <iostream>
#include <vector>
#include "TROOT.h"
#include "TGraphErrors.h"
#include "HiggsTauTau/interface/NLOWeighting.h"

namespace nloweights {

TGraphErrors* func[27][60];
std::vector<int> marray = {80, 90, 100, 110, 120, 130, 140, 160, 180, 200, 250, 300, 350, 400, 450, 500, 600, 700, 800, 900, 1000, 1200, 1400, 1500, 1600, 1800, 2000};


void ReadFile(){

  static bool done = false;
  if (done) return;

  TFile *file = new TFile("input/ggh_weights/Reweight.root");
  file->cd();

  const int num_of_tb = 60;  

  int imass = 0;
  for(auto mass: marray) {
    for(int tanb=0; tanb < num_of_tb; tanb++){

      TString wname = "weight_MSSM_";
      wname += mass;
      wname += "_tanb_";
      wname += tanb + 1;

      //      std::cout << wname << " " << func[imass][tanb] << " " << func[imass][tanb]->GetBinContent(1) << std::endl;
      func[imass][tanb] = (TGraphErrors*) gROOT->FindObject(wname);
    }    
    imass++;
  }

  done = true;
  //  delete file;
}

float returnNLOweight(Int_t mass, Int_t tanb, Double_t pt){

  if(pt > 800){
    //    std::cout << "[INFO] pT = " << pt << " exceeds the range --> set it to 800." << std::endl;    
    pt = 800;
  }

  auto iter = std::find(marray.begin(), marray.end(), mass);
  size_t index = std::distance(marray.begin(), iter);

  if(index == marray.size()){
    std::cout << "[WARNING] Invalid mass point ... " << mass << " -> return weight 1" << std::endl;    
    return 1;
  }
  
  if(tanb <1 || tanb > 60){
    std::cout << "[WARNING] Invalid tan(beta) point ... " << tanb << " -> return weight 1" << std::endl;
    return 1;
  }

  return func[index][tanb-1]->Eval(pt) ;

}

}

namespace ic {

  NLOWeighting::NLOWeighting(std::string const& name) : ModuleBase(name) {
    fs_ = NULL;
  }

  NLOWeighting::~NLOWeighting() {
    ;
  }

  int NLOWeighting::PreAnalysis() {
    if (fs_) {
      tout = fs_->make<TTree>("nlo_pt", "nlo_pt");
      tout->Branch("wt_nlo_pt", &nlo_wt_);
      tout->Branch("nlo_pt", &nlo_pt_);
    }
    return 0;
  }

  int NLOWeighting::Execute(TreeEvent *event) {
    auto const& parts = event->GetPtrVec<GenParticle>("genParticles");
    ROOT::Math::PtEtaPhiEVector gen_boson;
    unsigned higgs_idx = 0;
    for(unsigned i = 0; i < parts.size(); ++i) {
      std::vector<bool> status_flags = parts[i]->statusFlags();
      unsigned id = abs(parts[i]->pdgid());
      unsigned status = abs(parts[i]->status());
      if ((id >= 11 && id <= 16 && status_flags[FromHardProcess] && status == 1) ||
           status_flags[IsDirectHardProcessTauDecayProduct]) {
        gen_boson += parts[i]->vector();
      }
      if (id == 36) {
        higgs_idx = i;
        // std::cout << "Higgs with pT: " << parts[i]->pt() << "\n";
      }
    }
    double mssm_nlo_pt = parts[higgs_idx]->pt();
    // double mssm_nlo_pt = gen_boson.pt();
    // std::cout << "Summed pT: " << mssm_nlo_pt << "\n";
    double mssm_nlo_wt = nloweights::returnNLOweight(500, 30, mssm_nlo_pt);
    event->Add("mssm_nlo_wt", mssm_nlo_wt);
    event->Add("mssm_nlo_pt", mssm_nlo_pt);
    nlo_wt_ = mssm_nlo_wt;
    nlo_pt_ = mssm_nlo_pt;
    tout->Fill();
    return 0;
  }

  int NLOWeighting::PostAnalysis() {
    return 0;
  }

  void NLOWeighting::PrintInfo() {
    ;
  }
}

