#include <string>
#include <vector>
#include <iostream>
#include <TFile.h>
#include <TH1F.h>

double yield(TH1F* hist, bool weighted){
  if(weighted){
    return(hist->Integral(0,hist->GetNbinsX()+1));
  }
  else{
    return(hist->GetEntries());
  }
}

int yieldgetter(){
  std::string prefix = "../output/";
  bool weighted=true; //IF RAN WITH DOLUMIXSWEIGHT TRUE SETTING THIS TRUE WILL GET WEIGHTED NUMBERS

  //MAKE VECTOR OF SAMPLES TO GET YIELD FROM
  std::vector<std::string> *samples = new std::vector<std::string>;
  samples->push_back("MC_W1JetsToLNu");
  samples->push_back("MC_W2JetsToLNu");
  samples->push_back("MC_W3JetsToLNu");
  samples->push_back("MC_W4JetsToLNu");
  samples->push_back("MC_WJetsToLNu-v1");
  samples->push_back("MC_WJetsToLNu-v2");
  
  for(int i =0;i<samples->size();i++){
    TFile *fnunuenu = new TFile((prefix+"nunu/MET130/"+samples->at(i)+"_enu.root").c_str(),"read");
    TFile *fnunumunu = new TFile((prefix+"nunu/MET130/"+samples->at(i)+"_munu.root").c_str(),"read");
    TFile *fnunutaunu = new TFile((prefix+"nunu/MET130/"+samples->at(i)+"_taunu.root").c_str(),"read");
    TFile *fenu = new TFile((prefix+"enu/MET130/"+samples->at(i)+"_enu.root").c_str(),"read");
    TFile *fmunu = new TFile((prefix+"munu/MET130/"+samples->at(i)+"_munu.root").c_str(),"read");
    TFile *ftaunu = new TFile((prefix+"taunu/MET130/"+samples->at(i)+"_taunu.root").c_str(),"read");
    TH1F *nunuenu;
    TH1F *nunumunu;
    TH1F *nunutaununocjv;
    TH1F *nunutaunucjvpass;
    TH1F *enu;
    TH1F *munu;
    TH1F *taununocjv;
    TH1F *taunucjvpass;
    std::cout<<std::endl<<samples->at(i)<<":"<<std::endl;
    //enu
    std::cout<<"  enu:"<<std::endl;
    fnunuenu->GetObject("DPhiSIGNAL_CJVpass/n_jets",nunuenu);
    fenu->GetObject("DPhiSIGNAL_CJVpass/n_jets",enu);
    std::cout<<"    N_S: "<<yield(nunuenu,weighted)<<std::endl;
    std::cout<<"    N_C: "<<yield(enu,weighted)<<std::endl;
    
    //munu
    std::cout<<"  munu:"<<std::endl;
    fnunumunu->GetObject("DPhiSIGNAL_CJVpass/n_jets",nunumunu);
    fmunu->GetObject("DPhiSIGNAL_CJVpass/n_jets",munu);
    std::cout<<"    N_S: "<<yield(nunumunu,weighted)<<std::endl;
    std::cout<<"    N_C: "<<yield(munu,weighted)<<std::endl;
    
    //taunu
    std::cout<<"  taunu:"<<std::endl;
    fnunutaunu->GetObject("DPhiSIGNAL_noCJV/n_jets",nunutaununocjv);
    ftaunu->GetObject("DPhiSIGNAL_noCJV/n_jets",taununocjv);
    fnunutaunu->GetObject("DPhiSIGNAL_CJVpass/n_jets",nunutaunucjvpass);
    ftaunu->GetObject("DPhiSIGNAL_CJVpass/n_jets",taunucjvpass);
    std::cout<<"    no CJV, no tau reco: "<<yield(nunutaununocjv,weighted)<<std::endl;
    std::cout<<"    no CJV, tau reco: "<<yield(taununocjv,weighted)<<std::endl;
    std::cout<<"    CJV pass, no tau reco: "<<yield(nunutaunucjvpass,weighted)<<std::endl;
    std::cout<<"    CJV pass, tau reco: "<<yield(taunucjvpass,weighted)<<std::endl;
  }
}
