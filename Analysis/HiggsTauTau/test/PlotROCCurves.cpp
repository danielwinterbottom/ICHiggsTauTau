#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"
#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TMath.h"

#include "TGraphErrors.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plotting.h"

using namespace std;
using namespace ic;

 TH1F drawVariable(std::string const& variable, std::string const& selection, 
                                       std::string const& file, 
                                       std::string const& dirname,
                                       std::string const& treename, double const& start =0, double const& end = 2){
    TH1::SetDefaultSumw2(true);
    TH1F *htemp = nullptr;
 //     TH1::AddDirectory(true);
//      htemp = new TH1F("htemp","htemp", 100, 0, 5);
  //    TH1::AddDirectory(false);
      std::string full_variable = variable+">>htemp(500,"+boost::lexical_cast<std::string>(start)+","+boost::lexical_cast<std::string>(end)+")";
    std::string full_selection = "("+selection+")*wt";
  //   std::cout << full_selection << std::endl;
  //   std::cout << full_variable << std::endl;
    TFile *file1 = TFile::Open(file.c_str());
    TTree *tree = dynamic_cast<TTree*>(file1->Get((dirname+"/"+treename).c_str()));

//    TTree tree = GetFromTFile<TTree>(file, dirname, treename);
    TH1::AddDirectory(true);
    tree->Draw(full_variable.c_str(), full_selection.c_str(), "goff");
    TH1::AddDirectory(false);
    htemp = (TH1F*)gDirectory->Get("htemp");
    TH1F result = (*htemp);
//    result.SetName(("DY"+variable).c_str());
 //   std::cout<<result.GetName()<<std::endl;
    gDirectory->Delete("htemp;*");
    //file1->Close();
    return result;
  }
 
 TH1F addQCDHists(std::string const& variable, std::string const&selection, std::string const& path,std::string const& channel, std::string const& dirname, std::string const& treename){
   long numevents100to200 = 81637494;
   double xs100to200 = 2.75*10000000;
   long numevents200to300 = 18718905;
   double xs200to300 = 1.74*1000000;
   long numevents300to500 = 19826197;
   double xs300to500 = 3.67*10000;
   long numevents500to700 = 19664159;
   double xs500to700 = 2.94*10000;
   long numevents700to1000 = 15356448;
   double xs700to1000 = 6.524*1000;
   long numevents1000to1500  = 4963895;
   double xs1000to1500 = 1.064*1000;
   long numevents1500to2000 = 3868886;
   double xs1500to2000 = 121.5;
   long numevents2000toInf = 1912529;
   double xs2000toInf = 2.542*10;

      TH1F QCD_100to200 = drawVariable(variable, selection, (path+"/QCD_Ht100to200_"+channel+"_2015.root").c_str(),dirname, treename);
      //QCD_100to200.SetName(("QCD100to200"+variable).c_str());
      TH1F QCD_200to300 = drawVariable(variable, selection, (path+"/QCD_Ht200to300_"+channel+"_2015.root").c_str(),dirname, treename);
      //QCD_200to300.SetName(("QCD200to300"+variable).c_str());
      TH1F QCD_300to500 = drawVariable(variable, selection, (path+"/QCD_Ht300to500_"+channel+"_2015.root").c_str(),dirname, treename);
      //QCD_300to500.SetName(("QCD300to500"+variable).c_str());
      TH1F QCD_500to700 = drawVariable(variable, selection, (path+"/QCD_Ht500to700_"+channel+"_2015.root").c_str(),dirname, treename);
      //QCD_500to700.SetName(("QCD500to700"+variable).c_str());
      TH1F QCD_700to1000 = drawVariable(variable, selection, (path+"/QCD_Ht700to1000_"+channel+"_2015.root").c_str(),dirname, treename);
      //QCD_700to1000.SetName(("QCD700to1000"+variable).c_str());
      TH1F QCD_1000to1500 = drawVariable(variable, selection, (path+"/QCD_Ht1000to1500_"+channel+"_2015.root").c_str(),dirname, treename);
      //QCD_1000to1500.SetName(("QCD1000to1500"+variable).c_str());
      TH1F QCD_1500to2000 = drawVariable(variable, selection, (path+"/QCD_Ht1500to2000_"+channel+"_2015.root").c_str(),dirname, treename);
      //QCD_1500to2000.SetName(("QCD1500to2000"+variable).c_str());
      TH1F QCD_2000toInf = drawVariable(variable, selection, (path+"/QCD_Ht2000toInf_"+channel+"_2015.root").c_str(),dirname, treename,0,20);
      //QCD_2000toInf.SetName(("QCD2000toInf"+variable).c_str());
      QCD_100to200.Scale(xs100to200/numevents100to200);
      QCD_200to300.Scale(xs200to300/numevents200to300);
      QCD_300to500.Scale(xs300to500/numevents300to500);
      QCD_500to700.Scale(xs500to700/numevents500to700);
      QCD_700to1000.Scale(xs700to1000/numevents700to1000);
      QCD_1000to1500.Scale(xs1000to1500/numevents1000to1500);
      QCD_1500to2000.Scale(xs1500to2000/numevents1500to2000);
      QCD_2000toInf.Scale(xs2000toInf/numevents2000toInf);
      QCD_100to200.Add(&QCD_200to300);
      QCD_100to200.Add(&QCD_300to500);
      QCD_100to200.Add(&QCD_500to700);
      QCD_100to200.Add(&QCD_700to1000);
      QCD_100to200.Add(&QCD_1000to1500);
      QCD_100to200.Add(&QCD_1500to2000);
      QCD_100to200.Add(&QCD_2000toInf);
      return QCD_100to200;
  }
      


 int main(int /*argc*/, char* /*argv*/[]){
  using namespace ic;
  using namespace std;
  std::string real_sel = "(gen_match==2)";
  std::string fake_sel = "(gen_match==6)";
  std::string channel = "et";
  bool do_tau = true;
  
  if(do_tau){
   //real_sel = "(gen_match==5&&against_mu_tight>0&&against_ele_vloose>0)";
   real_sel = "(gen_match==5)";
   fake_sel = "(gen_match==6)";
   //fake_sel = "(gen_match==6&&against_mu_tight>0&&against_ele_vloose>0)";
   TH1F DY_real_iso_mva_newDMwLTraw = drawVariable("iso_mva_newDMwLTraw",real_sel,("output/Oct30TauIDNotForFit/TT-ext_"+channel+"_2015.root").c_str(),"HTTTauEfficiency","tree",-1,1);
   TH1F DY_fake_iso_mva_newDMwLTraw = drawVariable("iso_mva_newDMwLTraw",fake_sel,("output/Oct30TauIDNotForFit/TT-ext_"+channel+"_2015.root").c_str(),"HTTTauEfficiency","tree",-1,1);
   TH1F DY_real_iso_dbetacorr = drawVariable("iso_dbetacorr",real_sel,("output/Oct30TauIDNotForFit/TT-ext_"+channel+"_2015.root").c_str(),"HTTTauEfficiency","tree",0,20);
   TH1F DY_fake_iso_dbetacorr = drawVariable("iso_dbetacorr",fake_sel,("output/Oct30TauIDNotForFit/TT-ext_"+channel+"_2015.root").c_str(),"HTTTauEfficiency","tree",0,20);
  std::vector<double> mva_real_eff;
  std::vector<double> mva_fake_rej;
  std::vector<double> db_real_eff;
  std::vector<double> db_fake_rej;

 DY_real_iso_mva_newDMwLTraw.Scale(1./DY_real_iso_mva_newDMwLTraw.Integral());
 DY_fake_iso_mva_newDMwLTraw.Scale(1./DY_fake_iso_mva_newDMwLTraw.Integral());
 DY_real_iso_dbetacorr.Scale(1./DY_real_iso_dbetacorr.Integral());
 DY_fake_iso_dbetacorr.Scale(1./DY_fake_iso_dbetacorr.Integral());


  for(int i=1;i<=500;i++){
   mva_real_eff.push_back(1-DY_real_iso_mva_newDMwLTraw.Integral(1,i));     
   mva_fake_rej.push_back(DY_fake_iso_mva_newDMwLTraw.Integral(1,i));     
   db_real_eff.push_back(DY_real_iso_dbetacorr.Integral(1,i));     
   db_fake_rej.push_back(1-DY_fake_iso_dbetacorr.Integral(1,i));     
 }

  TGraph *mva = new TGraph(500,&(mva_real_eff[0]),&(mva_fake_rej[0]));
  TGraph *db = new TGraph(500,&(db_real_eff[0]),&(db_fake_rej[0]));

 mva->SetMarkerStyle(20);
 mva->SetMarkerColor(kRed);
 db->SetMarkerStyle(21);
 db->SetMarkerColor(kBlue);

 mva->GetXaxis()->SetRangeUser(0.6,1);
 mva->GetYaxis()->SetRangeUser(0,1);
 mva->SetTitle(";Real #tau efficiency;Fake #tau rejection");

  TCanvas plot_canv("plot_canv","plot_canv",1200,800); 
  plot_canv.cd();
  mva->Draw("AP"); 
  db->Draw("PSAME");
  TLegend *leg = PositionedLegend(0.3,0.3,4,0);
  leg->SetFillStyle(0);
  leg->AddEntry(mva,"MVA (with LT) raw iso","p");
  leg->AddEntry(db,"dB corr iso","p");

  leg->Draw("SAME");
  
 // plot_canv.SetGrid(1,1);

  plot_canv.SaveAs("TauROCCurve.pdf");



} else {

  TH1F DY_real_db03 = drawVariable("iso_db03",real_sel, ("/vols/cms04/amd12/CMSSW_7_4_7/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/Oct30IsoStudy/DYJetsToLL_M-50-LO_"+channel+"_2015.root").c_str(),"HTTMuonEfficiency","tree");
  TH1F QCD_fake_db03 = addQCDHists("iso_db03",fake_sel, "./output/Oct30IsoStudy/",channel,"HTTMuonEfficiency","tree");

  TH1F DY_real_db03allch = drawVariable("iso_db03allch",real_sel, ("/vols/cms04/amd12/CMSSW_7_4_7/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/Oct30IsoStudy/DYJetsToLL_M-50-LO_"+channel+"_2015.root").c_str(),"HTTMuonEfficiency","tree");
  TH1F QCD_fake_db03allch = addQCDHists("iso_db03allch",fake_sel, "./output/Oct30IsoStudy/",channel,"HTTMuonEfficiency","tree");


  TH1F DY_real_db04 = drawVariable("iso_db04",real_sel, ("./output/Oct30IsoStudy/DYJetsToLL_M-50-LO_"+channel+"_2015.root").c_str(),"HTTMuonEfficiency","tree");
  TH1F QCD_fake_db04 = addQCDHists("iso_db04",fake_sel, "./output/Oct30IsoStudy/",channel,"HTTMuonEfficiency","tree");

  TH1F DY_real_db04allch = drawVariable("iso_db04allch",real_sel, ("./output/Oct30IsoStudy/DYJetsToLL_M-50-LO_"+channel+"_2015.root").c_str(),"HTTMuonEfficiency","tree");
  TH1F QCD_fake_db04allch = addQCDHists("iso_db04allch",fake_sel, "./output/Oct30IsoStudy/",channel,"HTTMuonEfficiency","tree");

  TH1F DY_real_trk03 = drawVariable("iso_trk03",real_sel, ("./output/Oct30IsoStudy/DYJetsToLL_M-50-LO_"+channel+"_2015.root").c_str(),"HTTMuonEfficiency","tree");
  TH1F QCD_fake_trk03 = addQCDHists("iso_trk03",fake_sel, "./output/Oct30IsoStudy/",channel,"HTTMuonEfficiency","tree");
  
  std::vector<double> trk03_real_eff;
  std::vector<double> trk03_fake_rej;
  std::vector<double> db03_real_eff;
  std::vector<double> db03_fake_rej;
  std::vector<double> db03allch_real_eff;
  std::vector<double> db03allch_fake_rej;
  std::vector<double> db04allch_real_eff;
  std::vector<double> db04allch_fake_rej;
  std::vector<double> db04_real_eff;
  std::vector<double> db04_fake_rej;

 DY_real_db03.Scale(1./DY_real_db03.Integral());
 DY_real_db04.Scale(1./DY_real_db04.Integral());
 DY_real_db03allch.Scale(1./DY_real_db03allch.Integral());
 DY_real_db04allch.Scale(1./DY_real_db04allch.Integral());
 DY_real_trk03.Scale(1./DY_real_trk03.Integral());
 QCD_fake_db03.Scale(1./QCD_fake_db03.Integral());
 QCD_fake_db04.Scale(1./QCD_fake_db04.Integral());
 QCD_fake_db03allch.Scale(1./QCD_fake_db03allch.Integral());
 QCD_fake_db04allch.Scale(1./QCD_fake_db04allch.Integral());
 QCD_fake_trk03.Scale(1./QCD_fake_trk03.Integral());


  for(int i=1;i<=100;i++){
   trk03_real_eff.push_back(DY_real_trk03.Integral(1,i));     
   db03_real_eff.push_back(DY_real_db03.Integral(1,i));     
   db03allch_real_eff.push_back(DY_real_db03allch.Integral(1,i));     
   db04_real_eff.push_back(DY_real_db04.Integral(1,i));     
   db04allch_real_eff.push_back(DY_real_db04allch.Integral(1,i));     
   trk03_fake_rej.push_back(1-QCD_fake_trk03.Integral(1,i));     
   db03_fake_rej.push_back(1-QCD_fake_db03.Integral(1,i));     
   db03allch_fake_rej.push_back(1-QCD_fake_db03allch.Integral(1,i));     
   db04_fake_rej.push_back(1-QCD_fake_db04.Integral(1,i));     
   db04allch_fake_rej.push_back(1-QCD_fake_db04allch.Integral(1,i));     
 }

  TGraph *trk03 = new TGraph(100,&(trk03_real_eff[0]),&(trk03_fake_rej[0]));
  TGraph *db03 = new TGraph(100,&(db03_real_eff[0]),&(db03_fake_rej[0]));
  TGraph *db03allch = new TGraph(100,&(db03allch_real_eff[0]),&(db03allch_fake_rej[0]));
  TGraph *db04 = new TGraph(100,&(db04_real_eff[0]),&(db04_fake_rej[0]));
  TGraph *db04allch = new TGraph(100,&(db04allch_real_eff[0]),&(db04allch_fake_rej[0]));

 trk03->SetMarkerStyle(20);
 trk03->SetMarkerColor(kRed);
 db03->SetMarkerStyle(21);
 db03->SetMarkerColor(kBlue);
 db03allch->SetMarkerStyle(22);
 db03allch->SetMarkerColor(kGreen+3);
 db04->SetMarkerStyle(23);
 db04->SetMarkerColor(kRed+2);
 db04allch->SetMarkerStyle(24);
 db04allch->SetMarkerColor(kBlack);

 db04->GetXaxis()->SetRangeUser(0.6,1);
 db04->GetYaxis()->SetRangeUser(0,1);
 db04->SetTitle(";Real #mu efficiency;Fake #mu rejection");

  TCanvas plot_canv("plot_canv","plot_canv",1200,800); 
  plot_canv.cd();
  db04->Draw("AP"); 
  db03->Draw("PSAME");
  db03allch->Draw("PSAME");
  trk03->Draw("PSAME");
  db04allch->Draw("PSAME");
  TLegend *leg = PositionedLegend(0.3,0.3,4,0);
  leg->SetFillStyle(0);
  leg->AddEntry(trk03,"Tracker iso (cs 0.3)","p");
  leg->AddEntry(db03,"dBeta corr iso (cs 0.3)","p");
  leg->AddEntry(db03allch,"dBeta corr iso, allcharged (cs 0.3)","p");
  leg->AddEntry(db04,"dBeta corr iso (cs 0.4)","p");
  leg->AddEntry(db04allch,"dBeta corr iso, allcharged (cs 0.4)","p");

  leg->Draw("SAME");
  
 // plot_canv.SetGrid(1,1);

  plot_canv.SaveAs("MuonROCCurve.pdf");


}
  return 0;
  }




