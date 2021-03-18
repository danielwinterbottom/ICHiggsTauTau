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

#include "pola.hpp"

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
