#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/SimplePlots.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include <map>
//#include "CommonTools/Utils/interface/TFileDirectory.h"
#include <boost/algorithm/string.hpp>
#include "TDirectory.h"
#include "TFile.h"

namespace ic{

  SimplePlots::SimplePlots(std::string name) : LTModule(name){
  };

  SimplePlots::~SimplePlots(){ ;};

  int SimplePlots::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"Signal MC set is: "<<sigset_<<std::endl;
    std::cout<<"QCD MC set is: "<<qcdset_<<std::endl;
    std::cout<<"EWK MC sets are:" ;
    for (unsigned i(0); i<ewkset_.size();++i){
      std::cout << " " << ewkset_[i] ;
    }
    std::cout << std::endl;
    std::cout<<"Data set is: "<<dataset_<<std::endl;
    std::cout<<"Base selection is: "<<basesel_<<std::endl;
    std::cout<<"Extra selection is: "<<cat_<<std::endl;
    return 0;
  };


  TH1F * SimplePlots::make_histogram(TH1F aHist,
				     const std::vector<std::string> & strs)
  {
    std::string xaxis = strs[0];
    //    std::string unit = strs[2];
    int nbins = 0;
    std::istringstream(strs[1])>>nbins;
    float xmin = 0;
    std::istringstream(strs[2])>>xmin;
    float xmax = 0;
    std::istringstream(strs[3])>>xmax;
    std::string lUnit = "";//unit;
    //if (lUnit.size() > 0) lUnit = " ("+lUnit+")";
    TH1F *hTmp = new TH1F(aHist);
    hTmp->SetTitle("");
    //unsigned rebin = nbins/hTmp->GetNbinsX();
    //hTmp->Rebin(rebin>1?rebin:1);
    hTmp->GetXaxis()->SetTitle((xaxis+lUnit).c_str());
    //hTmp->GetXaxis()->SetRangeUser(xmin,xmax);
    return hTmp;
  }

  int SimplePlots::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;
    TFile* file=fs_;

    TDirectory*  dir=file->mkdir(module_name_.c_str());
    dir->cd();

    for(unsigned ishape=0;ishape<shapes_.size();ishape++){
      std::vector<std::string> strs;
      boost::split(strs, shapes_[ishape], boost::is_any_of("(,)"));
      std::cout << " -- Found " << strs.size() << " strings." << std::endl;
      if (strs.size()<4) {
	std::cout << " Wrong length of string given for shape: should be variable_name(nbins,xmin,xmax)." << std::endl;
	return 1;
      }
      std::string dirname=strs[0];
      std::cout<<dirname<<std::endl;
      TDirectory*  subdir=dir->mkdir(dirname.c_str());
      subdir->cd();

 
      //Get Shapes
      std::cout<<"  Getting signal MC shape"<<std::endl;
      TH1F * sigshape = 
	make_histogram(filemanager->GetSetShape(sigset_,shapes_[ishape],basesel_,cat_,"total_weight_lepveto",true),strs);
      std::cout<<"  Getting QCD MC shape"<<std::endl;
      TH1F * qcdshape = 
	make_histogram(filemanager->GetSetShape(qcdset_,shapes_[ishape],basesel_,cat_,"total_weight_lepveto",true),strs);
      std::cout<<"  Getting EWK MC shape"<<std::endl;
      TH1F * ewkshape = 
	make_histogram(filemanager->GetSetsShape(ewkset_,shapes_[ishape],basesel_,cat_,"total_weight_lepveto",true),strs);
      std::cout<<"  Getting DATA shape"<<std::endl;
      TH1F * datashape =
	make_histogram(filemanager->GetSetShape(dataset_,shapes_[ishape],basesel_,cat_,"1",false),strs);

      sigshape->SetName("signal");
      qcdshape->SetName("qcd");
      ewkshape->SetName("ewk");
      datashape->SetName("data");

      //Write to file
      dir->cd();
      sigshape->Write();
      qcdshape->Write();
      ewkshape->Write();
      datashape->Write();

      //Plot and write to file
      TCanvas *c1=new TCanvas("c1","",1);
      bool logy = true;
      c1->SetLogy(logy);

      std::cout << " number of events: " << std::endl
		<< " signal = " << sigshape->Integral()<< std::endl
		<< " data   = " << datashape->Integral()<< std::endl
		<< " ewk    = " << ewkshape->Integral()<< std::endl
		<< " qcd    = " << qcdshape->Integral()<< std::endl
		<< " qcd SF = " << (datashape->Integral()-ewkshape->Integral())/qcdshape->Integral() << std::endl
	;

      qcdshape->Scale((datashape->Integral()-ewkshape->Integral())/qcdshape->Integral());
      qcdshape->Add(ewkshape);
      sigshape->Scale(datashape->Integral()/sigshape->Integral());

      sigshape->SetMarkerStyle(21);
      sigshape->SetMarkerColor(4);
      sigshape->SetLineColor(4);
      sigshape->SetFillColor(4);
      sigshape->SetFillStyle(3004);
      qcdshape->SetMarkerStyle(22);
      qcdshape->SetMarkerColor(5);
      qcdshape->SetLineColor(5);
      qcdshape->SetFillColor(5);
      ewkshape->SetMarkerStyle(23);
      ewkshape->SetMarkerColor(6);
      ewkshape->SetLineColor(6);
      ewkshape->SetFillColor(6);
      ewkshape->SetFillStyle(3005);
      datashape->SetLineColor(1);
      datashape->SetMarkerColor(1);
      datashape->SetMarkerStyle(2);

      datashape->GetYaxis()->SetTitle("Events");
      if (datashape->GetMinimum() > qcdshape->GetMinimum()) datashape->SetMinimum(qcdshape->GetMinimum());
      if (datashape->GetMaximum() < ewkshape->GetMaximum()) datashape->SetMaximum(ewkshape->GetMaximum());
      if (datashape->GetMaximum() < sigshape->GetMaximum()) datashape->SetMaximum(sigshape->GetMaximum());
      if (logy==true && datashape->GetMinimum()==0) datashape->SetMinimum(0.1);
      

      gStyle->SetOptStat(0);

      TLegend* leg =new TLegend(0.67,0.75,1,1);
      leg->SetFillColor(10);
      leg->AddEntry(datashape,"Data","P");
      leg->AddEntry(sigshape,"VBF m_{H}=125 GeV","l");
      leg->AddEntry(qcdshape,"QCD","F");
      leg->AddEntry(ewkshape,"V+Top+VV","F");
      datashape->Draw("PE");
      qcdshape->Draw("histsame");
      ewkshape->Draw("histsame");
      sigshape->Draw("Lsame");
      datashape->Draw("PEsame");
      datashape->Draw("axissame");
      leg->Draw("same");
      c1->Update();

      std::ostringstream outputname;
      outputname << "PLOTS/" << module_name_ << "_" << dirname ;
      if (logy) outputname << "_log";
      outputname << ".pdf";
      c1->Print(outputname.str().c_str());
      c1->Write();

      datashape->Delete();
      sigshape->Delete();
      qcdshape->Delete();
      ewkshape->Delete();

    }



    return 0;
  };

}
