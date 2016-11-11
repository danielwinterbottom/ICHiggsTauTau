#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include <THStack.h>
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


double quadrature( const std::vector< double > & input )
{
  double output(0.0);
  for( auto const & item : input )
    output += item * item;
  return sqrt(output);
}

double Error(TH1F const* hist) {
  double err = 0.0;
  if (hist) {
    //hist->Sumw2();
    hist->IntegralAndError(0, hist->GetNbinsX()+1, err);
    if (err<0 || err != err) {
      std::cout << " -- Warning: error on integral is " << err << ". Removing overflows." << std::endl;
      //hist->IntegralAndError(1, hist->GetNbinsX(), err);
      if (err<0 || err != err) {
        std::cout << " -- Warning: error on integral is " << err << ". Setting to 0." << std::endl;
        //err=0;
      }
    }
  }
  return err;
}


int JESValidation(){//main

  // definitions 
  std::string munu_file         = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/munu.root";
  std::string munu_JESUP_file   = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/JESUP/munu.root";
  std::string munu_JESDOWN_file = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/JESDOWN/munu.root";
  
  std::string nunu_file         = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/nunu.root";
  std::string nunu_JESUP_file   = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/JESUP/nunu.root";
  std::string nunu_JESDOWN_file = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/JESDOWN/nunu.root";
  
  const unsigned nR = 13;
  
  std::string variables[nR] = {"mu1_pt",
                               "mu1_eta",
                               "n_vertices",
                               "jet1_pt",
                               "jet2_pt",
                               "jet1_eta",
                               "jet2_eta",
                               "metnomuons",
                               "dijet_M",
                               "dijet_deta",
                               "alljetsmetnomu_mindphi",
                               "central_tag_eta",
                               "forward_tag_eta"
  };

  // open file
  TFile *munu_Tfile, *munu_JESUP_Tfile, *munu_JESDOWN_Tfile;
  munu_Tfile         = TFile::Open(munu_file.c_str());
  munu_JESUP_Tfile   = TFile::Open(munu_JESUP_file.c_str());
  munu_JESDOWN_Tfile = TFile::Open(munu_JESDOWN_file.c_str());
  if (!munu_Tfile){
    std::cout << " Input file " << munu_file << " not found." << std::endl;
    return 1;
  }
  if (!munu_JESUP_Tfile){
    std::cout << " Input file " << munu_JESUP_file << " not found." << std::endl;
    return 1;
  }
  if (!munu_JESDOWN_Tfile){
    std::cout << " Input file " << munu_JESDOWN_file << " not found." << std::endl;
    return 1;
  }

  TFile *nunu_Tfile, *nunu_JESUP_Tfile, *nunu_JESDOWN_Tfile;
  nunu_Tfile         = TFile::Open(nunu_file.c_str());
  nunu_JESUP_Tfile   = TFile::Open(nunu_JESUP_file.c_str());
  nunu_JESDOWN_Tfile = TFile::Open(nunu_JESDOWN_file.c_str());
  if (!nunu_Tfile){
    std::cout << " Input file " << nunu_file << " not found." << std::endl;
    return 1;
  }
  if (!nunu_JESUP_Tfile){
    std::cout << " Input file " << nunu_JESUP_file << " not found." << std::endl;
    return 1;
  }
  if (!nunu_JESDOWN_Tfile){
    std::cout << " Input file " << nunu_JESDOWN_file << " not found." << std::endl;
    return 1;
  }

  THStack * st[nR];
  THStack * stRatio[nR];
  TCanvas *mycanvas[nR];
  TPad *pad1[nR];
  TPad *pad2[nR];
  for(unsigned i=0; i<nR; ++i){

    mycanvas[i] = new TCanvas(variables[i].c_str(),variables[i].c_str(),200,10,700,500);
    pad1[i] = new TPad("pad1","", 0,0.3,1,1  );
    pad2[i] = new TPad("pad2","", 0,0  ,1,0.3);
    mycanvas[i]->cd();
    
    pad1[i]->Draw();
    pad2[i]->Draw();
    pad1[i]->SetBottomMargin(0);
    pad2[i]->SetTopMargin(0);


    TH1F * munuQCD_hist         = (TH1F*)munu_Tfile->Get( Form("wmuqcd/%s", variables[i].c_str()) );
    TH1F * munuQCD_JESUP_hist   = (TH1F*)munu_JESUP_Tfile->Get( Form("wmuqcd/%s", variables[i].c_str()) );
    TH1F * munuQCD_JESDOWN_hist = (TH1F*)munu_JESDOWN_Tfile->Get( Form("wmuqcd/%s", variables[i].c_str()) );

    TH1F * munuQCD_JESUP_ratio_hist   = (TH1F*)munuQCD_JESUP_hist->Clone();
    TH1F * munuQCD_JESDOWN_ratio_hist = (TH1F*)munuQCD_JESDOWN_hist->Clone();

    munuQCD_JESUP_ratio_hist->Divide(munuQCD_hist);
    munuQCD_JESUP_ratio_hist->SetLineColor(kBlue);
    munuQCD_JESDOWN_ratio_hist->Divide(munuQCD_hist);
    munuQCD_JESDOWN_ratio_hist->SetLineColor(kRed);

    TH1F * munuEWK_hist         = (TH1F*)munu_Tfile->Get( Form("wmuewk/%s", variables[i].c_str()) );
    TH1F * munuEWK_JESUP_hist   = (TH1F*)munu_JESUP_Tfile->Get( Form("wmuewk/%s", variables[i].c_str()) );
    TH1F * munuEWK_JESDOWN_hist = (TH1F*)munu_JESDOWN_Tfile->Get( Form("wmuewk/%s", variables[i].c_str()) );

    TH1F * munuEWK_JESUP_ratio_hist   = (TH1F*)munuEWK_JESUP_hist->Clone();
    TH1F * munuEWK_JESDOWN_ratio_hist = (TH1F*)munuEWK_JESDOWN_hist->Clone();

    munuEWK_JESUP_ratio_hist->Divide(munuEWK_hist);
    munuEWK_JESUP_ratio_hist->SetLineColor(kBlue);
    munuEWK_JESDOWN_ratio_hist->Divide(munuEWK_hist);
    munuEWK_JESDOWN_ratio_hist->SetLineColor(kRed);
    
    
    TH1F * nunuQCD_hist         = (TH1F*)nunu_Tfile->Get( Form("wmuqcd/%s", variables[i].c_str()) );
    TH1F * nunuQCD_JESUP_hist   = (TH1F*)nunu_JESUP_Tfile->Get( Form("wmuqcd/%s", variables[i].c_str()) );
    TH1F * nunuQCD_JESDOWN_hist = (TH1F*)nunu_JESDOWN_Tfile->Get( Form("wmuqcd/%s", variables[i].c_str()) );
    
    //TH1F * nunuQCD_JESUP_ratio_hist   = (TH1F*)nunuQCD_JESUP_hist->Clone();
    //TH1F * nunuQCD_JESDOWN_ratio_hist = (TH1F*)nunuQCD_JESDOWN_hist->Clone();
    
    //nunuQCD_JESUP_ratio_hist->Divide(nunuQCD_hist);
    //nunuQCD_JESUP_ratio_hist->SetLineColor(kBlue);
    //nunuQCD_JESDOWN_ratio_hist->Divide(nunuQCD_hist);
    //nunuQCD_JESDOWN_ratio_hist->SetLineColor(kRed);
    
    TH1F * nunuEWK_hist         = (TH1F*)nunu_Tfile->Get( Form("wmuewk/%s", variables[i].c_str()) );
    TH1F * nunuEWK_JESUP_hist   = (TH1F*)nunu_JESUP_Tfile->Get( Form("wmuewk/%s", variables[i].c_str()) );
    TH1F * nunuEWK_JESDOWN_hist = (TH1F*)nunu_JESDOWN_Tfile->Get( Form("wmuewk/%s", variables[i].c_str()) );
    
    //TH1F * nunuEWK_JESUP_ratio_hist   = (TH1F*)nunuEWK_JESUP_hist->Clone();
    //TH1F * nunuEWK_JESDOWN_ratio_hist = (TH1F*)nunuEWK_JESDOWN_hist->Clone();
    
    //nunuEWK_JESUP_ratio_hist->Divide(nunuEWK_hist);
    //nunuEWK_JESUP_ratio_hist->SetLineColor(kBlue);
    //nunuEWK_JESDOWN_ratio_hist->Divide(nunuEWK_hist);
    //nunuEWK_JESDOWN_ratio_hist->SetLineColor(kRed);
    
    
    
    
    
    
    
    TH1F * munuDATAOBS_hist         = (TH1F*)munu_Tfile->Get( Form("data_obs/%s", variables[i].c_str()) );
    TH1F * munuDATAOBS_JESUP_hist   = (TH1F*)munu_JESUP_Tfile->Get( Form("data_obs/%s", variables[i].c_str()) );
    TH1F * munuDATAOBS_JESDOWN_hist = (TH1F*)munu_JESDOWN_Tfile->Get( Form("data_obs/%s", variables[i].c_str()) );
    
    TH1F * munuDATAOBS_JESUP_ratio_hist   = (TH1F*)munuDATAOBS_JESUP_hist->Clone();
    TH1F * munuDATAOBS_JESDOWN_ratio_hist = (TH1F*)munuDATAOBS_JESDOWN_hist->Clone();
    
    munuDATAOBS_JESUP_ratio_hist->Divide(munuDATAOBS_hist);
    munuDATAOBS_JESUP_ratio_hist->SetLineColor(kBlue);
    munuDATAOBS_JESDOWN_ratio_hist->Divide(munuDATAOBS_hist);
    munuDATAOBS_JESDOWN_ratio_hist->SetLineColor(kRed);
    
    TH1F * munuVV_hist         = (TH1F*)munu_Tfile->Get( Form("vv/%s", variables[i].c_str()) );
    TH1F * munuVV_JESUP_hist   = (TH1F*)munu_JESUP_Tfile->Get( Form("vv/%s", variables[i].c_str()) );
    TH1F * munuVV_JESDOWN_hist = (TH1F*)munu_JESDOWN_Tfile->Get( Form("vv/%s", variables[i].c_str()) );
    
    TH1F * munuVV_JESUP_ratio_hist   = (TH1F*)munuVV_JESUP_hist->Clone();
    TH1F * munuVV_JESDOWN_ratio_hist = (TH1F*)munuVV_JESDOWN_hist->Clone();
    
    munuVV_JESUP_ratio_hist->Divide(munuVV_hist);
    munuVV_JESUP_ratio_hist->SetLineColor(kBlue);
    munuVV_JESDOWN_ratio_hist->Divide(munuVV_hist);
    munuVV_JESDOWN_ratio_hist->SetLineColor(kRed);
    
    TH1F * munuTOP_hist         = (TH1F*)munu_Tfile->Get( Form("top/%s", variables[i].c_str()) );
    TH1F * munuTOP_JESUP_hist   = (TH1F*)munu_JESUP_Tfile->Get( Form("top/%s", variables[i].c_str()) );
    TH1F * munuTOP_JESDOWN_hist = (TH1F*)munu_JESDOWN_Tfile->Get( Form("top/%s", variables[i].c_str()) );
    
    TH1F * munuTOP_JESUP_ratio_hist   = (TH1F*)munuTOP_JESUP_hist->Clone();
    TH1F * munuTOP_JESDOWN_ratio_hist = (TH1F*)munuTOP_JESDOWN_hist->Clone();
    
    munuTOP_JESUP_ratio_hist->Divide(munuTOP_hist);
    munuTOP_JESUP_ratio_hist->SetLineColor(kBlue);
    munuTOP_JESDOWN_ratio_hist->Divide(munuTOP_hist);
    munuTOP_JESDOWN_ratio_hist->SetLineColor(kRed);
    
    TH1F * munuQCDmultijet_hist         = (TH1F*)munu_Tfile->Get( Form("qcd/%s", variables[i].c_str()) );
    TH1F * munuQCDmultijet_JESUP_hist   = (TH1F*)munu_JESUP_Tfile->Get( Form("qcd/%s", variables[i].c_str()) );
    TH1F * munuQCDmultijet_JESDOWN_hist = (TH1F*)munu_JESDOWN_Tfile->Get( Form("qcd/%s", variables[i].c_str()) );
    
    TH1F * munuQCDmultijet_JESUP_ratio_hist   = (TH1F*)munuQCDmultijet_JESUP_hist->Clone();
    TH1F * munuQCDmultijet_JESDOWN_ratio_hist = (TH1F*)munuQCDmultijet_JESDOWN_hist->Clone();
    
    munuQCDmultijet_JESUP_ratio_hist->Divide(munuQCDmultijet_hist);
    munuQCDmultijet_JESUP_ratio_hist->SetLineColor(kBlue);
    munuQCDmultijet_JESDOWN_ratio_hist->Divide(munuQCDmultijet_hist);
    munuQCDmultijet_JESDOWN_ratio_hist->SetLineColor(kRed);
    

    if (i==4) {
      
      std::cout << " -- Making table: " << std::endl;
      
      std::cout << "   &  Central  &  JESUP  &  JESDOWN\\\\ " <<std::endl;
      
      std::vector<double> error_munu_hist;
      error_munu_hist.push_back(Error(munuQCD_hist));
      error_munu_hist.push_back(Error(munuEWK_hist));
      std::vector<double> error_munu_JESUP_hist;
      error_munu_JESUP_hist.push_back(Error(munuQCD_JESUP_hist));
      error_munu_JESUP_hist.push_back(Error(munuEWK_JESUP_hist));
      std::vector<double> error_munu_JESDOWN_hist;
      error_munu_JESDOWN_hist.push_back(Error(munuQCD_JESDOWN_hist));
      error_munu_JESDOWN_hist.push_back(Error(munuEWK_JESDOWN_hist));
      std::cout << " N$_\\text{MC}^\\text{CR}$  &  " 
                << munuQCD_hist->Integral(0, munuQCD_hist->GetNbinsX() + 1) + munuEWK_hist->Integral(0, munuEWK_hist->GetNbinsX() + 1) 
                << " \\pm " << quadrature( error_munu_hist ) << " & "
                << munuQCD_JESUP_hist->Integral(0, munuQCD_JESUP_hist->GetNbinsX() + 1) + munuEWK_JESUP_hist->Integral(0, munuEWK_JESUP_hist->GetNbinsX() + 1) 
                << " \\pm " << quadrature( error_munu_JESUP_hist ) << " & "
                << munuQCD_JESDOWN_hist->Integral(0, munuQCD_JESDOWN_hist->GetNbinsX() + 1) + munuEWK_JESDOWN_hist->Integral(0, munuEWK_JESDOWN_hist->GetNbinsX() + 1) 
                << " \\pm " << quadrature( error_munu_JESDOWN_hist ) 
                << "\\\\ " << std::endl;
//       std::cout << " \\sigma_N$_\\text{MC}^\\text{CR}$  &  " 
//                 << quadrature( error_munu_hist )
//                 << " & "
//                 << quadrature( error_munu_JESUP_hist )
//                 << " & "
//                 << quadrature( error_munu_JESDOWN_hist )
//                 << "\\\\ " << std::endl;
      
      
      std::vector<double> error_nunu_hist;
      error_nunu_hist.push_back(Error(nunuQCD_hist));
      error_nunu_hist.push_back(Error(nunuEWK_hist));
      std::vector<double> error_nunu_JESUP_hist;
      error_nunu_JESUP_hist.push_back(Error(nunuQCD_JESUP_hist));
      error_nunu_JESUP_hist.push_back(Error(nunuEWK_JESUP_hist));
      std::vector<double> error_nunu_JESDOWN_hist;
      error_nunu_JESDOWN_hist.push_back(Error(nunuQCD_JESDOWN_hist));
      error_nunu_JESDOWN_hist.push_back(Error(nunuEWK_JESDOWN_hist));
      std::cout << " N$_\\text{MC}^\\text{SR}$  &  " 
                << nunuQCD_hist->Integral(0, nunuQCD_hist->GetNbinsX() + 1) + nunuEWK_hist->Integral(0, nunuEWK_hist->GetNbinsX() + 1) 
                << " \\pm " << quadrature( error_nunu_hist ) << " & "
                << nunuQCD_JESUP_hist->Integral(0, nunuQCD_JESUP_hist->GetNbinsX() + 1) + nunuEWK_JESUP_hist->Integral(0, nunuEWK_JESUP_hist->GetNbinsX() + 1) 
                << " \\pm " << quadrature( error_nunu_JESUP_hist ) << " & "
                << nunuQCD_JESDOWN_hist->Integral(0, nunuQCD_JESDOWN_hist->GetNbinsX() + 1) + nunuEWK_JESDOWN_hist->Integral(0, nunuEWK_JESDOWN_hist->GetNbinsX() + 1) 
                << " \\pm " << quadrature( error_nunu_JESDOWN_hist ) 
                << "\\\\ " << std::endl;
//       std::cout << " \\sigma_N$_\\text{MC}^\\text{SR}$  &  " 
//                 << quadrature( error_nunu_hist )
//                 << " & "
//                 << quadrature( error_nunu_JESUP_hist )
//                 << " & "
//                 << quadrature( error_nunu_JESDOWN_hist )
//                 << "\\\\ " << std::endl;
                
                
      std::cout << " N$_\\text{data_obs}^\\text{CR}$  &  " 
                << munuDATAOBS_hist->Integral(0, munuDATAOBS_hist->GetNbinsX() + 1) 
                << " \\pm " << Error(munuDATAOBS_hist) << " & "
                << munuDATAOBS_JESUP_hist->Integral(0, munuDATAOBS_JESUP_hist->GetNbinsX() + 1) 
                << " \\pm " << Error(munuDATAOBS_JESUP_hist) << " & "
                << munuDATAOBS_JESDOWN_hist->Integral(0, munuDATAOBS_JESDOWN_hist->GetNbinsX() + 1) 
                << " \\pm " << Error(munuDATAOBS_JESDOWN_hist)
                << "\\\\ " << std::endl;
//       std::cout << " \\sigma_N$_\\text{data_obs}^\\text{CR}$  &  " 
//                 << Error(munuDATAOBS_hist)
//                 << " & "
//                 << Error(munuDATAOBS_JESUP_hist)
//                 << " & "
//                 << Error(munuDATAOBS_JESDOWN_hist)
//                 << "\\\\ " << std::endl;
      
                
      std::vector<double> errorBKG_munu_hist;
      errorBKG_munu_hist.push_back(Error(munuTOP_hist));
      errorBKG_munu_hist.push_back(Error(munuVV_hist));
      errorBKG_munu_hist.push_back(Error(munuQCDmultijet_hist));
      std::vector<double> errorBKG_munu_JESUP_hist;
      errorBKG_munu_JESUP_hist.push_back(Error(munuTOP_JESUP_hist));
      errorBKG_munu_JESUP_hist.push_back(Error(munuVV_JESUP_hist));
      errorBKG_munu_JESUP_hist.push_back(Error(munuQCDmultijet_JESUP_hist));
      std::vector<double> errorBKG_munu_JESDOWN_hist;
      errorBKG_munu_JESDOWN_hist.push_back(Error(munuTOP_JESDOWN_hist));
      errorBKG_munu_JESDOWN_hist.push_back(Error(munuVV_JESDOWN_hist));
      errorBKG_munu_JESDOWN_hist.push_back(Error(munuQCDmultijet_JESDOWN_hist));
      std::cout << " N$_\\text{backgrounds}^\\text{CR}$  &  " 
                << munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) + munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) + munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) 
                << " \\pm " << quadrature( errorBKG_munu_hist ) << " & "
                << munuTOP_JESUP_hist->Integral(0, munuTOP_JESUP_hist->GetNbinsX() + 1) + munuVV_JESUP_hist->Integral(0, munuVV_JESUP_hist->GetNbinsX() + 1) + munuQCDmultijet_JESUP_hist->Integral(0, munuQCDmultijet_JESUP_hist->GetNbinsX() + 1) 
                << " \\pm " << quadrature( errorBKG_munu_JESUP_hist ) << " & "
                << munuTOP_JESDOWN_hist->Integral(0, munuTOP_JESDOWN_hist->GetNbinsX() + 1) + munuVV_JESDOWN_hist->Integral(0, munuVV_JESDOWN_hist->GetNbinsX() + 1) + munuQCDmultijet_JESDOWN_hist->Integral(0, munuQCDmultijet_JESDOWN_hist->GetNbinsX() + 1) 
                << " \\pm " << quadrature( errorBKG_munu_JESDOWN_hist ) 
                << "\\\\ " << std::endl;
