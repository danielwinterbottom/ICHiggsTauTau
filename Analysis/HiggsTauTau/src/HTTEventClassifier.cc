#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTEventClassifier.h"
#include "boost/format.hpp"


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

      var0_=vars[0], var1_=vars[1], var2_=vars[2], var3_=vars[3], var4_=vars[4], var5_=vars[5], var6_=vars[6], var7_=vars[7], var8_=vars[8], var9_=vars[9], var10_=vars[10], var11_=vars[11];

      if(isEven) scores = reader_even_->EvaluateMulticlass("BDT method"); 
      else       scores = reader_odd_->EvaluateMulticlass("BDT method");

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
          "/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/MVA/multi_fold1_sm_tt_tauspinner_2017_xgb.xml"; // apply to even here
      filename_odd  = (std::string)getenv("CMSSW_BASE") +
          "/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/MVA/multi_fold0_sm_tt_tauspinner_2017_xgb.xml"; // apply to odd
    } 
    else if (era_ == era::data_2018) {
      filename_even = (std::string)getenv("CMSSW_BASE") +
          "/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/MVA/multi_fold1_sm_tt_tauspinner_2018_xgb.xml"; // apply to even here
      filename_odd  = (std::string)getenv("CMSSW_BASE") +
          "/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/MVA/multi_fold0_sm_tt_tauspinner_2018_xgb.xml"; // apply to odd
    }

    reader_even_->AddVariable( "dijetpt", & var0_ );
    reader_even_->AddVariable( "jdeta",   & var1_ );
    reader_even_->AddVariable( "jpt_1",   & var2_ );
    reader_even_->AddVariable( "m_sv",    & var3_ );
    reader_even_->AddVariable( "m_vis",   & var4_ );
    reader_even_->AddVariable( "met",     & var5_ );
    reader_even_->AddVariable( "mjj",     & var6_ );
    reader_even_->AddVariable( "n_jets",  & var7_ );
    reader_even_->AddVariable( "pt_1",    & var8_ );
    reader_even_->AddVariable( "pt_2",    & var9_);
    reader_even_->AddVariable( "pt_tt",   & var10_);
    reader_even_->AddVariable( "pt_vis",  & var11_);
    
    reader_odd_->AddVariable( "dijetpt",  & var0_ );
    reader_odd_->AddVariable( "jdeta",    & var1_ );
    reader_odd_->AddVariable( "jpt_1",    & var2_ );
    reader_odd_->AddVariable( "m_sv",     & var3_ );
    reader_odd_->AddVariable( "m_vis",    & var4_ );
    reader_odd_->AddVariable( "met",      & var5_ );
    reader_odd_->AddVariable( "mjj",      & var6_ );
    reader_odd_->AddVariable( "n_jets",   & var7_ );
    reader_odd_->AddVariable( "pt_1",     & var8_ );
    reader_odd_->AddVariable( "pt_2",     & var9_);
    reader_odd_->AddVariable( "pt_tt",    & var10_);
    reader_odd_->AddVariable( "pt_vis",   & var11_);

    reader_even_->BookMVA( "BDT method", filename_even );
    reader_odd_->BookMVA( "BDT method", filename_odd );

    return 0;
  }

  int HTTEventClassifier::Execute(TreeEvent *event) {
    std::cout<<"test1"<<std::endl;
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    isEven_ = eventInfo->event() % 2 == 0; // if even then event_ = 1, odd = 0
    evt_ = eventInfo->event();
    event_ = (float)isEven_;

    std::cout<<"test2"<<std::endl;
    std::vector<CompositeCandidate *> const& ditau_vec = 
        event->GetPtrVec<CompositeCandidate>(ditau_label_);
    CompositeCandidate const* ditau = ditau_vec.at(0);
    Candidate * lep1 = ditau->GetCandidate("lepton1");
    Candidate * lep2 = ditau->GetCandidate("lepton2");
    
    std::cout<<"test3"<<std::endl;
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
    std::vector<PFJet*> lowpt_jets = jets;
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
    ic::erase_if(lowpt_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 4.7));

    std::cout<<"test4"<<std::endl;
    Met const* mets = NULL;
    mets = event->GetPtr<Met>(met_label_);

    n_jets_ = jets.size();
    n_lowpt_jets_ = lowpt_jets.size();

    pt_1_ = lep1->pt();
    pt_2_ = lep2->pt();
    if (event->Exists("svfitMass")) {
      m_sv_ = float(event->Get<double>("svfitMass"));
    } else {
      m_sv_ = -9999.;
    }
    m_vis_ = ditau->M();
    pt_tt_ = (ditau->vector() + mets->vector()).pt();
    pt_vis_ = ditau->pt();
    met_ = mets->vector().pt();

    if (n_lowpt_jets_ >= 1) jpt_1_ = lowpt_jets[0]->pt();
    if (n_lowpt_jets_ >= 2) {
      dijetpt_ =  (lowpt_jets[0]->vector() + lowpt_jets[1]->vector()).pt();
      jdeta_ = fabs(lowpt_jets[0]->eta() - lowpt_jets[1]->eta());
      mjj_ = (lowpt_jets[0]->vector() + lowpt_jets[1]->vector()).M();
    }

    std::cout<<"test5"<<std::endl;
    std::vector<float> inputs = {};
    if (channel_ == channel::tt) {
      inputs.resize(12);
      inputs[0]  = dijetpt_;
      inputs[1]  = jdeta_;
      inputs[2]  = jpt_1_;
      inputs[3]  = m_sv_;
      inputs[4]  = m_vis_;
      inputs[5]  = met_;
      inputs[6]  = mjj_;
      inputs[7]  = n_jets_;
      inputs[8]  = pt_1_;
      inputs[9]  = pt_2_;
      inputs[10] = pt_tt_;
      inputs[11] = pt_vis_;
      for (int i=0; i<12; i++) std::cout<<i<<"= "<<inputs[i]<<std::endl;
    }

    std::cout<<"test6"<<std::endl;
    std::vector<float> scores = read_mva_scores(isEven_,inputs);
    event->Add("higgs_score",    scores[0]);
    event->Add("jetFakes_score", scores[1]);
    event->Add("zttEmbed_score", scores[2]);

    std::cout<<"test7"<<std::endl;
    std::pair<float, int> max_pair = getMaxScoreWithIndex(scores);
    event->Add("IC_BDT_max_score", max_pair.first); 
    event->Add("IC_BDT_max_index", max_pair.second);


    std::cout<<"test8"<<std::endl;
    return 0;
  }

  int HTTEventClassifier::PostAnalysis() {
    std::cout<<"test9"<<std::endl;
    return 0;
  }

  void HTTEventClassifier::PrintInfo() {
    std::cout<<"test10"<<std::endl;
    ;
  }

}
