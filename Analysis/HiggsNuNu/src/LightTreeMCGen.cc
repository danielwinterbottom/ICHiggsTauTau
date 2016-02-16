#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/LightTreeMCGen.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "TVector3.h"


namespace ic {

  LightTreeMCGen::LightTreeMCGen(std::string const& name): ModuleBase(name){
    fs_ = NULL;

    outputTree_ = 0;

    //set vectors, do not reset later!!!
    //Used for setting branch addresses...
    nSave_ = 15;
    nJetsSave_ = 2;

    //initialise arrays fixed size
    pdgid_ = new int[nSave_];
    status_ = new unsigned[nSave_];
    pt_ = new double[nSave_];
    eta_ = new double[nSave_];
    phi_ = new double[nSave_];
    E_ = new double[nSave_];
    m_ = new double[nSave_];

    genjet_pt_ = new double[nJetsSave_];
    genjet_eta_ = new double[nJetsSave_];
    genjet_phi_ = new double[nJetsSave_];
    genjet_E_ = new double[nJetsSave_];

    countHt0_100_ = 0;
    countHt100_200_ = 0;
    countHt200_400_ = 0;
    countHt400_600_ = 0;
    countHt600_inf_ = 0;

    count0Parton_ = 0;
    count1Parton_ = 0;
    count2Parton_ = 0;
    count3Parton_ = 0;
    count4Parton_ = 0;

    resetAllTreeVariables();
  }

  void LightTreeMCGen::resetAllTreeVariables(){
    run_=0;
    lumi_=0;
    event_=0;

    nParticles_ = 0;

    for (unsigned ij(0); ij<nSave_;++ij){
      pdgid_[ij] = 0;
      status_[ij] = 0;
      pt_[ij] = -1;
      eta_[ij] = -5;
      phi_[ij] = -5;
      E_[ij] = -1;
      m_[ij] = -1;
    }

    nGenJets_ = 0;
    for (unsigned ij(0); ij<nJetsSave_;++ij){
      genjet_pt_[ij] = -1;
      genjet_eta_[ij] = -5;
      genjet_phi_[ij] = -5;
      genjet_E_[ij] = -1;
    }

  }

  LightTreeMCGen::~LightTreeMCGen(){
    ;
  }


  int  LightTreeMCGen::PreAnalysis(){
    std::cout << "--------------------------------------------- " << std::endl
	      << "------ PreAnalysis Info for LightTreeMCGen ------" << std::endl
	      << "--------------------------------------------- " << std::endl;

    outputTree_=fs_->make<TTree>("LightTree","Tree containing LightTreeAna input variables");//    outputTree_ = new TTree("LightTree","Tree containing LightTreeAna input variables"); 
    outputTree_->Branch("run",&run_);
    outputTree_->Branch("lumi",&lumi_);
    outputTree_->Branch("event",&event_);
    outputTree_->Branch("nParticles",&nParticles_);

    for (unsigned ij(0); ij<nSave_;++ij){
      std::ostringstream label;
      label << "gen" << ij+1;
      outputTree_->Branch((label.str()+"_pdgid").c_str(),&pdgid_[ij]);
      outputTree_->Branch((label.str()+"_status").c_str(),&status_[ij]);
      outputTree_->Branch((label.str()+"_pt").c_str(),&pt_[ij]);
      outputTree_->Branch((label.str()+"_eta").c_str(),&eta_[ij]);
      outputTree_->Branch((label.str()+"_phi").c_str(),&phi_[ij]);
      outputTree_->Branch((label.str()+"_E").c_str(),&E_[ij]);
      outputTree_->Branch((label.str()+"_M").c_str(),&m_[ij]);
   }

    outputTree_->Branch("nGenJets",&nGenJets_);
    for (unsigned ij(0); ij<nJetsSave_;++ij){
      std::ostringstream label;
      label << "genjet" << ij+1;
      outputTree_->Branch((label.str()+"_pt").c_str(),&genjet_pt_[ij]);
      outputTree_->Branch((label.str()+"_eta").c_str(),&genjet_eta_[ij]);
      outputTree_->Branch((label.str()+"_phi").c_str(),&genjet_phi_[ij]);
      outputTree_->Branch((label.str()+"_E").c_str(),&genjet_E_[ij]);
   }

    return 0;
  }

