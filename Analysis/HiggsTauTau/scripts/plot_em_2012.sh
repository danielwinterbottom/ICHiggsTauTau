make -j4

#FOLDER="./output/HCP_2012"


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=et --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=mt --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=em --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_sv_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat



./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=et --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  --x_axis_label="Visible Mass [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=mt --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  --x_axis_label="Visible Mass [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=em --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  --x_axis_label="Visible Mass [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

###### MET
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="met"  --x_axis_label="MVA MET [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="met"  --x_axis_label="MVA MET [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="met"  --x_axis_label="MVA MET [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

###### pt_1
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="pt_1"  --x_axis_label="Electron p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="pt_1"  --x_axis_label="Muon p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="pt_1"  --x_axis_label="Electron p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

###### pt_2
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="pt_2"  --x_axis_label="Tau p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="pt_2"  --x_axis_label="Tau p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="pt_2"  --x_axis_label="Muon p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

  ###### eta_1
  ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=et  \
    --method=8 --category="inclusive" --plot_name="eta_1"  --x_axis_label="Electron #eta" \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
    --make_datacard=false --norm_bins=false --verbose=true \
    --paramfile=scripts/moriond_params_2012.dat --extra_pad=1.4

  ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=mt \
    --method=8 --category="inclusive" --plot_name="eta_1"  --x_axis_label="Muon #eta" \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
    --make_datacard=false --norm_bins=false --verbose=true \
    --paramfile=scripts/moriond_params_2012.dat --extra_pad=1.4

  ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=em \
    --method=8 --category="inclusive" --plot_name="eta_1"  --x_axis_label="Electron #eta" \
    --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
    --make_datacard=false --norm_bins=false --verbose=true \
    --paramfile=scripts/moriond_params_2012.dat --extra_pad=1.4


###### eta_2
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="eta_2"  --x_axis_label="Tau #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat --extra_pad=1.4

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="eta_2"  --x_axis_label="Tau #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat --extra_pad=1.4

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="eta_2"  --x_axis_label="Muon #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat --extra_pad=1.4

###### n_vtx
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="n_vtx"  --x_axis_label="Number of Vertices" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="n_vtx"  --x_axis_label="Number of Vertices" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="n_vtx"  --x_axis_label="Number of Vertices" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

###### n_jets
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat --log_y=true

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat --log_y=true

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="n_jets"  --x_axis_label="Number of Jets" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=1 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat --log_y=true

###### jpt_1
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=4 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=4 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=4 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

###### jeta_1
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=et  \
  --method=8 --category="inclusive" --plot_name="jeta_1"  --x_axis_label="Leading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat --extra_pad=1.4

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=mt \
  --method=8 --category="inclusive" --plot_name="jeta_1"  --x_axis_label="Leading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat --extra_pad=1.4

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=em \
  --method=8 --category="inclusive" --plot_name="jeta_1"  --x_axis_label="Leading Jet #eta" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2 \
  --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat --extra_pad=1.4

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=et --rebin=2  \
  --method=8 --category="inclusive" --plot_name="mt_1"  --x_axis_label="m_{T} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat --replace_os_sel="os"  --replace_ss_sel="ss"

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=mt --rebin=2  \
  --method=8 --category="inclusive" --plot_name="mt_1"  --x_axis_label="m_{T} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat --replace_os_sel="os"  --replace_ss_sel="ss"

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013_2012D --tau_scale_mode=0 --channel=em --rebin=2  \
  --method=8 --category="inclusive" --plot_name="pzeta"  --x_axis_label="#slash{P}_{#zeta} - 0.85 #times P_{#zeta}^{vis}" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=false --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat --replace_os_sel="os"  --replace_ss_sel="ss"






./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=./output/Moriond_2013 --tau_scale_mode=0 --channel=mtmet --rebin=1  \
  --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=true \
  --paramfile=scripts/moriond_params_2012.dat

#   ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=$FOLDER --tau_scale_mode=0 --channel=em --rebin=1  \
#     --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
#     --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=true \
#     --paramfile=scripts/moriond_params_2012.dat


# ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder="./output/Moriond_2013" --tau_scale_mode=0 --channel=em --rebin=1  \
#   --method=8 --category="inclusive" --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
#   --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=true \
#   --paramfile=scripts/moriond_params_2012.dat

  # ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=$FOLDER  --tau_scale_mode=0 --channel=em --rebin=1  \
  #   --method=3 --category="1jet_high" --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  #   --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=true \
  #   --paramfile=scripts/moriond_params_2012.dat

  # ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=$FOLDER  --tau_scale_mode=0 --channel=em --rebin=1  \
  #   --method=2 --category="1jet_low" --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  #   --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=true \
  #   --paramfile=scripts/moriond_params_2012.dat

  # ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=$FOLDER  --tau_scale_mode=0 --channel=em --rebin=1  \
  #   --method=0 --category="0jet_low" --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  #   --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=true \
  #   --paramfile=scripts/moriond_params_2012.dat

  # ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --folder=$FOLDER  --tau_scale_mode=0 --channel=em --rebin=1  \
  #   --method=1 --category="0jet_high" --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  #   --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=true \
  #   --paramfile=scripts/moriond_params_2012.dat

# for j in "${TSCALE[@]}"
# do
#   CATS=(
#    '8'
#    '3'
#    '2'
#    '1'
#    '0'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
#       --category="$i" --plot_name="svfit_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
#       --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
#       --category="$i" --plot_name="svfit_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
#       --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true  --norm_bins=true --verbose=false
#   done

#   CATS=(
#     '5'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
#       --category="$i" --plot_name="svfit_sm"  --x_axis_label="m_{#tau#tau} [GeV]" \
#       --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
#       --category="$i" --plot_name="svfit_sm"  --x_axis_label="m_{#tau#tau} [GeV]" \
#       --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true  --norm_bins=true --verbose=false
#   done
# done
# hadd -f htt_et.inputs-sm-8TeV.root datacard_eleTau_*.root
# rm datacard_eleTau_*.root
# hadd -f htt_mt.inputs-sm-8TeV.root datacard_muTau_*.root
# rm datacard_muTau_*.root

# ###### DO SM VISIBLE MASS DATACARDS ########
# for j in "${TSCALE[@]}"
# do
#   CATS=(
#    '8'
#    '3'
#    '2'
#    '1'
#    '0'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
#       --category="$i" --plot_name="vis_sm_fine"  --x_axis_label="Visible Mass [GeV]" \
#       --blind=false --x_blind_min=60 --x_blind_max=120 --make_datacard=true --norm_bins=true
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
#       --category="$i" --plot_name="vis_sm_fine"  --x_axis_label="Visible Mass [GeV]" \
#       --blind=false --x_blind_min=60 --x_blind_max=120 --make_datacard=true  --norm_bins=true
#   done
#   CATS=(
#     '5'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
#       --category="$i" --plot_name="vis_sm"  --x_axis_label="Visible Mass [GeV]" \
#       --blind=false --x_blind_min=60 --x_blind_max=120 --make_datacard=true --norm_bins=true
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
#       --category="$i" --plot_name="vis_sm"  --x_axis_label="Visible Mass [GeV]" \
#       --blind=false --x_blind_min=60 --x_blind_max=120 --make_datacard=true  --norm_bins=true
#   done
# done

# hadd -f htt_et.inputs-sm-8TeV-mvis.root datacard_eleTau_*.root
# rm datacard_eleTau_*.root
# hadd -f htt_mt.inputs-sm-8TeV-mvis.root datacard_muTau_*.root
# rm datacard_muTau_*.root

# # ##### MSSM #######
# for j in "${TSCALE[@]}"
# do
#   CATS=(
#    '8'
#    '11'
#    '3'
#    '2'
#    '1'
#    '0'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
#       --category="$i" --plot_name="svfit_mssm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=true --norm_bins=true
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
#       --category="$i" --plot_name="svfit_mssm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=true  --norm_bins=true
#   done
#   CATS=(
#     '7'
#     '6'
#     '12'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
#       --category="$i" --plot_name="svfit_mssm"  --x_axis_label="m_{#tau#tau} [GeV]" \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=true --norm_bins=true
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
#       --category="$i" --plot_name="svfit_mssm"  --x_axis_label="m_{#tau#tau} [GeV]" \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=true  --norm_bins=true
#   done
# done

# hadd -f htt_et.inputs-mssm-8TeV.root datacard_eleTau_*.root
# rm datacard_eleTau_*.root
# hadd -f htt_mt.inputs-mssm-8TeV.root datacard_muTau_*.root
# rm datacard_muTau_*.root



# for j in "${TSCALE[@]}"
# do
#   CATS=(
#    '8'
#    '11'
#    '3'
#    '2'
#    '1'
#    '0'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
#       --category="$i" --plot_name="vis_mssm_fine"  --x_axis_label="Visible Mass [GeV]" \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=true --norm_bins=true
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
#       --category="$i" --plot_name="vis_mssm_fine"  --x_axis_label="Visible Mass [GeV]" \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=true  --norm_bins=true
#   done
#   CATS=(
#     '12'
#     '7'
#     '6'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
#       --category="$i" --plot_name="vis_mssm"  --x_axis_label="Visible Mass [GeV]" \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=true --norm_bins=true
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
#       --category="$i" --plot_name="vis_mssm"  --x_axis_label="Visible Mass [GeV]" \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=true  --norm_bins=true
#   done
# done
# hadd -f htt_et.inputs-mssm-8TeV-mvis.root datacard_eleTau_*.root
# rm datacard_eleTau_*.root
# hadd -f htt_mt.inputs-mssm-8TeV-mvis.root datacard_muTau_*.root
# rm datacard_muTau_*.root


# ## Just for the MSSM plots with SM binning, no datacards
#  TSCALE=(
#  '0'
#  )

# for j in "${TSCALE[@]}"
#  do
#   CATS=(
#    '8'
#    '11'
#    '3'
#    '2'
#    '1'
#    '0'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
#       --category="$i" --plot_name="svfit_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=false --norm_bins=true
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
#       --category="$i" --plot_name="svfit_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]"  --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=false  --norm_bins=true
#   done
#   CATS=(
#     '7'
#     '6'
#     '12'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
#       --category="$i" --plot_name="svfit_sm"  --x_axis_label="m_{#tau#tau} [GeV]"  --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=false --norm_bins=true --extra_pad=1.4
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
#       --category="$i" --plot_name="svfit_sm"  --x_axis_label="m_{#tau#tau} [GeV]"  --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=false  --norm_bins=true --extra_pad=1.2
#   done
# done


#   CATS=(
#    '8'
#    '11'
#    '3'
#    '2'
#    '1'
#    '0'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=0 --mode=0 --rebin=1  \
#       --category="$i" --plot_name="vis_sm_fine"  --x_axis_label="Visible Mass [GeV]"  --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=false --norm_bins=true
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=0 --mode=1 --rebin=1 \
#       --category="$i" --plot_name="vis_sm_fine"  --x_axis_label="Visible Mass [GeV]"  --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=false  --norm_bins=true
#   done
#   CATS=(
#     '7'
#     '6'
#     '12'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=0 --mode=0 --rebin=1  \
#       --category="$i" --plot_name="vis_sm"  --x_axis_label="Visible Mass [GeV]"  --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=false --norm_bins=true
#     ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=0 --mode=1 --rebin=1 \
#       --category="$i" --plot_name="vis_sm"  --x_axis_label="Visible Mass [GeV]"  --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=false  --norm_bins=true
#   done




