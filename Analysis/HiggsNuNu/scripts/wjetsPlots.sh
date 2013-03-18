#!/bin/sh
PARAMS=./scripts/Params.dat

for CHANNEL in nunu enu munu
  do

  for MET in 130 0 70
    do

    for DOQCD in 0 1 2
      do

      FOLDER=./output/$CHANNEL/MET$MET/DOQCD$DOQCD/
      PLOTDIR=PLOTS/$CHANNEL/MET$MET/DOQCD$DOQCD/

      mkdir -p $PLOTDIR

      BLIND=1
      if (( "$CHANNEL" != "nunu" )) || (( "$DOQCD" == "1" )) || (( "$MET" != "130" ))
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
#./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
#    --folder=$FOLDER --plot_dir=$PLOTDIR \
#    --plot_name="met_nomuons"  --x_axis_label="PF MET + #mu (GeV)" \
#    --blind=$BLIND \
#    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
#    --y_axis_min=0.01 --extra_pad=10000 \
#    --rebin=20 \
#    --norm_bins=false \
#    --paramfile=$PARAMS

###### met no elec
#./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
#    --folder=$FOLDER --plot_dir=$PLOTDIR \
#    --plot_name="met_noelectrons"  --x_axis_label="PF MET + e (GeV)" \
#    --blind=$BLIND \
#    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
#    --y_axis_min=0.01 --extra_pad=10000 \
#    --rebin=20 \
#    --norm_bins=false \
#    --paramfile=$PARAMS

    done
  done
done