#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/ZPass.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "TVector3.h"


struct greater_Candidate{
  bool operator() (const ic::Candidate *a,const ic::Candidate *b) {
    return (a->pt() > b->pt());
  }
};

namespace ic {

  ZPass::ZPass(std::string const& name, std::string channel, fwlite::TFileService *fs, struct L1Cuts l1Cuts, std::string L1MuTag, std::string subDirName) : ModuleBase(name) {
    
    channel_ = channel;
    
    Jet1PtCut = l1Cuts.Jet1Pt;
    Jet2PtCut = l1Cuts.Jet2Pt;
    DeltaEtaCut = l1Cuts.DeltaEta;
    MjjCut = l1Cuts.Mjj;
    EGPtCut = l1Cuts.EGPt;
    Tau1PtCut = l1Cuts.Tau1Pt;
    Tau2PtCut = l1Cuts.Tau2Pt;
    MuPtCut = l1Cuts.MuPt;
    IsoTauFilter = l1Cuts.IsoTau;
    IsoEGFilter = l1Cuts.IsoEG;
    IsoMuFilter = l1Cuts.IsoMu;
    JetFilter = l1Cuts.JetFilter;
    
    l1jets_label_ = "L1Jets"; 
    l1electrons_label_ = "L1EGammas";
    l1muons_label_ = L1MuTag;
    l1taus_label_ = "L1Taus";  
    
    subDir = fs->mkdir(subDirName.c_str());
 
  }

  ZPass::~ZPass() {
      ;
  }

  int ZPass::PreAnalysis() {
      
    hists_ = new DynamicHistoSet(subDir);
    std::string histname;
    for(unsigned i =0; i < 500; i++){
        histname = Form("h_Pass%d", i);
        hists_->Create(histname.c_str(), 1000000, i*1000000, (i+1)*1000000);    
    }

    return 0;
  }

  int ZPass::Execute(TreeEvent *event) {
      
      ic::EventInfo* EventInfo = event->GetPtr<ic::EventInfo>("eventInfo");
      
      std::string histname;
      for(unsigned i=0; i < 500; i++){
          histname = Form("h_Pass%d", i);
          TH1F *h_temp = hists_->Get_Histo(histname.c_str());    
          if(EventInfo->event() >= i*1000000 && EventInfo->event() < (i+1)*1000000) h_temp->Fill(EventInfo->event());
      }
      
      return 0;
  }
  
  int ZPass::PostAnalysis() {
    return 0;
  }

  void ZPass::PrintInfo() {
    ;
  }
}

