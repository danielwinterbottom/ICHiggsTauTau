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

int jetMetValidation(){//main

  const unsigned nR = 1;
  TFile *f[nR];
  f[0] = 0;

  const std::string process = "TTJets";
  //const std::string process = "Sig400";

  if (process == "TTJets") f[0] = TFile::Open("../../output_lighttree_noskim_150928/MC_TTJets-mg.root");


  if (!f[0]) return 1;

  const unsigned nV = 2;

  TCanvas *mycmindR = new TCanvas("mycmindR","mycmindR",1500,1000);
  const std::string var[nV] = {"pt","eta"};
  TCanvas *mycvar[nV];
  mycvar[0] = new TCanvas("mycpt","mycpt",1500,1000);
  mycvar[1] = new TCanvas("myceta","myceta",1500,1000);
  TCanvas *myceff[nV];
  myceff[0] = new TCanvas("myceffpt","myceffpt",1800,600);
  myceff[0]->Divide(3,1);
  myceff[1] = new TCanvas("myceffeta","myceffeta",1800,600);
  myceff[1]->Divide(3,1);
  const unsigned nCanvas = nV+4;  
  TCanvas *myc[nCanvas];
  for (unsigned iC(0);iC<nCanvas;++iC){
    std::ostringstream lName;
    lName << "myc" << iC;
    myc[iC] = new TCanvas(lName.str().c_str(),lName.str().c_str(),1500,1000);
    if (iC<nV) myc[iC]->Divide(5,2);
  }

  const unsigned nP = 10;
  const unsigned nS = 4;
  std::string ljetidcut[2] = {"_jetid>0.5","_puid>0.5"};
  TH1F *hratio[nV][nP];
  TH1F *hvarrec[nV][nS];
  TH1F *hvargen[nV];

  double varval[nV][nP];
  double binwidth[nV][nP];

  TGraphErrors *gr[2*nV];
  TGraphAsymmErrors *grEff[nV][nS-1];

  const unsigned nJets = 25;

  gStyle->SetOptStat("eMR");
  gStyle->SetStatW(0.4);

  TLegend *leg = new TLegend(0.8,0.91,1,1);
  leg->SetFillColor(0);

  TLatex lat;
  char buf[200];

  f[0]->cd();
  TTree *tree = (TTree*)gDirectory->Get("LightTreeJetMETval");
  if (!tree) return 1;

  std::ostringstream histname;
  std::ostringstream lvar,lcut;
  mycmindR->cd();
  histname << "mindeltaR";
  TH1F *hmindr = new TH1F(histname.str().c_str(),";min #DeltaR (jet,genjet);jets",100,0,2);
  for (unsigned ij(1);ij<nJets+1;++ij){//loop on jets
    lvar.str("");
    lvar << "jet" << ij << "_genjet_mindR>>+" << histname.str();
    lcut.str("");
    lcut << "nJets_15>=" << ij << " && genjet" << ij << "_pt>15 && TMath::Abs(genjet" << ij << "_eta)<4.7 && jet" << ij << "_jetid>0.5 && jet" << ij << "_puid>0.5";
    tree->Draw(lvar.str().c_str(),lcut.str().c_str());
  }
  gPad->SetLogy(1);
  gPad->SetGridx(1);
  hmindr->SetMarkerStyle(22);
  hmindr->Draw("PE");
  mycmindR->Update();
  mycmindR->Print(("JetGenJetMindR_"+process+".pdf").c_str());
  
  return 1;

  for (unsigned iv(0); iv<nV;++iv){//loop on variables

    mycvar[iv]->cd();
    for (unsigned is(0); is<nS;++is){//loop on jetid cond
      histname.str("");
      histname << "hrec" << var[iv] << "_" << is;
      hvarrec[iv][is] = new TH1F(histname.str().c_str(),(";"+var[iv]).c_str(),100,iv==0?0:-5,iv==0?500:5);
      hvarrec[iv][is]->Sumw2();
      for (unsigned ij(1);ij<nJets+1;++ij){//loop on jets
	lvar.str("");
	lvar << "jet" << ij << "_" << var[iv] << ">>+" << histname.str();
	lcut.str("");
	lcut << "nJets_15>=" << ij << " && genjet" << ij << "_pt>15 && TMath::Abs(genjet" << ij << "_eta)<4.7 && jet" << ij << "_genjet_mindR<0.4";
	if (is==1 || is==3) lcut << " && jet" << ij << ljetidcut[0];
	if (is==2 || is==3) lcut << " && jet" << ij << ljetidcut[1];	
	tree->Draw(lvar.str().c_str(),lcut.str().c_str());
      }
    }//loop on jetid cond
    histname.str("");
    histname << "hgen" << var[iv];
    hvargen[iv] = new TH1F(histname.str().c_str(),(";"+var[iv]).c_str(),100,iv==0?0:-5,iv==0?500:5);
    for (unsigned ij(1);ij<nJets;++ij){//loop on jets
      lvar.str("");
      lvar << "genjet" << ij << "_" << var[iv] << ">>+" << histname.str();
      lcut.str("");
      lcut << "nJets_15>=" << ij << " && genjet" << ij << "_pt>15 && TMath::Abs(genjet" << ij << "_eta)<4.7 && jet" << ij << "_genjet_mindR<0.4";
      tree->Draw(lvar.str().c_str(),lcut.str().c_str());
    }
    for (unsigned is(0); is<nS;++is){//loop on jetid cond
      hvarrec[iv][is]->SetMarkerColor(9-is);
      hvarrec[iv][is]->SetMarkerStyle(20+is);
      hvarrec[iv][is]->SetLineColor(9-is);
      hvarrec[iv][is]->Draw(is==0?"PE":"PEsame");
    }
    //hvargen[iv]->SetMarkerColor(2);
    //hvargen[iv]->SetMarkerStyle(23);
    hvargen[iv]->SetLineColor(2);
    hvargen[iv]->Draw("same");
    if (iv==0){
      leg->AddEntry(hvarrec[iv][0],"Reco","P");
      leg->AddEntry(hvarrec[iv][1],"Reco jetid","P");
      leg->AddEntry(hvarrec[iv][2],"Reco puid","P");
      leg->AddEntry(hvarrec[iv][3],"Reco jetid+puid","P");
      leg->AddEntry(hvargen[iv],"Gen","L");
    }
    leg->Draw("same");
    mycvar[iv]->Update();
    mycvar[iv]->Print(("Jet"+var[iv]+"_"+process+".pdf").c_str());


    //get efficiency graphs
    for (unsigned is(0); is<nS-1;++is){//loop on jetid cond
      grEff[iv][is] = new TGraphAsymmErrors();
      grEff[iv][is]->Divide(hvarrec[iv][is+1],hvarrec[iv][0]);
      grEff[iv][is]->SetTitle((";"+var[iv]+";efficiency").c_str());
      myceff[iv]->cd(is+1);
      grEff[iv][is]->SetMarkerStyle(22);
      grEff[iv][is]->Draw("APE");
      lat.DrawLatexNDC(0.2,0.96,is==0?"Jet ID":is==1?"PU ID":"Jet ID + PU ID");
    }
    myceff[iv]->Update();
    myceff[iv]->Print(("Jet"+var[iv]+"_idefficiencies_"+process+".pdf").c_str());


    for (unsigned ip(0); ip<nP;++ip){//loop on points
      histname.str("");
      histname << "hratio_" << iv << "_" << ip;
      hratio[iv][ip] = new TH1F(histname.str().c_str(),";pTreco/pTgen",100,0,2);
    }
    for (unsigned ij(1);ij<nJets;++ij){//loop on jets
      for (unsigned ip(0); ip<nP;++ip){//loop on points
	lvar.str("");
	lcut.str("");
	lvar << "jet" << ij << "_pt/genjet" << ij << "_pt>>+hratio_" << iv << "_" << ip;
	lcut << "genjet" << ij << "_pt>15 && TMath::Abs(genjet" << ij << "_eta)<4.7 && jet" << ij << "_genjet_mindR<0.4 && jet" << ij << "_jetid>0.5 && jet" << ij << "_puid>0.5 && ";
	myc[iv]->cd(ip+1);
	//gPad->SetLogy(1);
	double binmin = 0;
	double binmax = 0;
	std::ostringstream ltitle;
	if (iv==0) {
	  binmin = ip==0?0:30+ip*20;
	  binmax = 30+(ip+1)*20;
	  lcut << "genjet" << ij << "_pt" << ">=" << binmin << " && "
	       << "genjet" << ij << "_pt" << "<" << binmax;
	  if (ip==nP-1) ltitle << "p_{T}^{genjet} #geq " << binmin;
	  else ltitle << binmin << " #leq p_{T}^{genjet} < " << binmax;
	}
	else {
	  binmin = -4.7+(4.7*2/10.)*ip;
	  binmax = -4.7+(4.7*2/10.)*(ip+1);
	  lcut << "genjet" << ij << "_eta" << ">=" << binmin << " && "
		 << "genjet" << ij << "_eta" << "<" << binmax
	       << " && genjet" << ij << "_pt" << ">30";
	  ltitle << binmin << " #leq #eta^{genjet} < " << binmax;
	}
	varval[iv][ip] = (binmax+binmin)/2.;
	binwidth[iv][ip] = (binmax-binmin);
	hratio[iv][ip]->SetTitle(ltitle.str().c_str());
	tree->Draw(lvar.str().c_str(),lcut.str().c_str());
	//std::cout << " --- " << lvar.str() << std::endl
	//	    << " *** " << lcut.str() << std::endl;
      }//loop on points
    }//loop on jets

    for (unsigned igr(0);igr<2;++igr){
      gr[iv+2*igr] = new TGraphErrors();
      if (iv==0) gr[iv+2*igr]->SetName("pt");
      else gr[iv+2*igr]->SetName("eta");
      if (iv==0) gr[iv+2*igr]->SetTitle(";p_{T}^{gen} (GeV);<p_{T}^{reco}/p_{T}^{gen}>");
      else gr[iv+2*igr]->SetTitle(";#eta^{gen};<p_{T}^{reco}/p_{T}^{gen}>");
      gr[iv+2*igr]->SetMarkerStyle(22);
      gr[iv+2*igr]->SetMinimum(igr==0?0.8:0);
      gr[iv+2*igr]->SetMaximum(igr==0?1.2:0.3);
    }
    for (unsigned ip(0); ip<nP;++ip){//loop on runs
      myc[iv]->cd(ip+1);
      hratio[iv][ip]->Draw();
      hratio[iv][ip]->Fit("gaus");
      TF1 *fit = (TF1*)hratio[iv][ip]->GetFunction("gaus");
      if (!fit) continue;
      gr[iv]->SetPoint(ip,varval[iv][ip],fit->GetParameter(1));
      gr[iv]->SetPointError(ip,binwidth[iv][ip]/2.,fit->GetParError(1));
      gr[iv+2]->SetPoint(ip,varval[iv][ip],fit->GetParameter(2));
      gr[iv+2]->SetPointError(ip,binwidth[iv][ip]/2.,fit->GetParError(2));
    }
    myc[iv]->Update();
    myc[iv]->Print(("JetResponse_per"+var[iv]+"bin_"+process+".pdf").c_str());
    
    myc[iv+2]->cd();
    gr[iv]->Draw("APEL");
    lat.DrawLatexNDC(0.16,0.92,("CMS Simulation - "+process).c_str());
    if (iv==0) lat.DrawLatexNDC(0.5,0.85,"|#eta^{genjet}|<4.7");
    else lat.DrawLatexNDC(0.5,0.85,"p_{T}^{genjet}>30 GeV");
    myc[iv+2]->Update();
    myc[iv+2]->Print(("JetScale_vs"+var[iv]+"_"+process+".pdf").c_str());

    myc[iv+4]->cd();
    gr[iv+2]->Draw("APEL");
    gr[iv+2]->GetYaxis()->SetTitle("#sigma(p_{T}^{reco}/p_{T}^{gen})");
    lat.DrawLatexNDC(0.16,0.92,("CMS Simulation - "+process).c_str());
    if (iv==0) lat.DrawLatexNDC(0.5,0.85,"|#eta^{genjet}|<4.7");
    else lat.DrawLatexNDC(0.5,0.85,"p_{T}^{genjet}>30 GeV");
    myc[iv+4]->Update();
    myc[iv+4]->Print(("JetReso_vs"+var[iv]+"_"+process+".pdf").c_str());

  }//loop on variables

  return 0;
}//main

