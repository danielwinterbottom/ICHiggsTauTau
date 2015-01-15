#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeAnalyser.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataWEst.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataQCDEst.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/NormPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/MVATrain.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/MVAApply.h"
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"

namespace po=boost::program_options;
using namespace ic;

int main(int argc, char* argv[]){
  /*##########################################
  #                                          #
  #            SET UP OPTIONS                #
  #                                          #
  ##########################################*/

  //Input output and config options
  std::string cfg;
  std::string outputname;
  std::string inputfolder;
  std::string inputparams;
  std::string filelist;
  std::string basesel;
  std::string jetmetdphicut;
  
  po::options_description preconfig("Configuration"); 
  preconfig.add_options()("cfg",po::value<std::string>(&cfg)->required());
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
    //Input output and config options
    ("output_name,o",            po::value<std::string>(&outputname)->default_value("tmp.root"))
    ("input_folder,i",           po::value<std::string>(&inputfolder)->default_value("../output_lighttree_topreweighted"))//"root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/lighttree/"))
    ("input_params,p",           po::value<std::string>(&inputparams)->default_value("../filelists/Dec18/ParamsDec18.dat"))
    ("basesel,b",                po::value<std::string>(&basesel)->default_value(""))
    ("jetmetdphicut,j",                po::value<std::string>(&jetmetdphicut)->default_value(""))
    ("filelist,f",               po::value<std::string>(&filelist)->default_value("filelists/filelist.dat"));
    //Base selection cuts
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
  po::notify(vm);

  /*##########################################
  #                                          #
  #          INSTANTIATE ANALYSER            #
  #                                          #
  ##########################################*/

  LTAnalyser* analysis = new LTAnalyser(outputname);

  analysis->AddFiles(filelist);

  analysis->SetInFolder(inputfolder);
  analysis->SetInputParams(inputparams);

  //Set selection step common to all categories
  analysis->set_baseselection("");//passtrigger==1&& jet1_pt>"+boost::lexical_cast<std::string>(jet1ptcut)+"&& jet2_pt>"+boost::lexical_cast<std::string>(jet2ptcut)+" && dijet_M >"+boost::lexical_cast<std::string>(mjjcut)+"&& jet1_eta*jet2_eta<="+boost::lexical_cast<std::string>(etaprodcut)+"&& dijet_dphi<"+boost::lexical_cast<std::string>(dphicut)+"&& dijet_deta >"+boost::lexical_cast<std::string>(detacut));

  /*##########################################
  #                                          #
  #            DEFINE MODULES                #
  #                                          #
  ##########################################*/

  //MVA TRAIN
  std::vector<std::string> sigsets;
  sigsets.push_back("sig125");
  std::vector<std::string> bkgsets;
  //bkgsets.push_back("VBF-QCD");
  bkgsets.push_back("VV");
  bkgsets.push_back("Top");
  bkgsets.push_back("ZJets_ll_iglep");
  bkgsets.push_back("ZJets_ll_vbf_iglep");
  //bkgsets.push_back("ZJets_nunu");
  bkgsets.push_back("WJets_enu");
  bkgsets.push_back("WJets_munu");
  bkgsets.push_back("WJets_taunu");
  std::vector<std::string> bkgweightdir;
  bkgweightdir.push_back("");
  bkgweightdir.push_back("top");
  bkgweightdir.push_back("zvv");
  bkgweightdir.push_back("zvv");
  bkgweightdir.push_back("wel");
  bkgweightdir.push_back("wmu");
  bkgweightdir.push_back("wtau");
  std::vector<int> bkgisz;
  bkgisz.push_back(0);
  bkgisz.push_back(0);
  bkgisz.push_back(2);
  bkgisz.push_back(1);
  bkgisz.push_back(0);
  bkgisz.push_back(0);
  bkgisz.push_back(0);
  std::vector<std::string> variables;
//   variables.push_back("alljetsmetnomu_mindphi");
   variables.push_back("dijetmetnomu_ptfraction");
   variables.push_back("dijetmetnomu_vectorialSum_pt");
//   variables.push_back("dijetmetnomu_scalarSum_pt");
//   //variables.push_back("jet1metnomu_scalarprod_frac := jet1metnomu_scalarprod/met");
//   //variables.push_back("jet2metnomu_scalarprod_frac := jet2metnomu_scalarprod/met");
   variables.push_back("n_jets_cjv_30");
//   variables.push_back("jet1_pt");
//   variables.push_back("jet2_pt");
   variables.push_back("dijet_M");
//   variables.push_back("dijet_deta");
//   variables.push_back("dijet_sumeta");
//   variables.push_back("dijet_dphi");
   variables.push_back("metnomuons");
   variables.push_back("metnomu_significance");
   variables.push_back("sqrt(ht)");
//   variables.push_back("ht");
   variables.push_back("jetunclet_mindphi");
//   variables.push_back("metnomuunclet_dphi");
  



  std::vector<std::string> specvariables;

  MVATrain mvatrainer("mvatrainer");
  mvatrainer.set_sigsets(sigsets)
    .set_bkgsets(bkgsets)
    .set_datadrivenweightsfile("output_sigreg_greq/nunu.root")
    .set_bkgweightdir(bkgweightdir)
    .set_bkgisz(bkgisz)
    .set_variables(variables)
    .set_specvariables(specvariables)
    .set_basesel(basesel+"&&"+jetmetdphicut)//"jet1_eta*jet2_eta<0 && jet1_eta<4.7 && jet2_eta<4.7 && dijet_M>=1000&&jet1_pt>50&&dijet_deta>3.6&& jet2_pt>40&&metnomuons>90&&metnomu_significance>4.0&&alljetsmetnomu_mindphi>2.5&&nvetomuons==0&&nvetoelectrons==0"+jetmetdphicut)
       //("nvetomuons==0&&nvetoelectrons==0&&jet1_pt>50&&jet2_pt>50&&jet1_eta<4.7&&jet2_eta<4.7&&metnomu_significance>3&&dijet_deta>3.6&&jetmetnomu_mindphi>1.5")
    .set_sigcat("")
    .set_bkgcat("");

  DataWEst premvaest("premvaest");
    premvaest.set_sigmcset("sig125")
    .set_contmcset("sig125")
    .set_contdataset("sig125")
    .set_contbkgset(bkgsets)
    .set_basesel("nvetomuons==0&&nvetoelectrons==0&&jet1_pt>50&&jet2_pt>50&&jet1_eta<4.7&&jet2_eta<4.7&&metnomu_significance>3&&dijet_deta>3.6&&jetmetnomu_mindphi>1.5")
      .set_sigcat("")
      .set_contcat("");
  

  /*##########################################
  #                                          #
  #   SET UP ANALYSIS SEQUENCE AND RUN       #
  #                                          #
  ##########################################*/

    //  analysis->AddModule(&premvaest);
  analysis->AddModule(&mvatrainer);

  analysis->RunAnalysis();

  return 0;

}
