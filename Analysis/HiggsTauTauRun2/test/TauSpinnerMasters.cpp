#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "Math/GenVector/PtEtaPhiM4D.h"
#include "Tauola/Tauola.h"
#include "TauSpinner/SimpleParticle.h"
#include "TauSpinner/tau_reweight_lib.h"

struct Particle
{
	double px;
	double py;
	double pz;
	double E;
	int pdgID;
};

struct PEtaPhi
{
	double P;
	double Eta;
	double Phi;
	int pdgID;
};

/** Add two simple particles resulting in a third simple particles. Must specify new pdgID */
TauSpinner::SimpleParticle addSimpleParticles(TauSpinner::SimpleParticle &a, TauSpinner::SimpleParticle &b, int result_pdgID)
{
	return TauSpinner::SimpleParticle(a.px()+b.px(), a.py()+b.py(), a.pz()+b.pz(), a.e()+b.e(), result_pdgID);
}

/** Function to set particle data to be read from a_tree */
void setupParticle(TTree *a_tree, std::string a_name, Particle &a_particle, int a_pdgID, int a_fromTau)
{
	a_tree->SetBranchAddress((a_name+"_px_"+std::to_string(a_fromTau)).c_str(), &a_particle.px);
	a_tree->SetBranchAddress((a_name+"_py_"+std::to_string(a_fromTau)).c_str(), &a_particle.py);
	a_tree->SetBranchAddress((a_name+"_pz_"+std::to_string(a_fromTau)).c_str(), &a_particle.pz);
	a_tree->SetBranchAddress((a_name+"_E_"+std::to_string(a_fromTau)).c_str(), &a_particle.E);
	a_particle.pdgID = a_pdgID;
}

/** Function to set neutrino values to be read from a_tree */
void setupNeutrino(TTree *a_tree, std::string a_name, PEtaPhi &a_PEtaPhi, int a_pdgID, int a_fromTau)
{
	a_tree->SetBranchAddress((a_name+"_p_"+std::to_string(a_fromTau)).c_str(), &a_PEtaPhi.P);
	a_tree->SetBranchAddress((a_name+"_eta_"+std::to_string(a_fromTau)).c_str(), &a_PEtaPhi.Eta);
	a_tree->SetBranchAddress((a_name+"_phi_"+std::to_string(a_fromTau)).c_str(), &a_PEtaPhi.Phi);
	a_PEtaPhi.pdgID = a_pdgID;
}

TauSpinner::SimpleParticle convertToSimplePart(const Particle &a_particle)
{
  return TauSpinner::SimpleParticle(a_particle.px, a_particle.py, a_particle.pz, a_particle.E, a_particle.pdgID);
}

TauSpinner::SimpleParticle neutrinoToSimplePart(const PEtaPhi &a_PEtaPhi)
{
	double pt = a_PEtaPhi.P/std::cosh(a_PEtaPhi.Eta);
	ROOT::Math::PtEtaPhiM4D<double> particle(pt, a_PEtaPhi.Eta, a_PEtaPhi.Phi, 0);
  return TauSpinner::SimpleParticle(particle.Px(), particle.Py(), particle.Pz(), particle.E(), a_PEtaPhi.pdgID);
}

