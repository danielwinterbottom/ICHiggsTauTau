
ET_INC_SHIFT="  --draw_error_band=true --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=0.32 --shift_backgrounds=ZTT:0.996753,QCD:1.018,W:0.962,ZL:0.911294,ZJ:0.959214,VV:0.967928,TT:0.88158 --auto_error_band=0.0404751"
MT_INC_SHIFT="  --draw_error_band=true --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=-0.13 --shift_backgrounds=ZTT:0.966278,QCD:1.0258,W:1.03862,ZL:1.15101,ZJ:1.19543,VV:0.97629,TT:1.07285 --auto_error_band=0.0398268"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_mssm_2012.cfg --channel=et --set_alias="sel:mt_1<30."   --method=8 --cat="inclusive" --var="n_prebjets_SF(5,-0.5,4.5)"  --x_axis_label="Number of b-tagged Jets"   --draw_ratio=true --log_y=true --extra_pad=500 --datacard="inclusive"   --background_scheme="et_default"  --custom_y_axis_min=true --y_axis_min=0.99 $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_mssm_2012.cfg --channel=mt --set_alias="sel:mt_1<30."   --method=8 --cat="inclusive" --var="n_prebjets_SF(5,-0.5,4.5)"  --x_axis_label="Number of b-tagged Jets"   --draw_ratio=true --log_y=true --extra_pad=500 --datacard="inclusive"   --background_scheme="mt_with_zmm"  --custom_y_axis_min=true --y_axis_min=0.99 $MT_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_mssm_2012.cfg --channel=et --set_alias="sel:mt_1<30."   --method=8 --cat="inclusive" --var="n_prebjets(9,-0.5,8.5)"  --x_axis_label="Number of Jets"   --draw_ratio=true --log_y=true --extra_pad=500 --datacard="inclusive"   --background_scheme="et_default"  --custom_y_axis_min=true --y_axis_min=0.99 $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_mssm_2012.cfg --channel=mt --set_alias="sel:mt_1<30."   --method=8 --cat="inclusive" --var="n_prebjets(9,-0.5,8.5)"  --x_axis_label="Number of Jets"   --draw_ratio=true --log_y=true --extra_pad=500 --datacard="inclusive"   --background_scheme="mt_with_zmm"  --custom_y_axis_min=true --y_axis_min=0.99 $MT_INC_SHIFT

./bin/PlotCompare -p "M-ttbb:Without kinematic fit:datacard_mjj_tt_2jet2tagSFMassCuts_mt_2012.root:muTau_2jet2tagSFMassCuts:ggHTohhTo2Tau2B300:-1:2:2" -p "M-Hkinfit:With kinematic fit:datacard_m_H_hh_2jet2tagSFMassCuts_mt_2012.root:muTau_2jet2tagSFMassCuts:ggHTohhTo2Tau2B300:-1:2:4" --norm_mode=0 --big_label="#mu#tau_{h}" --outname="m_H_kinfit_vs_mttbb_2jet2tagSFMassCuts_mt_ggHTohh300" --x_axis_title="m_{H} [GeV]" --norm_bins=true  --custom_y_axis_min=true --y_axis_min=0.0 --custom_x_axis_range=true --x_axis_min=199.9  --x_axis_max=499.9

./bin/PlotCompare -p "M-H350:350GeV:datacard_m_H_hh_2jet2tagSFMassCuts_mt_2012.root:muTau_2jet2tagSFMassCuts:ggHTohhTo2Tau2B350:-1:2:604" -p "M-H300:300GeV:datacard_m_H_hh_2jet2tagSFMassCuts_mt_2012.root:muTau_2jet2tagSFMassCuts:ggHTohhTo2Tau2B300:-1:2:4" -p "M-H260:260GeV:datacard_m_H_hh_2jet2tagSFMassCuts_mt_2012.root:muTau_2jet2tagSFMassCuts:ggHTohhTo2Tau2B260:-1:2:868"  --norm_mode=0 --big_label="#mu#tau_{h}" --outname="m_H_kinfit_signalmasses_2jet2tagSFMassCuts_mt" --x_axis_title="m_{H}^{kinfit} [GeV]" --norm_bins=true  --custom_y_axis_min=true --y_axis_min=0.0 --custom_x_axis_range=true --x_axis_max=499.9


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_mssm_2012.cfg --channel=mt   --method=20 --cat="2jet1tagSFMassCuts" --set_alias="sel:1" --var="mt_1(25,0,200)"   --x_axis_label="m_{T} [GeV]" --datacard="2jet1tagSFMassCuts" --extra_pad=1.5 --background_scheme="mt_with_zmm" --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_mssm_2012.cfg --channel=mt   --method=16 --cat="2jet2tagSFMassCuts" --set_alias="sel:1" --var="mt_1(22,0,180)"   --x_axis_label="m_{T} [GeV]" --datacard="2jet2tagSFMassCuts" --extra_pad=1.5 --background_scheme="mt_with_zmm" --draw_error_band=true --auto_error_band=0.00001


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_mssm_2012.cfg --channel=mt --set_alias="sel:mt_1<30."   --method=8 --cat="2jet0tagMassCuts" --var="n_prebjets_SF(7,-0.5,6.5)"  --x_axis_label="Number of b-jets after b-tag SF"  --log_y=true --datacard="2jet0tagMassCuts"   --background_scheme="mt_with_zmm" --custom_y_axis_min=true --y_axis_min=0.0099 --extra_pad=100 --blind=true --x_blind_min=-0.5 --x_blind_max=5.5

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_mssm_2012.cfg --channel=mt --set_alias="sel:mt_1<30."   --method=8 --cat="2jet1tagMassCuts" --var="n_prebjets_SF(7,-0.5,6.5)"  --x_axis_label="Number of b-jets after b-tag SF"  --log_y=true --datacard="2jet1tagMassCuts"   --background_scheme="mt_with_zmm" --custom_y_axis_min=true --y_axis_min=0.0099 --extra_pad=100 --blind=true --x_blind_min=-0.5 --x_blind_max=5.5

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_mssm_2012.cfg --channel=mt --set_alias="sel:mt_1<30."   --method=8 --cat="2jet2tagMassCuts" --var="n_prebjets_SF(7,-0.5,6.5)"  --x_axis_label="Number of b-jets after b-tag SF"  --log_y=true --datacard="2jet2tagMassCuts"   --background_scheme="mt_with_zmm" --custom_y_axis_min=true --y_axis_min=0.0099 --extra_pad=100 --blind=true --x_blind_min=-0.5 --x_blind_max=5.5 --fix_negative_bins="QCD,W"
