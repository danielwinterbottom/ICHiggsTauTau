#include "TH1F.h"
#include "TFile.h"
#include <iostream>
#include <vector>
#include <map>
double Integral(TH1F const* hist) {
  if (hist) {
    double ltmp =hist->Integral(0, hist->GetNbinsX() + 1);
    if (ltmp<0 || ltmp != ltmp) {
      std::cout << " -- Warning: integral is " << ltmp << ". Setting to 0." << std::endl;
      ltmp=0;
    }
    return ltmp;
  }
  else return 0;
}

double Error(TH1F const* hist) {
  double err = 0.0;
  if (hist) {
    hist->IntegralAndError(0, hist->GetNbinsX() + 1, err);
    if (err<0 || err != err) {
      std::cout << " -- Warning: error on integral is " << err << ". Setting to 0." << std::endl;
      err=0;
    }
  }
  return err;
}

int extractTrigEff(){//main
  std::string folder = "../output_trigeff/nunu/";


  std::vector<std::string> files;
  files.push_back("SingleMu_SingleMu-2012A-22Jan2013-v1"); 
  files.push_back("SingleMu_SingleMu-2012B-22Jan2013-v1"); 
  files.push_back("SingleMu_SingleMu-2012C-22Jan2013-v1"); 
  files.push_back("SingleMu_SingleMu-2012D-22Jan2013-v1"); 

  std::vector<std::string> selections;
  selections.push_back("pretrig_CJVpass");
  selections.push_back("pretrig_CJVfail");
  selections.push_back("pretrig_nocjv");
  selections.push_back("posttrig_CJVpass");
  selections.push_back("posttrig_CJVfail");
  selections.push_back("posttrig_nocjv");

  std::vector<std::string> selectionsdir = selections;
  
  std::vector<std::string> mets;
  mets.push_back("40");
  mets.push_back("60");
  mets.push_back("80");
  mets.push_back("100");
  mets.push_back("110");
  mets.push_back("120");
  mets.push_back("130");
  mets.push_back("140");
  mets.push_back("150");
  mets.push_back("160");
  mets.push_back("170");
  mets.push_back("180");
  mets.push_back("190");
  mets.push_back("200");

  std::map<std::string, TFile *> tfiles;
  for (unsigned iFile = 0; iFile < files.size(); iFile++) {
    for (unsigned iMet =0;iMet<mets.size(); iMet++){
      std::string filename = (files[iFile]+"_MET"+mets[iMet]+".root");
      TFile * tmp = new TFile((folder+"/"+filename).c_str());
      if (!tmp) {
       	std::cerr << "Warning, file " << filename << " could not be opened." << std::endl;
      }
      else {
	tfiles[(files[iFile]+"MET"+mets[iMet])] = tmp;
      }
    }
  }

  double nevents[4][6][mets.size()];

  for (unsigned iFile = 0; iFile < files.size(); ++iFile) {
    for (unsigned iMet=0;iMet<mets.size(); ++iMet){
      for (unsigned iSel = 0; iSel < selectionsdir.size(); ++iSel) {
	
	TH1F* n_jets_hist;
	
	if(!(tfiles[(files[iFile]+"MET"+mets[iMet])]->GetDirectory(("/"+selectionsdir[iSel]).c_str()))){      
	  std::cout<<"No n_jets hist in directory: "<<selectionsdir[iSel]<<" Problem!"<<std::endl;
	  continue;
	}
	tfiles[(files[iFile]+"MET"+mets[iMet])]->GetObject(("/"+selectionsdir[iSel]+"/n_jets").c_str(),n_jets_hist);
	nevents[iFile][iSel][iMet]=Integral(n_jets_hist);
	//std::cout<<nevents[iFile][iSel]<<std::endl;
      }
    }
  }
  
  bool nostat[4][3][mets.size()];
  double trigeff[4][3][mets.size()];
  double trigefferr[4][3][mets.size()];

  std::string eraname[4]={"A","B","C","D"};
  std::string cjvname[3]={"Passing","Failing","No"};

  for (unsigned iEra = 0;iEra<4;iEra++){
    std::cout<<"Run "<<eraname[iEra]<<":"<<std::endl;
    for (unsigned iCJV=0;iCJV<3;iCJV++){
      std::cout<<"   Events "<<cjvname[iCJV]<<" CJV:"<<std::endl;
      for (unsigned iMet=0;iMet<mets.size();iMet++){
	trigeff[iEra][iCJV][iMet]=nevents[iEra][iCJV+3][iMet]/nevents[iEra][iCJV][iMet];
	trigefferr[iEra][iCJV][iMet]=sqrt(pow(sqrt(nevents[iEra][iCJV+3][iMet])/nevents[iEra][iCJV+3][iMet],2)+pow(sqrt(nevents[iEra][iCJV][iMet])/nevents[iEra][iCJV][iMet],2))*trigeff[iEra][iCJV][iMet];

	if(nevents[iEra][iCJV][iMet]==0) nostat[iEra][iCJV][iMet]=true;
	else nostat[iEra][iCJV][iMet]=false;

	//Output
	if(nostat[iEra][iCJV][iMet]==false){
	  if(iMet<(mets.size()-1)){
	    std::cout<<"      Trigger efficiency for MET "<<mets[iMet]<<"-"<<mets[iMet+1]<<" is: "<<trigeff[iEra][iCJV][iMet]<<"+/-"<<trigefferr[iEra][iCJV][iMet]<<std::endl;
	  }
	  else{
	    std::cout<<"      Trigger efficiency for MET "<<mets[iMet]<<"-inf"<<" is: "<<trigeff[iEra][iCJV][iMet]<<"+/-"<<trigefferr[iEra][iCJV][iMet]<<std::endl;
	  }
	}
	else{
	  if(iMet<(mets.size()-1)){
	    std::cout<<"      Not enough stat for MET "<<mets[iMet]<<"-"<<mets[iMet+1]<<std::endl;
	  }
	  else{
	    std::cout<<"      Not enough stat for MET "<<mets[iMet]<<"-inf"<<std::endl;
	  }
	}
	std::cout<<"       Num = "<<nevents[iEra][iCJV+3][iMet]<<", Den = "<<nevents[iEra][iCJV][iMet]<<std::endl;
      }
    }
  }
  return 0;
}
