./bin/DataCompare  	--file2="output/Moriond_2012_NewJEC/VBF_HToTauTau_M-125_mt_2012.root"  \
--file1="output/Moriond_2012/VBF_HToTauTau_M-125_mt_2012.root" \
--plot_folder="inclusive_os_sel/" --plot="jeta_1" --lumi1=-1 --lumi2=-1 --label2="NewJEC" --label1="CurrentJEC" \
--x_axis_title="Leading Jet #eta" --norm_mode=4  --channel=mt \
--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150

./bin/DataCompare  	--file2="output/Moriond_2012_NewJEC/VBF_HToTauTau_M-125_mt_2012.root"  \
--file1="output/Moriond_2012/VBF_HToTauTau_M-125_mt_2012.root" \
--plot_folder="inclusive_os_sel/" --plot="jpt_1" --lumi1=-1 --lumi2=-1 --label2="NewJEC" --label1="CurrentJEC" \
--x_axis_title="Leading Jet p_{T} [GeV]" --norm_mode=4  --channel=mt \
--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2

./bin/PrintPulls	 --input1=et_pulls_mvis.txt --input2=et_pulls_svfit.txt \
--splusb_1=0 --splusb_2=0 --bbb_treatment=1 --name1="et-mvis" --name2="et-svfit"

./bin/PrintPulls	 --input1=mt_pulls_mvis.txt --input2=mt_pulls_svfit.txt \
--splusb_1=0 --splusb_2=0 --bbb_treatment=1 --name1="mt-mvis" --name2="mt-svfit"

./bin/PrintPulls	 --input1==mlfit.txt --input2=mlfit.txt \
--splusb_1=0 --splusb_2=1 --bbb_treatment=1 --name1="tt" --name2="tt"

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=0 --channel=mt --rebin=1  \
  --method=2 --plot_name="m_sv_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
  --paramfile=./scripts/Moriond_params_2012.dat --log_y=true

  ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=0 --channel=et --rebin=1  \
  --method=2 --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
  --paramfile=./scripts/Moriond_params_2012.dat --log_y=true




./bin/DataCompare  	--file2="output/Moriond_2012/SUSYGluGluToHToTauTau_M-90_mt_2012.root"  \
--file1="htt_mt.inputs-sm-8TeV-lowsignal.root" \
--plot_folder="muTau_inclusive/" --plot="ggH90" \
--plot_folder2="inclusive_os_sel/" --plot2="m_sv_sm_fine" \
--lumi1=-1 --lumi2=-1 --label2="MSSM-ggH90" --label1="Extrap-ggH90" \
--x_axis_title="Mass" --norm_mode=3  --channel=mt \
--title="Compare ggH90 extrapolated to MSSM ggH90 in inclusive" \
--outname="ggH110_inclusive_extrap_MSSM.pdf"


./bin/DataCompare  	--file2="output/Moriond_2012/SUSYGluGluToHToTauTau_M-90_mt_2012.root"  \
--file1="htt_mt.inputs-sm-8TeV-lowsignal.root" \
--plot_folder="muTau_boost_high/" --plot="ggH90" \
--plot_folder2="1jet_high_os_sel/" --plot2="m_sv_sm_fine" \
--lumi1=-1 --lumi2=-1 --label2="MSSM-ggH90" --label1="Extrap-ggH90" \
--x_axis_title="Mass" --norm_mode=3  --channel=mt \
--title="Compare ggH90 extrapolated to MSSM ggH90 in boost_high" \
--outname="ggH110_boost_high_extrap_MSSM.pdf"


./bin/DataCompare  	--file2="output/Moriond_2012/SUSYGluGluToHToTauTau_M-110_mt_2012.root"  \
--file1="htt_mt.inputs-sm-8TeV-lowsignal.root" \
--plot_folder="muTau_inclusive/" --plot="ggH110" \
--plot_folder2="inclusive_os_sel/" --plot2="m_sv_sm_fine" \
--lumi1=-1 --lumi2=-1 --label2="MSSM-ggH110" --label1="SM-ggH110" \
--x_axis_title="Mass" --norm_mode=3  --channel=mt \
--title="Compare ggH110 inclusive in SM and MSSM samples" \
--outname="ggH110_inclusive_SM_MSSM.pdf"

