#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/EventList.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include <map>
#include "TEventList.h"

namespace ic{

  EventList::EventList(std::string name) : LTModule(name){
    weight_="weight_nolepnotrig*weight_trig_0";
  };

  EventList::~EventList(){ ;};

  int EventList::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"Set is: "<<set_<<std::endl;
    std::cout<<"Base selection is: "<<basesel_<<std::endl;
    std::cout<<"Extra selection is: "<<cat_<<std::endl;
    std::cout<<"Weight is: "<<weight_<<std::endl;
    return 0;
  };

  int EventList::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;
    
    std::string full_selection = BuildCutString(basesel_,cat_,weight_);
    std::vector<LTFile> filesetvec= filemanager->GetFileSet(set_);

    //Get EventLists for each file in set
    for(unsigned iFile=0;iFile<filesetvec.size();iFile++){
      filesetvec[iFile].Open(filemanager->infolder());
      TEntryList list=filesetvec[iFile].GetEntryList(basesel_,cat_,weight_);
      TTree* small=filesetvec[iFile].GetSubTree(list);

      unsigned run,lumi,event;
      small->SetBranchAddress("run",&run);
      small->SetBranchAddress("lumi",&lumi);
      small->SetBranchAddress("event",&event);
      int nentries=small->GetEntries();
      for(int iEvent=0;iEvent<nentries;iEvent++){
	small->GetEntry(iEvent);
	std::cout<<run<<" "<<lumi<<" "<<event<<std::endl;
      }
      filesetvec[iFile].Close();
    }

    return 0;
  };

}
