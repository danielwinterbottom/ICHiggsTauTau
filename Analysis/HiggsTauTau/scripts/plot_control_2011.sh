PROD=default
PARAMS=./scripts/"$PROD"_params_2011.dat
CONFIG=scripts/plot_sm_2011.cfg

# SCALE_ET_INC="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.030 --ztt_shift=0.991 \
#               --qcd_shift=1.003 --top_shift=0.849 --zl_shift=1.002 --zj_shift=1.000 --w_shift=1.010 --vv_shift=0.787"
# SCALE_MT_INC="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.025 --ztt_shift=0.976 \
#               --qcd_shift=1.145 --top_shift=0.836 --zl_shift=0.837 --zj_shift=0.951 --w_shift=1.004 --vv_shift=0.777"
# SCALE_ET_JET="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.029 --ztt_shift=1.004 \
#               --qcd_shift=0.981 --top_shift=0.847 --zl_shift=1.014 --zj_shift=0.891 --w_shift=0.945 --vv_shift=0.790"
# SCALE_MT_JET="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.028 --ztt_shift=0.989 \
#               --qcd_shift=1.168 --top_shift=0.834 --zl_shift=0.844 --zj_shift=0.981 --w_shift=1.117 --vv_shift=0.780"

SCALE_ET_INC=""
SCALE_MT_INC=""
SCALE_ET_JET=""
SCALE_MT_JET=""

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
  --paramfile=$PARAMS $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
  --paramfile=$PARAMS $SCALE_MT_INC

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em --rebin=1  \
#   --method=8 --category="inclusive" --plot_name="m_sv_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
#   --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
#   --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
  --paramfile=$PARAMS $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
  --paramfile=$PARAMS $SCALE_MT_INC

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em --rebin=1  \
#   --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
#   --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
#   --paramfile=$PARAMS

