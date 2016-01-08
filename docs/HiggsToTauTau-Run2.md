HiggsToTauTau - Run2 {#higgs-to-tau-tau-run2}
============

[TOC]

This page gives instructions for running the Higgs to tau tau analysis in the newest code being used with the 13 TeV data. The workflow is similar to that used during Run 1 with some further improvements. The code now supports mutau, etau, emu and tautau channels, as well as Z->ee and Z->mumu channels. We attempt for now to keep backwards compatibility with the 8 TeV (not the 7 TeV) analysis, although the possibility also remains to use an old branch of the code and the Run 1 instructions described on the previous page.

The full analysis chain involves several steps, each outlined in a section below.

Ntuple Production {#htt-ntuple-production-run2}
==========================================

Ntuple production is similar to Run 1, with the exception that the ntuples are by default production from the new CMS wide data format: miniAOD. Instructions will be added here shortly. 


Selection of events {#htt-event-selection-run2}
==========================================
Event selection is performed using HTTSequence.cc. As the name suggests the di-tau selection is applied for a given channel and sample, and it is possible to call and run the sequence multiple times within one job. This allows a slight improvement in efficiency compared with the Run 1 code. For e.g. applying the tau energy scale shift to generate systematic templates, the majority of the modules run in the selection are identical, and so time is saved by not loading branches from the trees multiple times.

The submission of the multiple sequences is controlled using HTT.cpp, and a config.json file which contains various configurable parameters. One call to HTT.cpp is made for each of the backgrounds, data and signal, with optional multiple calls in separate jobs for subsets of the larger samples. This is controlled by the number of files read in each job. It is optionally possible to have systematic shifts such as tau energy scale included in the same job.

First an appropriate output directory should be made with the correct directory structure:

    ./scripts/make_output_folder.sh output/Nov19/
    ./scripts/prepare_output_folder.sh output/Nov19/

Next, the following produces all of the necessary output files for the SM and MSSM analyses:
    
    ./scripts/htt_spring15.py --data --bkg --sm --mssm --submit='./scripts/submit_ic_batch_job.sh hepshort.q'

The jobs can be submitted to IC batch (as in this example), the lxbatch, or run locally. The jobs are separated by number of input files and optimised such that they all fit inside the hepshort queue (i.e. runtime of less than 1 hour). 
    

Producing plots and datacards {#htt-datacards-run2}
=============================================
The output of HiggsTauTau.cpp is a set of flat trees containing all variables of importance to the SM and MSSM analyses. The first step is to combine the numbered output files for each sample resulting from the splitting by number of input files, using the following script:

    ./scripts/hadd_spring15_output.py --folder=output/Nov19/

The script contains a hardcoded list of the sample names to look for, and checks the number of completed jobs against the number of created jobs.

Plots and datacards can then be produced with the help of test/HiggsTauTauPlot5.cpp. For example to make a plot of SVFit mass in the b-tag category of the MSSM:

    ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30."   --method=8 --var="m_sv"["0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,900"] --cat="btag"   --x_axis_label="M_{#tau#tau} [GeV]"  --norm_bins=true --datacard="btag" --log_y=true --draw_ratio=true   --custom_y_axis_min=true --y_axis_min=0.0099   --blind=true --x_blind_min=40 --x_blind_max=2000   --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

Datacard inputs can be produced using another helper python script as follows:

    ./scripts/makeDatacardsRun2.py --cfg=scripts/new_plot_mssm_2015.cfg -c 'mt' -i output/Nov19 --svfit scripts/Params_2015.dat -s 'run2_mssm'


