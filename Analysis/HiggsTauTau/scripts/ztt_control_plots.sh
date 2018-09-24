# tt channel plots

# embedding plots

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins  --var="m_sv(20,0,200)" --embedding
mv ztt_control_plots/m_sv_ztt_control_tt_2016.pdf ztt_control_plots/m_sv_ztt_control_tt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="m_vis(20,0,200)" --embedding
mv ztt_control_plots/m_vis_ztt_control_tt_2016.pdf ztt_control_plots/m_vis_ztt_control_tt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_1[40,45,50,55,60,70,80,90,100,120,140,160,200]" --embedding
mv ztt_control_plots/pt_1_ztt_control_tt_2016.pdf ztt_control_plots/pt_1_ztt_control_tt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_2[40,45,50,55,60,70,80,100]" --embedding
mv ztt_control_plots/pt_2_ztt_control_tt_2016.pdf ztt_control_plots/pt_2_ztt_control_tt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_1(20,-2.3,2.3)" --extra_pad=0.5 --embedding
mv ztt_control_plots/eta_1_ztt_control_tt_2016.pdf ztt_control_plots/eta_1_ztt_control_tt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_2(20,-2.3,2.3)" --extra_pad=0.5 --embedding
mv ztt_control_plots/eta_2_ztt_control_tt_2016.pdf ztt_control_plots/eta_2_ztt_control_tt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="met[0,10,20,30,40,50,60,80,100]" --embedding
mv ztt_control_plots/met_ztt_control_tt_2016.pdf ztt_control_plots/met_ztt_control_tt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_tt[0,40,60,80,100,120,140,160,180,200,250,300,350]" --embedding
mv ztt_control_plots/pt_tt_ztt_control_tt_2016.pdf ztt_control_plots/pt_tt_ztt_control_tt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --extra_pad=0.4 --var="n_jets(4,0,4)" --embedding
mv ztt_control_plots/n_jets_ztt_control_tt_2016.pdf ztt_control_plots/n_jets_ztt_control_tt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins --extra_pad=0.5 --var="sjdphi(10,-3.5,3.5)" --embedding
mv ztt_control_plots/sjdphi_ztt_control_dijet_tt_2016.pdf ztt_control_plots/sjdphi_ztt_control_tt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="mjj[0,50,100,150,200,300,400,600,800]" --embedding
mv ztt_control_plots/mjj_ztt_control_dijet_tt_2016.pdf ztt_control_plots/mjj_ztt_control_tt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="jdeta[0,0.5,1,1.5,2,2.5,3,3.5,4,5,7]" --embedding
mv ztt_control_plots/jdeta_ztt_control_dijet_tt_2016.pdf ztt_control_plots/jdeta_ztt_control_tt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="D0(10,0,1)" --embedding --extra_pad=0.55
mv ztt_control_plots/D0_ztt_control_dijet_tt_2016.pdf ztt_control_plots/D0_ztt_control_tt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="DCP(10,-1,1)" --embedding --extra_pad=0.55
mv ztt_control_plots/DCP_ztt_control_dijet_tt_2016.pdf ztt_control_plots/DCP_ztt_control_tt_2016_embed.pdf


