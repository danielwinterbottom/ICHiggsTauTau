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
    
    JetPtCut = offlineCuts.Jet1Pt;
    EtaCut = 4.7;
    DeltaEtaCut = offlineCuts.DeltaEta;
    MjjCut = offlineCuts.Mjj;

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
          if(DeltaEta > 3.5 && Mjj > 500 && jets[0]->vector().Pt() > 20 && jets[1]->vector().Pt() > 20) dontFilter = true;
                  
      }
      
      if(!dontFilter) Filter = true;
      /*
      unsigned jetCount=0;
      
      for(unsigned i=0; i<n_jets_; i++) if(jets[i]->vector().Pt() > JetPtCut && std::fabs(jets[i]->vector().Rapidity() < EtaCut)) jetCount++;  
      
      if(jetCount < 2) Filter = true;
      
      if(jetCount > 1){
      
          double Mjj = (jets[0]->vector() + jets[1]->vector()).M();
          if(Mjj < MjjCut) Filter=true;
          
          double DeltaEta = std::fabs(jets[0]->vector().Rapidity() - jets[1]->vector().Rapidity());
          if(DeltaEta < DeltaEtaCut) Filter = true;
          
          for(unsigned i=2; i<n_jets_; i++){
              double jetsEtaMin = std::min(jets[0]->vector().Rapidity(), jets[1]->vector().Rapidity());
              double jetsEtaMax = std::max(jets[0]->vector().Rapidity(), jets[1]->vector().Rapidity());
              if(jets[i]->vector().Rapidity() > jetsEtaMin && jets[i]->vector().Rapidity() < jetsEtaMax && jets[i]->vector().Pt() > JetPtCut) Filter = true;    
          }
          
      }*/
      
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

