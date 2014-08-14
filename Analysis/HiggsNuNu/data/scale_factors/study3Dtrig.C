#include "TFile.h"
#include "TGraphErrors.h"
#include "TH3D.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <iomanip>

int study3Dtrig(){//main


  TFile *file = TFile::Open("HLTEffParkedABCD_v1.root");

  TH3D *h[3];
  h[0] = (TH3D*)gDirectory->Get("h3DHLT_Dijet40_A");
  h[1] = (TH3D*)gDirectory->Get("h3DHLT_Dijet35_BC");
  h[2] = (TH3D*)gDirectory->Get("h3DHLT_Dijet30_D");

  std::string period[3] = {"A","BC","D"};
  std::string var[3] = {"pTj2","MET","Mjj"};

  const unsigned nC = 3*3; 
  TCanvas *myc[nC];
  for (unsigned iC(0); iC<nC;++iC){
    std::ostringstream lname,lLabel;
    lname << "myc" << iC;
    lLabel << var[iC%3] << "_run" << period[iC/3];
    myc[iC] = new TCanvas(lname.str().c_str(),lLabel.str().c_str(),1);
  }

  for (unsigned iP(0); iP<3;++iP){//periods
    if (!h[iP]) {
      std::cout << " Error, histo for period " << period[iP] << " not found. Continue..." << std::endl;
      continue;
    }
    TH3D *htmp = h[iP];
    TGraphErrors *gr1D[3][10][10];
 
    for (unsigned iV(0); iV<3; ++iV){//var

      unsigned idx = 3*iP+iV;

      std::ostringstream lLabel,lLabelBase;
      lLabelBase << var[iV] << "_run" << period[iP];
      int n1,n2;
      if (iV==0) { 
	n1 = htmp->GetNbinsY();
	n2 = htmp->GetNbinsZ(); 
      } else if (iV==1) {
	n1 = htmp->GetNbinsX();
	n2 = htmp->GetNbinsZ(); 
      } else {
	n1 = htmp->GetNbinsX();
	n2 = htmp->GetNbinsY(); 
      }

      TLegend *leg1 = new TLegend(0.85,0.2,1.,0.6);
      leg1->SetFillColor(10);
      TLegend *leg2 = new TLegend(0.85,0.6,1.,1.);
      leg2->SetFillColor(10);

      std::cout << lLabelBase.str() << ": " << n1 << "x" << n2 << " bins." << std::endl;
      double maxY = 0;
      for (unsigned iX(1); iX<htmp->GetNbinsX()+1; ++iX){
	for (unsigned iY(1); iY<htmp->GetNbinsY()+1; ++iY){
	  for (unsigned iZ(1); iZ<htmp->GetNbinsZ()+1; ++iZ){
	    unsigned i1,i2,i0;
	    double lval,lval1,lval2;
	    double lvalerr = 0;
	    std::string var1,var2;
	    if (iV==0) { 
	      i1 = iY-1; i2 = iZ-1; i0 = iX-1;
	      lval = htmp->GetXaxis()->GetBinCenter(iX);
	      lval1 = htmp->GetYaxis()->GetBinCenter(iY);
	      lval2 = htmp->GetZaxis()->GetBinCenter(iZ);
	      var1 = var[1]; var2 = var[2];
	    } else if (iV==1){
	      i1 = iX-1; i2 = iZ-1; i0 = iY-1;
	      lval = htmp->GetYaxis()->GetBinCenter(iY);
	      lval1 = htmp->GetXaxis()->GetBinCenter(iX);
	      lval2 = htmp->GetZaxis()->GetBinCenter(iZ);
	      var1 = var[0]; var2 = var[2];
	    } else {
	      i1 = iX-1; i2 = iY-1; i0 = iZ-1;
	      lval = htmp->GetZaxis()->GetBinCenter(iZ);
	      lval1 = htmp->GetXaxis()->GetBinCenter(iX);
	      lval2 = htmp->GetYaxis()->GetBinCenter(iY);
	      var1 = var[0]; var2 = var[1];
	    }

	    if (i0 == 0){
	      lLabel.str("");
	      lLabel << lLabelBase.str() << "_" << i1 << "_" << i2;
	      gr1D[iV][i1][i2] = new TGraphErrors();
	      gr1D[iV][i1][i2]->SetName(lLabel.str().c_str());
	      gr1D[iV][i1][i2]->SetMarkerStyle(20+i1);
	      gr1D[iV][i1][i2]->SetMarkerColor(1+i2);
	      gr1D[iV][i1][i2]->SetLineColor(1+i2);
	      if (i2==0){
		lLabel.str("");
		lLabel << var1 << "=" << lval1;
		leg1->AddEntry(gr1D[iV][i1][i2],lLabel.str().c_str(),"P");
	      }
	      if (i1==0){
		lLabel.str("");
		lLabel << var2 << "=" << lval2;
		leg2->AddEntry(gr1D[iV][i1][i2],lLabel.str().c_str(),"P");
	      }
	    }
	    if (htmp->GetBinContent(iX,iY,iZ)>maxY) maxY = htmp->GetBinContent(iX,iY,iZ);
	    gr1D[iV][i1][i2]->SetPoint(i0,lval,htmp->GetBinContent(iX,iY,iZ));
	    gr1D[iV][i1][i2]->SetPointError(i0,lvalerr,htmp->GetBinError(iX,iY,iZ));
	  }
	}
      }


      myc[idx]->cd();
      for (unsigned i1(0); i1<n1; ++i1){
	for (unsigned i2(0); i2<n2; ++i2){
	  gr1D[iV][i1][i2]->SetMaximum(maxY);
	  gr1D[iV][i1][i2]->GetXaxis()->SetTitle(var[iV].c_str());
	  gr1D[iV][i1][i2]->GetYaxis()->SetTitle("#varepsilon_{trig}");
	  if (i1==0 && i2 == 0) gr1D[iV][i1][i2]->Draw("APL");
	  else gr1D[iV][i1][i2]->Draw("PL");
	}
      }
      leg1->Draw("same");
      leg2->Draw("same");
      myc[idx]->Update();
      TString lname;
      lname = "PLOTS/"+lLabelBase.str();
      myc[idx]->Print(lname+".pdf");
      myc[idx]->Print(lname+".png");


    }//var
  }//period




  return 0;
}//main
