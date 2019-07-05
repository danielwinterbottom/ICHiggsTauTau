#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/MVADMEmbedder.h"

 

namespace ic {

  MVADMEmbedder::MVADMEmbedder(std::string const& name) : ModuleBase(name), channel_(channel::tt), strategy_(strategy::cpdecays16) {
    fs_ = NULL;
    gammas_pt_cut_ = 0.5;
  }

  MVADMEmbedder::~MVADMEmbedder() {
    ;
  }

  std::vector<float> MVADMEmbedder::read_mva_scores(unsigned isEven, std::vector<float> vars, int decay_mode) {
      std::vector<float> scores = {};
      var0=vars[0], var1=vars[1], var2=vars[2], var3=vars[3], var4=vars[4], var5=vars[5], var6=vars[6], var7=vars[7], var8=vars[8], var9=vars[9], var10=vars[10], var11=vars[11], var12=vars[12], var13=vars[13]; var14=vars[14]; var15=vars[15]; var16=vars[16]; var17=vars[17]; var18=vars[18]; var19=vars[19]; var20=vars[20]; var21=vars[21]; var22=vars[22]; var23=vars[23];
      if(decay_mode>=10) {
        var24=vars[24]; var25=vars[25]; var26=vars[26]; var27=vars[27]; var28=vars[28]; var29=vars[29]; var30=vars[30]; var31=vars[31]; var32=vars[32]; var33=vars[33]; var34=vars[34]; var35=vars[35]; var36=vars[36]; var37=vars[37]; var38=vars[38]; var39=vars[39];
        if(isEven) scores = reader_dm10_even_->EvaluateMulticlass("BDT method");
        else       scores = reader_dm10_odd_->EvaluateMulticlass("BDT method");
      } else {
        if(isEven) scores = reader_even_->EvaluateMulticlass("BDT method"); 
        else       scores = reader_odd_->EvaluateMulticlass("BDT method");
      }
      //////////////// 
      //std::cout << "-------" << std::endl;
      //std::cout << evt_ << std::endl;
      //for (unsigned i=0; i<vars.size(); ++i) std::cout << i << "    " << vars[i] << std::endl;
      //if(decay_mode==10) {
      //std::cout  << "    " << var0<< "    " << var1<< "    " << var2<< "    " << var3<< "    " << var4<< "    " << var5<< "    " << var6<< "    " << var7<< "    " << var8<< "    " << var9<< "    " << var10<< "    " << var11<< "    " << var12<< "    " << var13<< "    " << var14<< "    " << var15<< "    " << var16<< "    " << var17<< "    " << var18<< "    " << var19<< "    " << var20<< "    " << var21<< "    " << var22<< "    " << var23<< "    " << var24<< "    " << var25<< "    " << var26<< "    " << var27<< "    " << var28<< "    " << var29<< "    " << var30<< "    " << var31<< "    " << var32<< "    " << var33<< "    " << var34<< "    " << var35<< "    " << var36<< "    " << var37<< "    " << var38<< "    " << var39 << std::endl;

      //}
      //std::cout << "scores:" << std::endl;
      //for(auto s : scores) std::cout << s << "    ";
      //std::cout << "\n";
      ////////////////
      return scores;
  }


  int MVADMEmbedder::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "MVADMEmbedder" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    if(strategy_==strategy::cpdecays17) {
      gammas_pt_cut_ = 1.0;
    }

    reader_even_ = new TMVA::Reader();
    reader_odd_ = new TMVA::Reader();

    TString filename_even = (std::string)getenv("CMSSW_BASE")+"/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/MVA/mvadm_inclusive_2fold_applytoeven_LeadStripSignalThenIsoCone_2016_dm0_dm1.xml"; //add apply to even here
    TString filename_odd = (std::string)getenv("CMSSW_BASE")+"/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/MVA/mvadm_inclusive_2fold_applytoodd_LeadStripSignalThenIsoCone_2016_dm0_dm1.xml";

    if(strategy_==strategy::cpdecays17) {
      // make sure you add odd training!
      filename_even = (std::string)getenv("CMSSW_BASE")+"/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/MVA/mvadm_inclusive_2fold_applytoeven_LeadStripSignalThenIsoCone_2017_dm0_dm1.xml";
      filename_odd = (std::string)getenv("CMSSW_BASE")+"/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/MVA/mvadm_inclusive_2fold_applytoodd_LeadStripSignalThenIsoCone_2017_dm0_dm1.xml";
    }

    reader_even_->AddVariable( "Egamma1_tau", &var0 );
    reader_even_->AddVariable( "Egamma2_tau", &var1 );
    reader_even_->AddVariable( "Epi_tau", &var2 );
    reader_even_->AddVariable( "rho_dEta_tau", &var3 );
    reader_even_->AddVariable( "rho_dphi_tau", &var4 );
    reader_even_->AddVariable( "gammas_dEta_tau", &var5 );
    reader_even_->AddVariable( "gammas_dR_tau", &var6 );
    reader_even_->AddVariable( "DeltaR2WRTtau_tau", &var7 );
    reader_even_->AddVariable( "tau_decay_mode", &var8 );
    reader_even_->AddVariable( "eta", &var9 );
    reader_even_->AddVariable( "pt", &var10 );
    reader_even_->AddVariable( "Epi0", &var11 );
    reader_even_->AddVariable( "Epi", &var12 );
    reader_even_->AddVariable( "rho_dEta", &var13 );
    reader_even_->AddVariable( "rho_dphi", &var14 );
    reader_even_->AddVariable( "gammas_dEta", &var15 );
    reader_even_->AddVariable( "Mrho", &var16 );
    reader_even_->AddVariable( "Mpi0", &var17 );
    reader_even_->AddVariable( "DeltaR2WRTtau", &var18 );
    reader_even_->AddVariable( "Mpi0_TwoHighGammas", &var19 );
    reader_even_->AddVariable( "Mrho_OneHighGammas", &var20 );
    reader_even_->AddVariable( "Mrho_TwoHighGammas", &var21 );
    reader_even_->AddVariable( "Mrho_subleadingGamma", &var22 );
    reader_even_->AddVariable( "strip_pt", &var23 );
    
