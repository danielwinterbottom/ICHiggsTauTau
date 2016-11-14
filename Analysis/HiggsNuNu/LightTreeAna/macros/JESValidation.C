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

  // *************************************
  // ********* Open files for SR *********
  // *************************************
  std::string nunu_file         = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/nunu.root";
  std::string nunu_JESUP_file   = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/JESUP/nunu.root";
  std::string nunu_JESDOWN_file = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/JESDOWN/nunu.root";

  // ********************************************
  // ********* Open files for W(enu) CR *********
  // ********************************************
  std::string enu_file         = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/enu.root";
  std::string enu_JESUP_file   = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/JESUP/enu.root";
  std::string enu_JESDOWN_file = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/JESDOWN/enu.root";

  // *********************************************
  // ********* Open files for W(munu) CR *********
  // *********************************************
  std::string munu_file         = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/munu.root";
  std::string munu_JESUP_file   = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/JESUP/munu.root";
  std::string munu_JESDOWN_file = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/JESDOWN/munu.root";

  // **********************************************
  // ********* Open files for W(taunu) CR *********
  // **********************************************
  std::string taunu_file         = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/taunu.root";
  std::string taunu_JESUP_file   = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/JESUP/taunu.root";
  std::string taunu_JESDOWN_file = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/JESDOWN/taunu.root";

  // **********************************************
  // ********* Open files for Z(mumu) CR *********
  // **********************************************
  std::string mumu_file         = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/mumu.root";
  std::string mumu_JESUP_file   = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/JESUP/mumu.root";
  std::string mumu_JESDOWN_file = "/home/hep/rd1715/CMSSW_8_0_20/src/UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/output_run2ana_161031_ICHEP/JESDOWN/mumu.root";


  // ********* Variables of interest for plots *********
  const unsigned nR = 1;
  std::string variables[nR] = {"jet2_pt"};
