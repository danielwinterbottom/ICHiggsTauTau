#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeAnalyser.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataWEst.h"

using namespace ic;

int main(int argc, char* argv[]){
  std::string outputname="tmp.root";
  LTAnalyser* analysis = new LTAnalyser(outputname);

  analysis->SetInFolder("../output_lighttree/nunu/");
  analysis->SetInputParams("../filelists/Dec18/ParamsDec18test.dat");

  //Define Files
  analysis->AddFile("WJetsv1","WJets","MC_WJetsToLNu-v1.root");
  analysis->AddFile("WJetsv2","WJets","MC_WJetsToLNu-v2.root");
  analysis->AddFile("W1Jets","WJets","MC_W1JetsToLNu.root");
  analysis->AddFile("W2Jets","WJets","MC_W2JetsToLNu.root");
  analysis->AddFile("W3Jets","WJets","MC_W3JetsToLNu.root");
  analysis->AddFile("W4Jets","WJets","MC_W4JetsToLNu.root");

  analysis->AddFile("WJetsv1_enu","WJets_enu","MC_WJetsToLNu-v1_enu.root");
  analysis->AddFile("WJetsv2_enu","WJets_enu","MC_WJetsToLNu-v2_enu.root");
  analysis->AddFile("W1Jets_enu","WJets_enu","MC_W1JetsToLNu_enu.root");
  analysis->AddFile("W2Jets_enu","WJets_enu","MC_W2JetsToLNu_enu.root");
  analysis->AddFile("W3Jets_enu","WJets_enu","MC_W3JetsToLNu_enu.root");
  analysis->AddFile("W4Jets_enu","WJets_enu","MC_W4JetsToLNu_enu.root");

  analysis->AddFile("WJetsv1_munu","WJets_munu","MC_WJetsToLNu-v1_munu.root");
  analysis->AddFile("WJetsv2_munu","WJets_munu","MC_WJetsToLNu-v2_munu.root");
  analysis->AddFile("W1Jets_munu","WJets_munu","MC_W1JetsToLNu_munu.root");
  analysis->AddFile("W2Jets_munu","WJets_munu","MC_W2JetsToLNu_munu.root");
  analysis->AddFile("W3Jets_munu","WJets_munu","MC_W3JetsToLNu_munu.root");
  analysis->AddFile("W4Jets_munu","WJets_munu","MC_W4JetsToLNu_munu.root");

  analysis->AddFile("WJetsv1_taunu","WJets_taunu","MC_WJetsToLNu-v1_taunu.root");
  analysis->AddFile("WJetsv2_taunu","WJets_taunu","MC_WJetsToLNu-v2_taunu.root");
  analysis->AddFile("W1Jets_taunu","WJets_taunu","MC_W1JetsToLNu_taunu.root");
  analysis->AddFile("W2Jets_taunu","WJets_taunu","MC_W2JetsToLNu_taunu.root");
  analysis->AddFile("W3Jets_taunu","WJets_taunu","MC_W3JetsToLNu_taunu.root");
  analysis->AddFile("W4Jets_taunu","WJets_taunu","MC_W4JetsToLNu_taunu.root");

  analysis->AddFile("TTJets","Top","MC_TTJets.root");
  analysis->AddFile("T-tW","Top","MC_T-tW.root");
  analysis->AddFile("SingleT-s","Top","MC_SingleT-s-powheg-tauola.root");
  analysis->AddFile("SingleT-t","Top","MC_SingleT-t-powheg-tauola.root");
  analysis->AddFile("SingleTBar-s","Top","MC_SingleTBar-s-powheg-tauola.root");
  analysis->AddFile("SingleTBar-t","Top","MC_SingleTBar-t-powheg-tauola.root");

  analysis->AddFile("WW","VV","MC_WW-pythia6-tauola.root");
  analysis->AddFile("WZ","VV","MC_WZ-pythia6-tauola.root");
  analysis->AddFile("ZZ","VV","MC_ZZ-pythia6-tauola.root");
  analysis->AddFile("WGamma","VV","MC_WGamma.root");

  analysis->AddFile("DYJetsToLLHighPt","ZJets_ll","MC_DYJetsToLL_PtZ-100-madgraph.root");
  analysis->AddFile("DYJetsToLL","ZJets_ll","MC_DYJetsToLL.root");
  analysis->AddFile("DY1JetsToLL","ZJets_ll","MC_DY1JetsToLL.root");
  analysis->AddFile("DY2JetsToLL","ZJets_ll","MC_DY2JetsToLL.root");
  analysis->AddFile("DY3JetsToLL","ZJets_ll","MC_DY3JetsToLL.root");
  analysis->AddFile("DY4JetsToLL","ZJets_ll","MC_DY4JetsToLL.root");
  
  analysis->AddFile("EWK-Z2j","ZJets_ll_vbf","MC_EWK-Z2j.root");
  
  analysis->AddFile("ZJetsToNuNu50-100","ZJets_nunu","MC_ZJetsToNuNu_50_HT_100.root");
  analysis->AddFile("ZJetsToNuNu100-200","ZJets_nunu","MC_ZJetsToNuNu_100_HT_200.root");
  analysis->AddFile("ZJetsToNuNu200-400","ZJets_nunu","MC_ZJetsToNuNu_200_HT_400.root");
  analysis->AddFile("ZJetsToNuNu400-inf","ZJets_nunu","MC_ZJetsToNuNu_400_HT_inf.root");

  analysis->AddFile("META","MET","MET_MET-2012A-22Jan2013-v1.root");
  analysis->AddFile("METB","MET","MET_MET-2012B-22Jan2013-v1.root");
  analysis->AddFile("METC","MET","MET_MET-2012C-22Jan2013-v1.root");
  analysis->AddFile("METD","MET","MET_METParked-2012D-22Jan2013-v1.root");

  //Set selection step common to all categories
  analysis->set_baseselection("jet1_pt>50 && jet2_pt>50 && met>130 && dijet_M >1100 && jet1_eta*jet2_eta<=0 && n_jets_cjv_30<1 && dijet_dphi<1.0 && dijet_deta >4.2");

  //Define Modules

  std::vector<std::string> contbkgsets; //List of sets for ncbkg
  contbkgsets.push_back("VV");
  contbkgsets.push_back("Top");
  contbkgsets.push_back("ZJets_ll");
  contbkgsets.push_back("ZJets_ll_vbf");
  contbkgsets.push_back("ZJets_nunu");

  DataWEst wmunu("wmunu");
  wmunu.set_sigmcset("WJets_munu")
    .set_contmcset("WJets_munu")
    .set_contdataset("MET")
    .set_contbkgset(contbkgsets)
    .set_basesel(analysis->baseselection())
    .set_sigcat("nvetoelectrons==0 && nvetomuons==0")
    .set_contcat("nvetoelectrons==0 && nselmuons!=0");

  analysis->AddModule(&wmunu);

  analysis->RunAnalysis();

  return 0;

}
