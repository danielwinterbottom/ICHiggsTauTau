#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TKey.h"
#include "TF1.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "RooBinning.h"                                                                                                                                                               
#include "RooRealVar.h"   
#include "RooDataHist.h"   
#include "RooDataSet.h"    
#include "RooHistPdf.h"     
#include "RooGenericPdf.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooMCStudy.h"
#include "RooPlot.h"
#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"

//Clone the file excluding the histogram (code stolen from Rene Brun)
void copyDir(TDirectory *source,std::string iSkipHist,bool iFirst=true) { 
  //copy all objects and subdirs of directory source as a subdir of the current directory   
  TDirectory *savdir = gDirectory;
  TDirectory *adir   = savdir;
  if(!iFirst) adir   = savdir->mkdir(source->GetName());
  if(!iFirst) adir->cd();
  //loop on all entries of this directory
  TKey *key;
  TIter nextkey(source->GetListOfKeys());
  while ((key = (TKey*)nextkey())) {
    const char *classname = key->GetClassName();
    TClass *cl = gROOT->GetClass(classname);
    if (!cl) continue;
    if (cl->InheritsFrom(TDirectory::Class())) {
      source->cd(key->GetName());
      TDirectory *subdir = gDirectory;
      adir->cd();
      copyDir(subdir,iSkipHist,false);
      adir->cd();
    } else {
      source->cd();
      TObject *obj = key->ReadObj();
      std::string pFullName = std::string(adir->GetName())+"/"+std::string(obj->GetName());
      std::string iSkipHist2 = iSkipHist;
      std::string fine_binning = "_fine_binning";
      iSkipHist2.replace(iSkipHist2.find(fine_binning), fine_binning.length(),"");
      if(pFullName.find(iSkipHist) != std::string::npos || pFullName.find(iSkipHist2) != std::string::npos) {
	continue;
      }
      adir->cd();
      obj->Write();
      delete obj;
    }
  }
  adir->SaveSelf(kTRUE);
  savdir->cd();
}
void cloneFile(TFile *iOutputFile,TFile *iReadFile,std::string iSkipHist) {
  //copy all objects and subdirs of directory source as a subdir of the current directory   
  iOutputFile->cd();
  copyDir(iReadFile,iSkipHist);
}  
//Rebin the histogram
TH1F* rebin(TH1F* iH,int iNBins,double *iAxis) { 
  std::string lTmp = "tmp"; //Added to avoid Root output errors
  TH1F *lH = new TH1F(lTmp.c_str(),lTmp.c_str(),iNBins,iAxis);
  for(int i0 = 0; i0 < iH->GetNbinsX()+1; i0++) {
    int    lNBin = lH->GetXaxis()->FindBin(iH->GetXaxis()->GetBinCenter(i0));
    double lVal  = iH->GetBinContent(i0);
    double lErr  = iH->GetBinError  (i0);
    double lOldV = lH->GetBinContent(lNBin);
    double lOldE = lH->GetBinError  (lNBin);
    lH->SetBinContent(lNBin,lVal+lOldV);
    lH->SetBinError  (lNBin,sqrt(lOldE*lOldE+lErr*lErr));
  }
  std::string lName2 = iH->GetName();
  std::string fine_binning = "_fine_binning";
  lName2.replace(lName2.find(fine_binning),fine_binning.length(),"");
  lH->SetName (lName2.c_str());
  lH->SetTitle(lName2.c_str());
  delete iH;
  return lH;
}
//Merge Histogram with function histogram
TH1F * merge(std::string iName,double iMergePoint,TH1F *iH,TH1F *iFunc) {
  cout << "====> Name " << iName << " -- " << iFunc << " -- " << iH << endl;
  TH1F *lH = (TH1F*) iH->Clone(iName.c_str());
  lH->SetFillStyle(0);
  int lMergeBin = iH->GetXaxis()->FindBin(iMergePoint);
  double lVal  = iH->GetBinContent(lMergeBin);
  //iFunc->Scale(lVal/iFunc->GetBinContent(lMergeBin));
  iFunc->Scale( (iH->Integral(lMergeBin, iH->GetXaxis()->FindBin(1500))) / (iFunc->Integral(lMergeBin, iFunc->GetXaxis()->FindBin(1500)) )); // felix - last fit bin = 1500; this approach seems to work much better
  for(int i0 = 0;         i0 < lMergeBin;         i0++) lH->SetBinContent(i0,iH->GetBinContent(i0));
  for(int i0 = lMergeBin; i0 < iH->GetNbinsX()+1; i0++) lH->SetBinContent(i0,iFunc->GetBinContent(iFunc->GetXaxis()->FindBin(lH->GetXaxis()->GetBinCenter(i0))));
  lH->SetName(iName.c_str());
  return lH;
}
//Difference plotting
void drawDifference(TH1* iH0,TH1 *iH1,TH1 *iHH=0,TH1 *iHL=0,TH1 *iHH1=0,TH1 *iHL1=0) {
  std::string lName = std::string(iH0->GetName());
  TH1F *lHDiff   = (TH1F*) iH0->Clone("Diff");
  TH1F *lHDiffH  = (TH1F*) iH0->Clone("DiffH");
  TH1F *lHDiffL  = (TH1F*) iH0->Clone("DiffL"); 
  TH1F *lHDiffH1 = (TH1F*) iH0->Clone("DiffH1");
  TH1F *lHDiffL1 = (TH1F*) iH0->Clone("DiffL1"); 
  lHDiff  ->SetFillColor(kViolet); lHDiff->SetFillStyle(1001); lHDiff->SetLineWidth(1);
  lHDiffL ->SetLineWidth(1); lHDiffL ->SetLineColor(iHL ->GetLineColor());
  lHDiffH ->SetLineWidth(1); lHDiffH ->SetLineColor(iHH ->GetLineColor());
  lHDiffL1->SetLineWidth(1); lHDiffL1->SetLineColor(iHL1->GetLineColor());
  lHDiffH1->SetLineWidth(1); lHDiffH1->SetLineColor(iHH1->GetLineColor());
  TH1F *lXHDiff1 = new TH1F((lName+"XDiff1").c_str(),(lName+"XDiff1").c_str(),iH0->GetNbinsX(),iH0->GetXaxis()->GetXmin(),iH0->GetXaxis()->GetXmax());
  TH1F *lXHDiff2 = new TH1F((lName+"XDiff2").c_str(),(lName+"XDiff2").c_str(),iH0->GetNbinsX(),iH0->GetXaxis()->GetXmin(),iH0->GetXaxis()->GetXmax());
  int i1 = 0;
  lXHDiff1->SetLineWidth(2); lXHDiff1->SetLineColor(kRed);
  lXHDiff2->SetLineWidth(2); lXHDiff2->SetLineColor(kRed);

  lXHDiff1->GetYaxis()->SetTitle("Ratio");
  lXHDiff1->GetYaxis()->SetRangeUser(0.2,1.8);
  lXHDiff1->GetYaxis()->SetTitleOffset(0.4);
  lXHDiff1->GetYaxis()->SetTitleSize(0.2);
  lXHDiff1->GetYaxis()->SetLabelSize(0.11);
  for(int i0 = 0; i0 < lHDiff->GetNbinsX()+1; i0++) {
    double lXCenter = lHDiff->GetBinCenter(i0);
    double lXVal     = iH0   ->GetBinContent(i0);
    double lXValH    = iHH   ->GetBinContent(i0);
    double lXValL    = iHL   ->GetBinContent(i0);
    double lXValH1   = iHH1  ->GetBinContent(i0);
    double lXValL1   = iHL1  ->GetBinContent(i0);
    lXHDiff1->SetBinContent(i0, 1.0);
    lXHDiff2->SetBinContent(i0, 1.0);
    while(iH1->GetBinCenter(i1) < lXCenter) {i1++;}
    if(iH1->GetBinContent(i0) > 0) lHDiff->SetBinContent(i0,lXVal      /(iH1->GetBinContent(i0)));
    if(iH1->GetBinContent(i0) > 0) lHDiff->SetBinError  (i0,sqrt(lXVal)/(iH1->GetBinContent(i0)));
    if(iH1->GetBinContent(i0) > 0) lHDiffL->SetBinContent(i0,lXValL/(iH1->GetBinContent(i0)));
    if(iH1->GetBinContent(i0) > 0) lHDiffH->SetBinContent(i0,lXValH/(iH1->GetBinContent(i0)));
    if(iH1->GetBinContent(i0) > 0) lHDiffL1->SetBinContent(i0,lXValL1/(iH1->GetBinContent(i0)));
    if(iH1->GetBinContent(i0) > 0) lHDiffH1->SetBinContent(i0,lXValH1/(iH1->GetBinContent(i0)));
   //if(iH1->GetBinContent(i0) > 0)  cout << "unc" << lXVal << " -- " << sqrt(lXVal)/(iH1->GetBinContent(i0)) << endl;
  }
  lHDiff->SetMarkerStyle(kFullCircle);
  //lHDiff->Draw("EP");
  
  lXHDiff1->SetStats(0);
  lXHDiff2->SetStats(0);
  lHDiff->SetStats(0);
  lHDiffH->SetStats(0);
  lHDiffL->SetStats(0);
  lHDiffH1->SetStats(0);
  lHDiffL1->SetStats(0);

  lXHDiff1->Draw("hist");
  lXHDiff2->Draw("hist sames");
  lHDiff->Draw("EP sames");
  lHDiffH ->Draw("hist sames");
  lHDiffL ->Draw("hist sames");
  lHDiffH1->Draw("hist sames");
  lHDiffL1->Draw("hist sames");
}
//Get Axis from a TH1F
double * getAxis(TH1F *iH) { 
  const int lNHBins = iH->GetNbinsX(); 
  double *lX = new double[lNHBins+1];
  for(int i0 = 1; i0 < iH->GetNbinsX()+2; i0++) { 
    lX[i0-1] = iH->GetXaxis()->GetBinLowEdge(i0);
  }
  return lX;
}
//Make a histogram from a TF1
TH1F* makeHist(TF1 *iFit,TH1F *iH,std::string iName) { 
  TH1F *lH = (TH1F*) iH->Clone(iName.c_str());
  for(int i0 = 0; i0 < lH->GetNbinsX()+1; i0++) lH->SetBinContent(i0,iFit->Eval(lH->GetXaxis()->GetBinCenter(i0)));
  for(int i0 = 0; i0 < lH->GetNbinsX()+1; i0++) lH->SetBinContent(i0,lH->GetBinContent(i0)*lH->GetXaxis()->GetBinWidth(i0));
  for(int i0 = 0; i0 < lH->GetNbinsX()+1; i0++) lH->SetBinError (i0,lH->GetBinError   (i0)*lH->GetXaxis()->GetBinWidth(i0));
  return lH;
}
//I would recommend to use the other version of the fit code
void addVarBinNuisance(std::string iFileName,std::string iChannel,std::string iBkg,std::string iEnergy,std::string iName,std::string iDir,bool iRebin=true,int iFitModel=0,double iFirst=200,double iLast=1500) { 
  std::cout << "======> " << iDir << "/" << iBkg << " -- " << iFileName << std::endl;  
  TFile *lFile = new TFile(iFileName.c_str());
  TH1F  *lH0   = (TH1F*) lFile->Get((iDir+"/"+iBkg).c_str());
  TH1F  *lData = (TH1F*) lFile->Get((iDir+"/data_obs").c_str());
  
  for(int i0 = 0; i0 < lH0->GetNbinsX()+1; i0++) lH0->SetBinContent(i0,lH0->GetBinContent(i0)/lH0->GetXaxis()->GetBinWidth(i0));
  for(int i0 = 0; i0 < lH0->GetNbinsX()+1; i0++) lH0->SetBinError  (i0,lH0->GetBinError  (i0)/lH0->GetXaxis()->GetBinWidth(i0));
  //Define the fit function
  double lFirst = iFirst;
  double lLast  = iLast;
  //TF1 *lFit = new TF1("Fit","[2]*exp(-x/([0]+[1]*x))",0,5000);
  TF1 *lFit = new TF1("expspec","[2]*exp(-x/([0]+[1]*x))",0,5000);
  if(iFitModel == 1) lFit  = new TF1("expspec","[2]*exp(-[0]*pow(x,[1]))",0,5000);
  lFit->SetParLimits(2,0,10000000); lFit->SetParameter(2,lH0->Integral()); 
  lFit->SetParLimits(0,  0,100);    lFit->SetParameter(0,20);
  lFit->SetParLimits(1,-10,10);     lFit->SetParameter(1,0);
  if(iFitModel == 1) lFit->SetParameter(0,0.3);
  if(iFitModel == 2) lFit->SetParameter(1,0.5);
  
  //TFitResultPtr  lFitPtr = lH0->Fit("expspec","SEWL","IR",lFirst,lLast);
  TFitResultPtr  lFitPtr = lH0->Fit("expspec","SER","R",lFirst,lLast);
  TMatrixDSym lCovMatrix   = lFitPtr->GetCovarianceMatrix();
  TMatrixD  lEigVecs(3,3);    lEigVecs = TMatrixDSymEigen(lCovMatrix).GetEigenVectors();
  TVectorD  lEigVals(3);      lEigVals = TMatrixDSymEigen(lCovMatrix).GetEigenValues();
  double lACentral = lFit->GetParameter(0); 
  double lBCentral = lFit->GetParameter(1);
  lEigVals(0) = sqrt(lEigVals(1));
  lEigVals(1) = sqrt(lEigVals(2));

  for(int i0 = 0; i0 < lH0->GetNbinsX()+1; i0++) lH0->SetBinContent(i0,lH0->GetBinContent(i0)*lH0->GetXaxis()->GetBinWidth(i0));
  for(int i0 = 0; i0 < lH0->GetNbinsX()+1; i0++) lH0->SetBinError  (i0,lH0->GetBinError  (i0)*lH0->GetXaxis()->GetBinWidth(i0));

  lEigVecs(0,0) = lEigVecs(0,1);
  lEigVecs(1,0) = lEigVecs(1,1);
  lEigVecs(0,1) = lEigVecs(0,2);
  lEigVecs(1,1) = lEigVecs(1,2);
  
  TH1F* lH     = makeHist(lFit,lH0,"Def");
  lFit->SetParameter(0,lACentral + lEigVals(0)*lEigVecs(0,0));
  lFit->SetParameter(1,lBCentral + lEigVals(0)*lEigVecs(1,0));
  TH1F* lHUp   = makeHist(lFit,lH0,"Up");
  lFit->SetParameter(0,lACentral - lEigVals(0)*lEigVecs(0,0));
  lFit->SetParameter(1,lBCentral - lEigVals(0)*lEigVecs(1,0));
  TH1F* lHDown = makeHist(lFit,lH0,"Down");

  lFit->SetParameter(0,lACentral + lEigVals(1)*lEigVecs(0,1));
  lFit->SetParameter(1,lBCentral + lEigVals(1)*lEigVecs(1,1));
  TH1F* lHUp1   = makeHist(lFit,lH0,"Up1");
  lFit->SetParameter(0,lACentral - lEigVals(1)*lEigVecs(0,1));
  lFit->SetParameter(1,lBCentral - lEigVals(1)*lEigVecs(1,1));
  TH1F* lHDown1 = makeHist(lFit,lH0,"Down1");
  
  //lFirst = 200;
  std::string lNuisance1 =  iBkg+"_"+"CMS_"+iName+"1_" + iChannel + "_" + iEnergy;
  std::string lNuisance2 =  iBkg+"_"+"CMS_"+iName+"2_" + iChannel + "_" + iEnergy;
  lHUp    = merge(lNuisance1 + "Up"   ,lFirst,lH0,lHUp);
  lHDown  = merge(lNuisance1 + "Down" ,lFirst,lH0,lHDown);
  lHUp1   = merge(lNuisance2 + "Up"   ,lFirst,lH0,lHUp1);
  lHDown1 = merge(lNuisance2 + "Down" ,lFirst,lH0,lHDown1);
  lH      = merge(lH0->GetName()      ,lFirst,lH0,lH);  
  if(iRebin) { 
    const int lNBins = lData->GetNbinsX();
    double *lAxis    = getAxis(lData);
    lH0     = rebin(lH0    ,lNBins,lAxis);
    lH      = rebin(lH     ,lNBins,lAxis);
    lHUp    = rebin(lHUp   ,lNBins,lAxis);
    lHDown  = rebin(lHDown ,lNBins,lAxis);
    lHUp1   = rebin(lHUp1  ,lNBins,lAxis);
    lHDown1 = rebin(lHDown1,lNBins,lAxis);
  }
  TFile *lOutFile =new TFile("Output.root","RECREATE");
  cloneFile(lOutFile,lFile,iDir+"/"+iBkg);
  lOutFile->cd(iDir.c_str());
  lH     ->Write();
  lHUp   ->Write(); 
  lHDown ->Write(); 
  lHUp1  ->Write(); 
  lHDown1->Write(); 

  // Debug Plots
  lH0    ->SetLineWidth(1); lH0->SetMarkerStyle(kFullCircle);
  lH     ->SetLineColor(kGreen);
  lHUp   ->SetLineColor(kRed);
  lHDown ->SetLineColor(kRed);
  lHUp1  ->SetLineColor(kBlue);
  lHDown1->SetLineColor(kBlue);
  TCanvas *lC0 = new TCanvas("Can","Can",800,600);
  lC0->Divide(1,2); lC0->cd();  lC0->cd(1)->SetPad(0,0.2,1.0,1.0); gPad->SetLeftMargin(0.2) ;  
  lH0->Draw();
  lH     ->Draw("hist sames");
  lHUp   ->Draw("hist sames");
  lHDown ->Draw("hist sames");
  lHUp1  ->Draw("hist sames");
  lHDown1->Draw("hist sames");
  gPad->SetLogy();
  lC0->cd(2)->SetPad(0,0,1.0,0.2); gPad->SetLeftMargin(0.2) ;
  drawDifference(lH0,lH,lHUp,lHDown,lHUp1,lHDown1);
  lC0->SaveAs((iBkg+"_"+"CMS_"+iName+"1_" + iDir + "_" + iEnergy+".png").c_str());
  //lFile->Close();
  return;
}
void addNuisance(std::string iFileName,std::string iChannel,std::string iBkg,std::string iEnergy,std::string iName,std::string iDir,bool iRebin=true,bool iVarBin=false,int iFitModel=1,double iFirst=150,double iLast=1500) { 
  std::cout << "======> " << iDir << "/" << iBkg << " -- " << iFileName << std::endl;  
  if(iVarBin) addVarBinNuisance(iFileName,iChannel,iBkg,iEnergy,iName,iDir,iRebin,iFitModel,iFirst,iLast);
  if(iVarBin) return;

  TFile *lFile = new TFile(iFileName.c_str());
  TH1F  *lH0   = (TH1F*) lFile->Get((iDir+"/"+iBkg).c_str());
  TH1F  *lData = (TH1F*) lFile->Get((iDir+"/data_obs").c_str());

  //Define the fit function
  RooRealVar lM("m","m" ,0,5000);   //lM.setBinning(lBinning);
  RooRealVar lA("a","a" ,50,  0.1,100);
  RooRealVar lB("b","b" ,0.0 , -10.5,10.5); //lB.setConstant(kTRUE);
  RooDataHist *pH0  =  new RooDataHist("Data","Data" ,RooArgList(lM),lH0);
  RooGenericPdf *lFit  = 0; lFit = new RooGenericPdf("genPdf","exp(-m/(a+b*m))",RooArgList(lM,lA,lB));
  if(iFitModel == 1) lFit = new RooGenericPdf("genPdf","exp(-a*pow(m,b))",RooArgList(lM,lA,lB));
  if(iFitModel == 1) {lA.setVal(0.3); lB.setVal(0.5);}
  if(iFitModel == 2) lFit = new RooGenericPdf("genPdf","a*exp(b*m)",RooArgList(lM,lA,lB));
  if(iFitModel == 3) lFit = new RooGenericPdf("genPdf","a/pow(m,b)",RooArgList(lM,lA,lB));
  RooFitResult  *lRFit = 0;
  double lFirst = iFirst;
  double lLast  = iLast;
  //lRFit = lFit->chi2FitTo(*pH0,RooFit::Save(kTRUE),RooFit::Range(lFirst,lLast));
  lRFit = lFit->fitTo(*pH0,RooFit::Save(kTRUE),RooFit::Range(lFirst,lLast),RooFit::Strategy(0)); 
  TMatrixDSym lCovMatrix   = lRFit->covarianceMatrix(); 
  TMatrixD  lEigVecs(2,2);    lEigVecs = TMatrixDSymEigen(lCovMatrix).GetEigenVectors();
  TVectorD  lEigVals(2);      lEigVals = TMatrixDSymEigen(lCovMatrix).GetEigenValues();
  cout << " Ve---> " << lEigVecs(0,0) << " -- " << lEigVecs(1,0) << " -- " << lEigVecs(0,1) << " -- " << lEigVecs(1,1) << endl;
  cout << " Co---> " << lCovMatrix(0,0) << " -- " << lCovMatrix(1,0) << " -- " << lCovMatrix(0,1) << " -- " << lCovMatrix(1,1) << endl;
  double lACentral = lA.getVal();
  double lBCentral = lB.getVal();
  lEigVals(0) = sqrt(lEigVals(0));
  lEigVals(1) = sqrt(lEigVals(1));
  cout << "===> " << lEigVals(0) << " -- " << lEigVals(1) << endl;
  
  TH1F* lH     = (TH1F*) lFit->createHistogram("fit" ,lM,RooFit::Binning(lH0->GetNbinsX(),lH0->GetXaxis()->GetXmin(),lH0->GetXaxis()->GetXmax()));
  lA.setVal(lACentral + lEigVals(0)*lEigVecs(0,0));
  lB.setVal(lBCentral + lEigVals(0)*lEigVecs(1,0));
  TH1F* lHUp   = (TH1F*) lFit->createHistogram("Up"  ,lM,RooFit::Binning(lH0->GetNbinsX(),lH0->GetXaxis()->GetXmin(),lH0->GetXaxis()->GetXmax()));
  lA.setVal(lACentral - lEigVals(0)*lEigVecs(0,0));
  lB.setVal(lBCentral - lEigVals(0)*lEigVecs(1,0));
  TH1F* lHDown = (TH1F*) lFit->createHistogram("Down",lM,RooFit::Binning(lH0->GetNbinsX(),lH0->GetXaxis()->GetXmin(),lH0->GetXaxis()->GetXmax()));

  lA.setVal(lACentral + lEigVals(1)*lEigVecs(0,1));
  lB.setVal(lBCentral + lEigVals(1)*lEigVecs(1,1));
  TH1F* lHUp1   = (TH1F*) lFit->createHistogram("Up1",lM,RooFit::Binning(lH0->GetNbinsX(),lH0->GetXaxis()->GetXmin(),lH0->GetXaxis()->GetXmax()));
  lA.setVal(lACentral - lEigVals(1)*lEigVecs(0,1));
  lB.setVal(lBCentral - lEigVals(1)*lEigVecs(1,1));
  TH1F* lHDown1 = (TH1F*) lFit->createHistogram("Down1",lM,RooFit::Binning(lH0->GetNbinsX(),lH0->GetXaxis()->GetXmin(),lH0->GetXaxis()->GetXmax()));

  std::string lNuisance1 =  iBkg+"_"+"CMS_"+iName+"1_" + iChannel + "_" + iEnergy;
  std::string lNuisance2 =  iBkg+"_"+"CMS_"+iName+"2_" + iChannel + "_" + iEnergy;
  lHUp    = merge(lNuisance1 + "Up"   ,lFirst,lH0,lHUp);
  lHDown  = merge(lNuisance1 + "Down" ,lFirst,lH0,lHDown);
  lHUp1   = merge(lNuisance2 + "Up"   ,lFirst,lH0,lHUp1);
  lHDown1 = merge(lNuisance2 + "Down" ,lFirst,lH0,lHDown1);
  lH      = merge(lH0->GetName()      ,lFirst,lH0,lH);

  if(iRebin) { 
    const int lNBins = lData->GetNbinsX();
    double *lAxis    = getAxis(lData);
    lH0     = rebin(lH0    ,lNBins,lAxis);
    lH      = rebin(lH     ,lNBins,lAxis);
    lHUp    = rebin(lHUp   ,lNBins,lAxis);
    lHDown  = rebin(lHDown ,lNBins,lAxis);
    lHUp1   = rebin(lHUp1  ,lNBins,lAxis);
    lHDown1 = rebin(lHDown1,lNBins,lAxis);
  }

  // we dont need this bin errors since we do not use them (fit tails replaces bin-by-bin error!), therefore i set all errors to 0, this also saves us from modifying the add_bbb_error.py script in which I otherwise would have to include a option for adding bbb only in specific ranges
  int lMergeBin = lH->GetXaxis()->FindBin(iFirst);
  for(int i0 = lMergeBin; i0 < lH->GetNbinsX()+1; i0++){
    lH->SetBinError  (i0,0);
    lHUp->SetBinError  (i0,0);
    lHDown->SetBinError  (i0,0);
    lHUp1->SetBinError  (i0,0);
    lHDown1->SetBinError  (i0,0);
  }


  TFile *lOutFile =new TFile("Output.root","RECREATE");
  cloneFile(lOutFile,lFile,iDir+"/"+iBkg);
  lOutFile->cd(iDir.c_str());
  lH     ->Write();
  lHUp   ->Write(); 
  lHDown ->Write(); 
  lHUp1  ->Write(); 
  lHDown1->Write(); 

  // Debug Plots
  lH0->SetStats(0);
  lH->SetStats(0);
  lHUp->SetStats(0);
  lHDown->SetStats(0);
  lHUp1->SetStats(0);
  lHDown1->SetStats(0);
  lH0    ->SetLineWidth(1); lH0->SetMarkerStyle(kFullCircle);
  lH     ->SetLineColor(kGreen);
  lHUp   ->SetLineColor(kRed);
  lHDown ->SetLineColor(kRed);
  lHUp1  ->SetLineColor(kBlue);
  lHDown1->SetLineColor(kBlue);
  TCanvas *lC0 = new TCanvas("Can","Can",800,600);
  lC0->Divide(1,2); lC0->cd();  lC0->cd(1)->SetPad(0,0.2,1.0,1.0); gPad->SetLeftMargin(0.2) ; 
  lH0->Draw();
  lH     ->Draw("hist sames");
  lHUp   ->Draw("hist sames");
  lHDown ->Draw("hist sames");
  lHUp1  ->Draw("hist sames");
  lHDown1->Draw("hist sames");
  gPad->SetLogy();
  
  TLegend* leg1;
  /// setup the CMS Preliminary
  leg1 = new TLegend(0.7, 0.80, 1, 1); 
  leg1->SetBorderSize( 0 );
  leg1->SetFillStyle ( 1001 );
  leg1->SetFillColor (kWhite);
  leg1->AddEntry( lH0 , "orignal",  "PL" );
  leg1->AddEntry( lH , "cental fit",  "L" );
  leg1->AddEntry( lHUp , "shift1 up",  "L" );
  leg1->AddEntry( lHDown , "shift1 down",  "L" );
  leg1->AddEntry( lHUp1 , "shift2 up",  "L" );
  leg1->AddEntry( lHDown1 , "shift2 down",  "L" );
  leg1->Draw("same");


  lC0->cd(2)->SetPad(0,0,1.0,0.2); gPad->SetLeftMargin(0.2) ;
  drawDifference(lH0,lH,lHUp,lHDown,lHUp1,lHDown1);
  lH0->SetStats(0);
  lC0->Update();
  lC0->SaveAs((iBkg+"_"+"CMS_"+iName+"1_" + iDir + "_" + iEnergy+".png").c_str());
  //lFile->Close();
  return;
}
void addNuisanceWithToys(std::string iFileName,std::string iChannel,std::string iBkg,std::string iEnergy,std::string iName,std::string iDir,bool iRebin=true,bool iVarBin=false,int iFitModel=1,double iFirst=150,double iLast=1500,std::string iSigMass="500",double iSigScale=0.20,int iNToys=1000) { 
  std::cout << "======> " << iDir << "/" << iBkg << " -- " << iFileName << std::endl;  
  if(iVarBin) std::cout << "option not implemented yet!";
  if(iVarBin) return;
  //double lFirst = 200;
  //double lLast  = 1500;
  double lFirst = iFirst;
  double lLast  = iLast;

  TFile *lFile = new TFile(iFileName.c_str());
  TH1F  *lH0   = (TH1F*) lFile->Get((iDir+"/"+iBkg).c_str());
  TH1F  *lData = (TH1F*) lFile->Get((iDir+"/data_obs").c_str());
  TH1F  *lSig = 0;
  // for now, use bbH signal for testing in b-tag and ggH in no-btag
  if(iDir.find("_btag") != std::string::npos) lSig = (TH1F*)lFile->Get((iDir+"/bbH"+iSigMass+"_fine_binning").c_str());
  else lSig = (TH1F*)lFile->Get((iDir+"/ggH"+iSigMass+"_fine_binning").c_str());

  TH1F *lH0Clone = (TH1F*)lH0->Clone("lH0Clone");     // binning too fine as of now? start by rebinning
  TH1F *lDataClone = (TH1F*)lData->Clone("lDataClone");   
  TH1F *lSigClone = (TH1F*)lSig->Clone("lSigClone");  
 // lH0Clone->Rebin(2);
 // lDataClone->Rebin(2);
 // lSigClone->Rebin(2);

  //Define the fit function
  RooRealVar lM("m","m" ,0,5000);
  lM.setRange(lFirst,lLast);
  RooRealVar lA("a","a" ,50,  0.1,200);
  RooRealVar lB("b","b" ,0.0 , -10.5,10.5);
  RooRealVar lA1("a1","a1" ,50,  0.1,1000);
  RooRealVar lB1("b1","b1" ,0.0 , -10.5,10.5);

  RooDataHist *pH0  =  new RooDataHist("Data","Data" ,RooArgList(lM),lH0Clone);
  double lNB0 = lH0Clone->Integral(lH0Clone->FindBin(lFirst),lH0Clone->FindBin(lLast));
  lNB0=lNB0*1;
  double lNSig0 = lSig->Integral(lSigClone->FindBin(lFirst),lSigClone->FindBin(lLast));
  lNSig0=lNSig0*1;
  lH0->Scale(1);
  lH0Clone->Scale(1);
  lSig->Scale(iSigScale*lNB0/lNSig0);                                         // scale signal to iSigScale*(Background yield), could try other options
  lSigClone->Scale(iSigScale*lNB0/lNSig0);                                         // scale signal to iSigScale*(Background yield), could try other options
   
  //Generate the "default" fit model 

  RooGenericPdf *lFit  = 0; lFit = new RooGenericPdf("genPdf","exp(-m/(a+b*m))",RooArgList(lM,lA,lB));
  if(iFitModel == 1) lFit = new RooGenericPdf("genPdf","exp(-a*pow(m,b))",RooArgList(lM,lA,lB));
  if(iFitModel == 1) {lA.setVal(0.3); lB.setVal(0.5);}
  if(iFitModel == 2) lFit = new RooGenericPdf("genPdf","a*exp(b*m)",RooArgList(lM,lA,lB));
  if(iFitModel == 3) lFit = new RooGenericPdf("genPdf","a/pow(m,b)",RooArgList(lM,lA,lB));
 
  // Generate the alternative model, with a user defined range
  RooGenericPdf *lFit1  = 0; lFit1 = new RooGenericPdf("genPdf","exp(-m/(a1+b1*m))",RooArgList(lM,lA1,lB1));
  if(iFitModel == 1) lFit1 = new RooGenericPdf("genPdf","exp(-a1*pow(m,b1))",RooArgList(lM,lA1,lB1));
  if(iFitModel == 1) {lA1.setVal(0.3); lB1.setVal(0.5);}
  if(iFitModel == 2) lFit1 = new RooGenericPdf("genPdf","a1*exp(b1*m)",RooArgList(lM,lA1,lB1));
  if(iFitModel == 3) lFit1 = new RooGenericPdf("genPdf","a1/pow(m,b1)",RooArgList(lM,lA1,lB1));

  RooFitResult  *lRFit = 0;
  lRFit = lFit->fitTo(*pH0,RooFit::Save(kTRUE),RooFit::Range(lFirst,lLast),RooFit::Strategy(0)); 
  TMatrixDSym lCovMatrix   = lRFit->covarianceMatrix(); 
  TMatrixD  lEigVecs(2,2);    lEigVecs = TMatrixDSymEigen(lCovMatrix).GetEigenVectors();
  TVectorD  lEigVals(2);      lEigVals = TMatrixDSymEigen(lCovMatrix).GetEigenValues();
  cout << " Ve---> " << lEigVecs(0,0) << " -- " << lEigVecs(1,0) << " -- " << lEigVecs(0,1) << " -- " << lEigVecs(1,1) << endl;
  cout << " Co---> " << lCovMatrix(0,0) << " -- " << lCovMatrix(1,0) << " -- " << lCovMatrix(0,1) << " -- " << lCovMatrix(1,1) << endl;
  double lACentral = lA.getVal();
  double lBCentral = lB.getVal();
  lEigVals(0) = sqrt(lEigVals(0));
  lEigVals(1) = sqrt(lEigVals(1));
  cout << "===> " << lEigVals(0) << " -- " << lEigVals(1) << endl;
  
  TH1F* lH     = (TH1F*) lFit->createHistogram("fit" ,lM,RooFit::Binning(lH0->GetNbinsX(),lH0->GetXaxis()->GetXmin(),lH0->GetXaxis()->GetXmax()));

  lA.setVal(lACentral + lEigVals(0)*lEigVecs(0,0));
  lB.setVal(lBCentral + lEigVals(0)*lEigVecs(1,0));
  TH1F* lHUp   = (TH1F*) lFit->createHistogram("Up"  ,lM,RooFit::Binning(lH0->GetNbinsX(),lH0->GetXaxis()->GetXmin(),lH0->GetXaxis()->GetXmax()));

  lA.setVal(lACentral - lEigVals(0)*lEigVecs(0,0));
  lB.setVal(lBCentral - lEigVals(0)*lEigVecs(1,0));
  TH1F* lHDown = (TH1F*) lFit->createHistogram("Down",lM,RooFit::Binning(lH0->GetNbinsX(),lH0->GetXaxis()->GetXmin(),lH0->GetXaxis()->GetXmax()));

  lA.setVal(lACentral + lEigVals(1)*lEigVecs(0,1));
  lB.setVal(lBCentral + lEigVals(1)*lEigVecs(1,1));
  TH1F* lHUp1   = (TH1F*) lFit->createHistogram("Up1",lM,RooFit::Binning(lH0->GetNbinsX(),lH0->GetXaxis()->GetXmin(),lH0->GetXaxis()->GetXmax()));

  lA.setVal(lACentral - lEigVals(1)*lEigVecs(0,1));
  lB.setVal(lBCentral - lEigVals(1)*lEigVecs(1,1));
  TH1F* lHDown1 = (TH1F*) lFit->createHistogram("Down1",lM,RooFit::Binning(lH0->GetNbinsX(),lH0->GetXaxis()->GetXmin(),lH0->GetXaxis()->GetXmax()));

  std::string lNuisance1 =  iBkg+"_"+"CMS_"+iName+"1_" + iChannel + "_" + iEnergy;
  std::string lNuisance2 =  iBkg+"_"+"CMS_"+iName+"2_" + iChannel + "_" + iEnergy;
  lHUp    = merge(lNuisance1 + "Up"   ,lFirst,lH0,lHUp);
  lHDown  = merge(lNuisance1 + "Down" ,lFirst,lH0,lHDown);
  lHUp1   = merge(lNuisance2 + "Up"   ,lFirst,lH0,lHUp1);
  lHDown1 = merge(lNuisance2 + "Down" ,lFirst,lH0,lHDown1);
  lH      = merge(lH0->GetName()      ,lFirst,lH0,lH);

  lA.setVal(lACentral);
  lB.setVal(lBCentral);
  lA.removeRange();
  lB.removeRange();
  RooGenericPdf *lFitFinal  = 0; lFitFinal = new RooGenericPdf("genPdf","exp(-m/(a+b*m))",RooArgList(lM,lA,lB));

  RooFitResult  *lRFit1 = 0;
  lRFit1=lFit1->fitTo(*pH0,RooFit::Save(kTRUE),RooFit::Range(iFirst,iLast),RooFit::Strategy(0)); // fit once with alternative model to get initial values
  
  RooGenericPdf *lFit1Final  = 0; lFit1Final = new RooGenericPdf("genPdf","exp(-m/(a1+b1*m))",RooArgList(lM,lA1,lB1));

  lA1.removeRange();
  lB1.removeRange();
  lA.removeRange();
  lB.removeRange();

  RooRealVar lNB("nb","nb",lNB0,0,1000000);
  RooRealVar lNSig("nsig","nsig",lNSig0,0,100000);

  RooDataHist  *pS      = new RooDataHist("sigH","sigH",RooArgList(lM),lSig);
  RooHistPdf   *lSPdf   = new RooHistPdf ("sigPdf","sigPdf",lM,*pS);

  RooAddPdf    *lGenMod = new RooAddPdf  ("genmod","genmod",RooArgList(*lFitFinal ,*lSPdf),RooArgList(lNB,lNSig));
  RooAddPdf    *lFitMod = new RooAddPdf  ("fitmod","fitmod",RooArgList(*lFit1Final,*lSPdf),RooArgList(lNB,lNSig));
  //RooAddPdf    *lFitMod = new RooAddPdf  ("fitmod","fitmod",RooArgList(*lFit,*lSPdf),RooArgList(lNB,lNSig));

 RooMCStudy   *lToy    = new RooMCStudy(*lGenMod,lM,RooFit::FitModel(*lGenMod),RooFit::Binned(kTRUE),RooFit::Silence(),RooFit::Extended(kTRUE),RooFit::FitOptions(RooFit::Save(kTRUE),RooFit::Strategy(0)),RooFit::Verbose(1));
// RooMCStudy   *lToy    = new RooMCStudy(*lGenMod,lM,RooFit::Binned(kTRUE),RooFit::Silence(),RooFit::Extended(kTRUE),RooFit::FitOptions(RooFit::Save(kTRUE),RooFit::Strategy(0)),RooFit::Verbose(1));

  // generate and fit iNToys toy samples
  std::cout << "Number of background events: " << lNB0 << std::endl;
  lToy->generateAndFit(iNToys,lNB0,kTRUE);

  //RooPlot* lFrame1 = lToy->plotPull(lNSig,RooFit::Bins(100),RooFit::Range(-20,20));
  RooPlot* lFrame1 = lToy->plotPull(lNSig,-20,20,100,kTRUE);
  lFrame1->SetTitle("distribution of pulls on signal yield from toys");
  lFrame1->SetXTitle("N_{sig} pull");

  TCanvas* lC00 = new TCanvas("pulls","pulls",600,600) ;
  lC00->cd();
  lFrame1->GetYaxis()->SetTitleOffset(1.2); 
  lFrame1->GetXaxis()->SetTitleOffset(1.0); 
  lFrame1->Draw() ;
  lC00->SaveAs(("sig_pulls_toyfits_"+iBkg+"_" + iDir + "_" + iEnergy+".png").c_str());

  RooPlot* lFrame2 = lToy->plotParam(lA);
  lFrame2->SetTitle("distribution of values of parameter 1 (a) after toy fit");
  lFrame2->SetXTitle("Parameter 1 (a)");
  TCanvas* lC01 = new TCanvas("valA","valA",600,600) ;
  lFrame2->Draw() ;
  lC01->SaveAs(("valA_toyfits_"+iBkg+"_" + iDir + "_" + iEnergy+".png").c_str());

  RooPlot* lFrame3 = lToy->plotParam(lB);
  lFrame3->SetTitle("distribution of values of parameter 2 (b) after toy fit");
  lFrame3->SetXTitle("Parameter 2 (b)");
  TCanvas* lC02 = new TCanvas("valB","valB",600,600) ;
  lFrame3->Draw() ;
  lC02->SaveAs(("valB_toyfits_"+iBkg+"_" + iDir + "_" + iEnergy+".png").c_str());


  if(iRebin) { 
    const int lNBins = lData->GetNbinsX();
    double *lAxis    = getAxis(lData);
    lH0     = rebin(lH0    ,lNBins,lAxis);
    lH      = rebin(lH     ,lNBins,lAxis);
    lHUp    = rebin(lHUp   ,lNBins,lAxis);
    lHDown  = rebin(lHDown ,lNBins,lAxis);
    lHUp1   = rebin(lHUp1  ,lNBins,lAxis);
    lHDown1 = rebin(lHDown1,lNBins,lAxis);
  }

  // we dont need this bin errors since we do not use them (fit tails replaces bin-by-bin error!), therefore i set all errors to 0, this also saves us from modifying the add_bbb_error.py script in which I otherwise would have to include a option for adding bbb only in specific ranges
  int lMergeBin = lH->GetXaxis()->FindBin(iFirst);
  for(int i0 = lMergeBin; i0 < lH->GetNbinsX()+1; i0++){
    lH->SetBinError  (i0,0);
    lHUp->SetBinError  (i0,0);
    lHDown->SetBinError  (i0,0);
    lHUp1->SetBinError  (i0,0);
    lHDown1->SetBinError  (i0,0);
  }


  TFile *lOutFile =new TFile("Output.root","RECREATE");
  cloneFile(lOutFile,lFile,iDir+"/"+iBkg);
  lOutFile->cd(iDir.c_str());
  lH     ->Write();
  lHUp   ->Write(); 
  lHDown ->Write(); 
  lHUp1  ->Write(); 
  lHDown1->Write(); 

  // Debug Plots
  lH0->SetStats(0);
  lH->SetStats(0);
  lHUp->SetStats(0);
  lHDown->SetStats(0);
  lHUp1->SetStats(0);
  lHDown1->SetStats(0);
  lH0    ->SetLineWidth(1); lH0->SetMarkerStyle(kFullCircle);
  lH     ->SetLineColor(kGreen);
  lHUp   ->SetLineColor(kRed);
  lHDown ->SetLineColor(kRed);
  lHUp1  ->SetLineColor(kBlue);
  lHDown1->SetLineColor(kBlue);
  TCanvas *lC0 = new TCanvas("Can","Can",800,600);
  lC0->Divide(1,2); lC0->cd();  lC0->cd(1)->SetPad(0,0.2,1.0,1.0); gPad->SetLeftMargin(0.2) ; 
  lH0->Draw();
  lH     ->Draw("hist sames");
  lHUp   ->Draw("hist sames");
  lHDown ->Draw("hist sames");
  lHUp1  ->Draw("hist sames");
  lHDown1->Draw("hist sames");
  gPad->SetLogy();
  
  TLegend* leg1;
  /// setup the CMS Preliminary
  leg1 = new TLegend(0.7, 0.80, 1, 1); 
  leg1->SetBorderSize( 0 );
  leg1->SetFillStyle ( 1001 );
  leg1->SetFillColor (kWhite);
  leg1->AddEntry( lH0 , "orignal",  "PL" );
  leg1->AddEntry( lH , "cental fit",  "L" );
  leg1->AddEntry( lHUp , "shift1 up",  "L" );
  leg1->AddEntry( lHDown , "shift1 down",  "L" );
  leg1->AddEntry( lHUp1 , "shift2 up",  "L" );
  leg1->AddEntry( lHDown1 , "shift2 down",  "L" );
  leg1->Draw("same");


  lC0->cd(2)->SetPad(0,0,1.0,0.2); gPad->SetLeftMargin(0.2) ;
  drawDifference(lH0,lH,lHUp,lHDown,lHUp1,lHDown1);
  lH0->SetStats(0);
  lC0->Update();
  lC0->SaveAs((iBkg+"_"+"CMS_"+iName+"1_" + iDir + "_" + iEnergy+".png").c_str());
  //lFile->Close();
  return;
}
//void addFitNuisance(std::string iFileName="test.root",std::string iChannel="muTau",std::string iBkg="W",std::string iEnergy="8TeV",std::string iName="shift",std::string iCategory="9",double iFirst=150,int iFitModel=1,bool iVarBin=true,bool iRebin=true) { 
void addFitNuisanceBiasStudy(std::string iFileName="test.root",std::string iChannel="emu",std::string iBkg="ttbar_fine_binning",std::string iEnergy="8TeV",std::string iName="shift",std::string iCategory="9",double iFirst=150,double iLast=1500,int iFitModel=0,bool iVarBin=false,bool iRebin=false) { 
  // Also possible old MSSM categorization (for testing)
  if(iCategory=="0") addNuisance          (iFileName,iChannel,iBkg,iEnergy,iName,iChannel+"_0jet_low"  ,iRebin,iVarBin,iFitModel,iFirst);
  if(iCategory=="1") addNuisance          (iFileName,iChannel,iBkg,iEnergy,iName,iChannel+"_0jet_high" ,iRebin,iVarBin,iFitModel,iFirst);
  if(iCategory=="2") addNuisance          (iFileName,iChannel,iBkg,iEnergy,iName,iChannel+"_boost_low" ,iRebin,iVarBin,iFitModel,iFirst);
  if(iCategory=="3") addNuisance          (iFileName,iChannel,iBkg,iEnergy,iName,iChannel+"_boost_high",iRebin,iVarBin,iFitModel,iFirst);
  if(iCategory=="6") addNuisance          (iFileName,iChannel,iBkg,iEnergy,iName,iChannel+"_btag_low"  ,iRebin,iVarBin,iFitModel,iFirst);
  if(iCategory=="7") addNuisance          (iFileName,iChannel,iBkg,iEnergy,iName,iChannel+"_btag_high" ,iRebin,iVarBin,iFitModel,iFirst);
  if(iCategory=="8") addNuisanceWithToys          (iFileName,iChannel,iBkg,iEnergy,iName,iChannel+"_nobtag"    ,iRebin,iVarBin,iFitModel,iFirst,iLast); // run toys with injected signal
  if(iCategory=="9") addNuisanceWithToys          (iFileName,iChannel,iBkg,iEnergy,iName,iChannel+"_btag"      ,iRebin,iVarBin,iFitModel,iFirst,iLast); // run toys with injected signal
}
