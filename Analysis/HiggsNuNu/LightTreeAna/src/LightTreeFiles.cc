#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "boost/lexical_cast.hpp"
#include <iostream>
#include <vector>
#include <cstring>

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

  int LTFile::Open(std::string infolder,std::string dataeosfolder,std::string mceosfolder){
    //std::cout<<"Opening TFile..."<<std::endl;
    std::string filepath = (this->path());
    bool isMC=false;
    if (filepath.find("MC_")!=filepath.npos){
      //std::cout << " File " << filepath << " is MC." << std::endl;
	isMC=true;
    }
    else {
      //std::cout << " File " << dataeosfolder << infolder << filepath << " is data." << std::endl;
      isMC=false;
    }
    TFile * tmp = isMC?TFile::Open((mceosfolder+infolder+"/"+this->path()).c_str()) : TFile::Open((dataeosfolder+infolder+"/"+this->path()).c_str());
    if (!tmp) {
      std::cerr << "Warning, file " << this->name() << " could not be opened." << std::endl;
      return 1;
    }
    tfile_ = tmp;
    tree_=(TTree *)tfile_->Get("LightTree");
    if (!tree_) {
      std::cout << " Error opening tree, tree not found in file " << this->name() << std::endl;
      return 1;
    }
    tree_->SetEstimate(1000);
    if(friendTrees.size()!=0){
      for(unsigned iFriend=0;iFriend<friendTrees.size();iFriend++){
	tree_->AddFriend(friendTrees[iFriend].first.c_str(),friendTrees[iFriend].second.c_str());
      }
    }
    return 0;
  }

  int LTFile::Close(){
    tfile_->Close();
    return 0;
  }

  int LTFile::AddFriend(TTree* treeptr){
    TFile* file = treeptr->GetCurrentFile();
    tree_->AddFriend(treeptr->GetName(),file->GetName());
    friendTrees.push_back(std::make_pair(treeptr->GetName(),file->GetName()));
    return 0;
  };

  int LTFile::AddFriend(std::string treeName, std::string fileName){
    tree_->AddFriend(treeName.c_str(),fileName.c_str());
    friendTrees.push_back(std::make_pair(treeName,fileName));
    return 0;
  };

  TEntryList LTFile::GetEntryList(std::string const& selection, std::string const& category, std::string const& weight){
    std::string full_selection = BuildCutString(selection,category,weight);
    tree_->Draw(">>evtlist",full_selection.c_str(),"entrylist");
    TEntryList *list=(TEntryList*)gDirectory->Get("evtlist");
    return *list;
  }

  TTree* LTFile::GetSubTree(TEntryList list){
    tree_->SetEntryList(&list);
    TTree *small = tree_->CopyTree("");
    tree_->SetEventList(0);
    return small;

  }

  TH1F LTFile::GetShape(std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight){
    TH1F temp;
    if(tree_->GetEntries()<1){
      std::cout<<"WARNING: "<<name_<<" is empty."<<std::endl;
      temp.SetName("EMPTY");
      return temp;
    }
    temp=ic::GetShape(variable,selection,category,weight,tree_);
    if(strcmp(temp.GetName(),"ERROR")==0){
      std::cout<<"File with problem is: "<<name_<<std::endl;
    }
    return temp;
  };

  TH2F LTFile::GetShape2D(std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight){
    TH2F temp;
    if(tree_->GetEntries()<1){
      std::cout<<"WARNING: "<<name_<<" is empty."<<std::endl;
      temp.SetName("EMPTY");
      return temp;
    }
    temp=ic::GetShape2D(variable,selection,category,weight,tree_);
    if(strcmp(temp.GetName(),"ERROR")==0){
      std::cout<<"File with problem is: "<<name_<<std::endl;
    }
    return temp;
  };

  TH3F LTFile::GetShape3D(std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight){
    TH3F temp;
    if(tree_->GetEntries()<1){
      std::cout<<"WARNING: "<<name_<<" is empty."<<std::endl;
      temp.SetName("EMPTY");
      return temp;
    }
    temp=ic::GetShape3D(variable,selection,category,weight,tree_);
    if(strcmp(temp.GetName(),"ERROR")==0){
      std::cout<<"File with problem is: "<<name_<<std::endl;
    }
    return temp;
  };

  TTree* LTFile::GetTree(){
    return tree_;
  }




  LTFiles::LTFiles(){
  };

  LTFiles::LTFiles(std::string name, std::string set, std::string path){
    files_[name]=LTFile(name,set,path);
    setlists_[set].push_back(std::pair<std::string,bool>(name,true));
  };

  LTFiles::LTFiles(std::string name, std::string path){
    files_[name]=LTFile(name,path);
  };
  
  LTFiles::LTFiles(std::vector<std::string> names,std::vector<std::string> sets, std::vector<std::string> paths){
    if(names.size()!=sets.size() || sets.size()!=paths.size()) std::cout<<"Error different numbers of names, sets and paths making empty Files object"<<std::endl;
    else{
      for(unsigned iname=0;iname<names.size();iname++){
	files_[names[iname]]=LTFile(names[iname],sets[iname],paths[iname]);
	setlists_[sets[iname]].push_back(std::pair<std::string,bool>(names[iname],true));
      }
    }
  };

  LTFiles::LTFiles(LTFile file){
    files_[file.name()]=file;
    if(file.set()!=""){
      setlists_[file.set()].push_back(std::pair<std::string,bool>(file.name(),true));
    }
  };
  
  LTFiles::LTFiles(std::vector<LTFile> files){
    for(unsigned ifile=0;ifile<files.size();ifile++){
      files_[files[ifile].name()]=files[ifile];
      if(files[ifile].set()!=""){
	setlists_[files[ifile].set()].push_back(std::pair<std::string,bool>(files[ifile].name(),true));
      }
    }
  };

  void LTFiles::AddFile(std::string name, std::string set, std::string path){
    setlists_[set].push_back(std::pair<std::string,bool>(name,true));
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
        setlists_[sets[iname]].push_back(std::pair<std::string,bool>(names[iname],true));
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
      setlists_[file.set()].push_back(std::pair<std::string,bool>(file.name(),true));
    }
  }

  void LTFiles::AddFiles(std::vector<LTFile> files){
    for(unsigned ifile=0;ifile<files.size();ifile++){
      files_[files[ifile].name()]=files[ifile];
      if(files[ifile].set()!=""){
	setlists_[files[ifile].set()].push_back(std::pair<std::string,bool>(files[ifile].name(),true));
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
	filesinset.push_back(files_[(*iter).first]);
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
	pathsinset.push_back(files_[(*iter).first].path());
      }
    }
    else{
      std::cout<<"No set called "<<setname<<" returning empty string vector expect errors"<<std::endl;
    }
    return pathsinset;
  };

  std::vector<std::string> LTFiles::GetSetNames(std::string setname){
    std::vector<std::string> namesinset;
    if(setlists_.count(setname)>0){
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	namesinset.push_back(files_[(*iter).first].name());
      }
    }
    else{
      std::cout<<"No set called "<<setname<<" returning empty string vector expect errors"<<std::endl;
    }
    return namesinset;
  };

  int LTFiles::OpenFile(std::string filename){
    if(files_.count(filename)>0){
      if(files_[filename].Open(infolder_,dataeosfolder_,mceosfolder_)==1){
	std::cout<<"other stuff";
	return 1;
      }
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
	if((files_[(*iter).first].Open(infolder_,dataeosfolder_,mceosfolder_))==1){
	  std::cout<<"Warning, could not open element of set: " << (*iter).first << std::endl;
	  //return 1;
	  (*iter).second=false;
	  continue;
	}
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
      if((iter->second.Open(infolder_,dataeosfolder_,mceosfolder_))==1)return 1;
    }
    return 0;
  };



  int LTFiles::CloseFile(std::string filename){
    if(files_.count(filename)){
      files_[filename].Close();
      return 0;
    }
    else{
      std::cout<<"No file called "<<filename<<" error! "<<std::endl;
      return 1;
    }
  }

  int LTFiles::CloseSet(std::string setname){
    if(setlists_.count(setname)>0){
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	if ((*iter).second) files_[(*iter).first].Close();
      }
      return 0;
    }
    else{
      std::cout<<"No set called "<<setname<<" error!"<<std::endl;
      return 1;
    }
  }

  int LTFiles::CloseAll(){
    for(auto iter=files_.begin();iter!=files_.end();iter++){
      iter->second.Close();
    }
    return 0;
  };

  int LTFiles::AddFriend(std::string filename, std::string treename, std::string treefilename){
    files_[filename].AddFriend(treename,treefilename);
    return 0;
  };

  TH1F LTFiles::GetShape(std::string filename, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight){
    if(OpenFile(filename)==1){
      std::cout<<"Problem opening file "<<filename << " returning empty TH1F"<<std::endl;
      TH1F temp;
      return temp;
    }
    TH1F temp = files_[filename].GetShape(variable,selection,category,weight);
    CloseFile(filename);
    return(temp);
  };


  TH1F LTFiles::GetSetShape(std::string setname, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, bool do_lumixs_weights_=true){
    TH1F setshape;
    if(setlists_.count(setname)>0){
      if(OpenSet(setname)==1){
	std::cout<<"Problem opening set "<<setname<<" returning empty TH1F"<<std::endl;
	TH1F temp;
	return temp;
      }
      bool first=true;
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	if (!(*iter).second) continue;
	//ADAPT LUMIXS BIT
	//std::string sample_path_=files_[*iter].path();
	double lumixsweight=1;
	if(do_lumixs_weights_){
	  lumixsweight=this->GetLumiXSWeight(files_[(*iter).first]);
	}
	std::cout << "Set: " << setname << ", sample " << (*iter).first << ": " ;
	TH1F temp=files_[(*iter).first].GetShape(variable,selection,category,weight+"*"+boost::lexical_cast<std::string>(lumixsweight));
	if(strcmp(temp.GetName(),"EMPTY")==0){
	  continue;
	}
	temp.Sumw2();
	if(first){
	  setshape=temp;
	  first=false;
	}
	else setshape.Add(&temp);
      }
      CloseSet(setname);
    }
    else{
      std::cout<<"No set called "<<setname<<" returning empty TH1F object expect errors"<<std::endl;
    }
    return setshape;
  };

  TH1F LTFiles::GetSetsShape(std::vector<std::string> setnames, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, bool do_lumixs_weights_=true){
    TH1F setsshape;
    bool firstset=true;
    for(unsigned iset=0;iset<setnames.size();iset++){
      TH1F setshape;
	if(setlists_.count(setnames[iset])>0){
	  if(OpenSet(setnames[iset])==1){
	    std::cout<<"Problem opening set "<<setnames[iset]<<" returning empty TH1F"<<std::endl;
	    TH1F temp;
	    return temp;
	  }
	  bool firstshape=true;
	  for(auto iter=setlists_[setnames[iset]].begin(); iter!=setlists_[setnames[iset]].end();++iter){
	    if (!(*iter).second) continue;
	    //ADAPT LUMIXS BIT
	    //std::string sample_path_=files_[*iter].path();
	    double lumixsweight=1;
	    if(do_lumixs_weights_){
	      lumixsweight=this->GetLumiXSWeight(files_[(*iter).first]);
	    }
	    std::cout << "Set: " << setnames[iset] << ", sample " << (*iter).first << ": " ;
	    TH1F temp=files_[(*iter).first].GetShape(variable,selection,category,weight+"*"+boost::lexical_cast<std::string>(lumixsweight));
	    if(strcmp(temp.GetName(),"EMPTY")==0){
	      continue;
	    }
	    temp.Sumw2();
	    if(firstshape){
	      setshape=temp;
	      firstshape=false;
	    }
	    else setshape.Add(&temp);
	  }
	  CloseSet(setnames[iset]);
	}
	else{
	  std::cout<<"No set called "<<setnames[iset]<<" returning empty TH1F object expect errors"<<std::endl;
	}
	if(firstset){
	  setsshape=setshape;
	  firstset=false;
	}
	else setsshape.Add(&setshape);
    }     
    return setsshape;
  };

  TH2F LTFiles::GetShape2D(std::string filename, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight){
    if(OpenFile(filename)==1){
      std::cout<<"Problem opening file "<<filename << " returning empty TH2F"<<std::endl;
      TH2F temp;
      return temp;
    }
    TH2F temp = files_[filename].GetShape2D(variable,selection,category,weight);
    CloseFile(filename);
    return(temp);
  };


  TH2F LTFiles::GetSetShape2D(std::string setname, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, bool do_lumixs_weights_=true){
    TH2F setshape;
    if(setlists_.count(setname)>0){
      if(OpenSet(setname)==1){
	std::cout<<"Problem opening set "<<setname<<" returning empty TH2F"<<std::endl;
	TH2F temp;
	return temp;
      }
      bool first=true;
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	if (!(*iter).second) continue;
	//ADAPT LUMIXS BIT
	//std::string sample_path_=files_[*iter].path();
	double lumixsweight=1;
	if(do_lumixs_weights_){
	  lumixsweight=this->GetLumiXSWeight(files_[(*iter).first]);
	}
      
	TH2F temp=files_[(*iter).first].GetShape2D(variable,selection,category,weight+"*"+boost::lexical_cast<std::string>(lumixsweight));
	if(strcmp(temp.GetName(),"EMPTY")==0){
	  continue;
	}
	temp.Sumw2();
	if(first){
	  setshape=temp;
	  first=false;
	}
	else setshape.Add(&temp);
      }
      CloseSet(setname);
    }
    else{
      std::cout<<"No set called "<<setname<<" returning empty TH2F object expect errors"<<std::endl;
    }
    return setshape;
  };

  TH2F LTFiles::GetSetsShape2D(std::vector<std::string> setnames, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, bool do_lumixs_weights_=true){
    TH2F setsshape;
    bool firstset=true;
    for(unsigned iset=0;iset<setnames.size();iset++){
      TH2F setshape;
	if(setlists_.count(setnames[iset])>0){
	  if(OpenSet(setnames[iset])==1){
	    std::cout<<"Problem opening set "<<setnames[iset]<<" returning empty TH2F"<<std::endl;
	    TH2F temp;
	    return temp;
	  }
	  bool firstshape=true;
	  for(auto iter=setlists_[setnames[iset]].begin(); iter!=setlists_[setnames[iset]].end();++iter){
	    if (!(*iter).second) continue;
	    //ADAPT LUMIXS BIT
	    //std::string sample_path_=files_[*iter].path();
	    double lumixsweight=1;
	    if(do_lumixs_weights_){
	      lumixsweight=this->GetLumiXSWeight(files_[(*iter).first]);
	    }
	    TH2F temp=files_[(*iter).first].GetShape2D(variable,selection,category,weight+"*"+boost::lexical_cast<std::string>(lumixsweight));
	    if(strcmp(temp.GetName(),"EMPTY")==0){
	      continue;
	    }
	    temp.Sumw2();
	    if(firstshape){
	      setshape=temp;
	      firstshape=false;
	    }
	    else setshape.Add(&temp);
	  }
	  CloseSet(setnames[iset]);
	}
	else{
	  std::cout<<"No set called "<<setnames[iset]<<" returning empty TH2F object expect errors"<<std::endl;
	}
	if(firstset){
	  setsshape=setshape;
	  firstset=false;
	}
	else setsshape.Add(&setshape);
    }     
    return setsshape;
  };

  TH3F LTFiles::GetShape3D(std::string filename, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight){
    TH3F temp= files_[filename].GetShape3D(variable,selection,category,weight);    
    return(temp);
    
  };

  TH3F LTFiles::GetSetShape3D(std::string setname, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, bool do_lumixs_weights_=true){
    TH3F setshape;
    if(setlists_.count(setname)>0){
      bool first=true;
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	if (!(*iter).second) continue;
	std::string sample_path_=files_[(*iter).first].path();
	double lumixsweight=1;
	if(do_lumixs_weights_){
	  lumixsweight=this->GetLumiXSWeight(files_[(*iter).first]);
	}
	
	TH3F temp=files_[(*iter).first].GetShape3D(variable,selection,category,weight+"*"+boost::lexical_cast<std::string>(lumixsweight));
	if(strcmp(temp.GetName(),"EMPTY")==0){
	  continue;
	}
	if(first){
	  setshape=temp;
	  first=false;
	}
	else setshape.Add(&temp);
      }
    }
    else{
      std::cout<<"No set called "<<setname<<" returning empty TH3 object expect errors"<<std::endl;
    }
    return setshape;
  };

  double LTFiles::GetLumiXSWeight(LTFile file){
    std::string sample_path_=file.path();
    std::string suffix=".root";
    std::size_t found = sample_path_.find(suffix);
    double lumixsweight=1;
    if(found==std::string::npos){
      lumixsweight=1;
      std::cout<<"Non-standard sample name format not doing lumixs weight"<<std::endl;
    }
    else{
      sample_path_.erase(found,5);
      //std::cout << "Sample Path: "<<sample_path_<<std::endl;                                                                                         
      
      //Get lumi xs and events from params file                                                                                                        
      SimpleParamParser parser;
      //std::cout << "** Parsing parameter file... **" << input_params_ << std::endl;                                                                  
      parser.ParseFile(input_params_);
      //std::cout<<"parsed"<<std::endl;                                                                                                                
      double xs=parser.GetParam<double>("XS_"+sample_path_);
      //std::cout<<"got xs"<<std::endl;                                                                                                                
      double events=parser.GetParam<double>("EVT_"+sample_path_);
      //std::cout<<"got events"<<std::endl;                                                                                                            
      double lumi=parser.GetParam<double>("LUMI_DATA");
      //std::cout<<"got lumi"<<std::endl;
      //std::cout<<"XS is: "<<xs<<"pb"<<std::endl;
      // std::cout<<"EVT is: "<<events<<std::endl;
      // std::cout<<"LUMI is: "<<lumi<<"pb^-1"<<std::endl;

      if(xs==-1) lumixsweight=1;
      else lumixsweight=xs*lumi/events;
      //std::cout<<"LUMIXSWEIGHT is: "<<lumixsweight<<std::endl;
    }
    return lumixsweight;
  }
  
}




