#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTStitching.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagWeight.h"
#include "TMath.h"
#include "TSystem.h"
#include "TFile.h"
#include "boost/format.hpp"

namespace ic {

  HTTStitching::HTTStitching(std::string const& name) : ModuleBase(name),
    era_(era::data_2012_rereco) {
    do_w_soup_                = false;
    do_dy_soup_               = false;
    do_dy_soup_high_mass_     = false;
    do_dy_soup_htbinned_      = false;
    do_w_soup_htbinned_      = false;
    fs_ = NULL;
  }
  HTTStitching::~HTTStitching() {
    ;
  }

  int HTTStitching::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTStitching" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << boost::format(param_fmt()) % "era"                 % Era2String(era_);
    if (do_dy_soup_ && do_dy_soup_high_mass_){
      std::cerr << "Error : need to choose one of do_dy_soup_ and do_dy_soup_high_mass_!"<<std::endl;
      throw;
    }

    if (do_w_soup_ && era_!=era::data_2015 && era_!=era::data_2016) {
      std::cout << boost::format(param_fmt()) % "make_w_soup"      % true;
      std::cout << "nInc = " << n_inc_ << std::endl;
      w1_ = (n_inc_*f1_) / ( (n_inc_*f1_) + n1_ );
      w2_ = (n_inc_*f2_) / ( (n_inc_*f2_) + n2_ );
      w3_ = (n_inc_*f3_) / ( (n_inc_*f3_) + n3_ );
      w4_ = (n_inc_*f4_) / ( (n_inc_*f4_) + n4_ );
      std::cout << boost::format("f1=%-9.2f  n1=%-9i  w1=%-9.2f \n") % f1_ % n1_ % w1_;
      std::cout << boost::format("f2=%-9.2f  n2=%-9i  w2=%-9.2f \n") % f2_ % n2_ % w2_;
      std::cout << boost::format("f3=%-9.2f  n3=%-9i  w3=%-9.2f \n") % f3_ % n3_ % w3_;
      std::cout << boost::format("f4=%-9.2f  n4=%-9i  w4=%-9.2f \n") % f4_ % n4_ % w4_;
    }
    if (do_w_soup_ && (era_ == era::data_2015 || era_ ==era::data_2016)) {
      std::cout << boost::format(param_fmt()) % "make_w_soup"      % true;
      std::cout << "nInc = " << n_inc_ << std::endl;
      f1_ = wxs1_/wxs0_;
      f2_ = wxs2_/wxs0_;
      f3_ = wxs3_/wxs0_;
      f4_ = wxs4_/wxs0_;
      w1_ = (n_inc_*f1_) / ( (n_inc_*f1_) + n1_ );
      w2_ = (n_inc_*f2_) / ( (n_inc_*f2_) + n2_ );
      w3_ = (n_inc_*f3_) / ( (n_inc_*f3_) + n3_ );
      w4_ = (n_inc_*f4_) / ( (n_inc_*f4_) + n4_ );
      std::cout << boost::format("f1=%-9.2f  n1=%-9i  w1=%-9.2f \n") % f1_ % n1_ % w1_;
      std::cout << boost::format("f2=%-9.2f  n2=%-9i  w2=%-9.2f \n") % f2_ % n2_ % w2_;
      std::cout << boost::format("f3=%-9.2f  n3=%-9i  w3=%-9.2f \n") % f3_ % n3_ % w3_;
      std::cout << boost::format("f4=%-9.2f  n4=%-9i  w4=%-9.2f \n") % f4_ % n4_ % w4_;
      if (fs_) {
        t_gen_info_ = fs_->make<TTree>("genweights", "genweights");
        t_gen_info_->Branch("decay", &t_decay_);
        t_gen_info_->Branch("mll", &t_mll_);
        t_gen_info_->Branch("njets", &t_njets_);
        t_gen_info_->Branch("wt", &t_wt_);
      }
    }
    if (do_dy_soup_ && era_!=era::data_2015 &&era_!=era::data_2016) {
      std::cout << boost::format(param_fmt()) % "make_dy_soup"      % true;
      std::cout << "nInc = " << zn_inc_ << std::endl;
      zw1_ = (zn_inc_*zf1_) / ( (zn_inc_*zf1_) + zn1_ );
      zw2_ = (zn_inc_*zf2_) / ( (zn_inc_*zf2_) + zn2_ );
      zw3_ = (zn_inc_*zf3_) / ( (zn_inc_*zf3_) + zn3_ );
      zw4_ = (zn_inc_*zf4_) / ( (zn_inc_*zf4_) + zn4_ );
      std::cout << boost::format("f1=%-9.2f  n1=%-9i  w1=%-9.2f \n") % zf1_ % zn1_ % zw1_;
      std::cout << boost::format("f2=%-9.2f  n2=%-9i  w2=%-9.2f \n") % zf2_ % zn2_ % zw2_;
      std::cout << boost::format("f3=%-9.2f  n3=%-9i  w3=%-9.2f \n") % zf3_ % zn3_ % zw3_;
      std::cout << boost::format("f4=%-9.2f  n4=%-9i  w4=%-9.2f \n") % zf4_ % zn4_ % zw4_;
    }
    if (do_dy_soup_ && (era_==era::data_2015||era_==era::data_2016)) {
      std::cout << boost::format(param_fmt()) % "make_dy_soup"      % true;
      std::cout << "nInc = " << zn_inc_ << std::endl;
      zf1_ = zxs1_/zxs0_;
      zf2_ = zxs2_/zxs0_;
      zf3_ = zxs3_/zxs0_;
      zf4_ = zxs4_/zxs0_;
      zw1_ = (zn_inc_*zf1_) / ( (zn_inc_*zf1_) + zn1_ );
      zw2_ = (zn_inc_*zf2_) / ( (zn_inc_*zf2_) + zn2_ );
      zw3_ = (zn_inc_*zf3_) / ( (zn_inc_*zf3_) + zn3_ );
      zw4_ = (zn_inc_*zf4_) / ( (zn_inc_*zf4_) + zn4_ );
      std::cout << boost::format("f1=%-9.2f  n1=%-9i  w1=%-9.2f \n") % zf1_ % zn1_ % zw1_;
      std::cout << boost::format("f2=%-9.2f  n2=%-9i  w2=%-9.2f \n") % zf2_ % zn2_ % zw2_;
      std::cout << boost::format("f3=%-9.2f  n3=%-9i  w3=%-9.2f \n") % zf3_ % zn3_ % zw3_;
      std::cout << boost::format("f4=%-9.2f  n4=%-9i  w4=%-9.2f \n") % zf4_ % zn4_ % zw4_;
      if (fs_) {
        t_gen_info_ = fs_->make<TTree>("genweights", "genweights");
        t_gen_info_->Branch("decay", &t_decay_);
        t_gen_info_->Branch("mll", &t_mll_);
        t_gen_info_->Branch("njets", &t_njets_);
        t_gen_info_->Branch("wt", &t_wt_);
      }
    }
    if (do_dy_soup_high_mass_ ) {
      std::cout << boost::format(param_fmt()) % "make_dy_soup_high_mass"      % true;
      std::cout << "nInc = " << zn_inc_ << std::endl;
      zfhm_ = zxshm_/zxsinc_;
      zfhm1_ = zxshm_/zxs1_;
      zfhm2_ = zxshm_/zxs2_;
      zfhm3_ = zxshm_/zxs3_;
      zfhm4_ = zxshm_/zxs4_;
      zf1_ = zxs1_/zxsinc_;
      zf2_ = zxs2_/zxsinc_;
      zf3_ = zxs3_/zxsinc_;
      zf4_ = zxs4_/zxsinc_;
      zw0hi_ = (zn_inc_*zfhm_) / ( (zn_inc_*zfhm_) + zn_hm_);
      zw1lo_ = (zn_inc_*zf1_) / ( (zn_inc_*zf1_) + zn1_ );
      zw1hi_ = (zn_inc_*zfhm_) / ( (zn_inc_*zfhm_) + zn_hm_ + (zn1_*zfhm1_) );
      zw2lo_ = (zn_inc_*zf2_) / ( (zn_inc_*zf2_) + zn2_ );
      zw2hi_ = (zn_inc_*zfhm_) / ( (zn_inc_*zfhm_) + zn_hm_ + (zn2_*zfhm2_));
      zw3lo_ = (zn_inc_*zf3_) / ( (zn_inc_*zf3_) + zn3_ );
      zw3hi_ = (zn_inc_*zfhm_) / ( (zn_inc_*zfhm_) + zn_hm_ + (zn3_*zfhm3_));
      zw4lo_ = (zn_inc_*zf4_) / ( (zn_inc_*zf4_) + zn4_ );
      zw4hi_ = (zn_inc_*zfhm_) / ( (zn_inc_*zfhm_) + zn_hm_ + (zn4_*zfhm4_));
      std::cout << boost::format("w0hi=%-9.5f \n") % zw0hi_;
      std::cout << boost::format("w1lo=%-9.5f \n") % zw1lo_;
      std::cout << boost::format("w1hi=%-9.5f \n") % zw1hi_;
      std::cout << boost::format("w2lo=%-9.5f \n") % zw2lo_;
      std::cout << boost::format("w2hi=%-9.5f \n") % zw2hi_;
      std::cout << boost::format("w3lo=%-9.5f \n") % zw3lo_;
      std::cout << boost::format("w3hi=%-9.5f \n") % zw3hi_;
      std::cout << boost::format("w4lo=%-9.5f \n") % zw4lo_;
      std::cout << boost::format("w4hi=%-9.5f \n") % zw4hi_;

      if (fs_) {
        t_gen_info_ = fs_->make<TTree>("genweights", "genweights");
        t_gen_info_->Branch("decay", &t_decay_);
        t_gen_info_->Branch("mll", &t_mll_);
        t_gen_info_->Branch("njets", &t_njets_);
        t_gen_info_->Branch("wt", &t_wt_);
      }
    }
    if (do_w_soup_htbinned_ ) {
      std::cout << boost::format(param_fmt()) % "make_w_soup"      % true;
      f1_ = wxs1_/wxs0_;
      f2_ = wxs2_/wxs0_;
      f3_ = wxs3_/wxs0_;
      f4_ = wxs4_/wxs0_;
      w1_ = (n_inc_*f1_) / ((n_inc_*f1_) + n1_);
      w2_ = (n_inc_*f2_) / ((n_inc_*f2_) + n2_);
      w3_ = (n_inc_*f3_) / ((n_inc_*f3_) + n3_);
      w4_ = (n_inc_*f4_) / ((n_inc_*f4_) + n4_);
      std::cout << boost::format("f 100<ht<200=%-9.5f  n 100<ht<200=%-9i  w 100<ht<200=%-9.5f \n") % f1_ % n1_ % w1_;
      std::cout << boost::format("f 200<ht<400=%-9.5f  n 200<ht<400=%-9i  w 200<ht<400=%-9.5f \n") % f2_ % n2_ % w2_;
      std::cout << boost::format("f 400<ht<600=%-9.5f  n 400<ht<600=%-9i  w 400<ht<600=%-9.5f \n") % f3_ % n3_ % w3_;
      std::cout << boost::format("f ht>600=%-9.5f  n ht>600=%-9i  w ht>600=%-9.5f \n") % f4_ % n4_ % w4_;
    }

