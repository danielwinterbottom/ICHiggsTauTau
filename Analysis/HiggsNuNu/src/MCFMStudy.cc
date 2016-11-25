#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/MCFMStudy.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"

namespace ic {

  MCFMStudy::MCFMStudy(std::string const& name,
			 unsigned flavour,
			 double minMll, 
			 double maxMll) : ModuleBase(name) {
    flavour_ = flavour;
    minMll_ = minMll;
    maxMll_ = maxMll;
    countStatus3_ = 0;

    nInMassRange_ = 0;
    nZjj_ = 0;
    nZjj_VBF_ = 0;
    nZjj_VBF_MET_ = 0;
    nZjj_VBF_MET_DPhi_ = 0;
    nZmm_ = 0;
    nZjjmm_ = 0;
    nZjjmm_VBF_ = 0;
    nZjjmm_VBF_MET_ = 0;
    nZjjmm_VBF_MET_DPhi_ = 0;

  }

  MCFMStudy::~MCFMStudy() {
    ;
  }

  int MCFMStudy::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for MCFMStudy" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Flavour: " << flavour_ << std::endl;
    std::cout << " Mass range: " << minMll_ << "-" << maxMll_ << std::endl;

    countStatus3_ = 0;
    nInMassRange_ = 0;
    nZjj_ = 0;
    nZjj_VBF_ = 0;
    nZjj_VBF_MET_ = 0;
    nZjj_VBF_MET_DPhi_ = 0;
    nZmm_ = 0;
    nZjjmm_noDR_ = 0;
    nZjjmm_ = 0;
    nZjjmm_VBF_ = 0;
    nZjjmm_VBF_MET_ = 0;
    nZjjmm_VBF_MET_DPhi_ = 0;

    return 0;
  }

  int MCFMStudy::Execute(TreeEvent *event) {

    std::vector<GenParticle*> const& parts = event->GetPtrVec<GenParticle>("genParticles");

    GenParticle* lepplus = 0;
    GenParticle* lepminus = 0;

    std::vector<GenParticle*> partons;
    partons.clear();

    for (unsigned i = 0; i < parts.size(); ++i) {
      if (parts[i]->status() != 3) continue;

      int id = parts[i]->pdgid();

      ////if e or mu found and e or mu channel asked, just pass
      if (id == static_cast<int>(flavour_)) {
	//std::cout << " -- Found status 3 id " << id << ". Keeping event." << std::endl;
	countStatus3_++;
	lepplus = parts[i];
      }
      if (id == static_cast<int>(-flavour_)) {
	//std::cout << " -- Found status 3 id " << id << ". Keeping event." << std::endl;
	countStatus3_++;
	lepminus = parts[i];
      }

      //get the partons
      unsigned idP = abs(parts[i]->pdgid());
      if (idP == 1 || idP == 2 || idP == 3 || idP == 4 || idP == 5 || idP == 6 || idP == 21) {
	partons.push_back(parts[i]);
      }
    }//loop on genparticles
   
    if (lepplus && lepminus) {
      double mass = (lepplus->vector()+lepminus->vector()).M();
      //std::cout << " -- Found two leptons with mass = " << mass << std::endl;
      if (mass > minMll_ && mass < maxMll_){

	nInMassRange_++;
	bool ismm = false;
	double dRll = ROOT::Math::VectorUtil::DeltaR(lepplus->vector(),lepminus->vector());
	if (lepplus->pt() > 20 && fabs(lepplus->eta()) < 2.1 &&
	    lepminus->pt() > 20 && fabs(lepminus->eta()) < 2.1 &&
	    dRll > 0.2) {
	  ismm = true;
	  nZmm_++;
	}

	//apply jet cuts
	std::sort(partons.begin(), partons.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
	if (partons.size() < 2) return 1;
	if (partons[0]->pt() > 50 && fabs(partons[0]->eta()) < 4.7 &&
	    partons[1]->pt() > 50 && fabs(partons[1]->eta()) < 4.7){
	  nZjj_++;

	  double dRpj1 = ROOT::Math::VectorUtil::DeltaR(lepplus->vector(),partons[0]->vector());
	  double dRpj2 = ROOT::Math::VectorUtil::DeltaR(lepplus->vector(),partons[1]->vector());
	  double dRej1 = ROOT::Math::VectorUtil::DeltaR(lepminus->vector(),partons[0]->vector());
	  double dRej2 = ROOT::Math::VectorUtil::DeltaR(lepminus->vector(),partons[1]->vector());

	  if (ismm) nZjjmm_noDR_++;

	  ismm = ismm && dRpj1>0.6 && dRpj2 > 0.6 && dRej1 > 0.6 && dRej2 > 0.6;

	  if (ismm) nZjjmm_++;

	  //apply VBF cuts
	  ROOT::Math::PtEtaPhiEVector dijet = partons[0]->vector()+partons[1]->vector();
	  double detajj = fabs(partons[0]->eta() - partons[1]->eta());
	  double dphijj = fabs(ROOT::Math::VectorUtil::DeltaPhi(partons[0]->vector(),partons[1]->vector()));
	  if (dijet.M() > 1100 &&
	      detajj > 4.2 &&
	      partons[0]->eta()*partons[1]->eta()<0
	      ) {

	    nZjj_VBF_++;
	    if (ismm) nZjjmm_VBF_++;


	  //apply MET cut (pT(ll))
	    ROOT::Math::PtEtaPhiEVector Z = lepplus->vector()+lepminus->vector();
	    if (Z.Pt() > 130){

	      nZjj_VBF_MET_++;
	      if (ismm) nZjjmm_VBF_MET_++;
	      

	      //apply Dphi_jj cut
	      if (dphijj < 1.0) {

		nZjj_VBF_MET_DPhi_++;
		if (ismm) nZjjmm_VBF_MET_DPhi_++;

	      }//dphi cut

	    }//MET cut

	  }//VBF cuts

	}//2jets

      }//Mll

    }//2 muons
 
    return 0;
  }

  int MCFMStudy::PostAnalysis() {

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PostAnalysis Info for MCFMStudy" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << " -- Number of Z->ll: " << countStatus3_/2. << std::endl;


    std::cout << "nInMassRange[] = " << nInMassRange_ << ";" << std::endl;
    std::cout << "nZjj[] = " << nZjj_ << ";" << std::endl;
    std::cout << "nZjj_VBF[] = " << nZjj_VBF_ << ";" << std::endl;
    std::cout << "nZjj_VBF_MET[] = " << nZjj_VBF_MET_ << ";" << std::endl;
    std::cout << "nZjj_VBF_MET_DPhi[] = " << nZjj_VBF_MET_DPhi_ << ";" << std::endl;
    std::cout << "nZmm[] = " << nZmm_ << ";" << std::endl;
    std::cout << "nZjjmm_noDR[] = " << nZjjmm_noDR_ << ";" << std::endl;
    std::cout << "nZjjmm[] = " << nZjjmm_ << ";" << std::endl;
    std::cout << "nZjjmm_VBF[] = " << nZjjmm_VBF_ << ";" << std::endl;
    std::cout << "nZjjmm_VBF_MET[] = " << nZjjmm_VBF_MET_ << ";" << std::endl;
    std::cout << "nZjjmm_VBF_MET_DPhi[] = " << nZjjmm_VBF_MET_DPhi_ << ";" << std::endl;

    return 0;
  }

  void MCFMStudy::PrintInfo() {
    ;
  }

}//namespace