    reader_odd_->AddVariable( "Egamma1_tau", &var0 );
    reader_odd_->AddVariable( "Egamma2_tau", &var1 );
    reader_odd_->AddVariable( "Epi_tau", &var2 );
    reader_odd_->AddVariable( "rho_dEta_tau", &var3 );
    reader_odd_->AddVariable( "rho_dphi_tau", &var4 );
    reader_odd_->AddVariable( "gammas_dEta_tau", &var5 );
    reader_odd_->AddVariable( "gammas_dR_tau", &var6 );
    reader_odd_->AddVariable( "DeltaR2WRTtau_tau", &var7 );
    reader_odd_->AddVariable( "tau_decay_mode", &var8 );
    reader_odd_->AddVariable( "eta", &var9 );
    reader_odd_->AddVariable( "pt", &var10 );
    reader_odd_->AddVariable( "Epi0", &var11 );
    reader_odd_->AddVariable( "Epi", &var12 );
    reader_odd_->AddVariable( "rho_dEta", &var13 );
    reader_odd_->AddVariable( "rho_dphi", &var14 );
    reader_odd_->AddVariable( "gammas_dEta", &var15 );
    reader_odd_->AddVariable( "Mrho", &var16 );
    reader_odd_->AddVariable( "Mpi0", &var17 );
    reader_odd_->AddVariable( "DeltaR2WRTtau", &var18 );
    reader_odd_->AddVariable( "Mpi0_TwoHighGammas", &var19 );
    reader_odd_->AddVariable( "Mrho_OneHighGammas", &var20 );
    reader_odd_->AddVariable( "Mrho_TwoHighGammas", &var21 );
    reader_odd_->AddVariable( "Mrho_subleadingGamma", &var22 );
    reader_odd_->AddVariable( "strip_pt", &var23 );

    reader_even_->BookMVA( "BDT method", filename_even );
    reader_odd_->BookMVA( "BDT method", filename_odd );

    TString filename_dm10_even = (std::string)getenv("CMSSW_BASE")+"/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/MVA/mvadm_inclusive_2fold_applytoeven_LeadStripSignalThenIsoCone_2016_dm10.xml";
    TString filename_dm10_odd = (std::string)getenv("CMSSW_BASE")+"/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/MVA/mvadm_inclusive_2fold_applytoodd_LeadStripSignalThenIsoCone_2016_dm10.xml";

    if(strategy_==strategy::cpdecays17) {
      // make sure you add odd training!
      filename_dm10_even = (std::string)getenv("CMSSW_BASE")+"/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/MVA/mvadm_inclusive_2fold_applytoeven_LeadStripSignalThenIsoCone_2017_dm10.xml";
      filename_dm10_odd = (std::string)getenv("CMSSW_BASE")+"/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/MVA/mvadm_inclusive_2fold_applytoodd_LeadStripSignalThenIsoCone_2017_dm10.xml";
    }

    reader_dm10_even_ = new TMVA::Reader();
    reader_dm10_odd_ = new TMVA::Reader();    

    reader_dm10_even_->AddVariable( "E1_overEa1", &var0 );
    reader_dm10_even_->AddVariable( "E2_overEa1", &var1 );
    reader_dm10_even_->AddVariable( "E1_overEtau", &var2 );
    reader_dm10_even_->AddVariable( "E2_overEtau", &var3 );
    reader_dm10_even_->AddVariable( "E3_overEtau", &var4 );
    reader_dm10_even_->AddVariable( "a1_pi0_dEta_timesEtau", &var5 );
    reader_dm10_even_->AddVariable( "a1_pi0_dphi_timesEtau", &var6 );
    reader_dm10_even_->AddVariable( "h1_h2_dphi_timesE12", &var7 );
    reader_dm10_even_->AddVariable( "h1_h2_dEta_timesE12", &var8 );
    reader_dm10_even_->AddVariable( "h1_h3_dphi_timesE13", &var9 );
    reader_dm10_even_->AddVariable( "h1_h3_dEta_timesE13", &var10 );
    reader_dm10_even_->AddVariable( "h2_h3_dphi_timesE23", &var11 );
    reader_dm10_even_->AddVariable( "h2_h3_dEta_timesE23", &var12 );
    reader_dm10_even_->AddVariable( "gammas_dEta_timesEtau", &var13 );
    reader_dm10_even_->AddVariable( "gammas_dR_timesEtau", &var14 );
    reader_dm10_even_->AddVariable( "tau_decay_mode", &var15 );
    reader_dm10_even_->AddVariable( "mass0", &var16 );
    reader_dm10_even_->AddVariable( "mass1", &var17 );
    reader_dm10_even_->AddVariable( "mass2", &var18 );
    reader_dm10_even_->AddVariable( "E1", &var19 );
    reader_dm10_even_->AddVariable( "E2", &var20 );
    reader_dm10_even_->AddVariable( "E3", &var21 );
    reader_dm10_even_->AddVariable( "strip_E", &var22 );
    reader_dm10_even_->AddVariable( "a1_pi0_dEta", &var23 );
    reader_dm10_even_->AddVariable( "a1_pi0_dphi", &var24 );
    reader_dm10_even_->AddVariable( "strip_pt", &var25 );
    reader_dm10_even_->AddVariable( "pt", &var26 );
    reader_dm10_even_->AddVariable( "eta", &var27 );
    reader_dm10_even_->AddVariable( "E", &var28 );
    reader_dm10_even_->AddVariable( "h1_h2_dphi", &var29 );
    reader_dm10_even_->AddVariable( "h1_h3_dphi", &var30 );
    reader_dm10_even_->AddVariable( "h2_h3_dphi", &var31 );
    reader_dm10_even_->AddVariable( "h1_h2_dEta", &var32 );
    reader_dm10_even_->AddVariable( "h1_h3_dEta", &var33 );
    reader_dm10_even_->AddVariable( "h2_h3_dEta", &var34 );
    reader_dm10_even_->AddVariable( "Egamma1", &var35 );
    reader_dm10_even_->AddVariable( "Egamma2", &var36 );
    reader_dm10_even_->AddVariable( "gammas_dEta", &var37 );
    reader_dm10_even_->AddVariable( "Mpi0", &var38 );
    reader_dm10_even_->AddVariable( "Mpi0_TwoHighGammas", &var39 );
    
