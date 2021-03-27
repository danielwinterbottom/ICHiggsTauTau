pola_module.hpp contains two functions of interest, called appropriately in the master_pv_angle_a1_h.cpp code:

getPV_angle_pola: 
    
    To be used in a1-a1 channel.

    inputs - std::vector<TLorentzVector> pis_1, std::vector<TLorentzVector> pis_2, TVector3 sv_1, TVector3 sv_2, double met_x, double met_y
    
    1. Calculates using a1_a1_polarimetric the polarimetric neutrinos with the updated polarimetric method which:
        a. reshifts the tau direction from the sv direction to the closest direction to sv physically allowed.
        b. includes met constraints for better picking of the best polarimetric solution
        
    2. Using the best polarimetric neutrinos - calculates the pv_angle i.e. the angle between the two planes formed by the reconstructed taus and the poolarimetric vectors for the a1
    

getIPPV_angle:

    To be used in a1-(pi/rho) channels.
    
    inputs - std::vector<TLorentzVector> sumPionsMinus, TLorentzVector IP_vect_1, int mva_dm1, std::vector<TLorentzVector> sumPionsPlus, TLorentzVector IP_vect_2, int mva_dm2
    
    1. Calculates the a1 polarimetric direction in the a1-(pi/rho) ZMF frame
    
    2. In the same frame, forms the ippv angle following: 
    https://docs.google.com/presentation/d/1JP6dByO984HbkyQe4tVFTXkuHgTVnbPFIbmiChLLp1A/edit#slide=id.g79f9017c11_0_4
    
    3. For a1-rho cases: also performs the shift with respect to y for removing polarisations cancelling out.
    
    
    
    
Code conjointly written by A. Craplet (alie.craplet17@imperial.ac.uk) and K. Collie (kingsley.collie17@imperial.ac.uk). 

The pola.hpp code is another, older but working version of pola_module.cpp. The main difference is in the inputs of the getIPPV_angle function. 
