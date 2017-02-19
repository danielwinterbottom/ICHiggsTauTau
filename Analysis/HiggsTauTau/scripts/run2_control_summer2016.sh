y_title="Events"
channels=("et" "mt" "tt")

for chan in "${channels[@]}"; do
  plots=("mt_tot(20,0,200)" "m_vis(20,0,200)" "met(20,0,100)" "met_phi(35,-3.5,3.5)" "pt_1(25,0,100)" "pt_2(25,0,100)" "pt_tt(30,0,300)" "eta_1(30,-3,3)" "eta_2(30,-3,3)" "phi_1(35,-3.5,3.5)" "phi_2(35,-3.5,3.5)" "m_2(20,0,2)" "n_jets(10,-0.5,9.5)" "jpt_1(25,0,100)" "jpt_2(25,0,100)" "jeta_1(30,-3,3)" "jeta_2(30,-3,3)" "beta_1(30,-3,3)" "beta_2(30,-3,3)" "bpt_1(25,0,100)" "bpt_2(25,0,100)" "n_bjets(10,-0.5,9.5)" "mt_1(20,0,160)" "n_vtx(30,0,30)" "tau_decay_mode_2(14,0,14)")
  if [ $chan == "tt" ]; then
    plots+=("tau_decay_mode_1(14,0,14)")
    method="--method=12"
  elif [ $chan == "et" -o $chan == "mt" ]; then
    plots+=("iso_1(20,0,0.4)")
    method="--method=12"
  fi
  for plot in "${plots[@]}"; do
    x_title=$(echo $plot | cut -d"(" -f1)
    sel="--sel=(mt_1<50)"
    if [ $chan == "tt" -o $x_title == "mt_1" ]; then
      sel="--sel=(1)"
    fi
    set_alias=""
    if [ $x_title == "iso_1" ]; then
      set_alias="--set_alias=baseline:(mva_olddm_medium_2>0.5&&antiele_2&&antimu_2&&!leptonveto)"
    fi
    
    python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --ratio --y_title=$y_title --x_title=$x_title --var=$plot $sel $method --channel=$chan $set_alias --outputfolder="control_plots_newbeff" --folder="/vols/cms/dw515/Offline/output/Jan24_newbeff/"
    
  done
done
  


