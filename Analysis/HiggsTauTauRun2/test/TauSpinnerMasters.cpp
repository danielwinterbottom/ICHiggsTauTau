#include <iostream>
#include <string>
#include <random>
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

TauSpinner::SimpleParticle neutrinoToSimplePart(const PEtaPhi &a_PEtaPhi, std::normal_distribution<double> &a_smearDist, std::mt19937 &a_rng)
{
	double pt = a_PEtaPhi.P/std::cosh(a_PEtaPhi.Eta);
	ROOT::Math::PtEtaPhiM4D<double> particle(pt, a_PEtaPhi.Eta, a_PEtaPhi.Phi, 0);
	double px = particle.Px() + a_smearDist(a_rng);
	double py = particle.Py() + a_smearDist(a_rng);
	double pz = particle.Pz() + a_smearDist(a_rng);
	double E = std::sqrt(px*px + py*py + pz*pz);
  return TauSpinner::SimpleParticle(px, py, pz, E, a_PEtaPhi.pdgID);
}

inline void setupTauDaughters(int decayMode, std::vector<TauSpinner::SimpleParticle> &daughters, Particle &pi0, Particle &pi2, Particle &pi3)
{
	if (decayMode == 1 || decayMode == 2) // rho/pi+2pi0
	{
		daughters.push_back(convertToSimplePart(pi0));
	}
	else if (decayMode == 10) // 3pi
	{
		daughters.push_back(convertToSimplePart(pi2));
		daughters.push_back(convertToSimplePart(pi3));
	}
	else if (decayMode == 11) // 3pi+pi0
	{
		daughters.push_back(convertToSimplePart(pi2));
		daughters.push_back(convertToSimplePart(pi3));
		daughters.push_back(convertToSimplePart(pi0));
	}
}

inline double findPhitt(double sm, double mm, double ps)
{
	// only do calculation if everything input is finite
	if(std::isfinite(sm) && std::isfinite(mm) && std::isfinite(ps))
	{
		double max_weight = -1;
		double max_theta = -9999;
		for(double x = 0; x < 3.14159; x+=0.001 )
		{
			// calculate weight for this event given mixing angle theta
			double event_weight = std::cos(x)*std::cos(x)*sm + std::sin(x)*std::sin(x)*ps + 2*std::cos(x)*std::sin(x)*(mm-sm/2-ps/2);

			if(event_weight > max_weight)
			{
				max_weight = event_weight;
				max_theta = x;
			}
		}
		
		max_theta *= 180/M_PI;
		if(max_theta>90)
		{
			max_theta -= 180;
		}
		
		return max_theta;
	}
	
	// if something isn't finite return nan
	return std::numeric_limits<double>::quiet_NaN();
}

