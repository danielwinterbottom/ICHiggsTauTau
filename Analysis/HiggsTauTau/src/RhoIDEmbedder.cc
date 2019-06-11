#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/RhoIDEmbedder.h"

 

namespace ic {

  RhoIDEmbedder::RhoIDEmbedder(std::string const& name) : ModuleBase(name), channel_(channel::tt), strategy_(strategy::cpdecays16) {
    fs_ = NULL;
    maketrees_ = false;
    gammas_pt_cut_ = 0.5;
  }

  RhoIDEmbedder::~RhoIDEmbedder() {
    ;
  }

  double RhoIDEmbedder::read_mva_score(std::vector<double> vars) {
      var0=vars[0], var1=vars[1], var2=vars[2], var3=vars[3], var4=vars[4], var5=vars[5], var6=vars[6], var7=vars[7], var8=vars[8], var9=vars[9], var10=vars[10], var12=vars[12], var13=vars[13];
      double score = (reader_->EvaluateMVA( "BDT method" )+1)/2;
      return score;
  }


  int RhoIDEmbedder::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "RhoIDEmbedder" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    if(strategy_==strategy::cpdecays17) {
      gammas_pt_cut_ = 1.0;
    }

    if(fs_&&maketrees_){  
      outtree_ = fs_->make<TTree>("train_ntuple","train_ntuple");
      outtree_->Branch("tauFlag1"       , &tauFlag1_ );     
      outtree_->Branch("tauFlag2"       , &tauFlag2_ );
      outtree_->Branch("gen_match_1"    , &gen_match_1_);
      outtree_->Branch("gen_match_2"    , &gen_match_2_);
      outtree_->Branch("wt"             , &wt_       );
      outtree_->Branch("Ngammas_1"      , &Ngammas_1_ );
      
      outtree_->Branch("Egamma1_1"      , &Egamma1_1_ );
      outtree_->Branch("Egamma2_1"      , &Egamma2_1_ );
      outtree_->Branch("Egamma3_1"      , &Egamma3_1_ );
      outtree_->Branch("Egamma4_1"      , &Egamma4_1_ );
      outtree_->Branch("Egamma5_1"      , &Egamma5_1_ );
      outtree_->Branch("Egamma6_1"      , &Egamma6_1_ );
      outtree_->Branch("Egamma7_1"      , &Egamma7_1_ );
      outtree_->Branch("Egamma8_1"      , &Egamma8_1_ );
      outtree_->Branch("Egamma9_1"      , &Egamma9_1_ );
      outtree_->Branch("Egamma10_1"     , &Egamma10_1_ );
      outtree_->Branch("Egamma11_1"     , &Egamma11_1_ );
      outtree_->Branch("Egamma12_1"     , &Egamma12_1_ );
      
      outtree_->Branch("Epi_1"          , &Epi_1_ );
      outtree_->Branch("Mpi0_1"         , &Mpi0_1_ );
      outtree_->Branch("Mrho_1"         , &Mrho_1_ );
      //outtree_->Branch("dphi_1"       , &dphi_1_ );
      //outtree_->Branch("dEta_1"       , &dEta_1_ );
      outtree_->Branch("gammas_dEta_1"  , &gammas_dEta_1_ );
      outtree_->Branch("gammas_dphi_1"  , &gammas_dphi_1_ );
      outtree_->Branch("pt_1"           , &pt_1_ );
      outtree_->Branch("eta_1"          , &eta_1_ );
      outtree_->Branch("phi_1"          , &phi_1_ );
      outtree_->Branch("E_1"            , &E_1_);
      outtree_->Branch("rho_dphi_1"     ,&rho_dphi_1_);
      outtree_->Branch("rho_dEta_1"     ,&rho_dEta_1_);
      outtree_->Branch("Epi0_1"         ,&Epi0_1_);
      outtree_->Branch("tau_decay_mode_1",&tau_decay_mode_1_);
      outtree_->Branch("Ngammas_2"      , &Ngammas_2_ );

      outtree_->Branch("Egamma1_2"      , &Egamma1_2_ );
      outtree_->Branch("Egamma2_2"      , &Egamma2_2_ );
      outtree_->Branch("Egamma3_2"      , &Egamma3_2_ );
      outtree_->Branch("Egamma4_2"      , &Egamma4_2_ );
      outtree_->Branch("Egamma5_2"      , &Egamma5_2_ );
      outtree_->Branch("Egamma6_2"      , &Egamma6_2_ );
      outtree_->Branch("Egamma7_2"      , &Egamma7_2_ );
      outtree_->Branch("Egamma8_2"      , &Egamma8_2_ );
      outtree_->Branch("Egamma9_2"      , &Egamma9_2_ );
      outtree_->Branch("Egamma10_2"     , &Egamma10_2_ );
      outtree_->Branch("Egamma11_2"     , &Egamma11_2_ );
      outtree_->Branch("Egamma12_2"     , &Egamma12_2_ );
      
      outtree_->Branch("Epi_2"          , &Epi_2_ );
      outtree_->Branch("Mpi0_2"         , &Mpi0_2_ );
      outtree_->Branch("Mrho_2"         , &Mrho_2_ );
      //outtree_->Branch("dphi_2"       , &dphi_2_ );
      //outtree_->Branch("dEta_2"       , &dEta_2_ );
      outtree_->Branch("gammas_dEta_2", &gammas_dEta_2_ );
      outtree_->Branch("gammas_dphi_2", &gammas_dphi_2_ );
      outtree_->Branch("pt_2"         , &pt_2_ );
      outtree_->Branch("eta_2"        , &eta_2_ );
      outtree_->Branch("phi_2"        , &phi_2_ );
      outtree_->Branch("E_2"          , &E_2_);
      outtree_->Branch("rho_dphi_2"   ,&rho_dphi_2_);
      outtree_->Branch("rho_dEta_2"   ,&rho_dEta_2_);
      outtree_->Branch("Epi0_2"       ,&Epi0_2_);
      outtree_->Branch("tau_decay_mode_2",&tau_decay_mode_2_);
      
//Added by Mohammad
      outtree_->Branch("ConeRadiusMax_2"     ,&ConeRadiusMax_2_);
      outtree_->Branch("ConeRadiusMedian_2"  ,&ConeRadiusMedian_2_);
      outtree_->Branch("ConeRadiusMean_2"    ,&ConeRadiusMean_2_);
      outtree_->Branch("ConeRadiusStdDev_2"  ,&ConeRadiusStdDev_2_);
      outtree_->Branch("ConeRadiusMax_1"     ,&ConeRadiusMax_1_);
      outtree_->Branch("ConeRadiusMedian_1"  ,&ConeRadiusMedian_1_);
      outtree_->Branch("ConeRadiusMean_1"    ,&ConeRadiusMean_1_);
      outtree_->Branch("ConeRadiusStdDev_1"  ,&ConeRadiusStdDev_1_);

 
      outtree_->Branch("ConeRadiusMaxWRTtau_2"     ,&ConeRadiusMaxWRTtau_2_);
      outtree_->Branch("ConeRadiusMedianWRTtau_2"  ,&ConeRadiusMedianWRTtau_2_);
      outtree_->Branch("ConeRadiusMeanWRTtau_2"    ,&ConeRadiusMeanWRTtau_2_);
      outtree_->Branch("ConeRadiusStdDevWRTtau_2"  ,&ConeRadiusStdDevWRTtau_2_);
      outtree_->Branch("ConeRadiusMaxWRTtau_1"     ,&ConeRadiusMaxWRTtau_1_);
      outtree_->Branch("ConeRadiusMedianWRTtau_1"  ,&ConeRadiusMedianWRTtau_1_);
      outtree_->Branch("ConeRadiusMeanWRTtau_1"    ,&ConeRadiusMeanWRTtau_1_);
      outtree_->Branch("ConeRadiusStdDevWRTtau_1"  ,&ConeRadiusStdDevWRTtau_1_);

      outtree_->Branch("ConeRadiusMaxWRTpi0_2"     ,&ConeRadiusMaxWRTpi0_2_);
      outtree_->Branch("ConeRadiusMedianWRTpi0_2"  ,&ConeRadiusMedianWRTpi0_2_);
      outtree_->Branch("ConeRadiusMeanWRTpi0_2"    ,&ConeRadiusMeanWRTpi0_2_);
      outtree_->Branch("ConeRadiusStdDevWRTpi0_2"  ,&ConeRadiusStdDevWRTpi0_2_);
      outtree_->Branch("ConeRadiusMaxWRTpi0_1"     ,&ConeRadiusMaxWRTpi0_1_);
      outtree_->Branch("ConeRadiusMedianWRTpi0_1"  ,&ConeRadiusMedianWRTpi0_1_);
      outtree_->Branch("ConeRadiusMeanWRTpi0_1"    ,&ConeRadiusMeanWRTpi0_1_);
      outtree_->Branch("ConeRadiusStdDevWRTpi0_1"  ,&ConeRadiusStdDevWRTpi0_1_);
  
      outtree_->Branch("NgammasModif_1"       , &NgammasModif_1_ );
      outtree_->Branch("NgammasModif_2"       , &NgammasModif_2_ );

      outtree_->Branch("DeltaR2WRTtau_1"      , &DeltaR2WRTtau_1_);
      outtree_->Branch("DeltaR2WRTtau_2"      , &DeltaR2WRTtau_2_);
      outtree_->Branch("DeltaR2WRTpi0_1"      , &DeltaR2WRTpi0_1_);
      outtree_->Branch("DeltaR2WRTpi0_2"      , &DeltaR2WRTpi0_2_);

      outtree_->Branch("Etagamma1_1"          , &Etagamma1_1_);
      outtree_->Branch("Etagamma2_1"          , &Etagamma2_1_);
      outtree_->Branch("Etagamma3_1"          , &Etagamma3_1_);
      outtree_->Branch("Etagamma4_1"          , &Etagamma4_1_);
      outtree_->Branch("Etagamma5_1"          , &Etagamma5_1_);
      outtree_->Branch("Etagamma6_1"          , &Etagamma6_1_);
      outtree_->Branch("Etagamma7_1"          , &Etagamma7_1_);
      outtree_->Branch("Etagamma8_1"          , &Etagamma8_1_);
      outtree_->Branch("Etagamma9_1"          , &Etagamma9_1_);
      outtree_->Branch("Etagamma10_1"         , &Etagamma10_1_);
      outtree_->Branch("Etagamma11_1"         , &Etagamma11_1_);
      outtree_->Branch("Etagamma12_1"         , &Etagamma12_1_);
      
      outtree_->Branch("Etagamma1_2"          , &Etagamma1_2_);
      outtree_->Branch("Etagamma2_2"          , &Etagamma2_2_);
      outtree_->Branch("Etagamma3_2"          , &Etagamma3_2_);
      outtree_->Branch("Etagamma4_2"          , &Etagamma4_2_);
      outtree_->Branch("Etagamma5_2"          , &Etagamma5_2_);
      outtree_->Branch("Etagamma6_2"          , &Etagamma6_2_);
      outtree_->Branch("Etagamma7_2"          , &Etagamma7_2_);
      outtree_->Branch("Etagamma8_2"          , &Etagamma8_2_);
      outtree_->Branch("Etagamma9_2"          , &Etagamma9_2_);
      outtree_->Branch("Etagamma10_2"         , &Etagamma10_2_);
      outtree_->Branch("Etagamma11_2"         , &Etagamma11_2_);
      outtree_->Branch("Etagamma12_2"         , &Etagamma12_2_);
      
      outtree_->Branch("Etapi_1"              , &Etapi_1_);
      outtree_->Branch("Etapi_2"              , &Etapi_2_);

      outtree_->Branch("Phigamma1_1"          , &Phigamma1_1_);
      outtree_->Branch("Phigamma2_1"          , &Phigamma2_1_);
      outtree_->Branch("Phigamma3_1"          , &Phigamma3_1_);
      outtree_->Branch("Phigamma4_1"          , &Phigamma4_1_);
      outtree_->Branch("Phigamma5_1"          , &Phigamma5_1_);
      outtree_->Branch("Phigamma6_1"          , &Phigamma6_1_);
      outtree_->Branch("Phigamma7_1"          , &Phigamma7_1_);
      outtree_->Branch("Phigamma8_1"          , &Phigamma8_1_);
      outtree_->Branch("Phigamma9_1"          , &Phigamma9_1_);
      outtree_->Branch("Phigamma10_1"         , &Phigamma10_1_);
      outtree_->Branch("Phigamma11_1"         , &Phigamma11_1_);
      outtree_->Branch("Phigamma12_1"         , &Phigamma12_1_);
      
      outtree_->Branch("Phigamma1_2"          , &Phigamma1_2_);
      outtree_->Branch("Phigamma2_2"          , &Phigamma2_2_);
      outtree_->Branch("Phigamma3_2"          , &Phigamma3_2_);
      outtree_->Branch("Phigamma4_2"          , &Phigamma4_2_);
      outtree_->Branch("Phigamma5_2"          , &Phigamma5_2_);
      outtree_->Branch("Phigamma6_2"          , &Phigamma6_2_);
      outtree_->Branch("Phigamma7_2"          , &Phigamma7_2_);
      outtree_->Branch("Phigamma8_2"          , &Phigamma8_2_);
      outtree_->Branch("Phigamma9_2"          , &Phigamma9_2_);
      outtree_->Branch("Phigamma10_2"         , &Phigamma10_2_);
      outtree_->Branch("Phigamma11_2"         , &Phigamma11_2_);
      outtree_->Branch("Phigamma12_2"         , &Phigamma12_2_);
   
      outtree_->Branch("Phipi_1"              , &Phipi_1_);
      outtree_->Branch("Phipi_2"              , &Phipi_2_);

      outtree_->Branch("FracPtDepos_dRLessThan0p008_2"              , &FracPtDepos_dRLessThan0p008_2_);
      outtree_->Branch("FracPtDepos_dRMoreThan0p008_2"              , &FracPtDepos_dRMoreThan0p008_2_);

      outtree_->Branch("FracPtDepos_dRLessThan0p008_1"              , &FracPtDepos_dRLessThan0p008_1_);
      outtree_->Branch("FracPtDepos_dRMoreThan0p008_1"              , &FracPtDepos_dRMoreThan0p008_1_);
      
      outtree_->Branch("Mpi0_TwoHighGammas_2"                       , &Mpi0_TwoHighGammas_2_); 
      outtree_->Branch("Mpi0_ThreeHighGammas_2"                     , &Mpi0_ThreeHighGammas_2_); 
      outtree_->Branch("Mpi0_FourHighGammas_2"                      , &Mpi0_FourHighGammas_2_); 
      
      outtree_->Branch("Mpi0_TwoHighGammas_1"                       , &Mpi0_TwoHighGammas_1_); 
      outtree_->Branch("Mpi0_ThreeHighGammas_1"                     , &Mpi0_ThreeHighGammas_1_); 
      outtree_->Branch("Mpi0_FourHighGammas_1"                      , &Mpi0_FourHighGammas_1_); 
      
      outtree_->Branch("Mrho_OneHighGammas_1"                       , &Mrho_OneHighGammas_1_);
      outtree_->Branch("Mrho_TwoHighGammas_1"                       , &Mrho_TwoHighGammas_1_);
      outtree_->Branch("Mrho_ThreeHighGammas_1"                     , &Mrho_ThreeHighGammas_1_);
      outtree_->Branch("Mrho_subleadingGamma_1"                     , &Mrho_subleadingGamma_1_);

      outtree_->Branch("Mrho_OneHighGammas_2"                       , &Mrho_OneHighGammas_2_);
      outtree_->Branch("Mrho_TwoHighGammas_2"                       , &Mrho_TwoHighGammas_2_);
      outtree_->Branch("Mrho_ThreeHighGammas_2"                     , &Mrho_ThreeHighGammas_2_);
      outtree_->Branch("Mrho_subleadingGamma_2"                     , &Mrho_subleadingGamma_2_);
   
      outtree_->Branch("Ptgamma1_1"      , &Ptgamma1_1_ );
      outtree_->Branch("Ptgamma2_1"      , &Ptgamma2_1_ );
      outtree_->Branch("Ptgamma3_1"      , &Ptgamma3_1_ );
      outtree_->Branch("Ptgamma4_1"      , &Ptgamma4_1_ );
   
      outtree_->Branch("Ptgamma1_2"      , &Ptgamma1_2_ );
      outtree_->Branch("Ptgamma2_2"      , &Ptgamma2_2_ );
      outtree_->Branch("Ptgamma3_2"      , &Ptgamma3_2_ );
      outtree_->Branch("Ptgamma4_2"      , &Ptgamma4_2_ );

      outtree_->Branch("wt_cp_sm", &wt_cp_sm_);
      outtree_->Branch("wt_cp_ps", &wt_cp_ps_);
      outtree_->Branch("wt_cp_mm", &wt_cp_mm_);
      outtree_->Branch("strip_pt_2", &strip_pt_2_);
      outtree_->Branch("strip_pt_1", &strip_pt_1_);
      outtree_->Branch("strip_pi_mass_1", &strip_pi_mass_1_);
      outtree_->Branch("strip_pi_mass_2", &strip_pi_mass_2_);

      outtree_->Branch("lead_gamma_pt_1", &lead_gamma_pt_1_);
      outtree_->Branch("lead_gamma_pt_2", &lead_gamma_pt_2_);
    
      outtree_->Branch("event", &event_);

      outtree_->Branch("mass0_1", &mass0_1_      );
      outtree_->Branch("mass1_1", &mass1_1_      );
      outtree_->Branch("mass2_1", &mass2_1_      );
      outtree_->Branch("E1_1", &E1_1_         );
      outtree_->Branch("E2_1", &E2_1_         );
      outtree_->Branch("E3_1", &E3_1_         );
      outtree_->Branch("strip_E_1", &strip_E_1_    );
      outtree_->Branch("a1_pi0_dEta_1", &a1_pi0_dEta_1_);
      outtree_->Branch("a1_pi0_dphi_1", &a1_pi0_dphi_1_);

      outtree_->Branch("mass0_2", &mass0_2_      );
      outtree_->Branch("mass1_2", &mass1_2_      );
      outtree_->Branch("mass2_2", &mass2_2_      );
      outtree_->Branch("E1_2", &E1_2_         );
      outtree_->Branch("E2_2", &E2_2_         );
      outtree_->Branch("E3_2", &E3_2_         );
      outtree_->Branch("strip_E_2", &strip_E_2_    );
      outtree_->Branch("a1_pi0_dEta_2", &a1_pi0_dEta_2_);
      outtree_->Branch("a1_pi0_dphi_2", &a1_pi0_dphi_2_);

      outtree_->Branch("h1_h2_dphi_2", &h1_h2_dphi_2_);
      outtree_->Branch("h1_h3_dphi_2", &h1_h3_dphi_2_);
      outtree_->Branch("h2_h3_dphi_2", &h2_h3_dphi_2_);
      outtree_->Branch("h1_h2_dEta_2", &h1_h2_dEta_2_);
      outtree_->Branch("h1_h3_dEta_2", &h1_h3_dEta_2_);
      outtree_->Branch("h2_h3_dEta_2", &h2_h3_dEta_2_);
      outtree_->Branch("h1_h2_dphi_1", &h1_h2_dphi_1_);
      outtree_->Branch("h1_h3_dphi_1", &h1_h3_dphi_1_);
      outtree_->Branch("h2_h3_dphi_1", &h2_h3_dphi_1_);
      outtree_->Branch("h1_h2_dEta_1", &h1_h2_dEta_1_);
      outtree_->Branch("h1_h3_dEta_1", &h1_h3_dEta_1_);
      outtree_->Branch("h2_h3_dEta_1", &h2_h3_dEta_1_);

      outtree_->Branch("lead_strip_pt_2", &lead_strip_pt_2_);
      outtree_->Branch("closest_strip_pt_2", &closest_strip_pt_2_);
      outtree_->Branch("lead_strip_dR_2", &lead_strip_dR_2_);
      outtree_->Branch("closest_strip_dR_2", &closest_strip_dR_2_);
    }



