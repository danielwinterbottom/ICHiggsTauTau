#!/bin/sh
PRODUCTION=Apr04
#PARAMS=./filelists/$PRODUCTION/Params${PRODUCTION}.dat
PARAMS=./filelists/$PRODUCTION/Params${PRODUCTION}_noSignal.dat

for CHANNEL in nunu #taunu
  do
  for MET in 130
    do

	FOLDER=./output_tauEmbed/$CHANNEL/MET$MET/
	PLOTDIR=PLOTS_tauEmbed/$CHANNEL/MET$MET/
	PLOTDIRQCD=PLOTS_tauEmbed/$CHANNEL/MET$MET/QCD/
	#PLOTDIRDATAQCD=PLOTS_tauEmbed/$CHANNEL/MET$MET/DATAQCD/

	mkdir -p $PLOTDIR
	mkdir -p $PLOTDIRQCD
	mkdir -p $PLOTDIR/wjetsComp/

	#mkdir -p $PLOTDIRDATAQCD
	BLIND=0
	#if [ "$CHANNEL" != "nunu" ] || (( "$MET" != "130" ))
	#    then
	#    let BLIND=0
	#fi
	
 #     echo $CHANNEL " " $MET " " $DOQCD " : BLIND variable is "$BLIND
	
	
###### 

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --blind=$BLIND  --x_blind_min=0 --x_blind_max=1. \
    --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=400 \
    --y_axis_min=0.1 --extra_pad=100 \
    --rebin=20 \
    --use_embedded_data=true \
    --plot_wjets_comp=true \
    --norm_bins=false \
    --plot_qcd=false \
    --log_y=true \
    --addOverflows=true \
    --paramfile=$PARAMS

  done
done


#    --plot_name="jCSV_1"  --x_axis_label="Jet 1 discriminant" \
#   --plot_name="detajj"  --x_axis_label="#Delta#eta_{jj}" \
#    --plot_name="dphijj"  --x_axis_label="#Delta#phi_{jj}" \
#    --plot_name="n_jets"  --x_axis_label="Jet multiplicity" \
#   --plot_name="n_taus"  --x_axis_label="#tau multiplicity" \
#    --plot_name="mudxy_all"  --x_axis_label="Muon d_{xy}" \
#    --plot_name="dphijj"  --x_axis_label="#Delta#phi_{jj}" \
#    --x_blind_min=0 --x_blind_max=1.\
#    --plot_name="met_noelectrons"  --x_axis_label="PF MET + e (GeV)" \
#    --plot_name="met"  --x_axis_label="PF MET (GeV)" \
#    --plot_name="dphijj"  --x_axis_label="#Delta#phi_{jj}" \
#    --plot_name="met_noMuons"  --x_axis_label="PF MET + #mu (GeV)" \
#    --plot_name="n_taus"  --x_axis_label="#tau multiplicity" \
#    --plot_name="taupt_1"  --x_axis_label="p_{T}(#tau) (GeV)" \
#    --plot_name="mt_taunu"  --x_axis_label="m_{T}(#tau#nu) (GeV)" \
#    --plot_name="taueta_1"  --x_axis_label="#eta(#tau) (GeV)" \
#    --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \