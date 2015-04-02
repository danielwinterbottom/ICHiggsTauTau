#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HhhMTMVABoth.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "TMVA/Reader.h"
#include "Math/VectorUtil.h"
#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "Core/interface/TreeEvent.h"
#include <vector>

namespace ic {

	HhhMTMVABoth::HhhMTMVABoth(std::string const& name) : ModuleBase(name) {
		ditau_label_ = "emtauCandidates";
		met_label_ = "pfMVAMet";
//		gf_mva_file_bdt_ = "data/Hhh_mva/2jetinclusive_leppt_BDT.weights.xml";
gf_mva_file_bdt_ = "data/Hhh_mva/2jetinclusive_leppt_BDT.weights.xml";
		//gf_mva_file_bdtg_ = "data/Hhh_mva/final_with_bpt_BDTG.weights.xml";
		mva_input_data_ = "scripts/TMVAinputshad.dat";
		gf_reader_bdt_ = nullptr;
	}

	HhhMTMVABoth::~HhhMTMVABoth() {
		;
	}

	int HhhMTMVABoth::PreAnalysis() {
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "HhhMTMVA" << std::endl;
		std::cout << "-------------------------------------" << std::endl;    
		std::cout << boost::format(param_fmt()) % "dilepton_label"  % ditau_label_;
		std::cout << boost::format(param_fmt()) % "met_label"       % met_label_;
		std::cout << boost::format(param_fmt()) % "gf_mva_file"     % gf_mva_file_bdt_;
		//std::cout << boost::format(param_fmt()) % "gf_mva_file_2"    % gf_mva_file_bdtg_;
		gf_reader_bdt_ = new TMVA::Reader("!Color:!Silent:Error");
	//	gf_reader_bdtg_ = new TMVA::Reader("!Color:!Silent:Error");
		std::vector<TMVA::Reader *> readers = {gf_reader_bdt_};

		



		for (auto & r : readers) {
			r->AddVariable("met",&fmet_);
			r->AddVariable("mt_1",&fmt_1_);
			r->AddVariable("emu_dphi",&femu_dphi_);
			r->AddVariable("pzeta",&fpzeta_);
     	r->AddVariable("pt_1",&fpt_1_);
			r->AddVariable("pt_2",&fpt_2_);
			r->AddSpectator("n_prebjets",&nprebjets_);
			r->AddSpectator("prebjetbcsv_1",&fprebjetbcsv_1_);
			r->AddSpectator("prebjetbcsv_2",&fprebjetbcsv_2_);
			
			
/*			r->AddVariable("mt_2",&fmt_2_);
			r->AddVariable("pt_1",&fpt_1_);
			r->AddVariable("pt_2",&fpt_2_);
			r->AddVariable("mutau_dR",&fmutau_dR_);
			r->AddVariable("prebjetpt_bb",&fprebjetpt_bb_);
			r->AddVariable("prebjet_dR",&fprebjet_dR_);
			r->AddVariable("prebjetpt_1",&fprebjetpt_1_);
			r->AddVariable("prebjetpt_2",&fprebjetpt_2_);
			r->AddVariable("pt_tt",&fpt_tt_);
			r->AddSpectator("mt_1",&fmt_1_);
			r->AddSpectator("n_prebjets",&nprebjets_);
			r->AddSpectator("prebjetbcsv_1",&fprebjetbcsv_1_);
			r->AddSpectator("prebjetbcsv_2",&fprebjetbcsv_2_);
			*/



		}
		gf_reader_bdt_->BookMVA("BDT", gf_mva_file_bdt_);
		return 0;
	}

	int HhhMTMVABoth::Execute(TreeEvent *event) {
		std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
		CompositeCandidate const* ditau = ditau_vec.at(0);
		Candidate const* lep1 = ditau->GetCandidate("lepton1");
		Candidate const* lep2 = ditau->GetCandidate("lepton2");
		Met const* met = event->GetPtr<Met>(met_label_);

    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    std::vector<PFJet*> lowpt_jets = jets;
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
    ic::erase_if(lowpt_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 4.7));
    std::vector<PFJet*> prebjets = lowpt_jets;
    ic::erase_if(prebjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    std::vector<PFJet*> bjets = prebjets;
    std::vector<PFJet*> loose_bjets = prebjets;
    ic::erase_if(loose_bjets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.244);
    //Use prebjet collection for candidate jets for h->bb. Sort by CSV discriminator
    std::sort(prebjets.begin(), prebjets.end(), bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") > bind(&PFJet::GetBDiscriminator, _2, "combinedSecondaryVertexBJetTags"));

    nprebjets_ = (int) prebjets.size();
		femu_dphi_ = (float) std::fabs(ROOT::Math::VectorUtil::DeltaPhi(lep1->vector(), lep2->vector()));
		fpt_1_ = (float) lep1->pt();
		fpt_2_ = (float) lep2->pt();
		fmutau_dR_ = (float) std::fabs(ROOT::Math::VectorUtil::DeltaR(lep1->vector(),lep2->vector()));
		fpt_tt_ = (float) (ditau->vector()+met->vector()).pt();
		fmt_1_ = (float) MT(lep1, met);
		fmt_2_ = (float) MT(lep2, met);
		fmet_ = (float) met->pt();
		fpzeta_ = (float) PZeta(ditau,met,0.85);
		if(nprebjets_>1){
		fprebjetbcsv_1_ = (float) prebjets.at(0)->GetBDiscriminator("combinedSecondaryVertexBJetTags");
		fprebjetbcsv_2_ = (float) prebjets.at(1)->GetBDiscriminator("combinedSecondaryVertexBJetTags");
		fprebjetpt_1_ = (float) prebjets.at(0)->pt();
		fprebjetpt_2_ = (float) prebjets.at(1)->pt();
		fprebjet_dR_ = (float) std::fabs(ROOT::Math::VectorUtil::DeltaR(prebjets.at(0)->vector(),prebjets.at(1)->vector()));
		}
		else{
		fprebjetbcsv_1_=0.;
		fprebjetbcsv_2_=0.;
		}



		
	

		event->Add("em_gf_mva_bdt", gf_reader_bdt_->EvaluateMVA("BDT"));
		return 0;
	}

	int HhhMTMVABoth::PostAnalysis() {
		if (gf_reader_bdt_) delete gf_reader_bdt_;
		return 0;
	}

	void HhhMTMVABoth::PrintInfo() {
		;
	}

}
