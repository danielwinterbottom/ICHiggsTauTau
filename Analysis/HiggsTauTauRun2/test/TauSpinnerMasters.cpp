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
  	if ( mva_dm_1 == 1 && mva_dm_2 == 1
  			 && tau_decay_mode_1 == 1 && tau_decay_mode_2 == 1
  	) // Event Selection
  	{
			auto pi_1_simple = convertToSimplePart(pi_1);
			auto pi_2_simple = convertToSimplePart(pi_2);
			auto pi0_1_simple = convertToSimplePart(pi0_1);
			auto pi0_2_simple = convertToSimplePart(pi0_2);
			auto nu_1_simple = neutrinoToSimplePart(nu_1);
			auto nu_2_simple = neutrinoToSimplePart(nu_2);
			
			//std::cout << "nu_2_Epxpypz, E=" << nu_2_simple.e() << "\tpx=" << nu_2_simple.px() << "\tpy=" << nu_2_simple.py() << "\tpz=" << nu_2_simple.pz() << std::endl;
			
			auto a1_1_simple = addSimpleParticles(pi_1_simple, pi0_1_simple, -213);
			auto a1_2_simple = addSimpleParticles(pi_2_simple, pi0_2_simple, 213);
			auto tau_1_simple = addSimpleParticles(a1_1_simple, nu_1_simple, 15);
			auto tau_2_simple = addSimpleParticles(a1_2_simple, nu_2_simple, -15);
			auto Higgs_simple = addSimpleParticles(tau_1_simple, tau_2_simple, 25);
			//std::cout << "Higgs mass = " << std::sqrt(Higgs_simple.e()*Higgs_simple.e() - Higgs_simple.px()*Higgs_simple.px() - Higgs_simple.py()*Higgs_simple.py() - Higgs_simple.pz()*Higgs_simple.pz()) << std::endl;
			
			// Make simple_tau_daughters vectors
			std::vector<TauSpinner::SimpleParticle> simple_tau1_daughters, simple_tau2_daughters;
			simple_tau1_daughters.push_back(nu_1_simple);
			simple_tau1_daughters.push_back(pi0_1_simple);
			simple_tau1_daughters.push_back(pi_1_simple);
			
			simple_tau2_daughters.push_back(nu_2_simple);
			simple_tau2_daughters.push_back(pi0_2_simple);
			simple_tau2_daughters.push_back(pi_2_simple);
			
			//double wt_prod, wt_P, wt_M;//, wt_EW, wt_EW0;
			TauSpinner::setHiggsParametersTR(-cos(2*M_PI*0), cos(2*M_PI*0), -sin(2*M_PI*0), -sin(2*M_PI*0));
			double weight_sm = TauSpinner::calculateWeightFromParticlesH(Higgs_simple, tau_1_simple, tau_2_simple, simple_tau1_daughters, simple_tau2_daughters);
			/*
			wt_prod = TauSpinner::getWtNonSM();
			wt_P = TauSpinner::getWtamplitP();
			wt_M = TauSpinner::getWtamplitM();
			//wt_EW = TauSpinner::getEWwt();
			//wt_EW0 = TauSpinner::getEWwt0();
			std::cout << "Other weights SM: " << wt_prod << wt_P << wt_M << std::endl;// << wt_EW << wt_EW0 << std::endl;
			*/
			TauSpinner::setHiggsParametersTR(-cos(2*M_PI*0.25), cos(2*M_PI*0.25), -sin(2*M_PI*0.25), -sin(2*M_PI*0.25));
			double weight_mm = TauSpinner::calculateWeightFromParticlesH(Higgs_simple, tau_1_simple, tau_2_simple, simple_tau1_daughters, simple_tau2_daughters);
			TauSpinner::setHiggsParametersTR(-cos(2*M_PI*0.5), cos(2*M_PI*0.5), -sin(2*M_PI*0.5), -sin(2*M_PI*0.5));
			double weight_cp = TauSpinner::calculateWeightFromParticlesH(Higgs_simple, tau_1_simple, tau_2_simple, simple_tau1_daughters, simple_tau2_daughters);
			/*
			wt_prod = TauSpinner::getWtNonSM();
			wt_P = TauSpinner::getWtamplitP();
			wt_M = TauSpinner::getWtamplitM();
			//wt_EW = TauSpinner::getEWwt();
			//wt_EW0 = TauSpinner::getEWwt0();
			std::cout << "Other weights CP: " << wt_prod << wt_P << wt_M << std::endl;// << wt_EW << wt_EW0 << std::endl;
			*/
			std::cout << "Event " << i << " weights: sm = " << weight_sm << "\tweight_mm = " << weight_mm << "\tweightcp = " << weight_cp << std::endl;
  	} // Event selection
	} // Event loop
  
  // Write new trees here
  
  return 0;
}
