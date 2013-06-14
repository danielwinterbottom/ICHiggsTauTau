#!/bin/sh
PRODUCTION=Apr04
PARAMS=./filelists/$PRODUCTION/Params${PRODUCTION}.dat

for CHANNEL in enu
  do

  for MET in 0
    do

      FOLDER=./output/$CHANNEL/MET$MET/
      PLOTDIR=PLOTS/$CHANNEL/MET$MET/
      PLOTDIRQCD=PLOTS/$CHANNEL/MET$MET/QCD/

      mkdir -p $PLOTDIR
      mkdir -p $PLOTDIRQCD

      BLIND=1
      if (( "$CHANNEL" != "nunu" )) || (( "$MET" != "130" ))
	  then
	  let BLIND=0
      fi

###### n_electrons
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="n_electrons"  --x_axis_label="Number of electrons" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=5 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=1 \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

###### n_muons
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="n_muons"  --x_axis_label="Number of muons" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=5 \
    --y_axis_min=0.01 --extra_pad=2 \
    --rebin=1 \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

###### mT
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mt_munu"  --x_axis_label="m_{T}(#mu#nu) (GeV)" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=400 \
    --y_axis_min=0.01 --extra_pad=2 \
    --rebin=20 \
    --log_y=false \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mt_enu"  --x_axis_label="m_{T}(e#nu) (GeV)" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=400 \
    --y_axis_min=0.01 --extra_pad=2 \
    --rebin=20 \
    --log_y=false \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mt_munu"  --x_axis_label="m_{T}(#mu#nu) (GeV)" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=400 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=20 \
    --log_y=true \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mt_enu"  --x_axis_label="m_{T}(e#nu) (GeV)" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=400 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=20 \
    --log_y=true \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

###### mupt_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mupt_1"  --x_axis_label="Leading muon p_{T} [GeV]" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=400 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=40 \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

###### mueta_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mueta_1"  --x_axis_label="Leading muon #eta" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=-3 --x_axis_max=3 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=5 \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

###### ept_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="ept_1"  --x_axis_label="Leading electron p_{T} [GeV]" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=400 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=40 \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

###### eeta_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="eeta_1"  --x_axis_label="Leading electron #eta" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=-3 --x_axis_max=3 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=5 \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

###### met no mu
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="met_nomuons"  --x_axis_label="PF MET + #mu (GeV)" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=20 \
    --norm_bins=false \
    --paramfile=$PARAMS

###### met no elec
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="met_noelectrons"  --x_axis_label="PF MET + e (GeV)" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=700 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=20 \
    --norm_bins=false \
    --paramfile=$PARAMS

###### met
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="met"  --x_axis_label="PF MET (GeV)" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=700 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=20 \
    --norm_bins=false \
    --paramfile=$PARAMS

###### mu dxy
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mudxy_all"  --x_axis_label="Muon d_{xy}" \
    --blind=$BLIND \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=2 \
    --norm_bins=false \
    --paramfile=$PARAMS

###### mu dz
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mudz_all"  --x_axis_label="Muon d_{z}" \
    --blind=$BLIND \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=2 \
    --norm_bins=false \
    --paramfile=$PARAMS


###### mu iso
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="muiso_all"  --x_axis_label="Muon iso" \
    --blind=$BLIND \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=2 \
    --norm_bins=false \
    --paramfile=$PARAMS

###### e dxy
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="edxy_all"  --x_axis_label="Electron d_{xy}" \
    --blind=$BLIND \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=2 \
    --norm_bins=false \
    --paramfile=$PARAMS

###### e dz
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="edz_all"  --x_axis_label="Electron d_{z}" \
    --blind=$BLIND \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=2 \
    --norm_bins=false \
    --paramfile=$PARAMS

###### e iso
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="eiso_all"  --x_axis_label="Electron iso" \
    --blind=$BLIND \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=2 \
    --norm_bins=false \
    --paramfile=$PARAMS



  done
done