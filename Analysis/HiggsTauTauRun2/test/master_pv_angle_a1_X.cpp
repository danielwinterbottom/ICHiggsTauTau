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

struct Vector3
{
    double x;
    double y;
    double z;
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

void setupMVA(TTree *a_tree, int &mva_dm, int a_fromTau)
{
        a_tree->SetBranchAddress(("mva_dm_"+std::to_string(a_fromTau)).c_str(), &mva_dm);
}

void setupIP(TTree *a_tree, Vector3 &IP_direction, int a_fromTau)
{
        a_tree->SetBranchAddress(("ip_x_"+std::to_string(a_fromTau)).c_str(), &IP_direction.x);
        a_tree->SetBranchAddress(("ip_y_"+std::to_string(a_fromTau)).c_str(), &IP_direction.y);
        a_tree->SetBranchAddress(("ip_z_"+std::to_string(a_fromTau)).c_str(), &IP_direction.z);
}

TLorentzVector neutrinoToLorentz(const PEtaPhi &a_PEtaPhi)
{
        double pt = a_PEtaPhi.P/std::cosh(a_PEtaPhi.Eta);
        ROOT::Math::PtEtaPhiM4D<double> particle(pt, a_PEtaPhi.Eta, a_PEtaPhi.Phi, 0);
return TLorentzVector(particle.Px(), particle.Py(), particle.Pz(), particle.E());
}


TLorentzVector particleToLorentz(Particle &particle)
{
return TLorentzVector(particle.px, particle.py, particle.pz, particle.E);
}

/*Funtion to produce the vactor of pions, used by the AcopAngle method that
 * will then boost them in the Higgs rest frame*/
std::vector<TLorentzVector> getPis(Particle pi, Particle pi2, Particle pi3){
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

TLorentzVector getVis(Particle pi, Particle pi2, Particle pi3){
    TLorentzVector pi_L = TLorentzVector(pi.px, pi.py, pi.pz, pi.E);
    TLorentzVector pi2_L = TLorentzVector(pi2.px, pi2.py, pi2.pz, pi2.E);
    TLorentzVector pi3_L = TLorentzVector(pi3.px, pi3.py, pi3.pz, pi3.E);
    TLorentzVector tau = pi_L + pi2_L + pi3_L;
    //The neutrino is not used but keeping the same structure is easier
    return tau;
}

/*This is calculating the pv angle         
 *Note: the AcopAngle gets the pv angle for a1 a1s:
 *https://github.com/danielwinterbottom/ICHiggsTauTau/blob/c21542125ed10f82d01ca2ae3e4286abcba8d4f6/Analysis/Utilities/src/SCalculator.cc#L260*/
namespace ic {
	double getPV_angle(TLorentzVector Tauminus, std::vector<TLorentzVector> pis_1, std::vector<double> charges_1, TLorentzVector Tauplus, std::vector<TLorentzVector> pis_2, std::vector<double> charges_2){
		std::cout << pis_2.size() << std::endl;
		SCalculator Scalc("a1");
		double angle = -9999.;
		if(Scalc.isOk("a1", "pion", Tauminus, pis_1, charges_1, Tauplus, pis_2, charges_2)){
			std::cout << "It's ok 5 \n"<< std::endl;
			angle = Scalc.AcopAngle("a1", "pion", Tauminus, pis_1, charges_1, Tauplus, pis_2, charges_2);
			std::cout << "\nGood variables - Angle: " << angle << '\n';
		}
		else {
			std::cout << "Wrong variables";
		}
		return angle;
	}

	double getIPPV_angle(TLorentzVector Tauminus, std::vector<TLorentzVector> sumPionsMinus, TLorentzVector IP_vect_1, int mva_dm1, TLorentzVector Tauplus, std::vector<TLorentzVector> sumPionsPlus, TLorentzVector IP_vect_2, int mva_dm2){
		if ((mva_dm1 == 10) && (mva_dm2 == 0 || mva_dm2 == 1))
		{
			std::vector<TLorentzVector> tauandprodminus;
			TLorentzVector a1 = Tauminus;
			TLorentzVector pi = sumPionsPlus.at(0);
			TLorentzVector ZMF = a1 + Tauplus;
			SCalculator Scalc1("a1");
			tauandprodminus.push_back(Tauminus);
			for(unsigned int i=0; i<sumPionsMinus.size();i++) {
				tauandprodminus.push_back(sumPionsMinus.at(i));
			}
			//in lab frame we have the direction of the pion, of the a1 of the IP and of the pv_a1 (already calculated in ZMF)
			Scalc1.Configure(tauandprodminus, Tauminus+Tauplus, -1);
			TVector3 pv_vect=Scalc1.pv();
			TVector3 a1_vect(a1.Px(), a1.Py(), a1.Pz());
			//Normalise
			//TVector3 IP_vect = IP_vect_2.Unit();
			TLorentzVector IP = IP_vect_2;
			TVector3 PV_vect_ZMF = pv_vect.Unit();
			//form 4vectors and boost everything in visible decay products frame

			//TLorentzVector IP(IP_vect(0), IP_vect(1), IP_vect(2), 0);
			

			TLorentzVector IP_ZMF(Scalc1.Boost(IP, ZMF));
			TLorentzVector pi_ZMF(Scalc1.Boost(pi, ZMF));
			TLorentzVector a1_ZMF(Scalc1.Boost(a1, ZMF));
			//Take the normal to the planes
			TVector3 IP_vect_ZMF(IP_ZMF.Vect()); 
			TVector3 pi_vect_ZMF(pi_ZMF.Vect());
			TVector3 a1_vect_ZMF(a1_ZMF.Vect());
			//Same parameter as is done in the code instead of just cos
			TVector3 h1 = PV_vect_ZMF;
			TVector3 h2 = IP_vect_ZMF;
			TVector3 tauminus_HRF = a1_vect_ZMF;
			TVector3 tauplus_HRF = pi_vect_ZMF;
			TVector3 k1 = (h1.Cross(tauminus_HRF.Unit())).Unit();
			TVector3 k2 = (h2.Cross(tauplus_HRF.Unit())).Unit();
					
			double angle = -9999;

			if(((h1.Cross(h2))*(tauminus_HRF.Unit()))<=0){
				angle = TMath::ATan2((k1.Cross(k2)).Mag(),k1*k2);}
			else{
				angle = (2.*TMath::Pi()-TMath::ATan2((k1.Cross(k2)).Mag(),k1*k2));}
			
			//not do this shift in a1-rho case - debugging
//             if (mva_dm2 == 1){
//                 angle = TMath::ACos(k1.Dot(k2));
//             }
			
			if (isnan(angle)){
				angle = -9999;
			}
			std::cout << "\nAngle: " << angle << '\n';
			return angle;
		}
		
		if ((mva_dm1 == 0 || mva_dm1 == 1) && (mva_dm2 == 10))
		{
		//std::cout << "You are here\n";
			std::vector<TLorentzVector> tauandprodplus;
			TLorentzVector a1 = Tauplus;
			TLorentzVector pi = sumPionsMinus.at(0);
			TLorentzVector ZMF = a1 + Tauminus;
			SCalculator Scalc1("a1");
			tauandprodplus.push_back(Tauplus);
			for(unsigned int i=0; i<sumPionsPlus.size();i++) {
				tauandprodplus.push_back(sumPionsPlus.at(i));
			}
			//in lab frame we have the direction of the pion, of the a1 of the IP and of the pv_a1
			Scalc1.Configure(tauandprodplus, Tauminus+Tauplus, -1);
			TVector3 pv_vect=Scalc1.pv();
			
			TVector3 a1_vect(a1.Px(), a1.Py(), a1.Pz());
			//Normalise
			//TVector3 IP_vect = IP_vect_1.Unit();
			TVector3 PV_vect_ZMF = pv_vect.Unit();
			//a1_vect = a1_vect/a1_vect.Mag();
			//form 4vectors and boost everything in visible decay products frame
			//TLorentzVector IP(IP_vect(0), IP_vect(1), IP_vect(2), 0);
			//TLorentzVector PV(pv_vect(0), pv_vect(1), pv_vect(2), 0);
			
			TLorentzVector IP = IP_vect_1;
			
			TLorentzVector IP_ZMF(Scalc1.Boost(IP, ZMF));
			TLorentzVector pi_ZMF(Scalc1.Boost(pi, ZMF));
			//TLorentzVector PV_ZMF(Scalc1.Boost(PV, ZMF));
			TLorentzVector a1_ZMF(Scalc1.Boost(a1, ZMF));
			//Take the normal to the planes
			TVector3 IP_vect_ZMF(IP_ZMF.Vect()); 
			TVector3 pi_vect_ZMF(pi_ZMF.Vect());
			//TVector3 PV_vect_ZMF(PV_ZMF.Vect());
			TVector3 a1_vect_ZMF(a1_ZMF.Vect());
			//Same parameter as is done in the code instead of just cos
			TVector3 h2 = PV_vect_ZMF;
			TVector3 h1 = IP_vect_ZMF;
			TVector3 tauplus_HRF = a1_vect_ZMF;
			TVector3 tauminus_HRF = pi_vect_ZMF;
			TVector3 k1 = (h1.Cross(tauminus_HRF.Unit())).Unit();
			TVector3 k2 = (h2.Cross(tauplus_HRF.Unit())).Unit();
			double angle = -9999;  
			
		// double y = (sumPionsMinus.at(0) - sumPionsMinus.at(1))/(sumPionsMinus.at(0) + sumPionsMinus.at(1))
			
			//this is the bigO shift
			if(((h1.Cross(h2))*(tauminus_HRF.Unit()))<=0){
				angle = TMath::ATan2((k1.Cross(k2)).Mag(),k1*k2);}
				
			else{
				angle = (2.*TMath::Pi()-TMath::ATan2((k1.Cross(k2)).Mag(),k1*k2));}
			

			
			if (isnan(angle)){
				angle = -9999;
			}
			std::cout << "\nAngle: " << angle << '\n';
			return angle;
			}
		//could add something to calculate in the a1-a1 channel, make it cleaner and more compact
		//in case where we are not in an a1-pi decay, ippv = -9999
		else{
			return -9999;
		}
	}
}

int main(int argc, char* argv[])
{
	std::string inputFilename(argv[1]);
	std::string outputFilename(argv[2]);
	std::string neutrinoLevel = "gen";
	
	TFile oldFile(inputFilename.c_str(), "READ");
	if (oldFile.IsZombie())
	{
		std::cerr << "File didn't load correctly." << std::endl;
		return -1;
	}
	TTree *oldTree = static_cast<TTree*>(oldFile.Get("ntuple"));
	
	TFile newFile(outputFilename.c_str(), "recreate");
	std::cout << "Beginning cloning tree." << std::endl;
	std::cout << "It's ok 1 \n" << std::endl;
	TTree *tree = oldTree->CloneTree();
	std::cout << "Clone finished." << std::endl;
    
	double ippv_angle;
    //Set-up write-up branches

    //choosing to call this new angle ippv
	TBranch *ippv_angle_branch = tree->Branch("ippv_angle", &ippv_angle, "ippv_angle/D");

	// Setup particles
	Particle pi_1, pi2_1, pi3_1;	
	Particle pi_2, pi2_2, pi3_2;
	Particle pi0_1, pi0_2;
	Vector3 IP_1, IP_2;
	setupParticle(tree, "pi", pi_1, -211, 1);
	setupParticle(tree, "pi", pi_2, 211, 2);
	setupParticle(tree, "pi2", pi2_1, -211, 1);
	setupParticle(tree, "pi3", pi3_1, 211, 1);
	setupParticle(tree, "pi2", pi2_2, 211, 2);
	setupParticle(tree, "pi3", pi3_2, -211, 2);
	setupParticle(tree, "pi0", pi0_1, 111, 1);
	setupParticle(tree, "pi0", pi0_2, 111, 2);
	setupIP(tree, IP_1, 1);
	setupIP(tree, IP_2, 2);
    
	int mva_dm1, mva_dm2;
	setupMVA(tree, mva_dm1, 1);
	setupMVA(tree, mva_dm2, 2);
    
    /*Loop over the tree entries*/
    //tree->GetEntries()
	for (int i = 0, nEntries = tree->GetEntries(); i < nEntries; i++)
	{   
		tree->GetEntry(i);
    
		std::vector<TLorentzVector> pis_1;
		std::vector<TLorentzVector> pis_2;
        
		TLorentzVector Tauminus;
		TLorentzVector Tauplus;
        
		TLorentzVector IP_vect_1(IP_1.x, IP_1.y, IP_1.z, 0);
		TLorentzVector IP_vect_2(IP_2.x, IP_2.y, IP_2.z, 0);
		ippv_angle = -9999;
		if ((mva_dm1 == 10) && (mva_dm2 == 0))
		{
			Tauminus = getVis(pi_1, pi2_1, pi3_1);
			Tauplus = TLorentzVector(pi_2.px, pi_2.py, pi_2.pz, pi_2.E);
			pis_1 = getPis(pi_1, pi2_1, pi3_1);
			pis_2 = {TLorentzVector(pi_2.px, pi_2.py, pi_2.pz, pi_2.E)};
			ippv_angle = ic::getIPPV_angle(Tauminus, pis_1, IP_vect_1, mva_dm1, Tauplus, pis_2, IP_vect_2, mva_dm2);
		}
         
		if ((mva_dm1 == 0) && (mva_dm2 == 10))
		{
			Tauminus = TLorentzVector(pi_1.px, pi_1.py, pi_1.pz, pi_1.E);
			Tauplus = getVis(pi_2, pi2_2, pi3_2);
			pis_1 = {TLorentzVector(pi_1.px, pi_1.py, pi_1.pz, pi_1.E)};
			pis_2 = getPis(pi_2, pi2_2, pi3_2);
			ippv_angle = ic::getIPPV_angle(Tauminus, pis_1, IP_vect_1, mva_dm1, Tauplus, pis_2, IP_vect_2, mva_dm2);
		}
        

		if ((mva_dm1 == 10) && (mva_dm2 == 1))
		{
			Tauminus = getVis(pi_1, pi2_1, pi3_1);
			Tauplus = particleToLorentz(pi_2) + particleToLorentz(pi0_2);
			pis_1 = getPis(pi_1, pi2_1, pi3_1);
			pis_2 = {particleToLorentz(pi_2), particleToLorentz(pi0_2)};
            //in the case of a rho particle the IP is replaced by the direction of the pi0
			IP_vect_2 = particleToLorentz(pi0_2);
			ippv_angle = ic::getIPPV_angle(Tauminus, pis_1, IP_vect_1, mva_dm1, Tauplus, pis_2, IP_vect_2, mva_dm2);
		}
        
		if ((mva_dm1 == 1) && (mva_dm2 == 10))
		{
			Tauminus = particleToLorentz(pi_1) + particleToLorentz(pi0_1);
			Tauplus = getVis(pi_2, pi2_2, pi3_2);
			pis_1 = {particleToLorentz(pi_1), particleToLorentz(pi0_1)};
			pis_2 = getPis(pi_2, pi2_2, pi3_2);
			IP_vect_1 = particleToLorentz(pi0_1);
			ippv_angle = ic::getIPPV_angle(Tauminus, pis_1, IP_vect_1, mva_dm1, Tauplus, pis_2, IP_vect_2, mva_dm2);
		}
        
		ippv_angle_branch->Fill();

	}
	tree->Write("", TObject::kOverwrite);
	return 0;
}

    
