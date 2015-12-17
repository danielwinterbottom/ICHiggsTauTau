
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
#SMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
#MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500"
MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,900"
MSSMBINSCOARSE="0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,900"

################################################################################
##### Inclusive selection plots
################################################################################

#### SVFit Mass

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --var="m_sv"["$SMBINS"] --cat="notwoprong" \
#  --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]"\
#  --norm_bins=true --datacard="notwoprong" $ET_INC_SHIFT \
#  --background_scheme="et_default"

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --var="m_sv"["$SMBINS"] --cat="notwoprong" \
#  --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" $MT_INC_SHIFT \
#  --norm_bins=true --datacard="notwoprong" \
#  --background_scheme="mt_with_zmm"

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:mt_1<30" \
#  --method=8 --var="m_sv"["$SMBINS"] --cat="notwoprong" \
#  --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" $EM_INC_SHIFT \
#  --norm_bins=true --datacard="notwoprong" \
#  --background_scheme="em_no_qcd"

#### SVFit Mass MSSM

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --var="m_sv"["$MSSMBINS"] --cat="notwoprong" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="notwoprong" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=false --x_blind_min=100 --x_blind_max=2000 \
#  --background_scheme="et_default"

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --var="m_sv"["$MSSMBINS"] --cat="notwoprong" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
#  --norm_bins=true --datacard="notwoprong" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=false --x_blind_min=100 --x_blind_max=2000 \
#  --background_scheme="mt_with_zmm"
#
#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:mt_1<30" \
#  --method=8 --var="m_sv"["$MSSMBINS"] --cat="notwoprong" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $EM_INC_SHIFT \
#  --norm_bins=true --datacard="notwoprong" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=false --x_blind_min=100 --x_blind_max=2000 \
#  --background_scheme="em_no_qcd"
#