    if (do_dy_soup_htbinned_ ) {
      std::cout << boost::format(param_fmt()) % "make_dy_soup"      % true;
      zf1_ = zxs1_/zxs0_;
      zf2_ = zxs2_/zxs0_;
      zf3_ = zxs3_/zxs0_;
      zf4_ = zxs4_/zxs0_;
      zw1_ = (zn_inc_*zf1_) / ( (zn_inc_*zf1_) + zn1_ );
      zw2_ = (zn_inc_*zf2_) / ( (zn_inc_*zf2_) + zn2_ );
      zw3_ = (zn_inc_*zf3_) / ( (zn_inc_*zf3_) + zn3_ );
      zw4_ = (zn_inc_*zf4_) / ( (zn_inc_*zf4_) + zn4_ );
      std::cout << boost::format("f 100<ht<200=%-9.5f  n 100<ht<200=%-9i  w 100<ht<200=%-9.5f \n") % zf1_ % zn1_ % zw1_;
      std::cout << boost::format("f 200<ht<400=%-9.5f  n 200<ht<400=%-9i  w 200<ht<400=%-9.5f \n") % zf2_ % zn2_ % zw2_;
      std::cout << boost::format("f 400<ht<600=%-9.5f  n 400<ht<600=%-9i  w 400<ht<600=%-9.5f \n") % zf3_ % zn3_ % zw3_;
      std::cout << boost::format("f ht>600=%-9.5f  n ht>600=%-9i  w ht>600=%-9.5f \n") % zf4_ % zn4_ % zw4_;
    }