    if(ProductExists("MVAreader")){
        reader_ = GetProduct<TMVA::Reader*>("MVAreader");
        std::cout << "Getting MVAreader" << std::endl;
      } else { 
        reader_ = new TMVA::Reader();
        TString filename = (std::string)getenv("CMSSW_BASE")+"/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/MVA/TMVAClassification_BDT_rhoID.weights.xml";
      
        reader_->AddVariable( "Ngammas"    , &var11 );
        reader_->AddVariable( "Egamma1"    , &var0 );
        reader_->AddVariable( "Egamma2"    , &var1 );
        reader_->AddVariable( "Egamma3"    , &var2 );
        reader_->AddVariable( "Egamma4"    , &var3 );
        reader_->AddVariable( "Epi"        , &var4 );
        reader_->AddVariable( "Mpi0"       , &var5 );
        reader_->AddVariable( "Mrho"       , &var6 );
        reader_->AddVariable( "dphi"       , &var10 );
        reader_->AddVariable( "dEta"       , &var9 );
        reader_->AddVariable( "gammas_dEta", &var7 );
        reader_->AddVariable( "gammas_dphi", &var8 );
        reader_->AddVariable( "pt"         , &var13 );
        reader_->AddVariable( "eta"        , &var12 );
      
        reader_->BookMVA( "BDT method", filename );

        AddToProducts("MVAreader", reader_); 
        std::cout << "Adding MVAreader" << std::endl;
     }
    return 0;
  }

  int RhoIDEmbedder::Execute(TreeEvent *event) {

    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    event_ = eventInfo->event() % 2 == 0; // if even then event_ = 1, odd = 0


    wt_cp_sm_=1; wt_cp_ps_=1; wt_cp_mm_=1;
    if(event->ExistsInTree("tauspinner")){
      EventInfo const* tauspinner = event->GetPtr<EventInfo>("tauspinner");
      wt_cp_sm_ = tauspinner->weight("wt_cp_0");
      wt_cp_ps_ = tauspinner->weight("wt_cp_0p5");
      wt_cp_mm_ = tauspinner->weight("wt_cp_0p25");
    }


    if (!(channel_ == channel::tt||channel_ == channel::mt||channel_ == channel::et)) return 0;
    
    wt_ = 1;
    wt_ = eventInfo->total_weight();

    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>("ditau");
    CompositeCandidate const* ditau = ditau_vec.at(0);
    Candidate const* lep1 = ditau->GetCandidate("lepton1");
    Candidate const* lep2 = ditau->GetCandidate("lepton2");
    
    std::vector<ic::PFCandidate*> pfcands =  event->GetPtrVec<ic::PFCandidate>("pfCandidates");
    std::vector<ic::PFCandidate*> gammas1;
    std::vector<ic::PFCandidate*> gammas2;
    std::pair<ic::Candidate*, ic::Candidate*> rho_1 = std::make_pair(new Candidate(), new Candidate()); 
    std::pair<ic::Candidate*, ic::Candidate*> rho_2 = std::make_pair(new Candidate(), new Candidate());

    if (event->Exists("tauFlag1")) tauFlag1_ = event->Get<int>("tauFlag1");
    if (event->Exists("tauFlag2")) tauFlag2_ = event->Get<int>("tauFlag2");

    if(event->Exists("gen_match_1")) gen_match_1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
    if(event->Exists("gen_match_2")) gen_match_2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));