    reader_dm10_odd_->AddVariable( "E1_overEa1", &var0 );
    reader_dm10_odd_->AddVariable( "E2_overEa1", &var1 );
    reader_dm10_odd_->AddVariable( "E1_overEtau", &var2 );
    reader_dm10_odd_->AddVariable( "E2_overEtau", &var3 );
    reader_dm10_odd_->AddVariable( "E3_overEtau", &var4 );
    reader_dm10_odd_->AddVariable( "a1_pi0_dEta_timesEtau", &var5 );
    reader_dm10_odd_->AddVariable( "a1_pi0_dphi_timesEtau", &var6 );
    reader_dm10_odd_->AddVariable( "h1_h2_dphi_timesE12", &var7 );
    reader_dm10_odd_->AddVariable( "h1_h2_dEta_timesE12", &var8 );
    reader_dm10_odd_->AddVariable( "h1_h3_dphi_timesE13", &var9 );
    reader_dm10_odd_->AddVariable( "h1_h3_dEta_timesE13", &var10 );
    reader_dm10_odd_->AddVariable( "h2_h3_dphi_timesE23", &var11 );
    reader_dm10_odd_->AddVariable( "h2_h3_dEta_timesE23", &var12 );
    reader_dm10_odd_->AddVariable( "gammas_dEta_timesEtau", &var13 );
    reader_dm10_odd_->AddVariable( "gammas_dR_timesEtau", &var14 );
    reader_dm10_odd_->AddVariable( "tau_decay_mode", &var15 );
    reader_dm10_odd_->AddVariable( "mass0", &var16 );
    reader_dm10_odd_->AddVariable( "mass1", &var17 );
    reader_dm10_odd_->AddVariable( "mass2", &var18 );
    reader_dm10_odd_->AddVariable( "E1", &var19 );
    reader_dm10_odd_->AddVariable( "E2", &var20 );
    reader_dm10_odd_->AddVariable( "E3", &var21 );
    reader_dm10_odd_->AddVariable( "strip_E", &var22 );
    reader_dm10_odd_->AddVariable( "a1_pi0_dEta", &var23 );
    reader_dm10_odd_->AddVariable( "a1_pi0_dphi", &var24 );
    reader_dm10_odd_->AddVariable( "strip_pt", &var25 );
    reader_dm10_odd_->AddVariable( "pt", &var26 );
    reader_dm10_odd_->AddVariable( "eta", &var27 );
    reader_dm10_odd_->AddVariable( "E", &var28 );
    reader_dm10_odd_->AddVariable( "h1_h2_dphi", &var29 );
    reader_dm10_odd_->AddVariable( "h1_h3_dphi", &var30 );
    reader_dm10_odd_->AddVariable( "h2_h3_dphi", &var31 );
    reader_dm10_odd_->AddVariable( "h1_h2_dEta", &var32 );
    reader_dm10_odd_->AddVariable( "h1_h3_dEta", &var33 );
    reader_dm10_odd_->AddVariable( "h2_h3_dEta", &var34 );
    reader_dm10_odd_->AddVariable( "Egamma1", &var35 );
    reader_dm10_odd_->AddVariable( "Egamma2", &var36 );
    reader_dm10_odd_->AddVariable( "gammas_dEta", &var37 );
    reader_dm10_odd_->AddVariable( "Mpi0", &var38 );
    reader_dm10_odd_->AddVariable( "Mpi0_TwoHighGammas", &var39 );

    reader_dm10_even_->BookMVA( "BDT method", filename_dm10_even );
    reader_dm10_odd_->BookMVA( "BDT method", filename_dm10_odd );

