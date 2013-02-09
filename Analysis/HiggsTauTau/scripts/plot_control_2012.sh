PROD=Moriond
PARAMS=./scripts/"$PROD"_params_2012.dat


SCALE_ET_INC=""
SCALE_ET_JET=""
SCALE_ET_MT=""
SCALE_MT_INC=""
SCALE_MT_JET=""
SCALE_MT_MT=""
SCALE_EM_INC=""
SCALE_EM_JET=""
SCALE_EM_MT=""

SCALE_ET_INC="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.034515 --ztt_shift=1.029518 \
              --qcd_shift=0.970000 --top_shift=0.902772 --zl_shift=0.952106 --zj_shift=1.005999 --w_shift=1.244000 --vv_shift=0.769534"

SCALE_ET_JET="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.044761 --ztt_shift=1.005839 \
              --qcd_shift=1.029000 --top_shift=0.875811 --zl_shift=0.665334 --zj_shift=0.954358 --w_shift=1.051000 --vv_shift=0.762422"

SCALE_ET_MT="--shift_backgrounds=false --draw_band_on_stack=true --band_size_fractional=0.034515 --ztt_shift=1.029518 \
              --qcd_shift=0.970000 --top_shift=0.902772 --zl_shift=0.952106 --zj_shift=1.005999 --w_shift=1.244000 --vv_shift=0.769534"

SCALE_MT_INC="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.029962 --ztt_shift=0.972336 \
              --qcd_shift=1.034200 --top_shift=0.849368 --zl_shift=1.148073 --zj_shift=1.022327 --w_shift=1.048000 --vv_shift=0.729619"

SCALE_MT_JET="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.055049 --ztt_shift=0.960668 \
              --qcd_shift=1.247000 --top_shift=0.825925 --zl_shift=1.142247 --zj_shift=1.043585 --w_shift=0.974000 --vv_shift=0.717769"

SCALE_MT_MT="--shift_backgrounds=false --draw_band_on_stack=true --band_size_fractional=0.029962 --ztt_shift=0.972336 \
              --qcd_shift=1.034200 --top_shift=0.849368 --zl_shift=1.148073 --zj_shift=1.022327 --w_shift=1.048000 --vv_shift=0.729619"


###############################################################################
#### Inclusive selection plots
###############################################################################


#### SVFit Mass
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine" \
  --x_axis_label="m_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine" \
  --x_axis_label="m_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine"  \
  --x_axis_label="m_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS

#### Visible Mass
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  \
  --x_axis_label="m_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  \
  --x_axis_label="m_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_vis_sm_fine" \
  --x_axis_label="m_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS

#### MET
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="met" --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
  --paramfile=$PARAMS $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt  --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="met"  --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
  --paramfile=$PARAMS $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em  --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="met"  --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
  --paramfile=$PARAMS

#### pt_1
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=2 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="pt_1"  --x_axis_label="Electron p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="pt_1"  --x_axis_label="Muon p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="pt_1"  --x_axis_label="Electron p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS

#### pt_2
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="pt_2"  --x_axis_label="Tau p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="pt_2"  --x_axis_label="Tau p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em  --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="pt_2"  --x_axis_label="Muon p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS

#### eta_1
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=2 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="eta_1"  --x_axis_label="Electron #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.5 $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="eta_1"  --x_axis_label="Muon #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.4 $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="eta_1"  --x_axis_label="Electron #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.4

#### eta_2
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="eta_2"  --x_axis_label="Tau #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.5 $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="eta_2"  --x_axis_label="Tau #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.4 $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="eta_2"  --x_axis_label="Muon #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.4