# MC plots 

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins  --var="m_sv(20,0,200)" 
mv ztt_control_plots/m_sv_ztt_control_tt_2016.pdf ztt_control_plots/m_sv_ztt_control_tt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="m_vis(20,0,200)" 
mv ztt_control_plots/m_vis_ztt_control_tt_2016.pdf ztt_control_plots/m_vis_ztt_control_tt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_1[40,45,50,55,60,70,80,90,100,120,140,160,200]" 
mv ztt_control_plots/pt_1_ztt_control_tt_2016.pdf ztt_control_plots/pt_1_ztt_control_tt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_2[40,45,50,55,60,70,80,100]" 
mv ztt_control_plots/pt_2_ztt_control_tt_2016.pdf ztt_control_plots/pt_2_ztt_control_tt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_1(20,-2.3,2.3)" --extra_pad=0.5 
mv ztt_control_plots/eta_1_ztt_control_tt_2016.pdf ztt_control_plots/eta_1_ztt_control_tt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_2(20,-2.3,2.3)" --extra_pad=0.5 
mv ztt_control_plots/eta_2_ztt_control_tt_2016.pdf ztt_control_plots/eta_2_ztt_control_tt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="met[0,10,20,30,40,50,60,80,100]" 
mv ztt_control_plots/met_ztt_control_tt_2016.pdf ztt_control_plots/met_ztt_control_tt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_tt[0,40,60,80,100,120,140,160,180,200,250,300,350]" 
mv ztt_control_plots/pt_tt_ztt_control_tt_2016.pdf ztt_control_plots/pt_tt_ztt_control_tt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --extra_pad=0.4 --var="n_jets(4,0,4)" 
mv ztt_control_plots/n_jets_ztt_control_tt_2016.pdf ztt_control_plots/n_jets_ztt_control_tt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins --extra_pad=0.5 --var="sjdphi(10,-3.5,3.5)" 
mv ztt_control_plots/sjdphi_ztt_control_dijet_tt_2016.pdf ztt_control_plots/sjdphi_ztt_control_tt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="mjj[0,50,100,150,200,300,400,600,800]" 
mv ztt_control_plots/mjj_ztt_control_dijet_tt_2016.pdf ztt_control_plots/mjj_ztt_control_tt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="jdeta[0,0.5,1,1.5,2,2.5,3,3.5,4,5,7]" 
mv ztt_control_plots/jdeta_ztt_control_dijet_tt_2016.pdf ztt_control_plots/jdeta_ztt_control_tt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="D0(10,0,1)" --extra_pad=0.55
mv ztt_control_plots/D0_ztt_control_dijet_tt_2016.pdf ztt_control_plots/D0_ztt_control_tt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=tt --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="DCP(10,-1,1)" --extra_pad=0.55
mv ztt_control_plots/DCP_ztt_control_dijet_tt_2016.pdf ztt_control_plots/DCP_ztt_control_tt_2016_mc.pdf

# mt channel plots

# embedding plots

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins  --var="m_sv(20,0,200)" --embedding
mv ztt_control_plots/m_sv_ztt_control_mt_2016.pdf ztt_control_plots/m_sv_ztt_control_mt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="m_vis(20,0,200)" --embedding
mv ztt_control_plots/m_vis_ztt_control_mt_2016.pdf ztt_control_plots/m_vis_ztt_control_mt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_1[20,25,30,35,40,45,50,55,60,70,80,90,100]" --embedding
mv ztt_control_plots/pt_1_ztt_control_mt_2016.pdf ztt_control_plots/pt_1_ztt_control_mt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_2[30,35,40,45,50,55,60,70,80,100]" --embedding
mv ztt_control_plots/pt_2_ztt_control_mt_2016.pdf ztt_control_plots/pt_2_ztt_control_mt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_1(20,-2.3,2.3)" --extra_pad=0.5 --embedding
mv ztt_control_plots/eta_1_ztt_control_mt_2016.pdf ztt_control_plots/eta_1_ztt_control_mt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_2(20,-2.3,2.3)" --extra_pad=0.5 --embedding
mv ztt_control_plots/eta_2_ztt_control_mt_2016.pdf ztt_control_plots/eta_2_ztt_control_mt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="met[0,10,20,30,40,50,60,80,100]" --embedding
mv ztt_control_plots/met_ztt_control_mt_2016.pdf ztt_control_plots/met_ztt_control_mt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_tt[0,40,60,80,100,120,140,160,180,200,250,300,350]" --embedding
mv ztt_control_plots/pt_tt_ztt_control_mt_2016.pdf ztt_control_plots/pt_tt_ztt_control_mt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --extra_pad=0.4 --var="n_jets(4,0,4)" --embedding
mv ztt_control_plots/n_jets_ztt_control_mt_2016.pdf ztt_control_plots/n_jets_ztt_control_mt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins --extra_pad=0.5 --var="sjdphi(10,-3.5,3.5)" --embedding
mv ztt_control_plots/sjdphi_ztt_control_dijet_mt_2016.pdf ztt_control_plots/sjdphi_ztt_control_mt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="mjj[0,50,100,150,200,300,400,600,800]" --embedding
mv ztt_control_plots/mjj_ztt_control_dijet_mt_2016.pdf ztt_control_plots/mjj_ztt_control_mt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="jdeta[0,0.5,1,1.5,2,2.5,3,3.5,4,5,7]" --embedding
mv ztt_control_plots/jdeta_ztt_control_dijet_mt_2016.pdf ztt_control_plots/jdeta_ztt_control_mt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="D0(10,0,1)" --embedding --extra_pad=0.55
mv ztt_control_plots/D0_ztt_control_dijet_mt_2016.pdf ztt_control_plots/D0_ztt_control_mt_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="DCP(10,-1,1)" --embedding --extra_pad=0.55
mv ztt_control_plots/DCP_ztt_control_dijet_mt_2016.pdf ztt_control_plots/DCP_ztt_control_mt_2016_embed.pdf

