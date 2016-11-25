#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/ClosureTest.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TVectorD.h"
#include "TLegend.h"
#include "TStyle.h"
#include <cstdio>
#include <map>
#include "boost/lexical_cast.hpp"

namespace ic{

  ClosureTest::ClosureTest(std::string name) : LTModule(name){
    sigmcweight_="total_weight_lepveto";
    contmcweight_="total_weight_leptight";
    contmczweight_="weight_nolep";
    dataweight_="weight_nolep";
    dataextrasel_="";
    sigmcextrasel_="";
    contmcextrasel_="";
    contbkgextrasel_="";
    sigcontextrafactor_=1.;
    shape_="jet2_pt";
    binning_={45.,100.,150.,200.,1000.};
    dirname_="";
    do_latex_=false;
    additionalsyst_=0;
  };

  ClosureTest::~ClosureTest(){ ;};

  int ClosureTest::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"  Signal MC set is: "<<sigmcset_<<std::endl;
    std::cout<<"  Control MC set is: "<<contmcset_<<std::endl;
    std::cout<<"  Data set is: "<<dataset_<<std::endl;
    std::cout<<"  Base selection is: "<<basesel_<<std::endl;
    std::cout<<"  Signal extra selection is: "<<sigcat_<<std::endl;
    std::cout<<"  Control extra selection is: "<<contcat_<<std::endl;
    std::cout<<"  Signal MC weight is: "<<sigmcweight_<<std::endl;
    std::cout<<"  Control MC weight is: "<<contmcweight_<<std::endl;
    std::cout<<"  Data weight is: "<<dataweight_<<std::endl;
    return 0;
  };

  int ClosureTest::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;
    //sort out puweighting
    TFile *file=fs_;
    TDirectory* dir;
    if(dirname_==""){
      dir=file->mkdir(sigmcset_.c_str());
    }
    else if(!fs_->GetDirectory(dirname_.c_str())){
      dir=file->mkdir(dirname_.c_str());
    }
    else{
      dir=fs_->GetDirectory(dirname_.c_str());
    }
    dir->cd();

    double binarray[binning_.size()];
    for(unsigned iBin=0;iBin<binning_.size();iBin++){
      binarray[iBin]=binning_[iBin];
    }

    TH1F* closureRawMC=new TH1F("closureRawMC","",binning_.size()-1,binarray);
    TH1F* closureDDMC=new TH1F("closureDDMC","",binning_.size()-1,binarray);
    TH1F* closureData=new TH1F("closureData","",binning_.size()-1,binarray);
    TH1F* closureDDMCRatioSyst=new TH1F("closureDDMCRatioSyst","",binning_.size()-1,binarray);

    //LOOP OVER BINNING
    for(unsigned iBin=0;iBin<binning_.size()-1;iBin++){
      std::string varinput=shape_+"(1,"+boost::lexical_cast<std::string>(binning_[iBin])+","+boost::lexical_cast<std::string>(binning_[iBin+1])+")";
      //Get Shapes for NSMC, NCMC, NCData and NCBkg    
      std::cout<<"  Getting control MC shape"<<std::endl;
      TH1F  contmcshape = filemanager->GetSetShape(contmcset_,varinput,basesel_,(contcat_+contmcextrasel_),contmcweight_,false);
      std::cout<<"  Getting control MC Backgrounds shape"<<std::endl;
      TH1F contbkgshape;
      TH1F sigbkgshape;
      bool firstbkg=true;
      //IF NO DIRS TO GET DATA DRIVEN WEIGHTS, GET SETS SHAPE
      double extrancbkgerr=0;
      double extransbkgerr=0;
      if(contbkgextrafactordir_.size()==0){
	contbkgshape= filemanager->GetSetsShape(contbkgset_,varinput,basesel_,(contcat_+contbkgextrasel_),contmcweight_,false);
      }
      //WEIGHT INDIVIDUAL SHAPES BY DATA DRIVEN WEIGHTS
      else{
	if(contbkgextrafactordir_.size()==contbkgset_.size()){
	  for(unsigned iBkg=0;iBkg<contbkgset_.size();iBkg++){
	    double extrafactor=1;
	    double extrafactorfracerr=0;
	    TDirectory* extrafactordir;
	    TVectorD *ddweight;
	    TVectorD *normerrs;
	    if(contbkgextrafactordir_[iBkg]==""){
	      extrafactor=1;
	    }
	    else if(!fs_->GetDirectory(contbkgextrafactordir_[iBkg].c_str())){
	      std::cout<<"Directory "<<contbkgextrafactordir_[iBkg]<<" does not exist, exiting"<<std::endl;
	      return 1;
	    }
	    else{
	      extrafactordir=fs_->GetDirectory(contbkgextrafactordir_[iBkg].c_str());
	      ddweight = (TVectorD*)extrafactordir->Get("ddweight");
	      normerrs = (TVectorD*)extrafactordir->Get("normerrs");
	      if(contbkgisz_.size()==contbkgset_.size()){
		if(contbkgisz_[iBkg]==0){
		  extrafactor=(*ddweight)[0];
		  extrafactorfracerr=sqrt(pow((*normerrs)[0],2)+pow((*normerrs)[1],2));
		}
		if(contbkgisz_[iBkg]==1){
		  extrafactor=(*ddweight)[0];//EWK WEIGHT
		  extrafactorfracerr=sqrt(pow((*normerrs)[0],2)+pow((*normerrs)[2],2));
		}
		if(contbkgisz_[iBkg]==2){
		  extrafactor=(*ddweight)[1];//QCD WEIGHT
		  extrafactorfracerr=sqrt(pow((*normerrs)[0],2)+pow((*normerrs)[3],2));
		}
	      }
	    }
	    TH1F nextbkg;
	    TH1F nextsigbkg;
	    //SPECIAL CASE FOR Z BKG
	    if(contbkgisz_.size()==contbkgset_.size()){
	      if(contbkgisz_[iBkg]!=0){
		//GET Z SHAPE AND WEIGHT IT
		nextbkg=filemanager->GetSetShape(contbkgset_[iBkg],varinput,basesel_,(zcontcat_+contbkgextrasel_),contmczweight_+"*"+boost::lexical_cast<std::string>(extrafactor),false);
		nextsigbkg=filemanager->GetSetShape(contbkgset_[iBkg],varinput,basesel_,sigcat_+sigmcextrasel_,sigmcweight_+"*"+boost::lexical_cast<std::string>(extrafactor),false);
	      }
	      else{
		nextbkg=filemanager->GetSetShape(contbkgset_[iBkg],varinput,basesel_,(contcat_+contbkgextrasel_),contmcweight_+"*"+boost::lexical_cast<std::string>(extrafactor),false);
		nextsigbkg=filemanager->GetSetShape(contbkgset_[iBkg],varinput,basesel_,sigcat_+sigmcextrasel_,sigmcweight_,false);
	      }
	    }
	    else{
	      nextbkg=filemanager->GetSetShape(contbkgset_[iBkg],varinput,basesel_,(contcat_+contbkgextrasel_),contmcweight_+"*"+boost::lexical_cast<std::string>(extrafactor),false);
	      nextsigbkg=filemanager->GetSetShape(contbkgset_[iBkg],varinput,basesel_,(sigcat_+sigmcextrasel_),sigmcweight_+"*"+boost::lexical_cast<std::string>(extrafactor),false);
	    }
	    //std::cout<<contbkgset_[iBkg]<<" "<<Integral(&nextbkg)<<" "<<extrafactorfracerr<<std::endl;
	    double thisbkgextrancbkgerr=Integral(&nextbkg)*extrafactorfracerr;
	    double thisbkgextransbkgerr=Integral(&nextsigbkg)*extrafactorfracerr;
	    extrancbkgerr=sqrt(pow(extrancbkgerr,2)+pow(thisbkgextrancbkgerr,2));
	    extransbkgerr=sqrt(pow(extransbkgerr,2)+pow(thisbkgextransbkgerr,2));
	    
	    //ADD HISTOGRAMS TOGETHER
	    if(firstbkg){
	      contbkgshape=nextbkg;
	      sigbkgshape=nextsigbkg;
	      firstbkg=false;
	    }
	    else{
	      contbkgshape.Add(&nextbkg);
	      sigbkgshape.Add(&nextsigbkg);
	    }
	  }
	}
	else{
	  std::cout<<"Extra factor dir vector must be same size as bkg set vector expect errors!"<<std::endl;
	  return 1;
	}
      }
      std::cout<<"  Getting control Data shape"<<std::endl;
      TH1F  contdatashape = filemanager->GetSetShape(dataset_,varinput,basesel_,contcat_+dataextrasel_,dataweight_,false);
      TH1F  sigdatashape = filemanager->GetSetShape(dataset_,varinput,basesel_,sigcat_+dataextrasel_,dataweight_,false);



      double nsdata= Integral(&sigdatashape,1,1);
      double nsdataerr= Error(&sigdatashape,1,1);
      double nsbkg= Integral(&sigbkgshape,1,1);
      double nsbkgerr= Error(&sigbkgshape,1,1);

      closureData->SetBinContent(iBin+1,nsdata-nsbkg);
      closureData->SetBinError(iBin+1,sqrt(pow(nsdataerr,2)+pow(nsbkgerr,2)));

      //Integrate over shape to get number in each region
      double ncmc = Integral(&contmcshape,1,1);
      double ncdata = Integral(&contdatashape,1,1);
      double ncbkg = Integral(&contbkgshape,1,1);
      double ncmcerr = Error(&contmcshape,1,1);
//      double ncdataerr = Error(&contdatashape,1,1);
      double ncbkgerr = Error(&contbkgshape,1,1);
      
      ncbkgerr=sqrt(pow(ncbkgerr,2)+pow(extrancbkgerr,2));//Add error from other data driven weight extrapolation factors
      
      //std::cout<<"  ncmc: "<<ncmc<<"+-"<<ncmcerr<<", ncdata: "<<ncdata<<"+-"<<ncdataerr<<", ncbkg: "<<ncbkg<<"+-"<<ncbkgerr<<std::endl;
      
      //Calculate weight
      double weight=(ncdata-ncbkg)/ncmc;
      //double weighterrdatastatfrac=ncdataerr/(ncdata-ncbkg);
      double weighterrmcstatfrac=sqrt(((ncbkgerr/(ncdata-ncbkg))*(ncbkgerr/(ncdata-ncbkg)))+((ncmcerr/ncmc)*(ncmcerr/ncmc)));
      //std::cout<<"  weight: "<<weight<<"+-"<<weight*weighterrdatastatfrac<<"(data stat.)+-"<<weight*weighterrmcstatfrac<<"(MC stat.)"<<std::endl;
      std::cout<<"  Getting signal MC shapes"<<std::endl;
      TH1F  sigmcshape = filemanager->GetSetShape(sigmcset_,varinput,basesel_,sigcat_+sigmcextrasel_,sigmcweight_,false);
      dir->cd();
      std::string histname;
      if(shapename_==""){
	std::vector<std::string> strs;
	boost::split(strs, shape_, boost::is_any_of("("));
	histname=strs[0];
      }
      else{
	histname=shapename_;
      }
      sigmcshape.SetName(histname.c_str());
      double unnormnsmc=Integral(&sigmcshape,1,1);
      double unnormnsmcerr=Error(&sigmcshape,1,1);
      std::cout<<"RawMC: "<<unnormnsmc<<"+-"<<unnormnsmcerr<<std::endl;
      closureRawMC->SetBinContent(iBin+1,unnormnsmc);
      closureRawMC->SetBinError(iBin+1,unnormnsmcerr);
      sigmcshape.Scale(weight*sigcontextrafactor_);
      double nsmc=Integral(&sigmcshape,1,1);
      double nsmcerr=Error(&sigmcshape,1,1);
      double weightednsmcstatfrac=sqrt(((nsmcerr/nsmc)*(nsmcerr/nsmc))+(weighterrmcstatfrac*weighterrmcstatfrac));
      //!!MAKE SURE  TO TAKE NSMC ERROR OUT OF NORMALISATION ERROR AS SHOULD BE DONE BIN BY BIN: ONLY IF DOING SHAPE ANALYSIS
      std::cout<<"DDMC: "<<nsmc<<"+-"<<weightednsmcstatfrac*nsmc<<std::endl;
      closureDDMC->SetBinContent(iBin+1,nsmc);
      closureDDMC->SetBinError(iBin+1,weightednsmcstatfrac*nsmc);

      
      std::string baseinfolder=filemanager->infolder();
      filemanager->set_infolder(baseinfolder+"/JESUP");
      TH1F  jesupsigmcshape = filemanager->GetSetShape(sigmcset_,varinput,basesel_,sigcat_+sigmcextrasel_,sigmcweight_,false);
      jesupsigmcshape.Scale(weight*sigcontextrafactor_);
      filemanager->set_infolder(baseinfolder+"/JESDOWN");
      TH1F  jesdownsigmcshape = filemanager->GetSetShape(sigmcset_,varinput,basesel_,sigcat_+sigmcextrasel_,sigmcweight_,false);
      jesdownsigmcshape.Scale(weight*sigcontextrafactor_);
      filemanager->set_infolder(baseinfolder+"/JERBETTER");
      TH1F  jerbettersigmcshape = filemanager->GetSetShape(sigmcset_,varinput,basesel_,sigcat_+sigmcextrasel_,sigmcweight_,false);
      jerbettersigmcshape.Scale(weight*sigcontextrafactor_);
      filemanager->set_infolder(baseinfolder+"/JERWORSE");
      TH1F  jerworsesigmcshape = filemanager->GetSetShape(sigmcset_,varinput,basesel_,sigcat_+sigmcextrasel_,sigmcweight_,false);
      jerworsesigmcshape.Scale(weight*sigcontextrafactor_);

      filemanager->set_infolder(baseinfolder+"/UESUP");
      TH1F  uesupsigmcshape = filemanager->GetSetShape(sigmcset_,varinput,basesel_,sigcat_+sigmcextrasel_,sigmcweight_,false);
      uesupsigmcshape.Scale(weight*sigcontextrafactor_);
      filemanager->set_infolder(baseinfolder+"/UESDOWN");
      TH1F  uesdownsigmcshape = filemanager->GetSetShape(sigmcset_,varinput,basesel_,sigcat_+sigmcextrasel_,sigmcweight_,false);
      uesdownsigmcshape.Scale(weight*sigcontextrafactor_);

      filemanager->set_infolder(baseinfolder+"/ELEEFFUP");
      TH1F  eleeffupsigmcshape = filemanager->GetSetShape(sigmcset_,varinput,basesel_,sigcat_+sigmcextrasel_,sigmcweight_,false);
      eleeffupsigmcshape.Scale(weight*sigcontextrafactor_);
      filemanager->set_infolder(baseinfolder+"/ELEEFFDOWN");
      TH1F  eleeffdownsigmcshape = filemanager->GetSetShape(sigmcset_,varinput,basesel_,sigcat_+sigmcextrasel_,sigmcweight_,false);
      eleeffdownsigmcshape.Scale(weight*sigcontextrafactor_);

      filemanager->set_infolder(baseinfolder+"/MUEFFUP");
      TH1F  mueffupsigmcshape = filemanager->GetSetShape(sigmcset_,varinput,basesel_,sigcat_+sigmcextrasel_,sigmcweight_,false);
      mueffupsigmcshape.Scale(weight*sigcontextrafactor_);
      filemanager->set_infolder(baseinfolder+"/MUEFFDOWN");
      TH1F  mueffdownsigmcshape = filemanager->GetSetShape(sigmcset_,varinput,basesel_,sigcat_+sigmcextrasel_,sigmcweight_,false);
      mueffdownsigmcshape.Scale(weight*sigcontextrafactor_);

      filemanager->set_infolder(baseinfolder);
      TH1F  puupsigmcshape = filemanager->GetSetShape(sigmcset_,varinput,basesel_,sigcat_+sigmcextrasel_,sigmcweight_+"*puweight_up_scale",false);
      TH1F  pudownsigmcshape = filemanager->GetSetShape(sigmcset_,varinput,basesel_,sigcat_+sigmcextrasel_,sigmcweight_+"*puweight_down_scale",false);

      double ddmcjesup= Integral(&jesupsigmcshape,1,1);
      double ddmcjesdown= Integral(&jesdownsigmcshape,1,1);
      double ddmcjeserr;
      if(fabs(ddmcjesup-nsmc)>fabs(ddmcjesdown-nsmc))ddmcjeserr=fabs(ddmcjesup-nsmc);
      else ddmcjeserr=fabs(ddmcjesdown-nsmc);

      double ddmcjerbetter= Integral(&jerbettersigmcshape,1,1);
      double ddmcjerworse= Integral(&jerworsesigmcshape,1,1);
      double ddmcjererr;
      if(fabs(ddmcjerbetter-nsmc)>fabs(ddmcjerworse-nsmc))ddmcjererr=fabs(ddmcjerbetter-nsmc);
      else ddmcjererr=fabs(ddmcjerworse-nsmc);

      double ddmcuesup= Integral(&uesupsigmcshape,1,1);
      double ddmcuesdown= Integral(&uesdownsigmcshape,1,1);      
      double ddmcueserr;
      if(fabs(ddmcuesup-nsmc)>fabs(ddmcuesdown-nsmc))ddmcueserr=fabs(ddmcuesup-nsmc);
      else ddmcueserr=fabs(ddmcuesdown-nsmc);

      double ddmceleeffup= Integral(&eleeffupsigmcshape,1,1);
      double ddmceleeffdown= Integral(&eleeffdownsigmcshape,1,1);      
      double ddmceleefferr;
      if(fabs(ddmceleeffup-nsmc)>fabs(ddmceleeffdown-nsmc))ddmceleefferr=fabs(ddmceleeffup-nsmc);
      else ddmceleefferr=fabs(ddmceleeffdown-nsmc);

      double ddmcmueffup= Integral(&mueffupsigmcshape,1,1);
      double ddmcmueffdown= Integral(&mueffdownsigmcshape,1,1);      
      double ddmcmuefferr;
      if(fabs(ddmcmueffup-nsmc)>fabs(ddmcmueffdown-nsmc))ddmcmuefferr=fabs(ddmcmueffup-nsmc);
      else ddmcmuefferr=fabs(ddmcmueffdown-nsmc);

      double ddmcpuup= Integral(&puupsigmcshape,1,1);
      double ddmcpudown= Integral(&pudownsigmcshape,1,1);      
      double ddmcpuerr;
      if(fabs(ddmcpuup-nsmc)>fabs(ddmcpudown-nsmc))ddmcpuerr=fabs(ddmcpuup-nsmc);
      else ddmcpuerr=fabs(ddmcpudown-nsmc);
      
      double DDMCsyst=sqrt(pow(ddmcjeserr,2)+pow(ddmcjererr,2)+pow(ddmcueserr,2)+pow(ddmceleefferr,2)+pow(ddmcmuefferr,2)+pow(ddmcpuerr,2)+pow(additionalsyst_*nsmc,2));
      closureDDMCRatioSyst->SetBinContent(iBin+1,1);
      closureDDMCRatioSyst->SetBinError(iBin+1,DDMCsyst/nsmc);

    }
    dir->cd();

    TCanvas *c1=new TCanvas();
    TPad* upper = nullptr;
    TPad* lower = nullptr;
    upper = new TPad("upper","pad",0, 0.26 ,1 ,1);
    lower = new TPad("lower","pad",0, 0   ,1 ,0.26);
    upper->SetBottomMargin(0.02);
    upper->Draw();
    upper->cd();
    gStyle->SetOptStat(0);
    closureRawMC->SetMarkerColor(2);
    closureRawMC->SetMarkerSize(2);
    closureDDMC->SetMarkerColor(3);
    closureDDMC->SetMarkerSize(2);
    closureData->SetMarkerColor(4);
    closureData->SetMarkerSize(2);
    closureRawMC->SetLineColor(2);
    closureDDMC->SetLineColor(3);
    closureData->SetLineColor(4);

    TLegend* leg =new TLegend(0.75,0.3,0.89,0.89);
    leg->SetFillStyle(0);
    leg->SetLineColor(10);
    leg->AddEntry(closureRawMC,"Raw MC","lp");
    leg->AddEntry(closureDDMC,"Data Driven","lp");
    leg->AddEntry(closureData,"Data-Bkg","lp");

    closureRawMC->GetXaxis()->SetLabelOffset(999);
    closureRawMC->GetXaxis()->SetLabelSize(0);
    double max=0;
    if(closureRawMC->GetMaximum()>max)max=closureRawMC->GetMaximum();
    if(closureDDMC->GetMaximum()>max)max=closureDDMC->GetMaximum();
    if(closureData->GetMaximum()>max)max=closureData->GetMaximum();
    closureRawMC->GetYaxis()->SetRangeUser(0,1.1*max);
    closureRawMC->Draw();
    closureDDMC->Draw("same");
    closureData->Draw("same");
    leg->Draw("same");

    c1->cd();
     lower->SetTopMargin(0.03);
     lower->SetBottomMargin(0.2);
     lower->Draw();
     lower->cd();
     lower->SetGridy();

     closureDDMCRatioSyst->SetMarkerSize(0);
     closureDDMCRatioSyst->SetMarkerColor(16);
     closureDDMCRatioSyst->SetFillColor(16);
     closureDDMCRatioSyst->SetFillStyle(1000);//3013);                                                                                                  
     closureDDMCRatioSyst->SetLineWidth(1);
     closureDDMCRatioSyst->SetTitle("");
     closureDDMCRatioSyst->SetStats(0);
     closureDDMCRatioSyst->GetXaxis()->SetTitle(shape_.c_str());
     closureDDMCRatioSyst->GetXaxis()->SetLabelSize(0.1);
     closureDDMCRatioSyst->GetYaxis()->SetLabelSize(0.1);
     closureDDMCRatioSyst->GetXaxis()->SetTitleSize(0.1);
     closureDDMCRatioSyst->GetXaxis()->SetTitleOffset(0.8);
     closureDDMCRatioSyst->GetYaxis()->SetTitle("DD/data");
     closureDDMCRatioSyst->GetYaxis()->SetTitleSize(0.1);
     closureDDMCRatioSyst->GetYaxis()->SetTitleOffset(0.3);

     TH1F* ratio;
     closureRawMC->Sumw2();
     closureData->Sumw2();
     ratio=(TH1F*)(closureDDMC->Clone("ratio"));
     ratio->SetLineColor(1);
     ratio->Divide(closureData);

     double ratiomaxrange;
     
     if(ratio->GetMaximum()>(2.0/1.1))ratiomaxrange=ratio->GetMaximum()*1.1;
     else ratiomaxrange=2.0;
     closureDDMCRatioSyst->GetYaxis()->SetRangeUser(0.,ratiomaxrange);


     closureDDMCRatioSyst->Draw("E2");
     ratio->Draw("E1same");
     c1->Update();
    c1->SaveAs(("closure"+shape_+sigmcset_+".pdf").c_str());
    return 0;
  };

}
												  
