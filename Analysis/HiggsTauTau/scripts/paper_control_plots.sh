
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
#ET_INC_SHIFT="  --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=-0.05 --draw_error_band=true --shift_backgrounds=ZTT:0.988889,QCD:1.13,W:0.826,ZL:1.00394,ZJ:1.25966,VV:0.931043,TT:0.942773 --auto_error_band=0.0348036"
#ET_1JET_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=-0.05 --draw_error_band=true --shift_backgrounds=ZTT:1.00365,QCD:1.137,W:0.879405,ZL:1.28914,ZJ:0.991755,VV:0.897549,TT:0.897514 --auto_error_band=0.0410004"
#ET_2JET_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=-0.05 --draw_error_band=true --shift_backgrounds=ZTT:0.979685,QCD:1.006,W:1.00505,ZL:1.06674,ZJ:1.02207,VV:0.54884,TT:0.938535 --auto_error_band=0.063913"

ET_INC_SHIFT="  --draw_error_band=true --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=0.32 --shift_backgrounds=ZTT:0.996753,QCD:1.018,W:0.962,ZL:0.911294,ZJ:0.959214,VV:0.967928,TT:0.88158 --auto_error_band=0.0404751"
ET_1JET_SHIFT=" --draw_error_band=true --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=0.32 --shift_backgrounds=ZTT:0.985596,QCD:1.08031,W:0.939856,ZL:0.823454,ZJ:1.16531,VV:0.979768,TT:0.883425 --auto_error_band=0.0476111"
ET_2JET_SHIFT=" --draw_error_band=true --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=0.32 --shift_backgrounds=ZTT:0.931618,QCD:1.01504,W:0.999488,ZL:0.9751,ZJ:1.00661,VV:0.995278,TT:0.68669 --auto_error_band=0.0761749"

ET_BTAG_SHIFT=" --draw_error_band=true --shift_backgrounds=ZTT:1.01152,QCD:1.044,W:0.892,ZL:1.01274,ZJ:1.00913,VV:0.987908,TT:1.0186 --auto_error_band=0.0680558"
#ET_BAND_ONLY="  --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=-0.05 --draw_error_band=true --auto_error_band=0.0348036"
ET_BAND_ONLY="  --draw_error_band=true --syst_tau_scale=CMS_scale_t_etau_8TeV --shift_tscale=0.32 --auto_error_band=0.0404751"

#MT_INC_SHIFT="  --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=0.00 --draw_error_band=true --shift_backgrounds=ZTT:1.00828,QCD:1.054,W:0.973361,ZL:1.01038,ZJ:1.5733,VV:0.975726,TT:1.02835 --auto_error_band=0.0359505"
#MT_1JET_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=0.00 --draw_error_band=true --shift_backgrounds=ZTT:1.01857,QCD:1.12609,W:0.98738,ZL:1.00482,ZJ:1.06436,VV:0.965521,TT:1.01509 --auto_error_band=0.0415041"
#MT_2JET_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=0.00 --draw_error_band=true --shift_backgrounds=ZTT:0.974692,QCD:1.2451,W:1.02181,ZL:0.99925,ZJ:1.13157,VV:0.931733,TT:0.866722 --auto_error_band=0.0574947"

MT_INC_SHIFT="  --draw_error_band=true --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=-0.13 --shift_backgrounds=ZTT:0.966278,QCD:1.0258,W:1.03862,ZL:1.15101,ZJ:1.19543,VV:0.97629,TT:1.07285 --auto_error_band=0.0398268"
MT_1JET_SHIFT=" --draw_error_band=true --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=-0.13 --shift_backgrounds=ZTT:0.975796,QCD:1.05606,W:1.0081,ZL:1.13501,ZJ:1.02113,VV:0.954604,TT:1.0407 --auto_error_band=0.0463671"
MT_2JET_SHIFT=" --draw_error_band=true --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=-0.13 --shift_backgrounds=ZTT:0.962899,QCD:1.00001,W:0.97714,ZL:1.13431,ZJ:0.97044,VV:0.896924,TT:1.01001 --auto_error_band=0.0707328"

