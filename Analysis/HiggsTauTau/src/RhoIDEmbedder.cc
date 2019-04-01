#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/RhoIDEmbedder.h"

 

namespace ic {

  RhoIDEmbedder::RhoIDEmbedder(std::string const& name) : ModuleBase(name), channel_(channel::tt) {
    fs_ = NULL;
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
    if(fs_){  
      outtree_ = fs_->make<TTree>("train_ntuple","train_ntuple");
      outtree_->Branch("tauFlag1"       , &tauFlag1_ );     
      outtree_->Branch("tauFlag2"       , &tauFlag2_ );
      outtree_->Branch("gen_match_1"    , &gen_match_1_);
      outtree_->Branch("gen_match_2"    , &gen_match_2_);
      outtree_->Branch("wt"             , &wt_       );
      outtree_->Branch("Ngammas_1"      , &Ngammas_1_ );
      outtree_->Branch("NgammasModif_1" , &NgammasModif_1_ );
      outtree_->Branch("Egamma1_1"      , &Egamma1_1_ );
      outtree_->Branch("Egamma2_1"      , &Egamma2_1_ );
      outtree_->Branch("Egamma3_1"      , &Egamma3_1_ );
      outtree_->Branch("Egamma4_1"      , &Egamma4_1_ );
      outtree_->Branch("Epi_1"          , &Epi_1_ );
      outtree_->Branch("Mpi0_1"         , &Mpi0_1_ );
      outtree_->Branch("Mrho_1"         , &Mrho_1_ );
      //outtree_->Branch("dphi_1"       , &dphi_1_ );
      //outtree_->Branch("dEta_1"       , &dEta_1_ );
      outtree_->Branch("gammas_dEta_1"  , &gammas_dEta_1_ );
      outtree_->Branch("gammas_dphi_1"  , &gammas_dphi_1_ );
      outtree_->Branch("pt_1"           , &pt_1_ );
      outtree_->Branch("eta_1"          , &eta_1_ );
      outtree_->Branch("E_1"            , &E_1_);
      outtree_->Branch("rho_dphi_1"     ,&rho_dphi_1_);
      outtree_->Branch("rho_dEta_1"     ,&rho_dEta_1_);
      outtree_->Branch("Epi0_1"         ,&Epi0_1_);
      outtree_->Branch("tau_decay_mode_1",&tau_decay_mode_1_);
      outtree_->Branch("Ngammas_2"      , &Ngammas_2_ );
      outtree_->Branch("NgammasModif_2" , &NgammasModif_2_ );
      outtree_->Branch("Egamma1_2"      , &Egamma1_2_ );
      outtree_->Branch("Egamma2_2"      , &Egamma2_2_ );
      outtree_->Branch("Egamma3_2"      , &Egamma3_2_ );
      outtree_->Branch("Egamma4_2"      , &Egamma4_2_ );
      outtree_->Branch("Epi_2"          , &Epi_2_ );
      outtree_->Branch("Mpi0_2"         , &Mpi0_2_ );
      outtree_->Branch("Mrho_2"         , &Mrho_2_ );
      //outtree_->Branch("dphi_2"       , &dphi_2_ );
      //outtree_->Branch("dEta_2"       , &dEta_2_ );
      outtree_->Branch("gammas_dEta_2", &gammas_dEta_2_ );
      outtree_->Branch("gammas_dphi_2", &gammas_dphi_2_ );
      outtree_->Branch("pt_2"         , &pt_2_ );
      outtree_->Branch("eta_2"        , &eta_2_ );
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
 
    }
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

    return 0;
  }

