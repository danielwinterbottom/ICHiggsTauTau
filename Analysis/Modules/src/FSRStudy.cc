#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/FSRStudy.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/PileupInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagWeight.h"

#include "TEfficiency.h"


#include "boost/bind.hpp"


namespace ic {
  

  FSRStudy::FSRStudy(std::string const& name) : ModuleBase(name) {
    st1_eta_ = 2.1;
    st1_pt1_ = 20.0;
    st1_pt2_ = 20.0;
  }

  FSRStudy::~FSRStudy() {
    ;
  }

  int FSRStudy::PreAnalysis() {
    if (fs_) {
      hists_ = new DynamicHistoSet(fs_->mkdir("fsr"));
      hists_->Create("status_3", 240, 60, 120);
      hists_->Create("status_1", 240, 60, 120);
    }
    return 0;
  }

  int FSRStudy::Execute(TreeEvent *event) {
        using boost::bind;

    //EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");
    std::vector<GenParticle *> gen_particles = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<GenParticle *> gen_leptons = gen_particles;
    int decay_flav = 13;
    erase_if(gen_leptons, !((bind(&GenParticle::status, _1) == 3) && ((bind(&GenParticle::pdgid, _1) == decay_flav)||(bind(&GenParticle::pdgid, _1) == -decay_flav))));
    if (gen_leptons.size() != 2) return 0;
    double status3mass = (gen_leptons[0]->vector() + gen_leptons[1]->vector()).M();


    std::vector<GenParticle *> st1_1 = gen_particles;
    std::vector<GenParticle *> st1_2 = gen_particles;
    erase_if(st1_1, !(bind(MinPtMaxEta, _1, st1_pt1_, st1_eta_) && bind(&GenParticle::status, _1) == 1) );
    erase_if(st1_2, !(bind(MinPtMaxEta, _1, st1_pt2_, st1_eta_) && bind(&GenParticle::status, _1) == 1) );
    std::vector< std::pair<GenParticle*,GenParticle*> > pairs = MakePairs(st1_1, st1_2);
    std::vector<CompositeCandidate *> vec_out (pairs.size(), 0);

    for (unsigned i = 0; i < pairs.size(); ++i) {
      vec_out[i] = new CompositeCandidate();
      vec_out[i]->AddCandidate("l1", pairs[i].first);
      vec_out[i]->AddCandidate("l2", pairs[i].second);
    }
    erase_if(vec_out, !boost::bind(PairMassInRange, _1, 60, 120));
    erase_if(vec_out, !boost::bind(PairOppSign, _1));
    //std::cout << vec_out.size() << std::endl;

    if (vec_out.size() > 1) {
      double status1mass = vec_out[0]->M();
      hists_->Fill("status_1", status1mass);
      hists_->Fill("status_3", status3mass);
    }





    return 0;
  }





  int FSRStudy::PostAnalysis() {
    return 0;
  }

  void FSRStudy::PrintInfo() {
    ;
  }

}

