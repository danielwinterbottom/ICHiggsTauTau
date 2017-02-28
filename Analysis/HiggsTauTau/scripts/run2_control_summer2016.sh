y_title="Events"
#channels=("em" "et" "mt" "tt" "zee" "zmm")
channels=("zee" "zmm")

for chan in "${channels[@]}"; do
  plots=("mt_tot(20,0,200)" "m_vis(20,0,200)" "met(20,0,100)" "met_phi(35,-3.5,3.5)" "pt_1[0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,110,120,130,140,150,200,300]" "pt_2[0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,110,120,130,140,150,200,300]" "pt_tt(30,0,300)" "eta_1(30,-3,3)" "eta_2(30,-3,3)" "phi_1(35,-3.5,3.5)" "phi_2(35,-3.5,3.5)" "m_2(20,0,2)" "n_jets(10,-0.5,9.5)" "jpt_1(25,0,100)" "jpt_2(25,0,100)" "jeta_1(30,-3,3)" "jeta_2(30,-3,3)" "jphi_1(35,-3.5,3.5)" "jphi_2(35,-3.5,3.5)" "beta_1(30,-3,3)" "beta_2(30,-3,3)" "bpt_1(25,0,100)" "bpt_2(25,0,100)" "n_bjets(5,-0.5,5.5)" "mt_1(20,0,160)" "n_vtx(40,0,40)" "tau_decay_mode_2(14,0,14)" "dphi(30,0,6)" "met_dphi_1(30,0,6)" "met_dphi_2(30,0,6)")

  if [ $chan == "tt" ]; then
    plots+=("tau_decay_mode_1(14,0,14)")
    method="--method=8"
  elif [ $chan == "et" -o $chan == "mt" ]; then
    plots+=("iso_1(20,0,0.2)")
    method="--method=12"
  elif [ $chan == "em" ]; then
    plots+=("iso_1(20,0,0.4)")
    plots+=("iso_2(20,0,0.4)")
    method="--method=15"
  fi
  for plot in "${plots[@]}"; do
    if [[ $plot == *"["* ]];then 
      x_title=$(echo $plot | cut -d"[" -f1)
    else
      x_title=$(echo $plot | cut -d"(" -f1)
    fi
    
    if [ $x_title == "phi_1" -o $x_title == "phi_2" -o $x_title == "met_phi" -o $x_title == "jphi_1" -o $x_title == "jphi_2" -o $x_title == "eta_1" -o $x_title == "eta_2" -o $x_title == "jeta_1" -o $x_title == "jeta_2" ]; then
      extra_pad="--extra_pad=0.6"
    else
      extra_pad=""
    fi
    
    sel="--sel=(mt_1<40)"
    if [ $chan == "et" ]; then
      sel="--sel=(mt_1<50)"
    fi
    if [ $chan == "tt" -o $chan == "em" -o $x_title == "mt_1" ]; then
      sel="--sel=(1)"
    fi
    set_alias=""
    if [ $x_title == "phi_1" -o $x_title == "phi_2" ]; then
      python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --ratio --y_title=$y_title --x_title=$x_title --var=$plot $method --channel=$chan $set_alias --outputfolder="control_plots" --norm_bins $extra_pad --era=mssmsummer16
      mv "control_plots/"$x_title"_inclusive_"$chan"_2016.pdf" "control_plots/"$x_title"_nomtcut_inclusive_"$chan"_2016.pdf"
    fi
    #if [ $x_title == "iso_1" ]; then
    #  set_alias="--set_alias=baseline:(mva_olddm_medium_2>0.5&&antiele_2&&antimu_2&&leptonveto==0)"
    #fi
    
    python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --ratio --y_title=$y_title --x_title=$x_title --var=$plot $sel $method --channel=$chan $set_alias --outputfolder="control_plots" --norm_bins $extra_pad --era=mssmsummer16
    
  done
done
  


