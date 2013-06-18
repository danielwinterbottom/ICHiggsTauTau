#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvControlPlots.h"

#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

#include "TMVA/Reader.h"
#include "TVector3.h"


namespace ic {



  HinvCoreControlPlots::HinvCoreControlPlots(TFileDirectory const& dir) {
    TH1F::SetDefaultSumw2();
    n_vtx = dir.make<TH1F>("n_vtx","n_vtx", 40, 0, 40); 
    met = dir.make<TH1F>("met","met", 1000, 0, 1000); 
    met_noMuons = dir.make<TH1F>("met_noMuons","met_noMuons", 1000, 0, 1000); 
    met_phi = dir.make<TH1F>("met_phi","met_phi", 63, -3.15, 3.15); 
    n_jets = dir.make<TH1F>("n_jets","n_jets", 50, 0, 50); 
    n_jetsingap = dir.make<TH1F>("n_jetsingap","n_jetsingap", 50, 0, 50); 
    jpt_1 = dir.make<TH1F>("jpt_1","jpt_1", 1000, 0, 1000); 
    jpt_2 = dir.make<TH1F>("jpt_2","jpt_2", 1000, 0, 1000); 
    jeta_1 = dir.make<TH1F>("jeta_1","jeta_1", 100, -5, 5); 
    jeta_2 = dir.make<TH1F>("jeta_2","jeta_2", 100, -5, 5); 
    mjj = dir.make<TH1F>("mjj","mjj", 5000, 0, 5000); 
    detajj = dir.make<TH1F>("detajj","detajj", 100, 0, 10);
    etaprodjj = dir.make<TH1F>("etaprodjj","etaprodjj", 100, -25, 25);
    drjj = dir.make<TH1F>("drjj","drjj", 100, 0, 10);
    dphijj = dir.make<TH1F>("dphijj","dphijj", 100, 0, 3.1416);
  };


  HinvWeightPlots::HinvWeightPlots(TFileDirectory const& dir) {
    TH1F::SetDefaultSumw2();
    met_noW = dir.make<TH1F>("met_noW","met_noW", 1000, 0, 1000);
    dphijj_noW = dir.make<TH1F>("dphijj_noW","dphijj_noW", 100, 0, 3.1416);
    n_jets_noW = dir.make<TH1F>("n_jets_noW","n_jets_noW", 50, 0, 50);
    met_pu = dir.make<TH1F>("met_pu","met_pu", 1000, 0, 1000);
    dphijj_pu = dir.make<TH1F>("dphijj_pu","dphijj_pu", 100, 0, 3.1416);
    n_jets_pu = dir.make<TH1F>("n_jets_pu","n_jets_pu", 50, 0, 50);
    met_pu_trig = dir.make<TH1F>("met_pu_trig","met_pu_trig", 1000, 0, 1000);
    dphijj_pu_trig = dir.make<TH1F>("dphijj_pu_trig","dphijj_pu_trig", 100, 0, 3.1416);
    n_jets_pu_trig = dir.make<TH1F>("n_jets_pu_trig","n_jets_pu_trig", 50, 0, 50);
    met_pu_trig_idiso = dir.make<TH1F>("met_pu_trig_idiso","met_pu_trig_idiso", 1000, 0, 1000);
    dphijj_pu_trig_idiso = dir.make<TH1F>("dphijj_pu_trig_idiso","dphijj_pu_trig_idiso",100, 0, 3.1416); 
    n_jets_pu_trig_idiso = dir.make<TH1F>("n_jets_pu_trig_idiso","n_jets_pu_trig_idiso", 50, 0, 50);
  };


  HinvSystPlots::HinvSystPlots(TFileDirectory const& dir) {
    TH1F::SetDefaultSumw2();
    n_jets_puUp = dir.make<TH1F>("n_jets_puUp","n_jets_puUp", 50, 0, 50);
    n_jets_puDown = dir.make<TH1F>("n_jets_puDown","n_jets_puDown", 50, 0, 50);
    n_vtx_puUp = dir.make<TH1F>("n_vtx_puUp","n_vtx_puUp", 40, 0, 40);
    n_vtx_puDown = dir.make<TH1F>("n_vtx_puDown","n_vtx_puDown", 40, 0, 40);
    dphijj_puUp = dir.make<TH1F>("dphijj_puUp","dphijj_puUp", 100, 0, 3.1416);
    dphijj_puDown = dir.make<TH1F>("dphijj_puDown","dphijj_puDown", 100, 0, 3.1416);
  }


