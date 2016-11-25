
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <iomanip>

#include "TMath.h"
#include "Math/Vector4D.h"
#include "Math/VectorUtil.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h" 
#include "TGraphErrors.h" 
#include "TStyle.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TGraphAsymmErrors.h"

#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#include "TMVA/Tools.h"


void fillReader(TMVA::Reader *reader, std::vector<Float_t> & var, std::vector<Float_t> & spec){
  var.resize(13,0);
  reader->AddVariable("jet2_eta",&var[0]);
  reader->AddVariable("met_significance",&var[1]);
  reader->AddVariable("mht",&var[2]);
  reader->AddVariable("jet2met_dphi",&var[3]);
  reader->AddVariable("jetmet_mindphi",&var[4]);
  reader->AddVariable("jetunclet_mindphi",&var[5]);
  reader->AddVariable("metunclet_dphi",&var[6]);
  reader->AddVariable("dijetmet_scalarSum_pt",&var[7]);
  reader->AddVariable("dijetmet_ptfraction",&var[8]);
  reader->AddVariable("jet1met_scalarprod",&var[9]);
  reader->AddVariable("jet2met_scalarprod",&var[10]);
  reader->AddVariable("jet1met_scalarprod_frac := jet1met_scalarprod/met",&var[11]);
  reader->AddVariable("jet2met_scalarprod_frac := jet2met_scalarprod/met",&var[12]);

  spec.resize(20,0);
  reader->AddSpectator("jet1_pt",&spec[0]);
  reader->AddSpectator("jet2_pt",&spec[1]);
  reader->AddSpectator("jet1_eta",&spec[2]);
  reader->AddSpectator("jet1_phi",&spec[3]);
  reader->AddSpectator("jet2_phi",&spec[4]);
  reader->AddSpectator("dijet_M",&spec[5]);
  reader->AddSpectator("dijet_deta",&spec[6]);
  reader->AddSpectator("dijet_sumeta",&spec[7]);
  reader->AddSpectator("dijet_dphi",&spec[8]);
  reader->AddSpectator("met",&spec[9]);
  reader->AddSpectator("met_phi",&spec[10]);
  reader->AddSpectator("sumet",&spec[11]);
  reader->AddSpectator("ht",&spec[12]);
  reader->AddSpectator("sqrt_ht",&spec[13]);
  reader->AddSpectator("unclustered_et",&spec[14]);
  reader->AddSpectator("unclustered_phi",&spec[15]);
  reader->AddSpectator("jet1met_dphi",&spec[16]);
  reader->AddSpectator("dijetmet_vectorialSum_pt",&spec[17]);
  reader->AddSpectator("n_jets_cjv_30",&spec[18]);
  reader->AddSpectator("n_jets_cjv_20EB_30EE",&spec[19]);

   // Book method(s)
   TString methodName = "BDT method";
   TString dir    = "../data/TmvaWeights/";
   TString prefix = "TMVAClassification";
   TString weightfile = dir + prefix + TString("_BDT_QCDrej") + TString(".weights.xml");
   reader->BookMVA( methodName, weightfile ); 
   //methodName = "Likelihood method";
   //weightfile = dir + prefix + TString("_Likelihood") + TString(".weights.xml");
   //reader->BookMVA( methodName, weightfile ); 
   //methodName = "Fisher method";
   //weightfile = dir + prefix + TString("_Fisher") + TString(".weights.xml");
   //reader->BookMVA( methodName, weightfile ); 

};

TH1F * make_histogram(std::string aName, std::string aAxisTitle, std::string aUnit,
		      int nBins, float xMin, float xMax){
  std::string lUnit = aUnit;
  if (lUnit.size() > 0) lUnit = " ("+lUnit+")";
  TH1F *hTmp = new TH1F(aName.c_str(),(";"+aAxisTitle+lUnit).c_str(),nBins,xMin,xMax);
  return hTmp;
};

