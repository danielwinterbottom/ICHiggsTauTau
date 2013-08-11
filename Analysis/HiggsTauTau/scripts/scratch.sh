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
  --method=3 --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
  --blind=true --x_blind_min=100 --x_blind_max=160 --make_datacard=false --norm_bins=true --verbose=false \
  --paramfile=./scripts/Moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 --non_mass_plot=true  \
  --method=3 --category="jet_high" --plot_name="mt_1" --x_axis_label="m_{T} [GeV]" --extra_pad=1.15 \
  --paramfile=./scripts/Moriond_params_2012.dat --replace_os_sel="os"  --replace_ss_sel="ss"

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="1jet_high" --plot_name="met" --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
  --paramfile=./scripts/Moriond_params_2012.dat --extra_pad=1.1 

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=5 --non_mass_plot=true \
  --method=1 --category="0jet_high" --plot_name="pt_2"  --x_axis_label="Tau p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --draw_ratio=true \
  --paramfile=./scripts/Moriond_params_2012.dat

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=5 --non_mass_plot=true \
  --method=3 --category="1jet_high" --plot_name="pt_2"  --x_axis_label="Tau p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=./scripts/Moriond_params_2012.dat

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

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=1 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="met" --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
  --paramfile=./scripts/Moriond_params_2012.dat --extra_pad=1.1



### Plot the Higgs pT
./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="pt_h"  --x_axis_label="Higgs p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=./scripts/Moriond_params_2012.dat --signal_no_stack=true --log_y=true --draw_ratio=true \
  --exra_pad=3

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=et --rebin=2 --non_mass_plot=true \
  --method=8 --category="inclusive" --plot_name="pt_h"  --x_axis_label="Higgs p_{T} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=./scripts/Moriond_params_2012.dat --signal_no_stack=true --log_y=true --draw_ratio=true \
  --extra_pad=5

./bin/PlotCompare  \
-p "ggH125:ggH125:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:inclusive_os_sel/:pt_h:-1:2:1" \
-p "Embedded_ZTT:Embedded_ZTT:output/Paper_2013_3Hit/Embedded_mt_2012.root:inclusive_os_sel/:pt_h:-1:2:2" \
--x_axis_title=" p_{T}^{H} [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
--outname="ggh125_vs_embedded_pt_h.pdf" --rebin=2

# -p "1jet_low:1jet_low:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:1jet_low_os_sel/:pt_h:-1:0:3" \
# -p "1jet_high:1jet_high:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:1jet_high_os_sel/:pt_h:-1:0:4" \


./bin/PlotCompare  \
-p "ggH125:ggH125:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:hpt_60_1jet_high_os_sel/:m_sv_sm_fine:-1:2:1" \
-p "Embedded_ZTT:Embedded_ZTT:output/Paper_2013_3Hit/Embedded_mt_2012.root:hpt_60_1jet_high_os_sel/:m_sv_sm_fine:-1:2:2" \
--x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
--outname="ggh125_vs_embedded_hpt_60_1jet_high.pdf" --rebin=2 \
--ratios="ggH125/Embedded_ZTT/1" --ratio_axis_label="ggH/ZTT" --ratio_y_min=0 --ratio_y_max=20

./bin/PlotCompare  \
-p "ggH125:ggH125:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:jpt_60_1jet_high_os_sel/:m_sv_sm_fine:-1:2:1" \
-p "Embedded_ZTT:Embedded_ZTT:output/Paper_2013_3Hit/Embedded_mt_2012.root:jpt_60_1jet_high_os_sel/:m_sv_sm_fine:-1:2:2" \
--x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
--outname="ggh125_vs_embedded_jpt_60_1jet_high.pdf" --rebin=2 \
--ratios="ggH125/Embedded_ZTT/1" --ratio_axis_label="ggH/ZTT" --ratio_y_min=0 --ratio_y_max=20

./bin/PlotCompare  \
-p "ggH125:ggH125:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:hpt_60_0jet_high_os_sel/:m_sv_sm_fine:-1:2:1" \
-p "Embedded_ZTT:Embedded_ZTT:output/Paper_2013_3Hit/Embedded_mt_2012.root:hpt_60_0jet_high_os_sel/:m_sv_sm_fine:-1:2:2" \
--x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
--outname="ggh125_vs_embedded_hpt_60_0jet_high.pdf" --rebin=2 \
--ratios="ggH125/Embedded_ZTT/1" --ratio_axis_label="ggH/ZTT" --ratio_y_min=0 --ratio_y_max=20

