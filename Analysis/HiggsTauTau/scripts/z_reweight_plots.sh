out_dir=$1
channels=("zmm" "zee")
weights=("wt_zpt_down" "1")
systs=("" "JES")
for chan in "${channels[@]}"; do
  cats=("inclusive" "nobtag" "btag" "0jet" "1jet" "ge2jet")

for cat in "${cats[@]}"; do for weight in "${weights[@]}"; do
  
  if [[ $cat == *"bjet"* ]] || [[ $cat == "btag"* ]] ;then
    plots=("mt_tot[0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]" "m_vis[50,80,90,100,120,160,200,400,800,2000]" "pt_tt[0,10,20,30,40,60,80,100,120,160,200,280,320,400,600]" "n_jets(5,0,5)" "n_bjets(5,0,5)")
  else
    plots=("mt_tot[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]" "m_vis[0,50,80,90,100,120,160,200,400,800]" "pt_tt[0,10,20,30,40,60,80,100,120,160,200,280,320,400,600]" "n_jets(5,0,5)" "n_bjets(5,0,5)")
  fi
  for plot in "${plots[@]}"; do
    if [[ $plot == *"["* ]];then 
      var=$(echo $plot | cut -d"[" -f1)
    else
      var=$(echo $plot | cut -d"(" -f1)
    fi
    if [[ $var == "n_jets" ]] && [[ $cat == "0jet" || $cat == "1jet" || $cat == "ge2jet" ]]; then continue; fi 
    
    log_string="--log_y --log_x"
    log_name_string="logx_logy"
    if [ $var == "n_jets" -o $var == "n_lowpt_jets" -o $var == "n_bjets" ]; then
      log_string=" --log_y"
      log_name_string="logy"
    fi
    
    for syst in "${systs[@]}"; do
    
      uncert_title="Background uncertainty"
      syst_string="  --add_flat_uncert=0.04 --add_stat_to_syst "
      syst_name=""
      if [ "$syst" == "JES" ]; then
        uncert_title="JES uncertainty"
        syst_string=' --do_custom_uncerts  --custom_uncerts_up_name=total_bkg_JESUp --custom_uncerts_down_name=total_bkg_JESDown --syst_scale_j=JES --add_stat_to_syst'
        syst_name="_JES"
      fi
      
      python scripts/HiggsTauTauPlot.py --channel=$chan --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --var=$plot --method=8 --cat=$cat --add_wt=$weight --outputfolder=$out_dir --ratio  --norm_bins --extra_pad=0.5 $log_string $syst_string --sel="(m_vis>50)" --folder=/vols/cms/dw515/Offline/output/MSSM/gen_studies/ --uncert_title="$uncert_title" --set_alias="baseline:({baseline}&&pt_1>40&&pt_2>40)"
      if [ "$weight" == "wt_zpt_down" ]; then
        mv $out_dir/$var"_"$cat"_"$chan"_2016_"$log_name_string".pdf" $out_dir/$var"_"$cat"_"$chan"_2016_nozptwt_"$log_name_string$syst_name".pdf"
        mv $out_dir/$var"_"$cat"_"$chan"_2016_"$log_name_string".png" $out_dir/$var"_"$cat"_"$chan"_2016_nozptwt_"$log_name_string$syst_name".png"
      elif [ "$weight" == "1" ]; then
        mv $out_dir/$var"_"$cat"_"$chan"_2016_"$log_name_string".pdf" $out_dir/$var"_"$cat"_"$chan"_2016_incwt_"$log_name_string$syst_name".pdf"
        mv $out_dir/$var"_"$cat"_"$chan"_2016_"$log_name_string".png" $out_dir/$var"_"$cat"_"$chan"_2016_incwt_"$log_name_string$syst_name".png"
      fi
      rm $out_dir"/*.png"
      rm $out_dir"/*.root"
    done

  done; 
done; done; done;
