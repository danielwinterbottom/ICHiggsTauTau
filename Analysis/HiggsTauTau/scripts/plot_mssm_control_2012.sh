PROD=Paper
YEAR=2012
PARAMS=./scripts/"$PROD"_params_$YEAR.dat
#SCALE_ET_JET="--draw_band_on_stack=true --band_size_fractional=0.09"
#SCALE_MT_JET="--draw_band_on_stack=true --band_size_fractional=0.09"
#SCALE_ET_INC="--draw_band_on_stack=true --band_size_fractional=0.09"
#SCALE_MT_INC="--draw_band_on_stack=true --band_size_fractional=0.09"
#SCALE_MT_MT="--draw_band_on_stack=true --band_size_fractional=0.09"
#SCALE_ET_MT="--draw_band_on_stack=true --band_size_fractional=0.09"
SCALE_ET_JET=""
SCALE_MT_JET=""
SCALE_ET_INC=""
SCALE_MT_INC=""
SCALE_MT_MT=""
SCALE_ET_MT=""

#Jets
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=true --paramfile=$PARAMS --log_y=true --extra_pad=5

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=true --paramfile=$PARAMS --log_y=true --extra_pad=5

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=true --paramfile=$PARAMS --log_y=true --extra_pad=5


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=true --paramfile=$PARAMS --log_y=true --extra_pad=5

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=true --paramfile=$PARAMS --log_y=true --extra_pad=5

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=true --paramfile=$PARAMS --log_y=true --extra_pad=5


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="prebtag" --plot_name="bcsv_1" --x_axis_label="CSV Discriminator" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1 \
  --paramfile=$PARAMS $SCALE_ET_JET --log_y=false --extra_pad=1.1

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="prebtag" --plot_name="bcsv_1" --x_axis_label="CSV Discriminator" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1 \
  --paramfile=$PARAMS $SCALE_MT_JET --log_y=false --extra_pad=1.1

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=1 --non_mass_plot=true \
  --method=8 --category="prebtag" --plot_name="bcsv_1" --x_axis_label="CSV Discriminator" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1 \
  --paramfile=$PARAMS $SCALE_MT_JET --log_y=false --extra_pad=1.1


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=5 --non_mass_plot=true \
  --method=8 --category="btag" --plot_name="bpt_1" --x_axis_label="Leading b-tagged jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS $SCALE_ET_JET --log_y=false --extra_pad=1.1

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=5 --non_mass_plot=true \
  --method=8 --category="btag" --plot_name="bpt_1" --x_axis_label="Leading b-tagged jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS $SCALE_MT_JET --log_y=false --extra_pad=1.1

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=5 --non_mass_plot=true \
  --method=8 --category="btag" --plot_name="bpt_1" --x_axis_label="Leading b-tagged jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS $SCALE_MT_JET --log_y=false --extra_pad=1.1

  
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=5 --non_mass_plot=true \
  --method=8 --category="btag" --plot_name="beta_1" --x_axis_label="Leading b-tagged jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_ET_JET --log_y=false --extra_pad=1.7

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=5 --non_mass_plot=true \
  --method=8 --category="btag" --plot_name="beta_1" --x_axis_label="Leading b-tagged jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_MT_JET --log_y=false --extra_pad=1.7

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=5 --non_mass_plot=true \
  --method=8 --category="btag" --plot_name="beta_1" --x_axis_label="Leading b-tagged jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_MT_JET --log_y=false --extra_pad=1.7


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=2 --non_mass_plot=true \
  --method=8 --category="1jet" --plot_name="jpt_1" --x_axis_label="Leading jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS $SCALE_ET_JET --log_y=false --extra_pad=1.1

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="1jet" --plot_name="jpt_1" --x_axis_label="Leading jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS $SCALE_MT_JET --log_y=false --extra_pad=1.1

  ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=2 --non_mass_plot=true \
  --method=8 --category="1jet" --plot_name="jpt_1" --x_axis_label="Leading jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS $SCALE_MT_JET --log_y=false --extra_pad=1.1


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="1jet" --plot_name="jeta_1" --x_axis_label="Leading jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_ET_JET --log_y=false --extra_pad=1.7

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="1jet" --plot_name="jeta_1" --x_axis_label="Leading jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_MT_JET --log_y=false --extra_pad=1.5

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=1 --non_mass_plot=true \
  --method=8 --category="1jet" --plot_name="jeta_1" --x_axis_label="Leading jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_MT_JET --log_y=false --extra_pad=1.5


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=2 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jpt_2" --x_axis_label="Subleading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS $SCALE_ET_JET --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jpt_2" --x_axis_label="Subleading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS $SCALE_MT_JET --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=2 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jpt_2" --x_axis_label="Subleading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS $SCALE_MT_JET --log_y=false


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jeta_2" --x_axis_label="Subleading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.6 $SCALE_ET_JET --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jeta_2" --x_axis_label="Subleading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.6 $SCALE_MT_JET --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=1 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jeta_2" --x_axis_label="Subleading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.6 $SCALE_MT_JET --log_y=false

  
#Kinematic variables

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="met" --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
  --paramfile=$PARAMS --extra_pad=1.1 $SCALE_ET_INC --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="met" --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
  --paramfile=$PARAMS --extra_pad=1.1 $SCALE_MT_INC --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="met" --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
  --paramfile=$PARAMS --extra_pad=1.1 $SCALE_MT_INC --log_y=false


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=2 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="pt_1"  --x_axis_label="Electron p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_ET_INC --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="pt_1"  --x_axis_label="Muon p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_MT_INC --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="pt_1"  --x_axis_label="Electron p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_MT_INC --log_y=false

  
  ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=2 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="pt_2"  --x_axis_label="Tau p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_ET_INC --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="pt_2"  --x_axis_label="Tau p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_MT_INC --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="pt_2"  --x_axis_label="Muon p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_MT_INC --log_y=false


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=2 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="eta_1"  --x_axis_label="Electron #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.7 $SCALE_ET_INC --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="eta_1"  --x_axis_label="Muon #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.4 $SCALE_MT_INC --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="eta_1"  --x_axis_label="Electron #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.4 $SCALE_MT_INC --log_y=false


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=2 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="eta_2"  --x_axis_label="Tau #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.7 $SCALE_ET_INC --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="eta_2"  --x_axis_label="Tau #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.5 $SCALE_MT_INC --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="eta_2"  --x_axis_label="Muon #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.5 $SCALE_MT_INC --log_y=false


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=1 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="mt_1" --x_axis_label="m_{T} [GeV]" --extra_pad=1.5 \
  --paramfile=$PARAMS --replace_os_sel="os"  --replace_ss_sel="ss" $SCALE_ET_MT --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=1 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="mt_1" --x_axis_label="m_{T} [GeV]" \
  --paramfile=$PARAMS --replace_os_sel="os"  --replace_ss_sel="ss" $SCALE_MT_MT --log_y=false

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=1 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="pzeta" --x_axis_label="#slash{P}_{#zeta} - 0.85 #times P_{#zeta}^{vis}" \
  --paramfile=$PARAMS --extra_pad=1.4 --replace_os_sel="os"  --replace_ss_sel="ss" --log_y=false

