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

### To run without making post-fit plots, comment out the definitions below.

SCALE_ET_INC="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.033604 --ztt_shift=1.003582 \
              --qcd_shift=1.127000 --top_shift=0.748358 --zl_shift=1.104716 --zj_shift=1.125488 --w_shift=1.000000 --vv_shift=0.709526 \
              --shift_tscale=true --tscale_shift=-0.4"

SCALE_ET_JET="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.044062 --ztt_shift=0.998062 \
              --qcd_shift=1.033000 --top_shift=0.773659 --zl_shift=0.781297 --zj_shift=0.907874 --w_shift=1.064000 --vv_shift=0.719635 \
              --shift_tscale=true --tscale_shift=-0.4"

SCALE_ET_MT="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.033604 --ztt_shift=1.003582 \
              --qcd_shift=1.127000 --top_shift=0.748358 --zl_shift=1.104716 --zj_shift=1.125488 --w_shift=1.000000 --vv_shift=0.709526 \
              --shift_tscale=true --tscale_shift=-0.4"

SCALE_MT_INC="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.029575 --ztt_shift=0.960251 \
              --qcd_shift=1.133200 --top_shift=0.714139 --zl_shift=1.059042 --zj_shift=1.462669 --w_shift=0.918000 --vv_shift=0.680721 \
              --shift_tscale=true --tscale_shift=-0.3"

SCALE_MT_JET="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.055069 --ztt_shift=0.957370 \
              --qcd_shift=1.256000 --top_shift=0.698626 --zl_shift=1.088694 --zj_shift=1.021515 --w_shift=0.985000 --vv_shift=0.671322 \
              --shift_tscale=true --tscale_shift=-0.3"

SCALE_MT_MT="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.029575 --ztt_shift=0.960251 \
              --qcd_shift=1.133200 --top_shift=0.714139 --zl_shift=1.059042 --zj_shift=1.462669 --w_shift=1.000000 --vv_shift=0.680721 \
              --shift_tscale=true --tscale_shift=-0.3"


## Same-sign mass plots:
# CATS=('8' '3' '2' '1' '0')
# for i in "${CATS[@]}"
# do
#   ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=0 --channel=et --rebin=1  \
#     --method="$i" --plot_name="m_sv_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
#     --blind=false --x_blind_min=100 --x_blind_max=160 --norm_bins=true --verbose=false \
#     --paramfile=$PARAMS --draw_ss=true

#   ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=0 --channel=mt --rebin=1 \
#     --method="$i" --plot_name="m_sv_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
#     --blind=false --x_blind_min=100 --x_blind_max=160 --norm_bins=true --verbose=false \
#     --paramfile=$PARAMS --draw_ss=true
# done

# CATS=('5')
# for i in "${CATS[@]}"
# do
#   ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=0 --channel=et --rebin=1  \
#     --method="$i" --plot_name="m_sv_sm"  --x_axis_label="m_{#tau#tau} [GeV]" \
#     --blind=false --x_blind_min=100 --x_blind_max=160 --norm_bins=true --verbose=false \
#     --paramfile=$PARAMS --draw_ss=true

#   ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=0 --channel=mt --rebin=1 \
#     --method="$i" --plot_name="m_sv_sm"  --x_axis_label="m_{#tau#tau} [GeV]" \
#     --blind=false --x_blind_min=100 --x_blind_max=160 --norm_bins=true --verbose=false \
#     --paramfile=$PARAMS --draw_ss=true
# done

###############################################################################
#### Inclusive selection plots
###############################################################################


#### SVFit Mass
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine"  \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS

#### Visible Mass
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=1 \
  --method=8 --category="inclusive" --plot_name="m_vis_sm_fine" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" --norm_bins=true \
  --paramfile=$PARAMS

#### MET
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="met" --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
  --paramfile=$PARAMS --extra_pad=1.1 $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt  --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="met"  --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
  --paramfile=$PARAMS --extra_pad=1.1 $SCALE_MT_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em  --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="met"  --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
  --paramfile=$PARAMS --extra_pad=1.1

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
  --paramfile=$PARAMS --extra_pad=1.1 $SCALE_ET_INC

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="m_2"  --x_axis_label="Tau Mass [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.1 $SCALE_MT_INC

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
  --method=8 --category="inclusive" --plot_name="mt_1" --x_axis_label="m_{T} [GeV]" --extra_pad=1.15 \
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
  --paramfile=$PARAMS --extra_pad=1.2 $SCALE_ET_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 -\
  --paramfile=$PARAMS --extra_pad=1.2 $SCALE_MT_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=2 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.2

#### jdeta
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=2 --non_mass_plot=true  \
  --method=8 --category="twojet" --plot_name="jdeta"  --x_axis_label="#Delta#eta_{jj}" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.2 $SCALE_ET_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jdeta"  --x_axis_label="#Delta#eta_{jj}" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.2 $SCALE_MT_JET

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=em --rebin=2 --non_mass_plot=true \
  --method=8 --category="twojet" --plot_name="jdeta"  --x_axis_label="#Delta#eta_{jj}" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --paramfile=$PARAMS --extra_pad=1.2