channels=("mt" "et" "tt" "em" "zmm" "zee")
weights=("wt_zpt_down" "1")
systs=(" " "JES")
#systs=(" ")
#--set_alias="baseline:({baseline}&&pt_1>40&&pt_2>40)"
for chan in "${channels[@]}"; do
  cats=("inclusive" "nobtag" "btag" "0jet" "1jet" "ge2jet")

for cat in "${cats[@]}"; do for weight in "${weights[@]}"; do
  blind_string=""
  if [ $cat != "inclusive" ]; then if [ $chan == "et" -o $chan == "mt" -o $chan == "tt" -o $chan == "em" ]; then
    blind_string="--blind"
  fi; fi
  method="8"
  sel=""
  if [ $chan == "zmm" -o $chan == "zee" ]; then
    sel='--sel=(m_vis>50)'
  elif [ $chan == "em" ]; then
    method="15"
  elif [ $chan == "et" -o $chan == "mt" ]; then
    sel='--sel=(mt_1<70)'
    if [ $cat == "btag" ]; then
      method="16"
    else
      method="12"
    fi
  fi
  
  if [[ $cat == *"bjet"* ]] || [[ $cat == "btag"* ]] ;then
    plots=("mt_tot[0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]" "m_vis[50,80,90,100,120,160,200,400,800,2000]" "pt_tt[0,10,20,30,40,60,80,100,120,160,200,280,320,400,600]" "n_jets(5,0,5)" "n_lowpt_jets(5,0,5)" "n_bjets(5,0,5)")
  else
    plots=("mt_tot[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]" "m_vis[0,50,80,90,100,120,160,200,400,800]" "pt_tt[0,10,20,30,40,60,80,100,120,160,200,280,320,400,600]" "n_jets(5,0,5)" "n_lowpt_jets(5,0,5)" "n_bjets(5,0,5)")
  fi
  for plot in "${plots[@]}"; do
    if [[ $plot == *"["* ]];then 
      var=$(echo $plot | cut -d"[" -f1)
    else
      var=$(echo $plot | cut -d"(" -f1)
    fi
    
    log_string="--log_y --log_x"
    log_name_string="logx_logy"
    if [ $var == "n_jets" -o $var == "n_lowpt_jets" -o $var == "n_bjets" ]; then
      log_string=" --log_y"
      log_name_string="logy"
    fi
    
    for syst in "${systs[@]}"; do
      
      syst_string=""
      syst_name=""
      if [ "$syst" == "JES" ]; then
        syst_string=" --do_custom_uncerts  --uncert_title="JES-systematic" --custom_uncerts_up_name=total_bkg_JESUp --custom_uncerts_down_name=total_bkg_JESDown --syst_scale_j=JES" 
        syst_name="_JES"
      fi
      
      python scripts/HiggsTauTauPlot.py --channel=$chan --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --var=$plot --method=8 --cat=$cat --add_wt=$weight --outputfolder="ZptReWeightPlots/" --ratio --x_title=$var --norm_bins --extra_pad=0.5 $log_string $syst_string $blind_string --y_title="Entries/(bin width)" $sel --set_alias="baseline:({baseline}&&pt_1>40&&pt_2>40)"
      if [ "$weight" == "wt_zpt_down" ]; then
        mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_"$log_name_string".pdf" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_nozptwt_"$log_name_string$syst_name".pdf"
        mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_"$log_name_string".png" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_nozptwt_"$log_name_string$syst_name".png"
      elif [ "$weight" == "1" ]; then
        mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_"$log_name_string".pdf" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_incwt_"$log_name_string$syst_name".pdf"
        mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_"$log_name_string".png" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_incwt_"$log_name_string$syst_name".png"
      fi
    done

  done; 
done; done; done;
