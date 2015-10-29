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
    met_label_ = "pfMetType1Collection";
    jet_label_ = "pfJetsPFlow";
    is_data_ = false;
    dotrigskim_ = false;
    do_noskim_ = false;
    ignoreLeptons_=false;
    trig_obj_label_ = "triggerObjectsDiPFJet40PFMETnoMu65MJJ800VBFAllJets";
    trigger_path_ = "HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v";

    outputTree_ = 0;
    hetavspt_genall_ = 0;
    hetavspt_genmatched_ = 0;
    hetavspt_recmatched_ = 0;

    run_=-1;
    lumi_=-1;
    event_=-1;
    weight_nolep_=1;
    total_weight_lepveto_ = 1;
    total_weight_leptight_ = 1;
    nJetsSave_ = 50;
    nJets_ = 0;
    nGenJets_ = 0;
    for (unsigned ij(0); ij<nJetsSave_;++ij){
      jet_pt_.push_back(0);
      jet_E_.push_back(0);
      jet_eta_.push_back(-5);
      jet_phi_.push_back(-5);
      jet_csv_.push_back(-1);
      jet_genjet_mindR_.push_back(99);
      jet_flavour_.push_back(0);
      jet_jetid_.push_back(-1);
      jet_puid_.push_back(-1);
      jet_genid_.push_back(1000);
      jet_genpt_.push_back(0);
      jet_geneta_.push_back(-5);
      jet_genphi_.push_back(-5);
      genjet_pt_.push_back(0);
      genjet_eta_.push_back(-5);
      genjet_phi_.push_back(-5);
      genjet_jet_mindR_.push_back(99);
      genjet_recid_.push_back(1000);
      genjet_recpt_.push_back(0);
      genjet_receta_.push_back(-5);
      genjet_recphi_.push_back(-5);
    }

    sumet_ = 0;

    l1met_ = 0;
    met_ = 0;
    genmet_ = 0;
    genmetphi_ = 0;
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
    hetavspt_genall_ = fs_->make<TH2D>("hetavsptgenall",";pT (GeV); #eta",100,0,500,100,-5,5);
    hetavspt_genmatched_ = fs_->make<TH2D>("hetavsptgenmatched",";pT (GeV); #eta",100,0,500,100,-5,5);
    hetavspt_recmatched_ = fs_->make<TH2D>("hetavsptrecmatched",";pT (GeV); #eta",100,0,500,100,-5,5);
    outputTree_=fs_->make<TTree>("LightTreeJetMETval","Tree containing LightTreeAna input variables");//    outputTree_ = new TTree("LightTree","Tree containing LightTreeAna input variables"); 
    outputTree_->Branch("run",&run_);
    outputTree_->Branch("lumi",&lumi_);
    outputTree_->Branch("event",&event_);
    outputTree_->Branch("weight_nolep",&weight_nolep_);
    outputTree_->Branch("total_weight_lepveto",&total_weight_lepveto_);
    outputTree_->Branch("total_weight_leptight",&total_weight_leptight_);

    outputTree_->Branch("nJets",&nJets_);
    outputTree_->Branch("nGenJets",&nGenJets_);

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
      outputTree_->Branch((label.str()+"_flavour").c_str(),&jet_flavour_[ij]);
      outputTree_->Branch((label.str()+"_genid").c_str(),&jet_genid_[ij]);
      outputTree_->Branch((label.str()+"_genpt").c_str(),&jet_genpt_[ij]);
      outputTree_->Branch((label.str()+"_geneta").c_str(),&jet_geneta_[ij]);
      outputTree_->Branch((label.str()+"_genphi").c_str(),&jet_genphi_[ij]);
      outputTree_->Branch(("gen"+label.str()+"_pt").c_str(),&genjet_pt_[ij]);
      outputTree_->Branch(("gen"+label.str()+"_eta").c_str(),&genjet_eta_[ij]);
      outputTree_->Branch(("gen"+label.str()+"_phi").c_str(),&genjet_phi_[ij]);
     outputTree_->Branch(("gen"+label.str()+"_jet_mindR").c_str(),&genjet_jet_mindR_[ij]);
       outputTree_->Branch(("gen"+label.str()+"_recid").c_str(),&genjet_recid_[ij]);
       outputTree_->Branch(("gen"+label.str()+"_recpt").c_str(),&genjet_recpt_[ij]);
      outputTree_->Branch(("gen"+label.str()+"_receta").c_str(),&genjet_receta_[ij]);
      outputTree_->Branch(("gen"+label.str()+"_recphi").c_str(),&genjet_recphi_[ij]);
   }

    outputTree_->Branch("sumet",&sumet_);

    outputTree_->Branch("l1met",&l1met_);
    outputTree_->Branch("met",&met_);
    outputTree_->Branch("genmet",&genmet_);
    outputTree_->Branch("genmetphi",&genmetphi_);
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
    std::vector<Met*> metCol = event->GetPtrVec<Met>(met_label_);
    Met const* met = metCol[0];
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
    genmetphi_ = 0;
    if(!is_data_){

      std::vector<Met*> GenMet = event->GetPtrVec<Met>("genMetCollection");
      genmet_ = GenMet[0]->pt();
      genmetphi_ = GenMet[0]->phi();

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
    nJets_ = 0;

    std::vector<std::pair<unsigned,bool> > recotogenmatch;
    recotogenmatch.resize(jets.size(),std::pair<unsigned,bool>(1000,false));
    std::vector<std::pair<unsigned,bool> > gentorecomatch;
    gentorecomatch.resize(genvec.size(),std::pair<unsigned,bool>(1000,false));

    unsigned recotogenfilled = 0;
    unsigned gentorecofilled = 0;
    getGenRecoMatches(jets,genvec,recotogenmatch,gentorecomatch,true);

    for (unsigned i = 0; i < jets.size(); ++i) {//loop on jets
      //if(jets[i]->pt()<=15) continue;
      jet_pt_[nJets_]=jets[i]->pt();
      jet_E_[nJets_]=jets[i]->energy();
      jet_eta_[nJets_]=jets[i]->eta();
      jet_phi_[nJets_]=jets[i]->phi();
      jet_csv_[nJets_]=jets[i]->GetBDiscriminator("combinedSecondaryVertexBJetTags");
      jet_jetid_[nJets_]=PFJetID2015(jets[i]);
      jet_puid_[nJets_]=PileupJetID(jets[i],2);
      jet_flavour_[nJets_]=jets[i]->parton_flavour();

      if (!is_data_ && recotogenmatch[i].second){
	unsigned genid = recotogenmatch[i].first;
	jet_genjet_mindR_[nJets_]=ROOT::Math::VectorUtil::DeltaR(genvec[genid]->vector(),jets[i]->vector());
	jet_genid_[nJets_]=genid;
	jet_genpt_[nJets_]=genvec[genid]->pt();
	jet_geneta_[nJets_]=genvec[genid]->eta();
	jet_genphi_[nJets_]=genvec[genid]->phi();
	if (jet_genjet_mindR_[nJets_]<0.5 && genvec[genid]->pt()>30 && fabs(genvec[genid]->eta())<4.6) {
	  hetavspt_recmatched_->Fill(genvec[genid]->pt(),genvec[genid]->eta());
	  recotogenfilled++;
	}
	
	/*if (event_ == 139483659) {
	  if (i==0) std::cout << " Event " << event_ << std::endl;
	  std::cout  << " -- rec jet " << i << " mindR = " << jet_genjet_mindR_[nJets_] ;
	  jets[i]->Print();
	  std::cout << " -- genjetid" << genid << ": ";
	  genvec[genid]->Print();
	  }*/
      }

      nJets_++;
      if (nJets_>=nJetsSave_) {
	std::cout << " -- Warning! maximum number of jets reached ! nJets = " << jets.size() << ". Saving only first " << nJetsSave_ << " jets." << std::endl;
	break;
      }
    }//loop on jets

    if(!is_data_){
      nGenJets_ = 0;
      for (unsigned ig = 0; ig < genvec.size(); ++ig) {//loop on genjets
	//if(genvec[ig]->pt()<=15) continue;
	

	/*if (event_ == 139483659){
	  if (ig==0) std::cout << " Event " << event_ << std::endl;
	  std::cout<< " -- genjet = " << ig << ": " ;
	  genvec[ig]->Print();
	  std::cout << " -- gentorecomatch = " << gentorecomatch[ig].first << ", mindR = " << ROOT::Math::VectorUtil::DeltaR(genvec[ig]->vector(),jets[gentorecomatch[ig].first]->vector()) << ": ";
	  jets[gentorecomatch[ig].first]->Print();
	  }*/
	
	if (genvec[ig]->pt()>30 && fabs(genvec[ig]->eta())<4.6){
	  hetavspt_genall_->Fill(genvec[ig]->pt(),genvec[ig]->eta());
	}


	/*if (genvec[ig]->pt()>30 && fabs(genvec[ig]->eta())<4.6 && (mindR>=0.4 || (mindR<0.4 && jets[jetid]->pt()<=15))){
	  std::cout << " -- gen with no match: dRmin = " << mindR << std::endl
	  << " -- genjet: ";
	  genvec[ig]->Print();
	  std::cout << std::endl << " -- recojet: id " << jetid << " ** ";
	  jets[jetid]->Print();
	  std::cout << std::endl;
	  }*/
	
	genjet_pt_[nGenJets_]=genvec[ig]->pt();
	genjet_eta_[nGenJets_]=genvec[ig]->eta();
	genjet_phi_[nGenJets_]=genvec[ig]->phi();

	if (gentorecomatch[ig].second){
	  unsigned recid = gentorecomatch[ig].first;
	  genjet_jet_mindR_[nGenJets_]=ROOT::Math::VectorUtil::DeltaR(genvec[ig]->vector(),jets[recid]->vector());
	  if (genvec[ig]->pt()>30 && fabs(genvec[ig]->eta())<4.6){
	    if (genjet_jet_mindR_[nGenJets_]<0.5){
	      hetavspt_genmatched_->Fill(genvec[ig]->pt(),genvec[ig]->eta());
	      gentorecofilled++;
	    }
	  }
	  genjet_recid_[nGenJets_]=recid;
	  genjet_recpt_[nGenJets_]=jets[recid]->pt();
	  genjet_receta_[nGenJets_]=jets[recid]->eta();
	  genjet_recphi_[nGenJets_]=jets[recid]->phi();
	}

	nGenJets_++;
	if (nGenJets_>=nJetsSave_) {
	  std::cout << " -- Warning! maximum number of genjets reached ! nGenJets = " << genvec.size() << ". Saving only first " << nJetsSave_ << " jets." << std::endl;
	  break;
	}
      }//loop on genjets
    }

    if (recotogenfilled!=gentorecofilled) {
      std::cout << " Event " << event_ << ", found diff numbers: recotogenfilled = " << recotogenfilled << ", gentorecofilled = " << gentorecofilled << std::endl;

      /*for (unsigned i = 0; i < jets.size(); ++i) {//loop on jets
	recotogenmatch[i] = std::pair<unsigned,bool>(1000,false);
      }
      for (unsigned ig = 0; ig < genvec.size(); ++ig) {//loop on genjets
	gentorecomatch[ig] = std::pair<unsigned,bool>(1000,false);
	}*/
      //getGenRecoMatches(jets,genvec,recotogenmatch,gentorecomatch,false,true);
 

      /*for (unsigned i = 0; i < jets.size(); ++i) {//loop on jets
	std::cout << " rec " << i ;
	if (recotogenmatch[i].second==false){
	  std::cout << " no match "; jets[i]->Print();
	  continue;
	}
	std::cout << " match " << recotogenmatch[i].first << " gen-reco match " << gentorecomatch[recotogenmatch[i].first].first << " dr=" << ROOT::Math::VectorUtil::DeltaR(genvec[recotogenmatch[i].first]->vector(),jets[i]->vector()) << std::endl;
	std::cout << " reco "; jets[i]->Print();
	std::cout << " gen "; genvec[recotogenmatch[i].first]->Print();
      }

      for (unsigned ig = 0; ig < genvec.size(); ++ig) {//loop on genjets
	std::cout << " gen " << ig;
	if (gentorecomatch[ig].second==false){
	  std::cout << " no match "; 
	  genvec[ig]->Print();
	  continue;
	}

	std::cout << " match " << gentorecomatch[ig].first << " reco-gen match " << recotogenmatch[gentorecomatch[ig].first].first  << " dr=" << ROOT::Math::VectorUtil::DeltaR(jets[gentorecomatch[ig].first]->vector(),genvec[ig]->vector()) << std::endl;
	std::cout << " gen "; genvec[ig]->Print();
	std::cout << " reco "; jets[gentorecomatch[ig].first]->Print();
      }
      */
      //exit(1);
    }

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

  void LightTreeJetMETval::getGenRecoMatches(const std::vector<PFJet*> & jets,const std::vector<GenJet *> & genvec,std::vector<std::pair<unsigned,bool> > & recotogenmatch,std::vector<std::pair<unsigned,bool> > & gentorecomatch, bool firstTime, bool print){

    if (is_data_) return;
    
    unsigned nRecNotMatched = 0;
    for (unsigned i = 0; i < jets.size(); ++i) {//loop on jets
      
      if (print) std::cout << " - jet " << i << ": ";
      if (recotogenmatch[i].second==true && gentorecomatch[recotogenmatch[i].first].second==true) {
	continue;
      }
      double mindR = 1000;
      unsigned genjetid=1000;
      for (unsigned ig = 0; ig < genvec.size(); ++ig) {//loop on genjets
	if (!firstTime && gentorecomatch[ig].second==true && 
	    recotogenmatch[gentorecomatch[ig].first].second==true){
	    //if (recotogenmatch[gentorecomatch[ig].first].second==false) recotogenmatch[gentorecomatch[ig].first]=std::pair<unsigned,bool>(ig,true);
	  continue;
	}
	//double dphi = ROOT::Math::VectorUtil::DeltaPhi(genvec[ig]->vector(),jets[i]->vector());
	//double deta = fabs(genvec[ig]->eta()-jets[i]->eta());
	double dR = ROOT::Math::VectorUtil::DeltaR(genvec[ig]->vector(),jets[i]->vector());
	if (dR<mindR){
	  mindR = dR;
	  genjetid = ig;
	}
      }//loop on genjets
      if (genjetid==1000){
	//std::cout << " No gen match found for recjet " << i << "! firstTime= " << firstTime << ", ngenjets = " << genvec.size() << ", njets = " << jets.size() << std::endl;
	nRecNotMatched++;
	continue;
      }
      recotogenmatch[i]=std::pair<unsigned,bool>(genjetid,true);
      //loop again to find minimum dpT/pT
      double ptgen = genvec[genjetid]->pt();
      double mindpT = fabs(jets[i]->pt()-ptgen)/ptgen;
      if (print) std::cout << " match " << genjetid << " mindR = " << mindR << " dpTrel=" << mindpT << std::endl;
      for (unsigned j = 0; j < i; ++j) {//loop on jets
	if (recotogenmatch[j].first!=genjetid) continue;
	double dR = ROOT::Math::VectorUtil::DeltaR(genvec[genjetid]->vector(),jets[j]->vector());
	double dptrel = fabs(jets[j]->pt()-ptgen)/ptgen;
	if (print) std::cout << " --- found duplicate: jet " << j << " dR=" << dR << " dptrel=" << dptrel << std::endl;
	if (dptrel<mindpT && dR<0.5) {
	  mindpT = dptrel;
	  recotogenmatch[i]=std::pair<unsigned,bool>(2000,false);
	  if (print) std::cout << " --- keeping index " << j << std::endl;
	} else if (dR<mindR){
	  recotogenmatch[i]=std::pair<unsigned,bool>(2000,false);
	  if (print) std::cout << " --- keeping index " << j << std::endl;
	} else {
	  recotogenmatch[j]=std::pair<unsigned,bool>(2000,false);
	  if (print) std::cout << " --- keeping index " << i << std::endl;
	}
	nRecNotMatched++;
      }
    }

    unsigned nGenNotMatched = 0;
    for (unsigned ig = 0; ig < genvec.size(); ++ig) {//loop on genjets
      if (print) std::cout << " - firstTime= " << firstTime << ", genjet " << ig << ": ";
      if (gentorecomatch[ig].second==true && recotogenmatch[gentorecomatch[ig].first].second==true) continue;
      double mindR = 1000;
      unsigned jetid=1000;
      for (unsigned i = 0; i < jets.size(); ++i) {//loop on jets
	if (!firstTime && recotogenmatch[i].second==true && 
	    gentorecomatch[recotogenmatch[i].first].second==true) {
	  //if (gentorecomatch[recotogenmatch[i].first].second==false) gentorecomatch[recotogenmatch[i].first]=std::pair<unsigned,bool>(i,true);
	  continue;
	}
	double dR = ROOT::Math::VectorUtil::DeltaR(genvec[ig]->vector(),jets[i]->vector());
	//double dphi = genvec[ig]->phi()-jets[i]->phi();
	//if (dphi > ROOT::Math::Pi()) dphi -= 2.*ROOT::Math::Pi();
	//if (dphi < -1.*ROOT::Math::Pi()) dphi += 2.*ROOT::Math::Pi();
	//double deta = genvec[ig]->eta()-jets[i]->eta();
	//double dR = sqrt(dphi*dphi+deta*deta);
	if (dR<mindR){
	  mindR = dR;
	  jetid = i;
	}
      }//loop on jets
      if (jetid==1000){
	//std::cout << " No rec match found for genjet " << ig << "! firstTime= " << firstTime << ", ngenjets = " << genvec.size() << ", njets = " << jets.size() << std::endl;
	nGenNotMatched++;
	continue;
      }
      gentorecomatch[ig] = std::pair<unsigned,bool>(jetid,true);
      
      //loop again to find minimum dpT/pT
      double ptrec = jets[jetid]->pt();
      double mindpT = fabs(ptrec-genvec[ig]->pt())/genvec[ig]->pt();
      if (print) std::cout << " match " << jetid << " mindR = " << mindR << " dpTrel=" << mindpT << std::endl;
      
      for (unsigned jg = 0; jg < ig; ++jg) {//loop on jets
	if (gentorecomatch[jg].first!=jetid) continue;
	//if (!firstTime && gentorecomatch[jg].second==true) continue;
	double dptrel = fabs(ptrec-genvec[jg]->pt())/genvec[jg]->pt();
	double dR = ROOT::Math::VectorUtil::DeltaR(genvec[jg]->vector(),jets[jetid]->vector());
	if (print) std::cout << " --- found duplicate: genjet " << jg << " dR=" << dR << " dptrel=" << dptrel << std::endl;
	if (dptrel<mindpT && dR<0.5) {
	  mindpT = dptrel;
	  gentorecomatch[ig]=std::pair<unsigned,bool>(2000,false);
	  if (print) std::cout << " --- keeping index " << jg << std::endl;
	} else if (dR<mindR){
	  gentorecomatch[ig]=std::pair<unsigned,bool>(2000,false);
	  if (print) std::cout << " --- keeping index " << jg << std::endl;
	} else {
	  gentorecomatch[jg]=std::pair<unsigned,bool>(2000,false);
	  if (print) std::cout << " --- keeping index " << ig << std::endl;
	}
	nGenNotMatched++;
      }
    }
    
    if (print) std::cout << " -- First=" << firstTime << ", found " << nGenNotMatched << "/" << genvec.size() << " genjets not matched and " << nRecNotMatched << "/" << jets.size() << " recjets not matched" << std::endl;
    if (firstTime && (nGenNotMatched>0 && nRecNotMatched>0)) {
      getGenRecoMatches(jets,genvec,recotogenmatch,gentorecomatch,false);
    }
    
    
    
  }//getgenrecomatches
  
  

}//namespace

