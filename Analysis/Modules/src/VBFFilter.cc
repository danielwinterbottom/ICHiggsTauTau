#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/VBFFilter.h"

#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "TVector3.h"


struct greater_Candidate{
  bool operator() (const ic::Candidate *a,const ic::Candidate *b) {
    return (a->pt() > b->pt());
  }
};

namespace ic {

  VBFFilter::VBFFilter(std::string const& name, struct OfflineCuts offlineCuts) : ModuleBase(name) {

    jets_label_ = "ak4PFJetsCHS"; 
    Jet1PtCut = offlineCuts.Jet1Pt;
    Jet2PtCut = offlineCuts.Jet2Pt;
    EtaCut = 4.7;
    DeltaEtaCut = offlineCuts.DeltaEta;
    MjjCut = offlineCuts.Mjj;
    AvePtCut = offlineCuts.AvePt;
    VecPtCut = offlineCuts.VecPt;

  }

  VBFFilter::~VBFFilter() {
  ;
  }

  int VBFFilter::PreAnalysis() {

    return 0;
  }

  int VBFFilter::Execute(TreeEvent *event) {

      std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
          
      n_jets_ = jets.size();
      
      bool Filter = false;
      bool dontFilter = false;
      
      if(n_jets_ < 2) Filter = true;
      if(n_jets_ > 1) if(jets[1]->vector().Pt() < 20) Filter = true;
      
      if(n_jets_ > 1){
          double Mjj = (jets[0]->vector() + jets[1]->vector()).M();
          double DeltaEta = std::fabs(jets[0]->vector().Rapidity() - jets[1]->vector().Rapidity());
          double AvePt = (jets[0]->vector().Pt() + jets[1]->vector().Pt())/2;
          double VecPt = (jets[0]->vector() + jets[1]->vector()).Pt();
          if(DeltaEta > DeltaEtaCut && Mjj > MjjCut && jets[0]->vector().Pt() > Jet1PtCut && jets[1]->vector().Pt() > Jet2PtCut && VecPt > VecPtCut && AvePt > AvePtCut) dontFilter = true;
                  
      }
      
      if(!dontFilter) Filter = true;
      
      if(Filter) return 1;
      else return 0;
      
  }
  int VBFFilter::PostAnalysis() {
    return 0;
  }

  void VBFFilter::PrintInfo() {
    ;
  }
}

