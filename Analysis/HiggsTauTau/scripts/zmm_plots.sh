cats=("inclusive" "btag" "nobtag" "0jet" "1jet" "1bjet")
channels=("zmm" "zee")
plots=("pt_tt[0,20,40,60,80,100,120,140,160,200,240,280,320,400,600]" "pt_1(20,0,200)" "pt_2[0,10,20,30,40,50,60,70,80,90,100,120,140,200]" "n_jets(5,0,5)" "n_bjets(5,0,5)" "m_vis[50,60,70,80,85,90,95,100,110,120,140,200,400]")
#plots=("m_vis[50,60,70,80,85,90,95,100,110,120,140,200,400]")
weights=("wt_zpt_down" "wt_zpt_njets" "wt_zpt_njets_normxbins" "1")
#weights=("wt_zpt_njets")

for chan in "${channels[@]}"; do for cat in "${cats[@]}"; do for plot in "${plots[@]}"; do for weight in "${weights[@]}"; do
  if [[ $plot == *"["* ]];then 
    var=$(echo $plot | cut -d"[" -f1)
  else
    var=$(echo $plot | cut -d"(" -f1)
  fi
 
  python scripts/HiggsTauTauPlot.py --channel=$chan --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --var=$plot --method=8 --cat=$cat --add_wt=$weight --outputfolder="ZptReWeightPlots/" --ratio --x_title=$var --norm_bins --extra_pad=0.5 --log_y
  if [ "$weight" == "wt_zpt_down" ]; then
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_logy.pdf" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_nozptwt_logy.pdf"
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_logy.png" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_nozptwt_logy.png"
  elif [ "$weight" == "wt_zpt_njets" ]; then
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_logy.pdf" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_njetwt_logy.pdf"
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_logy.png" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_njetwt_logy.png"
  elif [ "$weight" == "wt_zpt_njets_normxbins" ]; then
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_logy.pdf" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_njetwt_normxbin_logy.pdf"
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_logy.png" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_njetwt_normxbin_logy.png"
  elif [ "$weight" == "1" ]; then
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_logy.pdf" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_incwt_logy.pdf"
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_logy.png" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_incwt_logy.png"
  fi

done; done; done; done;
