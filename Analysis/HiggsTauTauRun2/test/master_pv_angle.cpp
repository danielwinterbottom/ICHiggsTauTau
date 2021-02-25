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

TLorentzVector getTau(Particle pi, Particle pi2, Particle pi3, PEtaPhi nu){
    TLorentzVector pi_L = TLorentzVector(pi.px, pi.py, pi.pz, pi.E);
    TLorentzVector pi2_L = TLorentzVector(pi2.px, pi2.py, pi2.pz, pi2.E);
    TLorentzVector pi3_L = TLorentzVector(pi3.px, pi3.py, pi3.pz, pi3.E);
    TLorentzVector nu_L = neutrinoToLorentz(nu);
    TLorentzVector tau = pi_L + pi2_L + pi3_L + nu_L;
    
    return tau;
}

namespace ic {
    void getPV_angle(TLorentzVector Tauminus, std::vector<TLorentzVector> pis_1, std::vector<double> charges_1, TLorentzVector Tauplus, std::vector<TLorentzVector> pis_2, std::vector<double> charges_2){
        
        
        SCalculator Scalc("a1");
        
//         TLorentzVector Higgs_frame = Tauplus + Tauminus;
//         TLorentzVector Tauplus_HRF = Scalc.Boost(Tauplus, Higgs_frame);
//         TLorentzVector Tauminus_HRF = Scalc.Boost(Tauminus, Higgs_frame);
//         
//         TLorentzVector Higgs_Restframe = Scalc.Boost(Higgs_frame, Higgs_frame);
//         
//         double pz=Tauplus.Pz();
//         std::cout<<"Hey tau plus"<<pz<<std::endl;
//         
//         double pz_H=Higgs_frame.Pz();
//         std::cout<<"Hey Higgs_frame"<<pz_H<<std::endl;
//     
//         double pz_THRF=Tauplus_HRF.Pz();
//         std::cout<<"Hey tau_HRF plus"<<pz_THRF<<std::endl;
//         
//         double pz_HHRF=Higgs_Restframe.Pz();
//         std::cout<<"Hey H_HRF plus"<<pz_HHRF<<std::endl;
//         
        
        double angle = -9999.;
        if(Scalc.isOk("a1", "a1", Tauminus, pis_1, charges_1, Tauplus, pis_2, charges_2)){
            angle = Scalc.AcopAngle("a1", "a1", Tauminus, pis_1, charges_1, Tauplus, pis_2, charges_2);
            std::cout << "\nGood variables - Angle: " << angle << '\n';
        }
        else {
            std::cout << "Wrong variables";
        }
    }

}

int main(int argc, char* argv[])
{
	std::string inputFilename(argv[1]);
	std::string neutrinoLevel = "gen";
	TFile oldFile(inputFilename.c_str(), "READ");
	if (oldFile.IsZombie())
        {
                std::cerr << "File didn't load correctly." << std::endl;
                return -1;
        }
	TTree *tree = static_cast<TTree*>(oldFile.Get("ntuple"));
    std::cout << "Read file." << std::endl;
	
	// Setup particles
	Particle pi_1, pi2_1, pi3_1;	
	Particle pi_2, pi2_2, pi3_2;
	setupParticle(tree, "pi", pi_1, -211, 1);
	setupParticle(tree, "pi", pi_2, 211, 2);
	setupParticle(tree, "pi2", pi2_1, -211, 1);
	setupParticle(tree, "pi2", pi2_2, 211, 2);
	setupParticle(tree, "pi3", pi3_1, 211, 1);
	setupParticle(tree, "pi3", pi3_2, -211, 2);
	//a1-a1(3pr) channel - no need for pi0s
//	setupParticle(tree, "pi0", pi0_1, 111, 1);
//	setupParticle(tree, "pi0", pi0_2, 111, 2);//
		// Set neutrinos to read from gen for now
	PEtaPhi nu_1, nu_2;
	setupNeutrino(tree, (neutrinoLevel+"_nu").c_str(), nu_1, 16, 1);
	setupNeutrino(tree, (neutrinoLevel+"_nu").c_str(), nu_2, -16, 2);
	//                                                                                                                 
	std::cout << "Up to now, works:" << std::endl;
    
    //tree->GetEntries()
    for (int i = 0, nEntries = 10; i < nEntries; i++)
    {
        tree->GetEntry(i);
    
        std::vector<TLorentzVector> pis_1 = getPis(pi_1, pi2_1, pi3_1);
        std::vector<TLorentzVector> pis_2 = getPis(pi_2, pi2_2, pi3_2);
        
        TLorentzVector Tauminus = getTau(pi_1, pi2_1, pi3_1, nu_1);
        TLorentzVector Tauplus = getTau(pi_2, pi2_2, pi3_2, nu_2);
        
        //Arbitrarily choose the charge of the three pions as to respect charge conserv.
        std::vector<double> charges_1 = {-1.00, -1.00, 1.00};
        std::vector<double> charges_2 = {-1.00, 1.00, 1.00};
        
        //double angle;
        ic::getPV_angle(Tauminus, pis_1, charges_1, Tauplus, pis_2, charges_2);
        
    }
    
    
//     double angle = SCalculator::AcopAngle("a1", "a1", Tauminus, pis_1, charges_1, Tauplus, pis_2, charges_2);
//     std::cout << "\nwe have the angle\n";
//     if(Scalc.isOk("a1", "a1", Tauminus, pis_1, charges_1, Tauplus, pis_2, charges_2)) //(security checks, if there is no problem with variables)
//     {
//        angle = Scalc.AcopAngle("a1", "a1", Tauminus, pis_1, charges_1, Tauplus, pis_2, charges_2);
//        std::cout << "\nwe have the angle\n";
    }

    
    
    
	//PolarimetricA1A1(svminuspv_1, svminuspv_2, a1_1, a1_2, pis_1, pis_2, charges_1, charges_2);
