#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeAnalyser.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataNormShape.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/ClosureTest.h"
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
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "TColor.h"

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
  std::string contonlycontplotjetmetdphi;
  std::string cjvcut;
  std::string channel;
  std::string syst;
  std::string dataset;

  bool do_promptsel;
  bool use_promptdata;
  bool do_datatop;
  bool runblind;
  bool runblindreg;
  bool do_bdt;
  std::string bdtcut;
  bool do_preselranges;
  bool do_prepreselranges;
  bool do_plotmcqcd;
  bool dataonly;
  bool do_latex;
  bool do_closure;
  std::string closurebase;

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
    ("syst,s",                   po::value<std::string>(&syst)->default_value(""))
    ("input_params,p",           po::value<std::string>(&inputparams)->default_value("../filelists/Dec18/ParamsDec18test.dat"))
    ("filelist,f",               po::value<std::string>(&filelist)->default_value("filelists/filelist.dat"))
    ("dataset,d",                po::value<std::string>(&dataset)->default_value("SPLITPARKEDPLUSA"))
    ("dataonly",                 po::value<bool>(&dataonly)->default_value(false))
    ("basesel",                  po::value<std::string>(&basesel)->default_value("jet1_eta*jet2_eta<0 && jet1_eta<4.7 && jet2_eta<4.7 && dijet_M>=600&&jet1_pt>50&&dijet_deta>3.6&& jet2_pt>60&&metnomuons>60&&metnomu_significance>3&&jetmetnomu_mindphi>1.5"))
    ("jetmetdphicut",            po::value<std::string>(&jetmetdphicut)->default_value("alljetsmetnomu_mindphi>1.0"))
    ("contonlycontplotjetmetdphi",po::value<std::string>(&contonlycontplotjetmetdphi)->default_value(""))
    ("cjvcut",                   po::value<std::string>(&cjvcut)->default_value("n_jets_cjv_30<1"))
    ("channel",                  po::value<std::string>(&channel)->default_value("nunu"))
    ("do_datatop",               po::value<bool>(&do_datatop)->default_value(true))
    ("runblind",                 po::value<bool>(&runblind)->default_value(true))
    ("do_bdt",                   po::value<bool>(&do_bdt)->default_value(false))
    ("bdtcut",                   po::value<std::string>(&bdtcut)->default_value("BDT>-0.26"))
    ("do_latex",                 po::value<bool>(&do_latex)->default_value(false))
    ("do_closure",               po::value<bool>(&do_closure)->default_value(false))
    ("closurebase",               po::value<std::string>(&closurebase)->default_value("munu"))
    ("do_preselranges",          po::value<bool>(&do_preselranges)->default_value(false))
    ("do_prepreselranges",       po::value<bool>(&do_prepreselranges)->default_value(false))
    ("do_promptsel",             po::value<bool>(&do_promptsel)->default_value(false))
    ("use_promptdata",           po::value<bool>(&use_promptdata)->default_value(false))
    ("do_plotmcqcd",             po::value<bool>(&do_plotmcqcd)->default_value(false))
    ("runblindreg",              po::value<bool>(&runblindreg)->default_value(true));

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
  analysis->set_baseselection(basesel);
  
  /*##########################################
  #                                          #
  #            DEFINE MODULES                #
  #                                          #
  ##########################################*/

  std::vector<std::string> histTitle;
  std::vector<std::string> shape;
  //shape.push_back("BDT(12,-1.,0.2)");

  if(!(channel=="taunu"||channel=="top")){
    shape.push_back("jet2_pt(26,40.,300.)");histTitle.push_back(";p_{T}^{j2} (GeV);entries");
    shape.push_back("jet1_pt(25,50.,300.)");histTitle.push_back(";p_{T}^{j1} (GeV);entries");
    shape.push_back("metnomuons(21,90.,300.)");histTitle.push_back(";METnoMu (GeV);entries");
    shape.push_back("met(30,0.,300.)");histTitle.push_back(";MET (GeV);entries");
    shape.push_back("l1met(20,00.,200.)");histTitle.push_back(";L1MET (GeV);entries");
    shape.push_back("dijet_M(10,1000.,2000.)");histTitle.push_back(";M_{jj} (GeV);entries");
    shape.push_back("jetmetnomu_mindphi(12,2.0,3.142)");histTitle.push_back(";min #Delta#phi(j1/j2,METnoMu);entries");
    if(!do_preselranges&&!do_prepreselranges){
      shape.push_back("alljetsmetnomu_mindphi(12,2.0,3.142)");histTitle.push_back(";min #Delta#phi(all jets,METnoMu);entries");
      shape.push_back("metnomu_significance(35,3.,10.)");histTitle.push_back(";METnoMu/#sigma(METnoMu);entries");
    }
    else if(do_prepreselranges){
      shape.push_back("alljetsmetnomu_mindphi(32,0.,3.142)");histTitle.push_back(";min #Delta#phi(all jets,METnoMu);entries");
      shape.push_back("metnomu_significance(50,0.,10.)");histTitle.push_back(";METnoMu/#sigma(METnoMu);entries");
    }
    else{
      shape.push_back("alljetsmetnomu_mindphi(22,1.0,3.142)");histTitle.push_back(";min #Delta#phi(all jets,METnoMu);entries");
      //shape.push_back("alljetsmetnomu_mindphi(32,0.,3.142)");histTitle.push_back(";min #Delta#phi(all jets,METnoMu);entries");
      shape.push_back("metnomu_significance(45,1.,10.)");histTitle.push_back(";METnoMu/#sigma(METnoMu);entries");
    }
    shape.push_back("dijet_sumeta(50,-10,10)");histTitle.push_back(";#eta_{j1}+#eta_{j2};entries");
    shape.push_back("ht(50,0,1000)");histTitle.push_back(";H_{T} (GeV);entries");
    //shape.push_back("ht30(50,0,1000)");histTitle.push_back(";H_{T} (GeV);entries");
    shape.push_back("jetunclet_mindphi(32,0,3.142)");histTitle.push_back(";min #Delta#phi(j,E_{T}^{uncl});entries");
    shape.push_back("metnomuunclet_dphi(32,0,3.142)");histTitle.push_back(";#Delta#phi(METnoMu,E_{T}^{uncl};entries");
    shape.push_back("dijetmetnomu_scalarSum_pt(70,0,1400)");histTitle.push_back(";p_{T}^{jeta}+p_{T}^{jetb}+METnoMu;entries");
    shape.push_back("dijetmetnomu_vectorialSum_pt(20,0,400)");histTitle.push_back(";p_{T}(#vec{ja}+#vec{jb}+#vec{METnoMu});entries");
    shape.push_back("n_jets_cjv_30(5,0,5)");histTitle.push_back(";CJV jets (30 GeV);entries");
    shape.push_back("n_jets_cjv_20EB_30EE(5,0,5)");histTitle.push_back(";CJV jets (20 GeV EB, 30 GeV EE);entries");
    shape.push_back("dijet_dphi(30,0.,3.)");histTitle.push_back(";#Delta#phi_{jj};entries");
    shape.push_back("dijet_deta(18,3.4,7.)");histTitle.push_back(";#Delta#eta_{jj};entries");
    shape.push_back("lep_mt(20,0.,100.)");histTitle.push_back(";m_{T}(lepton+MET (GeV);entries");
    shape.push_back("dijetmetnomu_ptfraction(20,0.,1.)");histTitle.push_back(";p_{T}^{dijet}/(p_{T}^{dijet}+METnoMu);entries");
    shape.push_back("ele1_pt(40,0.,200.)");histTitle.push_back(";p_{T}(electron) (GeV);entries");
    shape.push_back("mu1_pt(40,0.,200.)");histTitle.push_back(";p_{T}(muon) (GeV);entries");
    shape.push_back("jet1_csv(21,0.,1.05)");histTitle.push_back(";Jet 1 CSV;entries");
    shape.push_back("jet2_csv(21,0.,1.05)");histTitle.push_back(";Jet 2 CSV;entries");
    shape.push_back("jet3_csv(21,0.,1.05)");histTitle.push_back(";Jet 3 CSV;entries");
    if(channel=="mumu"){
      shape.push_back("m_mumu(30,0.,150.)");histTitle.push_back(";m_{#mu#mu};entries");
    }
  }
  else{
    shape.push_back("jet2_pt(13,40.,300.)");histTitle.push_back(";p_{T}^{j2} (GeV);entries");
    shape.push_back("jet1_pt(12,50.,300.)");histTitle.push_back(";p_{T}^{j1} (GeV);entries");
    shape.push_back("metnomuons(11,90.,300.)");histTitle.push_back(";METnoMu (GeV);entries");
    shape.push_back("met(30,0.,300.)");histTitle.push_back(";MET (GeV);entries");
    shape.push_back("l1met(10,00.,200.)");histTitle.push_back(";L1MET (GeV);entries");
    shape.push_back("dijet_M(5,1000.,2000.)");histTitle.push_back(";M_{jj} (GeV);entries");
    shape.push_back("sqrt(2*ele1_pt*mu1_pt*(cosh(ele1_eta-mu1_eta)-cos(ele1_phi-mu1_phi)))(40,0.,1000.)");histTitle.push_back(";M_{e#mu} (GeV);entries");
    shape.push_back("jet1_csv(21,0.,1.05)");histTitle.push_back(";Jet 1 CSV;entries");
    shape.push_back("jet2_csv(21,0.,1.05)");histTitle.push_back(";Jet 2 CSV;entries");
    shape.push_back("jet3_csv(21,0.,1.05)");histTitle.push_back(";Jet 3 CSV;entries");
    shape.push_back("jetmetnomu_mindphi(16,0.,3.142)");histTitle.push_back(";min #Delta#phi(j1/j2,METnoMu);entries");
    shape.push_back("alljetsmetnomu_mindphi(14,0.,3.5)");histTitle.push_back(";min #Delta#phi(all jets,METnoMu);entries");
    if(!do_preselranges&&!do_prepreselranges){ shape.push_back("metnomu_significance(17,3.,10.)");histTitle.push_back(";METnoMu/#sigma(METnoMu);entries");}
    else if(do_prepreselranges){shape.push_back("metnomu_significance(25,0.,10.)");histTitle.push_back(";METnoMu/#sigma(METnoMu);entries");}
    else{ shape.push_back("metnomu_significance(22,1.,10.)");histTitle.push_back(";METnoMu/#sigma(METnoMu);entries");}
    shape.push_back("dijet_sumeta(25,-10,10)");histTitle.push_back(";#eta_{j1}+#eta_{j2};entries");
    shape.push_back("ht(25,0,1000)");histTitle.push_back(";H_{T} (GeV);entries");
    //shape.push_back("ht30(25,0,1000)");histTitle.push_back(";H_{T} (GeV);entries");
    shape.push_back("jetunclet_mindphi(16,0,3.142)");histTitle.push_back(";min #Delta#phi(j,E_{T}^{uncl});entries");
    shape.push_back("metnomuunclet_dphi(16,0,3.142)");histTitle.push_back(";#Delta#phi(METnoMu,E_{T}^{uncl};entries");
    shape.push_back("dijetmetnomu_scalarSum_pt(35,0,1400)");histTitle.push_back(";p_{T}^{jeta}+p_{T}^{jetb}+METnoMu;entries");
    shape.push_back("dijetmetnomu_vectorialSum_pt(10,0,400)");histTitle.push_back(";p_{T}(#vec{ja}+#vec{jb}+#vec{METnoMu});entries");
    shape.push_back("n_jets_cjv_30(5,0,5)");histTitle.push_back(";CJV jets (30 GeV);entries");
    shape.push_back("n_jets_cjv_20EB_30EE(5,0,5)");histTitle.push_back(";CJV jets (20 GeV EB, 30 GeV EE);entries");
    shape.push_back("dijet_dphi(15,0.,3.)");histTitle.push_back(";#Delta#phi_{jj};entries");
    shape.push_back("dijet_deta(9,3.4,7.)");histTitle.push_back(";#Delta#eta_{jj};entries");
    shape.push_back("lep_mt(10,0.,100.)");histTitle.push_back(";m_{T}(lepton+MET (GeV);entries");
    shape.push_back("dijetmetnomu_ptfraction(10,0.,1.)");histTitle.push_back(";p_{T}^{dijet}/(p_{T}^{dijet}+METnoMu);entries");
  }
  
  std::string dataextrasel;
  if(!use_promptdata)dataextrasel="&&((((run>=190456)&&(run<=193621))&&passtrigger==1)||(((run>=193833)&&(run<=203742))&&passparkedtrigger1==1)||(((run>=203777)&&(run<=208686))&&passparkedtrigger2==1))&&l1met>=40";
  else dataextrasel="&&passtrigger==1&&l1met>=40";
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
    nunucat=("nvetomuons==0&&nvetoelectrons==0&&"+jetmetdphicut);
    nunuzcat="&&"+jetmetdphicut;
  }
  
  std::string mumucat="nselmuons==2&&nvetomuons==2&&nvetoelectrons==0&&m_mumu>60&&m_mumu<120&&"+jetmetdphicut+contonlycontplotjetmetdphi;
  std::string mumuzcat="nselmuons==2&&nvetomuons==2&&m_mumu>60&&m_mumu<120&&"+jetmetdphicut+contonlycontplotjetmetdphi;//zmumu

  std::string munucat="nselmuons==1&&nvetomuons==1&&nvetoelectrons==0&&"+jetmetdphicut+contonlycontplotjetmetdphi;//&&lep_mt>10";
  std::string munuzcat="&&nselmuons==1&&nvetomuons==1&&nvetoelectrons==0&&m_mumu>60&&m_mumu<120&&"+jetmetdphicut+contonlycontplotjetmetdphi;//wmu

  std::string enucat="nselelectrons==1&&nvetomuons==0&&nvetoelectrons==1&&"+jetmetdphicut+contonlycontplotjetmetdphi;
  std::string enuzcat="&&nselelectrons==1&&nvetoelectrons==1&&"+jetmetdphicut+contonlycontplotjetmetdphi;//wel

  //  std::string taunucat="ntaus==1&&nvetomuons==0&&nvetoelectrons==0&&lep_mt>20&&alljetsmetnomu_mindphi>1.0";
  //  std::string taunuzcat="&&ntaus==1&&nvetoelectrons==0&&alljetsmetnomu_mindphi>1.0";//wtau
  //std::string taunucat="ntaus==1&&nvetomuons==0&&nvetoelectrons==0&&lep_mt>20";//"+jetmetdphicut;
  //std::string taunuzcat="&&ntaus==1&&nvetoelectrons==0&&lep_mt>20";//"+jetmetdphicut;//wtau
  std::string taunucat="ntaus==1&&nvetomuons==0&&nvetoelectrons==0&&lep_mt>20&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;
  std::string taunuzcat="&&ntaus==1&&nvetoelectrons==0&&lep_mt>20&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;//wtau
  //std::string taunucat="ntaus==1&&nvetomuons==0&&nvetoelectrons==0&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;
  //std::string taunuzcat="&&ntaus==1&&nvetoelectrons==0&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;//wtau

  std::string topcat="nvetomuons==1&&nvetoelectrons==1&&nselmuons==1&&nselelectrons==1";//&&jetmetnomu_mindphi>1.0";
  std::string topzcat="&&nvetomuons==1&&nvetoelectrons==1&&nselmuons==1&&nselelectrons==1";//&&jetmetnomu_mindphi>1.0";//top
  //std::string topcat="nvetomuons==1&&nvetoelectrons==1&&nselmuons==1&&nselelectrons==1&&jetmetnomu_mindphi>1.0";
  //std::string topzcat="&&nvetomuons==1&&nvetoelectrons==1&&nselmuons==1&&nselelectrons==1&&jetmetnomu_mindphi>1.0";//top
