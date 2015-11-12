#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeAnalyser.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataNormShape.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataZNormShape.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataShape.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataZEst.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/NormPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/SimplePlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/MVATrain.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/AddFriends.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/Plotter.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/HistPlotter.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/SummaryTable.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/EventList.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/deltaphi.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/BkgSubDataNormShape.h"
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "TColor.h"
#include "TMath.h"

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
  std::string channel;
  std::string syst;
  std::string dataset;

  bool do_debug;
  bool do_datatop=false;
  bool do_singletop=false;
//  bool do_separatez;
  bool runblind;
  bool runblindreg=false;
  bool blindcutreg=false;
  bool do_bdt = false;
  std::string bdtcut;
  bool do_pasfig = false;
 // bool do_pasqcdfigleft;
  //bool do_preselranges;
  bool do_prepreselranges=false;
 // bool do_relaxedqcdranges;
  bool do_plotmcqcd=false;
  //bool do_plotqcd;
  bool dataonly;
  //bool datalist;
  //bool do_list;
  bool do_nosigmcweight;
  std::string listset;
  bool do_latex=false;
  bool do_expandtopcat=false;
  bool do_logy;
  std::string contonlycontplotjetmetdphi;

  po::options_description preconfig("Configuration"); 
  preconfig.add_options()("cfg",po::value<std::string>(&cfg)->required());
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
    //Input output and config options
    ("output_name,o",            po::value<std::string>(&outputname)->default_value("tmp.root"))
    ("input_folder,i",           po::value<std::string>(&inputfolder)->default_value(""))
    ("syst,s",                   po::value<std::string>(&syst)->default_value(""))
    ("input_params,p",           po::value<std::string>(&inputparams)->default_value("../filelists/May20/ParamsMay20test.dat"))
    ("filelist,f",               po::value<std::string>(&filelist)->default_value("filelists/run2filelist.dat"))
    ("basesel",                  po::value<std::string>(&basesel)->default_value("jet1_eta*jet2_eta<0 && jet1_eta<4.7 && jet2_eta<4.7 && dijet_M>=600&&jet1_pt>50&&dijet_deta>3.6&& jet2_pt>60&&metnomuons>60&&metnomu_significance>3&&jetmetnomu_mindphi>1.5"))
    ("jetmetdphicut",            po::value<std::string>(&jetmetdphicut)->default_value("alljetsmetnomu_mindphi>1.0"))
    ("channel",                  po::value<std::string>(&channel)->default_value("nunu"))
    ("runblind",                 po::value<bool>(&runblind)->default_value(true))
    ("do_logy",                  po::value<bool>(&do_logy)->default_value(false))
    ("do_debug",                  po::value<bool>(&do_debug)->default_value(false))
    ("do_nosigmcweight",         po::value<bool>(&do_nosigmcweight)->default_value(false));

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
  if(syst!="PUUP"&&syst!="PUDOWN"){
    std::cout<<"Taking input from: "<<inputfolder<<"/"<<syst<<std::endl;
    analysis->SetInFolder(inputfolder+"/"+syst);
  }
  else{
    std::cout<<"Taking input from: "<<inputfolder<<std::endl;
    analysis->SetInFolder(inputfolder);
  }
  analysis->SetInputParams(inputparams);

  std::cout<<"Base selection: "<<basesel<<std::endl;

  analysis->set_baseselection(basesel);
  
  /*##########################################
  #                                          #
  #            DEFINE MODULES                #
  #                                          #
  ##########################################*/

  std::vector<std::string> histTitle;
  std::vector<std::string> shape;
  shape.reserve(100);
  //shape.push_back("BDT(12,-1.,0.2)");

  //shape.push_back("n_jets_30(10,0.,10.)");histTitle.push_back(";n_{jets} (p_{T}>30 GeV);Events");
  if(true){
    shape.push_back("jet2_pt(26,30.,300.)");histTitle.push_back(";p_{T}^{j2} (GeV);Events");
    shape.push_back("jet1_pt(30,30.,350.)");histTitle.push_back(";p_{T}^{j1} (GeV);Events");
    shape.push_back("jet2_eta(50,-5.,5.)");histTitle.push_back(";#eta_{j2};Events");
    shape.push_back("jet1_eta(50,-5.,5.)");histTitle.push_back(";#eta_{j1};Events");
    shape.push_back("jet1_phi(16,-3.2,3.2)");histTitle.push_back(";#phi_{j1};Events");
    shape.push_back("jet2_phi(16,-3.2,3.2)");histTitle.push_back(";#phi_{j2};Events");
    shape.push_back("cjvjetpt(27,15.,150.)");histTitle.push_back(";p_{T}^{CJV jet} (GeV);Events");
    shape.push_back("metnomuons(30,0.,800.)");histTitle.push_back(";Missing transverse energy (GeV);Events");
    shape.push_back("met(30,0.,800.)");histTitle.push_back(";MET (GeV);Events");
    shape.push_back("l1met(20,0.,200.)");histTitle.push_back(";L1MET (GeV);Events");
    shape.push_back("dijet_M(30,0.,3000.)");histTitle.push_back(";M_{jj} (GeV);Events");
    shape.push_back("jetmetnomu_mindphi(31,0,3.142)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j_{1,2});Events");
    shape.push_back("alljetsmetnomu_mindphi(31,0,3.1416)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j);Events");
    shape.push_back("metnomu_significance(100,0.,100.)");histTitle.push_back(";S;Events");
    shape.push_back("met*pow(sqrt(sumet),-1)(100,0.,100.)");histTitle.push_back(";Run 1 S;Events");
    shape.push_back("dijet_sumeta(50,-10,10)");histTitle.push_back(";#eta_{j1}+#eta_{j2};Events");
    shape.push_back("ht(50,0,1000)");histTitle.push_back(";H_{T} (GeV);Events");
    shape.push_back("jetunclet_mindphi(32,0,3.1416)");histTitle.push_back(";min #Delta#phi(j,E_{T}^{uncl});Events");
    shape.push_back("metnomuunclet_dphi(32,0,3.1416)");histTitle.push_back(";#Delta#phi(METnoMu,E_{T}^{uncl};Events");
    shape.push_back("dijetmetnomu_scalarSum_pt(35,0,1400)");histTitle.push_back(";p_{T}^{jeta}+p_{T}^{jetb}+METnoMu;Events");
    shape.push_back("dijetmetnomu_vectorialSum_pt(20,0,400)");histTitle.push_back(";p_{T}(#vec{ja}+#vec{jb}+#vec{METnoMu});Events");
    shape.push_back("n_jets_cjv_30(5,0,5)");histTitle.push_back(";CJV jets (30 GeV);Events");
    shape.push_back("n_jets_cjv_20EB_30EE(5,0,5)");histTitle.push_back(";CJV jets (20 GeV EB, 30 GeV EE);Events");
    shape.push_back("dijet_dphi(15,0.,3.1416)");histTitle.push_back(";#Delta#phi_{jj};Events");
    shape.push_back("dijet_deta(50,0.,10.)");histTitle.push_back(";#Delta#eta_{jj};Events");
    shape.push_back("lep_mt(20,0.,200.)");histTitle.push_back(";m_{T}(lepton+MET) (GeV);Events");
    shape.push_back("dijetmetnomu_ptfraction(20,0.,1.)");histTitle.push_back(";p_{T}^{dijet}/(p_{T}^{dijet}+METnoMu);Events");
    //    shape.push_back("ele1_pt(40,0.,200.)");histTitle.push_back(";p_{T}(electron) (GeV);Events");
    //    shape.push_back("mu1_pt(40,0.,200.)");histTitle.push_back(";p_{T}(muon) (GeV);Events");
    shape.push_back("jet_csv1(21,0.6,1.5)");histTitle.push_back(";Jet 1 CSV;Events");
    shape.push_back("jet_csv2(21,0.6,1.5)");histTitle.push_back(";Jet 2 CSV;Events");
    shape.push_back("jet_csv3(21,0.,1.)");histTitle.push_back(";Jet 3 CSV;Events");
    shape.push_back("n_jets_30(10,0.,10.)");histTitle.push_back(";N jets pt>30 GeV;Events");
    shape.push_back("n_jets_15(15,0.,15.)");histTitle.push_back(";N jets pt>15 GeV;Events");
    shape.push_back("central_tag_eta(25,-5.,5.)");histTitle.push_back(";Central tag jet #eta;Events");
    shape.push_back("forward_tag_eta(25,-5.,5.)");histTitle.push_back(";Forward tag jet #eta;Events");
    shape.push_back("genjet2_pt(30,0.,300.)");histTitle.push_back(";gen p_{T}^{j2} (GeV);Events");
    shape.push_back("genjet1_pt(30,0.,350.)");histTitle.push_back(";gen p_{T}^{j1} (GeV);Events");
    shape.push_back("genjet2_eta(50,-5.,5.)");histTitle.push_back(";gen #eta_{j2};Events");
    shape.push_back("genjet1_eta(50,-5.,5.)");histTitle.push_back(";gen #eta_{j1};Events");
    shape.push_back("genjet2_phi(16,-3.2,3.2)");histTitle.push_back(";gen #phi_{j2};Events");
    shape.push_back("genjet1_phi(16,-3.2,3.2)");histTitle.push_back(";gen #phi_{j1};Events");
    shape.push_back("digenjet_M(30,0.,3000.)");histTitle.push_back(";M_{jj} (GeV);Events");
    shape.push_back("digenjet_dphi(15,0.,3.1416)");histTitle.push_back(";#Delta#phi_{jj};Events");
    shape.push_back("digenjet_deta(50,0.,10.)");histTitle.push_back(";#Delta#eta_{jj};Events");
    shape.push_back("genjet3_pt(35,0.,350.)");histTitle.push_back(";gen p_{T}^{j3} (GeV);Events");
    shape.push_back("genjet3_eta(50,-5.,5.)");histTitle.push_back(";gen #eta_{j3};Events");
    shape.push_back("genjet3_phi(16,-3.2,3.2)");histTitle.push_back(";gen #phi_{j3};Events");


    // if(channel=="mumu"){
    //   shape.push_back("m_mumu(30,0.,150.)");histTitle.push_back(";m_{#mu#mu};Events");
    // }
  }
  
  std::string dataextrasel;
  dataextrasel="&&((((run>=190456)&&(run<=193621))&&passtrigger==1)||(((run>=193833)&&(run<=203742))&&passparkedtrigger1==1)||(((run>=203777)&&(run<=208686))&&passparkedtrigger2==1))&&l1met>=40";
  //std::string dataextrasel="&&((((run>=190456)&&(run<=193621))&&passtrigger==1)||(((run>=193833)&&(run<=196531))&&passparkedtrigger1==1)||(((run>=203777)&&(run<=208686))&&passparkedtrigger2==1))";
  //std::string dataextrasel="&&(((run>=190456)&&(run<=193621)&&passtrigger==1)||(passparkedtrigger1==1)||(passparkedtrigger2==1))&&l1met>40";
  //std::string dataextrasel="&&passtrigger==1&&l1met>40";
  //std::string dataextrasel="";
  std::string sigcat;
  std::string zextrasigcat;

  std::string nunucat;
  std::string nunuzcat;
  if(do_bdt){
    //    nunucat=("BDT>-0.26&&nvetomuons==0&&nvetoelectrons==0&&"+jetmetdphicut);
    //    nunuzcat="&&BDT>-0.26&&"+jetmetdphicut;
    nunucat=(bdtcut+"&&nvetomuons==0&&nvetoelectrons==0&&"+jetmetdphicut);
    nunuzcat="&&"+bdtcut+"&&"+jetmetdphicut;
  }
  else{
    nunucat=("nvetomuons==0&&nvetoelectrons==0&&"+jetmetdphicut);//+"&&jet3_pt>30&&jet_csv1>0.679&&jet_csv2>0.679");//CSVT: 0.898
    nunuzcat="&&"+jetmetdphicut;//+"&&jet3_pt>30&&jet_csv1>0.679&&jet_csv2>0.679";
    //nunucat=("nvetomuons==0&&nvetoelectrons==0&&"+jetmetdphicut+"&&ntaus==0");
    //nunuzcat="&&"+jetmetdphicut+"&&ntaus==0";
  }
  
  std::string qcdbasecat;
    std::string qcdshapecat,zextraqcdshapecat;
  if(do_pasfig){
    qcdbasecat="alljetsmetnomu_mindphi<1.0&&jetmetnomu_mindphi>1.0";
  }
  else{
    qcdbasecat="alljetsmetnomu_mindphi<1.0&&jetmetnomu_mindphi>2.3";
  }
  qcdshapecat=("nvetomuons==0&&nvetoelectrons==0&&"+qcdbasecat);
  zextraqcdshapecat=("&&"+qcdbasecat);

  std::string mumuqcdcat="nselmuons==2&&nvetomuons==2&&nvetoelectrons==0&&m_mumu>60&&m_mumu<120&&"+qcdbasecat;
  std::string mumuqcdzcat="&&nselmuons==2&&nvetomuons==2&&m_mumu>60&&m_mumu<120&&"+qcdbasecat;//zmumu
  std::string munuqcdcat="mu1_pt>25&&nselmuons==1&&nvetomuons==1&&nvetoelectrons==0&&"+qcdbasecat;//&&lep_mt>10";
  std::string munuqcdzcat="&&mu1_pt>25&&nselmuons==1&&nvetomuons==1&&nvetoelectrons==0&&m_mumu>60&&m_mumu<120&&"+qcdbasecat;//wmu
  std::string enuqcdcat="nselelectrons==1&&nvetomuons==0&&nvetoelectrons==1&&"+qcdbasecat;
  std::string enuqcdzcat="&&nselelectrons==1&&nvetoelectrons==1&&"+qcdbasecat;//wel
  std::string taunuqcdcat="ntaus==1&&nvetomuons==0&&nvetoelectrons==0&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;
  std::string taunuqcdzcat="&&ntaus==1&&nvetoelectrons==0&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;//wtau  

  std::string mumucat="nselmuons==2&&nvetomuons==2&&nvetoelectrons==0&&m_mumu>60&&m_mumu<120&&"+jetmetdphicut;
  std::string mumuzcat="&&nselmuons==2&&nvetomuons==2&&m_mumu>60&&m_mumu<120&&"+jetmetdphicut;//zmumu

  std::string munucat,munuzcat;
  munucat="nselmuons==1&&nvetomuons==1&&nvetoelectrons==0&&"+jetmetdphicut;//&&lep_mt>10";
  munuzcat="&&nselmuons==1&&nvetomuons==1&&nvetoelectrons==0&&m_mumu>60&&m_mumu<120&&"+jetmetdphicut;//wmu
  

  std::string enucat="nselelectrons==1&&nvetomuons==0&&nvetoelectrons==1&&"+jetmetdphicut;
  std::string enuzcat="&&nselelectrons==1&&nvetoelectrons==1&&"+jetmetdphicut;//wel

  std::string taunucat;
  if(!do_prepreselranges)taunucat="ntaus==1&&nvetomuons==0&&nvetoelectrons==0&&lep_mt>20&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;
  else taunucat="ntaus==1&&nvetomuons==0&&nvetoelectrons==0&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;
  std::string taunuzcat;
  if(!do_prepreselranges)taunuzcat="&&ntaus==1&&nvetoelectrons==0&&lep_mt>20&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;//wtau
  else taunuzcat="&&ntaus==1&&nvetoelectrons==0&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;//wtau


  std::string topcat,topzcat;
  if(do_expandtopcat){
    topcat="((nvetomuons==1&&nvetoelectrons==1&&nselmuons==1&&nselelectrons==1)||((m_mumu<60||m_mumu>120)&&nvetoelectrons==0&&nselmuons==2&&nvetomuons==2))";//&&jetmetnomu_mindphi>1.0";
    topzcat="&&((nvetomuons==1&&nvetoelectrons==1&&nselmuons==1&&nselelectrons==1)||((m_mumu<60||m_mumu>120)&&nvetoelectrons==0&&nselmuons==2&&nvetomuons==2))";//&&jetmetnomu_mindphi>1.0";//top
  }
  else{
    if(!do_singletop){
      topcat="nvetomuons==1&&nvetoelectrons==1&&nselmuons==1&&nselelectrons==1";//&&jetmetnomu_mindphi>1.0";
      topzcat="&&nvetomuons==1&&nvetoelectrons==1&&nselmuons==1&&nselelectrons==1";//&&jetmetnomu_mindphi>1.0";//top
    }
    else{
      topcat="((nvetomuons>=1&&nselmuons>=1)||(nvetoelectrons>=1&&nselelectrons>=1))&&jet_csv1>0.679&&(forward_tag_eta>2.8||forward_tag_eta<-2.8)";//&&jetmetnomu_mindphi>1.0";
      topzcat="&&((nvetomuons>=1&&nselmuons>=1)||(nvetoelectrons>=1&&nselelectrons>=1))&&jet_csv1>0.679&&(forward_tag_eta>2.8||forward_tag_eta<-2.8)";//&&jetmetnomu_mindphi>1.0";//top
    }
  }

  std::string qcdcat="nvetoelectrons==0&&nvetomuons==0&&"+jetmetdphicut+contonlycontplotjetmetdphi;
  std::string qcdzcat="&&"+jetmetdphicut+contonlycontplotjetmetdphi;//QCD

  if(channel=="nunu"){//nunu
    sigcat=nunucat;
    zextrasigcat=nunuzcat;
  }
  else if(channel=="mumu"){//zmumu
    sigcat=mumucat;
    zextrasigcat=mumuzcat;
  }
  else if(channel=="munu"){//wmu
    sigcat=munucat;
    zextrasigcat=munuzcat;
  }
  else if(channel=="enu"){//wel
    sigcat=enucat;
    zextrasigcat=enuzcat;
  }
  else if(channel=="taunu"){//wtau
    sigcat=taunucat;
    zextrasigcat=taunuzcat;
  }
  else if(channel=="qcd"){//QCD
    sigcat=qcdcat;
    zextrasigcat=qcdzcat;
  }
  else if(channel=="top"){//top
    sigcat=topcat;
    zextrasigcat=topzcat;
  }
  else{
    std::cout<<"Error: Channel "<<channel<<" not recognised, exiting"<<std::endl;
    return 1;
  }

  mumucat=mumucat+contonlycontplotjetmetdphi;
  mumuzcat=mumuzcat+contonlycontplotjetmetdphi;

  munucat=munucat+contonlycontplotjetmetdphi;
  munuzcat=munuzcat+contonlycontplotjetmetdphi;

  enucat=enucat+contonlycontplotjetmetdphi;
  enuzcat=enuzcat+contonlycontplotjetmetdphi;

  std::string sigmcweight;
  std::string mcweightpufactor="";
  if(syst=="PUUP") mcweightpufactor="*puweight_up_scale";
  if(syst=="PUDOWN") mcweightpufactor="*puweight_down_scale";
  
  if(channel=="nunu"||channel=="taunu"||channel=="qcd") sigmcweight="total_weight_lepveto"+mcweightpufactor;
  else sigmcweight="total_weight_leptight"+mcweightpufactor;
  if(do_nosigmcweight)sigmcweight="1";

  //DATA SHAPE GENERATION
  DataShape data("data");
  data.set_dataset(dataset)
    .set_dirname("data_obs")
    .set_shape(shape)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+dataextrasel);

  DataShape signal110("signal110");
  signal110.set_dataset("sig110")
    .set_dirname("qqH110")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);  

  DataShape qcdinc("qcdinc");
  qcdinc.set_dataset("qcd")
    .set_dirname("qcd")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);  

  DataShape run1qcdinc("run1qcdinc");
  run1qcdinc.set_dataset("run1QCD")
    .set_dirname("run1qcd")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);  

  DataShape signal125("signal125");
  signal125.set_dataset("sig125")
    .set_dirname("qqH125")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);  

  DataShape pu40signal125("pu40signal125");
  pu40signal125.set_dataset("pu40sig125")
    .set_dirname("pu40qqH125")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);  

  DataShape ns50signal125("ns50signal125");
  ns50signal125.set_dataset("ns50sig125")
    .set_dirname("ns50qqH125")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);  

  DataShape run1signal125("run1signal125");
  run1signal125.set_dataset("run1sig125")
    .set_dirname("run1qqH125")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);  

  DataShape signal150("signal150");
  signal150.set_dataset("sig150")
    .set_dirname("qqH150")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);  

  DataShape signal200("signal200");
  signal200.set_dataset("sig200")
    .set_dirname("qqH200")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);  

  DataShape signal300("signal300");
  signal300.set_dataset("sig300")
    .set_dirname("qqH300")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);  

  DataShape signal400("signal400");
  signal400.set_dataset("sig400")
    .set_dirname("qqH400")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);  

  DataShape ggHsignal110("ggHsignal110");
  ggHsignal110.set_dataset("ggH110")
    .set_dirname("ggH110")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  DataShape ggHsignal125("ggHsignal125");
  ggHsignal125.set_dataset("ggH125")
    .set_dirname("ggH125")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  DataShape ggHsignal150("ggHsignal150");
  ggHsignal150.set_dataset("ggH150")
    .set_dirname("ggH150")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  DataShape ggHsignal200("ggHsignal200");
  ggHsignal200.set_dataset("ggH200")
    .set_dirname("ggH200")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  DataShape ggHsignal300("ggHsignal300");
  ggHsignal300.set_dataset("ggH300")
    .set_dirname("ggH300")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  DataShape ggHsignal400("ggHsignal400");
  ggHsignal400.set_dataset("ggH400")
    .set_dirname("ggH400")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);


  DataShape vv("vv");
  vv.set_dataset("VV")
    .set_dirname("vv")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  DataShape topraw("topraw");
  topraw.set_dataset("Top")
    .set_dirname("top")
    .set_shape(shape)    
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  DataShape znunuraw("znunuraw");
  znunuraw.set_dataset("ZJets_nunu")
    .set_dirname("zvv")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  DataShape zmumuraw("zmumuraw");
  zmumuraw.set_dataset("ZJets_ll_all")
    .set_dirname("zmumu")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  DataShape run1zmumuraw("run1zmumuraw");
  run1zmumuraw.set_dataset("run1ZJets_ll")
    .set_dirname("run1zmumu")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  
  DataShape wmunuraw("wmunuraw");
  wmunuraw.set_dataset("WJets_munu")
    .set_dirname("wmu")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  DataShape wenuraw("wenuraw");
  wenuraw.set_dataset("WJets_enu")
    .set_dirname("wel")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  DataShape wtaunuraw("wtaunuraw");
  wtaunuraw.set_dataset("WJets_taunu")
    .set_dirname("wtau")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  DataShape run1wmunuraw("run1wmunuraw");
  run1wmunuraw.set_dataset("run1WJets_munu")
    .set_dirname("run1wmu")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  DataShape run1wenuraw("run1wenuraw");
  run1wenuraw.set_dataset("run1WJets_enu")
    .set_dirname("run1wel")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  DataShape run1wtaunuraw("run1wtaunuraw");
  run1wtaunuraw.set_dataset("run1WJets_taunu")
    .set_dirname("run1wtau")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  DataShape QCDraw("QCDraw");
  QCDraw.set_dataset("VBF-QCD")
    .set_dirname("qcd")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  //ZBKG SHAPE GENERATION
  std::vector<std::string> Zcontbkgsets;
  Zcontbkgsets.push_back("VV");
  Zcontbkgsets.push_back("Top");
  //Zcontbkgsets.push_back("WGamma");
  Zcontbkgsets.push_back("WJets_enu");
  Zcontbkgsets.push_back("WJets_munu");
  Zcontbkgsets.push_back("WJets_taunu");
  
  DataZNormShape zmumu("zmumu");
  zmumu.set_sigmcewkset("ZJets_ll_vbf")
    .set_do_latex(do_latex)
    .set_shape(shape)
    .set_dirname("zvv")
    .set_sigmcqcdset("ZJets_ll")
    .set_contmcewkset("ZJets_ll_vbf")
    .set_contmcqcdset("ZJets_ll")
    .set_contbkgset(Zcontbkgsets)
    .set_contdataset(dataset)
    .set_basesel(analysis->baseselection())
    .set_contdataextrasel(dataextrasel)
    .set_sigcat("m_mumu_gen>80&&m_mumu_gen<100"+zextrasigcat)
    .set_contcat(mumucat)//"nvetoelectrons==0 && nvetomuons==2 && nselmuons==2&&m_mumu>60&&m_mumu<120")
    .set_sigmcweight("weight_nolep"+mcweightpufactor)
    .set_contmcweight("total_weight_leptight"+mcweightpufactor)
    .set_contdataweight("weight_nolep")
    .set_sigmainccontewk(303)
    .set_sigmainccontqcd(3503700./3)
    .set_sigmaincsigewk(460*3)
    .set_sigmaincsigqcd(3503700./3*5.651)
    .set_ngenincewk(5781.91)
    .set_ngenincqcd(22789300)
    .set_ngenmassfilteredewk(4226.53)
    .set_ngenmassfilteredqcd(20334900);

  std::vector<std::string> qcdZcontbkgsets;
  qcdZcontbkgsets.push_back("VV");
  qcdZcontbkgsets.push_back("Top");
  qcdZcontbkgsets.push_back("WJets_enu");
  qcdZcontbkgsets.push_back("WJets_munu");
  qcdZcontbkgsets.push_back("WJets_taunu");

  DataZNormShape zmumuqcd("zmumuqcd");
  zmumuqcd.set_sigmcewkset("ZJets_ll_vbf")
    .set_do_latex(do_latex)
    .set_shape(shape)
    .set_dirname("zvvqcd")
    .set_ewkdirname("zvv_ewkqcd")
    .set_qcddirname("zvv_qcdqcd")
    .set_sigmcqcdset("ZJets_ll")
    .set_contmcewkset("ZJets_ll_vbf")
    .set_contmcqcdset("ZJets_ll")
    .set_contbkgset(qcdZcontbkgsets)
    .set_contdataset(dataset)
    .set_basesel(analysis->baseselection())
    .set_contdataextrasel(dataextrasel)
    .set_sigcat("m_mumu_gen>80&&m_mumu_gen<100"+zextraqcdshapecat)
    .set_contcat(mumuqcdcat)//"nvetoelectrons==0 && nvetomuons==2 && nselmuons==2&&m_mumu>60&&m_mumu<120")
    .set_sigmcweight("weight_nolep"+mcweightpufactor)
    .set_contmcweight("total_weight_leptight"+mcweightpufactor)
    .set_contdataweight("weight_nolep")
    .set_sigmainccontewk(303)
    .set_sigmainccontqcd(3503700./3)
    .set_sigmaincsigewk(460*3)
    .set_sigmaincsigqcd(3503700./3*5.651)
    .set_ngenincewk(5781.91)
    .set_ngenincqcd(22789300)
    .set_ngenmassfilteredewk(4226.53)
    .set_ngenmassfilteredqcd(20334900);

  std::vector<std::string> zcontsubsets;
  zcontsubsets.push_back("ZJets_ll_vbf");
  zcontsubsets.push_back("ZJets_ll");
  std::vector<std::string> zcontsubsetdirs;
  zcontsubsetdirs.push_back("zvv_ewk");
  zcontsubsetdirs.push_back("zvv_qcd");

  DataNormShape zmumuinzcont("zmumuinzcont");
  zmumuinzcont.set_sigmcset("ZJets_ll_all")
    .set_shape(shape)
    .set_dirname("zvv")
    .set_contmcset("ZJets_ll_all")
    .set_contbkgset(Zcontbkgsets)
    .set_contdataset(dataset)
    .set_do_subsets(true)
    .set_subsets(zcontsubsets)
    .set_subsetdirs(zcontsubsetdirs)
    .set_basesel(analysis->baseselection())
    .set_contdataextrasel(dataextrasel)
    .set_sigmcweight(sigmcweight)
    .set_contmcweight("total_weight_leptight"+mcweightpufactor)
    .set_sigcat("m_mumu_gen>80&&m_mumu_gen<100"+zextrasigcat)
    .set_contcat(mumucat);//"nvetoelectrons==0 && nvetomuons==2 && nselmuons==2&&m_mumu>60&&m_mumu<120");

  //WBKG SHAPE GENERATION
  std::vector<std::string> Wcontbkgsets; //List of sets for ncbkg
  Wcontbkgsets.push_back("VV");
  Wcontbkgsets.push_back("Top");

  std::vector<std::string> Wcontbkgextrafactordir;//list of dirs with data driven weights for above backgrounds
  Wcontbkgextrafactordir.push_back("");
  if(do_datatop&&!do_singletop) Wcontbkgextrafactordir.push_back("top");
  else Wcontbkgextrafactordir.push_back("");

  std::vector<int> Wcontbkgisz;
  Wcontbkgisz.push_back(0);
  Wcontbkgisz.push_back(0);

  DataNormShape wmunu("wmunu");
  wmunu.set_sigmcset("WJets_munu")
    .set_do_latex(do_latex)
    .set_shape(shape)
    .set_dirname("wmu")
    .set_contdataset(dataset)
    .set_contbkgset(Wcontbkgsets)
    .set_contbkgextrafactordir(Wcontbkgextrafactordir)
    .set_contbkgisz(Wcontbkgisz)
    .set_sigmcweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_contdataextrasel(dataextrasel)
    .set_sigcat(sigcat)
    .set_contmcset("WJets_munu")
    .set_contcat(munucat)//"nvetoelectrons==0 && nvetomuons==1 && nselmuons==1");
    .set_contmcweight("total_weight_leptight"+mcweightpufactor);

  

  DataNormShape wenu("wenu");
  wenu.set_sigmcset("WJets_enu")
    .set_do_latex(do_latex)
    .set_shape(shape)
    .set_dirname("wel")
    .set_contdataset(dataset)
    .set_contbkgset(Wcontbkgsets)
    .set_contbkgextrafactordir(Wcontbkgextrafactordir)
    .set_contbkgisz(Wcontbkgisz)
    .set_basesel(analysis->baseselection())
    .set_contdataextrasel(dataextrasel)
    .set_sigcat(sigcat)
    .set_sigmcweight(sigmcweight)
    .set_contmcset("WJets_enu")
    .set_contcat(enucat)//"nselelectrons==1 && nvetoelectrons ==1 && nvetomuons==0");
    .set_contmcweight("total_weight_leptight"+mcweightpufactor);
  

  DataNormShape wtaunu("wtaunu");
  wtaunu.set_sigmcset("WJets_taunu")
    .set_do_latex(do_latex)
    .set_shape(shape)
    .set_dirname("wtau")
    .set_contdataset(dataset)
    .set_contbkgset(Wcontbkgsets)
    .set_contbkgextrafactordir(Wcontbkgextrafactordir)
    .set_contbkgisz(Wcontbkgisz)
    .set_basesel(analysis->baseselection())
    .set_contdataextrasel(dataextrasel)
    .set_sigcat(sigcat)
    .set_sigmcweight(sigmcweight)
    .set_contmcset("WJets_taunu")
    .set_contcat(taunucat)//"ntaus>=1&&nvetoelectrons ==0 && nvetomuons==0&&lep_mt>20")
    .set_contmcweight("total_weight_lepveto"+mcweightpufactor);

  std::vector<std::string> qcdWcontbkgsets; //List of sets for ncbkg
  qcdWcontbkgsets.push_back("VV");
  qcdWcontbkgsets.push_back("Top");

  DataNormShape wmunuqcd("wmunuqcd");
  wmunuqcd.set_sigmcset("WJets_munu")
    .set_do_latex(do_latex)
    .set_shape(shape)
    .set_dirname("wmuqcd")
    .set_contdataset(dataset)
    .set_contbkgset(qcdWcontbkgsets)
    .set_sigmcweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_contdataextrasel(dataextrasel)
    .set_sigcat(qcdshapecat)
    .set_contmcset("WJets_munu")
    .set_contcat(munuqcdcat)//"nvetoelectrons==0 && nvetomuons==1 && nselmuons==1");
    .set_contmcweight("total_weight_leptight"+mcweightpufactor);
  
  DataNormShape wenuqcd("wenuqcd");
  wenuqcd.set_sigmcset("WJets_enu")
    .set_do_latex(do_latex)
    .set_shape(shape)
    .set_dirname("welqcd")
    .set_contdataset(dataset)
    .set_contbkgset(qcdWcontbkgsets)
    .set_basesel(analysis->baseselection())
    .set_contdataextrasel(dataextrasel)
    .set_sigcat(qcdshapecat)
    .set_sigmcweight(sigmcweight)
    .set_contmcset("WJets_enu")
    .set_contcat(enuqcdcat)//"nselelectrons==1 && nvetoelectrons ==1 && nvetomuons==0");
    .set_contmcweight("total_weight_leptight"+mcweightpufactor);

  
  DataNormShape wtaunuqcd("wtaunuqcd");
  wtaunuqcd.set_sigmcset("WJets_taunu")
    .set_do_latex(do_latex)
    .set_shape(shape)
    .set_dirname("wtauqcd")
    .set_contdataset(dataset)
    .set_contbkgset(qcdWcontbkgsets)
    .set_basesel(analysis->baseselection())
    .set_contdataextrasel(dataextrasel)
    .set_sigcat(qcdshapecat)
    .set_sigmcweight(sigmcweight)
    .set_contmcset("WJets_taunu")
    .set_contcat(taunuqcdcat)//"ntaus>=1&&nvetoelectrons ==0 && nvetomuons==0&&lep_mt>20")
    .set_contmcweight("total_weight_lepveto"+mcweightpufactor);
  
  //TOP
  std::vector<std::string> Topcontbkgsets;
  Topcontbkgsets.push_back("VV");
  Topcontbkgsets.push_back("WJets_enu");
  Topcontbkgsets.push_back("WJets_munu");
  if(do_singletop){
    Topcontbkgsets.push_back("ZJets_ll");
    Topcontbkgsets.push_back("ZJets_ll_vbf");
   }
  else Topcontbkgsets.push_back("WJets_taunu");

  std::vector<std::string> Topcontbkgextrafactordir;//list of dirs with data driven weights for above backgrounds
  Topcontbkgextrafactordir.push_back("");
  Topcontbkgextrafactordir.push_back("wel");
  Topcontbkgextrafactordir.push_back("wmu");
  Topcontbkgextrafactordir.push_back("zvv");
  Topcontbkgextrafactordir.push_back("zvv");
  

  std::vector<int> Topcontbkgisz;
  Topcontbkgisz.push_back(0);
  Topcontbkgisz.push_back(0);
  Topcontbkgisz.push_back(0);
  Topcontbkgisz.push_back(2);
  Topcontbkgisz.push_back(1);

  DataNormShape top("top");
  top.set_sigmcset("Top")
    .set_do_latex(do_latex)
    .set_shape(shape)
    .set_dirname("top")
    .set_contmcset("Top")
    .set_contdataset(dataset)
    .set_contbkgset(Topcontbkgsets)
    .set_basesel(analysis->baseselection())
    .set_contdataextrasel(dataextrasel)
    .set_sigcat(sigcat)
    .set_contcat(topcat)
    .set_sigmcweight(sigmcweight)
    .set_contmcweight("total_weight_leptight"+mcweightpufactor);
  if(do_singletop){
    top.set_contbkgextrafactordir(Topcontbkgextrafactordir)
      .set_zextracontcat(topzcat)
      .set_contbkgisz(Topcontbkgisz);
				     
  }

