#include "Modules/interface/HLTID.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "TVector3.h"

namespace ic {

  
  HLTID::HLTID(std::string const& name, 
                                         std::string output_name) : ModuleBase(name) {
    
    HLT1_label_ = "triggerObjectsEle12Mu23";
    HLT2_label_ = "triggerObjectsEle12Mu17";
    HLT3_label_ = "triggerObjectsEle23Mu8";
    HLT4_label_ = "triggerObjectsEle17Mu8";
    HLT5_label_ = "triggerObjectsEle22LooseTau20";
    HLT6_label_ = "triggerObjectsEle27Gsf";
    HLT7_label_ = "triggerObjectsEle32GsfTight";
    HLT8_label_ = "triggerObjectsEle22Gsf";
    HLT9_label_ = "triggerObjectsIsoMu17LooseTau20";
    HLT10_label_ = "triggerObjectsIsoMu17";
    HLT11_label_ = "triggerObjectsIsoMu22";
    HLT12_label_ = "triggerObjectsIsoMu18";
    HLT13_label_ = "triggerObjectsEle23";
    HLT14_label_ = "triggerObjectsDoubleMediumTau35";
    HLT15_label_ = "triggerObjectsDoubleMediumTau40";
    
    genParticles_label_ = "genParticles";
 

    
    output_name_ = output_name;
     

    fOut = new TFile((output_name2_+output_name_).c_str(),"RECREATE");
    
    h_HLTTriggerID = new TH1D("HLTTriggerID","HLTTriggerID",200, 0,20); h_HLTTriggerID->SetDirectory(fOut);
    h_HLTTriggerID->GetXaxis()->SetTitle("HLT ID");
    h_HLTTriggerID->GetYaxis()->SetTitle("# Entries");

    h_HLTTriggerID_em = new TH1D("HLTTriggerID_em","HLTTriggerID_em",200, 0,20); h_HLTTriggerID_em->SetDirectory(fOut);
    h_HLTTriggerID_em->GetXaxis()->SetTitle("HLT ID");
    h_HLTTriggerID_em->GetYaxis()->SetTitle("# Entries");
    
    h_HLTTriggerID_et = new TH1D("HLTTriggerID_et","HLTTriggerID_et",200, 0,20); h_HLTTriggerID_et->SetDirectory(fOut);
    h_HLTTriggerID_et->GetXaxis()->SetTitle("HLT ID");
    h_HLTTriggerID_et->GetYaxis()->SetTitle("# Entries");
    
    h_HLTTriggerID_mt = new TH1D("HLTTriggerID_mt","HLTTriggerID_mt",200, 0,20); h_HLTTriggerID_mt->SetDirectory(fOut);
    h_HLTTriggerID_mt->GetXaxis()->SetTitle("HLT ID");
    h_HLTTriggerID_mt->GetYaxis()->SetTitle("# Entries");
    
    h_HLTTriggerID_tt = new TH1D("HLTTriggerID_tt","HLTTriggerID_tt",200, 0,20); h_HLTTriggerID_tt->SetDirectory(fOut);
    h_HLTTriggerID_tt->GetXaxis()->SetTitle("HLT ID");
    h_HLTTriggerID_tt->GetYaxis()->SetTitle("# Entries");

  }

    HLTID::~HLTID(){
      fOut->Write();
  }
  
  int HLTID::PreAnalysis(){
    std::cout << "** Output Histograms for n_jet >= 2 catagory **" << std::endl;

    return 0;
  }