  // Initialization of HinvDijetMETPlots struct
  HinvDijetMETPlots::HinvDijetMETPlots(TFileDirectory const& dir) {
    TH1F::SetDefaultSumw2();
    htMET              = dir.make<TH1F>("htMET",             "htMET",             500,    0,1000);
    vecSumTriObjectPt  = dir.make<TH1F>("vecSumTriObjectPt", "vecSumTriObjectPt", 500,    0, 500);
    scalSumTriObjectPt = dir.make<TH1F>("scalSumTriObjectPt","scalSumTriObjectPt",500,    0,1000);
    dijetOverMetPt     = dir.make<TH1F>("dijetOverMetPt",    "dijetOverMetPt",    100,    0,   1);
    alphaT             = dir.make<TH1F>("alphaT",            "alphaT",            100,    0,  10);
    betaT              = dir.make<TH1F>("betaT",             "betaT",             100,    0,  10);
    
    vecSum_htMET       = dir.make<TH2F>("vecSum_htMET",   "vecSum_htMET",   100,0,500,100,0,1000);
    dijetFrac_htMET    = dir.make<TH2F>("dijetFrac_htMET","dijetFrac_htMET",100,0,  1,100,0,1000);
    alphaT_htMET       = dir.make<TH2F>("alphaT_htMET",   "alphaT_htMET",   100,0, 10,100,0,1000);
    betaT_htMET        = dir.make<TH2F>("betaT_htMET",    "betaT_htMET",    100,0, 10,100,0,1000);
  };

  
  HinvControlPlots::HinvControlPlots(std::string const& name): ModuleBase(name){
    fs_ = NULL;
    met_label_ = "pfMet";
    dijet_label_ = "jjCandidates";
    sel_label_ = "JetPair";
    channel_ = "nunu";
  }

  HinvControlPlots::~HinvControlPlots(){;}


  int  HinvControlPlots::PreAnalysis(){
    std::cout << "** PreAnalysis Info for HinvControlPlots **" << std::endl;
    if (fs_) {
      std::cout << "MET Label: " << met_label_ << std::endl;
      std::cout << "dijet Label: " << dijet_label_ << std::endl;
      std::cout << "Selection Label: " << sel_label_ << std::endl;
      std::cout << "Channel :" << channel_ << std::endl;
    }

    misc_plots_ = new DynamicHistoSet(fs_->mkdir("misc_plots"));
    misc_2dplots_ = new Dynamic2DHistoSet(fs_->mkdir("misc_2dplots"));
    InitCoreControlPlots();
    InitWeightPlots();
    InitSystPlots();
    InitDijetMETPlots();
    
    yields_ = 0;

    return 0;
  }

