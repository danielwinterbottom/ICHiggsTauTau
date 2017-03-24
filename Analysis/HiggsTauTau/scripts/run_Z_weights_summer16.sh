cats=("inclusive" "0jet" "1jet" "ge2jet")
for cat in "${cats[@]}"; do
  if [ $cat == "0jet" ]; then
    bins1="0,40,80,600"
    bins2="0,40,80,120,600"
    bins3="0,40,80,600"
    bins4="0,120,600"
  elif [ $cat == "1jet" ]; then
    bins1="0,40,80,120,160,200,600"
    bins2="0,40,80,120,160,200,280,320,600"
    bins3="0,40,80,120,160,200,600"
    bins4="0,120,200,600"
  elif [ $cat == "ge2jet" ]; then
    bins1="0,40,80,120,160,200,280,600"
    bins2="0,40,80,120,160,200,280,400,600"
    bins3="0,40,80,120,160,200,600"
    bins4="0,120,200,600"
  fi
  
  python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --channel=zmm --set_alias="sel:(m_vis>50 && m_vis<80)"  --method=8 --cat="$cat" --var="pt_tt[$bins1]"  --x_title="Z pt [GeV]" --y_title="Entries" --datacard="$cat" --extra_pad=0.5 --log_y --outputfolder="./" --ratio --add_wt=wt_zpt_down --syst_scale_j=JES
  mv datacard_pt_tt_$cat"_zmm_2016".root datacard_pt_tt_$cat"_zmm_2016_mvis50to80.root"
  mv pt_tt_$cat"_zmm_2016_logy.png" pt_tt_$cat"_zmm_2016_logy_mvis50to80.png"
  mv pt_tt_$cat"_zmm_2016_logy.pdf" pt_tt_$cat"_zmm_2016_logy_mvis50to80.pdf"
  
  python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --channel=zmm --set_alias="sel:(m_vis>80 && m_vis<120)"  --method=8 --cat="$cat" --var="pt_tt[$bins2]"  --x_title="Z pt [GeV]" --y_title="Entries" --datacard="$cat" --extra_pad=0.5 --log_y --outputfolder="./" --ratio --add_wt=wt_zpt_down --syst_scale_j=JES
  mv datacard_pt_tt_$cat"_zmm_2016.root" datacard_pt_tt_$cat"_zmm_2016_mvis80to120.root"
  mv pt_tt_$cat"_zmm_2016_logy.png" pt_tt_$cat"_zmm_2016_logy_mvis80to120.png"
  mv pt_tt_$cat"_zmm_2016_logy.pdf" pt_tt_$cat"_zmm_2016_logy_mvis80to120.pdf"
  
  python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --channel=zmm --set_alias="sel:(m_vis>120 && m_vis<200)"  --method=8 --cat="$cat" --var="pt_tt[$bins3]"  --x_title="Z pt [GeV]" --y_title="Entries" --datacard="$cat" --extra_pad=0.5 --log_y --outputfolder="./" --ratio --add_wt=wt_zpt_down --syst_scale_j=JES
  mv datacard_pt_tt_$cat"_zmm_2016.root" datacard_pt_tt_$cat"_zmm_2016_mvis120to200.root"
  mv pt_tt_$cat"_zmm_2016_logy.png" pt_tt_$cat"_zmm_2016_logy_mvis120to200.png"
  mv pt_tt_$cat"_zmm_2016_logy.pdf" pt_tt_$cat"_zmm_2016_logy_mvis120to200.pdf"
  
  python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --channel=zmm --set_alias="sel:(m_vis>200 && m_vis<800)"  --method=8 --cat="$cat" --var="pt_tt[$bins4]"  --x_title="Z pt [GeV]" --y_title="Entries" --datacard="$cat" --extra_pad=0.5 --log_y --outputfolder="./" --ratio --add_wt=wt_zpt_down --syst_scale_j=JES
  mv datacard_pt_tt_$cat"_zmm_2016.root" datacard_pt_tt_$cat"_zmm_2016_mvis200to800.root"
  mv pt_tt_$cat"_zmm_2016_logy.png" pt_tt_$cat"_zmm_2016_logy_mvis200to800.png"
  mv pt_tt_$cat"_zmm_2016_logy.pdf" pt_tt_$cat"_zmm_2016_logy_mvis200to800.pdf"

done;
