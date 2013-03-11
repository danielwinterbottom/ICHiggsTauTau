SCALE_MT_INC="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.029575 --ztt_shift=0.960251 \
              --qcd_shift=1.133200 --top_shift=0.714139 --zl_shift=1.059042 --zj_shift=1.462669 --w_shift=0.918000 --vv_shift=0.680721 \
              --shift_tscale=false --tscale_shift=-0.3"


 EXTRA=_NoShift

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="m_2"  --x_axis_label="Tau Mass [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0.0 --x_axis_max=1.6 \
  --add_text="#splitline{All Decay Modes}{No Correction},0.045,0.21,0.86" \
  --folder=output/TauPOG_AllModes$EXTRA/ --draw_signal=false --draw_ratio=false --use_ztt_mc=false \
  --paramfile=./scripts/Moriond_params_2012.dat --extra_pad=1.2 $SCALE_MT_INC

mv m_2_inclusive_mt_sm_2012.pdf tau_mass_allmodes_es$EXTRA.pdf
mv m_2_inclusive_mt_sm_2012.png tau_mass_allmodes_es$EXTRA.png


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="m_2"  --x_axis_label="Tau Mass [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0.2 --x_axis_max=1.4 \
  --add_text="#splitline{1 Prong 1 #pi^{0}}{No Correction},0.045,0.21,0.86" \
  --folder=output/TauPOG_1P1PZ$EXTRA/ --draw_signal=false --draw_ratio=false --use_ztt_mc=false \
  --paramfile=./scripts/Moriond_params_2012.dat --extra_pad=1.2 $SCALE_MT_INC

mv m_2_inclusive_mt_sm_2012.pdf tau_mass_1p1pz_es$EXTRA.pdf
mv m_2_inclusive_mt_sm_2012.png tau_mass_1p1pz_es$EXTRA.png


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="m_2"  --x_axis_label="Tau Mass [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0.6 --x_axis_max=1.8 \
  --add_text="#splitline{3 Prong}{No Correction},0.045,0.21,0.86" \
  --folder=output/TauPOG_3P$EXTRA/ --draw_signal=false --draw_ratio=false --use_ztt_mc=false \
  --paramfile=./scripts/Moriond_params_2012.dat --extra_pad=1.2 $SCALE_MT_INC

mv m_2_inclusive_mt_sm_2012.pdf tau_mass_3p_es$EXTRA.pdf
mv m_2_inclusive_mt_sm_2012.png tau_mass_3p_es$EXTRA.png


 EXTRA=""

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="m_2"  --x_axis_label="Tau Mass [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0.0 --x_axis_max=1.6 --ztt_by_decay_mode=true \
  --folder=output/TauPOG_AllModes$EXTRA/ --draw_signal=false --draw_ratio=false --use_ztt_mc=false \
  --paramfile=./scripts/Moriond_params_2012.dat --extra_pad=1.2 $SCALE_MT_INC
 # --add_text="#splitline{All Decay Modes}{ES Corrected},0.045,0.21,0.86" \

mv m_2_inclusive_mt_sm_2012.pdf tau_mass_allmodes_es$EXTRA.pdf
mv m_2_inclusive_mt_sm_2012.png tau_mass_allmodes_es$EXTRA.png


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="m_2"  --x_axis_label="Tau Mass [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0.2 --x_axis_max=1.4 \
  --add_text="#splitline{1 Prong 1 #pi^{0}}{ES Corrected},0.045,0.21,0.86" \
  --folder=output/TauPOG_1P1PZ$EXTRA/ --draw_signal=false --draw_ratio=false --use_ztt_mc=false \
  --paramfile=./scripts/Moriond_params_2012.dat --extra_pad=1.2 $SCALE_MT_INC

mv m_2_inclusive_mt_sm_2012.pdf tau_mass_1p1pz_es$EXTRA.pdf
mv m_2_inclusive_mt_sm_2012.png tau_mass_1p1pz_es$EXTRA.png


./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="m_2"  --x_axis_label="Tau Mass [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0.6 --x_axis_max=1.8 \
  --add_text="#splitline{3 Prong}{ES Corrected},0.045,0.21,0.86" \
  --folder=output/TauPOG_3P$EXTRA/ --draw_signal=false --draw_ratio=false --use_ztt_mc=false \
  --paramfile=./scripts/Moriond_params_2012.dat --extra_pad=1.2 $SCALE_MT_INC

mv m_2_inclusive_mt_sm_2012.pdf tau_mass_3p_es$EXTRA.pdf
mv m_2_inclusive_mt_sm_2012.png tau_mass_3p_es$EXTRA.png
