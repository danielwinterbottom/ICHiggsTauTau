#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/MVAApply.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include <map>
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"
#include "TTreeFormula.h"

namespace ic{

  MVAApply::MVAApply(std::string name) : LTModule(name){
    weightDir_="weights";
    friendDir_="friends";
  };

  MVAApply::~MVAApply(){ ;};

  int MVAApply::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"Signal samples are: ";
    for(unsigned iVec=0;iVec<sigsets_.size();iVec++){
      std::cout<<sigsets_[iVec]<<", "<<std::endl;
    }
    std::cout<<"Background samples are: ";
    for(unsigned iVec=0;iVec<bkgsets_.size();iVec++){
      std::cout<<bkgsets_[iVec]<<", "<<std::endl;
    }
    std::cout<<"Variables used are: ";
    for(unsigned iVec=0;iVec<variables_.size();iVec++){
      std::cout<<variables_[iVec]<<", "<<std::endl;
    }
    return 0;
  };

  int MVAApply::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;
    // MAKE TMVA READER                                                                                                              
    TMVA::Reader *reader = new TMVA::Reader( "Color:!Silent" );

    //ADD ALL THE VARIABLES
    Double_t var[variables_.size()];
    for(unsigned iVar=0;iVar<variables_.size();iVar++){
      reader->AddVariable(variables_[iVar].c_str(),(Float_t*)&var[iVar]);
    }

    //BOOK METHODS
    for(unsigned iMethod=0;iMethod<methodNames_.size();iMethod++){
      reader->BookMVA( methodNames_[iMethod].c_str(), (weightDir_+"/"+weightFiles_[iMethod]).c_str() );
    }
    //SET LOOP
    for(unsigned iVec=0;iVec<(bkgsets_.size()+sigsets_.size());iVec++){
      std::vector<LTFile> files;
      std::string setname;
      if(iVec<bkgsets_.size()) setname=bkgsets_[iVec];
      else setname=sigsets_[iVec-bkgsets_.size()];
      files=filemanager->GetFileSet(setname);
      std::cout<<"Processing "<<setname<<std::endl;
      //FILE LOOP
      for(unsigned iFile=0;iFile<files.size();iFile++){
	//MAKE A TREE TO BE THE FRIEND TREE AND SET UP THE BRANCHES
	std::cout<<"  "<<files[iFile].name()<<std::endl;
	TFile* friendfile=new TFile((friendDir_+files[iFile].name()+"_mvafriend.root").c_str(),"RECREATE");
	TTree* friendtree=new TTree("mvafriend","Friend to Light Trees to store MVA values");
	double mvavalues[methodNames_.size()];
	for(unsigned iMethod=0;iMethod<methodNames_.size();iMethod++){
	  friendtree->Branch(methodNames_[iMethod].c_str(),&mvavalues[iMethod]);
	}

        //GET TREE
	files[iFile].Open(filemanager->infolder());
	TTree *ttree_ =files[iFile].GetTree();
	std::vector<TTreeFormula*> formulas;

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
	  
	  for(unsigned iVar=0;iVar<variables_.size();iVar++){
	    formulas[iVar]->EvalInstance(iEvt);
	    var[iVar]=boost::lexical_cast<double>(formulas[iVar]->PrintValue(0,0,".9f"));
	  }
	  
          //EVALUATE MVA
	  for(unsigned iMethod=0;iMethod<methodNames_.size();iMethod++){
	    mvavalues[iMethod]=reader->EvaluateMVA(methodNames_[iMethod]);
	  }
	  //WRITE TO THE FRIEND TREE
	  friendtree->Fill();
	  ++processed;
	  if (processed == 500) friendtree->OptimizeBaskets();
	}
	//CLEAN UP
	friendfile->cd();
	friendtree->Write();
	friendfile->Write();
	friendfile->Close();
	files[iFile].AddFriend("mvafriend",(friendDir_+files[iFile].name()+"_mvafriend.root").c_str());
	files[iFile].Close();
      }
    }
    return 0;
  };

}
