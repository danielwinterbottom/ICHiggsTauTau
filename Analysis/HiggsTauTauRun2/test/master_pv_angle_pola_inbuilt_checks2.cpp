#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"

#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTCategories.h"
#include "UserCode/ICHiggsTauTau/interface/PFCandidate.hh"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SCalculator.h"
#include "Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"

#include "TMVA/Reader.h"
#include "TVector3.h"
#include "boost/format.hpp"
#include "TMath.h"
#include "TLorentzVector.h"

#include "UserCode/ICHiggsTauTau//interface/city.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/ElectronEffectiveArea.h"

#include <boost/lexical_cast.hpp>
#include "TRandom2.h"
#include "TMath.h"
#include <utility>
#include <set>


#include "Math/GenVector/PtEtaPhiM4D.h"
#include "Tauola/Tauola.h"
#include "TauSpinner/SimpleParticle.h"
#include "TauSpinner/tau_reweight_lib.h"


/*pv_angle for reco (=regressed with updated polarimetric neutrinos as inputs) and for reco2 (=regressed with 'old' polarimetric neutrinos) as well as for pola(=updated polarimetric neutrinos) and pola2(='old' polarimetric neutrinos) in this file: /vols/cms/ac4317/msci/CMSSW_10_2_19/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/MVAFILE_full_10_10_pv.root pseudo is with gen_neutrinos*/

//#include <iostream>
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

TLorentzVector neutrinoToLorentz(const PEtaPhi &a_PEtaPhi)
{
	double pt = a_PEtaPhi.P/std::cosh(a_PEtaPhi.Eta);
	ROOT::Math::PtEtaPhiM4D<double> particle(pt, a_PEtaPhi.Eta, a_PEtaPhi.Phi, 0);
	return TLorentzVector(particle.Px(), particle.Py(), particle.Pz(), particle.E());
}


/*Funtion to produce the vactor of pions, used by the AcopAngle method that
 * will then boost them in the Higgs rest frame*/
std::vector<TLorentzVector> getPis(Particle pi, Particle pi2, Particle pi3)
{
	TLorentzVector pi_L = TLorentzVector(pi.px, pi.py, pi.pz, pi.E);
	TLorentzVector pi2_L = TLorentzVector(pi2.px, pi2.py, pi2.pz, pi2.E);
	TLorentzVector pi3_L = TLorentzVector(pi3.px, pi3.py, pi3.pz, pi3.E);
	std::vector<TLorentzVector> pis = { 
		pi_L,
		pi2_L,
		pi3_L
  };
  return pis;
}

/*construct the tau in a1(3pr) decays with neutrinos and pions */
TLorentzVector getTau(Particle pi, Particle pi2, Particle pi3, PEtaPhi nu){
	TLorentzVector pi_L = TLorentzVector(pi.px, pi.py, pi.pz, pi.E);
	TLorentzVector pi2_L = TLorentzVector(pi2.px, pi2.py, pi2.pz, pi2.E);
	TLorentzVector pi3_L = TLorentzVector(pi3.px, pi3.py, pi3.pz, pi3.E);
	TLorentzVector nu_L = neutrinoToLorentz(nu);
	TLorentzVector tau = pi_L + pi2_L + pi3_L + nu_L;
	return tau;
}

namespace pola
{
	
	const double m_tau = 1.77686;
	const double m_higgs = 125.10;
	
	// Calibration for met
	const double alpha = 0.405;
	const double beta = 0.405;
	