./bin/PlotCompare  \
-p "ggH125:ggH125:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:hpt_30_0jet_high_os_sel/:m_sv_sm_fine:-1:2:1" \
-p "Embedded_ZTT:Embedded_ZTT:output/Paper_2013_3Hit/Embedded_mt_2012.root:hpt_30_0jet_high_os_sel/:m_sv_sm_fine:-1:2:2" \
--x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
--outname="ggh125_vs_embedded_hpt_30_0jet_high.pdf" --rebin=2 \
--ratios="ggH125/Embedded_ZTT/1" --ratio_axis_label="ggH/ZTT" --ratio_y_min=0 --ratio_y_max=20

./bin/PlotCompare  \
-p "ggH125:ggH125:output/Paper_2013_3Hit/GluGluToHToTauTau_M-125_mt_2012.root:jpt_60_0jet_high_os_sel/:m_sv_sm_fine:-1:2:1" \
-p "Embedded_ZTT:Embedded_ZTT:output/Paper_2013_3Hit/Embedded_mt_2012.root:jpt_60_0jet_high_os_sel/:m_sv_sm_fine:-1:2:2" \
--x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
--outname="ggh125_vs_embedded_jpt_60_0jet_high.pdf" --rebin=2 \
--ratios="ggH125/Embedded_ZTT/1" --ratio_axis_label="ggH/ZTT" --ratio_y_min=0 --ratio_y_max=20

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg --channel=mt --rebin=1 \
  --method=3 --category="inclusive" --plot_name="pt_tt" \
  --x_axis_label="M_{#tau#tau} [GeV]" --norm_bins=true \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --paramfile=./scripts/Moriond_params_2012.dat --replace_os_sel="os_con"  --replace_ss_sel="ss_con"

./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2011.cfg  --tau_scale_mode=0 --channel=em --rebin=1 \
  --method=8 --plot_name=m_vis_sm_fine  --x_axis_label="m_{#tau#tau} [GeV]"
  --make_datacard=false --norm_bins=true --verbose=true
  --paramfile=scripts/Paper_params_2011.dat --folder=output/Paper_2013/



./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_sm_2012.cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="n_jets==0" --var="pt_tt(40,0,200)" --extra_pad=1.1\
  --x_axis_label="Tau p_{T} [GeV]" --datacard="inclusive" \
  --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="mt_default" --signal_scheme="sm_default" --draw_ratio=true




./bin/PlotCompare  \
-p "Inclusive:Inclusive:htt_mt.inputs-sm-8TeV-wjets.root:muTau_0jet_high/:W:-1:2:1" \
-p "Soup:Soup:collected/Imperial/htt_mt.inputs-sm-8TeV.root:muTau_0jet_high/:W:-1:2:2" \
--x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
--outname="muTau_0jet_high_W.pdf"  \
--ratios="Inclusive/Soup/1" --ratio_axis_label="Inclusive/Soup" --ratio_y_min=0 --ratio_y_max=2




./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_sm_2012.cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_vis(40,70,130)" --cat="tau_decay_mode==1 && abs(eta_2)<1.5" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]"  \
  --norm_bins=true --datacard="inclusive"\
  --background_scheme="et_default" --signal_scheme="sm_default"


./bin/PlotCompare  \
-p "Ztt:Ztt:htt_em.inputs-mssm-8TeV-mvis.root:emu_btag/:Ztt:-1:2:1" \
-p "ttbarEmbedded:ttbarEmbedded:htt_em.inputs-mssm-8TeV-mvis.root:emu_btag/:ttbarEmbedded:-1:2:2" \
--x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=1  --big_label="#mu#tau_{h}" \
--outname="emu_btag.pdf"


./bin/PlotCompare  \
-p "ZTT:ZTT:output/Paper_2012_Vegas/DYJetsToTauTauSoup_et_2012.root:inclusive_os_sel/:m_sv:-1:2:1" \
-p "ZTT-L:ZTT-L:output/Paper_2012_Vegas/DYJetsToTauTau-LSoup_et_2012.root:inclusive_os_sel/:m_sv:-1:2:2" \
--x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=1  --big_label="e#tau_{h}" \
--outname="etau_check.pdf"

./bin/PlotCompare  \
-p "Ztt:Ztt:htt_em.inputs-mssm-8TeV-mvis.root:emu_btag/:Ztt:-1:2:1" \
-p "ttbarEmbedded:ttbarEmbedded:htt_em.inputs-mssm-8TeV-mvis.root:emu_btag/:ttbarEmbedded:-1:2:2" \
--x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=0  --big_label="#mu#tau_{h}" \
--outname="emu_btag.pdf"

