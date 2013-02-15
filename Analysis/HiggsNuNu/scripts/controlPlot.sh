#!/bin/sh
PARAMS=./scripts/Params.dat

###### jpt_1
#./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
#    --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
#    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=500 --rebin=1 \
#    --norm_bins=false --verbose=false \
#    --paramfile=$PARAMS


###### mjj
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=3000 \
    --y_axis_min=0.1 --extra_pad=15 \
    --rebin=50 \
    --norm_bins=false \
    --log_y=true \
    --paramfile=$PARAMS

#plot_name=mjj
#met,met_phi,n_vtx,n_jets,n_jetsingap
#jpt_1,jpt_2,jeta_1,jeta_2
#detajj,drjj,dphijj,etaprodjj