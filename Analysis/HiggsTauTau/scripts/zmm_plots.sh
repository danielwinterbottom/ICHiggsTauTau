cats=("inclusive" "btag" "0jet" "1jet" "ge2jet" "0bjet" "1bjet" "ge2bjet" "0lowptjet" "1lowptjet" "ge2lowptjet")
channels=("zmm" "zee")
plots=("pt_tt(20,0,200)" "pt_1(10,0,100)" "pt_2(10,0,100)" "n_jets(5,0,5)" "n_bjets(5,0,5)")
weights=("wt_zpt_down" "")

for chan in "${channels[@]}"; do for cat in "${cats[@]}"; do for plot in "${plots[@]}"; do for weight in "${weights[@]}"; do
  var=$(echo $plot | cut -d"(" -f1)
  
  python scripts/HiggsTauTauPlot.py --channel=$chan --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --var=$plot --method=8  --sel="(m_vis>80&&m_vis<120)" --cat=$cat --add_wt=$weight --outputfolder="ZptReWeightPlots/" --ratio --x_title=$var --y_title="Entries"
  if [ "$weight" == "wt_zpt_down" ]; then
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016.pdf" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_nozptwt.pdf"
    mv ZptReWeightPlots/$var"_"$cat"_"$chan"_2016.png" ZptReWeightPlots/$var"_"$cat"_"$chan"_2016_nozptwt.png"
  fi

done; done; done; done;