//-------------------------------------subleading tau--------------------

    if ((channel_ == channel::tt||channel_ == channel::mt||channel_ == channel::et) && event->ExistsInTree("pfCandidates")) {
      Tau const* tau2 = dynamic_cast<Tau const*>(lep2);
      tau_decay_mode_2_=tau2->decay_mode();
      //std::cout << "DM = " << tau_decay_mode_2_ << std::endl;

      std::vector<ic::PFCandidate*> gammas_before_2 = GetTauGammas(tau2, pfcands, gammas_pt_cut_);

      std::vector<ic::PFCandidate*> a1_daughters_2 = {};
      ic::Candidate *pi0_2 = new ic::Candidate();


      
      if(tau_decay_mode_2_>=10){
        std::pair<std::vector<ic::PFCandidate*>, ic::Candidate*>  a1 = GetA1(tau2, pfcands, gammas_pt_cut_);
        a1_daughters_2  = a1.first;
        pi0_2 = a1.second; 
        
      } else {
        rho_2 = GetRho(tau2, pfcands, gammas_pt_cut_);
        pi0_2 = rho_2.second;
      }


      gammas2 = GetTauGammas(tau2, pfcands, gammas_pt_cut_);

      if(gammas2.size()>0) lead_gamma_pt_2_ = gammas2[0]->pt();
      else lead_gamma_pt_2_ = -1;
      strip_pt_2_ = -1;
      strip_pt_2_ = pi0_2->pt();
 
      ic::PFCandidate* lead_strip = new ic::PFCandidate();
      ic::PFCandidate* closest_strip = new ic::PFCandidate(); 

      std::vector<ic::PFCandidate*> strips;

      if(tau_decay_mode_2_==0&& strip_pt_2_<=0) {
        strips = HPS (gammas_before_2, 0, 0, 0, 0.134, 1);
        if(strips.size()>0) {
          lead_strip = strips[0];
          //double cone_size = std::max(std::min(0.1, 3./tau2->pt()),0.05);
          //std::cout << "--------" << std::endl;
          //std::cout << "cone size = " << cone_size << std::endl;
          double min_dR = 0.4;
          for (auto s : strips) { 
            double dR = ROOT::Math::VectorUtil::DeltaR(s->vector(),tau2->vector());
            if(dR<min_dR) {
              min_dR = dR;
              closest_strip = s;
            } 
            //double mass_diff = (s->vector()+tau2->vector()).M();
            //std::cout << s->pt() << "    " << dR << "    " << mass_diff << std::endl;
          }
        }
      }

      lead_strip_pt_2_ = -1;
      closest_strip_pt_2_ = -1;
      lead_strip_dR_2_ = -1;
      closest_strip_dR_2_ = -1;
      if(strips.size()>0) {
        lead_strip_pt_2_ = lead_strip->pt();
        closest_strip_pt_2_ = closest_strip->pt();
        lead_strip_dR_2_ = ROOT::Math::VectorUtil::DeltaR(lead_strip->vector(),tau2->vector()); 
        closest_strip_dR_2_ = ROOT::Math::VectorUtil::DeltaR(closest_strip->vector(),tau2->vector());
      } 

      mass0_2_ = -1;
      mass1_2_ = -1;
      mass2_2_ = -1;
      E1_2_ =  -1;
      E2_2_ =  -1;
      E3_2_ =  -1;
      strip_E_2_ = -1;
      a1_pi0_dEta_2_ = -1;
      a1_pi0_dphi_2_ = -1;
      h1_h2_dphi_2_ = -1;
      h1_h3_dphi_2_ = -1;
      h2_h3_dphi_2_ = -1;
      h1_h2_dEta_2_ = -1;
      h1_h3_dEta_2_ = -1;
      h2_h3_dEta_2_ = -1;
      if(tau_decay_mode_2_>=10){
        mass0_2_ = (a1_daughters_2[0]->vector() + a1_daughters_2[1]->vector() + a1_daughters_2[2]->vector()).M();
        mass1_2_ = (a1_daughters_2[0]->vector() + a1_daughters_2[1]->vector()).M();
        mass2_2_ = (a1_daughters_2[0]->vector() + a1_daughters_2[2]->vector()).M();
        E1_2_ = a1_daughters_2[0]->energy();
        E2_2_ = a1_daughters_2[1]->energy();
        E3_2_ = a1_daughters_2[2]->energy();
        strip_E_2_ = pi0_2->energy();
        if(strip_pt_2_>0) {
          a1_pi0_dEta_2_ = std::fabs(pi0_2->eta()-tau2->eta());
          a1_pi0_dphi_2_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(pi0_2->vector(),tau2->vector()));
        }
        h1_h2_dphi_2_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(a1_daughters_2[0]->vector(),a1_daughters_2[1]->vector()));
        h1_h3_dphi_2_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(a1_daughters_2[0]->vector(),a1_daughters_2[2]->vector()));
        h2_h3_dphi_2_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(a1_daughters_2[1]->vector(),a1_daughters_2[2]->vector()));
        h1_h2_dEta_2_ = std::fabs(a1_daughters_2[0]->eta()-a1_daughters_2[1]->eta());
        h1_h3_dEta_2_ = std::fabs(a1_daughters_2[0]->eta()-a1_daughters_2[2]->eta());
        h2_h3_dEta_2_ = std::fabs(a1_daughters_2[1]->eta()-a1_daughters_2[2]->eta());
      }


      if(tau2->decay_mode()<2 || tau2->decay_mode()>9) {

        Candidate *pi_2 = rho_2.first;

        //------------------
        Egamma1_2_=-1, Egamma2_2_=-1, Egamma3_2_=-1;


        if(gammas2.size()>=1) Egamma1_2_ = gammas2[0]->energy();
        if(gammas2.size()>=2) Egamma2_2_ = gammas2[1]->energy();
        if(gammas2.size()>=3) Egamma3_2_ = gammas2[2]->energy();
        //--------Mpi0---
        Mpi0_TwoHighGammas_2_=-1; Mpi0_ThreeHighGammas_2_=-1; Mpi0_FourHighGammas_2_=-1;
        if(gammas2.size()>=2) Mpi0_TwoHighGammas_2_ = (gammas2[0]->vector() + gammas2[1]->vector()).M();
        if(gammas2.size()>=3) Mpi0_ThreeHighGammas_2_ = (gammas2[0]->vector() + gammas2[1]->vector() + gammas2[2]->vector()).M();
        if(gammas2.size()>=4) Mpi0_FourHighGammas_2_ = (gammas2[0]->vector() + gammas2[1]->vector() + gammas2[2]->vector() + gammas2[3]->vector()).M();
        //--------Mrho------
        Mrho_OneHighGammas_2_=-1; Mrho_TwoHighGammas_2_=-1; Mrho_ThreeHighGammas_2_=-1; Mrho_subleadingGamma_2_=-1;
        if(gammas2.size()>=1) Mrho_OneHighGammas_2_=( pi_2->vector() + gammas2[0]->vector() ).M();
        if(gammas2.size()>=2) Mrho_TwoHighGammas_2_=( pi_2->vector() + gammas2[0]->vector() + gammas2[1]->vector()  ).M();
        if(gammas2.size()>=3) Mrho_ThreeHighGammas_2_=( pi_2->vector() + gammas2[0]->vector() + gammas2[1]->vector() + gammas2[2]->vector() ).M();
        if(gammas2.size()>=2) Mrho_subleadingGamma_2_= (pi_2->vector() + gammas2[1]->vector()).M();
        //------------
        
        E_2_=-1, Epi_2_=-1, Mpi0_2_=-1, Mrho_2_=-1, rho_dEta_2_=-1, rho_dphi_2_=-1, gammas_dphi_2_ = -1., gammas_dEta_2_ = -1.,  pt_2_=-1, eta_2_=-999;
        Ngammas_2_=-999; phi_2_=-999;

        if(gammas2.size()>1) {
          gammas_dphi_2_ =  std::fabs(ROOT::Math::VectorUtil::DeltaPhi(gammas2[0]->vector(),gammas2[1]->vector()));
          gammas_dEta_2_ =  std::fabs(gammas2[0]->eta()-gammas2[1]->eta());
        }

        E_2_ = tau2->energy();

        Epi_2_ = pi_2->energy();
        Epi0_2_ = pi0_2->energy();
        ROOT::Math::PtEtaPhiEVector gammas_vector_2;
        for (auto g : gammas2) gammas_vector_2+=g->vector();
        Mpi0_2_ = gammas_vector_2.M();
        Mrho_2_ = tau2->M();
        Mrho_2_ = (pi_2->vector()+pi0_2->vector()).M();
        if(Epi0_2_>0) {
          rho_dphi_2_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(pi_2->vector(),pi0_2->vector()));
          rho_dEta_2_ = std::fabs(pi_2->eta()-pi0_2->eta());
        }

        pt_2_ = tau2->pt();
        eta_2_ = tau2->eta();
        phi_2_= tau2->phi();

        //-----NgammasModif---
        NgammasModif_2_=0;
        for (auto g : gammas2) 
          if (g->energy()>1.0){
            if (g->energy()>10.0)
              NgammasModif_2_+=2;
            else
              NgammasModif_2_+=1;
          }
        //------------------
        DeltaR2WRTtau_2_=-999; DeltaR2WRTpi0_2_=-999;
        if(gammas2.size()>=1){

          //--------------------------Shape based variables---------------
          //WRT tau
          DeltaR2WRTtau_2_=0;
          SumPt_2=0;
          SumPt_2=std::pow(pi_2->pt(),2);
          for(auto g : gammas2){
            DeltaR2WRTtau_2_+=std::pow(ROOT::Math::VectorUtil::DeltaR(g->vector(),tau2->vector()),2)*std::pow(g->pt(),2);
            SumPt_2+=std::pow(g->pt(),2);
          }
          DeltaR2WRTtau_2_/=SumPt_2;

          //WRT pi0 (ignoring prong)
          DeltaR2WRTpi0_2_=0;          
          SumPt_2=0;
          for(auto g : gammas2){
            DeltaR2WRTpi0_2_+=std::pow(ROOT::Math::VectorUtil::DeltaR(g->vector(),pi0_2->vector()),2)*std::pow(g->pt(),2);
            SumPt_2+=std::pow(g->pt(),2);
          }
          DeltaR2WRTpi0_2_/=SumPt_2;

        }


         //----------Gamma Pt-------------
        Ptgamma1_2_=-1; Ptgamma2_2_=-1; Ptgamma3_2_=-1; Ptgamma4_2_=-1;
        if(gammas2.size()>=1) Ptgamma1_2_ = gammas2[0]->pt();
        if(gammas2.size()>=2) Ptgamma2_2_ = gammas2[1]->pt();
        if(gammas2.size()>=3) Ptgamma3_2_ = gammas2[2]->pt();
        if(gammas2.size()>=4) Ptgamma4_2_ = gammas2[3]->pt();


        
    
        std::vector<double> inputs2 = {Egamma1_2_/E_2_, Egamma2_2_/E_2_, Egamma3_2_/E_2_, Egamma4_2_/E_2_, Epi_2_/E_2_, Mpi0_2_, Mrho_2_, gammas_dEta_2_, gammas_dphi_2_, rho_dEta_2_, rho_dphi_2_,(double)gammas2.size(), eta_2_, pt_2_};

        if(tau_decay_mode_2_>=10) {
          DeltaR2WRTtau_2_=0;
          SumPt_2=0;

          for(auto p : a1_daughters_2) {
            DeltaR2WRTtau_2_+=std::pow(ROOT::Math::VectorUtil::DeltaR(p->vector(),tau2->vector()),2)*std::pow(p->pt(),2);
            SumPt_2+=std::pow(p->pt(),2);
          }
          for(auto g : gammas2){
            DeltaR2WRTtau_2_+=std::pow(ROOT::Math::VectorUtil::DeltaR(g->vector(),tau2->vector()),2)*std::pow(g->pt(),2);
            SumPt_2+=std::pow(g->pt(),2);
          }
          DeltaR2WRTtau_2_/=SumPt_2;
        }


        // variables for subleading tau

        Ngammas_2_     = gammas2.size();
        Egamma1_2_     = Egamma1_2_;///Etau_2_;
        Egamma2_2_     = Egamma2_2_;///Etau_2_;
        Egamma3_2_     = Egamma3_2_;///Etau_2_;
        Egamma4_2_     = Egamma4_2_;///Etau_2_;
        Epi_2_         = Epi_2_;///Etau_2_;
        Mpi0_2_        = Mpi0_2_;
        Mrho_2_        = Mrho_2_;
        dphi_2_        = rho_dphi_2_;       
        dEta_2_        = rho_dEta_2_;       
        gammas_dEta_2_  = gammas_dEta_2_;
        gammas_dphi_2_ = gammas_dphi_2_;
        pt_2_          = pt_2_;
        eta_2_         = eta_2_;

        double score2 = read_mva_score(inputs2);

        event->Add("rho_id_2", score2);

      
      }
    }

