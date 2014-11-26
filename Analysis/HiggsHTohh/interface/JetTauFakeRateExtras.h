#ifndef ICHiggsTauTau_HiggsHTohh_JetTauFakeRateExtras_h
#define ICHiggsTauTau_HiggsHTohh_JetTauFakeRateExtras_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "TH2.h"

#include <string>

namespace ic {

	class JetTauFakeRateExtras : public ModuleBase {
		private:

			CLASS_MEMBER(JetTauFakeRateExtras, bool, write_tree)
			CLASS_MEMBER(JetTauFakeRateExtras, bool, write_plots)
			CLASS_MEMBER(JetTauFakeRateExtras, bool, by_decay_mode)
			CLASS_MEMBER(JetTauFakeRateExtras, bool, by_jet_type)
			CLASS_MEMBER(JetTauFakeRateExtras, bool, sync_mode)
			CLASS_MEMBER(JetTauFakeRateExtras, bool, debug_mode)
			CLASS_MEMBER(JetTauFakeRateExtras, bool, wjets_mode)
			CLASS_MEMBER(JetTauFakeRateExtras, fwlite::TFileService*, fs)

      std::map<std::string,TH1F*> jettype_histos_;
			std::map<std::string,TH1F*> standard_tau_histos_;
			std::map<std::string,TH1F*> tau_jettype_histos_;
			std::map<std::string,TH1F*> dm_tau_histos_;
			std::string sumet_string;
			std::string sumpt_string;
			std::string nvtx_string;
			std::string ntracks_string;


			TTree *outtree_;
			TH1F* sum_et_;
			TH1F* sum_pt_;
			TH1F* jetpt_;
			TH1F* histntracks_;
			TH1F* jeteta_;
			TH1F* jetflavour_;
			TH1F* jetphi_;
			TH1F* jetnvtx_;
					
			TH1F* taupt_dm_iso_nomatch_;
			TH1F* taueta_dm_iso_nomatch_;
			TH1F* tauphi_dm_iso_nomatch_;
		
		  TH1F* genjetpt_;
			TH1F* genjeteta_;
			TH1F* genjetpt_taupt_200_300_;
			TH1F* genjetpt_taupt_300_400_;
			TH1F* genjetpt_taupt_400_;
			TH1F* genjetpt_jetpt_200_300_;
			TH1F* genjetpt_jetpt_300_400_;
			TH1F* genjetpt_jetpt_400_;
			TH1F* taupt_test_hist_;
			TH1F* jets_dz_;
			TH1F* jets_pu_;
			TH1F* jets_overlap_;
			TH1F* jetpt_dz_rej_;
			TH1F* jetpt_puid_rej_;
			TH1F* jetpt_dz_and_puid_rej_;
			TH1F* loose_taupt_match_;

			

			double jpt_;     
			double sumet_;
			double sumpt_;
			int ntracks_;
			double jeta_;     
			double deltaR_;
			double deltaR_calc_;
			int nvtx_;
			int ntaus_;
			int nmuons_;
			int njets_;
			int ntaus_dm_;
			int ntaus_loose_;
			int ntaus_medium_;
			int ntaus_tight_;
			int thetaun;
			int thetrackid;
			std::vector<int> mothers_;
			double theDR;
			double theDRgj;
			int thegenjetn;

		public:
			JetTauFakeRateExtras(std::string const& name);
			virtual ~JetTauFakeRateExtras();

			virtual int PreAnalysis();
			virtual int Execute(TreeEvent *event);
			virtual int PostAnalysis();
			virtual void PrintInfo();

	};

}


#endif
