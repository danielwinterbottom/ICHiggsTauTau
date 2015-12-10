#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeAnalyser.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataNormShape.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataZNormShape.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataShape.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/TrigEff.h"
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
  std::string contonlycontplotjetmetdphi;
  std::string cjvcut;
  std::string channel;
  std::string syst;
  std::string dataset;

  std::string trigpresel;
  std::string trigtrigcut;

  bool do_singlemu;
  bool do_mettrig;
  bool do_trigeff;
  bool do_run1;
  bool do_nosigmcweight;
  bool do_promptsel;
  bool use_promptdata;
  bool do_datatop;
  bool do_singletop;
  bool do_separatez;
  bool runblind;
  bool runblindreg;
  bool blindcutreg;
  bool do_bdt;
  std::string bdtcut;
  bool do_pasfig;
  bool do_pasqcdfigleft;
  bool do_preselranges;
  bool do_prepreselranges;
  bool do_relaxedqcdranges;
  bool do_plotmcqcd;
  bool do_plotqcd;
  bool dataonly;
  bool datalist;
  bool do_list;
  std::string listset;
  bool do_latex;
  bool do_closure;
  bool do_expandtopcat;
  bool do_logy;
  std::string closurebase;
  bool run2;
  bool do_mcbkg;

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
    ("trigpresel",                po::value<std::string>(&trigpresel)->default_value("1==1"))
    ("trigtrigcut",                po::value<std::string>(&trigtrigcut)->default_value("pass_sigtrigger>0"))
    ("dataonly",                 po::value<bool>(&dataonly)->default_value(false))
    ("datalist",                 po::value<bool>(&datalist)->default_value(false))
    ("do_list",                  po::value<bool>(&do_list)->default_value(false))
    ("listset",                  po::value<std::string>(&listset)->default_value("Top"))
    ("basesel",                  po::value<std::string>(&basesel)->default_value("jet1_eta*jet2_eta<0 && jet1_eta<4.7 && jet2_eta<4.7 && dijet_M>=600&&jet1_pt>50&&dijet_deta>3.6&& jet2_pt>60&&metnomuons>60&&metnomu_significance>3&&jetmetnomu_mindphi>1.5"))
    ("jetmetdphicut",            po::value<std::string>(&jetmetdphicut)->default_value("alljetsmetnomu_mindphi>1.0"))
    ("contonlycontplotjetmetdphi",po::value<std::string>(&contonlycontplotjetmetdphi)->default_value(""))
    ("cjvcut",                   po::value<std::string>(&cjvcut)->default_value("n_jets_cjv_30<1"))
    ("channel",                  po::value<std::string>(&channel)->default_value("nunu"))
    ("do_datatop",               po::value<bool>(&do_datatop)->default_value(true))
    ("do_singletop",             po::value<bool>(&do_singletop)->default_value(false))
    ("do_separatez",             po::value<bool>(&do_separatez)->default_value(false))
    ("runblind",                 po::value<bool>(&runblind)->default_value(true))
    ("do_bdt",                   po::value<bool>(&do_bdt)->default_value(false))
    ("bdtcut",                   po::value<std::string>(&bdtcut)->default_value("BDT>-0.26"))
    ("do_pasfig",                po::value<bool>(&do_pasfig)->default_value(false))
    ("do_pasqcdfigleft",         po::value<bool>(&do_pasqcdfigleft)->default_value(false))
    ("do_latex",                 po::value<bool>(&do_latex)->default_value(false))
    ("do_closure",               po::value<bool>(&do_closure)->default_value(false))
    ("closurebase",              po::value<std::string>(&closurebase)->default_value("munu"))
    ("do_expandtopcat",          po::value<bool>(&do_expandtopcat)->default_value(false))
    ("do_preselranges",          po::value<bool>(&do_preselranges)->default_value(false))
    ("do_prepreselranges",       po::value<bool>(&do_prepreselranges)->default_value(false))
    ("do_relaxedqcdranges",      po::value<bool>(&do_relaxedqcdranges)->default_value(false))
    ("do_promptsel",             po::value<bool>(&do_promptsel)->default_value(false))
    ("do_run1",                  po::value<bool>(&do_run1)->default_value(false))
    ("do_mettrig",               po::value<bool>(&do_mettrig)->default_value(false))
    ("do_trigeff",               po::value<bool>(&do_trigeff)->default_value(false))
    ("do_nosigmcweight",         po::value<bool>(&do_nosigmcweight)->default_value(false))
    ("do_singlemu",              po::value<bool>(&do_singlemu)->default_value(false))
    ("use_promptdata",           po::value<bool>(&use_promptdata)->default_value(false))
    ("do_plotmcqcd",             po::value<bool>(&do_plotmcqcd)->default_value(false))
    ("do_plotqcd",               po::value<bool>(&do_plotqcd)->default_value(false))
    ("do_logy",                  po::value<bool>(&do_logy)->default_value(false))
    ("blindcutreg",              po::value<bool>(&blindcutreg)->default_value(true))
    ("runblindreg",              po::value<bool>(&runblindreg)->default_value(true))
    ("run2",                     po::value<bool>(&run2)->default_value(false))
    ("do_mcbkg",                 po::value<bool>(&do_mcbkg)->default_value(false));

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

  if(do_plotqcd&&channel!="nunu"){
    std::cout<<"QCD plotting only valid for nunu region, turning it off in this region"<<std::endl;
    do_plotqcd=false;
  }

  //Set selection step common to all categories
  //analysis->set_baseselection("passtrigger==1&&jet1_eta<4.7&&jet2_eta<4.7&& jet1_pt>"+boost::lexical_cast<std::string>(jet1ptcut)+"&& jet2_pt>"+boost::lexical_cast<std::string>(jet2ptcut)+" && dijet_M >"+boost::lexical_cast<std::string>(mjjcut)+"&& jet1_eta*jet2_eta<"+boost::lexical_cast<std::string>(etaprodcut)+"&& dijet_dphi<"+boost::lexical_cast<std::string>(dphicut)+"&& dijet_deta >"+boost::lexical_cast<std::string>(detacut));
  //analysis->set_baseselection("jet1_pt>50&&jet2_pt>50&&dijet_deta>3.6&&metnomu_significance>3&&jetmetnomu_mindphi>1.5");
  //analysis->set_baseselection("dijet_M>=1100&&l1met>=40&&jet1_pt>50&&jet2_pt>50&&metnomuons>60&&dijet_deta>3.6&&metnomu_significance>4.0&&jetmetnomu_mindphi>2.0");//BEST SO FAR
  //analysis->set_baseselection("jet1_eta*jet2_eta<0 && jet1_eta<4.7 && jet2_eta<4.7 && dijet_M>=1100&&jet1_pt>50&&dijet_deta>3.5&& jet2_pt>40&&metnomuons>90&&metnomu_significance>3.5&&jetmetnomu_mindphi>1.5");//&&jetunclet_mindphi<2.5&&jetunclet_mindphi>0.8");
  analysis->set_baseselection(basesel);
  
  /*##########################################
  #                                          #
  #            DEFINE MODULES                #
  #                                          #
  ##########################################*/

  //ADD FRIENDS TO GET EXTRA VARIABLES
  std::vector<std::string> setswithfriends;
  setswithfriends.push_back("WJets_taunu");
  setswithfriends.push_back("WJets_munu");
  setswithfriends.push_back("WJets_enu");
  setswithfriends.push_back("VBF-QCD");
  //setswithfriends.push_back("MET");
  //setswithfriends.push_back("PARKED");
  //setswithfriends.push_back("PARKEDPLUSA");
  setswithfriends.push_back("SPLITPARKEDPLUSA");
  setswithfriends.push_back("VV");
  setswithfriends.push_back("Top");
  setswithfriends.push_back("ZJets_ll");
  setswithfriends.push_back("ZJets_ll_vbf");
  setswithfriends.push_back("ZJets_nunu");
  setswithfriends.push_back("sig110");
  setswithfriends.push_back("ggH110");
  setswithfriends.push_back("sig125");
  setswithfriends.push_back("ggH125");
  setswithfriends.push_back("sig150");
  setswithfriends.push_back("ggH150");
  setswithfriends.push_back("sig200");
  setswithfriends.push_back("ggH200");
  setswithfriends.push_back("sig300");
  setswithfriends.push_back("ggH300");
  setswithfriends.push_back("sig400");
  setswithfriends.push_back("ggH400");
  
  AddFriends addfriends("addfriends");
  addfriends.set_frienddir("output_newbdt/")
    .set_friendtreename("mvafriend")
    .set_sets(setswithfriends);

  std::vector<std::string> histTitle;
  std::vector<std::string> shape;
  //shape.push_back("BDT(12,-1.,0.2)");

  //shape.push_back("n_jets_30(10,0.,10.)");histTitle.push_back(";n_{jets} (p_{T}>30 GeV);Events");
  if(!(channel=="taunu"||channel=="top"||channel=="mumu")){
    shape.push_back("jet2_pt(30,0.,300.)");histTitle.push_back(";p_{T}^{j2} (GeV);Events");
    shape.push_back("jet1_pt(35,0.,350.)");histTitle.push_back(";p_{T}^{j1} (GeV);Events");
    shape.push_back("cjvjetpt(27,15.,150.)");histTitle.push_back(";p_{T}^{CJV jet} (GeV);Events");
    shape.push_back("metnomuons(20,0.,550.)");histTitle.push_back(";Missing transverse energy (GeV);Events");
    shape.push_back("met(30,0.,300.)");histTitle.push_back(";MET (GeV);Events");
    shape.push_back("l1met(20,0.,200.)");histTitle.push_back(";L1MET (GeV);Events");
    //shape.push_back("dijet_M(17,300.,2000.)");histTitle.push_back(";M_{jj} (GeV);Events");
    if(!do_preselranges&&!do_prepreselranges&&!do_relaxedqcdranges){
      shape.push_back("dijet_M(18,0.,3000.)");histTitle.push_back(";M_{jj} (GeV);Events");
      //shape.push_back("alljetsmetnomu_mindphi(12,0.0,3.1416)");histTitle.push_back(";min #Delta#phi(all jets,METnoMu);Events");
      shape.push_back("jetmetnomu_mindphi(32,0,3.142)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j_{1,2});Events");
      shape.push_back("alljetsmetnomu_mindphi(32,0,3.1416)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j);Events");
      if(do_pasqcdfigleft){shape.push_back("metnomu_significance(50,0.,200.)");histTitle.push_back(";S;Events");}
      else shape.push_back("metnomu_significance(50,0.,200.)");histTitle.push_back(";S;Events");
    }
    else if(do_prepreselranges){
      shape.push_back("dijet_M(22,800.,3000.)");histTitle.push_back(";M_{jj} (GeV);Events");
      shape.push_back("jetmetnomu_mindphi(32,0.,3.142)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j_{1,2});Events");
      shape.push_back("alljetsmetnomu_mindphi(32,0.,3.142)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j);Events");
      shape.push_back("metnomu_significance(50,0.,10.)");histTitle.push_back(";S;Events");
      shape.push_back("met_significance(50,0.,10.)");histTitle.push_back(";METnoMu/#sigma(METnoMu);Events");
    }
    else if(do_relaxedqcdranges){
      shape.push_back("dijet_M(18,1200.,3000.)");histTitle.push_back(";M_{jj} (GeV);Events");
      shape.push_back("jetmetnomu_mindphi(32,0.,3.142)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j_{1,2});Events");
      shape.push_back("alljetsmetnomu_mindphi(32,0.,3.142)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j);Events");
      shape.push_back("metnomu_significance(33,3.,15.)");histTitle.push_back(";S;Events");
    }
    else{
      shape.push_back("dijet_M(30,0.,3000.)");histTitle.push_back(";M_{jj} (GeV);Events");
      shape.push_back("alljetsmetnomu_mindphi(22,0.0,3.1416)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j);Events");
      //shape.push_back("alljetsmetnomu_mindphi(32,0.,3.1416)");histTitle.push_back(";min #Delta#phi(all jets,METnoMu);Events");
      shape.push_back("metnomu_significance(45,1.,10.)");histTitle.push_back(";S;Events");
    }
    shape.push_back("dijet_sumeta(50,-10,10)");histTitle.push_back(";#eta_{j1}+#eta_{j2};Events");
    shape.push_back("ht(50,0,1000)");histTitle.push_back(";H_{T} (GeV);Events");
    //shape.push_back("ht30(50,0,1000)");histTitle.push_back(";H_{T} (GeV);Events");
    shape.push_back("jetunclet_mindphi(32,0,3.1416)");histTitle.push_back(";min #Delta#phi(j,E_{T}^{uncl});Events");
    shape.push_back("metnomuunclet_dphi(32,0,3.1416)");histTitle.push_back(";#Delta#phi(METnoMu,E_{T}^{uncl};Events");
    shape.push_back("dijetmetnomu_scalarSum_pt(35,0,1400)");histTitle.push_back(";p_{T}^{jeta}+p_{T}^{jetb}+METnoMu;Events");
    shape.push_back("dijetmetnomu_vectorialSum_pt(20,0,400)");histTitle.push_back(";p_{T}(#vec{ja}+#vec{jb}+#vec{METnoMu});Events");
    shape.push_back("n_jets_cjv_30(5,0,5)");histTitle.push_back(";CJV jets (30 GeV);Events");
    shape.push_back("n_jets_cjv_20EB_30EE(5,0,5)");histTitle.push_back(";CJV jets (20 GeV EB, 30 GeV EE);Events");
    shape.push_back("dijet_dphi(15,0.,3.1416)");histTitle.push_back(";#Delta#phi_{jj};Events");
    shape.push_back("dijet_deta(36,0,7.)");histTitle.push_back(";#Delta#eta_{jj};Events");
    shape.push_back("lep_mt(20,0.,200.)");histTitle.push_back(";m_{T}(lepton+MET) (GeV);Events");
    shape.push_back("dijetmetnomu_ptfraction(20,0.,1.)");histTitle.push_back(";p_{T}^{dijet}/(p_{T}^{dijet}+METnoMu);Events");
    shape.push_back("ele1_pt(40,0.,200.)");histTitle.push_back(";p_{T}(electron) (GeV);Events");
    shape.push_back("mu1_pt(40,0.,200.)");histTitle.push_back(";p_{T}(muon) (GeV);Events");
    // shape.push_back("jet_csv1(21,0.6,1.5)");histTitle.push_back(";Jet 1 CSV;Events");
    // shape.push_back("jet_csv2(21,0.6,1.5)");histTitle.push_back(";Jet 2 CSV;Events");
    // shape.push_back("jet_csv3(21,0.,1.)");histTitle.push_back(";Jet 3 CSV;Events");
    shape.push_back("n_jets_30(10,0.,10.)");histTitle.push_back(";N jets pt>30 GeV;Events");
    shape.push_back("n_jets_15(15,0.,15.)");histTitle.push_back(";N jets pt>15 GeV;Events");
    shape.push_back("central_tag_eta(25,-5.,5.)");histTitle.push_back(";Central tag jet #eta;Events");
    shape.push_back("forward_tag_eta(25,-5.,5.)");histTitle.push_back(";Forward tag jet #eta;Events");
    if(channel=="mumu"){
      shape.push_back("m_mumu(30,0.,150.)");histTitle.push_back(";m_{#mu#mu};Events");
    }
  }
  else{
    shape.push_back("jet2_pt(13,40.,300.)");histTitle.push_back(";p_{T}^{j2} (GeV);Events");
    shape.push_back("jet1_pt(12,50.,300.)");histTitle.push_back(";p_{T}^{j1} (GeV);Events");
    shape.push_back("metnomuons(15,0.,550.)");histTitle.push_back(";Missing transverse energy (GeV);Events");
    shape.push_back("met(30,0.,300.)");histTitle.push_back(";MET (GeV);Events");
    shape.push_back("l1met(10,00.,200.)");histTitle.push_back(";L1MET (GeV);Events");
    shape.push_back("dijet_M(5,1200.,2000.)");histTitle.push_back(";M_{jj} (GeV);Events");
    shape.push_back("sqrt(2*ele1_pt*mu1_pt*(cosh(ele1_eta-mu1_eta)-cos(ele1_phi-mu1_phi)))(40,0.,1000.)");histTitle.push_back(";M_{e#mu} (GeV);Events");
    // shape.push_back("jet_csv1(21,0.,1.05)");histTitle.push_back(";Jet 1 CSV;Events");
    // shape.push_back("jet_csv2(21,0.,1.05)");histTitle.push_back(";Jet 2 CSV;Events");
    // shape.push_back("jet_csv3(21,0.,1.05)");histTitle.push_back(";Jet 3 CSV;Events");
    shape.push_back("jetmetnomu_mindphi(16,0.,3.1416)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j_{1,2});Events");
    if(channel=="taunu"){ shape.push_back("alljetsmetnomu_mindphi(22,0.,3.1416)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j);Events");}
    else if(channel=="top"){ shape.push_back("alljetsmetnomu_mindphi(11,0.,3.1416)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j);Events");}
    else{ shape.push_back("alljetsmetnomu_mindphi(16,0,3.1416)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j);Events");}
    if(!do_preselranges&&!do_prepreselranges){ shape.push_back("metnomu_significance(6,4.,10.)");histTitle.push_back(";S;Events");}
    else if(do_prepreselranges){shape.push_back("metnomu_significance(25,0.,10.)");histTitle.push_back(";S;Events");}
    else{ shape.push_back("metnomu_significance(22,1.,10.)");histTitle.push_back(";S;Events");}
    shape.push_back("dijet_sumeta(25,-10,10)");histTitle.push_back(";#eta_{j1}+#eta_{j2};Events");
    shape.push_back("ht(25,0,1000)");histTitle.push_back(";H_{T} (GeV);Events");
    //shape.push_back("ht30(25,0,1000)");histTitle.push_back(";H_{T} (GeV);Events");
    shape.push_back("jetunclet_mindphi(16,0,3.1416)");histTitle.push_back(";min #Delta#phi(j,E_{T}^{uncl});Events");
    shape.push_back("metnomuunclet_dphi(16,0,3.1416)");histTitle.push_back(";#Delta#phi(METnoMu,E_{T}^{uncl};Events");
    shape.push_back("dijetmetnomu_scalarSum_pt(35,0,1400)");histTitle.push_back(";p_{T}^{jeta}+p_{T}^{jetb}+METnoMu;Events");
    shape.push_back("dijetmetnomu_vectorialSum_pt(10,0,400)");histTitle.push_back(";p_{T}(#vec{ja}+#vec{jb}+#vec{METnoMu});Events");
    shape.push_back("n_jets_cjv_30(5,0,5)");histTitle.push_back(";CJV jets (30 GeV);Events");
    shape.push_back("n_jets_cjv_20EB_30EE(5,0,5)");histTitle.push_back(";CJV jets (20 GeV EB, 30 GeV EE);Events");
    shape.push_back("dijet_dphi(15,0.,3.)");histTitle.push_back(";#Delta#phi_{jj};Events");
    shape.push_back("dijet_deta(18,0,7.)");histTitle.push_back(";#Delta#eta_{jj};Events");
    shape.push_back("lep_mt(20,0.,200.)");histTitle.push_back(";m_{T}(lepton+MET) (GeV);Events");
    shape.push_back("tau1_pt(40,0.,200.)");histTitle.push_back(";p_{T}(tau) (GeV);Events");
    shape.push_back("dijetmetnomu_ptfraction(10,0.,1.)");histTitle.push_back(";p_{T}^{dijet}/(p_{T}^{dijet}+METnoMu);Events");
    shape.push_back("n_jets_30(10,0.,10.)");histTitle.push_back(";N jets pt>30 GeV;Events");
    shape.push_back("n_jets_15(15,0.,15.)");histTitle.push_back(";N jets pt>15 GeV;Events");
    shape.push_back("central_tag_eta(25,-5.,5.)");histTitle.push_back(";Central tag jet #eta;Events");
    shape.push_back("forward_tag_eta(25,-5.,5.)");histTitle.push_back(";Forward tag jet #eta;Events");
    //mindR(tau,tagjets)
    shape.push_back("mymath::deltaRmin(jet1_eta,jet1_phi,jet2_eta,jet2_phi,tau1_eta,tau1_phi)(20,0.,4.)");histTitle.push_back(";min#DeltaR(#tau,tag jets);Events");
    if(channel=="mumu"){
      shape.push_back("m_mumu(30,0.,150.)");histTitle.push_back(";m_{#mu#mu};Events");
    }
  }
  
  std::string dataextrasel;
  if(!do_run1){
    if(!do_mettrig) dataextrasel="&&(pass_sigtrigger==1)";
    else dataextrasel="&&(pass_mettrigger==1)";
  }
  else{
    if(!use_promptdata)dataextrasel="&&((((run>=190456)&&(run<=193621))&&passtrigger==1)||(((run>=193833)&&(run<=203742))&&passparkedtrigger1==1)||(((run>=203777)&&(run<=208686))&&passparkedtrigger2==1))&&l1met>=40";
    else dataextrasel="&&passtrigger==1&&l1met>=40";
  }
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
  if(!do_singlemu){
    munucat="nselmuons==1&&nvetomuons==1&&nvetoelectrons==0&&"+jetmetdphicut;//&&lep_mt>10";
    munuzcat="&&nselmuons==1&&nvetomuons==1&&nvetoelectrons==0&&m_mumu>60&&m_mumu<120&&"+jetmetdphicut;//wmu
  }
  else{
    munucat="mu1_pt>25&&nselmuons==1&&nvetomuons==1&&nvetoelectrons==0&&"+jetmetdphicut;//&&lep_mt>10";
    munuzcat="&&mu1_pt>25&&nselmuons==1&&nvetomuons==1&&nvetoelectrons==0&&m_mumu>60&&m_mumu<120&&"+jetmetdphicut;//wmu
  }

  std::string enucat="nselelectrons==1&&nvetomuons==0&&nvetoelectrons==1&&"+jetmetdphicut;
  std::string enuzcat="&&nselelectrons==1&&nvetoelectrons==1&&"+jetmetdphicut;//wel

  std::string taunucat;
  if(!do_prepreselranges)taunucat="ntaus==1&&nvetomuons==0&&nvetoelectrons==0&&lep_mt>20&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;
  else taunucat="ntaus==1&&nvetomuons==0&&nvetoelectrons==0&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;
  std::string taunuzcat;
  if(!do_prepreselranges)taunuzcat="&&ntaus==1&&nvetoelectrons==0&&lep_mt>20&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;//wtau
  else taunuzcat="&&ntaus==1&&nvetoelectrons==0&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;//wtau


  //std::string taunucat="nvetomuons==0&&nvetoelectrons==0";//"+jetmetdphicut;
  //std::string taunuzcat="&&nvetoelectrons==0";//"+jetmetdphicut;//wtau
  //  std::string taunucat="ntaus==1&&nvetomuons==0&&nvetoelectrons==0&&lep_mt>20&&alljetsmetnomu_mindphi>1.0";
  //  std::string taunuzcat="&&ntaus==1&&nvetoelectrons==0&&alljetsmetnomu_mindphi>1.0";//wtau
  //std::string taunucat="ntaus==1&&nvetomuons==0&&nvetoelectrons==0&&lep_mt>20";//"+jetmetdphicut;
  //std::string taunuzcat="&&ntaus==1&&nvetoelectrons==0&&lep_mt>20";//"+jetmetdphicut;//wtau
  //std::string taunucat="ntaus==1&&nvetomuons==0&&nvetoelectrons==0&&lep_mt>20";//"+jetmetdphicut;
  //std::string taunuzcat="&&ntaus==1&&nvetoelectrons==0&&lep_mt>20";//"+jetmetdphicut;//wtau
  //std::string taunucat="ntaus==1&&nvetomuons==0&&nvetoelectrons==0&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;
  //std::string taunuzcat="&&ntaus==1&&nvetoelectrons==0&&jetmetnomu_mindphi>1.0";//"+jetmetdphicut;//wtau

  std::string topcat,topzcat;
  if(do_expandtopcat){
    //    topcat="((nvetomuons==1&&nvetoelectrons==1&&nselmuons==1&&nselelectrons==1)||((m_ee<60||m_ee>120)&&nvetoelectrons==2&&nselelectrons==2&&nvetomuons==0)||((m_mumu<60||m_mumu>120)&&nvetoelectrons==0&&nselmuons==2&&nvetomuons==2))";//&&jetmetnomu_mindphi>1.0";
    //topzcat="&&((nvetomuons==1&&nvetoelectrons==1&&nselmuons==1&&nselelectrons==1)||((m_ee<60||m_ee>120)&&nvetoelectrons==2&&nselelectrons==2&&nvetomuons==0)||((m_mumu<60||m_mumu>120)&&nvetoelectrons==0&&nselmuons==2&&nvetomuons==2))";//&&jetmetnomu_mindphi>1.0";//top
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

  //std::string topcat="nvetomuons==1&&nvetoelectrons==1&&nselmuons==1&&nselelectrons==1&&jetmetnomu_mindphi>1.0";
  //std::string topzcat="&&nvetomuons==1&&nvetoelectrons==1&&nselmuons==1&&nselelectrons==1&&jetmetnomu_mindphi>1.0";//top
//   std::string topcat="nvetomuons==1&&nvetoelectrons==1&&nselmuons==1";
//   std::string topzcat="&&nvetomuons==1&&nvetoelectrons==1&&nselmuons==1";//top

  std::string gammacat="ntightphotons==1&&nvetomuons==0&&nvetoelectrons==0&&"+jetmetdphicut;//&&lep_mt>10";
  std::string gammazcat="&&ntightphotons==1&&nvetomuons==0&&nvetoelectrons==0&&"+jetmetdphicut;//wmu

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
  else if(channel=="gamma"){
    sigcat=gammacat;
    zextrasigcat=gammazcat;
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
  
  if(channel=="nunu"||channel=="taunu"||channel=="qcd"||channel=="gamma") sigmcweight="total_weight_lepveto"+mcweightpufactor;
  else sigmcweight="total_weight_leptight"+mcweightpufactor;
  if(do_nosigmcweight)sigmcweight="1";

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
      

  std::string bothcentral="jet1_eta<3&&jet1_eta>-3&&jet2_eta<3&&jet2_eta>-3";
  std::string bothforward="(jet1_eta>3||jet1_eta<-3)&&(jet2_eta>3||jet2_eta<-3)";
  std::string j2forwardj1central="((jet1_eta<3||jet1_eta>-3)&&(jet2_eta>3||jet2_eta<-3))";
  std::string j1forwardj2central="((jet1_eta>3||jet1_eta<-3)&&(jet2_eta<3||jet2_eta>-3))";

  //std::string additionalcut="&&("+j2forwardj1central+"||"+j1forwardj2central+")";//"&&"+bothcentral;
  std::string additionalcut="";

  //std::string trigcut="pass_htquadjettrigger==1";
  //std::string trigcut="pass_controltrigger==1";
  //std::string trigcut="l1met>=60";
  std::string trigcut=trigtrigcut;//"pass_sigtrigger==1";
  std::string trigweight=trigtrigcut.substr(0,trigtrigcut.find(">"));
  std::cout<<trigweight<<std::endl;
  //std::string presel="pass_muontrigger==1&&l1met>=60";
  //std::string presel="pass_muontrigger==1";
  //std::string presel="l1met>=60";
  std::string presel=trigpresel;//"1==1";
  
  TrigEff metnomueff("metnomueff");
  metnomueff.set_dataset("SINGLEMUON")
    .set_dirname("eff")
    .set_shape("metnomuons(50,0.,1000.)")
    .set_basesel(presel+"&&jet1_pt>80&&jet2_pt>80&&dijet_M>600&&dijet_deta>3.6"+additionalcut)
    //.set_basesel(presel+"&&jet1_pt>50&&jet2_pt>50&&dijet_M>800&&dijet_deta>3.6"+additionalcut)
    //.set_basesel(presel+additionalcut)
    .set_cat("1")
    .set_histtitle("METnoMU;efficiency")
    .set_numweight("weight_nolep*"+trigweight).set_trigger(trigcut);

  TrigEff l1meteff("l1meteff");
  l1meteff.set_dataset("SINGLEMUON")
    .set_dirname("eff")
    .set_shape("l1met(50,0.,1000.)")
    .set_basesel(presel+"&&jet1_pt>80&&jet2_pt>80&&dijet_M>600&&dijet_deta>3.6"+additionalcut)
    //.set_basesel(presel+"&&jet1_pt>50&&jet2_pt>50&&dijet_M>800&&dijet_deta>3.6"+additionalcut)
    //.set_basesel(presel+additionalcut)
    .set_cat("1")
    .set_histtitle("L1 MET;efficiency")
    .set_numweight("weight_nolep*"+trigweight).set_trigger(trigcut);

  TrigEff meteff("meteff");
  meteff.set_dataset("SINGLEMUON")
    .set_dirname("eff")
    .set_shape("met(50,0.,1000.)")
    .set_basesel(presel+"&&jet1_pt>80&&jet2_pt>80&&dijet_M>600&&dijet_deta>3.6"+additionalcut)
    //.set_basesel(presel+"&&jet1_pt>50&&jet2_pt>50&&dijet_M>800&&dijet_deta>3.6"+additionalcut)
    //.set_basesel(presel+additionalcut)
    .set_cat("1")
    .set_histtitle("MET;efficiency")
    .set_numweight("weight_nolep*"+trigweight).set_trigger(trigcut);

  TrigEff jpteff("jpteff");
  jpteff.set_dataset("SINGLEMUON")
    .set_dirname("eff")
    .set_shape("jet2_pt(20,0.,200.)")
    .set_basesel(presel+"&&jet1_pt>80&&dijet_M>600&&dijet_deta>3.6&&metnomuons>300"+additionalcut)
    //.set_basesel(presel+"&&jet1_pt>50&&dijet_M>800&&dijet_deta>3.6&&metnomuons>200"+additionalcut)
    //.set_basesel(presel+additionalcut)
    .set_cat("1")
    .set_histtitle("j_{2} p_{T};efficiency")
    .set_numweight("weight_nolep*"+trigweight).set_trigger(trigcut);

  TrigEff mjjeff("mjjeff");
  mjjeff.set_dataset("SINGLEMUON")
    .set_dirname("eff")
    .set_shape("dijet_M(15,0.,3000.)")
    .set_basesel(presel+"&&jet1_pt>80&&jet2_pt>80&&dijet_deta>3.6&&metnomuons>300"+additionalcut)
    //.set_basesel(presel+"&&jet1_pt>50&&jet2_pt>50&&dijet_deta>3.6&&metnomuons>200"+additionalcut)
    //.set_basesel(presel+additionalcut)
    .set_cat("1")
    .set_histtitle("M_{jj};efficiency")
    .set_numweight("weight_nolep*"+trigweight).set_trigger(trigcut);

  TrigEff detaeff("detaeff");
  detaeff.set_dataset("SINGLEMUON")
    .set_dirname("eff")
    .set_shape("dijet_deta(50,0.,10.)")
    .set_basesel(presel+"&&jet1_pt>80&&jet2_pt>80&&dijet_M>600&&metnomuons>300"+additionalcut)
    //.set_basesel(presel+"&&jet1_pt>50&&jet2_pt>50&&dijet_M>800&&metnomuons>200"+additionalcut)
    //.set_basesel(presel+additionalcut)
    .set_cat("1")
    .set_histtitle("#Delta#eta_{jj};efficiency")
    .set_numweight("weight_nolep*"+trigweight).set_trigger(trigcut);
    
   std::vector<TrigEff> efficiencies;
  
   std::vector<std::string> jptbinning;
   jptbinning.push_back("jet1_pt>80&&jet2_pt>70&&jet2_pt<=80");
   jptbinning.push_back("jet1_pt>80&&jet2_pt>80");
   std::vector<std::string> jptname;
   jptname.push_back("j2pt70-80");
   jptname.push_back("j2pt80-inf");

   std::vector<std::string> mjjbinning;
   mjjbinning.push_back("dijet_M>800&&dijet_M<=1000");
   mjjbinning.push_back("dijet_M>1000");
   std::vector<std::string> mjjname;
   mjjname.push_back("mjj800-1000");
   mjjname.push_back("mjj1000-inf");
  
   for(unsigned ijpt=0;ijpt<jptbinning.size();ijpt++){
     for(unsigned imjj=0;imjj<mjjbinning.size();imjj++){
       TrigEff imetnomueff("metnomueff_"+jptname[ijpt]+mjjname[imjj]);
       imetnomueff.set_dataset("SINGLEMUON")
	 .set_dirname("binnedeff"+jptname[ijpt]+mjjname[imjj])
	 .set_shape("metnomuons(46,80.,1000.)")
	 .set_basesel(presel+"&&"+jptbinning[ijpt]+"&&"+mjjbinning[imjj]+"&&dijet_deta>3.6"+additionalcut)
	 .set_cat("1")
	 .set_histtitle("METnoMU;efficiency")
	 .set_namestr(jptname[ijpt]+mjjname[imjj])
	 .set_do_fit(true)
	 //.set_numweight("weight_nolep*"+trigweight)
	 .set_trigger(trigcut);
       efficiencies.push_back(imetnomueff);
     }
   }
  

  // TrigEff j4pteff("j4pteff");
  // j4pteff.set_dataset("SINGLEMUON")
  //   .set_dirname("eff")
  //   .set_shape("jet4_pt(20,0.,200.)")
  //   //.set_basesel("jet1_pt>80&&dijet_M>600&&dijet_deta>3.6&&metnomuons>300"+additionalcut)
  //   .set_basesel("ht>1200")//&&dijet_M>600&&dijet_deta>3.6&&metnomuons>300"+additionalcut)
  //   .set_cat("1")
  //   .set_histtitle("j_{4} p_{T};efficiency")
  //   .set_numweight("weight_nolep*"+trigweight).set_trigger(trigcut);

  // TrigEff j3pteff("j3pteff");
  // j3pteff.set_dataset("SINGLEMUON")
  //   .set_dirname("eff")
  //   .set_shape("jet3_pt(10,0.,400.)")
  //   //.set_basesel("jet1_pt>80&&dijet_M>600&&dijet_deta>3.6&&metnomuons>300"+additionalcut)
  //   .set_basesel("ht>1200&&jet1_pt>80")//&&dijet_M>600&&dijet_deta>3.6&&metnomuons>300"+additionalcut)
  //   .set_cat("1")
  //   .set_histtitle("j_{3} p_{T};efficiency")
  //   .set_numweight("weight_nolep*"+trigweight).set_trigger(trigcut);

  // TrigEff j1pteff("j1pteff");
  // j1pteff.set_dataset("SINGLEMUON")
  //   .set_dirname("eff")
  //   .set_shape("jet1_pt(10,0.,400.)")
  //   //.set_basesel("jet1_pt>80&&dijet_M>600&&dijet_deta>3.6&&metnomuons>300"+additionalcut)
  //   .set_basesel("ht>1200")//&&dijet_M>600&&dijet_deta>3.6&&metnomuons>300"+additionalcut)
  //   .set_cat("1")
  //   .set_histtitle("j_{1} p_{T};efficiency")
  //   .set_numweight("weight_nolep*"+trigweight).set_trigger(trigcut);

  // TrigEff hteff("hteff");
  // hteff.set_dataset("SINGLEMUON")
  //   .set_dirname("eff")
  //   .set_shape("ht(10,0.,1500.)")
  //   //.set_basesel("jet1_pt>80&&dijet_M>600&&dijet_deta>3.6&&metnomuons>300"+additionalcut)
  //   .set_basesel("jet1_pt>80")//&&dijet_M>600&&dijet_deta>3.6&&metnomuons>300"+additionalcut)
  //   .set_cat("1")
  //   .set_histtitle("H_{T};efficiency")
  //   .set_numweight("weight_nolep*"+trigweight).set_trigger(trigcut);



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

  DataShape signal125("signal125");
  signal125.set_dataset("sig125")
    .set_dirname("qqH125")
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

  DataShape wgamma("wgamma");
  wgamma.set_dataset("WGamma")
    .set_dirname("wg")
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
  //Wcontbkgsets.push_back("WGamma");

  //Wcontbkgsets.push_back("VBF-QCD");
//   Wcontbkgsets.push_back("ZJets_ll");
//   Wcontbkgsets.push_back("ZJets_ll_vbf");
//   Wcontbkgsets.push_back("ZJets_nunu");

  std::vector<std::string> Wcontbkgextrafactordir;//list of dirs with data driven weights for above backgrounds
  Wcontbkgextrafactordir.push_back("");
  if(do_datatop&&!do_singletop) Wcontbkgextrafactordir.push_back("top");
  else Wcontbkgextrafactordir.push_back("");
  //Wcontbkgextrafactordir.push_back("");

  std::vector<int> Wcontbkgisz;
  Wcontbkgisz.push_back(0);
  Wcontbkgisz.push_back(0);
  //Wcontbkgisz.push_back(0);

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
    .set_sigcat(sigcat);
  if(!do_closure){
    wmunu.set_contmcset("WJets_munu")
    .set_contcat(munucat)//"nvetoelectrons==0 && nvetomuons==1 && nselmuons==1");
    .set_contmcweight("total_weight_leptight"+mcweightpufactor);
  }
  else{
    wmunu.set_contmcset(closurecontmcset)
    .set_contcat(closurecat)
    .set_contmcweight(closuremcweight);
  }   
  

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
    .set_sigmcweight(sigmcweight);
  if(!do_closure){
    wenu.set_contmcset("WJets_enu")
    .set_contcat(enucat)//"nselelectrons==1 && nvetoelectrons ==1 && nvetomuons==0");
    .set_contmcweight("total_weight_leptight"+mcweightpufactor);
  }
  else{
    wenu.set_contmcset(closurecontmcset)
    .set_contcat(closurecat)
    .set_contmcweight(closuremcweight);
  }   
  

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
    .set_sigmcweight(sigmcweight);
  if(!do_closure){
    wtaunu.set_contmcset("WJets_taunu")
    .set_contcat(taunucat)//"ntaus>=1&&nvetoelectrons ==0 && nvetomuons==0&&lep_mt>20")
    .set_contmcweight("total_weight_lepveto"+mcweightpufactor);
  }
  else{
    wtaunu.set_contmcset(closurecontmcset)
    .set_contcat(closurecat)
    .set_contmcweight(closuremcweight);
  }   

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
    .set_sigcat(qcdshapecat);
  if(!do_closure){
    wmunuqcd.set_contmcset("WJets_munu")
    .set_contcat(munuqcdcat)//"nvetoelectrons==0 && nvetomuons==1 && nselmuons==1");
    .set_contmcweight("total_weight_leptight"+mcweightpufactor);
  }
  else{
    wmunuqcd.set_contmcset(closurecontmcset)
    .set_contcat(closurecat)
    .set_contmcweight(closuremcweight);
  }   
  
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
    .set_sigmcweight(sigmcweight);
  if(!do_closure){
    wenuqcd.set_contmcset("WJets_enu")
    .set_contcat(enuqcdcat)//"nselelectrons==1 && nvetoelectrons ==1 && nvetomuons==0");
    .set_contmcweight("total_weight_leptight"+mcweightpufactor);
  }
  else{
    wenuqcd.set_contmcset(closurecontmcset)
    .set_contcat(closurecat)
    .set_contmcweight(closuremcweight);
  }   
  
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
    .set_sigmcweight(sigmcweight);
  if(!do_closure){
    wtaunuqcd.set_contmcset("WJets_taunu")
    .set_contcat(taunuqcdcat)//"ntaus>=1&&nvetoelectrons ==0 && nvetomuons==0&&lep_mt>20")
    .set_contmcweight("total_weight_lepveto"+mcweightpufactor);
  }
  else{
    wtaunuqcd.set_contmcset(closurecontmcset)
    .set_contcat(closurecat)
    .set_contmcweight(closuremcweight);
  }   

  
  //TOP
  std::vector<std::string> Topcontbkgsets;
  Topcontbkgsets.push_back("VV");
  //Topcontbkgsets.push_back("WGamma");
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
//   std::vector<std::string> QCDcontbkgsets; //list of sets for ncbkg
//   QCDcontbkgsets.push_back("VV");
//   QCDcontbkgsets.push_back("Top");
//   //QCDcontbkgsets.push_back("WGamma");
//    QCDcontbkgsets.push_back("ZJets_ll");
//    QCDcontbkgsets.push_back("ZJets_ll_vbf");
//   QCDcontbkgsets.push_back("WJets_enu");
//   QCDcontbkgsets.push_back("WJets_munu");
//   QCDcontbkgsets.push_back("WJets_taunu");

//   std::vector<std::string> QCDcontbkgextrafactordir;//list of dirs with data driven weights for above backgrounds
//     QCDcontbkgextrafactordir.push_back("");
//     QCDcontbkgextrafactordir.push_back("top");
//     //QCDcontbkgextrafactordir.push_back("");
//    QCDcontbkgextrafactordir.push_back("zvv");
//    QCDcontbkgextrafactordir.push_back("zvv");
//     QCDcontbkgextrafactordir.push_back("wel");
//     QCDcontbkgextrafactordir.push_back("wmu");
//     QCDcontbkgextrafactordir.push_back("wtau");
  
//   std::vector<int> QCDcontbkgisz;
//   QCDcontbkgisz.push_back(0);
//   QCDcontbkgisz.push_back(0);
//    QCDcontbkgisz.push_back(0);
//   if(channel!="mumu"){
//     QCDcontbkgisz.push_back(2);
//     QCDcontbkgisz.push_back(1);
//   }
//   else{
//     QCDcontbkgisz.push_back(0);
//     QCDcontbkgisz.push_back(0);
//   }
//     QCDcontbkgisz.push_back(0);
//     QCDcontbkgisz.push_back(0);
//     QCDcontbkgisz.push_back(0);


//   DataNormShape QCD("QCD");
//   QCD.set_sigmcset("VBF-QCD")//VBF-QCD")
//     .set_shape(shape)
//     .set_dirname("qcd")
//     .set_contmcset("VBF-QCD")//VBF-QCD")
//     .set_contdataset(dataset)
//     .set_contbkgset(QCDcontbkgsets)
//     .set_contbkgextrafactordir(QCDcontbkgextrafactordir)
//     .set_contbkgisz(QCDcontbkgisz)
//     .set_sigmcweight(sigmcweight)
//     .set_contmcweight("total_weight_lepveto"+mcweightpufactor)
//     .set_basesel(analysis->baseselection())
//     .set_contdataextrasel(dataextrasel)
//     .set_sigcat(sigcat)
//     .set_zcontcat("m_mumu_gen>80&&m_mumu_gen<100"+qcdzcat)
//     .set_contcat(qcdcat);


  //NORMALISED PLOTS FOR REFEREE
//   std::vector<std::string> ewksets; //List of sets for ewk
//   ewksets.push_back("VV");
//   ewksets.push_back("Top");
//   ewksets.push_back("ZJets_ll");
//   ewksets.push_back("ZJets_ll_vbf");
//   ewksets.push_back("ZJets_nunu");
//   ewksets.push_back("WJets_enu");
//   ewksets.push_back("WJets_munu");
//   ewksets.push_back("WJets_taunu");

//   std::vector<std::string> shapes; //List of shapes to draw
//   shapes.push_back("dijet_M(370,150.,2000.)");
//   shapes.push_back("dijet_deta(160,0.,8.)");
//   shapes.push_back("dijet_dphi(310,0.,3.1)");
//   shapes.push_back("met(80,0.,400.)");
//   shapes.push_back("cjvjetpt(100,0.,100.)");
//   shapes.push_back("met(50,0.,500.)");
//   shapes.push_back("met_x(50,0.,500.)");
//   shapes.push_back("met_y(50,0.,500.)");

//   NormPlots normplots("normplots");
//   normplots.set_qcdset("QCD")
//     .set_sigset("sig125")
//     .set_ewkset(ewksets)
//     .set_cat("")
//     .set_basesel("jet1_eta<4.7&&jet2_eta<4.7&&jet1_pt>50&&jet2_pt>50&&nvetoelectrons==0 && nvetomuons==0&&dijet_M>150&&met>130")
//     .set_shapes(shapes);

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

  //PLOTTER
//   std::vector<std::vector<std::string> > plottersets;
//   std::vector<std::string> sets1;
//   sets1.push_back("sig125");
//   std::vector<std::string> sets2;
//   sets2.push_back("WJets_enu");
//   plottersets.push_back(sets1);
//   plottersets.push_back(sets2);
 
//   std::vector<std::string> plottervariables;
//   plottervariables.push_back("BDT(40,-1.,1.)");

//   Plotter plotter("plotter");
//   plotter.set_sets(plottersets)
//     .set_shapes(plottervariables)
//     .set_basesel("passtrigger==1&&nvetomuons==0&&nvetoelectrons==0&&jet1_pt>50&&jet2_pt>50&&jet1_eta<4.7&&jet2_eta<4.7&&met_significance>3&&dijet_deta>3.6&&jetmet_mindphi>1.5")
//     .set_cat("");
  
  //HISTPLOTTER
  //std::vector<std::string> shapevec;
  std::vector<LTShapeElement> shapevec;
  for(unsigned ishape=0;ishape<shape.size();ishape++){
    std::vector<std::string> strs;
    boost::split(strs, shape[ishape], boost::is_any_of("("));
    LTShapeElement thisshape;
    thisshape.set_name(strs[0]);
    thisshape.set_histtitle(histTitle[ishape]);
    //    shapevec.push_back(strs[0]);
    if(do_logy) thisshape.set_dology(true);
    if(channel=="mumu"&&(strs[0]=="dijet_M"||strs[0]=="metnomu_significance")) thisshape.set_axisrangemultiplier(1.3);
    else if(channel=="mumu"&&(strs[0]=="metnomuons")) thisshape.set_axisrangemultiplier(1.1);
    else if(channel=="nunu"&&(strs[0]=="metnomuons")) thisshape.set_axisrangemultiplier(1.3);
    else if(channel=="nunu"&&(strs[0]=="dijet_deta")) thisshape.set_axisrangemultiplier(1.3);
    else if(channel=="nunu"&&(strs[0]=="metnomu_significance"))thisshape.set_axisrangemultiplier(1.25);
    else if(strs[0]=="metnomuons")thisshape.set_axisrangemultiplier(1.25);
    else if(strs[0]=="dijet_M")thisshape.set_axisrangemultiplier(1.3);
    else if(channel=="enu"&&strs[0]=="alljetsmetnomu_mindphi")thisshape.set_axisrangemultiplier(1.3);
    else if(strs[0]=="alljetsmetnomu_mindphi")thisshape.set_axisrangemultiplier(1.2);

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
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("W#rightarrow#mu#nu")
    .set_sample("wmu");
  if(!do_mcbkg)wmunuele.set_has_dderrors(1);

  LTPlotElement wenuele;
  wenuele.set_is_data(false)
    .set_scale(1)
    .set_color(kOrange  + 2)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("W#rightarrow e#nu")
    .set_sample("wel");
  if(!do_mcbkg)wenuele.set_has_dderrors(1);


  LTPlotElement wtaunuele;
  wtaunuele.set_is_data(false)
    .set_scale(1)
    .set_color(kOrange + 4)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("W#rightarrow#tau#nu")
    .set_sample("wtau");
  if(!do_mcbkg)wtaunuele.set_has_dderrors(1);


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


  LTPlotElement sigele;
  sigele.set_is_data(false)
    .set_scale(40.24/10000)//!
    .set_color(kRed)
    .set_in_stack(false)
    .set_legname("Signal (x1)")
    .set_sample("qqH125");

  LTPlotElement ggHele;
  ggHele.set_is_data(false)
    .set_scale(1)
    .set_color(kBlue)
    .set_in_stack(false)
    .set_legname("gg#rightarrow H (x1)")
    .set_sample("ggH125");

  if(!(channel=="nunu"&&runblind))elementvec.push_back(dataele);
  if(!dataonly){
    if(channel!="mumu"){
      if(channel!="enu")elementvec.push_back(wmunuele);
      if(channel!="munu")elementvec.push_back(wenuele);
      if(channel!="munu")elementvec.push_back(wtaunuele);
    }
    //  elementvec.push_back(zmumuele);
    if(!run2){
      if(do_separatez&&channel=="mumu"){
	elementvec.push_back(znunuewkele);
	elementvec.push_back(znunuqcdele);
      }
      else{
	if(channel!="enu"&&channel!="munu"&&channel!="taunu")elementvec.push_back(znunuele);
      }
    
      if(do_plotmcqcd||do_plotqcd)elementvec.push_back(qcdele);
      if(channel!="munu")elementvec.push_back(vvele);
      if(channel!="mumu")elementvec.push_back(topele);
    }
    if(channel!="mumu"&&channel!="enu"&&channel!="munu"){
      elementvec.push_back(sigele);
      if(!run2) elementvec.push_back(ggHele);
    }
  }

  HistPlotter plotter("plotter");
  plotter.set_dirname("ControlPlots")
    .set_add_underflows(true)
    .set_add_overflows(true)
    .set_elements(elementvec)
    //.set_histTitles(histTitle)
    .set_shapes(shapevec);
  if(!dataonly)    plotter.set_do_ratio(true);
  else plotter.set_do_ratio(false);
  if(channel=="nunu"&&runblind)plotter.set_do_ratio(false);
  if(do_closure)plotter.set_do_ratio_fitline(true);

  std::vector<std::string> dirvec;
  if(!dataonly){
    dirvec.push_back("wel");
    dirvec.push_back("wmu");
    dirvec.push_back("wtau");
    //dirvec.push_back("zvv");
    //dirvec.push_back("qcd");
    //dirvec.push_back("vv");
    //dirvec.push_back("wg");  
    //dirvec.push_back("top");
    dirvec.push_back("qqH125");
  }
  if(!(channel=="nunu"&&runblind))dirvec.push_back("data_obs");

  SummaryTable summary("summary");
  summary.set_shape(shapevec)
    .set_dirs(dirvec);
  
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
    if(do_bdt)analysis->AddModule(&addfriends);
    //analysis->AddModule(&mvatrainer);
    //analysis->AddModule(&normplots);
    if(!run2){
      if(do_datatop&&!do_singletop)analysis->AddModule(&top);
      else if(!do_singletop) analysis->AddModule(&topraw);
    }
    if(do_list) analysis->AddModule(&mceventlist);

    if(!do_mcbkg){
      analysis->AddModule(&wmunu);
      analysis->AddModule(&wenu);
      analysis->AddModule(&wtaunu);
      if(channel!="mumu"){
	analysis->AddModule(&zmumu);
      }
      else analysis->AddModule(&zmumuinzcont);
    }
    else{
      analysis->AddModule(&wmunuraw);
      analysis->AddModule(&wenuraw);
      analysis->AddModule(&wtaunuraw);  
      //!!put in Z mc bkg
    }

    if(do_singletop)analysis->AddModule(&top);
    //analysis->AddModule(&QCD);
    if(do_plotmcqcd)analysis->AddModule(&QCDraw);
    if(do_plotqcd){
      analysis->AddModule(&wmunuqcd);
      analysis->AddModule(&wenuqcd);
      analysis->AddModule(&wtaunuqcd);
      if(channel!="mumu"){
	analysis->AddModule(&zmumuqcd);
      }
      analysis->AddModule(&QCDshape);
    }
    //analysis->AddModule(&zmumuraw);
    //analysis->AddModule(&znunuraw);
    analysis->AddModule(&vv);
    //analysis->AddModule(&wgamma);
  }
  if(!(channel=="nunu"&&runblind))analysis->AddModule(&data);
  if(do_trigeff){
    // analysis->AddModule(&meteff);
    // analysis->AddModule(&metnomueff);
    // analysis->AddModule(&l1meteff);
    // analysis->AddModule(&jpteff);
    // analysis->AddModule(&mjjeff);
    // analysis->AddModule(&detaeff);
    for(unsigned ieff=0;ieff<efficiencies.size();ieff++){
      analysis->AddModule(&efficiencies[ieff]);
    }
  }
  if(datalist) analysis->AddModule(&eventlist);
  if(!dataonly){
    if(!run2){
      analysis->AddModule(&signal110);
      analysis->AddModule(&signal150);
      analysis->AddModule(&signal200);
      analysis->AddModule(&signal300);
      analysis->AddModule(&signal400);
      analysis->AddModule(&ggHsignal110);
      analysis->AddModule(&ggHsignal125);
      analysis->AddModule(&ggHsignal150);
      analysis->AddModule(&ggHsignal200);
      analysis->AddModule(&ggHsignal300);
      analysis->AddModule(&ggHsignal400);
    }
    analysis->AddModule(&signal125);

  }
  if(!do_trigeff)analysis->AddModule(&plotter);
  if(!dataonly)analysis->AddModule(&summary);
  
  analysis->RunAnalysis();

  return 0;

}
