cd ../../Modules
make -j4
cd ../Analysis
make -j4
cd HiggsTauTau


# --draw_band_on_stack=true --band_size_fractional= \
# --shift_background=true \
# --ztt_shift= \
# --qcd_shift= \
# --top_shift= \
# --zll_shift= \
# --w_shift= \
# --vv_shift=

CONFIG=scripts/plot_mssm_as_sm_2011.cfg


 ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=2  \
   --category=12 --plot_name="lead_bjet_pt"  --x_axis_label="Leading b-tagged Jet p_{T} [GeV]" \
   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
   --make_datacard=false --non_mass_plot=true \
   --draw_band_on_stack=true --band_size_fractional=0.047312 \
  --shift_background=true \
  --ztt_shift=0.976924 \
  --qcd_shift=1.150000 \
  --top_shift=0.866549 \
  --zll_shift=0.879083 \
  --w_shift=1.030000 \
  --vv_shift=0.726045


 ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=2 \
   --category=12 --plot_name="lead_bjet_pt"  --x_axis_label="Leading b-tagged Jet p_{T} [GeV]" \
   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
   --make_datacard=false --non_mass_plot=true \
   --draw_band_on_stack=true --band_size_fractional=0.037522 \
   --shift_background=true \
   --ztt_shift=0.979171 \
   --qcd_shift=0.962000 \
   --top_shift=0.867239 \
   --zll_shift=0.909931 \
   --w_shift=1.174000 \
   --vv_shift=0.727715

../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=4  \
   --category=12 --plot_name="lead_bjet_eta"  --x_axis_label="Leading b-tagged Jet #eta" \
   --custom_x_axis_range=true --x_axis_min=-3 --x_axis_max=3 \
   --make_datacard=false --non_mass_plot=true --extra_pad=1.55 \
   --draw_band_on_stack=true --band_size_fractional=0.047312 \
   --shift_background=true \
   --ztt_shift=0.976924 \
   --qcd_shift=1.150000 \
   --top_shift=0.866549 \
   --zll_shift=0.879083 \
   --w_shift=1.030000 \
   --vv_shift=0.726045

 ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=4 \
   --category=12 --plot_name="lead_bjet_eta"  --x_axis_label="Leading b-tagged Jet #eta" \
   --custom_x_axis_range=true --x_axis_min=-3 --x_axis_max=3 \
   --make_datacard=false --non_mass_plot=true --extra_pad=1.55 \
   --draw_band_on_stack=true --band_size_fractional=0.037522 \
   --shift_background=true \
   --ztt_shift=0.979171 \
   --qcd_shift=0.962000 \
   --top_shift=0.867239 \
   --zll_shift=0.909931 \
   --w_shift=1.174000 \
   --vv_shift=0.727715






# ../bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2012.cfg --mode=1 --rebin=4 \
# --category=1 --plot_name="mvamet_et"  --x_axis_label="E_{T}^{miss} [GeV]" \
# --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
# --make_datacard=false --non_mass_plot=true \
  # --shift_background=true \
  # --ztt_shift=0.947422469 \
  # --qcd_shift=0.947422469 \
  # --top_shift=0.947422469 \
  # --zll_shift=0.947422469 \
  # --w_shift=0.947422469 \
  # --vv_shift=0.947422469


#  CATS=(
#   '8'
#   '0'
#   '11'
#   '12'
#  )
#  for i in "${CATS[@]}"
#  do
#    ../bin/HiggsTauTauPlot2 --cfg=$CONFIG  --tau_scale_mode=0 --mode=0 --rebin=1  \
#      --category="$i" --plot_name="svfit_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
#      --make_datacard=false --norm_bins=true --draw_ss=true
#    ../bin/HiggsTauTauPlot2 --cfg=$CONFIG  --tau_scale_mode=0 --mode=1 --rebin=1 \
#      --category="$i" --plot_name="svfit_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
#      --make_datacard=false  --norm_bins=true --draw_ss=true
#  done