int main(int argc, char* argv[])
{
	
	if(argc < 4)
	{
		std::cerr << "Usage: ./TauSpinnerMasters level:{pseudo, reco} input_file output_file [smearing]" << std::endl;
		return 1;
	}
	
	std::string neutrinoLevel;
	std::string level(argv[1]);
	std::string inputFilename(argv[2]);
	std::string outputFilename(argv[3]);
	
	if(level=="pseudo")
	{
		neutrinoLevel = "gen";
	}
	else
	{
		neutrinoLevel = level;
	}
	
	// Only set smearing if the arguement is specified
	double smearing = 0;
	if(argc>4)
	{
		smearing = std::stod(argv[4], nullptr);
	}
	
	// Partially seed mt19937
	std::mt19937 rng(std::random_device{}());
	std::normal_distribution<double> smearDist(0, smearing);
	std::cout << "Using smearing = " << smearing << std::endl;
	
  // Initalise here (open input file, create output, initalise tauspinner etc...)
	//TFile oldFile("/vols/cms/ktc17/MVAFILE_AllHiggs_tt.root", "READ");
  TFile oldFile(inputFilename.c_str(), "READ");
  if (oldFile.IsZombie())
	{
		std::cerr << "File didn't load correctly." << std::endl;
		return -1;
	}
	TTree *oldTree = static_cast<TTree*>(oldFile.Get("ntuple"));
	
	TFile newFile(outputFilename.c_str(), "recreate");
	
	// Check neutrinos with given name exist
	TString neutrinoBranchName(neutrinoLevel+"_nu_p_1");
	TBranch* neutrinoBranch = static_cast<TBranch *>(oldTree->GetListOfBranches()->FindObject(neutrinoBranchName));
	if (!neutrinoBranch)
	{
		std::cerr << "TauSpinnerMasters Error: Neutrinos with name \"" + neutrinoLevel + "\" not found in root file. Check level arguement. Exiting." << std::endl;
		return 2;
	}
	
	std::cout << "Beginning cloning tree." << std::endl;
	//TTree *tree = oldTree->CloneTree(10000);
	TTree *tree = oldTree->CloneTree();
	std::cout << "Clone finished." << std::endl;
	
	// Setup branches to write to ntuple
	double weight_sm, weight_mm, weight_ps, max_theta;
	TBranch *weight_sm_branch = tree->Branch((level+"_wt_cp_sm").c_str(), &weight_sm, (level+"_wt_cp_sm/D").c_str());
	TBranch *weight_mm_branch = tree->Branch((level+"_wt_cp_mm").c_str(), &weight_mm, (level+"_wt_cp_mm/D").c_str());
	TBranch *weight_ps_branch = tree->Branch((level+"_wt_cp_ps").c_str(), &weight_ps, (level+"_wt_cp_ps/D").c_str());
	TBranch *phitt_branch = tree->Branch((level+"_phitt").c_str(), &max_theta, (level+"_phitt/D").c_str());
	
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
	
	// Set particles to be read
	Particle pi_1, pi2_1, pi3_1, pi0_1;
	Particle pi_2, pi2_2, pi3_2, pi0_2;
	setupParticle(tree, "pi", pi_1, -211, 1);
	setupParticle(tree, "pi", pi_2, 211, 2);
	setupParticle(tree, "pi2", pi2_1, -211, 1);
	setupParticle(tree, "pi2", pi2_2, 211, 2);
	setupParticle(tree, "pi3", pi3_1, 211, 1);
	setupParticle(tree, "pi3", pi3_2, -211, 2);
	setupParticle(tree, "pi0", pi0_1, 111, 1);
	setupParticle(tree, "pi0", pi0_2, 111, 2);
	// Set neutrinos to be read
	PEtaPhi nu_1, nu_2;
	setupNeutrino(tree, (neutrinoLevel+"_nu").c_str(), nu_1, 16, 1);
	setupNeutrino(tree, (neutrinoLevel+"_nu").c_str(), nu_2, -16, 2);
	
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
  for (int i = 0, nEntries = tree->GetEntries(); i < nEntries; i++)
  //for (int i = 0, nEntries = 10000; i < nEntries; i++)
  {
  	tree->GetEntry(i);
		//std::cout << "Entry: " << i << std::endl;
		
		// Always a pi and nu_tau for hadronic decays
		TauSpinner::SimpleParticle pi_1_simple = convertToSimplePart(pi_1);
		TauSpinner::SimpleParticle pi_2_simple = convertToSimplePart(pi_2);
		TauSpinner::SimpleParticle nu_1_simple = neutrinoToSimplePart(nu_1, smearDist, rng);
		TauSpinner::SimpleParticle nu_2_simple = neutrinoToSimplePart(nu_2, smearDist, rng);
		
		TauSpinner::SimpleParticle tau_1_simple, tau_2_simple, Higgs_simple;
		std::vector<TauSpinner::SimpleParticle> simple_tau1_daughters, simple_tau2_daughters;
		simple_tau1_daughters.push_back(nu_1_simple);
		simple_tau2_daughters.push_back(nu_2_simple);
		simple_tau1_daughters.push_back(pi_1_simple);
		simple_tau2_daughters.push_back(pi_2_simple);
		
		// Handle tau decay modes
		setupTauDaughters(mva_dm_1, simple_tau1_daughters, pi0_1, pi2_1, pi3_1);
		setupTauDaughters(mva_dm_2, simple_tau2_daughters, pi0_2, pi2_2, pi3_2);
		
		/*
		std::cout << std::endl;
		std::cout << "mva_dm_1 = " << mva_dm_1 << std::endl;
		std::cout << "tauFlag_1 = " << tauFlag_1 << std::endl;
		std::cout << "Tau 1 daughters:" << std::endl;
		*/
		// add up tau_1
		for(auto daughter : simple_tau1_daughters)
		{
			//std::cout << daughter.pdgid() << ": " << daughter.e() << ", " << daughter.px() << ", " << daughter.py() << ", " << daughter.pz() << std::endl;
			tau_1_simple = addSimpleParticles(tau_1_simple, daughter, 0);
		}
		tau_1_simple.setPdgid(15);
		
		/*
		std::cout << std::endl;
		std::cout << "mva_dm_2 = " << mva_dm_2 << std::endl;
		std::cout << "tauFlag_2 = " << tauFlag_2 << std::endl;
		std::cout << "Tau 2 daughters:" << std::endl;
		*/
		// add up tau_2
		for(auto daughter : simple_tau2_daughters)
		{
			//std::cout << daughter.pdgid() << ": " << daughter.e() << ", " << daughter.px() << ", " << daughter.py() << ", " << daughter.pz() << std::endl;
			tau_2_simple = addSimpleParticles(tau_2_simple, daughter, 0);
		}
		tau_2_simple.setPdgid(-15);
		
		// add up Higgs
		Higgs_simple = addSimpleParticles(tau_1_simple, tau_2_simple, 25);
		
		// Calculate different weights: sm, mm, ps
		TauSpinner::setHiggsParametersTR(-cos(2*M_PI*0), cos(2*M_PI*0), -sin(2*M_PI*0), -sin(2*M_PI*0));
		weight_sm = TauSpinner::calculateWeightFromParticlesH(Higgs_simple, tau_1_simple, tau_2_simple, simple_tau1_daughters, simple_tau2_daughters);
		TauSpinner::setHiggsParametersTR(-cos(2*M_PI*0.25), cos(2*M_PI*0.25), -sin(2*M_PI*0.25), -sin(2*M_PI*0.25));
		weight_mm = TauSpinner::calculateWeightFromParticlesH(Higgs_simple, tau_1_simple, tau_2_simple, simple_tau1_daughters, simple_tau2_daughters);
		TauSpinner::setHiggsParametersTR(-cos(2*M_PI*0.5), cos(2*M_PI*0.5), -sin(2*M_PI*0.5), -sin(2*M_PI*0.5));
		weight_ps = TauSpinner::calculateWeightFromParticlesH(Higgs_simple, tau_1_simple, tau_2_simple, simple_tau1_daughters, simple_tau2_daughters);
		
		/*
		if (true) // mva_dm_1 == 0 && mva_dm_2 == 0 ) // Print selected weights
		{
			std::cout << "Event " << i << " calculated:\tsm = " << weight_sm << "\tmm = " << weight_mm << "\tps = " << weight_ps << std::endl;
			std::cout << "Event " << i << " .root true:\tsm = " << stored_wt_cp_sm << "\tmm = " << stored_wt_cp_mm << "\tps = " << stored_wt_cp_ps << std::endl << std::endl;
		}
		*/
		
		max_theta = findPhitt(weight_sm, weight_mm, weight_ps);
		
		// Fill branches
		//std::cout << "Filling branches, entry " << i << std::endl;
		weight_sm_branch->Fill();
		weight_mm_branch->Fill();
		weight_ps_branch->Fill();
		phitt_branch->Fill();
		
	} // Event loop
  
  // Write new trees here
  tree->Write("", TObject::kOverwrite);
  
  return 0;
}
