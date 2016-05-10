#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/FilterGoodEvents.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "TVector3.h"

namespace ic {

  FilterGoodEvents::FilterGoodEvents(std::string const& name, std::string channel) : ModuleBase(name) {
    channel_ = channel;
  }

  FilterGoodEvents::~FilterGoodEvents() {
;
  }

  int FilterGoodEvents::PreAnalysis() {

    return 0;
  }

  int FilterGoodEvents::Execute(TreeEvent *event) {
    
      std::vector<ic::GenParticle*> GenParticles = event->GetPtrVec<ic::GenParticle>("genParticles");
      std::vector<ic::GenJet*> genjets = event->GetPtrVec<ic::GenJet>("genJets");

      n_genParticles_ = GenParticles.size();
      n_genjets_ = genjets.size();
      
      std::vector<ic::Candidate> *HadTaus = new std::vector<ic::Candidate>();
      std::vector<ic::GenParticle> *Electrons = new std::vector<ic::GenParticle>();
      std::vector<ic::GenParticle> *Muons = new std::vector<ic::GenParticle>();
      std::vector<ic::GenJet> *VBFJets = new std::vector<ic::GenJet>();
      
      for(unsigned j=0; j< n_genParticles_; j++){
          
         int ID = std::fabs(GenParticles[j]->pdgid());
         
         bool isHadTau =  false;

         if(ID == 15 && GenParticles[j]->statusFlags().at(0)){
             
             isHadTau = true;
             ic::Candidate tausVis;
         
             for(unsigned k=0; k < GenParticles[j]->daughters().size(); k++){
                 if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 11 || std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 13) isHadTau = false;  
                 if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 16) tausVis.set_vector(GenParticles[j]->vector() - GenParticles[GenParticles[j]->daughters().at(k)]->vector());
             }
             
             if(isHadTau && std::fabs(tausVis.vector().Rapidity()) < 2.3 && tausVis.vector().Pt() > 18) HadTaus->push_back(tausVis);
         }
         
         if(ID == 11 && GenParticles[j]->statusFlags().at(5) && std::fabs(GenParticles[j]->vector().Rapidity()) < 2.4 && GenParticles[j]->vector().Pt() > 5) Electrons->push_back(*GenParticles[j]);
         if(ID == 13 && GenParticles[j]->statusFlags().at(5) && std::fabs(GenParticles[j]->vector().Rapidity()) < 2.4 && GenParticles[j]->vector().Pt() > 5) Muons->push_back(*GenParticles[j]);
         
         ic::GenJet VBFJet;
           
         double DeltaRMin1 = 100000;
         double DeltaRMin2 = 100000;
         int genjetIndex1 = -1;
         int genjetIndex2 = -1;
         
         for(unsigned i=0; i< n_genjets_; i++){
             
             double DeltaR1 = sqrt(pow(genjets[i]->vector().Phi()-GenParticles[5]->vector().Phi(),2) + pow(genjets[i]->vector().Rapidity()-GenParticles[5]->vector().Rapidity(),2));
             if(DeltaR1 < DeltaRMin1 && DeltaR1 < 0.5){
                 DeltaRMin1 = DeltaR1;
                 genjetIndex1 = i;
             }
             
             double DeltaR2 = sqrt(pow(genjets[i]->vector().Phi()-GenParticles[6]->vector().Phi(),2) + pow(genjets[i]->vector().Rapidity()-GenParticles[6]->vector().Rapidity(),2));
             if(DeltaR2 < DeltaRMin2 && DeltaR2 < 0.5){
                 DeltaRMin2 = DeltaR2;
                 genjetIndex2 = i;
             }
         }
         
         if(genjetIndex1 != -1 && genjetIndex2 != -1){
             
             double Mjj = (genjets[genjetIndex1]->vector() + genjets[genjetIndex2]->vector()).M();
             double deltaEta = std::fabs(genjets[genjetIndex1]->vector().Rapidity() - genjets[genjetIndex2]->vector().Rapidity());
             
             if(Mjj > 500 && deltaEta > 3.5 && genjets[genjetIndex1]->vector().Pt() > 20 && genjets[genjetIndex2]->vector().Pt() > 20 && std::fabs(genjets[genjetIndex1]->vector().Rapidity()) < 4.7 && std::fabs(genjets[genjetIndex2]->vector().Rapidity()) < 4.7){
                 VBFJets->push_back(*genjets[genjetIndex1]);
                 VBFJets->push_back(*genjets[genjetIndex2]);
             }
         }
         
      }

      bool Filter = false;
      
      if(VBFJets->size() < 2) Filter = true;
      
      if(channel_ == "em" && !(Electrons->size() >= 1 && Muons->size() >=1)) Filter = true;
      if(channel_ == "et" && !(Electrons->size() >= 1 && HadTaus->size() >=1)) Filter = true;
      if(channel_ == "mt" && !(Muons->size() >= 1 && HadTaus->size() >=1)) Filter = true;
      if(channel_ == "tt" && !(HadTaus->size() >= 2)) Filter = true;
      
      if(Filter) return 1;
      else return 0;
      
  }
  int FilterGoodEvents::PostAnalysis() {
    return 0;
  }

  void FilterGoodEvents::PrintInfo() {
    ;
  }
}