int main(/*int argc, char* argv[]*/)
{
	const bool useRecoSelectors = true;
  
  // Initalise here (open input file, create output, initalise tauspinner etc...)
	TFile file("/vols/cms/ktc17/MVAFILE_AllHiggs_tt.root", "READ");
  if (file.IsZombie())
	{
		std::cerr << "File didn't load correctly." << std::endl;
		return -1;
	}
	TTree *tree = static_cast<TTree*>(file.Get("ntuple"));
	
	// Setup variables to read from branches
	int tau_decay_mode_1, tau_decay_mode_2, mva_dm_1, mva_dm_2;
	tree->SetBranchAddress("tau_decay_mode_1", &tau_decay_mode_1);
	tree->SetBranchAddress("tau_decay_mode_2", &tau_decay_mode_2);
	tree->SetBranchAddress("mva_dm_1", &mva_dm_1);
	tree->SetBranchAddress("mva_dm_2", &mva_dm_2);
	// Gen selectors
	int tauFlag_1, tauFlag_2;
	tree->SetBranchAddress("tauFlag_1", &tauFlag_1);
	tree->SetBranchAddress("tauFlag_2", &tauFlag_2);
	// Stored weights
	double stored_wt_cp_sm, stored_wt_cp_mm, stored_wt_cp_ps;
	tree->SetBranchAddress("wt_cp_sm", &stored_wt_cp_sm);
	tree->SetBranchAddress("wt_cp_mm", &stored_wt_cp_mm);
	tree->SetBranchAddress("wt_cp_ps", &stored_wt_cp_ps);
	
	// Setup particles
	Particle pi_1, pi_2, pi0_1, pi0_2;
	setupParticle(tree, "pi", pi_1, -211, 1);
	setupParticle(tree, "pi", pi_2, 211, 2);
	setupParticle(tree, "pi0", pi0_1, 111, 1);
	setupParticle(tree, "pi0", pi0_2, 111, 2);
	// Set neutrinos to read from gen for now
	PEtaPhi nu_1, nu_2;
	setupNeutrino(tree, "gen_nu", nu_1, 16, 1);
	setupNeutrino(tree, "gen_nu", nu_2, -16, 2);
	
	// Variables for initialising TauSpinner
  std::string TauSpinnerSettingsPDF="NNPDF30_nlo_as_0118";
  bool Ipp=true;
  int Ipol=0;
  int nonSM2=0;
  int nonSMN=0;
  double CMSENE=13000.0;
	
	// Initialise TauSpinner
  Tauolapp::Tauola::setNewCurrents(1);
  Tauolapp::Tauola::initialize();
  LHAPDF::initPDFSetByName(TauSpinnerSettingsPDF);
  TauSpinner::initialize_spinner(Ipp, Ipol, nonSM2, nonSMN,  CMSENE);
  
  // Event loop
  //for (int i = 0, nEntries = tree->GetEntries(); i < nEntries; i++)
  for (int i = 0, nEntries = 1000; i < nEntries; i++)
  {
  	tree->GetEntry(i);
  	//std::cout << i << std::endl;
  	
  	// Standard for all hadronic decays
  	TauSpinner::SimpleParticle pi_1_simple = convertToSimplePart(pi_1);
		TauSpinner::SimpleParticle pi_2_simple = convertToSimplePart(pi_2);
		TauSpinner::SimpleParticle nu_1_simple = neutrinoToSimplePart(nu_1);
		TauSpinner::SimpleParticle nu_2_simple = neutrinoToSimplePart(nu_2);
		
		TauSpinner::SimpleParticle tau_1_simple, tau_2_simple, Higgs_simple;
		std::vector<TauSpinner::SimpleParticle> simple_tau1_daughters, simple_tau2_daughters;
		simple_tau1_daughters.push_back(nu_1_simple);
		simple_tau2_daughters.push_back(nu_2_simple);
		simple_tau1_daughters.push_back(pi_1_simple);
		simple_tau2_daughters.push_back(pi_2_simple);
  	
  	// rho-rho and a1-a1 decays
  	if (
  			(useRecoSelectors &&
  				(mva_dm_1 == 1 && mva_dm_2 == 1
					&& tau_decay_mode_1 == 1 && tau_decay_mode_2 == 1)
				)
			||
			(
				!useRecoSelectors && (tauFlag_1 == 1 && tauFlag_2 == 1)
			)
  	) // Event Selection
  	{
			TauSpinner::SimpleParticle pi0_1_simple = convertToSimplePart(pi0_1);
			TauSpinner::SimpleParticle pi0_2_simple = convertToSimplePart(pi0_2);
			
			//std::cout << "nu_2_Epxpypz, E=" << nu_2_simple.e() << "\tpx=" << nu_2_simple.px() << "\tpy=" << nu_2_simple.py() << "\tpz=" << nu_2_simple.pz() << std::endl;
			
			TauSpinner::SimpleParticle a1_1_simple = addSimpleParticles(pi_1_simple, pi0_1_simple, -213);
			TauSpinner::SimpleParticle a1_2_simple = addSimpleParticles(pi_2_simple, pi0_2_simple, 213);
			tau_1_simple = addSimpleParticles(a1_1_simple, nu_1_simple, 15);				// rho/a1 channel
			tau_2_simple = addSimpleParticles(a1_2_simple, nu_2_simple, -15);			// rho/a1 channel
			//auto tau_1_simple = addSimpleParticles(pi_1_simple, nu_1_simple, 15);			// pi channel
			//auto tau_2_simple = addSimpleParticles(pi_2_simple, nu_2_simple, -15);		// pi channel
			//std::cout << "Higgs mass = " << std::sqrt(Higgs_simple.e()*Higgs_simple.e() - Higgs_simple.px()*Higgs_simple.px() - Higgs_simple.py()*Higgs_simple.py() - Higgs_simple.pz()*Higgs_simple.pz()) << std::endl;
			
			// Pi0s add to tau1_daughters
			simple_tau1_daughters.push_back(pi0_1_simple);
			simple_tau2_daughters.push_back(pi0_2_simple);
  	} // Event selection
  	
  	Higgs_simple = addSimpleParticles(tau_1_simple, tau_2_simple, 25);
		TauSpinner::setHiggsParametersTR(-cos(2*M_PI*0), cos(2*M_PI*0), -sin(2*M_PI*0), -sin(2*M_PI*0));
		double weight_sm = TauSpinner::calculateWeightFromParticlesH(Higgs_simple, tau_1_simple, tau_2_simple, simple_tau1_daughters, simple_tau2_daughters);
		TauSpinner::setHiggsParametersTR(-cos(2*M_PI*0.25), cos(2*M_PI*0.25), -sin(2*M_PI*0.25), -sin(2*M_PI*0.25));
		double weight_mm = TauSpinner::calculateWeightFromParticlesH(Higgs_simple, tau_1_simple, tau_2_simple, simple_tau1_daughters, simple_tau2_daughters);
		TauSpinner::setHiggsParametersTR(-cos(2*M_PI*0.5), cos(2*M_PI*0.5), -sin(2*M_PI*0.5), -sin(2*M_PI*0.5));
		double weight_cp = TauSpinner::calculateWeightFromParticlesH(Higgs_simple, tau_1_simple, tau_2_simple, simple_tau1_daughters, simple_tau2_daughters);
		
  	if (
			mva_dm_1 == 1 && mva_dm_2 == 1
			&& tau_decay_mode_1 == 1 && tau_decay_mode_2 == 1
			//tauFlag_1 == 1 && tauFlag_2 == 1
  	) // Event Selection
  	{
			std::cout << "Event " << i << " calculated:\tsm = " << weight_sm << "\tmm = " << weight_mm << "\tps = " << weight_cp << std::endl;
			std::cout << "Event " << i << " .root true:\tsm = " << stored_wt_cp_sm << "\tmm = " << stored_wt_cp_mm << "\tps = " << stored_wt_cp_ps << std::endl << std::endl;
  	}
	} // Event loop
  
  // Write new trees here
  
  return 0;
}
