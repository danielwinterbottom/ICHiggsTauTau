#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTEventClassifier.h"


namespace ic {

  HTTEventClassifier::HTTEventClassifier(std::string const& name) : ModuleBase(name), 
    channel_(channel::tt), 
    era_(era::data_2012_rereco){
    fs_          = NULL;
    met_label_   = "pfMET";
    jets_label_  = "ak4PFJetsCHS";
    ditau_label_ = "ditau";
  }

  HTTEventClassifier::~HTTEventClassifier() {
    ;
  }

  std::vector<float> HTTEventClassifier::read_mva_scores(unsigned isEven, std::vector<float> vars) {
      std::vector<float> scores = {};

      var0_=vars[0], var1_=vars[1], var2_=vars[2], var3_=vars[3], var4_=vars[4], var5_=vars[5], var6_=vars[6], var7_=vars[7], var8_=vars[8], var9_=vars[9];

      if(isEven) scores = reader_even_->EvaluateMulticlass("Multi"); 
      else       scores = reader_odd_->EvaluateMulticlass("Multi");

      return scores;
  }

  // Add this function to take the maximum score and corresponding index
  // from the vector of scores
  std::pair<float,int> HTTEventClassifier::getMaxScoreWithIndex(std::vector<float> vec) {
    if (vec.empty()) return std::make_pair(0., 0);
    float max_score = vec[0];
    int max_index = 0;
    unsigned ind = 0;
    for (auto s : vec) {
      if (s > max_score) {
        max_score = s;
        max_index = ind;
      }
      ++ind;
    }
    std::pair<float, int> out_pair = std::make_pair(max_score, max_index);
    return out_pair;
  }


  int HTTEventClassifier::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTEventClassifier" << std::endl;
    std::cout << boost::format(param_fmt()) % "channel"             % Channel2String(channel_);
    std::cout << boost::format(param_fmt()) % "era"                 % Era2String(era_);
    std::cout << "-------------------------------------" << std::endl;

    reader_even_ = new TMVA::Reader();
    reader_odd_ = new TMVA::Reader();

    // fold0 is trained on even, so apply on odd, and vice versa

    TString filename_even = "";
    TString filename_odd  = "";
    if (era_ == era::data_2017) {
      filename_even = (std::string)getenv("CMSSW_BASE") +
          "/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/input/MVA/multi_fold1_sm_tt_tauspinner_2017_xgb.xml"; // apply to even here
      filename_odd  = (std::string)getenv("CMSSW_BASE") +
          "/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/input/MVA/multi_fold0_sm_tt_tauspinner_2017_xgb.xml"; // apply to odd
    } 
    else if (era_ == era::data_2018) {
      filename_even = (std::string)getenv("CMSSW_BASE") +
          "/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/input/MVA/multi_fold1_sm_tt_tauspinner_2018_xgb.xml"; // apply to even here
      filename_odd  = (std::string)getenv("CMSSW_BASE") +
          "/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/input/MVA/multi_fold0_sm_tt_tauspinner_2018_xgb.xml"; // apply to odd
    }

    reader_even_->AddVariable( "jdeta",      & var0_ );
    reader_even_->AddVariable( "jpt_1",      & var1_ );
    reader_even_->AddVariable( "m_vis",      & var2_ );
    reader_even_->AddVariable( "met",        & var3_ );
    reader_even_->AddVariable( "mjj",        & var4_ );
    reader_even_->AddVariable( "n_jets",     & var5_ );
    reader_even_->AddVariable( "pt_1",       & var6_ );
    reader_even_->AddVariable( "pt_tt",      & var7_);
    reader_even_->AddVariable( "pt_vis",     & var8_);
    reader_even_->AddVariable( "svfit_mass", & var9_ );

    reader_odd_->AddVariable( "jdeta",      & var0_ );
    reader_odd_->AddVariable( "jpt_1",      & var1_ );
    reader_odd_->AddVariable( "m_vis",      & var2_ );
    reader_odd_->AddVariable( "met",        & var3_ );
    reader_odd_->AddVariable( "mjj",        & var4_ );
    reader_odd_->AddVariable( "n_jets",     & var5_ );
    reader_odd_->AddVariable( "pt_1",       & var6_ );
    reader_odd_->AddVariable( "pt_tt",      & var7_);
    reader_odd_->AddVariable( "pt_vis",     & var8_);
    reader_odd_->AddVariable( "svfit_mass", & var9_ );

    reader_even_->BookMVA( "Multi", filename_even );
    reader_odd_->BookMVA( "Multi", filename_odd );

    return 0;
  }

  int HTTEventClassifier::Execute(TreeEvent *event) {

    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    isEven_ = eventInfo->event() % 2 == 0; // if even then event_ = 1, odd = 0
    evt_ = eventInfo->event();
    event_ = (float)isEven_;

    // Initialise variables
    jdeta_ = -9999.;
    jpt_1_ = -9999.;
    m_vis_ = -9999.;
    met_ = -9999.;
    mjj_ = -9999.;
    n_jets_ = 0;
    pt_1_ = -9999.;
    pt_tt_ = -9999.;
    pt_vis_ = -9999.;
    svfit_mass_ = -9999.;

    std::vector<CompositeCandidate *> const& ditau_vec = 
        event->GetPtrVec<CompositeCandidate>(ditau_label_);
    CompositeCandidate const* ditau = ditau_vec.at(0);
    Candidate * lep1 = ditau->GetCandidate("lepton1");
    
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));

    Met const* mets = NULL;
    mets = event->GetPtr<Met>(met_label_);

    n_jets_ = jets.size();

    pt_1_ = lep1->pt();
    if (event->Exists("svfitMass")) {
      svfit_mass_ = event->Get<double>("svfitMass");
    } else {
      svfit_mass_ = -1;
    }
    m_vis_ = ditau->M();
    pt_tt_ = (ditau->vector() + mets->vector()).pt();
    pt_vis_ = ditau->pt();
    met_ = mets->vector().pt();

    if (n_jets_ >= 1) jpt_1_ = jets[0]->pt();
    if (n_jets_ >= 2) {
      jdeta_ = fabs(jets[0]->eta() - jets[1]->eta());
      mjj_ = (jets[0]->vector() + jets[1]->vector()).M();
    }

    std::vector<float> inputs = {};
    if (channel_ == channel::tt) {
      inputs.resize(12);
      inputs[0]  = float(jdeta_);
      inputs[1]  = float(jpt_1_);
      inputs[2]  = float(m_vis_);
      inputs[3]  = float(met_);
      inputs[4]  = float(mjj_);
      inputs[5]  = unsigned(n_jets_);
      inputs[6]  = float(pt_1_);
      inputs[7] = float(pt_tt_);
      inputs[8] = float(pt_vis_);
      inputs[9] = float(svfit_mass_);
    }

    std::vector<float> scores = read_mva_scores(isEven_,inputs);
    event->Add("higgs_score",    scores[0]);
    event->Add("jetFakes_score", scores[1]);
    event->Add("zttEmbed_score", scores[2]);

    std::pair<float, int> max_pair = getMaxScoreWithIndex(scores);
    event->Add("IC_BDT_max_score", max_pair.first); 
    event->Add("IC_BDT_max_index", max_pair.second);

    return 0;
  }

  int HTTEventClassifier::PostAnalysis() {
    return 0;
  }

  void HTTEventClassifier::PrintInfo() {
    ;
  }

}
