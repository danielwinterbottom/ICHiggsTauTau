#include "TGraphAsymmErrors.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TAxis.h"
#include <iterator>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include "boost/lexical_cast.hpp"
#include "HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"

using namespace ic;

std::vector<double> GetTrigEff(TH1F const* hist, TH1F const* histpasstrigger, std::vector<double> bins, std::vector< std::pair<double,double> >* error){
  std::vector<double> trigeff;
  //Get efficiency for all but last bin

  for(unsigned iBin=0;iBin<bins.size();iBin++){
    double nevents[2];
    double err[2];
    int histbmin=hist->GetXaxis()->FindBin(bins[iBin]);
    int histpasstriggerbmin=histpasstrigger->GetXaxis()->FindBin(bins[iBin]);

    int histbmax,histpasstriggerbmax;
    if(iBin!=(bins.size()-1)){
      histbmax=hist->GetXaxis()->FindBin(bins[iBin+1])-1;
      histpasstriggerbmax=histpasstrigger->GetXaxis()->FindBin(bins[iBin+1])-1;
    }
    else{
      histbmax=(hist->GetNbinsX()+1);
      histpasstriggerbmax=(histpasstrigger->GetNbinsX()+1);
    }

    if((histbmin!=histpasstriggerbmin) && (histbmax !=histpasstriggerbmax)) std::cout<<"Warning binning of histograms with and without trigger is different!"<<std::endl;

    nevents[0]=Integral(hist,histbmin,histbmax);
    nevents[1]=Integral(histpasstrigger,histpasstriggerbmin,histpasstriggerbmax);
    err[0]=Error(hist,histbmin,histbmax);
    err[1]=Error(histpasstrigger,histpasstriggerbmin,histpasstriggerbmax);
    trigeff.push_back(nevents[1]/nevents[0]);
    double errbase=sqrt(pow(err[0]/nevents[0],2)+pow(err[1]/nevents[1],2)) * trigeff[iBin];
    double errorup;
    double errordown;
    if(trigeff[iBin]+errbase>1.) errorup=1-trigeff[iBin];
    else errorup=errbase;
    if(trigeff[iBin]-errbase<0.) errordown=trigeff[iBin];
    else errordown=errbase;
    error->push_back(std::make_pair(errorup,errordown));
  }
  return trigeff;
}

TGraphAsymmErrors MakeTrigEffGraph(std::vector<double> effs, std::vector< std::pair<double,double> > errors, std::vector<double> bins){
  std::vector<double> errup,errdown;
  for(unsigned iBin=0;iBin<errors.size();iBin++){
    errup.push_back(errors[iBin].first);
    errdown.push_back(errors[iBin].second);
  }
  std::vector<double> bincentres,binerrors;
  for(unsigned iBin=0;iBin<bins.size(); iBin++){
    if(iBin==bins.size()-1){
      bincentres.push_back(bins[iBin]+10);
      binerrors.push_back(10);
    }
    else{
      bincentres.push_back((bins[iBin]+bins[iBin+1])/2);
      binerrors.push_back(((bins[iBin+1])-bins[iBin])/2);
    }
  }
  TGraphAsymmErrors outgraph(bincentres.size(),&(bincentres.at(0)),&(effs.at(0)),&(binerrors.at(0)),&(binerrors.at(0)),&(errdown.at(0)),&(errup.at(0)));
  return outgraph;
}

TGraphAsymmErrors Make1DTrigEff(TH1F const* hist, TH1F const* histpasstrigger, std::vector<double> bins){
  std::vector< std::pair<double,double> > error;
  std::vector<double> trigeffs =GetTrigEff(hist, histpasstrigger, bins, &error);
  TGraphAsymmErrors effgraph=MakeTrigEffGraph(trigeffs,error,bins);
  return effgraph;
}

