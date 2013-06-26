#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <iomanip>

#include "TMath.h"
#include "TFile.h"
#include "TH1F.h" 
#include "TGraphErrors.h" 
#include "TStyle.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "THStack.h"

int metQCDSasha(){//main

  //TFile * fData;

  std::vector<std::string> files;
  files.push_back("MC_TTJets");
  files.push_back("MC_T-tW");
  files.push_back("MC_Tbar-tW");
  files.push_back("MC_SingleT-s-powheg-tauola");
  files.push_back("MC_SingleTBar-s-powheg-tauola");
  files.push_back("MC_SingleT-t-powheg-tauola");
  files.push_back("MC_SingleTBar-t-powheg-tauola");
  files.push_back("MC_WW-pythia6-tauola");
  files.push_back("MC_WZ-pythia6-tauola");
  files.push_back("MC_ZZ-pythia6-tauola");
  files.push_back("MC_DYJJ01JetsToLL_M-50_MJJ-200");
  files.push_back("MC_DYJetsToLL");
  files.push_back("MC_DY1JetsToLL");
  files.push_back("MC_DY2JetsToLL");
  files.push_back("MC_DY3JetsToLL");
  files.push_back("MC_DY4JetsToLL");
  files.push_back("MC_ZJetsToNuNu_100_HT_200");
  files.push_back("MC_ZJetsToNuNu_200_HT_400");
  files.push_back("MC_ZJetsToNuNu_400_HT_inf");
  files.push_back("MC_ZJetsToNuNu_50_HT_100");
  files.push_back("MC_WJetsToLNu-v1_enu");
  files.push_back("MC_WJetsToLNu-v2_enu");
  files.push_back("MC_W1JetsToLNu_enu");
  files.push_back("MC_W2JetsToLNu_enu");
  files.push_back("MC_W3JetsToLNu_enu");
  files.push_back("MC_W4JetsToLNu_enu");
  files.push_back("MC_WJetsToLNu-v1_munu");
  files.push_back("MC_WJetsToLNu-v2_munu");
  files.push_back("MC_W1JetsToLNu_munu");
  files.push_back("MC_W2JetsToLNu_munu");
  files.push_back("MC_W3JetsToLNu_munu");
  files.push_back("MC_W4JetsToLNu_munu");
  files.push_back("MC_WJetsToLNu-v1_taunu");
  files.push_back("MC_WJetsToLNu-v2_taunu");
  files.push_back("MC_W1JetsToLNu_taunu");
  files.push_back("MC_W2JetsToLNu_taunu");
  files.push_back("MC_W3JetsToLNu_taunu");
  files.push_back("MC_W4JetsToLNu_taunu");

  const unsigned nSamples = 9;
  unsigned indices[nSamples+1] = {0,1,7,10,11,16,20,26,32,38};
  std::string samples[nSamples] = {"TTJets","single top","VV","EWK Z+2j","DYJets","Znunu","Wenu","Wmunu","Wtaunu"};

  std::vector<std::string> datafiles;
  datafiles.push_back("Data_MET-2012A-13Jul2012-v1");
  datafiles.push_back("Data_MET-2012A-06Aug2012-v1");
  datafiles.push_back("Data_MET-2012B-13Jul2012-v1");
  datafiles.push_back("Data_MET-2012C-24Aug2012-v1");
  datafiles.push_back("Data_MET-2012C-11Dec2012-v1"); 
  datafiles.push_back("Data_MET-2012C-PromptReco-v2");
  datafiles.push_back("Data_MET-2012D-PromptReco-v1");

  std::string folder = "output";
  std::string suffix = "_method2";

  const unsigned nFiles = files.size();
  const unsigned nDataFiles = datafiles.size();

  TFile *fBkg[nFiles];
  TFile *fData[nDataFiles];

  TFile *output = new TFile(("BkgForQCDEstimation"+suffix+".root").c_str(),"RECREATE");

  double lLumi = 19619.;
  
  int lColor[nSamples];
  for (unsigned iN(0); iN<nSamples;++iN){
    lColor[iN] = iN+1;
  }

  int lStyle = 1001;
  double normalisation[nFiles];
  normalisation[0] = 234./6923750.0;
  normalisation[1] = 11.1/497658.0;
  normalisation[2] = 11.1/493460.0;
  normalisation[3] = 3.79/259961.0;
  normalisation[4] = 1.76/139974.0;
  normalisation[5] = 56.4/3758227.0;
  normalisation[6] = 30.7/1935072.0;
  normalisation[7] = 54.838/9970431.0;
  normalisation[8] = 33.21/10000283.0;
  normalisation[9] = 17.654/9799908.0;
  normalisation[10] = (0.70/0.101*0.2)/510501.0;
  normalisation[11] = 3503.7/30459503.0;
  normalisation[12] = 3503.7/30459503.0;
  normalisation[13] = 3503.7/30459503.0;
  normalisation[14] = 3503.7/30459503.0;
  normalisation[15] = 3503.7/30459503.0;
  normalisation[16] = 381.2/4040980.0;
  normalisation[17] = 160.3/4416646.0;
  normalisation[18] = 41.49/5055885.0;
  normalisation[19] = 5.274/1006928.0;
  for (unsigned iN(20); iN<nFiles;++iN){
    normalisation[iN] = 37509/76102995.0;
    //lColor[iN] = 6;
  }

  TCanvas *myc = new TCanvas("myc","",1);
  gStyle->SetOptStat(0);

  const unsigned nSel = 4;

  TH1F *met[nFiles][nSel];
  TH1F *metRange[nSamples][nSel];
  TH1F *metRangeTotal[nSel];

  TH1F *datamet[nDataFiles][nSel];
  TH1F *datametRange[nDataFiles][nSel];
  TH1F *datametRangeTotal[nSel];

  THStack thstack0("stack0","stack0");
  THStack thstack1("stack1","stack1");
  THStack thstack2("stack2","stack2");
  THStack thstack3("stack3","stack3");

  unsigned n_added_to_stack[nSel] = {0,0,0,0};

  std::ostringstream lPath,lHistName;

  lHistName.str("");
  //lHistName << "met_pu_trig";
  lHistName << "met";

  //lPath.str("");
  //lPath << "../output/nunu/MET0/Data.root";
  //fData = TFile::Open(lPath.str().c_str());

  std::string lSelection[nSel] = {"DPhiSIGNAL-noCJV","DPhiQCD-noCJV","DPhiSIGNAL","DPhiQCD"};
  std::string loutname[nSel] = {"DPhiSIGNAL_noCJV","DPhiQCD_noCJV","DPhiSIGNAL","DPhiQCD"};
 
  //define the binning
  //const unsigned nBins = 10;
  //double xmin[nBins+1] = {0,10,20,30,40,50,60,70,80,100,120};//,met[0]->GetXaxis()->GetBinUpEdge(met[0]->GetNbinsX())};
  const unsigned nBins = 60;
  double xmin[nBins+1];


  double lNCdata_above120[2] = {0,0};
  double lNCdata_below120[2] = {0,0};
  double lNCbkg_above120[2] = {0,0};
  double lNCbkg_below120[2] = {0,0};
  double lNSdata_below130[2] = {0,0};
  double lNSbkg_below130[2] = {0,0};

  double lErrCdata_above120[2] = {0,0};
  double lErrCdata_below120[2] = {0,0};
  double lErrCbkg_above120[2] = {0,0};
  double lErrCbkg_below120[2] = {0,0};
  double lErrSdata_below130[2] = {0,0};
  double lErrSbkg_below130[2] = {0,0};

  unsigned bin120 = 0;
  unsigned bin130 = 0;

  for (unsigned iS(0); iS<nSel; ++iS){//loop on selection

    //fData->cd(lSelection[iS].c_str());
    //met[0][iS] = (TH1F*)gDirectory->Get(lHistName.str().c_str()); 
    //met[0][iS]->Sumw2();

    //get histograms
    for (unsigned iBkg(0); iBkg<nFiles; ++iBkg){//loop on bkg files
      lPath.str("");
      lPath << "../" << folder << "/nunu/MET0/" << files[iBkg] << ".root";
      fBkg[iBkg] = TFile::Open(lPath.str().c_str());
      //fBkg[iBkg]->cd((lSelection[iS]+"/weights/").c_str());
      fBkg[iBkg]->cd((lSelection[iS]+"/").c_str());
      met[iBkg][iS] = (TH1F*)gDirectory->Get(lHistName.str().c_str());
      met[iBkg][iS]->Sumw2();
      met[iBkg][iS]->Scale(lLumi*normalisation[iBkg]);

      if (iBkg == 10) std::cout << " - " << files[iBkg] << " = " << met[iBkg][iS]->GetEntries()  << " " << met[iBkg][iS]->Integral() << std::endl;

     }//loop on bkg files
    //metRangeTotal[iS] = new TH1F(("metRangeTotal_"+lSelection[iS]).c_str(),";MET range (GeV);Sum bkg (events)",nBins,xmin);
    metRangeTotal[iS] = new TH1F(("metRangeTotal_"+loutname[iS]).c_str(),";MET (GeV);Sum bkg (events/5 GeV)",nBins,0,300);

    for (unsigned idx(0); idx<nSamples; ++idx){//loop on bkg files
      std::ostringstream lName;
      lName.str("");
      lName << "metRange_" << iS << "_" << idx;
      metRange[idx][iS] = new TH1F(lName.str().c_str(),(";MET range (GeV);"+samples[idx]+" (events/5 GeV)").c_str(),nBins,0,300);
      //metRange[idx][iS]->SetLineColor(lColor[idx]);
      //metRange[idx][iS]->SetMarkerColor(lColor[idx]);
      //metRange[idx][iS]->SetFillStyle(lStyle);
    }


    for (unsigned iD(0); iD<nDataFiles; ++iD){//loop on data files
      lPath.str("");
      lPath << "../" << folder << "/nunu/MET0/" << datafiles[iD] << ".root";
      fData[iD] = TFile::Open(lPath.str().c_str());
      fData[iD]->cd(lSelection[iS].c_str());
      datamet[iD][iS] = (TH1F*)gDirectory->Get(lHistName.str().c_str());
      //datamet[iD][iS]->Sumw2();
      
      std::ostringstream lName;
      lName.str("");
      lName << "datametRange_" << iD ;
      datametRange[iD][iS] = new TH1F(lName.str().c_str(),";MET range (GeV);Data (events/5 GeV)",nBins,0,300);
      datametRange[iD][iS]->SetLineColor(lColor[iD]);
      datametRange[iD][iS]->SetMarkerColor(lColor[iD]);
      datametRange[iD][iS]->SetFillStyle(lStyle);
    }//loop on data files

    datametRangeTotal[iS] = new TH1F(("datametRangeTotal_"+lSelection[iS]).c_str(),";MET range (GeV);Data (events/5 GeV)",nBins,0,300);
 
    unsigned binMin = 0;
    unsigned binMax = met[0][iS]->GetNbinsX()+1;
  
    for (unsigned iB(0); iB<nBins+1; iB++){
      xmin[iB] = 5*iB;
      if (xmin[iB] == 120) bin120 = iB;
      if (xmin[iB] == 130) bin130 = iB;
    }
    for (unsigned iB(0); iB<nBins+1; iB++){//loop on bins
      for (int iX(0); iX<met[0][iS]->GetNbinsX()+1; ++iX){
	if (met[0][iS]->GetBinLowEdge(iX+1) >= xmin[iB]){
	  binMin = iX+1;
	  break;
	}
      }
      
      if (iB<nBins){
	for (int iX(binMin); iX<met[0][iS]->GetNbinsX()+1; ++iX){
	  if (met[0][iS]->GetBinLowEdge(iX+1) >= xmin[iB+1]) {
	    binMax = iX;
	    break;
	  }
	}
      }
      else binMax = met[0][iS]->GetNbinsX()+1;

      //if (iB==nBins-1) binMax = met[0][iS]->GetNbinsX();
      
      std::cout << " Bin " << iB 
		<< " min = " <<  met[0][iS]->GetBinLowEdge(binMin)
		<< " max = " <<  met[0][iS]->GetXaxis()->GetBinUpEdge(binMax)
		<< std::endl
		<< " bin120 up-edge = " << metRange[0][iS]->GetXaxis()->GetBinUpEdge(bin120)
		<< " bin130 up-edge = " << metRange[0][iS]->GetXaxis()->GetBinUpEdge(bin130)
		<< std::endl;
      

      double nBkg = 0;
      double nBkgErr = 0;
      unsigned idx=0;
      double lBkgErr = 0;
      double lBkg = 0;
     for (unsigned iBkg(0); iBkg<nFiles; ++iBkg){
	double lErr = 0;
	double lNum = met[iBkg][iS]->IntegralAndError(binMin,binMax,lErr);
	if (lNum != lNum) lNum=0;
	if (lErr != lErr) lErr=0;

	nBkg += lNum;
	nBkgErr += lErr*lErr;
	//fill new histos
	if (iBkg == indices[idx]){
	  lBkgErr = 0;
	  lBkg = 0;
	  lBkg += lNum;
	  lBkgErr += lErr*lErr;
	  idx++;
	}
	else if (iBkg > indices[idx-1] && iBkg < indices[idx]) {
	  lBkg += lNum;
	  lBkgErr += lErr*lErr;
	}
	//fill new histos
	if (iBkg == indices[idx]-1) {	
	  metRange[idx-1][iS]->SetBinContent(iB+1,lBkg);
	  metRange[idx-1][iS]->SetBinError(iB+1,sqrt(lBkgErr));
	  //std::cout << samples[idx-1] << " " <<  lBkg << " " << sqrt(lBkgErr) << std::endl;
	}

	//std::cout << " - " << files[iBkg] << " = " <<  lBkg << " +/- " << lBkgErr << std::endl;
      }

      std::cout << " ---- Total bkg = " << nBkg << " +/- " << sqrt(nBkgErr) << std::endl;

      metRangeTotal[iS]->SetBinContent(iB+1,nBkg);
      metRangeTotal[iS]->SetBinError(iB+1,sqrt(nBkgErr));

      double nData = 0;
      double nDataErr = 0;
      for (unsigned iData(0); iData<nDataFiles; ++iData){
	double lDataErr = 0;
	double lData = datamet[iData][iS]->IntegralAndError(binMin,binMax,lDataErr);
	nData += lData;
	nDataErr += lDataErr*lDataErr;
	//fill new histos
	datametRange[iData][iS]->SetBinContent(iB+1,lData);
	datametRange[iData][iS]->SetBinError(iB+1,lDataErr);
	//std::cout << " - " << datafiles[iData] << " = " <<  lData << " +/- " << lDataErr << std::endl;
      }
      
      std::cout << " ---- Total data = " << nData << " +/- " << sqrt(nDataErr) << std::endl;

      datametRangeTotal[iS]->SetBinContent(iB+1,nData);
      datametRangeTotal[iS]->SetBinError(iB+1,sqrt(nDataErr));


    }//loop on bins

    //fill new histos
    for (unsigned iBkg(0); iBkg<nSamples; ++iBkg){//loop on bkg files
      if (iS==0){
	thstack0.Add(metRange[iBkg][iS],"Esame");
	if (n_added_to_stack[iS] == 0) thstack0.SetHistogram((TH1F*)metRange[iBkg][iS]->Clone());
      } else if (iS==1) {
	thstack1.Add(metRange[iBkg][iS],"Esame");
	if (n_added_to_stack[iS] == 0) thstack1.SetHistogram((TH1F*)metRange[iBkg][iS]->Clone());
      }
      else if (iS==2){
	thstack2.Add(metRange[iBkg][iS],"Esame");
	if (n_added_to_stack[iS] == 0) thstack2.SetHistogram((TH1F*)metRange[iBkg][iS]->Clone());
      } else if (iS==3) {
	thstack3.Add(metRange[iBkg][iS],"Esame");
	if (n_added_to_stack[iS] == 0) thstack3.SetHistogram((TH1F*)metRange[iBkg][iS]->Clone());
      }
      ++n_added_to_stack[iS];
    }//loop on bkg files


    //TGraphErrors *grSumBkg = new TGraphErrors(nBins,xaxis,yaxis,xaxisErr,yaxisErr);
    myc->cd();
    //grSumBkg->GetXaxis()->SetTitle("MET range (GeV)");
    //grSumBkg->GetYaxis()->SetTitle("Sum bkg (events)");
    //if (iS==0) grSumBkg->SetTitle("#Delta#phi_{jj} < 1.0");
    //else grSumBkg->SetTitle("#Delta#phi_{jj} > 2.6");
    //grSumBkg->SetMinimum(0);
    //grSumBkg->SetMaximum(maxVal*1.1);

    if (iS%2==0) {
      //thstack0.
      metRangeTotal[iS]->SetTitle("#Delta#phi_{jj} < 1.0");
    }
    else {
      metRangeTotal[iS]->SetTitle("#Delta#phi_{jj} > 2.6");
    }
    metRangeTotal[iS]->Draw("PE");

 
    myc->Update();
    myc->Print(("bkgQCDSasha"+suffix+"_"+lSelection[iS]+".pdf").c_str());
 
    if (iS==0) {
      thstack0.SetTitle("#Delta#phi_{jj} < 1.0");
      thstack0.Draw();
      myc->Update();
      myc->Print(("bkgQCDSasha_stack"+suffix+"_"+lSelection[iS]+".pdf").c_str());
    } 
    else if (iS==1) {
      thstack1.SetTitle("#Delta#phi_{jj} > 2.6");
      thstack1.Draw();
      myc->Update();
      myc->Print(("bkgQCDSasha_stack"+suffix+"_"+lSelection[iS]+".pdf").c_str());
    }
    else if (iS==2) {
      thstack2.SetTitle("#Delta#phi_{jj} < 1.0");
      thstack2.Draw();
      myc->Update();
      myc->Print(("bkgQCDSasha_stack"+suffix+"_"+lSelection[iS]+".pdf").c_str());
    } 
    else if (iS==3) {
      thstack3.SetTitle("#Delta#phi_{jj} > 2.6");
      thstack3.Draw();
      myc->Update();
      myc->Print(("bkgQCDSasha_stack"+suffix+"_"+lSelection[iS]+".pdf").c_str());
    }

    output->Add(metRangeTotal[iS]);

    if (iS%2==0) {//signal
      lNSdata_below130[iS/2] = datametRangeTotal[iS]->IntegralAndError(0,bin130,lErrSdata_below130[iS/2]);
      lNSbkg_below130[iS/2] = metRangeTotal[iS]->IntegralAndError(0,bin130,lErrSbkg_below130[iS/2]);
    }
    else if (iS%2==1){//control
      lNCdata_below120[iS/2] = datametRangeTotal[iS]->IntegralAndError(0,bin120,lErrCdata_below120[iS/2]);
      lNCbkg_below120[iS/2] = metRangeTotal[iS]->IntegralAndError(0,bin120,lErrCbkg_below120[iS/2]);
      lNCdata_above120[iS/2] = datametRangeTotal[iS]->IntegralAndError(bin120+1,nBins+1,lErrCdata_above120[iS/2]);
      lNCbkg_above120[iS/2] = metRangeTotal[iS]->IntegralAndError(bin120+1,nBins+1,lErrCbkg_above120[iS/2]);
    }


  }//loop on selection

  std::cout << " ------------------------------------------" << std::endl
	    << " ------------------------------------------" << std::endl
	    << " ------------------------------------------" << std::endl
	    << " -- Method 2 results without CJV:" << std::endl;

  std::cout << " ------------------------------------------" << std::endl
	    << " -- Details of EWK BKG in signal region: " << std::endl
	    << " ------------------------------------------" << std::endl;

  for (unsigned idx(0); idx<nSamples; ++idx){//loop on bkg files
    double err = 0;
    double val = metRange[idx][0]->IntegralAndError(0,bin130,err);
    std::cout << samples[idx] << " " << val << " \\pm " << err << std::endl;
  }
 std::cout << " -----------------------------------------------------" << std::endl
	    << " -- Details of EWK BKG in control region : " << std::endl
	    << " ----------------------------------------------------" << std::endl;

  for (unsigned idx(0); idx<nSamples; ++idx){//loop on bkg files
    double err = 0;
    double val = metRange[idx][1]->IntegralAndError(bin120+1,nBins+1,err);
    std::cout << samples[idx] << " MET>120: " << val << " \\pm " << err ;
    err = 0;
    val = metRange[idx][1]->IntegralAndError(0,bin120,err);
    std::cout << ", MET<120: " << val << " \\pm " << err<< std::endl;
  }

  double lqcdS = lNSdata_below130[0]-lNSbkg_below130[0];
  double lqcdSErr = sqrt(pow(lErrSdata_below130[0],2)+pow(lErrSbkg_below130[0],2));

  std::cout << " - $n_{Data}^S (MET<130) = " << lNSdata_below130[0] << " \\pm " << lErrSdata_below130[0] << " $" << std::endl
	    << " - $n_{Bkg}^S (MET<130) = " << lNSbkg_below130[0] << " \\pm " << lErrSbkg_below130[0] << " $" << std::endl
	    << " - $n_{QCD}^S (MET<130) = " << lqcdS << " \\pm " << lqcdSErr << " $" << std::endl;

  double lqcdCb = lNCdata_below120[0]-lNCbkg_below120[0];
  double lqcdCbErr = sqrt(pow(lErrCdata_below120[0],2)+pow(lErrCbkg_below120[0],2));
  std::cout << " - $n_{Data}^C (MET<120) = " << lNCdata_below120[0] << " \\pm " << lErrCdata_below120[0] << " $" << std::endl
	    << " - $n_{Bkg}^C (MET<120) = " << lNCbkg_below120[0] << " \\pm " << lErrCbkg_below120[0] << " $" << std::endl
	    << " - $n_{QCD}^C (MET<120) = " << lqcdCb << " \\pm " << lqcdCbErr << " $" << std::endl;

  double lqcdCa = lNCdata_above120[0]-lNCbkg_above120[0];
  double lqcdCaErr = sqrt(pow(lErrCdata_above120[0],2)+pow(lErrCbkg_above120[0],2));
  std::cout << " - $n_{Data}^C (MET>120) = " << lNCdata_above120[0] << " \\pm " << lErrCdata_above120[0] << " $" << std::endl
	    << " - $n_{Bkg}^C (MET>120) = " << lNCbkg_above120[0] << " \\pm " << lErrCbkg_above120[0] << " $" << std::endl
	    << " - $n_{QCD}^C (MET>120) = " << lqcdCa << " \\pm " << lqcdCaErr << " $" << std::endl;

  double result = lqcdS*lqcdCa/lqcdCb;
  double error = result * sqrt(pow(lqcdSErr/lqcdS,2)+pow(lqcdCaErr/lqcdCa,2)+pow(lqcdCbErr/lqcdCb,2));
  std::cout << " - Final estimate = $" << result << " \\pm " << error << " $" << std::endl;

  std::cout << " ------------------------------------------" << std::endl
	    << " ------------------------------------------" << std::endl
	    << " ------------------------------------------" << std::endl
	    << " -- Method 2 results with CJV:" << std::endl;

  std::cout << " ------------------------------------------" << std::endl
	    << " -- Details of EWK BKG in signal region: " << std::endl
	    << " ------------------------------------------" << std::endl;

  for (unsigned idx(0); idx<nSamples; ++idx){//loop on bkg files
    double err = 0;
    double val = metRange[idx][2]->IntegralAndError(0,bin130,err);
    std::cout << samples[idx] << " " << val << " \\pm " << err << std::endl;
  }

 std::cout << " -----------------------------------------------------" << std::endl
	    << " -- Details of EWK BKG in control region : " << std::endl
	    << " ----------------------------------------------------" << std::endl;

  for (unsigned idx(0); idx<nSamples; ++idx){//loop on bkg files
    double err = 0;
    double val = metRange[idx][3]->IntegralAndError(bin120+1,nBins+1,err);
    std::cout << samples[idx] << " MET>120: " << val << " \\pm " << err ;
    err = 0;
    val = metRange[idx][3]->IntegralAndError(0,bin120,err);
    std::cout << ", MET<120: " << val << " \\pm " << err<< std::endl;
  }

  lqcdS = lNSdata_below130[1]-lNSbkg_below130[1];
  lqcdSErr = sqrt(pow(lErrSdata_below130[1],2)+pow(lErrSbkg_below130[1],2));

  std::cout << " - $n_{Data}^S (MET<130) = " << lNSdata_below130[1] << " \\pm " << lErrSdata_below130[1] << " $" << std::endl
	    << " - $n_{Bkg}^S (MET<130) = " << lNSbkg_below130[1] << " \\pm " << lErrSbkg_below130[1] << " $" << std::endl
	    << " - $n_{QCD}^S (MET<130) = " << lqcdS << " \\pm " << lqcdSErr << " $" << std::endl;

  lqcdCb = lNCdata_below120[1]-lNCbkg_below120[1];
  lqcdCbErr = sqrt(pow(lErrCdata_below120[1],2)+pow(lErrCbkg_below120[1],2));
  std::cout << " - $n_{Data}^C (MET<120) = " << lNCdata_below120[1] << " \\pm " << lErrCdata_below120[1] << " $" << std::endl
	    << " - $n_{Bkg}^C (MET<120) = " << lNCbkg_below120[1] << " \\pm " << lErrCbkg_below120[1] << " $" << std::endl
	    << " - $n_{QCD}^C (MET<120) = " << lqcdCb << " \\pm " << lqcdCbErr << " $" << std::endl;

  lqcdCa = lNCdata_above120[1]-lNCbkg_above120[1];
  lqcdCaErr = sqrt(pow(lErrCdata_above120[1],2)+pow(lErrCbkg_above120[1],2));
  std::cout << " - $n_{Data}^C (MET>120) = " << lNCdata_above120[1] << " \\pm " << lErrCdata_above120[1] << " $" << std::endl
	    << " - $n_{Bkg}^C (MET>120) = " << lNCbkg_above120[1] << " \\pm " << lErrCbkg_above120[1] << " $" << std::endl
	    << " - $n_{QCD}^C (MET>120) = " << lqcdCa << " \\pm " << lqcdCaErr << " $" << std::endl;

  result = lqcdS*lqcdCa/lqcdCb;
  error = result * sqrt(pow(lqcdSErr/lqcdS,2)+pow(lqcdCaErr/lqcdCa,2)+pow(lqcdCbErr/lqcdCb,2));
  std::cout << " - Final estimate = $" << result << " \\pm " << error << " $" << std::endl;

  output->Write();
  output->Close();

  return 0;

}//main