# MC plots 

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins  --var="m_sv(20,0,200)" 
mv ztt_control_plots/m_sv_ztt_control_mt_2016.pdf ztt_control_plots/m_sv_ztt_control_mt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="m_vis(20,0,200)" 
mv ztt_control_plots/m_vis_ztt_control_mt_2016.pdf ztt_control_plots/m_vis_ztt_control_mt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_1[20,25,30,35,40,45,50,55,60,70,80,90,100]" 
mv ztt_control_plots/pt_1_ztt_control_mt_2016.pdf ztt_control_plots/pt_1_ztt_control_mt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_2[30,35,40,45,50,55,60,70,80,100]" 
mv ztt_control_plots/pt_2_ztt_control_mt_2016.pdf ztt_control_plots/pt_2_ztt_control_mt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_1(20,-2.3,2.3)" --extra_pad=0.5 
mv ztt_control_plots/eta_1_ztt_control_mt_2016.pdf ztt_control_plots/eta_1_ztt_control_mt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_2(20,-2.3,2.3)" --extra_pad=0.5 
mv ztt_control_plots/eta_2_ztt_control_mt_2016.pdf ztt_control_plots/eta_2_ztt_control_mt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="met[0,10,20,30,40,50,60,80,100]" 
mv ztt_control_plots/met_ztt_control_mt_2016.pdf ztt_control_plots/met_ztt_control_mt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_tt[0,40,60,80,100,120,140,160,180,200,250,300,350]" 
mv ztt_control_plots/pt_tt_ztt_control_mt_2016.pdf ztt_control_plots/pt_tt_ztt_control_mt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --extra_pad=0.4 --var="n_jets(4,0,4)" 
mv ztt_control_plots/n_jets_ztt_control_mt_2016.pdf ztt_control_plots/n_jets_ztt_control_mt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins --extra_pad=0.5 --var="sjdphi(10,-3.5,3.5)" 
mv ztt_control_plots/sjdphi_ztt_control_dijet_mt_2016.pdf ztt_control_plots/sjdphi_ztt_control_mt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="mjj[0,50,100,150,200,300,400,600,800]" 
mv ztt_control_plots/mjj_ztt_control_dijet_mt_2016.pdf ztt_control_plots/mjj_ztt_control_mt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="jdeta[0,0.5,1,1.5,2,2.5,3,3.5,4,5,7]" 
mv ztt_control_plots/jdeta_ztt_control_dijet_mt_2016.pdf ztt_control_plots/jdeta_ztt_control_mt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="D0(10,0,1)" --extra_pad=0.55
mv ztt_control_plots/D0_ztt_control_dijet_mt_2016.pdf ztt_control_plots/D0_ztt_control_mt_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=mt --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="DCP(10,-1,1)" --extra_pad=0.55
mv ztt_control_plots/DCP_ztt_control_dijet_mt_2016.pdf ztt_control_plots/DCP_ztt_control_mt_2016_mc.pdf


# et channel plot 

