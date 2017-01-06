#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataNormShape.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TVectorD.h"
#include <cstdio>
#include <map>
#include "boost/lexical_cast.hpp"

namespace ic{

  DataNormShape::DataNormShape(std::string name) : LTModule(name){
    sigmcweight_="total_weight_lepveto";
    contmcweight_="total_weight_leptight";
    contmczweight_="weight_nolepnotrig*weight_trig_0";
    contdataweight_="weight_nolepnotrig*weight_trig_0";
    contdataextrasel_="";
    contmcextrasel_="";
    contbkgextrasel_="";
    sigcontextrafactor_=1.;
    std::vector<std::string> shapes;
    shapes.push_back("jet2_pt(200,0.,1000.)");
    shape_=shapes;
    dirname_="";
    do_latex_=false;
    do_subsets_=false;
  };

  DataNormShape::~DataNormShape(){ ;};

  int DataNormShape::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"  Signal MC set is: "<<sigmcset_<<std::endl;
    std::cout<<"  Control MC set is: "<<contmcset_<<std::endl;
    std::cout<<"  Control data set is: "<<contdataset_<<std::endl;
    std::cout<<"  Base selection is: "<<basesel_<<std::endl;
    std::cout<<"  Signal extra selection is: "<<sigcat_<<std::endl;
    std::cout<<"  Control extra selection is: "<<contcat_<<std::endl;
    std::cout<<"  Signal MC weight is: "<<sigmcweight_<<std::endl;
    std::cout<<"  Control MC weight is: "<<contmcweight_<<std::endl;
    std::cout<<"  Control data weight is: "<<contdataweight_<<std::endl;
    if((shapename_.size()!=shape_.size())&&shapename_.size()!=0){
      std::cout<<"WARNING: different numbers of shape names and shapes expect errors!"<<std::endl;
    }
    return 0;
  };

  int DataNormShape::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;
    //sort out puweighting
    TFile *file=fs_;
    
    std::vector<TDirectory*> subsetdirvec;
    if(do_subsets_){
      for(unsigned isubset=0;isubset<subsetdirs_.size();isubset++){
	TDirectory* subdir;
	if(subsetdirs_[isubset]==""){
	  std::cout<<"ERROR: MUST SPECIFY SUBDIR NAME!"<<std::endl;
	  return 1;
	}
	else if(!fs_->GetDirectory(subsetdirs_[isubset].c_str())){
	  subdir=file->mkdir(subsetdirs_[isubset].c_str());
	}
	else{
	  subdir=fs_->GetDirectory(subsetdirs_[isubset].c_str());
	}
	subsetdirvec.push_back(subdir);
      }
    }

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
    //Get Shapes for NSMC, NCMC, NCData and NCBkg
    std::cout<<"  Getting control MC shape"<<std::endl;
    TH1F  contmcshape = filemanager->GetSetShape(contmcset_,"jet2_pt(200,0.,1000.)",basesel_,(contcat_+contmcextrasel_),contmcweight_,false);
    std::cout<<"  Getting control MC Backgrounds shape"<<std::endl;
    TH1F contbkgshape;
    bool firstbkg=true;
    //IF NO DIRS TO GET DATA DRIVEN WEIGHTS, GET SETS SHAPE
    double extrancbkgerr=0;
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
		std::cout<<extrafactor<<std::endl;
		extrafactorfracerr=sqrt(pow((*normerrs)[0],2)+pow((*normerrs)[2],2));
	      }
	      if(contbkgisz_[iBkg]==2){
		extrafactor=(*ddweight)[1];//QCD WEIGHT
		std::cout<<extrafactor<<std::endl;
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
	  //std::cout<<contbkgset_[iBkg]<<" "<<Integral(&nextbkg)<<" "<<extrafactorfracerr<<std::endl;
	  double thisbkgextrancbkgerr=Integral(&nextbkg)*extrafactorfracerr;
	  extrancbkgerr=sqrt(pow(extrancbkgerr,2)+pow(thisbkgextrancbkgerr,2));
	  if(contbkgisz_[iBkg]!=0)std::cout<<Integral(&nextbkg);//!!
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
    double ncmc = Integral(&contmcshape);
    double ncdata = Integral(&contdatashape);
    double ncbkg = Integral(&contbkgshape);
    double ncmcerr = Error(&contmcshape);
    double ncdataerr = Error(&contdatashape);
    double ncbkgerr = Error(&contbkgshape);

    ncbkgerr=sqrt(pow(ncbkgerr,2)+pow(extrancbkgerr,2));//Add error from other data driven weight extrapolation factors

    std::cout<<"  ncmc: "<<ncmc<<"+-"<<ncmcerr<<", ncdata: "<<ncdata<<"+-"<<ncdataerr<<", ncbkg: "<<ncbkg<<"+-"<<ncbkgerr<<std::endl;

    //Calculate weight
    double weight=(ncdata-ncbkg)/ncmc;
    double weighterrdatastatfrac=ncdataerr/(ncdata-ncbkg);
    double weighterrmcstatfrac=sqrt(((ncbkgerr/(ncdata-ncbkg))*(ncbkgerr/(ncdata-ncbkg)))+((ncmcerr/ncmc)*(ncmcerr/ncmc)));
    std::cout<<"  weight: "<<weight<<"+-"<<weight*weighterrdatastatfrac<<"(data stat.)+-"<<weight*weighterrmcstatfrac<<"(MC stat.)"<<std::endl;
    TVectorD weightvec(1);
    TVectorD errvec(2);
    weightvec[0]=weight*sigcontextrafactor_;
    std::cout<<"  Getting signal MC shapes"<<std::endl;
    for(unsigned iShape=0;iShape<shape_.size();iShape++){
      TH1F  sigmcshape = filemanager->GetSetShape(sigmcset_,shape_[iShape],basesel_,sigcat_,sigmcweight_,false);
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
      sigmcshape.SetName(histname.c_str());
      double unnormnsmc=Integral(&sigmcshape);
      double unnormnsmcerr=Error(&sigmcshape);
      sigmcshape.Scale(weight*sigcontextrafactor_);
      double nsmc=Integral(&sigmcshape);
      double nsmcerr=Error(&sigmcshape);
      double weightednsmcstatfrac=sqrt(((nsmcerr/nsmc)*(nsmcerr/nsmc))+(weighterrmcstatfrac*weighterrmcstatfrac));
      //!!MAKE SURE  TO TAKE NSMC ERROR OUT OF NORMALISATION ERROR AS SHOULD BE DONE BIN BY BIN
      if(iShape==0){
	std::cout<<"  Unnormalised NSMC: "<<unnormnsmc<<"+-"<<unnormnsmcerr<<" (MC stat.)"<<std::endl;
	std::cout<<"  Normalised NSMC: "<<nsmc<<"+-"<<nsmc*weighterrdatastatfrac<<"(data stat.)+-"<<nsmc*weightednsmcstatfrac<<"(MC stat.)"<<std::endl;
	if(do_latex_){
	  printf("N$^{data}$&XXX&$%.0f\\pm %.1f$(stat.)\\\\ \n",ncdata,ncdataerr);
	  printf("N$^{bkg}$&N/A&$%.1f\\pm %.1f$(stat.)\\\\ \n",ncbkg,ncbkgerr);
	  printf("N$^{MC}$&$%.1f\\pm%.1f$(stat.)&$%.1f\\pm %.1f$(stat.)\\\\ \n",unnormnsmc,unnormnsmcerr,ncmc,ncmcerr);
	  printf("Final estimate&\\textcolor{red}{$%.1f\\pm%.1f$(stat.)$\\pm%.1f$(MC stat.)}&N/A \\\\ \n",nsmc,nsmc*weighterrdatastatfrac,nsmc*weightednsmcstatfrac);
	}
	errvec[0]=weighterrdatastatfrac;
	errvec[1]=weightednsmcstatfrac;
      }
      sigmcshape.Write();
      if(do_subsets_){
	for(unsigned isubset=0;isubset<subsetdirs_.size();isubset++){
	  TH1F  subsetshape = filemanager->GetSetShape(subsets_[isubset],shape_[iShape],basesel_,sigcat_,sigmcweight_,false);
	  subsetshape.Scale(weight*sigcontextrafactor_);
	  subsetshape.SetName(histname.c_str());
	  subsetdirvec[isubset]->cd();
	  subsetshape.Write();
	  TVectorD errvec(2);
	  errvec[0]=weighterrdatastatfrac;
	  errvec[1]=sqrt(((Error(&subsetshape)/Integral(&subsetshape))*(Error(&subsetshape)/Integral(&subsetshape)))+(weighterrmcstatfrac*weighterrmcstatfrac));
	  weightvec.Write("ddweight");
	  errvec.Write("normerrs");
	}
      }
    }
    dir->cd();
    errvec.Write("normerrs");
    weightvec.Write("ddweight");
  
    return 0;
  };

}
