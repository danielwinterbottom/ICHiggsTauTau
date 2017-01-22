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
    //else {
    //std::cout << " File " << tmp->GetPath() << " successfully opened." << std::endl;
    //}
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

  bool LTFile::GetShape(TH1F & temp, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, const bool toadd){
    //TH1F temp;
    if(tree_->GetEntries()<1){
      std::cout<<"WARNING: "<<name_<<" is empty."<<std::endl;
      //temp.SetName("EMPTY");
      return false;
    }
    bool success = ic::GetShape(temp,variable,selection,category,weight,tree_,toadd);
    //if(strcmp(temp.GetName(),"ERROR")==0){
    //std::cout<<"File with problem is: "<<name_<<std::endl;
    //return false;
    //}
    return success;
  };

  bool LTFile::GetShape2D(TH2F & temp, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, const bool toadd){
    //TH2F temp;
    if(tree_->GetEntries()<1){
      std::cout<<"WARNING: "<<name_<<" is empty."<<std::endl;
      //temp.SetName("EMPTY");
      return false;
    }
    bool success =ic::GetShape2D(temp,variable,selection,category,weight,tree_,toadd);
    //if(strcmp(temp.GetName(),"ERROR")==0){
    //std::cout<<"File with problem is: "<<name_<<std::endl;
    //return false;
    //}
    return success;
  };

  bool LTFile::GetShape3D(TH3F & temp, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, const bool toadd){
    //TH2F temp;
    if(tree_->GetEntries()<1){
      std::cout<<"WARNING: "<<name_<<" is empty."<<std::endl;
      //temp.SetName("EMPTY");
      return false;
    }
    bool success =ic::GetShape3D(temp,variable,selection,category,weight,tree_,toadd);
    //if(strcmp(temp.GetName(),"ERROR")==0){
    //std::cout<<"File with problem is: "<<name_<<std::endl;
    //return false;
    //}
    return success;
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

  bool LTFiles::GetShape(TH1F & temp, std::string filename, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, const bool toadd){
    if(OpenFile(filename)==1){
      std::cout<<"Problem opening file "<<filename 
	//<< " returning empty TH1F"
	       <<std::endl;
      //TH1F temp;
      return false;
    }
    bool success = files_[filename].GetShape(temp,variable,selection,category,weight,toadd);
    CloseFile(filename);
    return success;
  };


  TH1F LTFiles::GetShape(std::string filename, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight){
    TH1F temp;
    GetShape(temp,filename,variable,selection,category,weight,false);
    return temp;
  }

  TH1F LTFiles::GetSetShape(std::string setname, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, const bool do_lumixs_weights_){
    TH1F temp;
    GetSetShape(temp,setname,variable,selection,category,weight,do_lumixs_weights_,false);
    return temp;

  }

  bool LTFiles::GetSetShape(TH1F & setshape, std::string setname, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, const bool do_lumixs_weights_, const bool toadd){
    //TH1F setshape;
    bool oneok = false;
    if(setlists_.count(setname)>0){
      if(OpenSet(setname)==1){
	std::cout<<"Problem opening set "<<setname
	  //<<" returning empty TH1F"
		 <<std::endl;
	//TH1F temp;
	return false;
      }
      bool first=toadd?false:true;
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	if (!(*iter).second) continue;
	//ADAPT LUMIXS BIT
	//std::string sample_path_=files_[*iter].path();
	double lumixsweight=1;
	if(do_lumixs_weights_){
	  lumixsweight=this->GetLumiXSWeight(files_[(*iter).first]);
	}
	std::cout << "Set: " << setname << ", sample " << (*iter).first << ": " ;
	bool success = files_[(*iter).first].GetShape(setshape,variable,selection,category,weight+"*"+boost::lexical_cast<std::string>(lumixsweight),!first);
	CloseFile((*iter).first);
	if (!success){
	  std::cout << " --- Error, Skipping" << std::endl;
	  continue;
	}
	else oneok=true;
	//if(strcmp(temp.GetName(),"EMPTY")==0){
	//continue;
	//}
	//temp.Sumw2();
	if(first){
	  //setshape=temp;
	  first=false;
	}
	//else {
	//bool addok = setshape.Add(&temp);
	//if (!addok) {
	// std::cout << " Error with addition of shape for element " << (*iter).first << " skipping..." << std::endl;
	// continue;
	//}
	//}
      }
      //CloseSet(setname);
    }
    else{
      std::cout<<"No set called "<<setname
	//<<" returning empty TH1F object expect errors"
	       <<std::endl;
      return false;
    }
    return oneok;
  };

  TH1F LTFiles::GetSetsShape(std::vector<std::string> setnames, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, const bool do_lumixs_weights_){
    TH1F setsshape;
    setsshape.Sumw2();
    bool firstset=true;
    for(unsigned iset=0;iset<setnames.size();iset++){
      GetSetShape(setsshape,setnames[iset], variable, selection, category, weight, do_lumixs_weights_,!firstset);
      if(firstset){
	//setsshape= temp;
	firstset=false;
      }
      //else setsshape.Add(&temp);
    }    
    return setsshape;
  };

  bool LTFiles::GetShape2D(TH2F & temp, std::string filename, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, bool toadd){
    if(OpenFile(filename)==1){
      std::cout<<"Problem opening file "<<filename 
	//<< " returning empty TH2F"
	       <<std::endl;
      //TH2F temp;
      return false;
    }
    bool success = files_[filename].GetShape2D(temp,variable,selection,category,weight,toadd);
    CloseFile(filename);
    return success;
  };


  bool LTFiles::GetSetShape2D(TH2F & setshape, std::string setname, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, const bool do_lumixs_weights_, const bool toadd){
    //TH2F setshape;
    bool oneok = false;
    if(setlists_.count(setname)>0){
      if(OpenSet(setname)==1){
	std::cout<<"Problem opening set "<<setname
	  //<<" returning empty TH2F"
		 <<std::endl;
	//TH2F temp;
	return false;
      }
      bool first=toadd?false:true;
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	if (!(*iter).second) continue;
	//ADAPT LUMIXS BIT
	//std::string sample_path_=files_[*iter].path();
	double lumixsweight=1;
	if(do_lumixs_weights_){
	  lumixsweight=this->GetLumiXSWeight(files_[(*iter).first]);
	}
      
	bool success = files_[(*iter).first].GetShape2D(setshape,variable,selection,category,weight+"*"+boost::lexical_cast<std::string>(lumixsweight),!first);
	CloseFile((*iter).first);
	if (!success){
	  std::cout << " --- Error, Skipping" << std::endl;
	  continue;
	}
	else oneok=true;
	
	//if(strcmp(temp.GetName(),"EMPTY")==0){
	//continue;
	//}
	//temp.Sumw2();
	if(first){
	  //setshape=temp;
	  first=false;
	}
	//else setshape.Add(&temp);
      }
      //CloseSet(setname);
    }
    else{
      std::cout<<"No set called "<<setname
	//<<" returning empty TH2F object expect errors"
	       <<std::endl;
      return false;
    }
    return oneok;
  };

  bool LTFiles::GetShape3D(TH3F & temp, std::string filename, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, bool toadd){
    if(OpenFile(filename)==1){
      std::cout<<"Problem opening file "<<filename 
	//<< " returning empty TH3F"
	       <<std::endl;
      //TH3F temp;
      return false;
    }
    bool success = files_[filename].GetShape3D(temp,variable,selection,category,weight,toadd);
    CloseFile(filename);
    return success;
  };


  bool LTFiles::GetSetShape3D(TH3F & setshape, std::string setname, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, const bool do_lumixs_weights_, const bool toadd){
    //TH3F setshape;
    bool oneok = false;
    if(setlists_.count(setname)>0){
      if(OpenSet(setname)==1){
	std::cout<<"Problem opening set "<<setname
	  //<<" returning empty TH3F"
		 <<std::endl;
	//TH3F temp;
	return false;
      }
      bool first=toadd?false:true;
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	if (!(*iter).second) continue;
	//ADAPT LUMIXS BIT
	//std::string sample_path_=files_[*iter].path();
	double lumixsweight=1;
	if(do_lumixs_weights_){
	  lumixsweight=this->GetLumiXSWeight(files_[(*iter).first]);
	}
      
	bool success = files_[(*iter).first].GetShape3D(setshape,variable,selection,category,weight+"*"+boost::lexical_cast<std::string>(lumixsweight),!first);
	CloseFile((*iter).first);
	if (!success){
	  std::cout << " --- Error, Skipping" << std::endl;
	  continue;
	}
	else oneok=true;
	
	//if(strcmp(temp.GetName(),"EMPTY")==0){
	//continue;
	//}
	//temp.Sumw2();
	if(first){
	  //setshape=temp;
	  first=false;
	}
	//else setshape.Add(&temp);
      }
      //CloseSet(setname);
    }
    else{
      std::cout<<"No set called "<<setname
	//<<" returning empty TH3F object expect errors"
	       <<std::endl;
      return false;
    }
    return oneok;
  };

  TH2F LTFiles::GetShape2D(std::string filename, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight){
    TH2F temp;
    GetShape2D(temp,filename,variable,selection,category,weight,false);
    return temp;
  }

  TH2F LTFiles::GetSetShape2D(std::string setname, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, const bool do_lumixs_weights_){
    TH2F temp;
    GetSetShape2D(temp,setname,variable,selection,category,weight,do_lumixs_weights_,false);
    return temp;

  }

  TH2F LTFiles::GetSetsShape2D(std::vector<std::string> setnames, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, const bool do_lumixs_weights_){
    TH2F setsshape;
    setsshape.Sumw2();
    bool firstset=true;
    for(unsigned iset=0;iset<setnames.size();iset++){
      GetSetShape2D(setsshape,setnames[iset], variable, selection, category, weight, do_lumixs_weights_,!firstset);
      if(firstset){
	//setsshape=temp;
	firstset=false;
      }
      //else setsshape.Add(&temp);
    }     
    return setsshape;
  };


  TH3F LTFiles::GetShape3D(std::string filename, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight){
    TH3F temp;
    GetShape3D(temp,filename,variable,selection,category,weight,false);
    return temp;
  }

  TH3F LTFiles::GetSetShape3D(std::string setname, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, const bool do_lumixs_weights_){
    TH3F temp;
    GetSetShape3D(temp,setname,variable,selection,category,weight,do_lumixs_weights_,false);
    return temp;
  }

  TH3F LTFiles::GetSetsShape3D(std::vector<std::string> setnames, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, const bool do_lumixs_weights_){
    TH3F setsshape;
    setsshape.Sumw2();
    bool firstset=true;
    for(unsigned iset=0;iset<setnames.size();iset++){
      GetSetShape3D(setsshape,setnames[iset], variable, selection, category, weight, do_lumixs_weights_,!firstset);
      if(firstset){
	//setsshape=temp;
	firstset=false;
      }
      //else setsshape.Add(&temp);
    }     
    return setsshape;
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