./bin/PlotCompare  \
-p "powheg:powheg:output/Paper_2012_Vegas/GluGluToHToTauTau_M-125_mt_2012.root:inclusive_os_sel/:n_jets:-1:2:1" \
-p "minlo:minlo:output/Paper_2012_Vegas/GluGluToHToTauTau_M-125-minloHJJ_mt_2012.root:inclusive_os_sel/:n_jets:-1:2:2" \
--x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
--outname="powheg_vs_minlo_n_jets_reco.pdf" \
--ratios="minlo/powheg/1" --ratio_axis_label="minlo/powheg" --ratio_y_min=0.5 --ratio_y_max=1.5

./bin/PlotCompare  \
-p "110:m_{H} = 110 GeV:htt_em.inputs-sm-8TeV.root:emu_vbf_tight/:qqH_hww110:-1:2:1" \
-p "125:m_{H} = 125 GeV:htt_em.inputs-sm-8TeV.root:emu_vbf_tight/:qqH_hww125:-1:2:2" \
-p "145:m_{H} = 145 GeV:htt_em.inputs-sm-8TeV.root:emu_vbf_tight/:qqH_hww145:-1:2:3" \
--x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=3  --big_label="e#mu" \
--outname="hww_shape.pdf"

htt_em.inputs-sm-8TeV.root
./bin/PlotCompare  \
-p "ZTT:ZTT(all):output/Paper_2012_Vegas/DYJetsToTauTauSoup_et_2012.root:inclusive_os_sel/:m_sv:-1:2:1" \
-p "ZTT-J:ZTT(veto-#tau_{h}-lepton-match):output/Paper_2012_Vegas/DYJetsToTauTau-JSoup_et_2012.root:inclusive_os_sel/:m_sv:-1:2:2" \
--x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=0  --big_label="e#tau_{h}" \
--outname="etau_diff.pdf" \
--ratios="ZTT/ZTT-J/1" --ratio_axis_label="Ratio" --ratio_y_min=0.9 --ratio_y_max=1.1

./bin/PlotCompare  \
-p "ZTT:ZTT(all):output/Paper_2012_Vegas/DYJetsToTauTauSoup_mt_2012.root:inclusive_os_sel/:m_sv:-1:2:1" \
-p "ZTT-J:ZTT(veto-#tau_{h}-lepton-match):output/Paper_2012_Vegas/DYJetsToTauTau-JSoup_mt_2012.root:inclusive_os_sel/:m_sv:-1:2:2" \
--x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=0  --big_label="#mu#tau_{h}" \
--outname="mutau_diff.pdf" \
--ratios="ZTT/ZTT-J/1" --ratio_axis_label="Ratio" --ratio_y_min=0.9 --ratio_y_max=1.1

./bin/PrintShifts --channel=et --selection="inclusive:0" --eras="8TeV" \
  --datacard_path=Paper-LIMITS/post-fit-sm-v3/cmb/125 \
  --root_file_path=Paper-LIMITS/post-fit-sm-v3/cmb/common \
  --pulls_file=Paper-LIMITS/post-fit-sm-v3/cmb/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=et --selection="1jet:3" --eras="8TeV" \
  --datacard_path=Paper-LIMITS/post-fit-sm-v3/cmb/125 \
  --root_file_path=Paper-LIMITS/post-fit-sm-v3/cmb/common \
  --pulls_file=Paper-LIMITS/post-fit-sm-v3/cmb/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=et --selection="twojet:6" --eras="8TeV" \
  --datacard_path=Paper-LIMITS/post-fit-sm-v3/cmb/125 \
  --root_file_path=Paper-LIMITS/post-fit-sm-v3/cmb/common \
  --pulls_file=Paper-LIMITS/post-fit-sm-v3/cmb/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=mt --selection="inclusive:0" --eras="8TeV" \
  --datacard_path=Paper-LIMITS/post-fit-sm-v3/cmb/125 \
  --root_file_path=Paper-LIMITS/post-fit-sm-v3/cmb/common \
  --pulls_file=Paper-LIMITS/post-fit-sm-v3/cmb/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=mt --selection="1jet:3" --eras="8TeV" \
  --datacard_path=Paper-LIMITS/post-fit-sm-v3/cmb/125 \
  --root_file_path=Paper-LIMITS/post-fit-sm-v3/cmb/common \
  --pulls_file=Paper-LIMITS/post-fit-sm-v3/cmb/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=mt --selection="twojet:6" --eras="8TeV" \
  --datacard_path=Paper-LIMITS/post-fit-sm-v3/cmb/125 \
  --root_file_path=Paper-LIMITS/post-fit-sm-v3/cmb/common \
  --pulls_file=Paper-LIMITS/post-fit-sm-v3/cmb/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=em --selection="inclusive:0" --eras="8TeV" \
  --datacard_path=Paper-LIMITS/post-fit-sm-v3/cmb/125 \
  --root_file_path=Paper-LIMITS/post-fit-sm-v3/cmb/common \
  --pulls_file=Paper-LIMITS/post-fit-sm-v3/cmb/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=em --selection="1jet:2" --eras="8TeV" \
  --datacard_path=Paper-LIMITS/post-fit-sm-v3/cmb/125 \
  --root_file_path=Paper-LIMITS/post-fit-sm-v3/cmb/common \
  --pulls_file=Paper-LIMITS/post-fit-sm-v3/cmb/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=em --selection="twojet:4" --eras="8TeV" \
  --datacard_path=Paper-LIMITS/post-fit-sm-v3/cmb/125 \
  --root_file_path=Paper-LIMITS/post-fit-sm-v3/cmb/common \
  --pulls_file=Paper-LIMITS/post-fit-sm-v3/cmb/125/out/mlfit.txt --signal_mass=125