int main(){//main
  std::string folder = "output_trigeff/nunu/";

  double mjjcut=1100;
  double metcut=130;
  double j2ptcut=50;
  double j1ptcut=50;
  double dijet_detacut=4.2;
  double dijet_dphicut=3.2;
  double cjvcut=100;
  
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
  
  std::vector<double> j2pts;
  j2pts.push_back(10);
  j2pts.push_back(20);
  j2pts.push_back(30);
  j2pts.push_back(40);
  j2pts.push_back(50);
  j2pts.push_back(60);
  j2pts.push_back(70);

  std::vector<double> mjjs;
  mjjs.push_back(100);
  mjjs.push_back(200);
  mjjs.push_back(300);
  mjjs.push_back(400);
  mjjs.push_back(500);
  mjjs.push_back(600);
  mjjs.push_back(700);
  mjjs.push_back(800);
  mjjs.push_back(900);
  mjjs.push_back(1000);
  mjjs.push_back(1100);
  mjjs.push_back(1200);
  mjjs.push_back(1300);
  mjjs.push_back(1400);
  mjjs.push_back(1500);
  mjjs.push_back(1600);

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


  for (unsigned iFile = 0; iFile < files.size(); ++iFile) {
    
    TTree *tree;

    tree = (TTree *)tfiles[(files[iFile])]->Get("TrigeffInputTree");
    tree->SetEstimate(1000);

    //Get 3D histo of met,j2pt,mjj
    std::string selection="jet1_pt>"+boost::lexical_cast<std::string>(j1ptcut)+" && n_jets_cjv_30<"+boost::lexical_cast<std::string>(cjvcut)+" && dijet_deta>"+boost::lexical_cast<std::string>(dijet_detacut)+" && dijet_dphi<"+boost::lexical_cast<std::string>(dijet_dphicut);
    std::string prompttriggerselection=" && passtrigger==1";
    std::string parked1triggerselection=" && passparkedtrigger1==1";
    std::string parked2triggerselection=" && passparkedtrigger2==1";

    std::string variable="met:jet2_pt:dijet_M(200,0.,1000.,20,0.,100.,400,0.,2000.)";
    TH3F heffnotrigger=GetShape3D(variable,selection,"","",tree);
    TH3F hefftrigger=GetShape3D(variable,selection+prompttriggerselection,"","",tree);


    //Get 1D histos of met,j2pt,mjj
    std::string metvariable="met(200,0.,1000.)";
    std::string extrametselection=" && jet2_pt>"+boost::lexical_cast<std::string>(j2ptcut)+" && dijet_M>"+boost::lexical_cast<std::string>(mjjcut);
    std::string mjjvariable="dijet_M(400,0.,2000.)";
    std::string extramjjselection=" && jet2_pt>"+boost::lexical_cast<std::string>(j2ptcut)+" && met>"+boost::lexical_cast<std::string>(metcut);
    std::string j2ptvariable="jet2_pt(20,0.,100.)";
    std::string extraj2ptselection=" && met>"+boost::lexical_cast<std::string>(metcut)+" && dijet_M>"+boost::lexical_cast<std::string>(mjjcut);

    TH1F hmet=GetShape(metvariable,selection+extrametselection,"","",tree);
    hmet.Sumw2();
    TH1F hmetpasstrigger=GetShape(metvariable,selection+prompttriggerselection+extrametselection,"","",tree);
    hmetpasstrigger.Sumw2();

    TH1F hj2pt=GetShape(j2ptvariable,selection+extraj2ptselection,"","",tree);
    hj2pt.Sumw2();
    TH1F hj2ptpasstrigger=GetShape(j2ptvariable,selection+prompttriggerselection+extraj2ptselection,"","",tree);
    hj2ptpasstrigger.Sumw2();

    TH1F hmjj=GetShape(mjjvariable,selection+extramjjselection,"","",tree);
    hmjj.Sumw2();
    TH1F hmjjpasstrigger=GetShape(mjjvariable,selection+prompttriggerselection+extramjjselection,"","",tree);
    hmjjpasstrigger.Sumw2();
    
    TCanvas *c1 = new TCanvas("c1","c1");
    

    TGraphAsymmErrors metprompteffgraph=Make1DTrigEff(&hmet,&hmetpasstrigger,mets);
    metprompteffgraph.SetTitle("Trigger Efficiency");
    metprompteffgraph.GetXaxis()->SetTitle("met/GeV");
    metprompteffgraph.GetYaxis()->SetTitle("Efficiency");
    metprompteffgraph.SetMarkerColor(4);                                                                                                   
    metprompteffgraph.SetLineColor(4);                                                                                                               
    metprompteffgraph.SetMarkerStyle(21);
    metprompteffgraph.Draw("AP");
    c1->SaveAs("tmp.pdf");

    TGraphAsymmErrors j2ptprompteffgraph =Make1DTrigEff(&hj2pt,&hj2ptpasstrigger,j2pts);

    TGraphAsymmErrors mjjprompteffgraph =Make1DTrigEff(&hmjj,&hmjjpasstrigger,mjjs);

  }
 
  return 0;
}