	/** Finds possible solutions for the polarimetric method. Performs tau direction cone rotation if required.
	    Returns true if successful, otherwise false.*/
	bool findTauSolutions(TLorentzVector &tau_sol_1, TLorentzVector &tau_sol_2, TLorentzVector tau_vis, TVector3 sv)
	{
		const double m_vis = tau_vis.M();
		
		if (sv.Mag2() == 0)
		{
			return false;
		}
		
		TVector3 vis_dir = tau_vis.Vect().Unit();
		TVector3 tau_dir = sv.Unit();
		
    double theta_GJ = std::acos(std::clamp(tau_dir.Dot(vis_dir.Unit()), -1., 1.));
    double theta_GJ_max = std::asin(std::clamp((m_tau*m_tau - m_vis*m_vis)/(2*m_tau*tau_vis.P()), -1., 1.));
		
		TVector3 new_dir;
		// Rotate tau back if theta_GJ is in unphysical region
		if (theta_GJ > theta_GJ_max)
		{
			// Create a normalised vector prependicular to a1
			double n_1_x = 1/std::sqrt(1+std::pow(tau_vis.X()/tau_vis.Y(), 2));
			double n_1_y = -n_1_x * tau_vis.X()/tau_vis.Y();
			TVector3 n_1(n_1_x, n_1_y, 0);
			// create n_2, a unit vector perpendicular to n_1 and the a1
			TVector3 n_2 = n_1.Cross(tau_vis.Vect()).Unit();
			
			// optimal phi from calculus
			double phi_opt = std::atan(sv.Dot(n_2)/sv.Dot(n_1));
			new_dir = std::cos(theta_GJ_max)*vis_dir + std::sin(theta_GJ_max)*(std::cos(phi_opt)*n_1 + std::sin(phi_opt)*n_2);
			
			
		}
		else
		{
			new_dir = tau_dir;
		}
		
		// Calculate the tau momentum
    double minus_b = (m_vis*m_vis + m_tau*m_tau) * tau_vis.P() * std::cos(theta_GJ);
    //TODO: check tau_vis.p2 properly replaced in these next 2 lines
    double two_a = 2*std::pow(m_vis*m_vis + std::pow(tau_vis.P(), 2) * std::sin(theta_GJ), 2);
		double b_squared_m_four_ac = (m_vis*m_vis + std::pow(tau_vis.P(), 2)) * (std::pow(m_vis*m_vis - m_tau*m_tau, 2) - 4*m_tau*m_tau*std::pow(tau_vis.P(), 2)*std::pow(std::sin(theta_GJ), 2));
    
    // two solutions for tau momentum magnitude
    double sol_1 = (minus_b + std::sqrt(b_squared_m_four_ac))/two_a;
		double sol_2 = (minus_b - std::sqrt(b_squared_m_four_ac))/two_a;
		
		tau_sol_1 = TLorentzVector(std::sqrt(sol_1*sol_1+m_tau*m_tau), new_dir.x()*sol_1, new_dir.y()*sol_1, new_dir.z()*sol_1);
		tau_sol_2 = TLorentzVector(std::sqrt(sol_2*sol_2+m_tau*m_tau), new_dir.x()*sol_2, new_dir.y()*sol_2, new_dir.z()*sol_2);
		
		return true;
	}
	
	
	/** Calculates the best neutrinos using the polarimetric method.
	    outputs true if successful or false if best neutrinos could not be found.
	    input sum of visisble products, met and secondary vertices for each tau. */
	bool a1_a1_polarimetric(TLorentzVector &best_nu_1, TLorentzVector &best_nu_2, 
	                        TLorentzVector tau_1_vis, TLorentzVector tau_2_vis,
	                        TVector3 sv_1, TVector3 sv_2,
	                        double met_x, double met_y)
	{
		TLorentzVector tau_1_sols[2];
		if (!findTauSolutions(tau_1_sols[0], tau_1_sols[1], tau_1_vis, sv_1))
		{
			return false;
		}
		
		TLorentzVector tau_2_sols[2];
		if (!findTauSolutions(tau_2_sols[0], tau_2_sols[1], tau_2_vis, sv_2))
		{
			return false;
		}
		
		// Loop through possible tau solutions to find best
		double min_diff = std::numeric_limits<double>::infinity();
		TLorentzVector best_tau_1;
		TLorentzVector best_tau_2;
		for( auto tau_1 : tau_1_sols)
		{
			for ( auto tau_2 : tau_2_sols)
			{
				TLorentzVector nu_1 = tau_1-tau_1_vis;
				TLorentzVector nu_2 = tau_2-tau_2_vis;
				TLorentzVector higgs = tau_1 + tau_2;
				
				double pred_met_x = nu_1.Px() + nu_2.Px();
				double pred_met_y = nu_1.Py() + nu_2.Py();
				
				// Calculate the difference between guessed and observed values for met and higgs mass
				double diff_Higgs = std::pow(higgs.M()-m_higgs, 2);
				double diff_metx = std::pow(pred_met_x - met_x, 2);
				double diff_mety = std::pow(pred_met_y - met_y, 2);
				double total_diff = diff_Higgs + alpha * diff_metx + beta * diff_mety;
				
				// Update the best tau guesses if predictions are closer to observations
				if (total_diff < min_diff)
				{
					best_tau_1 = tau_1;
					best_tau_2 = tau_2;
					min_diff = total_diff;
				}
			}
		}
		
		best_nu_1 = best_tau_1 - tau_1_vis;
		best_nu_2 = best_tau_2 - tau_2_vis;
		
		return true;
	}
}

