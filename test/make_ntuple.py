import ROOT
from array import array

inFile = ROOT.TFile.Open('MVAFILE_AllHiggs_tt.root', "READ")

tree = inFile.Get("ntuple")


momenta_features = [ "pi_E_1", "pi_px_1", "pi_py_1", "pi_pz_1", #leading charged pi 4-momentum
              "pi_E_2", "pi_px_2", "pi_py_2", "pi_pz_2", #subleading charged pi 4-momentum
              "pi0_E_1","pi0_px_1","pi0_py_1","pi0_pz_1", #leading neutral pi 4-momentum
              "pi0_E_2","pi0_px_2","pi0_py_2","pi0_pz_2", #subleading neutral pi 4-momentum
              "gen_nu_p_1", "gen_nu_phi_1", "gen_nu_eta_1", #leading neutrino, gen level
              "gen_nu_p_2", "gen_nu_phi_2", "gen_nu_eta_2", #subleading neutrino, gen level  
              #"pi2_E_1", "pi2_px_1", "pi2_py_1", "pi2_pz_1",
              #"pi3_E_1", "pi3_px_1", "pi3_py_1", "pi3_pz_1",
              #"pi2_E_2", "pi2_px_2", "pi2_py_2", "pi2_pz_2",
              #"pi3_E_2", "pi3_px_2", "pi3_py_2", "pi3_pz_2"
                ] 

other_features = [ "ip_x_1", "ip_y_1", "ip_z_1",        #leading impact parameter
                   "ip_x_2", "ip_y_2", "ip_z_2",        #subleading impact parameter
                   #"y_1_1", "y_1_2",
                   "gen_phitt", "ip_sig_2", "ip_sig_1"
                 ]    # ratios of energies

target = [ "met", "metx", "mety", #"aco_angle_1", "aco_angle_6", "aco_angle_5", "aco_angle_7"
         ]  #acoplanarity angle
    
selectors = [ "tau_decay_mode_1","tau_decay_mode_2",
             "mva_dm_1","mva_dm_2","rand","wt_cp_ps","wt_cp_sm",
            ]

additional_info = [ "sv_x_1", "sv_y_1", "sv_z_1",
                    "sv_x_2", "sv_y_2", "sv_z_2",
                    ]

sv_covariance_matrices = ["svcov00_1", "svcov01_1", "svcov02_1",
                       "svcov10_1", "svcov11_1", "svcov12_1", 
                       "svcov20_1", "svcov21_1", "svcov22_1", 
                       "svcov00_2", "svcov01_2", "svcov02_2",
                       "svcov10_2", "svcov11_2", "svcov12_2", 
                       "svcov20_2", "svcov21_2", "svcov22_2", 
    
]

ip_covariance_matrices = ["ipcov00_1", "ipcov01_1", "ipcov02_1",
                       "ipcov10_1", "ipcov11_1", "ipcov12_1", 
                       "ipcov20_1", "ipcov21_1", "ipcov22_1", 
                       "ipcov00_2", "ipcov01_2", "ipcov02_2",
                       "ipcov10_2", "ipcov11_2", "ipcov12_2", 
                       "ipcov20_2", "ipcov21_2", "ipcov22_2", 
    
]

met_covariance_matrices = ["metcov00", 
                           "metcov01", 
                           "metcov10", 
                           "metcov11" ]

covs = sv_covariance_matrices + ip_covariance_matrices + met_covariance_matrices

variables=(momenta_features+other_features+target+selectors+additional_info)

def decayMode(tree_name, tau1 = 1, tau2 = 1, mva1 = 10, mva2 = 10): #default a1 a1 decay
    #print (tree_name.tau_decay_mode_1)
    if tree_name.tau_decay_mode_1 == tau1 and tree_name.tau_decay_mode_2 == tau2 and tree_name.mva_dm_1 == mva1 and tree_name.mva_dm_2 == mva2 :# and tree_name.gen_nu_p_1 >= -4000 and tree_name.gen_nu_p_2 >= -4000:
        return True
    else:
        return False
    


#file = ROOT.TFile("rho_rho.root", "RECREATE") 1,1,1,1
#file = ROOT.TFile("a1_3pi_a1_3pi.root", "RECREATE") #1,1,10,10
file = ROOT.TFile("a1_3pi_a1_3pi.root", "RECREATE") #10,10,10,10
nbEvents = 10000

tree_out = ROOT.TTree("ntuple", "MyNTuple")


for name in variables:
    exec("%s  = array('d',[0])" % (name))
    exec("tree_out.Branch('%s', %s, 'normal/D')"%(name, name))
    
w=0
for i in range(0,tree.GetEntries()):
         tree.GetEntry(i)
         #print(i)
         if decayMode(tree, 10, 10, 10, 10) == True and w <= nbEvents:
             print ('Hey', w, i)
             for name in variables:
                 exec("%s[0] = getattr(tree, '%s')"%(name, name))
                 tree_out.Fill()
             w=w+1
             
         if w >= nbEvents :
             break
         
tree_out.SetDirectory(0)
inFile.Close()


file.cd()
tree_out.Write()
file.Close()


        
             