###### MET
./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="met"  --x_axis_label="MVA MET [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="met"  --x_axis_label="MVA MET [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS $SCALE_MT_INC

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em \
#   --method=8 --category="inclusive" --plot_name="met"  --x_axis_label="MVA MET [GeV]" \
#   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 --rebin=1 \
#   --make_datacard=false --norm_bins=false --verbose=false \
#   --paramfile=$PARAMS

###### pt_1
./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="pt_1"  --x_axis_label="Electron p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="pt_1"  --x_axis_label="Muon p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS $SCALE_MT_INC

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em \
#   --method=8 --category="inclusive" --plot_name="pt_1"  --x_axis_label="Electron p_{T} [GeV]" \
#   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=1 \
#   --make_datacard=false --norm_bins=false --verbose=false \
#   --paramfile=$PARAMS

###### pt_2
./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="pt_2"  --x_axis_label="Tau p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="pt_2"  --x_axis_label="Tau p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS $SCALE_MT_INC

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em \
#   --method=8 --category="inclusive" --plot_name="pt_2"  --x_axis_label="Muon p_{T} [GeV]" \
#   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=1 \
#   --make_datacard=false --norm_bins=false --verbose=false \
#   --paramfile=$PARAMS

###### eta_1
./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="eta_1"  --x_axis_label="Electron #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.6 $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="eta_1"  --x_axis_label="Muon #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.5 $SCALE_MT_INC

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em \
#   --method=8 --category="inclusive" --plot_name="eta_1"  --x_axis_label="Electron #eta" \
#   --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
#   --make_datacard=false --norm_bins=false --verbose=false \
#   --paramfile=$PARAMS --extra_pad=1.4


###### eta_2
./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="eta_2"  --x_axis_label="Tau #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.5 $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="eta_2"  --x_axis_label="Tau #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.5 $SCALE_MT_INC

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em \
#   --method=8 --category="inclusive" --plot_name="eta_2"  --x_axis_label="Muon #eta" \
#   --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
#   --make_datacard=false --norm_bins=false --verbose=false \
#   --paramfile=$PARAMS --extra_pad=1.4

###### n_vtx
./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="n_vtx"  --x_axis_label="Number of Vertices" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="n_vtx"  --x_axis_label="Number of Vertices" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS $SCALE_MT_INC

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em \
#   --method=8 --category="inclusive" --plot_name="n_vtx"  --x_axis_label="Number of Vertices" \
#   --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
#   --make_datacard=false --norm_bins=false --verbose=false \
#   --paramfile=$PARAMS

###### n_jets (log)
./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=true \
  --paramfile=$PARAMS --log_y=true $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=true \
  --paramfile=$PARAMS --log_y=true $SCALE_MT_INC

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em \
#   --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
#   --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
#   --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=true \
#   --paramfile=$PARAMS --log_y=true

###### n_jets
./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=true --x_axis_min=-0.5 --x_axis_max=5.5 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=false \
  --paramfile=$PARAMS --log_y=false $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=true --x_axis_min=-0.5 --x_axis_max=5.5 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=false \
  --paramfile=$PARAMS --log_y=false $SCALE_MT_INC

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em \
#   --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
#   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=1 \
#   --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=false \
#   --paramfile=$PARAMS --log_y=false

###### n_bjets (log)
./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=true \
  --paramfile=$PARAMS --log_y=true $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=true \
  --paramfile=$PARAMS --log_y=true $SCALE_MT_INC

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em \
#   --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
#   --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
#   --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=true \
#   --paramfile=$PARAMS --log_y=true

###### n_bjets
./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=true --x_axis_min=-0.5 --x_axis_max=3.5 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=false \
  --paramfile=$PARAMS --log_y=false $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=true --x_axis_min=-0.5 --x_axis_max=3.5 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=false \
  --paramfile=$PARAMS --log_y=false $SCALE_MT_INC

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em \
#   --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
#   --custom_x_axis_range=true --x_axis_min=-0.5 --x_axis_max=4.5 --rebin=1 \
#   --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=false \
#   --paramfile=$PARAMS --log_y=false

###### jpt_1
./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS $SCALE_ET_JET

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS $SCALE_MT_JET

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em \
#   --method=8 --category="inclusive" --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
#   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=1 \
#   --make_datacard=false --norm_bins=false --verbose=false \
#   --paramfile=$PARAMS

###### jeta_1
./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="jeta_1"  --x_axis_label="Leading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.4 $SCALE_ET_JET

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="jeta_1"  --x_axis_label="Leading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.4 $SCALE_MT_JET

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em \
#   --method=8 --category="inclusive" --plot_name="jeta_1"  --x_axis_label="Leading Jet #eta" \
#   --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
#   --make_datacard=false --norm_bins=false --verbose=false \
#   --paramfile=$PARAMS --extra_pad=1.4

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et --rebin=1  \
  --method=8 --category="inclusive" --plot_name="mt_1"  --x_axis_label="m_{T} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --replace_os_sel="os"  --replace_ss_sel="ss" $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt --rebin=1  \
  --method=8 --category="inclusive" --plot_name="mt_1"  --x_axis_label="m_{T} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --replace_os_sel="os"  --replace_ss_sel="ss" $SCALE_MT_INC

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em --rebin=1  \
#   --method=8 --category="inclusive" --plot_name="pzeta"  --x_axis_label="#slash{P}_{#zeta} - 0.85 #times P_{#zeta}^{vis}" \
#   --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=false --verbose=false \
#   --paramfile=$PARAMS --replace_os_sel="os"  --replace_ss_sel="ss"

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="tau_decay_mode"  --x_axis_label="Tau Decay Mode" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.4 --draw_ratio=true \
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="tau_decay_mode"  --x_axis_label="Tau Decay Mode" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.4 --draw_ratio=true \
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" $SCALE_MT_INC

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em \
#   --method=8 --category="inclusive" --plot_name="tau_decay_mode"  --x_axis_label="Tau Decay Mode" \
#   --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
#   --make_datacard=false --norm_bins=false --verbose=false \
#   --paramfile=$PARAMS --extra_pad=1.4 --draw_ratio=true \
#   --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong"


./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.0 $SCALE_ET_JET

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.0 $SCALE_MT_JET

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em \
#   --method=8 --category="inclusive" --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
#   --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
#   --make_datacard=false --norm_bins=false --verbose=false \
#   --paramfile=$PARAMS --extra_pad=1.0

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="jdeta"  --x_axis_label="#Delta#eta_{jj}" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.0 $SCALE_ET_JET

./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="jdeta"  --x_axis_label="#Delta#eta_{jj}" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.0 $SCALE_MT_JET

# ./bin/HiggsTauTauPlot3 --cfg=$CONFIG --tau_scale_mode=0 --channel=em \
#   --method=8 --category="inclusive" --plot_name="jdeta"  --x_axis_label="#Delta#eta_{jj}" \
#   --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
#   --make_datacard=false --norm_bins=false --verbose=false \
#   --paramfile=$PARAMS --extra_pad=1.0