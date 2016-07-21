#include <iostream>
#include <cmath>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH1.h"
#include "TF1.h"
#include "TMinuit.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TStyle.h"
#include "TAttAxis.h"
#include "TFitResult.h"
#include "TLatex.h"
#include <fstream>
#include <TSystem.h>
#include "TLegend.h"
#include "TGaxis.h"


void measure_os_ss(){

	/*TFile *file1 = TFile::Open("/srv/localstage/amd12/Feb12-WithDphi/SingleMuon-2015D_mt_2015.root");
	TTree *tree = dynamic_cast<TTree*>(file1->Get("ntuple"));
    
    double iso_1, iso_2;
    bool os;
    tree->SetBranchAddress("iso_1",&iso_1);
    tree->SetBranchAddress("iso_2",&iso_2);
    tree->SetBranchAddress("os",&os);

    TH1F * iso_os = new TH1F("iso_os","iso_os",20,0,0.5);
	TH1F * iso_ss = new TH1F("iso_ss","iso_ss",20,0,0.5);
    
    for (int i = 0; i < tree->GetEntries(); ++i) {
       tree->GetEntry(i); 
       if(os) iso_os->Fill(iso_1); 
       else iso_ss->Fill(iso_1); 
    }

	TCanvas canvas("canvas");
    iso_os->Draw();
    canvas.SaveAs("os.png");
    iso_ss->Draw();
    canvas.SaveAs("ss.png");
    iso_os->Divide(iso_ss);
    iso_os->Draw();
    canvas.SaveAs("os_ss.png");*/

	TFile *file1 = TFile::Open("datacard_iso_1_btag_mt_2015_OS.root");
	TFile *file2 = TFile::Open("datacard_iso_1_btag_mt_2015_SS.root");

    TH1F * data_os = new TH1F("data_os","data_os",20,0,0.5);
	TH1F * data_ss = new TH1F("data_ss","data_ss",20,0,0.5);
    TH1F * W_os = new TH1F("W_os","W_os",20,0,0.5);
	TH1F * W_ss = new TH1F("W_ss","W_ss",20,0,0.5);
    TH1F * VV_os = new TH1F("VV_os","VV_os",20,0,0.5);
	TH1F * VV_ss = new TH1F("VV_ss","VV_ss",20,0,0.5);
    TH1F * ZTT_os = new TH1F("ZTT_os","ZTT_os",20,0,0.5);
	TH1F * ZTT_ss = new TH1F("ZTT_ss","ZTT_ss",20,0,0.5);
    TH1F * TT_os = new TH1F("TT_os","TT_os",20,0,0.5);
	TH1F * TT_ss = new TH1F("TT_ss","TT_ss",20,0,0.5);
    TH1F * ZLL_os = new TH1F("ZLL_os","ZLL_os",20,0,0.5);
	TH1F * ZLL_ss = new TH1F("ZLL_ss","ZLL_ss",20,0,0.5);
    
    data_os = (TH1F*)file1->Get("mt_btag/data_obs"); 
    data_ss = (TH1F*)file2->Get("mt_btag/data_obs");
    W_os = (TH1F*)file1->Get("mt_btag/W"); 
    W_ss = (TH1F*)file2->Get("mt_btag/W");
    TT_os = (TH1F*)file1->Get("mt_btag/TT"); 
    TT_ss = (TH1F*)file2->Get("mt_btag/TT");
    ZTT_os = (TH1F*)file1->Get("mt_btag/ZTT"); 
    ZTT_ss = (TH1F*)file2->Get("mt_btag/ZTT");
    ZLL_os = (TH1F*)file1->Get("mt_btag/ZLL"); 
    ZLL_ss = (TH1F*)file2->Get("mt_btag/ZLL");
    VV_os = (TH1F*)file1->Get("mt_btag/VV"); 
    VV_ss = (TH1F*)file2->Get("mt_btag/VV");
	
    data_os->Add(W_os,-1);
    data_ss->Add(W_ss,-1);
    data_os->Add(TT_os,-1);
    data_ss->Add(TT_ss,-1);
    data_os->Add(ZTT_os,-1);
    data_ss->Add(ZTT_ss,-1);
    data_os->Add(ZLL_os,-1);
    data_ss->Add(ZLL_ss,-1);
    data_os->Add(VV_os,-1);
    data_ss->Add(VV_ss,-1);
    
    TH1F * data_os_ss = new TH1F("data_os_ss","data_os_ss",20,0,0.5);
    
    data_os_ss->Divide(data_os,data_ss);
	
    
    TCanvas canvas("canvas");
    data_os->GetYaxis()->SetTitle("OS Events");
    data_os->GetXaxis()->SetTitle("Muon Isolation");
    data_os->SetStats(0);
    data_os->SetTitle(0);
    data_os->Draw();
    canvas.SaveAs("os.png");
    data_ss->GetYaxis()->SetTitle("SS Events");
    data_ss->GetXaxis()->SetTitle("Muon Isolation");
    data_ss->SetStats(0);
    data_ss->SetTitle(0);
    data_ss->Draw();
    canvas.SaveAs("ss.png");
    data_os_ss->GetYaxis()->SetTitle("OS/SS Ratio");
    data_os_ss->GetXaxis()->SetTitle("Muon Isolation");
    data_os_ss->GetXaxis()->SetTitleColor(1);
    data_os_ss->GetXaxis()->SetTitleSize(0.045);
    data_os_ss->GetYaxis()->SetTitleSize(0.045);
    data_os_ss->SetStats(0);
    data_os_ss->SetTitle(0);
    
    data_os_ss->Draw();
    TF1 *f1 = new TF1("f1","pol1",0.2,0.5);
    f1->SetLineColor(kBlue);
    TFitResultPtr fit_result = data_os_ss->Fit("f1","RS");
    f1->Draw("same");
    //data_os_ss->Draw();
    f1->SetRange(0,0.5);
    f1->SetLineColor(kBlue);
    f1->Draw("same");
    std::cout << "OS/SS factor at isolation 0.05: " << f1->Eval(0.05) << std::endl; 
    double os_norm = data_os->Integral(data_os->FindBin(0.1),data_os->FindBin(0.2));
    double ss_norm = data_ss->Integral(data_ss->FindBin(0.1),data_ss->FindBin(0.2));
    std::cout << "OS/SS factor integrated over 0.2-0.5: " << os_norm/ss_norm << " +/- " << (os_norm*sqrt(pow((sqrt(os_norm)/os_norm),2) + pow((sqrt(ss_norm)/ss_norm),2)))/ss_norm<< std::endl; 
    
    double x[1] = { 0.05};
    double err[1];
    fit_result->GetConfidenceIntervals(1,1,1,x,err,0.683,false);
    std::cout << err[0] << std::endl;

    canvas.SaveAs("os_ss.png");

}