  int  LightTreeMCGen::Execute(TreeEvent *event){
 
    static unsigned processed = 0;
    
    resetAllTreeVariables();

    ////////////////////////////////
    // Get basic event variables
    ////////////////////////////////

    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    run_= eventInfo->run();
    lumi_= eventInfo->lumi_block();
    event_= eventInfo->event();
    
    double gen_ht  = eventInfo->gen_ht();

    if (gen_ht < 100) countHt0_100_++;
    else if (100 <= gen_ht&&gen_ht <200) countHt100_200_++;
    else if (200 <= gen_ht&&gen_ht <400) countHt200_400_++;
    else if (400 <= gen_ht &&gen_ht<600) countHt400_600_++;
    else if (gen_ht >= 600) countHt600_inf_++;

    ////////////////////////////////
    // Get GenLevel collections
    ////////////////////////////////

    std::vector<GenParticle*> const& parts = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<Candidate*> genLeptons;

    //std::cout << " -- Event: " << event_ << " print of hardProcess particles..." << std::endl;
    unsigned partons = 0;

    for (unsigned iGenPart = 0; iGenPart < parts.size(); ++iGenPart) {//Loop over gen particles
      int id = parts[iGenPart]->pdgid();
      std::vector<bool> flags=parts[iGenPart]->statusFlags();
      if (flags[GenStatusBits::IsHardProcess] && 
	  flags[GenStatusBits::FromHardProcess] &&
	  flags[GenStatusBits::IsFirstCopy]
	  ){
	if (abs(id) == 1 || abs(id) == 2 || abs(id) == 3 || abs(id) == 4 || abs(id) == 5 || abs(id) == 6 || abs(id) == 21) partons++;

	if (abs(id)==11 || abs(id)==13 || abs(id)==15){
	  genLeptons.push_back(parts[iGenPart]);
	}
	//parts[iGenPart]->Print();
	if (nParticles_<nSave_){
	  pdgid_[nParticles_] = id;
	  status_[nParticles_] = parts[iGenPart]->status();
	  pt_[nParticles_] = parts[iGenPart]->pt();
	  eta_[nParticles_] = parts[iGenPart]->eta();
	  phi_[nParticles_] = parts[iGenPart]->phi();
	  E_[nParticles_] = parts[iGenPart]->energy();
	  m_[nParticles_] = parts[iGenPart]->M();
	  nParticles_++;
	}
	else {
	  std::cout << " -- Warning, file " << __FILE__ << " line " << __LINE__ << " found more than " << nSave_ << " particles passing selection. Increase nSave_ ?" << std::endl;
	}
      }//for MC
    }//Loop over gen particles


    if (partons == 0) count0Parton_++;
    else if (partons == 1) count1Parton_++;
    else if (partons == 2) count2Parton_++;
    else if (partons == 3) count3Parton_++;
    else if (partons >= 4) count4Parton_++;

    std::vector<GenJet *> genvec= event->GetPtrVec<GenJet>("genJets");
    std::sort(genvec.begin(), genvec.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    nGenJets_ = 0;

    //clean from leptons from V....
    std::vector<std::pair<unsigned,bool> > gentoparton;
    gentoparton.resize(genvec.size(),std::pair<unsigned,bool>(1000,false));
    getGenRecoMatches<GenJet,Candidate>(genvec,genLeptons,gentoparton,0.3);

    for (unsigned i = 0; i < genvec.size(); ++i) {//loop on genjets
      if (gentoparton[i].second) continue;
      if ( nGenJets_<nJetsSave_){
	genjet_pt_[nGenJets_] = genvec[i]->pt();
	genjet_eta_[nGenJets_] = genvec[i]->eta();
	genjet_phi_[nGenJets_] = genvec[i]->phi();
	genjet_E_[nGenJets_] = genvec[i]->energy();
	nGenJets_++;
      }
      //else {
	//std::cout << " -- Warning, file " << __FILE__ << " line " << __LINE__ << " found more than " << nJetsSave_ << " genjets passing selection. Increase nJetsSave_ ?" << std::endl;
      //}
    }//loop on genjets



    outputTree_->Fill();
    ++processed;
    //if (processed == 500) outputTree_->OptimizeBaskets();
    if ((processed%1000000) == 0) outputTree_->AutoSave();

    return 0;


  }//execute method
  
  int  LightTreeMCGen::PostAnalysis(){

    fs_->cd();
    outputTree_->Write();

    std::cout << "----------------------------------------" << std::endl
	      << "PostAnalysis Info for LightTreeMCGen" << std::endl
	      << "----------------------------------------" << std::endl
	      << " hT<100: " << countHt0_100_ << std::endl
	      << " 100<ht<200: " << countHt100_200_ << std::endl
	      << " 200<ht<400: " << countHt200_400_ << std::endl
	      << " 400<ht<600: " << countHt400_600_ << std::endl
	      << " ht>600: " << countHt600_inf_ << std::endl
	      << " 0 parton: " << count0Parton_ << std::endl
	      << " 1 parton: " << count1Parton_ << std::endl
	      << " 2 parton: " << count2Parton_ << std::endl
	      << " 3 parton: " << count3Parton_ << std::endl
	      << " 4 parton: " << count4Parton_ << std::endl;

    return 0;
  }

  void  LightTreeMCGen::PrintInfo(){
    ;
  }


}//namespace

