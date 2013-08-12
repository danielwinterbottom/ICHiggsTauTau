PROD=Paper
YEAR=2012
ANA=mssm
PARAMS=./scripts/"$PROD"_params_$YEAR.dat
SMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500"

#############################################################
#### Add scale factors to backgrounds
#############################################################

ET_INC_SHIFT=""
ET_1JET_SHIFT=""
ET_2JET_SHIFT=""
ET_BTAG_SHIFT=""
MT_INC_SHIFT="" 
MT_1JET_SHIFT=""
MT_2JET_SHIFT=""
MT_BTAG_SHIFT=""
EM_INC_SHIFT=""
EM_1JET_SHIFT=""
EM_2JET_SHIFT=""
EM_BTAG_SHIFT=""

#### 8TeV
#ET_INC_SHIFT="  --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=0.34 --draw_error_band=true --shift_backgrounds=ZTT:0.991367,QCD:1.111,W:0.82,ZL:0.965,ZJ:1.43262,VV:0.872239,TT:0.98973 --auto_error_band=0.0257496"
#ET_1JET_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=0.34 --draw_error_band=true --shift_backgrounds=ZTT:0.936531,QCD:1.251,W:0.864412,ZL:0.816819,ZJ:1.54823,VV:0.776571,TT:0.859691 --auto_error_band=0.0370581"
#ET_2JET_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=0.34 --draw_error_band=true --shift_backgrounds=ZTT:0.948449,QCD:1.171,W:1.0568,ZL:0.985172,ZJ:1.1115,VV:0.180068,TT:0.794873 --auto_error_band=0.0573252"
#ET_BTAG_SHIFT=" --draw_error_band=true"
#MT_INC_SHIFT="  --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=0.24 --draw_error_band=true --shift_backgrounds=ZTT:1.00351,QCD:1.06,W:1.0063,ZL:1.0065,ZJ:1.5384,VV:0.844473,TT:0.941281 --auto_error_band=0.0230657"
#MT_1JET_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=0.24 --draw_error_band=true --shift_backgrounds=ZTT:1.00766,QCD:1.14702,W:1.04957,ZL:1.00342,ZJ:1.12958,VV:0.825885,TT:0.897123 --auto_error_band=0.0336586"
#MT_2JET_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=0.24 --draw_error_band=true --shift_backgrounds=ZTT:0.96372,QCD:1.228,W:1.07096,ZL:1.00021,ZJ:1.14786,VV:0.836621,TT:0.846627 --auto_error_band=0.0493541"
#MT_BTAG_SHIFT=" --draw_error_band=true"
#EM_INC_SHIFT="  --syst_tau_scale=CMS_scale_e_8TeV --shift_tscale=0.74 --draw_error_band=true --shift_backgrounds=Ztt:1.02916,Fakes:1.25921,EWK:0.8694,ttbar:0.986058 --auto_error_band=0.0268832"
#EM_1JET_SHIFT=" --syst_tau_scale=CMS_scale_e_8TeV --shift_tscale=0.74 --draw_error_band=true --shift_backgrounds=Ztt:1.01895,Fakes:1.21954,EWK:0.861756,ttbar:0.913756 --auto_error_band=0.0295444"
#EM_2JET_SHIFT=" --syst_tau_scale=CMS_scale_e_8TeV --shift_tscale=0.74 --draw_error_band=true --shift_backgrounds=Ztt:1.02091,Fakes:1.29636,EWK:0.797616,ttbar:0.933886 --auto_error_band=0.0449534"
#EM_BTAG_SHIFT=" --draw_error_band=true"

