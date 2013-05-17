PROD=Moriond
PARAMS=./scripts/"$PROD"_params_2012.dat
SCALE_ET_JET=""
SCALE_MT_JET="--draw_band_on_stack=true --band_size_fractional=0.09"


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=true --paramfile=$PARAMS --log_y=true --extra_pad=5

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=true --paramfile=$PARAMS --log_y=true --extra_pad=5

#### jpt_1
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg --channel=et --rebin=5 --non_mass_plot=true \
  --method=8 --category="btag" --plot_name="bpt_1" --x_axis_label="Leading b-tagged jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS $SCALE_ET_JET --log_y=false --extra_pad=1.1

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg --channel=mt --rebin=5 --non_mass_plot=true \
  --method=8 --category="btag" --plot_name="bpt_1" --x_axis_label="Leading b-tagged jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS $SCALE_MT_JET --log_y=false --extra_pad=1.1


#### jeta_1
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg --channel=et --rebin=5 --non_mass_plot=true \
  --method=8 --category="btag" --plot_name="beta_1" --x_axis_label="Leading b-tagged jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_ET_JET --log_y=false --extra_pad=1.5

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg --channel=mt --rebin=5 --non_mass_plot=true \
  --method=8 --category="btag" --plot_name="beta_1" --x_axis_label="Leading b-tagged jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_MT_JET --log_y=false --extra_pad=1.5