//   std::string topcat="nvetomuons==1&&nvetoelectrons==1&&nselmuons==1";
//   std::string topzcat="&&nvetomuons==1&&nvetoelectrons==1&&nselmuons==1";//top

  std::string qcdcat="nvetoelectrons==0&&nvetomuons==0&&"+jetmetdphicut+contonlycontplotjetmetdphi;
  std::string qcdzcat="&&"+jetmetdphicut+contonlycontplotjetmetdphi;//QCD

  if(do_promptsel){
    nunucat=nunucat+"&&n_jets_cjv_30<1";
    nunuzcat=nunuzcat+"&&n_jets_cjv_30<1";
    enucat=enucat+"&&n_jets_cjv_30<1";
    enuzcat=enuzcat+"&&n_jets_cjv_30<1";
    munucat=munucat+"&&n_jets_cjv_30<1";
    munuzcat=munuzcat+"&&n_jets_cjv_30<1";
    mumucat=mumucat+"&&n_jets_cjv_30<1";
    mumuzcat=mumuzcat+"&&n_jets_cjv_30<1";
  }

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


  std::string sigmcweight;
  std::string mcweightpufactor="";
  if(syst=="PUUP") mcweightpufactor="*puweight_up_scale";
  if(syst=="PUDOWN") mcweightpufactor="*puweight_down_scale";
  
  if(channel=="nunu"||channel=="taunu"||channel=="qcd") sigmcweight="total_weight_lepveto"+mcweightpufactor;
  else sigmcweight="total_weight_leptight"+mcweightpufactor;

  std::string closurecat;
  std::string zextraclosurecat;
  std::string closurecontmcset;
  std::string closuremcweight;
  if(do_closure){
    if(closurebase=="enu"){
      closurecat=enucat;
      zextraclosurecat=enuzcat;
      closurecontmcset="WJets_enu";
      closuremcweight="total_weight_leptight"+mcweightpufactor;
    }
    else if(closurebase=="munu"){
      closurecat=munucat;
      zextraclosurecat=munuzcat;
      closurecontmcset="WJets_munu";
      closuremcweight="total_weight_leptight"+mcweightpufactor;

    }
    else if(closurebase=="taunu"){
      closurecat=taunucat;
      zextraclosurecat=taunuzcat;
      closurecontmcset="WJets_taunu";
      closuremcweight="total_weight_lepveto"+mcweightpufactor;
    }
    else{
      std::cout<<"Error: closurebase "<<closurebase<<" not supported, exiting"<<std::endl;
      return 1;
    }
  }
      

    
  DataShape topraw("topraw");
  topraw.set_dataset("Top")
    .set_dirname("top")
    .set_shape(shape)    
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat);

  //WBKG SHAPE GENERATION
  std::vector<std::string> Wcontbkgsets; //List of sets for ncbkg
  Wcontbkgsets.push_back("VV");
  Wcontbkgsets.push_back("Top");
  //Wcontbkgsets.push_back("WGamma");

  //Wcontbkgsets.push_back("VBF-QCD");
