#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/TrigeffInputs.h"

#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "TVector3.h"


namespace ic {


  TrigeffInputs::TrigeffInputs(std::string const& name): ModuleBase(name){
    fs_ = NULL;
    met_label_ = "pfMet";
    dijet_label_ = "jjCandidates";
    sel_label_ = "JetPair";
    channel_ = "nunu";
    is_data_ = false;
    is_embedded_ = false;
    trig_obj_label_ = "triggerObjectsDiPFJet40PFMETnoMu65MJJ800VBFAllJets";
    trigger_path_ = "HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v";

    outputTree_ = 0;

    total_weight_ = 1;
    jet1_pt_ = 0;
    jet2_pt_ = 0;
    jet1_eta_ = 0;
    jet2_eta_ = 0;
    jet1_phi_ = 0;
    jet2_phi_ = 0;
    jet1_E_ = 0;
    jet2_E_ = 0;
    dijet_M_ = 0;
    dijet_deta_ = 0;
    dijet_sumeta_ = 0;
    dijet_dphi_ = 0;
    met_ = 0;
    met_x_ = 0;
    met_y_ = 0;
    met_phi_ = 0;
    met_significance_ = 0;
    sumet_ = 0;
    ht_ = 0;
    mht_ = 0;
    sqrt_ht_ = 0;
    unclustered_et_ = 0;
    unclustered_phi_ = 0;
    jet1met_dphi_ = 0;
    jet2met_dphi_ = 0;
    jetmet_mindphi_ = 0;
    jetunclet_mindphi_ = 0;
    metunclet_dphi_ = 0;
    dijetmet_scalarSum_pt_ = 0;
    dijetmet_vectorialSum_pt_ = 0;
    dijetmet_ptfraction_ = 0;
    jet1met_scalarprod_ = 0;
    jet2met_scalarprod_ = 0;
    n_jets_cjv_30_ = 0;
    n_jets_cjv_20EB_30EE_ = 0;
    passtrigger_ = -1;
    passparkedtrigger1_ = -1;
    passparkedtrigger2_ = -1;
    l1met_ = 0;
    metnomuons_ =0;
  }

  TrigeffInputs::~TrigeffInputs(){
    ;
  }


  int  TrigeffInputs::PreAnalysis(){
    std::cout << "--------------------------------------------- " << std::endl
	      << "------ PreAnalysis Info for TrigeffInputs ------" << std::endl
	      << "--------------------------------------------- " << std::endl;
    if (fs_) {
      std::cout << "MET Label: " << met_label_ << std::endl;
      std::cout << "dijet Label: " << dijet_label_ << std::endl;
      std::cout << "Selection Label: " << sel_label_ << std::endl;
      std::cout << "Channel :" << channel_ << std::endl;
      if (is_embedded_ && is_data_) std::cout << "Processing set for embedded data !" << std::endl;
      else if (is_data_) std::cout << "Processing set for data !" << std::endl;
      else if (is_embedded_) std::cout << "Processing set for embedded MC !" << std::endl;
      else  std::cout << "Processing set for MC !" << std::endl;
    }

    outputTree_ = fs_->make<TTree>("TrigeffInputTree","Tree containing TrigEff input variables");

    outputTree_->Branch("total_weight",&total_weight_);
    outputTree_->Branch("jet1_pt",&jet1_pt_);
    outputTree_->Branch("jet2_pt",&jet2_pt_);
    outputTree_->Branch("jet1_eta",&jet1_eta_);
    outputTree_->Branch("jet2_eta",&jet2_eta_);
    outputTree_->Branch("jet1_phi",&jet1_phi_);
    outputTree_->Branch("jet2_phi",&jet2_phi_);
    outputTree_->Branch("jet1_E",&jet1_E_);
    outputTree_->Branch("jet2_E",&jet2_E_);
    outputTree_->Branch("dijet_M",&dijet_M_);
    outputTree_->Branch("dijet_deta",&dijet_deta_);
    outputTree_->Branch("dijet_sumeta",&dijet_sumeta_);
    outputTree_->Branch("dijet_dphi",&dijet_dphi_);
    outputTree_->Branch("met",&met_);
    outputTree_->Branch("met_x",&met_x_);
    outputTree_->Branch("met_y",&met_y_);
    outputTree_->Branch("met_phi",&met_phi_);
    outputTree_->Branch("met_significance",&met_significance_);
    outputTree_->Branch("sumet",&sumet_);
    outputTree_->Branch("ht",&ht_);
    outputTree_->Branch("mht",&mht_);
    outputTree_->Branch("sqrt_ht",&sqrt_ht_);
    outputTree_->Branch("unclustered_et",&unclustered_et_);
    outputTree_->Branch("unclustered_phi",&unclustered_phi_);
    outputTree_->Branch("jet1met_dphi",&jet1met_dphi_);
    outputTree_->Branch("jet2met_dphi",&jet2met_dphi_);
    outputTree_->Branch("jetmet_mindphi",&jetmet_mindphi_);
    outputTree_->Branch("jetunclet_mindphi",&jetunclet_mindphi_);
    outputTree_->Branch("metunclet_dphi",&metunclet_dphi_);
    outputTree_->Branch("dijetmet_scalarSum_pt",&dijetmet_scalarSum_pt_);
    outputTree_->Branch("dijetmet_vectorialSum_pt",&dijetmet_vectorialSum_pt_);
    outputTree_->Branch("dijetmet_ptfraction",&dijetmet_ptfraction_);
    outputTree_->Branch("jet1met_scalarprod",&jet1met_scalarprod_);
    outputTree_->Branch("jet2met_scalarprod",&jet2met_scalarprod_);
    outputTree_->Branch("n_jets_cjv_30",&n_jets_cjv_30_);
    outputTree_->Branch("n_jets_cjv_20EB_30EE",&n_jets_cjv_20EB_30EE_);
    outputTree_->Branch("passtrigger",&passtrigger_);
    outputTree_->Branch("passparkedtrigger1",&passparkedtrigger1_);
    outputTree_->Branch("passparkedtrigger2",&passparkedtrigger2_);
    outputTree_->Branch("l1met",&l1met_);
    outputTree_->Branch("metnomuons",&metnomuons_);

    return 0;
  }

