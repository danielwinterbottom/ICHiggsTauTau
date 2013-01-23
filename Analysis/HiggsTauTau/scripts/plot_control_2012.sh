
PROD=Moriond
PARAMS=./scripts/"$PROD"_params_2012.dat


#   ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
#     --method=1 --category="0jet_high" --plot_name="eta_1"  --x_axis_label="Electron #eta" \
#     --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
#     --make_datacard=false --norm_bins=false --verbose=false \
#     --paramfile=./scripts/Moriond_params_2012.dat --extra_pad=1.8

# ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
#   --method=1 --category="0jet_high" --plot_name="pt_2"  --x_axis_label="Muon p_{T} [GeV]" \
#   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=1 \
#   --make_datacard=false --norm_bins=false --verbose=false \
#   --paramfile=./scripts/Moriond_params_2012.dat

# ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
#   --method=1 --category="0jet_high" --plot_name="pt_1"  --x_axis_label="Electron p_{T} [GeV]" \
#   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2 \
#   --make_datacard=false --norm_bins=false --verbose=false \
#   --paramfile=./scripts/Moriond_params_2012.dat


# ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em --rebin=1  \
#   --method=1 --category="0jet_high" --plot_name="pzeta"  --x_axis_label="#slash{P}_{#zeta} - 0.85 #times P_{#zeta}^{vis}" \
#   --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=false --verbose=false \
#   --paramfile=./scripts/Moriond_params_2012.dat --replace_os_sel="os"  --replace_ss_sel="ss"


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
  --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
  --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
  --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
  --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
  --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
  --paramfile=$PARAMS

###### MET
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="met"  --x_axis_label="MVA MET [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="met"  --x_axis_label="MVA MET [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="met"  --x_axis_label="MVA MET [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS

###### pt_1
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="pt_1"  --x_axis_label="Electron p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="pt_1"  --x_axis_label="Muon p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="pt_1"  --x_axis_label="Electron p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS

###### pt_2
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="pt_2"  --x_axis_label="Tau p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="pt_2"  --x_axis_label="Tau p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="pt_2"  --x_axis_label="Muon p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS

  ###### eta_1
  ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et  \
    --method=8 --category="inclusive" --plot_name="eta_1"  --x_axis_label="Electron #eta" \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
    --make_datacard=false --norm_bins=false --verbose=false \
    --paramfile=$PARAMS --extra_pad=1.4

  ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt \
    --method=8 --category="inclusive" --plot_name="eta_1"  --x_axis_label="Muon #eta" \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
    --make_datacard=false --norm_bins=false --verbose=false \
    --paramfile=$PARAMS --extra_pad=1.4

  ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
    --method=8 --category="inclusive" --plot_name="eta_1"  --x_axis_label="Electron #eta" \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
    --make_datacard=false --norm_bins=false --verbose=false \
    --paramfile=$PARAMS --extra_pad=1.4


###### eta_2
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="eta_2"  --x_axis_label="Tau #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.4

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="eta_2"  --x_axis_label="Tau #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.4

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="eta_2"  --x_axis_label="Muon #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.4

###### n_vtx
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="n_vtx"  --x_axis_label="Number of Vertices" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="n_vtx"  --x_axis_label="Number of Vertices" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="n_vtx"  --x_axis_label="Number of Vertices" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS

###### n_jets
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=true \
  --paramfile=$PARAMS --log_y=true

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=true \
  --paramfile=$PARAMS --log_y=true

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=true \
  --paramfile=$PARAMS --log_y=true

###### n_bjets
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=true \
  --paramfile=$PARAMS --log_y=true

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=true \
  --paramfile=$PARAMS --log_y=true

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="n_bjets"  --x_axis_label="Number of b-tagged Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false --draw_ratio=true \
  --paramfile=$PARAMS --log_y=true



###### jpt_1
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS

###### jeta_1
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="jeta_1"  --x_axis_label="Leading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.4

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="jeta_1"  --x_axis_label="Leading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.4

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="jeta_1"  --x_axis_label="Leading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.4

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et --rebin=1  \
  --method=8 --category="inclusive" --plot_name="mt_1"  --x_axis_label="m_{T} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --replace_os_sel="os"  --replace_ss_sel="ss"

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt --rebin=1  \
  --method=8 --category="inclusive" --plot_name="mt_1"  --x_axis_label="m_{T} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --replace_os_sel="os"  --replace_ss_sel="ss"

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em --rebin=1  \
  --method=8 --category="inclusive" --plot_name="pzeta"  --x_axis_label="#slash{P}_{#zeta} - 0.85 #times P_{#zeta}^{vis}" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --replace_os_sel="os"  --replace_ss_sel="ss"

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="tau_decay_mode"  --x_axis_label="Tau Decay Mode" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.4 --draw_ratio=true \
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong"

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="tau_decay_mode"  --x_axis_label="Tau Decay Mode" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.4 --draw_ratio=true \
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong"

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="tau_decay_mode"  --x_axis_label="Tau Decay Mode" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.4 --draw_ratio=true \
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong"


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.0

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.0

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.0

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="jdeta"  --x_axis_label="#Delta#eta_{jj}" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.0

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="jdeta"  --x_axis_label="#Delta#eta_{jj}" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.0

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="jdeta"  --x_axis_label="#Delta#eta_{jj}" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.0



### Some extra checks
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --tau_scale_mode=0 --channel=em \
  --method=1 --category="0jet_high" --plot_name="eta_2"  --x_axis_label="Muon #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=false \
  --paramfile=$PARAMS --extra_pad=1.8



