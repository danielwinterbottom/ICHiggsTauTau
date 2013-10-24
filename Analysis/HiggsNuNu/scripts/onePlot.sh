#!/bin/sh
PRODUCTION=Apr04
#PARAMS=./filelists/$PRODUCTION/Params${PRODUCTION}.dat
PARAMS=./filelists/$PRODUCTION/Params${PRODUCTION}_noSignal.dat

for CHANNEL in nunu #taunu
  do
  for MET in 130
    do

	FOLDER=./output/$CHANNEL/MET$MET/
	PLOTDIR=PLOTS/$CHANNEL/MET$MET/
	PLOTDIRQCD=PLOTS/$CHANNEL/MET$MET/QCD/
	#PLOTDIRDATAQCD=PLOTS/$CHANNEL/MET$MET/DATAQCD/

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
    --plot_name="jCSV_1"  --x_axis_label="Jet 1 discriminant" \
    --y_axis_min=0.1 --extra_pad=100 \
    --rebin=10 \
    --plot_wjets_comp=false \
    --norm_bins=false \
    --plot_qcd=false \
    --log_y=true \
    --addOverflows=false \
    --paramfile=$PARAMS

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --blind=$BLIND  --x_blind_min=0 --x_blind_max=1. \
    --plot_name="jCSV_2"  --x_axis_label="Jet 2 discriminant" \
    --y_axis_min=0.1 --extra_pad=100 \
    --rebin=10 \
    --plot_wjets_comp=false \
    --norm_bins=false \
    --plot_qcd=false \
    --log_y=true \
    --addOverflows=false \
    --paramfile=$PARAMS

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --blind=$BLIND  --x_blind_min=0 --x_blind_max=1. \
    --plot_name="jCSV_3"  --x_axis_label="Jet 3 discriminant" \
    --y_axis_min=0.1 --extra_pad=100 \
    --rebin=10 \
    --plot_wjets_comp=false \
    --norm_bins=false \
    --plot_qcd=false \
    --log_y=true \
    --addOverflows=false \
    --paramfile=$PARAMS

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --blind=$BLIND  --x_blind_min=0 --x_blind_max=1. \
    --plot_name="jCSV_4"  --x_axis_label="Jet 4 discriminant" \
    --y_axis_min=0.1 --extra_pad=100 \
    --rebin=10 \
    --plot_wjets_comp=false \
    --norm_bins=false \
    --plot_qcd=false \
    --log_y=true \
    --addOverflows=false \
    --paramfile=$PARAMS

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --blind=$BLIND  --x_blind_min=0 --x_blind_max=1. \
    --plot_name="jCSV_allJets"  --x_axis_label="CSV discriminant" \
    --y_axis_min=0.1 --extra_pad=100 \
    --rebin=10 \
    --plot_wjets_comp=false \
    --norm_bins=false \
    --plot_qcd=false \
    --log_y=true \
    --addOverflows=false \
    --paramfile=$PARAMS

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="n_jets"  --x_axis_label="Number of jets (p_{T}^{i>2}>30 GeV)" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=20 \
    --y_axis_min=0.01 --extra_pad=1000 \
    --rebin=1 \
    --norm_bins=false --verbose=false \
    --plot_qcd=false \
    --log_y=true \
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
