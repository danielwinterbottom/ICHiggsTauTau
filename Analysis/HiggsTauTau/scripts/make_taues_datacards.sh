
if (( "$#" != "2" ))
then
  echo "Usage: make_taues_datacards.sh [CAT] [MODE]"
  exit
fi

CAT=$1
MODE=$2

if [ "$CAT" = "inclusive" ] 
then     
    if [ "$MODE" = "1" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30."   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="inclusive"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="0,0.25,0.5,0.75,1"
        mv datacard_m_2_inclusive_mt_2015.root datacard_m_2_inclusive_mt_2015_Mode1.root
    fi
    if [ "$MODE" = "2" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30."   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="inclusive"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="1.25,1.5,1.75,2" --no_central=true
        mv datacard_m_2_inclusive_mt_2015.root datacard_m_2_inclusive_mt_2015_Mode2.root
    fi
    if [ "$MODE" = "3" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30."   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="inclusive"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="2.25,2.5,2.75,3" --no_central=true
        mv datacard_m_2_inclusive_mt_2015.root datacard_m_2_inclusive_mt_2015_Mode3.root
    fi
    if [ "$MODE" = "4" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30."   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="inclusive"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="3.25,3.5,3.75,4" --no_central=true
        mv datacard_m_2_inclusive_mt_2015.root datacard_m_2_inclusive_mt_2015_Mode4.root
    fi
    if [ "$MODE" = "5" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30."   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="inclusive"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="4.25,4.5,4.75,5,5.5,6" --no_central=true
        mv datacard_m_2_inclusive_mt_2015.root datacard_m_2_inclusive_mt_2015_Mode5.root
    fi
    if [ "$MODE" = "6" ]
    then
        hadd datacard_m_2_inclusive_mt_2015.root datacard_m_2_inclusive_mt_2015_Mode*.root
    fi
fi

if [ "$CAT" = "1prong1pi0" ] 
then     
    if [ "$MODE" = "1" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="0,0.25,0.5,0.75,1"
        mv datacard_m_2_1prong1pi0_mt_2015.root datacard_m_2_1prong1pi0_mt_2015_Mode1.root
    fi
    if [ "$MODE" = "2" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="1.25,1.5,1.75,2" --no_central=true
        mv datacard_m_2_1prong1pi0_mt_2015.root datacard_m_2_1prong1pi0_mt_2015_Mode2.root
    fi
    if [ "$MODE" = "3" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="2.25,2.5,2.75,3" --no_central=true
        mv datacard_m_2_1prong1pi0_mt_2015.root datacard_m_2_1prong1pi0_mt_2015_Mode3.root
    fi
    if [ "$MODE" = "4" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="3.25,3.5,3.75,4" --no_central=true
        mv datacard_m_2_1prong1pi0_mt_2015.root datacard_m_2_1prong1pi0_mt_2015_Mode4.root
    fi
    if [ "$MODE" = "5" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="4.25,4.5,4.75,5,5.5,6" --no_central=true
        mv datacard_m_2_1prong1pi0_mt_2015.root datacard_m_2_1prong1pi0_mt_2015_Mode5.root
    fi
    if [ "$MODE" = "6" ]
    then
        hadd datacard_m_2_1prong1pi0_mt_2015.root datacard_m_2_1prong1pi0_mt_2015_Mode*.root
    fi
fi

if [ "$CAT" = "1prong1pi0Pt20To30" ] 
then     
    if [ "$MODE" = "1" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>20 && pt_2<30"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt20To30"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="0,0.25,0.5,0.75,1"
        mv datacard_m_2_1prong1pi0Pt20To30_mt_2015.root datacard_m_2_1prong1pi0Pt20To30_mt_2015_Mode1.root
    fi
    if [ "$MODE" = "2" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>20 && pt_2<30"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt20To30"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="1.25,1.5,1.75,2" --no_central=true
        mv datacard_m_2_1prong1pi0Pt20To30_mt_2015.root datacard_m_2_1prong1pi0Pt20To30_mt_2015_Mode2.root
    fi
    if [ "$MODE" = "3" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>20 && pt_2<30"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt20To30"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="2.25,2.5,2.75,3" --no_central=true
        mv datacard_m_2_1prong1pi0Pt20To30_mt_2015.root datacard_m_2_1prong1pi0Pt20To30_mt_2015_Mode3.root
    fi
    if [ "$MODE" = "4" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>20 && pt_2<30"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt20To30"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="3.25,3.5,3.75,4" --no_central=true
        mv datacard_m_2_1prong1pi0Pt20To30_mt_2015.root datacard_m_2_1prong1pi0Pt20To30_mt_2015_Mode4.root
    fi
    if [ "$MODE" = "5" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>20 && pt_2<30"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt20To30"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="4.25,4.5,4.75,5,5.5,6" --no_central=true
        mv datacard_m_2_1prong1pi0Pt20To30_mt_2015.root datacard_m_2_1prong1pi0Pt20To30_mt_2015_Mode5.root
    fi
    if [ "$MODE" = "6" ]
    then
        hadd datacard_m_2_1prong1pi0Pt20To30_mt_2015.root datacard_m_2_1prong1pi0Pt20To30_mt_2015_Mode*.root
    fi
fi

if [ "$CAT" = "1prong1pi0Pt30To40" ] 
then     
    if [ "$MODE" = "1" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>30 && pt_2<40"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt30To40"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="0,0.25,0.5,0.75,1"
        mv datacard_m_2_1prong1pi0Pt30To40_mt_2015.root datacard_m_2_1prong1pi0Pt30To40_mt_2015_Mode1.root
    fi
    if [ "$MODE" = "2" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>30 && pt_2<40"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt30To40"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="1.25,1.5,1.75,2" --no_central=true
        mv datacard_m_2_1prong1pi0Pt30To40_mt_2015.root datacard_m_2_1prong1pi0Pt30To40_mt_2015_Mode2.root
    fi
    if [ "$MODE" = "3" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>30 && pt_2<40"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt30To40"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="2.25,2.5,2.75,3" --no_central=true
        mv datacard_m_2_1prong1pi0Pt30To40_mt_2015.root datacard_m_2_1prong1pi0Pt30To40_mt_2015_Mode3.root
    fi
    if [ "$MODE" = "4" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>30 && pt_2<40"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt30To40"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="3.25,3.5,3.75,4" --no_central=true
        mv datacard_m_2_1prong1pi0Pt30To40_mt_2015.root datacard_m_2_1prong1pi0Pt30To40_mt_2015_Mode4.root
    fi
    if [ "$MODE" = "5" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>30 && pt_2<40"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt30To40"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="4.25,4.5,4.75,5,5.5,6" --no_central=true
        mv datacard_m_2_1prong1pi0Pt30To40_mt_2015.root datacard_m_2_1prong1pi0Pt30To40_mt_2015_Mode5.root
    fi
    if [ "$MODE" = "6" ]
    then
        hadd datacard_m_2_1prong1pi0Pt30To40_mt_2015.root datacard_m_2_1prong1pi0Pt30To40_mt_2015_Mode*.root
    fi
fi

if [ "$CAT" = "1prong1pi0Pt40To50" ] 
then     
    if [ "$MODE" = "1" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>40 && pt_2<50"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt40To50"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="0,0.25,0.5,0.75,1"
        mv datacard_m_2_1prong1pi0Pt40To50_mt_2015.root datacard_m_2_1prong1pi0Pt40To50_mt_2015_Mode1.root
    fi
    if [ "$MODE" = "2" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>40 && pt_2<50"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt40To50"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="1.25,1.5,1.75,2" --no_central=true
        mv datacard_m_2_1prong1pi0Pt40To50_mt_2015.root datacard_m_2_1prong1pi0Pt40To50_mt_2015_Mode2.root
    fi
    if [ "$MODE" = "3" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>40 && pt_2<50"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt40To50"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="2.25,2.5,2.75,3" --no_central=true
        mv datacard_m_2_1prong1pi0Pt40To50_mt_2015.root datacard_m_2_1prong1pi0Pt40To50_mt_2015_Mode3.root
    fi
    if [ "$MODE" = "4" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>40 && pt_2<50"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt40To50"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="3.25,3.5,3.75,4" --no_central=true
        mv datacard_m_2_1prong1pi0Pt40To50_mt_2015.root datacard_m_2_1prong1pi0Pt40To50_mt_2015_Mode4.root
    fi
    if [ "$MODE" = "5" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>40 && pt_2<50"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt40To50"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="4.25,4.5,4.75,5,5.5,6" --no_central=true
        mv datacard_m_2_1prong1pi0Pt40To50_mt_2015.root datacard_m_2_1prong1pi0Pt40To50_mt_2015_Mode5.root
    fi
    if [ "$MODE" = "6" ]
    then
        hadd datacard_m_2_1prong1pi0Pt40To50_mt_2015.root datacard_m_2_1prong1pi0Pt40To50_mt_2015_Mode*.root
    fi
fi

if [ "$CAT" = "1prong1pi0Pt50To60" ] 
then     
    if [ "$MODE" = "1" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>50 && pt_2<60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt50To60"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="0,0.25,0.5,0.75,1"
        mv datacard_m_2_1prong1pi0Pt50To60_mt_2015.root datacard_m_2_1prong1pi0Pt50To60_mt_2015_Mode1.root
    fi
    if [ "$MODE" = "2" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>50 && pt_2<60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt50To60"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="1.25,1.5,1.75,2" --no_central=true
        mv datacard_m_2_1prong1pi0Pt50To60_mt_2015.root datacard_m_2_1prong1pi0Pt50To60_mt_2015_Mode2.root
    fi
    if [ "$MODE" = "3" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>50 && pt_2<60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt50To60"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="2.25,2.5,2.75,3" --no_central=true
        mv datacard_m_2_1prong1pi0Pt50To60_mt_2015.root datacard_m_2_1prong1pi0Pt50To60_mt_2015_Mode3.root
    fi
    if [ "$MODE" = "4" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>50 && pt_2<60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt50To60"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="3.25,3.5,3.75,4" --no_central=true
        mv datacard_m_2_1prong1pi0Pt50To60_mt_2015.root datacard_m_2_1prong1pi0Pt50To60_mt_2015_Mode4.root
    fi
    if [ "$MODE" = "5" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>50 && pt_2<60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt50To60"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="4.25,4.5,4.75,5,5.5,6" --no_central=true
        mv datacard_m_2_1prong1pi0Pt50To60_mt_2015.root datacard_m_2_1prong1pi0Pt50To60_mt_2015_Mode5.root
    fi
    if [ "$MODE" = "6" ]
    then
        hadd datacard_m_2_1prong1pi0Pt50To60_mt_2015.root datacard_m_2_1prong1pi0Pt50To60_mt_2015_Mode*.root
    fi
fi

if [ "$CAT" = "1prong1pi0Pt60ToInf" ] 
then     
    if [ "$MODE" = "1" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt60ToInf"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="0,0.25,0.5,0.75,1"
        mv datacard_m_2_1prong1pi0Pt60ToInf_mt_2015.root datacard_m_2_1prong1pi0Pt60ToInf_mt_2015_Mode1.root
    fi
    if [ "$MODE" = "2" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt60ToInf"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="1.25,1.5,1.75,2" --no_central=true
        mv datacard_m_2_1prong1pi0Pt60ToInf_mt_2015.root datacard_m_2_1prong1pi0Pt60ToInf_mt_2015_Mode2.root
    fi
    if [ "$MODE" = "3" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt60ToInf"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="2.25,2.5,2.75,3" --no_central=true
        mv datacard_m_2_1prong1pi0Pt60ToInf_mt_2015.root datacard_m_2_1prong1pi0Pt60ToInf_mt_2015_Mode3.root
    fi
    if [ "$MODE" = "4" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt60ToInf"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="3.25,3.5,3.75,4" --no_central=true
        mv datacard_m_2_1prong1pi0Pt60ToInf_mt_2015.root datacard_m_2_1prong1pi0Pt60ToInf_mt_2015_Mode4.root
    fi
    if [ "$MODE" = "5" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==1 && pt_2>60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="1prong1pi0Pt60ToInf"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="4.25,4.5,4.75,5,5.5,6" --no_central=true
        mv datacard_m_2_1prong1pi0Pt60ToInf_mt_2015.root datacard_m_2_1prong1pi0Pt60ToInf_mt_2015_Mode5.root
    fi
    if [ "$MODE" = "6" ]
    then
        hadd datacard_m_2_1prong1pi0Pt60ToInf_mt_2015.root datacard_m_2_1prong1pi0Pt60ToInf_mt_2015_Mode*.root
    fi
fi

if [ "$CAT" = "3prong" ] 
then     
    if [ "$MODE" = "1" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prong"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="0,0.25,0.5,0.75,1"
        mv datacard_m_2_3prong_mt_2015.root datacard_m_2_3prong_mt_2015_Mode1.root
    fi
    if [ "$MODE" = "2" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prong"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="1.25,1.5,1.75,2" --no_central=true
        mv datacard_m_2_3prong_mt_2015.root datacard_m_2_3prong_mt_2015_Mode2.root
    fi
    if [ "$MODE" = "3" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prong"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="2.25,2.5,2.75,3" --no_central=true
        mv datacard_m_2_3prong_mt_2015.root datacard_m_2_3prong_mt_2015_Mode3.root
    fi
    if [ "$MODE" = "4" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prong"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="3.25,3.5,3.75,4" --no_central=true
        mv datacard_m_2_3prong_mt_2015.root datacard_m_2_3prong_mt_2015_Mode4.root
    fi
    if [ "$MODE" = "5" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prong"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="4.25,4.5,4.75,5,5.5,6" --no_central=true
        mv datacard_m_2_3prong_mt_2015.root datacard_m_2_3prong_mt_2015_Mode5.root
    fi
    if [ "$MODE" = "6" ]
    then
        hadd datacard_m_2_3prong_mt_2015.root datacard_m_2_3prong_mt_2015_Mode*.root
    fi
fi

if [ "$CAT" = "3prongPt20To30" ] 
then     
    if [ "$MODE" = "1" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>20 && pt_2<30"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt20To30"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="0,0.25,0.5,0.75,1"
        mv datacard_m_2_3prongPt20To30_mt_2015.root datacard_m_2_3prongPt20To30_mt_2015_Mode1.root
    fi
    if [ "$MODE" = "2" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>20 && pt_2<30"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt20To30"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="1.25,1.5,1.75,2" --no_central=true
        mv datacard_m_2_3prongPt20To30_mt_2015.root datacard_m_2_3prongPt20To30_mt_2015_Mode2.root
    fi
    if [ "$MODE" = "3" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>20 && pt_2<30"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt20To30"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="2.25,2.5,2.75,3" --no_central=true
        mv datacard_m_2_3prongPt20To30_mt_2015.root datacard_m_2_3prongPt20To30_mt_2015_Mode3.root
    fi
    if [ "$MODE" = "4" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>20 && pt_2<30"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt20To30"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="3.25,3.5,3.75,4" --no_central=true
        mv datacard_m_2_3prongPt20To30_mt_2015.root datacard_m_2_3prongPt20To30_mt_2015_Mode4.root
    fi
    if [ "$MODE" = "5" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>20 && pt_2<30"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt20To30"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="4.25,4.5,4.75,5,5.5,6" --no_central=true
        mv datacard_m_2_3prongPt20To30_mt_2015.root datacard_m_2_3prongPt20To30_mt_2015_Mode5.root
    fi
    if [ "$MODE" = "6" ]
    then
        hadd datacard_m_2_3prongPt20To30_mt_2015.root datacard_m_2_3prongPt20To30_mt_2015_Mode*.root
    fi
fi

if [ "$CAT" = "3prongPt30To40" ] 
then     
    if [ "$MODE" = "1" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>30 && pt_2<40"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt30To40"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="0,0.25,0.5,0.75,1"
        mv datacard_m_2_3prongPt30To40_mt_2015.root datacard_m_2_3prongPt30To40_mt_2015_Mode1.root
    fi
    if [ "$MODE" = "2" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>30 && pt_2<40"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt30To40"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="1.25,1.5,1.75,2" --no_central=true
        mv datacard_m_2_3prongPt30To40_mt_2015.root datacard_m_2_3prongPt30To40_mt_2015_Mode2.root
    fi
    if [ "$MODE" = "3" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>30 && pt_2<40"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt30To40"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="2.25,2.5,2.75,3" --no_central=true
        mv datacard_m_2_3prongPt30To40_mt_2015.root datacard_m_2_3prongPt30To40_mt_2015_Mode3.root
    fi
    if [ "$MODE" = "4" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>30 && pt_2<40"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt30To40"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="3.25,3.5,3.75,4" --no_central=true
        mv datacard_m_2_3prongPt30To40_mt_2015.root datacard_m_2_3prongPt30To40_mt_2015_Mode4.root
    fi
    if [ "$MODE" = "5" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>30 && pt_2<40"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt30To40"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="4.25,4.5,4.75,5,5.5,6" --no_central=true
        mv datacard_m_2_3prongPt30To40_mt_2015.root datacard_m_2_3prongPt30To40_mt_2015_Mode5.root
    fi
    if [ "$MODE" = "6" ]
    then
        hadd datacard_m_2_3prongPt30To40_mt_2015.root datacard_m_2_3prongPt30To40_mt_2015_Mode*.root
    fi
fi

if [ "$CAT" = "3prongPt40To50" ] 
then     
    if [ "$MODE" = "1" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>40 && pt_2<50"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt40To50"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="0,0.25,0.5,0.75,1"
        mv datacard_m_2_3prongPt40To50_mt_2015.root datacard_m_2_3prongPt40To50_mt_2015_Mode1.root
    fi
    if [ "$MODE" = "2" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>40 && pt_2<50"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt40To50"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="1.25,1.5,1.75,2" --no_central=true
        mv datacard_m_2_3prongPt40To50_mt_2015.root datacard_m_2_3prongPt40To50_mt_2015_Mode2.root
    fi
    if [ "$MODE" = "3" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>40 && pt_2<50"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt40To50"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="2.25,2.5,2.75,3" --no_central=true
        mv datacard_m_2_3prongPt40To50_mt_2015.root datacard_m_2_3prongPt40To50_mt_2015_Mode3.root
    fi
    if [ "$MODE" = "4" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>40 && pt_2<50"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt40To50"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="3.25,3.5,3.75,4" --no_central=true
        mv datacard_m_2_3prongPt40To50_mt_2015.root datacard_m_2_3prongPt40To50_mt_2015_Mode4.root
    fi
    if [ "$MODE" = "5" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>40 && pt_2<50"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt40To50"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="4.25,4.5,4.75,5,5.5,6" --no_central=true
        mv datacard_m_2_3prongPt40To50_mt_2015.root datacard_m_2_3prongPt40To50_mt_2015_Mode5.root
    fi
    if [ "$MODE" = "6" ]
    then
        hadd datacard_m_2_3prongPt40To50_mt_2015.root datacard_m_2_3prongPt40To50_mt_2015_Mode*.root
    fi
fi

if [ "$CAT" = "3prongPt50To60" ] 
then     
    if [ "$MODE" = "1" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>50 && pt_2<60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt50To60"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="0,0.25,0.5,0.75,1"
        mv datacard_m_2_3prongPt50To60_mt_2015.root datacard_m_2_3prongPt50To60_mt_2015_Mode1.root
    fi
    if [ "$MODE" = "2" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>50 && pt_2<60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt50To60"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="1.25,1.5,1.75,2" --no_central=true
        mv datacard_m_2_3prongPt50To60_mt_2015.root datacard_m_2_3prongPt50To60_mt_2015_Mode2.root
    fi
    if [ "$MODE" = "3" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>50 && pt_2<60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt50To60"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="2.25,2.5,2.75,3" --no_central=true
        mv datacard_m_2_3prongPt50To60_mt_2015.root datacard_m_2_3prongPt50To60_mt_2015_Mode3.root
    fi
    if [ "$MODE" = "4" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>50 && pt_2<60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt50To60"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="3.25,3.5,3.75,4" --no_central=true
        mv datacard_m_2_3prongPt50To60_mt_2015.root datacard_m_2_3prongPt50To60_mt_2015_Mode4.root
    fi
    if [ "$MODE" = "5" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>50 && pt_2<60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt50To60"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="4.25,4.5,4.75,5,5.5,6" --no_central=true
        mv datacard_m_2_3prongPt50To60_mt_2015.root datacard_m_2_3prongPt50To60_mt_2015_Mode5.root
    fi
    if [ "$MODE" = "6" ]
    then
        hadd datacard_m_2_3prongPt50To60_mt_2015.root datacard_m_2_3prongPt50To60_mt_2015_Mode*.root
    fi
fi

if [ "$CAT" = "3prongPt60ToInf" ] 
then     
    if [ "$MODE" = "1" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt60ToInf"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="0,0.25,0.5,0.75,1"
        mv datacard_m_2_3prongPt60ToInf_mt_2015.root datacard_m_2_3prongPt60ToInf_mt_2015_Mode1.root
    fi
    if [ "$MODE" = "2" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt60ToInf"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="1.25,1.5,1.75,2" --no_central=true
        mv datacard_m_2_3prongPt60ToInf_mt_2015.root datacard_m_2_3prongPt60ToInf_mt_2015_Mode2.root
    fi
    if [ "$MODE" = "3" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt60ToInf"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="2.25,2.5,2.75,3" --no_central=true
        mv datacard_m_2_3prongPt60ToInf_mt_2015.root datacard_m_2_3prongPt60ToInf_mt_2015_Mode3.root
    fi
    if [ "$MODE" = "4" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt60ToInf"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="3.25,3.5,3.75,4" --no_central=true
        mv datacard_m_2_3prongPt60ToInf_mt_2015.root datacard_m_2_3prongPt60ToInf_mt_2015_Mode4.root
    fi
    if [ "$MODE" = "5" ]
    then
        ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_mssm_2015.cfg --channel=mt --set_alias="sel:mt_1<30. && tau_decay_mode_2==10 && pt_2>60"   --method=8 --cat="inclusive" --var="m_2(200,0,2)" --extra_pad=0.2  --x_axis_label="Tau Mass [GeV]" --datacard="3prongPt60ToInf"   --background_scheme="mt_with_zmm" $MT_INC_SHIFT --draw_error_band=true --auto_error_band=0.00001 --folder=/srv/localstage/amd12/Feb12-Fall15SF/ --tau_es_scales="4.25,4.5,4.75,5,5.5,6" --no_central=true
        mv datacard_m_2_3prongPt60ToInf_mt_2015.root datacard_m_2_3prongPt60ToInf_mt_2015_Mode5.root
    fi
    if [ "$MODE" = "6" ]
    then
        hadd datacard_m_2_3prongPt60ToInf_mt_2015.root datacard_m_2_3prongPt60ToInf_mt_2015_Mode*.root
    fi
fi

if [ "$CAT" = "HADD" ] 
then     
    hadd -f datacard_m_2_mt_2015_combined.root datacard_m_2_inclusive_mt_2015.root datacard_m_2_1prong1pi0*_mt_2015.root datacard_m_2_3prong*_mt_2015.root
fi    