//-------------------------------------leading tau--------------------
    if (channel_ == channel::tt && event->ExistsInTree("pfCandidates")) {
      Tau const* tau1 = dynamic_cast<Tau const*>(lep1);
      tau_decay_mode_1_=tau1->decay_mode();

      std::vector<ic::PFCandidate*> a1_daughters_1 = {};
      ic::Candidate *pi0_1 = new ic::Candidate();

      if(tau_decay_mode_1_>=10){
        std::pair<std::vector<ic::PFCandidate*>, ic::Candidate*>  a1 = GetA1(tau1, pfcands, gammas_pt_cut_);
        a1_daughters_1  = a1.first;
        pi0_1 = a1.second;
      } else {
        rho_1 = GetRho(tau1, pfcands, gammas_pt_cut_);
        pi0_1 = rho_1.second;
      }


      gammas1 = GetTauGammas(tau1, pfcands, gammas_pt_cut_);
      if(gammas1.size()>0) lead_gamma_pt_1_ = gammas1[0]->pt();
      else lead_gamma_pt_1_ = -1;
      strip_pt_1_ = -1;
      strip_pt_1_ = pi0_1->pt();


      mass0_1_ = -1;
      mass1_1_ = -1;
      mass2_1_ = -1;
      E1_1_ =  -1;
      E2_1_ =  -1;
      E3_1_ =  -1;
      strip_E_1_ = -1;
      a1_pi0_dEta_1_ = -1;
      a1_pi0_dphi_1_ = -1;
      h1_h2_dphi_1_ = -1;
      h1_h3_dphi_1_ = -1;
      h2_h3_dphi_1_ = -1;
      h1_h2_dEta_1_ = -1;
      h1_h3_dEta_1_ = -1;
      h2_h3_dEta_1_ = -1;
      if(tau_decay_mode_1_>=10){
        mass0_1_ = (a1_daughters_1[0]->vector() + a1_daughters_1[1]->vector() + a1_daughters_1[2]->vector()).M();
        mass1_1_ = (a1_daughters_1[0]->vector() + a1_daughters_1[1]->vector()).M();
        mass2_1_ = (a1_daughters_1[0]->vector() + a1_daughters_1[2]->vector()).M();
        E1_1_ = a1_daughters_1[0]->energy();
        E2_1_ = a1_daughters_1[1]->energy();
        E3_1_ = a1_daughters_1[2]->energy();
        strip_E_1_ = pi0_1->energy();
        if(strip_pt_1_>0) {
          a1_pi0_dEta_1_ = std::fabs(pi0_1->eta()-tau1->eta());
          a1_pi0_dphi_1_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(pi0_1->vector(),tau1->vector()));
        }
        h1_h2_dphi_1_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(a1_daughters_1[0]->vector(),a1_daughters_1[1]->vector()));
        h1_h3_dphi_1_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(a1_daughters_1[0]->vector(),a1_daughters_1[2]->vector()));
        h2_h3_dphi_1_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(a1_daughters_1[1]->vector(),a1_daughters_1[2]->vector()));
        h1_h2_dEta_1_ = std::fabs(a1_daughters_1[0]->eta()-a1_daughters_1[1]->eta());
        h1_h3_dEta_1_ = std::fabs(a1_daughters_1[0]->eta()-a1_daughters_1[2]->eta());
        h2_h3_dEta_1_ = std::fabs(a1_daughters_1[1]->eta()-a1_daughters_1[2]->eta()); 
      }

      
      if(tau1->decay_mode()<2 || tau1->decay_mode() > 9) {

        Candidate *pi_1 = rho_1.first;
       
        //------------------
        Egamma1_1_=-1, Egamma2_1_=-1, Egamma3_1_=-1;

        if(gammas1.size()>=1) Egamma1_1_ = gammas1[0]->energy();
        if(gammas1.size()>=2) Egamma2_1_ = gammas1[1]->energy();
        if(gammas1.size()>=3) Egamma3_1_ = gammas1[2]->energy();

        //--------------------
        Mpi0_TwoHighGammas_1_=-1; Mpi0_ThreeHighGammas_1_=-1; Mpi0_FourHighGammas_1_=-1;
        if(gammas1.size()>=2) Mpi0_TwoHighGammas_1_= (gammas1[0]->vector() + gammas1[1]->vector()).M();
        if(gammas1.size()>=3) Mpi0_ThreeHighGammas_1_ = (gammas1[0]->vector() + gammas1[1]->vector() + gammas1[2]->vector()).M();
        if(gammas1.size()>=4) Mpi0_FourHighGammas_1_ = (gammas1[0]->vector() + gammas1[1]->vector() + gammas1[2]->vector() + gammas1[3]->vector()).M();
        //-----------
        Mrho_OneHighGammas_1_=-1; Mrho_TwoHighGammas_1_=-1; Mrho_ThreeHighGammas_1_=-1; Mrho_subleadingGamma_1_=-1;
        if(gammas1.size()>=1) Mrho_OneHighGammas_1_=( pi_1->vector() + gammas1[0]->vector() ).M();
        if(gammas1.size()>=2) Mrho_TwoHighGammas_1_=( pi_1->vector() + gammas1[0]->vector() + gammas1[1]->vector()  ).M();
        if(gammas1.size()>=3) Mrho_ThreeHighGammas_1_=( pi_1->vector() + gammas1[0]->vector() + gammas1[1]->vector() + gammas1[2]->vector() ).M();
        if(gammas1.size()>=2) Mrho_subleadingGamma_1_= (pi_1->vector() + gammas1[1]->vector()).M();
        //------------
        E_1_=-1,  Epi_1_=-1, Mpi0_1_=-1, Mrho_1_=-1, rho_dEta_1_=-1, rho_dphi_1_=-1, pt_1_=-1, eta_1_=-999, gammas_dphi_1_ = -1., gammas_dEta_1_ = -1.; 
        Ngammas_1_=-999; phi_1_=-999;

        if(gammas1.size()>1) { 
          gammas_dphi_1_ =  std::fabs(ROOT::Math::VectorUtil::DeltaPhi(gammas1[0]->vector(),gammas1[1]->vector()));
          gammas_dEta_1_ =  std::fabs(gammas1[0]->eta()-gammas1[1]->eta());
        }

        
        E_1_ = tau1->energy();

        Epi_1_ = pi_1->energy();
        Epi0_1_ = pi0_1->energy();
        ROOT::Math::PtEtaPhiEVector gammas_vector_1;
        for (auto g : gammas1) gammas_vector_1+=g->vector();
        Mpi0_1_ = gammas_vector_1.M();
        Mrho_1_ = (pi_1->vector()+pi0_1->vector()).M();
        if(Epi0_1_>0) {
          rho_dphi_1_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(pi_1->vector(),pi0_1->vector()));
          rho_dEta_1_ = std::fabs(pi_1->eta()-pi0_1->eta());
        }

        pt_1_ = tau1->pt();
        eta_1_ = tau1->eta();
        phi_1_ = tau1->phi();

        DeltaR2WRTtau_1_=-999; DeltaR2WRTpi0_1_=-999;
        if(gammas1.size()>=1){

          //--------------------------Shape based variables---------------
          //WRT tau
          DeltaR2WRTtau_1_=0;
          SumPt_1=0;
          DeltaR2WRTtau_1_=std::pow(ROOT::Math::VectorUtil::DeltaR(pi_1->vector(),tau1->vector()),2)*std::pow(pi_1->pt(),2);
          SumPt_1=std::pow(pi_1->pt(),2);
          for(auto g : gammas1){
            DeltaR2WRTtau_1_+=std::pow(ROOT::Math::VectorUtil::DeltaR(g->vector(),tau1->vector()),2)*std::pow(g->pt(),2);
            SumPt_1+=std::pow(g->pt(),2);
          }
          DeltaR2WRTtau_1_/=SumPt_1;
          

          //WRT pi0 (ignoring prong)
          DeltaR2WRTpi0_1_=0;          
          SumPt_1=0;
          for(auto g : gammas1){
            DeltaR2WRTpi0_1_+=std::pow(ROOT::Math::VectorUtil::DeltaR(g->vector(),pi0_1->vector()),2)*std::pow(g->pt(),2);
            SumPt_1+=std::pow(g->pt(),2);
          }
          DeltaR2WRTpi0_1_/=SumPt_1;

        }

        if(tau_decay_mode_1_>=10) {
          DeltaR2WRTtau_1_=0;
          SumPt_1=0;

          for(auto p : a1_daughters_1) {
            DeltaR2WRTtau_1_+=std::pow(ROOT::Math::VectorUtil::DeltaR(p->vector(),tau1->vector()),2)*std::pow(p->pt(),2);
            SumPt_1+=std::pow(p->pt(),2);
          }
          for(auto g : gammas1){
            DeltaR2WRTtau_1_+=std::pow(ROOT::Math::VectorUtil::DeltaR(g->vector(),tau1->vector()),2)*std::pow(g->pt(),2);
            SumPt_1+=std::pow(g->pt(),2);
          }
          DeltaR2WRTtau_1_/=SumPt_1;
        }



        std::vector<double> inputs1 = {Egamma1_1_/E_1_, Egamma2_1_/E_1_, Egamma3_1_/E_1_, Egamma4_1_/E_1_, Epi_1_/E_1_, Mpi0_1_, Mrho_1_, gammas_dEta_1_, gammas_dphi_1_, rho_dEta_1_, rho_dphi_1_,(double)gammas1.size(), eta_1_, pt_1_};


        // variables for leading tau
      

        Ngammas_1_     = gammas1.size();       
        Egamma1_1_     = Egamma1_1_;///Etau_1_;  
        Egamma2_1_     = Egamma2_1_;///Etau_1_;  
        Egamma3_1_     = Egamma3_1_;///Etau_1_;   
        Egamma4_1_     = Egamma4_1_;///Etau_1_;   
        Epi_1_         = Epi_1_;///Etau_1_;
        Mpi0_1_        = Mpi0_1_;   
        Mrho_1_        = Mrho_1_; 
        dphi_1_        = rho_dphi_1_; 
        dEta_1_        = rho_dEta_1_; 
        gammas_dEta_1_  = gammas_dEta_1_;  
        gammas_dphi_1_ = gammas_dphi_1_;
        pt_1_          = pt_1_; 
        eta_1_         = eta_1_;

        double score1 = read_mva_score(inputs1);
        event->Add("rho_id_1", score1);
      } 
    }
    if(fs_&&maketrees_) outtree_->Fill();

    return 0;
  }
  int RhoIDEmbedder::PostAnalysis() {
    return 0;
  }

  void RhoIDEmbedder::PrintInfo() {
    ;
  }

}