# embedding plots

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins  --var="m_sv(20,0,200)" --embedding
mv ztt_control_plots/m_sv_ztt_control_et_2016.pdf ztt_control_plots/m_sv_ztt_control_et_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="m_vis(20,0,200)" --embedding
mv ztt_control_plots/m_vis_ztt_control_et_2016.pdf ztt_control_plots/m_vis_ztt_control_et_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_1[20,25,30,35,40,45,50,55,60,70,80,90,100]" --embedding
mv ztt_control_plots/pt_1_ztt_control_et_2016.pdf ztt_control_plots/pt_1_ztt_control_et_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_2[30,35,40,45,50,55,60,70,80,100]" --embedding
mv ztt_control_plots/pt_2_ztt_control_et_2016.pdf ztt_control_plots/pt_2_ztt_control_et_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_1(20,-2.3,2.3)" --extra_pad=0.5 --embedding
mv ztt_control_plots/eta_1_ztt_control_et_2016.pdf ztt_control_plots/eta_1_ztt_control_et_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_2(20,-2.3,2.3)" --extra_pad=0.5 --embedding
mv ztt_control_plots/eta_2_ztt_control_et_2016.pdf ztt_control_plots/eta_2_ztt_control_et_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="met[0,10,20,30,40,50,60,80,100]" --embedding
mv ztt_control_plots/met_ztt_control_et_2016.pdf ztt_control_plots/met_ztt_control_et_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_tt[0,40,60,80,100,120,140,160,180,200,250,300,350]" --embedding
mv ztt_control_plots/pt_tt_ztt_control_et_2016.pdf ztt_control_plots/pt_tt_ztt_control_et_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --extra_pad=0.4 --var="n_jets(4,0,4)" --embedding
mv ztt_control_plots/n_jets_ztt_control_et_2016.pdf ztt_control_plots/n_jets_ztt_control_et_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins --extra_pad=0.5 --var="sjdphi(10,-3.5,3.5)" --embedding
mv ztt_control_plots/sjdphi_ztt_control_dijet_et_2016.pdf ztt_control_plots/sjdphi_ztt_control_et_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="mjj[0,50,100,150,200,300,400,600,800]" --embedding
mv ztt_control_plots/mjj_ztt_control_dijet_et_2016.pdf ztt_control_plots/mjj_ztt_control_et_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="jdeta[0,0.5,1,1.5,2,2.5,3,3.5,4,5,7]" --embedding
mv ztt_control_plots/jdeta_ztt_control_dijet_et_2016.pdf ztt_control_plots/jdeta_ztt_control_et_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="D0(10,0,1)" --embedding --extra_pad=0.55
mv ztt_control_plots/D0_ztt_control_dijet_et_2016.pdf ztt_control_plots/D0_ztt_control_et_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="DCP(10,-1,1)" --embedding --extra_pad=0.55
mv ztt_control_plots/DCP_ztt_control_dijet_et_2016.pdf ztt_control_plots/DCP_ztt_control_et_2016_embed.pdf


