#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataQCDEst.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include <map>

namespace ic{

  DataQCDEst::DataQCDEst(std::string name) : LTModule(name){
  };

  DataQCDEst::~DataQCDEst(){ ;};

  int DataQCDEst::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"A set is: "<<Aset_<<std::endl;
    std::cout<<"B set is: "<<Bset_<<std::endl;
    std::cout<<"C set is: "<<Cset_<<std::endl;
    std::cout<<"Base selection is: "<<basesel_<<std::endl;
    std::cout<<"A extra selection is: "<<Acat_<<std::endl;
    std::cout<<"B extra selection is: "<<Bcat_<<std::endl;
    std::cout<<"C extra selection is: "<<Ccat_<<std::endl;
    return 0;
  };

  int DataQCDEst::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;

    //Get Shapes for NSMC, NCMC, NCData and NCBkg
    std::cout<<"  Getting A shape"<<std::endl;
    TH1F  Ashape = filemanager->GetSetShape(Aset_,"jet2_pt(200,0.,1000.)",basesel_,Acat_,"weight_nolepnotrig*weight_trig_0",false);
    std::cout<<"  Getting B shape"<<std::endl;
    TH1F  Bshape = filemanager->GetSetShape(Bset_,"jet2_pt(200,0.,1000.)",basesel_,Bcat_,"weight_nolepnotrig*weight_trig_0",false);
    std::cout<<"  Getting C shape"<<std::endl;
    TH1F  Cshape = filemanager->GetSetShape(Cset_,"jet2_pt(200,0.,1000.)",basesel_,Ccat_,"weight_nolepnotrig*weight_trig_0",false);
    std::cout<<"  Getting A bkg shape"<<std::endl;
    TH1F  Abkgshape = filemanager->GetSetsShape(Abkgset_,"jet2_pt(200,0.,1000.)",basesel_,Acat_,"total_weight_lepveto",false);
    std::cout<<"  Getting B bkg shape"<<std::endl;
    TH1F  Bbkgshape = filemanager->GetSetsShape(Bbkgset_,"jet2_pt(200,0.,1000.)",basesel_,Bcat_,"total_weight_lepveto",false);
    std::cout<<"  Getting C bkg shape"<<std::endl;
    TH1F  Cbkgshape = filemanager->GetSetsShape(Cbkgset_,"jet2_pt(200,0.,1000.)",basesel_,Ccat_,"total_weight_lepveto",false);

    //Integrate over shape to get number in each region
    double nA = Integral(&Ashape);
    double nB = Integral(&Bshape);
    double nC = Integral(&Cshape);
    double nAbkg = Integral(&Abkgshape);
    double nBbkg = Integral(&Bbkgshape);
    double nCbkg = Integral(&Cbkgshape);

    std::cout<<"  nA: "<<nA<<"  nB: "<<nB<<"  nC: "<<nC<<"  nAbkg: "<<nAbkg<<"  nBbkg: "<<nBbkg<<"  nCbkg: "<<nCbkg<<std::endl;

    //Calculate background
    double nD=(nB-nBbkg)*(nC-nCbkg)/(nA-nAbkg);

    std::cout<<"  "<<module_name_<<" background in signal region is: "<<nD<<std::endl;

    return 0;
  };

}
