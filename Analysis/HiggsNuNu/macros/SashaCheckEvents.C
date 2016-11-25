#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>

#include "boost/lexical_cast.hpp"
#include "TFile.h"
#include "TChain.h"
#include "TMath.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TF1.h"
#include "TString.h"
#include "TLatex.h"
#include "TGaxis.h"
#include "TTreeFormula.h"


int SashaCheckEvents(){

  //  double btagthreshold=0.679;
  double btagthreshold=0.898;
  double centraljetthreshold=30;
  double forwardjetthreshold=30;
  bool cutforward=false;

  // std::string lMuCut = "mu1_pt>25&&mu2_pt>25";
  // //std::string lCentralCut="((jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_csv>"+boost::lexical_cast<std::string>(btagthreshold)+")";
  // //std::string lCentralCut="((jet1_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_csv>"+boost::lexical_cast<std::string>(btagthreshold)+"&&!((jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet4_eta<2.4&&jet4_eta>-2.4)&&jet4_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"))||(jet2_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_csv>"+boost::lexical_cast<std::string>(btagthreshold)+"&&!((jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet4_eta<2.4&&jet4_eta>-2.4)&&jet4_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"))||(jet3_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_csv>"+boost::lexical_cast<std::string>(btagthreshold)+"&&!((jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet4_eta<2.4&&jet4_eta>-2.4)&&jet4_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")))";
  // std::string lCentralCut="((jet1_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_csv>"+boost::lexical_cast<std::string>(btagthreshold)+")||(jet2_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_csv>"+boost::lexical_cast<std::string>(btagthreshold)+")||(jet3_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_csv>"+boost::lexical_cast<std::string>(btagthreshold)+"))";
  // std::string lForwardCut="(((jet1_eta>=2.4||jet1_eta<=-2.4)&&jet1_pt>"+boost::lexical_cast<std::string>(forwardjetthreshold)+")||((jet2_eta>=2.4||jet2_eta<=-2.4)&&jet2_pt>"+boost::lexical_cast<std::string>(forwardjetthreshold)+")||((jet3_eta>=2.4||jet3_eta<=-2.4)&&jet3_pt>"+boost::lexical_cast<std::string>(forwardjetthreshold)+")||((jet4_eta>=2.4||jet4_eta<=-2.4)&&jet4_pt>"+boost::lexical_cast<std::string>(forwardjetthreshold)+"))";
  //  unsigned nV=1;
  //  std::string histName = "m_mumu";

  std::vector<unsigned> run;
  std::vector<unsigned> lumi;
  std::vector<unsigned> event;
  std::vector<double> mmumu;

  std::ifstream infile("eventlist.txt");
  std::ofstream outfile;
  outfile.open("sasahaevents.txt");

  unsigned run_,lumi_,event_;
  double mmumu_;
  std::cout<<"Looking for the following events:"<<std::endl;
  while (infile >> run_ >> event_ >> lumi_ >> mmumu_){
    run.push_back(run_);
    lumi.push_back(lumi_);
    event.push_back(event_);
    mmumu.push_back(mmumu_);
    std::cout<<"  "<<run_<<" "<<lumi_<<" "<<event_<<" "<<mmumu_<<std::endl;
  }


  std::vector<std::string> files;
  files.push_back("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_070815/SINGLEMU_SingleMu-2012A-22Jan2013-v1.root");
  files.push_back("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_070815/SINGLEMU_SingleMu-2012B-22Jan2013-v1-split1.root");
  files.push_back("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_070815/SINGLEMU_SingleMu-2012B-22Jan2013-v1-split2.root");
  files.push_back("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_070815/SINGLEMU_SingleMu-2012B-22Jan2013-v1-split3.root");
  files.push_back("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_070815/SINGLEMU_SingleMu-2012B-22Jan2013-v1-split4.root");
  files.push_back("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_070815/SINGLEMU_SingleMu-2012C-22Jan2013-v1-split1.root");
  files.push_back("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_070815/SINGLEMU_SingleMu-2012C-22Jan2013-v1-split2.root");
  files.push_back("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_070815/SINGLEMU_SingleMu-2012C-22Jan2013-v1-split3.root");
  files.push_back("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_070815/SINGLEMU_SingleMu-2012C-22Jan2013-v1-split4.root");
  files.push_back("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_070815/SINGLEMU_SingleMu-2012D-22Jan2013-v1-split1.root");
  files.push_back("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_070815/SINGLEMU_SingleMu-2012D-22Jan2013-v1-split2.root");
  files.push_back("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_070815/SINGLEMU_SingleMu-2012D-22Jan2013-v1-split3.root");
  files.push_back("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_070815/SINGLEMU_SingleMu-2012D-22Jan2013-v1-split4.root");

  std::cout<<"Searching:"<<std::endl;
  for(unsigned iFile=0;iFile<files.size();iFile++){
    std::cout<<files[iFile]<<std::endl;
    TFile* fS = TFile::Open(files[iFile].c_str());
    TTree *tree = (TTree*)gDirectory->Get("SashaLightTree");
    
    TTreeFormula* runnum=new TTreeFormula("run","run",tree);
    TTreeFormula* lumisec=new TTreeFormula("lumi","lumi",tree);
    TTreeFormula* eventnum=new TTreeFormula("event","event",tree);
    TTreeFormula* m_mumu=new TTreeFormula("m_mumu","m_mumu",tree);
    
    
    int nEvents=tree->GetEntries();
    std::cout<<"  "<<nEvents<<" events to process in this file"<<std::endl;
    for (Int_t iEvent=0;iEvent<nEvents;iEvent++) {//Event Loop
      if(iEvent%1000000==0)std::cout<<"  Processed "<<iEvent<<" events"<<std::endl;
      tree->GetEvent(iEvent);
      //    std::cout<<iEvent<<std::endl;
      runnum->EvalInstance(iEvent);
      eventnum->EvalInstance(iEvent);
      lumisec->EvalInstance(iEvent);
      m_mumu->EvalInstance(iEvent);
      //std::cout<<boost::lexical_cast<unsigned>(runnum->PrintValue(0,0,"u"))<<" "<<boost::lexical_cast<unsigned>(eventnum->PrintValue(0,0,"u"))<<" "<<boost::lexical_cast<unsigned>(lumisec->PrintValue(0,0,"u"))<<std::endl;
      for(unsigned irun=0;irun<run.size();irun++){
	if(boost::lexical_cast<unsigned>(runnum->PrintValue(0,0,"u"))==run[irun]){
	  if(boost::lexical_cast<unsigned>(eventnum->PrintValue(0,0,"u"))==event[irun]){
	    if(boost::lexical_cast<unsigned>(lumisec->PrintValue(0,0,"u"))==lumi[irun]){
	      std::cout<<"Run: "<<run[irun]<<", Lumi: "<<lumi[irun]<<", Event: "<<event[irun]<<std::endl;
	      outfile<<files[iFile]<<" "<<iEvent<<std::endl;
	    }
	  }
	}
      }
    }
  }

  outfile.close();
  
  // TH1F *htmp = new TH1F("htmp","",35,0,70);

  // TCanvas *myc;
  // std::ostringstream lName;
  // lName << "myc0";
  // myc = new TCanvas(lName.str().c_str(),lName.str().c_str(),1500,1000);

  // std::string fullcut;
  // fullcut=lMuCut+"&&"+lCentralCut;
  // if(cutforward){
  //   fullcut=fullcut+"&&"+lForwardCut;
  // }

  // std::ostringstream var;
  // var << histName << ">>htmp";
  // //tree->Draw(var.str().c_str(),(lCut+"&&"+lForwardCut).c_str());
  // tree->Draw(var.str().c_str(),fullcut.c_str());

  // htmp->SetMarkerColor(1);
  // htmp->SetLineColor(1);
  // htmp->SetFillColor(1);
  // htmp->SetFillStyle(0);
  // htmp->SetLineWidth(2);
  // htmp->SetMarkerStyle(20);
  // htmp->SetMarkerSize(1.1);  

  // htmp->GetXaxis()->SetTitle("M_{#mu#mu}/GeV");
  // htmp->GetYaxis()->SetTitle("Events");
  // htmp->SetTitle("");
  
  // htmp->Draw("e1lp");
  
  // gStyle->SetOptStat(0);

  // TLegend* leg=new TLegend(0.8,0.8,0.9,0.9);
  // leg->AddEntry(htmp,"Data","elp");
  // leg->Draw("same");

  // myc->SaveAs("mmumu.pdf");
  return 0;
}