#  CATS=(
#    '5'
#    '7'
#    '6'
#    '1'
#    '3'
#    '2'
#  )
#  for i in "${CATS[@]}"
#  do
#    ../bin/HiggsTauTauPlot2 --cfg=$CONFIG  --tau_scale_mode=0 --mode=0 --rebin=1  \
#      --category="$i" --plot_name="svfit_sm"  --x_axis_label="m_{#tau#tau} [GeV]" \
#      --make_datacard=false --norm_bins=true --draw_ss=true --extra_pad=1.3
#    ../bin/HiggsTauTauPlot2 --cfg=$CONFIG  --tau_scale_mode=0 --mode=1 --rebin=1 \
#      --category="$i" --plot_name="svfit_sm"  --x_axis_label="m_{#tau#tau} [GeV]" \
#      --make_datacard=false  --norm_bins=true --draw_ss=true --extra_pad=1.3
#  done






# CATS=(
#  '7'
#  '6'
#  '5'
# )
# for i in "${CATS[@]}"
# do
#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=8  \
#    --category="$i" --plot_name="lep1_mvamet_mt_all"  --x_axis_label="m_{T} [GeV]" \
#    --make_datacard=false --non_mass_plot=true --extra_pad=1.3
#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=8 \
#    --category="$i" --plot_name="lep1_mvamet_mt_all"  --x_axis_label="m_{T}  [GeV]" \
#    --make_datacard=false --non_mass_plot=true --extra_pad=1.3

#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=10  \
#    --category="$i" --plot_name="lep1_pt"  --x_axis_label="Electron p_{T} [GeV]" \
#    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
#    --make_datacard=false --non_mass_plot=true --extra_pad=1.3
#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=10 \
#    --category="$i" --plot_name="lep1_pt"  --x_axis_label="Muon p_{T} [GeV]" \
#    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
#    --make_datacard=false --non_mass_plot=true --extra_pad=1.3

#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=10  \
#    --category="$i" --plot_name="lep2_pt"  --x_axis_label="Tau p_{T} [GeV]" \
#    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
#    --make_datacard=false --non_mass_plot=true
#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=10 \
#    --category="$i" --plot_name="lep2_pt"  --x_axis_label="Tau p_{T} [GeV]" \
#    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
#    --make_datacard=false --non_mass_plot=true 

#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=5  \
#    --category="$i" --plot_name="lep1_eta"  --x_axis_label="Electron #eta" \
#    --custom_x_axis_range=true --x_axis_min=-2.4 --x_axis_max=2.4 \
#    --make_datacard=false --non_mass_plot=true --extra_pad=1.6
#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=5 \
#    --category="$i" --plot_name="lep1_eta"  --x_axis_label="Muon #eta" \
#    --custom_x_axis_range=true --x_axis_min=-2.4 --x_axis_max=2.4 \
#    --make_datacard=false --non_mass_plot=true --extra_pad=1.6

#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=5  \
#    --category="$i" --plot_name="lep2_eta"  --x_axis_label="Tau #eta" \
#    --custom_x_axis_range=true --x_axis_min=-2.4 --x_axis_max=2.4 \
#    --make_datacard=false --non_mass_plot=true --extra_pad=1.6
#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=5 \
#    --category="$i" --plot_name="lep2_eta"  --x_axis_label="Tau #eta" \
#    --custom_x_axis_range=true --x_axis_min=-2.4 --x_axis_max=2.4 \
#    --make_datacard=false --non_mass_plot=true --extra_pad=1.6    

#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=10  \
#    --category="$i" --plot_name="mvamet_et"  --x_axis_label="E_{T}^{miss} [GeV]" \
#    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
#    --make_datacard=false  --non_mass_plot=true
#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=10 \
#    --category="$i" --plot_name="mvamet_et"  --x_axis_label="E_{T}^{miss} [GeV]" \
#    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
#    --make_datacard=false --non_mass_plot=true
# done

#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=20  \
#    --category=5 --plot_name="mvamet_et"  --x_axis_label="E_{T}^{miss} [GeV]" \
#    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
#    --make_datacard=false  --non_mass_plot=true --extra_pad=1.2
#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=20 \
#    --category=5 --plot_name="mvamet_et"  --x_axis_label="E_{T}^{miss} [GeV]" \
#    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
#    --make_datacard=false --non_mass_plot=true --extra_pad=1.2


#  CATS=(

#   '3'
#   '2'
#   '1'
#   '0'
#  )


