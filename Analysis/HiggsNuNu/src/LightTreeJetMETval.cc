#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/LightTreeJetMETval.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "TVector3.h"


namespace ic {

  LightTreeJetMETval::LightTreeJetMETval(std::string const& name): ModuleBase(name){
    fs_ = NULL;
    met_label_ = "pfMetType1";
    jet_label_ = "pfJetsPFlow";
    is_data_ = false;
    dotrigskim_ = false;
    do_noskim_ = false;
    ignoreLeptons_=false;
    trig_obj_label_ = "triggerObjectsDiPFJet40PFMETnoMu65MJJ800VBFAllJets";
    trigger_path_ = "HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v";

    outputTree_ = 0;

    run_=-1;
    lumi_=-1;
    event_=-1;
    weight_nolep_=1;
    total_weight_lepveto_ = 1;
    total_weight_leptight_ = 1;
    nJetsSave_ = 25;
    nJets_15_ = 0;
    nGenJets_15_ = 0;
    for (unsigned ij(0); ij<nJetsSave_;++ij){
      jet_pt_.push_back(0);
      jet_E_.push_back(0);
      jet_eta_.push_back(-5);
      jet_phi_.push_back(-5);
      jet_csv_.push_back(-1);
      jet_genjet_mindR_.push_back(99);
      jet_genMatched_.push_back(0);
      jet_jetid_.push_back(-1);
      jet_puid_.push_back(-1);
      jet_genpt_.push_back(0);
      jet_geneta_.push_back(-5);
      jet_genphi_.push_back(-5);
      genjet_pt_.push_back(0);
      genjet_eta_.push_back(-5);
      genjet_phi_.push_back(-5);
    }

    sumet_ = 0;

    l1met_ = 0;
    met_ = 0;
    genmet_ = 0;
    metnomuons_ =0;
    met_x_ = 0;
    met_y_ = 0;
    metnomu_x_ = 0;
    metnomu_y_ = 0;
    met_significance_ = 0;
    metnomu_significance_ = 0;

    pass_sigtrigger_ = -1;
    pass_mettrigger_ = -1;
    pass_controltrigger_ = -1;

    nvetomuons_=0;
    nselmuons_=0;
    nvetoelectrons_=0;
    nselelectrons_=0;
    ntaus_=0;
    m_mumu_=-1;
    m_ee_=-1;
    m_mumu_gen_=-1;
    m_ee_gen_=-1;
    lep_mt_=-1;

    n_vertices_=-1;
  }

  LightTreeJetMETval::~LightTreeJetMETval(){
    ;
  }