  int RhoIDEmbedder::Execute(TreeEvent *event) {

    
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    wt_ = 1;
    wt_ = eventInfo->total_weight();

    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>("ditau");
    CompositeCandidate const* ditau = ditau_vec.at(0);
    Candidate const* lep1 = ditau->GetCandidate("lepton1");
    Candidate const* lep2 = ditau->GetCandidate("lepton2");
    
    std::vector<ic::PFCandidate*> pfcands =  event->GetPtrVec<ic::PFCandidate>("pfCandidates");
    std::vector<ic::PFCandidate*> gammas1;
    std::vector<ic::PFCandidate*> gammas2;
    std::pair<ic::Candidate*, ic::Candidate*> rho_1; 
    std::pair<ic::Candidate*, ic::Candidate*> rho_2;

    if (event->Exists("tauFlag1")) tauFlag1_ = event->Get<int>("tauFlag1");
    if (event->Exists("tauFlag2")) tauFlag2_ = event->Get<int>("tauFlag2");

    if(event->Exists("gen_match_1")) gen_match_1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
    if(event->Exists("gen_match_2")) gen_match_2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));

//-------------------------------------subleading tau--------------------
    Egamma1_2_=-1, Egamma2_2_=-1, Egamma3_2_=-1, Egamma4_2_=-1;
    E_2_=-1, Epi_2_=-1, Mpi0_2_=-1, Mrho_2_=-1, rho_dEta_2_=-1, rho_dphi_2_=-1, gammas_dphi_2_ = -1., gammas_dEta_2_ = -1.,  pt_2_=-1, eta_2_=-1;
    CenterEta_2=-1;  CenterPhi_2=-1;//temp variables 
    ConeRadiusMax_2_=-1; ConeRadiusMedian_2_=-1; ConeRadiusMean_2_=-1; ConeRadiusStdDev_2_=-1;
    ConeRadiusMaxWRTtau_2_=-1; ConeRadiusMedianWRTtau_2_=-1; ConeRadiusMeanWRTtau_2_=-1; ConeRadiusStdDevWRTtau_2_=-1;
 
    if ((channel_ == channel::tt||channel_ == channel::mt||channel_ == channel::et) && event->ExistsInTree("pfCandidates")) {
      Tau const* tau2 = dynamic_cast<Tau const*>(lep2);
      tau_decay_mode_2_=tau2->decay_mode();
      if (tau2->decay_mode()==1) {
        gammas2 = GetTauGammas(tau2, pfcands);

    

        Candidate *pi_2 = rho_2.first;
        Candidate *pi0_2 = rho_2.second;

       if(gammas2.size()>=1) Egamma1_2_ = gammas2[0]->energy();
        if(gammas2.size()>=2) Egamma2_2_ = gammas2[1]->energy();
        if(gammas2.size()>=3) Egamma3_2_ = gammas2[2]->energy();
        if(gammas2.size()>=4) Egamma4_2_ = gammas2[3]->energy();

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
        rho_dphi_2_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(pi_2->vector(),pi0_2->vector()));
        rho_dEta_2_ = std::fabs(pi_2->eta()-pi0_2->eta());

        pt_2_ = tau2->pt();
        eta_2_ = tau2->eta();
    
    //New variables by Mohammad
        
        NgammasModif_2_=0;
        for (auto g : gammas2) 
          if (g->energy()>1.0){
            if (g->energy()>10.0)
              NgammasModif_2_+=2;
            else
              NgammasModif_2_+=1;
          }



        if(gammas2.size()>=1){
          
          CenterEta_2=pi_2->eta();
          CenterPhi_2=pi_2->phi();
          for(auto g : gammas2) {
          CenterEta_2+=g->eta();
          CenterPhi_2+=g->phi();
          }
          CenterEta_2/=(1.0+double(gammas2.size()));
          CenterPhi_2/=(1.0+double(gammas2.size()));            
        
          std::vector<double> DistToCenter2; 
          DistToCenter2.push_back( sqrt( std::pow(CenterPhi_2-pi_2->phi(),2) + std::pow(CenterEta_2-pi_2->eta(),2) ) );
          for(auto g : gammas2) DistToCenter2.push_back( sqrt( std::pow(CenterPhi_2-g->phi(),2) + std::pow(CenterEta_2-g->eta(),2) ) );//FYI: DistToCenter2.size()=1+gammas.size()
          sort( DistToCenter2.begin() , DistToCenter2.end() );
          ConeRadiusMedian_2_= DistToCenter2[DistToCenter2.size()/2] * 0.5 + DistToCenter2[(DistToCenter2.size()+1)/2-1] * 0.5;
          ConeRadiusMax_2_=DistToCenter2[DistToCenter2.size()-1];
          ConeRadiusMean_2_= std::accumulate(DistToCenter2.begin(), DistToCenter2.end(), 0.0)/double(DistToCenter2.size());
          ConeRadiusStdDev_2_=(std::inner_product(DistToCenter2.begin(), DistToCenter2.end(), DistToCenter2.begin(), 0.0) - DistToCenter2.size()*pow(ConeRadiusMean_2_,2))/(-1+DistToCenter2.size());
          ConeRadiusStdDev_2_=sqrt(ConeRadiusStdDev_2_);//Variance to StdDeV                    
        

          //--------------Now calculate the same thing with tau->Eta and tau->phi, instead of unweighted average which are  CenterPhi and CenterEta------
          std::vector<double> DistTotau2;
          DistTotau2.push_back(std::fabs(ROOT::Math::VectorUtil::DeltaR(pi_2->vector(),tau2->vector())));
          for(auto g : gammas2) DistTotau2.push_back(std::fabs(ROOT::Math::VectorUtil::DeltaR(g->vector(),tau2->vector())));       
          sort( DistTotau2.begin() , DistTotau2.end() );
          ConeRadiusMedianWRTtau_2_= DistTotau2[DistTotau2.size()/2] * 0.5 + DistTotau2[(DistTotau2.size()+1)/2-1] * 0.5;
          ConeRadiusMaxWRTtau_2_=DistTotau2[DistTotau2.size()-1];
          ConeRadiusMeanWRTtau_2_= std::accumulate(DistTotau2.begin(), DistTotau2.end(), 0.0)/double(DistTotau2.size());
          
          ConeRadiusStdDevWRTtau_2_=(std::inner_product(DistTotau2.begin(), DistTotau2.end(), DistTotau2.begin(), 0.0) - DistTotau2.size()*pow(ConeRadiusMeanWRTtau_2_,2))
                              /(-1+DistTotau2.size());
          ConeRadiusStdDevWRTtau_2_=sqrt(ConeRadiusStdDevWRTtau_2_);//Variance to StdDeV                    
        
        } 



        
        
    
        std::vector<double> inputs2 = {Egamma1_2_/E_2_, Egamma2_2_/E_2_, Egamma3_2_/E_2_, Egamma4_2_/E_2_, Epi_2_/E_2_, Mpi0_2_, Mrho_2_, gammas_dEta_2_, gammas_dphi_2_, rho_dEta_2_, rho_dphi_2_,(double)gammas2.size(), eta_2_, pt_2_};

        double score2 = read_mva_score(inputs2);
        event->Add("rho_id_2", score2);
      }
    }

