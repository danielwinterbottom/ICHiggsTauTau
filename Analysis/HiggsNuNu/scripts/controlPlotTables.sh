#!/bin/sh
PARAMS=./scripts/Params.dat

for CHANNEL in enu munu
  do

  for MET in 130 0 70
    do

    for DOQCD in 0 2
      do

      FOLDER=./output/$CHANNEL/MET$MET/DOQCD$DOQCD/
      PLOTDIR=TABLES/$CHANNEL/MET$MET/DOQCD$DOQCD/

      mkdir -p $PLOTDIR

      BLIND=1
      if [ "$CHANNEL" != "nunu" ] || (( "$DOQCD" == "1" )) || (( "$MET" != "130" ))
	  then
	  let BLIND=0
      fi

 #     echo $CHANNEL " " $MET " " $DOQCD " : BLIND variable is "$BLIND


###### jpt_1
#./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
#    --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
#    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
#    --y_axis_min=0.01 --extra_pad=1000 \
#    --rebin=20 \
#    --norm_bins=false --verbose=false \
#    --log_y=true \
#    --paramfile=$PARAMS

###### n_jets
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="n_jets"  --x_axis_label="Number of jets" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=20 \
    --y_axis_min=0.01 --extra_pad=1000 \
    --rebin=1 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --paramfile=$PARAMS

###### dphijj
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="dphijj"  --x_axis_label="#Delta#phi_{jj}" \
    --blind=$BLIND  --x_blind_min=0 --x_blind_max=1. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=3.2 \
    --y_axis_min=0.01 --extra_pad=2 \
    --rebin=4 \
    --norm_bins=false \
    --log_y=false \
    --paramfile=$PARAMS

    done
  done
done