
if (( "$#" != "4" ))
then
  echo "Usage: new_plot_control.sh [PROD] [YEAR] [ANALYSIS] [POSTFIT]"
  exit
fi


PROD=$1
YEAR=$2
ANA=$3
POSTFIT=$4
PARAMS=./scripts/"$PROD"_params_$YEAR.dat
SMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
#MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500"
MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,1000,1500"

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

if [ "$POSTFIT" -gt "0" ]
then
  if [ "$YEAR" = "2012" ]
  then
#### 8TeV
echo "Applying 8TeV scale factors..."
ET_INC_SHIFT="  --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=-0.05 --draw_error_band=true --shift_backgrounds=ZTT:0.988889,QCD:1.13,W:0.826,ZL:1.00394,ZJ:1.25966,VV:0.931043,TT:0.942773 --auto_error_band=0.0348036"
ET_1JET_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=-0.05 --draw_error_band=true --shift_backgrounds=ZTT:1.00365,QCD:1.137,W:0.879405,ZL:1.28914,ZJ:0.991755,VV:0.897549,TT:0.897514 --auto_error_band=0.0410004"
ET_2JET_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=-0.05 --draw_error_band=true --shift_backgrounds=ZTT:0.979685,QCD:1.006,W:1.00505,ZL:1.06674,ZJ:1.02207,VV:0.54884,TT:0.938535 --auto_error_band=0.063913"
ET_BTAG_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=-0.05 --draw_error_band=true --shift_backgrounds=ZTT:1.01152,QCD:1.044,W:0.892,ZL:1.01274,ZJ:1.00913,VV:0.987908,TT:1.0186 --auto_error_band=0.0680558"
ET_BAND_ONLY="  --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=-0.05 --draw_error_band=true --auto_error_band=0.0348036"
MT_INC_SHIFT="  --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=0.00 --draw_error_band=true --shift_backgrounds=ZTT:1.00828,QCD:1.054,W:0.973361,ZL:1.01038,ZJ:1.5733,VV:0.975726,TT:1.02835 --auto_error_band=0.0359505"
MT_1JET_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=0.00 --draw_error_band=true --shift_backgrounds=ZTT:1.01857,QCD:1.12609,W:0.98738,ZL:1.00482,ZJ:1.06436,VV:0.965521,TT:1.01509 --auto_error_band=0.0415041"
MT_2JET_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=0.00 --draw_error_band=true --shift_backgrounds=ZTT:0.974692,QCD:1.2451,W:1.02181,ZL:0.99925,ZJ:1.13157,VV:0.931733,TT:0.866722 --auto_error_band=0.0574947"
MT_BTAG_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=0.00 --draw_error_band=true --shift_backgrounds=ZTT:1.00899,QCD:1.032,W:0.988593,ZL:1.11027,ZJ:1.0071,VV:1.02051,TT:0.993992 --auto_error_band=0.0502441"
MT_BAND_ONLY="  --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=0.00 --draw_error_band=true --auto_error_band=0.0359505"
EM_INC_SHIFT="  --syst_tau_scale=CMS_scale_e_8TeV --shift_tscale=0.73 --draw_error_band=true --shift_backgrounds=Ztt:1.02932,Fakes:1.24846,EWK:0.839777,ttbar:0.953858 --auto_error_band=0.0279584"
EM_1JET_SHIFT=" --syst_tau_scale=CMS_scale_e_8TeV --shift_tscale=0.73 --draw_error_band=true --shift_backgrounds=Ztt:1.01789,Fakes:1.22099,EWK:0.830893,ttbar:0.878769 --auto_error_band=0.0304102"
EM_2JET_SHIFT=" --syst_tau_scale=CMS_scale_e_8TeV --shift_tscale=0.73 --draw_error_band=true --shift_backgrounds=Ztt:1.02724,Fakes:1.29028,EWK:0.764114,ttbar:0.878092 --auto_error_band=0.0435572"
EM_BTAG_SHIFT=" --syst_tau_scale=CMS_scale_e_8TeV --shift_tscale=0.73 --draw_error_band=true --shift_backgrounds=Ztt:1.02257,Fakes:1.14773,EWK:0.85753,ttbar:1.01484 --auto_error_band=0.0549386"
EM_BAND_ONLY="  --syst_tau_scale=CMS_scale_e_8TeV --shift_tscale=0.73 --draw_error_band=true --auto_error_band=0.0279584"
  else
