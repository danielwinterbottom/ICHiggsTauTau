#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TAxis.h"
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

double IntegralWithError(TH1F const* hist, int binmin, int binmax, double err){
  if (hist) {
    double ltmp =hist->IntegralAndError(binmin, binmax, err);
    if (ltmp<0 || ltmp != ltmp) {
      std::cout << " -- Warning: integral is " << ltmp << ". Setting to 0." << std::endl;
      ltmp=0;
    }
    return ltmp;
  }
  else return 0;
}

double Integral(TH1F const* hist, int binmin, int binmax){
  if (hist) {
    double ltmp =hist->Integral(binmin, binmax);
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

std::vector<double> GetTrigEff(TH1F const* hist, TH1F const* histpasstrigger, std::vector<double> bins){
  std::vector<double> trigeff;
  //Get efficiency for all but last bin
  double histtot=Integral(hist,0,hist->GetXaxis()->FindBin(bins[0])-1);
  double histpasstriggertot=Integral(histpasstrigger,0,histpasstrigger->GetXaxis()->FindBin(bins[0])-1);;

  for(int iBin=0;iBin<(bins.size()-1);iBin++){
    double nevents[2];
    int histbmin=hist->GetXaxis()->FindBin(bins[iBin]);
    int histbmax=hist->GetXaxis()->FindBin(bins[iBin+1])-1;

    int histpasstriggerbmin=histpasstrigger->GetXaxis()->FindBin(bins[iBin]);
    int histpasstriggerbmax=histpasstrigger->GetXaxis()->FindBin(bins[iBin+1])-1;

    if((histbmin!=histpasstriggerbmin) && (histbmax !=histpasstriggerbmax)) std::cout<<"Warning binning of histograms with and without trigger is different!"<<std::endl;

    nevents[0]=Integral(hist,histbmin,histbmax); std::cout<<nevents[0]<<std::endl;
    nevents[1]=Integral(histpasstrigger,histpasstriggerbmin,histpasstriggerbmax); std::cout<<nevents[1]<<std::endl;
    trigeff.push_back(nevents[1]/nevents[0]);
    histtot+=nevents[0];
    histpasstriggertot+=nevents[1];
  }
  //For last bin do integral to infinity
  double nevents[2];
  int histbmin=hist->GetXaxis()->FindBin(bins[bins.size()-1]);
  int histpasstriggerbmin=hist->GetXaxis()->FindBin(bins[bins.size()-1]);

  if(histbmin!=histpasstriggerbmin) std::cout<<"Warning binning of histograms with and without trigger is different!"<<std::endl;

  nevents[0]=Integral(hist,histbmin,(hist->GetNbinsX()+1));
  nevents[1]=Integral(histpasstrigger,histpasstriggerbmin,(histpasstrigger->GetNbinsX()+1));
  trigeff.push_back(nevents[1]/nevents[0]);
  histtot+=nevents[0];
  histpasstriggertot+=nevents[1];

  std::cout<<histtot<<" "<<histpasstriggertot<<std::endl;
  std::cout<<Integral(hist)<<" "<<Integral(histpasstrigger)<<std::endl;


  return trigeff;
}

int main(){//main
  std::string folder = "output_trigeff/nunu/";

  double mjjcut=1100;
  double metcut=130;
  double j2ptcut=50;
  double j1ptcut=50;
  double dijet_detacut=4.2;
  double dijet_dphicut=1.0;
  
  std::vector<std::string> files;
  files.push_back("SingleMu_SingleMu-2012A-22Jan2013-v1"); 
  files.push_back("SingleMu_SingleMu-2012B-22Jan2013-v1"); 
  files.push_back("SingleMu_SingleMu-2012C-22Jan2013-v1"); 
  files.push_back("SingleMu_SingleMu-2012D-22Jan2013-v1"); 
  
  std::vector<double> mets;
  mets.push_back(40);
  mets.push_back(60);
  mets.push_back(80);
  mets.push_back(100);
  mets.push_back(110);
  mets.push_back(120);
  mets.push_back(130);
  mets.push_back(140);
  mets.push_back(150);
  mets.push_back(160);
  mets.push_back(170);
  mets.push_back(180);
  mets.push_back(190);
  mets.push_back(200);
  
  std::vector<int> j2pts;
  j2pts.push_back(10);
  j2pts.push_back(20);
  j2pts.push_back(30);
  j2pts.push_back(40);
  j2pts.push_back(50);
  j2pts.push_back(60);
  j2pts.push_back(70);

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


  double neventsmet[4][2][mets.size()][2];
  double trigeffmet[4][mets.size()][2];

  double neventsj2pt[4][2][mets.size()][2];
  double trigeffj2pt[4][mets.size()][2];

  for (unsigned iFile = 0; iFile < files.size(); ++iFile) {
    
    TTree *tree;

    TH1F *hpasstrigger;

    TH1F *hmet;
    TH1F *hj2pt;
    TH1F *hmjj;

    TH1F *hj2ptpasstrigger;
    TH1F *hmetpasstrigger;
    TH1F *hmjjpasstrigger;
    
    double met;
    double j1pt;
    double j2pt;
    double mjj;
    double dijet_deta;
    double dijet_dphi;
    unsigned n_jets_cjv_30; 
    double passtrigger;
    

    tree = (TTree *)tfiles[(files[iFile])]->Get("TrigeffInputTree");
    tree->SetBranchAddress("met",&met);
    tree->SetBranchAddress("jet1_pt",&j1pt);
    tree->SetBranchAddress("jet2_pt",&j2pt);
    tree->SetBranchAddress("dijet_M",&mjj);
    tree->SetBranchAddress("dijet_deta",&dijet_deta);
    tree->SetBranchAddress("dijet_dphi",&dijet_dphi);
    tree->SetBranchAddress("n_jets_cjv_30",&n_jets_cjv_30);
    tree->SetBranchAddress("passtrigger",&passtrigger);

    
    hmet = new TH1F("hmet","met in GeV",1000,0,1000);
    hmet->Sumw2();
    hj2pt =new TH1F("hj2pt","j2pt in GeV",200,0,200);
    hj2pt->Sumw2();
    hmjj =new TH1F("hmjj","mjj in GeV",2000,0,2000);
    hmjj->Sumw2();
    hpasstrigger =new TH1F("hpasstrigger","Does the event pass the trigger",2,-2,2);
    hpasstrigger->Sumw2();
    hj2ptpasstrigger = new TH1F("hj2ptpasstrigger","jet 2pt passing trigger",200,0,200);
    hj2ptpasstrigger->Sumw2();
    hmetpasstrigger = new TH1F("hmetpasstrigger","met passing trigger",1000,0,1000);
    hmetpasstrigger->Sumw2();
    hmjjpasstrigger = new TH1F("hmjjpasstrigger","mjj passing trigger",2000,0,2000);
    hmjjpasstrigger->Sumw2();

    int nentries = tree->GetEntries();
    for(int iEntry = 0;iEntry<nentries;iEntry++){
      tree->GetEntry(iEntry);
      if(j1pt>j1ptcut && n_jets_cjv_30<1 && dijet_deta>dijet_detacut && dijet_dphi<dijet_dphicut){
	if(mjj>mjjcut && j2pt>j2ptcut){
	  hmet->Fill(met);
	  if(passtrigger==1) hmetpasstrigger->Fill(met);
	}
	if(mjj>mjjcut && met>metcut){
	  hj2pt->Fill(j2pt);
	  if(passtrigger==1) hj2ptpasstrigger->Fill(j2pt);
	}
      }
      hpasstrigger->Fill(passtrigger);

    }
    hmet->Draw("");
    hj2pt->Draw("");
    //hmetj2pt->Draw("col3");
    hmetpasstrigger->Draw("col3");
    //    hpasstrigger->Draw("");
    hmjj->Draw("");

    std::vector<double> mettrigeffs = GetTrigEff(hmet,hmetpasstrigger,mets);
    for(unsigned iEff=0;iEff<mettrigeffs.size();iEff++){
      std::cout<<"trig eff for met: "<<mets[iEff]<<" is: "<<mettrigeffs[iEff]<<std::endl;
    }
    
//     for(int iMet=0;iMet<mets.size();iMet++){
//       if(iMet==(mets.size()-1)){
// 	neventsmet[iFile][0][iMet][0]=IntegralWithError(hmet,mets[iMet],(hmet->GetNbinsX()+1));
// 	neventsmet[iFile][1][iMet][0]=IntegralWithError(hmetpasstrigger,mets[iMet],(hmetpasstrigger->GetNbinsX()+1));
//       }
//       else{
// 	neventsmet[iFile][0][iMet][0]=IntegralWithError(hmet,mets[iMet],mets[iMet+1]-1);
// 	neventsmet[iFile][1][iMet][0]=IntegralWithError(hmetpasstrigger,mets[iMet],mets[iMet+1]-1);
//       }
//       trigeffmet[iFile][iMet][0]=neventsmet[iFile][1][iMet]/neventsmet[iFile][0][iMet];
//       std::cout<<"trig eff for met: "<<mets[iMet]<<" is: "<<trigeffmet[iFile][iMet]<<std::endl;
//     }

    for(unsigned iJ2pt=0;iJ2pt<j2pts.size();iJ2pt++){
      if(iJ2pt==(j2pts.size()-1)){
	neventsj2pt[iFile][0][iJ2pt][0]=hj2pt->Integral(j2pts[iJ2pt],(hj2pt->GetNbinsX()+1));
	neventsj2pt[iFile][1][iJ2pt][0]=hj2ptpasstrigger->Integral(j2pts[iJ2pt],(hj2ptpasstrigger->GetNbinsX()+1));
      }
      else{
	neventsj2pt[iFile][0][iJ2pt][0]=hj2pt->Integral(j2pts[iJ2pt],j2pts[iJ2pt+1]-1);
	neventsj2pt[iFile][1][iJ2pt][0]=hj2ptpasstrigger->Integral(j2pts[iJ2pt],j2pts[iJ2pt+1]-1);
      }
      trigeffj2pt[iFile][iJ2pt][0]=neventsj2pt[iFile][1][iJ2pt][0]/neventsj2pt[iFile][0][iJ2pt][0];
      std::cout<<"trig eff for j2pt: "<<j2pts[iJ2pt]<<" is: "<<trigeffj2pt[iFile][iJ2pt][0]<<std::endl;
    }
  }
 
//   bool nostat[4][3][mets.size()];
//   double trigeffj2pt[4][3][mets.size()];
//   double trigeffj2pterr[4][3][mets.size()];

//   std::string eraname[4]={"A","B","C","D"};
//   std::string cjvname[3]={"Passing","Failing","No"};

//   for (unsigned iEra = 0;iEra<4;iEra++){
//     std::cout<<"Run "<<eraname[iEra]<<":"<<std::endl;
//     for (unsigned iCJV=0;iCJV<3;iCJV++){
//       std::cout<<"   Events "<<cjvname[iCJV]<<" CJV:"<<std::endl;
//       for (unsigned iMet=0;iMet<mets.size();iMet++){
// 	trigeffj2pt[iEra][iCJV][iMet]=neventsj2pt[iEra][iCJV+3][iMet]/neventsj2pt[iEra][iCJV][iMet];
// 	trigeffj2pterr[iEra][iCJV][iMet]=sqrt(pow(sqrt(neventsj2pt[iEra][iCJV+3][iMet])/neventsj2pt[iEra][iCJV+3][iMet],2)+pow(sqrt(neventsj2pt[iEra][iCJV][iMet])/neventsj2pt[iEra][iCJV][iMet],2))*trigeffj2pt[iEra][iCJV][iMet];

// 	if(neventsj2pt[iEra][iCJV][iMet]==0) nostat[iEra][iCJV][iMet]=true;
// 	else nostat[iEra][iCJV][iMet]=false;

// 	//Output
// 	if(nostat[iEra][iCJV][iMet]==false){
// 	  if(iMet<(mets.size()-1)){
// 	    std::cout<<"      Trigger efficiency for MET "<<mets[iMet]<<"-"<<mets[iMet+1]<<" is: "<<trigeffj2pt[iEra][iCJV][iMet]<<"+/-"<<trigeffj2pterr[iEra][iCJV][iMet]<<std::endl;
// 	  }
// 	  else{
// 	    std::cout<<"      Trigger efficiency for MET "<<mets[iMet]<<"-inf"<<" is: "<<trigeffj2pt[iEra][iCJV][iMet]<<"+/-"<<trigeffj2pterr[iEra][iCJV][iMet]<<std::endl;
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
// 	std::cout<<"       Num = "<<neventsj2pt[iEra][iCJV+3][iMet]<<", Den = "<<neventsj2pt[iEra][iCJV][iMet]<<std::endl;
//       }
//     }
//   }

  return 0;
}