# Mass plots
## Inclusive with MSSM binning
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_sv_mssm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_ET_INC \
  --blind=true --x_blind_min=110 --x_blind_max=1500

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_sv_mssm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_MT_INC \
  --blind=true --x_blind_min=110 --x_blind_max=1500

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_sv_mssm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_EM_INC \
  --blind=true --x_blind_min=110 --x_blind_max=1500

## Inclusive with SM binning
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_ET_INC --log_y=false \
  --blind=true --x_blind_min=110 --x_blind_max=1500

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_MT_INC --log_y=false \
  --blind=true --x_blind_min=110 --x_blind_max=1500

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_EM_INC --log_y=false \
  --blind=true --x_blind_min=110 --x_blind_max=1500

## Inclusive mvis 
  
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_vis" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_ET_INC --log_y=false \

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_vis" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_MT_INC --log_y=false \

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_vis" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_EM_INC --log_y=false \


## btag with MSSM binning
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=1 \
  --method=12 --category="btag" --plot_name="m_sv_mssm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_ET_INC \
  --blind=true --x_blind_min=110 --x_blind_max=1500

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=1 \
  --method=12 --category="btag" --plot_name="m_sv_mssm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_MT_INC \
  --blind=true --x_blind_min=110 --x_blind_max=1500

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=1 \
  --method=12 --category="btag" --plot_name="m_sv_mssm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_EM_INC \
  --blind=true --x_blind_min=110 --x_blind_max=1500

## btag with SM binning
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=1 \
  --method=12 --category="btag" --plot_name="m_sv_sm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_ET_INC --log_y=false \
  --blind=true --x_blind_min=110 --x_blind_max=1500

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=1 \
  --method=12 --category="btag" --plot_name="m_sv_sm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_MT_INC --log_y=false \
  --blind=true --x_blind_min=110 --x_blind_max=1500

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=1 \
  --method=12 --category="btag" --plot_name="m_sv_sm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_EM_INC --log_y=false \
  --blind=true --x_blind_min=110 --x_blind_max=1500

## nobtag with MSSM binning
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=1 \
  --method=11 --category="nobtag" --plot_name="m_sv_mssm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_ET_INC \
  --blind=true --x_blind_min=110 --x_blind_max=1500

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=1 \
  --method=11 --category="nobtag" --plot_name="m_sv_mssm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_MT_INC \
  --blind=true --x_blind_min=110 --x_blind_max=1500

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=1 \
  --method=11 --category="nobtag" --plot_name="m_sv_mssm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_EM_INC \
  --blind=true --x_blind_min=110 --x_blind_max=1500

## nobtag with SM binning
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=et --rebin=1 \
  --method=11 --category="nobtag" --plot_name="m_sv_sm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_ET_INC --log_y=false \
  --blind=true --x_blind_min=110 --x_blind_max=1500

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=mt --rebin=1 \
  --method=11 --category="nobtag" --plot_name="m_sv_sm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_MT_INC --log_y=false \
  --blind=true --x_blind_min=110 --x_blind_max=1500

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_$YEAR.cfg --channel=em --rebin=1 \
  --method=11 --category="nobtag" --plot_name="m_sv_sm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_EM_INC --log_y=false \
  --blind=true --x_blind_min=110 --x_blind_max=1500