  int  HinvControlPlots::Execute(TreeEvent *event){

    // Get the objects we need from the event
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    EventInfo * eventInfoNonConst = event->GetPtr<EventInfo>("eventInfo");

    wt_ = eventInfo->total_weight();

    std::vector<CompositeCandidate *> const& dijet_vec = event->GetPtrVec<CompositeCandidate>(dijet_label_);

    Met const* met = event->GetPtr<Met>(met_label_);
    Met const* met_noMuons = event->GetPtr<Met>("metNoMuons");
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    

    // Define event properties
    // IMPORTANT: Make sure each property is re-set
    // for each new event

    jpt_1_ = -1;
    jeta_1_ = -10;
    jpt_2_ = -1;
    jeta_2_ = -10;

    mjj_ = -1;
    detajj_ = -10;
    etaprodjj_ = -100;
    drjj_ = -1;
    dphijj_ = -1;

    n_jetsingap_ = 0;
    
    // Start: HinvDijetMETPlots Variables __________________________________
    ROOT::Math::PtEtaPhiEVector diffVector;
    double diffEtValue;
    double diffPtValue;
    double scalSumPt;
    
    double dijet_MET;
    double htPlusMET = 0;
    // End: Filling HinvDijetMETPlots Variables ____________________________
    
    // Enable filling of plots
    bool fillPlots             = true;
    //AM-FIX: no need for different bool, or signal/QCD won't be filled properly !
    //bool fillHinvDijetMETPlots = true;
    
    if (dijet_vec.size() != 0) {
      
      CompositeCandidate const* dijet = dijet_vec.at(0);

      if (sel_label_.find("QCD") != sel_label_.npos &&
	  PairAbsDPhiLessThan(dijet,2.6)) fillPlots = false;
      if (sel_label_.find("SIGNAL") != sel_label_.npos &&
	  !PairAbsDPhiLessThan(dijet,1.0)) fillPlots = false;
        
      if (sel_label_.find("AN") != sel_label_.npos) {
	if (!(met->pt() > 130 && dijet->M()>1000)) fillPlots=false;
      }

      Candidate const* jet1 = dijet->GetCandidate("jet1");
      Candidate const* jet2 = dijet->GetCandidate("jet2");

      jpt_1_ = jet1->pt();
      jeta_1_ = jet1->eta();
      jpt_2_ = jet2->pt();
      jeta_2_ = jet2->eta();
      
      mjj_ = dijet->M();
      detajj_ = fabs(jet1->eta() - jet2->eta());
      etaprodjj_ = jet1->eta() * jet2->eta();
      drjj_ = ROOT::Math::VectorUtil::DeltaR(jet1->vector(),jet2->vector());
      dphijj_ = fabs(ROOT::Math::VectorUtil::DeltaPhi(jet1->vector(),jet2->vector()));
      
      double eta_high = (jet1->eta() > jet2->eta()) ? jet1->eta() : jet2->eta();
      double eta_low = (jet1->eta() > jet2->eta()) ? jet2->eta() : jet1->eta();
      n_jetsingap_ = 0;
      std::vector<PFJet*> cjvjets = event->GetPtrVec<PFJet>("cjvpfJetsPFlow");
      std::sort(cjvjets.begin(), cjvjets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));


      if (cjvjets.size() > 2) {
	for (unsigned i = 0; i < cjvjets.size(); ++i) {
	  if (cjvjets[i]->pt() > 30.0 &&  cjvjets[i]->eta() > eta_low && cjvjets[i]->eta() < eta_high) ++n_jetsingap_;
	}
      }
      
      
      // Start: Calculating HinvDijetMETPlots __________________________________
      htPlusMET = dijet->ScalarPtSum() + met->vector().Pt();
      
      diffVector   = dijet->vector() + met->vector();
      diffEtValue  = diffVector.Et();
      diffPtValue  = diffVector.Pt();

      dijet_MET = dijet->vector().Pt()/(met->vector().Pt()+dijet->vector().Pt());
      scalSumPt = dijet->vector().Pt()+met->vector().Pt();
      // End: Calculating HinvDijetMETPlots ____________________________________
      
    }
    else if (jets.size() > 1) {
      Candidate const* jet1 = jets[0];
      Candidate const* jet2 = jets[1];
      jpt_1_  = jet1->pt();
      jeta_1_ = jet1->eta();
      jpt_2_  = jet2->pt();
      jeta_2_ = jet2->eta();
      
      mjj_ = (jet1->vector()+jet2->vector()).M();
      detajj_ = fabs(jet1->eta() - jet2->eta());
      etaprodjj_ = jet1->eta() * jet2->eta();
      drjj_ = ROOT::Math::VectorUtil::DeltaR(jet1->vector(),jet2->vector());
      dphijj_ = fabs(ROOT::Math::VectorUtil::DeltaPhi(jet1->vector(),jet2->vector()));
      
      double eta_high = (jet1->eta() > jet2->eta()) ? jet1->eta() : jet2->eta();
      double eta_low = (jet1->eta() > jet2->eta()) ? jet2->eta() : jet1->eta();
      n_jetsingap_ = 0;
      if (n_jets_ > 2) {
	for (unsigned i = 2; i < jets.size(); ++i) {
	  if (jets[i]->pt() > 30.0 &&  jets[i]->eta() > eta_low && jets[i]->eta() < eta_high) ++n_jetsingap_;
	}
      }
      
       // Start: Calculating HinvDijetMETPlots __________________________________
       ROOT::Math::PtEtaPhiEVector dijet = jet1->vector()+jet2->vector();

       diffVector  = dijet + met->vector();
       diffEtValue = diffVector.Et();
       diffPtValue = diffVector.Pt();
       
       dijet_MET = dijet.Pt()/(met->vector().Pt()+dijet.Pt());
       scalSumPt = dijet.Pt()+met->vector().Pt();
       // End: Calculating HinvDijetMETPlots ____________________________________
      
    }
    
    n_vtx_ = eventInfo->good_vertices();
    
    met_ = met->pt();
    met_noMuons_ = met_noMuons->pt();
    met_phi_ = met->phi();
    
    n_jets_ = jets.size();

    if (fillPlots) {
      FillCoreControlPlots();
      FillWeightPlots(eventInfoNonConst);
      FillSystPlots(eventInfoNonConst);
    }

