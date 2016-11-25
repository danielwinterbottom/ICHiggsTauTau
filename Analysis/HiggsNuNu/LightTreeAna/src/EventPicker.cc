#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/EventPicker.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include <map>
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"
#include "TTreeFormula.h"

namespace ic{

  EventPicker::EventPicker(std::string name) : LTModule(name){
    setorfile_=false;
  };

  EventPicker::~EventPicker(){ ;};

  int EventPicker::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"Samples are: ";
    for(unsigned iVec=0;iVec<sets_.size();iVec++){
      std::cout<<sets_[iVec]<<", "<<std::endl;
    }
    std::cout<<"Variables used are: ";
    for(unsigned iVec=0;iVec<variables_.size();iVec++){
      std::cout<<variables_[iVec]<<", "<<std::endl;
    }
    return 0;
  };

  int EventPicker::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;
    //ADD ALL THE VARIABLES
    Double_t var[variables_.size()];
    //SET LOOP
    for(unsigned iVec=0;iVec<sets_.size();iVec++){
      std::vector<LTFile> files;
      if(setorfile_)files=filemanager->GetFileSet(sets_[iVec]);
      else files.push_back(filemanager->GetFile(sets_[iVec]));
      std::cout<<"Processing "<<sets_[iVec]<<std::endl;
      //FILE LOOP
      for(unsigned iFile=0;iFile<files.size();iFile++){
	//MAKE A TREE TO BE THE FRIEND TREE AND SET UP THE BRANCHES
	std::cout<<"  "<<files[iFile].name()<<std::endl;

        //GET TREE
	files[iFile].Open(filemanager->infolder());
	TTree *ttree_ =files[iFile].GetTree();
	std::vector<TTreeFormula*> formulas;
	TTreeFormula* runnum=new TTreeFormula("run","run",ttree_);
	TTreeFormula* lumisec=new TTreeFormula("lumi","lumi",ttree_);
	TTreeFormula* eventnum=new TTreeFormula("event","event",ttree_);
	//SET UP TTREEFORMULAS
	std::vector<std::string> formulavar;
	for(unsigned iVar=0;iVar<variables_.size();iVar++){
	  if(variables_[iVar].find(":=")!=std::string::npos) formulavar.push_back(variables_[iVar].substr(variables_[iVar].find(":=")+2));
	  else formulavar.push_back(variables_[iVar]);
	  formulas.push_back(new TTreeFormula(variables_[iVar].c_str(),formulavar[iVar].c_str(),ttree_));
	}
	
	//EVENT LOOP
	static unsigned processed = 0;
	for (Long64_t iEvt=0; iEvt<ttree_->GetEntries();iEvt++) {
	  ttree_->GetEntry(iEvt);
	  runnum->EvalInstance(iEvt);
	  for(unsigned irun=0;irun<run_.size();irun++){
	    if(boost::lexical_cast<unsigned>(runnum->PrintValue(0,0,"u"))==run_[irun]){
	      eventnum->EvalInstance(iEvt);
	      if(boost::lexical_cast<unsigned>(eventnum->PrintValue(0,0,"u"))==event_[irun]){
		lumisec->EvalInstance(iEvt);
		if(boost::lexical_cast<unsigned>(lumisec->PrintValue(0,0,"u"))==lumi_[irun]){
		  //OUTPUT STUFF
		  std::cout<<"Run: "<<run_[irun]<<", Lumi: "<<lumi_[irun]<<", Event: "<<event_[irun]<<std::endl;
		  for(unsigned iVar=0;iVar<variables_.size();iVar++){
		    formulas[iVar]->EvalInstance(iEvt);
		    var[iVar]=boost::lexical_cast<double>(formulas[iVar]->PrintValue(0,0,".9f"));
		    std::cout<<"  "<<variables_[iVar]<<"="<<var[iVar]<<std::endl;
		  }
		}
	      }
	    }
	  }
	  processed++;
	}
	//CLEAN UP
	files[iFile].Close();
      }
    }
    return 0;
  };

}