######## notwoprong
#### Visible Mass

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$SMBINS"] --cat="notwoprong" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="notwoprong"\
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$SMBINS"] --cat="notwoprong" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="notwoprong"\
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-20." \
  --method=8 --var="m_sv"["$SMBINS"] --cat="inclusive" \
 --x_axis_label="M_{#tau#tau} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="inclusive"\
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="em_default" --extra_pad=0.2

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_sv"["$SMBINS"] --cat="notwoprong" \
 --x_axis_label="M_{#tau#tau} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="notwoprong"\
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001


#### Visible Mass MSSM

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="notwoprong" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="notwoprong" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="notwoprong" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="notwoprong" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001 

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-20." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="inclusive" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="em_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001 

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="notwoprong" \
  --x_axis_label="M_{#tau#tau} [GeV]" $TT_INC_SHIFT \
  --norm_bins=true --datacard="notwoprong" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001
#
######## btagnotwoprong

#### Visible Mass

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$SMBINS"] --cat="btagnotwoprong" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="btagnotwoprong"\
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$SMBINS"] --cat="btagnotwoprong" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="btagnotwoprong"\
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-20." \
  --method=8 --var="m_sv"["$SMBINS"] --cat="btag" \
 --x_axis_label="M_{#tau#tau} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="btag"\
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="em_default" --extra_pad=0.2

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_sv"["$SMBINS"] --cat="btagnotwoprong" \
 --x_axis_label="M_{#tau#tau} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="btagnotwoprong"\
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001


#### Visible Mass MSSM

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINSCOARSE"] --cat="btagnotwoprong" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="btagnotwoprong" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINSCOARSE"] --cat="btagnotwoprong" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="btagnotwoprong" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-20." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="btag" \
  --x_axis_label="M_{#tau#tau} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="btag" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="em_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="btagnotwoprong" \
  --x_axis_label="M_{#tau#tau} [GeV]" $TT_INC_SHIFT \
  --norm_bins=true --datacard="btagnotwoprong" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001
#
######## no-nobtagnotwoprong

#### Visible Mass

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$SMBINS"] --cat="nobtagnotwoprong" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="nobtagnotwoprong"\
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$SMBINS"] --cat="nobtagnotwoprong" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="nobtagnotwoprong"\
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-20." \
  --method=8 --var="m_sv"["$SMBINS"] --cat="nobtag" \
 --x_axis_label="M_{#tau#tau} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="nobtag"\
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="em_default" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_sv"["$SMBINS"] --cat="nobtagnotwoprong" \
 --x_axis_label="M_{#tau#tau} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="nobtagnotwoprong"\
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001


#### Visible Mass MSSM

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="nobtagnotwoprong" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="nobtagnotwoprong" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="nobtagnotwoprong" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="nobtagnotwoprong" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-20." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="nobtag" \
  --x_axis_label="M_{#tau#tau} [GeV]" $EM_INC_SHIFT \
  --norm_bins=true --datacard="nobtag" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="em_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="nobtagnotwoprong" \
  --x_axis_label="M_{#tau#tau} [GeV]" $TT_INC_SHIFT \
  --norm_bins=true --datacard="nobtagnotwoprong" --log_y=true --draw_ratio=true \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=80 --x_blind_max=2000 \
  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001


######## MSSM update categories

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --var="m_sv"["$MSSMBINS"] --cat="nobtagnotwopronglow" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="nobtagnotwopronglow" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=80 --x_blind_max=2000 \
#  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --var="m_sv"["$MSSMBINS"] --cat="nobtagnotwopronglow" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
#  --norm_bins=true --datacard="nobtagnotwopronglow" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=80 --x_blind_max=2000 \
#  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
#  --method=8 --var="m_sv"["$MSSMBINS"] --cat="nobtagnotwopronglow" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $TT_INC_SHIFT \
#  --norm_bins=true --datacard="nobtagnotwopronglow" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=80 --x_blind_max=2000 \
#  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --var="m_sv"["$MSSMBINS"] --cat="nobtagnotwoprongmed" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="nobtagnotwoprongmed" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=80 --x_blind_max=2000 \
#  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --var="m_sv"["$MSSMBINS"] --cat="nobtagnotwoprongmed" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
#  --norm_bins=true --datacard="nobtagnotwoprongmed" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=80 --x_blind_max=2000 \
#  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
#  --method=8 --var="m_sv"["$MSSMBINS"] --cat="nobtagnotwoprongmed" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $TT_INC_SHIFT \
#  --norm_bins=true --datacard="nobtagnotwoprongmed" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=80 --x_blind_max=2000 \
#  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --var="m_sv"["$MSSMBINS"] --cat="nobtagnotwopronghigh" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="nobtagnotwopronghigh" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=80 --x_blind_max=2000 \
#  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --var="m_sv"["$MSSMBINS"] --cat="nobtagnotwopronghigh" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
#  --norm_bins=true --datacard="nobtagnotwopronghigh" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=80 --x_blind_max=2000 \
#  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
#  --method=8 --var="m_sv"["$MSSMBINS"] --cat="nobtagnotwopronghigh" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $TT_INC_SHIFT \
#  --norm_bins=true --datacard="nobtagnotwopronghigh" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=80 --x_blind_max=2000 \
#  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --var="m_sv"["$MSSMBINSCOARSE"] --cat="btagnotwopronglow" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="btagnotwopronglow" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=80 --x_blind_max=2000 \
#  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
 # --method=8 --var="m_sv"["$MSSMBINSCOARSE"] --cat="btagnotwopronglow" \
 # --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
 # --norm_bins=true --datacard="btagnotwopronglow" --log_y=true --draw_ratio=true \
 # --custom_y_axis_min=true --y_axis_min=0.0099 \
 # --blind=true --x_blind_min=80 --x_blind_max=2000 \
#  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
#  --method=8 --var="m_sv"["$MSSMBINSCOARSE"] --cat="btagnotwopronglow" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $TT_INC_SHIFT \
#  --norm_bins=true --datacard="btagnotwopronglow" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=80 --x_blind_max=2000 \
#  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --var="m_sv"["$MSSMBINSCOARSE"] --cat="btagnotwopronghigh" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="btagnotwopronghigh" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=80 --x_blind_max=2000 \
#  --background_scheme="et_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --var="m_sv"["$MSSMBINSCOARSE"] --cat="btagnotwopronghigh" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
#  --norm_bins=true --datacard="btagnotwopronghigh" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=80 --x_blind_max=2000 \
#  --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001

#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=tt --set_alias="sel:1" \
#  --method=8 --var="m_sv"["$MSSMBINSCOARSE"] --cat="btagnotwopronghigh" \
#  --x_axis_label="M_{#tau#tau} [GeV]" $TT_INC_SHIFT \
#  --norm_bins=true --datacard="btagnotwopronghigh" --log_y=true --draw_ratio=true \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=80 --x_blind_max=2000 \
#  --background_scheme="tt_default" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001
