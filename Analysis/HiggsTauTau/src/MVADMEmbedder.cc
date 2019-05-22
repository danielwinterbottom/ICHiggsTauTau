#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/MVADMEmbedder.h"

 

namespace ic {

  MVADMEmbedder::MVADMEmbedder(std::string const& name) : ModuleBase(name), channel_(channel::tt) {
    fs_ = NULL;
  }

  MVADMEmbedder::~MVADMEmbedder() {
    ;
  }

  std::vector<float> MVADMEmbedder::read_mva_scores(unsigned isEven) {
      std::vector<float> scores;
      if(isEven) scores = reader_even_->EvaluateMulticlass("BDT method"); 
      else       scores = reader_odd_->EvaluateMulticlass("BDT method");
      
      return scores;
  }


  int MVADMEmbedder::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "MVADMEmbedder" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    if(ProductExists("MVADMreaderOdd") && ProductExists("MVADMreaderEven")){
        reader_even_ = GetProduct<TMVA::Reader*>("MVADMreaderEven");
        std::cout << "Getting MVADMreaderEven" << std::endl;
        reader_odd_ = GetProduct<TMVA::Reader*>("MVADMreaderOdd");
        std::cout << "Getting MVADMreaderOdd" << std::endl;
      } else { 
        reader_even_ = new TMVA::Reader();
        reader_odd_ = new TMVA::Reader();

        TString filename_even = (std::string)getenv("CMSSW_BASE")+"/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/MVA/mvadm_inclusive_0p5gammas_2fold_applytoeven.xml";
        TString filename_odd = (std::string)getenv("CMSSW_BASE")+"/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/MVA/mvadm_inclusive_0p5gammas_2fold_applytoodd.xml";
        reader_even_->AddVariable( "event", &event_);
        reader_even_->AddVariable( "Egamma1_tau", &Egamma1_tau_ );
        reader_even_->AddVariable( "Egamma2_tau", &Egamma2_tau_ );
        reader_even_->AddVariable( "Egamma3_tau", &Egamma3_tau_ );
        reader_even_->AddVariable( "Epi_tau", &Epi_tau_ );
        reader_even_->AddVariable( "rho_dEta_tau", &rho_dEta_tau_ );
        reader_even_->AddVariable( "rho_dphi_tau", &rho_dphi_tau_ );
        reader_even_->AddVariable( "gammas_dEta_tau", &gammas_dEta_tau_ );
        reader_even_->AddVariable( "gammas_dR_tau", &gammas_dR_tau_ );
        reader_even_->AddVariable( "DeltaR2WRTtau_tau", &DeltaR2WRTtau_tau_ );
        reader_even_->AddVariable( "eta", &eta_ );
        reader_even_->AddVariable( "pt", &pt_ );
        reader_even_->AddVariable( "Epi0", &Epi0_ );
        reader_even_->AddVariable( "Epi", &Epi_ );
        reader_even_->AddVariable( "rho_dEta", &rho_dEta_ );
        reader_even_->AddVariable( "rho_dphi", &rho_dphi_ );
        reader_even_->AddVariable( "gammas_dEta", &gammas_dEta_ );
        reader_even_->AddVariable( "tau_decay_mode", &tau_decay_mode_ );
        reader_even_->AddVariable( "Mrho", &Mrho_ );
        reader_even_->AddVariable( "Mpi0", &Mpi0_ );
        reader_even_->AddVariable( "DeltaR2WRTtau", &DeltaR2WRTtau_ );
        reader_even_->AddVariable( "Mpi0_TwoHighGammas", &Mpi0_TwoHighGammas_ );
        reader_even_->AddVariable( "Mpi0_ThreeHighGammas", &Mpi0_ThreeHighGammas_ );
        reader_even_->AddVariable( "Mpi0_FourHighGammas", &Mpi0_FourHighGammas_ );
        reader_even_->AddVariable( "Mrho_OneHighGammas", &Mrho_OneHighGammas_ );
        reader_even_->AddVariable( "Mrho_TwoHighGammas", &Mrho_TwoHighGammas_ );
        reader_even_->AddVariable( "Mrho_ThreeHighGammas", &Mrho_ThreeHighGammas_ );
        reader_even_->AddVariable( "Mrho_subleadingGamma", &Mrho_subleadingGamma_ );

        reader_odd_->AddVariable( "event", &event_);
        reader_odd_->AddVariable( "Egamma1_tau", &Egamma1_tau_ );
        reader_odd_->AddVariable( "Egamma2_tau", &Egamma2_tau_ );
        reader_odd_->AddVariable( "Egamma3_tau", &Egamma3_tau_ );
        reader_odd_->AddVariable( "Epi_tau", &Epi_tau_ );
        reader_odd_->AddVariable( "rho_dEta_tau", &rho_dEta_tau_ );
        reader_odd_->AddVariable( "rho_dphi_tau", &rho_dphi_tau_ );
        reader_odd_->AddVariable( "gammas_dEta_tau", &gammas_dEta_tau_ );
        reader_odd_->AddVariable( "gammas_dR_tau", &gammas_dR_tau_ );
        reader_odd_->AddVariable( "DeltaR2WRTtau_tau", &DeltaR2WRTtau_tau_ );
        reader_odd_->AddVariable( "eta", &eta_ );
        reader_odd_->AddVariable( "pt", &pt_ );
        reader_odd_->AddVariable( "Epi0", &Epi0_ );
        reader_odd_->AddVariable( "Epi", &Epi_ );
        reader_odd_->AddVariable( "rho_dEta", &rho_dEta_ );
        reader_odd_->AddVariable( "rho_dphi", &rho_dphi_ );
        reader_odd_->AddVariable( "gammas_dEta", &gammas_dEta_ );
        reader_odd_->AddVariable( "tau_decay_mode", &tau_decay_mode_ );
        reader_odd_->AddVariable( "Mrho", &Mrho_ );
        reader_odd_->AddVariable( "Mpi0", &Mpi0_ );
        reader_odd_->AddVariable( "DeltaR2WRTtau", &DeltaR2WRTtau_ );
        reader_odd_->AddVariable( "Mpi0_TwoHighGammas", &Mpi0_TwoHighGammas_ );
        reader_odd_->AddVariable( "Mpi0_ThreeHighGammas", &Mpi0_ThreeHighGammas_ );
        reader_odd_->AddVariable( "Mpi0_FourHighGammas", &Mpi0_FourHighGammas_ );
        reader_odd_->AddVariable( "Mrho_OneHighGammas", &Mrho_OneHighGammas_ );
        reader_odd_->AddVariable( "Mrho_TwoHighGammas", &Mrho_TwoHighGammas_ );
        reader_odd_->AddVariable( "Mrho_ThreeHighGammas", &Mrho_ThreeHighGammas_ );
        reader_odd_->AddVariable( "Mrho_subleadingGamma", &Mrho_subleadingGamma_ );
       
        reader_even_->BookMVA( "BDT method", filename_even );
        reader_odd_->BookMVA( "BDT method", filename_odd );

        AddToProducts("MVADMreaderOdd", reader_odd_); 
        std::cout << "Adding MVADMreaderOdd" << std::endl;
        AddToProducts("MVADMreaderEven", reader_even_);
        std::cout << "Adding MVADMreaderEven" << std::endl;
     }
    return 0;
  }

  int MVADMEmbedder::Execute(TreeEvent *event) {

    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    isEven_ = eventInfo->event() % 2 == 0; // if even then event_ = 1, odd = 0
    event_ = (float)isEven_;

    if (!(channel_ == channel::tt||channel_ == channel::mt||channel_ == channel::et)) return 0;
    

    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>("ditau");
    CompositeCandidate const* ditau = ditau_vec.at(0);
    Candidate const* lep1 = ditau->GetCandidate("lepton1");
    Candidate const* lep2 = ditau->GetCandidate("lepton2");
    
    std::vector<ic::PFCandidate*> pfcands =  event->GetPtrVec<ic::PFCandidate>("pfCandidates");
    std::vector<ic::PFCandidate*> gammas1;
    std::vector<ic::PFCandidate*> gammas2;
    std::pair<ic::Candidate*, ic::Candidate*> rho_1; 
    std::pair<ic::Candidate*, ic::Candidate*> rho_2;


//-------------------------------------subleading tau--------------------

    if ((channel_ == channel::tt||channel_ == channel::mt||channel_ == channel::et) && event->ExistsInTree("pfCandidates")) {
      Tau const* tau2 = dynamic_cast<Tau const*>(lep2);
      tau_decay_mode_=tau2->decay_mode();
      gammas2 = GetTauGammas(tau2, pfcands);

      if (tau_decay_mode_<2) {

        std::pair<ic::Candidate*, ic::Candidate*> rho_2 = GetRho(tau2, pfcands);
        Candidate *pi_2 = rho_2.first;
        Candidate *pi0_2 = rho_2.second;
        
        double Egamma1=-1, Egamma2=-1, Egamma3=-1;
        double E = tau2->energy();
        Epi_ = pi_2->energy();
        Epi0_ = pi0_2->energy();

        if(gammas2.size()>=1) Egamma1 = gammas2[0]->energy();
        if(gammas2.size()>=2) Egamma2 = gammas2[1]->energy();
        if(gammas2.size()>=3) Egamma3 = gammas2[2]->energy();
 
        Egamma1_tau_ = Egamma1/E;
        Egamma2_tau_ = Egamma2/E;
        Egamma3_tau_ = Egamma3/E;

        Epi_tau_ = Epi_/E;

        pt_ = tau2->pt();
        eta_ = tau2->eta();

        rho_dEta_=-1, rho_dphi_=-1, gammas_dEta_ = -1.;
        double gammas_dphi = -1.,

        rho_dphi_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(pi_2->vector(),pi0_2->vector()));
        rho_dEta_ = std::fabs(pi_2->eta()-pi0_2->eta());
        rho_dEta_tau_ = rho_dEta_*E;
        rho_dphi_tau_ = rho_dphi_*E;

        if(gammas2.size()>1) {
          gammas_dphi =  std::fabs(ROOT::Math::VectorUtil::DeltaPhi(gammas2[0]->vector(),gammas2[1]->vector()));
          gammas_dEta_ =  std::fabs(gammas2[0]->eta()-gammas2[1]->eta());
        }
        gammas_dEta_tau_ = gammas_dEta_ * E;
        gammas_dR_tau_ =  sqrt(gammas_dEta_*gammas_dEta_ + gammas_dphi*gammas_dphi)*E;
        //--------Mpi0---
        Mpi0_=-1, Mpi0_TwoHighGammas_=-1; Mpi0_ThreeHighGammas_=-1; Mpi0_FourHighGammas_=-1;
        ROOT::Math::PtEtaPhiEVector gammas_vector_2;
        for (auto g : gammas2) gammas_vector_2+=g->vector();
        Mpi0_ = gammas_vector_2.M();
        if(gammas2.size()>=2) Mpi0_TwoHighGammas_ = (gammas2[0]->vector() + gammas2[1]->vector()).M();
        if(gammas2.size()>=3) Mpi0_ThreeHighGammas_ = (gammas2[0]->vector() + gammas2[1]->vector() + gammas2[2]->vector()).M();
        if(gammas2.size()>=4) Mpi0_FourHighGammas_ = (gammas2[0]->vector() + gammas2[1]->vector() + gammas2[2]->vector() + gammas2[3]->vector()).M();
        //--------Mrho------
        Mrho_=-1, Mrho_OneHighGammas_=-1; Mrho_TwoHighGammas_=-1; Mrho_ThreeHighGammas_=-1; Mrho_subleadingGamma_=-1;
        Mrho_ = tau2->M();
        if(gammas2.size()>=1) Mrho_OneHighGammas_=( pi_2->vector() + gammas2[0]->vector() ).M();
        if(gammas2.size()>=2) Mrho_TwoHighGammas_=( pi_2->vector() + gammas2[0]->vector() + gammas2[1]->vector()  ).M();
        if(gammas2.size()>=3) Mrho_ThreeHighGammas_=( pi_2->vector() + gammas2[0]->vector() + gammas2[1]->vector() + gammas2[2]->vector() ).M();
        if(gammas2.size()>=2) Mrho_subleadingGamma_= (pi_2->vector() + gammas2[1]->vector()).M();
        //-----------


        DeltaR2WRTtau_=0;
        double SumPt=0;
        DeltaR2WRTtau_=std::pow(ROOT::Math::VectorUtil::DeltaR(pi_2->vector(),tau2->vector()),2)*std::pow(pi_2->pt(),2);
        SumPt=std::pow(pi_2->pt(),2);
        for(auto g : gammas2){
          DeltaR2WRTtau_+=std::pow(ROOT::Math::VectorUtil::DeltaR(g->vector(),tau2->vector()),2)*std::pow(g->pt(),2);
          SumPt+=std::pow(g->pt(),2);
        }
        DeltaR2WRTtau_/=SumPt;

        std::vector<float> scores = read_mva_scores(isEven_);

        event->Add("mvadm_other_2", scores[0]);
        event->Add("mvadm_rho_2", scores[1]);
        event->Add("mvadm_pi_2", scores[2]);
        event->Add("mvadm_a1_2", scores[3]); 
      }
    }