    return 0;
  }

  int HTTStitching::Execute(TreeEvent *event) {

    std::vector<CompositeCandidate *> dilepton;

    EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");

    if (do_w_soup_) {
      unsigned partons = 0;
      double gen_mll = 0;
      if(era_ != era::data_2015 && era_ != era::data_2016){
        std::vector<GenParticle*> const& parts = event->GetPtrVec<GenParticle>("genParticles");
        bool count_jets = false;
        for (unsigned i = 0; i < parts.size(); ++i) {
          if (parts[i]->status() != 3) continue;
          unsigned id = abs(parts[i]->pdgid());
          if (count_jets) { 
            if (id == 1 || id == 2 || id == 3 || id == 4 || id == 5 || id == 6 || id == 21) partons++;
          }
          if (id == 24) count_jets = true; 
        }
      } else if(era_ == era::data_2015 || era_ == era::data_2016) {
        std::vector<GenParticle*> const& lhe_parts = event->GetPtrVec<GenParticle>("lheParticles");
        std::vector<GenParticle*> zll_cands;
        for(unsigned i = 0; i< lhe_parts.size(); ++i){
         if(lhe_parts[i]->status() != 1) continue;
         unsigned id = abs(lhe_parts[i]->pdgid());
         if ((id >= 1 && id <=6) || id == 21) partons++;
        }
          t_mll_ = gen_mll;
          t_decay_ = 0;  // ee, mumu, tautau
      }
      if (partons > 4) {
        std::cerr << "Error making soup, event has " << partons << " partons!" << std::endl;
        throw;
      }
      if (partons == 1) eventInfo->set_weight("wsoup", w1_);
      if (partons == 2) eventInfo->set_weight("wsoup", w2_);
      if (partons == 3) eventInfo->set_weight("wsoup", w3_);
      if (partons == 4) eventInfo->set_weight("wsoup", w4_);
      t_njets_ = partons;

      t_wt_ = eventInfo->weight_defined("wsoup") ? eventInfo->weight("wsoup") : 1.;
      if(fs_) t_gen_info_->Fill();
    }

    if (do_dy_soup_) {
      unsigned partons = 0;
      double gen_mll = 0;
      if(era_ != era::data_2015&&era_!=era::data_2016){
        std::vector<GenParticle*> const& parts = event->GetPtrVec<GenParticle>("genParticles");
        bool count_jets = false;
      
        for (unsigned i = 0; i < parts.size(); ++i) {
          // std::cout << i << "\t" << parts[i]->status() << "\t" << parts[i]->pdgid() << "\t" << parts[i]->vector() << std::endl;
          if (parts[i]->status() != 3) continue;
          unsigned id = abs(parts[i]->pdgid());
          if (count_jets) { 
            if (id == 1 || id == 2 || id == 3 || id == 4 || id == 5 || id == 6 || id == 21) partons++;
          }
          if (id == 23) count_jets = true; 
        }
      } else if(era_ == era::data_2015 || era_ == era::data_2016){ 
        std::vector<GenParticle*> const& lhe_parts = event->GetPtrVec<GenParticle>("lheParticles");
        std::vector<GenParticle*> zll_cands;
        for(unsigned i = 0; i< lhe_parts.size(); ++i){
         if(lhe_parts[i]->status() != 1) continue;
         unsigned id = abs(lhe_parts[i]->pdgid());
         if ((id >= 1 && id <=6) || id == 21) partons++;
         if (id == 11|| id ==13 || id ==15) zll_cands.push_back(lhe_parts[i]);
        }
        if(zll_cands.size() == 2){
          gen_mll = (zll_cands[0]->vector()+zll_cands[1]->vector()).M();
          t_mll_ = gen_mll;
        } else {
          std::cerr << "Error making soup, event has " << zll_cands.size() << " Z->ll candidates, 2 expected!" <<std::endl;
          throw;
        }
        t_decay_ = 0;  // ee, mumu, tautau
        if (std::abs(zll_cands[0]->pdgid()) == 11 &&
            std::abs(zll_cands[1]->pdgid()) == 11) {
          t_decay_ = 0;
        } else if (std::abs(zll_cands[0]->pdgid()) == 13 &&
                   std::abs(zll_cands[1]->pdgid()) == 13) {
          t_decay_ = 1;
        } else if (std::abs(zll_cands[0]->pdgid()) == 15 &&
                   std::abs(zll_cands[1]->pdgid()) == 15) {
          t_decay_ = 2;
        } else {
          std::cerr << "Error!\n";
          throw;
        }
      }
      if (partons > 4) {
        std::cerr << "Error making soup, event has " << partons << " partons!" << std::endl;
        throw;
      }
      if (partons == 1) eventInfo->set_weight("dysoup", zw1_);
      if (partons == 2) eventInfo->set_weight("dysoup", zw2_);
      if (partons == 3) eventInfo->set_weight("dysoup", zw3_);
      if (partons == 4) eventInfo->set_weight("dysoup", zw4_);
      t_njets_ = partons;

      t_wt_ = eventInfo->weight_defined("dysoup") ? eventInfo->weight("dysoup") : 1.;
      if(fs_) t_gen_info_->Fill();

      // unsigned gen_match_1 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
    }


    if (do_dy_soup_high_mass_) {
        unsigned partons = 0; 
        double gen_mll = 0;
        std::vector<GenParticle*> const& lhe_parts = event->GetPtrVec<GenParticle>("lheParticles");
        std::vector<GenParticle*> zll_cands;
        for(unsigned i = 0; i< lhe_parts.size(); ++i){
         if(lhe_parts[i]->status() != 1) continue;
         unsigned id = abs(lhe_parts[i]->pdgid());
         if ((id >= 1 && id <=6) || id == 21) partons++;
         if (id == 11|| id ==13 || id ==15) zll_cands.push_back(lhe_parts[i]);
        }
        if(zll_cands.size() == 2){
         gen_mll = (zll_cands[0]->vector()+zll_cands[1]->vector()).M();
         t_mll_ = gen_mll;
        } else {
          std::cerr << "Error making soup, event has " << zll_cands.size() << " Z->ll candidates, 2 expected!" <<std::endl;
          throw;
        } 
      if (partons > 4) {
        std::cerr << "Error making soup, event has " << partons << " partons!" << std::endl;
        throw;
      }
      t_njets_ = partons;

      t_decay_ = 0;  // ee, mumu, tautau
      if (std::abs(zll_cands[0]->pdgid()) == 11 &&
          std::abs(zll_cands[1]->pdgid()) == 11) {
        t_decay_ = 0;
      } else if (std::abs(zll_cands[0]->pdgid()) == 13 &&
                 std::abs(zll_cands[1]->pdgid()) == 13) {
        t_decay_ = 1;
      } else if (std::abs(zll_cands[0]->pdgid()) == 15 &&
                 std::abs(zll_cands[1]->pdgid()) == 15) {
        t_decay_ = 2;
      } else {
        std::cerr << "Error!\n";
        throw;
      }

      // unsigned gen_match_1 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
      bool is_ztt = (t_decay_ == 2);
      // if(gen_match_1 < 3) is_ztt=false;
      if (partons == 0 && gen_mll > 150 && is_ztt) eventInfo->set_weight("dysoup",zw0hi_);
      if (partons == 1 && gen_mll <= 150) eventInfo->set_weight("dysoup", zw1lo_);
      if (partons == 1 && gen_mll > 150 && !is_ztt) eventInfo->set_weight("dysoup", zw1lo_);
      if (partons == 1 && gen_mll > 150 && is_ztt) eventInfo->set_weight("dysoup", zw1hi_);
      if (partons == 2 && gen_mll <= 150) eventInfo->set_weight("dysoup", zw2lo_);
      if (partons == 2 && gen_mll > 150 && !is_ztt) eventInfo->set_weight("dysoup", zw2lo_);
      if (partons == 2 && gen_mll > 150 && is_ztt) eventInfo->set_weight("dysoup", zw2hi_);
      if (partons == 3 && gen_mll <= 150) eventInfo->set_weight("dysoup", zw3lo_);
      if (partons == 3 && gen_mll > 150 && !is_ztt) eventInfo->set_weight("dysoup", zw3lo_);
      if (partons == 3 && gen_mll > 150 && is_ztt) eventInfo->set_weight("dysoup", zw3hi_);
      if (partons == 4 && gen_mll <= 150) eventInfo->set_weight("dysoup", zw4lo_);
      if (partons == 4 && gen_mll > 150 && !is_ztt) eventInfo->set_weight("dysoup", zw4lo_);
      if (partons == 4 && gen_mll > 150 && is_ztt) eventInfo->set_weight("dysoup", zw4hi_);
      t_wt_ = eventInfo->weight_defined("dysoup") ? eventInfo->weight("dysoup") : 1.;
      if(fs_) t_gen_info_->Fill();
    }

   if (do_w_soup_htbinned_){
     double gen_ht = eventInfo->gen_ht() ;
     if (100 <= gen_ht&&gen_ht <200) eventInfo->set_weight("wsoup", w1_);
     if (200 <= gen_ht&&gen_ht <400) eventInfo->set_weight("wsoup", w2_);
     if (400 <= gen_ht &&gen_ht<600) eventInfo->set_weight("wsoup", w3_);
     if (gen_ht >= 600) eventInfo->set_weight("wsoup", w4_);
   }


   if (do_dy_soup_htbinned_){
     double gen_ht = eventInfo->gen_ht() ;
     if (100 <= gen_ht&&gen_ht <200) eventInfo->set_weight("dysoup", zw1_);
     if (200 <= gen_ht&&gen_ht <400) eventInfo->set_weight("dysoup", zw2_);
     if (400 <= gen_ht &&gen_ht<600) eventInfo->set_weight("dysoup", zw3_);
     if (gen_ht >= 600) eventInfo->set_weight("dysoup", zw4_);
   }

    return 0;
  }

  int HTTStitching::PostAnalysis() {
    return 0;
  }

  void HTTStitching::PrintInfo() {
    ;
  }

  void HTTStitching::SetWTargetFractions(double f0, double f1, double f2, double f3, double f4) {
    f0_ = f0;
    f1_ = f1;
    f2_ = f2;
    f3_ = f3;
    f4_ = f4;

  }
  void HTTStitching::SetWInputYields(double n_inc, double n1, double n2, double n3, double n4) {
    n_inc_ = n_inc;
    n1_ = n1;
    n2_ = n2;
    n3_ = n3;
    n4_ = n4;
  }

  void HTTStitching::SetDYTargetFractions(double zf0, double zf1, double zf2, double zf3, double zf4) {
    zf0_ = zf0;
    zf1_ = zf1;
    zf2_ = zf2;
    zf3_ = zf3;
    zf4_ = zf4;

  }
  void HTTStitching::SetDYInputYields(double zn_inc, double zn1, double zn2, double zn3, double zn4) {
    zn_inc_ = zn_inc;
    zn1_ = zn1;
    zn2_ = zn2;
    zn3_ = zn3;
    zn4_ = zn4;
  }

  void HTTStitching::SetDYInputYieldsHighMass(double zn_inc, double zn1, double zn2, double zn3, double zn4, double zn_hm) {
    zn_inc_ = zn_inc;
    zn1_ = zn1;
    zn2_ = zn2;
    zn3_ = zn3;
    zn4_ = zn4;
    zn_hm_ = zn_hm;
  }

  void HTTStitching::SetDYInputCrossSections(double zxs0, double zxs1, double zxs2, double zxs3, double zxs4) {
    zxs0_ = zxs0;
    zxs1_ = zxs1;
    zxs2_ = zxs2;
    zxs3_ = zxs3;
    zxs4_ = zxs4;
  }


  void HTTStitching::SetDYInputCrossSectionsHighMass(double zxsinc, double zxs1, double zxs2, double zxs3, double zxs4, double zxshm) {
    zxsinc_ = zxsinc;
    zxs1_ = zxs1;
    zxs2_ = zxs2;
    zxs3_ = zxs3;
    zxs4_ = zxs4;
    zxshm_ = zxshm;
  }

  void HTTStitching::SetWInputCrossSections(double wxs0, double wxs1, double wxs2, double wxs3, double wxs4) {
    wxs0_ = wxs0;
    wxs1_ = wxs1;
    wxs2_ = wxs2;
    wxs3_ = wxs3;
    wxs4_ = wxs4;
  }

}