//   //QCDBKG
  std::vector<std::string> QCDcontbkgsets; //list of sets for ncbkg
  QCDcontbkgsets.push_back("VV");
  QCDcontbkgsets.push_back("Top");
  QCDcontbkgsets.push_back("ZJets_ll");
  QCDcontbkgsets.push_back("ZJets_ll_vbf");
  QCDcontbkgsets.push_back("WJets_enu");
  QCDcontbkgsets.push_back("WJets_munu");
  QCDcontbkgsets.push_back("WJets_taunu");

  std::vector<std::string> QCDshapebkgextrafactordir;//list of dirs with data driven weights for above backgrounds
  QCDshapebkgextrafactordir.push_back("");
  QCDshapebkgextrafactordir.push_back("");
  QCDshapebkgextrafactordir.push_back("zvvqcd");
  QCDshapebkgextrafactordir.push_back("zvvqcd");
  QCDshapebkgextrafactordir.push_back("welqcd");
  QCDshapebkgextrafactordir.push_back("wmuqcd");
  QCDshapebkgextrafactordir.push_back("wtauqcd");

  std::vector<std::string> QCDcontbkgextrafactordir;//list of dirs with data driven weights for above backgrounds
  QCDcontbkgextrafactordir.push_back("");
  QCDcontbkgextrafactordir.push_back("");
  QCDcontbkgextrafactordir.push_back("zvv");
  QCDcontbkgextrafactordir.push_back("zvv");
  QCDcontbkgextrafactordir.push_back("wel");
  QCDcontbkgextrafactordir.push_back("wmu");
  QCDcontbkgextrafactordir.push_back("wtau");
  
  std::vector<int> QCDcontbkgisz;
  QCDcontbkgisz.push_back(0);
  QCDcontbkgisz.push_back(0);
  if(channel!="mumu"){
    QCDcontbkgisz.push_back(2);
    QCDcontbkgisz.push_back(1);
  }
  else{
    QCDcontbkgisz.push_back(0);
    QCDcontbkgisz.push_back(0);
  }
  QCDcontbkgisz.push_back(0);
  QCDcontbkgisz.push_back(0);
  QCDcontbkgisz.push_back(0);

  BkgSubDataNormShape QCDshape("QCDshape");
  QCDshape.set_shape(shape)
    .set_contbkgset(QCDcontbkgsets)
    .set_shapebkgset(QCDcontbkgsets)
    .set_contbkgextrafactordir(QCDcontbkgextrafactordir)
    .set_shapebkgextrafactordir(QCDshapebkgextrafactordir)
    .set_contbkgisz(QCDcontbkgisz)
    .set_shapebkgisz(QCDcontbkgisz)
    .set_contdataset(dataset)
    .set_shapecat(qcdshapecat)
    .set_contcat(sigcat)
    .set_zextrashapecat("m_mumu_gen>80&&m_mumu_gen<100"+zextraqcdshapecat)
    .set_zextracontcat("m_mumu_gen>80&&m_mumu_gen<100"+zextrasigcat)
    .set_contdataextrasel(dataextrasel)
    .set_shapebaseextrasel(dataextrasel)
    .set_basesel(analysis->baseselection())
    .set_shapebaseweight("weight_nolep")
    .set_contmcweight(sigmcweight)
    .set_shapemcweight(sigmcweight)
    .set_contdataweight("weight_nolep")
    .set_do_ddnorm(false)
    .set_shapecontextrafactor(0.05)
    .set_dirname("qcd")
    .set_shapebaseset(dataset);

  //MVA TRAIN
  std::vector<std::string> sigsets;
  sigsets.push_back("sig125");
  std::vector<std::string> bkgsets;
  bkgsets.push_back("QCD");
  std::vector<std::string> variables;
  variables.push_back("jet2_eta");
  variables.push_back("met");
  variables.push_back("met_significance");
  variables.push_back("mht");
  variables.push_back("jet2met_dphi");
  variables.push_back("jetmet_mindphi");
  variables.push_back("jetunclet_mindphi");
  variables.push_back("metunclet_dphi");
  variables.push_back("dijetmet_scalarSum_pt");
  variables.push_back("dijetmet_vectorialSum_pt");
  variables.push_back("dijetmet_ptfraction");
  variables.push_back("jet1met_scalarprod");
  variables.push_back("jet2met_scalarprod");
  variables.push_back("jet1met_scalarprod_frac := jet1met_scalarprod/met");
  variables.push_back("jet2met_scalarprod_frac := jet2met_scalarprod/met");
  std::vector<std::string> specvariables;

  MVATrain mvatrainer("mvatrainer");
  mvatrainer.set_sigsets(sigsets)
    .set_bkgsets(bkgsets)
    .set_variables(variables)
    .set_specvariables(specvariables)
    .set_basesel("passtrigger==1&&nvetomuons==0&&nvetoelectrons==0&&jet1_pt>50&&jet2_pt>50&&jet1_eta<4.7&&jet2_eta<4.7")
    .set_sigcat("")
    .set_bkgcat("");

  //HISTPLOTTER
  //std::vector<std::string> shapevec;
  std::vector<LTShapeElement> shapevec;
  for(unsigned ishape=0;ishape<shape.size();ishape++){
    std::vector<std::string> strs;
    boost::split(strs, shape[ishape], boost::is_any_of("("));
    LTShapeElement thisshape;
    thisshape.set_name(strs[0]);
    thisshape.set_histtitle(histTitle[ishape]);
    if(do_logy) thisshape.set_dology(true);
    //    if(channel=="mumu"&&(strs[0]=="dijet_M"||strs[0]=="metnomu_significance")) thisshape.set_axisrangemultiplier(1.3);

    thisshape.set_legleft(0.5);
    thisshape.set_legright(0.89);
     if(channel=="nunu"&&(strs[0]=="central_tag_eta"||strs[0]=="forward_tag_eta")){
       thisshape.set_legleft(0.39);
       thisshape.set_legright(0.61);
    }
    shapevec.push_back(thisshape);
  }

  std::vector<LTPlotElement> elementvec;

  LTPlotElement dataele;
  dataele.set_is_data(true)
    .set_scale(1)
    .set_legname("Data")
    .set_is_inrationum(true)
    .set_sample("data_obs");
  if(runblindreg&&channel=="nunu"){
    std::vector<std::string> blindvars;
    std::vector<std::pair<double,double> > blindrange;
    blindvars.push_back("alljetsmetnomu_mindphi");
    blindvars.push_back("metnomu_significance");
    std::pair<double,double>mindphirange(2.,6.);
    std::pair<double,double>metsigrange(4.,20.);
    blindrange.push_back(mindphirange);
    blindrange.push_back(metsigrange);
    dataele.set_blindvar(blindvars)
      .set_blindrange(blindrange);
  }

  if(blindcutreg&&!runblindreg){
    std::vector<std::string> blindvars;
    std::vector<std::pair<double,double> > blindrange;
    blindvars.push_back("forward_tag_eta");
    std::pair<double,double>forwardetarange(-1.8,1.8);
    blindrange.push_back(forwardetarange);
    dataele.set_blindvar(blindvars)
      .set_blindrange(blindrange);
  }

  LTPlotElement wmunuele;
  wmunuele.set_is_data(false)
    .set_scale(1)
    .set_color(kOrange-4)
    .set_in_stack(false)
    .set_is_inrationum(true)
    .set_has_dderrors(0)
    .set_legname("W#rightarrow#mu#nu")
    .set_sample("wmu");

  LTPlotElement wenuele;
  wenuele.set_is_data(false)
    .set_scale(1)
    .set_color(kOrange  + 2)
    .set_in_stack(false)
    .set_is_inrationum(true)
    .set_has_dderrors(0)
    .set_legname("W#rightarrow e#nu")
    .set_sample("wel");

  LTPlotElement wtaunuele;
  wtaunuele.set_is_data(false)
    .set_scale(1)
    .set_color(kOrange + 4)
    .set_in_stack(false)
    .set_is_inrationum(true)
    .set_has_dderrors(0)
    .set_legname("W#rightarrow#tau#nu")
    .set_sample("wtau");

  LTPlotElement run1wmunuele;
  run1wmunuele.set_is_data(false)
    .set_scale(1)
    .set_color(kBlue)
    .set_in_stack(false)
    .set_is_inratioden(true)
    .set_has_dderrors(0)
    .set_legname("Run 1 W#rightarrow#mu#nu")
    .set_sample("run1wmu");

  LTPlotElement run1wenuele;
  run1wenuele.set_is_data(false)
    .set_scale(1)
    .set_color(kRed)
    .set_in_stack(false)
    .set_is_inratioden(true)
    .set_has_dderrors(0)
    .set_legname("Run 1 W#rightarrow e#nu")
    .set_sample("run1wel");

  LTPlotElement run1wtaunuele;
  run1wtaunuele.set_is_data(false)
    .set_scale(1)
    .set_color(kMagenta)
    .set_in_stack(false)
    .set_is_inratioden(true)
    .set_has_dderrors(0)
    .set_legname("Run 1 W#rightarrow#tau#nu")
    .set_sample("run1wtau");

  LTPlotElement znunuele;
  znunuele.set_is_data(false)
    .set_scale(1)
    .set_color(kAzure  + 2)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_has_dderrors(1)
    .set_legname("Z#rightarrow#nu#nu")
    .set_sample("zvv");

  LTPlotElement znunuewkele;
  znunuewkele.set_is_data(false)
    .set_scale(1)
    .set_color(kAzure  + 2)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_has_dderrors(1)
    .set_legname("Z#rightarrow#nu#nu EWK")
    .set_sample("zvv_ewk");

  LTPlotElement znunuqcdele;
  znunuqcdele.set_is_data(false)
    .set_scale(1)
    .set_color(kAzure  + 4)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_has_dderrors(1)
    .set_legname("Z#rightarrow#nu#nu QCD")
    .set_sample("zvv_qcd");



  LTPlotElement zmumuele;
  zmumuele.set_is_data(false)
    .set_scale(1)
    .set_color(kAzure  + 2)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_has_dderrors(1)
    .set_legname("Z#rightarrow#mu#mu")
    .set_sample("zmumu");

  LTPlotElement qcdele;
  qcdele.set_is_data(false)
    .set_scale(1)
    .set_color(kMagenta-10)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_sample("qcd");
  if(do_plotmcqcd) qcdele.set_legname("MC QCD");
  else qcdele.set_legname("QCD");
    


  LTPlotElement vvele;
  vvele.set_is_data(false)
    .set_scale(1)
    .set_color(kGreen-5)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("VV")
    .set_sample("vv");

  LTPlotElement wgele;
  wgele.set_is_data(false)
    .set_scale(1)
    .set_color(kGreen-3)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("WGamma")
    .set_sample("wg");

  LTPlotElement topele;
  topele.set_is_data(false)
    .set_scale(1)
    .set_color(kBlue-8)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("Top")
    .set_sample("top");
  if(do_datatop)topele.set_has_dderrors(1);

  LTPlotElement qcdincele;
  qcdincele.set_is_data(false)
    .set_scale(1)
    .set_color(kMagenta)
    .set_is_inrationum(false)
    .set_is_inratioden(true)
    .set_in_stack(false)
    .set_legname("Run 2 QCD inclusive")
    .set_sample("qcd");

  LTPlotElement run1qcdincele;
  run1qcdincele.set_is_data(false)
    .set_scale(1)
    .set_color(kCyan)
    .set_is_inratioden(false)
    .set_in_stack(false)
    .set_legname("Run 1 QCD inclusive")
    .set_sample("run1qcd");

  LTPlotElement dyele;
  dyele.set_is_data(false)
    .set_scale(1)
    .set_color(kAzure  + 2)
    .set_is_inrationum(true)
    .set_in_stack(false)
    .set_legname("Run 2 DY")
    .set_sample("zmumu");

  LTPlotElement run1dyele;
  run1dyele.set_is_data(false)
    .set_scale(1)
    .set_color(kRed)
    .set_is_inratioden(true)
    .set_in_stack(false)
    .set_legname("Run 1 DY")
    .set_sample("run1zmumu");

  LTPlotElement sigele;
  sigele.set_is_data(false)
    .set_scale(1)
    .set_color(kRed)
    .set_is_inrationum(true)
    .set_in_stack(false)
    .set_legname("Run 2 Signal PU20BX25")
    .set_sample("qqH125");

  LTPlotElement pu40sigele;
  pu40sigele.set_is_data(false)
    .set_scale(1)
    .set_color(kGreen)
    .set_is_inrationum(false)
    .set_in_stack(false)
    .set_legname("Run 2 Signal PU40BX25")
    .set_sample("pu40qqH125");

  LTPlotElement ns50sigele;
  ns50sigele.set_is_data(false)
    .set_scale(1)
    .set_color(kBlack)
    .set_is_inrationum(false)
    .set_in_stack(false)
    .set_legname("Run 2 Signal PU30BX50")
    .set_sample("ns50qqH125");

  LTPlotElement run1sigele;
  run1sigele.set_is_data(false)
    .set_scale(1)
    .set_color(kBlue)
    .set_is_inratioden(true)
    .set_in_stack(false)
    .set_legname("Run 1 Signal")
    .set_sample("run1qqH125");

  LTPlotElement ggHele;
  ggHele.set_is_data(false)
    .set_scale(1)
    .set_color(kBlue)
    .set_in_stack(false)
    .set_legname("gg#rightarrow H (x1)")
    .set_sample("ggH125");

  // if(!(channel=="nunu"&&runblind))elementvec.push_back(dataele);
  // if(channel!="mumu"){
  //   if(channel!="enu")elementvec.push_back(wmunuele);
  //   if(channel!="munu")elementvec.push_back(wenuele);
  //   if(channel!="munu")elementvec.push_back(wtaunuele);
  // }
  // //  elementvec.push_back(zmumuele);
  // if(do_separatez&&channel=="mumu"){
  //   elementvec.push_back(znunuewkele);
  //   elementvec.push_back(znunuqcdele);
  // }
  // else{
  //   if(channel!="enu"&&channel!="munu"&&channel!="taunu")elementvec.push_back(znunuele);
  // }
  // if(do_plotmcqcd||do_plotqcd)elementvec.push_back(qcdele);
  // if(channel!="munu")elementvec.push_back(vvele);
  // //   elementvec.push_back(wgele);
  // if(channel!="mumu")elementvec.push_back(topele);
  // if(channel!="mumu"&&channel!="enu"&&channel!="munu"){
  //   elementvec.push_back(sigele);
  //   elementvec.push_back(ggHele);
  // }

  if(channel=="nunu"){
    elementvec.push_back(sigele);
    elementvec.push_back(pu40sigele);
    elementvec.push_back(ns50sigele);
    elementvec.push_back(run1sigele);
  }
  //   elementvec.push_back(qcdincele);
  //   elementvec.push_back(run1qcdincele);
  // }

  if(channel=="mumu"){
    elementvec.push_back(dyele);
    elementvec.push_back(run1dyele);
  }

  if(channel=="munu"){
    elementvec.push_back(wmunuele);
    elementvec.push_back(run1wmunuele);
  }
  else if(channel=="enu"){
    elementvec.push_back(wenuele);
    elementvec.push_back(run1wenuele);
  }
  else if(channel=="taunu"){
    elementvec.push_back(wtaunuele);
    elementvec.push_back(run1wtaunuele);
  }

  HistPlotter plotter("plotter");//!!
  plotter.set_dirname("ControlPlots")
    .set_do_debug(do_debug)
    .set_add_underflows(false)
    .set_add_overflows(false)
    .set_elements(elementvec)
    .set_do_ratio(true)
    //.set_histTitles(histTitle)
    .set_shapes(shapevec);
  //  if(channel=="nunu"&&runblind)plotter.set_do_ratio(false);

  HistPlotter normplotter("normplotter");//!!
  normplotter.set_dirname("NormControlPlots")
    .set_do_debug(do_debug)
    .set_outsuffix("_norm")
    .set_add_underflows(false)
    .set_add_overflows(false)
    .set_elements(elementvec)
    .set_do_norm(true)
    .set_do_ratio(true)
    //.set_histTitles(histTitle)
    .set_shapes(shapevec);
  //  if(channel=="nunu"&&runblind)normplotter.set_do_ratio(false);

  // std::vector<std::string> dirvec;
  // dirvec.push_back("wel");
  // dirvec.push_back("wmu");
  // dirvec.push_back("wtau");
  // dirvec.push_back("zvv");
  // //dirvec.push_back("qcd");
  // dirvec.push_back("vv");
  // //dirvec.push_back("wg");  
  // dirvec.push_back("top");
  // dirvec.push_back("qqH125");
  // if(!(channel=="nunu"&&runblind))dirvec.push_back("data_obs");

  // SummaryTable summary("summary");
  // summary.set_shape(shapevec)
  //   .set_dirs(dirvec);
  
  EventList eventlist("eventlist");
  eventlist.set_set(dataset)
    .set_basesel(analysis->baseselection()+dataextrasel)
    .set_cat(sigcat);

  EventList mceventlist("mceventlist");
  mceventlist.set_set(listset)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  /*##########################################
  #                                          #
  #   SET UP ANALYSIS SEQUENCE AND RUN       #
  #                                          #
  ##########################################*/

  if(!dataonly){
    // if(do_datatop&&!do_singletop)analysis->AddModule(&top);
    // else if(!do_singletop) analysis->AddModule(&topraw);
    // if(do_list) analysis->AddModule(&mceventlist);
    
    // analysis->AddModule(&wmunu);
    // analysis->AddModule(&wenu);
    // analysis->AddModule(&wtaunu);
    // if(channel!="mumu"){
    //   analysis->AddModule(&zmumu);
    // }
    // else analysis->AddModule(&zmumuinzcont);
    // if(do_singletop)analysis->AddModule(&top);
    //analysis->AddModule(&QCD);
    //analysis->AddModule(&wmunuraw);
    //analysis->AddModule(&wenuraw);
    //analysis->AddModule(&wtaunuraw);  
    // if(do_plotmcqcd)analysis->AddModule(&QCDraw);
    // if(do_plotqcd){
    //   analysis->AddModule(&wmunuqcd);
    //   analysis->AddModule(&wenuqcd);
    //   analysis->AddModule(&wtaunuqcd);
    //   if(channel!="mumu"){
    // 	analysis->AddModule(&zmumuqcd);
    //   }
    //   analysis->AddModule(&QCDshape);
    // }
    //analysis->AddModule(&zmumuraw);
    //analysis->AddModule(&znunuraw);
    //analysis->AddModule(&vv);
    //analysis->AddModule(&wgamma);
  }
  //if(!(channel=="nunu"&&runblind))analysis->AddModule(&data);
  //if(datalist) analysis->AddModule(&eventlist);
  //if(!dataonly){
    // analysis->AddModule(&signal110);
  if(channel=="nunu"){
    //analysis->AddModule(&qcdinc);
    //analysis->AddModule(&run1qcdinc);
  
    analysis->AddModule(&signal125);
    analysis->AddModule(&pu40signal125);
    analysis->AddModule(&ns50signal125);
    analysis->AddModule(&run1signal125);
  }

  if(channel=="mumu"){
    analysis->AddModule(&zmumuraw);
    analysis->AddModule(&run1zmumuraw);
  }

  if(channel=="enu"){
    analysis->AddModule(&wenuraw);
    analysis->AddModule(&run1wenuraw);
  }
  if(channel=="munu"){
    analysis->AddModule(&wmunuraw);
    analysis->AddModule(&run1wmunuraw);
  }
  if(channel=="taunu"){
    analysis->AddModule(&wtaunuraw);
    analysis->AddModule(&run1wtaunuraw);
  }
  // analysis->AddModule(&signal150);
  // analysis->AddModule(&signal200);
  // analysis->AddModule(&signal300);
  // analysis->AddModule(&signal400);
  // analysis->AddModule(&ggHsignal110);
  // analysis->AddModule(&ggHsignal125);
  // analysis->AddModule(&ggHsignal150);
  // analysis->AddModule(&ggHsignal200);
  // analysis->AddModule(&ggHsignal300);
  // analysis->AddModule(&ggHsignal400);
  analysis->AddModule(&plotter);
  analysis->AddModule(&normplotter);
  //analysis->AddModule(&summary);
  //}
  analysis->RunAnalysis();

  return 0;

}