  int HLTID::Execute(TreeEvent *event){
      
      std::vector<ic::GenParticle*> GenParticles = event->GetPtrVec<ic::GenParticle>(genParticles_label_);
      
      n_genParticles_ = GenParticles.size();
      unsigned int n_GenTaus = 0;
      
      bool e_found[2] = {false};
      bool mu_found[2] = {false};
      bool tau_found[2] = {false};
      
      bool em_channel = false;
      bool et_channel = false;
      bool mt_channel = false;
      bool tt_channel = false;
      
      for(unsigned int i=0; i < n_genParticles_; i++){
          
          if(fabs(GenParticles[i]->pdgid()) == 15 && fabs(GenParticles[GenParticles[i]->mothers().at(0)]->pdgid()) == 25){
              
              for(unsigned int j=0; j < GenParticles[i]->daughters().size(); j++){
                  if(fabs(GenParticles[GenParticles[i]->daughters().at(j)]->pdgid()) == 11) e_found[n_GenTaus] = true; 
                  if(fabs(GenParticles[GenParticles[i]->daughters().at(j)]->pdgid()) == 13) mu_found[n_GenTaus] = true;
              }
              if(e_found[n_GenTaus] != true && mu_found[n_GenTaus] != true) tau_found[n_GenTaus] = true;
              n_GenTaus++;
          }
          
      }
      
      if((e_found[0] == true && mu_found[1] == true) || (e_found[1] == true && mu_found[0] == true)) em_channel = true;
      if((e_found[0] == true && tau_found[1] == true) || (e_found[1] == true && tau_found[0] == true)) et_channel = true;
      if((tau_found[0] == true && mu_found[1] == true) || (tau_found[1] == true && mu_found[0] == true)) mt_channel = true;
      if((tau_found[0] == true && tau_found[1] == true) || (tau_found[1] == true && tau_found[0] == true)) tt_channel = true;
    
      std::vector<ic::TriggerObject*> HLTObject1 = event->GetPtrVec<ic::TriggerObject>(HLT1_label_);
      std::vector<ic::TriggerObject*> HLTObject2 = event->GetPtrVec<ic::TriggerObject>(HLT2_label_);
      std::vector<ic::TriggerObject*> HLTObject3 = event->GetPtrVec<ic::TriggerObject>(HLT3_label_);
      std::vector<ic::TriggerObject*> HLTObject4 = event->GetPtrVec<ic::TriggerObject>(HLT4_label_);
      std::vector<ic::TriggerObject*> HLTObject5 = event->GetPtrVec<ic::TriggerObject>(HLT5_label_);
      std::vector<ic::TriggerObject*> HLTObject6 = event->GetPtrVec<ic::TriggerObject>(HLT6_label_);
      std::vector<ic::TriggerObject*> HLTObject7 = event->GetPtrVec<ic::TriggerObject>(HLT7_label_);
      std::vector<ic::TriggerObject*> HLTObject8 = event->GetPtrVec<ic::TriggerObject>(HLT8_label_);
      std::vector<ic::TriggerObject*> HLTObject9 = event->GetPtrVec<ic::TriggerObject>(HLT9_label_);
      std::vector<ic::TriggerObject*> HLTObject10 = event->GetPtrVec<ic::TriggerObject>(HLT10_label_);
      std::vector<ic::TriggerObject*> HLTObject11 = event->GetPtrVec<ic::TriggerObject>(HLT11_label_);
      std::vector<ic::TriggerObject*> HLTObject12 = event->GetPtrVec<ic::TriggerObject>(HLT12_label_);
      std::vector<ic::TriggerObject*> HLTObject13 = event->GetPtrVec<ic::TriggerObject>(HLT13_label_);
      std::vector<ic::TriggerObject*> HLTObject14 = event->GetPtrVec<ic::TriggerObject>(HLT14_label_);
      std::vector<ic::TriggerObject*> HLTObject15 = event->GetPtrVec<ic::TriggerObject>(HLT15_label_);
      
      bool HLTFired = false;
      
      h_HLTTriggerID->Fill(0);
      if(em_channel == true) h_HLTTriggerID_em->Fill(0); 
      if(et_channel == true) h_HLTTriggerID_et->Fill(0);
      if(mt_channel == true) h_HLTTriggerID_mt->Fill(0);
      if(tt_channel == true) h_HLTTriggerID_tt->Fill(0);
      
      if(HLTObject1.size()!= 0){
          h_HLTTriggerID->Fill(1);
          if(em_channel == true) h_HLTTriggerID_em->Fill(1); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(1);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(1);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(1);
          HLTFired = true;
      }
      if(HLTObject2.size()!= 0){
          h_HLTTriggerID->Fill(2);
          if(em_channel == true) h_HLTTriggerID_em->Fill(2); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(2);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(2);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(2);
          HLTFired = true;
      }
      if(HLTObject3.size()!= 0){
          h_HLTTriggerID->Fill(3);
          if(em_channel == true) h_HLTTriggerID_em->Fill(3); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(3);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(3);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(3);
          HLTFired = true;
      }
      if(HLTObject4.size()!= 0){
          h_HLTTriggerID->Fill(4);
          if(em_channel == true) h_HLTTriggerID_em->Fill(4); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(4);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(4);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(4);
          HLTFired = true;
      }
      if(HLTObject5.size()!= 0){
          h_HLTTriggerID->Fill(5);
          if(em_channel == true) h_HLTTriggerID_em->Fill(5); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(5);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(5);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(5);
          HLTFired = true;
      }
      if(HLTObject6.size()!= 0){
          h_HLTTriggerID->Fill(6);
          if(em_channel == true) h_HLTTriggerID_em->Fill(6); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(6);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(6);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(6);
          HLTFired = true;
      }
      if(HLTObject7.size()!= 0){
          h_HLTTriggerID->Fill(7);
          if(em_channel == true) h_HLTTriggerID_em->Fill(7); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(7);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(7);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(7);
          HLTFired = true;
      }
      if(HLTObject8.size()!= 0){
          h_HLTTriggerID->Fill(8);
          if(em_channel == true) h_HLTTriggerID_em->Fill(8); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(8);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(8);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(8);
          HLTFired = true;
      }
      if(HLTObject9.size()!= 0){
          h_HLTTriggerID->Fill(9);
          if(em_channel == true) h_HLTTriggerID_em->Fill(9); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(9);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(9);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(9);
          HLTFired = true;
      }
      if(HLTObject10.size()!= 0){
          h_HLTTriggerID->Fill(10);
          if(em_channel == true) h_HLTTriggerID_em->Fill(10); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(10);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(10);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(10);
          HLTFired = true;
      }
      if(HLTObject11.size()!= 0){
          h_HLTTriggerID->Fill(11);
          if(em_channel == true) h_HLTTriggerID_em->Fill(11); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(11);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(11);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(11);
          HLTFired = true;
      }
      if(HLTObject12.size()!= 0){
          h_HLTTriggerID->Fill(12);
          if(em_channel == true) h_HLTTriggerID_em->Fill(12); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(12);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(12);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(12);
          HLTFired = true;
      }
      if(HLTObject13.size()!= 0){
          h_HLTTriggerID->Fill(13);
          if(em_channel == true) h_HLTTriggerID_em->Fill(13); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(13);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(13);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(13);
          HLTFired = true;
      }
      if(HLTObject14.size()!= 0){
          h_HLTTriggerID->Fill(14);
          if(em_channel == true) h_HLTTriggerID_em->Fill(14); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(14);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(14);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(14);
          HLTFired = true;
      }
      if(HLTObject15.size()!= 0){
          h_HLTTriggerID->Fill(15);
          if(em_channel == true) h_HLTTriggerID_em->Fill(15); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(15);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(15);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(15);
          HLTFired = true;
      }
      
      if(HLTFired){
          h_HLTTriggerID->Fill(16);
          if(em_channel == true) h_HLTTriggerID_em->Fill(16); 
          if(et_channel == true) h_HLTTriggerID_et->Fill(16);
          if(mt_channel == true) h_HLTTriggerID_mt->Fill(16);
          if(tt_channel == true) h_HLTTriggerID_tt->Fill(16);
      }
      return 0;
  }

  int HLTID::PostAnalysis(){
    return 0;
  }

  void HLTID::PrintInfo(){
    ;
  }
  



}//namespace


