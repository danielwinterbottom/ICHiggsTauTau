#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/GenChannelFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "TVector3.h"

namespace ic {

  GenChannelFilter::GenChannelFilter(std::string const& name, fwlite::TFileService *fs, std::string channel, unsigned isDY) : ModuleBase(name) {
    genParticles_label_ = "genParticles";
    channel_ = channel;
    if(isDY==1) DY = true;
    else DY = false;
    
    TFileDirectory subDir = fs->mkdir("PassedOffline");
    
    h_PassedOffline = subDir.make<TH1D>("h_PassedOffline","h_PassedOffline",2, 0,2);
    
  }

  GenChannelFilter::~GenChannelFilter() {
;
  }

  int GenChannelFilter::PreAnalysis() {
    return 0;
  }

  int GenChannelFilter::Execute(TreeEvent *event) {
    
      std::vector<ic::GenParticle*> GenParticles = event->GetPtrVec<ic::GenParticle>(genParticles_label_);

      n_genParticles_ = GenParticles.size();
      
      unsigned nHadTaus = 0;
      unsigned nElectrons = 0;
      unsigned nMuons = 0;
      
      for(unsigned j=0; j< n_genParticles_; j++){

          int genID = std::fabs(GenParticles[j]->pdgid());
              
          if(genID == 15){
              
              if(std::fabs(GenParticles[GenParticles[j]->mothers().at(0)]->pdgid()) == 25 || std::fabs(GenParticles[GenParticles[j]->mothers().at(0)]->pdgid()) == 23){
                  bool isMuon = false;
                  bool isElectron = false;
            
                  for(unsigned l=0; l < GenParticles[j]->daughters().size(); l++) {
                          if(std::fabs(GenParticles[GenParticles[j]->daughters().at(l)]->pdgid()) == 11) isElectron = true;
                          if(std::fabs(GenParticles[GenParticles[j]->daughters().at(l)]->pdgid()) == 13) isMuon = true;
                  }
                  
                  if(isElectron) nElectrons++;
                  else if(isMuon) nMuons++;
                  else nHadTaus++;
              }
          }
      }

      bool Filter = true;
      
      if(!DY){
          if(channel_ == "em" && (nElectrons == 1 && nMuons == 1 && nHadTaus == 0)) Filter = false;
          if(channel_ == "et" && (nElectrons == 1 && nMuons == 0 && nHadTaus == 1)) Filter = false;
          if(channel_ == "mt" && (nElectrons == 0 && nMuons == 1 && nHadTaus == 1)) Filter = false;
          if(channel_ == "tt" && (nElectrons == 0 && nMuons == 0 && nHadTaus == 2)) Filter = false;
      }
      else if(DY && (nHadTaus+nElectrons+nMuons) == 2) Filter = false;
      
      if(Filter) return 1;
      else{
          if(!DY){
              bool *PassedOffline = event->GetPtr<bool>("PassedOffline");
              h_PassedOffline->Fill(*PassedOffline);
          }
          return 0;
      }
      
  }
  int GenChannelFilter::PostAnalysis() {
    return 0;
  }

  void GenChannelFilter::PrintInfo() {
    ;
  }
}

