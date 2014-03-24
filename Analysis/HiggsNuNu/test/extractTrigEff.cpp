#include "TGraphAsymmErrors.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TAxis.h"
#include "TLegend.h"
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
    if(nevents[0]!=0) trigeff.push_back(nevents[1]/nevents[0]);
    else{
      trigeff.push_back(0.);
      std::cout<<"0 events even without trigger setting efficiency to 0"<<std::endl;
    }
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
      bincentres.push_back(bins[iBin]+0.1);
      binerrors.push_back(0.05);
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
  std::string folder = "output_trigeff_alltrigs/nunu/";

  double mjjcut=1000;
  double metcut=130;
  double j2ptcut=55;
  double j1ptcut=55;
  double dijet_detacut=4.2;
  double dijet_dphicut=3.2;
  double cjvcut=1;

  std::string outfolder="trigeffplotsnocuts/";
  
  std::vector<std::string> files;
  files.push_back("SingleMu_SingleMu-2012A-22Jan2013-v1"); 
  files.push_back("SingleMu_SingleMu-2012B-22Jan2013-v1"); 
  files.push_back("SingleMu_SingleMu-2012C-22Jan2013-v1"); 
  files.push_back("SingleMu_SingleMu-2012D-22Jan2013-v1"); 
  
  std::vector<std::string> filename;
  filename.push_back("Run A");
  filename.push_back("Run B");
  filename.push_back("Run C");
  filename.push_back("Run D");

  std::string baseselection="jet1_pt>"+boost::lexical_cast<std::string>(j1ptcut)+" && n_jets_cjv_30<"+boost::lexical_cast<std::string>(cjvcut)+" && dijet_dphi<"+boost::lexical_cast<std::string>(dijet_dphicut);
  
  std::string multidvariable="met:jet2_pt:dijet_M(200,0.,1000.,20,0.,100.,400,0.,2000.)";

  std::vector<std::string> triggerselection;
  triggerselection.push_back(" && passtrigger==1");
  triggerselection.push_back(" && passparkedtrigger1==1");
  triggerselection.push_back(" && passparkedtrigger2==1");

  std::vector<std::string> triggernames;
  triggernames.push_back("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets");
  triggernames.push_back("HLT_DiJet35_MJJ700_AllJets_DEta3p5_VBF");
  triggernames.push_back("HLT_DiJet30_MJJ700_AllJets_DEta3p5_VBF");

  std::vector<std::string> varnames;
  varnames.push_back("met");
  varnames.push_back("mjj");
  varnames.push_back("j2pt");
  varnames.push_back("deta");

  std::vector<std::string> varvariable;
  varvariable.push_back("met(200,0.,1000.)");
  varvariable.push_back("dijet_M(400,0.,2000.)");
  varvariable.push_back("jet2_pt(20,0.,100.)");
  varvariable.push_back("dijet_deta(100,0.,10.)");

  std::vector<std::string> varextraselection;
  varextraselection.push_back(" && jet2_pt>"+boost::lexical_cast<std::string>(j2ptcut)+" && dijet_M>"+boost::lexical_cast<std::string>(mjjcut)+" && dijet_de\
ta>"+boost::lexical_cast<std::string>(dijet_detacut));
  varextraselection.push_back(" && jet2_pt>"+boost::lexical_cast<std::string>(j2ptcut)+" && met>"+boost::lexical_cast<std::string>(metcut)+" && dijet_de\
ta>"+boost::lexical_cast<std::string>(dijet_detacut));
  varextraselection.push_back(" && met>"+boost::lexical_cast<std::string>(metcut)+" && dijet_M>"+boost::lexical_cast<std::string>(mjjcut)+" && dijet_de\
ta>"+boost::lexical_cast<std::string>(dijet_detacut));
  varextraselection.push_back(" && met>"+boost::lexical_cast<std::string>(metcut)+" && dijet_M>"+boost::lexical_cast<std::string>(mjjcut)+" && jet2_pt>"+boost::lexical_cast<std::string>(j2ptcut));
  
  
  std::vector<std::string> varlatex;
  varlatex.push_back("met");
  varlatex.push_back("m_{jj}");
  varlatex.push_back("jet_{2} p_{T}");
  varlatex.push_back("#Delta#eta_{jj}");

  std::vector<double> varbins[varnames.size()];
  varbins[0].push_back(40);
  varbins[0].push_back(60);
  varbins[0].push_back(80);
  varbins[0].push_back(100);
  varbins[0].push_back(110);
  varbins[0].push_back(120);
  varbins[0].push_back(130);
  varbins[0].push_back(140);
  varbins[0].push_back(150);
  varbins[0].push_back(160);
  varbins[0].push_back(170);
  varbins[0].push_back(180);
  varbins[0].push_back(190);
  varbins[0].push_back(200);

  varbins[1].push_back(100);
  varbins[1].push_back(200);
  varbins[1].push_back(300);
  varbins[1].push_back(400);
  varbins[1].push_back(500);
  varbins[1].push_back(600);
  varbins[1].push_back(700);
  varbins[1].push_back(800);
  varbins[1].push_back(900);
  varbins[1].push_back(1000);
  varbins[1].push_back(1100);
  varbins[1].push_back(1200);
  varbins[1].push_back(1300);
  varbins[1].push_back(1400);
  varbins[1].push_back(1500);
  varbins[1].push_back(1600);

  varbins[2].push_back(10);
  varbins[2].push_back(20);
  varbins[2].push_back(30);
  varbins[2].push_back(40);
  varbins[2].push_back(50);
  varbins[2].push_back(60);
  varbins[2].push_back(70);
  varbins[2].push_back(80);
  varbins[2].push_back(90);
  varbins[2].push_back(100);

  varbins[3].push_back(3.1);
  varbins[3].push_back(3.3);
  varbins[3].push_back(3.5);
  varbins[3].push_back(3.6);
  varbins[3].push_back(3.7);
  varbins[3].push_back(3.8);
  varbins[3].push_back(3.9);
  varbins[3].push_back(4.0);
  varbins[3].push_back(4.1);
  varbins[3].push_back(4.2);
  varbins[3].push_back(4.3);
  varbins[3].push_back(4.4);
  varbins[3].push_back(4.5);
  varbins[3].push_back(4.6);
  varbins[3].push_back(4.7);
  varbins[3].push_back(4.8);
  varbins[3].push_back(4.9);
  varbins[3].push_back(5.0);

  std::cout<<"Opening TFiles";

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
    std::cout<<".";
  }
  std::cout<<std::endl;

  TH3F heffnotrigger[4],hefftrigger[4],heffparked1trigger[4],heffparked2trigger[4];
  TH1F hvar[varnames.size()][4], hvarpasstrigger[varnames.size()][triggernames.size()][4];

  for (unsigned iFile = 0; iFile < files.size(); ++iFile) {
    std::cout<<filename[iFile]<<std::endl;
    TTree *tree;

    tree = (TTree *)tfiles[(files[iFile])]->Get("TrigeffInputTree");
    tree->SetEstimate(1000);

    std::cout<<"Getting 3D Histograms";

    //Get 3D histo of met,j2pt,mjj

    heffnotrigger[iFile]=GetShape3D(multidvariable,baseselection,"","",tree);
    std::cout<<".";
    hefftrigger[iFile]=GetShape3D(multidvariable,baseselection+triggerselection[0],"","",tree);
    std::cout<<".";
    heffparked1trigger[iFile]=GetShape3D(multidvariable,baseselection+triggerselection[1],"","",tree);
    std::cout<<".";
    heffparked2trigger[iFile]=GetShape3D(multidvariable,baseselection+triggerselection[2],"","",tree);
    std::cout<<std::endl;


    //Get 1D histos of met,j2pt,mjj
    std::cout<<"Getting 1D Histograms:"<<std::endl;
    for(unsigned iVar=0; iVar<varnames.size();iVar++){
      std::cout<<"  "<<varnames[iVar]<<std::endl;
      std::cout<<"    No trigger"<<std::endl;
      hvar[iVar][iFile]=GetShape(varvariable[iVar],baseselection+varextraselection[iVar],"","",tree);
      hvar[iVar][iFile].Sumw2();
      for(unsigned iTrigger=0;iTrigger<triggernames.size();iTrigger++){
	std::cout<<"    Trigger "<<iTrigger<<std::endl;
	hvarpasstrigger[iVar][iTrigger][iFile]=GetShape(varvariable[iVar],baseselection+varextraselection[iVar]+triggerselection[iTrigger],"","",tree);
	hvarpasstrigger[iVar][iTrigger][iFile].Sumw2();
      }
    }
  }

  std::cout<<"Calculating and drawing efficiencies"<<std::endl;
  
  TH1F hvarD[varnames.size()],hvarBCD[varnames.size()],hvarABCD[varnames.size()];
  TH1F hvarpasstriggerABCD[varnames.size()],hvarpassparked1triggerBCD[varnames.size()],hvarpassparked2triggerD[varnames.size()];
  TGraphAsymmErrors varprompteffgraph[varnames.size()],varparked1effgraph[varnames.size()],varparked2effgraph[varnames.size()];

  TCanvas *c1 = new TCanvas("c1","c1");
  for(unsigned iVar=0;iVar<varnames.size();iVar++){
    c1->Clear();
    hvarD[iVar]=hvar[iVar][3];
    hvarBCD[iVar]=hvar[iVar][3];
    hvarBCD[iVar].Add(&hvar[iVar][2]);
    hvarBCD[iVar].Add(&hvar[iVar][1]);
    hvarABCD[iVar]=hvarBCD[iVar];
    hvarABCD[iVar].Add(&hvar[iVar][0]);

    hvarpasstriggerABCD[iVar]=hvarpasstrigger[iVar][0][0];
    hvarpasstriggerABCD[iVar].Add(&hvarpasstrigger[iVar][0][1]);
    hvarpasstriggerABCD[iVar].Add(&hvarpasstrigger[iVar][0][2]);
    hvarpasstriggerABCD[iVar].Add(&hvarpasstrigger[iVar][0][3]);
    hvarpassparked1triggerBCD[iVar]=hvarpasstrigger[iVar][1][1];
    hvarpassparked1triggerBCD[iVar].Add(&hvarpasstrigger[iVar][1][2]);
    hvarpassparked1triggerBCD[iVar].Add(&hvarpasstrigger[iVar][1][3]);
    hvarpassparked2triggerD[iVar]=hvarpasstrigger[iVar][2][3];

    c1->SetGrid();
    
    varprompteffgraph[iVar]=Make1DTrigEff(&hvarABCD[iVar],&hvarpasstriggerABCD[iVar],varbins[iVar]);
    varparked1effgraph[iVar]=Make1DTrigEff(&hvarBCD[iVar],&hvarpassparked1triggerBCD[iVar],varbins[iVar]);
    varparked2effgraph[iVar]=Make1DTrigEff(&hvarD[iVar],&hvarpassparked2triggerD[iVar],varbins[iVar]);

    varprompteffgraph[iVar].SetTitle("Trigger Efficiency");
    varprompteffgraph[iVar].GetXaxis()->SetTitle((varlatex[iVar]+"/GeV").c_str());
    varprompteffgraph[iVar].GetYaxis()->SetTitle("Efficiency");
    varprompteffgraph[iVar].SetMarkerColor(4);                                                                                                   
    varprompteffgraph[iVar].SetLineColor(4);
    varprompteffgraph[iVar].SetMarkerStyle(20);
    varprompteffgraph[iVar].SetMarkerSize(1);
    varprompteffgraph[iVar].Draw("AP");
    
    varparked1effgraph[iVar].SetMarkerColor(6);
    varparked1effgraph[iVar].SetLineColor(6);
    varparked1effgraph[iVar].SetMarkerStyle(20);
    varparked1effgraph[iVar].SetMarkerSize(0.9);
    varparked1effgraph[iVar].Draw("P");
    
    varparked2effgraph[iVar].SetMarkerColor(2);
    varparked2effgraph[iVar].SetLineColor(2);
    varparked2effgraph[iVar].SetMarkerStyle(20);
    varparked2effgraph[iVar].SetMarkerSize(0.8);
    varparked2effgraph[iVar].Draw("P");
    
    TLegend* varleg=new TLegend(0.5,0.2,0.9,0.3);
    varleg->AddEntry(&varprompteffgraph[iVar],triggernames[0].c_str(),"p");
    varleg->AddEntry(&varparked1effgraph[iVar],triggernames[1].c_str(),"p");
    varleg->AddEntry(&varparked2effgraph[iVar],triggernames[2].c_str(),"p");
    
    varleg->Draw("same");
    c1->SaveAs((outfolder+varnames[iVar]+"efficiency.pdf").c_str());
  }

  return 0;
}