#  for i in "${CATS[@]}"
#  do
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=4  \
#     --category="$i" --plot_name="lep1_mvamet_mt_all"  --x_axis_label="m_{T} [GeV]" \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.3
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=4 \
#     --category="$i" --plot_name="lep1_mvamet_mt_all"  --x_axis_label="m_{T}  [GeV]" \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.3

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=5  \
#     --category="$i" --plot_name="lep1_pt"  --x_axis_label="Electron p_{T} [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.3 --verbose=false
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=5 \
#     --category="$i" --plot_name="lep1_pt"  --x_axis_label="Muon p_{T} [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.3 --verbose=false

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=5  \
#     --category="$i" --plot_name="lep2_pt"  --x_axis_label="Tau p_{T} [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
#     --make_datacard=false --non_mass_plot=true
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=5 \
#     --category="$i" --plot_name="lep2_pt"  --x_axis_label="Tau p_{T} [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
#     --make_datacard=false --non_mass_plot=true 

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=4  \
#     --category="$i" --plot_name="lep1_eta"  --x_axis_label="Electron #eta" \
#     --custom_x_axis_range=true --x_axis_min=-2.4 --x_axis_max=2.4 \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.6
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=4 \
#     --category="$i" --plot_name="lep1_eta"  --x_axis_label="Muon #eta" \
#     --custom_x_axis_range=true --x_axis_min=-2.4 --x_axis_max=2.4 \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.6

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=4  \
#     --category="$i" --plot_name="lep2_eta"  --x_axis_label="Tau #eta" \
#     --custom_x_axis_range=true --x_axis_min=-2.4 --x_axis_max=2.4 \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.6
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=4 \
#     --category="$i" --plot_name="lep2_eta"  --x_axis_label="Tau #eta" \
#     --custom_x_axis_range=true --x_axis_min=-2.4 --x_axis_max=2.4 \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.6    

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=8  \
#     --category="$i" --plot_name="mvamet_et"  --x_axis_label="E_{T}^{miss} [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
#     --make_datacard=false  --non_mass_plot=true
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=8 \
#     --category="$i" --plot_name="mvamet_et"  --x_axis_label="E_{T}^{miss} [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
#     --make_datacard=false --non_mass_plot=true
#  done



#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=2  \
#     --category=9 --plot_name="lep1_mvamet_mt"  --x_axis_label="m_{T} [GeV]" \
#     --make_datacard=false --non_mass_plot=true

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=2 \
#     --category=9 --plot_name="lep1_mvamet_mt"  --x_axis_label="m_{T}  [GeV]" \
#     --make_datacard=false --non_mass_plot=true

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=4  \
#     --category=8 --plot_name="lep1_pt"  --x_axis_label="Electron p_{T} [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
#     --make_datacard=false --non_mass_plot=true
  # ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=4 \
  #   --category=8 --plot_name="lep1_pt"  --x_axis_label="Muon p_{T} [GeV]" \
  #   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
  #   --make_datacard=false --non_mass_plot=true 

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=4  \
#     --category=8 --plot_name="lep2_pt"  --x_axis_label="Tau p_{T} [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
#     --make_datacard=false --non_mass_plot=true
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=4 \
#     --category=8 --plot_name="lep2_pt"  --x_axis_label="Tau p_{T} [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
#     --make_datacard=false --non_mass_plot=true 

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=2  \
#     --category=8 --plot_name="lep1_eta"  --x_axis_label="Electron #eta" \
#     --custom_x_axis_range=true --x_axis_min=-2.4 --x_axis_max=2.4 \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.5
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=2 \
#     --category=8 --plot_name="lep1_eta"  --x_axis_label="Muon #eta" \
#     --custom_x_axis_range=true --x_axis_min=-2.4 --x_axis_max=2.4 \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.4

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=2  \
#     --category=8 --plot_name="lep2_eta"  --x_axis_label="Tau #eta" \
#     --custom_x_axis_range=true --x_axis_min=-2.4 --x_axis_max=2.4 \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.5
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=2 \
#     --category=8 --plot_name="lep2_eta"  --x_axis_label="Tau #eta" \
#     --custom_x_axis_range=true --x_axis_min=-2.4 --x_axis_max=2.4 \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.4

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=1  \
#     --category=8 --plot_name="n_jets"  --x_axis_label="Number of jets" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=5 --y_axis_min=10 \
#     --make_datacard=false --non_mass_plot=true --log_y=true --extra_pad=10.0
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=1 \
#     --category=8 --plot_name="n_jets"  --x_axis_label="Number of jets" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=5 --y_axis_min=10 \
#     --make_datacard=false --non_mass_plot=true --log_y=true --extra_pad=10.0

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=1  \
#     --category=8 --plot_name="n_bjets"  --x_axis_label="Number of b-tagged jets" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=4 \
#     --make_datacard=false --non_mass_plot=true --log_y=true
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=1 \
#     --category=8 --plot_name="n_bjets"  --x_axis_label="Number of b-tagged jets" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=4 \
#     --make_datacard=false --non_mass_plot=true --log_y=true

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=1  \
#     --category=8 --plot_name="lead_jet_pt"  --x_axis_label="Leading Jet p_{T} [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
#     --make_datacard=false --non_mass_plot=true
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=1 \
#     --category=8 --plot_name="lead_jet_pt"  --x_axis_label="Leading Jet p_{T} [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
#     --make_datacard=false --non_mass_plot=true 