#### 7TeV
# ET_INC_SHIFT="  --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.14 --draw_error_band=true --shift_backgrounds=ZTT:0.967112,QCD:1.08,W:0.93,ZL:0.889359,ZJ:1.02048,VV:0.887048,TT:0.954773 --auto_error_band=0.0279033"
# ET_1JET_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.14 --draw_error_band=true --shift_backgrounds=ZTT:0.957653,QCD:1.153,W:0.92,ZL:0.552128,ZJ:1.30115,VV:0.835264,TT:0.929195 --auto_error_band=0.0482993"
# ET_2JET_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.14 --draw_error_band=true --shift_backgrounds=ZTT:0.985241,QCD:1.237,W:0.9655,ZL:1.0,ZJ:1.0,VV:0.106822,TT:0.886736 --auto_error_band=0.0642359"
# ET_BTAG_SHIFT=" --draw_error_band=true"
# MT_INC_SHIFT="  --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.05 --draw_error_band=true --shift_backgrounds=ZTT:0.990761,QCD:1.16,W:0.998,ZL:0.753903,ZJ:1.03425,VV:0.899156,TT:0.980518 --auto_error_band=0.0259864"
# MT_1JET_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.05 --draw_error_band=true --shift_backgrounds=ZTT:0.998705,QCD:1.23606,W:0.989,ZL:0.752468,ZJ:0.954764,VV:0.866584,TT:0.970583 --auto_error_band=0.0482516"
# MT_2JET_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.05 --draw_error_band=true --shift_backgrounds=ZTT:0.981851,QCD:1.4708,W:1.1071,ZL:1.0,ZJ:1.0,VV:0.797717,TT:0.945004 --auto_error_band=0.0527429"
# MT_BTAG_SHIFT=" --draw_error_band=true"
# EM_INC_SHIFT="  --syst_tau_scale=CMS_scale_e_7TeV --shift_tscale=0.57 --draw_error_band=true --shift_backgrounds=Ztt:0.951007,Fakes:1.78618,EWK:0.854453,ttbar:0.945838 --auto_error_band=0.0281248"
# EM_1JET_SHIFT=" --syst_tau_scale=CMS_scale_e_7TeV --shift_tscale=0.57 --draw_error_band=true --shift_backgrounds=Ztt:0.944727,Fakes:1.88442,EWK:0.818771,ttbar:0.897095 --auto_error_band=0.0344615"
# EM_2JET_SHIFT=" --syst_tau_scale=CMS_scale_e_7TeV --shift_tscale=0.57 --draw_error_band=true --shift_backgrounds=Ztt:0.924359,Fakes:1.88585,EWK:0.733632,ttbar:0.811283 --auto_error_band=0.0541756"
# EM_BTAG_SHIFT=" --draw_error_band=true"

###############################################################################
#### Inclusive selection plots
###############################################################################

#### SVFit Mass

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$SMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau} [GeV]"\
  --norm_bins=true --datacard="inclusive" $ET_INC_SHIFT \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$SMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="inclusive" \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --var="m_sv"["$SMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="inclusive" \
  --background_scheme="em_default"

#### SVFit Mass MSSM

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1500 \
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.00099 \
  --blind=true --x_blind_min=110 --x_blind_max=2000 \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1500 \
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.00099 \
  --blind=true --x_blind_min=110 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau} [GeV]" $EM_INC_SHIFT \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1500 \
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.00099 \
  --blind=true --x_blind_min=110 --x_blind_max=2000 \
  --background_scheme="em_default"

