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

#include "TMVA/Factory.h"
#include "TMVA/Tools.h"


int main( int argc, char** argv )
{//main
  std::string folder;

  if (argc > 1) {
    folder = argv[1];
  }
  else {
    folder = "output_tmva/nunu/MET130/";
  }

  bool useQCD = true;
  bool useJoaosFiles = true;
  bool useOthers = true;
  bool useOthersAsSignal = false;
  bool applyPreSelection = true;

  //List of input signal files
  std::vector<std::string> sigfiles;
  //sigfiles.push_back("MC_VBF_HToZZTo4Nu_M-120");
  sigfiles.push_back("MC_Powheg-Htoinv-mH125");

  if (useOthersAsSignal) {
    sigfiles.push_back("MC_TTJets");
    //powheg samples
    //sigfiles.push_back("MC_TT-v1");
    //sigfiles.push_back("MC_TT-v2");
    //
    sigfiles.push_back("MC_T-tW");
    sigfiles.push_back("MC_Tbar-tW");
    sigfiles.push_back("MC_SingleT-s-powheg-tauola");
    sigfiles.push_back("MC_SingleTBar-s-powheg-tauola");
    sigfiles.push_back("MC_SingleT-t-powheg-tauola");
    sigfiles.push_back("MC_SingleTBar-t-powheg-tauola");
    sigfiles.push_back("MC_WW-pythia6-tauola");
    sigfiles.push_back("MC_WZ-pythia6-tauola");
    sigfiles.push_back("MC_ZZ-pythia6-tauola");
    sigfiles.push_back("MC_W1JetsToLNu_enu");
    sigfiles.push_back("MC_W2JetsToLNu_enu");
    sigfiles.push_back("MC_W3JetsToLNu_enu");
    sigfiles.push_back("MC_W4JetsToLNu_enu");
    sigfiles.push_back("MC_WJetsToLNu-v1_enu");
    sigfiles.push_back("MC_WJetsToLNu-v2_enu");
    sigfiles.push_back("MC_W1JetsToLNu_munu");
    sigfiles.push_back("MC_W2JetsToLNu_munu");
    sigfiles.push_back("MC_W3JetsToLNu_munu");
    sigfiles.push_back("MC_W4JetsToLNu_munu");
    sigfiles.push_back("MC_WJetsToLNu-v1_munu");
    sigfiles.push_back("MC_WJetsToLNu-v2_munu");
    sigfiles.push_back("MC_W1JetsToLNu_taunu");
    sigfiles.push_back("MC_W2JetsToLNu_taunu");
    sigfiles.push_back("MC_W3JetsToLNu_taunu");
    sigfiles.push_back("MC_W4JetsToLNu_taunu");
    sigfiles.push_back("MC_WJetsToLNu-v1_taunu");
    sigfiles.push_back("MC_WJetsToLNu-v2_taunu");
    sigfiles.push_back("MC_DYJetsToLL");
    sigfiles.push_back("MC_DY1JetsToLL");
    sigfiles.push_back("MC_DY2JetsToLL");
    sigfiles.push_back("MC_DY3JetsToLL");
    sigfiles.push_back("MC_DY4JetsToLL");
    sigfiles.push_back("MC_ZJetsToNuNu_100_HT_200");
    sigfiles.push_back("MC_ZJetsToNuNu_200_HT_400");
    sigfiles.push_back("MC_ZJetsToNuNu_400_HT_inf");
    sigfiles.push_back("MC_ZJetsToNuNu_50_HT_100");
    sigfiles.push_back("MC_GJets-HT-200To400-madgraph");
    sigfiles.push_back("MC_GJets-HT-400ToInf-madgraph");
    sigfiles.push_back("MC_WGamma");
    sigfiles.push_back("MC_EWK-Z2j");
    sigfiles.push_back("MC_EWK-Z2jiglep");
    sigfiles.push_back("MC_EWK-W2jminus_enu");
    sigfiles.push_back("MC_EWK-W2jplus_enu");
    sigfiles.push_back("MC_EWK-W2jminus_munu");
    sigfiles.push_back("MC_EWK-W2jplus_munu");
    sigfiles.push_back("MC_EWK-W2jminus_taunu");
    sigfiles.push_back("MC_EWK-W2jplus_taunu");
  }

  //List of input files
  //all files
  std::vector<std::string> bkgfiles;
  if (useQCD){
    if (useJoaosFiles){
      bkgfiles.push_back("MC_QCD-Pt-80to120_VBF-MET40");
      bkgfiles.push_back("MC_QCD-Pt-120to170_VBF-MET40");
      bkgfiles.push_back("MC_QCD-Pt-170to300_VBF-MET40");
      bkgfiles.push_back("MC_QCD-Pt-300to470_VBF-MET40");
      bkgfiles.push_back("MC_QCD-Pt-470to600_VBF-MET40");
    }
    else {
      bkgfiles.push_back("MC_QCD-Pt-30to50-pythia6");
      bkgfiles.push_back("MC_QCD-Pt-50to80-pythia6");
      bkgfiles.push_back("MC_QCD-Pt-80to120-pythia6");
      bkgfiles.push_back("MC_QCD-Pt-120to170-pythia6");
      bkgfiles.push_back("MC_QCD-Pt-170to300-pythia6");
      bkgfiles.push_back("MC_QCD-Pt-300to470-pythia6");
      bkgfiles.push_back("MC_QCD-Pt-470to600-pythia6");
    }
    bkgfiles.push_back("MC_QCD-Pt-600to800-pythia6");
    bkgfiles.push_back("MC_QCD-Pt-800to1000-pythia6");
    bkgfiles.push_back("MC_QCD-Pt-1000to1400-pythia6");
    bkgfiles.push_back("MC_QCD-Pt-1400to1800-pythia6");
    bkgfiles.push_back("MC_QCD-Pt-1800-pythia6");
    bkgfiles.push_back("MC_GJets-HT-200To400-madgraph");
    bkgfiles.push_back("MC_GJets-HT-400ToInf-madgraph");
  }
  if (useOthers) {
    bkgfiles.push_back("MC_TTJets");
    //powheg samples
    //bkgfiles.push_back("MC_TT-v1");
    //bkgfiles.push_back("MC_TT-v2");
    //
    bkgfiles.push_back("MC_T-tW");
    bkgfiles.push_back("MC_Tbar-tW");
    bkgfiles.push_back("MC_SingleT-s-powheg-tauola");
    bkgfiles.push_back("MC_SingleTBar-s-powheg-tauola");
    bkgfiles.push_back("MC_SingleT-t-powheg-tauola");
    bkgfiles.push_back("MC_SingleTBar-t-powheg-tauola");
    bkgfiles.push_back("MC_WW-pythia6-tauola");
    bkgfiles.push_back("MC_WZ-pythia6-tauola");
    bkgfiles.push_back("MC_ZZ-pythia6-tauola");
    bkgfiles.push_back("MC_W1JetsToLNu_enu");
    bkgfiles.push_back("MC_W2JetsToLNu_enu");
    bkgfiles.push_back("MC_W3JetsToLNu_enu");
    bkgfiles.push_back("MC_W4JetsToLNu_enu");
    bkgfiles.push_back("MC_WJetsToLNu-v1_enu");
    bkgfiles.push_back("MC_WJetsToLNu-v2_enu");
    bkgfiles.push_back("MC_W1JetsToLNu_munu");
    bkgfiles.push_back("MC_W2JetsToLNu_munu");
    bkgfiles.push_back("MC_W3JetsToLNu_munu");
    bkgfiles.push_back("MC_W4JetsToLNu_munu");
    bkgfiles.push_back("MC_WJetsToLNu-v1_munu");
    bkgfiles.push_back("MC_WJetsToLNu-v2_munu");
    bkgfiles.push_back("MC_W1JetsToLNu_taunu");
    bkgfiles.push_back("MC_W2JetsToLNu_taunu");
    bkgfiles.push_back("MC_W3JetsToLNu_taunu");
    bkgfiles.push_back("MC_W4JetsToLNu_taunu");
    bkgfiles.push_back("MC_WJetsToLNu-v1_taunu");
    bkgfiles.push_back("MC_WJetsToLNu-v2_taunu");
    bkgfiles.push_back("MC_DYJetsToLL");
    bkgfiles.push_back("MC_DY1JetsToLL");
    bkgfiles.push_back("MC_DY2JetsToLL");
    bkgfiles.push_back("MC_DY3JetsToLL");
    bkgfiles.push_back("MC_DY4JetsToLL");
    bkgfiles.push_back("MC_ZJetsToNuNu_100_HT_200");
    bkgfiles.push_back("MC_ZJetsToNuNu_200_HT_400");
    bkgfiles.push_back("MC_ZJetsToNuNu_400_HT_inf");
    bkgfiles.push_back("MC_ZJetsToNuNu_50_HT_100");
    bkgfiles.push_back("MC_WGamma");
    bkgfiles.push_back("MC_EWK-Z2j");
    bkgfiles.push_back("MC_EWK-Z2jiglep");
    bkgfiles.push_back("MC_EWK-W2jminus_enu");
    bkgfiles.push_back("MC_EWK-W2jplus_enu");
    bkgfiles.push_back("MC_EWK-W2jminus_munu");
    bkgfiles.push_back("MC_EWK-W2jplus_munu");
    bkgfiles.push_back("MC_EWK-W2jminus_taunu");
    bkgfiles.push_back("MC_EWK-W2jplus_taunu");
  }

   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
  TFile *output_tmva = TFile::Open((folder+"/TMVA_signalSel.root").c_str(),"RECREATE");

  // Create the factory object. Later you can choose the methods
  // whose performance you'd like to investigate. The factory is 
  // the only TMVA object you have to interact with
   //
   // The first argument is the base of the name of all the
   // weightfiles in the directory weight/
   //
   // The second argument is the output file for the training results
   // All TMVA output can be suppressed by removing the "!" (not) in
   // front of the "Silent" argument in the option string
   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", output_tmva,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );


  //fill the variables with event weight from the trees
  //const unsigned nVars = 4;

   /*
   factory->AddSpectator("jet1_pt","Jet 1 p_{T}", "GeV", 'F');
   factory->AddSpectator("jet2_pt","Jet 2 p_{T}", "GeV", 'F');
   factory->AddSpectator("jet1_eta","Jet 1 #eta", "", 'F');
   factory->AddVariable("jet2_eta","Jet 2 #eta", "", 'F');// **
   factory->AddSpectator("jet1_phi","Jet 1 #phi", "", 'F');
   factory->AddSpectator("jet2_phi","Jet 2 #phi", "", 'F');
   factory->AddSpectator("dijet_deta","#Delta#eta_{jj}", "", 'F');
   factory->AddSpectator("dijet_sumeta","#eta_{j1}+#eta_{j2}", "", 'F');
   factory->AddSpectator("dijet_dphi","#Delta#phi_{jj}", "", 'F');
   factory->AddSpectator("met","MET", "GeV", 'F');// **
   factory->AddSpectator("met_phi","MET #phi", "", 'F');
   factory->AddVariable("met_significance","MET significance", "", 'F');// **
   factory->AddSpectator("sumet","#Sum E_{T}", "GeV", 'F');
   factory->AddSpectator("ht","H_{T}", "GeV", 'F');
   factory->AddVariable("mht","MH_{T}", "GeV", 'F');// **
   factory->AddSpectator("sqrt_ht","#sqrt{H_{T}}", "GeV^{0.5}", 'F');
   factory->AddSpectator("unclustered_et","Unclustered E_{T}", "GeV", 'F');
   factory->AddSpectator("unclustered_phi","Unclustered #phi", "GeV", 'F');
   factory->AddSpectator("jet1met_dphi","#Delta#phi(MET,jet1)", "", 'F');
   factory->AddVariable("jet2met_dphi","#Delta#phi(MET,jet2)", "", 'F');// **
   factory->AddVariable("jetunclet_mindphi","minimum #Delta#phi(unclustered,jet)", "",  'F');// **
   factory->AddVariable("metunclet_dphi","#Delta#phi(MET,unclustered)", "",  'F');// **
   factory->AddVariable("dijetmet_scalarSum_pt", "p_{T}^{jet1}+p_{T}^{jet2}+MET", "GeV", 'F');// **
   factory->AddSpectator("dijetmet_vectorialSum_pt","p_{T}(#vec{j1}+#vec{j2}+#vec{MET})", "GeV", 'F');
   //factory->AddVariable("jet1met_scalarprod := (jet1_pt*cos(jet1_phi)*met_x+jet1_pt*sin(jet1_phi)*met_y)/met", "#vec{p_{T}^{jet1}}.#vec{MET}/MET", "GeV" , 'F');
   //factory->AddVariable("jet2met_scalarprod := (jet2_pt*cos(jet2_phi)*met_x+jet2_pt*sin(jet2_phi)*met_y)/met", "#vec{p_{T}^{jet2}}.#vec{MET}/MET", "GeV" , 'F');
   factory->AddVariable("jet1met_scalarprod", "#vec{p_{T}^{jet1}}.#vec{MET}/MET", "GeV" , 'F');// **
   factory->AddVariable("jet2met_scalarprod", "#vec{p_{T}^{jet2}}.#vec{MET}/MET", "GeV" , 'F');// **
   factory->AddVariable("jet1met_scalarprod_frac := jet1met_scalarprod/met", "#vec{p_{T}^{jet1}}.#vec{MET}/MET^{2}", "" , 'F');// **
   factory->AddVariable("jet2met_scalarprod_frac := jet2met_scalarprod/met", "#vec{p_{T}^{jet2}}.#vec{MET}/MET^{2}", "" , 'F');// **
   factory->AddSpectator("n_jets_cjv_20EB_30EE","CJV jets (|#eta|<2.4 and 20 GeV, or 30 GeV)", "" , 'I');
   */



   //factory->AddVariable("jet1_pt","Jet 1 p_{T}", "GeV", 'F');
   factory->AddVariable("jet2_pt","Jet 2 p_{T}", "GeV", 'F');
   factory->AddVariable("dijet_M","M_{jj}", " GeV", 'F');
   factory->AddVariable("dijet_deta","#Delta#eta_{jj}", "", 'F');
   factory->AddVariable("dijet_dphi","#Delta#phi_{jj}", "", 'F');
   factory->AddVariable("met","MET", "GeV", 'F');// **
   factory->AddVariable("met_significance","MET significance", "", 'F');// **
   //factory->AddVariable("mht","MH_{T}", "GeV", 'F');// **
   factory->AddVariable("sqrt_ht","#sqrt{H_{T}}", "GeV^{0.5}", 'F');
   factory->AddVariable("jetmet_mindphi","minimum #Delta#phi(MET,jet)", "", 'F');// **
   factory->AddVariable("dijetmet_ptfraction","p_{T}^{dijet}/(p_{T}^{dijet}+MET)", "", 'F');// **
   //factory->AddVariable("dijetmet_scalarSum_pt", "p_{T}^{jet1}+p_{T}^{jet2}+MET", "GeV", 'F');// **
   factory->AddVariable("dijetmet_vectorialSum_pt","p_{T}(#vec{j1}+#vec{j2}+#vec{MET})", "GeV", 'F');
   //factory->AddVariable("jet1met_scalarprod", "#vec{p_{T}^{jet1}}.#vec{MET}/MET", "GeV" , 'F');// **
   //factory->AddVariable("jet2met_scalarprod", "#vec{p_{T}^{jet2}}.#vec{MET}/MET", "GeV" , 'F');// **
   factory->AddVariable("jet1met_scalarprod_frac := jet1met_scalarprod/met", "#vec{p_{T}^{jet1}}.#vec{MET}/MET^{2}", "" , 'F');// **
   factory->AddVariable("jet2met_scalarprod_frac := jet2met_scalarprod/met", "#vec{p_{T}^{jet2}}.#vec{MET}/MET^{2}", "" , 'F');// **
   factory->AddVariable("n_jets_cjv_30","CJV jets (30 GeV)", "" , 'I');

   //test with only VBF variables used in cut-based analysis
   //factory->AddVariable("dijet_M","M_{jj}", " GeV", 'F');
   //factory->AddVariable("dijet_deta","#Delta#eta_{jj}", "", 'F');
   //factory->AddVariable("dijet_dphi","#Delta#phi_{jj}", "", 'F');
   //factory->AddVariable("met","MET", "GeV", 'F');
   //factory->AddVariable("n_jets_cjv_30","CJV jets (30 GeV)", "" , 'I');


  //get input files
  //signal
  //TFile *signalfile = TFile::Open((folder+"/"+"MC_VBF_HToZZTo4Nu_M-120.root").c_str());
  //TTree *signal = (TTree*)signalfile->Get("TmvaInputTree");
  //Double_t signalWeight     = 1.0;
  //factory->AddSignalTree(signal,signalWeight);
  //Set individual event weights (the variables must exist in the original TTree)
  //factory->SetSignalWeightExpression("total_weight");

  //background
  std::map<std::string, TFile *> tfiles;
  for (unsigned i = 0; i < bkgfiles.size(); ++i) {
    std::string filename = (bkgfiles[i]+".root");
    TFile * tmp = new TFile((folder+"/"+filename).c_str());
    if (!tmp) {
      std::cerr << "Warning, file " << filename << " could not be opened." << std::endl;
    } else {
      tfiles[bkgfiles[i]] = tmp;      
    }
  }
  TTree *background[bkgfiles.size()];

  //signal
  std::map<std::string, TFile *> sfiles;
  for (unsigned i = 0; i < sigfiles.size(); ++i) {
    std::string filename = (sigfiles[i]+".root");
    TFile * tmp = new TFile((folder+"/"+filename).c_str());
    if (!tmp) {
      std::cerr << "Warning, file " << filename << " could not be opened." << std::endl;
    } else {
      sfiles[sigfiles[i]] = tmp;      
    }
  }
  TTree *signal[sigfiles.size()];

  for (unsigned i = 0; i < bkgfiles.size(); ++i) {

    std::string f = bkgfiles[i];
    if (tfiles[f]){
      background[i] = (TTree*)tfiles[f]->Get("TmvaInputTree");
      //if (f.find("QCD-Pt")!=f.npos){
      //}
      Double_t backgroundWeight = 1.0;
      if (f.find("MC_QCD")!=f.npos || f.find("MC_GJets")!=f.npos) backgroundWeight = 1.44;
      factory->AddBackgroundTree(background[i],backgroundWeight);
      factory->SetBackgroundWeightExpression("total_weight");

    }//if file exist
    else {
      std::cout << " Cannot find background file " << f << std::endl;
    }
  }//loop on files

  for (unsigned i = 0; i < sigfiles.size(); ++i) {

    std::string f = sigfiles[i];
    if (sfiles[f]){
      signal[i] = (TTree*)sfiles[f]->Get("TmvaInputTree");
      //if (f.find("QCD-Pt")!=f.npos){
      //}
      Double_t signalWeight = 1.0;
      factory->AddSignalTree(signal[i],signalWeight);
      factory->SetSignalWeightExpression("total_weight");

    }//if file exist
    else {
      std::cout << " Cannot find signal file " << f << std::endl;
    }
  }//loop on files


   // Apply additional cuts on the signal and background samples (can be different)
  TCut mycuts = "";
  TCut mycutb = "";
  if (applyPreSelection) {
    mycuts = "dijet_deta>3.6 && met_significance>3 && jetmet_mindphi>1.5";
    mycutb = mycuts;
  }

  factory->PrepareTrainingAndTestTree( mycuts, mycutb,
				       "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );
  


   // Likelihood ("naive Bayes estimator")
  //factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood",
  //"H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" );

  // Linear discriminant (same as Fisher discriminant)
  //factory->BookMethod( TMVA::Types::kLD, "LD", "H:!V:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

  // Fisher discriminant (same as LD)
  factory->BookMethod( TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

  // TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
  //factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=60:HiddenLayers=N+5:TestRate=5:!UseRegulator" );

  // Boosted Decision Trees
  // Gradient Boost
  //factory->BookMethod( TMVA::Types::kBDT, "BDTG",
  //"!H:!V:NTrees=1000:MinNodeSize=1.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );
  //factory->BookMethod( TMVA::Types::kBDT, "BDTG",
  //                       "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:nCuts=20:MaxDepth=2" );


  // Adaptive Boost
  //factory->BookMethod( TMVA::Types::kBDT, "BDT1000",
  //	       "!H:!V:NTrees=1000:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20" );

  factory->BookMethod( TMVA::Types::kBDT, "BDT",
		       "!H:!V:NTrees=100:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.2:SeparationType=GiniIndex:nCuts=20" );

  // Bagging
  //factory->BookMethod( TMVA::Types::kBDT, "BDTB",
  //                       "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20" );

  // Decorrelation + Adaptive Boost
  //factory->BookMethod( TMVA::Types::kBDT, "BDTD",
  //                       "!H:!V:NTrees=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:VarTransform=Decorrelate" );

  // Allow Using Fisher discriminant in node splitting for (strong) linearly correlated variables
  //factory->BookMethod( TMVA::Types::kBDT, "BDTMitFisher",
  //       "!H:!V:NTrees=50:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20" );

   // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // ---- Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();

   // --------------------------------------------------------------

   // Save the output
   output_tmva->Close();

   std::cout << "==> Wrote root file: " << output_tmva->GetName() << std::endl
             << "==> TMVAClassification is done!" << std::endl
             << std::endl
             << "==> To view the results, launch the GUI: \"root -l ./TMVAGui.C\"" << std::endl
             << std::endl;

   // Clean up
   delete factory;

  return 0;
}//main
