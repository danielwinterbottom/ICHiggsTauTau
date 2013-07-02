#!/bin/sh
PRODUCTION=Apr04
#PARAMS=./filelists/$PRODUCTION/Params${PRODUCTION}.dat
PARAMS=./filelists/$PRODUCTION/Params${PRODUCTION}_noSignal.dat

for CHANNEL in enu munu taunu
  do

  for MET in 130 0
    do

      FOLDER=./output_mjj1100/$CHANNEL/MET$MET/
      PLOTDIR=PLOTS/$CHANNEL/MET$MET/
      PLOTDIRQCD=PLOTS/$CHANNEL/MET$MET/QCD/

      mkdir -p $PLOTDIR
      mkdir -p $PLOTDIRQCD

      BLIND=1
      if [ "$CHANNEL" != "nunu" ] || (( "$MET" != "130" ))
	  then
	  let BLIND=0
      fi

###### n_electrons
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="n_electrons"  --x_axis_label="Number of electrons" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=5 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=1 \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

###### n_muons
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="n_muons"  --x_axis_label="Number of muons" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=5 \
    --y_axis_min=0.01 --extra_pad=2 \
    --rebin=1 \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

###### n_taus
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="n_taus"  --x_axis_label="Number of taus" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=3 \
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

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mt_munu"  --x_axis_label="m_{T}(#mu#nu) (GeV)" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=400 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=20 \
    --log_y=true \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mt_enu"  --x_axis_label="m_{T}(e#nu) (GeV)" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=400 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=20 \
    --log_y=true \
    --norm_bins=false --verbose=false \
    --paramfile=$PARAMS

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mt_taunu"  --x_axis_label="m_{T}(#tau#nu) (GeV)" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0.\
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

###### met no mu
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="met_nomuons"  --x_axis_label="PF MET + #mu (GeV)" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=20 \
    --norm_bins=false \
    --paramfile=$PARAMS

###### met no elec
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="met_noelectrons"  --x_axis_label="PF MET + e (GeV)" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=700 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=20 \
    --norm_bins=false \
    --paramfile=$PARAMS

###### met
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="met"  --x_axis_label="PF MET (GeV)" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=700 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=20 \
    --norm_bins=false \
    --paramfile=$PARAMS


#dR tau-jets
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
    --plot_name="dRmin_taujet1"  --x_axis_label="#Delta R_{min}(#tau,tag1)" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=6.5 \
    --y_axis_min=0.01 --extra_pad=100 \
    --rebin=10 \
    --plot_wjets_comp=false \
    --norm_bins=false \
    --plot_qcd=false \
    --log_y=true \
    --paramfile=$PARAMS

./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="dRmin_taujet2"  --x_axis_label="#Delta R_{min}(#tau,tag2)" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=6.5 \
    --y_axis_min=0.01 --extra_pad=100 \
    --rebin=10 \
    --plot_wjets_comp=false \
    --norm_bins=false \
    --plot_qcd=false \
    --log_y=true \
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

###### mu dxy
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mudxy_all"  --x_axis_label="Muon d_{xy}" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=2 \
    --norm_bins=false \
    --paramfile=$PARAMS

###### mu dz
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="mudz_all"  --x_axis_label="Muon d_{z}" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=2 \
    --norm_bins=false \
    --paramfile=$PARAMS


###### mu iso
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="muiso_all"  --x_axis_label="Muon iso" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=2 \
    --norm_bins=false \
    --paramfile=$PARAMS

###### e dxy
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="edxy_all"  --x_axis_label="Electron d_{xy}" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=2 \
    --norm_bins=false \
    --paramfile=$PARAMS

###### e dz
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="edz_all"  --x_axis_label="Electron d_{z}" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=2 \
    --norm_bins=false \
    --paramfile=$PARAMS

###### e iso
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR \
    --plot_name="eiso_all"  --x_axis_label="Electron iso" \
    --blind=$BLIND  --x_blind_min=1 --x_blind_max=0. \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=2 \
    --norm_bins=false \
    --paramfile=$PARAMS



  done
done