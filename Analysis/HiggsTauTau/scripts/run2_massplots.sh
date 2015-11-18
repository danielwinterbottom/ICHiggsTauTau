
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
SMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150"
#SMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
#MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500"
MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,900"

################################################################################
##### Inclusive selection plots
################################################################################

#### SVFit Mass

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:1." \
#  --method=8 --var="m_sv"["$SMBINS"] --cat="inclusive" \
#  --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]"\
#  --norm_bins=true --datacard="inclusive" $ET_INC_SHIFT \
#  --background_scheme="et_default"

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:1." \
#  --method=8 --var="m_sv"["$SMBINS"] --cat="inclusive" \
#  --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" $MT_INC_SHIFT \
#  --norm_bins=true --datacard="inclusive" \
#  --background_scheme="mt_with_zmm"

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
#  --method=8 --var="m_sv"["$SMBINS"] --cat="inclusive" \
#  --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" $EM_INC_SHIFT \
#  --norm_bins=true --datacard="inclusive" \
#  --background_scheme="em_no_qcd"

#### SVFit Mass MSSM

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:1." \
#  --method=8 --var="m_sv"["$MSSMBINS"] --cat="inclusive" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=false --x_blind_min=100 --x_blind_max=2000 \
#  --background_scheme="et_default"

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:1." \
#  --method=8 --var="m_sv"["$MSSMBINS"] --cat="inclusive" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
#  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=false --x_blind_min=100 --x_blind_max=2000 \
#  --background_scheme="mt_with_zmm"
#
#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
#  --method=8 --var="m_sv"["$MSSMBINS"] --cat="inclusive" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $EM_INC_SHIFT \
#  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=false --x_blind_min=100 --x_blind_max=2000 \
#  --background_scheme="em_no_qcd"
#

######## inclusive
#### Visible Mass

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:1." \
  --method=8 --var="m_vis"["$SMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="inclusive"\
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:1." \
  --method=8 --var="m_vis"["$SMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="inclusive"\
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --var="m_vis"["$SMBINS"] --cat="inclusive" \
 --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="inclusive"\
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="em_default" --extra_pad=0.2

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_vis"["$SMBINS"] --cat="inclusive" \
 --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="inclusive"\
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001


#### Visible Mass MSSM

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001 

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="em_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001 

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $TT_INC_SHIFT \
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001
#
######## btag

#### Visible Mass

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:1." \
  --method=8 --var="m_vis"["$SMBINS"] --cat="btag" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="btag"\
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:1." \
  --method=8 --var="m_vis"["$SMBINS"] --cat="btag" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="btag"\
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --var="m_vis"["$SMBINS"] --cat="btag" \
 --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="btag"\
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="em_default" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_vis"["$SMBINS"] --cat="btag" \
 --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="btag"\
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001


#### Visible Mass MSSM

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="btag" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="btag" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="btag" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="btag" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="btag" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="btag" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="em_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="btag" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $TT_INC_SHIFT \
  --norm_bins=true --datacard="btag" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001
#
######## no-nobtag

#### Visible Mass

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:1." \
  --method=8 --var="m_vis"["$SMBINS"] --cat="nobtag" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="nobtag"\
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:1." \
  --method=8 --var="m_vis"["$SMBINS"] --cat="nobtag" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="nobtag"\
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --var="m_vis"["$SMBINS"] --cat="nobtag" \
 --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="nobtag"\
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="em_default" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_vis"["$SMBINS"] --cat="nobtag" \
 --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="nobtag"\
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001


#### Visible Mass MSSM

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="nobtag" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="nobtag" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="nobtaglow" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="nobtaglow" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="nobtaglow" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="nobtaglow" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="em_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="nobtaglow" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $TT_INC_SHIFT \
  --norm_bins=true --datacard="nobtaglow" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001
#

######## MSSM update categories

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="nobtaglow" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="nobtaglow" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="nobtaglow" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="nobtaglow" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="nobtaglow" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $TT_INC_SHIFT \
  --norm_bins=true --datacard="nobtaglow" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="nobtagmed" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="nobtagmed" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="nobtagmed" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="nobtagmed" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="nobtagmed" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $TT_INC_SHIFT \
  --norm_bins=true --datacard="nobtagmed" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="nobtaghigh" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="nobtaghigh" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="nobtaghigh" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="nobtaghigh" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="nobtaghigh" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $TT_INC_SHIFT \
  --norm_bins=true --datacard="nobtaghigh" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="btaglow" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="btaglow" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="btaglow" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="btaglow" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="btaglow" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $TT_INC_SHIFT \
  --norm_bins=true --datacard="btaglow" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="btaghigh" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="btaghigh" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:1." \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="btaghigh" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="btaghigh" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_vis"["$MSSMBINS"] --cat="btaghigh" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $TT_INC_SHIFT \
  --norm_bins=true --datacard="btaghigh" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=40 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001