#### 7TeV
echo "Applying 7TeV scale factors..."
 ET_INC_SHIFT="  --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.38 --draw_error_band=true --shift_backgrounds=ZTT:1.0112,QCD:1.069,W:0.908,ZL:0.94465,ZJ:1.07646,VV:0.955839,TT:0.976438 --auto_error_band=0.0366997"
 ET_1JET_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.38 --draw_error_band=true --shift_backgrounds=ZTT:1.04227,QCD:1.039,W:0.994,ZL:0.904975,ZJ:1.03252,VV:0.967582,TT:0.995355 --auto_error_band=0.0522536"
 ET_2JET_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.38 --draw_error_band=true --shift_backgrounds=ZTT:1.03043,QCD:1.168,W:0.9195,ZL:0.996062,ZJ:0.994579,VV:0.707416,TT:0.942674 --auto_error_band=0.0662644"
 ET_BTAG_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.38 --draw_error_band=true --shift_backgrounds=ZTT:0.99654,QCD:1.028,W:0.967,ZL:0.942253,ZJ:1.00641,VV:0.978346,TT:0.996264 --auto_error_band=0.0658388"
 ET_BAND_ONLY="  --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.38 --draw_error_band=true --auto_error_band=0.0366997"
 MT_INC_SHIFT="  --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.18 --draw_error_band=true --shift_backgrounds=ZTT:0.997977,QCD:1.198,W:0.936,ZL:0.935024,ZJ:1.05041,VV:0.930463,TT:0.945546 --auto_error_band=0.0373457"
 MT_1JET_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.18 --draw_error_band=true --shift_backgrounds=ZTT:0.994171,QCD:1.27103,W:1,ZL:0.934182,ZJ:1.01369,VV:0.925674,TT:0.935423 --auto_error_band=0.0535629"
 MT_2JET_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.18 --draw_error_band=true --shift_backgrounds=ZTT:0.985273,QCD:1.44,W:1.0782,ZL:0.928469,ZJ:1.06782,VV:0.937195,TT:0.993202 --auto_error_band=0.0616504"
 MT_BTAG_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.18 --draw_error_band=true --shift_backgrounds=ZTT:1.02187,QCD:0.966,W:1.045,ZL:1.10017,ZJ:1.00731,VV:0.994653,TT:1.0061 --auto_error_band=0.0584436"
 MT_BAND_ONLY="  --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.18 --draw_error_band=true --auto_error_band=0.0373457"
 EM_INC_SHIFT="  --syst_tau_scale=CMS_scale_e_7TeV --shift_tscale=0.01 --draw_error_band=true --shift_backgrounds=Ztt:0.956985,Fakes:1.42411,EWK:0.881394,ttbar:0.967891 --auto_error_band=0.0292367"
 EM_1JET_SHIFT=" --syst_tau_scale=CMS_scale_e_7TeV --shift_tscale=0.01 --draw_error_band=true --shift_backgrounds=Ztt:0.954401,Fakes:1.46798,EWK:0.849535,ttbar:0.929497 --auto_error_band=0.0355015"
 EM_2JET_SHIFT=" --syst_tau_scale=CMS_scale_e_7TeV --shift_tscale=0.01 --draw_error_band=true --shift_backgrounds=Ztt:0.940717,Fakes:1.49461,EWK:0.773418,ttbar:0.848893 --auto_error_band=0.0552723"
 EM_BTAG_SHIFT=" --syst_tau_scale=CMS_scale_e_7TeV --shift_tscale=0.01 --draw_error_band=true --shift_backgrounds=Ztt:0.973808,Fakes:1.4744,EWK:0.889495,ttbar:0.959302 --auto_error_band=0.0739144"
 EM_BAND_ONLY="  --syst_tau_scale=CMS_scale_e_7TeV --shift_tscale=0.01 --draw_error_band=true --auto_error_band=0.0292367"
   fi
 fi

################################################################################
##### Inclusive selection plots
################################################################################

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
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=2000 \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=2000 \
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
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=110 --x_blind_max=2000 \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=110 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=110 --x_blind_max=2000 \
  --background_scheme="em_default"

#### MET

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="met(20,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
  --norm_bins=true --datacard="inclusive" --extra_pad=1.1 \
  --background_scheme="et_default" $ET_INC_SHIFT 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="met(20,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
  --norm_bins=true --datacard="inclusive" --extra_pad=1.1 \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="met(20,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
  --norm_bins=true --datacard="inclusive" --extra_pad=1.1 \
  --background_scheme="em_default" $EM_INC_SHIFT