    return 0;
  }

  int MVADMEmbedder::Execute(TreeEvent *event) {

    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    isEven_ = eventInfo->event() % 2 == 0; // if even then event_ = 1, odd = 0
    evt_ = eventInfo->event();
    event_ = (float)isEven_;

    if (!(channel_ == channel::tt||channel_ == channel::mt||channel_ == channel::et)) return 0;
    

    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>("ditau");
    CompositeCandidate const* ditau = ditau_vec.at(0);
    Candidate * lep1 = ditau->GetCandidate("lepton1");
    Candidate * lep2 = ditau->GetCandidate("lepton2");
    
    std::vector<ic::PFCandidate*> pfcands =  event->GetPtrVec<ic::PFCandidate>("pfCandidates");
    std::vector<ic::PFCandidate*> gammas1;
    std::vector<ic::PFCandidate*> gammas2;
    std::pair<ic::Candidate*, ic::Candidate*> rho_1; 
    std::pair<ic::Candidate*, ic::Candidate*> rho_2;


//-------------------------------------subleading tau--------------------

    if ((channel_ == channel::tt||channel_ == channel::mt||channel_ == channel::et) && event->ExistsInTree("pfCandidates")) {
      Tau * tau2 = dynamic_cast<Tau *>(lep2);
      tau_decay_mode_2_=tau2->decay_mode();

      //std::cout << tau2->GetTauID("MVADM2016v1DM0raw") << "    " << tau2->GetTauID("MVADM2016v1DM1raw") << "    " << tau2->GetTauID("MVADM2016v1DM2raw") << "    " << tau2->GetTauID("MVADM2016v1DM10raw") << "    " << tau2->GetTauID("MVADM2016v1DM11raw") << std::endl;

      std::vector<ic::PFCandidate*> a1_daughters_2 = {};
      std::pair<ic::Candidate*, ic::Candidate*> rho_2 = std::make_pair(new Candidate(), new Candidate());
 
      ic::Candidate *pi0_2 = new ic::Candidate();

      if(tau_decay_mode_2_>=10){
        std::pair<std::vector<ic::PFCandidate*>, ic::Candidate*>  a1 = GetA1(tau2, pfcands, gammas_pt_cut_);
        a1_daughters_2  = a1.first;
        pi0_2 = a1.second;

      } else {
        rho_2 = GetRho(tau2, pfcands, gammas_pt_cut_);
        pi0_2 = rho_2.second;
      }

      gammas2 = GetTauGammas(tau2, pfcands);
      strip_pt_2_ = pi0_2->pt();

      E_2_ = tau2->energy();;
      E1_2_=-1;
      E2_2_=-1;
      E3_2_=-1;
      a1_pi0_dEta_2_=-1;
      a1_pi0_dphi_2_=-1;
      h1_h2_dEta_2_=-1;
      h1_h2_dphi_2_=-1;
      h1_h3_dEta_2_=-1;
      h1_h3_dphi_2_=-1;
      h2_h3_dEta_2_=-1;
      h2_h3_dphi_2_=-1;
      mass0_2_=-1;
      mass1_2_=-1;
      mass2_2_=-1;
      strip_E_2_=-1;

      if(tau_decay_mode_2_==10){
        strip_E_2_ = pi0_2->energy();
        mass0_2_ = (a1_daughters_2[0]->vector() + a1_daughters_2[1]->vector() + a1_daughters_2[2]->vector()).M();
        mass1_2_ = (a1_daughters_2[0]->vector() + a1_daughters_2[1]->vector()).M();
        mass2_2_ = (a1_daughters_2[0]->vector() + a1_daughters_2[2]->vector()).M();
        E1_2_ = a1_daughters_2[0]->energy();
        E2_2_ = a1_daughters_2[1]->energy();
        E3_2_ = a1_daughters_2[2]->energy();

        double Ea1 = E1_2_+E2_2_+E3_2_;
        E1_overEa1_2_ = E1_2_/Ea1;
        E2_overEa1_2_ = E2_2_/Ea1;
        E3_overEa1_2_ = E3_2_/Ea1;
        double Etau = Ea1+strip_E_2_;
        E1_overEtau_2_ = E1_2_/Etau;
        E2_overEtau_2_ = E2_2_/Etau;
        E3_overEtau_2_ = E3_2_/Etau;

        if(strip_pt_2_>0) {
          a1_pi0_dEta_2_ = std::fabs(pi0_2->eta()-tau2->eta());
          a1_pi0_dphi_2_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(pi0_2->vector(),tau2->vector()));
        }

        a1_pi0_dEta_timesEtau_2_=a1_pi0_dEta_2_*Etau;
        a1_pi0_dphi_timesEtau_2_=a1_pi0_dphi_2_*Etau;

        h1_h2_dphi_2_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(a1_daughters_2[0]->vector(),a1_daughters_2[1]->vector()));
        h1_h3_dphi_2_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(a1_daughters_2[0]->vector(),a1_daughters_2[2]->vector()));
        h2_h3_dphi_2_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(a1_daughters_2[1]->vector(),a1_daughters_2[2]->vector()));
        h1_h2_dEta_2_ = std::fabs(a1_daughters_2[0]->eta()-a1_daughters_2[1]->eta());
        h1_h3_dEta_2_ = std::fabs(a1_daughters_2[0]->eta()-a1_daughters_2[2]->eta());
        h2_h3_dEta_2_ = std::fabs(a1_daughters_2[1]->eta()-a1_daughters_2[2]->eta());

        h1_h2_dphi_timesE12_2_=h1_h2_dphi_2_*(E1_2_+E2_2_);
        h1_h3_dphi_timesE13_2_=h1_h3_dphi_2_*(E1_2_+E3_2_);
        h2_h3_dphi_timesE23_2_=h2_h3_dphi_2_*(E2_2_+E3_2_);
        h1_h2_dEta_timesE12_2_=h1_h2_dEta_2_*(E1_2_+E2_2_);
        h1_h3_dEta_timesE13_2_=h1_h3_dEta_2_*(E1_2_+E3_2_);
        h2_h3_dEta_timesE23_2_=h2_h3_dEta_2_*(E2_2_+E3_2_);

      }

      if (tau_decay_mode_2_<12) {

        Candidate *pi_2 = rho_2.first;
        
        double Egamma1_2_=-1, Egamma2_2_=-1;
        double E = tau2->energy();
        Epi_2_ = pi_2->energy();
        Epi0_2_ = pi0_2->energy();

        if(gammas2.size()>=1) Egamma1_2_ = gammas2[0]->energy();
        if(gammas2.size()>=2) Egamma2_2_ = gammas2[1]->energy();
 
        Egamma1_tau_2_ = Egamma1_2_/E;
        Egamma2_tau_2_ = Egamma2_2_/E;

        Epi_tau_2_ = Epi_2_/E;

        pt_2_ = tau2->pt();
        eta_2_ = tau2->eta();

        rho_dEta_2_=-1, rho_dphi_2_=-1, gammas_dEta_2_ = -1.;
        double gammas_dphi = -1.;

        if(Epi0_2_>0) {
          rho_dphi_2_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(pi_2->vector(),pi0_2->vector()));
          rho_dEta_2_ = std::fabs(pi_2->eta()-pi0_2->eta());
        }
        rho_dEta_tau_2_ = rho_dEta_2_*E;
        rho_dphi_tau_2_ = rho_dphi_2_*E;


        if(gammas2.size()>1) {
          gammas_dphi =  std::fabs(ROOT::Math::VectorUtil::DeltaPhi(gammas2[0]->vector(),gammas2[1]->vector()));
          gammas_dEta_2_ =  std::fabs(gammas2[0]->eta()-gammas2[1]->eta());
        }
        gammas_dEta_tau_2_ = gammas_dEta_2_ * E;
        gammas_dR_tau_2_ =  sqrt(gammas_dEta_2_*gammas_dEta_2_ + gammas_dphi*gammas_dphi)*E;
        //--------Mpi0---
        Mpi0_2_=-1, Mpi0_TwoHighGammas_2_=-1; 
        ROOT::Math::PtEtaPhiEVector gammas_vector_2;
        for (auto g : gammas2) gammas_vector_2+=g->vector();
        Mpi0_2_ = gammas_vector_2.M();
        if(gammas2.size()>=2) Mpi0_TwoHighGammas_2_ = (gammas2[0]->vector() + gammas2[1]->vector()).M();
        //--------Mrho------
        Mrho_2_=-1, Mrho_OneHighGammas_2_=-1; Mrho_TwoHighGammas_2_=-1; Mrho_subleadingGamma_2_=-1;
        Mrho_2_ = (pi_2->vector() + pi0_2->vector()).M();
        if(gammas2.size()>=1) Mrho_OneHighGammas_2_=( pi_2->vector() + gammas2[0]->vector() ).M();
        if(gammas2.size()>=2) Mrho_TwoHighGammas_2_=( pi_2->vector() + gammas2[0]->vector() + gammas2[1]->vector()  ).M();
        if(gammas2.size()>=2) Mrho_subleadingGamma_2_= (pi_2->vector() + gammas2[1]->vector()).M();
        //-----------


        DeltaR2WRTtau_2_=-999; 
        if(gammas2.size()>=1){
          DeltaR2WRTtau_2_=0;
          double SumPt=0;
          DeltaR2WRTtau_2_=std::pow(ROOT::Math::VectorUtil::DeltaR(pi_2->vector(),tau2->vector()),2)*std::pow(pi_2->pt(),2);
          SumPt=std::pow(pi_2->pt(),2);
          for(auto g : gammas2){
            DeltaR2WRTtau_2_+=std::pow(ROOT::Math::VectorUtil::DeltaR(g->vector(),tau2->vector()),2)*std::pow(g->pt(),2);
            SumPt+=std::pow(g->pt(),2);
          }
          DeltaR2WRTtau_2_/=SumPt;
        }
        DeltaR2WRTtau_tau_2_ = DeltaR2WRTtau_2_*E*E;
 
        std::vector<float> inputs2 = {};

        if(tau_decay_mode_2_<2) {
          inputs2.resize(24);

          inputs2[0] = Egamma1_tau_2_ ;
          inputs2[1] = Egamma2_tau_2_ ;
          inputs2[2] = Epi_tau_2_ ;
          inputs2[3] = rho_dEta_tau_2_ ;
          inputs2[4] = rho_dphi_tau_2_ ;
          inputs2[5] = gammas_dEta_tau_2_ ;
          inputs2[6] = gammas_dR_tau_2_ ;
          inputs2[7] = DeltaR2WRTtau_tau_2_ ;
          inputs2[8] = tau_decay_mode_2_ ;
          inputs2[9] = eta_2_ ;
          inputs2[10] = pt_2_ ;
          inputs2[11] = Epi0_2_ ;
          inputs2[12] = Epi_2_ ;
          inputs2[13] = rho_dEta_2_ ;
          inputs2[14] = rho_dphi_2_ ;
          inputs2[15] = gammas_dEta_2_ ;
          inputs2[16] = Mrho_2_ ;
          inputs2[17] = Mpi0_2_ ;
          inputs2[18] = DeltaR2WRTtau_2_ ;
          inputs2[19] = Mpi0_TwoHighGammas_2_ ;
          inputs2[20] = Mrho_OneHighGammas_2_ ;
          inputs2[21] = Mrho_TwoHighGammas_2_ ;
          inputs2[22] = Mrho_subleadingGamma_2_ ;
          inputs2[23] = strip_pt_2_ ;
        }

        if(tau_decay_mode_2_>9) {
          inputs2.resize(40);

          inputs2[0] = E1_overEa1_2_ ;
          inputs2[1] = E2_overEa1_2_ ;
          inputs2[2] = E1_overEtau_2_ ;
          inputs2[3] = E2_overEtau_2_ ;
          inputs2[4] = E3_overEtau_2_ ;
          inputs2[5] = a1_pi0_dEta_timesEtau_2_ ;
          inputs2[6] = a1_pi0_dphi_timesEtau_2_ ;
          inputs2[7] = h1_h2_dphi_timesE12_2_ ;
          inputs2[8] = h1_h2_dEta_timesE12_2_ ;
          inputs2[9] = h1_h3_dphi_timesE13_2_ ;
          inputs2[10] = h1_h3_dEta_timesE13_2_ ;
          inputs2[11] = h2_h3_dphi_timesE23_2_ ;
          inputs2[12] = h2_h3_dEta_timesE23_2_ ;
          inputs2[13] = gammas_dEta_tau_2_ ;
          inputs2[14] = gammas_dR_tau_2_ ;
          inputs2[15] = tau_decay_mode_2_ ;
          inputs2[16] = mass0_2_ ;
          inputs2[17] = mass1_2_ ;
          inputs2[18] = mass2_2_ ;
          inputs2[19] = E1_2_ ;
          inputs2[20] = E2_2_ ;
          inputs2[21] = E3_2_ ;
          inputs2[22] = strip_E_2_ ;
          inputs2[23] = a1_pi0_dEta_2_ ;
          inputs2[24] = a1_pi0_dphi_2_ ;
          inputs2[25] = strip_pt_2_ ;
          inputs2[26] = pt_2_ ;
          inputs2[27] = eta_2_ ;
          inputs2[28] = E_2_ ;
          inputs2[29] = h1_h2_dphi_2_ ;
          inputs2[30] = h1_h3_dphi_2_ ;
          inputs2[31] = h2_h3_dphi_2_ ;
          inputs2[32] = h1_h2_dEta_2_ ;
          inputs2[33] = h1_h3_dEta_2_ ;
          inputs2[34] = h2_h3_dEta_2_ ;
          inputs2[35] = Egamma1_2_ ;
          inputs2[36] = Egamma2_2_ ;
          inputs2[37] = gammas_dEta_2_ ;
          inputs2[38] = Mpi0_2_ ;
          inputs2[39] = Mpi0_TwoHighGammas_2_ ;
        }

        if(tau_decay_mode_2_<2) {
          std::vector<float> scores2 = read_mva_scores(isEven_,inputs2,tau2->decay_mode());
          event->Add("mvadm_other_2", scores2[0]);
          event->Add("mvadm_rho_2", scores2[1]);
          event->Add("mvadm_pi_2", scores2[2]);
          event->Add("mvadm_a1_2", scores2[3]); 
        } else if (tau_decay_mode_2_>9){
          std::vector<float> scores2 = read_mva_scores(isEven_,inputs2,tau2->decay_mode());
          event->Add("mvadm_other_2", scores2[0]);
          event->Add("mvadm_a1_2", scores2[1]);
          event->Add("mvadm_3pipi0_2", scores2[2]);
        }
      }
    }