  int  LightTreeJetMETval::PreAnalysis(){
    std::cout << "--------------------------------------------- " << std::endl
	      << "------ PreAnalysis Info for LightTreeJetMETval ------" << std::endl
	      << "--------------------------------------------- " << std::endl;
    if (fs_) {
      std::cout << "MET Label: " << met_label_ << std::endl;
      if (is_data_) std::cout << "Processing set for data !" << std::endl;
      else  std::cout << "Processing set for MC !" << std::endl;
    }
    outputTree_=fs_->make<TTree>("LightTreeJetMETval","Tree containing LightTreeAna input variables");//    outputTree_ = new TTree("LightTree","Tree containing LightTreeAna input variables"); 
    outputTree_->Branch("run",&run_);
    outputTree_->Branch("lumi",&lumi_);
    outputTree_->Branch("event",&event_);
    outputTree_->Branch("weight_nolep",&weight_nolep_);
    outputTree_->Branch("total_weight_lepveto",&total_weight_lepveto_);
    outputTree_->Branch("total_weight_leptight",&total_weight_leptight_);

    outputTree_->Branch("nJets_15",&nJets_15_);
    outputTree_->Branch("nGenJets_15",&nGenJets_15_);

    for (unsigned ij(0); ij<nJetsSave_;++ij){
      std::ostringstream label;
      label << "jet" << ij+1;
      outputTree_->Branch((label.str()+"_pt").c_str(),&jet_pt_[ij]);
      outputTree_->Branch((label.str()+"_E").c_str(),&jet_E_[ij]);
      outputTree_->Branch((label.str()+"_eta").c_str(),&jet_eta_[ij]);
      outputTree_->Branch((label.str()+"_phi").c_str(),&jet_phi_[ij]);
      outputTree_->Branch((label.str()+"_csv").c_str(),&jet_csv_[ij]);
      outputTree_->Branch((label.str()+"_jetid").c_str(),&jet_jetid_[ij]);
      outputTree_->Branch((label.str()+"_puid").c_str(),&jet_puid_[ij]);
      outputTree_->Branch((label.str()+"_genjet_mindR").c_str(),&jet_genjet_mindR_[ij]);
      outputTree_->Branch((label.str()+"_genMatched").c_str(),&jet_genMatched_[ij]);
      outputTree_->Branch((label.str()+"_genpt").c_str(),&jet_genpt_[ij]);
      outputTree_->Branch((label.str()+"_geneta").c_str(),&jet_geneta_[ij]);
      outputTree_->Branch((label.str()+"_genphi").c_str(),&jet_genphi_[ij]);
      outputTree_->Branch(("gen"+label.str()+"_pt").c_str(),&genjet_pt_[ij]);
      outputTree_->Branch(("gen"+label.str()+"_eta").c_str(),&genjet_eta_[ij]);
      outputTree_->Branch(("gen"+label.str()+"_phi").c_str(),&genjet_phi_[ij]);
    }

    outputTree_->Branch("sumet",&sumet_);

    outputTree_->Branch("l1met",&l1met_);
    outputTree_->Branch("met",&met_);
    outputTree_->Branch("genmet",&genmet_);
    outputTree_->Branch("metnomuons",&metnomuons_);
    outputTree_->Branch("met_x",&met_x_);
    outputTree_->Branch("met_y",&met_y_);
    outputTree_->Branch("metnomu_x",&metnomu_x_);
    outputTree_->Branch("metnomu_y",&metnomu_y_);
    outputTree_->Branch("met_significance",&met_significance_);
    outputTree_->Branch("metnomu_significance",&metnomu_significance_);


    outputTree_->Branch("pass_sigtrigger",&pass_sigtrigger_);
    outputTree_->Branch("pass_mettrigger",&pass_mettrigger_);
    outputTree_->Branch("pass_controltrigger",&pass_controltrigger_);

    outputTree_->Branch("nvetomuons",&nvetomuons_);
    outputTree_->Branch("nselmuons",&nselmuons_);
    outputTree_->Branch("nvetoelectrons",&nvetoelectrons_);
    outputTree_->Branch("nselelectrons",&nselelectrons_);
    outputTree_->Branch("ntaus",&ntaus_);
    outputTree_->Branch("m_mumu",&m_mumu_);
    outputTree_->Branch("m_ee",&m_ee_);
    outputTree_->Branch("m_mumu_gen",&m_mumu_gen_);
    outputTree_->Branch("m_ee_gen",&m_ee_gen_);
    outputTree_->Branch("lep_mt",&lep_mt_);
 
    outputTree_->Branch("n_vertices",&n_vertices_);

    return 0;
  }