void MakeAllHistograms(TH1F * hist[], const std::string aLabel){
  hist[0] = make_histogram((aLabel+"_jet1_pt").c_str(),"Jet 1 p_{T}", "GeV", 100,0,1000);
  hist[1] = make_histogram((aLabel+"_jet2_pt").c_str(),"Jet 2 p_{T}", "GeV", 100,0,1000);
  hist[2] = make_histogram((aLabel+"_jet1_eta").c_str(),"Jet 1 #eta", "", 50,-5.,5);
  hist[3] = make_histogram((aLabel+"_jet2_eta").c_str(),"Jet 2 #eta", "", 50,-5,5);
  hist[4] = make_histogram((aLabel+"_dijet_M").c_str(),"M_{jj}", " GeV", 100,0,5000);
  hist[5] = make_histogram((aLabel+"_dijet_deta").c_str(),"#Delta#eta_{jj}", "", 100,0,8);
  hist[6] = make_histogram((aLabel+"_dijet_sumeta").c_str(),"#eta_{j1}+#eta_{j2}", "", 100,-5,5);
  hist[7] = make_histogram((aLabel+"_dijet_dphi").c_str(),"#Delta#phi_{jj}", "", 100,0,3.1416);
  hist[8] = make_histogram((aLabel+"_met").c_str(),"MET", "GeV", 100,0,1000);
  hist[9] = make_histogram((aLabel+"_met_phi").c_str(),"MET #phi", "", 100,-3.1416,3.1416);
  hist[10] = make_histogram((aLabel+"_met_significance").c_str(),"MET significance", "", 100,0,20);
  hist[11] = make_histogram((aLabel+"_sumet").c_str(),"#Sigma E_{T}", "GeV", 100,0,5000);
  hist[12] = make_histogram((aLabel+"_ht").c_str(),"H_{T}", "GeV", 100,0,4000);
  hist[13] = make_histogram((aLabel+"_mht").c_str(),"MH_{T}", "GeV", 100,0,4000);
  hist[14] = make_histogram((aLabel+"_sqrt_ht").c_str(),"#sqrt{H_{T}}", "GeV^{1/2}", 100,0,70);
  hist[15] = make_histogram((aLabel+"_unclustered_et").c_str(),"Unclustered E_{T}", "GeV", 100,0,6000);
  hist[16] = make_histogram((aLabel+"_unclustered_phi").c_str(),"Unclustered #phi", "GeV", 100,-3.1416,3.1416);
  hist[17] = make_histogram((aLabel+"_jet1met_dphi").c_str(),"#Delta#phi(MET,jet1)", "", 100,0,3.1416);
  hist[18] = make_histogram((aLabel+"_jet2met_dphi").c_str(),"#Delta#phi(MET,jet2)", "", 100,0,3.1416);
  hist[19] = make_histogram((aLabel+"_jetmet_mindphi").c_str(),"minimum #Delta#phi(MET,jet)", "", 100,0,3.1416);
  hist[20] = make_histogram((aLabel+"_jetunclet_mindphi").c_str(),"minimum #Delta#phi(unclustered,jet)", "", 100,0,3.1416);
  hist[21] = make_histogram((aLabel+"_metunclet_dphi").c_str(),"#Delta#phi(MET,unclustered)", "", 100,0,3.1416);
  hist[22] = make_histogram((aLabel+"_dijetmet_scalarSum_pt").c_str(), "p_{T}^{jet1}+p_{T}^{jet2}+MET", "GeV" ,100,0,4000);
  hist[23] = make_histogram((aLabel+"_dijetmet_vectorialSum_pt").c_str(),"p_{T}(#vec{j1}+#vec{j2}+#vec{MET})", "GeV", 50,0,1000);
  hist[24] = make_histogram((aLabel+"_dijetmet_ptfraction").c_str(),"p_{T}^{dijet}/(p_{T}^{dijet}+MET)", "", 100,0,1);
  hist[25] = make_histogram((aLabel+"_jet1met_scalarprod").c_str(), "#vec{p_{T}^{jet1}}.#vec{MET}/MET", "GeV" ,100,-1500,1500);
  hist[26] = make_histogram((aLabel+"_jet2met_scalarprod").c_str(), "#vec{p_{T}^{jet2}}.#vec{MET}/MET", "GeV" ,100,-1500,1500);
  hist[27] = make_histogram((aLabel+"_jet1met_scalarprod_frac").c_str(), "#vec{p_{T}^{jet1}}.#vec{MET}/MET^{2}", "" ,50,-8,8);
  hist[28] = make_histogram((aLabel+"_jet2met_scalarprod_frac").c_str(), "#vec{p_{T}^{jet2}}.#vec{MET}/MET^{2}", "" ,50,-8,8);
  hist[29] = make_histogram((aLabel+"_n_jets_cjv_30").c_str(), "CJV jets (30 GeV)", "" ,15,0,15);
  hist[30] = make_histogram((aLabel+"_n_jets_cjv_20EB_30EE").c_str(), "CJV jets (20 EB, 30 EE)", "" ,15,0,15);
  hist[31] = make_histogram((aLabel+"_BDToutput").c_str(), "QCD BDT","",100,-1,1);
  hist[32] = make_histogram((aLabel+"_alphaT").c_str(), "#alpha_{T}","",100,0,2);

}