//-------------------------------------leading tau--------------------
    Egamma1_1_=-1, Egamma2_1_=-1, Egamma3_1_=-1, Egamma4_1_=-1;
    E_1_=-1,  Epi_1_=-1, Mpi0_1_=-1, Mrho_1_=-1, rho_dEta_1_=-1, rho_dphi_1_=-1, pt_1_=-1, eta_1_=-1, gammas_dphi_1_ = -1., gammas_dEta_1_ = -1.; 
    CenterEta_1=-1;  CenterPhi_1=-1;//temp variables 
    ConeRadiusMax_1_=-1; ConeRadiusMedian_1_=-1; ConeRadiusMean_1_=-1; ConeRadiusStdDev_1_=-1;
    ConeRadiusMaxWRTtau_1_=-1; ConeRadiusMedianWRTtau_1_=-1; ConeRadiusMeanWRTtau_1_=-1; ConeRadiusStdDevWRTtau_1_=-1;

    if (channel_ == channel::tt && event->ExistsInTree("pfCandidates")) {
      Tau const* tau1 = dynamic_cast<Tau const*>(lep1);
      tau_decay_mode_1_=tau1->decay_mode();
      if(tau1->decay_mode()==1) {
        gammas1 = GetTauGammas(tau1, pfcands);

        std::pair<ic::Candidate*, ic::Candidate*> rho_1 = GetRho(tau1, pfcands);

        Candidate *pi_1 = rho_1.first;
        Candidate *pi0_1 = rho_1.second;


       if(gammas1.size()>=1) Egamma1_1_ = gammas1[0]->energy();
        if(gammas1.size()>=2) Egamma2_1_ = gammas1[1]->energy();
        if(gammas1.size()>=3) Egamma3_1_ = gammas1[2]->energy();
        if(gammas1.size()>=4) Egamma4_1_ = gammas1[3]->energy();

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
        Mrho_1_ = tau1->M();
        rho_dphi_1_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(pi_1->vector(),pi0_1->vector()));
        rho_dEta_1_ = std::fabs(pi_1->eta()-pi0_1->eta());

        pt_1_ = tau1->pt();
        eta_1_ = tau1->eta();





    //New variables by Mohammad
    
        NgammasModif_1_=0;
        for (auto g : gammas1) 
          if (g->energy()>1.0){
            if (g->energy()>10.0)
              NgammasModif_1_+=2;
            else
              NgammasModif_1_+=1;
          }



        if(gammas1.size()>=1){
          
          CenterEta_1=pi_1->eta();
          CenterPhi_1=pi_1->phi();
          for(auto g : gammas1) {
          CenterEta_1+=g->eta();
          CenterPhi_1+=g->phi();
          }
          CenterEta_1/=(1.0+double(gammas1.size()));
          CenterPhi_1/=(1.0+double(gammas1.size()));            
                 
          std::vector<double> DistToCenter1; 
          DistToCenter1.push_back( sqrt( std::pow(CenterPhi_1-pi_1->phi(),2) + std::pow(CenterEta_1-pi_1->eta(),2) ) );
          for(auto g : gammas1) DistToCenter1.push_back( sqrt( std::pow(CenterPhi_1-g->phi(),2) + std::pow(CenterEta_1-g->eta(),2) ) );//FYI: DistToCenter1.size()=1+gammas.size()
          sort( DistToCenter1.begin() , DistToCenter1.end() );
          ConeRadiusMedian_1_= DistToCenter1[DistToCenter1.size()/2] * 0.5 + DistToCenter1[(DistToCenter1.size()+1)/2-1] * 0.5;
          ConeRadiusMax_1_=DistToCenter1[DistToCenter1.size()-1];
          ConeRadiusMean_1_= std::accumulate(DistToCenter1.begin(), DistToCenter1.end(), 0.0)/double(DistToCenter1.size());
          
          ConeRadiusStdDev_1_=(std::inner_product(DistToCenter1.begin(), DistToCenter1.end(), DistToCenter1.begin(), 0.0) - DistToCenter1.size()*pow(ConeRadiusMean_1_,2))
                              /(-1+DistToCenter1.size());
          ConeRadiusStdDev_1_=sqrt(ConeRadiusStdDev_1_);//Variance to StdDeV                    
          //Problem: if Phi=3.1 and another Phi=-3.0 the delta=6.1???? 



          //--------------Now calculate the same thing with tau->Eta and tau->phi, instead of unweighted average which are  CenterPhi and CenterEta---------
        
          std::vector<double> DistTotau1;
          DistTotau1.push_back(std::fabs(ROOT::Math::VectorUtil::DeltaR(pi_1->vector(),tau1->vector())));
          for(auto g : gammas1) DistTotau1.push_back(std::fabs(ROOT::Math::VectorUtil::DeltaR(g->vector(),tau1->vector())));       
          sort( DistTotau1.begin() , DistTotau1.end() );
          ConeRadiusMedianWRTtau_1_= DistTotau1[DistTotau1.size()/2] * 0.5 + DistTotau1[(DistTotau1.size()+1)/2-1] * 0.5;
          ConeRadiusMaxWRTtau_1_=DistTotau1[DistTotau1.size()-1];
          ConeRadiusMeanWRTtau_1_= std::accumulate(DistTotau1.begin(), DistTotau1.end(), 0.0)/double(DistTotau1.size());
          
          ConeRadiusStdDevWRTtau_1_=(std::inner_product(DistTotau1.begin(), DistTotau1.end(), DistTotau1.begin(), 0.0) - DistTotau1.size()*pow(ConeRadiusMeanWRTtau_1_,2))
                              /(-1+DistTotau1.size());
          ConeRadiusStdDevWRTtau_1_=sqrt(ConeRadiusStdDevWRTtau_1_);//Variance to StdDeV                    
          
            

         /* std::cout<<"tauflag1="<<tauFlag1_<<", N_g1="<<gammas1.size()<<" --> ";
          for(auto g : gammas1) std::cout<<g->energy()<<"  ";
          std::cout<<std::endl<<"-------------------------------------------------"<<std::endl;*/

          /*if(ConeRadiusMax_1_>1.0){
            std::cout<<"ConeMax="<<ConeRadiusMax_1_<<std::endl;
            for(unsigned j=0;j<DistToCenter.size();j++)
                std::cout <<"index="<< j <<", Dist=" <<DistToCenter[j] << std::endl;
            for(auto g: gammas1){
              std::cout<<"gEta="<<g->eta()<<std::endl;
              std::cout<<"gPhi="<<g->phi()<<std::endl;
            }
            std::cout<<"pi_1Eta="<<pi_1->eta()<<std::endl;
            std::cout<<"pi_1phi="<<pi_1->phi()<<std::endl;
            std::cout<<"Center_Eta="<<CenterEta<<std::endl;
            std::cout<<"Center_phi="<<CenterPhi<<std::endl;
            std::cout<<"-----------------------"<<std::endl;
            
          }
          double tmpsum=0;
          double tmpmean =0;
          for (unsigned i=0; i<DistToCenter.size(); i++)
            tmpsum += DistToCenter[i];
          tmpmean = tmpsum/DistToCenter.size();
          
          std::cout << "gammas1.size()=: " << gammas1.size() <<"  DistToCenter.size()= "<<DistToCenter.size()<< std::endl;
          //std::cout << "Dist.begin="<<DistToCenter.begin()<< "  Dist.end=" <<DistToCenter.end()<<std::endl;  
          for(unsigned j=0;j<DistToCenter.size();j++)
            std::cout <<"index="<< j <<", Dist=" <<DistToCenter[j] << std::endl;
          
          double tmpStdDeV=0;
          for(unsigned j=0;j<DistToCenter.size();j++)
            tmpStdDeV+=pow(DistToCenter[j],2);
          tmpStdDeV-=DistToCenter.size()*pow(tmpmean,2);
          tmpStdDeV/=(double(DistToCenter.size())-1.0);
          tmpStdDeV=sqrt(tmpStdDeV);
          std::cout << "tmpmean=" << tmpmean << std::endl;
          std::cout << "ConeMean=" << ConeRadiusMean_1_ << std::endl;
          std::cout <<"ConeStd: " << ConeRadiusStdDev_1_ << std::endl;
          std::cout<<"tpmStdDeV: "<<tmpStdDeV<<std::endl;
          std::cout<<"ConeMax:"<<ConeRadiusMax_1_<<std::endl;
          std::cout<<"ConeMedian:"<<ConeRadiusMedian_1_<<std::endl;
          std::cout<<"-----------------------"<<std::endl;
          */
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

        double score1 = read_mva_score(inputs1);
        event->Add("rho_id_1", score1);
      } 
    }

    if(fs_) outtree_->Fill();

    return 0;
  }
  int RhoIDEmbedder::PostAnalysis() {
    return 0;
  }

  void RhoIDEmbedder::PrintInfo() {
    ;
  }

}
