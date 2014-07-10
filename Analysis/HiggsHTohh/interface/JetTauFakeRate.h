#ifndef ICHiggsTauTau_HiggsHTohh_JetTauFakeRate_h
#define ICHiggsTauTau_HiggsHTohh_JetTauFakeRate_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"

#include <string>

namespace ic {

	class JetTauFakeRate : public ModuleBase {
		private:

			CLASS_MEMBER(JetTauFakeRate, bool, write_tree)
				CLASS_MEMBER(JetTauFakeRate, bool, write_plots)
				CLASS_MEMBER(JetTauFakeRate, fwlite::TFileService*, fs)

				TTree *outtree_;
			TH1F* jetpt_;
			TH1F* jeteta_;
			TH1F* jetflavour_;
			TH1F* jetphi_;
			TH1F* jetpt_c_;
			TH1F* jeteta_c_;
			TH1F* jetphi_c_;
			TH1F* jetpt_b_;
			TH1F* jeteta_b_;
			TH1F* jetphi_b_;
			TH1F* jetpt_g_;
			TH1F* jeteta_g_;
			TH1F* jetphi_g_;
			TH1F* jetpt_light_;
			TH1F* jeteta_light_;
			TH1F* jetphi_light_;
			TH1F* jetpt_undef_;
			TH1F* jeteta_undef_;
			TH1F* jetphi_undef_;
			TH1F* taupt_;
			TH1F* taueta_;
			TH1F* tauphi_;
			TH1F* dm_taupt_;
			TH1F* dm_taueta_;
			TH1F* dm_tauphi_;
			TH1F* loose_taupt_;
			TH1F* loose_taueta_;
			TH1F* loose_tauphi_;

			TH1F* medium_taupt_;
			TH1F* medium_taueta_;
			TH1F* medium_tauphi_;
			TH1F* tight_taupt_;
			TH1F* tight_taueta_;
			TH1F* tight_tauphi_;
			TH1F* deltar_;
			TH1F* dm_deltar_;
			TH1F* loose_deltar_;
			TH1F* medium_deltar_;
			TH1F* tight_deltar_;
			TH1F* taupt_match_;
			TH1F* taueta_match_;
			TH1F* tauphi_match_;
			TH1F* dm_taupt_match_;
			TH1F* dm_taueta_match_;
			TH1F* dm_tauphi_match_;
			TH1F* loose_taupt_match_;
			TH1F* loose_taueta_match_;
			TH1F* loose_tauphi_match_;
			TH1F* loose_taupt_match_b_;
			TH1F* loose_taueta_match_b_;
			TH1F* loose_tauphi_match_b_;
			TH1F* loose_taupt_match_c_;
			TH1F* loose_taueta_match_c_;
			TH1F* loose_tauphi_match_c_;
			TH1F* loose_taupt_match_g_;
			TH1F* loose_taueta_match_g_;
			TH1F* loose_tauphi_match_g_;
			TH1F* loose_taupt_match_light_;
			TH1F* loose_taueta_match_light_;
			TH1F* loose_tauphi_match_light_;
			TH1F* loose_taupt_match_undef_;
			TH1F* loose_taueta_match_undef_;
			TH1F* loose_tauphi_match_undef_;

			TH1F* medium_taupt_match_;
			TH1F* medium_taueta_match_;
			TH1F* medium_tauphi_match_;
			TH1F* tight_taupt_match_;
			TH1F* tight_taueta_match_;
			TH1F* tight_tauphi_match_;
			TH1F* taunvtx_;
			TH1F* jetnvtx_;
			TH1F* dm_taunvtx_;
			TH1F* loose_taunvtx_;
			TH1F* medium_taunvtx_;
			TH1F* tight_taunvtx_;
			TH1F* genjetpt_;

			double jpt_;     
			double jeta_;     
			double deltaR_;
			double deltaR_calc_;
			std::pair <double,unsigned int> deltaR_pair_;
			int nvtx_;
			int ntaus_;
			int njets_;
			int ntaus_dm_;
			int ntaus_loose_;
			int ntaus_medium_;
			int ntaus_tight_;
			int thetaun;
			double theDR;

		public:
			JetTauFakeRate(std::string const& name);
			virtual ~JetTauFakeRate();

			virtual int PreAnalysis();
			virtual int Execute(TreeEvent *event);
			std::pair<double,unsigned int> GetDeltaR(PFJet *tau_sig,std::vector<Tau*> jet_vector);
			virtual int PostAnalysis();
			virtual void PrintInfo();

	};

}


#endif
