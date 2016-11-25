#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataZNormShape.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include <map>
#include <cstdio>
#include "TVectorD.h"

namespace ic{

  DataZNormShape::DataZNormShape(std::string name) : LTModule(name){
    sigmcweight_="weight_nolep";
    contmcweight_="total_weight_leptight";
    contdataweight_="weight_nolep";
    contdataextrasel_="";
    sigcontextrafactor_=1;
    std::vector<std::string> shapes;
    shapes.push_back("jet2_pt(200,0.,1000.)");
    shape_=shapes;
    dirname_="";
    ewkdirname_="";
    qcddirname_="";
    do_latex_=false;
  };

  DataZNormShape::~DataZNormShape(){ ;};

  int DataZNormShape::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"Signal MC ewk set is: "<<sigmcewkset_<<std::endl;
    std::cout<<"Signal MC qcd set is: "<<sigmcqcdset_<<std::endl;
    std::cout<<"Control MC ewk set  is: "<<contmcewkset_<<std::endl;
    std::cout<<"Control MC qcd set is: "<<contmcqcdset_<<std::endl;
    std::cout<<"Control data set is: "<<contdataset_<<std::endl;
    std::cout<<"Base selection is: "<<basesel_<<std::endl;
    std::cout<<"Signal extra selection is: "<<sigcat_<<std::endl;
    std::cout<<"Control extra selection is: "<<contcat_<<std::endl;
    
    std::cout<<"Inclusive control ewk cross-section: "<<sigmainccontewk_<<std::endl;
    std::cout<<"Inclusive control qcd cross-section: "<<sigmainccontqcd_<<std::endl;
    std::cout<<"Inclusive signal ewk cross-section: "<<sigmaincsigewk_<<std::endl;
    std::cout<<"Inclusive signal qcd cross-section: "<<sigmaincsigqcd_<<std::endl;
    std::cout<<"Number of generated ewk Z events: "<<ngenincewk_<<std::endl;
    std::cout<<"Number of generated qcd Z events: "<<ngenincqcd_<<std::endl;
    std::cout<<"Number of generated ewk Z events after mass filtering: "<<ngenmassfilteredewk_<<std::endl;
    std::cout<<"Number of generated qcd Z events after mass filtering: "<<ngenmassfilteredqcd_<<std::endl;
    return 0;
  };

  int DataZNormShape::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;
    TFile *file=fs_;
    TDirectory* dir;
    if(dirname_==""){
      dir=file->mkdir("zmumu");
    }
    else if(!fs_->GetDirectory(dirname_.c_str())){
      dir=file->mkdir(dirname_.c_str());
    }
    else{
      dir=fs_->GetDirectory(dirname_.c_str());
    }
    dir->cd();

    TDirectory* ewkdir;
    if(ewkdirname_==""){
      ewkdir=file->mkdir("zmumu_ewk");
    }
    else if(!fs_->GetDirectory(ewkdirname_.c_str())){
      ewkdir=file->mkdir(ewkdirname_.c_str());
    }
    else{
      ewkdir=fs_->GetDirectory(ewkdirname_.c_str());
    }
    ewkdir->cd();

    TDirectory* qcddir;
    if(qcddirname_==""){
      qcddir=file->mkdir("zmumu_qcd");
    }
    else if(!fs_->GetDirectory(qcddirname_.c_str())){
      qcddir=file->mkdir(qcddirname_.c_str());
    }
    else{
      qcddir=fs_->GetDirectory(qcddirname_.c_str());
    }
    qcddir->cd();

    dir->cd();

    //Get Shapes for NSMC, NCMC, NCData and NCBkg
    std::cout<<"  Getting control MC shape"<<std::endl;
    TH1F  contmcewkshape = filemanager->GetSetShape(contmcewkset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,contmcweight_,false);
    TH1F  contmcqcdshape = filemanager->GetSetShape(contmcqcdset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,contmcweight_,false);
    std::cout<<"  Getting control MC Backgrounds shape"<<std::endl;
    TH1F  contbkgshape = filemanager->GetSetsShape(contbkgset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,contmcweight_,false);
    std::cout<<"  Getting control Data shape"<<std::endl;
    TH1F  contdatashape = filemanager->GetSetShape(contdataset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_+contdataextrasel_,contdataweight_,false);
    
    //Integrate over shape to get number in each region
    double ncmcewk = Integral(&contmcewkshape);
    double ncmcqcd = Integral(&contmcqcdshape);
    double ncdata = Integral(&contdatashape);
    double ncbkg = Integral(&contbkgshape);
    double ncmcewkerr = Error(&contmcewkshape);
    double ncmcqcderr = Error(&contmcqcdshape);
    double ncdataerr = Error(&contdatashape);
    double ncbkgerr = Error(&contbkgshape);

    std::cout<<"  ncmcewk: "<<ncmcewk<<"+-"<<ncmcewkerr<<", ncmcqcd: "<<ncmcqcd<<"+-"<<ncmcqcderr<<", ncdata: "<<ncdata<<"+-"<<ncdataerr<<", ncbkg: "<<ncbkg<<"+-"<<ncbkgerr<<std::endl;

    //Do Weighting
    double effcvbfewk=ncmcewk/ngenincewk_;
    double effcvbfqcd=ncmcqcd/ngenincqcd_;
    double baseweight=(ncdata-ncbkg)/(sigmainccontewk_*effcvbfewk+sigmainccontqcd_*effcvbfqcd);
    double ewkweight=baseweight*sigmaincsigewk_/ngenmassfilteredewk_;
    double qcdweight=baseweight*sigmaincsigqcd_/ngenmassfilteredqcd_;
    double weightdatafracerr=(ncdataerr/(ncdata-ncbkg));
    
