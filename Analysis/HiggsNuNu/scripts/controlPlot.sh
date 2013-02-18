#!/bin/sh
PARAMS=./scripts/Params.dat


###### n_jets
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --plot_name="n_jets"  --x_axis_label="Number of jets" \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=20 \
    --y_axis_min=0.01 --extra_pad=15 \
    --rebin=1 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --paramfile=$PARAMS

###### jpt_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=15 \
    --rebin=20 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --paramfile=$PARAMS

###### jeta_1
#./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
#    --plot_name="jeta_1"  --x_axis_label="Leading Jet #eta" \
#    --custom_x_axis_range=true --x_axis_min=-5 --x_axis_max=5 \
#    --y_axis_min=0.01 --extra_pad=15 \
#    --rebin=1 \
#    --norm_bins=false --verbose=false \
#    --log_y=true \
#    --paramfile=$PARAMS


###### mjj
#./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
#    --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
#    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=3000 \
#    --y_axis_min=0.1 --extra_pad=15 \
#    --rebin=50 \
#    --norm_bins=false \
#    --log_y=true \
#    --paramfile=$PARAMS

###### detajj
#./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
#    --plot_name="detajj"  --x_axis_label="#Delta#eta_{jj}" \
#    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=5 \
#    --y_axis_min=0.01 --extra_pad=15 \
#    --rebin=1 \
#    --norm_bins=false \
#    --log_y=true \
#    --paramfile=$PARAMS

#plot_name=mjj
#met,met_phi,n_vtx,n_jets,n_jetsingap
#jpt_1,jpt_2,jeta_1,jeta_2
#detajj,drjj,dphijj,etaprodjj