./bin/DataCompare  	--file2="output/Moriond_2012/SUSYGluGluToHToTauTau_M-110_mt_2012.root"  \
--file1="htt_mt.inputs-sm-8TeV-lowsignal.root" \
--plot_folder="muTau_boost_high/" --plot="ggH110" \
--plot_folder2="1jet_high_os_sel/" --plot2="m_sv_sm_fine" \
--lumi1=-1 --lumi2=-1 --label2="MSSM-ggH110" --label1="SM-ggH110" \
--x_axis_title="Mass" --norm_mode=3  --channel=mt \
--title="Compare ggH110 boost_high in SM and MSSM samples" \
--outname="ggH110_boost_high_SM_MSSM.pdf"


./bin/DataCompare  \
--file1="htt_mt.inputs-sm-8TeV-lowsignal.root" --label1="SM-qqH110" \
--plot_folder="muTau_vbf/" --plot="qqH110" \
--file2="output/Moriond_2012/SUSYGluGluToHToTauTau_M-110_mt_2012.root"  \
--plot_folder2="vbf_os_sel/" --plot2="m_sv_sm" --label2="MSSM-ggH110" \
--lumi1=-1 --lumi2=-1 \
--x_axis_title="Mass" --norm_mode=3  --channel=mt \
--title="Compare qqH110 vbf SM to ggH110 vbf MSSM" \
--outname="qqH110_vbf_SM_MSSM.pdf"

./bin/DataCompare  	--file2="output/Moriond_2012/SUSYGluGluToHToTauTau_M-110_mt_2012.root"  \
--file1="htt_mt.inputs-sm-8TeV-lowsignal.root" \
--plot_folder="muTau_boost_high/" --plot="ggH110" \
--plot_folder2="1jet_high_os_sel/" --plot2="m_sv_sm_fine" \
--lumi1=-1 --lumi2=-1 --label2="MSSM-ggH110" --label1="SM-ggH110" \
--x_axis_title="Mass" --norm_mode=3  --channel=mt \
--title="Compare ggH110 boost_high in SM and MSSM samples" \
--outname="ggH110_boost_high_SM_MSSM.pdf"

    ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2011.cfg  --tau_scale_mode=0 --channel=et --rebin=1  \
      --method=3 --plot_name="m_sv_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
      --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=true \
      --paramfile=./scripts/default_params_2011.dat




./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=0 --channel=mt --rebin=1 \
--method=5 --category="vbf_tight" --plot_name="m_sv_sm"  --x_axis_label="m_{#tau#tau} [GeV]" \
--blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
--paramfile=scripts/Moriond_params_2012.dat --folder=output/Moriond_2012_BigTScale

./bin/DatacardCompare --inputs collected/Imperial/htt_mt.inputs-sm-8TeV-moriond-andrew.root --show_errs=true \
--labels VBF --category muTau_vbf --output_file=yields_moriond_vbf_mt-sm-8TeV.pdf


./bin/DataCompare  \
--file1="htt_mt.inputs-sm-8TeV-lowsignal.root" \
--plot_folder="muTau_boost_high/" --plot="ggH90" \
--file2="htt_mt.inputs-sm-8TeV-lowsignal-real.root"  \
--plot_folder2="muTau_boost_high/" --plot2="ggH90" \
--lumi1=-1 --lumi2=-1 --label2="Sim" --label1="Extrap" \
--x_axis_title="Mass" --norm_mode=3  --channel=mt \
--title="Compare ggH90 shape: extrapolation vs simulation" \
--outname="ggH90_mutau_boost_high.pdf"