void FillVariables(TFile *inputFile, const unsigned nVars,
		   TMVA::Reader *reader,
		   std::vector<Float_t> & var, std::vector<Float_t> & spec,
		   TH1F * hist[], double & n_tot,
		   bool reversePass=false){

  double variable[nVars];
  for (unsigned iV(0); iV<nVars; ++iV){//loop on variables
    variable[iV] = 0;
  }

  double weight = 1;
  double jet1_phi=0,jet2_phi=0,jet1_E=0,jet2_E=0,metx=0,mety=0;
  unsigned nCJV_30=0,nCJV_20_30=0;

  TTree *t = (TTree*)inputFile->Get("TmvaInputTree");
  t->SetBranchAddress("total_weight", &weight);
  t->SetBranchAddress("jet1_pt",                 &variable[0]);
  t->SetBranchAddress("jet2_pt",                 &variable[1]);
  t->SetBranchAddress("jet1_eta",                &variable[2]);
  t->SetBranchAddress("jet2_eta",                &variable[3]);
  t->SetBranchAddress("jet1_phi",                &jet1_phi);
  t->SetBranchAddress("jet2_phi",                &jet2_phi);
  t->SetBranchAddress("jet1_E",                  &jet1_E);
  t->SetBranchAddress("jet2_E",                  &jet2_E);
  t->SetBranchAddress("dijet_M",                 &variable[4]);
  t->SetBranchAddress("dijet_deta",              &variable[5]);
  t->SetBranchAddress("dijet_sumeta",            &variable[6]);
  t->SetBranchAddress("dijet_dphi",              &variable[7]);
  t->SetBranchAddress("met",                     &variable[8]);
  t->SetBranchAddress("met_x",                   &metx);
  t->SetBranchAddress("met_y",                   &mety);
  t->SetBranchAddress("met_phi",                 &variable[9]);
  t->SetBranchAddress("met_significance",        &variable[10]);
  t->SetBranchAddress("sumet",                   &variable[11]);
  t->SetBranchAddress("ht",                      &variable[12]);
  t->SetBranchAddress("mht",                     &variable[13]);
  t->SetBranchAddress("sqrt_ht",                 &variable[14]);
  t->SetBranchAddress("unclustered_et",          &variable[15]);
  t->SetBranchAddress("unclustered_phi",         &variable[16]);
  t->SetBranchAddress("jet1met_dphi",            &variable[17]);
  t->SetBranchAddress("jet2met_dphi",            &variable[18]);
  t->SetBranchAddress("jetmet_mindphi",          &variable[19]);
  t->SetBranchAddress("jetunclet_mindphi",       &variable[20]);
  t->SetBranchAddress("metunclet_dphi",          &variable[21]);
  t->SetBranchAddress("dijetmet_scalarSum_pt",   &variable[22]);
  t->SetBranchAddress("dijetmet_vectorialSum_pt",&variable[23]);
  t->SetBranchAddress("dijetmet_ptfraction",     &variable[24]);
  t->SetBranchAddress("jet1met_scalarprod",      &variable[25]);
  t->SetBranchAddress("jet2met_scalarprod",      &variable[26]);
  t->SetBranchAddress("n_jets_cjv_30",           &nCJV_30);
  t->SetBranchAddress("n_jets_cjv_20EB_30EE",    &nCJV_20_30);
  //get indices of TMVA variables and spectators, in same order as used during training
  unsigned specIndex[18] = {0,1,2,4,5,6,7,8,9,11,12,14,15,16,17,23,29,30};
  unsigned varIndex[13] = {3,10,13,18,19,20,21,22,24,25,26,27,28};

  Int_t nevent = t->GetEntries();
  for (Int_t i=0;i<nevent;i++) {//loop on events
    t->GetEntry(i);

    //ROOT::Math::PtEtaPhiEVector jet1(variable[0],variable[2],jet1_phi,jet1_E);
    //ROOT::Math::PtEtaPhiEVector jet2(variable[1],variable[3],jet2_phi,jet2_E);
    //ROOT::Math::PtEtaPhiEVector metvec(variable[8],0,variable[9],variable[8]);
    //ROOT::Math::PtEtaPhiEVector unclustered(variable[15],0,variable[16],variable[15]);
    //variable[23] = (jet1.px()*metx+jet1.py()*mety)/variable[8];
    //variable[24] = (jet2.px()*metx+jet2.py()*mety)/variable[8];
    //variable[25] = std::min(fabs(ROOT::Math::VectorUtil::DeltaPhi(jet1,unclustered)),
    //fabs(ROOT::Math::VectorUtil::DeltaPhi(jet2,unclustered)));
  //variable[26] = fabs(ROOT::Math::VectorUtil::DeltaPhi(unclustered,metvec));
    variable[27] = variable[25]/variable[8];
    variable[28] = variable[26]/variable[8];
    variable[29] = static_cast<double>(nCJV_30);
    variable[30] = static_cast<double>(nCJV_20_30);

    variable[32] = (variable[12]-variable[0]+variable[1])/(2*sqrt(pow(variable[12],2)-pow(variable[13],2)));

    for (unsigned iV(0); iV<var.size();++iV){
      var[iV] = variable[varIndex[iV]];
    }
    for (unsigned iV(0); iV<spec.size()-2;++iV){
      spec[iV] = variable[specIndex[iV]];
    }

    spec[18] = (Float_t)nCJV_30;
    spec[19] = (Float_t)nCJV_20_30;

    n_tot += weight;

    variable[31] = reader->EvaluateMVA( "BDT method" );

    //cuts
    bool presel = variable[19]>1.5 && variable[5]>3.6;
    bool pass =  presel && variable[10] > 3.0;//variable[31] > 0.3;
    if (reversePass) pass = presel && variable[10] > 2.0 && variable[10]<2.6;
    //variable[5]>3.8 //deta cut
    //  && variable[4] > 1100 //Mjj cut
    //  && variable[8] > 100 //MET cut
    //  && variable[10] > 5. //MET significance
    //  ;
    if (pass) {
      for (unsigned iV(0); iV<nVars; ++iV){//loop on variables
	hist[iV]->Fill(variable[iV],weight);
	//if (iV==0 && i<10) std::cout << inputFile->GetName() << " " << i << " jet1_pt = " << variable[iV] << " wt=" << weight << std::endl;
      }//loop on variables
    }
    else {
      //fill BDT also for failed events
      //hist[31]->Fill(variable[31],weight);
      if (variable[19]>1.5 && variable[10] > 3.0) hist[5]->Fill(variable[5],weight);
      if (presel) hist[10]->Fill(variable[10],weight);
      if (variable[10] > 3.0) hist[19]->Fill(variable[19],weight);
    }
  }//loop on events
}