#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=4  \
#     --category=8 --plot_name="lead_jet_eta"  --x_axis_label="Leading Jet #eta" \
#     --custom_x_axis_range=true --x_axis_min=-5 --x_axis_max=5 \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.5
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=4 \
#     --category=8 --plot_name="lead_jet_eta"  --x_axis_label="Leading Jet #eta" \
#     --custom_x_axis_range=true --x_axis_min=-5 --x_axis_max=5 \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.5

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=2  \
#     --category=8 --plot_name="sublead_jet_pt"  --x_axis_label="Trailing Jet p_{T} [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
#     --make_datacard=false --non_mass_plot=true
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=2 \
#     --category=8 --plot_name="sublead_jet_pt"  --x_axis_label="Trailing Jet p_{T} [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
#     --make_datacard=false --non_mass_plot=true

#  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=5  \
#     --category=8 --plot_name="sublead_jet_eta"  --x_axis_label="Trailing Jet #eta" \
#     --custom_x_axis_range=true --x_axis_min=-5 --x_axis_max=5 \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.5
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=5 \
#     --category=8 --plot_name="sublead_jet_eta"  --x_axis_label="Trailing Jet #eta" \
#     --custom_x_axis_range=true --x_axis_min=-5 --x_axis_max=5 \
#     --make_datacard=false --non_mass_plot=true --extra_pad=1.5

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=5  \
#     --category=8 --plot_name="mvamet_et"  --x_axis_label="E_{T}^{miss}  [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
#     --make_datacard=false  --non_mass_plot=true
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=5 \
#     --category=8 --plot_name="mvamet_et"  --x_axis_label="E_{T}^{miss}  [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=80 \
#     --make_datacard=false --non_mass_plot=true


#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=2  \
#     --category=10 --plot_name="vbf_mjj"  --x_axis_label="M_{jj} [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
#     --make_datacard=false --non_mass_plot=true
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=2 \
#     --category=10 --plot_name="vbf_mjj"  --x_axis_label="M_{jj} [GeV]" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
#     --make_datacard=false --non_mass_plot=true 

