#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <iostream>
#include <vector>

namespace ic{
  LTFile::LTFile(){
  };

  LTFile::LTFile(std::string name, std::string path){
    name_=name;
    path_=path;
    set_="";
  };
  
  LTFile::LTFile(std::string name, std::string set, std::string path){
    name_=name;
    set_=set;
    path_=path;
  };

  int LTFile::Open(std::string infolder){
    //std::cout<<"Opening TFile..."<<std::endl;
    std::string filepath = (this->path());
    TFile * tmp = new TFile((infolder+"/"+this->path()).c_str());
    if (!tmp) {
      std::cerr << "Warning, file " << this->name() << " could not be opened." << std::endl;
      return 1;
    }
    else {
      tfile_ = tmp;
      tree_=(TTree *)tfile_->Get("LightTree");
      tree_->SetEstimate(1000);
      return 0;
    }
  }

  int LTFile::AddFriend(TTree* treeptr){
    TFile* file = treeptr->GetCurrentFile();
    tree_->AddFriend(treeptr->GetName(),file->GetName());
    return 0;
  };

  //WRITE GETSHAPE ACCESS METHODS

  LTFiles::LTFiles(){
  };

  LTFiles::LTFiles(std::string name, std::string set, std::string path){
    files_[name]=LTFile(name,set,path);
    setlists_[set].push_back(name);
  };

  LTFiles::LTFiles(std::string name, std::string path){
    files_[name]=LTFile(name,path);
  };
  
  LTFiles::LTFiles(std::vector<std::string> names,std::vector<std::string> sets, std::vector<std::string> paths){
    if(names.size()!=sets.size() || sets.size()!=paths.size()) std::cout<<"Error different numbers of names, sets and paths making empty Files object"<<std::endl;
    else{
      for(unsigned iname=0;iname<names.size();iname++){
	files_[names[iname]]=LTFile(names[iname],sets[iname],paths[iname]);
	setlists_[sets[iname]].push_back(names[iname]);
      }
    }
  };

  LTFiles::LTFiles(LTFile file){
    files_[file.name()]=file;
    if(file.set()!=""){
      setlists_[file.set()].push_back(file.name());
    }
  };
  
  LTFiles::LTFiles(std::vector<LTFile> files){
    for(unsigned ifile=0;ifile<files.size();ifile++){
      files_[files[ifile].name()]=files[ifile];
      if(files[ifile].set()!=""){
	setlists_[files[ifile].set()].push_back(files[ifile].name());
      }
    }
  };

  void LTFiles::AddFile(std::string name, std::string set, std::string path){
    setlists_[set].push_back(name);
    files_[name]=LTFile(name,set,path);
  };

  void LTFiles::AddFile(std::string name, std::string path){
    files_[name]=LTFile(name,"",path);
  };

  void LTFiles::AddFiles(std::vector<std::string> names, std::vector<std::string> sets, std::vector<std::string> paths){
    if(names.size()!=sets.size() || sets.size()!=paths.size()) std::cout<<"Error different numbers of names, sets and paths not adding!"<<std::endl;
    else{
      for(unsigned iname=0;iname<names.size();iname++){
        files_[names[iname]]=LTFile(names[iname],sets[iname],paths[iname]);
        setlists_[sets[iname]].push_back(names[iname]);
      }
    }
  };

  void LTFiles::AddFiles(std::vector<std::string> names, std::vector<std::string> paths){
    if(names.size()!=paths.size()) std::cout<<"Error different numbers of names, and paths not adding!"<<std::endl;
    else{
      for(unsigned iname=0;iname<names.size();iname++){
        files_[names[iname]]=LTFile(names[iname],"",paths[iname]);
      }
    }
  };

  void LTFiles::AddFile(LTFile file){
    files_[file.name()]=file;
    if(file.set()!=""){
      setlists_[file.set()].push_back(file.name());
    }
  }

  void LTFiles::AddFiles(std::vector<LTFile> files){
    for(unsigned ifile=0;ifile<files.size();ifile++){
      files_[files[ifile].name()]=files[ifile];
      if(files[ifile].set()!=""){
	setlists_[files[ifile].set()].push_back(files[ifile].name());
      }
    }
  }

  LTFile LTFiles::GetFile(std::string filename){
    if (files_.count(filename)>0){
      LTFile file=files_[filename];
      return file;
    }
    else{
      std::cout<<"No file called "<<filename<<" returning empty File object expect errors"<<std::endl;
      LTFile file;
      return file;
    }
  };

  std::vector<LTFile> LTFiles::GetFileSet(std::string setname){
    std::vector<LTFile> filesinset;
    if(setlists_.count(setname)>0){
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	filesinset.push_back(files_[*iter]);
      }
    }
    else{
      std::cout<<"No set called "<<setname<<" returning empty File vector object expect errors"<<std::endl;
    }
    return filesinset;
  };

  std::string LTFiles::GetPath(std::string filename){
    std::string path;
    if(files_.count(filename)>0){
      path=files_[filename].path();
    }
    else{
      std::cout<<"No file called "<<filename<<" returning empty string expect errors"<<std::endl;
    }
    return path;
  };

  std::vector<std::string> LTFiles::GetSetPaths(std::string setname){
    std::vector<std::string> pathsinset;
    if(setlists_.count(setname)>0){
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	pathsinset.push_back(files_[*iter].path());
      }
    }
    else{
      std::cout<<"No set called "<<setname<<" returning empty string vector expect errors"<<std::endl;
    }
    return pathsinset;
  };

  int LTFiles::OpenFile(std::string filename){
    if(files_.count(filename)>0){
      files_[filename].Open(infolder_);
      return 0;
    }
    else{
      std::cout<<"No file called "<<filename<<" error! "<<std::endl;
      return 1;
    }
  };

  int LTFiles::OpenSet(std::string setname){
    if(setlists_.count(setname)>0){
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	files_[*iter].Open(infolder_);      
      }
      return 0;
    }
    else{
      std::cout<<"No set called "<<setname<<" error!"<<std::endl;
      return 1;
    }
    
  };

  int LTFiles::OpenAll(){
    for(auto iter=files_.begin();iter!=files_.end();iter++){
      iter->second.Open(infolder_);
    }
    return 0;
  };

  int LTFiles::AddFriend(std::string filename,TTree* treeptr){
    files_[filename].AddFriend(treeptr);
    return 0;
  };


  //WRITE GETSHAPE ACCESS METHODS
  
}




