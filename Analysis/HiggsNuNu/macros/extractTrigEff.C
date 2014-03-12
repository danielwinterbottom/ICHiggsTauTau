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
  std::string folder = "../trigeffoutput/nunu/";


  std::vector<string> files;
  files.push_back("SingleMu_SingleMu-2012A-22Jan2013-v1"); 
  files.push_back("SingleMu_SingleMu-2012B-22Jan2013-v1"); 
  files.push_back("SingleMu_SingleMu-2012C-22Jan2013-v1"); 
  files.push_back("SingleMu_SingleMu-2012D-22Jan2013-v1"); 

  std::vector<string> selections;
  selections.push_back("pretrig_CJVpass");
  selections.push_back("pretrig_CJVfail");
  selections.push_back("pretrig_nocjv");
  selections.push_back("posttrig_CJVpass");
  selections.push_back("posttrig_CJVfail");
  selections.push_back("posttrig_nocjv");

  std::vector<string> selectionsdir = selections;

  std::map<std::string, TFile *> tfiles;
  for (unsigned i = 0; i < files.size(); ++i) {
    std::string filename = (files[i]+".root");
    TFile * tmp = new TFile((folder+"/"+filename).c_str());
    if (!tmp) {
      std::cerr << "Warning, file " << filename << " could not be opened." << std::endl;
    } else {
      tfiles[files[i]] = tmp;
    }
  }

  bool skip[selectionsdir.size()];
  for (unsigned k = 0; k < selectionsdir.size(); ++k) {
    skip[k] = false;
  }

  double nevents[4][6];
  for (unsigned iFile = 0; iFile < files.size(); ++iFile) {
    for (unsigned iSel = 0; iSel < selectionsdir.size(); ++iSel) {

      TH1F* n_jets_hist;

      if(!(tfiles[files[iFile]]->GetDirectory(("/"+selectionsdir[iSel]).c_str()))){      
	std::cout<<"No n_jets hist in directory: "<<selectionsdir[iSel]<<" Problem!"<<std::endl;
 	continue;
      }
      tfiles[files[iFile]]->GetObject(("/"+selectionsdir[iSel]+"/n_jets").c_str(),n_jets_hist);
      nevents[iFile][iSel]=Integral(n_jets_hist);
      std::cout<<nevents[iFile][iSel]<<std::endl;
    }
  }
  return 0;
}
