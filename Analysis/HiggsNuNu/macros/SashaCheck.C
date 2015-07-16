#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>

#include "boost/lexical_cast.hpp"
#include "TFile.h"
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
#include "TChain.h"
#include "TEntryList.h"


int SashaCheck(){

  double btagthreshold=0.679;
  //double btagthreshold=0.898;
  double centraljetthreshold=30;
  double forwardjetthreshold=30;

  std::string outfolder="output_sashacheck_csvmedium";

  std::string lMuCut = "mu1_pt>25&&mu2_pt>25&&(mu1_charge*mu2_charge==-1)";

  std::string lAnyCentralCut="(((jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_pt>30)||((jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_pt>30)||((jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_pt>30)||((jet4_eta<2.4&&jet4_eta>-2.4)&&jet4_pt>30))";
  std::string lCentralBCut="(((jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_pt>30&&jet1_csv>"+boost::lexical_cast<std::string>(btagthreshold)+")||((jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_pt>30&&jet2_csv>"+boost::lexical_cast<std::string>(btagthreshold)+")||((jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_pt>30&&jet3_csv>"+boost::lexical_cast<std::string>(btagthreshold)+")||((jet4_eta<2.4&&jet4_eta>-2.4)&&jet4_pt>30&&jet4_csv>"+boost::lexical_cast<std::string>(btagthreshold)+"))";

  std::string lCentral1BCut="((jet1_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_csv>"+boost::lexical_cast<std::string>(btagthreshold)+"&&!((jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet4_eta<2.4&&jet4_eta>-2.4)&&jet4_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"))||(jet2_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_csv>"+boost::lexical_cast<std::string>(btagthreshold)+"&&!((jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet4_eta<2.4&&jet4_eta>-2.4)&&jet4_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"))||(jet3_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_csv>"+boost::lexical_cast<std::string>(btagthreshold)+"&&!((jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet4_eta<2.4&&jet4_eta>-2.4)&&jet4_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"))||(jet4_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet4_eta<2.4&&jet4_eta>-2.4)&&jet4_csv>"+boost::lexical_cast<std::string>(btagthreshold)+"&&!((jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")))";
    
  //std::string lCentralCut="((jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_csv>"+boost::lexical_cast<std::string>(btagthreshold)+")";
  //std::string lCentralCut="((jet1_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_csv>"+boost::lexical_cast<std::string>(btagthreshold)+")||(jet2_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_csv>"+boost::lexical_cast<std::string>(btagthreshold)+")||(jet3_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_csv>"+boost::lexical_cast<std::string>(btagthreshold)+"))";
  std::string lForwardCut="(((jet1_eta>=2.4||jet1_eta<=-2.4)&&jet1_pt>"+boost::lexical_cast<std::string>(forwardjetthreshold)+")||((jet2_eta>=2.4||jet2_eta<=-2.4)&&jet2_pt>"+boost::lexical_cast<std::string>(forwardjetthreshold)+")||((jet3_eta>=2.4||jet3_eta<=-2.4)&&jet3_pt>"+boost::lexical_cast<std::string>(forwardjetthreshold)+")||((jet4_eta>=2.4||jet4_eta<=-2.4)&&jet4_pt>"+boost::lexical_cast<std::string>(forwardjetthreshold)+"))";

  std::string lMassWindowCut = "(m_mumu>26&&m_mumu<32)";

  std::string histName = "m_mumu";

  //TFile* fS = TFile::Open("allsinglemu.root");
  //  TTree *tree = (TTree*)gDirectory->Get("LightTree");
  TChain *tree = new TChain("SashaLightTree");
  tree->Add("output_lighttree_singlemuforsashacrosschecksashaonly_150715/SINGLEMU_SingleMu-2012A-22Jan2013-v1.root");
  tree->Add("output_lighttree_singlemuforsashacrosschecksashaonly_150715/SINGLEMU_SingleMu-2012B-22Jan2013-v1-split1.root");
  tree->Add("output_lighttree_singlemuforsashacrosschecksashaonly_150715/SINGLEMU_SingleMu-2012B-22Jan2013-v1-split2.root");
  tree->Add("output_lighttree_singlemuforsashacrosschecksashaonly_150715/SINGLEMU_SingleMu-2012B-22Jan2013-v1-split3.root");
  tree->Add("output_lighttree_singlemuforsashacrosschecksashaonly_150715/SINGLEMU_SingleMu-2012B-22Jan2013-v1-split4.root");
  tree->Add("output_lighttree_singlemuforsashacrosschecksashaonly_150715/SINGLEMU_SingleMu-2012C-22Jan2013-v1-split1.root");
  tree->Add("output_lighttree_singlemuforsashacrosschecksashaonly_150715/SINGLEMU_SingleMu-2012C-22Jan2013-v1-split2.root");
  tree->Add("output_lighttree_singlemuforsashacrosschecksashaonly_150715/SINGLEMU_SingleMu-2012C-22Jan2013-v1-split3.root");
  tree->Add("output_lighttree_singlemuforsashacrosschecksashaonly_150715/SINGLEMU_SingleMu-2012C-22Jan2013-v1-split4.root");
  tree->Add("output_lighttree_singlemuforsashacrosschecksashaonly_150715/SINGLEMU_SingleMu-2012D-22Jan2013-v1-split1.root");
  tree->Add("output_lighttree_singlemuforsashacrosschecksashaonly_150715/SINGLEMU_SingleMu-2012D-22Jan2013-v1-split2.root");
  tree->Add("output_lighttree_singlemuforsashacrosschecksashaonly_150715/SINGLEMU_SingleMu-2012D-22Jan2013-v1-split3.root");
  tree->Add("output_lighttree_singlemuforsashacrosschecksashaonly_150715/SINGLEMU_SingleMu-2012D-22Jan2013-v1-split4.root");

  

  TCanvas *myc;
  std::ostringstream lName;
  lName << "myc0";
  myc = new TCanvas(lName.str().c_str(),lName.str().c_str(),1500,1000);

  std::vector<std::string> cuts;
  std::vector<std::string> cutname;

  std::string fullcut;
  fullcut=lMuCut;
  cuts.push_back(fullcut);
  cutname.push_back("_mupair");
  fullcut=fullcut+"&&"+lAnyCentralCut;
  cuts.push_back(fullcut);
  cutname.push_back("_centraljet");
  fullcut=fullcut+"&&"+lCentralBCut;
  cuts.push_back(fullcut);
  cutname.push_back("_centralbjet");
  fullcut=fullcut+"&&"+lCentral1BCut;
  cuts.push_back(fullcut);
  cutname.push_back("_centralbjetcjv");
  fullcut=fullcut+"&&"+lForwardCut;
  cuts.push_back(fullcut);
  cutname.push_back("_forwardjet");
  fullcut=fullcut+"&&"+lMassWindowCut;
  cuts.push_back(fullcut);
  cutname.push_back("_26-32");

  std::ostringstream var;
  var << histName << ">>htmp";

  for(unsigned iCut=0;iCut<cuts.size();iCut++){
    
    TH1F* htmp = new TH1F("htmp","",29,12,70);
    //tree->Draw(var.str().c_str(),(lCut+"&&"+lForwardCut).c_str());
    std::cout<<"getting histogram"<<std::endl;
    tree->Draw(var.str().c_str(),cuts[iCut].c_str());

    //get event list
    // std::cout<<"getting event list"<<std::endl;
    // TEntryList *list=(TEntryList*)gDirectory->Get("evtlist");
    // tree->SetEntryList(list);
    // TFile *tmpfile=new TFile("tmp.root","RECREATE");
    // TTree *small = tree->CopyTree("");
    //   tree->SetEventList(0);
    //   unsigned run,lumi,event;
    //   small->SetBranchAddress("run",&run);
    //   small->SetBranchAddress("lumi",&lumi);
    //   small->SetBranchAddress("event",&event);
    //   int nentries=small->GetEntries();
      
    //   std::cout<<"printing event list"<<std::endl;
    //   ofstream myfile;
    //   myfile.open(("mumuevents"+cutname[iCut]+".txt").c_str());
      
    //   for(int iEvent=0;iEvent<nentries;iEvent++){
    // 	small->GetEntry(iEvent);
    // 	myfile<<run<<" "<<lumi<<" "<<event<<std::endl;
    //   }
    //   myfile.close();
    //small->Delete();
    //tmpfile->Delete();
    std::cout<<cutname[iCut]<<" has "<<htmp->Integral(1,htmp->GetNbinsX())<<" events left"<<std::endl;

    std::cout<<"drawing histogram"<<std::endl;    
    htmp->SetMarkerColor(1);
    htmp->SetLineColor(1);
    htmp->SetFillColor(1);
    htmp->SetFillStyle(0);
    htmp->SetLineWidth(2);
    htmp->SetMarkerStyle(20);
    htmp->SetMarkerSize(1.1);  
    
    htmp->GetXaxis()->SetTitle("M_{#mu#mu}/GeV");
    htmp->GetYaxis()->SetTitle("Events");
    htmp->SetTitle("");
    
    htmp->Draw("e1lp");
    
    gStyle->SetOptStat(0);
    
    TLegend* leg=new TLegend(0.8,0.8,0.9,0.9);
    leg->AddEntry(htmp,"Data","elp");
    leg->Draw("same");
    
    myc->SaveAs((outfolder+"/mmumu"+cutname[iCut]+".pdf").c_str());
    myc->Clear();
    htmp->Delete();
    leg->Delete();
  }

  return 0;
}
