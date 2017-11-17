### To do for SM analysis:

- [ ] mu+tau trigger SFs disagreement with Vienna

- [ ] event number differences with other groups?

- [ ] Run SV fit for all samples/energy scale/met shifts - do e->tau mu->tau ES shifts only apply to Zee/mm events? - if so then only need to run for DY/Z samples - if MET is shape systematic then will need to run for MET shifts as well

- [ ] MET systematics as shape uncertainties or not?

- [ ] W+gamma samples for emu channel only?

- [ ] H->WW background - which templates to include this under (maybe has a seperate template)

- [ ] Add EWk Z to Z->tautau background (also to Z->ll ?)

- [ ] No Ewk W included?

- [ ] Z->mumu correction for mjj shape?

- [ ] background method for W - OSSS method?

- [ ] W CR mT > 80 GeV
- [ ] W CR for boosted used to constrain VBF - what to use as nominal estimate for W in VBF category

- [ ] QCD CR - boosted used to constrain VBF so same comments as above

- [ ] mu->tau ES correction uncertainty = 1.5 %
e->tau ES correction uncertainty = 3 % 

- [ ] above 2 uncetainties seperate for different DMs but shold the uncertainty be included for 3-prong decays only?

- [ ] n_bjets==0 for emu channel only?

- [ ] JES uncertainties - implemented at the moment as seperate shifts - maybe better to add relevant variables (n_jets_X, n_bjets_X, mjj_X etc ....) for each JES shift X - use seperate module to do this after HTTCategories? - e.g copy HTTCateogry tree somehow and add extra branches?

- [ ] For plotting code make sure trees are only run over once for all systematics - relevant for JES corrections after change described above

- [ ] write script to make datacards
