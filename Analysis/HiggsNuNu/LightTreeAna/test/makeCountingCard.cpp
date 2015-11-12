#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <algorithm>
#include <iomanip>
#include <map>
#include <cmath>
#include <cstdio>
#include "TVectorD.h"
#include "boost/program_options.hpp"
namespace po=boost::program_options;
using namespace ic;
class Syst{
  CLASS_MEMBER(Syst,std::string,name)
  CLASS_MEMBER(Syst,std::string,latexname)
  CLASS_MEMBER(Syst,std::string,type)
  CLASS_MEMBER(Syst,TFile*,uptfile)
  CLASS_MEMBER(Syst,TFile*,downtfile)
  CLASS_MEMBER(Syst,std::vector<std::string>,procsaffected)
  CLASS_MEMBER(Syst,double,constvalue)
  CLASS_MEMBER(Syst,bool,is_datastat)
  Syst();
};
Syst::Syst(){
  is_datastat_=false;
};

int main(int argc, char* argv[]){
  bool blind=true;
  bool do_datatop;
  bool do_qcdfromshape;
  bool do_qcdfromnumber;
  bool do_ggh;
  bool do_ues;
  bool verbose;
  bool do_individualsystsummary;
  bool do_latex;
  double qcdrate;
  double zvvstat;
  std::string mass;
  std::string indir;
  std::string outname;


  po::variables_map vm;
  po::options_description config("Configuration");
  config.add_options()
    //Input output and config options                                                                                                                   
    ("input_folder,i",           po::value<std::string>(&indir)->default_value("output_contplots_alljets15metsig4cjvmjj1100"))
    ("outname,o",                po::value<std::string>(&outname)->default_value("vbfhinv.txt"))
    ("blind",                    po::value<bool>(&blind)->default_value(true))
    ("do_qcdfromshape,s",        po::value<bool>(&do_qcdfromshape)->default_value(false))
    ("do_qcdfromnumber,q",       po::value<bool>(&do_qcdfromnumber)->default_value(true))
    ("do_ggh,g",                 po::value<bool>(&do_ggh)->default_value(true))
    ("do_ues,u",                 po::value<bool>(&do_ues)->default_value(true))
    ("verbose,v",                po::value<bool>(&verbose)->default_value(false))
    ("do_individualsystsummary", po::value<bool>(&do_individualsystsummary)->default_value(false))
    ("do_latex,l",               po::value<bool>(&do_latex)->default_value(false))
    ("qcdrate",                  po::value<double>(&qcdrate)->default_value(17))
    ("zvvstat",                  po::value<double>(&zvvstat)->default_value(18))
    ("mass,m",                   po::value<std::string>(&mass)->default_value("125"))
    ("do_datatop,t",             po::value<bool>(&do_datatop)->default_value(false));

  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::notify(vm);

  std::map<std::string,double > procsysttotal;
  std::map<std::string,double > procstattotal;
  std::map<std::string,double > systbkgtotal;
  std::map<std::string,double > systsigtotal;
  double totalbkgstat=0;
  double totalbkgsyst=0;
  double totalsigstat=0;
  double totalsigsyst=0;

  //Rate to use if qcd not taken from a shape
  double qcdabserr=14;

  if((mass!="110")&&(mass!="125")&&(mass!="150")&&(mass!="200")&&(mass!="300")&&(mass!="400")){
    std::cout<<"Mass "<<mass<<" not currently supported, please use 110, 125, 200, 300 or 400. Exiting!"<<std::endl;
    return 1;
  }
  
  std::vector<std::string> sigprocesses;
  sigprocesses.push_back("qqH"+mass);
  if(do_ggh)sigprocesses.push_back("ggH"+mass);
  std::vector<std::string> sigprocessesnames;
  sigprocessesnames.push_back("qqH");
  if(do_ggh)sigprocessesnames.push_back("ggH");
  std::vector<std::string> sigprocesslatex;
  sigprocesslatex.push_back("Signal(VBF)");
  if(do_ggh)sigprocesslatex.push_back("Signal(ggH)");
  std::vector<std::string> bkgprocesses;
  bkgprocesses.push_back("zvv");
  bkgprocesses.push_back("wmu");
  bkgprocesses.push_back("wel");
  bkgprocesses.push_back("wtau");
  bkgprocesses.push_back("top");
  if(do_qcdfromshape)bkgprocesses.push_back("qcd");
  bkgprocesses.push_back("vv");

  std::vector<std::string> bkgprocesslatex;
  bkgprocesslatex.push_back("$Z\\rightarrow\\nu\\nu$");
  bkgprocesslatex.push_back("$W\\rightarrow\\mu\\nu$");
  bkgprocesslatex.push_back("$W\\rightarrow e\\nu$");
  bkgprocesslatex.push_back("$W\\rightarrow\\tau\\nu$");
  bkgprocesslatex.push_back("top");
  if(do_qcdfromshape)bkgprocesslatex.push_back("QCD multijet");
  bkgprocesslatex.push_back("VV");
  
  //CENTRAL ROOT FILE
  std::cout<<"IMPORTANT NOTE: GMN UNCERTAINTY MUST BE PUT IN MANUALLY!!"<<std::endl;
  std::cout<<"Processing indir: "<<indir<<std::endl;
  TFile* nunu=new TFile((indir+"/nunu.root").c_str());

  //SYST VARIED ROOT FILES
  TFile* jesup=new TFile((indir+"/JESUP/nunu.root").c_str());
  TFile* jesdown=new TFile((indir+"/JESDOWN/nunu.root").c_str());
  TFile* jerbetter=new TFile((indir+"/JERBETTER/nunu.root").c_str());
  TFile* jerworse=new TFile((indir+"/JERWORSE/nunu.root").c_str());
  TFile* uesup=new TFile((indir+"/UESUP/nunu.root").c_str());
  TFile* uesdown=new TFile((indir+"/UESDOWN/nunu.root").c_str());
  TFile* eleup=new TFile((indir+"/ELEEFFUP/nunu.root").c_str());
  TFile* eledown=new TFile((indir+"/ELEEFFDOWN/nunu.root").c_str());
  TFile* muup=new TFile((indir+"/MUEFFUP/nunu.root").c_str());
  TFile* mudown=new TFile((indir+"/MUEFFDOWN/nunu.root").c_str());
  TFile* puup=new TFile((indir+"/PUUP/nunu.root").c_str());
  TFile* pudown=new TFile((indir+"/PUDOWN/nunu.root").c_str());

  //SYSTEMATICS
  std::vector<Syst> systematics;

  std::vector<std::string> lumi8tevprocsaffected={"ggH,qqH,ggH110","ggH125","ggH150","ggH200","ggH300","ggH400","qqH110","qqH125","qqH150","qqH200","qqH300","qqH400","wg","vv","qcd"};
  Syst lumi8tev;
  lumi8tev.set_name("lumi_8TeV")
    .set_latexname("Luminosity")
    .set_type("constlnN")
    .set_procsaffected(lumi8tevprocsaffected)
    .set_constvalue(1.026);

  std::vector<std::string> allprocs={"ggH,qqH,ggH110","ggH125","ggH150","ggH200","ggH300","ggH400","qqH110","qqH125","qqH1C50","qqH200","qqH300","qqH400","zvv","wmu","wel","wtau","top","qcd","wg","vv"};
  std::vector<std::string> allprocsnotqcd={"ggH,qqH,ggH110","ggH125","ggH150","ggH200","ggH300","ggH400","qqH110","qqH125","qqH150","qqH200","qqH300","qqH400","zvv","wmu","wel","wtau","top","wg","vv"};
  std::vector<std::string> ggHprocs={"ggH110","ggH125","ggH150","ggH200","ggH300","ggH400","ggH"};
  std::vector<std::string> qqHprocs={"qqH110","qqH125","qqH150","qqH200","qqH300","qqH400","qqH"};
  Syst eleeff;
  eleeff.set_name("CMS_eff_e")
    .set_latexname("Electron efficiency")
    .set_type("fromfilelnN")
    .set_procsaffected(allprocsnotqcd)
    .set_uptfile(eleup)
    .set_downtfile(eledown);

  Syst mueff;
  mueff.set_name("CMS_eff_m")
    .set_latexname("Muon efficiency")
    .set_type("fromfilelnN")
    .set_procsaffected(allprocsnotqcd)
    .set_uptfile(muup)
    .set_downtfile(mudown);

  Syst jes;
  jes.set_name("CMS_scale_j")
    .set_latexname("Jet energy scale")
    .set_type("fromfilelnN")
    .set_procsaffected(allprocsnotqcd)
    .set_uptfile(jesup)
    .set_downtfile(jesdown);

  Syst jer;
  jer.set_name("CMS_res_j")
    .set_latexname("Jet energy resolution")
    .set_type("fromfilelnN")
    .set_procsaffected(allprocsnotqcd)
    .set_uptfile(jerbetter)
    .set_downtfile(jerworse);

  Syst ues;
  ues.set_name("CMS_scale_met")
    .set_latexname("Unclustered energy scale")
    .set_type("fromfilelnN")
    .set_procsaffected(allprocsnotqcd)
    .set_uptfile(uesup)
    .set_downtfile(uesdown);

  Syst pu;
  pu.set_name("CMS_VBFHinv_puweight")
    .set_latexname("Pileup weight")
    .set_type("fromfilelnN")
    .set_procsaffected(allprocsnotqcd)
    .set_uptfile(puup)
    .set_downtfile(pudown);

  Syst zvvmcstat;
  zvvmcstat.set_name("CMS_VBFHinv_zvv_norm")
    .set_latexname("$Z\\rightarrow\\nu\\nu$ MC stat.")
    .set_type("datadrivenMCstatlnN")
    .set_procsaffected({"zvv"});

  Syst zvvdatastat;
  zvvdatastat.set_name("CMS_VBFHinv_zvv_stat")
    .set_latexname("$Z\\rightarrow\\nu\\nu$ data stat.")
    .set_is_datastat(true)
    .set_type("datadrivendatastatlnN")
    .set_procsaffected({"zvv"});

  Syst zvvdatastatgmn;
  zvvdatastatgmn.set_name("CMS_VBFHinv_zvv_stat")
    .set_latexname("$Z\\rightarrow\\nu\\nu$ data stat.")
    .set_is_datastat(true)
    .set_type("datadrivendatastatgmN")
    .set_constvalue(zvvstat)
    .set_procsaffected({"zvv"});

  Syst welmcstat;
  welmcstat.set_name("CMS_VBFHinv_wel_norm")
    .set_latexname("$W\\rightarrow e\\nu$ MC stat.")
    .set_type("datadrivenMCstatlnN")
    .set_procsaffected({"wel"});

  Syst weldatastat;
  weldatastat.set_name("CMS_VBFHinv_wel_stat")
    .set_latexname("$W\\rightarrow e\\nu$ data stat.")
    .set_is_datastat(true)
    .set_type("datadrivendatastatlnN")
    .set_procsaffected({"wel"});

  Syst wmumcstat;
  wmumcstat.set_name("CMS_VBFHinv_wmu_norm")
    .set_latexname("$W\\rightarrow \\mu\\nu$ MC stat.")
    .set_type("datadrivenMCstatlnN")
    .set_procsaffected({"wmu"});

  Syst wmudatastat;
  wmudatastat.set_name("CMS_VBFHinv_wmu_stat")
    .set_latexname("$W\\rightarrow \\mu\\nu$ data stat.")
    .set_is_datastat(true)
    .set_type("datadrivendatastatlnN")
    .set_procsaffected({"wmu"});

  Syst wtauideff;
  wtauideff.set_name("CMS_VBFHinv_tau_eff")
    .set_latexname("Tau efficiency")
    .set_procsaffected({"wtau"})
    .set_type("constlnN")
    .set_constvalue(1.08);

  Syst wtaujetmetextrap;
  wtaujetmetextrap.set_name("CMS_VBFHinv_tau_extrapfacunc")
    .set_latexname("$W\\rightarrow\\tau\\nu$ control region extrapolation")
    .set_procsaffected({"wtau"})
    .set_type("constlnN")
    .set_constvalue(1.2);

  Syst wtaumcstat;
  wtaumcstat.set_name("CMS_VBFHinv_wtau_norm")
    .set_latexname("$W\\rightarrow\\tau\\nu$ MC stat.")
    .set_type("datadrivenMCstatlnN")
    .set_procsaffected({"wtau"});

  Syst wtaudatastat;
  wtaudatastat.set_name("CMS_VBFHinv_wtau_stat")
    .set_latexname("$W\\rightarrow\\tau\\nu$ data stat.")
    .set_is_datastat(true)
    .set_type("datadrivendatastatlnN")
    .set_procsaffected({"wtau"});

  Syst topmcstat;
  topmcstat.set_name("CMS_VBFHinv_top_norm")
    .set_latexname("Top MC stat.")
    .set_type("datadrivenMCstatlnN")
    .set_procsaffected({"top"});

  Syst topmcsfunc;
  topmcsfunc.set_name("CMS_VBFHinv_top_norm")
    .set_latexname("Top MC scale factor unc.")
    .set_type("constlnN")
    .set_constvalue(1.2)
    .set_procsaffected({"top"});

  Syst topdatastat;
  topdatastat.set_name("CMS_VBFHinv_top_stat")
    .set_latexname("Top data stat.")
    .set_is_datastat(true)
    .set_type("datadrivendatastatlnN")
    .set_procsaffected({"top"});

  Syst mctopmcstat;
  mctopmcstat.set_name("CMS_VBFHinv_top_norm")
    .set_latexname("TOP FROM MC SHOULD NOT APPEAR IN FINAL RESULT")
    .set_type("fromMCstatlnN")
    .set_procsaffected({"top"});  

  Syst qqHmcstat;
  qqHmcstat.set_name("CMS_VBFHinv_qqH_norm")
    .set_latexname("VBF MC stat.")
    .set_type("fromMCstatlnN")
    .set_procsaffected(qqHprocs);

  Syst ggHmcstat;
  ggHmcstat.set_name("CMS_VBFHinv_ggH_norm")
    .set_latexname("ggH MC stat.")
    .set_type("fromMCstatlnN")
    .set_procsaffected(ggHprocs);

  Syst ggHqcdscale;
  ggHqcdscale.set_name("QCDscale_ggH2in")
    .set_latexname("ggH QCD scale")
    .set_type("constlnN")
    .set_procsaffected(ggHprocs)
    .set_constvalue(1.553);

  //UPDATE TO BE MASS DEPENDENT
  Syst ggHpdf;
  ggHpdf.set_name("pdf_gg")
    .set_latexname("ggH pdf")
    .set_type("constlnN")
    .set_procsaffected(ggHprocs)
    .set_constvalue(1.113);

  Syst ggHUEPS;
  ggHUEPS.set_name("UEPS")
    .set_latexname("UEPS")
    .set_type("constlnN")
    .set_procsaffected(ggHprocs)
    .set_constvalue(1.168);

  Syst qcdmcstat;
  qcdmcstat.set_name("CMS_VBFHinv_qcd_norm")
    .set_latexname("QCD FROM MC SHOULD NOT APPEAR IN FINAL RESULT")
    .set_type("fromMCstatlnN")
    .set_procsaffected({"qcd"});  

  Syst qcdfromnumerr;
  qcdfromnumerr.set_name("CMS_VBFHinv_qcd_norm")
    .set_latexname("QCD normalisation")
    .set_type("qcdfromnumberlnN")
    .set_procsaffected({"qcd"});  

  Syst qqHqcdscale;
  qqHqcdscale.set_name("QCDscale_qqH")
    .set_latexname("VBF QCD scale")
    .set_type("constlnN")
    .set_procsaffected(qqHprocs);
  if(mass=="110")qqHqcdscale.set_constvalue(1.002);
  if(mass=="125")qqHqcdscale.set_constvalue(1.002);
  if(mass=="150")qqHqcdscale.set_constvalue(1.0025);
  if(mass=="200")qqHqcdscale.set_constvalue(1.002);
  if(mass=="300")qqHqcdscale.set_constvalue(1.002);
  if(mass=="400")qqHqcdscale.set_constvalue(1.0035);

  Syst qqHpdf;
  qqHpdf.set_name("pdf_qqbar")
    .set_latexname("VBF pdf")
    .set_type("constlnN")
    .set_procsaffected(qqHprocs);
  if(mass=="110")qqHpdf.set_constvalue(1.0265);
  if(mass=="125")qqHpdf.set_constvalue(1.027);
  if(mass=="150")qqHpdf.set_constvalue(1.026);
  if(mass=="200")qqHpdf.set_constvalue(1.026);
  if(mass=="300")qqHpdf.set_constvalue(1.0255);
  if(mass=="400")qqHpdf.set_constvalue(1.027);

  Syst vvmcstat;
  vvmcstat.set_name("CMS_VBFHinv_vv_norm")
    .set_latexname("VV MC stat.")
    .set_type("fromMCstatlnN")
    .set_procsaffected({"vv"});  

  Syst vvxsunc;
  vvxsunc.set_name("CMS_VBFHinv_vv_xsunc")
    .set_latexname("VV cross-section")
    .set_type("constlnN")
    .set_procsaffected({"vv"})
    .set_constvalue(1.007);

  

  Syst wgmcstat;
  wgmcstat.set_name("CMS_VBFHinv_wg_norm")
    .set_latexname("WGAMMA UNCERTAINTY SHOULDN'T APPEAR IN FINAL RESULT")
    .set_type("fromMCstatlnN")
    .set_procsaffected({"wg"});  

  Syst mcfmzvv;
  mcfmzvv.set_name("CMS_VBFHinv_zvv_extrapfacunc")
    .set_latexname("$Z/\\gamma^{*}\\rightarrow\\mu\\mu$ to $Z\\rightarrow\\nu\\nu$ extrapolation")
    .set_type("constlnN")
    .set_procsaffected({"zvv"})
    .set_constvalue(1.2);

  systematics.push_back(lumi8tev);
  systematics.push_back(eleeff);
  systematics.push_back(mueff);
  systematics.push_back(jes);
  systematics.push_back(jer);
  if(do_ues)systematics.push_back(ues);
  systematics.push_back(pu);
  systematics.push_back(zvvmcstat);
  //  systematics.push_back(zvvdatastat);
  systematics.push_back(zvvdatastatgmn);
  systematics.push_back(wmumcstat);
  systematics.push_back(wmudatastat);
  systematics.push_back(welmcstat);
  systematics.push_back(weldatastat);
  systematics.push_back(wtauideff);
  systematics.push_back(wtaujetmetextrap);
  systematics.push_back(wtaumcstat);
  systematics.push_back(wtaudatastat);
  //systematics.push_back(mcfmzvv);
    if(do_datatop){
    systematics.push_back(topmcstat);
    systematics.push_back(topdatastat);
  }
  else{
    systematics.push_back(mctopmcstat);
    systematics.push_back(topmcsfunc);
  }
  if(do_qcdfromshape)systematics.push_back(qcdmcstat);
  if(do_qcdfromnumber)systematics.push_back(qcdfromnumerr);
  //systematics.push_back(wgmcstat);
  systematics.push_back(vvmcstat);
  systematics.push_back(vvxsunc);
  systematics.push_back(qqHmcstat);
  systematics.push_back(qqHqcdscale);
  systematics.push_back(qqHpdf);
  if(do_ggh){
    systematics.push_back(ggHmcstat);
    systematics.push_back(ggHqcdscale);
    systematics.push_back(ggHpdf);
    systematics.push_back(ggHUEPS);
  }
  

  std::cout<<"Setting up datacard header.."<<std::endl;
  std::ofstream datacard;
  datacard.open (outname.c_str());
  //Header information
  datacard << "# Invisible Higgs analysis for mH="<<mass<<" GeV"<<std::endl;
  datacard << "imax 1"<<std::endl;
  datacard <<"jmax *  number of backgrounds"<<std::endl;
  datacard <<"kmax *  number of nuisance parameters (sources of systematical uncertainties)"<<std::endl;
  datacard <<"shapes *    ch1  FAKE"<<std::endl;
  datacard <<"bin ch1"<<std::endl;

  if(blind)std::cout<<"Blind is true observation will be sum of background expected rates"<<std::endl;
  else std::cout<<"Getting observation"<<std::endl;
  //OBSERVATION
  TDirectory* dir;
  if(!blind){
    std::string dirname="data_obs";
    if(dirname==""){
      std::cout<<"Error: Empty dir name given exiting"<<std::endl;
      return 1;
    }
    else if(!nunu->GetDirectory(dirname.c_str())){
      std::cout<<"Error: No directory called: "<<dirname<<" exiting"<<std::endl;
      return 1;
    }
    else{
      dir=nunu->GetDirectory(dirname.c_str());
    }
    dir->cd();
    TH1F* histo = (TH1F*)dir->Get("jet2_pt");
    double rate=histo->Integral(0, histo->GetNbinsX() + 1);
    datacard<<"observation "<<rate<<std::endl;
  }
  else{
    double totalbkgs=0;
    for(unsigned iProc=0;iProc<bkgprocesses.size();iProc++){
      std::string dirname=bkgprocesses[iProc];
      if(dirname==""){
	std::cout<<"Error: Empty dir name given exiting"<<std::endl;
	return 1;
      }
      else if(!nunu->GetDirectory(dirname.c_str())){
	std::cout<<"Error: No directory called: "<<dirname<<" exiting"<<std::endl;
	return 1;
      }
      else{
	dir=nunu->GetDirectory(dirname.c_str());
      }
      dir->cd();
      TH1F* histo = (TH1F*)dir->Get("jet2_pt");
      double rate=histo->Integral(0, histo->GetNbinsX() + 1);
      totalbkgs+=rate;
    }
    if(do_qcdfromnumber) totalbkgs+=qcdrate;
    datacard<<"observation "<<totalbkgs<<std::endl;

  }

  //Bin information and process names
  std::cout<<"Writing out bin and process names.."<<std::endl;
  datacard <<"------------"<<std::endl;

  datacard <<"bin\t\t";
  for(unsigned iProc=0;iProc<(sigprocesses.size()+bkgprocesses.size());iProc++){
    datacard<<"\tch1";
  }
  if(do_qcdfromnumber) datacard<<"\tch1";
  datacard<<std::endl;

  datacard<<"process\t\t";
  for(unsigned iSigProc=0;iSigProc<sigprocesses.size();iSigProc++){
    datacard<<"\t"<<sigprocessesnames[iSigProc];
  }

  for(unsigned iBkgProc=0;iBkgProc<bkgprocesses.size();iBkgProc++){
    datacard<<"\t"<<bkgprocesses[iBkgProc];
  }
  if(do_qcdfromnumber) datacard<<"\tqcd";
  datacard<<std::endl;

  datacard<<"process\t\t";
  for(unsigned iSigProc=0;iSigProc<sigprocesses.size();iSigProc++){
    datacard<<"\t"<<(-((int)sigprocesses.size())+1+(int)iSigProc);
  }

  for(unsigned iBkgProc=0;iBkgProc<bkgprocesses.size();iBkgProc++){
    datacard<<"\t"<<iBkgProc+1;
  }

  if(do_qcdfromnumber) datacard<<"\t"<<bkgprocesses.size()+1;
  datacard<<std::endl;
  //rate
  std::cout<<"Getting rates from file.."<<std::endl;
  std::vector<double> sigcentralrates;//store central rates for syst calculation
  std::vector<double> bkgcentralrates;
  datacard<<"rate\t\t";
  for(unsigned iProc=0;iProc<(sigprocesses.size()+bkgprocesses.size());iProc++){
    std::string dirname;
    if(iProc<sigprocesses.size())dirname=sigprocesses[iProc];
    else dirname=bkgprocesses[iProc-sigprocesses.size()];
    if(dirname==""){
      std::cout<<"Error: Empty dir name given exiting"<<std::endl;
      return 1;
    }
    else if(!nunu->GetDirectory(dirname.c_str())){
      std::cout<<"Error: No directory called: "<<dirname<<" exiting"<<std::endl;
      return 1;
    }
    else{
      dir=nunu->GetDirectory(dirname.c_str());
    }
    dir->cd();
    TH1F* histo = (TH1F*)dir->Get("jet2_pt");
    double rate=Integral(histo);
    if(iProc<sigprocesses.size())sigcentralrates.push_back(rate);
    else bkgcentralrates.push_back(rate);
    datacard<<"\t"<<rate;
  }
  if(do_qcdfromnumber)datacard<<"\t"<<qcdrate;
  datacard<<std::endl<<"------------"<<std::endl;
  //Systematics
  std::cout<<"Processing systematics.."<<std::endl;
  for(unsigned iSyst=0;iSyst<systematics.size();iSyst++){
    if(verbose)std::cout<<"  "<<systematics[iSyst].name()<<std::endl;
    //Constant lnN systs
    if(systematics[iSyst].type().find("lnN")!=std::string::npos){
      datacard<<systematics[iSyst].name()<<"\tlnN";
    }
    if(systematics[iSyst].type().find("gmN")!=std::string::npos){
      datacard<<systematics[iSyst].name()<<"\tgmN"<<"\t"<<systematics[iSyst].constvalue();
    }
    double thissystbkgstat=0;
    double thissystbkgsyst=0;
    double thissystsigstat=0;
    double thissystsigsyst=0;

    for(unsigned iProc=0;iProc<(sigprocesses.size()+bkgprocesses.size());iProc++){
      std::string dirname;
      if(iProc<sigprocesses.size())dirname=sigprocesses[iProc];
      else dirname=bkgprocesses[iProc-sigprocesses.size()];
      if(std::find(systematics[iSyst].procsaffected().begin(), systematics[iSyst].procsaffected().end(), dirname)==systematics[iSyst].procsaffected().end())datacard<<"\t-";
      else{

	//constant lnN
	if(systematics[iSyst].type()=="constlnN"){
	  datacard<<"\t"<<systematics[iSyst].constvalue();
	  //store for total error calculation
	  double error=systematics[iSyst].constvalue()-1;

	  double abserror=0;
	  if(iProc<sigprocesses.size()){
	    abserror=error*sigcentralrates[iProc];
	    if(systematics[iSyst].is_datastat())thissystsigstat+=abserror;
	    else thissystsigsyst+=abserror;
	  }
	  else{
	    abserror=error*bkgcentralrates[iProc-sigprocesses.size()];
	    if(systematics[iSyst].is_datastat())thissystbkgstat+=abserror;
	    else thissystbkgsyst+=abserror;
	  }

	  if(systematics[iSyst].is_datastat()){
	    if(procstattotal.find(dirname)==procstattotal.end()) procstattotal[dirname]=error;
	    else procstattotal[dirname]=sqrt(pow(procstattotal[dirname],2)+pow(error,2));
	  }
	  else{
	    if(procsysttotal.find(dirname)==procsysttotal.end()) procsysttotal[dirname]=error;
	    else procsysttotal[dirname]=sqrt(pow(procsysttotal[dirname],2)+pow(error,2));
	  }
	  if(verbose)std::cout<<"    "<<dirname<<" "<<procsysttotal[dirname]<<" "<<procstattotal[dirname]<<std::endl;
	}

	//from file lnN
	else if(systematics[iSyst].type()=="fromfilelnN"){
	  TDirectory* updir;
	  TDirectory* downdir;
	  if(dirname==""){
	    std::cout<<"Error: Empty dir name given exiting"<<std::endl;
	    return 1;
	  }
	  else if(!systematics[iSyst].uptfile()->GetDirectory(dirname.c_str())){
	  std::cout<<"Error: No directory called: "<<dirname<<" exiting"<<std::endl;
	  return 1;
	  }
	  else{
	    updir=systematics[iSyst].uptfile()->GetDirectory(dirname.c_str());
	    downdir=systematics[iSyst].downtfile()->GetDirectory(dirname.c_str());
	  }
	  updir->cd();
	  TH1F* uphisto = (TH1F*)updir->Get("jet2_pt");
	  double uprate=Integral(uphisto);
	  TH1F* downhisto = (TH1F*)downdir->Get("jet2_pt");
	  double downrate=Integral(downhisto);
	  double centralrate;
	  if(iProc<sigprocesses.size())centralrate=sigcentralrates[iProc];
	  else centralrate=bkgcentralrates[iProc-sigprocesses.size()];
	  double downlnnfac=downrate/centralrate;
	  double uplnnfac=uprate/centralrate;
	  datacard<<"\t"<<downlnnfac<<"/"<<uplnnfac;
	  
	  double error=(fabs(1-uplnnfac)+fabs(1-downlnnfac))/2;

	  double abserror=0;
	  if(iProc<sigprocesses.size()){
	    abserror=error*sigcentralrates[iProc];
	    if(systematics[iSyst].is_datastat())thissystsigstat+=abserror;
	    else thissystsigsyst+=abserror;
	  }
	  else{
	    abserror=error*bkgcentralrates[iProc-sigprocesses.size()];
	    if(systematics[iSyst].is_datastat())thissystbkgstat+=abserror;
	    else thissystbkgsyst+=abserror;
	  }

	  if(systematics[iSyst].is_datastat()){
	    if(procstattotal.find(dirname)==procstattotal.end()) procstattotal[dirname]=error;
	    else procstattotal[dirname]=sqrt(pow(procstattotal[dirname],2)+pow(error,2));
	  }
	  else{
	    if(procsysttotal.find(dirname)==procsysttotal.end()) procsysttotal[dirname]=error;
	    else procsysttotal[dirname]=sqrt(pow(procsysttotal[dirname],2)+pow(error,2));
	  }
	  if(verbose)std::cout<<"    "<<dirname<<" "<<procsysttotal[dirname]<<" "<<procstattotal[dirname]<<std::endl;
	}
	
	//from mc stat lnn
 	else if(systematics[iSyst].type()=="fromMCstatlnN"){
	  TDirectory* dir=nunu->GetDirectory(dirname.c_str());	  
	  dir->cd();
	  TH1F* histo = (TH1F*)dir->Get("jet2_pt");
	  double error=Error(histo);
	  double centralrate;
	  if(iProc<sigprocesses.size())centralrate=sigcentralrates[iProc];
	  else centralrate=bkgcentralrates[iProc-sigprocesses.size()];
	  double lnnfac=error/centralrate;
	  lnnfac=lnnfac+1;
	  datacard<<"\t"<<lnnfac;
	  
	  double relerror=lnnfac-1;

	  double abserror=0;
	  if(iProc<sigprocesses.size()){
	    abserror=relerror*sigcentralrates[iProc];
	    if(systematics[iSyst].is_datastat())thissystsigstat+=abserror;
	    else thissystsigsyst+=abserror;
	  }
	  else{
	    abserror=relerror*bkgcentralrates[iProc-sigprocesses.size()];
	    if(systematics[iSyst].is_datastat())thissystbkgstat+=abserror;
	    else thissystbkgsyst+=abserror;
	  }

	  if(systematics[iSyst].is_datastat()){
	    if(procstattotal.find(dirname)==procstattotal.end()) procstattotal[dirname]=relerror;
	    else procstattotal[dirname]=sqrt(pow(procstattotal[dirname],2)+pow(relerror,2));
	  }
	  else{
	    if(procsysttotal.find(dirname)==procsysttotal.end()) procsysttotal[dirname]=relerror;
	    else procsysttotal[dirname]=sqrt(pow(procsysttotal[dirname],2)+pow(relerror,2));
	  }
	  if(verbose)std::cout<<"    "<<dirname<<" "<<procsysttotal[dirname]<<" "<<procstattotal[dirname]<<std::endl;
	}

      //data driven data stat lnnn
	else if(systematics[iSyst].type()=="datadrivendatastatlnN"){
	  TDirectory* dir=nunu->GetDirectory(dirname.c_str());
	  TVectorD* errvec = (TVectorD*)dir->Get("normerrs");
	  double lnnfac=(*errvec)[0];
	  lnnfac=lnnfac+1;
	  datacard<<"\t"<<lnnfac;
	  
	  double error=lnnfac-1;
	  double abserror=0;
	  if(iProc<sigprocesses.size()){
	    abserror=error*sigcentralrates[iProc];
	    thissystsigstat+=abserror;
	  }
	  else{
	    abserror=error*bkgcentralrates[iProc-sigprocesses.size()];
	    thissystbkgstat+=abserror;
	  }

	  if(procstattotal.find(dirname)==procstattotal.end()) procstattotal[dirname]=error;
	  else procstattotal[dirname]=sqrt(pow(procstattotal[dirname],2)+pow(error,2));
	  if(verbose)std::cout<<"    "<<dirname<<" "<<procsysttotal[dirname]<<" "<<procstattotal[dirname]<<std::endl;
	}
	else if(systematics[iSyst].type()=="datadrivendatastatgmN"){
	  /*TDirectory* dir=nunu->GetDirectory(dirname.c_str());*/
	  double gmnfac=0;
	  double nevents=systematics[iSyst].constvalue();
	  double error=1/sqrt(systematics[iSyst].constvalue());
	  double abserror=0;
	  if(iProc<sigprocesses.size()){
	    gmnfac=sigcentralrates[iProc]/nevents;
	    abserror=error*sigcentralrates[iProc];
	    thissystsigstat+=abserror;
	  }
	  else{
	    gmnfac=bkgcentralrates[iProc-sigprocesses.size()]/nevents;
	    abserror=error*bkgcentralrates[iProc-sigprocesses.size()];
	    thissystbkgstat+=abserror;
	  }

	  datacard<<"\t"<<gmnfac;

	  if(procstattotal.find(dirname)==procstattotal.end()) procstattotal[dirname]=error;
	  else procstattotal[dirname]=sqrt(pow(procstattotal[dirname],2)+pow(error,2));
	  if(verbose)std::cout<<"    "<<dirname<<" "<<procsysttotal[dirname]<<" "<<procstattotal[dirname]<<std::endl;
	}

	//data driven mc stat lnn
	else if(systematics[iSyst].type()=="datadrivenMCstatlnN"){
	  TDirectory* dir=nunu->GetDirectory(dirname.c_str());
	  TVectorD* errvec= (TVectorD*)dir->Get("normerrs");
	  double lnnfac=(*errvec)[1];
	  lnnfac=lnnfac+1;
	  datacard<<"\t"<<lnnfac;
	  double error =lnnfac-1;
	  double abserror=0;
	  if(iProc<sigprocesses.size()){
	    abserror=error*sigcentralrates[iProc];
	    thissystsigsyst+=abserror;
	  }
	  else{
	    abserror=error*bkgcentralrates[iProc-sigprocesses.size()];
	    thissystbkgsyst+=abserror;
	  }
	  if(procsysttotal.find(dirname)==procsysttotal.end()) procsysttotal[dirname]=error;
	  else procsysttotal[dirname]=sqrt(pow(procsysttotal[dirname],2)+pow(error,2));
	  if(verbose)std::cout<<"    "<<dirname<<" "<<procsysttotal[dirname]<<" "<<procstattotal[dirname]<<std::endl;
	}
	else if(systematics[iSyst].type()=="qcdfromnumberlnN"){
	  datacard<<systematics[iSyst].name()<<"\t-";
	}
	else std::cout<<"Warning: Unrecognised systematic type skipping!!"<<std::endl;
      }
    }
    if(do_qcdfromnumber){
      if(systematics[iSyst].type()=="qcdfromnumberlnN")datacard<<"\t"<<(1+qcdabserr/qcdrate);
      else datacard<<"\t-";
    }
    datacard<<std::endl;
    totalbkgsyst=sqrt(pow(totalbkgsyst,2)+pow(thissystbkgsyst,2));
    totalbkgstat=sqrt(pow(totalbkgstat,2)+pow(thissystbkgstat,2));
    totalsigsyst=sqrt(pow(totalsigsyst,2)+pow(thissystsigsyst,2));
    totalsigstat=sqrt(pow(totalsigstat,2)+pow(thissystsigstat,2));
    systsigtotal[systematics[iSyst].name()]=thissystsigsyst;
    if(systematics[iSyst].is_datastat()) systbkgtotal[systematics[iSyst].name()]=thissystbkgstat;
    else systbkgtotal[systematics[iSyst].name()]=thissystbkgsyst;
    
  }
  //LOOP OVER PROCESSES AND OUTPUT TOTAL STAT AND SYST
  std::cout<<"Totals are:"<<std::endl;
  double bkgtotal=0;
  for(unsigned ibkg=0;ibkg<bkgcentralrates.size();ibkg++){
    bkgtotal+=bkgcentralrates[ibkg];
  }
  double sigtotal=0;
  for(unsigned isig=0;isig<sigcentralrates.size();isig++){
    sigtotal+=sigcentralrates[isig];
  }
  if(do_qcdfromnumber){
    bkgtotal+=qcdrate;
    totalbkgsyst=sqrt(pow(totalbkgsyst,2)+pow(qcdabserr,2));
  }

  if(do_individualsystsummary){
    for(unsigned iSyst=0;iSyst<systematics.size();iSyst++){
      printf("%s & %.2f & %.2f \\\\ \n",systematics[iSyst].latexname().c_str(),systbkgtotal[systematics[iSyst].name()]/bkgtotal*100,systsigtotal[systematics[iSyst].name()]/sigtotal*100 );
            
    }
  }
  if(!do_latex){
    printf("Background: %.1f +- %.1f (stat.) +- %.1f (syst.)\n",bkgtotal,totalbkgstat,totalbkgsyst);
    printf("Signal: %.1f +- %.1f (stat.) +- %.1f (syst.)\n",sigtotal,totalsigstat,totalsigsyst);

    std::cout<<"Error on following processes is:"<<std::endl;
    for(unsigned iProc=0;iProc<(sigprocesses.size());iProc++){
      printf("  %s: %.1f +- %.1f (stat.) +- %.1f (syst.)\n",sigprocesses[iProc].c_str(),sigcentralrates[iProc],procstattotal[sigprocesses[iProc]]*sigcentralrates[iProc],procsysttotal[sigprocesses[iProc]]*sigcentralrates[iProc]);
    }  
    for(unsigned iProc=0;iProc<(bkgprocesses.size());iProc++){
      printf("  %s: %.1f +- %.1f (stat.) +- %.1f (syst.)\n",bkgprocesses[iProc].c_str(),bkgcentralrates[iProc],procstattotal[bkgprocesses[iProc]]*bkgcentralrates[iProc],procsysttotal[bkgprocesses[iProc]]*bkgcentralrates[iProc]);
    }  
  }
  else{
    std::cout<<"\\begin{tabular}{|l|c|}"<<std::endl;
    std::cout<<"\\hline"<<std::endl;
    std::cout<<"Background       & $N_{est} \\pm (stat) \\pm (syst)$ \\\\"<<std::endl;
    std::cout<<"\\hline"<<std::endl;
    for(unsigned iProc=0;iProc<(bkgprocesses.size());iProc++){
      printf("%s&$%.1f \\pm %.1f \\pm %.1f$\\\\\n",bkgprocesslatex[iProc].c_str(),bkgcentralrates[iProc],procstattotal[bkgprocesses[iProc]]*bkgcentralrates[iProc],procsysttotal[bkgprocesses[iProc]]*bkgcentralrates[iProc]);
    }  
    if(do_qcdfromnumber){
      std::cout<<"QCD multijet &$"<<qcdrate<<"\\pm 0 \\pm"<<qcdabserr<<"$\\\\"<<std::endl;
    }
    std::cout<<"\\hline"<<std::endl;
    printf("Total Background &$%.1f \\pm %.1f \\pm %.1f $\\\\\n",bkgtotal,totalbkgstat,totalbkgsyst);
    std::cout<<"\\hline"<<std::endl;
    for(unsigned iProc=0;iProc<(sigprocesses.size());iProc++){
      printf("%s &$%.1f \\pm %.1f \\pm %.1f $\\\\\n",sigprocesslatex[iProc].c_str(),sigcentralrates[iProc],procstattotal[sigprocesses[iProc]]*sigcentralrates[iProc],procsysttotal[sigprocesses[iProc]]*sigcentralrates[iProc]);
    }  
    std::cout<<"\\hline"<<std::endl;
    std::cout<<"\\end{tabular}"<<std::endl;
  }

  datacard.close();  

  }
