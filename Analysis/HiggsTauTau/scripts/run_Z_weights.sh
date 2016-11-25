#./bin/ZllPlot --cfg=scripts/new_plot_zll_2016.cfg --channel=zmm --set_alias="sel:pt_tt>0 && pt_tt<40"  --method=8 --cat="inclusive" --var="m_vis[0,10,20,30,40,50,60,70,80,90,100,120,160,200,400,800]"  --x_axis_label="m_{#mu#mu} [GeV]" --datacard="inclusive" --background_scheme="zmm_default" --extra_pad=0.2 --verbosity=1 --folder=output/Mar05_Zll/ --log_y=true --draw_error_band=true --auto_error_band=0.0001
#mv datacard_m_vis_inclusive_zmm_2016.root datacard_m_vis_inclusive_zmm_2016_pt0to40.root
./bin/ZllPlot --cfg=scripts/new_plot_zll_2016.cfg --channel=zmm --set_alias="sel:m_vis>50 && m_vis<80"  --method=8 --cat="inclusive" --var="pt_tt[0,40,80,120,160,200,240,280,320,400,500,600]"  --x_axis_label="Z pt [GeV]" --datacard="inclusive" --background_scheme="zmm_default" --extra_pad=0.2 --verbosity=1 --folder=/vols/cms/rcl11/CMSSW_7_6_3/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/Aug11-zmm/ --log_y=true --draw_error_band=true --auto_error_band=0.0001
mv datacard_pt_tt_inclusive_zmm_2016.root datacard_pt_tt_inclusive_zmm_2016_mvis50to80.root
mv pt_tt_inclusive_zmm_2016_log.png pt_tt_inclusive_zmm_2016_log_mvis50to80.png
mv pt_tt_inclusive_zmm_2016_log.pdf pt_tt_inclusive_zmm_2016_log_mvis50to80.pdf
./bin/ZllPlot --cfg=scripts/new_plot_zll_2016.cfg --channel=zmm --set_alias="sel:m_vis>80 && m_vis<120"  --method=8 --cat="inclusive" --var="pt_tt[0,40,80,120,160,200,280,400,500,600]"  --x_axis_label="Z pt [GeV]" --datacard="inclusive" --background_scheme="zmm_default" --extra_pad=0.2 --verbosity=1 --folder=/vols/cms/rcl11/CMSSW_7_6_3/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/Aug11-zmm/ --log_y=true --draw_error_band=true --auto_error_band=0.0001
mv datacard_pt_tt_inclusive_zmm_2016.root datacard_pt_tt_inclusive_zmm_2016_mvis80to120.root
mv pt_tt_inclusive_zmm_2016_log.png pt_tt_inclusive_zmm_2016_log_mvis80to120.png
mv pt_tt_inclusive_zmm_2016_log.pdf pt_tt_inclusive_zmm_2016_log_mvis80to120.pdf
./bin/ZllPlot --cfg=scripts/new_plot_zll_2016.cfg --channel=zmm --set_alias="sel:m_vis>120 && m_vis<160"  --method=8 --cat="inclusive" --var="pt_tt[0,40,80,120,160,200,400,600]"  --x_axis_label="Z pt [GeV]" --datacard="inclusive" --background_scheme="zmm_default" --extra_pad=0.2 --verbosity=1 --folder=/vols/cms/rcl11/CMSSW_7_6_3/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/Aug11-zmm/ --log_y=true --draw_error_band=true --auto_error_band=0.0001
mv datacard_pt_tt_inclusive_zmm_2016.root datacard_pt_tt_inclusive_zmm_2016_mvis120to160.root
mv pt_tt_inclusive_zmm_2016_log.png pt_tt_inclusive_zmm_2016_log_mvis120to160.png
mv pt_tt_inclusive_zmm_2016_log.pdf pt_tt_inclusive_zmm_2016_log_mvis120to160.pdf
./bin/ZllPlot --cfg=scripts/new_plot_zll_2016.cfg --channel=zmm --set_alias="sel:m_vis>160 && m_vis<200"  --method=8 --cat="inclusive" --var="pt_tt[0,120,200,400,600]"  --x_axis_label="Z pt [GeV]" --datacard="inclusive" --background_scheme="zmm_default" --extra_pad=0.2 --verbosity=1 --folder=/vols/cms/rcl11/CMSSW_7_6_3/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/Aug11-zmm/ --log_y=true --draw_error_band=true --auto_error_band=0.0001
mv datacard_pt_tt_inclusive_zmm_2016.root datacard_pt_tt_inclusive_zmm_2016_mvis160to200.root
mv pt_tt_inclusive_zmm_2016_log.png pt_tt_inclusive_zmm_2016_log_mvis160to200.png
mv pt_tt_inclusive_zmm_2016_log.pdf pt_tt_inclusive_zmm_2016_log_mvis160to200.pdf
./bin/ZllPlot --cfg=scripts/new_plot_zll_2016.cfg --channel=zmm --set_alias="sel:m_vis>200 && m_vis<400"  --method=8 --cat="inclusive" --var="pt_tt[0,120,200,400,600]"  --x_axis_label="Z pt [GeV]" --datacard="inclusive" --background_scheme="zmm_default" --extra_pad=0.2 --verbosity=1 --folder=/vols/cms/rcl11/CMSSW_7_6_3/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/Aug11-zmm/ --log_y=true --draw_error_band=true --auto_error_band=0.0001
mv datacard_pt_tt_inclusive_zmm_2016.root datacard_pt_tt_inclusive_zmm_2016_mvis200to400.root
mv pt_tt_inclusive_zmm_2016_log.png pt_tt_inclusive_zmm_2016_log_mvis200to400.png
mv pt_tt_inclusive_zmm_2016_log.pdf pt_tt_inclusive_zmm_2016_log_mvis200to400.pdf
./bin/ZllPlot --cfg=scripts/new_plot_zll_2016.cfg --channel=zmm --set_alias="sel:m_vis>400 && m_vis<800"  --method=8 --cat="inclusive" --var="pt_tt[0,200,600]"  --x_axis_label="Z pt [GeV]" --datacard="inclusive" --background_scheme="zmm_default" --extra_pad=0.2 --verbosity=1 --folder=/vols/cms/rcl11/CMSSW_7_6_3/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/Aug11-zmm/ --log_y=true --draw_error_band=true --auto_error_band=0.0001
mv datacard_pt_tt_inclusive_zmm_2016.root datacard_pt_tt_inclusive_zmm_2016_mvis400to800.root
mv pt_tt_inclusive_zmm_2016_log.png pt_tt_inclusive_zmm_2016_log_mvis400to800.png
mv pt_tt_inclusive_zmm_2016_log.pdf pt_tt_inclusive_zmm_2016_log_mvis400to800.pdf
