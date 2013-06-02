#ifndef ICHiggsTauTau_Utilities_HTTPlots_h
#define ICHiggsTauTau_Utilities_HTTPlots_h

#include "TH1F.h"
#include "TH2F.h"

#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"

namespace ic {

struct MassPlots {
	TH1F *m_sv;
	TH1F *m_sv_sm;
	TH1F *m_sv_sm_fine;
	TH1F *m_sv_mssm;
	TH1F *m_sv_mssm_fine;
	TH1F *m_vis;
	TH1F *m_vis_sm;
	TH1F *m_vis_sm_fine;
	TH1F *m_vis_mssm;
	TH1F *m_vis_mssm_fine;
	MassPlots(TFileDirectory const& dir);
};

struct CoreControlPlots {
	TH1F *n_vtx;
	TH1F *mt_1;
	TH1F *pzeta;
	TH1F *pt_h;
	TH1F *pt_tt;
	TH1F *pt_1;
	TH1F *pt_2;
	TH1F *eta_1;
	TH1F *eta_2;
	TH1F *m_2;
	TH1F *met;
	TH1F *met_phi;
	TH1F *n_jets;
	TH1F *n_bjets;
	TH1F *n_jetsingap;
	TH1F *jpt_1;
	TH1F *jpt_2;
	TH1F *jeta_1;
	TH1F *jeta_2;
	TH1F *bpt_1;
	TH1F *beta_1;
	TH1F *bcsv_1;
	TH1F *mjj;
	TH1F *jdeta;
	TH1F *tau_decay_mode;
	TH1F *l1_met;
	TH1F *calo_nohf_met;
	CoreControlPlots(TFileDirectory const& dir);
};

}


#endif
