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
  std::string outputfolder;
  std::string inputfolder;
  std::string inputparams;
  std::string filelist;
  std::string settorun;
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
    ("output_folder",            po::value<std::string>(&outputfolder)->default_value("friends"))
    ("input_folder,i",           po::value<std::string>(&inputfolder)->default_value("root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/lighttree"))//"../output_lighttree"))
    ("basesel,b",           po::value<std::string>(&basesel)->default_value(""))
    ("jetmetdphicut,j",           po::value<std::string>(&jetmetdphicut)->default_value(""))
    ("input_params,p",           po::value<std::string>(&inputparams)->default_value("../filelists/Dec18/ParamsDec18test.dat"))
    ("filelist,f",               po::value<std::string>(&filelist)->default_value("filelists/filelist.dat"))
    ("settorun,r",              po::value<std::string>(&settorun)->default_value(""));
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
  po::notify(vm);

  /*##########################################
  #                                          #
  #          INSTANTIATE ANALYSER            #
  #                                          #
  ##########################################*/

  LTAnalyser* analysis = new LTAnalyser(outputfolder+"/"+outputname);

  analysis->AddFiles(filelist);

  analysis->SetInFolder(inputfolder);
  analysis->SetInputParams(inputparams);

  //Set selection step common to all categories
  analysis->set_baseselection("");

  /*##########################################
  #                                          #
  #            DEFINE MODULES                #
  #                                          #
  ##########################################*/

  std::vector<std::string> methodNames;
  methodNames.push_back("BDT");
  std::vector<std::string> weightFiles;
  weightFiles.push_back("TMVAClassification_BDT.weights.xml");
    

  std::vector<std::string> variables;
//   variables.push_back("jetmet_mindphi");
//   variables.push_back("dijetmet_ptfraction");
//   variables.push_back("dijetmet_vectorialSum_pt");
//   variables.push_back("jet1met_scalarprod_frac := jet1met_scalarprod/met");
//   variables.push_back("jet2met_scalarprod_frac := jet2met_scalarprod/met");
//   variables.push_back("n_jets_cjv_30");
//   variables.push_back("jet2_pt");
//   variables.push_back("dijet_M");
//   variables.push_back("dijet_deta");
//   variables.push_back("dijet_dphi");
//   variables.push_back("met");
//   variables.push_back("met_significance");
//   variables.push_back("sqrt(ht)");

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
  
  std::vector<std::string> sets;
  sets.push_back(settorun);
  
  MVAApply mvaapply("mvaapply");                                                                                                                         
  mvaapply.set_sets(sets)
    .set_friendDir(outputfolder)
    .set_variables(variables)
    .set_methodNames(methodNames) 
    .set_weightFiles(weightFiles);

  /*##########################################
  #                                          #
  #   SET UP ANALYSIS SEQUENCE AND RUN       #
  #                                          #
  ##########################################*/

  analysis->AddModule(&mvaapply);
  
  analysis->RunAnalysis();

  return 0;

}
