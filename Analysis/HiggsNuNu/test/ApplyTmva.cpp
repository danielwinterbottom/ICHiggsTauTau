#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"

#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#include "TMVA/Tools.h"

using namespace TMVA;

int main( int argc, char** argv )
{//main
  std::string folder;

  if (argc < 3) {
    std::cout << " Usage: " << argv[0] 
	      << " <signalFactor> <useRealData> <Opt:path to input folder>"
	      << std::endl; 
    return 1;
  }
  if (argc > 3) {
    folder = argv[3];
  }
  else {
    folder = "output_tmva/nunu/MET130/";
  }

  bool useData = false;
  double signalFactor = 1.0;
  std::istringstream(argv[1])>>signalFactor;
  std::istringstream(argv[2])>>useData;

  double qcdfactor = 1;

  // --- Create the Reader object

   TMVA::Reader *reader = new TMVA::Reader( "Color:!Silent" );    

   // Create a set of variables and declare them to the reader
   // - the variable names MUST corresponds in name and type to those given in the weight file(s) used

   const unsigned nVars=13;
   //const unsigned nSpecs=0;

   Float_t var[nVars];
//   Float_t spec[nSpecs];

   for (unsigned iV(0); iV<nVars;++iV){
     //if (iV<nSpecs) spec[iV] = 0;
     var[iV] = 0;
   }


   reader->AddVariable("jet2_pt",&var[0]);
   reader->AddVariable("dijet_M",&var[1]);
   reader->AddVariable("dijet_deta",&var[2]);
   reader->AddVariable("dijet_dphi",&var[3]);
   reader->AddVariable("met",&var[4]);
   reader->AddVariable("met_significance",&var[5]);
   reader->AddVariable("sqrt_ht",&var[6]);
   reader->AddVariable("jetmet_mindphi",&var[7]);
   reader->AddVariable("dijetmet_ptfraction",&var[8]);
   reader->AddVariable("dijetmet_vectorialSum_pt",&var[9]);
   reader->AddVariable("jet1met_scalarprod_frac := jet1met_scalarprod/met",&var[10]);
   reader->AddVariable("jet2met_scalarprod_frac := jet2met_scalarprod/met",&var[11]);
   reader->AddVariable("n_jets_cjv_30",&var[12]);



//    reader->AddVariable("jet2_eta",&var[0]);
//    reader->AddVariable("met_significance",&var[1]);
//    reader->AddVariable("mht",&var[2]);
//    reader->AddVariable("jet2met_dphi",&var[3]);
//    reader->AddVariable("jetmet_mindphi",&var[4]);
//    reader->AddVariable("jetunclet_mindphi",&var[5]);
//    reader->AddVariable("metunclet_dphi",&var[6]);
//    reader->AddVariable("dijetmet_scalarSum_pt",&var[7]);
//    reader->AddVariable("dijetmet_ptfraction",&var[8]);
//    reader->AddVariable("jet1met_scalarprod",&var[9]);
//    reader->AddVariable("jet2met_scalarprod",&var[10]);
//    reader->AddVariable("jet1met_scalarprod_frac := jet1met_scalarprod/met",&var[11]);
//    reader->AddVariable("jet2met_scalarprod_frac := jet2met_scalarprod/met",&var[12]);

//    reader->AddSpectator("jet1_pt",&spec[0]);
//    reader->AddSpectator("jet2_pt",&spec[1]);
//    reader->AddSpectator("jet1_eta",&spec[2]);
//    reader->AddSpectator("jet1_phi",&spec[3]);
//    reader->AddSpectator("jet2_phi",&spec[4]);
//    reader->AddSpectator("dijet_M",&spec[5]);
//    reader->AddSpectator("dijet_deta",&spec[6]);
//    reader->AddSpectator("dijet_sumeta",&spec[7]);
//    reader->AddSpectator("dijet_dphi",&spec[8]);
//    reader->AddSpectator("met",&spec[9]);
//    reader->AddSpectator("met_phi",&spec[10]);
//    reader->AddSpectator("sumet",&spec[11]);
//    reader->AddSpectator("ht",&spec[12]);
//    reader->AddSpectator("sqrt_ht",&spec[13]);
//    reader->AddSpectator("unclustered_et",&spec[14]);
//    reader->AddSpectator("unclustered_phi",&spec[15]);
//    reader->AddSpectator("jet1met_dphi",&spec[16]);
//    reader->AddSpectator("dijetmet_vectorialSum_pt",&spec[17]);
//    reader->AddSpectator("n_jets_cjv_30",&spec[18]);
//    reader->AddSpectator("n_jets_cjv_20EB_30EE",&spec[19]);

   // Book method(s)
   TString methodName = "BDT method";
   TString dir    = "weights/";
   TString prefix = "TMVAClassification";
   TString weightfile = dir + prefix + TString("_BDT") + TString(".weights.xml");
   reader->BookMVA( methodName, weightfile ); 
   //methodName = "Likelihood method";
   //weightfile = dir + prefix + TString("_Likelihood") + TString(".weights.xml");
   //reader->BookMVA( methodName, weightfile ); 
   methodName = "Fisher method";
   weightfile = dir + prefix + TString("_Fisher") + TString(".weights.xml");
   reader->BookMVA( methodName, weightfile ); 

   // Book output histograms
   UInt_t nbin = 100;
   TH1F   *histLk_data(0),*histFi_data(0),*histBdt_data(0);
   histLk_data      = new TH1F( "MVA_Likelihood_data",    "MVA_Likelihood",    nbin, -1, 1 );
   histFi_data      = new TH1F( "MVA_Fisher_data",        "MVA_Fisher",        nbin, -4, 4 );
   histBdt_data     = new TH1F( "MVA_BDT_data",           "MVA_BDT",           nbin, -1, 1 );
   TH1F   *histLk_qcd(0),*histFi_qcd(0),*histBdt_qcd(0);
   histLk_qcd      = new TH1F( "MVA_Likelihood_qcd",    "MVA_Likelihood",    nbin, -1, 1 );
   histFi_qcd      = new TH1F( "MVA_Fisher_qcd",        "MVA_Fisher",        nbin, -4, 4 );
   histBdt_qcd     = new TH1F( "MVA_BDT_qcd",           "MVA_BDT",           nbin, -1, 1 );
   TH1F   *histLk_ewk(0),*histFi_ewk(0),*histBdt_ewk(0);
   histLk_ewk      = new TH1F( "MVA_Likelihood_ewk",    "MVA_Likelihood",    nbin, -1, 1 );
   histFi_ewk      = new TH1F( "MVA_Fisher_ewk",        "MVA_Fisher",        nbin, -4, 4 );
   histBdt_ewk     = new TH1F( "MVA_BDT_ewk",           "MVA_BDT",           nbin, -1, 1 );
   TH1F   *histLk_sig(0),*histFi_sig(0),*histBdt_sig(0);
   histLk_sig      = new TH1F( "MVA_Likelihood_sig",    "MVA_Likelihood_sig",    nbin, -1, 1 );
   histFi_sig      = new TH1F( "MVA_Fisher_sig",        "MVA_Fisher_sig",        nbin, -4, 4 );
   histBdt_sig = new TH1F( "MVA_BDT_sig",       "MVA_BDT_sig",       nbin, -1, 1 );

   // Prepare input tree (this must be replaced by your data source)
 
  //List of input files
  bool useJoaosFiles = true;
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

  std::vector<std::string> ewkfiles;
  ewkfiles.push_back("MC_TTJets");
  //powheg samples
  //ewkfiles.push_back("MC_TT-v1");
  //ewkfiles.push_back("MC_TT-v2");
  //
  ewkfiles.push_back("MC_T-tW");
  ewkfiles.push_back("MC_Tbar-tW");
  ewkfiles.push_back("MC_SingleT-s-powheg-tauola");
  ewkfiles.push_back("MC_SingleTBar-s-powheg-tauola");
  ewkfiles.push_back("MC_SingleT-t-powheg-tauola");
  ewkfiles.push_back("MC_SingleTBar-t-powheg-tauola");
  ewkfiles.push_back("MC_WW-pythia6-tauola");
  ewkfiles.push_back("MC_WZ-pythia6-tauola");
  ewkfiles.push_back("MC_ZZ-pythia6-tauola");
  ewkfiles.push_back("MC_W1JetsToLNu_enu");
  ewkfiles.push_back("MC_W2JetsToLNu_enu");
  ewkfiles.push_back("MC_W3JetsToLNu_enu");
  ewkfiles.push_back("MC_W4JetsToLNu_enu");
  ewkfiles.push_back("MC_WJetsToLNu-v1_enu");
  ewkfiles.push_back("MC_WJetsToLNu-v2_enu");
  ewkfiles.push_back("MC_W1JetsToLNu_munu");
  ewkfiles.push_back("MC_W2JetsToLNu_munu");
  ewkfiles.push_back("MC_W3JetsToLNu_munu");
  ewkfiles.push_back("MC_W4JetsToLNu_munu");
  ewkfiles.push_back("MC_WJetsToLNu-v1_munu");
  ewkfiles.push_back("MC_WJetsToLNu-v2_munu");
  ewkfiles.push_back("MC_W1JetsToLNu_taunu");
  ewkfiles.push_back("MC_W2JetsToLNu_taunu");
  ewkfiles.push_back("MC_W3JetsToLNu_taunu");
  ewkfiles.push_back("MC_W4JetsToLNu_taunu");
  ewkfiles.push_back("MC_WJetsToLNu-v1_taunu");
  ewkfiles.push_back("MC_WJetsToLNu-v2_taunu");
  ewkfiles.push_back("MC_DYJetsToLL");
  ewkfiles.push_back("MC_DY1JetsToLL");
  ewkfiles.push_back("MC_DY2JetsToLL");
  ewkfiles.push_back("MC_DY3JetsToLL");
  ewkfiles.push_back("MC_DY4JetsToLL");
  ewkfiles.push_back("MC_ZJetsToNuNu_100_HT_200");
  ewkfiles.push_back("MC_ZJetsToNuNu_200_HT_400");
  ewkfiles.push_back("MC_ZJetsToNuNu_400_HT_inf");
  ewkfiles.push_back("MC_ZJetsToNuNu_50_HT_100");
  ewkfiles.push_back("MC_WGamma");
  ewkfiles.push_back("MC_EWK-Z2j");
  ewkfiles.push_back("MC_EWK-Z2jiglep");
  ewkfiles.push_back("MC_EWK-W2jminus_enu");
  ewkfiles.push_back("MC_EWK-W2jplus_enu");
  ewkfiles.push_back("MC_EWK-W2jminus_munu");
  ewkfiles.push_back("MC_EWK-W2jplus_munu");
  ewkfiles.push_back("MC_EWK-W2jminus_taunu");
  ewkfiles.push_back("MC_EWK-W2jplus_taunu");
  //signal
  std::vector<std::string> sigfiles;
  //sigfiles.push_back("MC_VBF_HToZZTo4Nu_M-120");
  sigfiles.push_back("MC_Powheg-Htoinv-mH125");

  std::vector<std::string> datafiles;
  datafiles.push_back("Data_MET-2012A-13Jul2012-v1");
  datafiles.push_back("Data_MET-2012A-06Aug2012-v1");
  datafiles.push_back("Data_MET-2012B-13Jul2012-v1");
  datafiles.push_back("Data_MET-2012C-24Aug2012-v1");
  datafiles.push_back("Data_MET-2012C-11Dec2012-v1"); 
  datafiles.push_back("Data_MET-2012C-PromptReco-v2");
  datafiles.push_back("Data_MET-2012D-PromptReco-v1");
  
  unsigned tmp = 0;
  if (useData) tmp = datafiles.size();
  else tmp = ewkfiles.size()+qcdfiles.size()+sigfiles.size();
  const unsigned nData = tmp;

  TFile *fData[nData];
  std::string mcfiles[nData];

  for (unsigned iD(0); iD<nData;++iD){//loop on data files
    if (useData) fData[iD] = TFile::Open((folder+datafiles[iD]+".root").c_str());
    else {
      if (iD<ewkfiles.size()) fData[iD] = TFile::Open((folder+ewkfiles[iD]+".root").c_str());
      else if (iD<ewkfiles.size()+qcdfiles.size()) {
	fData[iD] = TFile::Open((folder+qcdfiles[iD-ewkfiles.size()]+".root").c_str());
	qcdfactor = 1.44;
      }
      else fData[iD] = TFile::Open((folder+sigfiles[iD-ewkfiles.size()-qcdfiles.size()]+".root").c_str());
    }
    if (!fData[iD]) {
      std::cout << "ERROR: could not open data file" << std::endl;
      exit(1);
    }
    std::cout << "--- TMVAClassificationApp    : Using input file: " << fData[iD]->GetName() << std::endl;
    mcfiles[iD] = fData[iD]->GetName();

    // --- Event loop

    // Prepare the event tree
    if (useData) std::cout << "--- Select real data file " << datafiles[iD] << " " << std::endl;
    else {
      std::cout << "--- Select MC file " << mcfiles[iD];
      if (iD==nData-1) std::cout << " enhanced by a factor " << signalFactor ;
      std::cout << std::endl;
    }

    double weight = 1;
   // double jet1_E=0,jet2_E=0,metx=0,mety=0;
    double jet1met=0,jet2met=0;
    //Double_t treeSpec[nSpecs-2];
    Double_t treeVar[nVars];
    UInt_t nCJV = 0;
    //UInt_t nCJV20 = 0;

    TTree *t = (TTree*)fData[iD]->Get("TmvaInputTree");
//     t->SetBranchAddress("total_weight", &weight);
//     t->SetBranchAddress("jet1_pt",                 &treeSpec[0]);
//     t->SetBranchAddress("jet2_pt",                 &treeSpec[1]);
//     t->SetBranchAddress("jet1_eta",                &treeSpec[2]);
//     t->SetBranchAddress("jet2_eta",                &treeVar[0]);
//     t->SetBranchAddress("jet1_phi",                &treeSpec[3]);
//     t->SetBranchAddress("jet2_phi",                &treeSpec[4]);
//     t->SetBranchAddress("jet1_E",                  &jet1_E);
//     t->SetBranchAddress("jet2_E",                  &jet2_E);
//     t->SetBranchAddress("dijet_M",                 &treeSpec[5]);
//     t->SetBranchAddress("dijet_deta",              &treeSpec[6]);
//     t->SetBranchAddress("dijet_sumeta",            &treeSpec[7]);
//     t->SetBranchAddress("dijet_dphi",              &treeSpec[8]);
//     t->SetBranchAddress("met",                     &treeSpec[9]);
//     t->SetBranchAddress("met_x",                   &metx);
//     t->SetBranchAddress("met_y",                   &mety);
//     t->SetBranchAddress("met_phi",                 &treeSpec[10]);
//     t->SetBranchAddress("met_significance",        &treeVar[1]);
//     t->SetBranchAddress("sumet",                   &treeSpec[11]);
//     t->SetBranchAddress("ht",                      &treeSpec[12]);
//     t->SetBranchAddress("mht",                     &treeVar[2]);
//     t->SetBranchAddress("sqrt_ht",                 &treeSpec[13]);
//     t->SetBranchAddress("unclustered_et",          &treeSpec[14]);
//     t->SetBranchAddress("unclustered_phi",         &treeSpec[15]);
//     t->SetBranchAddress("jet1met_dphi",            &treeSpec[16]);
//     t->SetBranchAddress("jet2met_dphi",            &treeVar[3]);
//     t->SetBranchAddress("jetmet_mindphi",          &treeVar[4]);
//     t->SetBranchAddress("jetunclet_mindphi",       &treeVar[5]);
//     t->SetBranchAddress("metunclet_dphi",          &treeVar[6]);
//     t->SetBranchAddress("dijetmet_scalarSum_pt",   &treeVar[7]);
//     t->SetBranchAddress("dijetmet_vectorialSum_pt",&treeSpec[17]);
//     t->SetBranchAddress("dijetmet_ptfraction",     &treeVar[8]);
//     t->SetBranchAddress("jet1met_scalarprod",      &treeVar[9]);
//     t->SetBranchAddress("jet2met_scalarprod",      &treeVar[10]);
//     t->SetBranchAddress("n_jets_cjv_30",           &nCJV);
//     t->SetBranchAddress("n_jets_cjv_20EB_30EE",    &nCJV20);
    t->SetBranchAddress("total_weight", &weight);
    t->SetBranchAddress("jet2_pt",                 &treeVar[0]);
    t->SetBranchAddress("dijet_M",                 &treeVar[1]);
    t->SetBranchAddress("dijet_deta",              &treeVar[2]);
    t->SetBranchAddress("dijet_dphi",              &treeVar[3]);
    t->SetBranchAddress("met",                     &treeVar[4]);
    t->SetBranchAddress("met_significance",        &treeVar[5]);
    t->SetBranchAddress("sqrt_ht",                 &treeVar[6]);
    t->SetBranchAddress("jetmet_mindphi",          &treeVar[7]);
    t->SetBranchAddress("dijetmet_ptfraction",     &treeVar[8]);
    t->SetBranchAddress("dijetmet_vectorialSum_pt",&treeVar[9]);
    t->SetBranchAddress("jet1met_scalarprod",      &jet1met);
    t->SetBranchAddress("jet2met_scalarprod",      &jet2met);
    t->SetBranchAddress("n_jets_cjv_30",           &nCJV);


    std::cout << "--- Processing: " << t->GetEntries() << " events" << std::endl;
    TStopwatch sw;
    sw.Start();
    for (Long64_t ievt=0; ievt<t->GetEntries();ievt++) {
      
      if (ievt%1000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;
      
      t->GetEntry(ievt);
      
      treeVar[10] = jet1met/treeVar[4];
      treeVar[11] = jet2met/treeVar[4];
      
      for (unsigned iV(0); iV<nVars;++iV){
	var[iV] = (Float_t)treeVar[iV];
	//spec[iV] = (Float_t)treeSpec[iV];
      }
      var[12] = (Float_t)nCJV;
      //spec[19] = (Float_t)nCJV20;

      bool pass = //true;
	treeVar[2]>3.6 //deta cut
	&& treeVar[5] > 3
	&& treeVar[7] > 1.5
	;
      if (!pass) continue;

      //enhance signal
      if (!useData && iD==nData-1) weight = weight*signalFactor;
      weight = weight*qcdfactor;

      // --- Return the MVA outputs and fill into histograms
      if (useData) {
	//histLk_data     ->Fill( reader->EvaluateMVA( "Likelihood method"    ) , weight);
	histFi_data     ->Fill( reader->EvaluateMVA( "Fisher method"        ) , weight);
	histBdt_data    ->Fill( reader->EvaluateMVA( "BDT method"           ) , weight);
      }
      else {
	if (iD<ewkfiles.size()) {
	  //histLk_ewk->Fill( reader->EvaluateMVA( "Likelihood method"    ) , weight);
	  histFi_ewk->Fill( reader->EvaluateMVA( "Fisher method"        ) , weight);
	  histBdt_ewk->Fill( reader->EvaluateMVA( "BDT method"           ) , weight);
	}
	else if (iD<ewkfiles.size()+qcdfiles.size()) {
	  //histLk_qcd->Fill( reader->EvaluateMVA( "Likelihood method"    ) , weight);
	  histFi_qcd->Fill( reader->EvaluateMVA( "Fisher method"        ) , weight);
	  histBdt_qcd->Fill( reader->EvaluateMVA( "BDT method"           ) , weight);
	}
	else if (iD==nData-1) {
	  //histLk_sig->Fill( reader->EvaluateMVA( "Likelihood method"    ) , weight);
	  histFi_sig->Fill( reader->EvaluateMVA( "Fisher method"        ) , weight);
	  histBdt_sig->Fill( reader->EvaluateMVA( "BDT method"           ) , weight);
	}
      }
      
      
    }

    // Get elapsed time
    sw.Stop();
    std::cout << "--- End of event loop: "; sw.Print();
    
  }//loop on data files


  std::ostringstream outputName;
  outputName << "TMVApp_" ;
  if (useData) outputName << "realData.root";
  else outputName << "pseudoData_signalTimes" << signalFactor << ".root";
  
  TFile *target  = new TFile( outputName.str().c_str(),"RECREATE" );
  histLk_data     ->Write();
  histFi_data     ->Write();
  histBdt_data    ->Write();
  histLk_ewk     ->Write();
  histFi_ewk     ->Write();
  histBdt_ewk    ->Write();
  histLk_qcd     ->Write();
  histFi_qcd     ->Write();
  histBdt_qcd    ->Write();
  histLk_sig ->Write();
  histFi_sig ->Write();
  histBdt_sig->Write();
  target->Close();
  
  std::cout << "--- Created root file: " << outputName.str() << " containing the MVA output histograms" << std::endl;
  
  delete reader;
  
  std::cout << "==> TMVAClassificationApplication is done!" << std::endl << std::endl;

  return 0;

}//main 