#### pt_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_1(50,0,100)" \
  --x_axis_label="Electron p_{T} [GeV]" --datacard="inclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT
  
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_1(50,0,100)" \
  --x_axis_label="Muon p_{T} [GeV]" --datacard="inclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="pt_1(50,0,100)" \
  --x_axis_label="Electron p_{T} [GeV]" --datacard="inclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT

#### pt_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_2(50,0,100)" \
  --x_axis_label="Tau p_{T} [GeV]" --datacard="inclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_2(50,0,100)" \
  --x_axis_label="Tau p_{T} [GeV]" --datacard="inclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="pt_2(50,0,100)" \
  --x_axis_label="Muon p_{T} [GeV]" --datacard="inclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT

#### pt_tt

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="inclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="inclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="inclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT

#### pt_tt log

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="inclusive" --log_y=true --draw_ratio=true \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="inclusive" --log_y=true --draw_ratio=true \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="inclusive" --log_y=true --draw_ratio=true \
  --background_scheme="em_default" $EM_INC_SHIFT

#### eta_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="eta_1(30,-3,3)" --extra_pad=1.7 \
  --x_axis_label="Electron #eta" --datacard="inclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="eta_1(30,-3,3)" --extra_pad=1.6 \
  --x_axis_label="Muon #eta" --datacard="inclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5"\
  --method=8 --cat="inclusive" --var="eta_1(30,-3,3)" --extra_pad=1.6 \
  --x_axis_label="Electron #eta" --datacard="inclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT

#### eta_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="eta_2(30,-3,3)" --extra_pad=1.7 \
  --x_axis_label="Tau #eta" --datacard="inclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="eta_2(30,-3,3)" --extra_pad=1.5 \
  --x_axis_label="Tau #eta" --datacard="inclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5"\
  --method=8 --cat="inclusive" --var="eta_2(30,-3,3)" --extra_pad=1.5 \
  --x_axis_label="Muon #eta" --datacard="inclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT

#### m_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="m_2(20,0,2)" --extra_pad=1.1 \
  --x_axis_label="Tau Mass [GeV]" --datacard="inclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="m_2(20,0,2)" --extra_pad=1.1\
  --x_axis_label="Tau Mass [GeV]" --datacard="inclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### n_jets (log)

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="n_jets(10,-0.5,9.5)"  --x_axis_label="Number of Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="inclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="n_jets(10,-0.5,9.5)"  --x_axis_label="Number of Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="inclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="n_jets(10,-0.5,9.5)"  --x_axis_label="Number of Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="inclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

#### n_bjets (log)

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="n_bjets(5,-0.5,4.5)"  --x_axis_label="Number of b-tagged Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="inclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="n_bjets(5,-0.5,4.5)"  --x_axis_label="Number of b-tagged Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="inclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="n_bjets(5,-0.5,4.5)"  --x_axis_label="Number of b-tagged Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="inclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