//       std::cout << " \\sigma_N$_\\text{backgrounds}^\\text{CR}$  &  " 
//                 << quadrature( errorBKG_munu_hist )
//                 << " & "
//                 << quadrature( errorBKG_munu_JESUP_hist )
//                 << " & "
//                 << quadrature( errorBKG_munu_JESDOWN_hist )
//                 << "\\\\ " << std::endl;
      std::cout << " -- TOP Integral contribution                    " << munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) << std::endl;
      std::cout << " -- Top GetEntries contribution                  " << munuTOP_hist->GetEntries() << std::endl;
      std::cout << " -- TOP_JESUP Integral contribution              " << munuTOP_JESUP_hist->Integral(0, munuTOP_JESUP_hist->GetNbinsX() + 1) << std::endl;
      std::cout << " -- Top_JESUP GetEntries contribution            " << munuTOP_JESUP_hist->GetEntries() << std::endl;
      std::cout << " -- TOP_JESDOWN Integral contribution            " << munuTOP_JESDOWN_hist->Integral(0, munuTOP_JESDOWN_hist->GetNbinsX() + 1) << std::endl;
      std::cout << " -- Top_JESDOWN GetEntries contribution          " << munuTOP_JESDOWN_hist->GetEntries() << std::endl;
      std::cout << " -- VV Integral contribution                     " << munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) << std::endl;          
      std::cout << " -- vv GetEntries contribution                   " << munuVV_hist->GetEntries() << std::endl;
      std::cout << " -- VV_JESUP Integral contribution               " << munuVV_JESUP_hist->Integral(0, munuVV_JESUP_hist->GetNbinsX() + 1) << std::endl;
      std::cout << " -- vv_JESUP GetEntries contribution             " << munuVV_JESUP_hist->GetEntries() << std::endl;
      std::cout << " -- VV_JESDOWN Integral contribution             " << munuVV_JESDOWN_hist->Integral(0, munuVV_JESDOWN_hist->GetNbinsX() + 1) << std::endl;          
      std::cout << " -- vv_JESDOWN GetEntries contribution           " << munuVV_JESDOWN_hist->GetEntries() << std::endl;          
      std::cout << " -- QCDmultijet Integral contribution            " << munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) << std::endl;          
      std::cout << " -- qcdmultijet GetEntries contribution          " << munuQCDmultijet_hist->GetEntries() << std::endl;
      std::cout << " -- QCDmultijet_JESUP Integral contribution      " << munuQCDmultijet_JESUP_hist->Integral(0, munuQCDmultijet_JESUP_hist->GetNbinsX() + 1) << std::endl;
      std::cout << " -- qcdmultijet_JESUP GetEntries contribution    " << munuQCDmultijet_JESUP_hist->GetEntries() << std::endl;          
      std::cout << " -- QCDmultijet_JESDOWN Integral contribution    " << munuQCDmultijet_JESDOWN_hist->Integral(0, munuQCDmultijet_JESDOWN_hist->GetNbinsX() + 1) << std::endl;             
      std::cout << " -- qcdmultijet_JESDOWN GetEntries contribution  " << munuQCDmultijet_JESDOWN_hist->GetEntries() << std::endl;          

    }
    

    //histDOWN->SetTitle(variables[i].c_str());
    munuQCD_hist->SetFillStyle(3003);
    munuQCD_hist->SetFillColor(kOrange);
    munuQCD_hist->SetLineColor(kOrange);

    munuQCD_JESUP_hist->SetFillStyle(3003);
    munuQCD_JESUP_hist->SetFillColor(kBlue);
    munuQCD_JESUP_hist->SetLineColor(kBlue);

    munuQCD_JESDOWN_hist->SetFillStyle(3003);
    munuQCD_JESDOWN_hist->SetFillColor(kRed);
    munuQCD_JESDOWN_hist->SetLineColor(kRed);


    st[i] = new THStack(variables[i].c_str(),variables[i].c_str());
    st[i]->Add(munuQCD_JESUP_hist, "hist");
    st[i]->Add(munuQCD_hist, "hist");
    st[i]->Add(munuQCD_JESDOWN_hist, "hist");
    pad1[i]->cd();
    st[i]->Draw("nostack");
    double upperScale = 1.0/0.7;
    st[i]->GetXaxis()->SetLabelSize(
      st[i]->GetXaxis()->GetLabelSize() * upperScale
    );
    st[i]->GetXaxis()->SetTitleSize(
      st[i]->GetXaxis()->GetTitleSize() * upperScale
    );
    st[i]->GetYaxis()->SetLabelSize(
      st[i]->GetYaxis()->GetLabelSize() * upperScale
    );
    st[i]->GetYaxis()->SetTitleSize(
      st[i]->GetYaxis()->GetTitleSize() * upperScale
    );

    stRatio[i] = new THStack();
    stRatio[i]->SetMinimum(0.0);
    stRatio[i]->SetMaximum(2.5);
    stRatio[i]->Add(munuQCD_JESUP_ratio_hist, "hist");
    stRatio[i]->Add(munuQCD_JESDOWN_ratio_hist, "hist");
    pad2[i]->cd();
    stRatio[i]->Draw("nostack");
    double lowerScale = 1.0/0.3;
    //stRatio[i]->GetYaxis()->SetNdivisions(5,3,0);
    stRatio[i]->GetXaxis()->SetLabelSize(
      stRatio[i]->GetXaxis()->GetLabelSize() * lowerScale
    );
    stRatio[i]->GetXaxis()->SetTitleSize(
      stRatio[i]->GetXaxis()->GetTitleSize() * lowerScale
    );
    stRatio[i]->GetYaxis()->SetLabelSize(
      stRatio[i]->GetYaxis()->GetLabelSize() * lowerScale
    );
    stRatio[i]->GetYaxis()->SetTitle("JES / central    ");
    stRatio[i]->GetXaxis()->SetTitleOffset(1.1);
    stRatio[i]->GetYaxis()->SetTitleOffset(0.3);
    stRatio[i]->GetYaxis()->SetTitleSize(
      stRatio[i]->GetYaxis()->GetTitleSize() * lowerScale
    );
    stRatio[i]->GetXaxis()->SetTitleSize(
      stRatio[i]->GetXaxis()->GetTitleSize() * lowerScale
    );




    if ( i == 0 ){
      mycanvas[i]->Print("JESValidation.pdf[");
      mycanvas[i]->Print("JESValidation.pdf");
    }
    else if ( i == nR-1 ){
      mycanvas[i]->Print("JESValidation.pdf");
      mycanvas[i]->Print("JESValidation.pdf]");
    }
    else{mycanvas[i]->Print("JESValidation.pdf");}
  }

  return 1;
  
}//main