./bin/PlotCompare  \
-p "minlo-hres:minlo+HRes:GluGluToHToTauTau_M-125-minloHJJ-WithHRes_mt_2012.root:ggh_study/:njets:-1:2:8" \
-p "minlo:minlo:GluGluToHToTauTau_M-125-minloHJJ_mt_2012.root:ggh_study/:njets:-1:2:2" \
-p "hres:powheg+HRes:GluGluToHToTauTau_M-125_WithHRes_mt_2012.root:ggh_study/:njets:-1:2:4" \
-p "powheg:powheg:GluGluToHToTauTau_M-125_mt_2012.root:ggh_study/:njets:-1:2:1" \
--x_axis_title="Number of Jets" --norm_mode=3  --big_label="Generator" \
--outname="powheg_vs_minlo_n_jets.pdf" \
--ratios="minlo/powheg/2:hres/powheg/4:minlo-hres/powheg/8" --ratio_axis_label="Ratio" --ratio_y_min=0.5 --ratio_y_max=1.5

./bin/PlotCompare  \
-p "minlo:minlo:GluGluToHToTauTau_M-125-minloHJJ_mt_2012.root:ggh_study/:njets:-1:2:2" \
-p "hres:powheg+HRes:GluGluToHToTauTau_M-125_WithHRes_mt_2012.root:ggh_study/:njets:-1:2:4" \
-p "powheg:powheg:GluGluToHToTauTau_M-125_mt_2012.root:ggh_study/:njets:-1:2:1" \
--x_axis_title="Number of Jets" --norm_mode=3  --big_label="Generator" \
--outname="powheg_vs_minlo_n_jets.pdf" \
--ratios="minlo/powheg/2:hres/powheg/4" --ratio_axis_label="Ratio" --ratio_y_min=0.5 --ratio_y_max=1.5


./bin/PlotCompare  \
-p "minlo:minlo:GluGluToHToTauTau_M-125-minloHJJ_mt_2012.root:ggh_study/:h_pt:-1:2:2" \
-p "hres:powheg+HRes:GluGluToHToTauTau_M-125_WithHRes_mt_2012.root:ggh_study/:h_pt:-1:2:4" \
-p "powheg:powheg:GluGluToHToTauTau_M-125_mt_2012.root:ggh_study/:h_pt:-1:2:1" \
--x_axis_title="Higgs p_{T} [GeV]" --norm_mode=3  --big_label="Generator" \
--outname="powheg_vs_minlo_hpt.pdf" --rebin=5 \
--ratios="minlo/powheg/2:hres/powheg/4" --ratio_axis_label="Ratio" --ratio_y_min=0.5 --ratio_y_max=1.5


./bin/SOBPlot --cfg=scripts/new_plot_sm_2012.cfg \
 --datacard_regex="htt_.._._.TeV.txt" --root_file_regex="htt.*root" \
 --datacard_path=Paper-LIMITS/sm-v1/mt/125/ --root_file_path=Paper-LIMITS/sm-v1/mt/common/ --pulls_file=Paper-LIMITS/sm-v1/mt/125/out/mlfit.txt \
 --signal_mass=125 --postfit=false --mssm=false --log_y=false \
 --title_left="CMS Preliminary, #sqrt{s} = 8 TeV, L = 19.8 fb^{-1}" \
 --blind=true --x_blind_min=100 --x_blind_max=160 --norm_bins=true \
 --background_scheme=all --signal_scheme=sm_default --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true






