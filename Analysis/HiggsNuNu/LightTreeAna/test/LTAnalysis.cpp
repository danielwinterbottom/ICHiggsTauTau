#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeAnalyser.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataWEst.h"
#include "boost/lexical_cast.hpp"

using namespace ic;

int main(int argc, char* argv[]){
  std::string outputname="tmp.root";
  LTAnalyser* analysis = new LTAnalyser(outputname);

  analysis->SetInFolder("../output_lighttree_lepweightscorrected/nunu/");
  analysis->SetInputParams("../filelists/Dec18/ParamsDec18test.dat");

  //Define Files
  analysis->AddFile("WJetsv1","WJets","MC_WJetsToLNu-v1.root");
  analysis->AddFile("WJetsv2","WJets","MC_WJetsToLNu-v2.root");
  analysis->AddFile("W1Jets","WJets","MC_W1JetsToLNu.root");
  analysis->AddFile("W2Jets","WJets","MC_W2JetsToLNu.root");
  analysis->AddFile("W3Jets","WJets","MC_W3JetsToLNu.root");
  analysis->AddFile("W4Jets","WJets","MC_W4JetsToLNu.root");
  analysis->AddFile("W4Jets","WJets","MC_EWK-W2jminus_enu.root");


  analysis->AddFile("WJetsv1_enu","WJets_enu","MC_WJetsToLNu-v1_enu.root");
  analysis->AddFile("WJetsv2_enu","WJets_enu","MC_WJetsToLNu-v2_enu.root");
  analysis->AddFile("W1Jets_enu","WJets_enu","MC_W1JetsToLNu_enu.root");
  analysis->AddFile("W2Jets_enu","WJets_enu","MC_W2JetsToLNu_enu.root");
  analysis->AddFile("W3Jets_enu","WJets_enu","MC_W3JetsToLNu_enu.root");
  analysis->AddFile("W4Jets_enu","WJets_enu","MC_W4JetsToLNu_enu.root");
  analysis->AddFile("EWK-Wminus_enu","WJets_enu","MC_EWK-W2jminus_enu.root");
  analysis->AddFile("EWK-Wplus_enu","WJets_enu","MC_EWK-W2jplus_enu.root");

  analysis->AddFile("WJetsv1_munu","WJets_munu","MC_WJetsToLNu-v1_munu.root");
  analysis->AddFile("WJetsv2_munu","WJets_munu","MC_WJetsToLNu-v2_munu.root");
  analysis->AddFile("W1Jets_munu","WJets_munu","MC_W1JetsToLNu_munu.root");
  analysis->AddFile("W2Jets_munu","WJets_munu","MC_W2JetsToLNu_munu.root");
  analysis->AddFile("W3Jets_munu","WJets_munu","MC_W3JetsToLNu_munu.root");
  analysis->AddFile("W4Jets_munu","WJets_munu","MC_W4JetsToLNu_munu.root");
  analysis->AddFile("EWK-Wminus_munu","WJets_munu","MC_EWK-W2jminus_munu.root");
  analysis->AddFile("EWK-Wplus_munu","WJets_munu","MC_EWK-W2jplus_munu.root");

  analysis->AddFile("WJetsv1_taunu","WJets_taunu","MC_WJetsToLNu-v1_taunu.root");
  analysis->AddFile("WJetsv2_taunu","WJets_taunu","MC_WJetsToLNu-v2_taunu.root");
  analysis->AddFile("W1Jets_taunu","WJets_taunu","MC_W1JetsToLNu_taunu.root");
  analysis->AddFile("W2Jets_taunu","WJets_taunu","MC_W2JetsToLNu_taunu.root");
  analysis->AddFile("W3Jets_taunu","WJets_taunu","MC_W3JetsToLNu_taunu.root");
  analysis->AddFile("W4Jets_taunu","WJets_taunu","MC_W4JetsToLNu_taunu.root");
  analysis->AddFile("EWK-Wminus_taunu","WJets_taunu","MC_EWK-W2jminus_taunu.root");
  analysis->AddFile("EWK-Wplus_taunu","WJets_taunu","MC_EWK-W2jplus_taunu.root");

  analysis->AddFile("TTJets","Top","MC_TTJets.root");
  analysis->AddFile("T-tW","Top","MC_T-tW.root");
  analysis->AddFile("Tbar-tW","Top","MC_Tbar-tW.root");
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
  double jet1ptcut=50;
  double jet2ptcut=50;
  double metcut=130;
  double mjjcut=1100;
  double cjvcut=1;
  double dphicut=1;
  double detacut=4.2;
  double etaprodcut=0;
  analysis->set_baseselection("passtrigger==1&& jet1_pt>"+boost::lexical_cast<std::string>(jet1ptcut)+"&& jet2_pt>"+boost::lexical_cast<std::string>(jet2ptcut)+" && dijet_M >"+boost::lexical_cast<std::string>(mjjcut)+"&& jet1_eta*jet2_eta<="+boost::lexical_cast<std::string>(etaprodcut)+" && n_jets_cjv_30<"+boost::lexical_cast<std::string>(cjvcut)+"&& dijet_dphi<"+boost::lexical_cast<std::string>(dphicut)+"&& dijet_deta >"+boost::lexical_cast<std::string>(detacut));

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
    .set_sigcat("nvetoelectrons==0 && nvetomuons==0&& met>"+boost::lexical_cast<std::string>(metcut))
    .set_contcat("nvetoelectrons==0 && nvetomuons==1 && nselmuons==1&& metnomuons>"+boost::lexical_cast<std::string>(metcut));

  DataWEst wenu("wenu");
  wenu.set_sigmcset("WJets_enu")
    .set_contmcset("WJets_enu")
    .set_contdataset("MET")
    .set_contbkgset(contbkgsets)
    .set_basesel(analysis->baseselection())
    .set_sigcat("nvetoelectrons==0 && nvetomuons==0&& met>"+boost::lexical_cast<std::string>(metcut))
    .set_contcat("nselelectrons==1 && nvetoelectrons ==1 && nvetomuons==0&& met>"+boost::lexical_cast<std::string>(metcut));


  analysis->AddModule(&wmunu);
  analysis->AddModule(&wenu);

  analysis->RunAnalysis();

  return 0;

}
