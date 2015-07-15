#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2014.cfg --channel=et --set_alias="sel:1"   --method=8 --cat="inclusive" --var="pt_1(25,0,100)"  --x_axis_label="Electron p_{T} [GeV]" --datacard="inclusive"   --background_scheme="et_no_qcd" --extra_pad=0.2 --verbosity=0
#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2014.cfg --channel=mt --smt_alias="sel:1"   --method=8 --cat="inclusive" --var="pt_1(25,0,100)"  --x_axis_label="Muon p_{T} [GeV]" --datacard="inclusive"   --background_scheme="mt_no_qcd" --extra_pad=0.2 --verbosity=0
#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2014.cfg --channel=et --set_alias="sel:1"   --method=8 --cat="inclusive" --var="pt_2(25,0,100)"  --x_axis_label="#tau_{h} p_{T} [GeV]" --datacard="inclusive"   --background_scheme="et_no_qcd" --extra_pad=0.2 --verbosity=0
#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2014.cfg --channel=mt --smt_alias="sel:1"   --method=8 --cat="inclusive" --var="pt_2(25,0,100)"  --x_axis_label="#tau_{h} p_{T} [GeV]" --datacard="inclusive"   --background_scheme="mt_no_qcd" --extra_pad=0.2 --verbosity=0
#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2014.cfg --channel=et --set_alias="sel:1"   --method=8 --cat="inclusive" --var="m_vis(25,0,200)"  --x_axis_label="Visible mass [GeV]" --datacard="inclusive"   --background_scheme="et_no_qcd" --extra_pad=0.2 --verbosity=0
./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --smt_alias="sel:1"   --method=8 --cat="inclusive" --var="m_vis(25,0,200)"  --x_axis_label="Visible mass [GeV]" --datacard="inclusive"   --background_scheme="mt_no_qcd" --extra_pad=0.2 --verbosity=0




#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2014.cfg --channel=tt --set_alias="sel:1"   --method=8 --cat="inclusive" --var="pt_1(25,0,100)"  --x_axis_label="#tau_1 p_{T} [GeV]" --datacard="inclusive"   --background_scheme="tt_default" --extra_pad=0.2 --verbosity=0
#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2014.cfg --channel=mt --smt_alias="sel:1"   --mmthod=8 --cat="1jmt" --var="pt_1(25,0,100)"   --x_axis_label="Electron p_{T} [GeV]" --datacard="1jmt"   --background_scheme="mt_no_qcd" --extra_pad=0.2 --verbosity=0
#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2014.cfg --channel=et --set_alias="sel:1"   --method=8 --cat="vbf" --var="pt_1(25,0,100)"   --x_axis_label="Electron p_{T} [GeV]" --datacard="vbf"   --background_scheme="et_no_qcd" --extra_pad=0.2 --verbosity=0