./bin/DataCompare  \
--file1="htt_et.inputs-sm-8TeV-lowsignal.root" \
--plot_folder="eleTau_boost_high/" --plot="ggH90" \
--file2="htt_et.inputs-sm-8TeV-lowsignal-real.root"  \
--plot_folder2="eleTau_boost_high/" --plot2="ggH90" \
--lumi1=-1 --lumi2=-1 --label2="Sim" --label1="Extrap" \
--x_axis_title="Mass" --norm_mode=3  --channel=et \
--title="Compare ggH90 shape: extrapolation vs simulation" \
--outname="ggH90_eletau_boost_high.pdf"

./bin/DataCompare  \
--file1="htt_mt.inputs-sm-8TeV-lowsignal.root" \
--plot_folder="muTau_vbf/" --plot="ggH90" \
--file2="htt_mt.inputs-sm-8TeV-lowsignal-real.root"  \
--plot_folder2="muTau_vbf/" --plot2="ggH90" \
--lumi1=-1 --lumi2=-1 --label2="Sim" --label1="Extrap" \
--x_axis_title="Mass" --norm_mode=3  --channel=mt \
--title="Compare ggH90 shape: extrapolation vs simulation" \
--outname="ggH90_mutau_vbf.pdf"

./bin/DataCompare  \
--file1="htt_et.inputs-sm-8TeV-lowsignal.root" \
--plot_folder="eleTau_vbf/" --plot="ggH90" \
--file2="htt_et.inputs-sm-8TeV-lowsignal-real.root"  \
--plot_folder2="eleTau_vbf/" --plot2="ggH90" \
--lumi1=-1 --lumi2=-1 --label2="Sim" --label1="Extrap" \
--x_axis_title="Mass" --norm_mode=3  --channel=et \
--title="Compare ggH90 shape: extrapolation vs simulation" \
--outname="ggH90_eletau_vbf.pdf"



./bin/DataCompare   \
--file1="output/Moriond_2012_NewTScale/GluGluToHToTauTau_M-110_mt_2012.root" \
--plot_folder="1jet_os_sel/" --plot="pt_1" \
--file2="output/Moriond_2012_NewTScale/GluGluToHToTauTau_M-90_mt_2012.root"  \
--plot_folder2="1jet_os_sel/" --plot2="pt_1" \
--lumi1=-1 --lumi2=-1 --label2="m_{H} = 90" --label1="m_{H} = 110" \
--x_axis_title="Muon p_{T} [GeV]" --norm_mode=3  --channel=mt \
--title="" \
--outname="pt_1_90_110.pdf" --rebin=2

./bin/DataCompare   \
--file1="output/Moriond_2012_NewTScale/GluGluToHToTauTau_M-110_mt_2012.root" \
--plot_folder="1jet_os_sel/" --plot="pt_2" \
--file2="output/Moriond_2012_NewTScale/GluGluToHToTauTau_M-90_mt_2012.root"  \
--plot_folder2="1jet_os_sel/" --plot2="pt_2" \
--lumi1=-1 --lumi2=-1 --label2="m_{H} = 90" --label1="m_{H} = 110" \
--x_axis_title="Tau p_{T} [GeV]" --norm_mode=3  --channel=mt \
--title="" \
--outname="pt_2_90_110.pdf" --rebin=2

./bin/DataCompare   \
--file1="output/Moriond_2012_NewTScale/GluGluToHToTauTau_M-130_mt_2012.root" \
--plot_folder="1jet_os_sel/" --plot="pt_2" \
--file2="output/Moriond_2012_NewTScale/GluGluToHToTauTau_M-110_mt_2012.root"  \
--plot_folder2="1jet_os_sel/" --plot2="pt_2" \
--lumi1=-1 --lumi2=-1 --label2="m_{H} = 110" --label1="m_{H} = 130" \
--x_axis_title="Tau p_{T} [GeV]" --norm_mode=3  --channel=mt \
--title="" \
--outname="pt_2_110_130.pdf" --rebin=2


