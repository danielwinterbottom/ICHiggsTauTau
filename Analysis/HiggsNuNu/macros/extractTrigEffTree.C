#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include <iostream>
#include <vector>
#include <map>
double Integral(TH1F const* hist) {
  if (hist) {
    double ltmp =hist->Integral(0, hist->GetNbinsX() + 1);
    if (ltmp<0 || ltmp != ltmp) {
      std::cout << " -- Warning: integral is " << ltmp << ". Setting to 0." << std::endl;
      ltmp=0;
    }
    return ltmp;
  }
  else return 0;
}

double Error(TH1F const* hist) {
  double err = 0.0;
  if (hist) {
    hist->IntegralAndError(0, hist->GetNbinsX() + 1, err);
    if (err<0 || err != err) {
      std::cout << " -- Warning: error on integral is " << err << ". Setting to 0." << std::endl;
      err=0;
    }
  }
  return err;
}

int extractTrigEffTree(){//main
  std::string folder = "../output_trigeff/nunu/";
  
  
  std::vector<std::string> files;
  files.push_back("SingleMu_SingleMu-2012A-22Jan2013-v1"); 
  //   files.push_back("SingleMu_SingleMu-2012B-22Jan2013-v1"); 
  //   files.push_back("SingleMu_SingleMu-2012C-22Jan2013-v1"); 
  //   files.push_back("SingleMu_SingleMu-2012D-22Jan2013-v1"); 
  
  std::vector<std::string> mets;
  mets.push_back("40");
  mets.push_back("60");
  mets.push_back("80");
  mets.push_back("100");
  mets.push_back("110");
  mets.push_back("120");
  mets.push_back("130");
  mets.push_back("140");
  mets.push_back("150");
  mets.push_back("160");
  mets.push_back("170");
  mets.push_back("180");
  mets.push_back("190");
  mets.push_back("200");
  
  std::map<std::string, TFile *> tfiles;
  for (unsigned iFile = 0; iFile < files.size(); iFile++) {
    std::string filename = (files[iFile]+".root");
    TFile * tmp = new TFile((folder+"/"+filename).c_str());
    if (!tmp) {
      std::cerr << "Warning, file " << filename << " could not be opened." << std::endl;
    }
    else {
      tfiles[(files[iFile])] = tmp;
    }
  }


  double nevents[4][6][mets.size()];

  for (unsigned iFile = 0; iFile < files.size(); ++iFile) {
    
    TTree *tree;
    TBranch *metBranch;
    TH1F *hmet;
    TBranch *j2ptBranch;
    TH1F *hj2pt;
    TBranch *passtriggerBranch;
    TH1F *hpasstrigger;

    TH2F *hmetj2pt;
    TH2F *hmetpasstrigger;
    
    
    double met;
    double j2pt;
    bool passtrigger;
    

    tree = (TTree *)tfiles[(files[iFile])]->Get("TrigeffInputTree");
    tree->SetBranchAddress("met",&met,&metBranch);
    tree->SetBranchAddress("jet2_pt",&j2pt,&j2ptBranch);
    tree->SetBranchAddress("passtrigger",&passtrigger,&passtriggerBranch);

    
    hmet = new TH1F("hmet","met in GeV",10,0,200);
    hmet->Sumw2();
    hj2pt =new TH1F("hj2pt","j2pt in GeV",10,0,100);
    hj2pt->Sumw2();
    hpasstrigger =new TH1F("hpasstrigger","Does the event pass the trigger",2,-1,1);
    hpasstrigger->Sumw2();
    hmetj2pt = new TH2F("hmetj2pt","met against jet 2pt in GeV",10,0,200,10,0,100);
    hmetj2pt->Sumw2();
    hmetpasstrigger = new TH2F("hmetpasstrigger","met against trigger",10,0,200,2,-1,1);
    hmetpasstrigger->Sumw2();

    int nentries = tree->GetEntries();
    for(int iEntry = 0;iEntry<nentries;iEntry++){
      metBranch->GetEntry(iEntry);
      j2ptBranch->GetEntry(iEntry);
      passtriggerBranch->GetEntry(iEntry);
      double triggerdouble;
      if(passtrigger==true)triggerdouble=1;
      else triggerdouble=-1;
      hmet->Fill(met);
      hj2pt->Fill(j2pt);
      hpasstrigger->Fill(triggerdouble);
      hmetj2pt->Fill(met,j2pt);
      hmetpasstrigger->Fill(j2pt,triggerdouble);
    }
    hmet->Draw("");
    hj2pt->Draw("");
    hmetj2pt->Draw("col3");
    hmetpasstrigger->Draw("col3");
  }
 
//   bool nostat[4][3][mets.size()];
//   double trigeff[4][3][mets.size()];
//   double trigefferr[4][3][mets.size()];

//   std::string eraname[4]={"A","B","C","D"};
//   std::string cjvname[3]={"Passing","Failing","No"};

//   for (unsigned iEra = 0;iEra<4;iEra++){
//     std::cout<<"Run "<<eraname[iEra]<<":"<<std::endl;
//     for (unsigned iCJV=0;iCJV<3;iCJV++){
//       std::cout<<"   Events "<<cjvname[iCJV]<<" CJV:"<<std::endl;
//       for (unsigned iMet=0;iMet<mets.size();iMet++){
// 	trigeff[iEra][iCJV][iMet]=nevents[iEra][iCJV+3][iMet]/nevents[iEra][iCJV][iMet];
// 	trigefferr[iEra][iCJV][iMet]=sqrt(pow(sqrt(nevents[iEra][iCJV+3][iMet])/nevents[iEra][iCJV+3][iMet],2)+pow(sqrt(nevents[iEra][iCJV][iMet])/nevents[iEra][iCJV][iMet],2))*trigeff[iEra][iCJV][iMet];

// 	if(nevents[iEra][iCJV][iMet]==0) nostat[iEra][iCJV][iMet]=true;
// 	else nostat[iEra][iCJV][iMet]=false;

// 	//Output
// 	if(nostat[iEra][iCJV][iMet]==false){
// 	  if(iMet<(mets.size()-1)){
// 	    std::cout<<"      Trigger efficiency for MET "<<mets[iMet]<<"-"<<mets[iMet+1]<<" is: "<<trigeff[iEra][iCJV][iMet]<<"+/-"<<trigefferr[iEra][iCJV][iMet]<<std::endl;
// 	  }
// 	  else{
// 	    std::cout<<"      Trigger efficiency for MET "<<mets[iMet]<<"-inf"<<" is: "<<trigeff[iEra][iCJV][iMet]<<"+/-"<<trigefferr[iEra][iCJV][iMet]<<std::endl;
// 	  }
// 	}
// 	else{
// 	  if(iMet<(mets.size()-1)){
// 	    std::cout<<"      Not enough stat for MET "<<mets[iMet]<<"-"<<mets[iMet+1]<<std::endl;
// 	  }
// 	  else{
// 	    std::cout<<"      Not enough stat for MET "<<mets[iMet]<<"-inf"<<std::endl;
// 	  }
// 	}
// 	std::cout<<"       Num = "<<nevents[iEra][iCJV+3][iMet]<<", Den = "<<nevents[iEra][iCJV][iMet]<<std::endl;
//       }
//     }
//   }

  return 0;
}
