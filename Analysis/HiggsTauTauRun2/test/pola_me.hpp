#include "TMath.h"
#include "TLorentzVector.h"

#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SCalculator.h"

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
		double m_vis = tau_vis.M();
		if (m_vis == 0)
		{
			m_vis = 1.205;
		}
		
		// Fail if magnitudes are 0
		if (sv.Mag2() == 0 || tau_vis.Vect().Mag2() == 0)
		{
			return false;
		}
		
		TVector3 vis_dir = tau_vis.Vect().Unit();
		TVector3 tau_dir = sv.Unit();
    double theta_GJ = std::acos(std::clamp(tau_dir.Dot(vis_dir.Unit()), -1.0, 1.0));
    double theta_GJ_max = std::asin(std::clamp((m_tau*m_tau - m_vis*m_vis)/(2*m_tau*tau_vis.P()), -1.0, 1.0));
		
		TVector3 new_dir;
		// Rotate tau back if theta_GJ is in unphysical region
		if (theta_GJ > theta_GJ_max)
		{
			theta_GJ = theta_GJ_max;
			// Create a normalised vector prependicular to a1
			double n_1_x = 1/std::sqrt(1+std::pow(tau_vis.X()/tau_vis.Y(), 2));
			double n_1_y = -n_1_x * tau_vis.X()/tau_vis.Y();
			TVector3 n_1(n_1_x, n_1_y, 0);
			// create n_2, a unit vector perpendicular to n_1 and the a1
			TVector3 n_2 = n_1.Cross(tau_vis.Vect()).Unit();
			
			// optimal phi from calculus
			double phi_opt_1 = std::atan(sv.Dot(n_2)/sv.Dot(n_1));
			TVector3 new_dir_1 = std::cos(theta_GJ)*vis_dir + std::sin(theta_GJ)*(std::cos(phi_opt_1)*n_1 + std::sin(phi_opt_1)*n_2);
			
			// tan so can have phi+pi solution
			double phi_opt_2 = phi_opt_1 + M_PI;
			TVector3 new_dir_2 = std::cos(theta_GJ)*vis_dir + std::sin(theta_GJ)*(std::cos(phi_opt_2)*n_1 + std::sin(phi_opt_2)*n_2);
			
			// test which solution maximises dot product
			if ( new_dir_1.Dot(sv) > new_dir_2.Dot(sv) )
			{
				new_dir = new_dir_1;
			}
			else
			{
				new_dir = new_dir_2;
			}
		}
		else
		{
			new_dir = tau_dir;
		}
		
		// Calculate the tau momentum
    double minus_b = (m_vis*m_vis + m_tau*m_tau) * tau_vis.P() * std::cos(theta_GJ);
    double two_a = 2*(m_vis*m_vis + std::pow(tau_vis.P(), 2) * std::pow(std::sin(theta_GJ), 2));

    double b_squared_m_four_ac = (m_vis*m_vis + std::pow(tau_vis.P(), 2)) * (std::pow(m_vis*m_vis - m_tau*m_tau, 2) - 4*m_tau*m_tau*std::pow(tau_vis.P(), 2)*std::pow(std::sin(theta_GJ), 2));
    b_squared_m_four_ac = std::max(b_squared_m_four_ac, 0.0);
    
    // two solutions for tau momentum magnitude
    double sol_1 = (minus_b + std::sqrt(b_squared_m_four_ac))/two_a;
    double sol_2 = (minus_b - std::sqrt(b_squared_m_four_ac))/two_a;
		
    tau_sol_1 = TLorentzVector(sol_1*new_dir, std::sqrt(sol_1*sol_1+m_tau*m_tau));
    tau_sol_2 = TLorentzVector(sol_2*new_dir, std::sqrt(sol_2*sol_2+m_tau*m_tau));
		
		return true;
	}
	
	
	/** Calculates the best neutrinos using the polarimetric method.
	    outputs true if successful or false if best neutrinos could not be found.	    input sum of visisble products, met and secondary vertices for each tau. */
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
				TLorentzVector nu_1 = tau_1 - tau_1_vis;
				TLorentzVector nu_2 = tau_2 - tau_2_vis;
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
	double getPV_angle(TLorentzVector Tauminus, std::vector<TLorentzVector> pis_1, std::vector<double> charges_1, TLorentzVector Tauplus, std::vector<TLorentzVector> pis_2, std::vector<double> charges_2)
	{
		std::cout << pis_2.size() << std::endl;
		SCalculator Scalc("a1");
		double angle = -9999.;
		if(Scalc.isOk("a1", "a1", Tauminus, pis_1, charges_1, Tauplus, pis_2, charges_2)){
			std::cout << "It's ok 5 \n"<< std::endl;
			angle = Scalc.AcopAngle("a1", "a1", Tauminus, pis_1, charges_1, Tauplus, pis_2, charges_2);
			std::cout << "\nGood variables - Angle: " << angle << '\n';
		}
		else {
			std::cout << "Wrong variables" << std::endl;
		}
		return angle;
	}
	
	double getPV_angle_pola(std::vector<TLorentzVector> pis_1, std::vector<TLorentzVector> pis_2,
	                        TVector3 sv_1, TVector3 sv_2,
	                        double met_x, double met_y)
	{
		// Visibles are the sums of the pis
		TLorentzVector tau_1_vis;
		for(auto pi : pis_1)
		{
			tau_1_vis += pi;
		}

		TLorentzVector tau_2_vis;
		for(auto pi : pis_2)
		{
			tau_2_vis += pi;
		}
		
		TLorentzVector nu_1;
		TLorentzVector nu_2;
		
		// Reconstruct the neutrinos using polarimetric method, if this fails output fail marker value
		if(!pola::a1_a1_polarimetric(nu_1, nu_2, tau_1_vis, tau_2_vis, sv_1, sv_2, met_x, met_y))
		{
			return -9999;
		}
		
		// Setup variables needed for Scalc
		std::vector<double> charges_1 = {1.00, -1.00, -1.00};
		std::vector<double> charges_2 = {-1.00, 1.00, 1.00};
		
		TLorentzVector Tauminus = tau_1_vis + nu_1;
		TLorentzVector Tauplus = tau_2_vis + nu_2;
		
		SCalculator Scalc("a1");
		if(!Scalc.isOk("a1", "a1", Tauminus, pis_1, charges_1, Tauplus, pis_2, charges_2))
		{
			std::cout << "Wrong variables." << std::endl;
			return -9999;
		}
		
		return Scalc.AcopAngle("a1", "a1", Tauminus, pis_1, charges_1, Tauplus, pis_2, charges_2);
  }
	
	double getIPPV_angle(TLorentzVector Tauminus, std::vector<TLorentzVector> sumPionsMinus, TLorentzVector IP_vect_1, int mva_dm1,
	                     TLorentzVector Tauplus, std::vector<TLorentzVector> sumPionsPlus, TLorentzVector IP_vect_2, int mva_dm2)
	{
		if ((mva_dm1 == 10))
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
			
			if (isnan(angle)){
				angle = -9999;
			}
			//std::cout << "\nAngle: " << angle << '\n';
			return angle;
		}
		
		if (mva_dm2 == 10)
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
				angle = (2.*TMath::Pi()-TMath::ATan2((k1.Cross(k2)).Mag(),k1*k2));
			}
			
			if (isnan(angle)){
				angle = -9999;
			}
			//std::cout << "\nAngle: " << angle << '\n';
			return angle;
			}
		//could add something to calculate in the a1-a1 channel, make it cleaner and more compact
		//in case where we are not in an a1-pi decay, ippv = -9999
		else{
			return -9999;
		}
	}
}