//-------------------------------------leading tau--------------------
    if (channel_ == channel::tt && event->ExistsInTree("pfCandidates")) {

      Tau * tau1 = dynamic_cast<Tau *>(lep1);
      tau_decay_mode_1_=tau1->decay_mode();

      std::vector<ic::PFCandidate*> a1_daughters_1 = {};
      std::pair<ic::Candidate*, ic::Candidate*> rho_1 = std::make_pair(new Candidate(), new Candidate());

      ic::Candidate *pi0_1 = new ic::Candidate();

      if(tau_decay_mode_1_>=10){
        std::pair<std::vector<ic::PFCandidate*>, ic::Candidate*>  a1 = GetA1(tau1, pfcands, gammas_pt_cut_);
        a1_daughters_1  = a1.first;
        pi0_1 = a1.second;

      } else {
        rho_1 = GetRho(tau1, pfcands, gammas_pt_cut_);
        pi0_1 = rho_1.second;
      }

      gammas1 = GetTauGammas(tau1, pfcands);
      strip_pt_1_ = pi0_1->pt();

      E_1_ = tau1->energy();;
      E1_1_=-1;
      E2_1_=-1;
      E3_1_=-1;
      a1_pi0_dEta_1_=-1;
      a1_pi0_dphi_1_=-1;
      h1_h2_dEta_1_=-1;
      h1_h2_dphi_1_=-1;
      h1_h3_dEta_1_=-1;
      h1_h3_dphi_1_=-1;
      h2_h3_dEta_1_=-1;
      h2_h3_dphi_1_=-1;
      mass0_1_=-1;
      mass1_1_=-1;
      mass2_1_=-1;
      strip_E_1_=-1;


      if(tau_decay_mode_1_==10){
        strip_E_1_ = pi0_1->energy();
        mass0_1_ = (a1_daughters_1[0]->vector() + a1_daughters_1[1]->vector() + a1_daughters_1[2]->vector()).M();
        mass1_1_ = (a1_daughters_1[0]->vector() + a1_daughters_1[1]->vector()).M();
        mass2_1_ = (a1_daughters_1[0]->vector() + a1_daughters_1[2]->vector()).M();
        E1_1_ = a1_daughters_1[0]->energy();
        E2_1_ = a1_daughters_1[1]->energy();
        E3_1_ = a1_daughters_1[2]->energy();

        double Ea1 = E1_1_+E2_1_+E3_1_;
        E1_overEa1_1_ = E1_1_/Ea1;
        E2_overEa1_1_ = E2_1_/Ea1;
        E3_overEa1_1_ = E3_1_/Ea1;
        double Etau = Ea1+strip_E_1_;
        E1_overEtau_1_ = E1_1_/Etau;
        E2_overEtau_1_ = E2_1_/Etau;
        E3_overEtau_1_ = E3_1_/Etau;

        if(strip_pt_1_>0) {
          a1_pi0_dEta_1_ = std::fabs(pi0_1->eta()-tau1->eta());
          a1_pi0_dphi_1_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(pi0_1->vector(),tau1->vector()));
        }

        a1_pi0_dEta_timesEtau_1_=a1_pi0_dEta_1_*Etau;
        a1_pi0_dphi_timesEtau_1_=a1_pi0_dphi_1_*Etau;

        h1_h2_dphi_1_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(a1_daughters_1[0]->vector(),a1_daughters_1[1]->vector()));
        h1_h3_dphi_1_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(a1_daughters_1[0]->vector(),a1_daughters_1[2]->vector()));
        h2_h3_dphi_1_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(a1_daughters_1[1]->vector(),a1_daughters_1[2]->vector()));
        h1_h2_dEta_1_ = std::fabs(a1_daughters_1[0]->eta()-a1_daughters_1[1]->eta());
        h1_h3_dEta_1_ = std::fabs(a1_daughters_1[0]->eta()-a1_daughters_1[2]->eta());
        h2_h3_dEta_1_ = std::fabs(a1_daughters_1[1]->eta()-a1_daughters_1[2]->eta());

        h1_h2_dphi_timesE12_1_=h1_h2_dphi_1_*(E1_1_+E2_1_);
        h1_h3_dphi_timesE13_1_=h1_h3_dphi_1_*(E1_1_+E3_1_);
        h2_h3_dphi_timesE23_1_=h2_h3_dphi_1_*(E2_1_+E3_1_);
        h1_h2_dEta_timesE12_1_=h1_h2_dEta_1_*(E1_1_+E2_1_);
        h1_h3_dEta_timesE13_1_=h1_h3_dEta_1_*(E1_1_+E3_1_);
        h2_h3_dEta_timesE23_1_=h2_h3_dEta_1_*(E2_1_+E3_1_);

      }


      if (tau_decay_mode_1_<12) {

        Candidate *pi_1 = rho_1.first;
        
        double Egamma1_1_=-1, Egamma2_1_=-1;
        double E = tau1->energy();
        Epi_1_ = pi_1->energy();
        Epi0_1_ = pi0_1->energy();

        if(gammas1.size()>=1) Egamma1_1_ = gammas1[0]->energy();
        if(gammas1.size()>=2) Egamma2_1_ = gammas1[1]->energy();
 
        Egamma1_tau_1_ = Egamma1_1_/E;
        Egamma2_tau_1_ = Egamma2_1_/E;

        Epi_tau_1_ = Epi_1_/E;

        pt_1_ = tau1->pt();
        eta_1_ = tau1->eta();

        rho_dEta_1_=-1, rho_dphi_1_=-1, gammas_dEta_1_ = -1.;
        double gammas_dphi = -1.;

        if(Epi0_1_>0) {
          rho_dphi_1_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(pi_1->vector(),pi0_1->vector()));
          rho_dEta_1_ = std::fabs(pi_1->eta()-pi0_1->eta());
        }
        rho_dEta_tau_1_ = rho_dEta_1_*E;
        rho_dphi_tau_1_ = rho_dphi_1_*E;

        if(gammas1.size()>1) {
          gammas_dphi =  std::fabs(ROOT::Math::VectorUtil::DeltaPhi(gammas1[0]->vector(),gammas1[1]->vector()));
          gammas_dEta_1_ =  std::fabs(gammas1[0]->eta()-gammas1[1]->eta());
        }
        gammas_dEta_tau_1_ = gammas_dEta_1_* E;
        gammas_dR_tau_1_ =  sqrt(gammas_dEta_1_*gammas_dEta_1_ + gammas_dphi*gammas_dphi)*E;

        //--------Mpi0---
        Mpi0_1_=-1, Mpi0_TwoHighGammas_1_=-1; 
        ROOT::Math::PtEtaPhiEVector gammas_vector_1;
        for (auto g : gammas1) gammas_vector_1+=g->vector();
        Mpi0_1_ = gammas_vector_1.M();
        if(gammas1.size()>=2) Mpi0_TwoHighGammas_1_ = (gammas1[0]->vector() + gammas1[1]->vector()).M();
        //--------Mrho------
        Mrho_1_=-1, Mrho_OneHighGammas_1_=-1; Mrho_TwoHighGammas_1_=-1; Mrho_subleadingGamma_1_=-1;
        Mrho_1_ = (pi_1->vector() + pi0_1->vector()).M();
        if(gammas1.size()>=1) Mrho_OneHighGammas_1_=( pi_1->vector() + gammas1[0]->vector() ).M();
        if(gammas1.size()>=2) Mrho_TwoHighGammas_1_=( pi_1->vector() + gammas1[0]->vector() + gammas1[1]->vector()  ).M();
        if(gammas1.size()>=2) Mrho_subleadingGamma_1_= (pi_1->vector() + gammas1[1]->vector()).M();
        //------------

        DeltaR2WRTtau_1_=-999; 
        if(gammas1.size()>=1){
          DeltaR2WRTtau_1_=0;
          double SumPt=0;
          DeltaR2WRTtau_1_=std::pow(ROOT::Math::VectorUtil::DeltaR(pi_1->vector(),tau1->vector()),2)*std::pow(pi_1->pt(),2);
          SumPt=std::pow(pi_1->pt(),2);
          for(auto g : gammas1){
            DeltaR2WRTtau_1_+=std::pow(ROOT::Math::VectorUtil::DeltaR(g->vector(),tau1->vector()),2)*std::pow(g->pt(),2);
            SumPt+=std::pow(g->pt(),2);
          }
          DeltaR2WRTtau_1_/=SumPt;
        }
        DeltaR2WRTtau_tau_1_ = DeltaR2WRTtau_1_*E*E;

        std::vector<float> inputs1 = {};

        if(tau_decay_mode_1_<2) {
          inputs1.resize(24);

          inputs1[0] = Egamma1_tau_1_ ;
          inputs1[1] = Egamma2_tau_1_ ;
          inputs1[2] = Epi_tau_1_ ;
          inputs1[3] = rho_dEta_tau_1_ ;
          inputs1[4] = rho_dphi_tau_1_ ;
          inputs1[5] = gammas_dEta_tau_1_ ;
          inputs1[6] = gammas_dR_tau_1_ ;
          inputs1[7] = DeltaR2WRTtau_tau_1_ ;
          inputs1[8] = tau_decay_mode_1_ ;
          inputs1[9] = eta_1_ ;
          inputs1[10] = pt_1_ ;
          inputs1[11] = Epi0_1_ ;
          inputs1[12] = Epi_1_ ;
          inputs1[13] = rho_dEta_1_ ;
          inputs1[14] = rho_dphi_1_ ;
          inputs1[15] = gammas_dEta_1_ ;
          inputs1[16] = Mrho_1_ ;
          inputs1[17] = Mpi0_1_ ;
          inputs1[18] = DeltaR2WRTtau_1_ ;
          inputs1[19] = Mpi0_TwoHighGammas_1_ ;
          inputs1[20] = Mrho_OneHighGammas_1_ ;
          inputs1[21] = Mrho_TwoHighGammas_1_ ;
          inputs1[22] = Mrho_subleadingGamma_1_ ;
          inputs1[23] = strip_pt_1_ ;
        }

        if(tau_decay_mode_1_>9) {
          inputs1.resize(40);
          
          inputs1[0] = E1_overEa1_1_ ;
          inputs1[1] = E2_overEa1_1_ ;
          inputs1[2] = E1_overEtau_1_ ;
          inputs1[3] = E2_overEtau_1_ ;
          inputs1[4] = E3_overEtau_1_ ;
          inputs1[5] = a1_pi0_dEta_timesEtau_1_ ;
          inputs1[6] = a1_pi0_dphi_timesEtau_1_ ;
          inputs1[7] = h1_h2_dphi_timesE12_1_ ;
          inputs1[8] = h1_h2_dEta_timesE12_1_ ;
          inputs1[9] = h1_h3_dphi_timesE13_1_ ;
          inputs1[10] = h1_h3_dEta_timesE13_1_ ;
          inputs1[11] = h2_h3_dphi_timesE23_1_ ;
          inputs1[12] = h2_h3_dEta_timesE23_1_ ;
          inputs1[13] = gammas_dEta_tau_1_ ;
          inputs1[14] = gammas_dR_tau_1_ ;
          inputs1[15] = tau_decay_mode_1_ ;
          inputs1[16] = mass0_1_ ;
          inputs1[17] = mass1_1_ ;
          inputs1[18] = mass2_1_ ;
          inputs1[19] = E1_1_ ;
          inputs1[20] = E2_1_ ;
          inputs1[21] = E3_1_ ;
          inputs1[22] = strip_E_1_ ;
          inputs1[23] = a1_pi0_dEta_1_ ;
          inputs1[24] = a1_pi0_dphi_1_ ;
          inputs1[25] = strip_pt_1_ ;
          inputs1[26] = pt_1_ ;
          inputs1[27] = eta_1_ ;
          inputs1[28] = E_1_ ;
          inputs1[29] = h1_h2_dphi_1_ ;
          inputs1[30] = h1_h3_dphi_1_ ;
          inputs1[31] = h2_h3_dphi_1_ ;
          inputs1[32] = h1_h2_dEta_1_ ;
          inputs1[33] = h1_h3_dEta_1_ ;
          inputs1[34] = h2_h3_dEta_1_ ;
          inputs1[35] = Egamma1_1_ ;
          inputs1[36] = Egamma2_1_ ;
          inputs1[37] = gammas_dEta_1_ ;
          inputs1[38] = Mpi0_1_ ;
          inputs1[39] = Mpi0_TwoHighGammas_1_ ;
        }


        if(tau_decay_mode_1_<2) {
          std::vector<float> scores1 = read_mva_scores(isEven_,inputs1,tau1->decay_mode());
          event->Add("mvadm_other_1", scores1[0]);
          event->Add("mvadm_rho_1", scores1[1]);
          event->Add("mvadm_pi_1", scores1[2]);
          event->Add("mvadm_a1_1", scores1[3]); 
        } else if (tau_decay_mode_1_>9){
          std::vector<float> scores1 = read_mva_scores(isEven_,inputs1,tau1->decay_mode());
          event->Add("mvadm_other_1", scores1[0]);
          event->Add("mvadm_a1_1", scores1[1]);
          event->Add("mvadm_3pipi0_1", scores1[2]);
        }

 
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