//   const unsigned nR = 13;
//   std::string variables[nR] = {"jet2_pt",
//                                      "mu1_pt",
//                                      "mu1_eta",
//                                      "n_vertices",
//                                      "jet1_pt",
//                                      "jet1_eta",
//                                      "jet2_eta",
//                                      "metnomuons",
//                                      "dijet_M",
//                                      "dijet_deta",
//                                      "alljetsmetnomu_mindphi",
//                                      "central_tag_eta",
//                                      "forward_tag_eta"
//   };

  // **************************************
  // ********* Open TFiles for SR *********
  // **************************************
  TFile *nunu_Tfile, 
        *nunu_JESUP_Tfile, 
        *nunu_JESDOWN_Tfile;
  nunu_Tfile         = TFile::Open(nunu_file.c_str());
  nunu_JESUP_Tfile   = TFile::Open(nunu_JESUP_file.c_str());
  nunu_JESDOWN_Tfile = TFile::Open(nunu_JESDOWN_file.c_str());
  if (!nunu_Tfile)        { std::cout << " Input file " << nunu_file << " not found." << std::endl; return 1; }
  if (!nunu_JESUP_Tfile)  { std::cout << " Input file " << nunu_JESUP_file << " not found." << std::endl; return 1; }
  if (!nunu_JESDOWN_Tfile){ std::cout << " Input file " << nunu_JESDOWN_file << " not found." << std::endl; return 1; }

  // *********************************************
  // ********* Open TFiles for W(enu) CR *********
  // *********************************************
  TFile *enu_Tfile, 
        *enu_JESUP_Tfile, 
        *enu_JESDOWN_Tfile;
  enu_Tfile         = TFile::Open(enu_file.c_str());
  enu_JESUP_Tfile   = TFile::Open(enu_JESUP_file.c_str());
  enu_JESDOWN_Tfile = TFile::Open(enu_JESDOWN_file.c_str());
  if (!enu_Tfile)        { std::cout << " Input file " << enu_file << " not found." << std::endl; return 1; }
  if (!enu_JESUP_Tfile)  { std::cout << " Input file " << enu_JESUP_file << " not found." << std::endl; return 1; }
  if (!enu_JESDOWN_Tfile){ std::cout << " Input file " << enu_JESDOWN_file << " not found." << std::endl; return 1; }

  // **********************************************
  // ********* Open TFiles for W(munu) CR *********
  // **********************************************
  TFile *munu_Tfile, 
        *munu_JESUP_Tfile, 
        *munu_JESDOWN_Tfile;
  munu_Tfile         = TFile::Open(munu_file.c_str());
  munu_JESUP_Tfile   = TFile::Open(munu_JESUP_file.c_str());
  munu_JESDOWN_Tfile = TFile::Open(munu_JESDOWN_file.c_str());
  if (!munu_Tfile)        { std::cout << " Input file " << munu_file << " not found." << std::endl; return 1; }
  if (!munu_JESUP_Tfile)  { std::cout << " Input file " << munu_JESUP_file << " not found." << std::endl; return 1; }
  if (!munu_JESDOWN_Tfile){ std::cout << " Input file " << munu_JESDOWN_file << " not found." << std::endl; return 1; }

  // ***********************************************
  // ********* Open TFiles for W(taunu) CR *********
  // ***********************************************
  TFile *taunu_Tfile, 
        *taunu_JESUP_Tfile, 
        *taunu_JESDOWN_Tfile;
  taunu_Tfile         = TFile::Open(taunu_file.c_str());
  taunu_JESUP_Tfile   = TFile::Open(taunu_JESUP_file.c_str());
  taunu_JESDOWN_Tfile = TFile::Open(taunu_JESDOWN_file.c_str());
  if (!taunu_Tfile)        { std::cout << " Input file " << taunu_file << " not found." << std::endl; return 1; }
  if (!taunu_JESUP_Tfile)  { std::cout << " Input file " << taunu_JESUP_file << " not found." << std::endl; return 1; }
  if (!taunu_JESDOWN_Tfile){ std::cout << " Input file " << taunu_JESDOWN_file << " not found." << std::endl; return 1; }

  // ***********************************************
  // ********* Open TFiles for Z(mumu) CR *********
  // ***********************************************
  TFile *mumu_Tfile, 
        *mumu_JESUP_Tfile, 
        *mumu_JESDOWN_Tfile;
  mumu_Tfile         = TFile::Open(mumu_file.c_str());
  mumu_JESUP_Tfile   = TFile::Open(mumu_JESUP_file.c_str());
  mumu_JESDOWN_Tfile = TFile::Open(mumu_JESDOWN_file.c_str());
  if (!taunu_Tfile)        { std::cout << " Input file " << mumu_file << " not found." << std::endl; return 1; }
  if (!taunu_JESUP_Tfile)  { std::cout << " Input file " << mumu_JESUP_file << " not found." << std::endl; return 1; }
  if (!taunu_JESDOWN_Tfile){ std::cout << " Input file " << mumu_JESDOWN_file << " not found." << std::endl; return 1; }



  THStack * st[nR];
  THStack * stRatio[nR];
  TCanvas *mycanvas[nR];
  TPad *pad1[nR];
  TPad *pad2[nR];

  for(unsigned i=0; i<nR; ++i){//loop over variable of interest

    mycanvas[i] = new TCanvas(variables[i].c_str(),variables[i].c_str(),200,10,700,500);
    pad1[i] = new TPad("pad1","", 0,0.3,1,1  );
    pad2[i] = new TPad("pad2","", 0,0  ,1,0.3);
    mycanvas[i]->cd();

    pad1[i]->Draw();
    pad2[i]->Draw();
    pad1[i]->SetBottomMargin(0);
    pad2[i]->SetTopMargin(0);


    // ********************************
    // ********* Hists for SR *********
    // ********************************

    // ********* Central *********
    TH1F * nunuWenuQCD_hist   = (TH1F*)nunu_Tfile->Get( Form("welqcd/%s", variables[i].c_str()) );
    TH1F * nunuWenuEWK_hist   = (TH1F*)nunu_Tfile->Get( Form("welewk/%s", variables[i].c_str()) );
    TH1F * nunuWmunuQCD_hist  = (TH1F*)nunu_Tfile->Get( Form("wmuqcd/%s", variables[i].c_str()) );
    TH1F * nunuWmunuEWK_hist  = (TH1F*)nunu_Tfile->Get( Form("wmuewk/%s", variables[i].c_str()) );
    TH1F * nunuWtaunuQCD_hist = (TH1F*)nunu_Tfile->Get( Form("wtauqcd/%s", variables[i].c_str()) );
    TH1F * nunuWtaunuEWK_hist = (TH1F*)nunu_Tfile->Get( Form("wtauewk/%s", variables[i].c_str()) );
    TH1F * nunuZmumuQCD_hist  = (TH1F*)nunu_Tfile->Get( Form("zmumuqcd/%s", variables[i].c_str()) );
    TH1F * nunuZmumuEWK_hist  = (TH1F*)nunu_Tfile->Get( Form("zmumuewk/%s", variables[i].c_str()) );

    // ********* JESUP *********
    TH1F * nunuWenuQCD_JESUP_hist   = (TH1F*)nunu_JESUP_Tfile->Get( Form("welqcd/%s", variables[i].c_str()) );
    TH1F * nunuWenuEWK_JESUP_hist   = (TH1F*)nunu_JESUP_Tfile->Get( Form("welewk/%s", variables[i].c_str()) );
    TH1F * nunuWmunuQCD_JESUP_hist  = (TH1F*)nunu_JESUP_Tfile->Get( Form("wmuqcd/%s", variables[i].c_str()) );
    TH1F * nunuWmunuEWK_JESUP_hist  = (TH1F*)nunu_JESUP_Tfile->Get( Form("wmuewk/%s", variables[i].c_str()) );
    TH1F * nunuWtaunuQCD_JESUP_hist = (TH1F*)nunu_JESUP_Tfile->Get( Form("wtauqcd/%s", variables[i].c_str()) );
    TH1F * nunuWtaunuEWK_JESUP_hist = (TH1F*)nunu_JESUP_Tfile->Get( Form("wtauewk/%s", variables[i].c_str()) );
    TH1F * nunuZmumuQCD_JESUP_hist  = (TH1F*)nunu_JESUP_Tfile->Get( Form("zmumuqcd/%s", variables[i].c_str()) );
    TH1F * nunuZmumuEWK_JESUP_hist  = (TH1F*)nunu_JESUP_Tfile->Get( Form("zmumuewk/%s", variables[i].c_str()) );

    // ********* JESDOWN *********
    TH1F * nunuWenuQCD_JESDOWN_hist   = (TH1F*)nunu_JESDOWN_Tfile->Get( Form("welqcd/%s", variables[i].c_str()) );
    TH1F * nunuWenuEWK_JESDOWN_hist   = (TH1F*)nunu_JESDOWN_Tfile->Get( Form("welewk/%s", variables[i].c_str()) );
    TH1F * nunuWmunuQCD_JESDOWN_hist  = (TH1F*)nunu_JESDOWN_Tfile->Get( Form("wmuqcd/%s", variables[i].c_str()) );
    TH1F * nunuWmunuEWK_JESDOWN_hist  = (TH1F*)nunu_JESDOWN_Tfile->Get( Form("wmuewk/%s", variables[i].c_str()) );
    TH1F * nunuWtaunuQCD_JESDOWN_hist = (TH1F*)nunu_JESDOWN_Tfile->Get( Form("wtauqcd/%s", variables[i].c_str()) );
    TH1F * nunuWtaunuEWK_JESDOWN_hist = (TH1F*)nunu_JESDOWN_Tfile->Get( Form("wtauewk/%s", variables[i].c_str()) );
    TH1F * nunuZmumuQCD_JESDOWN_hist  = (TH1F*)nunu_JESDOWN_Tfile->Get( Form("zmumuqcd/%s", variables[i].c_str()) );
    TH1F * nunuZmumuEWK_JESDOWN_hist  = (TH1F*)nunu_JESDOWN_Tfile->Get( Form("zmumuewk/%s", variables[i].c_str()) );


    // ***************************************
    // ********* Hists for W(enu) CR *********
    // ***************************************

    // ********* Central *********
    TH1F * enuDATAOBS_hist = (TH1F*)enu_Tfile->Get( Form("data_obs/%s", variables[i].c_str()) );
    TH1F * enuQCD_hist     = (TH1F*)enu_Tfile->Get( Form("welqcd/%s", variables[i].c_str()) );
    TH1F * enuEWK_hist     = (TH1F*)enu_Tfile->Get( Form("welewk/%s", variables[i].c_str()) );
    TH1F * enuVV_hist      = (TH1F*)enu_Tfile->Get( Form("vv/%s", variables[i].c_str()) );
    TH1F * enuTOP_hist     = (TH1F*)enu_Tfile->Get( Form("top/%s", variables[i].c_str()) );

    // ********* JESUP *********
    TH1F * enuQCD_JESUP_hist = (TH1F*)enu_JESUP_Tfile->Get( Form("welqcd/%s", variables[i].c_str()) );
    TH1F * enuEWK_JESUP_hist = (TH1F*)enu_JESUP_Tfile->Get( Form("welewk/%s", variables[i].c_str()) );
    TH1F * enuVV_JESUP_hist  = (TH1F*)enu_JESUP_Tfile->Get( Form("vv/%s", variables[i].c_str()) );
    TH1F * enuTOP_JESUP_hist = (TH1F*)enu_JESUP_Tfile->Get( Form("top/%s", variables[i].c_str()) );

    // ********* JESDOWN *********
    TH1F * enuQCD_JESDOWN_hist = (TH1F*)enu_JESDOWN_Tfile->Get( Form("welqcd/%s", variables[i].c_str()) );
    TH1F * enuEWK_JESDOWN_hist = (TH1F*)enu_JESDOWN_Tfile->Get( Form("welewk/%s", variables[i].c_str()) );
    TH1F * enuVV_JESDOWN_hist  = (TH1F*)enu_JESDOWN_Tfile->Get( Form("vv/%s", variables[i].c_str()) );
    TH1F * enuTOP_JESDOWN_hist = (TH1F*)enu_JESDOWN_Tfile->Get( Form("top/%s", variables[i].c_str()) );


    // ****************************************
    // ********* Hists for W(munu) CR *********
    // ****************************************

    // ********* Central *********
    TH1F * munuDATAOBS_hist     = (TH1F*)munu_Tfile->Get( Form("data_obs/%s", variables[i].c_str()) );
    TH1F * munuQCD_hist         = (TH1F*)munu_Tfile->Get( Form("wmuqcd/%s", variables[i].c_str()) );
    TH1F * munuEWK_hist         = (TH1F*)munu_Tfile->Get( Form("wmuewk/%s", variables[i].c_str()) );
    TH1F * munuVV_hist          = (TH1F*)munu_Tfile->Get( Form("vv/%s", variables[i].c_str()) );
    TH1F * munuTOP_hist         = (TH1F*)munu_Tfile->Get( Form("top/%s", variables[i].c_str()) );
    TH1F * munuQCDmultijet_hist = (TH1F*)munu_Tfile->Get( Form("qcd/%s", variables[i].c_str()) );

    // ********* JESUP *********
    TH1F * munuQCD_JESUP_hist         = (TH1F*)munu_JESUP_Tfile->Get( Form("wmuqcd/%s", variables[i].c_str()) );
    TH1F * munuEWK_JESUP_hist         = (TH1F*)munu_JESUP_Tfile->Get( Form("wmuewk/%s", variables[i].c_str()) );
    TH1F * munuVV_JESUP_hist          = (TH1F*)munu_JESUP_Tfile->Get( Form("vv/%s", variables[i].c_str()) );
    TH1F * munuTOP_JESUP_hist         = (TH1F*)munu_JESUP_Tfile->Get( Form("top/%s", variables[i].c_str()) );
    TH1F * munuQCDmultijet_JESUP_hist = (TH1F*)munu_JESUP_Tfile->Get( Form("qcd/%s", variables[i].c_str()) );

    // ********* JESDOWN *********
    TH1F * munuQCD_JESDOWN_hist         = (TH1F*)munu_JESDOWN_Tfile->Get( Form("wmuqcd/%s", variables[i].c_str()) );
    TH1F * munuEWK_JESDOWN_hist         = (TH1F*)munu_JESDOWN_Tfile->Get( Form("wmuewk/%s", variables[i].c_str()) );
    TH1F * munuVV_JESDOWN_hist          = (TH1F*)munu_JESDOWN_Tfile->Get( Form("vv/%s", variables[i].c_str()) );
    TH1F * munuTOP_JESDOWN_hist         = (TH1F*)munu_JESDOWN_Tfile->Get( Form("top/%s", variables[i].c_str()) );
    TH1F * munuQCDmultijet_JESDOWN_hist = (TH1F*)munu_JESDOWN_Tfile->Get( Form("qcd/%s", variables[i].c_str()) );

    // ********* Ratio_Hits *********
    TH1F * munuQCD_JESUP_ratio_hist   = (TH1F*)munuQCD_JESUP_hist->Clone();
    TH1F * munuQCD_JESDOWN_ratio_hist = (TH1F*)munuQCD_JESDOWN_hist->Clone();

    munuQCD_JESUP_ratio_hist->Divide(munuQCD_hist);
    munuQCD_JESUP_ratio_hist->SetLineColor(kBlue);
    munuQCD_JESDOWN_ratio_hist->Divide(munuQCD_hist);
    munuQCD_JESDOWN_ratio_hist->SetLineColor(kRed);


    // *****************************************
    // ********* Hists for W(taunu) CR *********
    // *****************************************

    // ********* Central *********
    TH1F * taunuDATAOBS_hist     = (TH1F*)taunu_Tfile->Get( Form("data_obs/%s", variables[i].c_str()) );
    TH1F * taunuQCD_hist         = (TH1F*)taunu_Tfile->Get( Form("wtauqcd/%s", variables[i].c_str()) );
    TH1F * taunuEWK_hist         = (TH1F*)taunu_Tfile->Get( Form("wtauewk/%s", variables[i].c_str()) );
    TH1F * taunuVV_hist          = (TH1F*)taunu_Tfile->Get( Form("vv/%s", variables[i].c_str()) );
    TH1F * taunuTOP_hist         = (TH1F*)taunu_Tfile->Get( Form("top/%s", variables[i].c_str()) );
    TH1F * taunuQCDmultijet_hist = (TH1F*)taunu_Tfile->Get( Form("qcd/%s", variables[i].c_str()) );

    // ********* JESUP *********
    TH1F * taunuQCD_JESUP_hist         = (TH1F*)taunu_JESUP_Tfile->Get( Form("wtauqcd/%s", variables[i].c_str()) );
    TH1F * taunuEWK_JESUP_hist         = (TH1F*)taunu_JESUP_Tfile->Get( Form("wtauewk/%s", variables[i].c_str()) );
    TH1F * taunuVV_JESUP_hist          = (TH1F*)taunu_JESUP_Tfile->Get( Form("vv/%s", variables[i].c_str()) );
    TH1F * taunuTOP_JESUP_hist         = (TH1F*)taunu_JESUP_Tfile->Get( Form("top/%s", variables[i].c_str()) );
    TH1F * taunuQCDmultijet_JESUP_hist = (TH1F*)taunu_JESUP_Tfile->Get( Form("qcd/%s", variables[i].c_str()) );

    // ********* JESDOWN *********
    TH1F * taunuQCD_JESDOWN_hist         = (TH1F*)taunu_JESDOWN_Tfile->Get( Form("wtauqcd/%s", variables[i].c_str()) );
    TH1F * taunuEWK_JESDOWN_hist         = (TH1F*)taunu_JESDOWN_Tfile->Get( Form("wtauewk/%s", variables[i].c_str()) );
    TH1F * taunuVV_JESDOWN_hist          = (TH1F*)taunu_JESDOWN_Tfile->Get( Form("vv/%s", variables[i].c_str()) );
    TH1F * taunuTOP_JESDOWN_hist         = (TH1F*)taunu_JESDOWN_Tfile->Get( Form("top/%s", variables[i].c_str()) );
    TH1F * taunuQCDmultijet_JESDOWN_hist = (TH1F*)taunu_JESDOWN_Tfile->Get( Form("qcd/%s", variables[i].c_str()) );


    // ***************************************
    // ********* Hists for Z(mumu) CR *********
    // ***************************************

    // ********* Central *********
    TH1F * mumuDATAOBS_hist = (TH1F*)mumu_Tfile->Get( Form("data_obs/%s", variables[i].c_str()) );
    TH1F * mumuQCD_hist     = (TH1F*)mumu_Tfile->Get( Form("zmumuqcd/%s", variables[i].c_str()) );
    TH1F * mumuEWK_hist     = (TH1F*)mumu_Tfile->Get( Form("zmumuewk/%s", variables[i].c_str()) );
    TH1F * mumuVV_hist      = (TH1F*)mumu_Tfile->Get( Form("vv/%s", variables[i].c_str()) );
    TH1F * mumuTOP_hist     = (TH1F*)mumu_Tfile->Get( Form("top/%s", variables[i].c_str()) );

    // ********* JESUP *********
    TH1F * mumuQCD_JESUP_hist = (TH1F*)mumu_JESUP_Tfile->Get( Form("zmumuqcd/%s", variables[i].c_str()) );
    TH1F * mumuEWK_JESUP_hist = (TH1F*)mumu_JESUP_Tfile->Get( Form("zmumuewk/%s", variables[i].c_str()) );
    TH1F * mumuVV_JESUP_hist  = (TH1F*)mumu_JESUP_Tfile->Get( Form("vv/%s", variables[i].c_str()) );
    TH1F * mumuTOP_JESUP_hist = (TH1F*)mumu_JESUP_Tfile->Get( Form("top/%s", variables[i].c_str()) );

    // ********* JESDOWN *********
    TH1F * mumuQCD_JESDOWN_hist = (TH1F*)mumu_JESDOWN_Tfile->Get( Form("zmumuqcd/%s", variables[i].c_str()) );
    TH1F * mumuEWK_JESDOWN_hist = (TH1F*)mumu_JESDOWN_Tfile->Get( Form("zmumuewk/%s", variables[i].c_str()) );
    TH1F * mumuVV_JESDOWN_hist  = (TH1F*)mumu_JESDOWN_Tfile->Get( Form("vv/%s", variables[i].c_str()) );
    TH1F * mumuTOP_JESDOWN_hist = (TH1F*)mumu_JESDOWN_Tfile->Get( Form("top/%s", variables[i].c_str()) );


    if (i==0) {

      std::cout << " -- Making tables: " << std::endl;

      std::cout << std::endl;

      std::cout << " ****************************************" << std::endl;
      std::cout << " ********* Tables for W(enu) CR *********" << std::endl;
      std::cout << " ****************************************" << std::endl;
      // ****************************************
      // ****************************************
      // ****************************************
      std::cout << " \\textbf{W($e\\nu$)}              &  Central             &  JESUP              &  JESDOWN\\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;

      // ********* Central error *********
      std::vector<double> error_enu_hist;
      error_enu_hist.push_back(Error(enuQCD_hist));
      error_enu_hist.push_back(Error(enuEWK_hist));

      std::vector<double> error_nunuWenu_hist;
      error_nunuWenu_hist.push_back(Error(nunuWenuQCD_hist));
      error_nunuWenu_hist.push_back(Error(nunuWenuEWK_hist));

      std::vector<double> errorBKG_enu_hist;
      errorBKG_enu_hist.push_back(Error(enuTOP_hist));
      errorBKG_enu_hist.push_back(Error(enuVV_hist));
      // ********* JESUP error *********
      std::vector<double> error_enu_JESUP_hist;
      error_enu_JESUP_hist.push_back(Error(enuQCD_JESUP_hist));
      error_enu_JESUP_hist.push_back(Error(enuEWK_JESUP_hist));

      std::vector<double> error_nunuWenu_JESUP_hist;
      error_nunuWenu_JESUP_hist.push_back(Error(nunuWenuQCD_JESUP_hist));
      error_nunuWenu_JESUP_hist.push_back(Error(nunuWenuEWK_JESUP_hist));

      std::vector<double> errorBKG_enu_JESUP_hist;
      errorBKG_enu_JESUP_hist.push_back(Error(enuTOP_JESUP_hist));
      errorBKG_enu_JESUP_hist.push_back(Error(enuVV_JESUP_hist));
      // ********* JESDOWN error *********
      std::vector<double> error_enu_JESDOWN_hist;
      error_enu_JESDOWN_hist.push_back(Error(enuQCD_JESDOWN_hist));
      error_enu_JESDOWN_hist.push_back(Error(enuEWK_JESDOWN_hist));

      std::vector<double> error_nunuWenu_JESDOWN_hist;
      error_nunuWenu_JESDOWN_hist.push_back(Error(nunuWenuQCD_JESDOWN_hist));
      error_nunuWenu_JESDOWN_hist.push_back(Error(nunuWenuEWK_JESDOWN_hist));

      std::vector<double> errorBKG_enu_JESDOWN_hist;
      errorBKG_enu_JESDOWN_hist.push_back(Error(enuTOP_JESDOWN_hist));
      errorBKG_enu_JESDOWN_hist.push_back(Error(enuVV_JESDOWN_hist));

      std::cout << " N$_\\text{MC}^\\text{CR}$           &  " 
                << enuQCD_hist->Integral(0, enuQCD_hist->GetNbinsX() + 1) + 
                   enuEWK_hist->Integral(0, enuEWK_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_enu_hist ) << " & "
                << enuQCD_JESUP_hist->Integral(0, enuQCD_JESUP_hist->GetNbinsX() + 1) + 
                   enuEWK_JESUP_hist->Integral(0, enuEWK_JESUP_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_enu_JESUP_hist ) << " & "
                << enuQCD_JESDOWN_hist->Integral(0, enuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                   enuEWK_JESDOWN_hist->Integral(0, enuEWK_JESDOWN_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_enu_JESDOWN_hist ) 
                << "\\\\ " << std::endl;

      std::cout << " N$_\\text{MC}^\\text{SR}$           &  " 
                << nunuWenuQCD_hist->Integral(0, nunuWenuQCD_hist->GetNbinsX() + 1) + 
                   nunuWenuEWK_hist->Integral(0, nunuWenuEWK_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_nunuWenu_hist ) << " & "
                << nunuWenuQCD_JESUP_hist->Integral(0, nunuWenuQCD_JESUP_hist->GetNbinsX() + 1) + 
                   nunuWenuEWK_JESUP_hist->Integral(0, nunuWenuEWK_JESUP_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_nunuWenu_JESUP_hist ) << " & "
                << nunuWenuQCD_JESDOWN_hist->Integral(0, nunuWenuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                   nunuWenuEWK_JESDOWN_hist->Integral(0, nunuWenuEWK_JESDOWN_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_nunuWenu_JESDOWN_hist ) 
                << "\\\\ " << std::endl;

      std::cout << " N$_\\text{data\\_obs}^\\text{CR}$     &  " 
                << enuDATAOBS_hist->Integral(0, enuDATAOBS_hist->GetNbinsX() + 1) << " $\\pm$ " << Error(enuDATAOBS_hist) << "     & "
                << enuDATAOBS_hist->Integral(0, enuDATAOBS_hist->GetNbinsX() + 1) << " $\\pm$ " << Error(enuDATAOBS_hist) << "     & "
                << enuDATAOBS_hist->Integral(0, enuDATAOBS_hist->GetNbinsX() + 1) << " $\\pm$ " << Error(enuDATAOBS_hist) 
                << "\\\\ " << std::endl;

      std::cout << " N$_\\text{backgrounds}^\\text{CR}$  &  " 
                << enuTOP_hist->Integral(0, enuTOP_hist->GetNbinsX() + 1) + 
                   enuVV_hist->Integral(0, enuVV_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( errorBKG_enu_hist ) << " & "
                << enuTOP_JESUP_hist->Integral(0, enuTOP_JESUP_hist->GetNbinsX() + 1) + 
                   enuVV_JESUP_hist->Integral(0, enuVV_JESUP_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( errorBKG_enu_JESUP_hist ) << " & "
                << enuTOP_JESDOWN_hist->Integral(0, enuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                   enuVV_JESDOWN_hist->Integral(0, enuVV_JESDOWN_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( errorBKG_enu_JESDOWN_hist ) 
                << "\\\\ " << std::endl;

      std::cout << " \\hline" << std::endl;

      std::cout << " N$_\\text{W($e\\nu$)}^\\text{SR}$  &  " 
                << ( ( nunuWenuQCD_hist->Integral(0, nunuWenuQCD_hist->GetNbinsX() + 1) + 
                       nunuWenuEWK_hist->Integral(0, nunuWenuEWK_hist->GetNbinsX() + 1) )/( 
                       enuQCD_hist->Integral(0, enuQCD_hist->GetNbinsX() + 1) + 
                       enuEWK_hist->Integral(0, enuEWK_hist->GetNbinsX() + 1) ) )*( 
                       enuDATAOBS_hist->Integral(0, enuDATAOBS_hist->GetNbinsX() + 1) - ( 
                       enuTOP_hist->Integral(0, enuTOP_hist->GetNbinsX() + 1) + 
                       enuVV_hist->Integral(0, enuVV_hist->GetNbinsX() + 1) ) ) << "             & "
                << ( ( nunuWenuQCD_JESUP_hist->Integral(0, nunuWenuQCD_JESUP_hist->GetNbinsX() + 1) + 
                       nunuWenuEWK_JESUP_hist->Integral(0, nunuWenuEWK_JESUP_hist->GetNbinsX() + 1) )/( 
                       enuQCD_JESUP_hist->Integral(0, enuQCD_JESUP_hist->GetNbinsX() + 1) + 
                       enuEWK_JESUP_hist->Integral(0, enuEWK_JESUP_hist->GetNbinsX() + 1) ) )*( 
                       enuDATAOBS_hist->Integral(0, enuDATAOBS_hist->GetNbinsX() + 1) - ( 
                       enuTOP_JESUP_hist->Integral(0, enuTOP_JESUP_hist->GetNbinsX() + 1) + 
                       enuVV_JESUP_hist->Integral(0, enuVV_JESUP_hist->GetNbinsX() + 1) ) ) << "             & "
                << ( ( nunuWenuQCD_JESDOWN_hist->Integral(0, nunuWenuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                       nunuWenuEWK_JESDOWN_hist->Integral(0, nunuWenuEWK_JESDOWN_hist->GetNbinsX() + 1) )/( 
                       enuQCD_JESDOWN_hist->Integral(0, enuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                       enuEWK_JESDOWN_hist->Integral(0, enuEWK_JESDOWN_hist->GetNbinsX() + 1) ) )*( 
                       enuDATAOBS_hist->Integral(0, enuDATAOBS_hist->GetNbinsX() + 1) - ( 
                       enuTOP_JESDOWN_hist->Integral(0, enuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                       enuVV_JESDOWN_hist->Integral(0, enuVV_JESDOWN_hist->GetNbinsX() + 1) ) )
                << "\\\\ " << std::endl;
      std::cout << std::endl;

      // ****************************************
      std::cout << " *****************************************" << std::endl;
      // ****************************************

      std::cout << " \\textbf{W($e\\nu$)}             &  JESUP\\_Impact (\\%)              &  JESDOWN\\_Impact (\\%)\\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;

      std::cout << " N$_\\text{MC}^\\text{CR}$           &  " 
                << (( enuQCD_JESUP_hist->Integral(0, enuQCD_JESUP_hist->GetNbinsX() + 1) + 
                      enuEWK_JESUP_hist->Integral(0, enuEWK_JESUP_hist->GetNbinsX() + 1) ) - 
                    ( enuQCD_hist->Integral(0, enuQCD_hist->GetNbinsX() + 1) + 
                      enuEWK_hist->Integral(0, enuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( enuQCD_hist->Integral(0, enuQCD_hist->GetNbinsX() + 1) + 
                      enuEWK_hist->Integral(0, enuEWK_hist->GetNbinsX() + 1) ) << " & "
                << (( enuQCD_JESDOWN_hist->Integral(0, enuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                      enuEWK_JESDOWN_hist->Integral(0, enuEWK_JESDOWN_hist->GetNbinsX() + 1) ) - 
                    ( enuQCD_hist->Integral(0, enuQCD_hist->GetNbinsX() + 1) + 
                      enuEWK_hist->Integral(0, enuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( enuQCD_hist->Integral(0, enuQCD_hist->GetNbinsX() + 1) + 
                      enuEWK_hist->Integral(0, enuEWK_hist->GetNbinsX() + 1) ) << "\\\\ " << std::endl;

      std::cout << " N$_\\text{MC}^\\text{SR}$           &  " 
                << (( nunuWenuQCD_JESUP_hist->Integral(0, nunuWenuQCD_JESUP_hist->GetNbinsX() + 1) + 
                      nunuWenuEWK_JESUP_hist->Integral(0, nunuWenuEWK_JESUP_hist->GetNbinsX() + 1) ) - 
                    ( nunuWenuQCD_hist->Integral(0, nunuWenuQCD_hist->GetNbinsX() + 1) + 
                      nunuWenuEWK_hist->Integral(0, nunuWenuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( nunuWenuQCD_hist->Integral(0, nunuWenuQCD_hist->GetNbinsX() + 1) + 
                      nunuWenuEWK_hist->Integral(0, nunuWenuEWK_hist->GetNbinsX() + 1) ) << " & "
                << (( nunuWenuQCD_JESDOWN_hist->Integral(0, nunuWenuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                      nunuWenuEWK_JESDOWN_hist->Integral(0, nunuWenuEWK_JESDOWN_hist->GetNbinsX() + 1) ) - 
                    ( nunuWenuQCD_hist->Integral(0, nunuWenuQCD_hist->GetNbinsX() + 1) + 
                      nunuWenuEWK_hist->Integral(0, nunuWenuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( nunuWenuQCD_hist->Integral(0, nunuWenuQCD_hist->GetNbinsX() + 1) + 
                      nunuWenuEWK_hist->Integral(0, nunuWenuEWK_hist->GetNbinsX() + 1) ) << "\\\\ " << std::endl;

      std::cout << " N$_\\text{data\\_obs}^\\text{CR}$     &  " 
                << " 0 " << "     & "
                << " 0 " << "\\\\ " << std::endl;

      std::cout << " N$_\\text{backgrounds}^\\text{CR}$  &  " 
                << (( enuTOP_JESUP_hist->Integral(0, enuTOP_JESUP_hist->GetNbinsX() + 1) + 
                      enuVV_JESUP_hist->Integral(0, enuVV_JESUP_hist->GetNbinsX() + 1) ) - 
                    ( enuTOP_hist->Integral(0, enuTOP_hist->GetNbinsX() + 1) + 
                      enuVV_hist->Integral(0, enuVV_hist->GetNbinsX() + 1) ))*100/
                    ( enuTOP_hist->Integral(0, enuTOP_hist->GetNbinsX() + 1) + 
                      enuVV_hist->Integral(0, enuVV_hist->GetNbinsX() + 1) ) << " & "
                << (( enuTOP_JESDOWN_hist->Integral(0, enuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                      enuVV_JESDOWN_hist->Integral(0, enuVV_JESDOWN_hist->GetNbinsX() + 1) ) - 
                    ( enuTOP_hist->Integral(0, enuTOP_hist->GetNbinsX() + 1) + 
                      enuVV_hist->Integral(0, enuVV_hist->GetNbinsX() + 1) ))*100/
                    ( enuTOP_hist->Integral(0, enuTOP_hist->GetNbinsX() + 1) + 
                      enuVV_hist->Integral(0, enuVV_hist->GetNbinsX() + 1) ) << "\\\\ " << std::endl;

      std::cout << " \\hline" << std::endl;

      std::cout << " N$_\\text{W($e\\nu$)}^\\text{SR}$  &  " 
                << (( ( ( nunuWenuQCD_JESUP_hist->Integral(0, nunuWenuQCD_JESUP_hist->GetNbinsX() + 1) + 
                          nunuWenuEWK_JESUP_hist->Integral(0, nunuWenuEWK_JESUP_hist->GetNbinsX() + 1) )/( 
                          enuQCD_JESUP_hist->Integral(0, enuQCD_JESUP_hist->GetNbinsX() + 1) + 
                          enuEWK_JESUP_hist->Integral(0, enuEWK_JESUP_hist->GetNbinsX() + 1) ) )*( 
                          enuDATAOBS_hist->Integral(0, enuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          enuTOP_JESUP_hist->Integral(0, enuTOP_JESUP_hist->GetNbinsX() + 1) + 
                          enuVV_JESUP_hist->Integral(0, enuVV_JESUP_hist->GetNbinsX() + 1) ) ) ) - 
                    ( ( ( nunuWenuQCD_hist->Integral(0, nunuWenuQCD_hist->GetNbinsX() + 1) + 
                          nunuWenuEWK_hist->Integral(0, nunuWenuEWK_hist->GetNbinsX() + 1) )/( 
                          enuQCD_hist->Integral(0, enuQCD_hist->GetNbinsX() + 1) + 
                          enuEWK_hist->Integral(0, enuEWK_hist->GetNbinsX() + 1) ) )*( 
                          enuDATAOBS_hist->Integral(0, enuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          enuTOP_hist->Integral(0, enuTOP_hist->GetNbinsX() + 1) + 
                          enuVV_hist->Integral(0, enuVV_hist->GetNbinsX() + 1) ) ) ))*100/
                    ( ( ( nunuWenuQCD_hist->Integral(0, nunuWenuQCD_hist->GetNbinsX() + 1) + 
                          nunuWenuEWK_hist->Integral(0, nunuWenuEWK_hist->GetNbinsX() + 1) )/( 
                          enuQCD_hist->Integral(0, enuQCD_hist->GetNbinsX() + 1) + 
                          enuEWK_hist->Integral(0, enuEWK_hist->GetNbinsX() + 1) ) )*( 
                          enuDATAOBS_hist->Integral(0, enuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          enuTOP_hist->Integral(0, enuTOP_hist->GetNbinsX() + 1) + 
                          enuVV_hist->Integral(0, enuVV_hist->GetNbinsX() + 1) ) ) ) << " & "
                << (( ( ( nunuWenuQCD_JESDOWN_hist->Integral(0, nunuWenuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                          nunuWenuEWK_JESDOWN_hist->Integral(0, nunuWenuEWK_JESDOWN_hist->GetNbinsX() + 1) )/( 
                          enuQCD_JESDOWN_hist->Integral(0, enuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                          enuEWK_JESDOWN_hist->Integral(0, enuEWK_JESDOWN_hist->GetNbinsX() + 1) ) )*( 
                          enuDATAOBS_hist->Integral(0, enuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          enuTOP_JESDOWN_hist->Integral(0, enuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                          enuVV_JESDOWN_hist->Integral(0, enuVV_JESDOWN_hist->GetNbinsX() + 1) ) ) ) - 
                    ( ( ( nunuWenuQCD_hist->Integral(0, nunuWenuQCD_hist->GetNbinsX() + 1) + 
                          nunuWenuEWK_hist->Integral(0, nunuWenuEWK_hist->GetNbinsX() + 1) )/( 
                          enuQCD_hist->Integral(0, enuQCD_hist->GetNbinsX() + 1) + 
                          enuEWK_hist->Integral(0, enuEWK_hist->GetNbinsX() + 1) ) )*( 
                          enuDATAOBS_hist->Integral(0, enuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          enuTOP_hist->Integral(0, enuTOP_hist->GetNbinsX() + 1) + 
                          enuVV_hist->Integral(0, enuVV_hist->GetNbinsX() + 1) ) ) ))*100/
                    ( ( ( nunuWenuQCD_hist->Integral(0, nunuWenuQCD_hist->GetNbinsX() + 1) + 
                          nunuWenuEWK_hist->Integral(0, nunuWenuEWK_hist->GetNbinsX() + 1) )/( 
                          enuQCD_hist->Integral(0, enuQCD_hist->GetNbinsX() + 1) + 
                          enuEWK_hist->Integral(0, enuEWK_hist->GetNbinsX() + 1) ) )*( 
                          enuDATAOBS_hist->Integral(0, enuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          enuTOP_hist->Integral(0, enuTOP_hist->GetNbinsX() + 1) + 
                          enuVV_hist->Integral(0, enuVV_hist->GetNbinsX() + 1) ) ) ) << "\\\\ " << std::endl;

      std::cout << " \\hline" << std::endl;

      std::cout << " $\\frac{\\text{N}_\\text{MC}^\\text{SR}}{\\text{N}_\\text{MC}^\\text{CR}}$  &  " 
                << (( ( nunuWenuQCD_JESUP_hist->Integral(0, nunuWenuQCD_JESUP_hist->GetNbinsX() + 1) + 
                        nunuWenuEWK_JESUP_hist->Integral(0, nunuWenuEWK_JESUP_hist->GetNbinsX() + 1) )/( 
                        enuQCD_JESUP_hist->Integral(0, enuQCD_JESUP_hist->GetNbinsX() + 1) + 
                        enuEWK_JESUP_hist->Integral(0, enuEWK_JESUP_hist->GetNbinsX() + 1) ) ) - 
                    ( ( nunuWenuQCD_hist->Integral(0, nunuWenuQCD_hist->GetNbinsX() + 1) + 
                        nunuWenuEWK_hist->Integral(0, nunuWenuEWK_hist->GetNbinsX() + 1) )/( 
                        enuQCD_hist->Integral(0, enuQCD_hist->GetNbinsX() + 1) + 
                        enuEWK_hist->Integral(0, enuEWK_hist->GetNbinsX() + 1) ) ))*100/
                    ( ( nunuWenuQCD_hist->Integral(0, nunuWenuQCD_hist->GetNbinsX() + 1) + 
                        nunuWenuEWK_hist->Integral(0, nunuWenuEWK_hist->GetNbinsX() + 1) )/( 
                        enuQCD_hist->Integral(0, enuQCD_hist->GetNbinsX() + 1) + 
                        enuEWK_hist->Integral(0, enuEWK_hist->GetNbinsX() + 1) ) ) << " & "
                << (( ( nunuWenuQCD_JESDOWN_hist->Integral(0, nunuWenuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                        nunuWenuEWK_JESDOWN_hist->Integral(0, nunuWenuEWK_JESDOWN_hist->GetNbinsX() + 1) )/( 
                        enuQCD_JESDOWN_hist->Integral(0, enuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                        enuEWK_JESDOWN_hist->Integral(0, enuEWK_JESDOWN_hist->GetNbinsX() + 1) ) ) - 
                    ( ( nunuWenuQCD_hist->Integral(0, nunuWenuQCD_hist->GetNbinsX() + 1) + 
                        nunuWenuEWK_hist->Integral(0, nunuWenuEWK_hist->GetNbinsX() + 1) )/( 
                        enuQCD_hist->Integral(0, enuQCD_hist->GetNbinsX() + 1) + 
                        enuEWK_hist->Integral(0, enuEWK_hist->GetNbinsX() + 1) ) ))*100/
                    ( ( nunuWenuQCD_hist->Integral(0, nunuWenuQCD_hist->GetNbinsX() + 1) + 
                        nunuWenuEWK_hist->Integral(0, nunuWenuEWK_hist->GetNbinsX() + 1) )/( 
                        enuQCD_hist->Integral(0, enuQCD_hist->GetNbinsX() + 1) + 
                        enuEWK_hist->Integral(0, enuEWK_hist->GetNbinsX() + 1) ) ) << "\\\\ " << std::endl;

      std::cout << " ( N$_\\text{data\\_obs}^\\text{CR}$ - N$_\\text{backgrounds}^\\text{CR}$)  &  " 
                << ( enuTOP_hist->Integral(0, enuTOP_hist->GetNbinsX() + 1) + 
                     enuVV_hist->Integral(0, enuVV_hist->GetNbinsX() + 1) -
                   ( enuTOP_JESUP_hist->Integral(0, enuTOP_JESUP_hist->GetNbinsX() + 1) + 
                     enuVV_JESUP_hist->Integral(0, enuVV_JESUP_hist->GetNbinsX() + 1) ) )*100/
                   ( enuDATAOBS_hist->Integral(0, enuDATAOBS_hist->GetNbinsX() + 1) - 
                   ( enuTOP_hist->Integral(0, enuTOP_hist->GetNbinsX() + 1) + 
                     enuVV_hist->Integral(0, enuVV_hist->GetNbinsX() + 1) ) ) << " & "
                << ( enuTOP_hist->Integral(0, enuTOP_hist->GetNbinsX() + 1) + 
                     enuVV_hist->Integral(0, enuVV_hist->GetNbinsX() + 1) -
                   ( enuTOP_JESDOWN_hist->Integral(0, enuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                     enuVV_JESDOWN_hist->Integral(0, enuVV_JESDOWN_hist->GetNbinsX() + 1) ) )*100/
                   ( enuDATAOBS_hist->Integral(0, enuDATAOBS_hist->GetNbinsX() + 1) - 
                   ( enuTOP_hist->Integral(0, enuTOP_hist->GetNbinsX() + 1) + 
                     enuVV_hist->Integral(0, enuVV_hist->GetNbinsX() + 1) ) ) << "\\\\ " << std::endl;

      std::cout << std::endl;

      // ****************************************
      std::cout << " *****************************************" << std::endl;
      // ****************************************

      std::cout << " \\textbf{W($e\\nu$)}     &              &  Central             &  JESUP              &  JESDOWN\\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;
      std::cout << " \\multirow{2}*{TOP}       &  Integral    & " 
                << enuTOP_hist->Integral(0, enuTOP_hist->GetNbinsX() + 1) << "              & " 
                << enuTOP_JESUP_hist->Integral(0, enuTOP_JESUP_hist->GetNbinsX() + 1) << "             & "
                << enuTOP_JESDOWN_hist->Integral(0, enuTOP_JESDOWN_hist->GetNbinsX() + 1) << " \\\\ " << std::endl;
      std::cout << "                          &  GetEntries  & " 
                << enuTOP_hist->GetEntries() << "                  & " 
                << enuTOP_JESUP_hist->GetEntries() << "                 & "
                << enuTOP_JESDOWN_hist->GetEntries() << " \\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;
      std::cout << " \\multirow{2}*{VV}       &  Integral    & " 
                << enuVV_hist->Integral(0, enuVV_hist->GetNbinsX() + 1) << "              & " 
                << enuVV_JESUP_hist->Integral(0, enuVV_JESUP_hist->GetNbinsX() + 1) << "             & "
                << enuVV_JESDOWN_hist->Integral(0, enuVV_JESDOWN_hist->GetNbinsX() + 1) << " \\\\ " << std::endl;
      std::cout << "                          &  GetEntries  & " 
                << enuVV_hist->GetEntries() << "                  & " 
                << enuVV_JESUP_hist->GetEntries() << "                 & "
                << enuVV_JESDOWN_hist->GetEntries() << " \\\\ " << std::endl;

      std::cout << std::endl;

      std::cout << std::endl;

      std::cout << " *****************************************" << std::endl;
      std::cout << " ********* Tables for W(munu) CR *********" << std::endl;
      std::cout << " *****************************************" << std::endl;
      // ****************************************
      // ****************************************
      // ****************************************
      std::cout << " \\textbf{W($\\mu\\nu$)}              &  Central             &  JESUP              &  JESDOWN\\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;

      // ********* Central error *********
      std::vector<double> error_munu_hist;
      error_munu_hist.push_back(Error(munuQCD_hist));
      error_munu_hist.push_back(Error(munuEWK_hist));

      std::vector<double> error_nunuWmunu_hist;
      error_nunuWmunu_hist.push_back(Error(nunuWmunuQCD_hist));
      error_nunuWmunu_hist.push_back(Error(nunuWmunuEWK_hist));

      std::vector<double> errorBKG_munu_hist;
      errorBKG_munu_hist.push_back(Error(munuTOP_hist));
      errorBKG_munu_hist.push_back(Error(munuVV_hist));
      errorBKG_munu_hist.push_back(Error(munuQCDmultijet_hist));
      // ********* JESUP error *********
      std::vector<double> error_munu_JESUP_hist;
      error_munu_JESUP_hist.push_back(Error(munuQCD_JESUP_hist));
      error_munu_JESUP_hist.push_back(Error(munuEWK_JESUP_hist));

      std::vector<double> error_nunuWmunu_JESUP_hist;
      error_nunuWmunu_JESUP_hist.push_back(Error(nunuWmunuQCD_JESUP_hist));
      error_nunuWmunu_JESUP_hist.push_back(Error(nunuWmunuEWK_JESUP_hist));

      std::vector<double> errorBKG_munu_JESUP_hist;
      errorBKG_munu_JESUP_hist.push_back(Error(munuTOP_JESUP_hist));
      errorBKG_munu_JESUP_hist.push_back(Error(munuVV_JESUP_hist));
      errorBKG_munu_JESUP_hist.push_back(Error(munuQCDmultijet_JESUP_hist));
      // ********* JESDOWN error *********
      std::vector<double> error_munu_JESDOWN_hist;
      error_munu_JESDOWN_hist.push_back(Error(munuQCD_JESDOWN_hist));
      error_munu_JESDOWN_hist.push_back(Error(munuEWK_JESDOWN_hist));

      std::vector<double> error_nunuWmunu_JESDOWN_hist;
      error_nunuWmunu_JESDOWN_hist.push_back(Error(nunuWmunuQCD_JESDOWN_hist));
      error_nunuWmunu_JESDOWN_hist.push_back(Error(nunuWmunuEWK_JESDOWN_hist));

      std::vector<double> errorBKG_munu_JESDOWN_hist;
      errorBKG_munu_JESDOWN_hist.push_back(Error(munuTOP_JESDOWN_hist));
      errorBKG_munu_JESDOWN_hist.push_back(Error(munuVV_JESDOWN_hist));
      errorBKG_munu_JESDOWN_hist.push_back(Error(munuQCDmultijet_JESDOWN_hist));

      std::cout << " N$_\\text{MC}^\\text{CR}$           &  " 
                << munuQCD_hist->Integral(0, munuQCD_hist->GetNbinsX() + 1) + 
                   munuEWK_hist->Integral(0, munuEWK_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_munu_hist ) << " & "
                << munuQCD_JESUP_hist->Integral(0, munuQCD_JESUP_hist->GetNbinsX() + 1) + 
                   munuEWK_JESUP_hist->Integral(0, munuEWK_JESUP_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_munu_JESUP_hist ) << " & "
                << munuQCD_JESDOWN_hist->Integral(0, munuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                   munuEWK_JESDOWN_hist->Integral(0, munuEWK_JESDOWN_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_munu_JESDOWN_hist ) 
                << "\\\\ " << std::endl;

      std::cout << " N$_\\text{MC}^\\text{SR}$           &  " 
                << nunuWmunuQCD_hist->Integral(0, nunuWmunuQCD_hist->GetNbinsX() + 1) + 
                   nunuWmunuEWK_hist->Integral(0, nunuWmunuEWK_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_nunuWmunu_hist ) << " & "
                << nunuWmunuQCD_JESUP_hist->Integral(0, nunuWmunuQCD_JESUP_hist->GetNbinsX() + 1) + 
                   nunuWmunuEWK_JESUP_hist->Integral(0, nunuWmunuEWK_JESUP_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_nunuWmunu_JESUP_hist ) << " & "
                << nunuWmunuQCD_JESDOWN_hist->Integral(0, nunuWmunuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                   nunuWmunuEWK_JESDOWN_hist->Integral(0, nunuWmunuEWK_JESDOWN_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_nunuWmunu_JESDOWN_hist ) 
                << "\\\\ " << std::endl;

      std::cout << " N$_\\text{data\\_obs}^\\text{CR}$     &  " 
                << munuDATAOBS_hist->Integral(0, munuDATAOBS_hist->GetNbinsX() + 1) << " $\\pm$ " << Error(munuDATAOBS_hist) << "     & "
                << munuDATAOBS_hist->Integral(0, munuDATAOBS_hist->GetNbinsX() + 1) << " $\\pm$ " << Error(munuDATAOBS_hist) << "     & "
                << munuDATAOBS_hist->Integral(0, munuDATAOBS_hist->GetNbinsX() + 1) << " $\\pm$ " << Error(munuDATAOBS_hist) 
                << "\\\\ " << std::endl;

      std::cout << " N$_\\text{backgrounds}^\\text{CR}$  &  " 
                << munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) + 
                   munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) + 
                   munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( errorBKG_munu_hist ) << " & "
                << munuTOP_JESUP_hist->Integral(0, munuTOP_JESUP_hist->GetNbinsX() + 1) + 
                   munuVV_JESUP_hist->Integral(0, munuVV_JESUP_hist->GetNbinsX() + 1) + 
                   munuQCDmultijet_JESUP_hist->Integral(0, munuQCDmultijet_JESUP_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( errorBKG_munu_JESUP_hist ) << " & "
                << munuTOP_JESDOWN_hist->Integral(0, munuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                   munuVV_JESDOWN_hist->Integral(0, munuVV_JESDOWN_hist->GetNbinsX() + 1) + 
                   munuQCDmultijet_JESDOWN_hist->Integral(0, munuQCDmultijet_JESDOWN_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( errorBKG_munu_JESDOWN_hist ) 
                << "\\\\ " << std::endl;

      std::cout << " \\hline" << std::endl;

      std::cout << " N$_\\text{W($\\mu\\nu$)}^\\text{SR}$  &  " 
                << ( ( nunuWmunuQCD_hist->Integral(0, nunuWmunuQCD_hist->GetNbinsX() + 1) + 
                       nunuWmunuEWK_hist->Integral(0, nunuWmunuEWK_hist->GetNbinsX() + 1) )/( 
                       munuQCD_hist->Integral(0, munuQCD_hist->GetNbinsX() + 1) + 
                       munuEWK_hist->Integral(0, munuEWK_hist->GetNbinsX() + 1) ) )*( 
                       munuDATAOBS_hist->Integral(0, munuDATAOBS_hist->GetNbinsX() + 1) - ( 
                       munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) + 
                       munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) + 
                       munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) ) ) << "             & "
                << ( ( nunuWmunuQCD_JESUP_hist->Integral(0, nunuWmunuQCD_JESUP_hist->GetNbinsX() + 1) + 
                       nunuWmunuEWK_JESUP_hist->Integral(0, nunuWmunuEWK_JESUP_hist->GetNbinsX() + 1) )/( 
                       munuQCD_JESUP_hist->Integral(0, munuQCD_JESUP_hist->GetNbinsX() + 1) + 
                       munuEWK_JESUP_hist->Integral(0, munuEWK_JESUP_hist->GetNbinsX() + 1) ) )*( 
                       munuDATAOBS_hist->Integral(0, munuDATAOBS_hist->GetNbinsX() + 1) - ( 
                       munuTOP_JESUP_hist->Integral(0, munuTOP_JESUP_hist->GetNbinsX() + 1) + 
                       munuVV_JESUP_hist->Integral(0, munuVV_JESUP_hist->GetNbinsX() + 1) + 
                       munuQCDmultijet_JESUP_hist->Integral(0, munuQCDmultijet_JESUP_hist->GetNbinsX() + 1) ) ) << "             & "
                << ( ( nunuWmunuQCD_JESDOWN_hist->Integral(0, nunuWmunuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                       nunuWmunuEWK_JESDOWN_hist->Integral(0, nunuWmunuEWK_JESDOWN_hist->GetNbinsX() + 1) )/( 
                       munuQCD_JESDOWN_hist->Integral(0, munuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                       munuEWK_JESDOWN_hist->Integral(0, munuEWK_JESDOWN_hist->GetNbinsX() + 1) ) )*( 
                       munuDATAOBS_hist->Integral(0, munuDATAOBS_hist->GetNbinsX() + 1) - ( 
                       munuTOP_JESDOWN_hist->Integral(0, munuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                       munuVV_JESDOWN_hist->Integral(0, munuVV_JESDOWN_hist->GetNbinsX() + 1) + 
                       munuQCDmultijet_JESDOWN_hist->Integral(0, munuQCDmultijet_JESDOWN_hist->GetNbinsX() + 1) ) )
                << "\\\\ " << std::endl;
      std::cout << std::endl;

      // ****************************************
      std::cout << " *****************************************" << std::endl;
      // ****************************************

      std::cout << " \\textbf{W($\\mu\\nu$)}             &  JESUP\\_Impact (\\%)              &  JESDOWN\\_Impact (\\%)\\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;

      std::cout << " N$_\\text{MC}^\\text{CR}$           &  " 
                << (( munuQCD_JESUP_hist->Integral(0, munuQCD_JESUP_hist->GetNbinsX() + 1) + 
                      munuEWK_JESUP_hist->Integral(0, munuEWK_JESUP_hist->GetNbinsX() + 1) ) - 
                    ( munuQCD_hist->Integral(0, munuQCD_hist->GetNbinsX() + 1) + 
                      munuEWK_hist->Integral(0, munuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( munuQCD_hist->Integral(0, munuQCD_hist->GetNbinsX() + 1) + 
                      munuEWK_hist->Integral(0, munuEWK_hist->GetNbinsX() + 1) ) << " & "
                << (( munuQCD_JESDOWN_hist->Integral(0, munuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                      munuEWK_JESDOWN_hist->Integral(0, munuEWK_JESDOWN_hist->GetNbinsX() + 1) ) - 
                    ( munuQCD_hist->Integral(0, munuQCD_hist->GetNbinsX() + 1) + 
                      munuEWK_hist->Integral(0, munuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( munuQCD_hist->Integral(0, munuQCD_hist->GetNbinsX() + 1) + 
                      munuEWK_hist->Integral(0, munuEWK_hist->GetNbinsX() + 1) ) << "\\\\ " << std::endl;

      std::cout << " N$_\\text{MC}^\\text{SR}$           &  " 
                << (( nunuWmunuQCD_JESUP_hist->Integral(0, nunuWmunuQCD_JESUP_hist->GetNbinsX() + 1) + 
                      nunuWmunuEWK_JESUP_hist->Integral(0, nunuWmunuEWK_JESUP_hist->GetNbinsX() + 1) ) - 
                    ( nunuWmunuQCD_hist->Integral(0, nunuWmunuQCD_hist->GetNbinsX() + 1) + 
                      nunuWmunuEWK_hist->Integral(0, nunuWmunuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( nunuWmunuQCD_hist->Integral(0, nunuWmunuQCD_hist->GetNbinsX() + 1) + 
                      nunuWmunuEWK_hist->Integral(0, nunuWmunuEWK_hist->GetNbinsX() + 1) ) << " & "
                << (( nunuWmunuQCD_JESDOWN_hist->Integral(0, nunuWmunuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                      nunuWmunuEWK_JESDOWN_hist->Integral(0, nunuWmunuEWK_JESDOWN_hist->GetNbinsX() + 1) ) - 
                    ( nunuWmunuQCD_hist->Integral(0, nunuWmunuQCD_hist->GetNbinsX() + 1) + 
                      nunuWmunuEWK_hist->Integral(0, nunuWmunuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( nunuWmunuQCD_hist->Integral(0, nunuWmunuQCD_hist->GetNbinsX() + 1) + 
                      nunuWmunuEWK_hist->Integral(0, nunuWmunuEWK_hist->GetNbinsX() + 1) ) << "\\\\ " << std::endl;

      std::cout << " N$_\\text{data\\_obs}^\\text{CR}$     &  " 
                << " 0 " << "     & "
                << " 0 " << "\\\\ " << std::endl;

      std::cout << " N$_\\text{backgrounds}^\\text{CR}$  &  " 
                << (( munuTOP_JESUP_hist->Integral(0, munuTOP_JESUP_hist->GetNbinsX() + 1) + 
                      munuVV_JESUP_hist->Integral(0, munuVV_JESUP_hist->GetNbinsX() + 1) + 
                      munuQCDmultijet_JESUP_hist->Integral(0, munuQCDmultijet_JESUP_hist->GetNbinsX() + 1) ) - 
                    ( munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) + 
                      munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) + 
                      munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) ))*100/
                    ( munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) + 
                      munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) + 
                      munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) ) << " & "
                << (( munuTOP_JESDOWN_hist->Integral(0, munuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                      munuVV_JESDOWN_hist->Integral(0, munuVV_JESDOWN_hist->GetNbinsX() + 1) + 
                      munuQCDmultijet_JESDOWN_hist->Integral(0, munuQCDmultijet_JESDOWN_hist->GetNbinsX() + 1) ) - 
                    ( munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) + 
                      munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) + 
                      munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) ))*100/
                    ( munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) + 
                      munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) + 
                      munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) ) << "\\\\ " << std::endl;

      std::cout << " \\hline" << std::endl;

      std::cout << " N$_\\text{W($\\mu\\nu$)}^\\text{SR}$  &  " 
                << (( ( ( nunuWmunuQCD_JESUP_hist->Integral(0, nunuWmunuQCD_JESUP_hist->GetNbinsX() + 1) + 
                          nunuWmunuEWK_JESUP_hist->Integral(0, nunuWmunuEWK_JESUP_hist->GetNbinsX() + 1) )/( 
                          munuQCD_JESUP_hist->Integral(0, munuQCD_JESUP_hist->GetNbinsX() + 1) + 
                          munuEWK_JESUP_hist->Integral(0, munuEWK_JESUP_hist->GetNbinsX() + 1) ) )*( 
                          munuDATAOBS_hist->Integral(0, munuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          munuTOP_JESUP_hist->Integral(0, munuTOP_JESUP_hist->GetNbinsX() + 1) + 
                          munuVV_JESUP_hist->Integral(0, munuVV_JESUP_hist->GetNbinsX() + 1) + 
                          munuQCDmultijet_JESUP_hist->Integral(0, munuQCDmultijet_JESUP_hist->GetNbinsX() + 1) ) ) ) - 
                    ( ( ( nunuWmunuQCD_hist->Integral(0, nunuWmunuQCD_hist->GetNbinsX() + 1) + 
                          nunuWmunuEWK_hist->Integral(0, nunuWmunuEWK_hist->GetNbinsX() + 1) )/( 
                          munuQCD_hist->Integral(0, munuQCD_hist->GetNbinsX() + 1) + 
                          munuEWK_hist->Integral(0, munuEWK_hist->GetNbinsX() + 1) ) )*( 
                          munuDATAOBS_hist->Integral(0, munuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) + 
                          munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) + 
                          munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) ) ) ))*100/
                    ( ( ( nunuWmunuQCD_hist->Integral(0, nunuWmunuQCD_hist->GetNbinsX() + 1) + 
                          nunuWmunuEWK_hist->Integral(0, nunuWmunuEWK_hist->GetNbinsX() + 1) )/( 
                          munuQCD_hist->Integral(0, munuQCD_hist->GetNbinsX() + 1) + 
                          munuEWK_hist->Integral(0, munuEWK_hist->GetNbinsX() + 1) ) )*( 
                          munuDATAOBS_hist->Integral(0, munuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) + 
                          munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) + 
                          munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) ) ) ) << " & "
                << (( ( ( nunuWmunuQCD_JESDOWN_hist->Integral(0, nunuWmunuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                          nunuWmunuEWK_JESDOWN_hist->Integral(0, nunuWmunuEWK_JESDOWN_hist->GetNbinsX() + 1) )/( 
                          munuQCD_JESDOWN_hist->Integral(0, munuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                          munuEWK_JESDOWN_hist->Integral(0, munuEWK_JESDOWN_hist->GetNbinsX() + 1) ) )*( 
                          munuDATAOBS_hist->Integral(0, munuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          munuTOP_JESDOWN_hist->Integral(0, munuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                          munuVV_JESDOWN_hist->Integral(0, munuVV_JESDOWN_hist->GetNbinsX() + 1) + 
                          munuQCDmultijet_JESDOWN_hist->Integral(0, munuQCDmultijet_JESDOWN_hist->GetNbinsX() + 1) ) ) ) - 
                    ( ( ( nunuWmunuQCD_hist->Integral(0, nunuWmunuQCD_hist->GetNbinsX() + 1) + 
                          nunuWmunuEWK_hist->Integral(0, nunuWmunuEWK_hist->GetNbinsX() + 1) )/( 
                          munuQCD_hist->Integral(0, munuQCD_hist->GetNbinsX() + 1) + 
                          munuEWK_hist->Integral(0, munuEWK_hist->GetNbinsX() + 1) ) )*( 
                          munuDATAOBS_hist->Integral(0, munuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) + 
                          munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) + 
                          munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) ) ) ))*100/
                    ( ( ( nunuWmunuQCD_hist->Integral(0, nunuWmunuQCD_hist->GetNbinsX() + 1) + 
                          nunuWmunuEWK_hist->Integral(0, nunuWmunuEWK_hist->GetNbinsX() + 1) )/( 
                          munuQCD_hist->Integral(0, munuQCD_hist->GetNbinsX() + 1) + 
                          munuEWK_hist->Integral(0, munuEWK_hist->GetNbinsX() + 1) ) )*( 
                          munuDATAOBS_hist->Integral(0, munuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) + 
                          munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) + 
                          munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) ) ) ) << "\\\\ " << std::endl;

      std::cout << " \\hline" << std::endl;

      std::cout << " $\\frac{\\text{N}_\\text{MC}^\\text{SR}}{\\text{N}_\\text{MC}^\\text{CR}}$  &  " 
                << (( ( nunuWmunuQCD_JESUP_hist->Integral(0, nunuWmunuQCD_JESUP_hist->GetNbinsX() + 1) + 
                        nunuWmunuEWK_JESUP_hist->Integral(0, nunuWmunuEWK_JESUP_hist->GetNbinsX() + 1) )/( 
                        munuQCD_JESUP_hist->Integral(0, munuQCD_JESUP_hist->GetNbinsX() + 1) + 
                        munuEWK_JESUP_hist->Integral(0, munuEWK_JESUP_hist->GetNbinsX() + 1) ) ) - 
                    ( ( nunuWmunuQCD_hist->Integral(0, nunuWmunuQCD_hist->GetNbinsX() + 1) + 
                        nunuWmunuEWK_hist->Integral(0, nunuWmunuEWK_hist->GetNbinsX() + 1) )/( 
                        munuQCD_hist->Integral(0, munuQCD_hist->GetNbinsX() + 1) + 
                        munuEWK_hist->Integral(0, munuEWK_hist->GetNbinsX() + 1) ) ))*100/
                    ( ( nunuWmunuQCD_hist->Integral(0, nunuWmunuQCD_hist->GetNbinsX() + 1) + 
                        nunuWmunuEWK_hist->Integral(0, nunuWmunuEWK_hist->GetNbinsX() + 1) )/( 
                        munuQCD_hist->Integral(0, munuQCD_hist->GetNbinsX() + 1) + 
                        munuEWK_hist->Integral(0, munuEWK_hist->GetNbinsX() + 1) ) ) << " & "
                << (( ( nunuWmunuQCD_JESDOWN_hist->Integral(0, nunuWmunuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                        nunuWmunuEWK_JESDOWN_hist->Integral(0, nunuWmunuEWK_JESDOWN_hist->GetNbinsX() + 1) )/( 
                        munuQCD_JESDOWN_hist->Integral(0, munuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                        munuEWK_JESDOWN_hist->Integral(0, munuEWK_JESDOWN_hist->GetNbinsX() + 1) ) ) - 
                    ( ( nunuWmunuQCD_hist->Integral(0, nunuWmunuQCD_hist->GetNbinsX() + 1) + 
                        nunuWmunuEWK_hist->Integral(0, nunuWmunuEWK_hist->GetNbinsX() + 1) )/( 
                        munuQCD_hist->Integral(0, munuQCD_hist->GetNbinsX() + 1) + 
                        munuEWK_hist->Integral(0, munuEWK_hist->GetNbinsX() + 1) ) ))*100/
                    ( ( nunuWmunuQCD_hist->Integral(0, nunuWmunuQCD_hist->GetNbinsX() + 1) + 
                        nunuWmunuEWK_hist->Integral(0, nunuWmunuEWK_hist->GetNbinsX() + 1) )/( 
                        munuQCD_hist->Integral(0, munuQCD_hist->GetNbinsX() + 1) + 
                        munuEWK_hist->Integral(0, munuEWK_hist->GetNbinsX() + 1) ) ) << "\\\\ " << std::endl;

      std::cout << " ( N$_\\text{data\\_obs}^\\text{CR}$ - N$_\\text{backgrounds}^\\text{CR}$)  &  " 
                << ( munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) + 
                    munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) + 
                    munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) -
                  ( munuTOP_JESUP_hist->Integral(0, munuTOP_JESUP_hist->GetNbinsX() + 1) + 
                    munuVV_JESUP_hist->Integral(0, munuVV_JESUP_hist->GetNbinsX() + 1) + 
                    munuQCDmultijet_JESUP_hist->Integral(0, munuQCDmultijet_JESUP_hist->GetNbinsX() + 1) ) )*100/
                  ( munuDATAOBS_hist->Integral(0, munuDATAOBS_hist->GetNbinsX() + 1) - 
                  ( munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) + 
                    munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) + 
                    munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) ) ) << " & "
                << ( munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) + 
                    munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) + 
                    munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) -
                  ( munuTOP_JESDOWN_hist->Integral(0, munuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                    munuVV_JESDOWN_hist->Integral(0, munuVV_JESDOWN_hist->GetNbinsX() + 1) + 
                    munuQCDmultijet_JESDOWN_hist->Integral(0, munuQCDmultijet_JESDOWN_hist->GetNbinsX() + 1) ) )*100/
                  ( munuDATAOBS_hist->Integral(0, munuDATAOBS_hist->GetNbinsX() + 1) - 
                  ( munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) + 
                    munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) + 
                    munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) ) ) << "\\\\ " << std::endl;

      std::cout << std::endl;

      // ****************************************
      std::cout << " *****************************************" << std::endl;
      // ****************************************

      std::cout << " \\textbf{W($\\mu\\nu$)}     &              &  Central             &  JESUP              &  JESDOWN\\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;
      std::cout << " \\multirow{2}*{TOP}       &  Integral    & " 
                << munuTOP_hist->Integral(0, munuTOP_hist->GetNbinsX() + 1) << "              & " 
                << munuTOP_JESUP_hist->Integral(0, munuTOP_JESUP_hist->GetNbinsX() + 1) << "             & "
                << munuTOP_JESDOWN_hist->Integral(0, munuTOP_JESDOWN_hist->GetNbinsX() + 1) << " \\\\ " << std::endl;
      std::cout << "                          &  GetEntries  & " 
                << munuTOP_hist->GetEntries() << "                  & " 
                << munuTOP_JESUP_hist->GetEntries() << "                 & "
                << munuTOP_JESDOWN_hist->GetEntries() << " \\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;
      std::cout << " \\multirow{2}*{VV}       &  Integral    & " 
                << munuVV_hist->Integral(0, munuVV_hist->GetNbinsX() + 1) << "              & " 
                << munuVV_JESUP_hist->Integral(0, munuVV_JESUP_hist->GetNbinsX() + 1) << "             & "
                << munuVV_JESDOWN_hist->Integral(0, munuVV_JESDOWN_hist->GetNbinsX() + 1) << " \\\\ " << std::endl;
      std::cout << "                          &  GetEntries  & " 
                << munuVV_hist->GetEntries() << "                  & " 
                << munuVV_JESUP_hist->GetEntries() << "                 & "
                << munuVV_JESDOWN_hist->GetEntries() << " \\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;
      std::cout << " \\multirow{2}*{QCDmultijet}       &  Integral    & " 
                << munuQCDmultijet_hist->Integral(0, munuQCDmultijet_hist->GetNbinsX() + 1) << "              & " 
                << munuQCDmultijet_JESUP_hist->Integral(0, munuQCDmultijet_JESUP_hist->GetNbinsX() + 1) << "             & "
                << munuQCDmultijet_JESDOWN_hist->Integral(0, munuQCDmultijet_JESDOWN_hist->GetNbinsX() + 1) << " \\\\ " << std::endl;
      std::cout << "                                  &  GetEntries  & " 
                << munuQCDmultijet_hist->GetEntries() << "                  & " 
                << munuQCDmultijet_JESUP_hist->GetEntries() << "                 & "
                << munuQCDmultijet_JESDOWN_hist->GetEntries() << " \\\\ " << std::endl;


      std::cout << std::endl;


      std::cout << std::endl;

      std::cout << " *****************************************" << std::endl;
      std::cout << " ********* Tables for W(taunu) CR *********" << std::endl;
      std::cout << " *****************************************" << std::endl;
      // ****************************************
      // ****************************************
      // ****************************************
      std::cout << " \\textbf{W($\\tau\\nu$)}              &  Central             &  JESUP              &  JESDOWN\\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;

      // ********* Central error *********
      std::vector<double> error_taunu_hist;
      error_taunu_hist.push_back(Error(taunuQCD_hist));
      error_taunu_hist.push_back(Error(taunuEWK_hist));

      std::vector<double> error_nunuWtaunu_hist;
      error_nunuWtaunu_hist.push_back(Error(nunuWtaunuQCD_hist));
      error_nunuWtaunu_hist.push_back(Error(nunuWtaunuEWK_hist));

      std::vector<double> errorBKG_taunu_hist;
      errorBKG_taunu_hist.push_back(Error(taunuTOP_hist));
      errorBKG_taunu_hist.push_back(Error(taunuVV_hist));
      errorBKG_taunu_hist.push_back(Error(taunuQCDmultijet_hist));
      // ********* JESUP error *********
      std::vector<double> error_taunu_JESUP_hist;
      error_taunu_JESUP_hist.push_back(Error(taunuQCD_JESUP_hist));
      error_taunu_JESUP_hist.push_back(Error(taunuEWK_JESUP_hist));

      std::vector<double> error_nunuWtaunu_JESUP_hist;
      error_nunuWtaunu_JESUP_hist.push_back(Error(nunuWtaunuQCD_JESUP_hist));
      error_nunuWtaunu_JESUP_hist.push_back(Error(nunuWtaunuEWK_JESUP_hist));

      std::vector<double> errorBKG_taunu_JESUP_hist;
      errorBKG_taunu_JESUP_hist.push_back(Error(taunuTOP_JESUP_hist));
      errorBKG_taunu_JESUP_hist.push_back(Error(taunuVV_JESUP_hist));
      errorBKG_taunu_JESUP_hist.push_back(Error(taunuQCDmultijet_JESUP_hist));
      // ********* JESDOWN error *********
      std::vector<double> error_taunu_JESDOWN_hist;
      error_taunu_JESDOWN_hist.push_back(Error(taunuQCD_JESDOWN_hist));
      error_taunu_JESDOWN_hist.push_back(Error(taunuEWK_JESDOWN_hist));

      std::vector<double> error_nunuWtaunu_JESDOWN_hist;
      error_nunuWtaunu_JESDOWN_hist.push_back(Error(nunuWtaunuQCD_JESDOWN_hist));
      error_nunuWtaunu_JESDOWN_hist.push_back(Error(nunuWtaunuEWK_JESDOWN_hist));

      std::vector<double> errorBKG_taunu_JESDOWN_hist;
      errorBKG_taunu_JESDOWN_hist.push_back(Error(taunuTOP_JESDOWN_hist));
      errorBKG_taunu_JESDOWN_hist.push_back(Error(taunuVV_JESDOWN_hist));
      errorBKG_taunu_JESDOWN_hist.push_back(Error(taunuQCDmultijet_JESDOWN_hist));

      std::cout << " N$_\\text{MC}^\\text{CR}$           &  " 
                << taunuQCD_hist->Integral(0, taunuQCD_hist->GetNbinsX() + 1) + 
                   taunuEWK_hist->Integral(0, taunuEWK_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_taunu_hist ) << " & "
                << taunuQCD_JESUP_hist->Integral(0, taunuQCD_JESUP_hist->GetNbinsX() + 1) + 
                   taunuEWK_JESUP_hist->Integral(0, taunuEWK_JESUP_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_taunu_JESUP_hist ) << " & "
                << taunuQCD_JESDOWN_hist->Integral(0, taunuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                   taunuEWK_JESDOWN_hist->Integral(0, taunuEWK_JESDOWN_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_taunu_JESDOWN_hist ) 
                << "\\\\ " << std::endl;

      std::cout << " N$_\\text{MC}^\\text{SR}$           &  " 
                << nunuWtaunuQCD_hist->Integral(0, nunuWtaunuQCD_hist->GetNbinsX() + 1) + 
                   nunuWtaunuEWK_hist->Integral(0, nunuWtaunuEWK_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_nunuWtaunu_hist ) << " & "
                << nunuWtaunuQCD_JESUP_hist->Integral(0, nunuWtaunuQCD_JESUP_hist->GetNbinsX() + 1) + 
                   nunuWtaunuEWK_JESUP_hist->Integral(0, nunuWtaunuEWK_JESUP_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_nunuWtaunu_JESUP_hist ) << " & "
                << nunuWtaunuQCD_JESDOWN_hist->Integral(0, nunuWtaunuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                   nunuWtaunuEWK_JESDOWN_hist->Integral(0, nunuWtaunuEWK_JESDOWN_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_nunuWtaunu_JESDOWN_hist ) 
                << "\\\\ " << std::endl;

      std::cout << " N$_\\text{data\\_obs}^\\text{CR}$     &  " 
                << taunuDATAOBS_hist->Integral(0, taunuDATAOBS_hist->GetNbinsX() + 1) << " $\\pm$ " << Error(taunuDATAOBS_hist) << "     & "
                << taunuDATAOBS_hist->Integral(0, taunuDATAOBS_hist->GetNbinsX() + 1) << " $\\pm$ " << Error(taunuDATAOBS_hist) << "     & "
                << taunuDATAOBS_hist->Integral(0, taunuDATAOBS_hist->GetNbinsX() + 1) << " $\\pm$ " << Error(taunuDATAOBS_hist) 
                << "\\\\ " << std::endl;

      std::cout << " N$_\\text{backgrounds}^\\text{CR}$  &  " 
                << taunuTOP_hist->Integral(0, taunuTOP_hist->GetNbinsX() + 1) + 
                   taunuVV_hist->Integral(0, taunuVV_hist->GetNbinsX() + 1) + 
                   taunuQCDmultijet_hist->Integral(0, taunuQCDmultijet_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( errorBKG_taunu_hist ) << " & "
                << taunuTOP_JESUP_hist->Integral(0, taunuTOP_JESUP_hist->GetNbinsX() + 1) + 
                   taunuVV_JESUP_hist->Integral(0, taunuVV_JESUP_hist->GetNbinsX() + 1) + 
                   taunuQCDmultijet_JESUP_hist->Integral(0, taunuQCDmultijet_JESUP_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( errorBKG_taunu_JESUP_hist ) << " & "
                << taunuTOP_JESDOWN_hist->Integral(0, taunuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                   taunuVV_JESDOWN_hist->Integral(0, taunuVV_JESDOWN_hist->GetNbinsX() + 1) + 
                   taunuQCDmultijet_JESDOWN_hist->Integral(0, taunuQCDmultijet_JESDOWN_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( errorBKG_taunu_JESDOWN_hist ) 
                << "\\\\ " << std::endl;

      std::cout << " \\hline" << std::endl;

      std::cout << " N$_\\text{W($\\tau\\nu$)}^\\text{SR}$  &  " 
                << ( ( nunuWtaunuQCD_hist->Integral(0, nunuWtaunuQCD_hist->GetNbinsX() + 1) + 
                       nunuWtaunuEWK_hist->Integral(0, nunuWtaunuEWK_hist->GetNbinsX() + 1) )/( 
                       taunuQCD_hist->Integral(0, taunuQCD_hist->GetNbinsX() + 1) + 
                       taunuEWK_hist->Integral(0, taunuEWK_hist->GetNbinsX() + 1) ) )*( 
                       taunuDATAOBS_hist->Integral(0, taunuDATAOBS_hist->GetNbinsX() + 1) - ( 
                       taunuTOP_hist->Integral(0, taunuTOP_hist->GetNbinsX() + 1) + 
                       taunuVV_hist->Integral(0, taunuVV_hist->GetNbinsX() + 1) + 
                       taunuQCDmultijet_hist->Integral(0, taunuQCDmultijet_hist->GetNbinsX() + 1) ) ) << "             & "
                << ( ( nunuWtaunuQCD_JESUP_hist->Integral(0, nunuWtaunuQCD_JESUP_hist->GetNbinsX() + 1) + 
                       nunuWtaunuEWK_JESUP_hist->Integral(0, nunuWtaunuEWK_JESUP_hist->GetNbinsX() + 1) )/( 
                       taunuQCD_JESUP_hist->Integral(0, taunuQCD_JESUP_hist->GetNbinsX() + 1) + 
                       taunuEWK_JESUP_hist->Integral(0, taunuEWK_JESUP_hist->GetNbinsX() + 1) ) )*( 
                       taunuDATAOBS_hist->Integral(0, taunuDATAOBS_hist->GetNbinsX() + 1) - ( 
                       taunuTOP_JESUP_hist->Integral(0, taunuTOP_JESUP_hist->GetNbinsX() + 1) + 
                       taunuVV_JESUP_hist->Integral(0, taunuVV_JESUP_hist->GetNbinsX() + 1) + 
                       taunuQCDmultijet_JESUP_hist->Integral(0, taunuQCDmultijet_JESUP_hist->GetNbinsX() + 1) ) ) << "             & "
                << ( ( nunuWtaunuQCD_JESDOWN_hist->Integral(0, nunuWtaunuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                       nunuWtaunuEWK_JESDOWN_hist->Integral(0, nunuWtaunuEWK_JESDOWN_hist->GetNbinsX() + 1) )/( 
                       taunuQCD_JESDOWN_hist->Integral(0, taunuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                       taunuEWK_JESDOWN_hist->Integral(0, taunuEWK_JESDOWN_hist->GetNbinsX() + 1) ) )*( 
                       taunuDATAOBS_hist->Integral(0, taunuDATAOBS_hist->GetNbinsX() + 1) - ( 
                       taunuTOP_JESDOWN_hist->Integral(0, taunuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                       taunuVV_JESDOWN_hist->Integral(0, taunuVV_JESDOWN_hist->GetNbinsX() + 1) + 
                       taunuQCDmultijet_JESDOWN_hist->Integral(0, taunuQCDmultijet_JESDOWN_hist->GetNbinsX() + 1) ) )
                << "\\\\ " << std::endl;
      std::cout << std::endl;

      // ****************************************
      std::cout << " *****************************************" << std::endl;
      // ****************************************

      std::cout << " \\textbf{W($\\tau\\nu$)}             &  JESUP\\_Impact (\\%)              &  JESDOWN\\_Impact (\\%)\\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;

      std::cout << " N$_\\text{MC}^\\text{CR}$           &  " 
                << (( taunuQCD_JESUP_hist->Integral(0, taunuQCD_JESUP_hist->GetNbinsX() + 1) + 
                      taunuEWK_JESUP_hist->Integral(0, taunuEWK_JESUP_hist->GetNbinsX() + 1) ) - 
                    ( taunuQCD_hist->Integral(0, taunuQCD_hist->GetNbinsX() + 1) + 
                      taunuEWK_hist->Integral(0, taunuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( taunuQCD_hist->Integral(0, taunuQCD_hist->GetNbinsX() + 1) + 
                      taunuEWK_hist->Integral(0, taunuEWK_hist->GetNbinsX() + 1) ) << " & "
                << (( taunuQCD_JESDOWN_hist->Integral(0, taunuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                      taunuEWK_JESDOWN_hist->Integral(0, taunuEWK_JESDOWN_hist->GetNbinsX() + 1) ) - 
                    ( taunuQCD_hist->Integral(0, taunuQCD_hist->GetNbinsX() + 1) + 
                      taunuEWK_hist->Integral(0, taunuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( taunuQCD_hist->Integral(0, taunuQCD_hist->GetNbinsX() + 1) + 
                      taunuEWK_hist->Integral(0, taunuEWK_hist->GetNbinsX() + 1) ) << "\\\\ " << std::endl;

      std::cout << " N$_\\text{MC}^\\text{SR}$           &  " 
                << (( nunuWtaunuQCD_JESUP_hist->Integral(0, nunuWtaunuQCD_JESUP_hist->GetNbinsX() + 1) + 
                      nunuWtaunuEWK_JESUP_hist->Integral(0, nunuWtaunuEWK_JESUP_hist->GetNbinsX() + 1) ) - 
                    ( nunuWtaunuQCD_hist->Integral(0, nunuWtaunuQCD_hist->GetNbinsX() + 1) + 
                      nunuWtaunuEWK_hist->Integral(0, nunuWtaunuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( nunuWtaunuQCD_hist->Integral(0, nunuWtaunuQCD_hist->GetNbinsX() + 1) + 
                      nunuWtaunuEWK_hist->Integral(0, nunuWtaunuEWK_hist->GetNbinsX() + 1) ) << " & "
                << (( nunuWtaunuQCD_JESDOWN_hist->Integral(0, nunuWtaunuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                      nunuWtaunuEWK_JESDOWN_hist->Integral(0, nunuWtaunuEWK_JESDOWN_hist->GetNbinsX() + 1) ) - 
                    ( nunuWtaunuQCD_hist->Integral(0, nunuWtaunuQCD_hist->GetNbinsX() + 1) + 
                      nunuWtaunuEWK_hist->Integral(0, nunuWtaunuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( nunuWtaunuQCD_hist->Integral(0, nunuWtaunuQCD_hist->GetNbinsX() + 1) + 
                      nunuWtaunuEWK_hist->Integral(0, nunuWtaunuEWK_hist->GetNbinsX() + 1) ) << "\\\\ " << std::endl;

      std::cout << " N$_\\text{data\\_obs}^\\text{CR}$     &  " 
                << " 0 " << "     & "
                << " 0 " << "\\\\ " << std::endl;

      std::cout << " N$_\\text{backgrounds}^\\text{CR}$  &  " 
                << (( taunuTOP_JESUP_hist->Integral(0, taunuTOP_JESUP_hist->GetNbinsX() + 1) + 
                      taunuVV_JESUP_hist->Integral(0, taunuVV_JESUP_hist->GetNbinsX() + 1) + 
                      taunuQCDmultijet_JESUP_hist->Integral(0, taunuQCDmultijet_JESUP_hist->GetNbinsX() + 1) ) - 
                    ( taunuTOP_hist->Integral(0, taunuTOP_hist->GetNbinsX() + 1) + 
                      taunuVV_hist->Integral(0, taunuVV_hist->GetNbinsX() + 1) + 
                      taunuQCDmultijet_hist->Integral(0, taunuQCDmultijet_hist->GetNbinsX() + 1) ))*100/
                    ( taunuTOP_hist->Integral(0, taunuTOP_hist->GetNbinsX() + 1) + 
                      taunuVV_hist->Integral(0, taunuVV_hist->GetNbinsX() + 1) + 
                      taunuQCDmultijet_hist->Integral(0, taunuQCDmultijet_hist->GetNbinsX() + 1) ) << " & "
                << (( taunuTOP_JESDOWN_hist->Integral(0, taunuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                      taunuVV_JESDOWN_hist->Integral(0, taunuVV_JESDOWN_hist->GetNbinsX() + 1) + 
                      taunuQCDmultijet_JESDOWN_hist->Integral(0, taunuQCDmultijet_JESDOWN_hist->GetNbinsX() + 1) ) - 
                    ( taunuTOP_hist->Integral(0, taunuTOP_hist->GetNbinsX() + 1) + 
                      taunuVV_hist->Integral(0, taunuVV_hist->GetNbinsX() + 1) + 
                      taunuQCDmultijet_hist->Integral(0, taunuQCDmultijet_hist->GetNbinsX() + 1) ))*100/
                    ( taunuTOP_hist->Integral(0, taunuTOP_hist->GetNbinsX() + 1) + 
                      taunuVV_hist->Integral(0, taunuVV_hist->GetNbinsX() + 1) + 
                      taunuQCDmultijet_hist->Integral(0, taunuQCDmultijet_hist->GetNbinsX() + 1) ) << "\\\\ " << std::endl;

      std::cout << " \\hline" << std::endl;

      std::cout << " N$_\\text{W($\\tau\\nu$)}^\\text{SR}$  &  " 
                << (( ( ( nunuWtaunuQCD_JESUP_hist->Integral(0, nunuWtaunuQCD_JESUP_hist->GetNbinsX() + 1) + 
                          nunuWtaunuEWK_JESUP_hist->Integral(0, nunuWtaunuEWK_JESUP_hist->GetNbinsX() + 1) )/( 
                          taunuQCD_JESUP_hist->Integral(0, taunuQCD_JESUP_hist->GetNbinsX() + 1) + 
                          taunuEWK_JESUP_hist->Integral(0, taunuEWK_JESUP_hist->GetNbinsX() + 1) ) )*( 
                          taunuDATAOBS_hist->Integral(0, taunuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          taunuTOP_JESUP_hist->Integral(0, taunuTOP_JESUP_hist->GetNbinsX() + 1) + 
                          taunuVV_JESUP_hist->Integral(0, taunuVV_JESUP_hist->GetNbinsX() + 1) + 
                          taunuQCDmultijet_JESUP_hist->Integral(0, taunuQCDmultijet_JESUP_hist->GetNbinsX() + 1) ) ) ) - 
                    ( ( ( nunuWtaunuQCD_hist->Integral(0, nunuWtaunuQCD_hist->GetNbinsX() + 1) + 
                          nunuWtaunuEWK_hist->Integral(0, nunuWtaunuEWK_hist->GetNbinsX() + 1) )/( 
                          taunuQCD_hist->Integral(0, taunuQCD_hist->GetNbinsX() + 1) + 
                          taunuEWK_hist->Integral(0, taunuEWK_hist->GetNbinsX() + 1) ) )*( 
                          taunuDATAOBS_hist->Integral(0, taunuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          taunuTOP_hist->Integral(0, taunuTOP_hist->GetNbinsX() + 1) + 
                          taunuVV_hist->Integral(0, taunuVV_hist->GetNbinsX() + 1) + 
                          taunuQCDmultijet_hist->Integral(0, taunuQCDmultijet_hist->GetNbinsX() + 1) ) ) ))*100/
                    ( ( ( nunuWtaunuQCD_hist->Integral(0, nunuWtaunuQCD_hist->GetNbinsX() + 1) + 
                          nunuWtaunuEWK_hist->Integral(0, nunuWtaunuEWK_hist->GetNbinsX() + 1) )/( 
                          taunuQCD_hist->Integral(0, taunuQCD_hist->GetNbinsX() + 1) + 
                          taunuEWK_hist->Integral(0, taunuEWK_hist->GetNbinsX() + 1) ) )*( 
                          taunuDATAOBS_hist->Integral(0, taunuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          taunuTOP_hist->Integral(0, taunuTOP_hist->GetNbinsX() + 1) + 
                          taunuVV_hist->Integral(0, taunuVV_hist->GetNbinsX() + 1) + 
                          taunuQCDmultijet_hist->Integral(0, taunuQCDmultijet_hist->GetNbinsX() + 1) ) ) ) << " & "
                << (( ( ( nunuWtaunuQCD_JESDOWN_hist->Integral(0, nunuWtaunuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                          nunuWtaunuEWK_JESDOWN_hist->Integral(0, nunuWtaunuEWK_JESDOWN_hist->GetNbinsX() + 1) )/( 
                          taunuQCD_JESDOWN_hist->Integral(0, taunuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                          taunuEWK_JESDOWN_hist->Integral(0, taunuEWK_JESDOWN_hist->GetNbinsX() + 1) ) )*( 
                          taunuDATAOBS_hist->Integral(0, taunuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          taunuTOP_JESDOWN_hist->Integral(0, taunuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                          taunuVV_JESDOWN_hist->Integral(0, taunuVV_JESDOWN_hist->GetNbinsX() + 1) + 
                          taunuQCDmultijet_JESDOWN_hist->Integral(0, taunuQCDmultijet_JESDOWN_hist->GetNbinsX() + 1) ) ) ) - 
                    ( ( ( nunuWtaunuQCD_hist->Integral(0, nunuWtaunuQCD_hist->GetNbinsX() + 1) + 
                          nunuWtaunuEWK_hist->Integral(0, nunuWtaunuEWK_hist->GetNbinsX() + 1) )/( 
                          taunuQCD_hist->Integral(0, taunuQCD_hist->GetNbinsX() + 1) + 
                          taunuEWK_hist->Integral(0, taunuEWK_hist->GetNbinsX() + 1) ) )*( 
                          taunuDATAOBS_hist->Integral(0, taunuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          taunuTOP_hist->Integral(0, taunuTOP_hist->GetNbinsX() + 1) + 
                          taunuVV_hist->Integral(0, taunuVV_hist->GetNbinsX() + 1) + 
                          taunuQCDmultijet_hist->Integral(0, taunuQCDmultijet_hist->GetNbinsX() + 1) ) ) ))*100/
                    ( ( ( nunuWtaunuQCD_hist->Integral(0, nunuWtaunuQCD_hist->GetNbinsX() + 1) + 
                          nunuWtaunuEWK_hist->Integral(0, nunuWtaunuEWK_hist->GetNbinsX() + 1) )/( 
                          taunuQCD_hist->Integral(0, taunuQCD_hist->GetNbinsX() + 1) + 
                          taunuEWK_hist->Integral(0, taunuEWK_hist->GetNbinsX() + 1) ) )*( 
                          taunuDATAOBS_hist->Integral(0, taunuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          taunuTOP_hist->Integral(0, taunuTOP_hist->GetNbinsX() + 1) + 
                          taunuVV_hist->Integral(0, taunuVV_hist->GetNbinsX() + 1) + 
                          taunuQCDmultijet_hist->Integral(0, taunuQCDmultijet_hist->GetNbinsX() + 1) ) ) ) << "\\\\ " << std::endl;

      std::cout << " \\hline" << std::endl;

      std::cout << " $\\frac{\\text{N}_\\text{MC}^\\text{SR}}{\\text{N}_\\text{MC}^\\text{CR}}$  &  " 
                << (( ( nunuWtaunuQCD_JESUP_hist->Integral(0, nunuWtaunuQCD_JESUP_hist->GetNbinsX() + 1) + 
                        nunuWtaunuEWK_JESUP_hist->Integral(0, nunuWtaunuEWK_JESUP_hist->GetNbinsX() + 1) )/( 
                        taunuQCD_JESUP_hist->Integral(0, taunuQCD_JESUP_hist->GetNbinsX() + 1) + 
                        taunuEWK_JESUP_hist->Integral(0, taunuEWK_JESUP_hist->GetNbinsX() + 1) ) ) - 
                    ( ( nunuWtaunuQCD_hist->Integral(0, nunuWtaunuQCD_hist->GetNbinsX() + 1) + 
                        nunuWtaunuEWK_hist->Integral(0, nunuWtaunuEWK_hist->GetNbinsX() + 1) )/( 
                        taunuQCD_hist->Integral(0, taunuQCD_hist->GetNbinsX() + 1) + 
                        taunuEWK_hist->Integral(0, taunuEWK_hist->GetNbinsX() + 1) ) ))*100/
                    ( ( nunuWtaunuQCD_hist->Integral(0, nunuWtaunuQCD_hist->GetNbinsX() + 1) + 
                        nunuWtaunuEWK_hist->Integral(0, nunuWtaunuEWK_hist->GetNbinsX() + 1) )/( 
                        taunuQCD_hist->Integral(0, taunuQCD_hist->GetNbinsX() + 1) + 
                        taunuEWK_hist->Integral(0, taunuEWK_hist->GetNbinsX() + 1) ) ) << " & "
                << (( ( nunuWtaunuQCD_JESDOWN_hist->Integral(0, nunuWtaunuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                        nunuWtaunuEWK_JESDOWN_hist->Integral(0, nunuWtaunuEWK_JESDOWN_hist->GetNbinsX() + 1) )/( 
                        taunuQCD_JESDOWN_hist->Integral(0, taunuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                        taunuEWK_JESDOWN_hist->Integral(0, taunuEWK_JESDOWN_hist->GetNbinsX() + 1) ) ) - 
                    ( ( nunuWtaunuQCD_hist->Integral(0, nunuWtaunuQCD_hist->GetNbinsX() + 1) + 
                        nunuWtaunuEWK_hist->Integral(0, nunuWtaunuEWK_hist->GetNbinsX() + 1) )/( 
                        taunuQCD_hist->Integral(0, taunuQCD_hist->GetNbinsX() + 1) + 
                        taunuEWK_hist->Integral(0, taunuEWK_hist->GetNbinsX() + 1) ) ))*100/
                    ( ( nunuWtaunuQCD_hist->Integral(0, nunuWtaunuQCD_hist->GetNbinsX() + 1) + 
                        nunuWtaunuEWK_hist->Integral(0, nunuWtaunuEWK_hist->GetNbinsX() + 1) )/( 
                        taunuQCD_hist->Integral(0, taunuQCD_hist->GetNbinsX() + 1) + 
                        taunuEWK_hist->Integral(0, taunuEWK_hist->GetNbinsX() + 1) ) ) << "\\\\ " << std::endl;

      std::cout << " ( N$_\\text{data\\_obs}^\\text{CR}$ - N$_\\text{backgrounds}^\\text{CR}$)  &  " 
                << ( taunuTOP_hist->Integral(0, taunuTOP_hist->GetNbinsX() + 1) + 
                     taunuVV_hist->Integral(0, taunuVV_hist->GetNbinsX() + 1) + 
                     taunuQCDmultijet_hist->Integral(0, taunuQCDmultijet_hist->GetNbinsX() + 1) -
                   ( taunuTOP_JESUP_hist->Integral(0, taunuTOP_JESUP_hist->GetNbinsX() + 1) + 
                     taunuVV_JESUP_hist->Integral(0, taunuVV_JESUP_hist->GetNbinsX() + 1) + 
                     taunuQCDmultijet_JESUP_hist->Integral(0, taunuQCDmultijet_JESUP_hist->GetNbinsX() + 1) ) )*100/
                   ( taunuDATAOBS_hist->Integral(0, taunuDATAOBS_hist->GetNbinsX() + 1) - 
                   ( taunuTOP_hist->Integral(0, taunuTOP_hist->GetNbinsX() + 1) + 
                     taunuVV_hist->Integral(0, taunuVV_hist->GetNbinsX() + 1) + 
                     taunuQCDmultijet_hist->Integral(0, taunuQCDmultijet_hist->GetNbinsX() + 1) ) ) << " & "
                << ( taunuTOP_hist->Integral(0, taunuTOP_hist->GetNbinsX() + 1) + 
                     taunuVV_hist->Integral(0, taunuVV_hist->GetNbinsX() + 1) + 
                     taunuQCDmultijet_hist->Integral(0, taunuQCDmultijet_hist->GetNbinsX() + 1) -
                   ( taunuTOP_JESDOWN_hist->Integral(0, taunuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                     taunuVV_JESDOWN_hist->Integral(0, taunuVV_JESDOWN_hist->GetNbinsX() + 1) + 
                     taunuQCDmultijet_JESDOWN_hist->Integral(0, taunuQCDmultijet_JESDOWN_hist->GetNbinsX() + 1) ) )*100/
                   ( taunuDATAOBS_hist->Integral(0, taunuDATAOBS_hist->GetNbinsX() + 1) - 
                   ( taunuTOP_hist->Integral(0, taunuTOP_hist->GetNbinsX() + 1) + 
                     taunuVV_hist->Integral(0, taunuVV_hist->GetNbinsX() + 1) + 
                     taunuQCDmultijet_hist->Integral(0, taunuQCDmultijet_hist->GetNbinsX() + 1) ) ) << "\\\\ " << std::endl;

      std::cout << std::endl;

      // ****************************************
      std::cout << " *****************************************" << std::endl;
      // ****************************************

      std::cout << " \\textbf{W($\\tau\\nu$)}     &              &  Central             &  JESUP              &  JESDOWN\\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;
      std::cout << " \\multirow{2}*{TOP}       &  Integral    & " 
                << taunuTOP_hist->Integral(0, taunuTOP_hist->GetNbinsX() + 1) << "              & " 
                << taunuTOP_JESUP_hist->Integral(0, taunuTOP_JESUP_hist->GetNbinsX() + 1) << "             & "
                << taunuTOP_JESDOWN_hist->Integral(0, taunuTOP_JESDOWN_hist->GetNbinsX() + 1) << " \\\\ " << std::endl;
      std::cout << "                          &  GetEntries  & " 
                << taunuTOP_hist->GetEntries() << "                  & " 
                << taunuTOP_JESUP_hist->GetEntries() << "                 & "
                << taunuTOP_JESDOWN_hist->GetEntries() << " \\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;
      std::cout << " \\multirow{2}*{VV}       &  Integral    & " 
                << taunuVV_hist->Integral(0, taunuVV_hist->GetNbinsX() + 1) << "              & " 
                << taunuVV_JESUP_hist->Integral(0, taunuVV_JESUP_hist->GetNbinsX() + 1) << "             & "
                << taunuVV_JESDOWN_hist->Integral(0, taunuVV_JESDOWN_hist->GetNbinsX() + 1) << " \\\\ " << std::endl;
      std::cout << "                          &  GetEntries  & " 
                << taunuVV_hist->GetEntries() << "                  & " 
                << taunuVV_JESUP_hist->GetEntries() << "                 & "
                << taunuVV_JESDOWN_hist->GetEntries() << " \\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;
      std::cout << " \\multirow{2}*{QCDmultijet}       &  Integral    & " 
                << taunuQCDmultijet_hist->Integral(0, taunuQCDmultijet_hist->GetNbinsX() + 1) << "              & " 
                << taunuQCDmultijet_JESUP_hist->Integral(0, taunuQCDmultijet_JESUP_hist->GetNbinsX() + 1) << "             & "
                << taunuQCDmultijet_JESDOWN_hist->Integral(0, taunuQCDmultijet_JESDOWN_hist->GetNbinsX() + 1) << " \\\\ " << std::endl;
      std::cout << "                                  &  GetEntries  & " 
                << taunuQCDmultijet_hist->GetEntries() << "                  & " 
                << taunuQCDmultijet_JESUP_hist->GetEntries() << "                 & "
                << taunuQCDmultijet_JESDOWN_hist->GetEntries() << " \\\\ " << std::endl;


      std::cout << std::endl;


      std::cout << std::endl;

      std::cout << " ****************************************" << std::endl;
      std::cout << " ********* Tables for Z(mumu) CR *********" << std::endl;
      std::cout << " ****************************************" << std::endl;
      // ****************************************
      // ****************************************
      // ****************************************
      std::cout << " \\textbf{Z($\\mu\\mu$)}              &  Central             &  JESUP              &  JESDOWN\\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;

      // ********* Central error *********
      std::vector<double> error_mumu_hist;
      error_mumu_hist.push_back(Error(mumuQCD_hist));
      error_mumu_hist.push_back(Error(mumuEWK_hist));

      std::vector<double> error_nunuZmumu_hist;
      error_nunuZmumu_hist.push_back(Error(nunuZmumuQCD_hist));
      error_nunuZmumu_hist.push_back(Error(nunuZmumuEWK_hist));

      std::vector<double> errorBKG_mumu_hist;
      errorBKG_mumu_hist.push_back(Error(mumuTOP_hist));
      errorBKG_mumu_hist.push_back(Error(mumuVV_hist));
      // ********* JESUP error *********
      std::vector<double> error_mumu_JESUP_hist;
      error_mumu_JESUP_hist.push_back(Error(mumuQCD_JESUP_hist));
      error_mumu_JESUP_hist.push_back(Error(mumuEWK_JESUP_hist));

      std::vector<double> error_nunuZmumu_JESUP_hist;
      error_nunuZmumu_JESUP_hist.push_back(Error(nunuZmumuQCD_JESUP_hist));
      error_nunuZmumu_JESUP_hist.push_back(Error(nunuZmumuEWK_JESUP_hist));

      std::vector<double> errorBKG_mumu_JESUP_hist;
      errorBKG_mumu_JESUP_hist.push_back(Error(mumuTOP_JESUP_hist));
      errorBKG_mumu_JESUP_hist.push_back(Error(mumuVV_JESUP_hist));
      // ********* JESDOWN error *********
      std::vector<double> error_mumu_JESDOWN_hist;
      error_mumu_JESDOWN_hist.push_back(Error(mumuQCD_JESDOWN_hist));
      error_mumu_JESDOWN_hist.push_back(Error(mumuEWK_JESDOWN_hist));

      std::vector<double> error_nunuZmumu_JESDOWN_hist;
      error_nunuZmumu_JESDOWN_hist.push_back(Error(nunuZmumuQCD_JESDOWN_hist));
      error_nunuZmumu_JESDOWN_hist.push_back(Error(nunuZmumuEWK_JESDOWN_hist));

      std::vector<double> errorBKG_mumu_JESDOWN_hist;
      errorBKG_mumu_JESDOWN_hist.push_back(Error(mumuTOP_JESDOWN_hist));
      errorBKG_mumu_JESDOWN_hist.push_back(Error(mumuVV_JESDOWN_hist));

      std::cout << " N$_\\text{MC}^\\text{CR}$           &  " 
                << mumuQCD_hist->Integral(0, mumuQCD_hist->GetNbinsX() + 1) + 
                   mumuEWK_hist->Integral(0, mumuEWK_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_mumu_hist ) << " & "
                << mumuQCD_JESUP_hist->Integral(0, mumuQCD_JESUP_hist->GetNbinsX() + 1) + 
                   mumuEWK_JESUP_hist->Integral(0, mumuEWK_JESUP_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_mumu_JESUP_hist ) << " & "
                << mumuQCD_JESDOWN_hist->Integral(0, mumuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                   mumuEWK_JESDOWN_hist->Integral(0, mumuEWK_JESDOWN_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_mumu_JESDOWN_hist ) 
                << "\\\\ " << std::endl;

      std::cout << " N$_\\text{MC}^\\text{SR}$           &  " 
                << nunuZmumuQCD_hist->Integral(0, nunuZmumuQCD_hist->GetNbinsX() + 1) + 
                   nunuZmumuEWK_hist->Integral(0, nunuZmumuEWK_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_nunuZmumu_hist ) << " & "
                << nunuZmumuQCD_JESUP_hist->Integral(0, nunuZmumuQCD_JESUP_hist->GetNbinsX() + 1) + 
                   nunuZmumuEWK_JESUP_hist->Integral(0, nunuZmumuEWK_JESUP_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_nunuZmumu_JESUP_hist ) << " & "
                << nunuZmumuQCD_JESDOWN_hist->Integral(0, nunuZmumuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                   nunuZmumuEWK_JESDOWN_hist->Integral(0, nunuZmumuEWK_JESDOWN_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( error_nunuZmumu_JESDOWN_hist ) 
                << "\\\\ " << std::endl;

      std::cout << " N$_\\text{data\\_obs}^\\text{CR}$     &  " 
                << mumuDATAOBS_hist->Integral(0, mumuDATAOBS_hist->GetNbinsX() + 1) << " $\\pm$ " << Error(mumuDATAOBS_hist) << "     & "
                << mumuDATAOBS_hist->Integral(0, mumuDATAOBS_hist->GetNbinsX() + 1) << " $\\pm$ " << Error(mumuDATAOBS_hist) << "     & "
                << mumuDATAOBS_hist->Integral(0, mumuDATAOBS_hist->GetNbinsX() + 1) << " $\\pm$ " << Error(mumuDATAOBS_hist) 
                << "\\\\ " << std::endl;

      std::cout << " N$_\\text{backgrounds}^\\text{CR}$  &  " 
                << mumuTOP_hist->Integral(0, mumuTOP_hist->GetNbinsX() + 1) + 
                   mumuVV_hist->Integral(0, mumuVV_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( errorBKG_mumu_hist ) << " & "
                << mumuTOP_JESUP_hist->Integral(0, mumuTOP_JESUP_hist->GetNbinsX() + 1) + 
                   mumuVV_JESUP_hist->Integral(0, mumuVV_JESUP_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( errorBKG_mumu_JESUP_hist ) << " & "
                << mumuTOP_JESDOWN_hist->Integral(0, mumuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                   mumuVV_JESDOWN_hist->Integral(0, mumuVV_JESDOWN_hist->GetNbinsX() + 1) 
                << " $\\pm$ " << quadrature( errorBKG_mumu_JESDOWN_hist ) 
                << "\\\\ " << std::endl;

      std::cout << " \\hline" << std::endl;

      std::cout << " N$_\\text{Z($\\mu\\mu$)}^\\text{SR}$  &  " 
                << ( ( nunuZmumuQCD_hist->Integral(0, nunuZmumuQCD_hist->GetNbinsX() + 1) + 
                       nunuZmumuEWK_hist->Integral(0, nunuZmumuEWK_hist->GetNbinsX() + 1) )/( 
                       mumuQCD_hist->Integral(0, mumuQCD_hist->GetNbinsX() + 1) + 
                       mumuEWK_hist->Integral(0, mumuEWK_hist->GetNbinsX() + 1) ) )*( 
                       mumuDATAOBS_hist->Integral(0, mumuDATAOBS_hist->GetNbinsX() + 1) - ( 
                       mumuTOP_hist->Integral(0, mumuTOP_hist->GetNbinsX() + 1) + 
                       mumuVV_hist->Integral(0, mumuVV_hist->GetNbinsX() + 1) ) ) << "             & "
                << ( ( nunuZmumuQCD_JESUP_hist->Integral(0, nunuZmumuQCD_JESUP_hist->GetNbinsX() + 1) + 
                       nunuZmumuEWK_JESUP_hist->Integral(0, nunuZmumuEWK_JESUP_hist->GetNbinsX() + 1) )/( 
                       mumuQCD_JESUP_hist->Integral(0, mumuQCD_JESUP_hist->GetNbinsX() + 1) + 
                       mumuEWK_JESUP_hist->Integral(0, mumuEWK_JESUP_hist->GetNbinsX() + 1) ) )*( 
                       mumuDATAOBS_hist->Integral(0, mumuDATAOBS_hist->GetNbinsX() + 1) - ( 
                       mumuTOP_JESUP_hist->Integral(0, mumuTOP_JESUP_hist->GetNbinsX() + 1) + 
                       mumuVV_JESUP_hist->Integral(0, mumuVV_JESUP_hist->GetNbinsX() + 1) ) ) << "             & "
                << ( ( nunuZmumuQCD_JESDOWN_hist->Integral(0, nunuZmumuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                       nunuZmumuEWK_JESDOWN_hist->Integral(0, nunuZmumuEWK_JESDOWN_hist->GetNbinsX() + 1) )/( 
                       mumuQCD_JESDOWN_hist->Integral(0, mumuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                       mumuEWK_JESDOWN_hist->Integral(0, mumuEWK_JESDOWN_hist->GetNbinsX() + 1) ) )*( 
                       mumuDATAOBS_hist->Integral(0, mumuDATAOBS_hist->GetNbinsX() + 1) - ( 
                       mumuTOP_JESDOWN_hist->Integral(0, mumuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                       mumuVV_JESDOWN_hist->Integral(0, mumuVV_JESDOWN_hist->GetNbinsX() + 1) ) )
                << "\\\\ " << std::endl;
      std::cout << std::endl;

      // ****************************************
      std::cout << " *****************************************" << std::endl;
      // ****************************************

      std::cout << " \\textbf{Z($\\mu\\mu$)}             &  JESUP\\_Impact (\\%)              &  JESDOWN\\_Impact (\\%)\\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;

      std::cout << " N$_\\text{MC}^\\text{CR}$           &  " 
                << (( mumuQCD_JESUP_hist->Integral(0, mumuQCD_JESUP_hist->GetNbinsX() + 1) + 
                      mumuEWK_JESUP_hist->Integral(0, mumuEWK_JESUP_hist->GetNbinsX() + 1) ) - 
                    ( mumuQCD_hist->Integral(0, mumuQCD_hist->GetNbinsX() + 1) + 
                      mumuEWK_hist->Integral(0, mumuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( mumuQCD_hist->Integral(0, mumuQCD_hist->GetNbinsX() + 1) + 
                      mumuEWK_hist->Integral(0, mumuEWK_hist->GetNbinsX() + 1) ) << " & "
                << (( mumuQCD_JESDOWN_hist->Integral(0, mumuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                      mumuEWK_JESDOWN_hist->Integral(0, mumuEWK_JESDOWN_hist->GetNbinsX() + 1) ) - 
                    ( mumuQCD_hist->Integral(0, mumuQCD_hist->GetNbinsX() + 1) + 
                      mumuEWK_hist->Integral(0, mumuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( mumuQCD_hist->Integral(0, mumuQCD_hist->GetNbinsX() + 1) + 
                      mumuEWK_hist->Integral(0, mumuEWK_hist->GetNbinsX() + 1) ) << "\\\\ " << std::endl;

      std::cout << " N$_\\text{MC}^\\text{SR}$           &  " 
                << (( nunuZmumuQCD_JESUP_hist->Integral(0, nunuZmumuQCD_JESUP_hist->GetNbinsX() + 1) + 
                      nunuZmumuEWK_JESUP_hist->Integral(0, nunuZmumuEWK_JESUP_hist->GetNbinsX() + 1) ) - 
                    ( nunuZmumuQCD_hist->Integral(0, nunuZmumuQCD_hist->GetNbinsX() + 1) + 
                      nunuZmumuEWK_hist->Integral(0, nunuZmumuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( nunuZmumuQCD_hist->Integral(0, nunuZmumuQCD_hist->GetNbinsX() + 1) + 
                      nunuZmumuEWK_hist->Integral(0, nunuZmumuEWK_hist->GetNbinsX() + 1) ) << " & "
                << (( nunuZmumuQCD_JESDOWN_hist->Integral(0, nunuZmumuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                      nunuZmumuEWK_JESDOWN_hist->Integral(0, nunuZmumuEWK_JESDOWN_hist->GetNbinsX() + 1) ) - 
                    ( nunuZmumuQCD_hist->Integral(0, nunuZmumuQCD_hist->GetNbinsX() + 1) + 
                      nunuZmumuEWK_hist->Integral(0, nunuZmumuEWK_hist->GetNbinsX() + 1) ))*100/
                    ( nunuZmumuQCD_hist->Integral(0, nunuZmumuQCD_hist->GetNbinsX() + 1) + 
                      nunuZmumuEWK_hist->Integral(0, nunuZmumuEWK_hist->GetNbinsX() + 1) ) << "\\\\ " << std::endl;

      std::cout << " N$_\\text{data\\_obs}^\\text{CR}$     &  " 
                << " 0 " << "     & "
                << " 0 " << "\\\\ " << std::endl;

      std::cout << " N$_\\text{backgrounds}^\\text{CR}$  &  " 
                << (( mumuTOP_JESUP_hist->Integral(0, mumuTOP_JESUP_hist->GetNbinsX() + 1) + 
                      mumuVV_JESUP_hist->Integral(0, mumuVV_JESUP_hist->GetNbinsX() + 1) ) - 
                    ( mumuTOP_hist->Integral(0, mumuTOP_hist->GetNbinsX() + 1) + 
                      mumuVV_hist->Integral(0, mumuVV_hist->GetNbinsX() + 1) ))*100/
                    ( mumuTOP_hist->Integral(0, mumuTOP_hist->GetNbinsX() + 1) + 
                      mumuVV_hist->Integral(0, mumuVV_hist->GetNbinsX() + 1) ) << " & "
                << (( mumuTOP_JESDOWN_hist->Integral(0, mumuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                      mumuVV_JESDOWN_hist->Integral(0, mumuVV_JESDOWN_hist->GetNbinsX() + 1) ) - 
                    ( mumuTOP_hist->Integral(0, mumuTOP_hist->GetNbinsX() + 1) + 
                      mumuVV_hist->Integral(0, mumuVV_hist->GetNbinsX() + 1) ))*100/
                    ( mumuTOP_hist->Integral(0, mumuTOP_hist->GetNbinsX() + 1) + 
                      mumuVV_hist->Integral(0, mumuVV_hist->GetNbinsX() + 1) ) << "\\\\ " << std::endl;

      std::cout << " \\hline" << std::endl;

      std::cout << " N$_\\text{Z($\\mu\\mu$)}^\\text{SR}$  &  " 
                << (( ( ( nunuZmumuQCD_JESUP_hist->Integral(0, nunuZmumuQCD_JESUP_hist->GetNbinsX() + 1) + 
                          nunuZmumuEWK_JESUP_hist->Integral(0, nunuZmumuEWK_JESUP_hist->GetNbinsX() + 1) )/( 
                          mumuQCD_JESUP_hist->Integral(0, mumuQCD_JESUP_hist->GetNbinsX() + 1) + 
                          mumuEWK_JESUP_hist->Integral(0, mumuEWK_JESUP_hist->GetNbinsX() + 1) ) )*( 
                          mumuDATAOBS_hist->Integral(0, mumuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          mumuTOP_JESUP_hist->Integral(0, mumuTOP_JESUP_hist->GetNbinsX() + 1) + 
                          mumuVV_JESUP_hist->Integral(0, mumuVV_JESUP_hist->GetNbinsX() + 1) ) ) ) - 
                    ( ( ( nunuZmumuQCD_hist->Integral(0, nunuZmumuQCD_hist->GetNbinsX() + 1) + 
                          nunuZmumuEWK_hist->Integral(0, nunuZmumuEWK_hist->GetNbinsX() + 1) )/( 
                          mumuQCD_hist->Integral(0, mumuQCD_hist->GetNbinsX() + 1) + 
                          mumuEWK_hist->Integral(0, mumuEWK_hist->GetNbinsX() + 1) ) )*( 
                          mumuDATAOBS_hist->Integral(0, mumuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          mumuTOP_hist->Integral(0, mumuTOP_hist->GetNbinsX() + 1) + 
                          mumuVV_hist->Integral(0, mumuVV_hist->GetNbinsX() + 1) ) ) ))*100/
                    ( ( ( nunuZmumuQCD_hist->Integral(0, nunuZmumuQCD_hist->GetNbinsX() + 1) + 
                          nunuZmumuEWK_hist->Integral(0, nunuZmumuEWK_hist->GetNbinsX() + 1) )/( 
                          mumuQCD_hist->Integral(0, mumuQCD_hist->GetNbinsX() + 1) + 
                          mumuEWK_hist->Integral(0, mumuEWK_hist->GetNbinsX() + 1) ) )*( 
                          mumuDATAOBS_hist->Integral(0, mumuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          mumuTOP_hist->Integral(0, mumuTOP_hist->GetNbinsX() + 1) + 
                          mumuVV_hist->Integral(0, mumuVV_hist->GetNbinsX() + 1) ) ) ) << " & "
                << (( ( ( nunuZmumuQCD_JESDOWN_hist->Integral(0, nunuZmumuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                          nunuZmumuEWK_JESDOWN_hist->Integral(0, nunuZmumuEWK_JESDOWN_hist->GetNbinsX() + 1) )/( 
                          mumuQCD_JESDOWN_hist->Integral(0, mumuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                          mumuEWK_JESDOWN_hist->Integral(0, mumuEWK_JESDOWN_hist->GetNbinsX() + 1) ) )*( 
                          mumuDATAOBS_hist->Integral(0, mumuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          mumuTOP_JESDOWN_hist->Integral(0, mumuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                          mumuVV_JESDOWN_hist->Integral(0, mumuVV_JESDOWN_hist->GetNbinsX() + 1) ) ) ) - 
                    ( ( ( nunuZmumuQCD_hist->Integral(0, nunuZmumuQCD_hist->GetNbinsX() + 1) + 
                          nunuZmumuEWK_hist->Integral(0, nunuZmumuEWK_hist->GetNbinsX() + 1) )/( 
                          mumuQCD_hist->Integral(0, mumuQCD_hist->GetNbinsX() + 1) + 
                          mumuEWK_hist->Integral(0, mumuEWK_hist->GetNbinsX() + 1) ) )*( 
                          mumuDATAOBS_hist->Integral(0, mumuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          mumuTOP_hist->Integral(0, mumuTOP_hist->GetNbinsX() + 1) + 
                          mumuVV_hist->Integral(0, mumuVV_hist->GetNbinsX() + 1) ) ) ))*100/
                    ( ( ( nunuZmumuQCD_hist->Integral(0, nunuZmumuQCD_hist->GetNbinsX() + 1) + 
                          nunuZmumuEWK_hist->Integral(0, nunuZmumuEWK_hist->GetNbinsX() + 1) )/( 
                          mumuQCD_hist->Integral(0, mumuQCD_hist->GetNbinsX() + 1) + 
                          mumuEWK_hist->Integral(0, mumuEWK_hist->GetNbinsX() + 1) ) )*( 
                          mumuDATAOBS_hist->Integral(0, mumuDATAOBS_hist->GetNbinsX() + 1) - ( 
                          mumuTOP_hist->Integral(0, mumuTOP_hist->GetNbinsX() + 1) + 
                          mumuVV_hist->Integral(0, mumuVV_hist->GetNbinsX() + 1) ) ) ) << "\\\\ " << std::endl;

      std::cout << " \\hline" << std::endl;

      std::cout << " $\\frac{\\text{N}_\\text{MC}^\\text{SR}}{\\text{N}_\\text{MC}^\\text{CR}}$  &  " 
                << (( ( nunuZmumuQCD_JESUP_hist->Integral(0, nunuZmumuQCD_JESUP_hist->GetNbinsX() + 1) + 
                        nunuZmumuEWK_JESUP_hist->Integral(0, nunuZmumuEWK_JESUP_hist->GetNbinsX() + 1) )/( 
                        mumuQCD_JESUP_hist->Integral(0, mumuQCD_JESUP_hist->GetNbinsX() + 1) + 
                        mumuEWK_JESUP_hist->Integral(0, mumuEWK_JESUP_hist->GetNbinsX() + 1) ) ) - 
                    ( ( nunuZmumuQCD_hist->Integral(0, nunuZmumuQCD_hist->GetNbinsX() + 1) + 
                        nunuZmumuEWK_hist->Integral(0, nunuZmumuEWK_hist->GetNbinsX() + 1) )/( 
                        mumuQCD_hist->Integral(0, mumuQCD_hist->GetNbinsX() + 1) + 
                        mumuEWK_hist->Integral(0, mumuEWK_hist->GetNbinsX() + 1) ) ))*100/
                    ( ( nunuZmumuQCD_hist->Integral(0, nunuZmumuQCD_hist->GetNbinsX() + 1) + 
                        nunuZmumuEWK_hist->Integral(0, nunuZmumuEWK_hist->GetNbinsX() + 1) )/( 
                        mumuQCD_hist->Integral(0, mumuQCD_hist->GetNbinsX() + 1) + 
                        mumuEWK_hist->Integral(0, mumuEWK_hist->GetNbinsX() + 1) ) ) << " & "
                << (( ( nunuZmumuQCD_JESDOWN_hist->Integral(0, nunuZmumuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                        nunuZmumuEWK_JESDOWN_hist->Integral(0, nunuZmumuEWK_JESDOWN_hist->GetNbinsX() + 1) )/( 
                        mumuQCD_JESDOWN_hist->Integral(0, mumuQCD_JESDOWN_hist->GetNbinsX() + 1) + 
                        mumuEWK_JESDOWN_hist->Integral(0, mumuEWK_JESDOWN_hist->GetNbinsX() + 1) ) ) - 
                    ( ( nunuZmumuQCD_hist->Integral(0, nunuZmumuQCD_hist->GetNbinsX() + 1) + 
                        nunuZmumuEWK_hist->Integral(0, nunuZmumuEWK_hist->GetNbinsX() + 1) )/( 
                        mumuQCD_hist->Integral(0, mumuQCD_hist->GetNbinsX() + 1) + 
                        mumuEWK_hist->Integral(0, mumuEWK_hist->GetNbinsX() + 1) ) ))*100/
                    ( ( nunuZmumuQCD_hist->Integral(0, nunuZmumuQCD_hist->GetNbinsX() + 1) + 
                        nunuZmumuEWK_hist->Integral(0, nunuZmumuEWK_hist->GetNbinsX() + 1) )/( 
                        mumuQCD_hist->Integral(0, mumuQCD_hist->GetNbinsX() + 1) + 
                        mumuEWK_hist->Integral(0, mumuEWK_hist->GetNbinsX() + 1) ) ) << "\\\\ " << std::endl;

      std::cout << " ( N$_\\text{data\\_obs}^\\text{CR}$ - N$_\\text{backgrounds}^\\text{CR}$)  &  " 
                << ( mumuTOP_hist->Integral(0, mumuTOP_hist->GetNbinsX() + 1) + 
                     mumuVV_hist->Integral(0, mumuVV_hist->GetNbinsX() + 1) -
                   ( mumuTOP_JESUP_hist->Integral(0, mumuTOP_JESUP_hist->GetNbinsX() + 1) + 
                     mumuVV_JESUP_hist->Integral(0, mumuVV_JESUP_hist->GetNbinsX() + 1) ) )*100/
                   ( mumuDATAOBS_hist->Integral(0, mumuDATAOBS_hist->GetNbinsX() + 1) - 
                   ( mumuTOP_hist->Integral(0, mumuTOP_hist->GetNbinsX() + 1) + 
                     mumuVV_hist->Integral(0, mumuVV_hist->GetNbinsX() + 1) ) ) << " & "
                << ( mumuTOP_hist->Integral(0, mumuTOP_hist->GetNbinsX() + 1) + 
                     mumuVV_hist->Integral(0, mumuVV_hist->GetNbinsX() + 1) -
                   ( mumuTOP_JESDOWN_hist->Integral(0, mumuTOP_JESDOWN_hist->GetNbinsX() + 1) + 
                     mumuVV_JESDOWN_hist->Integral(0, mumuVV_JESDOWN_hist->GetNbinsX() + 1) ) )*100/
                   ( mumuDATAOBS_hist->Integral(0, mumuDATAOBS_hist->GetNbinsX() + 1) - 
                   ( mumuTOP_hist->Integral(0, mumuTOP_hist->GetNbinsX() + 1) + 
                     mumuVV_hist->Integral(0, mumuVV_hist->GetNbinsX() + 1) ) ) << "\\\\ " << std::endl;

      std::cout << std::endl;

      // ****************************************
      std::cout << " *****************************************" << std::endl;
      // ****************************************

      std::cout << " \\textbf{Z($\\mu\\mu$)}     &              &  Central             &  JESUP              &  JESDOWN\\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;
      std::cout << " \\multirow{2}*{TOP}       &  Integral    & " 
                << mumuTOP_hist->Integral(0, mumuTOP_hist->GetNbinsX() + 1) << "              & " 
                << mumuTOP_JESUP_hist->Integral(0, mumuTOP_JESUP_hist->GetNbinsX() + 1) << "             & "
                << mumuTOP_JESDOWN_hist->Integral(0, mumuTOP_JESDOWN_hist->GetNbinsX() + 1) << " \\\\ " << std::endl;
      std::cout << "                          &  GetEntries  & " 
                << mumuTOP_hist->GetEntries() << "                  & " 
                << mumuTOP_JESUP_hist->GetEntries() << "                 & "
                << mumuTOP_JESDOWN_hist->GetEntries() << " \\\\ " << std::endl;
      std::cout << " \\hline" << std::endl;
      std::cout << " \\multirow{2}*{VV}       &  Integral    & " 
                << mumuVV_hist->Integral(0, mumuVV_hist->GetNbinsX() + 1) << "              & " 
                << mumuVV_JESUP_hist->Integral(0, mumuVV_JESUP_hist->GetNbinsX() + 1) << "             & "
                << mumuVV_JESDOWN_hist->Integral(0, mumuVV_JESDOWN_hist->GetNbinsX() + 1) << " \\\\ " << std::endl;
      std::cout << "                          &  GetEntries  & " 
                << mumuVV_hist->GetEntries() << "                  & " 
                << mumuVV_JESUP_hist->GetEntries() << "                 & "
                << mumuVV_JESDOWN_hist->GetEntries() << " \\\\ " << std::endl;

      std::cout << std::endl;

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
    }
    mycanvas[i]->Print("JESValidation.pdf");
    if ( i == nR-1 ){
      mycanvas[i]->Print("JESValidation.pdf]");
    }

  }//endof loop over variable of interest

  return 1;

}//main

