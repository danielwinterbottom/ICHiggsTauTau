HiggsToTauTau {#higgs-to-tau-tau}
============

[TOC]

This page gives instructions for running the Higgs to tau tau analysis, with the goal of generating the datacard inputs used to produce the public results in the SM search [HIG-13-004](http://arxiv.org/abs/1401.5041) and MSSM search [HIG-13-021](http://cds.cern.ch/record/1623367?ln=en) and the not yet public MSSM H->hh search [HIG-14-034](http://cms.cern.ch/iCMS/analysisadmin/cadi?ancode=HIG-14-034). It is possible to produce these inputs for the mu-tau, e-tau and e-mu channels and for both 7 and 8 TeV analyses. For the H->hh analysis only e-tau and mu-tau channels are used with the 8 TeV data.

The full analysis chain involves several steps, each outlined in a section below.

Ntuple Production {#htt-ntuple-production}
==========================================
Production of a complete set of ntuples from AOD using the grid.  This is by far the most time and CPU intensive step and should only be performed when changes to the underlying object reconstruction and discrimination are needed which cannot be realised in this framework with existing ntuples. Some notes on preparing a new production can be found on [this] page along with a suggested list of samples to process. For running the published version of the analyses it is sufficient to use existing productions, for which all ntuple files are located on dCache at Imperial. For book-keeping and scripting convenience, all the ntuples for a given sample can be found under the folder structure:

    srm://gfe02.grid.hep.ph.ic.ac.uk:8443/srm/managerv2?SFN=/pnfs/hep.ph.ic.ac.uk/data/cms/store/user/[USERNAME]/[DATE]/[Data/MC]_[VERSION]/[SAMPLE_LABEL]

 In this `USERNAME` is the lxplus account name of the user who initiated the grid jobs for this sample. A given production is primarily indentified by `DATE`, which is the date that production was started. The `June6` production is used for the 8 TeV analysis, and `Apr11` for the 7 TeV analysis. Within this  folder samples are split into folders starting either `Data_` or `MC_` and ending in the CMSSW software version used,  `53X` or `42X` for the 8 TeV and 7 TeV analyses respectively. Finally, shorted versions of the AOD dataset names are used to indentify a particular sample. A full example path would be:

    srm://gfe02.grid.hep.ph.ic.ac.uk:8443/srm/managerv2?SFN=/pnfs/hep.ph.ic.ac.uk/data/cms/store/user/agilbert/Apr11/MC_42X/WJetsToLNu

Skimming {#htt-skimming}
========================
To reduce the size of the ntuples a skim is applied which only retains events containing a good candidate pair, either electron + tau, muon + tau or electron + muon. Skimming is performed from within the HiggsTauTau directory using the /test/HiggsTauTau.cpp code with skim_mode turned on. Example scripts which perform skims of all data and MC samples can be found [here](https://github.com/ajgilbert/ICHiggsTauTau/blob/master/Analysis/HiggsTauTau/scripts/June6_et_mt_skim.sh) for the mu-tau and e-tau channels and [here](https://github.com/ajgilbert/ICHiggsTauTau/blob/master/Analysis/HiggsTauTau/scripts/June6_em_skim.sh) for the e-mu channel. A good workflow for skimming is to skim directly from dCache at Imperial onto the SSD at Imperial, before copying the skims back onto dCache as a backup. For running at CERN, the skimmed ntuples will comfortably fit in an eos area or on local disk on a PC. 

Selection of events {#htt-event-selection}
==========================================
Event selection is performed using HiggsTauTau.cpp. This should be run once for each of the data and MC samples, and then again for the generation of templates for systematics such as tau or jet energy scale. To simplify this, a python script higgsTauTau.py is available which generates all the jobs for backgrounds, data and signal for the central tau energy scale and the tau energy scale shifted up and down. First an appropriate output directory should be made with the correct directory structure:

    ./scripts/make_output_folder.sh output/Paper_2012/
    ./scripts/prepare_output_folder.sh output/Paper_2012/

Next, the following produces all of the necessary output files for the SM and MSSM analyses:

    ./scripts/higgsTauTau.py -p June6 -i root://eoscms//eos/cms/store/user/rlane/httskims/ --data --bkg --mssm --sm -s '0,1,2' -c 'et,mt,em' -e Paper --submit='./scripts/submit_lxbatch_job.sh 1nh'

The jobs can be submitted to lxbatch (as in this example), the IC batch, or run locally. Similarly for the H->hh signal jobs: 
    
    ./scripts/higgsTauTau.py -p June6 -i root://eoscms//eos/cms/store/user/rlane/httskims/ --Hhh -s '0,1,2' -c 'et,mt' -e Paper --submit='./scripts/submit_lxbatch_job.sh 1nh'

The -s '0,1,2' option produces the tau energy scale systematics. To produce jet energy scale systematics (in particular important in the H->hh analysis), the jobs should be rerun setting jes_mode to 1 and 2 in the cfg file (e.g. Paper_2012.cfg).

Producing plots and datacards {#htt-datacards}
=============================================
The output of HiggsTauTau.cpp is a set of flat trees containing all variables of importance to the SM, MSSM and H->hh analyses. Plots and datacards can then be produced with the help of test/HiggsTauTauPlot4.cpp. Datacard inputs can be produced using another helper python script as follows:

     ./scripts/makeDatacards2.py -i output/Paper_2012/  -c 'et,mt,em' --svfit --cfg=scripts/new_plot_sm_2012.cfg -p scripts/Paper_params_2012.dat --scheme="new_sm",

for the SM legacy analysis, 
     
     ./scripts/makeDatacards2.py -i output/Paper_2012/  -c 'et,mt,em' --svfit --cfg=scripts/new_plot_mssm_2012.cfg -p scripts/Paper_params_2012.dat --scheme="new_mssm",

for the MSSM legacy analysis, and

     ./scripts/makeDatacards2.py -i output/Paper_2012/  -c 'et,mt' --mH --masscuts --cfg=scripts/new_plot_Hhh_2012.cfg -p scripts/Paper_params_2012.dat --scheme="HTohh",

for the H->hh analysis.
