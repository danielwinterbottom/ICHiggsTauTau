#!/bin/sh
PRODUCTION=Apr04
PARAMS=./filelists/$PRODUCTION/Params${PRODUCTION}.dat

for CHANNEL in nunu enu munu taunu
  do
  
  for MET in 130 #0 70
    do
    for SYST in central #JESUP JESDOWN JERBETTER JERWORSE PUUP PUDOWN
      do

	FOLDER=./output/$CHANNEL/MET$MET/
	PLOTDIR=PLOTS/$CHANNEL/MET$MET/
	PLOTDIRQCD=PLOTS/$CHANNEL/MET$MET/QCD/
	
	if [ "$SYST" != "central" ] #if not doing central                                                                                                               
            then
            if [[ "$SYST" != PU* ]] #For PU syst info is in central root file if not doing PU get output from syst subdirectory                                         
                then
                FOLDER=$FOLDER"/"$SYST"/"
            #else #If doing PU syst pass correct options
		#CORRECT WITH CORRECT PU OPTIONS
            fi
            PLOTDIR=$PLOTDIR"/"$SYST"/"
	fi	

		
	mkdir -p $PLOTDIR
	mkdir -p $PLOTDIRQCD
	
	
	BLIND=1
	if [ "$CHANNEL" != "nunu" ] || (( "$MET" != "130" ))
	    then
	    let BLIND=0
	fi
	

###### jpt_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=40 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --paramfile=$PARAMS

###### jeta_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="jeta_1"  --x_axis_label="Leading Jet #eta" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=-5 --x_axis_max=5 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=5 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --paramfile=$PARAMS

###### jpt_2
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="jpt_2"  --x_axis_label="Next-to-Leading Jet p_{T} [GeV]" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=40 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --paramfile=$PARAMS

###### jeta_2
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="jeta_2"  --x_axis_label="Next-to-Leading Jet #eta" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=-5 --x_axis_max=5 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=5 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --paramfile=$PARAMS

###### mjj
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=3000 \
    --y_axis_min=0.1 --extra_pad=10000 \
    --rebin=100 \
    --norm_bins=false \
    --log_y=true \
    --paramfile=$PARAMS

###### detajj
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="detajj"  --x_axis_label="#Delta#eta_{jj}" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=5 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=2 \
    --norm_bins=false \
    --log_y=true \
    --paramfile=$PARAMS

###### met
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="met"  --x_axis_label="PF MET (GeV)" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=10 \
    --norm_bins=false \
    --log_y=true \
    --paramfile=$PARAMS

###### metNoMuons
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="met_noMuons"  --x_axis_label="PF MET + #mu (GeV)" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=10 \
    --norm_bins=false \
    --log_y=true \
    --paramfile=$PARAMS

###### n_taus
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="n_taus"  --x_axis_label="Number of taus" \
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
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=400 \
    --y_axis_min=0.01 --extra_pad=2 \
    --rebin=20 \
    --log_y=false \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mt_enu"  --x_axis_label="m_{T}(e#nu) (GeV)" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=400 \
    --y_axis_min=0.01 --extra_pad=2 \
    --rebin=20 \
    --log_y=false \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mt_taunu"  --x_axis_label="m_{T}(#tau#nu) (GeV)" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
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
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0.\
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=400 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=40 \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

###### mueta_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mueta_1"  --x_axis_label="Leading muon #eta" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=-3 --x_axis_max=3 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=5 \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

###### ept_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="ept_1"  --x_axis_label="Leading electron p_{T} [GeV]" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=400 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=40 \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

###### eeta_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="eeta_1"  --x_axis_label="Leading electron #eta" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=-3 --x_axis_max=3 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=5 \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

###### taupt_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="taupt_1"  --x_axis_label="Leading tau p_{T} [GeV]" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=400 \
    --y_axis_min=0.01 --extra_pad=100 \
    --rebin=20 \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

###### taupt_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="taupt_1"  --x_axis_label="Leading tau p_{T} [GeV]" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
    --y_axis_min=0.01 --extra_pad=1.2 \
    --rebin=20 \
    --norm_bins=false --verbose=false \
    --log_y=false \
    --paramfile=$PARAMS


###### taueta_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="taueta_1"  --x_axis_label="Leading tau #eta" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=-2.5 --x_axis_max=2.5 \
    --y_axis_min=0.01 --extra_pad=1000 \
    --rebin=5 \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

###### taueta_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="taueta_1"  --x_axis_label="Leading tau #eta" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=-2.5 --x_axis_max=2.5 \
    --y_axis_min=0.01 --extra_pad=1.5 \
    --rebin=5 \
    --log_y=false \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

#####dR tau-jets
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="taggingJetsFlavour"  --x_axis_label="#tau as tagging jet" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --y_axis_min=0.01 --extra_pad=1.1 \
    --rebin=1 \
    --plot_wjets_comp=false \
    --norm_bins=false \
    --plot_qcd=false \
    --log_y=false \
    --paramfile=$PARAMS

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="dRmin_taujet"  --x_axis_label="#Delta R_{min}(#tau,tag)" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=4.5 \
    --y_axis_min=0.01 --extra_pad=100 \
    --rebin=10 \
    --plot_wjets_comp=false \
    --norm_bins=false \
    --plot_qcd=false \
    --log_y=true \
    --paramfile=$PARAMS

    done
  done
done


#plot_name=mjj
#met,met_phi,n_vtx,n_jets,n_jetsingap
#jpt_1,jpt_2,jeta_1,jeta_2
#detajj,drjj,dphijj,etaprodjj