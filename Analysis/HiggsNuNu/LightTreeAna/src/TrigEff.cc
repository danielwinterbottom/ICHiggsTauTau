#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/TrigEff.h"
#include <iostream>
#include <sstream>
#include "TH1F.h"
#include "TF1.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TDirectory.h"
#include <map>

namespace ic{

  TrigEff::TrigEff(std::string name) : LTModule(name){
    std::vector<std::string> defaulttrigger;
    defaulttrigger.push_back("pass_sigtrigger==1");
    trigger_=defaulttrigger;
    numweight_="weight_nolep";
    denweight_="weight_nolep";
    shape_=("jet2_pt(200,0.,1000.)");
    shapename_="";
    dirname_="";
    namestr_="";
    do_fit_=false;
  };

  TrigEff::~TrigEff(){ ;};

  int TrigEff::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"  Data sets are: "<<std::endl;
      for(unsigned iset=0;iset<dataset_.size();iset++){
	std::cout<<"    "<<dataset_[iset]<<std::endl;
      }
    std::cout<<"  Base selection is: "<<basesel_<<std::endl;
    std::cout<<"  Extra selection is: "<<cat_<<std::endl;
    std::cout<<"  Num weight is: "<<numweight_<<std::endl;
    std::cout<<"  Den weight is: "<<denweight_<<std::endl;
    return 0;
  };

  int TrigEff::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;

    TFile *trigweightsout=new TFile("TrigEff2015D_MET1DFit.root","UPDATE");

    TFile *file=fs_;
    TDirectory* dir;
    if(dirname_==""){
      dir=file->mkdir(this->module_name().c_str());
    }
    else if(!fs_->GetDirectory(dirname_.c_str())){
      dir=file->mkdir(dirname_.c_str());
    }
    else{
      dir=file->GetDirectory(dirname_.c_str());
    }
    dir->cd();
    
    //Prepare for making plots
    std::string histname;
    if(shapename_==""){
      std::vector<std::string> strs;
      boost::split(strs, shape_, boost::is_any_of("("));
      histname=strs[0];
    }
    else{
      histname=shapename_;
    }
    TCanvas* c1= new TCanvas("canvas","canvas");
    c1->cd();
    c1->SetGrid(1,1);
    c1->SetName(histname.c_str());
    TPad* upper = new TPad("upper","pad",0, 0 ,1 ,1);
    c1->SetBottomMargin(0.15);
    upper->SetBottomMargin(0.15);
    upper->Draw();
    upper->cd();

    double legleft=0.5;
    double legright=0.89;
    double legbottom=0.3+((10-2)*(0.89-0.3)/10);
    if(legbottom<0.3||legbottom>0.89)legbottom=0.3;
    TLegend* leg=new TLegend(legleft,legbottom,legright,0.89);
    leg->SetName("thelegend");
    leg->SetTextSize(0.06);
    leg->SetFillStyle(0);
    leg->SetLineColor(10);
    

    //Get Shapes for Data
    std::cout<<"  Getting shape"<<std::endl;
    TH1F  datashapenotrig = filemanager->GetSetsShape(dataset_,shape_,basesel_,cat_,denweight_,false);  
    datashapenotrig.Sumw2();
    for(unsigned iTrigger=0;iTrigger<trigger_.size();iTrigger++){
      TH1F  datashapewithtrig = filemanager->GetSetsShape(dataset_,shape_,basesel_,cat_+"&&"+trigger_[iTrigger],numweight_,false);
      datashapewithtrig.Sumw2();
      //TH1F* efficiency=(TH1F*)datashapenotrig.Clone();
      //efficiency->Divide(&datashapewithtrig,&datashapenotrig,1,1,"B");
      
      //Check one histo is not larger than the other
      // for(int ibin=0;ibin<datashapenotrig.GetNbinsX()+1;ibin++){
      //   if(datashapewithtrig.GetBinContent(ibin)>datashapenotrig.GetBinContent(ibin))datashapewithtrig.SetBinContent(ibin,datashapenotrig.GetBinContent(ibin));
      // }
      
      TGraphAsymmErrors* efficiency=new TGraphAsymmErrors();
      efficiency->Divide(&datashapewithtrig,&datashapenotrig,"cl=0.683 b(1,1) mode");
      dir->cd();
      std::cout<<"  nevents before trigger: "<<Integral(&datashapenotrig)<<"+-"<<Error(&datashapenotrig)<<std::endl;
      std::cout<<"  nevents after trigger: "<<Integral(&datashapewithtrig)<<"+-"<<Error(&datashapewithtrig)<<std::endl;

      if(iTrigger==0){
	efficiency->SetName(histname.c_str());
	datashapenotrig.SetName((histname+"notrig").c_str());
	datashapewithtrig.SetName((histname+"withtrig").c_str());
	datashapenotrig.Write();
	datashapewithtrig.Write();
	efficiency->Write();
      

	//Fit efficiency
	TF1* fitresult;
	if(do_fit_){
	  std::string varlow=shape_.substr(shape_.find("(")+1,shape_.find(")")-1);  
	  std::string varhigh=varlow;
	  varlow=varlow.substr(varlow.find(",")+1);
	  varlow=varlow.substr(0,varlow.find(","));
	  varhigh=varhigh.substr(varhigh.find(",")+1);
	  varhigh=varhigh.substr(varhigh.find(",")+1);
	  varhigh=varhigh.substr(0,varhigh.find(")"));
	  
	  double rangelow, rangehigh;
	  std::stringstream convertlow(varlow);
	  if( !(convertlow >> rangelow) ) throw;
	  std::stringstream converthigh(varhigh);
	  if( !(converthigh >> rangehigh) ) throw;
	  TF1 *func = new TF1("erf","[0]*0.5*(1+TMath::Erf((x-[1])/(sqrt([2]))))",rangelow,rangehigh);
	  //TF1 *func = new TF1("erf","[0]*0.5*(1+TMath::Erf((x-[1])/(sqrt([2]))))+[3]*0.5*(1+TMath::Erf((x-[4])/(sqrt([5]))))",rangelow,rangehigh);
	  func->SetParameters(1,100,10000);
	  func->SetParNames("Max","Turn On","Sigma");
	  func->SetParLimits(0,0.,1.);
	  func->SetParLimits(1,-10000,10000);
	  func->SetParLimits(2,0,100000);
	  // func->SetParameters(1,100,10000,0.1,250,10000);
	  // func->SetParNames("Max1","Turn On1","Sigma1","Max2","Turn On2","Sigma2");
	  // func->SetParLimits(0,0.,1.);
	  // func->SetParLimits(1,-10000,10000);
	  // func->SetParLimits(2,0,100000);
	  // func->SetParLimits(3,0.,1.);
	  // func->SetParLimits(4,-10000,10000);
	  // func->SetParLimits(5,0,100000);
	  
	  
	  efficiency->Fit("erf","R");
	  
	  fitresult=efficiency->GetFunction("erf");
	  trigweightsout->cd();
	  fitresult->Write((namestr_+"eff").c_str());
	  dir->cd();
	}
      
    
	efficiency->GetYaxis()->SetRangeUser(0.,1.35);
	efficiency->Draw("AP");
	DrawCMSLogoTest(upper,"CMS","preliminary",10);
	//efficiency->Draw(elements_[iElement].drawopts().c_str());
	//efficiency->GetYaxis()->SetRangeUser(0.,shapes_[iShape].axisrangemultiplier()*(ymax+sqrt(ymax)+1));
	//efficiency->Draw(elements_[iElement].drawopts().c_str());
	//c1->Update();                                                                                                                                                                                                                
	efficiency->SetMarkerColor(1);
	efficiency->SetLineColor(1);
	efficiency->SetLineWidth(2);
	efficiency->SetMarkerStyle(20);
	efficiency->SetMarkerSize(1.1);
	efficiency->GetYaxis()->SetLabelSize(0.06);
	efficiency->GetYaxis()->SetTitleSize(0.06);
	efficiency->GetYaxis()->SetTitleFont(62);
	efficiency->GetYaxis()->SetTitleOffset(0.67);
	efficiency->GetXaxis()->SetLabelSize(0.06);
	efficiency->GetXaxis()->SetTitleFont(62);
	efficiency->GetXaxis()->SetTitleSize(0.08);
	efficiency->GetXaxis()->SetTitleOffset(0.70);
	std::string xtitle;
	xtitle=histtitle_.substr(0,histtitle_.find(";")+1);
	xtitle=xtitle.substr(0,xtitle.find(";"));
	efficiency->GetXaxis()->SetTitle(xtitle.c_str());
	std::string ytitle;
	ytitle=histtitle_.substr(histtitle_.find(";")+1);
	ytitle=ytitle.substr(ytitle.find(";")+1);
	ytitle=ytitle.substr(0,ytitle.find(";"));
	efficiency->GetYaxis()->SetTitle(ytitle.c_str());
	
	//SETUP AND DRAW THE LEGEND                                                                                                                         
	if(triggername_.size()!=0)leg->AddEntry(efficiency,triggername_[iTrigger].c_str(),"ep");
	
      }
      else{
	efficiency->SetMarkerColor(iTrigger+1);
	efficiency->SetLineColor(iTrigger+1);
	efficiency->SetLineWidth(2);
	efficiency->SetMarkerStyle(20);
	efficiency->SetMarkerSize(1.1);
	efficiency->Draw("Psame");
	if(triggername_.size()!=0)leg->AddEntry(efficiency,triggername_[iTrigger].c_str(),"ep");
      }
    }
    if(triggername_.size()!=0){
      leg->Draw("same");
      c1->Update();                                                                                                            
    }     
    //leg->Defficiencyte();                                                                                                                                                                                                                       lg

    //save as PDF                                                                                                                                                                                                                          
    //c1->Update();                                                                                                                                                                                                                        
    std::ostringstream lsave;
    std::ostringstream lsavepng;
    std::string tmpstr = file->GetName();
    tmpstr.erase(std::string(file->GetName()).find(".root"),5);
    tmpstr=tmpstr;
    lsave << tmpstr ;
    lsave << ".pdf" ;
    lsave << "[";
    c1->Print(lsave.str().c_str());//open the file                                                                                                                                                                                       
    lsave.str("");//reset for adding the first plot                                                                                                                                                                                      
    lsave << tmpstr ;
    lsave << ".pdf" ;

    c1->Print(lsave.str().c_str());
    lsave << "]";
    c1->Print(lsave.str().c_str());//close the file                                                                                                                                                                                      
    

    // lsave.str("");
    // lsave << tmpstr << "_" << c1->GetName() << ".pdf" ;
    // c1->Print((lsave.str()).c_str());
    // lsavepng.str("");
    // lsavepng << tmpstr << "_" << c1->GetName() << ".png" ;
    // c1->Print((lsavepng.str()).c_str());

    c1->SaveAs((tmpstr+namestr_+"_"+c1->GetName()+".pdf").c_str());
    c1->SaveAs((tmpstr+namestr_+"_"+c1->GetName()+".png").c_str());

    //WRITE TO FILE                                                                                                                                                                                                                        
    dir->cd();
    c1->Write();
    //upper->Close();                                                                                                                                                                                                                      
    //lower->Close();                                                                                                                                                                                                                      
    c1->Clear("D");
    //      upper->Clear();                                                                                                                                                                                                                
    //      lower->Clear();                                                                                                                                                                                                                
  
    dir->Close();
    trigweightsout->Close();

    return 0;
  };

}
