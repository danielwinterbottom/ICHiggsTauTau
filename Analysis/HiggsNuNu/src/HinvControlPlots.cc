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
    jCSV_allJets = dir.make<TH1F>("jCSV_allJets","jCSV_allJets",100,0,1);
    jCSV[0] = dir.make<TH1F>("jCSV_1","jCSV_1",100,0,1);
    jCSV[1] = dir.make<TH1F>("jCSV_2","jCSV_2",100,0,1);
    jCSV[2] = dir.make<TH1F>("jCSV_3","jCSV_3",100,0,1);
    jCSV[3] = dir.make<TH1F>("jCSV_4","jCSV_4",100,0,1);
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

  //Initialisation of HinvHTPlots struct
  HinvHTPlots::HinvHTPlots(TFileDirectory const& dir) {
    TH1F::SetDefaultSumw2();
    Ht                 = dir.make<TH1F>("Ht",                "Ht",                1000,   0,1000);
    SqrtHt             = dir.make<TH1F>("SqrtHt",            "SqrtHt",            1000,   0,1000);
    MetHt              = dir.make<TH2F>("MetHt",             "MetHt",             1000,   0,1000,1000,0,1000);
    MetSqrtHt          = dir.make<TH2F>("SqrtMetHt",         "SqrtMetHt",         1000,   0,1000,1000,0,1000);
    unclusteredEt      = dir.make<TH1F>("unclusteredEt",     "unclusteredEt",     1000,   0,1000);
    MHT                = dir.make<TH1F>("MHT",               "MHT",               1000,   0,1000);
    dphimetMHT         = dir.make<TH1F>("dphimetMHT",        "dphimetMHT",        630,    0,6.3);
    MetHt0to10         = dir.make<TH1F>("MetHt0to10",        "MetHt0to10",        1000,   0,1000);
    MetHt10to20         = dir.make<TH1F>("MetHt10to20",        "MetHt10to20",        1000,   0,1000);
    MetHt20to30         = dir.make<TH1F>("MetHt20to30",        "MetHt20to30",        1000,   0,1000);
    MetHt30to40         = dir.make<TH1F>("MetHt30to40",        "MetHt30to40",        1000,   0,1000);
    MetHt40to50         = dir.make<TH1F>("MetHt40to50",        "MetHt40to50",        1000,   0,1000);
    MetHt50to60         = dir.make<TH1F>("MetHt50to60",        "MetHt50to60",        1000,   0,1000);
    MetHt60to70         = dir.make<TH1F>("MetHt60to70",        "MetHt60to70",        1000,   0,1000);
    MetHt70to80         = dir.make<TH1F>("MetHt70to80",        "MetHt70to80",        1000,   0,1000);
    MetHt80to90         = dir.make<TH1F>("MetHt80to90",        "MetHt80to90",        1000,   0,1000);
    MetHt90to100         = dir.make<TH1F>("MetHt90to100",        "MetHt90to100",        1000,   0,1000);
    MetHt100to110         = dir.make<TH1F>("MetHt100to110",        "MetHt100to110",        1000,   0,1000);
    MetHt110to120         = dir.make<TH1F>("MetHt110to120",        "MetHt110to120",        1000,   0,1000);
    MetHt120to130         = dir.make<TH1F>("MetHt120to130",        "MetHt120to130",        1000,   0,1000);
    MetSqrtHt0to10         = dir.make<TH1F>("MetSqrtHt0to10",        "MetSqrtHt0to10",        1000,   0,1000);
    MetSqrtHt10to20         = dir.make<TH1F>("MetSqrtHt10to20",        "MetSqrtHt10to20",        1000,   0,1000);
    MetSqrtHt20to30         = dir.make<TH1F>("MetSqrtHt20to30",        "MetSqrtHt20to30",        1000,   0,1000);
    MetSqrtHt30to40         = dir.make<TH1F>("MetSqrtHt30to40",        "MetSqrtHt30to40",        1000,   0,1000);
    MetSqrtHt40to50         = dir.make<TH1F>("MetSqrtHt40to50",        "MetSqrtHt40to50",        1000,   0,1000);
    MetSqrtHt50to60         = dir.make<TH1F>("MetSqrtHt50to60",        "MetSqrtHt50to60",        1000,   0,1000);
    MetSqrtHt60to70         = dir.make<TH1F>("MetSqrtHt60to70",        "MetSqrtHt60to70",        1000,   0,1000);
    MetSqrtHt70to80         = dir.make<TH1F>("MetSqrtHt70to80",        "MetSqrtHt70to80",        1000,   0,1000);
    MetSqrtHt80to90         = dir.make<TH1F>("MetSqrtHt80to90",        "MetSqrtHt80to90",        1000,   0,1000);
    MetSqrtHt90to100         = dir.make<TH1F>("MetSqrtHt90to100",        "MetSqrtHt90to100",        1000,   0,1000);
    MetSqrtHt100to110         = dir.make<TH1F>("MetSqrtHt100to110",        "MetSqrtHt100to110",        1000,   0,1000);
    MetSqrtHt110to120         = dir.make<TH1F>("MetSqrtHt110to120",        "MetSqrtHt110to120",        1000,   0,1000);
    MetSqrtHt120to130         = dir.make<TH1F>("MetSqrtHt120to130",        "MetSqrtHt120to130",        1000,   0,1000);
  };

  
  HinvGenPlots::HinvGenPlots(TFileDirectory const& dir) {
    TH1F::SetDefaultSumw2();
    taupt = dir.make<TH1F>("taupt","taupt", 1000, 0, 1000); 
    taueta = dir.make<TH1F>("taueta","taueta", 100, -5, 5);
    tauptvseta = dir.make<TH2F>("tauptvseta","tauptvseta", 100, -5, 5, 500,0,500);
    mindR_gentau_tagjets = dir.make<TH1F>("mindR_gentau_tagjets","mindR_gentau_tagjets", 100, 0, 6);
    dR_genjet_gentau = dir.make<TH1F>("dR_genjet_gentau","dR_genjet_gentau", 100, 0, 6);
    dR_recotau_genjet = dir.make<TH1F>("dR_recotau_genjet","dR_recotau_genjet", 100, 0, 6);
    dR_recotau_status3tau = dir.make<TH1F>("dR_recotau_status3tau","dR_recotau_status3tau", 100, 0, 6);
    recotaupt = dir.make<TH1F>("recotaupt","recotaupt", 500, 0, 500);
    recotaueta = dir.make<TH1F>("recotaueta","recotaueta", 100, -3, 3);
    recotaupt_status3 = dir.make<TH1F>("recotaupt_status3","recotaupt_status3", 500, 0, 500);
    recotaueta_status3 = dir.make<TH1F>("recotaueta_status3","recotaueta_status3", 100, -3, 3);
    recojet_isMatched[0] = dir.make<TH1F>("recojet_0_isMatched","recojet_0_isMatched",2,0,2);
    recojet_isMatched[1] = dir.make<TH1F>("recojet_1_isMatched","recojet_1_isMatched",2,0,2);
    recojet_isMatched[2] = dir.make<TH1F>("recojet_2_isMatched","recojet_2_isMatched",2,0,2);
    recojet_isMatched[3] = dir.make<TH1F>("recojet_3_isMatched","recojet_3_isMatched",2,0,2);
    recojet_isMatched[4] = dir.make<TH1F>("recojet_4_isMatched","recojet_4_isMatched",2,0,2);
  }


  HinvControlPlots::HinvControlPlots(std::string const& name): ModuleBase(name){
    fs_ = NULL;
    met_label_ = "pfMet";
    dijet_label_ = "jjCandidates";
    genparticles_label_ = "genParticlesTaus";
    sel_label_ = "JetPair";
    channel_ = "nunu";
    is_data_ = false;
    is_embedded_ = false;
  }

  HinvControlPlots::~HinvControlPlots(){;}


  int  HinvControlPlots::PreAnalysis(){
    std::cout << "-------------------------------------------- " << std::endl
	      << "** PreAnalysis Info for HinvControlPlots **" << std::endl
	      << "-------------------------------------------- " << std::endl;
    if (fs_) {
      std::cout << "MET Label: " << met_label_ << std::endl;
      std::cout << "dijet Label: " << dijet_label_ << std::endl;
      std::cout << "gen particles label: " << genparticles_label_ << std::endl;
      std::cout << "Selection Label: " << sel_label_ << std::endl;
      std::cout << "Channel :" << channel_ << std::endl;
      if (is_embedded_ && is_data_) std::cout << "Processing set for embedded data !" << std::endl;
      else if (is_data_) std::cout << "Processing set for data !" << std::endl;
      else if (is_embedded_) std::cout << "Processing set for embedded MC !" << std::endl;
      else  std::cout << "Processing set for MC !" << std::endl;
    }

    misc_plots_ = new DynamicHistoSet(fs_->mkdir("misc_plots"));
    misc_2dplots_ = new Dynamic2DHistoSet(fs_->mkdir("misc_2dplots"));
    InitCoreControlPlots();
    InitWeightPlots();
    InitSystPlots();
    InitDijetMETPlots();
    InitHTPlots();
    InitGenPlots();

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
    
    std::vector<PFJet*> alljets = event->GetPtrVec<PFJet>("AllpfJetsPFlow");
    std::sort(alljets.begin(), alljets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    

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
    
    // Start: HinvDijetMETPlots Variables __________________________________
    ROOT::Math::PtEtaPhiEVector diffVector(0,0,0,0);
    //double diffEtValue = 0;
    double diffPtValue = 0;
    double scalSumPt = 0;
    
    double dijet_MET = 0;
    double htPlusMET = 0;
    // End: Filling HinvDijetMETPlots Variables ____________________________


    // Enable filling of plots
    bool fillPlots             = true;
    //AM-FIX: no need for different bool, or signal/QCD won't be filled properly !
    //bool fillHinvDijetMETPlots = true;

    //get nJetsInGap from the event.
    n_jetsingap_ = 0;

    if (event->Exists("nJetsInGap")) n_jetsingap_ = event->Get<unsigned>("nJetsInGap");
    //else std::cerr << " WARNING ! Variable nJetsInGap not found in event ! Set to 0." << std::endl;
    
    if (dijet_vec.size() != 0) {
      
      CompositeCandidate const* dijet = dijet_vec.at(0);

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
      
      //decide whether to fill plots or not for a few specific selections
      //allowing to have them in parallel to the main stream
        
      if (sel_label_.find("AN") != sel_label_.npos) {
	if(channel_ == "munu" || channel_ == "mumu" || channel_ == "nunuiglep"){
	  if (!(met_noMuons->pt() > 130 && dijet->M()>1000)) fillPlots=false;
	}
	else{
	  if (!(met->pt() > 130 && dijet->M()>1000)) fillPlots=false;
	}
      }

      if (sel_label_.find("CJVfail") != sel_label_.npos && n_jetsingap_==0) fillPlots = false;
      if (sel_label_.find("CJVpass") != sel_label_.npos && n_jetsingap_!=0) fillPlots = false;

      if (sel_label_.find("QCD") != sel_label_.npos &&
	  PairAbsDPhiLessThan(dijet,2.6)) fillPlots = false;
      if (sel_label_.find("SIGNAL") != sel_label_.npos &&
	  !PairAbsDPhiLessThan(dijet,1.0)) fillPlots = false;
      
      // Start: Calculating HinvDijetMETPlots __________________________________
      htPlusMET = dijet->ScalarPtSum() + met->vector().Pt();
      
      diffVector   = dijet->vector() + met->vector();
      //diffEtValue  = diffVector.Et();
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
       //diffEtValue = diffVector.Et();
       diffPtValue = diffVector.Pt();
       
       dijet_MET = dijet.Pt()/(met->vector().Pt()+dijet.Pt());
       scalSumPt = dijet.Pt()+met->vector().Pt();
       // End: Calculating HinvDijetMETPlots ____________________________________
      
    }

    //Start: HinvHTPlots variables
    double ht =0;
    double sqrtht=0;
    double metet=0;
    ROOT::Math::PtEtaPhiEVector mhtVec(0,0,0,0);
    // End: HinvHTPlots variables

    //Start: Calculating ht things
    for(int i =0; unsigned(i)<alljets.size();++i){
      ht+=alljets[i]->vector().Et();
      mhtVec += alljets[i]->vector();
    }
    sqrtht=sqrt(ht);
    metet=met->vector().Et();

    ROOT::Math::PtEtaPhiEVector unclVec = mhtVec + met->vector();

    //End: Calculating ht things
    
    n_vtx_ = eventInfo->good_vertices();
    
    met_ = met->pt();
    met_noMuons_ = met_noMuons->pt();
    met_phi_ = met->phi();
    
    n_jets_ = jets.size();
    if (fillPlots) {//if fillplots
      FillCoreControlPlots();
      FillWeightPlots(eventInfoNonConst);
      FillSystPlots(eventInfoNonConst);

      //fill btagging
      for (unsigned iR(0); iR < jets.size(); ++iR){//loop on recojets
	double lCSV = jets[iR]->GetBDiscriminator("combinedSecondaryVertexBJetTags");
	controlplots_->jCSV_allJets->Fill(lCSV,wt_);
	if (iR<4)controlplots_->jCSV[iR]->Fill(lCSV,wt_);
      }

      if(!is_data_ || is_embedded_){

	std::vector<GenParticle*> const& taus = event->GetPtrVec<GenParticle>(genparticles_label_);
	GenParticle* theTau = 0;
	for (unsigned iTau(0); iTau<taus.size(); ++iTau){
	  if (fabs(taus[iTau]->pdgid())==15) {
	    theTau=taus[iTau];
	    break;
	  }
	}
	GenJet *gentau = 0;

	//get recojets matched to genjets
	if (!is_embedded_) {
	  std::vector<GenJet *> genjets = event->GetPtrVec<GenJet>("genJets");

	  //get generically the jets matched...
	  for (unsigned iR(0); iR < jets.size(); ++iR){//loop on recojets
	    double mindR = 10;
	    GenJet *genmatch = 0;
	    for (unsigned iG(0); iG < genjets.size(); ++iG){//loop on genjets
	      double dR = ROOT::Math::VectorUtil::DeltaR(jets[iR]->vector(),genjets[iG]->vector());
	      if (dR < mindR){
		mindR = dR;
		genmatch = genjets[iG];
	      }
	    }//loop on genjets
	    if (mindR < 0.5 && iR<5) genPlots_->recojet_isMatched[iR]->Fill(1.,wt_);
	    else if (iR<5) genPlots_->recojet_isMatched[iR]->Fill(0.,wt_);
	  }//loop on recojets

	  //get genjet matched with leading tau
	  if (theTau) {
	    //std::cout << "first tauParticle pdgid = " << theTau->pdgid() << std::endl;
	    double mindR = 10;
	    for (unsigned iG(0); iG < genjets.size(); ++iG){//loop on genjets
	      double dR = ROOT::Math::VectorUtil::DeltaR(theTau->vector(),genjets[iG]->vector());
	      if (dR < mindR){
		mindR = dR;
		if (mindR < 0.5) gentau = genjets[iG];
	      }
	    }//end of loop on genjets
	    
	    genPlots_->dR_genjet_gentau->Fill(mindR,wt_);
	  }//if theTau

	}//if !embedded
	if (gentau) {
	  genPlots_->taupt->Fill(gentau->pt(),wt_);
	  genPlots_->taueta->Fill(gentau->eta(),wt_);
	  genPlots_->tauptvseta->Fill(gentau->eta(),gentau->pt(),wt_);
	}
	else if (theTau){
	  genPlots_->taupt->Fill(theTau->pt(),wt_);
	  genPlots_->taueta->Fill(theTau->eta(),wt_);
	  genPlots_->tauptvseta->Fill(theTau->eta(),theTau->pt(),wt_);
	}
	//get dR with tagging jets
	if (dijet_vec.size() != 0) {
	  
	  CompositeCandidate const* dijet = dijet_vec.at(0);
	  
	  Candidate const* jet1 = dijet->GetCandidate("jet1");
	  Candidate const* jet2 = dijet->GetCandidate("jet2");
	  
	  double dR1 = 10;
	  double dR2 = 10;
	  if (gentau) {
	    dR1 = ROOT::Math::VectorUtil::DeltaR(jet1->vector(),gentau->vector());
	    dR2 = ROOT::Math::VectorUtil::DeltaR(jet2->vector(),gentau->vector());
	    if (gentau->pt()>10 && fabs(gentau->eta())<2.4) genPlots_->mindR_gentau_tagjets->Fill(std::min(dR1,dR2),wt_);
	  }
	  else if (theTau){
	    dR1 = ROOT::Math::VectorUtil::DeltaR(jet1->vector(),theTau->vector());
	    dR2 = ROOT::Math::VectorUtil::DeltaR(jet2->vector(),theTau->vector());
	    if (theTau->pt()>10 && fabs(theTau->eta())<2.4) genPlots_->mindR_gentau_tagjets->Fill(std::min(dR1,dR2),wt_);
	  }
	  
	}
	
	//get recojet matched with genjet or gentau
	double mindR = 10;
	PFJet* recotau = 0;
	PFJet* recotau_status3 = 0;
	double mindR_status3 = 10;
	
	for (unsigned iR(0); iR < alljets.size(); ++iR){//loop on reco jets
	  //match to genjets
	  if (gentau){
	    double dR = ROOT::Math::VectorUtil::DeltaR(alljets[iR]->vector(),gentau->vector());
	    if (dR < mindR){
	      mindR = dR;
	      recotau = alljets[iR];
	    }
	  }
	  if (theTau){
	    //match to status 3 tau
	    double dR_status3 = ROOT::Math::VectorUtil::DeltaR(alljets[iR]->vector(),theTau->vector());
	    if (dR_status3 < mindR_status3){
	      mindR_status3 = dR_status3;
	      recotau_status3 = alljets[iR];
	    }
	  }
	}//end of loop on reco jets
	    
	genPlots_->dR_recotau_genjet->Fill(mindR,wt_);
	genPlots_->dR_recotau_status3tau->Fill(mindR_status3,wt_);
	
	if (gentau && mindR < 0.5 && gentau->pt()>10 && fabs(gentau->eta())<2.4) {
	  genPlots_->recotaupt->Fill(recotau->pt(),wt_);
	  genPlots_->recotaueta->Fill(recotau->eta(),wt_);
	}
	if (theTau && mindR_status3 < 0.5 && theTau->pt()>10 && fabs(theTau->eta())<2.4) {
	  genPlots_->recotaupt_status3->Fill(recotau_status3->pt(),wt_);
	  genPlots_->recotaueta_status3->Fill(recotau_status3->eta(),wt_);
	}
      }//for MC or embedded data
    }//end of if fillplots
    

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

    //Start: Filling HinvHTPlots
    if(fillPlots){
      HTPlots_->Ht->Fill(ht,wt_);
      HTPlots_->SqrtHt->Fill(sqrtht,wt_);
      HTPlots_->MetHt->Fill(ht,metet,wt_);
      HTPlots_->MetSqrtHt->Fill(sqrtht,metet,wt_);
      HTPlots_->MHT->Fill(mhtVec.Et(),wt_);
      HTPlots_->dphimetMHT->Fill(fabs(mhtVec.Phi()-met->vector().Phi()),wt_);
      HTPlots_->unclusteredEt->Fill(unclVec.Et(),wt_);
      if((metet>0.)&&(metet<=10.)){
	HTPlots_->MetHt0to10->Fill(ht,wt_);
	HTPlots_->MetSqrtHt0to10->Fill(sqrtht,wt_);
      }
      else if((metet>10.)&&(metet<=20.)){
	HTPlots_->MetHt10to20->Fill(ht,wt_);
	HTPlots_->MetSqrtHt10to20->Fill(sqrtht,wt_);
      }
      else if((metet>20.)&&(metet<=30.)){
	HTPlots_->MetHt20to30->Fill(ht,wt_);
	HTPlots_->MetSqrtHt20to30->Fill(sqrtht,wt_);
      }
      else if((metet>30.)&&(metet<=40.)){
	HTPlots_->MetHt30to40->Fill(ht,wt_);
	HTPlots_->MetSqrtHt30to40->Fill(sqrtht,wt_);
      }
      else if((metet>40.)&&(metet<=50.)){
	HTPlots_->MetHt40to50->Fill(ht,wt_);
	HTPlots_->MetSqrtHt40to50->Fill(sqrtht,wt_);
      }
      else if((metet>50.)&&(metet<=60.)){
	HTPlots_->MetHt50to60->Fill(ht,wt_);
	HTPlots_->MetSqrtHt50to60->Fill(sqrtht,wt_);
      }
      else if((metet>60.)&&(metet<=70.)){
	HTPlots_->MetHt60to70->Fill(ht,wt_);
	HTPlots_->MetSqrtHt60to70->Fill(sqrtht,wt_);
      }
      else if((metet>70.)&&(metet<=80.)){
	HTPlots_->MetHt70to80->Fill(ht,wt_);
	HTPlots_->MetSqrtHt70to80->Fill(sqrtht,wt_);
      }
      else if((metet>80.)&&(metet<=90.)){
	HTPlots_->MetHt80to90->Fill(ht,wt_);
	HTPlots_->MetSqrtHt80to90->Fill(sqrtht,wt_);
      }
      else if((metet>90.)&&(metet<=100.)){
	HTPlots_->MetHt90to100->Fill(ht,wt_);
	HTPlots_->MetSqrtHt90to100->Fill(sqrtht,wt_);
      }
      else if((metet>100.)&&(metet<=110.)){
	HTPlots_->MetHt100to110->Fill(ht,wt_);
	HTPlots_->MetSqrtHt100to110->Fill(sqrtht,wt_);
      }
      else if((metet>110.)&&(metet<=120.)){
	HTPlots_->MetHt110to120->Fill(ht,wt_);
	HTPlots_->MetSqrtHt110to120->Fill(sqrtht,wt_);
      }
      else if((metet>120.)&&(metet<=130.)){
	HTPlots_->MetHt120to130->Fill(ht,wt_);
	HTPlots_->MetSqrtHt120to130->Fill(sqrtht,wt_);
      }
      //std::cout << " MHT " << mhtVec << std::endl
      //		<< " MuVEC " << muVec << std::endl
      //	<< " Uncl " << unclVec << std::endl;

    }
    //End: Filling HinvHTPlots

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

  void HinvControlPlots::InitHTPlots() {
    HTPlots_ = new HinvHTPlots(fs_->mkdir(sel_label_+"/Ht"));
    std::cout << " Ht plots initialised" << std::endl;
  }
  
  void HinvControlPlots::InitGenPlots() {
    genPlots_ = new HinvGenPlots(fs_->mkdir(sel_label_+"/Gen"));
    std::cout << " Gen plots initialised" << std::endl;
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


