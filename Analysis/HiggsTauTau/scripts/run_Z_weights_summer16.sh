#cats=("inclusive" "0jet" "1jet" "ge2jet")
cats=("inclusive")
for cat in "${cats[@]}"; do
  if [ $cat == "0jet" ]; then
    bins1="0,10,20,30,40,60,80,600"
    bins2="0,10,20,30,40,60,80,600" 
    bins3="0,10,20,30,40,60,80,600" 
    bins4="0,10,20,30,40,60,600" 
    bins5="0,10,20,30,40,60,600"
    bins6="0,10,20,40,600"
    bins7="0,40,600"
    bins8="0,40,600"
  elif [ $cat == "1jet" ]; then
    bins1="0,10,20,30,40,60,80,120,160,200,600"
    bins2="0,10,20,30,40,60,80,100,120,160,200,280,600"
    bins3="0,10,20,30,40,60,80,100,120,160,200,280,600"
    bins4="0,10,20,30,40,60,80,100,120,160,600"
    bins5="0,10,20,30,40,60,80,120,160,600"
    bins6="0,10,20,30,40,60,80,120,600"
    bins7="0,40,80,120,200,600"
    bins8="0,40,80,600"
  elif [ $cat == "ge2jet" ]; then
    bins1="0,10,20,30,40,60,80,120,160,200,400,600"
    bins2="0,10,20,30,40,60,80,100,120,160,200,280,600"
    bins3="0,10,20,30,40,60,80,100,120,160,200,280,400,600"
    bins4="0,10,20,30,40,60,80,100,120,160,200,600"
    bins5="0,20,40,80,120,160,600"
    bins6="0,20,40,80,120,600"
    bins7="0,120,200,600"
    bins8="0,120,600"
  elif [ $cat == "inclusive" ]; then
    bins1="0,10,20,30,40,60,80,120,160,200,600"
    bins2="0,10,20,30,40,60,80,100,120,160,200,280,320,400,600"
    bins3="0,10,20,30,40,60,80,100,120,160,200,280,320,400,600"
    bins4="0,10,20,30,40,60,80,100,120,160,200,600"
    bins5="0,10,20,30,40,60,80,120,160,600"
    bins6="0,10,20,30,40,60,80,120,200,600"
    bins7="0,40,80,120,600"
    bins8="0,40,80,600"
  fi

  python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --channel=zmm --set_alias="sel:(m_vis>50 && m_vis<80)"  --method=8 --cat="$cat" --var="pt_tt[$bins1]"  --x_title="Z pt [GeV]" --y_title="Entries" --datacard="$cat" --extra_pad=0.5 --log_y --outputfolder="./" --ratio --add_wt=wt_zpt_down  #--syst_tau_scale=ES
  mv datacard_pt_tt_$cat"_zmm_2016".root datacard_pt_tt_$cat"_zmm_2016_mvis50to80.root"
  mv pt_tt_$cat"_zmm_2016_logy.png" pt_tt_$cat"_zmm_2016_logy_mvis50to80.png"
  mv pt_tt_$cat"_zmm_2016_logy.pdf" pt_tt_$cat"_zmm_2016_logy_mvis50to80.pdf"
  
  python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --channel=zmm --set_alias="sel:(m_vis>80 && m_vis<90)"  --method=8 --cat="$cat" --var="pt_tt[$bins2]"  --x_title="Z pt [GeV]" --y_title="Entries" --datacard="$cat" --extra_pad=0.5 --log_y --outputfolder="./" --ratio --add_wt=wt_zpt_down #--syst_tau_scale=ES
  mv datacard_pt_tt_$cat"_zmm_2016".root datacard_pt_tt_$cat"_zmm_2016_mvis80to90.root"
  mv pt_tt_$cat"_zmm_2016_logy.png" pt_tt_$cat"_zmm_2016_logy_mvis80to90.png"
  mv pt_tt_$cat"_zmm_2016_logy.pdf" pt_tt_$cat"_zmm_2016_logy_mvis80to90.pdf"
  
  python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --channel=zmm --set_alias="sel:(m_vis>90 && m_vis<100)"  --method=8 --cat="$cat" --var="pt_tt[$bins3]"  --x_title="Z pt [GeV]" --y_title="Entries" --datacard="$cat" --extra_pad=0.5 --log_y --outputfolder="./" --ratio --add_wt=wt_zpt_down  #--syst_tau_scale=ES
  mv datacard_pt_tt_$cat"_zmm_2016".root datacard_pt_tt_$cat"_zmm_2016_mvis90to100.root"
  mv pt_tt_$cat"_zmm_2016_logy.png" pt_tt_$cat"_zmm_2016_logy_mvis90to100.png"
  mv pt_tt_$cat"_zmm_2016_logy.pdf" pt_tt_$cat"_zmm_2016_logy_mvis90to100.pdf"
  
  python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --channel=zmm --set_alias="sel:(m_vis>100 && m_vis<120)"  --method=8 --cat="$cat" --var="pt_tt[$bins4]"  --x_title="Z pt [GeV]" --y_title="Entries" --datacard="$cat" --extra_pad=0.5 --log_y --outputfolder="./" --ratio --add_wt=wt_zpt_down  #--syst_tau_scale=ES
  mv datacard_pt_tt_$cat"_zmm_2016".root datacard_pt_tt_$cat"_zmm_2016_mvis100to120.root"
  mv pt_tt_$cat"_zmm_2016_logy.png" pt_tt_$cat"_zmm_2016_logy_mvis100to120.png"
  mv pt_tt_$cat"_zmm_2016_logy.pdf" pt_tt_$cat"_zmm_2016_logy_mvis100to120.pdf"
  
  python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --channel=zmm --set_alias="sel:(m_vis>120 && m_vis<160)"  --method=8 --cat="$cat" --var="pt_tt[$bins5]"  --x_title="Z pt [GeV]" --y_title="Entries" --datacard="$cat" --extra_pad=0.5 --log_y --outputfolder="./" --ratio --add_wt=wt_zpt_down #--syst_tau_scale=ES
  mv datacard_pt_tt_$cat"_zmm_2016.root" datacard_pt_tt_$cat"_zmm_2016_mvis120to160.root"
  mv pt_tt_$cat"_zmm_2016_logy.png" pt_tt_$cat"_zmm_2016_logy_mvis120to160.png"
  mv pt_tt_$cat"_zmm_2016_logy.pdf" pt_tt_$cat"_zmm_2016_logy_mvis120to160.pdf"
  
  python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --channel=zmm --set_alias="sel:(m_vis>160 && m_vis<200)"  --method=8 --cat="$cat" --var="pt_tt[$bins6]"  --x_title="Z pt [GeV]" --y_title="Entries" --datacard="$cat" --extra_pad=0.5 --log_y --outputfolder="./" --ratio --add_wt=wt_zpt_down #--syst_tau_scale=ES
  mv datacard_pt_tt_$cat"_zmm_2016.root" datacard_pt_tt_$cat"_zmm_2016_mvis160to200.root"
  mv pt_tt_$cat"_zmm_2016_logy.png" pt_tt_$cat"_zmm_2016_logy_mvis160to200.png"
  mv pt_tt_$cat"_zmm_2016_logy.pdf" pt_tt_$cat"_zmm_2016_logy_mvis160to200.pdf"
  
  python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --channel=zmm --set_alias="sel:(m_vis>200 && m_vis<400)"  --method=8 --cat="$cat" --var="pt_tt[$bins7]"  --x_title="Z pt [GeV]" --y_title="Entries" --datacard="$cat" --extra_pad=0.5 --log_y --outputfolder="./" --ratio --add_wt=wt_zpt_down #--syst_tau_scale=ES
  mv datacard_pt_tt_$cat"_zmm_2016.root" datacard_pt_tt_$cat"_zmm_2016_mvis200to400.root"
  mv pt_tt_$cat"_zmm_2016_logy.png" pt_tt_$cat"_zmm_2016_logy_mvis200to400.png"
  mv pt_tt_$cat"_zmm_2016_logy.pdf" pt_tt_$cat"_zmm_2016_logy_mvis200to400.pdf"
  
  python scripts/HiggsTauTauPlot.py --cfg=scripts/new_plot_mssm_2016_NewPlotting.cfg --channel=zmm --set_alias="sel:(m_vis>400)"  --method=8 --cat="$cat" --var="pt_tt[$bins8]"  --x_title="Z pt [GeV]" --y_title="Entries" --datacard="$cat" --extra_pad=0.5 --log_y --outputfolder="./" --ratio --add_wt=wt_zpt_down #--syst_tau_scale=ES
  mv datacard_pt_tt_$cat"_zmm_2016.root" datacard_pt_tt_$cat"_zmm_2016_mvis400toinf.root"
  mv pt_tt_$cat"_zmm_2016_logy.png" pt_tt_$cat"_zmm_2016_logy_mvis400toinf.png"
  mv pt_tt_$cat"_zmm_2016_logy.pdf" pt_tt_$cat"_zmm_2016_logy_mvis400toinf.pdf"

done;
