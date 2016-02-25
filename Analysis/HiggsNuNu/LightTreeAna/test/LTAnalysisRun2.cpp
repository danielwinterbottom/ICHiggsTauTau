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
  std::string eos_path_mc;
  std::string eos_path_data;
  std::string inputparams;
  std::string filelist;
  std::string basesel;

  std::string channel;
  std::string syst;
  std::string dataset;

  bool do_mettrig;
  bool apply_trig_in_mc;
  bool runblind;
  bool runblindreg;
  bool blindcutreg;

  bool dataonly;
  bool datalist;
  bool do_list;
  std::string listset;

  bool do_latex;
  bool do_logy;
  bool do_mcbkg;
  bool use_nlo;

  std::string jetmetdphicut;
  std::string metsigcut;

  unsigned debug;

  po::options_description preconfig("Configuration"); 
  preconfig.add_options()("cfg",po::value<std::string>(&cfg)->required());
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
    //Input output and config options
    ("output_name,o",            po::value<std::string>(&outputname)->default_value("tmp.root"))
    ("input_folder,i",           po::value<std::string>(&inputfolder)->default_value("../output_lighttree_151123/"))
    ("eos_path_mc",           po::value<std::string>(&eos_path_mc)->default_value(""))
    //root://eoscms//eos/cms/store/user/amagnan/SkimFiles/151030/"))
    ("eos_path_data",           po::value<std::string>(&eos_path_data)->default_value(""))
    //root://eoscms//eos/cms/store/user/amagnan/SkimFiles/151113/"))
    ("syst,s",                   po::value<std::string>(&syst)->default_value(""))
    ("input_params,p",           po::value<std::string>(&inputparams)->default_value("../filelists/151030/Params151030.dat"))
    ("filelist,f",               po::value<std::string>(&filelist)->default_value("filelists/run2filelist.dat"))
    ("dataset,d",                po::value<std::string>(&dataset)->default_value("MET"))
    ("dataonly",                 po::value<bool>(&dataonly)->default_value(false))
    ("datalist",                 po::value<bool>(&datalist)->default_value(false))
    ("do_list",                  po::value<bool>(&do_list)->default_value(false))
    ("listset",                  po::value<std::string>(&listset)->default_value("Top"))
    ("basesel",                  po::value<std::string>(&basesel)->default_value("jet1_eta*jet2_eta<0 && jet1_eta<4.7 && jet2_eta<4.7 && dijet_M>=800&&jet1_pt>80&&dijet_deta>3.6&& jet2_pt>70&&metnomuons>200"))
    ("channel",                  po::value<std::string>(&channel)->default_value("nunu"))

    ("runblind",                 po::value<bool>(&runblind)->default_value(true))

    ("do_latex",                 po::value<bool>(&do_latex)->default_value(false))

    ("do_mettrig",               po::value<bool>(&do_mettrig)->default_value(false))
    ("apply_trig_in_mc",               po::value<bool>(&apply_trig_in_mc)->default_value(false))

    ("do_logy",                  po::value<bool>(&do_logy)->default_value(false))
    ("blindcutreg",              po::value<bool>(&blindcutreg)->default_value(true))
    ("runblindreg",              po::value<bool>(&runblindreg)->default_value(true))
    ("debug",                    po::value<unsigned>(&debug)->default_value(0))
    ("do_mcbkg",                 po::value<bool>(&do_mcbkg)->default_value(true))
    ("use_nlo",                 po::value<bool>(&use_nlo)->default_value(false))
    ("jetmetdphicut",            po::value<std::string>(&jetmetdphicut)->default_value("alljetsmetnomu_mindphi>1.0"))
    ("metsigcut",               po::value<std::string>(&metsigcut)->default_value(">4.0"))


    ;

  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
  po::notify(vm);

  /*##########################################
  #                                          #
  #          INSTANTIATE ANALYSER            #
  #                                          #
  ##########################################*/

  LTAnalyser* analysis = new LTAnalyser(outputname);


  if (debug) std::cout << "syst=" << syst << " size " << syst.size() << std::endl;

  analysis->SetEosFolders(eos_path_data,eos_path_mc);

  analysis->AddFiles(filelist);
  if(syst!="PUUP"&&syst!="PUDOWN"&&syst.size()!=0){
    std::cout<<"Syst, taking input from: "<<inputfolder<<"/"<<syst<<std::endl;
    analysis->SetInFolder(inputfolder+"/"+syst);
  }
  else{
    std::cout<<"Taking input from: "<<inputfolder<<std::endl;
    analysis->SetInFolder(inputfolder);
  }
  analysis->SetInputParams(inputparams);

  std::cout<<"Base selection: "<<basesel<<std::endl;

  //add metsig cut
  std::string metsigsel;
  if(channel=="mumu") metsigsel="&& metnomuons/sqrt(sumet-mu1_pt-mu2_pt)>"+metsigcut;
  else if (channel=="munu") metsigsel="&& metnomuons/sqrt(sumet-mu1_pt)>"+metsigcut;
  else metsigsel="&& metnomuons/sqrt(sumet)>"+metsigcut;

  basesel = basesel;//+metsigsel;

  analysis->set_baseselection(basesel);
  
  /*##########################################
  #                                          #
  #            DEFINE MODULES                #
  #                                          #
  ##########################################*/

  std::vector<std::string> histTitle;
  std::vector<std::string> shape;

  if (debug) {
    /*shape.push_back("jet1_pt(47,80.,550.)");histTitle.push_back(";p_{T}^{j1} (GeV);Events");
    shape.push_back("jet2_pt(47,80.,550.)");histTitle.push_back(";p_{T}^{j2} (GeV);Events");
    if(channel=="enu" || channel=="munu" || channel=="taunu") {
      shape.push_back("lep_mt(30,0.,150.)");histTitle.push_back(";m_{T}(lepton+MET) (GeV);Events");
    }
    else if(channel=="mumu"){
      shape.push_back("m_mumu(12,60.,120.)");histTitle.push_back(";m_{#mu#mu};Events");
      }*/
    //shape.push_back("central_tag_eta(25,-5.,5.)");histTitle.push_back(";Central tag jet #eta;Events");
    shape.push_back("forward_tag_eta(25,-5.,5.)");histTitle.push_back(";Forward tag jet #eta;Events");
    shape.push_back("alljetsmetnomu_mindphi(32,0.,3.1416)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j);Events");

    /*if (channel=="munu"){
      shape.push_back("metnomuons*pow(sqrt(sumet-mu1_pt),-1)(30,0.,15.)");histTitle.push_back(";S;Events");
    }
    else if (channel=="mumu"){
      shape.push_back("metnomuons*pow(sqrt(sumet-mu1_pt-mu2_pt),-1)(30,0.,15.)");histTitle.push_back(";S;Events");
    }
    else {
      shape.push_back("metnomuons*pow(sqrt(sumet),-1)(30,0.,15.)");histTitle.push_back(";S;Events");
      }*/
  }
  else {
    if(channel=="enu" || channel=="munu" || channel=="taunu" || channel=="topl" || channel=="topb"){
      if(channel=="taunu") {
	shape.push_back("lep_mt(10,0.,100.)");histTitle.push_back(";m_{T}(lepton+MET) (GeV);Events");
      }
      else {
	shape.push_back("lep_mt(30,0.,150.)");histTitle.push_back(";m_{T}(lepton+MET) (GeV);Events");
      }
      if (channel=="enu" || channel=="topl" || channel=="topb"){
	shape.push_back("ele1_pt(30,0.,300.)");histTitle.push_back(";p_{T}(electron) (GeV);Events");
	shape.push_back("ele1_eta(30,-2.4,2.4)");histTitle.push_back(";#eta(electron) (GeV);Events");
      }
      if (channel=="munu" || channel=="topl" || channel=="topb"){
	shape.push_back("mu1_pt(30,0.,300.)");histTitle.push_back(";p_{T}(muon) (GeV);Events");
	shape.push_back("mu1_eta(30,-2.1,2.1)");histTitle.push_back(";#eta(muon) (GeV);Events");
      }
      if (channel=="taunu") {
	shape.push_back("tau1_pt(15,0.,300.)");histTitle.push_back(";p_{T}(tau) (GeV);Events");
	shape.push_back("tau1_eta(15,-2.4,2.4)");histTitle.push_back(";#eta(tau) (GeV);Events");
      }
    }

    if (channel=="munu" || channel=="topl" || channel=="topb"){
      shape.push_back("metnomuons*pow(sqrt(sumet-mu1_pt),-1)(30,0.,15.)");histTitle.push_back(";S;Events");
    }
    else if (channel=="mumu"){
      shape.push_back("metnomuons*pow(sqrt(sumet-mu1_pt-mu2_pt),-1)(30,0.,15.)");histTitle.push_back(";S;Events");
    }
    else {
      shape.push_back("metnomuons*pow(sqrt(sumet),-1)(30,0.,15.)");histTitle.push_back(";S;Events");
    }

    if (channel=="gamma"){
      shape.push_back("gamma1_pt(50,0,500)");histTitle.push_back(";p_{T}(#gamma) (GeV);Events");
      shape.push_back("gamma1_eta(30,-2.5,2.5)");histTitle.push_back(";#eta(#gamma);Events");
    }
    
    if(channel=="mumu"){
      shape.push_back("m_mumu(12,60.,120.)");histTitle.push_back(";m_{#mu#mu};Events");
      shape.push_back("pt_mumu(15,0.,400.)");histTitle.push_back(";p_{T}^{#mu#mu};Events");
      shape.push_back("mu1_pt(30,0.,300.)");histTitle.push_back(";p_{T}(lead muon) (GeV);Events");
      shape.push_back("mu1_eta(30,-2.1,2.1)");histTitle.push_back(";#eta(lead muon) (GeV);Events");
      shape.push_back("mu2_pt(30,0.,300.)");histTitle.push_back(";p_{T}(sublead muon) (GeV);Events");
      shape.push_back("mu2_eta(50,-2.1,2.1)");histTitle.push_back(";#eta(sublead muon) (GeV);Events");
    }
    
    shape.push_back("n_vertices(40,0,40)");histTitle.push_back(";n_{vtx};Events");

    shape.push_back("jet1_pt(47,80.,550.)");histTitle.push_back(";p_{T}^{j1} (GeV);Events");
    shape.push_back("jet2_pt(28,70.,350.)");histTitle.push_back(";p_{T}^{j2} (GeV);Events");
    shape.push_back("jet1_uncorpt(47,80.,550.)");histTitle.push_back(";uncor p_{T}^{j1} (GeV);Events");
    shape.push_back("jet2_uncorpt(28,70.,350.)");histTitle.push_back(";uncor p_{T}^{j2} (GeV);Events");
    shape.push_back("jet1_eta(94,-4.7,4.7)");histTitle.push_back(";#eta^{j1} (GeV);Events");
    shape.push_back("jet2_eta(94,-4.7,4.7)");histTitle.push_back(";#eta^{j2} (GeV);Events");
    shape.push_back("cjvjetpt(19,15.,205.)");histTitle.push_back(";p_{T}^{CJV jet} (GeV);Events");
    
    if(channel=="mumu") {
      shape.push_back("metnomuons(15,200.,450.)");histTitle.push_back(";MET_{no #mu} (GeV);Events");
    }
    else {
      shape.push_back("metnomuons(35,200.,550.)");histTitle.push_back(";MET_{no #mu} (GeV);Events");
    }
    shape.push_back("met(30,0.,550.)");histTitle.push_back(";MET (GeV);Events");
    shape.push_back("l1met(30,0.,550.)");histTitle.push_back(";L1MET (GeV);Events");
    
    shape.push_back("ht(30,100,1000)");histTitle.push_back(";H_{T}^{p_{T}^{j}>15 GeV} (GeV);Events");
    shape.push_back("ht30(30,100,1000)");histTitle.push_back(";H_{T}^{p_{T}^{j}>30 GeV} (GeV);Events");
    
    if(channel=="mumu") {shape.push_back("dijet_M(15,800.,3000.)");histTitle.push_back(";M_{jj} (GeV);Events");}
    else {shape.push_back("dijet_M(30,800.,4000.)");histTitle.push_back(";M_{jj} (GeV);Events");}
    shape.push_back("dijet_dphi(15,0.,3.1416)");histTitle.push_back(";#Delta#phi_{jj};Events");
    shape.push_back("dijet_deta(30,3.6,7.)");histTitle.push_back(";#Delta#eta_{jj};Events");
    shape.push_back("dijet_sumeta(30,-5,5)");histTitle.push_back(";#eta_{j1}+#eta_{j2};Events");
    
    shape.push_back("jetmetnomu_mindphi(32,0.,3.1416)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j_{1,2});Events");
    shape.push_back("alljetsmetnomu_mindphi(32,0.,3.1416)");histTitle.push_back(";#Delta#phi(E_{T}^{miss},j);Events");
    shape.push_back("metnomu_significance(30,0.,300.)");histTitle.push_back(";S;Events");
    
    shape.push_back("jetunclet_mindphi(32,0,3.1416)");histTitle.push_back(";min #Delta#phi(j,E_{T}^{uncl});Events");
    shape.push_back("metnomuunclet_dphi(32,0,3.1416)");histTitle.push_back(";#Delta#phi(METnoMu,E_{T}^{uncl});Events");
    shape.push_back("dijetmetnomu_scalarSum_pt(35,0,1400)");histTitle.push_back(";p_{T}^{jeta}+p_{T}^{jetb}+METnoMu;Events");
    shape.push_back("dijetmetnomu_vectorialSum_pt(20,0,400)");histTitle.push_back(";p_{T}(#vec{ja}+#vec{jb}+#vec{METnoMu});Events");
    shape.push_back("dijetmetnomu_ptfraction(20,0.,1.)");histTitle.push_back(";p_{T}^{dijet}/(p_{T}^{dijet}+METnoMu);Events");
    
    shape.push_back("n_jets_cjv_30(5,0,5)");histTitle.push_back(";CJV jets (30 GeV);Events");
    shape.push_back("n_jets_cjv_20EB_30EE(5,0,5)");histTitle.push_back(";CJV jets (20 GeV EB, 30 GeV EE);Events");
    
    shape.push_back("jet1_csv(21,0.,1.)");histTitle.push_back(";Jet 1 CSV;Events");
    shape.push_back("jet2_csv(21,0.,1.)");histTitle.push_back(";Jet 2 CSV;Events");
    shape.push_back("jet3_csv(21,0.,1.)");histTitle.push_back(";Jet 3 CSV;Events");
    
    shape.push_back("n_jets_30(10,0.,10.)");histTitle.push_back(";N jets pt>30 GeV;Events");
    shape.push_back("n_jets_15(15,0.,15.)");histTitle.push_back(";N jets pt>15 GeV;Events");
    shape.push_back("central_tag_eta(25,-5.,5.)");histTitle.push_back(";Central tag jet #eta;Events");
    shape.push_back("forward_tag_eta(25,-5.,5.)");histTitle.push_back(";Forward tag jet #eta;Events");

    shape.push_back("alljetsmetnomu_mindphi:dijet_M(24,800.,2000.,20,1.,3.)");histTitle.push_back(";M_{jj} (GeV);#Delta#phi(E_{T}^{miss},j);Events");
 

    //mindR(tau,tagjets)
    //shape.push_back("mymath::deltaRmin(jet1_eta,jet1_phi,jet2_eta,jet2_phi,tau1_eta,tau1_phi)(20,0.,4.)");histTitle.push_back(";min#DeltaR(#tau,tag jets);Events");
  }

  std::string dataextrasel;
  std::string mcextrasel;

  if (channel!="gamma"){
    if(!do_mettrig) dataextrasel="&&(pass_sigtrigger==1)";
    else dataextrasel="&&(pass_mettrigger==1)";
  }
  else {
    dataextrasel="&&(pass_photontrigger==1)";
  }
  if (apply_trig_in_mc) mcextrasel=dataextrasel;

  std::string sigcat;
  std::string zextrasigcat;

  std::string nunucat="nvetomuons==0&&nvetoelectrons==0&&"+jetmetdphicut;

  std::string mumucat="nselmuons==2&&nvetomuons==2&&nvetoelectrons==0&&m_mumu>60&&m_mumu<120&&"+jetmetdphicut;
  std::string munucat="nselmuons==1&&nvetomuons==1&&nvetoelectrons==0&&lep_mt>=0&&"+jetmetdphicut;
  std::string enucat="nselelectrons==1&&nvetomuons==0&&nvetoelectrons==1&&"+jetmetdphicut;
  std::string taunucat="ntaus==1&&nvetomuons==0&&nvetoelectrons==0&&jetmetnomu_mindphi>1.0";
  std::string gammacat="ntightphotons==1&&nvetomuons==0&&nvetoelectrons==0&&"+jetmetdphicut;
  std::string toplcat="nvetomuons==1&&nvetoelectrons==1&&nselmuons==1&&nselelectrons==1";
  std::string topbcat = "(nselmuons>=1 || nselelectrons>=1)&&(jet1_csv>0.679||jet2_csv>0.679)&&(forward_tag_eta>2.8||forward_tag_eta<-2.8)";

  if(channel=="nunu" || channel=="qcd"){//nunu
    sigcat=nunucat;
  }
  else {
    if(channel=="mumu"){//zmumu
      sigcat=mumucat;
    }
    else if(channel=="munu"){//wmu
      sigcat=munucat;
    }
    else if(channel=="enu"){//wel
      sigcat=enucat;
    }
    else if(channel=="taunu"){//wtau
      sigcat=taunucat;
    }
    else if(channel=="gamma"){
      sigcat=gammacat;
    }
    else if(channel=="topl"){
      sigcat=toplcat;
    }
    else if(channel=="topb"){
      sigcat=topbcat;
    }
     else{
      std::cout<<"Error: Channel "<<channel<<" not recognised, exiting"<<std::endl;
      return 1;
    }
  }

  std::string sigmcweight;
  std::string mcweightpufactor="";
  if(syst=="PUUP") mcweightpufactor="*puweight_up_scale";
  if(syst=="PUDOWN") mcweightpufactor="*puweight_down_scale";
  
  if (syst=="TRIG0UP") mcweightpufactor="*weight_trig_1/weight_trig_0";
  if (syst=="TRIG0DOWN") mcweightpufactor="*weight_trig_2/weight_trig_0";
  if (syst=="TRIG1UP") mcweightpufactor="*weight_trig_3/weight_trig_0";
  if (syst=="TRIG1DOWN") mcweightpufactor="*weight_trig_4/weight_trig_0";
  if (syst=="TRIG2UP") mcweightpufactor="*weight_trig_5/weight_trig_0";
  if (syst=="TRIG2DOWN") mcweightpufactor="*weight_trig_6/weight_trig_0";

  if(channel=="taunu"||channel=="gamma"||channel=="nunu"||channel=="qcd") sigmcweight="total_weight_lepveto"+mcweightpufactor;
  else sigmcweight="total_weight_leptight"+mcweightpufactor;

  std::string bothcentral="TMath::Abs(jet1_eta)<3&&TMath::Abs(jet2_eta)<3";
  std::string bothforward="TMath::Abs(jet1_eta)>=3&&TMath::Abs(jet2_eta)>=3";
  std::string j2forwardj1central="TMath::Abs(jet1_eta)<3&&TMath::Abs(jet2_eta)>=3";
  std::string j1forwardj2central="TMath::Abs(jet1_eta)>=3&&TMath::Abs(jet2_eta)<3";

  std::string additionalcut=(syst=="PUUP")?("&&abs(puweight_up_scale)<200"): (syst=="PUDOWN")?("&&abs(puweight_down_scale)<10") : ("");
  if (syst.find("TRIG")!=syst.npos) additionalcut="&&weight_trig_0>0";
  analysis->set_baseselection(basesel+additionalcut);

  //DATA SHAPE GENERATION
  DataShape data("data");
  data.set_dataset(dataset)
    .set_dirname("data_obs")
    .set_shape(shape)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+dataextrasel);

  DataShape signal110("signal110");
  signal110.set_dataset("VBFH110")
    .set_dirname("qqH110")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);  

  DataShape signal125("signal125");
  signal125.set_dataset("VBFH125")
    .set_dirname("qqH125")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);  

  DataShape signal150("signal150");
  signal150.set_dataset("VBFH150")
    .set_dirname("qqH150")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);  

  DataShape signal200("signal200");
  signal200.set_dataset("VBFH200")
    .set_dirname("qqH200")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);  

  DataShape signal300("signal300");
  signal300.set_dataset("VBFH300")
    .set_dirname("qqH300")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);  

  DataShape signal400("signal400");
  signal400.set_dataset("VBFH400")
    .set_dirname("qqH400")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);  

  DataShape signal500("signal500");
  signal500.set_dataset("VBFH500")
    .set_dirname("qqH500")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);

  DataShape signal600("signal600");
  signal500.set_dataset("VBFH600")
    .set_dirname("qqH600")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);

  DataShape ggH110("ggH110");
  ggH110.set_dataset("GluGluH110")
    .set_dirname("ggH110")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);  

  DataShape ggH125("ggH125");
  ggH125.set_dataset("GluGluH125")
    .set_dirname("ggH125")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);  

  DataShape ggH150("ggH150");
  ggH150.set_dataset("GluGluH150")
    .set_dirname("ggH150")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);  

  DataShape ggH200("ggH200");
  ggH200.set_dataset("GluGluH200")
    .set_dirname("ggH200")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);  

  DataShape ggH300("ggH300");
  ggH300.set_dataset("GluGluH300")
    .set_dirname("ggH300")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);  

  DataShape ggH400("ggH400");
  ggH400.set_dataset("GluGluH400")
    .set_dirname("ggH400")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);  

  DataShape ggH500("ggH500");
  ggH500.set_dataset("GluGluH500")
    .set_dirname("ggH500")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);

  DataShape ggH600("ggH600");
  ggH600.set_dataset("GluGluH600")
    .set_dirname("ggH600")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);

  DataShape vv("vv");
  vv.set_dataset("VV")
    .set_dirname("vv")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);

  DataShape topraw("topraw");
  topraw.set_dataset("Top")
    .set_dirname("top")
    .set_shape(shape)    
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);

  DataShape znunuraw("znunuraw");
  znunuraw.set_dataset("ZJets_nunu")
    .set_dirname("zvv")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);


  DataShape qcdznunuraw("qcdznunuraw");
  qcdznunuraw.set_dataset("ZJets_nunu")
    .set_dirname("zvvqcd")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);


  DataShape ewkznunuraw("ewkznunuraw");
  ewkznunuraw.set_dataset("EWK_ZJets_nunu")
    .set_dirname("zvvewk")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);

  DataShape zmumuraw("zmumuraw");
  zmumuraw.set_dataset("ZJets_ll")
    .set_dirname("zmumu")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);
  DataShape qcdzmumuraw("qcdzmumuraw");
  qcdzmumuraw.set_dataset("ZJets_ll")
    .set_dirname("zmumuqcd")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);
  DataShape ewkzmumuraw("ewkzmumuraw");
  ewkzmumuraw.set_dataset("EWK_ZJets_ll")
    .set_dirname("zmumuewk")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);

  DataShape wmunuraw("wmunuraw");
  wmunuraw.set_dataset("WJets_munu")
    .set_dirname("wmu")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);

  if (use_nlo)  wmunuraw.set_dataset("WJets_nlo_munu");

  DataShape qcdwmunuraw("qcdwmunuraw");
  qcdwmunuraw.set_dataset("WJets_munu")
    .set_dirname("wmuqcd")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);
  DataShape ewkwmunuraw("ewkwmunuraw");
  ewkwmunuraw.set_dataset("EWK_WJets_munu")
    .set_dirname("wmuewk")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);


  DataShape wenuraw("wenuraw");
  wenuraw.set_dataset("WJets_enu")
    .set_dirname("wel")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);

  if (use_nlo)  wenuraw.set_dataset("WJets_nlo_enu");
  DataShape qcdwenuraw("qcdwenuraw");
  qcdwenuraw.set_dataset("WJets_enu")
    .set_dirname("welqcd")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);
  DataShape ewkwenuraw("ewkwenuraw");
  ewkwenuraw.set_dataset("EWK_WJets_enu")
    .set_dirname("welewk")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);


  DataShape wtaunuraw("wtaunuraw");
  wtaunuraw.set_dataset("WJets_taunu")
    .set_dirname("wtau")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);

  if (use_nlo) wtaunuraw.set_dataset("WJets_nlo_taunu");
  DataShape qcdwtaunuraw("qcdwtaunuraw");
  qcdwtaunuraw.set_dataset("WJets_taunu")
    .set_dirname("wtauqcd")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);
  DataShape ewkwtaunuraw("ewkwtaunuraw");
  ewkwtaunuraw.set_dataset("EWK_WJets_taunu")
    .set_dirname("wtauewk")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);


  DataShape qcdraw("qcdraw");
  qcdraw.set_dataset("QCD")
    .set_dirname("qcd")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);

  DataShape gjetsraw("gjetsraw");
  gjetsraw.set_dataset("GJets")
    .set_dirname("gjets")
    .set_shape(shape)
    .set_dataweight(sigmcweight)
    .set_basesel(analysis->baseselection())
    .set_cat(sigcat+mcextrasel);

  //HISTPLOTTER
  //std::vector<std::string> shapevec;
  std::vector<LTShapeElement> shapevec;
  for(unsigned ishape=0;ishape<shape.size();ishape++){
    std::vector<std::string> strs;
    boost::split(strs, shape[ishape], boost::is_any_of("("));
    LTShapeElement thisshape;
    thisshape.set_name(strs[0]);

    //do not plot 2D hists....
    if (strs[0].find(":")!=strs[0].npos) continue;

    if (strs[0]=="forward_tag_eta"){
      thisshape.set_legleft(0.39);
      thisshape.set_legright(0.61);
    }

    thisshape.set_histtitle(histTitle[ishape]);
    //    shapevec.push_back(strs[0]);
    if(do_logy) thisshape.set_dology(true);
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
    blindvars.push_back("metnomuons");
    blindvars.push_back("dijet_deta");
    blindvars.push_back("metnomuons*pow");
    std::pair<double,double>mindphirange(1.,10.);
    std::pair<double,double>metsigrange(150.,1000.);
    std::pair<double,double>metnomurange(350.,1000.);
    std::pair<double,double>detarange(5.,10.);
    blindrange.push_back(mindphirange);
    blindrange.push_back(metsigrange);
    blindrange.push_back(metnomurange);
    blindrange.push_back(detarange);
    blindrange.push_back(std::pair<double,double>(3,100));
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
  LTPlotElement qcdwmunuele;
  qcdwmunuele.set_is_data(false)
    .set_scale(1)
    .set_color(kOrange-4)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("QCD W#rightarrow#mu#nu")
    .set_sample("wmuqcd");
  if(!do_mcbkg)qcdwmunuele.set_has_dderrors(1);
  LTPlotElement ewkwmunuele;
  ewkwmunuele.set_is_data(false)
    .set_scale(1)
    .set_color(kOrange-3)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("EWK W#rightarrow#mu#nu")
    .set_sample("wmuewk");
  if(!do_mcbkg)ewkwmunuele.set_has_dderrors(1);

  LTPlotElement wenuele;
  wenuele.set_is_data(false)
    .set_scale(1)
    .set_color(kOrange  + 2)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("W#rightarrow e#nu")
    .set_sample("wel");
  if(!do_mcbkg)wenuele.set_has_dderrors(1);
  LTPlotElement qcdwenuele;
  qcdwenuele.set_is_data(false)
    .set_scale(1)
    .set_color(kOrange  + 2)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("QCD W#rightarrow e#nu")
    .set_sample("welqcd");
  if(!do_mcbkg)qcdwenuele.set_has_dderrors(1);
  LTPlotElement ewkwenuele;
  ewkwenuele.set_is_data(false)
    .set_scale(1)
    .set_color(kOrange  + 1)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("EWK W#rightarrow e#nu")
    .set_sample("welewk");
  if(!do_mcbkg)ewkwenuele.set_has_dderrors(1);


  LTPlotElement wtaunuele;
  wtaunuele.set_is_data(false)
    .set_scale(1)
    .set_color(kOrange + 4)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("W#rightarrow#tau#nu")
    .set_sample("wtau");
  if(!do_mcbkg)wtaunuele.set_has_dderrors(1);
  LTPlotElement qcdwtaunuele;
  qcdwtaunuele.set_is_data(false)
    .set_scale(1)
    .set_color(kOrange + 4)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("QCD W#rightarrow#tau#nu")
    .set_sample("wtauqcd");
  if(!do_mcbkg)qcdwtaunuele.set_has_dderrors(1);
  LTPlotElement ewkwtaunuele;
  ewkwtaunuele.set_is_data(false)
    .set_scale(1)
    .set_color(kOrange + 5)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("EWK W#rightarrow#tau#nu")
    .set_sample("wtauewk");
  if(!do_mcbkg)ewkwtaunuele.set_has_dderrors(1);


  LTPlotElement zmumuele;
  zmumuele.set_is_data(false)
    .set_scale(1)
    .set_color(kAzure  + 2)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("Z#rightarrow#mu#mu")
    .set_sample("zmumu");
  if(!do_mcbkg)zmumuele.set_has_dderrors(1);
  LTPlotElement qcdzmumuele;
  qcdzmumuele.set_is_data(false)
    .set_scale(1)
    .set_color(kAzure  + 2)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("QCD Z#rightarrow#mu#mu")
    .set_sample("zmumuqcd");
  if(!do_mcbkg)qcdzmumuele.set_has_dderrors(1);
  LTPlotElement ewkzmumuele;
  ewkzmumuele.set_is_data(false)
    .set_scale(1)
    .set_color(kAzure  + 4)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("EWK Z#rightarrow#mu#mu")
    .set_sample("zmumuewk");
  if(!do_mcbkg)ewkzmumuele.set_has_dderrors(1);

  LTPlotElement znunuele;
  znunuele.set_is_data(false)
    .set_scale(1)
    .set_color(kAzure  + 2)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("Z#rightarrow#nu#nu")
    .set_sample("zvv");
  if(!do_mcbkg)znunuele.set_has_dderrors(1);
  LTPlotElement qcdznunuele;
  qcdznunuele.set_is_data(false)
    .set_scale(1)
    .set_color(kAzure  + 2)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("QCD Z#rightarrow#nu#nu")
    .set_sample("zvvqcd");
  if(!do_mcbkg)qcdznunuele.set_has_dderrors(1);
  LTPlotElement ewkznunuele;
  ewkznunuele.set_is_data(false)
    .set_scale(1)
    .set_color(kAzure  + 4)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("EWK Z#rightarrow#nu#nu")
    .set_sample("zvvewk");
  if(!do_mcbkg)ewkznunuele.set_has_dderrors(1);

  LTPlotElement vvele;
  vvele.set_is_data(false)
    .set_scale(1)
    .set_color(kGreen-5)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("VV")
    .set_sample("vv");

  LTPlotElement topele;
  topele.set_is_data(false)
    .set_scale(1)
    .set_color(kBlue-8)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("Top")
    .set_sample("top");

  LTPlotElement qcdele;
  qcdele.set_is_data(false)
    .set_scale(1)
    .set_color(kMagenta-10)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("QCD")
    .set_sample("qcd");

  LTPlotElement gjetsele;
  gjetsele.set_is_data(false)
    .set_scale(1)
    .set_color(kYellow+2)
    .set_in_stack(true)
    .set_is_inratioden(true)
    .set_legname("#gamma+jets")
    .set_sample("gjets");


  LTPlotElement sigele;
  sigele.set_is_data(false)
    .set_scale(1)
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
    if (channel=="gamma"){
      elementvec.push_back(gjetsele);
    }
    if(channel!="mumu"){
      elementvec.push_back(qcdwmunuele);
      elementvec.push_back(qcdwenuele);
      elementvec.push_back(qcdwtaunuele);
      elementvec.push_back(ewkwmunuele);
      elementvec.push_back(ewkwenuele);
      elementvec.push_back(ewkwtaunuele);
    } //else {
    if(channel!="nunu") {
      elementvec.push_back(qcdzmumuele);
      elementvec.push_back(ewkzmumuele);
    }
    //}
    elementvec.push_back(topele);
    elementvec.push_back(qcdele);
    elementvec.push_back(vvele);
    if(channel!="mumu"&&channel!="enu"&&channel!="munu"&&channel!="taunu"){
      elementvec.push_back(sigele);
      elementvec.push_back(qcdznunuele);
      elementvec.push_back(ewkznunuele);
      elementvec.push_back(ggHele);
    }
  }

  if (debug){
    std::cout << " number of elements: " << elementvec.size() << std::endl;
    for(unsigned iElement=0;iElement<elementvec.size();iElement++){
      std::cout<<" --- Ele " << iElement << " sample " << elementvec[iElement].sample() << std::endl;
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

  if (debug) plotter.set_do_debug(true);

  std::vector<std::string> dirvec;
  if(!dataonly){
    dirvec.push_back("welqcd");
    dirvec.push_back("wmuqcd");
    dirvec.push_back("wtauqcd");
    dirvec.push_back("welewk");
    dirvec.push_back("wmuewk");
    dirvec.push_back("wtauewk");
    dirvec.push_back("zmumuqcd");
    dirvec.push_back("zmumuewk");
    dirvec.push_back("zvvqcd");
    dirvec.push_back("zvvewk");
    dirvec.push_back("gjets");
    dirvec.push_back("qcd");
    dirvec.push_back("vv");
    //dirvec.push_back("wg");  
    dirvec.push_back("top");
    dirvec.push_back("qqH125");
    dirvec.push_back("ggH125");
  }
  if(!(channel=="nunu"&&runblind))dirvec.push_back("data_obs");

  SummaryTable summary("summary");
  summary.set_shape(shapevec)
    .set_dirs(dirvec);
  
  /*##########################################
  #                                          #
  #   SET UP ANALYSIS SEQUENCE AND RUN       #
  #                                          #
  ##########################################*/

  if(!dataonly){

    if(do_mcbkg){
      analysis->AddModule(&qcdraw);
      analysis->AddModule(&qcdwmunuraw);
      analysis->AddModule(&qcdwenuraw);
      analysis->AddModule(&qcdwtaunuraw);
      analysis->AddModule(&ewkwmunuraw);
      analysis->AddModule(&ewkwenuraw);
      analysis->AddModule(&ewkwtaunuraw);
      analysis->AddModule(&topraw);
      analysis->AddModule(&gjetsraw);
      analysis->AddModule(&qcdzmumuraw);
      analysis->AddModule(&ewkzmumuraw);
      analysis->AddModule(&qcdznunuraw);
      analysis->AddModule(&ewkznunuraw);
    }
    analysis->AddModule(&vv);
  }
  if(!(channel=="nunu"&&runblind))analysis->AddModule(&data);

  if(!dataonly){
    analysis->AddModule(&ggH110);
    analysis->AddModule(&ggH150);
    analysis->AddModule(&ggH200);
    analysis->AddModule(&ggH300);
    analysis->AddModule(&ggH400);
    analysis->AddModule(&ggH500);
    analysis->AddModule(&ggH600);
    analysis->AddModule(&signal110);
    analysis->AddModule(&signal150);
    analysis->AddModule(&signal200);
    analysis->AddModule(&signal300);
    analysis->AddModule(&signal400);
    analysis->AddModule(&signal500);
    analysis->AddModule(&signal600);
    analysis->AddModule(&signal125);
    analysis->AddModule(&ggH125);
  }
  analysis->AddModule(&plotter);
  if(!dataonly) analysis->AddModule(&summary);
  
  analysis->RunAnalysis();
  
  return 0;
  
}
