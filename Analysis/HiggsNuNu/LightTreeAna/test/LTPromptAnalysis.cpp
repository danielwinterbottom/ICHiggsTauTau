#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeAnalyser.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/NormPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataWEst.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataZEst.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataQCDEst.h"
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
  double metcut;
  double mjjcut;
  double cjvcut;
  double dphicut;
  double detacut;
  double etaprodcut;

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
    ("input_params,p",           po::value<std::string>(&inputparams)->default_value("../filelists/Dec18/ParamsDec18test.dat"))
    ("filelist,f",               po::value<std::string>(&filelist)->default_value("filelists/filelist.dat"))
    //Base selection cuts
    ("jet1ptcut",                po::value<double>(&jet1ptcut)->default_value(50))
    ("jet2ptcut",                po::value<double>(&jet2ptcut)->default_value(50))
    ("metcut",                   po::value<double>(&metcut)->default_value(130))
    ("mjjcut",                   po::value<double>(&mjjcut)->default_value(1100))
    ("cjvcut",                   po::value<double>(&cjvcut)->default_value(1))
    ("dphicut",                  po::value<double>(&dphicut)->default_value(1))
    ("detacut",                  po::value<double>(&detacut)->default_value(4.2))
    ("etaprodcut",               po::value<double>(&etaprodcut)->default_value(0));
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
  analysis->set_baseselection("passtrigger==1&&jet1_eta<4.7&&jet2_eta<4.7&& jet1_pt>"+boost::lexical_cast<std::string>(jet1ptcut)+"&& jet2_pt>"+boost::lexical_cast<std::string>(jet2ptcut)+" && dijet_M >"+boost::lexical_cast<std::string>(mjjcut)+"&& jet1_eta*jet2_eta<"+boost::lexical_cast<std::string>(etaprodcut)+"&& dijet_dphi<"+boost::lexical_cast<std::string>(dphicut)+"&& dijet_deta >"+boost::lexical_cast<std::string>(detacut));
    //analysis->set_baseselection("passtrigger==1&&BDT>-0.25&&jet1_pt>50&&jet2_pt>50&&dijet_deta>3.6&&met_significance>3&&jetmet_mindphi>1.5");
  /*##########################################
  #                                          #
  #            DEFINE MODULES                #
  #                                          #
  ##########################################*/

  //WBKG
  std::vector<std::string> Wcontbkgsets; //List of sets for ncbkg
  Wcontbkgsets.push_back("VV");
  Wcontbkgsets.push_back("Top");
  Wcontbkgsets.push_back("ZJets_ll");
  Wcontbkgsets.push_back("ZJets_ll_vbf");
  Wcontbkgsets.push_back("ZJets_nunu");

  DataWEst wmunu("wmunu");
  wmunu.set_sigmcset("WJets_munu")
    .set_contmcset("WJets_munu")
    .set_contdataset("MET")
    .set_contbkgset(Wcontbkgsets)
    .set_basesel(analysis->baseselection())
    .set_sigcat("nvetoelectrons==0 && nvetomuons==0&& met>"+boost::lexical_cast<std::string>(metcut)+" && n_jets_cjv_30<"+boost::lexical_cast<std::string>(cjvcut))
    .set_contcat("nvetoelectrons==0 && nvetomuons==1 && nselmuons==1&& metnomuons>"+boost::lexical_cast<std::string>(metcut)+" && n_jets_cjv_30<"+boost::lexical_cast<std::string>(cjvcut));

  DataWEst wenu("wenu");
  wenu.set_sigmcset("WJets_enu")
    .set_contmcset("WJets_enu")
    .set_contdataset("MET")
    .set_contbkgset(Wcontbkgsets)
    .set_basesel(analysis->baseselection())
    .set_sigcat("nvetoelectrons==0 && nvetomuons==0&& met>"+boost::lexical_cast<std::string>(metcut)+" && n_jets_cjv_30<"+boost::lexical_cast<std::string>(cjvcut))
    .set_contcat("nselelectrons==1 && nvetoelectrons ==1 && nvetomuons==0&& met>"+boost::lexical_cast<std::string>(metcut)+" && n_jets_cjv_30<"+boost::lexical_cast<std::string>(cjvcut));

  DataWEst wtaunu("wtaunu");
  wtaunu.set_sigmcset("WJets_taunu")
    .set_contmcset("WJets_taunu")
    .set_contdataset("MET")
    .set_contbkgset(Wcontbkgsets)
    .set_basesel(analysis->baseselection())
    .set_sigcat("nvetoelectrons==0 && nvetomuons==0&& met>"+boost::lexical_cast<std::string>(metcut)+" && n_jets_cjv_30<"+boost::lexical_cast<std::string>(cjvcut))
    .set_contcat("ntaus>=1&&nvetoelectrons ==0 && nvetomuons==0&& met>"+boost::lexical_cast<std::string>(metcut))
    .set_sigmcweight("total_weight_lepveto")
    .set_contmcweight("total_weight_lepveto")
    .set_contdataweight("weight_nolep");


  //ZBKG
  std::vector<std::string> Zcontbkgsets;//List of sets for ncbkg !!FILL THIS
  Zcontbkgsets.push_back("VV");
  Zcontbkgsets.push_back("Top");
  Zcontbkgsets.push_back("WJets_enu");
  Zcontbkgsets.push_back("WJets_munu");
  Zcontbkgsets.push_back("WJets_taunu");
  
  DataZEst zmumu("zmumu");
  zmumu.set_sigmcewkset("ZJets_ll_vbf")
    .set_sigmcqcdset("ZJets_ll")
    .set_contmcewkset("ZJets_ll_vbf")
    .set_contmcqcdset("ZJets_ll")
   .set_contbkgset(Zcontbkgsets)
    .set_contdataset("MET")
    .set_basesel(analysis->baseselection())
    .set_sigcat("metnomuons>"+boost::lexical_cast<std::string>(metcut)+" && n_jets_cjv_30<"+boost::lexical_cast<std::string>(cjvcut)+"&&m_mumu_gen>80&&m_mumu_gen<100")//!!MAKE GEN WINDOW CONFIGURABLE WITH WARNING ABOUT GEN MASS FILTERED NUMBER BELOW NOT CHANGING
    .set_contcat("nvetoelectrons==0 && nvetomuons==2 && nselmuons==2&& metnomuons>"+boost::lexical_cast<std::string>(metcut)+" && n_jets_cjv_30<"+boost::lexical_cast<std::string>(cjvcut)+"&&m_mumu>60&&m_mumu<120")//!!MAKE GEN WINDOW CONFIGURABLE
    .set_sigmainccontewk(303)
    .set_sigmainccontqcd(3503700./3)
    .set_sigmaincsigewk(460*3)
    .set_sigmaincsigqcd(3503700./3*5.651)
    .set_ngenincewk(5781.91)
    .set_ngenincqcd(22789300)
    .set_ngenmassfilteredewk(4226.53)
    .set_ngenmassfilteredqcd(20334900);
    
    

  //QCDBKG
  std::vector<std::string> QCDcontbkgsets; //list of sets for ncbkg
  QCDcontbkgsets.push_back("VV");
  QCDcontbkgsets.push_back("Top");
  QCDcontbkgsets.push_back("ZJets_ll");
  QCDcontbkgsets.push_back("ZJets_ll_vbf");
  QCDcontbkgsets.push_back("ZJets_nunu");
  QCDcontbkgsets.push_back("WJets_enu");
  QCDcontbkgsets.push_back("WJets_munu");
  QCDcontbkgsets.push_back("WJets_taunu");
  
  DataQCDEst QCD("QCD");
  QCD.set_Aset("MET")
    .set_Bset("MET")
    .set_Cset("MET")
    .set_Abkgset(QCDcontbkgsets)
    .set_Bbkgset(QCDcontbkgsets)
    .set_Cbkgset(QCDcontbkgsets)
    .set_basesel(analysis->baseselection())
    .set_Acat("nvetoelectrons==0 && nvetomuons==0&& met<="+boost::lexical_cast<std::string>(metcut)+" && n_jets_cjv_30>="+boost::lexical_cast<std::string>(cjvcut))
    .set_Bcat("nvetoelectrons==0 && nvetomuons==0&& met>"+boost::lexical_cast<std::string>(metcut)+" && n_jets_cjv_30>="+boost::lexical_cast<std::string>(cjvcut))
    .set_Ccat("nvetoelectrons==0 && nvetomuons==0&& met<="+boost::lexical_cast<std::string>(metcut)+" && n_jets_cjv_30<"+boost::lexical_cast<std::string>(cjvcut));


  //NORMALISED PLOTS FOR REFEREE
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
  shapes.push_back("dijet_M(370,150.,2000.)");
  shapes.push_back("dijet_deta(160,0.,8.)");
  shapes.push_back("dijet_dphi(310,0.,3.1)");
  shapes.push_back("met(80,0.,400.)");
  shapes.push_back("cjvjetpt(100,0.,100.)");

  NormPlots normplots("normplots");
  normplots.set_qcdset("QCD")
    .set_sigset("sig125")
    .set_ewkset(ewksets)
    .set_cat("")
    .set_basesel("jet1_eta<4.7&&jet2_eta<4.7&&jet1_pt>50&&jet2_pt>50&&nvetoelectrons==0 && nvetomuons==0&&dijet_M>150&&met>130")
    .set_shapes(shapes);


  /*##########################################
  #                                          #
  #   SET UP ANALYSIS SEQUENCE AND RUN       #
  #                                          #
  ##########################################*/
  
   analysis->AddModule(&wmunu);
   analysis->AddModule(&wenu);
   analysis->AddModule(&wtaunu);
   analysis->AddModule(&zmumu);
   analysis->AddModule(&QCD);
   
   analysis->RunAnalysis();
   
   return 0;

}