#### Visible Mass

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_vis"["$SMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="inclusive"\
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_vis"["$SMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="inclusive"\
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --var="m_vis"["$SMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="inclusive"\
  --background_scheme="em_default"

#### Visible Mass MSSM

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $ET_INC_SHIFT \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1500 \
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.00099 \
  --blind=true --x_blind_min=110 --x_blind_max=2000 \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $MT_INC_SHIFT \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1500 \
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.00099 \
  --blind=true --x_blind_min=110 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $EM_INC_SHIFT \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1500 \
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.00099 \
  --blind=true --x_blind_min=110 --x_blind_max=2000 \
  --background_scheme="em_default"

#### MET

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="met(20,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 $ET_INC_SHIFT \
  --norm_bins=true --datacard="inclusive" --extra_pad=1.1 \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="met(20,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 $MT_INC_SHIFT  \
  --norm_bins=true --datacard="inclusive" --extra_pad=1.1 \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="met(20,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 $EM_INC_SHIFT \
  --norm_bins=true --datacard="inclusive" --extra_pad=1.1 \
  --background_scheme="em_default"

#### pt_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_1[0,5,10,15,20,24,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150]" \
  --x_axis_label="Electron p_{T} [GeV]" --datacard="inclusive" $ET_INC_SHIFT \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="et_default"
  
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_1(30,0,150)" $MT_INC_SHIFT \
  --x_axis_label="Muon p_{T} [GeV]" --datacard="inclusive" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="pt_1(30,0,150)" $EM_INC_SHIFT \
  --x_axis_label="Electron p_{T} [GeV]" --datacard="inclusive" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="em_default"

#### pt_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_2(30,0,150)" $ET_INC_SHIFT \
  --x_axis_label="Tau p_{T} [GeV]" --datacard="inclusive" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_2(30,0,150)" $MT_INC_SHIFT \
  --x_axis_label="Tau p_{T} [GeV]" --datacard="inclusive" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="pt_2(30,0,150)" $EM_INC_SHIFT \
  --x_axis_label="Muon p_{T} [GeV]" --datacard="inclusive" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="em_default"

#### pt_tt

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_tt(30,0,300)" $ET_INC_SHIFT \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="inclusive" \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_tt(30,0,300)" $MT_INC_SHIFT \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="inclusive" \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="pt_tt(30,0,300)" $EM_INC_SHIFT \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="inclusive" \
  --background_scheme="em_default"

#### pt_tt log

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_tt(30,0,300)" $ET_INC_SHIFT \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="inclusive" --log_y=true --draw_ratio=true \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_tt(30,0,300)" $MT_INC_SHIFT \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="inclusive" --log_y=true --draw_ratio=true \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="pt_tt(30,0,300)" $EM_INC_SHIFT \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="inclusive" --log_y=true --draw_ratio=true \
  --background_scheme="em_default"

#### eta_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="eta_1(30,-3,3)" --extra_pad=1.7 \
  --x_axis_label="Electron #eta" --datacard="inclusive" $ET_INC_SHIFT \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="eta_1(30,-3,3)" --extra_pad=1.6 \
  --x_axis_label="Muon #eta" --datacard="inclusive" $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5"\
  --method=8 --cat="inclusive" --var="eta_1(30,-3,3)" --extra_pad=1.6 \
  --x_axis_label="Electron #eta" --datacard="inclusive" $EM_INC_SHIFT \
  --background_scheme="em_default"

#### eta_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="eta_2(30,-3,3)" --extra_pad=1.7 \
  --x_axis_label="Tau #eta" --datacard="inclusive" $ET_INC_SHIFT\
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="eta_2(30,-3,3)" --extra_pad=1.5 \
  --x_axis_label="Tau #eta" --datacard="inclusive" $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5"\
  --method=8 --cat="inclusive" --var="eta_2(30,-3,3)" --extra_pad=1.5 \
  --x_axis_label="Muon #eta" --datacard="inclusive" $EM_INC_SHIFT \
  --background_scheme="em_default"

#### m_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="m_2(20,0,2)" --extra_pad=1.1 \
  --x_axis_label="Tau Mass [GeV]" --datacard="inclusive" $ET_INC_SHIFT \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="m_2(20,0,2)" --extra_pad=1.1\
  --x_axis_label="Tau Mass [GeV]" --datacard="inclusive" $MT_INC_SHIFT \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="m_2(20,0,2)" --extra_pad=1.1 \
  --x_axis_label="Tau Mass [GeV]" --datacard="inclusive" $EM_INC_SHIFT \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="em_default"

#### n_jets (log)

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="n_jets(10,-0.5,9.5)"  --x_axis_label="Number of Jets" $ET_INC_SHIFT \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="inclusive" \
  --background_scheme="et_default" \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="n_jets(10,-0.5,9.5)"  --x_axis_label="Number of Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="inclusive" $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm" \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="n_jets(10,-0.5,9.5)"  --x_axis_label="Number of Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="inclusive" $EM_INC_SHIFT \
  --background_scheme="em_default" \
  --custom_y_axis_min=true --y_axis_min=0.99

#### n_bjets (log)

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="n_bjets(5,-0.5,4.5)"  --x_axis_label="Number of b-tagged Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="inclusive" $ET_INC_SHIFT \
  --background_scheme="et_default" \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="n_bjets(5,-0.5,4.5)"  --x_axis_label="Number of b-tagged Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="inclusive" $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm" \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="n_bjets(5,-0.5,4.5)"  --x_axis_label="Number of b-tagged Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="inclusive" $EM_INC_SHIFT \
  --background_scheme="em_default" \
  --custom_y_axis_min=true --y_axis_min=0.99

#### n_jets

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --cat="inclusive" --var="n_jets(10,0,9)"  --x_axis_label="Number of Jets" \
#  --draw_ratio=true --datacard="inclusive" \
#  --background_scheme="et_default" \

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --cat="inclusive" --var="n_jets(10,0,9)"  --x_axis_label="Number of Jets" \
#  --draw_ratio=true --datacard="inclusive" \
#  --background_scheme="et_default" \

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
#  --method=8 --cat="inclusive" --var="n_jets(10,0,9)"  --x_axis_label="Number of Jets" \
#  --draw_ratio=true --datacard="inclusive" \
# --background_scheme="et_default" \

#### n_bjets

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --cat="inclusive" --var="n_bjets(5,0,4)"  --x_axis_label="Number of b-tagged Jets" \
#  --draw_ratio=true --datacard="inclusive" \
#  --background_scheme="et_default" \

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --cat="inclusive" --var="n_bjets(5,0,4)"  --x_axis_label="Number of b-tagged Jets" \
#  --draw_ratio=true --datacard="inclusive" \
#  --background_scheme="mt_with_zmm" \

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
#  --method=8 --cat="inclusive" --var="n_bjets(5,0,4)"  --x_axis_label="Number of b-tagged Jets" \
#  --draw_ratio=true --datacard="inclusive" \
#  --background_scheme="em_default" \

#### mt_1 / pzeta

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et  \
  --method=8 --cat="inclusive" --set_alias="sel:1" --var="mt_1(40,0,160)" \
  --x_axis_label="m_{T} [GeV]" --datacard="inclusive" --extra_pad=1.5 \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt \
  --method=8 --cat="inclusive" --set_alias="sel:1" --var="mt_1(40,0,160)" \
  --x_axis_label="m_{T} [GeV]" --datacard="inclusive" --extra_pad=1.5 \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="pzeta(48,-100,100)" --extra_pad=1.5 \
  --x_axis_label="#slash{P}_{#zeta} - 0.85 #times P_{#zeta}^{vis}" --datacard="inclusive" \
  --background_scheme="em_default"

#### n_vtx

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="n_vtx(30,0,30)" $ET_INC_SHIFT \
  --x_axis_label="Number of Vertices" --datacard="inclusive" --extra_pad=1.5 \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="n_vtx(30,0,30)" $MT_INC_SHIFT \
  --x_axis_label="Number of Vertices" --datacard="inclusive" --extra_pad=1.5 \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="n_vtx(30,0,30)" $EM_INC_SHIFT \
  --x_axis_label="Number of Vertices" --datacard="inclusive" --extra_pad=1.5 \
  --background_scheme="em_default"

#### tau_decay_mode

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --cat="inclusive" --var="tau_decay_mode(3,0,2)" \
#  --x_axis_label="Tau Decay Mode" --datacard="inclusive" --extra_pad=1.5 \
#  --background_scheme="et_default"\
#  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" 

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --cat="inclusive" --var="tau_decay_mode(3,0,2)" \
#  --x_axis_label="Tau Decay Mode" --datacard="inclusive" --extra_pad=1.5 \
#  --background_scheme="mt_with_zmm"\
#  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" 

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
#  --method=8 --cat="inclusive" --var="tau_decay_mode(3,0,2)" \
#  --x_axis_label="Tau Decay Mode" --datacard="inclusive" --extra_pad=1.5 \
#  --background_scheme="em_default" \
#  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" 

###############################################################################
#### Jet selection plots
###############################################################################

#### jpt_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="1jet" --var="jpt_1(20,0,200)" $ET_1JET_SHIFT \
  --x_axis_label="Leading Jet p_{T} [GeV]" --datacard="1jet" \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="1jet" --var="jpt_1(20,0,200)" $MT_1JET_SHIFT \
  --x_axis_label="Leading Jet p_{T} [GeV]" --datacard="1jet" \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="1jet" --var="jpt_1(20,0,200)" $EM_1JET_SHIFT \
  --x_axis_label="Leading Jet p_{T} [GeV]" --datacard="1jet" \
  --background_scheme="em_default"

#### jeta_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="1jet" --var="jeta_1(25,-5,5)" --extra_pad=1.7 $ET_1JET_SHIFT \
  --x_axis_label="Leading Jet #eta [GeV]" --datacard="1jet" \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="1jet" --var="jeta_1(25,-5,5)" --extra_pad=1.5 $MT_1JET_SHIFT \
  --x_axis_label="Leading Jet #eta [GeV]" --datacard="1jet" \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="1jet" --var="jeta_1(25,-5,5)" --extra_pad=1.5 $EM_1JET_SHIFT \
  --x_axis_label="Leading Jet #eta [GeV]" --datacard="1jet" \
  --background_scheme="em_default"

#### jpt_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="twojet" --var="jpt_2(20,0,200)" $ET_2JET_SHIFT \
  --x_axis_label="Subleading Jet p_{T} [GeV]" --datacard="twojet" \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="twojet" --var="jpt_2(20,0,200)" $MT_2JET_SHIFT \
  --x_axis_label="Subleading Jet p_{T} [GeV]" --datacard="twojet" \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="twojet" --var="jpt_2(20,0,200)" $EM_2JET_SHIFT \
  --x_axis_label="Subleading Jet p_{T} [GeV]" --datacard="twojet" \
  --background_scheme="em_default"

#### jeta_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="twojet" --var="jeta_2(25,-5,5)" --extra_pad=1.6 $ET_2JET_SHIFT \
  --x_axis_label="Subleading Jet #eta [GeV]" --datacard="twojet" \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="twojet" --var="jeta_2(25,-5,5)" --extra_pad=1.6 $MT_2JET_SHIFT \
  --x_axis_label="Subleading Jet #eta [GeV]" --datacard="twojet" \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="twojet" --var="jeta_2(25,-5,5)" --extra_pad=1.6 $EM_2JET_SHIFT \
  --x_axis_label="Subleading Jet #eta [GeV]" --datacard="twojet" \
  --background_scheme="em_default"

#### bpt_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="btag" --var="bpt_1(20,0,200)" $ET_BTAG_SHIFT \
  --x_axis_label="Leading b-tagged jet p_{T} [GeV]" --datacard="btag" \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="btag" --var="bpt_1(20,0,200)" \
  --x_axis_label="Leading b-tagged jet p_{T} [GeV]" --datacard="btag" $MT_BTAG_SHIFT \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="btag" --var="bpt_1(20,0,200)" $EM_BTAG_SHIFT \
  --x_axis_label="Leading b-tagged jet p_{T} [GeV]" --datacard="btag" \
  --background_scheme="em_default"

#### beta_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="btag" --var="beta_1(15,-3,3)" --extra_pad=1.7 $ET_BTAG_SHIFT \
  --x_axis_label="Leading b-tagged jet #eta" --datacard="btag" \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="btag" --var="beta_1(15,-3,3)" --extra_pad=1.7 \
  --x_axis_label="Leading b-tagged jet #eta" --datacard="btag" $MT_BTAG_SHIFT \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="btag" --var="beta_1(15,-3,3)" --extra_pad=1.7 \
  --x_axis_label="Leading b-tagged jet #eta" --datacard="btag" $EM_BTAG_SHIFT \
  --background_scheme="em_default"

#### bcsv

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --cat="prebtag" --var="beta_1(25,0,1)" \
#  --x_axis_label="CSV discriminator" --datacard="prebtag" \
#  --background_scheme="et_default"

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --cat="prebtag" --var="beta_1(25,0,1)" \
#  --x_axis_label="CSV discriminator" --datacard="prebtag" \
#  --background_scheme="mt_with_zmm"

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
#  --method=8 --cat="prebtag" --var="beta_1(25,0,1)" \
#  --x_axis_label="CSV discriminator" --datacard="prebtag" \
#  --background_scheme="em_default"

#### mjj

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="twojet" --var="mjj(20,0,1000)" $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="twojet" \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="twojet" --var="mjj(20,0,1000)" $MT_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="twojet" \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="twojet" --var="mjj(20,0,1000)" $EM_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="twojet" \
  --background_scheme="em_default"

#### jdeta

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="twojet" --var="jdeta(20,0,10)" $ET_2JET_SHIFT \
  --x_axis_label="#Delta#eta_{jj}" \
  --datacard="twojet" \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="twojet" --var="jdeta(20,0,10)" $MT_2JET_SHIFT \
  --x_axis_label="#Delta#eta_{jj}" \
  --datacard="twojet" \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="twojet" --var="jdeta(20,0,10)" $EM_2JET_SHIFT \
  --x_axis_label="#Delta#eta_{jj}" \
  --datacard="twojet" \
  --background_scheme="em_default"