//-------------------------------------leading tau--------------------
    if (channel_ == channel::tt && event->ExistsInTree("pfCandidates")) {

      Tau const* tau1 = dynamic_cast<Tau const*>(lep1);
      tau_decay_mode_=tau1->decay_mode();
      gammas1 = GetTauGammas(tau1, pfcands);

      if (tau_decay_mode_<2) {

        std::pair<ic::Candidate*, ic::Candidate*> rho_1 = GetRho(tau1, pfcands);
        Candidate *pi_1 = rho_1.first;
        Candidate *pi0_1 = rho_1.second;
        
        double Egamma1=-1, Egamma2=-1, Egamma3=-1;
        double E = tau1->energy();
        Epi_ = pi_1->energy();
        Epi0_ = pi0_1->energy();

        if(gammas1.size()>=1) Egamma1 = gammas1[0]->energy();
        if(gammas1.size()>=2) Egamma2 = gammas1[1]->energy();
        if(gammas1.size()>=3) Egamma3 = gammas1[2]->energy();
 
        Egamma1_tau_ = Egamma1/E;
        Egamma2_tau_ = Egamma2/E;
        Egamma3_tau_ = Egamma3/E;

        Epi_tau_ = Epi_/E;

        pt_ = tau1->pt();
        eta_ = tau1->eta();

        rho_dEta_=-1, rho_dphi_=-1, gammas_dEta_ = -1.;
        double gammas_dphi = -1.,

        rho_dphi_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(pi_1->vector(),pi0_1->vector()));
        rho_dEta_ = std::fabs(pi_1->eta()-pi0_1->eta());
        rho_dEta_tau_ = rho_dEta_*E;
        rho_dphi_tau_ = rho_dphi_*E;

        if(gammas1.size()>1) {
          gammas_dphi =  std::fabs(ROOT::Math::VectorUtil::DeltaPhi(gammas1[0]->vector(),gammas1[1]->vector()));
          gammas_dEta_ =  std::fabs(gammas1[0]->eta()-gammas1[1]->eta());
        }
        gammas_dEta_tau_ = gammas_dEta_ * E;
        gammas_dR_tau_ =  sqrt(gammas_dEta_*gammas_dEta_ + gammas_dphi*gammas_dphi)*E;

        //--------Mpi0---
        Mpi0_=-1, Mpi0_TwoHighGammas_=-1; Mpi0_ThreeHighGammas_=-1; Mpi0_FourHighGammas_=-1;
        ROOT::Math::PtEtaPhiEVector gammas_vector_1;
        for (auto g : gammas1) gammas_vector_1+=g->vector();
        Mpi0_ = gammas_vector_1.M();
        if(gammas1.size()>=2) Mpi0_TwoHighGammas_ = (gammas1[0]->vector() + gammas1[1]->vector()).M();
        if(gammas1.size()>=3) Mpi0_ThreeHighGammas_ = (gammas1[0]->vector() + gammas1[1]->vector() + gammas1[2]->vector()).M();
        if(gammas1.size()>=4) Mpi0_FourHighGammas_ = (gammas1[0]->vector() + gammas1[1]->vector() + gammas1[2]->vector() + gammas1[3]->vector()).M();
        //--------Mrho------
        Mrho_=-1, Mrho_OneHighGammas_=-1; Mrho_TwoHighGammas_=-1; Mrho_ThreeHighGammas_=-1; Mrho_subleadingGamma_=-1;
        Mrho_ = tau1->M();
        if(gammas1.size()>=1) Mrho_OneHighGammas_=( pi_1->vector() + gammas1[0]->vector() ).M();
        if(gammas1.size()>=2) Mrho_TwoHighGammas_=( pi_1->vector() + gammas1[0]->vector() + gammas1[1]->vector()  ).M();
        if(gammas1.size()>=3) Mrho_ThreeHighGammas_=( pi_1->vector() + gammas1[0]->vector() + gammas1[1]->vector() + gammas1[2]->vector() ).M();
        if(gammas1.size()>=2) Mrho_subleadingGamma_= (pi_1->vector() + gammas1[1]->vector()).M();
        //------------


        DeltaR2WRTtau_=0;
        double SumPt=0;
        DeltaR2WRTtau_=std::pow(ROOT::Math::VectorUtil::DeltaR(pi_1->vector(),tau1->vector()),2)*std::pow(pi_1->pt(),2);
        SumPt=std::pow(pi_1->pt(),2);
        for(auto g : gammas1){
          DeltaR2WRTtau_+=std::pow(ROOT::Math::VectorUtil::DeltaR(g->vector(),tau1->vector()),2)*std::pow(g->pt(),2);
          SumPt+=std::pow(g->pt(),2);
        }
        DeltaR2WRTtau_/=SumPt;

        std::vector<float> scores = read_mva_scores(isEven_);

        event->Add("mvadm_other_1", scores[0]);
        event->Add("mvadm_rho_1", scores[1]);
        event->Add("mvadm_pi_1", scores[2]);
        event->Add("mvadm_a1_1", scores[3]); 
 
      }
    }

    return 0;
  }
  int MVADMEmbedder::PostAnalysis() {
    return 0;
  }

  void MVADMEmbedder::PrintInfo() {
    ;
  }

}