#### m_2
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="m_2"  --x_axis_label="Tau Mass [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.0 $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="m_2"  --x_axis_label="Tau Mass [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.0 $SCALE_MT_INC

#### n_vtx
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 --non_mass_plot=true   \
  --method=8 --category="inclusive" --plot_name="n_vtx"  --x_axis_label="Number of Vertices" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="n_vtx"  --x_axis_label="Number of Vertices" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=1 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="n_vtx"  --x_axis_label="Number of Vertices" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS

#### n_jets
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=false --paramfile=$PARAMS --log_y=false $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=false --paramfile=$PARAMS --log_y=false $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=false --paramfile=$PARAMS --log_y=false

#### n_bjets
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=false --paramfile=$PARAMS --log_y=false $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=false --paramfile=$PARAMS --log_y=false $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=false --paramfile=$PARAMS --log_y=false


#### n_jets (log)
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=false --paramfile=$PARAMS --log_y=true $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=false --paramfile=$PARAMS --log_y=true $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=false --paramfile=$PARAMS --log_y=true

#### n_bjets (log)
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=false --paramfile=$PARAMS --log_y=true $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=false --paramfile=$PARAMS --log_y=true $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --draw_ratio=false --paramfile=$PARAMS --log_y=true

#### mt_1 / pzeta
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="mt_1" --x_axis_label="m_{T} [GeV]" \
  --paramfile=$PARAMS --replace_os_sel="os"  --replace_ss_sel="ss" $SCALE_ET_MT

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="mt_1" --x_axis_label="m_{T} [GeV]" \
  --paramfile=$PARAMS --replace_os_sel="os"  --replace_ss_sel="ss" $SCALE_MT_MT

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=1 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="pzeta" --x_axis_label="#slash{P}_{#zeta} - 0.85 #times P_{#zeta}^{vis}" \
  --paramfile=$PARAMS --replace_os_sel="os"  --replace_ss_sel="ss"

#### tau_decay_mode
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 --non_mass_plot=true   \
  --method=8 --category="inclusive" --plot_name="tau_decay_mode"  --x_axis_label="Tau Decay Mode" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.5 --draw_ratio=false \
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="tau_decay_mode"  --x_axis_label="Tau Decay Mode" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.4 --draw_ratio=false \
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=1 --non_mass_plot=true  \
  --method=8 --category="inclusive" --plot_name="tau_decay_mode"  --x_axis_label="Tau Decay Mode" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.4 --draw_ratio=false \
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong"

###############################################################################
#### Jet selection plots
###############################################################################

#### jpt_1
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=2 --non_mass_plot=true \
  --method=8 --category="1jet" --plot_name="jpt_1" --x_axis_label="Leading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS $SCALE_ET_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="1jet" --plot_name="jpt_1" --x_axis_label="Leading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS $SCALE_MT_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=2 --non_mass_plot=true \
  --method=8 --category="1jet" --plot_name="jpt_1" --x_axis_label="Leading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS

#### jeta_1
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="1jet" --plot_name="jeta_1" --x_axis_label="Leading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.6 $SCALE_ET_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="1jet" --plot_name="jeta_1" --x_axis_label="Leading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.4 $SCALE_MT_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=1 --non_mass_plot=true \
  --method=8 --category="1jet" --plot_name="jeta_1" --x_axis_label="Leading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.4

#### jpt_2
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=2 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jpt_2" --x_axis_label="Subleading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS $SCALE_ET_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jpt_2" --x_axis_label="Subleading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS $SCALE_MT_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=2 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jpt_2" --x_axis_label="Subleading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  --paramfile=$PARAMS

#### jeta_2
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jeta_2" --x_axis_label="Subleading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.6 $SCALE_ET_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jeta_2" --x_axis_label="Subleading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.4 $SCALE_MT_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=1 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jeta_2" --x_axis_label="Subleading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.4


#### mjj
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=2 --non_mass_plot=true  \
  --method=8 --category="twojet" --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.0 $SCALE_ET_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 -\
  --paramfile=$PARAMS --extra_pad=1.0 $SCALE_MT_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=2 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.0

#### jdeta
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=2 --non_mass_plot=true  \
  --method=8 --category="twojet" --plot_name="jdeta"  --x_axis_label="#Delta#eta_{jj}" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.0 $SCALE_ET_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jdeta"  --x_axis_label="#Delta#eta_{jj}" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.0 $SCALE_MT_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=2 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jdeta"  --x_axis_label="#Delta#eta_{jj}" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.0