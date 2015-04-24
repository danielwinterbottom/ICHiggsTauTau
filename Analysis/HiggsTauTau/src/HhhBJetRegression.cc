#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HhhBJetRegression.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "TMVA/Reader.h"
#include "Math/VectorUtil.h"
#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "Core/interface/TreeEvent.h"
#include <vector>

namespace ic {

  HhhBJetRegression::HhhBJetRegression(std::string const& name) : ModuleBase(name) {
    jets_label_ = "pfJetsPFlow";
    regression_mva_file_ = "input/Hhh_regression_mva/factoryJetRegNewGenJetsAll_BDT.weights.xml";
    regression_reader_ = nullptr;
  }

  HhhBJetRegression::~HhhBJetRegression() {
    ;
  }

  int HhhBJetRegression::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HhhBJetRegression" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
    std::cout << boost::format(param_fmt()) % "jets_label"  % jets_label_;
    std::cout << boost::format(param_fmt()) % "regression_mva_file"     % regression_mva_file_;
    //regression_reader_ = new TMVA::Reader("!Color:!Silent:Error");
    regression_reader_ = new TMVA::Reader();
    std::vector<TMVA::Reader *> readers = {regression_reader_};
    for (auto & reader : readers) {
	  reader->AddVariable("jetBtag", &prebjet_bcsv );
	  reader->AddVariable("jetPt", &prebjet_pt );
      reader->AddVariable("jetEta", &prebjet_eta );
	  reader->AddVariable("jetChf", &prebjet_chf );
	  reader->AddVariable("jetPhf", &prebjet_phf );
	  reader->AddVariable("jetNhf", &prebjet_nhf );
	  reader->AddVariable("jetElf", &prebjet_elf );
	  reader->AddVariable("jetMuf", &prebjet_muf );
	  reader->AddSpectator("jetPt",  &prebjet_pt);
	  reader->AddSpectator("jetGenPt",  &prebjet_genPt );
	  reader->AddSpectator("jetEta",  &prebjet_eta);
    }
    regression_reader_->BookMVA("BDT", regression_mva_file_);
    return 0;
  }
  
  int HhhBJetRegression::Execute(TreeEvent *event) {
  
    std::vector<PFJet*> prebjets = event->GetPtrVec<PFJet>(jets_label_);
    std::vector<PFJet> prebjets_clone;
    for (unsigned i=0; i<prebjets.size(); ++i) {
      prebjets_clone.push_back(*(prebjets[i]));
    }

    std::vector<PFJet>::iterator it;
    std::vector<PFJet> vec_out;
    std::vector<PFJet*> ptr_vec_out;
    for(it=prebjets_clone.begin(); it!=prebjets_clone.end(); ++it) { 
      prebjet_bcsv   =0;
      prebjet_pt     =0;
      prebjet_genPt  =0;
      prebjet_eta    =0;
      prebjet_chf    =0;
      prebjet_elf    =0;
      prebjet_muf    =0;
      prebjet_nhf    =0;
      prebjet_phf    =0;

      prebjet_bcsv   = (*it).GetBDiscriminator("combinedSecondaryVertexBJetTags");
      prebjet_pt     = (*it).pt();
      prebjet_eta    = (*it).eta();
      prebjet_chf    = (*it).charged_had_energy_frac();
      prebjet_elf    = (*it).electron_energy_frac();
      prebjet_muf    = (*it).muon_energy_frac();
      prebjet_nhf    = (*it).neutral_had_energy_frac();
      prebjet_phf    = (*it).photon_energy_frac();
          
      double SF = regression_reader_->EvaluateMVA("BDT");
      double pt_corr = prebjet_pt*SF;
      double E_corr = (*it).energy()*SF;
      (*it).set_pt(pt_corr);
      (*it).set_energy(E_corr);
      vec_out.push_back(*it); 
    }
    //Set of corrected jets saved as a new collection. Each jet will exist even though the correction is only valid for pt>20.
    event->Add(jets_label_+"CorrectedProduct", vec_out);
    std::vector<PFJet> &vec_in = event->Get<std::vector<PFJet> >(jets_label_+"CorrectedProduct");
    ptr_vec_out.resize(vec_in.size());
    for (unsigned i = 0; i < vec_in.size(); ++i) {
      ptr_vec_out[i] = &(vec_in[i]);
    }
    event->Add(jets_label_+"Corrected", ptr_vec_out);
    
    return 0;
  }

  int HhhBJetRegression::PostAnalysis() {
    if (regression_reader_) delete regression_reader_;
    return 0;
  }

  void HhhBJetRegression::PrintInfo() {
    ;
  }

}