# MC plots 

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins  --var="m_sv(20,0,200)" 
mv ztt_control_plots/m_sv_ztt_control_et_2016.pdf ztt_control_plots/m_sv_ztt_control_et_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="m_vis(20,0,200)" 
mv ztt_control_plots/m_vis_ztt_control_et_2016.pdf ztt_control_plots/m_vis_ztt_control_et_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_1[20,25,30,35,40,45,50,55,60,70,80,90,100]" 
mv ztt_control_plots/pt_1_ztt_control_et_2016.pdf ztt_control_plots/pt_1_ztt_control_et_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_2[30,35,40,45,50,55,60,70,80,100]" 
mv ztt_control_plots/pt_2_ztt_control_et_2016.pdf ztt_control_plots/pt_2_ztt_control_et_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_1(20,-2.3,2.3)" --extra_pad=0.5 
mv ztt_control_plots/eta_1_ztt_control_et_2016.pdf ztt_control_plots/eta_1_ztt_control_et_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_2(20,-2.3,2.3)" --extra_pad=0.5 
mv ztt_control_plots/eta_2_ztt_control_et_2016.pdf ztt_control_plots/eta_2_ztt_control_et_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="met[0,10,20,30,40,50,60,80,100]" 
mv ztt_control_plots/met_ztt_control_et_2016.pdf ztt_control_plots/met_ztt_control_et_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_tt[0,40,60,80,100,120,140,160,180,200,250,300,350]" 
mv ztt_control_plots/pt_tt_ztt_control_et_2016.pdf ztt_control_plots/pt_tt_ztt_control_et_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --extra_pad=0.4 --var="n_jets(4,0,4)" 
mv ztt_control_plots/n_jets_ztt_control_et_2016.pdf ztt_control_plots/n_jets_ztt_control_et_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins --extra_pad=0.5 --var="sjdphi(10,-3.5,3.5)" 
mv ztt_control_plots/sjdphi_ztt_control_dijet_et_2016.pdf ztt_control_plots/sjdphi_ztt_control_et_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="mjj[0,50,100,150,200,300,400,600,800]" 
mv ztt_control_plots/mjj_ztt_control_dijet_et_2016.pdf ztt_control_plots/mjj_ztt_control_et_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="jdeta[0,0.5,1,1.5,2,2.5,3,3.5,4,5,7]" 
mv ztt_control_plots/jdeta_ztt_control_dijet_et_2016.pdf ztt_control_plots/jdeta_ztt_control_et_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="D0(10,0,1)" --extra_pad=0.55
mv ztt_control_plots/D0_ztt_control_dijet_et_2016.pdf ztt_control_plots/D0_ztt_control_et_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=et --set_alias="sel:(mt_1<50)" --ratio  --method=17 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="DCP(10,-1,1)" --extra_pad=0.55
mv ztt_control_plots/DCP_ztt_control_dijet_et_2016.pdf ztt_control_plots/DCP_ztt_control_et_2016_mc.pdf


# em channel plots

# embedding plots

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins  --var="m_sv(20,0,200)" --embedding
mv ztt_control_plots/m_sv_ztt_control_em_2016.pdf ztt_control_plots/m_sv_ztt_control_em_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="m_vis(20,0,200)" --embedding
mv ztt_control_plots/m_vis_ztt_control_em_2016.pdf ztt_control_plots/m_vis_ztt_control_em_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_1[10,15,20,25,30,35,40,45,50,55,60,70,80,90,100]" --embedding
mv ztt_control_plots/pt_1_ztt_control_em_2016.pdf ztt_control_plots/pt_1_ztt_control_em_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_2[10,15,20,25,30,35,40,45,50,55,60,70,80,90,100]" --embedding
mv ztt_control_plots/pt_2_ztt_control_em_2016.pdf ztt_control_plots/pt_2_ztt_control_em_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_1(20,-2.3,2.3)" --extra_pad=0.5 --embedding
mv ztt_control_plots/eta_1_ztt_control_em_2016.pdf ztt_control_plots/eta_1_ztt_control_em_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_2(20,-2.3,2.3)" --extra_pad=0.5 --embedding
mv ztt_control_plots/eta_2_ztt_control_em_2016.pdf ztt_control_plots/eta_2_ztt_control_em_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="met[0,10,20,30,40,50,60,80,100]" --embedding
mv ztt_control_plots/met_ztt_control_em_2016.pdf ztt_control_plots/met_ztt_control_em_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_tt[0,40,60,80,100,120,140,160,180,200,250,300,350]" --embedding
mv ztt_control_plots/pt_tt_ztt_control_em_2016.pdf ztt_control_plots/pt_tt_ztt_control_em_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --extra_pad=0.4 --var="n_jets(4,0,4)" --embedding
mv ztt_control_plots/n_jets_ztt_control_em_2016.pdf ztt_control_plots/n_jets_ztt_control_em_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins --extra_pad=0.5 --var="sjdphi(10,-3.5,3.5)" --embedding
mv ztt_control_plots/sjdphi_ztt_control_dijet_em_2016.pdf ztt_control_plots/sjdphi_ztt_control_em_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="mjj[0,50,100,150,200,300,400,600,800]" --embedding
mv ztt_control_plots/mjj_ztt_control_dijet_em_2016.pdf ztt_control_plots/mjj_ztt_control_em_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="jdeta[0,0.5,1,1.5,2,2.5,3,3.5,4,5,7]" --embedding
mv ztt_control_plots/jdeta_ztt_control_dijet_em_2016.pdf ztt_control_plots/jdeta_ztt_control_em_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="D0(10,0,1)" --embedding --extra_pad=0.55
mv ztt_control_plots/D0_ztt_control_dijet_em_2016.pdf ztt_control_plots/D0_ztt_control_em_2016_embed.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="DCP(10,-1,1)" --embedding --extra_pad=0.55
mv ztt_control_plots/DCP_ztt_control_dijet_em_2016.pdf ztt_control_plots/DCP_ztt_control_em_2016_embed.pdf


