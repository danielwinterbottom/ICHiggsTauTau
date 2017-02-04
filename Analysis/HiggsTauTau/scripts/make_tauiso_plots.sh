#!/bin/bash

channels=("et" "mt")
cats=("inclusive" "nobtag" "nobtag_loose" "nobtag_tight" "btag" "btag_loose" "btag_tight" "nobtag_loose_wjets_cr" "nobtag_loose_wjets_ss_cr" "nobtag_loose_qcd_cr" "nobtag_tight_wjets_cr" "nobtag_tight_wjets_ss_cr" "nobtag_tight_qcd_cr")
#cats=("inclusive")

for ch in "${channels[@]}"; do for ca in "${cats[@]}"; do

  if [ $ch == "et" ]; then
    bkg_sch="et_default"
  else
    bkg_sch="mt_with_zmm"
  fi
  
  method=12
  var="mt_tot"["0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500"]
  if [ $ca == "btag" -o $ca == "btag_loose" -o $ca == "btag_tight" ]; then
    method=16
    var="mt_tot"["0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500"]
  fi
  
  if [ $ch == "et" ]; then
      if [ $ca == "inclusive" ];then
        alias="--set_alias=\"sel:mt_1<60\""
      elif [ "$ca" == "nobtag" ];then
        alias="--set_alias=\"sel:mt_1<50\"  --set_alias=\"baseline:(iso_1<0.1  && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && !leptonveto)\""
      elif [ $ca == "nobtag_loose" ];then
        alias="--set_alias=\"sel:mt_1<60\""
      elif [ $ca == "nobtag_tight" ]; then
        alias="--set_alias=\"sel:mt_1<30\""
      elif [ $ca == "btag" ]; then
        alias="--set_alias=\"sel:mt_1<50\" --set_alias=\"baseline:(iso_1<0.1  && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && !leptonveto)\""
      elif [ $ca == "btag_loose" ]; then 
        alias="--set_alias=\"sel:mt_1<60\" --set_alias=\"btag_wnobtag:(n_jets<=1 && n_lowpt_jets>=1 && (mt_1>30 || mva_olddm_tight_2<0.5))\""
      elif [ $ca == "btag_tight" ]; then
        alias="--set_alias=\"sel:mt_1<30\" --set_alias=\"btag_wnobtag:(n_jets<=1 && n_lowpt_jets>=1 && mva_olddm_tight_2>0.5)\""
      elif [ $ca == "nobtag_loose_wjets_cr" ]; then
        alias="--set_alias=\"sel:mt_1>70\""
      elif [ $ca == "nobtag_loose_wjets_ss_cr" ]; then
        alias="--set_alias=\"sel:mt_1>70\" --do_ss=true"
      elif [ $ca == "nobtag_loose_qcd_cr" ]; then
        alias="--set_alias=\"sel:mt_1<30\"  --do_ss=true"
      elif [ $ca == "nobtag_tight_wjets_cr" ]; then
        alias="--set_alias=\"sel:mt_1>70\" --set_alias=\"btag_wnobtag:(n_jets<=1 && n_lowpt_jets>=1 && mva_olddm_tight_2>0.5)\""
      elif [ $ca == "nobtag_tight_wjets_ss_cr" ]; then
        alias="--set_alias=\"sel:mt_1>70\"  --do_ss=true --set_alias=\"btag_wnobtag:(n_jets<=1 && n_lowpt_jets>=1 && mva_olddm_tight_2>0.5)\""  
      elif [ $ca == "nobtag_tight_qcd_cr" ]; then
        alias="--set_alias=\"sel:mt_1<30\" --do_ss=true --set_alias=\"btag_wnobtag:(n_jets<=1 && n_lowpt_jets>=1 && mva_olddm_tight_2>0.5)"\"
      fi
  else
      if [ $ca == "inclusive" ];then
        alias="--set_alias=\"sel:mt_1<70\""
      elif [ $ca == "nobtag" ];then
        alias="--set_alias=\"sel:mt_1<40\"  --set_alias=\"baseline:(iso_1<0.15  && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && !leptonveto)\""
      elif [ $ca == "nobtag_loose" ];then
        alias="--set_alias=\"sel:mt_1<70\"" 
      elif [ $ca == "nobtag_tight" ]; then
        alias="--set_alias=\"sel:mt_1<40\""
      elif [ $ca == "btag" ]; then
        alias="--set_alias=\"sel:mt_1<40\" --set_alias=\"baseline:(iso_1<0.15  && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && !leptonveto)\""
      elif [ $ca == "btag_loose" ]; then 
        alias="--set_alias=\"sel:mt_1<70\" --set_alias=\"btag_wnobtag:(n_jets<=1 && n_lowpt_jets>=1 && (mt_1>40  || mva_olddm_tight_2<0.5))\""
      elif [ $ca == "btag_tight" ]; then
        alias="--set_alias=\"sel:mt_1<40\" --set_alias=\"btag_wnobtag:(n_jets<=1 && n_lowpt_jets>=1 && mva_olddm_tight_2>0.5)\""
          elif [ $ca == "nobtag_loose_wjets_cr" ]; then
        alias="--set_alias=\"sel:mt_1>70\""
      elif [ $ca == "nobtag_loose_wjets_ss_cr" ]; then
        alias="--set_alias=\"sel:mt_1>70\" --do_ss=true"
      elif [ $ca == "nobtag_loose_qcd_cr" ]; then
        alias="--set_alias=\"sel:mt_1<40\"  --do_ss=true"
      elif [ $ca == "nobtag_tight_wjets_cr" ]; then
        alias="--set_alias=\"sel:mt_1>70\" --set_alias=\"btag_wnobtag:(n_jets<=1 && n_lowpt_jets>=1 && mva_olddm_tight_2>0.5)\""
      elif [ $ca == "nobtag_tight_wjets_ss_cr" ]; then
        alias="--set_alias=\"sel:mt_1>70\"  --do_ss=true --set_alias=\"btag_wnobtag:(n_jets<=1 && n_lowpt_jets>=1 && mva_olddm_tight_2>0.5)\""  
      elif [ $ca == "nobtag_tight_qcd_cr" ]; then
        alias="--set_alias=\"sel:mt_1<40\" --do_ss=true --set_alias=\"btag_wnobtag:(n_jets<=1 && n_lowpt_jets>=1 && mva_olddm_tight_2>0.5)"\"
      fi
  fi
  
  cat=$ca
  blind="true"
  if [ $cat == "inclusive" ]; then
    blind="false"
  fi
  if [[ $cat == *"_cr"* ]]; then
    blind="false"
    if [[ $cat == "nobtag_loose"* ]]; then
      cat="nobtag_loose"
    elif [[ $cat == "nobtag_tight"* ]]; then
      cat="nobtag_tight"
    elif [[ $cat == "btag_loose"* ]]; then
      cat="btag_loose"
    elif [[ $cat == "btag_tight"* ]]; then
      cat="btag_tight"
    fi
  fi
  
  ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2016.cfg --channel=$ch $alias   --method=12 --var=$var --cat=$cat   --x_axis_label="mt_tot_{#tau#tau} [GeV]"  --norm_bins=true --datacard=$ca --log_y=false --draw_ratio=true   --custom_y_axis_min=true --y_axis_min=0.0099   --blind=$blind --x_blind_min=100 --x_blind_max=3200   --background_scheme=$bkg_sch --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001
  
  #./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2016.cfg --channel=mt --set_alias="sel:mt_1<70"   --method=12 --var="mt_tot"["0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500"] --cat="inclusive"   --x_axis_label="M_{#tau#tau} [GeV]"  --norm_bins=true --datacard="inclusive" --log_y=false --draw_ratio=true   --custom_y_axis_min=true --y_axis_min=0.0099   --blind=false --x_blind_min=100 --x_blind_max=3200   --background_scheme="mt_with_zmm" --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001
  
  #./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2016.cfg --channel=et --set_alias="sel:mt_1<60" --method=12 --var=mt_tot[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500] --cat=inclusive --x_axis_label=mt_tot_{#tau#tau} [GeV] --norm_bins=true --datacard=inclusive --log_y=false --draw_ratio=true --custom_y_axis_min=true --y_axis_min=0.0099 --blind=false --x_blind_min=100 --x_blind_max=3200 --background_scheme=et_default --extra_pad=0.3 --draw_error_band=true --auto_error_band=0.00001



done; done
