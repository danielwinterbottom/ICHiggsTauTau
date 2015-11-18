#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/LightTreeTrig.h"
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

  //bool ptorderedgenjetsort(GenJet * i,GenJet * j){return (i->pt())>(j->pt());}
  //bool ptorderedjetsort(PFJet * i,PFJet * j){return (i->pt())>(j->pt());}
  //bool ptorderedcandidatesort(Candidate * i,Candidate * j){return (i->pt())>(j->pt());}

  LightTreeTrig::LightTreeTrig(std::string const& name): ModuleBase(name){
    fs_ = NULL;
    met_label_ = "pfMetType1";
    jet_label_ = "pfJetsPFlow";
    dijet_label_ = "jjCandidates";
    is_data_ = false;
    dotrigskim_ = false;
    do_noskim_ = false;
    trig_obj_label_ = "triggerObjectsDiPFJet40DEta3p5MJJ600PFMETNoMu140";
    cont_trig_obj_label_ = "triggerObjectsDiPFJet40DEta3p5MJJ600PFMETNoMu80";
    trigger_path_ = "HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v";

    outputTree_ = 0;
    processed_ = 0;

    nGenJetsSave_ = 6;
    nTrigJetsSave_ = 6;

    trigjet_jet_mindR_ = new double[nTrigJetsSave_];
    trigjet_trigMatched_ = new int[nTrigJetsSave_];
    trigjet_pt_ = new double[nTrigJetsSave_];
    trigjet_eta_ = new double[nTrigJetsSave_];
    trigjet_phi_ = new double[nTrigJetsSave_];
    trigjet_E_ = new double[nTrigJetsSave_];

    genjetptordered_pt_ = new double[nGenJetsSave_];
    genjetptordered_eta_ = new double[nGenJetsSave_];
    genjetptordered_phi_ = new double[nGenJetsSave_];
    genjetptordered_E_ = new double[nGenJetsSave_];

    trigjetptordered_pt_ = new double[nTrigJetsSave_];
    trigjetptordered_eta_ = new double[nTrigJetsSave_];
    trigjetptordered_phi_ = new double[nTrigJetsSave_];
    trigjetptordered_E_ = new double[nTrigJetsSave_];

    resetAllTreeVariables();
  }

  void LightTreeTrig::resetAllTreeVariables(){

    run_=0;
    lumi_=0;
    event_=0;


    for (unsigned ijet(0); ijet<nTrigJetsSave_;++ijet){
      trigjet_jet_mindR_[ijet] = -1;
      trigjet_trigMatched_[ijet] = 0;
      trigjet_pt_[ijet] = -1;
      trigjet_eta_[ijet] = -5;
      trigjet_phi_[ijet] = -5;
      trigjet_E_[ijet] = -5;
    }
    sigtrigcalomet_=-1;
    sigtrigpfmet_=-1;
    conttrigcalomet_=-1;
    conttrigpfmet_=-1;

    jet1met_dphi_ = 0;
    jet2met_dphi_ = 0;
    jet1metnomu_dphi_ = 0;
    jet2metnomu_dphi_ = 0;
    jetmet_mindphi_ = 0;
    alljetsmet_mindphi_ = 0;

    dijetmet_scalarSum_pt_ = 0;
    dijetmet_vectorialSum_pt_ = 0;
    dijetmet_ptfraction_ = 0;
    jet1met_scalarprod_ = 0;
    jet2met_scalarprod_ = 0;

    pass_muontrigger_ = -1;
    pass_singlejettrigger_ = -1;
    pass_htquadjettrigger_ = -1;

    genlep1_pt_=-1;
    genlep1_eta_=-10000;
    genlep1_phi_=-1;
    genlep1_id_=-1;
    genlep2_pt_=-1;
    genlep2_eta_=-10000;
    genlep2_phi_=-1;
    genlep2_id_=-1;

    for (unsigned ijet(0); ijet<nGenJetsSave_;++ijet){
      genjetptordered_pt_[ijet] = -1;
      genjetptordered_eta_[ijet] = -5;
      genjetptordered_phi_[ijet] = -5;
      genjetptordered_E_[ijet] = -5;
    }
    for (unsigned ijet(0); ijet<nTrigJetsSave_;++ijet){
      trigjetptordered_pt_[ijet] = -1;
      trigjetptordered_eta_[ijet] = -5;
      trigjetptordered_phi_[ijet] = -5;
      trigjetptordered_E_[ijet] = -5;
    }

    digenjet_M_=-1;
    digenjet_deta_=-1;
    digenjet_dphi_=-1;
    ditrigjet_M_=-1;
    ditrigjet_deta_=-1;
    ditrigjet_dphi_=-1;
  }

  LightTreeTrig::~LightTreeTrig(){
    ;
  }


  int  LightTreeTrig::PreAnalysis(){
    std::cout << "--------------------------------------------- " << std::endl
	      << "------ PreAnalysis Info for LightTreeTrig ------" << std::endl
	      << "--------------------------------------------- " << std::endl;
    if (fs_) {
      std::cout << "MET Label: " << met_label_ << std::endl;
      std::cout << "dijet Label: " << dijet_label_ << std::endl;
      if (is_data_) std::cout << "Processing set for data !" << std::endl;
      else  std::cout << "Processing set for MC !" << std::endl;
    }
    outputTree_=fs_->make<TTree>("LightTreeTrig","Tree containing LightTreeTrigAna input variables");//    outputTree_ = new TTree("LightTreeTrig","Tree containing LightTreeTrigAna input variables"); 
    outputTree_->Branch("run",&run_);
    outputTree_->Branch("lumi",&lumi_);
    outputTree_->Branch("event",&event_);

    for (unsigned ijet(0); ijet<nTrigJetsSave_;++ijet){
      std::ostringstream label;
      label << "trigjet" << ijet+1;
      outputTree_->Branch((label.str()+"_jet_mindR").c_str(),&trigjet_jet_mindR_[ijet]);
      outputTree_->Branch((label.str()+"_trigMatched").c_str(),&trigjet_trigMatched_[ijet]);
      outputTree_->Branch((label.str()+"_pt").c_str(),&trigjet_pt_[ijet]);
      outputTree_->Branch((label.str()+"_eta").c_str(),&trigjet_eta_[ijet]);
      outputTree_->Branch((label.str()+"_phi").c_str(),&trigjet_phi_[ijet]);
      outputTree_->Branch((label.str()+"_E").c_str(),&trigjet_E_[ijet]);
    }
    //keep 2 loops so variables are next to each others in tree.
    for (unsigned ijet(0); ijet<nTrigJetsSave_;++ijet){
      std::ostringstream label;
      label << "trigjetptordered" << ijet+1;
      outputTree_->Branch((label.str()+"_pt").c_str(),&trigjetptordered_pt_[ijet]);
      outputTree_->Branch((label.str()+"_eta").c_str(),&trigjetptordered_eta_[ijet]);
      outputTree_->Branch((label.str()+"_phi").c_str(),&trigjetptordered_phi_[ijet]);
      outputTree_->Branch((label.str()+"_E").c_str(),&trigjetptordered_E_[ijet]);
    }
    outputTree_->Branch("ditrigjet_M",&ditrigjet_M_);
    outputTree_->Branch("ditrigjet_deta",&ditrigjet_deta_);
    outputTree_->Branch("ditrigjet_dphi",&ditrigjet_dphi_);

    outputTree_->Branch("sigtrigcalomet",&sigtrigcalomet_);
    outputTree_->Branch("sigtrigpfmet",&sigtrigpfmet_);
    outputTree_->Branch("conttrigcalomet",&conttrigcalomet_);
    outputTree_->Branch("conttrigpfmet",&conttrigpfmet_);

    outputTree_->Branch("jet1met_dphi",&jet1met_dphi_);
    outputTree_->Branch("jet2met_dphi",&jet2met_dphi_);
    outputTree_->Branch("jet1metnomu_dphi",&jet1metnomu_dphi_);
    outputTree_->Branch("jet2metnomu_dphi",&jet2metnomu_dphi_);
    outputTree_->Branch("jetmet_mindphi",&jetmet_mindphi_);
    outputTree_->Branch("alljetsmet_mindphi",&alljetsmet_mindphi_);
    outputTree_->Branch("dijetmet_scalarSum_pt",&dijetmet_scalarSum_pt_);
    outputTree_->Branch("dijetmet_vectorialSum_pt",&dijetmet_vectorialSum_pt_);
    outputTree_->Branch("dijetmet_ptfraction",&dijetmet_ptfraction_);
    outputTree_->Branch("jet1met_scalarprod",&jet1met_scalarprod_);
    outputTree_->Branch("jet2met_scalarprod",&jet2met_scalarprod_);

    outputTree_->Branch("pass_muontrigger",&pass_muontrigger_);
    outputTree_->Branch("pass_singlejettrigger",&pass_singlejettrigger_);
    outputTree_->Branch("pass_htquadjettrigger",&pass_htquadjettrigger_);

    outputTree_->Branch("genlep1_pt",&genlep1_pt_);
    outputTree_->Branch("genlep1_eta",&genlep1_eta_);
    outputTree_->Branch("genlep1_phi",&genlep1_phi_);
    outputTree_->Branch("genlep1_id",&genlep1_id_);
    outputTree_->Branch("genlep2_pt",&genlep2_pt_);
    outputTree_->Branch("genlep2_eta",&genlep2_eta_);
    outputTree_->Branch("genlep2_phi",&genlep2_phi_);
    outputTree_->Branch("genlep2_id",&genlep2_id_);

    for (unsigned ijet(0); ijet<nGenJetsSave_;++ijet){
      std::ostringstream label;
      label << "genjetptordered" << ijet+1;
      outputTree_->Branch((label.str()+"_pt").c_str(),&genjetptordered_pt_[ijet]);
      outputTree_->Branch((label.str()+"_eta").c_str(),&genjetptordered_eta_[ijet]);
      outputTree_->Branch((label.str()+"_phi").c_str(),&genjetptordered_phi_[ijet]);
      outputTree_->Branch((label.str()+"_E").c_str(),&genjetptordered_E_[ijet]);
    }
    outputTree_->Branch("digenjet_M",&digenjet_M_);
    outputTree_->Branch("digenjet_deta",&digenjet_deta_);
    outputTree_->Branch("digenjet_dphi",&digenjet_dphi_);

    processed_ = 0;

    return 0;
  }

  int  LightTreeTrig::Execute(TreeEvent *event){


    resetAllTreeVariables();

    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    run_= eventInfo->run();
    lumi_= eventInfo->lumi_block();
    event_= eventInfo->event();
    //if (is_data_) {
	
    ////////////////////////////////
    // Get trigger variables
    ////////////////////////////////

    auto const& triggerPathPtrVec =
      event->GetPtrVec<TriggerPath>("triggerPathPtrVec","triggerPaths");

    for (unsigned i = 0; i < triggerPathPtrVec.size(); ++i) {
      std::string name = triggerPathPtrVec[i]->name();
      double prescale = triggerPathPtrVec[i]->prescale();
      if (name.find("HLT_IsoMu20_") != name.npos) pass_muontrigger_ = prescale;
      if (name.find("HLT_DiPFJetAve40_") != name.npos) pass_singlejettrigger_ = prescale;
      if (name.find("HLT_PFHT750_4JetPt50_") != name.npos) pass_htquadjettrigger_ = prescale;
    }
    if(dotrigskim_){
      if(!(pass_singlejettrigger_==1||pass_htquadjettrigger_==1||pass_muontrigger_==1)){
	return 1;
      }
    }
    
    std::vector<TriggerObject*> triggerobjects=event->GetPtrVec<TriggerObject>(trig_obj_label_);
    std::vector<TriggerObject*> conttriggerobjects=event->GetPtrVec<TriggerObject>(cont_trig_obj_label_);

    //Get vector of all trigger jets
    std::vector<Candidate *> const& l1forwardjets = event->GetPtrVec<Candidate>("l1extraForwardJets");
    std::vector<Candidate *> const& l1centraljets = event->GetPtrVec<Candidate>("l1extraCentralJets");
    std::vector<Candidate *> trigveccopy = l1forwardjets;
    trigveccopy.insert(trigveccopy.end(),l1centraljets.begin(),l1centraljets.end());
    std::sort(trigveccopy.begin(),trigveccopy.end(),bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));

    for(unsigned itrigjet=0;(itrigjet<trigveccopy.size()&&itrigjet<nTrigJetsSave_);itrigjet++){
      trigjetptordered_pt_[itrigjet]=trigveccopy[itrigjet]->pt();
      trigjetptordered_eta_[itrigjet]=trigveccopy[itrigjet]->eta();
      trigjetptordered_phi_[itrigjet]=trigveccopy[itrigjet]->phi();
      trigjetptordered_E_[itrigjet]=trigveccopy[itrigjet]->energy();
    }
    if(trigveccopy.size()>=2){
      ROOT::Math::PtEtaPhiEVector trigjet1vec = trigveccopy[0]->vector();
      ROOT::Math::PtEtaPhiEVector trigjet2vec = trigveccopy[1]->vector();
      ROOT::Math::PtEtaPhiEVector ditrigjetvec = trigjet1vec+trigjet2vec;
      ditrigjet_M_=ditrigjetvec.M();
      ditrigjet_deta_ = fabs(trigveccopy[0]->eta() - trigveccopy[1]->eta());
      ditrigjet_dphi_ = fabs(ROOT::Math::VectorUtil::DeltaPhi(trigjet1vec,trigjet2vec));
    }

   ////////////////////////////////
    // Get GenLevel collections
    ////////////////////////////////

    std::vector<GenJet *> genvec;
    if(!is_data_){
      std::vector<GenJet *> & genvec= event->GetPtrVec<GenJet>("genJets");
      std::sort(genvec.begin(), genvec.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));   
      //Find highest 4 pt gen jets
      for(unsigned igenjet=0;(igenjet<genvec.size()&&igenjet<nGenJetsSave_);igenjet++){
	genjetptordered_pt_[igenjet]=genvec[igenjet]->pt();
	genjetptordered_eta_[igenjet]=genvec[igenjet]->eta();
	genjetptordered_phi_[igenjet]=genvec[igenjet]->phi();
	genjetptordered_E_[igenjet]=genvec[igenjet]->energy();
      }
      if(genvec.size()>=2){
	ROOT::Math::PtEtaPhiEVector genjet1vec = genvec[0]->vector();
	ROOT::Math::PtEtaPhiEVector genjet2vec = genvec[1]->vector();
	ROOT::Math::PtEtaPhiEVector digenjetvec = genjet1vec+genjet2vec;
	digenjet_M_=digenjetvec.M();
	digenjet_deta_ = fabs(genvec[0]->eta() - genvec[1]->eta());
	digenjet_dphi_ = fabs(ROOT::Math::VectorUtil::DeltaPhi(genjet1vec,genjet2vec));
      }

      //Get gen leptons
      std::vector<GenParticle*> const& parts = event->GetPtrVec<GenParticle>("genParticles");
      for (unsigned i = 0; i < parts.size(); ++i) {
	if (parts[i]->status() != 1) continue;
	
	int id = parts[i]->pdgid();

	if((abs(id)==11)||(abs(id)==13)||(abs(id)==15)){
	  if(parts[i]->pt()>genlep1_pt_){
	    genlep2_pt_=genlep1_pt_;
	    genlep2_eta_=genlep1_eta_;
	    genlep2_phi_=genlep1_phi_;
	    genlep2_id_=genlep1_id_;
	    genlep1_pt_=parts[i]->pt();
	    genlep1_eta_=parts[i]->eta();
	    genlep1_phi_=parts[i]->phi();
	    genlep1_id_=id;
	  }
	  else if(parts[i]->pt()>genlep2_pt_){
	    genlep2_pt_=parts[i]->pt();
	    genlep2_eta_=parts[i]->eta();
	    genlep2_phi_=parts[i]->phi();
	    genlep2_id_=id;
	  }
	}
      }//loop on genparticles

    }//!data

    ////////////////////////////////
    // Get MET collections
    ////////////////////////////////

    Met *met = 0;
    try {
      std::vector<Met*> metCol = event->GetPtrVec<Met>(met_label_);
      met = metCol[0];
    } catch (...){
      //std::cout << " Met vec not found..." << std::endl;
      met = event->GetPtr<Met>(met_label_);
      if (!met) {
	std::cerr << " -- Found no MET " << met_label_ << " in event! Exiting..." << std::endl;
	exit(1);
      }
    }
    Met const* metnomuons = event->GetPtr<Met>("metNoMuons");

    ROOT::Math::PtEtaPhiEVector metvec = met->vector();
    ROOT::Math::PtEtaPhiEVector metnomuvec = metnomuons->vector();

    ////////////////////////////////
    // Get Jets collections
    ////////////////////////////////
    //jet pair
 
    std::vector<CompositeCandidate *> const& dijet_vec = event->GetPtrVec<CompositeCandidate>(dijet_label_);
    //Met const* met = event->GetPtr<Met>(met_label_);

    if (dijet_vec.size() != 0) {
      
      CompositeCandidate const* dijet = dijet_vec.at(0);

      Candidate const* jet1 = dijet->GetCandidate("jet1");
      Candidate const* jet2 = dijet->GetCandidate("jet2");
      ROOT::Math::PtEtaPhiEVector jet1vec = jet1->vector();
      ROOT::Math::PtEtaPhiEVector jet2vec = jet2->vector();
      jet1met_dphi_ = fabs(ROOT::Math::VectorUtil::DeltaPhi(jet1vec,metvec));
      jet2met_dphi_ = fabs(ROOT::Math::VectorUtil::DeltaPhi(jet2vec,metvec));
      jet1metnomu_dphi_ = fabs(ROOT::Math::VectorUtil::DeltaPhi(jet1vec,metnomuvec));
      jet2metnomu_dphi_ = fabs(ROOT::Math::VectorUtil::DeltaPhi(jet2vec,metnomuvec));
      jetmet_mindphi_ = std::min(jet1met_dphi_,jet2met_dphi_);
      dijetmet_scalarSum_pt_ = jet1->pt()+jet2->pt()+met->pt();
      dijetmet_vectorialSum_pt_ = (jet1vec+jet2vec+metvec).Pt();
      dijetmet_ptfraction_ = dijet->pt()/(dijet->pt()+met->pt());
      jet1met_scalarprod_ = (jet1vec.Px()*metvec.Px()+jet1vec.Py()*metvec.Py())/met->pt();
      jet2met_scalarprod_ = (jet2vec.Px()*metvec.Px()+jet2vec.Py()*metvec.Py())/met->pt();
    }

    //all jets
    std::vector<PFJet*> & jets = event->GetPtrVec<PFJet>(jet_label_);
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
       
    alljetsmet_mindphi_=jetmet_mindphi_;

    for(unsigned ijet=0;ijet<jets.size();ijet++){
      if(jets[ijet]->pt()>30.0){
	double thisjetmetdphi = fabs(ROOT::Math::VectorUtil::DeltaPhi(jets[ijet]->vector(),metvec));
	if(thisjetmetdphi<alljetsmet_mindphi_)alljetsmet_mindphi_=thisjetmetdphi;
      }
      if (ijet>=nTrigJetsSave_) continue;
      std::pair<bool,unsigned> trigmatch=IsFilterMatchedWithIndex(jets[ijet],triggerobjects,"hltDiCaloJet30MJJ500DEta3p0",0.4);
      if(trigmatch.first){
	trigjet_trigMatched_[ijet]=1;
	trigjet_pt_[ijet]=triggerobjects[trigmatch.second]->pt();
	trigjet_eta_[ijet]=triggerobjects[trigmatch.second]->eta();
	trigjet_phi_[ijet]=triggerobjects[trigmatch.second]->phi();
	trigjet_jet_mindR_[ijet]=ROOT::Math::VectorUtil::DeltaR(triggerobjects[trigmatch.second]->vector(),jets[ijet]->vector());
      }
    }

    std::pair<bool,unsigned> sigcalomatch=IsFilterMatchedWithIndex(met,triggerobjects,"hltMET80",11);
    std::pair<bool,unsigned> sigpfmatch=IsFilterMatchedWithIndex(metnomuons,triggerobjects,"hltPFMETNoMu140",11);
    std::pair<bool,unsigned> contcalomatch=IsFilterMatchedWithIndex(met,conttriggerobjects,"hltMET50",11);
    std::pair<bool,unsigned> contpfmatch=IsFilterMatchedWithIndex(metnomuons,conttriggerobjects,"hltPFMETNoMu80",11);
    
    if(sigcalomatch.first)sigtrigcalomet_=triggerobjects[sigcalomatch.second]->pt();
    if(sigpfmatch.first)sigtrigpfmet_=triggerobjects[sigpfmatch.second]->pt();
    if(contcalomatch.first)conttrigcalomet_=conttriggerobjects[contcalomatch.second]->pt();
    if(contpfmatch.first)conttrigpfmet_=conttriggerobjects[contpfmatch.second]->pt();
    
    
    //IF PASSES CUTS FILL TREE
    if(do_noskim_){
      outputTree_->Fill();
      ++processed_;
    }
    else {
      //add preselection as required....
      outputTree_->Fill();
      ++processed_;
    }
    //else {
      //if (dijet_deta_>3.6&&metnomuons_>90&&jet_pt_[0]>50){
	//if (dijet_M_>1000 &&  dijet_deta_>3.6 && metnomuons_>100 && jet1_pt_>50){//for prompt presel
	//outputTree_->Fill();
	//++processed;
    //}
    //}

    //if (processed == 500) outputTree_->OptimizeBaskets();
    return 0;
  }

  int  LightTreeTrig::PostAnalysis(){
    std::cout<<"----------------------------------------"<<std::endl
	     <<"PostAnalysis for LightTreeTrig"<<std::endl
	     <<"----------------------------------------"<<std::endl;
    std::cout<<"Light Tree Trig saved "<<processed_<<" events"<<std::endl;
    return 0;
  }

  void  LightTreeTrig::PrintInfo(){
    ;
  }

}//namespace