double baseweightdenmcfracerr=(sigmainccontewk_*effcvbfewk*(ncmcewkerr/ncmcewk)+sigmainccontqcd_*effcvbfqcd*(ncmcqcderr/ncmcqcd))/(sigmainccontewk_*effcvbfewk+sigmainccontqcd_*effcvbfqcd);
 double weightmcfracerr=sqrt((ncbkgerr/(ncdata-ncbkg))*(ncbkgerr/(ncdata-ncbkg))+baseweightdenmcfracerr*baseweightdenmcfracerr);

    std::cout<<"(NCData-NCBkg)/(NCMCEWK+NCMCQCD): "<<(ncdata-ncbkg)/(ncmcewk+ncmcqcd)<<" n.b. this is not directly used in calculation due to mumu nunu cross-section reweighting procedure"<<std::endl;

    TVectorD weightvec(2);
    TVectorD errvec(4);

    
    for(unsigned iShape=0;iShape<shape_.size();iShape++){
      std::string histname;
      if(shapename_.size()==0){
	std::vector<std::string> strs;
	boost::split(strs, shape_[iShape], boost::is_any_of("("));
        histname=strs[0];
      }
      else{
        histname=shapename_[iShape];
      }     
      TH1F  sigmcewkshape = filemanager->GetSetShape(sigmcewkset_,shape_[iShape],basesel_,sigcat_,sigmcweight_,false);
      TH1F  sigmcqcdshape = filemanager->GetSetShape(sigmcqcdset_,shape_[iShape],basesel_,sigcat_,sigmcweight_,false);
      dir->cd();
      weightvec[0]=sigcontextrafactor_*ewkweight;
      weightvec[1]=sigcontextrafactor_*qcdweight;
      double unnormnsmcewk=Integral(&sigmcewkshape);
      double unnormnsmcqcd=Integral(&sigmcqcdshape);
      double unnormnsmcewkerr=Error(&sigmcewkshape);
      double unnormnsmcqcderr=Error(&sigmcqcdshape);
      sigmcewkshape.Scale(sigcontextrafactor_*ewkweight);//baseweight*sigmaincsigewk_/ngenmassfilteredewk_);
      sigmcqcdshape.Scale(sigcontextrafactor_*qcdweight);//baseweight*sigmaincsigqcd_/ngenmassfilteredqcd_);
      if(iShape==0){
	double nsmcewk=Integral(&sigmcewkshape);
	double nsmcqcd=Integral(&sigmcqcdshape);
 	double nsmcewkerr=Error(&sigmcewkshape);
 	double nsmcqcderr=Error(&sigmcqcdshape);
	
	double ewkweightnsmcfracerr;
	double qcdweightnsmcfracerr;
	if(nsmcewk!=0)	ewkweightnsmcfracerr=sqrt(weightmcfracerr*weightmcfracerr+(nsmcewkerr/nsmcewk)*(nsmcewkerr/nsmcewk));
	else ewkweightnsmcfracerr=0;
	if(nsmcqcd!=0)  qcdweightnsmcfracerr=sqrt(weightmcfracerr*weightmcfracerr+(nsmcqcderr/nsmcqcd)*(nsmcqcderr/nsmcqcd));
	else qcdweightnsmcfracerr=0;

	errvec[0]=weightdatafracerr;
	errvec[1]=sqrt(pow(ewkweightnsmcfracerr*nsmcewk,2)+pow(qcdweightnsmcfracerr*nsmcqcd,2))/(nsmcewk+nsmcqcd);
	errvec[2]=ewkweightnsmcfracerr;
	errvec[3]=qcdweightnsmcfracerr;
	std::cout<<"NSMC EWK: "<<nsmcewk<<"+-"<<nsmcewkerr <<" NSMC QCD: "<<nsmcqcd<<"+-"<<nsmcqcderr<<std::endl;
      }
     
      //!!MAKE OVERALL SIGMCSHAPE HISTO
      //Get binning info from shape
      std::vector<std::string> strs;
      std::string binstr=shape_[iShape].substr(shape_[iShape].find_last_of("("));
      boost::split(strs, binstr, boost::is_any_of("(,)"));
      TH1F* sigmcshape=new TH1F(histname.c_str(),histname.c_str(),boost::lexical_cast<int>(strs[1]),boost::lexical_cast<double>(strs[2]),boost::lexical_cast<double>(strs[3]));
      sigmcshape->Add(&sigmcewkshape,&sigmcqcdshape);
      sigmcshape->SetName(histname.c_str());
      sigmcshape->Write();
      ewkdir->cd();
      sigmcewkshape.Write();
      qcddir->cd();
      sigmcqcdshape.Write();
      dir->cd();
      if(iShape==0){
	double ns=Integral(sigmcshape);
	//double nserr=Integral(sigmcshape);
	std::cout<<"Final estimate: "<<ns<<"+-"<<ns*errvec[0]<<" (data stat.) +-"<<ns*errvec[1]<<"(MC stat.)"<<std::endl;
	if(do_latex_){
	  printf("N$_{Gen}$(EWK)&\\multicolumn{2}{c|}{$%.1f$}   \\\\ \n",ngenincewk_);
	  printf("N$_{Gen}$(Z mass, EWK)&\\multicolumn{2}{c|}{$%.1f$}   \\\\ \n",ngenmassfilteredewk_);
	  printf("N$_{Gen}$(QCD)&\\multicolumn{2}{c|}{$%.1f$}   \\\\ \n",ngenincqcd_);
	  printf("N$_{Gen}$(Z mass, QCD)&\\multicolumn{2}{c|}{$%.1f$}   \\\\ \n",ngenmassfilteredqcd_);
	  printf("\\hline \n & Signal region & Control region \\\\ \n \\hline \n");
	  printf("$N^{data}$ & XXX & $%.0f \\pm %.1f (stat.)$\\\\ \n",ncdata,ncdataerr);
	  printf("$N^{bkg}$ & N/A  & $%.1f \\pm %.1f (stat.)$  \\\\ \n",ncbkg,ncbkgerr);
	  printf("$N^{MC}(EWK)$ & $%.1f \\pm %.1f (stat.)$  & $%.1f \\pm %.1f (stat.) $   \\\\ \n",unnormnsmcewk,unnormnsmcewkerr,ncmcewk,ncmcewkerr);
	  printf("$N^{MC}(QCD)$ & $%.1f \\pm %.1f (stat.)$  & $%.1f \\pm %.1f (stat.) $   \\\\ \n",unnormnsmcqcd,unnormnsmcqcderr,ncmcqcd,ncmcqcderr);
	  printf("$Final N^{Z\\rightarrow\\nu\\nu} estimate$ & \\textcolor{red}{$%.1f \\pm %.1f (data stat.) \\pm %.1f (MC stat.)$}  & N/A  \\\\ \n",ns,ns*errvec[0],ns*errvec[1]);
	}
      }

    }
    dir->cd();
    weightvec.Write("ddweight");
    errvec.Write("normerrs");

    return 0;
  };

}
