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

  const unsigned nR = 2;
  TFile *f[nR];
  f[0] = 0;
  f[1] = 0;

  const unsigned minptcut = 30;

  const std::string process = "TTJets";
  //const std::string process = "Sig400";
  std::string jettype[nR] = {"pfJetsPFlow","ak4SlimmedJetsPuppi"};

  //if (process == "TTJets") f[0] = TFile::Open("../../output_lighttree_noskim_150928/MC_TTJets-mg.root");
  
    const unsigned nV = 2;

  TCanvas *mycmindR = new TCanvas("mycmindR","mycmindR",1500,1000);
  const std::string var[nV] = {"pt","eta"};
  TCanvas *mycvar[nV];
  mycvar[0] = new TCanvas("mycpt","mycpt",1500,1000);
  mycvar[1] = new TCanvas("myceta","myceta",1500,1000);
  TCanvas *mycpu[nV];
  mycpu[0] = new TCanvas("mycpupt","mycpupt",1500,1000);
  mycpu[1] = new TCanvas("mycpueta","mycpueta",1500,1000);
  TCanvas *myceff[nV];
  myceff[0] = new TCanvas("myceffpt","myceffpt",1800,600);
  myceff[0]->Divide(3,1);
  myceff[1] = new TCanvas("myceffeta","myceffeta",1800,600);
  myceff[1]->Divide(3,1);
  const unsigned nCanvas = nV*nR+2*nV;  
  TCanvas *myc[nCanvas];
  for (unsigned iC(0);iC<nCanvas;++iC){
    std::ostringstream lName;
    lName << "myc" << iC;
    myc[iC] = new TCanvas(lName.str().c_str(),lName.str().c_str(),1500,1000);
    if (iC<nR*nV) myc[iC]->Divide(5,2);
  }

  const unsigned nP = 10;
  const unsigned nS = 4;
  std::string ljetidcut[2] = {"_jetid>0.5","_puid>0.5"};

  TH1F *hmindr[nR];
  TH1F *hratio[nV][nR][nP];
  TH1F *hvarrec[nV][nR][nS];
  TH1F *hvarall[nV][nR][nS];
  TH1F *hvargen[nV][nR];

  double varval[nV][nR][nP];
  double binwidth[nV][nR][nP];

  TGraphErrors *gr[2*nV][nR];
  TGraphAsymmErrors *grEff[nV][nR][nS-1];
  TGraphAsymmErrors *grPu[nV][nR][2];

  const unsigned nJets = 25;

  gStyle->SetOptStat(0);//"eMR");
  //gStyle->SetStatW(0.4);

  TLegend *leg = new TLegend(0.7,0.7,0.95,0.95);
  leg->SetFillColor(0);
  TLegend *legrun = new TLegend(0.8,0.85,0.99,0.99);
  legrun->SetFillColor(0);

  TLatex lat;
  char buf[200];
  TTree *tree[nR];

  std::ostringstream histname;
  std::ostringstream lvar,lcut;
  
  for (unsigned ir(0); ir<nR;++ir){//loop on jettypes
    std::string inputfile = "../../output_lighttree_noskim_150930/"+jettype[ir]+"/MC_"+process+"-mg.root";
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

    mycmindR->cd();
    histname.str("");
    histname << "mindeltaR_" << ir;
    hmindr[ir] = new TH1F(histname.str().c_str(),";min #DeltaR (jet,genjet);jets",100,0,2);
    for (unsigned ij(1);ij<nJets+1;++ij){//loop on jets
      lvar.str("");
      lvar << "jet" << ij << "_genjet_mindR>>+" << histname.str();
      lcut.str("");
      lcut << "nJets_15>=" << ij << " && genjet" << ij << "_pt>15 && TMath::Abs(genjet" << ij << "_eta)<4.7 && jet" << ij << "_jetid>0.5 && jet" << ij << "_puid>0.5";
      tree[ir]->Draw(lvar.str().c_str(),lcut.str().c_str());
    }
  }

  mycmindR->cd();
  gPad->SetLogy(1);
  gPad->SetGridx(1);
  for (unsigned ir(0); ir<nR;++ir){//loop on jettypes
    hmindr[ir]->SetMarkerStyle(22+ir);
    hmindr[ir]->SetMarkerColor(1+ir);
    hmindr[ir]->SetLineColor(1+ir);
    hmindr[ir]->SetMinimum(1);
    hmindr[ir]->Draw(ir==0?"PE":"PEsame");
  }
  legrun->AddEntry(hmindr[0],"ak4PFchs","P");
  legrun->AddEntry(hmindr[1],"Puppi","P");
  legrun->Draw("same");
  mycmindR->Update();
  mycmindR->Print(("PlotsJMETval/JetGenJetMindR_"+process+".pdf").c_str());

  //return 1;

  for (unsigned iv(0); iv<nV;++iv){//loop on variables
    mycvar[iv]->Divide(2,1);;
    
    for (unsigned ir(0); ir<nR;++ir){//loop on jettypes
      mycvar[iv]->cd(ir+1);
      for (unsigned is(0); is<nS;++is){//loop on jetid cond
	histname.str("");
	histname << "hrec" << var[iv] << "_" << ir << "_" << is;
	hvarrec[iv][ir][is] = new TH1F(histname.str().c_str(),(";"+var[iv]).c_str(),100,iv==0?0:-5,iv==0?500:5);
	hvarrec[iv][ir][is]->Sumw2();
	for (unsigned ij(1);ij<nJets+1;++ij){//loop on jets
	  lvar.str("");
	  lvar << "jet" << ij << "_" << var[iv] << ">>+" << histname.str();
	  lcut.str("");
	  lcut << "nJets_15>=" << ij << " && TMath::Abs(genjet" << ij << "_eta)<4.7 && jet" << ij << "_genjet_mindR<0.4 && genjet" << ij << "_pt>";
	  if (iv==1) lcut << minptcut;
	  else lcut << "15";
	  if (is==1 || is==3) lcut << " && jet" << ij << ljetidcut[0];
	  if (is==2 || is==3) lcut << " && jet" << ij << ljetidcut[1];	
	  tree[ir]->Draw(lvar.str().c_str(),lcut.str().c_str());
	}
      }//loop on jetid cond
      for (unsigned is(0); is<nS;++is){//loop on jetid cond
	histname.str("");
	histname << "hall" << var[iv] << "_" << ir << "_" << is;
	hvarall[iv][ir][is] = new TH1F(histname.str().c_str(),(";"+var[iv]).c_str(),100,iv==0?0:-5,iv==0?500:5);
	hvarall[iv][ir][is]->Sumw2();
	for (unsigned ij(1);ij<nJets+1;++ij){//loop on jets
	  lvar.str("");
	  lvar << "jet" << ij << "_" << var[iv] << ">>+" << histname.str();
	  lcut.str("");
	  lcut << "nJets_15>=" << ij << " && TMath::Abs(genjet" << ij << "_eta)<4.7 && genjet" << ij << "_pt>";
	  if (iv==1) lcut << minptcut;
	  else lcut << "15";
	  if (is==1 || is==3) lcut << " && jet" << ij << ljetidcut[0];
	  if (is==2 || is==3) lcut << " && jet" << ij << ljetidcut[1];	
	  tree[ir]->Draw(lvar.str().c_str(),lcut.str().c_str());
	}
      }
      histname.str("");
      histname << "hgen" << var[iv] << "_" << ir;
      hvargen[iv][ir] = new TH1F(histname.str().c_str(),(";"+var[iv]).c_str(),100,iv==0?0:-5,iv==0?500:5);
      for (unsigned ij(1);ij<nJets;++ij){//loop on jets
	lvar.str("");
	lvar << "genjet" << ij << "_" << var[iv] << ">>+" << histname.str();
	lcut.str("");
	lcut << "nJets_15>=" << ij << " && TMath::Abs(genjet" << ij << "_eta)<4.7 && jet" << ij << "_genjet_mindR<0.4 && genjet" << ij << "_pt>";
	if (iv==1) lcut << minptcut;
	else lcut << "15";
	tree[ir]->Draw(lvar.str().c_str(),lcut.str().c_str());
      }
      for (unsigned is(0); is<nS;++is){//loop on jetid cond
	hvarrec[iv][ir][is]->SetMarkerColor(9-is);
	hvarrec[iv][ir][is]->SetMarkerStyle(20+is);
	hvarrec[iv][ir][is]->SetLineColor(9-is);
	hvarrec[iv][ir][is]->Draw(is==0?"PE":"PEsame");
      }
      //hvargen[iv][ir]->SetMarkerColor(2);
      //hvargen[iv][ir]->SetMarkerStyle(23);
      hvargen[iv][ir]->SetLineColor(2);
      hvargen[iv][ir]->Draw("same");
      if (iv==0&&ir==0){
	leg->AddEntry(hvarrec[iv][ir][0],"Reco","P");
	leg->AddEntry(hvarrec[iv][ir][1],"Reco jetid","P");
	leg->AddEntry(hvarrec[iv][ir][2],"Reco puid","P");
	leg->AddEntry(hvarrec[iv][ir][3],"Reco jetid+puid","P");
	leg->AddEntry(hvargen[iv][ir],"Gen","L");
      }
      leg->Draw("same");

      lat.DrawLatexNDC(0.2,0.94,jettype[ir].c_str());

    }//loop on jet types
    mycvar[iv]->Update();
    mycvar[iv]->Print(("PlotsJMETval/Jet"+var[iv]+"_"+process+".pdf").c_str());


    //get efficiency graphs
    mycpu[iv]->Divide(2,1);
    for (unsigned ir(0); ir<nR;++ir){//loop on jettypes
      for (unsigned is(0); is<nS-1;++is){//loop on jetid cond
	grEff[iv][ir][is] = new TGraphAsymmErrors();
	grEff[iv][ir][is]->Divide(hvarrec[iv][ir][is+1],hvarrec[iv][ir][0]);
	grEff[iv][ir][is]->SetTitle((";"+var[iv]+";efficiency").c_str());
	myceff[iv]->cd(is+1);
	grEff[iv][ir][is]->SetMarkerStyle(22+ir);
	grEff[iv][ir][is]->SetMarkerColor(1+ir);
	grEff[iv][ir][is]->SetLineColor(1+ir);
	grEff[iv][ir][is]->Draw(ir==0?"APE":"PEsame");
	if (ir==0) lat.DrawLatexNDC(0.4,0.94,is==0?"Jet ID":is==1?"PU ID":"Jet ID + PU ID");
	legrun->Draw("same");

      }

      mycpu[iv]->cd(ir+1);
      gPad->SetGridy(1);
      grPu[iv][ir][0] = new TGraphAsymmErrors();
      if (ir==0) grPu[iv][ir][0]->Divide(hvarrec[iv][ir][0],hvarall[iv][ir][3],"pois");
      else grPu[iv][ir][0]->Divide(hvarrec[iv][ir][0],hvarall[iv][ir][2],"pois");
      if (iv==0) grPu[iv][ir][0]->GetXaxis()->SetRangeUser(0,100);
      grPu[iv][ir][0]->SetTitle((";"+var[iv]+";matched/all").c_str());
      grPu[iv][ir][0]->SetMarkerStyle(22);
      grPu[iv][ir][0]->SetMarkerColor(1);
      grPu[iv][ir][0]->SetLineColor(1);
      grPu[iv][ir][0]->Draw("APE");

      grPu[iv][ir][1] = new TGraphAsymmErrors();
      grPu[iv][ir][1]->Divide(hvarrec[iv][ir][0],hvarall[iv][ir][0]);
      if (iv==0) grPu[iv][ir][1]->GetXaxis()->SetRangeUser(0,100);
      grPu[iv][ir][1]->SetTitle((";"+var[iv]+";matched/all").c_str());
      grPu[iv][ir][1]->SetMarkerStyle(23);
      grPu[iv][ir][1]->SetMarkerColor(2);
      grPu[iv][ir][1]->SetLineColor(2);
      grPu[iv][ir][1]->Draw("PEsame");
      lat.SetTextColor(2);
      lat.DrawLatexNDC(0.15,0.85,"matched/all");
      lat.SetTextColor(1);
      lat.DrawLatexNDC(0.15,0.75,"matched/ID");
      lat.DrawLatexNDC(0.2,0.94,jettype[ir].c_str());

    }//loop on jet types

    myceff[iv]->Update();
    myceff[iv]->Print(("PlotsJMETval/Jet"+var[iv]+"_idefficiencies_"+process+".pdf").c_str());
    mycpu[iv]->Update();
    mycpu[iv]->Print(("PlotsJMETval/Jet"+var[iv]+"_fakerate_"+process+".pdf").c_str());

    //continue;

    for (unsigned ir(0); ir<nR;++ir){//loop on jettypes
      for (unsigned ip(0); ip<nP;++ip){//loop on points
	histname.str("");
	histname << "hratio_" << iv << "_" << ir << "_" << ip;
	hratio[iv][ir][ip] = new TH1F(histname.str().c_str(),";pTreco/pTgen",100,0,2);
      }
      for (unsigned ij(1);ij<nJets;++ij){//loop on jets
	for (unsigned ip(0); ip<nP;++ip){//loop on points
	  lvar.str("");
	  lcut.str("");
	  histname.str("");
	  histname << "hratio_" << iv << "_" << ir << "_" << ip;
	  lvar << "jet" << ij << "_pt/genjet" << ij << "_pt>>+" << histname.str();
	  lcut << "genjet" << ij << "_pt>";
	  if (iv==1) lcut << minptcut;
	  else lcut << "15";
	  lcut << " && TMath::Abs(genjet" << ij << "_eta)<4.7 && jet" << ij << "_genjet_mindR<0.4";
	  //if (ir==1) lcut << " && jet" << ij << "_puid>0.5";
	  //if (ir==0) lcut << " && jet" << ij << "_jetid>0.5";
	  myc[nR*iv+ir]->cd(ip+1);
	  gStyle->SetOptStat("eMR");
	  gStyle->SetStatW(0.4);

	  //gPad->SetLogy(1);
	  double binmin = 0;
	  double binmax = 0;
	  std::ostringstream ltitle;
	  if (iv==0) {
	    binmin = ip==0?0:30+ip*20;
	    binmax = 30+(ip+1)*20;
	    lcut << " && genjet" << ij << "_pt" << ">=" << binmin << " && "
		 << "genjet" << ij << "_pt" << "<" << binmax;
	    if (ip==nP-1) ltitle << "p_{T}^{genjet} #geq " << binmin;
	    else ltitle << binmin << " #leq p_{T}^{genjet} < " << binmax;
	  }
	  else {
	    binmin = -4.7+(4.7*2/10.)*ip;
	    binmax = -4.7+(4.7*2/10.)*(ip+1);
	    lcut << " && genjet" << ij << "_eta" << ">=" << binmin << " && "
		 << "genjet" << ij << "_eta" << "<" << binmax
		 << " && genjet" << ij << "_pt" << ">" << minptcut;
	    ltitle << binmin << " #leq #eta^{genjet} < " << binmax;
	  }
	  varval[iv][ir][ip] = (binmax+binmin)/2.;
	  binwidth[iv][ir][ip] = (binmax-binmin);
	  hratio[iv][ir][ip]->SetTitle(ltitle.str().c_str());
	  tree[ir]->Draw(lvar.str().c_str(),lcut.str().c_str());
	  //std::cout << " --- " << lvar.str() << std::endl
	  //	    << " *** " << lcut.str() << std::endl;
	}//loop on points
      }//loop on jets
      
      for (unsigned igr(0);igr<2;++igr){
	gr[iv+2*igr][ir] = new TGraphErrors();
	if (iv==0) gr[iv+2*igr][ir]->SetName("pt");
	else gr[iv+2*igr][ir]->SetName("eta");
	if (iv==0) gr[iv+2*igr][ir]->SetTitle(";p_{T}^{gen} (GeV);<p_{T}^{reco}/p_{T}^{gen}>");
	else gr[iv+2*igr][ir]->SetTitle(";#eta^{gen};<p_{T}^{reco}/p_{T}^{gen}>");
	gr[iv+2*igr][ir]->SetMarkerStyle(22+ir);
	gr[iv+2*igr][ir]->SetMarkerColor(1+ir);
	gr[iv+2*igr][ir]->SetLineColor(1+ir);
	gr[iv+2*igr][ir]->SetMinimum(igr==0?0.7:0);
	gr[iv+2*igr][ir]->SetMaximum(igr==0?1.3:0.3);
      }
      for (unsigned ip(0); ip<nP;++ip){//loop on runs
	myc[nR*iv+ir]->cd(ip+1);
	hratio[iv][ir][ip]->Draw();
	hratio[iv][ir][ip]->Fit("gaus");
	TF1 *fit = (TF1*)hratio[iv][ir][ip]->GetFunction("gaus");
	if (!fit) continue;
	gr[iv][ir]->SetPoint(ip,varval[iv][ir][ip],fit->GetParameter(1));
	gr[iv][ir]->SetPointError(ip,binwidth[iv][ir][ip]/2.,fit->GetParError(1));
	gr[iv+2][ir]->SetPoint(ip,varval[iv][ir][ip],fit->GetParameter(2));
	gr[iv+2][ir]->SetPointError(ip,binwidth[iv][ir][ip]/2.,fit->GetParError(2));
      }
      myc[nR*iv+ir]->Update();
      myc[nR*iv+ir]->Print(("PlotsJMETval/JetResponse_per"+var[iv]+"bin_"+process+"_"+jettype[ir]+".pdf").c_str());
    }//loop on jet types

    gStyle->SetOptStat(0);

    for (unsigned ir(0); ir<nR;++ir){//loop on jettypes
      myc[nR*nV+iv]->cd();
      gr[iv][ir]->Draw(ir==0?"APEL":"PELsame");
      myc[nR*nV+iv+nV]->cd();
      gr[iv+2][ir]->Draw(ir==0?"APEL":"PELsame");
      gr[iv+2][ir]->GetYaxis()->SetTitle("#sigma(p_{T}^{reco}/p_{T}^{gen})");
    }
    myc[nR*nV+iv]->cd();
    lat.DrawLatexNDC(0.16,0.92,("CMS Simulation - "+process).c_str());
    sprintf(buf,"p_{T}^{genjet}>%d GeV",minptcut);
    if (iv==0) lat.DrawLatexNDC(0.5,0.85,"|#eta^{genjet}|<4.7");
    else lat.DrawLatexNDC(0.5,0.85,buf);
    legrun->Draw("same");
    myc[nR*nV+iv]->Update();
    myc[nR*nV+iv]->Print(("PlotsJMETval/JetScale_vs"+var[iv]+"_"+process+".pdf").c_str());

    myc[nR*nV+iv+nV]->cd();
    lat.DrawLatexNDC(0.16,0.92,("CMS Simulation - "+process).c_str());
    sprintf(buf,"p_{T}^{genjet}>%d GeV",minptcut);
    if (iv==0) lat.DrawLatexNDC(0.5,0.85,"|#eta^{genjet}|<4.7");
    else lat.DrawLatexNDC(0.5,0.85,buf);
    legrun->Draw("same");
    myc[nR*nV+iv+nV]->Update();
    myc[nR*nV+iv+nV]->Print(("PlotsJMETval/JetReso_vs"+var[iv]+"_"+process+".pdf").c_str());

  }//loop on variables

  return 0;
}//main