# MC plots 

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins  --var="m_sv(20,0,200)" 
mv ztt_control_plots/m_sv_ztt_control_em_2016.pdf ztt_control_plots/m_sv_ztt_control_em_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="m_vis(20,0,200)" 
mv ztt_control_plots/m_vis_ztt_control_em_2016.pdf ztt_control_plots/m_vis_ztt_control_em_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_1[10,15,20,25,30,35,40,45,50,55,60,70,80,90,100]" 
mv ztt_control_plots/pt_1_ztt_control_em_2016.pdf ztt_control_plots/pt_1_ztt_control_em_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_2[10,15,20,25,30,35,40,45,50,55,60,70,80,90,100]" 
mv ztt_control_plots/pt_2_ztt_control_em_2016.pdf ztt_control_plots/pt_2_ztt_control_em_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_1(20,-2.3,2.3)" --extra_pad=0.5 
mv ztt_control_plots/eta_1_ztt_control_em_2016.pdf ztt_control_plots/eta_1_ztt_control_em_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="eta_2(20,-2.3,2.3)" --extra_pad=0.5 
mv ztt_control_plots/eta_2_ztt_control_em_2016.pdf ztt_control_plots/eta_2_ztt_control_em_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="met[0,10,20,30,40,50,60,80,100]" 
mv ztt_control_plots/met_ztt_control_em_2016.pdf ztt_control_plots/met_ztt_control_em_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --var="pt_tt[0,40,60,80,100,120,140,160,180,200,250,300,350]" 
mv ztt_control_plots/pt_tt_ztt_control_em_2016.pdf ztt_control_plots/pt_tt_ztt_control_em_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control --norm_bins --extra_pad=0.4 --var="n_jets(4,0,4)" 
mv ztt_control_plots/n_jets_ztt_control_em_2016.pdf ztt_control_plots/n_jets_ztt_control_em_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins --extra_pad=0.5 --var="sjdphi(10,-3.5,3.5)" 
mv ztt_control_plots/sjdphi_ztt_control_dijet_em_2016.pdf ztt_control_plots/sjdphi_ztt_control_em_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="mjj[0,50,100,150,200,300,400,600,800]" 
mv ztt_control_plots/mjj_ztt_control_dijet_em_2016.pdf ztt_control_plots/mjj_ztt_control_em_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="jdeta[0,0.5,1,1.5,2,2.5,3,3.5,4,5,7]" 
mv ztt_control_plots/jdeta_ztt_control_dijet_em_2016.pdf ztt_control_plots/jdeta_ztt_control_em_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="D0(10,0,1)" --extra_pad=0.55
mv ztt_control_plots/D0_ztt_control_dijet_em_2016.pdf ztt_control_plots/D0_ztt_control_em_2016_mc.pdf

python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg --channel=em --set_alias="sel:(pzeta>-10)" --ratio  --method=19 --outputfolder=ztt_control_plots/ --draw_signal_mass="" --cat=ztt_control_dijet --norm_bins  --var="DCP(10,-1,1)" --extra_pad=0.55
mv ztt_control_plots/DCP_ztt_control_dijet_em_2016.pdf ztt_control_plots/DCP_ztt_control_em_2016_mc.pdf


rm ztt_control_plots/*.png ztt_control_plots/*.root