/*This is calculating the pv angle         
 *Note: the AcopAngle gets the pv angle for a1 a1s:
 *https://github.com/danielwinterbottom/ICHiggsTauTau/blob/c21542125ed10f82d01ca2ae3e4286abcba8d4f6/Analysis/Utilities/src/SCalculator.cc#L260*/
namespace ic
{
	double getPV_angle(TLorentzVector Tauminus, std::vector<TLorentzVector> pis_1, std::vector<double> charges_1,
	                   TLorentzVector Tauplus, std::vector<TLorentzVector> pis_2, std::vector<double> charges_2)
	{
		SCalculator Scalc("a1");
		double angle = -9999.;
		if(Scalc.isOk("a1", "a1", Tauminus, pis_1, charges_1, Tauplus, pis_2, charges_2))
		{
			angle = Scalc.AcopAngle("a1", "a1", Tauminus, pis_1, charges_1, Tauplus, pis_2, charges_2);
			std::cout << "\nGood variables - Angle: " << angle << '\n';
		}
		else
		{
			std::cout << "Wrong variables";
		}
		return angle;
	}
}

int main(int argc, char* argv[])
{
	std::string inputFilename(argv[1]);
	std::string neutrinoLevel = "gen";
  std::string reco = "reco";
  std::string pola = "pola";
  std::string level = "pseudo";
  std::string outputFilename(argv[2]);
	
  TFile oldFile(inputFilename.c_str(), "READ");
	if (oldFile.IsZombie())
	{
		std::cerr << "File didn't load correctly." << std::endl;
		return -1;
	}
	TTree *oldTree = static_cast<TTree*>(oldFile.Get("ntuple"));
	
	TFile newFile(outputFilename.c_str(), "recreate");
	std::cout << "Beginning cloning tree." << std::endl;
	TTree *tree = oldTree->CloneTree();
	std::cout << "Clone finished." << std::endl;
  
  //Set-up write-up branches
  double pv_angle, reco_pv_angle, reco2_pv_angle, pola_pv_angle, pola2_pv_angle, pola3_pv_angle, pola4_pv_angle;
	double pola9_nu_p_1, pola9_nu_eta_1, pola9_nu_phi_1; 
	double pola9_nu_p_2, pola9_nu_eta_2, pola9_nu_phi_2; 



  
  TBranch *pv_angle_branch = tree->Branch((level+"_pv_angle").c_str(), &pv_angle, (level+"_pv_angle/D").c_str());
  
  TBranch *reco_pv_angle_branch = tree->Branch((reco+"_pv_angle").c_str(), &reco_pv_angle, (reco+"_pv_angle/D").c_str());
  TBranch *reco2_pv_angle_branch = tree->Branch((reco+"2_pv_angle").c_str(), &reco2_pv_angle, (reco+"2_pv_angle/D").c_str());
  
  TBranch *pola_pv_angle_branch = tree->Branch((pola+"_pv_angle").c_str(), &pola_pv_angle, (pola+"_pv_angle/D").c_str());
  TBranch *pola2_pv_angle_branch = tree->Branch((pola+"2_pv_angle").c_str(), &pola2_pv_angle, (pola+"2_pv_angle/D").c_str());
  TBranch *pola3_pv_angle_branch = tree->Branch((pola+"3_pv_angle").c_str(), &pola3_pv_angle, (pola+"3_pv_angle/D").c_str());
  TBranch *pola4_pv_angle_branch = tree->Branch((pola+"4_pv_angle").c_str(), &pola4_pv_angle, (pola+"4_pv_angle/D").c_str());

	TBranch *pola9_nu_p_1_branch = tree->Branch((pola+"9_nu_p_1").c_str(), &pola9_nu_p_1, (pola+"9_nu_p_1/D").c_str());
	TBranch *pola9_nu_eta_1_branch = tree->Branch((pola+"9_nu_eta_1").c_str(), &pola9_nu_eta_1, (pola+"9_nu_eta_1/D").c_str());
	TBranch *pola9_nu_phi_1_branch = tree->Branch((pola+"9_nu_phi_1").c_str(), &pola9_nu_phi_1, (pola+"9_nu_phi_1/D").c_str());
	
	TBranch *pola9_nu_p_2_branch = tree->Branch((pola+"9_nu_p_2").c_str(), &pola9_nu_p_2, (pola+"9_nu_p_2/D").c_str());
	TBranch *pola9_nu_eta_2_branch = tree->Branch((pola+"9_nu_eta_2").c_str(), &pola9_nu_eta_2, (pola+"9_nu_eta_2/D").c_str());
	TBranch *pola9_nu_phi_2_branch = tree->Branch((pola+"9_nu_phi_2").c_str(), &pola9_nu_phi_2, (pola+"9_nu_phi_2/D").c_str());
	
	
	
	// Setup particles
	Particle pi_1, pi2_1, pi3_1;	
	Particle pi_2, pi2_2, pi3_2;
	setupParticle(tree, "pi", pi_1, -211, 1);
	setupParticle(tree, "pi", pi_2, 211, 2);
	setupParticle(tree, "pi2", pi2_1, -211, 1);
	setupParticle(tree, "pi2", pi2_2, 211, 2);
	setupParticle(tree, "pi3", pi3_1, 211, 1);
	setupParticle(tree, "pi3", pi3_2, -211, 2);
	
	// Setup MET
	double met_x, met_y;
	tree->SetBranchAddress("metx", &met_x);
	tree->SetBranchAddress("mety", &met_y);
	// Setup SV
	double sv_1[3];
	double sv_2[3];
	tree->SetBranchAddress("sv_x_1", &sv_1[0]);
	tree->SetBranchAddress("sv_y_1", &sv_1[1]);
	tree->SetBranchAddress("sv_z_1", &sv_1[2]);
	tree->SetBranchAddress("sv_x_2", &sv_2[0]);
	tree->SetBranchAddress("sv_y_2", &sv_2[1]);
	tree->SetBranchAddress("sv_z_2", &sv_2[2]);
	
  //Setup Neutrinos
  PEtaPhi nu_1, nu_2;
	setupNeutrino(tree, (neutrinoLevel+"_nu").c_str(), nu_1, 16, 1);
	setupNeutrino(tree, (neutrinoLevel+"_nu").c_str(), nu_2, -16, 2);
    
  PEtaPhi reco_nu_1, reco_nu_2;
	setupNeutrino(tree, (reco + "_nu").c_str(), reco_nu_1, 16, 1);
	setupNeutrino(tree, (reco + "_nu").c_str(), reco_nu_2, -16, 2);
    
  PEtaPhi reco2_nu_1, reco2_nu_2;
	setupNeutrino(tree, (reco + "2_nu").c_str(), reco2_nu_1, 16, 1);
	setupNeutrino(tree, (reco + "2_nu").c_str(), reco2_nu_2, -16, 2);
    
  PEtaPhi pola_nu_1, pola_nu_2;
	setupNeutrino(tree, (pola + "_nu").c_str(), pola_nu_1, 16, 1);
	setupNeutrino(tree, (pola + "_nu").c_str(), pola_nu_2, -16, 2);
    
  PEtaPhi pola2_nu_1, pola2_nu_2;
	setupNeutrino(tree, (pola + "2_nu").c_str(), pola2_nu_1, 16, 1);
	setupNeutrino(tree, (pola + "2_nu").c_str(), pola2_nu_2, -16, 2);

  PEtaPhi pola3_nu_1, pola3_nu_2;
	setupNeutrino(tree, (pola + "3_nu").c_str(), pola3_nu_1, 16, 1);
	setupNeutrino(tree, (pola + "3_nu").c_str(), pola3_nu_2, -16, 2);
    
  PEtaPhi pola4_nu_1, pola4_nu_2;
	setupNeutrino(tree, (pola + "4_nu").c_str(), pola4_nu_1, 16, 1);
	setupNeutrino(tree, (pola + "4_nu").c_str(), pola4_nu_2, -16, 2);
    
    
  /*Loop over the tree entries*/
  //tree->GetEntries()
  for (int i = 0, nEntries = tree->GetEntries(); i < nEntries; i++)
  {
    tree->GetEntry(i);

	  std::vector<TLorentzVector> pis_1 = getPis(pi_1, pi2_1, pi3_1);
	  std::vector<TLorentzVector> pis_2 = getPis(pi_2, pi2_2, pi3_2);
	  
	  TLorentzVector Tauminus = getTau(pi_1, pi2_1, pi3_1, nu_1);
	  TLorentzVector Tauplus = getTau(pi_2, pi2_2, pi3_2, nu_2);
	  
	  TLorentzVector reco_Tauminus = getTau(pi_1, pi2_1, pi3_1, reco_nu_1);
	  TLorentzVector reco_Tauplus = getTau(pi_2, pi2_2, pi3_2, reco_nu_2);
	  
	  TLorentzVector reco2_Tauminus = getTau(pi_1, pi2_1, pi3_1, reco2_nu_1);
	  TLorentzVector reco2_Tauplus = getTau(pi_2, pi2_2, pi3_2, reco2_nu_2);
	  
	  TLorentzVector pola_Tauminus = getTau(pi_1, pi2_1, pi3_1, pola_nu_1);
	  TLorentzVector pola_Tauplus = getTau(pi_2, pi2_2, pi3_2, pola_nu_2);
	  
	  TLorentzVector pola2_Tauminus = getTau(pi_1, pi2_1, pi3_1, pola2_nu_1);
	  TLorentzVector pola2_Tauplus = getTau(pi_2, pi2_2, pi3_2, pola2_nu_2);
	  
	  TLorentzVector pola3_Tauminus = getTau(pi_1, pi2_1, pi3_1, pola3_nu_1);
	  TLorentzVector pola3_Tauplus = getTau(pi_2, pi2_2, pi3_2, pola3_nu_2);
	  
	  TLorentzVector pola4_Tauminus = getTau(pi_1, pi2_1, pi3_1, pola4_nu_1);
	  TLorentzVector pola4_Tauplus = getTau(pi_2, pi2_2, pi3_2, pola4_nu_2);
	  
	  //Arbitrarily choose the charge of the three pions as to respect charge conserv.
	  std::vector<double> charges_1 = {1.00, -1.00, -1.00};
	  std::vector<double> charges_2 = {-1.00, 1.00, 1.00};
	  
	  pv_angle = ic::getPV_angle(Tauminus, pis_1, charges_1, Tauplus, pis_2, charges_2);
	  
	  reco_pv_angle = ic::getPV_angle(reco_Tauminus, pis_1, charges_1, reco_Tauplus, pis_2, charges_2);
	  reco2_pv_angle = ic::getPV_angle(reco2_Tauminus, pis_1, charges_1, reco2_Tauplus, pis_2, charges_2);
	  
	  pola_pv_angle = ic::getPV_angle(pola_Tauminus, pis_1, charges_1, pola_Tauplus, pis_2, charges_2);
	  pola2_pv_angle = ic::getPV_angle(pola2_Tauminus, pis_1, charges_1, pola2_Tauplus, pis_2, charges_2);
	  pola3_pv_angle = ic::getPV_angle(pola3_Tauminus, pis_1, charges_1, pola3_Tauplus, pis_2, charges_2);
	  pola4_pv_angle = ic::getPV_angle(pola4_Tauminus, pis_1, charges_1, pola4_Tauplus, pis_2, charges_2);
		
		TLorentzVector best_nu_1, best_nu_2;
		TVector3 sv_1_vect(sv_1);
		TVector3 sv_2_vect(sv_2);
		
		//making sure we have a proper solution
		bool check_pola = pola::a1_a1_polarimetric(best_nu_1, best_nu_2, Tauminus, Tauplus, sv_1_vect, sv_2_vect, met_x, met_y);
		
		if(check_pola){
			pola9_nu_p_1 = best_nu_1.P();
			pola9_nu_eta_1 = best_nu_1.Eta();
			pola9_nu_phi_1 = best_nu_1.Phi();
			
			pola9_nu_p_2 = best_nu_2.P();
			pola9_nu_eta_2 = best_nu_2.Eta();
			pola9_nu_phi_2 = best_nu_2.Phi();
		}
		
		if (!check_pola){
			pola9_nu_p_1 = -9999;
			pola9_nu_eta_1 = -9999;
			pola9_nu_phi_1 = -9999;
			
			pola9_nu_p_2 = -9999;
			pola9_nu_eta_2 = -9999;
			pola9_nu_phi_2 = -9999;
		}
		
	  pv_angle_branch->Fill();
	  
	  reco_pv_angle_branch->Fill();
	  reco2_pv_angle_branch->Fill();
	  
	  pola_pv_angle_branch->Fill();
	  pola2_pv_angle_branch->Fill();
	  pola3_pv_angle_branch->Fill();
	  pola4_pv_angle_branch->Fill();
		
		pola9_nu_p_1_branch->Fill();
		pola9_nu_eta_1_branch->Fill(); 
		pola9_nu_phi_1_branch->Fill(); 
		
		pola9_nu_p_2_branch->Fill();
		pola9_nu_eta_2_branch->Fill(); 
		pola9_nu_phi_2_branch->Fill(); 
		  
		  
  }
  tree->Write("", TObject::kOverwrite);
  return 0;
}
