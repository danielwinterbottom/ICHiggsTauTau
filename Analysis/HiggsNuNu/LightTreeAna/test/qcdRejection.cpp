#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeAnalyser.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/SimplePlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/MVATrain.h"
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
  
  //Base selection cuts
  double jet1ptcut;
  double jet2ptcut;
  double metsigcut;
  double detajjcut;
  double mindphicut;

  po::options_description preconfig("Configuration"); 
  preconfig.add_options()("cfg",po::value<std::string>(&cfg)->required());
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
    //Input output and config options
    ("output_name,o",            po::value<std::string>(&outputname)->default_value("tmp.root"))
    ("input_folder,i",           po::value<std::string>(&inputfolder)->default_value("../output_lighttree_withrle2/"))
    ("input_params,p",           po::value<std::string>(&inputparams)->default_value("../filelists/Dec18/ParamsDec18.dat"))
    ("filelist,f",               po::value<std::string>(&filelist)->default_value("filelists/filelist.dat"))
    //Base selection cuts
    ("jet1ptcut",                po::value<double>(&jet1ptcut)->default_value(30))
    ("jet2ptcut",                po::value<double>(&jet2ptcut)->default_value(30))
    ("metsigcut",                po::value<double>(&metsigcut)->default_value(3))
    ("mindphicut",               po::value<double>(&mindphicut)->default_value(1.5))
    ("detajjcut",                po::value<double>(&detajjcut)->default_value(3.6));
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
  analysis->set_baseselection("nvetoelectrons==0 && nvetomuons==0 && passtrigger==1 && jet1_pt>"+boost::lexical_cast<std::string>(jet1ptcut)+"&& jet2_pt>"+boost::lexical_cast<std::string>(jet2ptcut));

  /*##########################################
  #                                          #
  #            DEFINE MODULES                #
  #                                          #
  ##########################################*/

  std::vector<std::string> ewksets; //List of sets for ewk
  ewksets.push_back("VV");
  ewksets.push_back("Top");
  ewksets.push_back("ZJets_ll");
  ewksets.push_back("ZJets_ll_vbf");
  ewksets.push_back("ZJets_nunu");
  ewksets.push_back("WJets_enu");
  ewksets.push_back("WJets_munu");
  ewksets.push_back("WJets_taunu");

  std::vector<std::string> shapes; //List of shapes to draw
  //shapes.push_back("dijet_M(370,150.,2000.)");
  shapes.push_back("dijet_deta(160,0.,10.)");
  //shapes.push_back("dijet_dphi(310,0.,3.1)");
  //shapes.push_back("met(80,0.,400.)");
  //shapes.push_back("met_significance(100,0.,20.)");
  //shapes.push_back("jetmet_mindphi(100,0.,3.1416)");
  //shapes.push_back("cjvjetpt(100,0.,100.)");

  SimplePlots plots("plots");
  plots.set_qcdset("VBF-QCD")
    .set_sigset("sig125")
    .set_ewkset(ewksets)
    .set_dataset("MET")
    .set_cat("")
    .set_basesel(analysis->baseselection())
    .set_shapes(shapes);

  shapes.clear();
  shapes.push_back("met_significance(100,0.,20.)");

  SimplePlots afterDeta("afterDeta");
  afterDeta.set_qcdset("VBF-QCD")
    .set_sigset("sig125")
    .set_ewkset(ewksets)
    .set_dataset("MET")
    .set_cat("dijet_deta >"+boost::lexical_cast<std::string>(detajjcut))
    .set_basesel(analysis->baseselection())
    .set_shapes(shapes);

  shapes.clear();
  shapes.push_back("jetmet_mindphi(100,0.,3.5)");

  SimplePlots afterMetsig("afterMetsig");
  afterMetsig.set_qcdset("VBF-QCD")
    .set_sigset("sig125")
    .set_ewkset(ewksets)
    .set_dataset("MET")
    .set_cat("dijet_deta >"+boost::lexical_cast<std::string>(detajjcut)+" && met_significance >"+boost::lexical_cast<std::string>(metsigcut))
    .set_basesel(analysis->baseselection())
    .set_shapes(shapes);

  /*##########################################
  #                                          #
  #   SET UP ANALYSIS SEQUENCE AND RUN       #
  #                                          #
  ##########################################*/

  analysis->AddModule(&plots);
  analysis->AddModule(&afterDeta);
  analysis->AddModule(&afterMetsig);

  analysis->RunAnalysis();

  return 0;

}
