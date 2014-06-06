Z2BSEL="met<50 && m_z>76 && m_z<106"
Z1BSEL="m_z>76 && m_z<106"

./bin/ZbbPlot --cfg=scripts/plot.cfg --sel="$Z1BSEL" --cat="n_b_jets==1" --wt="wt*wt_1b_exc" \
--var="m_z(30,76,106)" --datacard="z1b" --background_scheme="all" --syst_scale_j="jes" \
--extra_pad=100  --x_axis_label="m_{ll} [GeV]" --log_y=true --custom_y_axis_min=true --y_axis_min=20

./bin/ZbbPlot --cfg=scripts/plot.cfg --sel="$Z1BSEL" --cat="n_b_jets==1" --wt="wt*wt_1b_exc" \
--var="bpt_1(20,25,265)" --datacard="z1b" --background_scheme="all" --syst_scale_j="jes" \
--extra_pad=1.3  --x_axis_label="Leading b-tagged Jet p_{T} [GeV]"

./bin/ZbbPlot --cfg=scripts/plot.cfg --sel="$Z2BSEL" --cat="n_b_jets>=2" --wt="wt*wt_2b_inc" \
--var="m_z(15,76,106)" --datacard="z2b" --background_scheme="all" --syst_scale_j="jes" \
--extra_pad=1.7  --x_axis_label="m_{ll} [GeV]"

./bin/ZbbPlot --cfg=scripts/plot.cfg --sel="$Z2BSEL" --cat="n_b_jets>=2" --wt="wt*wt_2b_inc" \
--var="pt_z(17,0,255)" --datacard="z2b" --background_scheme="all" --syst_scale_j="jes" \
--extra_pad=1.1  --x_axis_label="p_{T}^{ll} [GeV]"

./bin/ZbbPlot --cfg=scripts/plot.cfg --sel="$Z1BSEL" --cat="n_b_jets==1" --wt="wt*wt_1b_exc" \
--var="pt_z(25,0,250)" --datacard="z1b" --background_scheme="all" --syst_scale_j="jes" \
--extra_pad=1.1  --x_axis_label="p_{T}^{ll} [GeV]"

./bin/ZbbPlot --cfg=scripts/plot.cfg --sel="$Z2BSEL" --cat="n_b_jets>=2" --wt="wt*wt_2b_inc" \
--var="pt_bb(17,0,255)" --datacard="z2b" --background_scheme="all" --syst_scale_j="jes" \
--extra_pad=1.1  --x_axis_label="p_{T}^{bb} [GeV]"

./bin/ZbbPlot --cfg=scripts/plot.cfg --sel="$Z2BSEL" --cat="n_b_jets>=2" --wt="wt*wt_2b_inc" \
--var="dphi_z_bb(25,0,3.15)" --datacard="z2b" --background_scheme="all" --syst_scale_j="jes" \
--extra_pad=1.3  --x_axis_label="#Delta#phi_{Z,bb}" --legend_pos=1 --legend_left=0.22

./bin/ZbbPlot --cfg=scripts/plot.cfg --sel="$Z1BSEL" --cat="n_b_jets==1" --wt="wt*wt_1b_exc" \
--var="dphi_z_b(25,0,3.15)" --datacard="z1b" --background_scheme="all" --syst_scale_j="jes" \
--extra_pad=1.3  --x_axis_label="#Delta#phi_{Z,b}" --legend_pos=1 --legend_left=0.22

./bin/ZbbPlot --cfg=scripts/plot.cfg --sel="$Z2BSEL" --cat="n_b_jets>=2" --wt="wt*wt_2b_inc" \
--var="bpt_1(20,25,265)" --datacard="z2b" --background_scheme="all" --syst_scale_j="jes" \
--extra_pad=1.3  --x_axis_label="Leading b-tagged Jet p_{T} [GeV]"

./bin/ZbbPlot --cfg=scripts/plot.cfg --sel="$Z2BSEL" --cat="n_b_jets>=2" --wt="wt*wt_2b_inc" \
--var="bpt_2(20,25,265)" --datacard="z2b" --background_scheme="all" --syst_scale_j="jes" \
--extra_pad=1.4  --x_axis_label="Sub-leading b-tagged Jet p_{T} [GeV]"

./bin/ZbbPlot --cfg=scripts/plot.cfg --sel="$Z1BSEL" --cat="n_b_jets>=2" --wt="wt*wt_2b_inc" \
--var="met(20,0,200)" --datacard="z2b_nomet" --background_scheme="all" --syst_scale_j="jes" \
--extra_pad=1.1  --x_axis_label="E_{T}^{miss} [GeV]"

./bin/ZbbPlot --cfg=scripts/plot.cfg --sel="$Z1BSEL" --cat="n_jets>=1" --wt="wt" \
--var="jnsv_1(5,-0.5,4.5)" --datacard="z1j" --background_scheme="all" --syst_scale_j="jes" \
--extra_pad=10  --x_axis_label="Leading Jet N_{SV}" --log_y=true --custom_y_axis_min=true --y_axis_min=0.99

./bin/ZbbPlot --cfg=scripts/plot.cfg --sel="$Z1BSEL" --cat="n_jets>=1 && jnsv_1>0" --wt="wt" \
--var="jssv_1(60,0,6)" --datacard="z1j_sv" --background_scheme="all" --syst_scale_j="jes" \
--extra_pad=1.3  --x_axis_label="Leading Jet SSV Discriminant"

./bin/ZbbPlot --cfg=scripts/plot.cfg --sel="$Z1BSEL" --cat="n_jets>=0" --wt="wt" \
--var="n_jets(5,-0.5,4.5)" --datacard="z_inc" --background_scheme="all" --syst_scale_j="jes" \
--extra_pad=10  --x_axis_label="Number of Jets" --log_y=true

./bin/ZbbPlot --cfg=scripts/plot.cfg --sel="$Z1BSEL" --cat="n_jets>=0" --wt="wt*(((n_b_jets==0)*1.0)+((n_b_jets==1)*wt_1b_exc)+((n_b_jets>=2)*wt_2b_inc))" \
--var="n_b_jets(5,-0.5,4.5)" --datacard="z_inc" --background_scheme="all" --syst_scale_j="jes" \
--extra_pad=10  --x_axis_label="Number of b-tagged Jets" --log_y=true --custom_y_axis_min=true --y_axis_min=0.99
