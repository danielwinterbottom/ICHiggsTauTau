#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HhhEMuMVABoth.h"
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

	HhhEMuMVABoth::HhhEMuMVABoth(std::string const& name) : ModuleBase(name) {
		ditau_label_ = "emtauCandidates";
		met_label_ = "pfMVAMet";
		jets_label_ = "pfJetsPFlow";
		gf_mva_file_bdt_ = "input/Hhh_mva/HhhEmuMVAInit_BDT.weights.xml";
		gf_mva_file_bdtg_ = "input/Hhh_mva/HhhEmuMVAInit_BDTG.weights.xml";
		mva_input_data_ = "scripts/TMVAinputs.dat";
		gf_reader_bdt_ = nullptr;
		gf_reader_bdtg_ = nullptr;
	}

	HhhEMuMVABoth::~HhhEMuMVABoth() {
		;
	}

	int HhhEMuMVABoth::PreAnalysis() {
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "HhhEMuMVA" << std::endl;
		std::cout << "-------------------------------------" << std::endl;    
		std::cout << boost::format(param_fmt()) % "dilepton_label"  % ditau_label_;
		std::cout << boost::format(param_fmt()) % "met_label"       % met_label_;
		std::cout << boost::format(param_fmt()) % "gf_mva_file"     % gf_mva_file_bdt_;
		std::cout << boost::format(param_fmt()) % "gf_mva_file_2"    % gf_mva_file_bdtg_;
		gf_reader_bdt_ = new TMVA::Reader("!Color:!Silent:Error");
		gf_reader_bdtg_ = new TMVA::Reader("!Color:!Silent:Error");
		std::vector<TMVA::Reader *> readers = {gf_reader_bdt_,gf_reader_bdtg_};

/*		std::vector<std::string> vars;
		ifstream parafile(mva_input_data_.c_str());
		std::string line;
		while(getline(parafile,line)){
			vars.push_back(line);
		}
		parafile.close();


		std::vector<float> floatvars;
		for(unsigned int varit=0;varit<vars.size();++varit){
		  float (::atof(("f"+vars.at(varit)+"_").c_str()));
			floatvars.push_back(::atof(("f"+vars.at(varit)+"_").c_str()));
			std::cout<<("f"+vars.at(varit)+"_").c_str()<<std::endl;
		}*/



		for (auto & r : readers) {
			/*for(unsigned int varit=0;varit<vars.size();++varit){
				r->AddVariable((vars.at(varit)).c_str(),&(floatvars.at(varit)));
			}*/
			r->AddVariable("pt_1",&fpt_1_);
			r->AddVariable("pt_2",&fpt_2_);
			r->AddVariable("met",&fmet_);
			r->AddVariable("mt_ll",&fmt_ll_);
			r->AddVariable("pzeta",&fpzeta_);
			r->AddVariable("pzetamiss",&fpzetamiss_);
			r->AddVariable("emu_dphi",&femu_dphi_);
//			r->AddVariable("prebjetbcsv_1",&fprebjetbcsv_1_);
//			r->AddVariable("prebjetbcsv_2",&fprebjetbcsv_2_);
//			r->AddVariable("prebjet_deta",&fprebjet_deta_);
			r->AddSpectator("n_prebjets",&nprebjets_);
			


			/*r->AddVariable("pt_1", &fpt_1_);
				r->AddVariable("met", &fmet_);
				r->AddVariable("mt_ll", &fmt_ll_);
				r->AddVariable("pzetamiss", &fpzetamiss_);
				r->AddVariable("pt_2", &fpt_2_);
				r->AddVariable("emu_dphi",&femu_dphi_);
				r->AddVariable("pzeta", &fpzeta_);
			 */
			// r->AddVariable("d02", &mu_dxy_);
		}
		gf_reader_bdtg_->BookMVA("BDTG", gf_mva_file_bdtg_);
		gf_reader_bdt_->BookMVA("BDT", gf_mva_file_bdt_);
		return 0;
	}

	int HhhEMuMVABoth::Execute(TreeEvent *event) {
		std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
		CompositeCandidate const* ditau = ditau_vec.at(0);
		Candidate const* lep1 = ditau->GetCandidate("lepton1");
		Candidate const* lep2 = ditau->GetCandidate("lepton2");
		Met const* met = event->GetPtr<Met>(met_label_);
		std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_); // Take a copy of the jets
		ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
		std::sort(jets.begin(), jets.end(), boost::bind(&Candidate::pt, _1) > boost::bind(&Candidate::pt, _2));
		std::vector<PFJet*> lowpt_jets = jets;
		ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
		ic::erase_if(lowpt_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 4.7));

		std::vector<PFJet*> prebjets = lowpt_jets;
		ic::erase_if(prebjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));


	 fpzeta_ = (float) PZeta(ditau,met,0.85);

		nprebjets_ = (int) prebjets.size();
		fpzetamiss_ = (float) PZeta(ditau, met, 0.0);
		femu_dphi_ = (float) std::fabs(ROOT::Math::VectorUtil::DeltaPhi(lep1->vector(), lep2->vector()));
		fpt_1_ = (float) lep1->pt();
		fpt_2_ = (float) lep2->pt();
		fmt_ll_ = (float) MT(ditau, met);
		fmet_ = (float) met->pt();
		
		if (prebjets.size() >= 1) {
			fprebjetbcsv_1_ = (float) prebjets[0]->GetBDiscriminator("combinedSecondaryVertexBJetTags");

		} else {
			fprebjetbcsv_1_ = -9999;
		}
	
		if (prebjets.size() >= 2) {
			fprebjetbcsv_2_ = (float) prebjets[1]->GetBDiscriminator("combinedSecondaryVertexBJetTags");
			fprebjet_deta_ = fabs(prebjets[0]->eta() - prebjets[1]->eta());


		} else {
			fprebjetbcsv_2_ = -9999;
			fprebjet_deta_ = -9999;
		}
		

		event->Add("em_gf_mva_bdtg", gf_reader_bdtg_->EvaluateMVA("BDTG"));
		event->Add("em_gf_mva_bdt", gf_reader_bdt_->EvaluateMVA("BDT"));
		return 0;
	}

	int HhhEMuMVABoth::PostAnalysis() {
		if (gf_reader_bdt_) delete gf_reader_bdt_;
		if (gf_reader_bdtg_) delete gf_reader_bdtg_;
		return 0;
	}

	void HhhEMuMVABoth::PrintInfo() {
		;
	}

}
