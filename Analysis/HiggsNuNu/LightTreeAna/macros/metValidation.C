#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>

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
#include "TGraphAsymmErrors.h"
#include "TLegend.h"
#include "TF1.h"
#include "TString.h"
#include "TLatex.h"
#include "TGaxis.h"

double deltaPhi(double phi1,double phi2)
{
  double result = phi1 - phi2;
  while (result > TMath::Pi()) result -= 2*TMath::Pi();
  while (result <= -TMath::Pi()) result += 2*TMath::Pi();
  return result;
};

double deltaR(double eta1,double phi1,double eta2,double phi2)
{
  double deta = eta1 - eta2;
  double dphi = deltaPhi(phi1, phi2);
  return std::sqrt(deta*deta + dphi*dphi);
};

int metValidation(){//main

  const unsigned nR = 4;
  TFile *f[nR];
  for (unsigned ir(0); ir<nR;++ir){//loop on jettypes
    f[ir] = 0;
  }

  //const std::string fullprocess = "TTJets-mg";
  //const std::string process = "TTJets";
  const std::string fullprocess = "Powheg-Htoinv-mH125Asympt25ns";
  const std::string process = "VBFH125";
  std::string jettype[nR] = {"pfJetsPFlowreclustered","pfJetsPFlow","ak4SlimmedJetsPuppi","pfMVAMet"};
  std::string mettype[nR] = {"uncorrectedpfMet","pfMetType1","pfMetPuppi","pfMVAMet"};
  //if (process == "TTJets") f[0] = TFile::Open("../../output_lighttree_noskim_150928/MC_TTJets-mg.root");
  
  const unsigned nV = 2;

  const std::string var[nV] = {"pt","phi"};
  TCanvas *mycvar[nV];
  mycvar[0] = new TCanvas("mycpt","mycpt",1500,1000);
  mycvar[1] = new TCanvas("mycphi","mycphi",1500,1000);

  TCanvas *mycratio[nV];
  mycratio[0] = new TCanvas("mycratiopt","mycratiopt",1500,1000);
  mycratio[1] = new TCanvas("mycratiophi","mycratiophi",1500,1000);

  TCanvas *myc2D = new TCanvas("myc2D","myc2D",1500,1000);
  myc2D->Divide(2,2);
  TCanvas *myc2Dmean = new TCanvas("myc2Dmean","myc2Dmean",1800,800);
  myc2Dmean->Divide(3,1);

  TH1F *hvarrec[nV][nR];
  TH1F *hvargen[nV];
  TH1F *hvarratio[nV][nR];

  TH2D *hratiovspt[nR];
  TH2F *hmean[nR];
  TH2F *hrms[nR];
  TH2F *hchi2[nR];
  gStyle->SetOptStat(0);//"eMRuo");
  //gStyle->SetStatW(0.4);

  TLegend *leg = new TLegend(0.7,0.7,0.95,0.95);
  leg->SetFillColor(0);
  TLegend *legratio = new TLegend(0.7,0.7,0.95,0.95);
  legratio->SetFillColor(0);

  TLatex lat;
  char buf[200];
  TTree *tree[nR];

  std::ostringstream histname;
  std::ostringstream lvar,lcut;
  
  for (unsigned ir(0); ir<nR;++ir){//loop on jettypes
    std::string inputfile = "../../output_lighttree_noskim_151026/"+jettype[ir]+"/MC_"+fullprocess+".root";
    f[ir] = TFile::Open(inputfile.c_str());
    
    if (!f[ir]) {
      std::cout << " input file " << inputfile << " not found." << std::endl;
      return 1;
    }

    f[ir]->cd();
    tree[ir] = (TTree*)gDirectory->Get("LightTreeJetMETval");
    if (!tree[ir]) {
      std::cout << " Tree not found for file " << inputfile << std::endl;
      return 1;
    }
  }

  for (unsigned iv(0); iv<nV;++iv){//loop on variables
    mycvar[iv]->cd();
    histname.str("");
    histname << "hgen" << var[iv];
    hvargen[iv] = new TH1F(histname.str().c_str(),(";"+var[iv]).c_str(),100,iv==0?0:-3.1416,iv==0?600:3.1416);
    hvargen[iv]->Sumw2();
    lvar.str("");
    if (iv==0) lvar << "genmet";
    else lvar << "genmetphi";
    lvar << ">>+" << histname.str();
    lcut.str("");
    tree[0]->Draw(lvar.str().c_str(),lcut.str().c_str());
    for (unsigned ir(0); ir<nR;++ir){//loop on jettypes
      //if (iv==0) gPad->SetLogy(1);
      histname.str("");
      histname << "hrec" << var[iv] << "_" << ir;
      hvarrec[iv][ir] = new TH1F(histname.str().c_str(),(";"+var[iv]).c_str(),100,iv==0?0:-3.1416,iv==0?600:3.1416);
      hvarrec[iv][ir]->Sumw2();
      lvar.str("");
      if (iv==0) lvar << "met";
      else lvar << "TMath::ATan2(met_y,met_x)";
      lvar << ">>+" << histname.str();
      lcut.str("");
      //lcut << "nJets>=2 && jet" << ij << "_genjet_mindR<0.4 && jet" << ij << "_pt>" << minptcut;
      tree[ir]->Draw(lvar.str().c_str(),lcut.str().c_str());
    }

    mycvar[iv]->cd();
    gPad->SetLogy(1);
    gPad->SetGridx(1);
    for (unsigned ir(0); ir<nR;++ir){//loop on jettypes
      hvarrec[iv][ir]->SetMarkerColor(1+ir);
      hvarrec[iv][ir]->SetMarkerStyle(20+ir);
      hvarrec[iv][ir]->SetLineColor(1+ir);
      hvarrec[iv][ir]->Draw(ir==0?"PE":"PEsame");
      if (iv==0){
	leg->AddEntry(hvarrec[iv][ir],mettype[ir].c_str(),"P");
      }
    }
    hvargen[iv]->SetLineColor(2);
    hvargen[iv]->SetFillColor(2);
    hvargen[iv]->SetFillStyle(3004);
    hvargen[iv]->Draw("histsame");
    if (iv==0) leg->AddEntry(hvargen[iv],"GenMet","L");
    leg->Draw("same");

    mycvar[iv]->Update();
    mycvar[iv]->Print(("PlotsJMETval/Met"+var[iv]+"_"+process+".pdf").c_str());

    mycratio[iv]->cd();
    gPad->SetLogy(1);
    gPad->SetGridx(1);
    for (unsigned ir(0); ir<nR;++ir){//loop on jettypes
      histname.str("");
      histname << "hratio" << var[iv] << "_" << ir;
      hvarratio[iv][ir] = new TH1F(histname.str().c_str(),(";reco/gen "+var[iv]).c_str(),100,iv==0?0:-5,iv==0?10:5);
      hvarratio[iv][ir]->Sumw2();
      lvar.str("");
      if (iv==0) lvar << "met/genmet";
      else lvar << "TMath::ATan2(met_y,met_x)/genmetphi";
      lvar << ">>+" << histname.str();
      lcut.str("");
      //lcut << "nJets>=2 && jet" << ij << "_genjet_mindR<0.4 && jet" << ij << "_pt>" << minptcut;
      tree[ir]->Draw(lvar.str().c_str(),lcut.str().c_str());
    }

    for (unsigned ir(0); ir<nR;++ir){//loop on jettypes
      hvarratio[iv][ir]->SetMarkerColor(1+ir);
      hvarratio[iv][ir]->SetMarkerStyle(20+ir);
      hvarratio[iv][ir]->SetLineColor(1+ir);
      hvarratio[iv][ir]->Draw(ir==0?"PE":"PEsame");
      if (iv==0) legratio->AddEntry(hvarratio[iv][ir],mettype[ir].c_str(),"P");
    }//loop on jet types
    legratio->Draw("same");

    mycratio[iv]->Update();
    mycratio[iv]->Print(("PlotsJMETval/Met"+var[iv]+"_recoovergen"+process+".pdf").c_str());

  }//loop on variables
  for (unsigned ir(0); ir<nR;++ir){//loop on jettypes
    myc2D->cd(ir+1);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    histname.str("");
    histname << "hratiovspt_" << ir;
    hratiovspt[ir] = new TH2D(histname.str().c_str(),";genMet (GeV); reco/gen",100,0,600,100,0,10);
    hratiovspt[ir]->Sumw2();
    lvar.str("");
    lvar << "met/genmet:genmet";
    lvar << ">>+" << histname.str();
    lcut.str("");
    //lcut << "nJets>=2 && jet" << ij << "_genjet_mindR<0.4 && jet" << ij << "_pt>" << minptcut;
    tree[ir]->Draw(lvar.str().c_str(),lcut.str().c_str(),"colz");
    TF1 *fit = new TF1("fit","gaus(0)",0,2);
    fit->SetParameters(500,1,0.5);
    //TObjArray aSlices;
    hratiovspt[ir]->FitSlicesY(fit,0,-1,5,"NG2");
    //aSlices.Print();

    hmean[ir] = (TH2F*)gDirectory->Get((histname.str()+"_1").c_str());
    hrms[ir] = (TH2F*)gDirectory->Get((histname.str()+"_2").c_str());
    hchi2[ir] = (TH2F*)gDirectory->Get((histname.str()+"_chi2").c_str());

    for (int ix(1);ix<hmean[ir]->GetNbinsX()+1;++ix){
      for (int iy(1);iy<hmean[ir]->GetNbinsY()+1;++iy){
	if (hrms[ir]->GetBinContent(ix,iy)<0) hrms[ir]->SetBinContent(ix,iy,fabs(hrms[ir]->GetBinContent(ix,iy)));
      }
    }
    lat.DrawLatexNDC(0.2,0.94,mettype[ir].c_str());
  }

  myc2D->Update();
  myc2D->Print(("PlotsJMETval/Metrecoovergen_vsgen_"+process+".pdf").c_str());

  for (unsigned ir(0); ir<nR;++ir){//loop on jettypes
    myc2Dmean->cd(1);
    gPad->SetGridy(1);
    hmean[ir]->SetMarkerColor(1+ir);
    hmean[ir]->SetMarkerStyle(20+ir);
    hmean[ir]->SetLineColor(1+ir);
    hmean[ir]->SetMinimum(0.6);
    hmean[ir]->SetMaximum(2);
    hmean[ir]->Draw(ir==0?"PE":"PEsame");
    if (ir==0) legratio->Draw("same");
    myc2Dmean->cd(2);
    gPad->SetGridy(1);
    hrms[ir]->SetMarkerColor(1+ir);
    hrms[ir]->SetMarkerStyle(20+ir);
    hrms[ir]->SetLineColor(1+ir);
    hrms[ir]->SetMinimum(0);
    hrms[ir]->SetMaximum(1.4);
    hrms[ir]->Draw(ir==0?"PE":"PEsame");
    if (ir==0) legratio->Draw("same");
    myc2Dmean->cd(3);
    gPad->SetGridy(1);
    hchi2[ir]->SetMarkerColor(1+ir);
    hchi2[ir]->SetMarkerStyle(20+ir);
    hchi2[ir]->SetLineColor(1+ir);
    //hchi2[ir]->SetMinimum(0);
    //hchi2[ir]->SetMaximum(1.4);
    hchi2[ir]->Draw(ir==0?"PE":"PEsame");
    if (ir==0) legratio->Draw("same");
  }//loop on jet types
  myc2Dmean->Update();
  myc2Dmean->Print(("PlotsJMETval/MetMeanRMSrecoovergen_vsgen_"+process+".pdf").c_str());


  return 0;
}//main