//   Wcontbkgsets.push_back("ZJets_ll");
//   Wcontbkgsets.push_back("ZJets_ll_vbf");
//   Wcontbkgsets.push_back("ZJets_nunu");

  std::vector<std::string> Wcontbkgextrafactordir;//list of dirs with data driven weights for above backgrounds
  Wcontbkgextrafactordir.push_back("");
  if(do_datatop) Wcontbkgextrafactordir.push_back("top");
  else Wcontbkgextrafactordir.push_back("");
  //Wcontbkgextrafactordir.push_back("");

  std::vector<int> Wcontbkgisz;
  Wcontbkgisz.push_back(0);
  Wcontbkgisz.push_back(0);
  //Wcontbkgisz.push_back(0);

  std::vector<ClosureTest> enutests;
  std::vector<ClosureTest> mumutests;
  std::vector<ClosureTest> taunutests;

  std::vector<std::string> shapes;
  std::vector<std::vector<double> > enubinning;
  std::vector<std::vector<double> > taunubinning;
  
  shapes.push_back("dijet_M");
  enubinning.push_back({1200.,1500.,2000.,3000.});
  taunubinning.push_back({1200.,1500.,2000.,3000.});
  shapes.push_back("alljetsmetnomu_mindphi");
  enubinning.push_back({2.3,2.6,2.9,3.2});
  taunubinning.push_back({1.,1.5,2.0,2.3,3.2});
  shapes.push_back("metnomu_significance");
  enubinning.push_back({4.,5.,6.,10.,20.});
  taunubinning.push_back({4.,5.,6.,10.,20.});
  shapes.push_back("metnomuons");
  enubinning.push_back({90.,130.,200.,300,500});
  taunubinning.push_back({90.,130.,200.,300,500});
  

  for(unsigned iShape=0;iShape<shapes.size();iShape++){

    ClosureTest closureenu("closureenu");
    closureenu.set_sigmcset("WJets_enu")
      .set_shape("jet2_pt")
      .set_dirname("closure")
      .set_dataset(dataset)
      .set_contbkgset(Wcontbkgsets)
      .set_contbkgextrafactordir(Wcontbkgextrafactordir)
      .set_contbkgisz(Wcontbkgisz)
      .set_sigmcweight("total_weight_leptight")
      .set_basesel(analysis->baseselection())
      .set_dataextrasel(dataextrasel)
      .set_sigcat(enucat)
      .set_contmcset(closurecontmcset)
      .set_contcat(closurecat)
      .set_shape(shapes[iShape])
      .set_binning(enubinning[iShape])
      .set_contmcweight(closuremcweight);
    
    enutests.push_back(closureenu);

    ClosureTest closuremumu("closuremumu");
    closuremumu.set_sigmcset("ZJets_ll_all")
      .set_shape("jet2_pt")
      .set_dirname("closure")
      .set_dataset(dataset)
      .set_contbkgset(Wcontbkgsets)
      .set_contbkgextrafactordir(Wcontbkgextrafactordir)
      .set_contbkgisz(Wcontbkgisz)
      .set_sigmcweight("total_weight_leptight")
      .set_basesel(analysis->baseselection())
      .set_dataextrasel(dataextrasel)
      .set_sigmcextrasel("&&m_mumu_gen>80&&m_mumu_gen<100")
      .set_sigcat(mumuzcat)
      .set_contmcset(closurecontmcset)
      .set_contcat(closurecat)
      .set_shape(shapes[iShape])
      .set_binning(enubinning[iShape])
      .set_contmcweight(closuremcweight);
    
    mumutests.push_back(closuremumu);

    ClosureTest closuretaunu("closuretaunu");
    closuretaunu.set_sigmcset("WJets_taunu")
      .set_shape("jet2_pt")
      .set_dirname("closure")
      .set_dataset(dataset)
      .set_contbkgset(Wcontbkgsets)
      .set_contbkgextrafactordir(Wcontbkgextrafactordir)
      .set_contbkgisz(Wcontbkgisz)
      .set_sigmcweight("total_weight_lepveto")
      .set_basesel(analysis->baseselection())
      .set_dataextrasel(dataextrasel)
      .set_sigcat(taunucat)
      .set_contmcset(closurecontmcset)
      .set_contcat("nselmuons==1&&nvetomuons==1&&nvetoelectrons==0")
      .set_shape(shapes[iShape])
      .set_binning(taunubinning[iShape])
      .set_additionalsyst(0.2)
      .set_contmcweight(closuremcweight);

    taunutests.push_back(closuretaunu);
    
  }
  //TOP
  std::vector<std::string> Topcontbkgsets;
  Topcontbkgsets.push_back("VV");
  //Topcontbkgsets.push_back("WGamma");
  Topcontbkgsets.push_back("WJets_enu");
  Topcontbkgsets.push_back("WJets_munu");
  Topcontbkgsets.push_back("WJets_taunu");

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


  /*##########################################
  #                                          #
  #   SET UP ANALYSIS SEQUENCE AND RUN       #
  #                                          #
  ##########################################*/
  
  if(do_datatop)analysis->AddModule(&top);
  else analysis->AddModule(&topraw);

  for(unsigned iShape=0;iShape<shapes.size();iShape++){
    analysis->AddModule(&enutests[iShape]);
    analysis->AddModule(&mumutests[iShape]);
    analysis->AddModule(&taunutests[iShape]);
  }
  
  analysis->RunAnalysis();

  return 0;

}
