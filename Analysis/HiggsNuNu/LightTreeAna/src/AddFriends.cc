#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/AddFriends.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include <map>

namespace ic{

  AddFriends::AddFriends(std::string name) : LTModule(name){
  };

  AddFriends::~AddFriends(){ ;};

  int AddFriends::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"Sets friends will be added to: "<<std::endl;
    for(unsigned iSet=0;iSet<sets_.size();iSet++){
      std::cout<<"  "<<sets_[iSet]<<std::endl;
    }
    std::cout<<"Friend file dir: "<<frienddir_<<std::endl;
    std::cout<<"Friend tree name:"<<friendtreename_<<std::endl;
    return 0;
  };

  int AddFriends::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;

    for(unsigned iSet=0;iSet<sets_.size();iSet++){
      std::cout<<"Adding friends for set: "<<sets_[iSet]<<std::endl;
      std::vector<LTFile> files = filemanager->GetFileSet(sets_[iSet]);
      for(unsigned iFile=0;iFile<files.size();iFile++){
	std::cout<<"  "<<files[iFile].name()<<std::endl;
	std::cout<<"    opening"<<std::endl;
	filemanager->OpenFile(files[iFile].name());
	std::cout<<"    adding"<<std::endl;
	filemanager->AddFriend(files[iFile].name(),friendtreename_.c_str(),(frienddir_+files[iFile].name()+"_"+friendtreename_+".root").c_str());
	std::cout<<"    closing"<<std::endl;
	filemanager->CloseFile(files[iFile].name());
      }
    }

    return 0;
  };

}