    // Start: Filling HinvDijetMETPlots __________________________________
    if (fillPlots) {
      //dijetMETPlots_->vecSumTriObjectEt ->Fill(diffEtValue,wt_);
      dijetMETPlots_->vecSumTriObjectPt ->Fill(diffPtValue,wt_);
      dijetMETPlots_->dijetOverMetPt    ->Fill(dijet_MET,  wt_);
      dijetMETPlots_->scalSumTriObjectPt->Fill(scalSumPt,  wt_);
      dijetMETPlots_->htMET             ->Fill(htPlusMET,  wt_);

      //dijetMETPlots_->alphaT->Fill(htPlusMET,wt_);
      //dijetMETPlots_->betaT ->Fill(htPlusMET,wt_);

      dijetMETPlots_->vecSum_htMET   ->Fill(diffPtValue,htPlusMET,wt_);
      dijetMETPlots_->dijetFrac_htMET->Fill(dijet_MET,  htPlusMET,wt_);
      //dijetBalancePlots_->alphaT_htMET   ->Fill(htPlusMET,wt_);
      //dijetBalancePlots_->betaT_htMET    ->Fill(htPlusMET,wt_);
    }
    // End: Filling HinvDijetMETPlots ____________________________________
    
    return 0;
  }

  int  HinvControlPlots::PostAnalysis(){
    return 0;
  }

  void  HinvControlPlots::PrintInfo(){
    ;
  }

  void HinvControlPlots::InitCoreControlPlots() {
    controlplots_ = new HinvCoreControlPlots(fs_->mkdir(sel_label_));
    std::cout << " Core control plots initialised" << std::endl;
  }

  void HinvControlPlots::InitWeightPlots() {
    weightplots_ = new HinvWeightPlots(fs_->mkdir(sel_label_+"/weights"));
    std::cout << " weight plots initialised" << std::endl;
  }

  void HinvControlPlots::InitSystPlots() {
    systplots_ = new HinvSystPlots(fs_->mkdir(sel_label_+"/systematics"));
    std::cout << " syst plots initialised" << std::endl;
  }

  void HinvControlPlots::InitDijetMETPlots() {
    dijetMETPlots_ = new HinvDijetMETPlots(fs_->mkdir(sel_label_+"/dijetMet"));
    std::cout << " dijetMET plots initialised" << std::endl;
  }
      
 void HinvControlPlots::FillYields() {
   yields_ = yields_ + wt_;
 }


  void HinvControlPlots::FillCoreControlPlots() {
    controlplots_->n_vtx->Fill(n_vtx_, wt_);
    controlplots_->met->Fill(met_, wt_);
    controlplots_->met_noMuons->Fill(met_noMuons_, wt_);
    controlplots_->met_phi->Fill(met_phi_, wt_);
    controlplots_->n_jets->Fill(n_jets_, wt_);
    controlplots_->n_jetsingap->Fill(n_jetsingap_, wt_);
    controlplots_->jpt_1->Fill(jpt_1_, wt_);
    controlplots_->jeta_1->Fill(jeta_1_, wt_);
    controlplots_->jpt_2->Fill(jpt_2_, wt_);
    controlplots_->jeta_2->Fill(jeta_2_, wt_);            
    controlplots_->mjj->Fill(mjj_, wt_);
    controlplots_->detajj->Fill(detajj_, wt_);
    controlplots_->etaprodjj->Fill(etaprodjj_, wt_);
    controlplots_->drjj->Fill(drjj_, wt_);
    controlplots_->dphijj->Fill(dphijj_, wt_);
  }


  void HinvControlPlots::FillWeightPlots(EventInfo * info){

    //bool lPrint=false;
    //if(sel_label_.find("DEta")!= sel_label_.npos) lPrint = true;
//     if (lPrint) {
//       std::cout << "------ debug ------" << std::endl
// 		<< "-- Total weight: " << wt_ << std::endl
// 		<< "--  pileup:" << info->weight_is_enabled("pileup") << " " << info->weight_defined("pileup") << " " << info->weight("pileup") << std::endl
// 		<< "-- trig_metL1: " << info->weight_is_enabled("trig_metL1") << " " << info->weight_defined("trig_metL1") << " " << info->weight("trig_metL1") << std::endl
// 		<< "-- trig_metHLT: " << info->weight_is_enabled("trig_metHLT") << " " << info->weight_defined("trig_metHLT") << " " << info->weight("trig_metHLT") << std::endl
// 		<< "-- trig_mjjHLT: " << info->weight_is_enabled("trig_mjjHLT") << " " << info->weight_defined("trig_mjjHLT") << " " << info->weight("trig_mjjHLT") << std::endl
// 		<< "-- trig_jet1HLT: " << info->weight_is_enabled("trig_jet1HLT") << " " << info->weight_defined("trig_jet1HLT") << " " << info->weight("trig_jet1HLT") << std::endl
// 		<< "-- trig_jet2HLT: " << info->weight_is_enabled("trig_jet2HLT") << " " << info->weight_defined("trig_jet2HLT") << " " << info->weight("trig_jet2HLT") << std::endl
// 		<< "-- idisoVeto: " << info->weight_is_enabled("idisoVeto") << " " << info->weight_defined("idisoVeto") << " " << info->weight("idisoVeto") << std::endl;
//     }

    //first check what is in total_weight: without the !
    //need to do this because for W samples, additional weight from parton reweighting....
    double wt_pu = info->weight_is_enabled("pileup")? info->weight("pileup") : 1.0;
    double wt_trig = (info->weight_is_enabled("trig_metL1")? info->weight("trig_metL1") : 1.0) *
      (info->weight_is_enabled("trig_metHLT") ? info->weight("trig_metHLT") : 1.0) *
      (info->weight_is_enabled("trig_mjjHLT") ? info->weight("trig_mjjHLT") : 1.0) *
      (info->weight_is_enabled("trig_jet1HLT") ? info->weight("trig_jet1HLT") : 1.0) *
      (info->weight_is_enabled("trig_jet2HLT") ? info->weight("trig_jet2HLT") : 1.0) ;
    double wt_idiso = 1.0;
    if (channel_ == "nunu" || channel_ == "taunu") wt_idiso = info->weight_is_enabled("idisoVeto")? info->weight("idisoVeto") : 1.0;
    else wt_idiso = info->weight_is_enabled("idisoTight")? info->weight("idisoTight") : 1.0;

    double wt = wt_/(wt_pu*wt_trig*wt_idiso);
    weightplots_->met_noW->Fill(met_, wt);
    weightplots_->dphijj_noW->Fill(dphijj_, wt);
    weightplots_->n_jets_noW->Fill(n_jets_, wt);

    wt = wt*wt_pu;
    weightplots_->met_pu->Fill(met_, wt);
    weightplots_->dphijj_pu->Fill(dphijj_, wt);
    weightplots_->n_jets_pu->Fill(n_jets_, wt);

    //now add also trig and idiso also if they were not enabled
    wt_trig = (info->weight_defined("trig_metL1") ? info->weight("trig_metL1") : 1.0) *
      (info->weight_defined("trig_metHLT") ? info->weight("trig_metHLT") : 1.0)  *
      (info->weight_defined("trig_mjjHLT") ? info->weight("trig_mjjHLT") : 1.0) *
      (info->weight_defined("trig_jet1HLT") ? info->weight("trig_jet1HLT") : 1.0)  *
      (info->weight_defined("trig_jet2HLT") ? info->weight("trig_jet2HLT") : 1.0) ;

    wt = wt*wt_trig;
    weightplots_->met_pu_trig->Fill(met_, wt);
    weightplots_->dphijj_pu_trig->Fill(dphijj_, wt);
    weightplots_->n_jets_pu_trig->Fill(n_jets_, wt);
    
    if (channel_ == "nunu" || channel_ == "taunu") wt_idiso = info->weight_defined("idisoVeto")? info->weight("idisoVeto") : 1.0;
    else wt_idiso = info->weight_defined("idisoTight") ? info->weight("idisoTight") : 1.0;
    //check against NAN
    if (wt_idiso != wt_idiso) wt_idiso=1.0;
    wt = wt*wt_idiso;
    weightplots_->met_pu_trig_idiso->Fill(met_, wt);
    weightplots_->dphijj_pu_trig_idiso->Fill(dphijj_, wt);
    weightplots_->n_jets_pu_trig_idiso->Fill(n_jets_, wt);
    
  }

  void HinvControlPlots::FillSystPlots(EventInfo * info){
    double wt_pu = info->weight_is_enabled("pileup") ? info->weight("pileup") : 1.0;
    double wt_pu_up = info->weight_defined("pileup_up") ? info->weight("pileup_up") : 1.0 ;
    double wt_pu_down = info->weight_defined("pileup_down") ? info->weight("pileup_down") : 1.0 ;
    systplots_->n_jets_puUp->Fill(n_jets_, wt_/wt_pu*wt_pu_up);
    systplots_->n_jets_puDown->Fill(n_jets_, wt_/wt_pu*wt_pu_down);
    systplots_->n_vtx_puUp->Fill(n_vtx_, wt_/wt_pu*wt_pu_down);
    systplots_->n_vtx_puDown->Fill(n_vtx_, wt_/wt_pu*wt_pu_down);
    systplots_->dphijj_puUp->Fill(dphijj_, wt_/wt_pu*wt_pu_down);
    systplots_->dphijj_puDown->Fill(dphijj_, wt_/wt_pu*wt_pu_down);
  }

}//namespace