SCALE_MT_INC="--shift_backgrounds=true --draw_band_on_stack=true --band_size_fractional=0.029575 --ztt_shift=0.960251 \
              --qcd_shift=1.133200 --top_shift=0.714139 --zl_shift=1.059042 --zj_shift=1.462669 --w_shift=0.918000 --vv_shift=0.680721 \
              --shift_tscale=true --tscale_shift=-0.3"

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="m_2"  --x_axis_label="Tau Mass [GeV]" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --folder=output/TauPOG_AllModes/ \
  --paramfile=./scripts/Moriond_params_2012.dat --extra_pad=1.1 #$SCALE_MT_INC


  PLOT="m_vis_sm"
  ./bin/PlotCompare  \
  -p "vbf:Full VBF:output/Paper_2013_3Hit/Special_3_Data_mtmet_2012.root:vbf_ss_sel/:m_vis_sm:-1:0:1" \
  -p "vbf_loose_jets20:Loose VBF, Jets 20:output/Paper_2013_3Hit/Special_3_Data_mtmet_2012.root:vbf_loose_jets20_ss_sel/:m_vis_sm:-1:0:2" \
  -p "vbf_no_cjv:Full VBF, No CJV:output/Paper_2013_3Hit/Special_3_Data_mtmet_2012.root:vbf_no_cjv_ss_sel/:m_vis_sm:-1:0:4" \
  --x_axis_title="Visible Mass [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --outname="qcd_vbf_comp_mtmet_$PLOT.pdf" --ratios="" --rebin=1


./bin/PlotCompare  \
-p "0jet_low:0jet_low:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:0jet_low_os_sel/:pt_tt:-1:0:1" \
-p "0jet_high:0jet_high:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:0jet_high_os_sel/:pt_tt:-1:0:2" \
-p "1jet_low:1jet_low:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:1jet_low_os_sel/:pt_tt:-1:0:3" \
-p "1jet_high:1jet_high:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:1jet_high_os_sel/:pt_tt:-1:0:4" \
-p "inclusive:inclusive:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:inclusive_os_sel/:pt_tt:-1:0:6" \
--x_axis_title="Simple Higgs p_{T} [GeV]" --norm_mode=0  --big_label="#mu#tau_{h}" \
--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
--outname="ggh_125_pt_tt.pdf" --rebin=2

./bin/PlotCompare  \
-p "0jet_low:0jet_low:output/Paper_2013_3Hit/Embedded_mt_2012.root:0jet_low_os_sel/:pt_tt:-1:0:1" \
-p "0jet_high:0jet_high:output/Paper_2013_3Hit/Embedded_mt_2012.root:0jet_high_os_sel/:pt_tt:-1:0:2" \
-p "1jet_low:1jet_low:output/Paper_2013_3Hit/Embedded_mt_2012.root:1jet_low_os_sel/:pt_tt:-1:0:3" \
-p "1jet_high:1jet_high:output/Paper_2013_3Hit/Embedded_mt_2012.root:1jet_high_os_sel/:pt_tt:-1:0:4" \
-p "inclusive:inclusive:output/Paper_2013_3Hit/Embedded_mt_2012.root:inclusive_os_sel/:pt_tt:-1:0:6" \
--x_axis_title="Simple Higgs p_{T} [GeV]" --norm_mode=0  --big_label="#mu#tau_{h}" \
--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
--outname="embedded_pt_tt.pdf" --rebin=2


./bin/PlotCompare  \
-p "0jet_low:0jet_low:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:0jet_low_os_sel/:pt_h:-1:0:1" \
-p "0jet_high:0jet_high:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:0jet_high_os_sel/:pt_h:-1:0:2" \
-p "1jet_low:1jet_low:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:1jet_low_os_sel/:pt_h:-1:0:3" \
-p "1jet_high:1jet_high:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:1jet_high_os_sel/:pt_h:-1:0:4" \
--x_axis_title="Simple Higgs p_{T} [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
--outname="ggh_125_pt_h.pdf" --rebin=2