MT_BTAG_SHIFT=" --draw_error_band=true --shift_backgrounds=ZTT:1.00899,QCD:1.032,W:0.988593,ZL:1.11027,ZJ:1.0071,VV:1.02051,TT:0.993992 --auto_error_band=0.0502441"
#MT_BAND_ONLY="  --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=0.00 --draw_error_band=true --auto_error_band=0.0359505"
MT_BAND_ONLY="  --draw_error_band=true --syst_tau_scale=CMS_scale_t_mutau_8TeV --shift_tscale=-0.13 --auto_error_band=0.0398268"


EM_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=Ztt:1.03944,Fakes:1.04722,EWK:0.877523,ttbar:0.973903 --auto_error_band=0.0279987"
EM_1JET_SHIFT=" --draw_error_band=true --shift_backgrounds=Ztt:1.01729,Fakes:1.00743,EWK:0.877523,ttbar:0.892491 --auto_error_band=0.0303276"
EM_2JET_SHIFT=" --draw_error_band=true --shift_backgrounds=Ztt:1.00304,Fakes:1.04984,EWK:0.76028,ttbar:0.853779 --auto_error_band=0.0449435"
EM_BTAG_SHIFT=" --draw_error_band=true --shift_backgrounds=Ztt:1.00304,Fakes:1.04984,EWK:0.76028,ttbar:0.853779 --auto_error_band=0.0449435"
EM_BAND_ONLY="  --draw_error_band=true  --auto_error_band=0.0279987"

  else
