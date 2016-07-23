#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/VBFFilter.h"

#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "TVector3.h"
#include <fstream>


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
      std::vector<Met*> met_vec = event->GetPtrVec<Met>("pfMVAMet");
      
      //std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>("emtauCandidates");
      //CompositeCandidate const* ditau = ditau_vec.at(0);
      //Candidate const* lep1 = ditau->GetCandidate("lepton1");
      //Candidate const* lep2 = ditau->GetCandidate("lepton2");
          
      n_jets_ = jets.size();
      
      bool Filter = false;
      
      if(n_jets_ < 2) Filter = true;
      if(n_jets_ > 1) if(jets[1]->vector().Pt() < 30) Filter = true;
      
      if(n_jets_ > 1){
          double Mjj = (jets[0]->vector() + jets[1]->vector()).M();
          double DeltaEta = std::fabs(jets[0]->vector().Rapidity() - jets[1]->vector().Rapidity());
          double AvePt = (jets[0]->vector().Pt() + jets[1]->vector().Pt())/2;
          double VecPt = (jets[0]->vector() + jets[1]->vector()).Pt();
          if(!(DeltaEta > DeltaEtaCut && Mjj > MjjCut && jets[0]->vector().Pt() > Jet1PtCut && jets[1]->vector().Pt() > Jet2PtCut && VecPt > VecPtCut && AvePt > AvePtCut)) Filter = true;
          
          
          double EtaMin = std::min(jets[0]->vector().Rapidity(), jets[1]->vector().Rapidity());
          double EtaMax = std::max(jets[0]->vector().Rapidity(), jets[1]->vector().Rapidity());
          
          for(unsigned i=2; i<n_jets_; i++){
                double CentralJetPt = jets[i]->vector().Pt();
                double CentralJetEta = jets[i]->vector().Rapidity();
                if(CentralJetPt > 30 && CentralJetEta > EtaMin && CentralJetEta < EtaMax) Filter = true;
          }
                  
      }

      //double HPt = (lep1->vector() + lep2->vector() + met_vec[0]->vector()).Pt();
      //if(HPt < 100) Filter = true;

      if(Filter) return 1;
      else{
          //ic::EventInfo const* eventInfo = event->GetPtr<ic::EventInfo>("eventInfo");    
              //std::ofstream outfile;
              //outfile.open("DoubleJet30_Mj30Mj30_360_Mu6.dat", std::ios_base::app);
              //outfile << eventInfo->event() << "\n"; 
              //outfile.close();
          /*if(HPt > 100){
              ic::EventInfo const* eventInfo = event->GetPtr<ic::EventInfo>("eventInfo");    
              std::ofstream outfile;
              outfile.open("DoubleIsoTau28er_HPtCut.dat", std::ios_base::app);
              outfile << eventInfo->event() << "\n"; 
              outfile.close();
          } */
          
          return 0;
      }
      
  }
  int VBFFilter::PostAnalysis() {
    return 0;
  }

  void VBFFilter::PrintInfo() {
    ;
  }
}

