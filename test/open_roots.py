import ROOT
from array import array
import uproot
import matplotlib.pyplot as plt
import numpy as np
from pylorentz import Momentum4
from pylorentz import Vector4
from pylorentz import Position4

tree2 = uproot.open("/vols/cms/ac4317/msci/CMSSW_10_2_19/src/UserCode/ICHiggsTauTau/test/Event_cfg.root")["icEventProducer/EventTree"]

tree = uproot.open("/vols/cms/ac4317/msci/CMSSW_10_2_19/src/UserCode/ICHiggsTauTau/test/EventTree.root")["icEventProducer/EventTree"]


#This shows all the available info in my TTree
print(tree2.keys())
tree2.show()

#a = tree2.array('weights_')
#a2 = tree.array('weights_')

# these are the interesting arrays they actually all are leaves within the EventTree subbranches. I only have 10 events
particle_pdgid = tree2.array('genParticles.pdgid_')
particle_E = tree2.array('genParticles.vector_.fCoordinates.fE')
particle_phi = tree2.array('genParticles.vector_.fCoordinates.fPhi')
particle_eta = tree2.array('genParticles.vector_.fCoordinates.fEta')
particle_pt = tree2.array('genParticles.vector_.fCoordinates.fPt')


particle_pdgid_1 = tree.array('genParticles.pdgid_')
particle_E_1 = tree.array('genParticles.vector_.fCoordinates.fE')
particle_phi_1 = tree.array('genParticles.vector_.fCoordinates.fPhi')
particle_eta_1 = tree.array('genParticles.vector_.fCoordinates.fEta')
particle_pt_1 = tree.array('genParticles.vector_.fCoordinates.fPt')

print("\nThis is pt:", particle_pt[:10], '\n', particle_pt_1[:10])
print('\nThis is eta:', particle_eta[:10], '\n', particle_eta_1[:10])
print('\nThis is phi', particle_phi[:10], '\n', particle_phi_1[:10])
print('\nThis is E', particle_E[:10], '\n', particle_E_1[:10])


#the tauspinner and eventInfo Branches do not seem to be saved in the same way which makes it difficult to access the info 
#second = tree2.array('second')

print(particle_pdgid[1])

nu_E = []
nu_phi = []
nu_eta = []
nu_pt = []

nu_E_1 = []
nu_phi_1 = []
nu_eta_1 = []
nu_pt_1 = []

nu_coordinates = []
nu_coordinates_1 = []


#crudely re-arrange the neutrinos
for i in range(len(particle_pdgid)):
    for j in range(len(particle_pdgid[i])):
        particle = particle_pdgid[i][j]
        if particle == 16 or particle == -16 or particle == 14 or particle == -14 or particle == 12 or particle == 12:
            nu_coordinates.append([i,j])
            nu_E.append(particle_E[i][j])
            nu_phi.append(particle_phi[i][j])
            nu_eta.append(particle_eta[i][j])
            nu_pt.append(particle_pt[i][j])


#crudely re-arrange the neutrinos
for i in range(len(particle_pdgid_1)):
    for j in range(len(particle_pdgid_1[i])):
        particle = particle_pdgid_1[i][j]
        if particle == 16 or particle == -16 or particle == 14 or particle == -14 or particle == 12 or particle == 12:
            nu_coordinates_1.append([i,j])
            nu_E_1.append(particle_E_1[i][j])
            nu_phi_1.append(particle_phi_1[i][j])
            nu_eta_1.append(particle_eta_1[i][j])
            nu_pt_1.append(particle_pt_1[i][j])


nu_coordinates = np.array(nu_coordinates)
nu_E = np.array(nu_E)
nu_phi = np.array(nu_phi)
nu_eta = np.array(nu_eta)
nu_pt = np.array(nu_pt)


print('This is E', nu_E_1-nu_E[:28],'This is eta', nu_eta_1, 'This is phi', nu_phi_1, 'This is pt', nu_pt_1)

nu_4mom =  Momentum4.e_eta_phi_pt(nu_E, nu_eta, nu_phi, nu_pt)



print(nu_4mom.e**2 - nu_4mom.p2)
#This is the wird bit with gen_weight and wt_mc_sign
#print(a[7])
#print(a[8][b'gen_weight'], a2[8][b'gen_weight'])
#wt_mc_sign = []
#gen_weights = []

#for i in range(len(a)):
    #gen_weights.append(a[i][b'gen_weight'])
    #wt_mc_sign.append(a[i][b'wt_mc_sign'])



#some lotting, it is all the same
#plt.subplot(2,1,1)
#plt.ylabel('wt_mc_sign')    
#plt.plot(wt_mc_sign,'x')
#plt.grid()
    
#plt.subplot(2,1,2)
#plt.plot(gen_weights,'x')
#plt.grid()
#plt.ylabel('gen_weight')

#plt.xlabel('event nb')
#plt.show()
    
raise End
