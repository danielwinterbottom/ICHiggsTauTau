### To do for SM analysis:

- [ ] mu+tau trigger SFs disagreement with Vienna

- [ ] event number differences with other groups?

- [ ] Run SV fit for all samples/energy scale/met shifts - do e->tau mu->tau ES shifts only apply to Zee/mm events? - if so then only need to run for DY/Z samples - if MET is shape systematic then will need to run for MET shifts as well

- [x] MET systematics as shape uncertainties or not?
- There are 2 MET shape systematics included unclusted and clustered which are added for all MC processes

- [ ] Find out how to comput MET unclustered and clustered systematic shifts and add to datacards

- [ ] W+gamma samples for emu channel only?

- [ ] H->WW background - add these as seperate templates for em channel only called ggH_hww125 and qqH_hww125

- [ ] Add EWk Z as seperate template called EWKZ I presume this is not split by any gen matching

- [x] No Ewk W included?
- Yes no Ewk W

- [ ] Z->mumu correction for mjj shape?

- [ ] background method for W - OSSS simultanious equations method?

- [ ] W CR mT > 80 GeV
- [ ] W CR for boosted used to constrain VBF - what to use as nominal estimate for W in VBF category

- [ ] QCD CR - boosted used to constrain VBF so same comments as above

- [x] mu->tau ES correction uncertainty = 1.5 %
e->tau ES correction uncertainty = 3 % 

- [x] above 2 uncetainties seperate for different DMs but shold the uncertainty be included for 3-prong decays as well?
- this is not included  for the 3prong decays and the systematic shifts are only included for ZL templates in et and mt channels

- [ ] n_bjets==0 for emu channel only?

- [x] JES uncertainties - implemented at the moment as seperate shifts - maybe better to add relevant variables (n_jets_X, n_bjets_X, mjj_X etc ....) for each JES shift X - use seperate module to do this after HTTCategories? - e.g copy HTTCateogry tree somehow and add extra branches?

- [ ] For plotting code make sure trees are only run over once for all systematics - relevant for JES corrections after change described above

- [ ] write script to make datacards and add required systematics to plotting code