int CheckTmvaVariables() {//main

  // --- Create the Reader object

   TMVA::Reader *reader = new TMVA::Reader( "Color:!Silent" );    

   std::vector<Float_t> var;
   std::vector<Float_t> spec;
   fillReader(reader,var,spec);

   //open input files
  std::string folder = "../output_tmva/nunu/MET130/";

  std::vector<std::string> datafiles;
  datafiles.push_back("Data_MET-2012A-13Jul2012-v1");
  datafiles.push_back("Data_MET-2012A-06Aug2012-v1");
  datafiles.push_back("Data_MET-2012B-13Jul2012-v1");
  datafiles.push_back("Data_MET-2012C-24Aug2012-v1");
  datafiles.push_back("Data_MET-2012C-11Dec2012-v1"); 
  datafiles.push_back("Data_MET-2012C-PromptReco-v2");
  datafiles.push_back("Data_MET-2012D-PromptReco-v1");

  //all files
  bool useJoaosFiles = false;
  std::vector<std::string> qcdfiles;
  if (useJoaosFiles){
    qcdfiles.push_back("MC_QCD-Pt-80to120_VBF-MET40");
    qcdfiles.push_back("MC_QCD-Pt-120to170_VBF-MET40");
    qcdfiles.push_back("MC_QCD-Pt-170to300_VBF-MET40");
    qcdfiles.push_back("MC_QCD-Pt-300to470_VBF-MET40");
    qcdfiles.push_back("MC_QCD-Pt-470to600_VBF-MET40");
  }
  else {
    qcdfiles.push_back("MC_QCD-Pt-30to50-pythia6");
    qcdfiles.push_back("MC_QCD-Pt-50to80-pythia6");
    qcdfiles.push_back("MC_QCD-Pt-80to120-pythia6");
    qcdfiles.push_back("MC_QCD-Pt-120to170-pythia6");
    qcdfiles.push_back("MC_QCD-Pt-170to300-pythia6");
    qcdfiles.push_back("MC_QCD-Pt-300to470-pythia6");
    qcdfiles.push_back("MC_QCD-Pt-470to600-pythia6");
  }
  qcdfiles.push_back("MC_QCD-Pt-600to800-pythia6");
  qcdfiles.push_back("MC_QCD-Pt-800to1000-pythia6");
  qcdfiles.push_back("MC_QCD-Pt-1000to1400-pythia6");
  qcdfiles.push_back("MC_QCD-Pt-1400to1800-pythia6");
  qcdfiles.push_back("MC_QCD-Pt-1800-pythia6");
  qcdfiles.push_back("MC_GJets-HT-200To400-madgraph");
  qcdfiles.push_back("MC_GJets-HT-400ToInf-madgraph");

  //all bkg without qcd
  std::vector<std::string> files;
  files.push_back("MC_TTJets");
  //powheg samples
  //files.push_back("MC_TT-v1");
  //files.push_back("MC_TT-v2");
  //
  files.push_back("MC_T-tW");
  files.push_back("MC_Tbar-tW");
  files.push_back("MC_SingleT-s-powheg-tauola");
  files.push_back("MC_SingleTBar-s-powheg-tauola");
  files.push_back("MC_SingleT-t-powheg-tauola");
  files.push_back("MC_SingleTBar-t-powheg-tauola");
  files.push_back("MC_WW-pythia6-tauola");
  files.push_back("MC_WZ-pythia6-tauola");
  files.push_back("MC_ZZ-pythia6-tauola");
  files.push_back("MC_W1JetsToLNu_enu");
  files.push_back("MC_W2JetsToLNu_enu");
  files.push_back("MC_W3JetsToLNu_enu");
  files.push_back("MC_W4JetsToLNu_enu");
  files.push_back("MC_WJetsToLNu-v1_enu");
  files.push_back("MC_WJetsToLNu-v2_enu");
  files.push_back("MC_W1JetsToLNu_munu");
  files.push_back("MC_W2JetsToLNu_munu");
  files.push_back("MC_W3JetsToLNu_munu");
  files.push_back("MC_W4JetsToLNu_munu");
  files.push_back("MC_WJetsToLNu-v1_munu");
  files.push_back("MC_WJetsToLNu-v2_munu");
  files.push_back("MC_W1JetsToLNu_taunu");
  files.push_back("MC_W2JetsToLNu_taunu");
  files.push_back("MC_W3JetsToLNu_taunu");
  files.push_back("MC_W4JetsToLNu_taunu");
  files.push_back("MC_WJetsToLNu-v1_taunu");
  files.push_back("MC_WJetsToLNu-v2_taunu");
  files.push_back("MC_DYJetsToLL");
  files.push_back("MC_DY1JetsToLL");
  files.push_back("MC_DY2JetsToLL");
  files.push_back("MC_DY3JetsToLL");
  files.push_back("MC_DY4JetsToLL");
  files.push_back("MC_ZJetsToNuNu_100_HT_200");
  files.push_back("MC_ZJetsToNuNu_200_HT_400");
  files.push_back("MC_ZJetsToNuNu_400_HT_inf");
  files.push_back("MC_ZJetsToNuNu_50_HT_100");
  files.push_back("MC_WGamma");
  files.push_back("MC_EWK-Z2j");
  files.push_back("MC_EWK-Z2jiglep");
  files.push_back("MC_EWK-W2jminus_enu");
  files.push_back("MC_EWK-W2jplus_enu");
  files.push_back("MC_EWK-W2jminus_munu");
  files.push_back("MC_EWK-W2jplus_munu");
  files.push_back("MC_EWK-W2jminus_taunu");
  files.push_back("MC_EWK-W2jplus_taunu");

  const unsigned nData = datafiles.size();
  const unsigned nQCD = qcdfiles.size();
  const unsigned nMC = files.size();
  TFile *fData[nData];
  TFile *fQCD[nQCD];
  TFile *fMC[nMC];

  TFile *fSignal120 = TFile::Open((folder+"/MC_VBF_HToZZTo4Nu_M-120.root").c_str());
  TFile *fSignal125 = TFile::Open((folder+"/MC_Powheg-Htoinv-mH125.root").c_str());
  TFile *fSignal400 = TFile::Open((folder+"/MC_VBF_HToZZTo4Nu_M-400.root").c_str());

  //TFile *output_withQCD = new TFile("DataMC_SumBkgWithQCD.root","RECREATE");
  //TFile *output_withoutQCD = new TFile("DataMC_SumBkgWithoutQCD.root","RECREATE");

  const unsigned nVars = 33;

  TCanvas *myc[nVars];
  gStyle->SetOptStat(0);


  bool logy[nVars] = {
    1,1,0,0,1,
    0,1,1,1,1,
    1,1,1,1,1,
    1,1,1,1,1,
    1,1,1,1,0,
    1,1,1,1,1,
    1,1,1
  };

  double n_dijet_data = 0;
  double n_dijet_dataqcd = 0;
  double n_dijet_bkg = 0;
  double n_dijet_bkgqcd = 0;
  double n_dijet_qcd = 0;
  double n_dijet_signal = 0;
  double n_sel_data = 0;
  double n_sel_dataqcd = 0;
  double n_sel_bkg = 0;
  double n_sel_bkgqcd = 0;
  double n_sel_qcd = 0;
  double n_sel_signal = 0;

  for (unsigned iV(0); iV<nVars;++iV){
    std::ostringstream canvasName;
    canvasName << "myc_" << iV;
    myc[iV] = new TCanvas(canvasName.str().c_str(),"",1);
    myc[iV]->SetLogy(logy[iV]);
  }

  TH1F *hVarEWK[nVars];
  MakeAllHistograms(hVarEWK,"Bkg");
  TH1F *hVarEWKQCD[nVars];
  MakeAllHistograms(hVarEWKQCD,"BkgQCD");

  TH1F *hVarSignal120[nVars];
  MakeAllHistograms(hVarSignal120,"Signal120");
  double n_dijet_120 = 0;
  FillVariables(fSignal120,nVars,reader,var,spec,hVarSignal120,n_dijet_120);

  TH1F *hVarSignal125[nVars];
  MakeAllHistograms(hVarSignal125,"Signal125");
  FillVariables(fSignal125,nVars,reader,var,spec,hVarSignal125,n_dijet_signal);

  TH1F *hVarSignal400[nVars];
  MakeAllHistograms(hVarSignal400,"Signal400");
  double n_dijet_400 = 0;
  FillVariables(fSignal400,nVars,reader,var,spec,hVarSignal400,n_dijet_400);

  TH1F *hVarData[nVars];
  MakeAllHistograms(hVarData,"Data");

  TH1F *hVarDataQCD[nVars];
  MakeAllHistograms(hVarDataQCD,"DataQCD");

  TH1F *hVarQCD[nVars];
  MakeAllHistograms(hVarQCD,"QCD");
  

  for (unsigned iBkg = 0; iBkg < files.size(); ++iBkg) {//loop on bkg files
    fMC[iBkg] = TFile::Open((folder+"/"+files[iBkg]+".root").c_str());
    if (!fMC[iBkg]) {
      std::cerr << "Warning, file " << files[iBkg] << " could not be opened." << std::endl;
      continue;//return 1;
    }

    std::cout << " Processing tree " <<  files[iBkg] << std::endl;
    
    FillVariables(fMC[iBkg],nVars,reader,var,spec,hVarEWK,n_dijet_bkg);
    FillVariables(fMC[iBkg],nVars,reader,var,spec,hVarEWKQCD,n_dijet_bkgqcd,true);

  }//loop on bkg files

  for (unsigned iD(0); iD<nData;++iD){//loop on data files
    fData[iD] = TFile::Open((folder+"/"+datafiles[iD]+".root").c_str());
    if (!fData[iD]) {
      std::cerr << "Warning, file " << datafiles[iD] << " could not be opened." << std::endl;
      continue;//return 1;
    }   
    std::cout << " Processing tree " <<  datafiles[iD] << std::endl;
 
    FillVariables(fData[iD],nVars,reader,var,spec,hVarData,n_dijet_data);
    FillVariables(fData[iD],nVars,reader,var,spec,hVarDataQCD,n_dijet_dataqcd,true);
   }//loop on data files

  for (unsigned iQ(0); iQ<nQCD;++iQ){//loop on qcd files
    fQCD[iQ] = TFile::Open((folder+"/"+qcdfiles[iQ]+".root").c_str());
    if (!fQCD[iQ]) {
      std::cerr << "Warning, file " << qcdfiles[iQ] << " could not be opened." << std::endl;
      continue;//return 1;
    }   
    std::cout << " Processing tree " <<  qcdfiles[iQ] << std::endl;
 
    FillVariables(fQCD[iQ],nVars,reader,var,spec,hVarQCD,n_dijet_qcd);
  }//loop on qcd files

  for (unsigned iV(0); iV<nVars; ++iV){//loop on variables
    hVarDataQCD[iV]->Add(hVarEWKQCD[iV],-1);
  }

  n_sel_qcd = hVarQCD[0]->Integral();
  n_sel_bkg = hVarEWK[0]->Integral();
  n_sel_bkgqcd = hVarEWKQCD[0]->Integral();
  n_sel_data = hVarData[0]->Integral();
  n_sel_dataqcd = hVarDataQCD[0]->Integral();
  n_sel_signal = hVarSignal125[0]->Integral();
  
  double reweight = (n_sel_data-n_sel_bkg)/n_sel_qcd;
  double reweight_data = (n_sel_data-n_sel_bkg)/n_sel_dataqcd;

  std::cout << " --INFO: Scaling QCD by factor " <<  reweight << std::endl;
  std::cout << " --INFO: Scaling Data QCD by factor " <<  reweight_data << std::endl;


  for (unsigned iV(0); iV<nVars; ++iV){//loop on variables
    myc[iV]->cd();
  
    //manual reweight to fit data-bkg integral...
    hVarQCD[iV]->Scale(reweight);
    hVarDataQCD[iV]->Scale(reweight_data);
    hVarDataQCD[iV]->Add(hVarEWK[iV]);
    hVarQCD[iV]->Add(hVarEWK[iV]);
    hVarSignal120[iV]->Scale(hVarData[iV]->Integral()/hVarSignal120[iV]->Integral());
    hVarSignal125[iV]->Scale(hVarData[iV]->Integral()/hVarSignal125[iV]->Integral());
    //hVarSignal400[iV]->Scale(hVarQCD[iV]->Integral()/hVarSignal400[iV]->Integral());
    //hVarEWK[iV]->Scale(hVarData[iV]->Integral()/hVarEWK[iV]->Integral());
    //hVarQCD[iV]->Scale(hVarData[iV]->Integral()/hVarQCD[iV]->Integral());

    hVarData[iV]->SetMarkerColor(1);
    hVarData[iV]->SetMarkerStyle(2);
    hVarData[iV]->GetYaxis()->SetTitle("Events");
    if (hVarData[iV]->GetMinimum() > hVarQCD[iV]->GetMinimum()) hVarData[iV]->SetMinimum(hVarQCD[iV]->GetMinimum());
    if (hVarData[iV]->GetMaximum() < hVarEWK[iV]->GetMaximum()) hVarData[iV]->SetMaximum(hVarEWK[iV]->GetMaximum());
    if (hVarData[iV]->GetMaximum() < hVarSignal125[iV]->GetMaximum()) hVarData[iV]->SetMaximum(hVarSignal125[iV]->GetMaximum());
    //if (hVarData[iV]->GetMinimum() > hVarSignal120[iV]->GetMinimum()) hVarData[iV]->SetMinimum(hVarSignal120[iV]->GetMinimum());
    if (logy[iV]==true && hVarData[iV]->GetMinimum()==0) hVarData[iV]->SetMinimum(0.1);
    hVarData[iV]->Draw("PE");
    //plot QCD+others
    hVarQCD[iV]->SetFillColor(5);
    hVarQCD[iV]->SetLineColor(5);
    hVarQCD[iV]->GetYaxis()->SetTitle("Events");
    hVarQCD[iV]->Draw("same");
    hVarDataQCD[iV]->SetFillColor(7);
    hVarDataQCD[iV]->SetLineColor(7);
    hVarDataQCD[iV]->SetFillStyle(3004);
    hVarDataQCD[iV]->GetYaxis()->SetTitle("Events");
    hVarDataQCD[iV]->Draw("same");
    //plot others
    hVarEWK[iV]->SetFillColor(6);
    hVarEWK[iV]->SetLineColor(6);
    hVarEWK[iV]->SetFillStyle(3005);
    hVarEWK[iV]->GetYaxis()->SetTitle("Events");
    hVarEWK[iV]->Draw("same");
    hVarEWK[iV]->Draw("Psame");

    //draw signal120
    hVarSignal120[iV]->SetFillColor(9);
    hVarSignal120[iV]->SetFillStyle(3005);
    hVarSignal120[iV]->SetLineColor(9);
    hVarSignal120[iV]->SetMarkerColor(9);
    hVarSignal120[iV]->SetMarkerStyle(23);
    hVarSignal120[iV]->GetYaxis()->SetTitle("Events");
    //hVarSignal120[iV]->Draw("same");
    //hVarSignal120[iV]->Draw("Psame");
     //draw signal125
    hVarSignal125[iV]->SetFillColor(4);
    hVarSignal125[iV]->SetFillStyle(3004);
    hVarSignal125[iV]->SetLineColor(4);
    hVarSignal125[iV]->SetMarkerColor(4);
    hVarSignal125[iV]->SetMarkerStyle(22);
    hVarSignal125[iV]->GetYaxis()->SetTitle("Events");
    hVarSignal125[iV]->Draw("same");
    hVarSignal125[iV]->Draw("Psame");
    //draw signal400
    //hVarSignal400[iV]->SetFillColor(6);
    hVarSignal400[iV]->SetLineColor(4);
    //hVarSignal400[iV]->SetMarkerColor(4);
    //hVarSignal400[iV]->SetMarkerStyle(23);
    hVarSignal400[iV]->GetYaxis()->SetTitle("Events");
    //hVarSignal400[iV]->Draw("Lsame");

    //replot data on top
    hVarData[iV]->Draw("PEsame");
    hVarData[iV]->Draw("axissame");

    //double proba = hVarData[iV]->KolmogorovTest(hVarQCD[iV],"UON");
    //std::cout << hVarEWK[iV]->GetName() << " = " << proba << std::endl;

    TLegend *leg = new TLegend(0.75,0.8,1.0,1.0);
    leg->SetFillColor(10);
    leg->AddEntry(hVarData[iV],"Data","P");
    //leg->AddEntry(hVarSignal120[iV],"VBF m_{H}=120 GeV","PF");
    leg->AddEntry(hVarSignal125[iV],"VBF m_{H}=125 GeV","PF");
    //leg->AddEntry(hVarSignal400[iV],"VBF m_{H}=400 GeV","L");
    leg->AddEntry(hVarQCD[iV],"QCD","F");
    leg->AddEntry(hVarDataQCD[iV],"Data-driven QCD","F");
    leg->AddEntry(hVarEWK[iV],"V+top+VV","F");
    leg->Draw("same");

    std::ostringstream output;
    //output << "TmvaPlots_deta3p8_Mjj1100_met100_metsig5/" << hVarEWK[iV]->GetName() << ".pdf";
    output << "TmvaPlots_qcdrej/" << hVarEWK[iV]->GetName() << ".pdf";
    myc[iV]->Print(output.str().c_str() );


  }//loop on variables

  std::cout << "----------------------------------------------" << std::endl
	    << "---- Summary of number of events selected ----" << std::endl
	    << " - N_dijet = " << n_dijet_data << " dataqcd = " << n_dijet_dataqcd<< " MCqcd = " << n_dijet_qcd << " bkg = " << n_dijet_bkg << " signal = " << n_dijet_signal << std::endl
	    <<" - N_sel = " << n_sel_data << " dataqcd = " << n_sel_dataqcd*reweight_data << " MCqcd = " << n_sel_qcd*reweight << " bkg = " << n_sel_bkg << " signal = " << n_sel_signal 
	    << std::endl;

  return 0;
}//main

