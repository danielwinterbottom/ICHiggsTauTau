 This files containst a check list of todos for 2017 data/MC. Item can be checked off once completed and additional items should be added to the list as the come up. Try and keep the list ordered by importance (item that need to be completed sooner nearer the top of the list).

- [x] Pile-up reweighting: Find or produce the pileup profile for 2017 data, first check the HTT twiki to see if one exists already or if not produce this (instructions for producing this are here https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJSONFileforData). Find / produce pileup profile for MC (I'm not sure if this is centrally proeduced or not so might requre some digging). Since the pileup profile is different for 2017 this we need to check the range/binning of the pileup histograms is sufficient (i.e don't just use the same bining as for 2016.). Check pileup modelling using n_vtx and/or rho variables

- [ ] General analyser improvements (these don't need to be made all at once but can be done as they come up): We have increasing issues with the jobs using too much memory or needing to be split up alot more (to reduce memory usage and/or run time). These issues are due to the larger number of systematic shifts that we now need to run e.g tau ES in bins of DM, JES by region ect.. We should try and improve the analysis code as we go along to try and tackle these issues e.g for the JES shifts it would be better to avoid running the analyser again for each JES shift and instead just repeating the parts of the code that compute the jet variables. These could then just store a vectors on the output trees. This can however require quite a bit of restructuring to the code since the JES uncertainties should also be propogated to the MET and therefore also the SV fit mass - we would need to change how we run the SV fit jobs for this examples. We would alos have to change the plotting code a bit to use these vectors of variables. - Try and make these improvements as we go along!

- [ ] Create a new version of HTTCategories - this module has got very large and messy and takes ages to compile. It would be better to make a new version of this module adding only the variables/weights we need to 2017/2016 analyses

- [x] Update the electron/muon IDs to 2017 trainings/cuts. This is largly done for the muons but needs to be added for the electrons (for 2017 the MVA cut is varied as a function of pT - can be found on the EGamma pog twiki: https://twiki.cern.ch/twiki/bin/view/CMS/Egamma2017DataRecommendations)

- [x] Update the single lepton triggers: For 2017 there are several lower pT triggers that are prescaled for part of datataking but I think these should still be used in an OR with the other un-prescaled single-lepton trigger since the pT threholds for these triggers are getting quite high. I have set these up based on the information on the twikis about the which trigger are prescaled but we should check that the trigger I have included in the OR are actually needed (check if not using them in the OR changes the acceptance). There are also some additional triggers that weren't excplcitly reccomended by the POGS but could increase the trigger efficincy (e.g for the single muon trigger tehre is a Mu24 path with eta restriction) We should check this adding these in the trigger OR impoprves the trigger efficiency or not and if it does we should include them.

- [x] New Parameters file: make new parameters file with updated event numbers and cross-sectios. Some MC sample may have changed or been split by decay channel (e.g TT) so will need to find new cross sections for these processes

- [x] Tag and probe measurments: We need to produce SFs for electrons / muons id,iso and trigger. Currently the tag and probe code I wrote only works for the trigger scale-factors since it doesn't not fit the data to properly account for the presense of fakes, this is OK for the trigger SFs since the ID/isolation are applied to the probe leptons so the number of fakes is very small but the number of fakes can be large when measuring the id/iso scale factors. The tag and probe code should be changed to fit the invariant mass distribution (in bins of pT and eta) using function to properly account for the fakes (need to use simultanious fit of pass and fail categories). Once this is updated SFs can be produced and tested using Z->ee/mumu evenst 

- [x] DY pT/mass weights - measure new weights using di-muon data and apply/test using Z->mumu/ee events

- [x] Tau IDs/ SF: I added the next tau ID training to the ntuples, these will need to be applied instead ofthe old IDs in the analyser. Also update the SFs to the recommended ones on the TauPOG twiki. Anti-electron/muon IDs will likly be updated by the Tau POG - add these to nutples when they are available

- [x] Tau energy scales: Apply the POG recommendation. For e->tau and mu->tau fakes we should measure these outselves (see how Adinda did this for MSSM)

- [ ] JES corrctions + uncertainties: Check these are applied automatically by using latest global tag (if not will require new ntuples). Get Latest uncertainties .xml file

- [x] Jet ID: Check if the PF jet ID has been updated is so update also in our code

- [ ] B-tagger: Check if we are still using the latest recomended btagger, if not then update this. Also make sure we are using the latest SFs and uncertainties for this tagger

- [x] B-tag efficincies: Appliedated of b-tag Sfs require measurment of the btag efficiencies in MC - update this for the latest btagger and 2017 MC

- [ ] MET recoil corrections - Check if DESY group plan to measure then and if so us ethere measurments (if not we can discuss measring these ourselves but shouldn't be a high priority)

- [ ] SV-fit / MELA / MVA score usage: It is very inefficienct to have to run the analyser repeatadly to add the SV-fit mass and MELA variables to the final trees. It would be better to have some code that can just loop over the trees proeduced without the SVfit/MELA and add these to the branches - something similar to what is done to add the MVA scores to the trees. Options to add either SV-fit or MELA (or both at the same time) would also be useful

- [ ] Apply suggested MET filters once they are on the ntuples (https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2)

- [ ] Update params file after next production. Some sample names may have changed. event numbers will have changed. some ext samples added. DY low mass if now 5-50 GeV so will need to find latest XS for this sample
