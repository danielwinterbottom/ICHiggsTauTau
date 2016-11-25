#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/BkgSubDataNormShape.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TVectorD.h"
#include <cstdio>
#include <map>
#include "boost/lexical_cast.hpp"

namespace ic{

  BkgSubDataNormShape::BkgSubDataNormShape(std::string name) : LTModule(name){
    shapebaseweight_="total_weight_lepveto";
    contmcweight_="total_weight_lepveto";
    contmczweight_="weight_nolep";
    shapemcweight_="total_weight_lepveto";
    shapemczweight_="weight_nolep";
    contmczweight_="weight_nolep";
    contdataweight_="weight_nolep";
    contdataextrasel_="";
    contbkgextrasel_="";
    shapecontextrafactor_=1.;
    std::vector<std::string> shapes;
    shapes.push_back("jet2_pt(200,0.,1000.)");
    shape_=shapes;
    dirname_="";
    do_latex_=false;
  };

  BkgSubDataNormShape::~BkgSubDataNormShape(){ ;};

  int BkgSubDataNormShape::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"  Shape set is: "<<shapebaseset_<<std::endl;
    std::cout<<"  Control data set is: "<<contdataset_<<std::endl;
    std::cout<<"  Base selection is: "<<basesel_<<std::endl;
    std::cout<<"  Shape extra selection is: "<<shapecat_<<std::endl;
    std::cout<<"  Control extra selection is: "<<contcat_<<std::endl;
    std::cout<<"  Shape MC weight is: "<<shapebaseweight_<<std::endl;
    std::cout<<"  Control MC weight is: "<<contmcweight_<<std::endl;
    std::cout<<"  Control data weight is: "<<contdataweight_<<std::endl;
    if((shapename_.size()!=shape_.size())&&shapename_.size()!=0){
      std::cout<<"WARNING: different numbers of shape names and shapes expect errors!"<<std::endl;
    }
    return 0;
  };

  int BkgSubDataNormShape::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;
    //sort out puweighting
    TFile *file=fs_;
    

    TDirectory* dir;
    if(dirname_==""){
      dir=file->mkdir(shapebaseset_.c_str());
    }
    else if(!fs_->GetDirectory(dirname_.c_str())){
      dir=file->mkdir(dirname_.c_str());
    }
    else{
      dir=fs_->GetDirectory(dirname_.c_str());
    }
    dir->cd();

    std::vector<TH1F> finalshapes;
    std::cout<<"  Getting Shape Reg shapes"<<std::endl;
    for(unsigned iShape=0;iShape<shape_.size();iShape++){
      TH1F  shaperegshape = filemanager->GetSetShape(shapebaseset_,shape_[iShape],basesel_,shapecat_+shapebaseextrasel_,shapebaseweight_,false);
      dir->cd();
      std::string histname;
      if(shapename_.size()==0){
	std::vector<std::string> strs;
	boost::split(strs, shape_[iShape], boost::is_any_of("("));
	histname=strs[0];
      }
      else{
	histname=shapename_[iShape];
      }
      shaperegshape.SetName(histname.c_str());
      double unnormnshapebase=Integral(&shaperegshape);
      //double unnormnshapebaseerr=Error(&shaperegshape);
      if(iShape==0) std::cout<<"unnormnshapebase: "<<unnormnshapebase<<std::endl;
      //Get Shapes for NSMC, NCMC, NCData and NCBkg
      std::cout<<"  Getting Shape Reg Backgrounds shape"<<std::endl;
      TH1F shapebkgshape;
      bool firstbkg=true;
      //IF NO DIRS TO GET DATA DRIVEN WEIGHTS, GET SETS SHAPE
      double extranshapebkgerr=0;
      if(shapebkgextrafactordir_.size()==0){
	shapebkgshape= filemanager->GetSetsShape(shapebkgset_,shape_[iShape],basesel_,(shapecat_+shapebkgextrasel_),shapemcweight_,false);
      }
      //WEIGHT INDIVIDUAL SHAPES BY DATA DRIVEN WEIGHTS
      else{
	if(shapebkgextrafactordir_.size()==shapebkgset_.size()){
	  for(unsigned iBkg=0;iBkg<shapebkgset_.size();iBkg++){
	    double extrafactor=1;
	    double extrafactorfracerr=0;
	    TDirectory* extrafactordir;
	    TVectorD *ddweight;
	    TVectorD *normerrs;
	    if(shapebkgextrafactordir_[iBkg]==""){
	      extrafactor=1;
	    }
	    else if(!fs_->GetDirectory(shapebkgextrafactordir_[iBkg].c_str())){
	      std::cout<<"Directory "<<shapebkgextrafactordir_[iBkg]<<" does not exist, exiting"<<std::endl;
	      return 1;
	    }
	    else{
	      extrafactordir=fs_->GetDirectory(shapebkgextrafactordir_[iBkg].c_str());
	      ddweight = (TVectorD*)extrafactordir->Get("ddweight");
	      normerrs = (TVectorD*)extrafactordir->Get("normerrs");
	      if(shapebkgisz_.size()==shapebkgset_.size()){
		if(shapebkgisz_[iBkg]==0){//NOT Z
		  extrafactor=(*ddweight)[0];
		  extrafactorfracerr=sqrt(pow((*normerrs)[0],2)+pow((*normerrs)[1],2));
		}
		if(shapebkgisz_[iBkg]==1){
		  extrafactor=(*ddweight)[0];//EWK WEIGHT
		  //std::cout<<extrafactor<<std::endl;
		  extrafactorfracerr=sqrt(pow((*normerrs)[0],2)+pow((*normerrs)[2],2));
		}
		if(shapebkgisz_[iBkg]==2){
		  extrafactor=(*ddweight)[1];//QCD WEIGHT
		  //std::cout<<extrafactor<<std::endl;
		  extrafactorfracerr=sqrt(pow((*normerrs)[0],2)+pow((*normerrs)[3],2));
		}
	      }
	    }
	    TH1F nextbkg;
	    //SPECIAL CASE FOR Z BKG
	    if(shapebkgisz_.size()==shapebkgset_.size()){
	      if(shapebkgisz_[iBkg]!=0){
		//GET Z SHAPE AND WEIGHT IT
		nextbkg=filemanager->GetSetShape(shapebkgset_[iBkg],shape_[iShape],basesel_,(zextrashapecat_+shapebkgextrasel_),shapemczweight_+"*"+boost::lexical_cast<std::string>(extrafactor),false);
	      }
	      else nextbkg=filemanager->GetSetShape(shapebkgset_[iBkg],shape_[iShape],basesel_,(shapecat_+shapebkgextrasel_),shapemcweight_+"*"+boost::lexical_cast<std::string>(extrafactor),false);
	    }
	    else nextbkg=filemanager->GetSetShape(shapebkgset_[iBkg],shape_[iShape],basesel_,(shapecat_+shapebkgextrasel_),shapemcweight_+"*"+boost::lexical_cast<std::string>(extrafactor),false);
	    //std::cout<<contbkgset_[iBkg]<<" "<<Integral(&nextbkg)<<" "<<extrafactorfracerr<<std::endl;
	    double thisbkgextranshapebkgerr=Integral(&nextbkg)*extrafactorfracerr;
	    extranshapebkgerr=sqrt(pow(extranshapebkgerr,2)+pow(thisbkgextranshapebkgerr,2));
	    //if(shapebkgisz_[iBkg]!=0)std::cout<<Integral(&nextbkg);//!!
	    //ADD HISTOGRAMS TOGETHER
	    if(firstbkg){
	      shapebkgshape=nextbkg;
	      firstbkg=false;
	    }
	    else{
	      shapebkgshape.Add(&nextbkg);
	    }
	    if(iShape==0)std::cout<<"Bkg: "<<shapebkgset_[iBkg]<<" n from this background"<<Integral(&nextbkg)<<std::endl;
	  }
	}
	else{
	  std::cout<<"Extra factor dir vector must be same size as bkg set vector expect errors!"<<std::endl;
	  return 1;
	}
      }
      if(iShape==0)std::cout<<"nshapebkg: "<<Integral(&shapebkgshape)<<std::endl;
      shaperegshape.Add(&shapebkgshape,-1);
      
      if(iShape==0){//Errors
	std::cout<<"Calculate some errors!!"<<std::endl;
      }
      finalshapes.push_back(shaperegshape);
    }

    double extrancbkgerr=0;
    double weight=1;
    double weighterrdatastatfrac=0;
    double weighterrmcstatfrac=0;
    if(do_ddnorm_){
      //Get Shapes for NCData and NCBkg
      std::cout<<"  Getting control MC Backgrounds shape"<<std::endl;
      TH1F contbkgshape;
      bool firstbkg=true;
      //IF NO DIRS TO GET DATA DRIVEN WEIGHTS, GET SETS SHAPE
      if(contbkgextrafactordir_.size()==0){
	contbkgshape= filemanager->GetSetsShape(contbkgset_,"jet2_pt(200,0.,1000.)",basesel_,(contcat_+contbkgextrasel_),contmcweight_,false);
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
		  //std::cout<<extrafactor<<std::endl;
		  extrafactorfracerr=sqrt(pow((*normerrs)[0],2)+pow((*normerrs)[2],2));
		}
		if(contbkgisz_[iBkg]==2){
		  extrafactor=(*ddweight)[1];//QCD WEIGHT
		  //std::cout<<extrafactor<<std::endl;
		  extrafactorfracerr=sqrt(pow((*normerrs)[0],2)+pow((*normerrs)[3],2));
		}
	      }
	    }
	    TH1F nextbkg;
	    //SPECIAL CASE FOR Z BKG
	    if(contbkgisz_.size()==contbkgset_.size()){
	      if(contbkgisz_[iBkg]!=0){
		//GET Z SHAPE AND WEIGHT IT
		nextbkg=filemanager->GetSetShape(contbkgset_[iBkg],"jet2_pt(200,0.,1000.)",basesel_,(zextracontcat_+contbkgextrasel_),contmczweight_+"*"+boost::lexical_cast<std::string>(extrafactor),false);
	      }
	      else nextbkg=filemanager->GetSetShape(contbkgset_[iBkg],"jet2_pt(200,0.,1000.)",basesel_,(contcat_+contbkgextrasel_),contmcweight_+"*"+boost::lexical_cast<std::string>(extrafactor),false);
	    }
	    else nextbkg=filemanager->GetSetShape(contbkgset_[iBkg],"jet2_pt(200,0.,1000.)",basesel_,(contcat_+contbkgextrasel_),contmcweight_+"*"+boost::lexical_cast<std::string>(extrafactor),false);
	    std::cout<<contbkgset_[iBkg]<<" "<<Integral(&nextbkg)<<" "<<extrafactor<<std::endl;//!!
	    double thisbkgextrancbkgerr=Integral(&nextbkg)*extrafactorfracerr;
	    extrancbkgerr=sqrt(pow(extrancbkgerr,2)+pow(thisbkgextrancbkgerr,2));
	    //ADD HISTOGRAMS TOGETHER
	    if(firstbkg){
	      contbkgshape=nextbkg;
	      firstbkg=false;
	    }
	    else{
	      contbkgshape.Add(&nextbkg);
	    }
	  }
	}
	else{
	  std::cout<<"Extra factor dir vector must be same size as bkg set vector expect errors!"<<std::endl;
	  return 1;
	}
      }
      std::cout<<"  Getting control Data shape"<<std::endl;
      TH1F  contdatashape = filemanager->GetSetShape(contdataset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_+contdataextrasel_,contdataweight_,false);
      
      
      //Integrate over shape to get number in each region
      double nshape = Integral(&(finalshapes[0]));
      double ncdata = Integral(&contdatashape);
      double ncbkg = Integral(&contbkgshape);
      double nshapeerr = Error(&(finalshapes[0]));//!!FIX THIS
      double ncdataerr = Error(&contdatashape);
      double ncbkgerr = Error(&contbkgshape);
      
      ncbkgerr=sqrt(pow(ncbkgerr,2)+pow(extrancbkgerr,2));//Add error from other data driven weight extrapolation factors
      
      std::cout<<"  nshape: "<<nshape<<"+-"<<"Calculate something"<<", ncdata: "<<ncdata<<"+-"<<ncdataerr<<", ncbkg: "<<ncbkg<<"+-"<<ncbkgerr<<std::endl;
      
      //Calculate weight
      weight=(ncdata-ncbkg)/nshape;
      weighterrdatastatfrac=ncdataerr/(ncdata-ncbkg);
      weighterrmcstatfrac=sqrt(((ncbkgerr/(ncdata-ncbkg))*(ncbkgerr/(ncdata-ncbkg)))+((nshapeerr/nshape)*(nshapeerr/nshape)));
      std::cout<<"  weight: "<<weight<<"+-"<<weight*weighterrdatastatfrac<<"(data stat.)+-"<<weight*weighterrmcstatfrac<<"(MC stat.)"<<std::endl;
      TVectorD weightvec(1);
      TVectorD errvec(2);
      weightvec[0]=weight*shapecontextrafactor_;
    }


  
    std::cout<<"  Scaling shape reg shapes"<<std::endl;
    for(unsigned iShape=0;iShape<shape_.size();iShape++){
      finalshapes[iShape].Scale(weight*shapecontextrafactor_);
      double nshape=Integral(&(finalshapes[iShape]));
      double nshapeerr=Error(&(finalshapes[iShape]));
      double weightednshapestatfrac=sqrt(pow((nshapeerr/nshape),2)+(weighterrmcstatfrac*weighterrmcstatfrac));
      if(iShape==0){
	std::cout<<"  Normalised NSHAPE: "<<nshape<<"+-"<<nshape*weighterrdatastatfrac<<"(data stat.)+-"<<nshape*weightednshapestatfrac<<"(MC stat.)"<<std::endl;
// 	if(do_latex_){
// 	  printf("N$^{data}$&XXX&$%.0f\\pm %.1f$(stat.)\\\\ \n",ncdata,ncdataerr);
// 	  printf("N$^{bkg}$&N/A&$%.1f\\pm %.1f$(stat.)\\\\ \n",ncbkg,ncbkgerr);
// 	  printf("N$^{MC}$&$%.1f\\pm%.1f$(stat.)&$%.1f\\pm %.1f$(stat.)\\\\ \n",unnormnshape,unnormnshapeerr,nshape,nshapeerr);
// 	  printf("Final estimate&\\textcolor{red}{$%.1f\\pm%.1f$(stat.)$\\pm%.1f$(MC stat.)}&N/A \\\\ \n",nshape,nshape*weighterrdatastatfrac,nshape*weightednshapestatfrac);
// 	}
//	errvec[0]=weighterrdatastatfrac;
//	errvec[1]=weightednshapestatfrac;
      }
      dir->cd();
      finalshapes[iShape].Write();
    }
    //    errvec.Write("normerrs");
    //    weightvec.Write("ddweight");
  
    return 0;
  };

}
