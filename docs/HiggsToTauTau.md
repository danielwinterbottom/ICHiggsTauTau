HiggsToTauTau {#higgs-to-tau-tau}
============

[TOC]

This page gives instructions for running the Higgs to tau tau analysis, with the goal of generating the datacard inputs used to produce the public results in the SM search [HIG-13-004](http://arxiv.org/abs/1401.5041) and MSSM search [HIG-13-021](http://cds.cern.ch/record/1623367?ln=en). It is possible to produce these inputs for the mu-tau, e-tau and e-mu channels and for both 7 and 8 TeV analyses.

The full analysis chain involves several steps, each outlined in a section below.

Ntuple Production {#htt-ntuple-production}
==========================================
Production of a complete set of ntuples from AOD using the grid.  This is by far the most time and CPU intensive step and should only be performed when changes to the underlying object reconstruction and discrimination are needed which cannot be realised in this framework with existing ntuples. Some notes on preparing a new production can be found on [this] page along with a suggested list of samples to process. For running the published version of the analyses it is sufficient to use existing productions, for which all ntuple files are located on dCache at Imperial. For book-keeping and scripting convenience, all the ntuples for a given sample can be found under the folder structure:

    srm://gfe02.grid.hep.ph.ic.ac.uk:8443/srm/managerv2?SFN=/pnfs/hep.ph.ic.ac.uk/data/cms/store/user/[USERNAME]/[DATE]/[Data/MC]_[VERSION]/[SAMPLE_LABEL]

 In this `USERNAME` is the lxplus account name of the user who initiated the grid jobs for this sample. A given production is primarily indentified by `DATE`, which is the date that production was started. The `June6` production is used for the 8 TeV analysis, and `Apr11` for the 7 TeV analysis. Within this  folder samples are split into folders starting either `Data_` or `MC_` and ending in the CMSSW software version used,  `53X` or `42X` for the 8 TeV and 7 TeV analyses respectively. Finally, shorted versions of the AOD dataset names are used to indentify a particular sample. A full example path would be:

    srm://gfe02.grid.hep.ph.ic.ac.uk:8443/srm/managerv2?SFN=/pnfs/hep.ph.ic.ac.uk/data/cms/store/user/agilbert/Apr11/MC_42X/WJetsToLNu

Skimming {#htt-skimming}
========================
Skimming 



