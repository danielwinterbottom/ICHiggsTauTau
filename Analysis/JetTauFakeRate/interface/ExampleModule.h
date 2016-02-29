#ifndef ICHiggsTauTau_JetTauFakeRate_ExampleModule_h
#define ICHiggsTauTau_JetTauFakeRate_ExampleModule_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "TH2.h"

#include <string>

namespace ic {

	class ExampleModule : public ModuleBase {
		private:

			CLASS_MEMBER(ExampleModule, bool, write_tree)
			CLASS_MEMBER(ExampleModule, bool, write_plots)
			CLASS_MEMBER(ExampleModule, bool, by_decay_mode)
			CLASS_MEMBER(ExampleModule, bool, by_jet_type)
			CLASS_MEMBER(ExampleModule, bool, sync_mode)
			CLASS_MEMBER(ExampleModule, bool, debug_mode)
			CLASS_MEMBER(ExampleModule, bool, wjets_mode)
			CLASS_MEMBER(ExampleModule, fwlite::TFileService*, fs)

      std::map<std::string,TH1F*> jettype_histos_;
			std::map<std::string,TH1F*> standard_tau_histos_;
				std::map<std::string,TH1F*> tau_jettype_histos_;
				std::map<std::string,TH1F*> dm_tau_histos_;

				TTree *outtree_;
/*			TH1F* jetpt_;
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
			TH1F* leading_trackpt_;
			TH1F* jetpt_puid_rej_;
			TH1F* jetpt_dz_and_puid_rej_;
			*/

			

			double jetpt_;     
			double jeteta_;     
			double jetphi_;
			double taupt_;
			double taueta_;
			double tauphi_;
			int jet_flavour_;
			double taudmpass_;
			double tauloose_;
			double taumedium_;
			double tautight_;
			int taudm_;
			int nvtx_;

		public:
			ExampleModule(std::string const& name);
			virtual ~ExampleModule();

			virtual int PreAnalysis();
			virtual int Execute(TreeEvent *event);
			virtual int PostAnalysis();
			virtual void PrintInfo();

	};

}


#endif
