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

  //Define thresholds
  //double btagthreshold=0.679;
  double btagthreshold=0.898;
  double centraljetthreshold=30;
  double forwardjetthreshold=30;

  std::string outfolder="output_sashacheck_pfloose/";

  //Define cuts
  std::string lMuCut = "mu1_pt>25&&mu2_pt>25&&(mu1_charge*mu2_charge==-1)&&(passparkedtrigger1==1||passparkedtrigger2==1)";

  std::string lAnyCentralCut="(((jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_pt>30)||((jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_pt>30)||((jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_pt>30)||((jet4_eta<2.4&&jet4_eta>-2.4)&&jet4_pt>30))";
  std::string lCentralBCut="(((jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_pt>30&&jet1_csv>"+boost::lexical_cast<std::string>(btagthreshold)+")||((jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_pt>30&&jet2_csv>"+boost::lexical_cast<std::string>(btagthreshold)+")||((jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_pt>30&&jet3_csv>"+boost::lexical_cast<std::string>(btagthreshold)+")||((jet4_eta<2.4&&jet4_eta>-2.4)&&jet4_pt>30&&jet4_csv>"+boost::lexical_cast<std::string>(btagthreshold)+"))";

  std::string lCentral1BCut="((jet1_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_csv>"+boost::lexical_cast<std::string>(btagthreshold)+"&&!((jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet4_eta<2.4&&jet4_eta>-2.4)&&jet4_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"))||(jet2_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_csv>"+boost::lexical_cast<std::string>(btagthreshold)+"&&!((jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet4_eta<2.4&&jet4_eta>-2.4)&&jet4_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"))||(jet3_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_csv>"+boost::lexical_cast<std::string>(btagthreshold)+"&&!((jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet4_eta<2.4&&jet4_eta>-2.4)&&jet4_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"))||(jet4_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+"&&(jet4_eta<2.4&&jet4_eta>-2.4)&&jet4_csv>"+boost::lexical_cast<std::string>(btagthreshold)+"&&!((jet2_eta<2.4&&jet2_eta>-2.4)&&jet2_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet1_eta<2.4&&jet1_eta>-2.4)&&jet1_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")&&!((jet3_eta<2.4&&jet3_eta>-2.4)&&jet3_pt>"+boost::lexical_cast<std::string>(centraljetthreshold)+")))";
    
  std::string lForwardCut="(((jet1_eta>=2.4||jet1_eta<=-2.4)&&jet1_pt>"+boost::lexical_cast<std::string>(forwardjetthreshold)+")||((jet2_eta>=2.4||jet2_eta<=-2.4)&&jet2_pt>"+boost::lexical_cast<std::string>(forwardjetthreshold)+")||((jet3_eta>=2.4||jet3_eta<=-2.4)&&jet3_pt>"+boost::lexical_cast<std::string>(forwardjetthreshold)+")||((jet4_eta>=2.4||jet4_eta<=-2.4)&&jet4_pt>"+boost::lexical_cast<std::string>(forwardjetthreshold)+"))";

  std::string lMassWindowCut = "(m_mumu>26&&m_mumu<32)";

  std::string histName = "m_mumu";

  //Define files to read
  TChain *tree = new TChain("SashaLightTree");
  tree->Add("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_pfloose_170815/SINGLEMU_SingleMu-2012A-22Jan2013-v1.root");
  tree->Add("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_pfloose_170815/SINGLEMU_SingleMu-2012B-22Jan2013-v1-split1.root");
  tree->Add("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_pfloose_170815/SINGLEMU_SingleMu-2012B-22Jan2013-v1-split2.root");
  tree->Add("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_pfloose_170815/SINGLEMU_SingleMu-2012B-22Jan2013-v1-split3.root");
  tree->Add("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_pfloose_170815/SINGLEMU_SingleMu-2012B-22Jan2013-v1-split4.root");
  tree->Add("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_pfloose_170815/SINGLEMU_SingleMu-2012C-22Jan2013-v1-split1.root");
  tree->Add("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_pfloose_170815/SINGLEMU_SingleMu-2012C-22Jan2013-v1-split2.root");
  tree->Add("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_pfloose_170815/SINGLEMU_SingleMu-2012C-22Jan2013-v1-split3.root");
  tree->Add("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_pfloose_170815/SINGLEMU_SingleMu-2012C-22Jan2013-v1-split4.root");
  tree->Add("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_pfloose_170815/SINGLEMU_SingleMu-2012D-22Jan2013-v1-split1.root");
  tree->Add("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_pfloose_170815/SINGLEMU_SingleMu-2012D-22Jan2013-v1-split2.root");
  tree->Add("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_pfloose_170815/SINGLEMU_SingleMu-2012D-22Jan2013-v1-split3.root");
  tree->Add("sashachecks/output_lighttree_singlemuforsashacrosschecksashaonly_pfloose_170815/SINGLEMU_SingleMu-2012D-22Jan2013-v1-split4.root");

  

  TCanvas *myc;
  std::ostringstream lName;
  lName << "myc0";
  myc = new TCanvas(lName.str().c_str(),lName.str().c_str(),1500,1000);

  std::vector<std::string> cuts;
  std::vector<std::string> cutname;
  
  //Set up cut steps
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

  TFile* file=new TFile((outfolder+"output.root").c_str(),"RECREATE");

  //Loop over cut steps and get m_mumu plot and integral at each step
  for(unsigned iCut=0;iCut<cuts.size();iCut++){
    
    TH1F* htmp = new TH1F("htmp","",29,12,70);
    //TH1F* htmp = new TH1F("htmp","",58,12,70);
    std::cout<<"getting histogram"<<std::endl;
    tree->Draw(var.str().c_str(),cuts[iCut].c_str());

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

    
    TF1* myfit = new TF1("myfit","gaus + [3]+[4]*x+[5]*x*x",0,70);
    myfit->SetParName(0, "Norm");
    myfit->SetParName(1, "Center");
    myfit->SetParName(2, "#sigma");
    myfit->SetParName(3, "const term");
    myfit->SetParName(4, "linear term");
    myfit->SetParName(5, "quad term");
    myfit->SetParameters(100, 45., 4, 10, 1, 1);
    
    //Fit to distribution after requiring forward jet
    // if(cutname[iCut]=="_forwardjet"){
    //   htmp->Fit("myfit");
    // }

    //Draw and save
    htmp->Draw("e1lp");
    
    gStyle->SetOptStat(0);
    
    TLegend* leg=new TLegend(0.8,0.8,0.9,0.9);
    leg->AddEntry(htmp,"Data","elp");
    leg->Draw("same");
    
    myc->SaveAs((outfolder+"/mmumu"+cutname[iCut]+".pdf").c_str());
    //myc->SetName(cutname[iCut].c_str());
    file->cd();
    //myc->Write();
    htmp->SetName(cutname[iCut].c_str());
    htmp->Write();
    

    myc->Clear();
    htmp->Delete();
    leg->Delete();
  }

  return 0;
}
