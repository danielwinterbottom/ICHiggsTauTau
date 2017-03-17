y_title="Events"
channels=("em" "et" "mt" "tt" "zee" "zmm")
#channels=("zee" "zmm")
OSSS=("ss" "os")
CRs=("nom" "w_cr")
for chan in "${channels[@]}"; do
  if [ $chan == "et" -o $chan == "mt" ]; then
    cats=("inclusive" "nobtag_looseiso" "nobtag_loosemt" "btag_tight"  "btag_looseiso" "btag_loosemt" "btag_tight")
  else
    cats=("inclusive" "nobtag" "btag")
  fi
  if [ $chan == "zmm" -o $chan == "zee" ]; then
    cats+=("atleast1bjet")
  fi
  plots=("m_vis(20,0,200)" "met(20,0,100)" "met_phi(35,-3.5,3.5)" "pt_1[0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,110,120,130,140,150,200,300]" "pt_2[0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,110,120,130,140,150,200,300]" "pt_tt(30,0,300)" "eta_1(30,-3,3)" "eta_2(30,-3,3)" "phi_1(35,-3.5,3.5)" "phi_2(35,-3.5,3.5)" "m_2(20,0,2)" "n_jets(10,-0.5,9.5)" "jpt_1(25,0,100)" "jpt_2(25,0,100)" "jeta_1(50,-5,5)" "jeta_2(50,-5,5)" "jphi_1(35,-3.5,3.5)" "jphi_2(35,-3.5,3.5)" "n_bjets(5,-0.5,5.5)" "mt_1(20,0,160)" "n_vtx(40,0,40)" "tau_decay_mode_2(14,0,14)" "dphi(30,0,6)" "met_dphi_1(30,0,6)" "met_dphi_2(30,0,6)")
  for cat in "${cats[@]}"; do
    if [ $chan == "tt" ]; then
      plots+=("tau_decay_mode_1(14,0,14)")
      methods=("8")
    elif [ $chan == "et" -o $chan == "mt" ]; then
      plots+=("iso_1(20,0,0.2)")
      methods=("12" "8")
      if [[ $cat == "btag"* ]]; then
        methods=("12" "8" "16")      
      fi
    elif [ $chan == "em" ]; then
      plots+=("iso_1(20,0,0.2)")
      plots+=("iso_2(20,0,0.2)")
      methods=("15")
    elif [ $chan == "zmm" -o $chan == "zee" ]; then
      plots+=("iso_1(20,0,0.4)")
      plots+=("iso_2(20,0,0.4)")
      methods=("8")
    fi
    for k in "${methods[@]}"; do for plot in "${plots[@]}"; do
      method="--method="$k
      if [[ $plot == *"["* ]];then 
        x_title=$(echo $plot | cut -d"[" -f1)
      else
        x_title=$(echo $plot | cut -d"(" -f1)
      fi
      
      if [ $x_title == "phi_1" -o $x_title == "phi_2" -o $x_title == "met_phi" -o $x_title == "jphi_1" -o $x_title == "jphi_2" -o $x_title == "eta_1" -o $x_title == "eta_2" -o $x_title == "jeta_1" -o $x_title == "jeta_2" ]; then
        extra_pad="--extra_pad=0.6"
      else
        extra_pad="--extra_pad=0.2"
      fi
      
      sel="--sel=(1)"
      if [ $chan == "et" ]; then
        sel="--sel=(mt_1<70)"
      elif [ $chan == "zee" -o $chan == "zmm" ]; then
        sel="--sel=(m_vis>60&&m_vis<120)"
      fi
      for q in "${OSSS[@]}"; do for p in "${CRs[@]}"; do
        set_alias=""
        add_name=""
        if [ $p == "w_cr" ]; then
          set_alias+=" --set_alias=sel:(mt_1>70)"
          add_name+="_wcr"
        fi
        if [ $q == "ss" ]; then
          ss_string="--do_ss"
          add_name+="_ss"
        else
          ss_string=""
          add_name+="_os"
        fi
        
        if [ $x_title == "jpt_1" ]; then
          set_alias+=" --set_alias=baseline:({baseline} && jeta_1<3&&jeta_1>-3)" #this is wrong!!
          python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --ratio --y_title=$y_title --x_title=$x_title --var=$plot $sel $method --channel=$chan $set_alias --outputfolder="control_plots" --norm_bins $extra_pad --era=mssmsummer16 $ss_string
          mv "control_plots/jpt_1_"$cat"_"$chan"_2016.pdf" "control_plots/jpt_1_central_m"$k"_"$cat$add_name"_"$chan"_2016.pdf"
          mv "control_plots/jpt_1_"$cat"_"$chan"_2016.pbg" "control_plots/jpt_1_central_m"$k"_"$cat$add_name"_"$chan"_2016.png"
          
          set_alias+=" --set_alias=baseline:({baseline} && (jeta_1>3||jeta_1<-3))"
          python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --ratio --y_title=$y_title --x_title=$x_title --var=$plot $sel $method --channel=$chan $set_alias --outputfolder="control_plots" --norm_bins $extra_pad --era=mssmsummer16  $ss_string
          mv "control_plots/jpt_1_"$cat"_"$chan"_2016.pdf" "control_plots/jpt_1_forward_m"$k"_"$cat$add_name"_"$chan"_2016.pdf"
          mv "control_plots/jpt_1_"$cat"_"$chan"_2016.png" "control_plots/jpt_1_forward_m"$k"_"$cat$add_name"_"$chan"_2016.png"
        fi
        
        if [ $x_title == "jpt_2" ]; then
          set_alias+=" --set_alias=baseline:({baseline} && jeta_2<3&&jeta_2>-3)"
          python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --ratio --y_title=$y_title --x_title=$x_title --var=$plot $sel $method --channel=$chan $set_alias --outputfolder="control_plots" --norm_bins $extra_pad --era=mssmsummer16 $ss_string
          mv "control_plots/jpt_2_"$cat"_"$chan"_2016.pdf" "control_plots/jpt_2_central_m"$k"_"$cat$add_name"_"$chan"_2016.pdf"
          mv "control_plots/jpt_2_"$cat"_"$chan"_2016.png" "control_plots/jpt_2_central_m"$k"_"$cat$add_name"_"$chan"_2016.png"
          
          set_alias+=" --set_alias=baseline:({baseline} && (jeta_2>3||jeta_2<-3))"
          python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --ratio --y_title=$y_title --x_title=$x_title --var=$plot $sel $method --channel=$chan $set_alias --outputfolder="control_plots" --norm_bins $extra_pad --era=mssmsummer16 $ss_string
          mv "control_plots/jpt_2_"$cat"_"$chan"_2016.pdf" "control_plots/jpt_2_forward_m"$k"_"$cat$add_name"_"$chan"_2016.pdf"
          mv "control_plots/jpt_2_"$cat"_"$chan"_2016.png" "control_plots/jpt_2_forward_m"$k"_"$cat$add_name"_"$chan"_2016.png"
        fi
        
        python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --ratio --y_title=$y_title --x_title=$x_title --var=$plot $sel $method --channel=$chan $set_alias --outputfolder="control_plots" --norm_bins $extra_pad --era=mssmsummer16 $ss_string
        mv  "control_plots/"$x_title"_"$cat"_"$chan"_2016.pdf" "control_plots/"$x_title"_m"$k"_"$cat$add_name"_"$chan"_2016.pdf" 
        mv  "control_plots/"$x_title"_"$cat"_"$chan"_2016.png" "control_plots/"$x_title"_m"$k"_"$cat$add_name"_"$chan"_2016.png"
    done; done
    done; done
  done
done
  


