cats=("inclusive" "btag" "nobtag" "0jet" "1jet" "ge2jet" "0bjet" "1bjet" "ge2bjet")
channels=("zmm" "zee")
plots=("pt_tt[0,20,40,60,80,100,120,140,160,200,240,280,320,400,600]" "pt_1(20,0,200)" "pt_2[0,10,20,30,40,50,60,70,80,90,100,120,140,200]" "n_jets(5,0,5)" "n_bjets(5,0,5)")
weights=("wt_zpt_down" "wt_zpt_njets" "wt_zpt_njets_normxbins" "1")
#weights=("1")

for chan in "${channels[@]}"; do for cat in "${cats[@]}"; do for plot in "${plots[@]}"; do for weight in "${weights[@]}"; do
  if [[ $plot == *"["* ]];then 
    var=$(echo $plot | cut -d"[" -f1)
  else
    var=$(echo $plot | cut -d"(" -f1)
  fi
 
  python scripts/HiggsTauTauPlot.py --channel=$chan --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --var=$plot --method=8 --cat=$cat --add_wt=$weight --outputfolder="ZptReWeightPlots/" --ratio --x_title=$var --norm_bins
  if [ "$weight" == "wt_zpt_down" ]; then
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016.pdf" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_nozptwt.pdf"
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016.png" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_nozptwt.png"
  elif [ "$weight" == "wt_zpt_njets" ]; then
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016.pdf" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_njetwt.pdf"
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016.png" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_njetwt.png"
  elif [ "$weight" == "wt_zpt_njets_normxbins" ]; then
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016.pdf" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_njetwt_normxbin.pdf"
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016.png" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_njetwt_normxbin.png"
  elif [ "$weight" == "1" ]; then
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016.pdf" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_njetwt_incwt.pdf"
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016.png" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_njetwt_incwt.png"
  fi

done; done; done; done;
