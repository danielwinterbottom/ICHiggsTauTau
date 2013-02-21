#!/bin/sh
PARAMS=./scripts/Params.dat


###### n_jets
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --plot_name="n_jets"  --x_axis_label="Number of jets" \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=20 \
    --y_axis_min=0.01 --extra_pad=1000 \
    --rebin=1 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --blind=true --x_blind_min=1 --x_blind_max=0 \
    --paramfile=$PARAMS

###### n_jetsingap
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --plot_name="n_jetsingap"  --x_axis_label="Number of jets in rapidity gap" \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=20 \
    --y_axis_min=0.01 --extra_pad=1000 \
    --rebin=1 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --blind=true --x_blind_min=1 --x_blind_max=0 \
    --paramfile=$PARAMS

###### n_vtx
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --plot_name="n_vtx"  --x_axis_label="Number of vertices" \
    --y_axis_min=0.01 --extra_pad=1000 \
    --rebin=1 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --blind=true --x_blind_min=1 --x_blind_max=0 \
    --paramfile=$PARAMS

###### jpt_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=1000 \
    --rebin=40 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --blind=true --x_blind_min=1 --x_blind_max=0 \
    --paramfile=$PARAMS

###### jeta_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --plot_name="jeta_1"  --x_axis_label="Leading Jet #eta" \
    --custom_x_axis_range=true --x_axis_min=-5 --x_axis_max=5 \
    --y_axis_min=0.01 --extra_pad=1000 \
    --rebin=5 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --blind=true --x_blind_min=1 --x_blind_max=0 \
    --paramfile=$PARAMS


###### jpt_2
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --plot_name="jpt_2"  --x_axis_label="Next-to-Leading Jet p_{T} [GeV]" \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=1000 \
    --rebin=40 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --blind=true --x_blind_min=1 --x_blind_max=0 \
    --paramfile=$PARAMS

###### jeta_2
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --plot_name="jeta_2"  --x_axis_label="Next-to-Leading Jet #eta" \
    --custom_x_axis_range=true --x_axis_min=-5 --x_axis_max=5 \
    --y_axis_min=0.01 --extra_pad=1000 \
    --rebin=5 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --blind=true --x_blind_min=1 --x_blind_max=0 \
    --paramfile=$PARAMS


###### mjj
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=3000 \
    --y_axis_min=0.1 --extra_pad=1000 \
    --rebin=100 \
    --norm_bins=false \
    --log_y=true \
    --blind=true --x_blind_min=1 --x_blind_max=0 \
    --paramfile=$PARAMS

###### detajj
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --plot_name="detajj"  --x_axis_label="#Delta#eta_{jj}" \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=5 \
    --y_axis_min=0.01 --extra_pad=1000 \
    --rebin=2 \
    --norm_bins=false \
    --log_y=true \
    --blind=true --x_blind_min=1 --x_blind_max=0 \
    --paramfile=$PARAMS

###### dphijj
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --plot_name="dphijj"  --x_axis_label="#Delta#phi_{jj}" \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=3.2 \
    --y_axis_min=0.01 --extra_pad=1000 \
    --rebin=1 \
    --norm_bins=false \
    --log_y=true \
    --blind=true --x_blind_min=0 --x_blind_max=1. \
    --paramfile=$PARAMS

###### met
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --plot_name="met"  --x_axis_label="PF MET (GeV)" \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=1000 \
    --rebin=20 \
    --norm_bins=false \
    --log_y=true \
    --blind=true --x_blind_min=1 --x_blind_max=0 \
    --paramfile=$PARAMS




#plot_name=mjj
#met,met_phi,n_vtx,n_jets,n_jetsingap
#jpt_1,jpt_2,jeta_1,jeta_2
#detajj,drjj,dphijj,etaprodjj