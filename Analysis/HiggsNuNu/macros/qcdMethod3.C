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
#include "TGraphAsymmErrors.h" 
#include "TStyle.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TFitResult.h"
#include "TLegend.h"
#include "TF1.h"
#include "TColor.h"
#include "TLatex.h"

int qcdMethod3() {//main
  

  std::vector<std::string> files;
  files.push_back("MC_TTJets");//0
  files.push_back("MC_T-tW");//1
  files.push_back("MC_Tbar-tW");
  files.push_back("MC_SingleT-s-powheg-tauola");
  files.push_back("MC_SingleTBar-s-powheg-tauola");
  files.push_back("MC_SingleT-t-powheg-tauola");
  files.push_back("MC_SingleTBar-t-powheg-tauola");
  files.push_back("MC_WW-pythia6-tauola");//7
  files.push_back("MC_WZ-pythia6-tauola");
  files.push_back("MC_ZZ-pythia6-tauola");
  files.push_back("MC_DYJJ01JetsToLL_M-50_MJJ-200");//10
  files.push_back("MC_DYJetsToLL");//11
  files.push_back("MC_DY1JetsToLL");
  files.push_back("MC_DY2JetsToLL");
  files.push_back("MC_DY3JetsToLL");
  files.push_back("MC_DY4JetsToLL");
  files.push_back("MC_ZJetsToNuNu_100_HT_200");//16
  files.push_back("MC_ZJetsToNuNu_200_HT_400");
  files.push_back("MC_ZJetsToNuNu_400_HT_inf");
  files.push_back("MC_ZJetsToNuNu_50_HT_100");
  files.push_back("MC_WJetsToLNu-v1_enu");//20
  files.push_back("MC_WJetsToLNu-v2_enu");
  files.push_back("MC_W1JetsToLNu_enu");
  files.push_back("MC_W2JetsToLNu_enu");
  files.push_back("MC_W3JetsToLNu_enu");
  files.push_back("MC_W4JetsToLNu_enu");
  files.push_back("MC_WJetsToLNu-v1_munu");//26
  files.push_back("MC_WJetsToLNu-v2_munu");
  files.push_back("MC_W1JetsToLNu_munu");
  files.push_back("MC_W2JetsToLNu_munu");
  files.push_back("MC_W3JetsToLNu_munu");
  files.push_back("MC_W4JetsToLNu_munu");
  files.push_back("MC_WJetsToLNu-v1_taunu");//32
  files.push_back("MC_WJetsToLNu-v2_taunu");
  files.push_back("MC_W1JetsToLNu_taunu");
  files.push_back("MC_W2JetsToLNu_taunu");
  files.push_back("MC_W3JetsToLNu_taunu");
  files.push_back("MC_W4JetsToLNu_taunu");

  const unsigned nSamples = 9;
  unsigned indices[nSamples+1] = {0,1,7,10,11,16,20,26,32,38};
  std::string samples[nSamples] = {"TTJets","single top","VV","EWK Z+2j","DYJets","Znunu","Wenu","Wmunu","Wtaunu"};

  const unsigned nFiles = files.size();

  TFile *fBkg[nFiles];

  double lLumi = 19619.;
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
  }

  std::vector<std::string> datafiles;
  datafiles.push_back("Data_MET-2012A-13Jul2012-v1");
  datafiles.push_back("Data_MET-2012A-06Aug2012-v1");
  datafiles.push_back("Data_MET-2012B-13Jul2012-v1");
  datafiles.push_back("Data_MET-2012C-24Aug2012-v1");
  datafiles.push_back("Data_MET-2012C-11Dec2012-v1"); 
  datafiles.push_back("Data_MET-2012C-PromptReco-v2");
  datafiles.push_back("Data_MET-2012D-PromptReco-v1");
  
  const unsigned nDataFiles = datafiles.size();
  TFile *fData[nDataFiles];


  std::string lHistName = "met";//,"n_jetsingap","Ht/Ht","Ht/SqrtHt"};

  const unsigned nSel = 4;
  std::string lSelection[nSel] = {"DPhiQCD_CJVfail","DPhiQCD_CJVpass","DPhiSIGNAL_CJVfail","DPhiSIGNAL_CJVpass"};
  std::string lSelectionName[nSel] = {"#Delta#phi_{jj}>2.6, fail CJV","#Delta#phi_{jj}>2.6, pass CJV","#Delta#phi_{jj}<1.0, fail CJV","#Delta#phi_{jj}<1.0, pass CJV"};
  
  std::ostringstream lPath;
  
  std::string folder = "output";

  TH1F *hMET[nSel];
  TH1F *hMETBKG[nSel];
  double nBkgLowMET[nSamples][nSel];
  double nBkgHighMET[nSamples][nSel];
  TH1F *hMETQCD[nSel];
  
  for (unsigned iS(0); iS<nSel; ++iS){//loop on selection

    for (unsigned iD(0); iD<nDataFiles; ++iD){//loop on data files
      lPath.str("");
      lPath << "../" << folder << "/nunu/MET0/" << datafiles[iD] << ".root";
      fData[iD] = TFile::Open(lPath.str().c_str());
      fData[iD]->cd(lSelection[iS].c_str());
      TH1F *lTmp = (TH1F*)gDirectory->Get(lHistName.c_str());
      if (iD==0) hMET[iS] = (TH1F*)lTmp->Clone();
      else hMET[iS]->Add(lTmp);
    }//loop on data files
  }
    
  std::cout << " -- Data files uploaded..." << std::endl;
  int bin130 = hMET[0]->FindBin(130)-1;
  std::cout << " Check: bin130 edges = " << hMET[0]->GetBinLowEdge(bin130) << " " << hMET[0]->GetXaxis()->GetBinUpEdge(bin130) << std::endl;
  int binMax = hMET[0]->GetNbinsX()+1;

  for (unsigned iS(0); iS<nSel; ++iS){//loop on selection
    //get histograms
    unsigned idx=0;
    std::cout << " -- Processing selection: " << lSelection[iS] << std::endl;

    for (unsigned iBkg(0); iBkg<nFiles; ++iBkg){//loop on bkg files
      lPath.str("");
      lPath << "../" << folder << "/nunu/MET0/" << files[iBkg] << ".root";
      fBkg[iBkg] = TFile::Open(lPath.str().c_str());
      fBkg[iBkg]->cd(lSelection[iS].c_str());
      TH1F *lTmp = (TH1F*)gDirectory->Get(lHistName.c_str());
      //met[iBkg][iS]->Sumw2();
      lTmp->Scale(lLumi*normalisation[iBkg]);
      if (iBkg==0) hMETBKG[iS] = (TH1F*)lTmp->Clone();
      else hMETBKG[iS]->Add(lTmp);

      if (iBkg == indices[idx]){
	nBkgLowMET[idx][iS] += lTmp->Integral(0,bin130);
	nBkgHighMET[idx][iS] += lTmp->Integral(bin130+1,binMax);
	idx++;
      }
      if (iBkg > indices[idx-1] && iBkg < indices[idx]){
	nBkgLowMET[idx-1][iS] += lTmp->Integral(0,bin130);
	nBkgHighMET[idx-1][iS] += lTmp->Integral(bin130+1,binMax);
      }
      
    }//loop on bkg files
    
    hMETQCD[iS] = (TH1F*)hMET[iS]->Clone();
    hMETQCD[iS]->Add(hMETBKG[iS],-1);
    
  }//loop on selection

  std::cout << " -- BKG files uploaded..." << std::endl;
  
  double lErr[3][nSel][2];
  double lNum[3][nSel][2];

  for (unsigned iS(0); iS<nSel; ++iS){//loop on selection
    lNum[0][iS][0] = hMET[iS]->IntegralAndError(0,bin130,lErr[0][iS][0]);
    lNum[0][iS][1] = hMET[iS]->IntegralAndError(bin130+1,hMET[iS]->GetNbinsX()+1,lErr[0][iS][1]);
    lNum[1][iS][0] = hMETBKG[iS]->IntegralAndError(0,bin130,lErr[1][iS][0]);
    lNum[1][iS][1] = hMETBKG[iS]->IntegralAndError(bin130+1,hMET[iS]->GetNbinsX()+1,lErr[1][iS][1]);
    lNum[2][iS][0] = hMETQCD[iS]->IntegralAndError(0,bin130,lErr[2][iS][0]);
    lNum[2][iS][1] = hMETQCD[iS]->IntegralAndError(bin130+1,hMET[iS]->GetNbinsX()+1,lErr[2][iS][1]);
  }

 
  std::cout << "$\\Delta\\phi_{jj}>2.6$" << std::endl;      
  std::string lPoint[3] = {"Data","MC Bkg","Estimated QCD"};
  std::string lABCD[4] = {"A","B","C","D"};

  for (unsigned iP(0); iP<3; ++iP){
    std::cout << lPoint[iP] << std::endl;
    for (unsigned iS(0); iS<2; ++iS){//loop on selection
      for (unsigned iR(0); iR<2; ++iR){//loop on region MET<130 - MET>130
	std::cout << lABCD[iR+2*iS] << " = " << lNum[iP][iS][iR] << " \\pm " << lErr[iP][iS][iR] << std::endl;
	if (iP==1){
	  for (unsigned iSample(0); iSample<nSamples; ++iSample){
	    std::cout << samples[iSample] << " :  " ;
	    if (iR==0) std::cout << nBkgLowMET[iSample][iS] <<std::endl;
	    else std::cout << nBkgHighMET[iSample][iS] << std::endl;
	  }
	}
      }
    }//loop on selection
  }

  std::cout << "$\\Delta\\phi_{jj}<1.0$" << std::endl;
  for (unsigned iP(0); iP<3; ++iP){
    std::cout << lPoint[iP] << std::endl;
    for (unsigned iS(2); iS<nSel; ++iS){//loop on selection
      for (unsigned iR(0); iR<2; ++iR){//loop on region MET<130 - MET>130
	std::cout << lABCD[iR+2*(iS-2)] << " = " << lNum[iP][iS][iR] << " \\pm " << lErr[iP][iS][iR] << std::endl;
	if (iP==1){
	  for (unsigned iSample(0); iSample<nSamples; ++iSample){
	    std::cout << samples[iSample] << " :  " ;
	    if (iR==0) std::cout << nBkgLowMET[iSample][iS] <<std::endl;
	    else std::cout << nBkgHighMET[iSample][iS] << std::endl;
	  }
	}
      }
    }//loop on selection
  }


  return 0;
  
}//main
