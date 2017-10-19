#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataZEst.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include <map>

namespace ic{

  DataZEst::DataZEst(std::string name) : LTModule(name){
  };

  DataZEst::~DataZEst(){ ;};

  int DataZEst::Init(TFile* fs){
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

  int DataZEst::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;

    //Get Shapes for NSMC, NCMC, NCData and NCBkg
    std::cout<<"  Getting signal MC shape"<<std::endl;
    TH1F  sigmcewkshape = filemanager->GetSetShape(sigmcewkset_,"jet2_pt(200,0.,1000.)",basesel_,sigcat_,"weight_nolepnotrig*weight_trig_0",false);
    TH1F  sigmcqcdshape = filemanager->GetSetShape(sigmcqcdset_,"jet2_pt(200,0.,1000.)",basesel_,sigcat_,"weight_nolepnotrig*weight_trig_0",false);
    std::cout<<"  Getting control MC shape"<<std::endl;
    TH1F  contmcewkshape = filemanager->GetSetShape(contmcewkset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,"total_weight_leptight",false);
    TH1F  contmcqcdshape = filemanager->GetSetShape(contmcqcdset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,"total_weight_leptight",false);
    std::cout<<"  Getting control MC Backgrounds shape"<<std::endl;
    TH1F  contbkgshape = filemanager->GetSetsShape(contbkgset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,"total_weight_leptight",false);
    std::cout<<"  Getting control Data shape"<<std::endl;
    TH1F  contdatashape = filemanager->GetSetShape(contdataset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,"weight_nolepnotrig*weight_trig_0",false);
    
    //Integrate over shape to get number in each region
    double nsmcewk = Integral(&sigmcewkshape);
    double nsmcqcd = Integral(&sigmcqcdshape);
    double ncmcewk = Integral(&contmcewkshape);
    double ncmcqcd = Integral(&contmcqcdshape);
    double ncdata = Integral(&contdatashape);
    double ncbkg = Integral(&contbkgshape);

    std::cout<<"  nsmcewk: "<<nsmcewk<<",  nsmcqcd: "<<nsmcqcd<<", ncmcewk: "<<ncmcewk<<", ncmcqcd: "<<ncmcqcd<<", ncdata: "<<ncdata<<", ncbkg: "<<ncbkg<<std::endl;

    //!!GOOD TO HERE
    //Calculate background
    double effsvbfewk=nsmcewk/ngenmassfilteredewk_;
    double effsvbfqcd=nsmcqcd/ngenmassfilteredqcd_;
    double effcvbfewk=ncmcewk/ngenincewk_;
    double effcvbfqcd=ncmcqcd/ngenincqcd_;
    double nsdata=(ncdata-ncbkg)*(sigmaincsigewk_*effsvbfewk+sigmaincsigqcd_*effsvbfqcd)/(sigmainccontewk_*effcvbfewk+sigmainccontqcd_*effcvbfqcd);

    std::cout<<"  "<<module_name_<<" background in signal region is: "<<nsdata<<std::endl;

    return 0;
  };

}