  int  LightTreeJetMETval::Execute(TreeEvent *event){
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    run_= eventInfo->run();
    lumi_= eventInfo->lumi_block();
    event_= eventInfo->event();
    n_vertices_=eventInfo->good_vertices();

    //if (is_data_) {
	
    auto const& triggerPathPtrVec =
      event->GetPtrVec<TriggerPath>("triggerPathPtrVec","triggerPaths");
    //EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo"); //Can be used in future, but commented out to remove compiler warnings      
    //unsigned run = eventInfo->run(); //Can be used in future, but commented out to remove compiler warnings                                         
    pass_sigtrigger_=-1;
    pass_mettrigger_=-1;
    pass_controltrigger_=-1;
    for (unsigned i = 0; i < triggerPathPtrVec.size(); ++i) {
      std::string name = triggerPathPtrVec[i]->name();
      triggerPathPtrVec[i]->prescale();
      if (name.find("HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu140_v") != name.npos) pass_sigtrigger_ = 1;
      if (name.find("HLT_PFMET170_NoiseCleaned_v") != name.npos) pass_mettrigger_ = 1;
      if (name.find("HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu80_v") != name.npos) pass_controltrigger_ = 1;
    }
    if(dotrigskim_){
      if(!(pass_sigtrigger_==1||pass_controltrigger_==1||pass_mettrigger_==1)){
	return 1;
      }
    }
    //}
    //for MC                                                                                                                                       
    // else {
    // 	pass_sigtrigger_=-1;
    // 	pass_controltrigger_=-1;
    // 	std::vector<TriggerObject *> const& sigobjs = event->GetPtrVec<TriggerObject>("triggerObjectsDiPFJet40DEta3p5MJJ600PFMETNoMu140");
    // 	std::vector<TriggerObject *> const& contobjs = event->GetPtrVec<TriggerObject>("triggerObjectsDiPFJet40DEta3p5MJJ600PFMETNoMu80");
    // 	if (sigobjs.size() > 0) pass_sigtrigger_=1;
    // 	if (contobjs.size() > 0) pass_controltrigger_=1;
    // } // do obj match                                                                            
    
    
    double wt = eventInfo->total_weight();
    double vetowt=1;
    double tightwt=1;
    //double pileupwt=1;
    //double topwt=1;
    if(!is_data_){
      vetowt= eventInfo->weight("idisoVeto");
      tightwt = eventInfo->weight("idisoTight");
      //pileupwt=eventInfo->weight("pileup");
      //topwt=eventInfo->weight("tquark_weight");
    }

    //get collections
    Met const* met = event->GetPtr<Met>(met_label_);
    Met const* metnomuons = event->GetPtr<Met>("metNoMuons");
    std::vector<Candidate *> const& l1met = event->GetPtrVec<Candidate>("l1extraMET");
    //!!if(l1met.size()!=1)std::cout<<"There seem to be "<<l1met.size()<<" l1mets!!"<<std::endl;
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jet_label_);
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    std::vector<Muon*> vetomuons=event->GetPtrVec<Muon>("vetoMuons");
    std::vector<Muon*> selmuons=event->GetPtrVec<Muon>("selMuons");
    std::vector<Electron*> vetoelectrons=event->GetPtrVec<Electron>("vetoElectrons");
    std::vector<Electron*> selelectrons=event->GetPtrVec<Electron>("selElectrons");
    std::vector<Tau*> taus=event->GetPtrVec<Tau>("taus");
    std::vector<GenJet *> genvec;
    if(!is_data_){
      genvec= event->GetPtrVec<GenJet>("genJets");
      std::sort(genvec.begin(), genvec.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    }

    if(!ignoreLeptons_) nvetomuons_=vetomuons.size();
    else nvetomuons_=0;
    nselmuons_=selmuons.size();
    nvetoelectrons_=vetoelectrons.size();
    nselelectrons_=selelectrons.size();
    ntaus_=taus.size();

    std::sort(selmuons.begin(), selmuons.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    std::sort(selelectrons.begin(), selelectrons.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));

    //Get MT
    if(nselmuons_==1&&nselelectrons_==0&&ntaus_==0){//If 1 muon and no electrons use muon to make mt
      lep_mt_=sqrt(2*selmuons[0]->pt()*met->pt()*(1-cos(selmuons[0]->phi()-met->phi())));
    }
    else if(nselelectrons_==1&&nselmuons_==0&&ntaus_==0){//If 1 electron and no muons use electron to make mt
      lep_mt_=sqrt(2*selelectrons[0]->pt()*met->pt()*(1-cos(selelectrons[0]->phi()-met->phi())));      
    }
    else if(ntaus_==1&&nselelectrons_==0&&nselmuons_==0){//If 1 electron and no muons use electron to make mt
      lep_mt_=sqrt(2*taus[0]->pt()*met->pt()*(1-cos(taus[0]->phi()-met->phi())));      
    }
    else{//If otherwise set mt to dummy value
      lep_mt_=-2;
    }

    if(nselmuons_==2){
      m_mumu_=((selmuons.at(0)->vector())+(selmuons.at(1)->vector())).M();
    }
    else m_mumu_=-1;

    if(nselelectrons_==2){
      m_ee_=((selelectrons.at(0)->vector())+(selelectrons.at(1)->vector())).M();
    }
    else m_ee_=-1;

    //Get gen z mass
    int ngenmuplus=0;
    int ngenmuminus=0;
    m_mumu_gen_=-1;
    int ngeneplus=0;
    int ngeneminus=0;
    m_ee_gen_=-1;

    genmet_ = 0;
    if(!is_data_){
      std::vector<GenParticle*> const& parts = event->GetPtrVec<GenParticle>("genParticles");
      GenParticle* muplus = 0;
      GenParticle* muminus = 0;
      GenParticle* eplus = 0;
      GenParticle* eminus = 0;

      for (unsigned i = 0; i < parts.size(); ++i) {
	if (parts[i]->status() != 3) continue;
	
	int id = parts[i]->pdgid();

	if (id == static_cast<int>(13)) {
	  muminus = parts[i];
	  ngenmuminus++;
	}
	if (id == static_cast<int>(-13)) {
	  muplus = parts[i];
	  ngenmuplus++;
	}
	if (id == static_cast<int>(11)) {
	  eminus = parts[i];
	  ngeneminus++;
	}
	if (id == static_cast<int>(-11)) {
	  eplus = parts[i];
	  ngeneplus++;
	}  
      }//loop on genparticles                                                                                                                                  
      
      if (ngenmuminus==1&&ngenmuplus==1) {
	m_mumu_gen_ = (muplus->vector()+muminus->vector()).M();
      }

      if (ngeneminus==1&&ngeneplus==1) {
	m_ee_gen_ = (eplus->vector()+eminus->vector()).M();
      }
    }//if MC


    ROOT::Math::PtEtaPhiEVector metvec = met->vector();
    ROOT::Math::PtEtaPhiEVector metnomuvec = metnomuons->vector();

    weight_nolep_ = wt;
    total_weight_lepveto_ =wt*vetowt;
    if(total_weight_lepveto_!=total_weight_lepveto_)std::cout<<"NAN lepveto weight: "<<total_weight_lepveto_<<" "<<wt<<" "<<vetowt<<std::endl;//!!
    total_weight_leptight_=wt*tightwt;
    
    met_ = met->pt();
    met_x_ = metvec.Px();
    met_y_ = metvec.Py();
    met_significance_ = met->et_sig();
    sumet_ = met->sum_et();
    //      if(l1met.size()==1){//!!
    l1met_ = l1met[0]->energy();
    //}
    metnomuons_ = metnomuons->pt();
    metnomu_x_ = metnomuvec.Px();
    metnomu_y_ = metnomuvec.Py();
    metnomu_significance_ = met_significance_/met_*metnomuons_;
    nJets_15_ = 0;

    for (unsigned i = 0; i < jets.size(); ++i) {//loop on jets
      if(jets[i]->pt()<=15) continue;

      jet_pt_[nJets_15_]=jets[i]->pt();
      jet_E_[nJets_15_]=jets[i]->energy();
      jet_eta_[nJets_15_]=jets[i]->eta();
      jet_phi_[nJets_15_]=jets[i]->phi();
      jet_csv_[nJets_15_]=jets[i]->GetBDiscriminator("combinedSecondaryVertexBJetTags");
      jet_jetid_[nJets_15_]=PFJetID2015(jets[i]);
      jet_puid_[nJets_15_]=PileupJetID(jets[i],2);

      if(!is_data_){
	//fill gen matched
	double mindR = 1000;
	unsigned genjetid=0;
	for (unsigned ig = 0; ig < genvec.size(); ++ig) {//loop on genjets
	  double dR = ROOT::Math::VectorUtil::DeltaR(genvec[ig]->vector(),jets[i]->vector());
	  if (dR<mindR){
	    mindR = dR;
	    genjetid = ig;
	  }
	}//loop on genjets
	jet_genjet_mindR_[nJets_15_]=mindR;
	if (mindR<0.5){
	  jet_genMatched_[nJets_15_]=jets[i]->parton_flavour();
	  jet_genpt_[nJets_15_]=genvec[genjetid]->pt();
	  jet_geneta_[nJets_15_]=genvec[genjetid]->eta();
	  jet_genphi_[nJets_15_]=genvec[genjetid]->phi();
	}
      }

      nJets_15_++;
      if (nJets_15_>=nJetsSave_) {
	std::cout << " -- Warning! maximum number of jets reached ! nJets = " << jets.size() << ". Saving only first " << nJetsSave_ << " jets." << std::endl;
	break;
      }
    }//loop on jets

    nGenJets_15_ = 0;
    for (unsigned ig = 0; ig < genvec.size(); ++ig) {//loop on genjets
      if(genvec[ig]->pt()<=15) continue;
      genjet_pt_[nGenJets_15_]=genvec[ig]->pt();
      genjet_eta_[nGenJets_15_]=genvec[ig]->eta();
      genjet_phi_[nGenJets_15_]=genvec[ig]->phi();
      nGenJets_15_++;
      if (nGenJets_15_>=nJetsSave_) {
	std::cout << " -- Warning! maximum number of genjets reached ! nGenJets = " << genvec.size() << ". Saving only first " << nJetsSave_ << " jets." << std::endl;
	break;
      }
    }//loop on genjets

    static unsigned processed = 0;
    //IF PASSES CUTS FILL TREE

    outputTree_->Fill();
    ++processed;
    
    if (processed == 500) outputTree_->OptimizeBaskets();
    
    return 0;
  }
  
  int  LightTreeJetMETval::PostAnalysis(){
    
    return 0;
  }

  void  LightTreeJetMETval::PrintInfo(){
    ;
  }

}//namespace