  int  TrigeffInputs::Execute(TreeEvent *event){
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    double wt = eventInfo->total_weight();

    //get collections
    std::vector<CompositeCandidate *> const& dijet_vec = event->GetPtrVec<CompositeCandidate>(dijet_label_);
    Met const* met = event->GetPtr<Met>(met_label_);
    Met const* metnomuons = event->GetPtr<Met>("metNoMuons");
    std::vector<Candidate *> const& l1met = event->GetPtrVec<Candidate>("l1extraMET");
    if(l1met.size()!=1)std::cout<<"There seem to be "<<l1met.size()<<" l1mets!!"<<std::endl;
    std::vector<PFJet*> alljets = event->GetPtrVec<PFJet>("AllpfJetsPFlow");
    std::sort(alljets.begin(), alljets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));

    if (dijet_vec.size() != 0) {
      
      CompositeCandidate const* dijet = dijet_vec.at(0);

      Candidate const* jet1 = dijet->GetCandidate("jet1");
      Candidate const* jet2 = dijet->GetCandidate("jet2");
      ROOT::Math::PtEtaPhiEVector jet1vec = jet1->vector();
      ROOT::Math::PtEtaPhiEVector jet2vec = jet2->vector();
      ROOT::Math::PtEtaPhiEVector metvec = met->vector();

      total_weight_ = wt;
      jet1_pt_ = jet1->pt();
      jet2_pt_ = jet2->pt();
      jet1_eta_ = jet1->eta();
      jet2_eta_ = jet2->eta();
      jet1_phi_ = jet1->phi();
      jet2_phi_ = jet2->phi();
      jet1_E_ = jet1vec.E();
      jet2_E_ = jet2vec.E();
      dijet_M_ = dijet->M();
      dijet_deta_ = fabs(jet1->eta() - jet2->eta());
      dijet_sumeta_ = jet1->eta() + jet2->eta();
      dijet_dphi_ = fabs(ROOT::Math::VectorUtil::DeltaPhi(jet1vec,jet2vec));
      met_ = met->pt();
      met_x_ = metvec.Px();
      met_y_ = metvec.Py();
      met_phi_ = met->phi();
      met_significance_ = met->et_sig();
      sumet_ = met->sum_et();
      l1met_ = l1met[0]->energy();
      metnomuons_ = metnomuons->pt();

      double ht =0;
      ROOT::Math::PtEtaPhiEVector mhtVec(0,0,0,0);
      for(unsigned i =0; i<alljets.size();++i){
	ht+=alljets[i]->vector().Et();
	mhtVec += alljets[i]->vector();
      }
      ROOT::Math::PtEtaPhiEVector unclVec = mhtVec + metvec;

      ht_ = ht;
      mht_ = mhtVec.Et();
      sqrt_ht_ = sqrt(ht);
      unclustered_et_ = unclVec.Et();
      unclustered_phi_ = unclVec.Phi();

      double dphi1 = fabs(ROOT::Math::VectorUtil::DeltaPhi(jet1vec,metvec));
      double dphi2 = fabs(ROOT::Math::VectorUtil::DeltaPhi(jet2vec,metvec));
      jet1met_dphi_ = dphi1;
      jet2met_dphi_ = dphi2;
      jetmet_mindphi_ = std::min(dphi1,dphi2);


      dijetmet_scalarSum_pt_ = jet1->pt()+jet2->pt()+met->pt();
      dijetmet_vectorialSum_pt_ = (jet1vec+jet2vec+metvec).Pt();
      dijetmet_ptfraction_ = dijet->pt()/(dijet->pt()+met->pt());

      jet1met_scalarprod_ = (jet1vec.Px()*met_x_+jet1vec.Py()*met_y_)/met_;
      jet2met_scalarprod_ = (jet2vec.Px()*met_x_+jet2vec.Py()*met_y_)/met_;

      jetunclet_mindphi_ = std::min(fabs(ROOT::Math::VectorUtil::DeltaPhi(jet1vec,unclVec)),
				    fabs(ROOT::Math::VectorUtil::DeltaPhi(jet2vec,unclVec)));
      metunclet_dphi_ = fabs(ROOT::Math::VectorUtil::DeltaPhi(unclVec,metvec));

      double eta_high = (jet1->eta() > jet2->eta()) ? jet1->eta() : jet2->eta();
      double eta_low = (jet1->eta() > jet2->eta()) ? jet2->eta() : jet1->eta();
      n_jets_cjv_30_ = 0;
      n_jets_cjv_20EB_30EE_ = 0;
      if (alljets.size() > 2) {
	for (unsigned i = 2; i < alljets.size(); ++i) {
	  bool isInCentralGap = fabs(alljets[i]->eta())<4.7 && alljets[i]->eta() > eta_low && alljets[i]->eta() < eta_high;
	  if (alljets[i]->pt() > 30.0 && isInCentralGap) ++n_jets_cjv_30_;
	  if ( ((alljets[i]->eta()<2.4 && alljets[i]->pt() > 20.0) ||
		(alljets[i]->eta()>=2.4 && alljets[i]->pt() > 30.0)) && 
	       isInCentralGap)
	    ++n_jets_cjv_20EB_30EE_;
	}
      }
      
      if (is_data_) {

	TriggerPathPtrVec const& triggerPathPtrVec =
	  event->GetPtrVec<TriggerPath>("triggerPathPtrVec","triggerPaths");
	//EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo"); //Can be used in future, but commented out to remove compiler warnings      
	//unsigned run = eventInfo->run(); //Can be used in future, but commented out to remove compiler warnings                                         
	passtrigger_=-1;
	passparkedtrigger1_=-1;
	passparkedtrigger2_=-1;
	for (unsigned i = 0; i < triggerPathPtrVec.size(); ++i) {
	  std::string name = triggerPathPtrVec[i]->name();
	  triggerPathPtrVec[i]->prescale();
	  if (name.find(trigger_path_) != name.npos) passtrigger_ = 1;
	  if (name.find("HLT_DiJet35_MJJ700_AllJets_DEta3p5_VBF") != name.npos) passparkedtrigger1_ = 1;
	  if (name.find("HLT_DiJet30_MJJ700_AllJets_DEta3p5_VBF") != name.npos) passparkedtrigger2_ = 1;
	}
      }
      //for MC                                                                                                                                       
      else {
	std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trig_obj_label_);
	if (objs.size() > 0) passtrigger_=1;
      } // do obj match                                                                            


      outputTree_->Fill();
      static unsigned processed = 0;
      ++processed;
      if (processed == 500) outputTree_->OptimizeBaskets();
    }

    return 0;
  }

  int  TrigeffInputs::PostAnalysis(){
    return 0;
  }

  void  TrigeffInputs::PrintInfo(){
    ;
  }

}//namespace