#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=4  \
#     --category=10 --plot_name="vbf_dEta"  --x_axis_label="#Delta#eta_{jj}" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=6 \
#     --make_datacard=false --non_mass_plot=true
#   ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=4 \
#     --category=10 --plot_name="vbf_dEta"  --x_axis_label="#Delta#eta_{jj}" \
#     --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=6 \
#     --make_datacard=false --non_mass_plot=true 


  # ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=2  \
  #   --category=10 --plot_name="vbf_mjj"  --x_axis_label="M_{jj} [GeV]" \
  #   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 --extra_pad=2.0 --signal_split_vbf=true \
  #   --make_datacard=false --non_mass_plot=true --log_y=true --signal_no_stack=true
  # ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=2 \
  #   --category=10 --plot_name="vbf_mjj"  --x_axis_label="M_{jj} [GeV]" \
  #   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 --extra_pad=6.0 --signal_split_vbf=true \
  #   --make_datacard=false --non_mass_plot=true  --log_y=true --signal_no_stack=true

  # ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=4  \
  #   --category=10 --plot_name="vbf_dEta"  --x_axis_label="#Delta#eta_{jj}" \
  #   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=6 --extra_pad=4.0 --signal_split_vbf=true \
  #   --make_datacard=false --non_mass_plot=true --log_y=true --signal_no_stack=true
  # ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=4 \
  #   --category=10 --plot_name="vbf_dEta"  --x_axis_label="#Delta#eta_{jj}" \
  #   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=6 --extra_pad=3.0 --signal_split_vbf=true \
  #   --make_datacard=false --non_mass_plot=true --log_y=true --signal_no_stack=true

    

  # ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=2  \
  #   --category=10 --plot_name="vbf_C1"  --x_axis_label="min(#eta_{vis}-#eta_{j1}, #eta_{vis}-#eta_{j2})" \
  #   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=5 \
  #   --make_datacard=false --non_mass_plot=true
  # ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=2 \
  #   --category=10 --plot_name="vbf_C1"  --x_axis_label="min(#eta_{vis}-#eta_{j1}, #eta_{vis}-#eta_{j2})" \
  #   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=5 \
  #   --make_datacard=false --non_mass_plot=true 

  # ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=10  \
  #   --category=10 --plot_name="vbf_C2"  --x_axis_label="Visible Di-tau p_{T} (GeV)" \
  #   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  #   --make_datacard=false --non_mass_plot=true
  # ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=10 \
  #   --category=10 --plot_name="vbf_C2"  --x_axis_label="Visible Di-tau p_{T} (GeV)" \
  #   --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=200 \
  #   --make_datacard=false --non_mass_plot=true 

  # ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=2  \
  #   --category=10 --plot_name="vbf_mva_val"  --x_axis_label="VBF MVA Output" \
  #   --custom_x_axis_range=true --x_axis_min=-1 --x_axis_max=1 \
  #   --make_datacard=false --non_mass_plot=true
  # ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=2 \
  #   --category=10 --plot_name="vbf_mva_val"  --x_axis_label="VBF MVA Output" \
  #   --custom_x_axis_range=true --x_axis_min=-1 --x_axis_max=1 \
  #   --make_datacard=false --non_mass_plot=true

  # ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=5  \
  #   --category=10 --plot_name="vbf_mva_val"  --x_axis_label="VBF MVA Output" \
  #   --custom_x_axis_range=true --x_axis_min=-1 --x_axis_max=1 \
  #   --make_datacard=false --non_mass_plot=true \
  #   --log_y=true --y_axis_min=0.9 --signal_no_stack=true --signal_split_vbf=true --extra_pad=10 \
  #   --blind=true --x_blind_min=0.8 --x_blind_max=1

  # ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=5 \
  #   --category=10 --plot_name="vbf_mva_val"  --x_axis_label="VBF MVA Output" \
  #   --custom_x_axis_range=true --x_axis_min=-1 --x_axis_max=1 \
  #   --make_datacard=false --non_mass_plot=true \
  #   --log_y=true --y_axis_min=0.9 --signal_no_stack=true --signal_split_vbf=true --extra_pad=10 \
  #   --blind=true --x_blind_min=0.8 --x_blind_max=1


 #  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=2  \
 #    --category=8 --plot_name="lead_bjet_pt"  --x_axis_label="Leading b-tagged Jet p_{T} [GeV]" \
 #    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
 #    --make_datacard=false --non_mass_plot=true
 #  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=2 \
 #    --category=8 --plot_name="lead_bjet_pt"  --x_axis_label="Leading b-tagged Jet p_{T} [GeV]" \
 #    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
 #    --make_datacard=false --non_mass_plot=true 

 # ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=0 --rebin=4  \
 #    --category=8 --plot_name="lead_bjet_eta"  --x_axis_label="Leading b-tagged Jet #eta" \
 #    --custom_x_axis_range=true --x_axis_min=-3 --x_axis_max=3 \
 #    --make_datacard=false --non_mass_plot=true --extra_pad=1.55
 #  ../bin/HiggsTauTauPlot2 --cfg=$CONFIG --mode=1 --rebin=4 \
 #    --category=8 --plot_name="lead_bjet_eta"  --x_axis_label="Leading b-tagged et #eta" \
 #    --custom_x_axis_range=true --x_axis_min=-3 --x_axis_max=3 \
 #    --make_datacard=false --non_mass_plot=true --extra_pad=1.55