#### 7TeV
echo "Applying 7TeV scale factors..."
 #ET_INC_SHIFT="  --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.38 --draw_error_band=true --shift_backgrounds=ZTT:1.0112,QCD:1.069,W:0.908,ZL:0.94465,ZJ:1.07646,VV:0.955839,TT:0.976438 --auto_error_band=0.0366997"
 #ET_1JET_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.38 --draw_error_band=true --shift_backgrounds=ZTT:1.04227,QCD:1.039,W:0.994,ZL:0.904975,ZJ:1.03252,VV:0.967582,TT:0.995355 --auto_error_band=0.0522536"
 #ET_2JET_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.38 --draw_error_band=true --shift_backgrounds=ZTT:1.03043,QCD:1.168,W:0.9195,ZL:0.996062,ZJ:0.994579,VV:0.707416,TT:0.942674 --auto_error_band=0.0662644"
 
 ET_INC_SHIFT="  --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.0 --draw_error_band=true --shift_backgrounds=ZTT:0.997709,QCD:0.978,W:0.966,ZL:1.09724,ZJ:1.05877,VV:0.943978,TT:0.933292 --auto_error_band=0.0449155"
 ET_1JET_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.0 --draw_error_band=true --shift_backgrounds=ZTT:1.02092,QCD:1.13813,W:0.933,ZL:0.777645,ZJ:1.1493,VV:0.946284,TT:0.944463 --auto_error_band=0.0527342"
 ET_2JET_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.0 --draw_error_band=true --shift_backgrounds=ZTT:0.97902,QCD:0.936015,W:0.735,ZL:1.02007,ZJ:2.1522,VV:0.849706,TT:0.869947 --auto_error_band=0.0965299"
 
 ET_BTAG_SHIFT=" --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.0 --draw_error_band=true --shift_backgrounds=ZTT:0.99654,QCD:1.028,W:0.967,ZL:0.942253,ZJ:1.00641,VV:0.978346,TT:0.996264 --auto_error_band=0.0658388"
 #ET_BAND_ONLY="  --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.38 --draw_error_band=true --auto_error_band=0.0366997"
 ET_BAND_ONLY="  --syst_tau_scale=CMS_scale_t_etau_7TeV --shift_tscale=0.0 --draw_error_band=true --auto_error_band=0.0449155"
 
 #MT_INC_SHIFT="  --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.18 --draw_error_band=true --shift_backgrounds=ZTT:0.997977,QCD:1.198,W:0.936,ZL:0.935024,ZJ:1.05041,VV:0.930463,TT:0.945546 --auto_error_band=0.0373457"
 #MT_1JET_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.18 --draw_error_band=true --shift_backgrounds=ZTT:0.994171,QCD:1.27103,W:1,ZL:0.934182,ZJ:1.01369,VV:0.925674,TT:0.935423 --auto_error_band=0.0535629"
 #MT_2JET_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.18 --draw_error_band=true --shift_backgrounds=ZTT:0.985273,QCD:1.44,W:1.0782,ZL:0.928469,ZJ:1.06782,VV:0.937195,TT:0.993202 --auto_error_band=0.0616504"
 
 MT_INC_SHIFT="  --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.0 --draw_error_band=true --shift_backgrounds=ZTT:0.978648,QCD:1.128,W:1.024,ZL:0.951236,ZJ:1.07314,VV:0.944919,TT:0.967328 --auto_error_band=0.0429093"
 MT_1JET_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.0 --draw_error_band=true --shift_backgrounds=ZTT:0.989626,QCD:1.25502,W:0.996,ZL:0.953328,ZJ:1.02666,VV:0.942276,TT:0.965781 --auto_error_band=0.0535321"
 MT_2JET_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.0 --draw_error_band=true --shift_backgrounds=ZTT:0.973136,QCD:1.3476,W:1.138,ZL:1.06602,ZJ:1.19576,VV:0.908979,TT:0.981982 --auto_error_band=0.0745095"
 
 MT_BTAG_SHIFT=" --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.00 --draw_error_band=true --shift_backgrounds=ZTT:1.02187,QCD:0.966,W:1.045,ZL:1.10017,ZJ:1.00731,VV:0.994653,TT:1.0061 --auto_error_band=0.0584436"
 #MT_BAND_ONLY="  --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.18 --draw_error_band=true --auto_error_band=0.0373457"
 MT_BAND_ONLY="  --syst_tau_scale=CMS_scale_t_mutau_7TeV --shift_tscale=0.0 --draw_error_band=true --auto_error_band=0.0429093"
 
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
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_2(25,30,80)" \
  --x_axis_label="p_{T}^{#tau_{h}} [GeV]" --datacard="inclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT --extra_pad=1.15 --draw_ratio=true

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_2(25,30,80)" \
  --x_axis_label="p_{T}^{#tau_{h}} [GeV]" --datacard="inclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT --extra_pad=1.15 --draw_ratio=true --log_y=true
#
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="inclusive" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="inclusive" --log_y=true --draw_ratio=true \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT --extra_pad=10 

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_sm_2012.cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="pt_2>30." --var="m_2(30,0,2)" --add_ztt_modes=true \
  --x_axis_label="m_{vis}^{#tau_{h}} [GeV]" --datacard="tau_modes" \
  --background_scheme="tau_modes" --draw_ratio=false --extra_pad=1.4 --use_htt_style=true \
  --draw_error_band=true --set_alias="inclusive:pt_2>30"  \
  --shift_backgrounds="ZTT-1P0PZ:0.966278,ZTT-1P1PZ:0.966278,ZTT-3P:0.966278,ZTT:0.966278,QCD:1.0258,W:1.03862,ZL:1.15101,ZJ:1.19543,VV:0.97629,TT:1.07285" --auto_error_band=0.0397582

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
  --method=8 --cat="inclusive" --var="n_jets(6,-0.5,5.5)"  --x_axis_label="Number of Jets" \
  --draw_ratio=true --log_y=true --extra_pad=10 --datacard="inclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=50


./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt \
  --method=8 --cat="inclusive" --set_alias="sel:1" --var="mt_1(40,0,160)" \
  --x_axis_label="m_{T} [GeV]" --datacard="inclusive" --extra_pad=1.2 $MT_BAND_ONLY \
  --background_scheme="mt_with_zmm" --draw_ratio=true