#### mt_1 / pzeta

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et  \
  --method=8 --cat="inclusive" --set_alias="sel:1" --var="mt_1(40,0,160)" \
  --x_axis_label="m_{T} [GeV]" --datacard="inclusive" --extra_pad=1.1 $ET_BAND_ONLY \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt \
  --method=8 --cat="inclusive" --set_alias="sel:1" --var="mt_1(40,0,160)" \
  --x_axis_label="m_{T} [GeV]" --datacard="inclusive" --extra_pad=1.1 $MT_BAND_ONLY \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="inclusive" --var="pzeta(48,-100,100)" --extra_pad=1.5 $EM_BAND_ONLY \
  --x_axis_label="D_{#zeta} [GeV]" --datacard="inclusive" \
  --background_scheme="em_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="inclusive" --var="pzetavis(25,0,50)" --extra_pad=1. $EM_BAND_ONLY \
  --x_axis_label="P_{#zeta}^{vis} [GeV]" --datacard="inclusive" \
  --background_scheme="em_default"

 ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="inclusive" --var="pzetamiss(25,-100,100)" --extra_pad=1. $EM_BAND_ONLY \
  --x_axis_label="#slash{P}_{#zeta} [GeV]" --datacard="inclusive" \
  --background_scheme="em_default"

 ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="inclusive" --var="em_gf_mva(40,-1,1)" --extra_pad=1. $EM_BAND_ONLY \
  --x_axis_label="BDT output" --datacard="inclusive" \
  --background_scheme="em_default"
 
 ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="inclusive" --var="em_gf_mva(40,-1,1)" --extra_pad=100. $EM_BAND_ONLY \
  --x_axis_label="BDT output" --datacard="inclusive" --log_y=true --draw_ratio=true \
  --background_scheme="em_default"
 
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="inclusive" --var="mt_ll(40,0,160)" --extra_pad=1. $EM_BAND_ONLY \
  --x_axis_label="m_{T}(ll) [GeV]" --datacard="inclusive" \
  --background_scheme="em_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="inclusive" --var="emu_dphi(40,0,3.15)" --extra_pad=1.5 $EM_BAND_ONLY \
  --x_axis_label="#Delta#phi_{e,#mu}" --datacard="inclusive" \
  --background_scheme="em_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="inclusive" --var="emu_dxy_1(40,-0.02,0.02)" --extra_pad=10 --log_y=true --draw_ratio=true \
  --x_axis_label="Electron d_{0}^{vtx} [cm]" --datacard="inclusive" $EM_BAND_ONLY \
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

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="tau_decay_mode(3,-4,11)" $ET_INC_SHIFT \
  --x_axis_label="Tau Decay Mode" --datacard="inclusive" --extra_pad=1.6 \
  --background_scheme="et_default"\
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="tau_decay_mode(3,-4,11)" $MT_INC_SHIFT \
  --x_axis_label="Tau Decay Mode" --datacard="inclusive" --extra_pad=1.6 \
  --background_scheme="mt_with_zmm"\
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" 


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
  --method=8 --cat="1jet" --var="jeta_1(20,-5,5)" --extra_pad=1.7 $ET_1JET_SHIFT \
  --x_axis_label="Leading Jet #eta [GeV]" --datacard="1jet" \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="1jet" --var="jeta_1(20,-5,5)" --extra_pad=1.5 $MT_1JET_SHIFT \
  --x_axis_label="Leading Jet #eta [GeV]" --datacard="1jet" \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="1jet" --var="jeta_1(20,-5,5)" --extra_pad=1.5 $EM_1JET_SHIFT \
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
  --method=8 --cat="twojet" --var="jeta_2(20,-5,5)" --extra_pad=1.6 $ET_2JET_SHIFT \
  --x_axis_label="Subleading Jet #eta [GeV]" --datacard="twojet" \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="twojet" --var="jeta_2(20,-5,5)" --extra_pad=1.6 $MT_2JET_SHIFT \
  --x_axis_label="Subleading Jet #eta [GeV]" --datacard="twojet" \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="twojet" --var="jeta_2(20,-5,5)" --extra_pad=1.6 $EM_2JET_SHIFT \
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
BETA_BINS="beta_1(10,-3,3)"
if [ "$YEAR" = "2012" ]
then
  BETA_BINS="beta_1(15,-3,3)"
fi
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="btag" --var=$BETA_BINS --extra_pad=1.7 $ET_BTAG_SHIFT \
  --x_axis_label="Leading b-tagged jet #eta" --datacard="btag" \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="btag" --var=$BETA_BINS --extra_pad=1.7 \
  --x_axis_label="Leading b-tagged jet #eta" --datacard="btag" $MT_BTAG_SHIFT \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="btag" --var=$BETA_BINS --extra_pad=1.7 \
  --x_axis_label="Leading b-tagged jet #eta" --datacard="btag" $EM_BTAG_SHIFT \
  --background_scheme="em_default"

#### bcsv

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="n_jets<=1 && n_prebjets>=1" --var="bcsv_1(25,0,1)" \
  --x_axis_label="CSV discriminator" --datacard="prebtag"  $ET_BTAG_SHIFT \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="n_jets<=1 && n_prebjets>=1" --var="bcsv_1(25,0,1)" \
  --x_axis_label="CSV discriminator" --datacard="prebtag"  $MT_BTAG_SHIFT \
  --background_scheme="mt_with_zmm"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="n_jets<=1 && n_prebjets>=1" --var="bcsv_1(25,0,1)" \
  --x_axis_label="CSV discriminator" --datacard="prebtag"  $EM_BTAG_SHIFT \
  --background_scheme="em_default"

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
