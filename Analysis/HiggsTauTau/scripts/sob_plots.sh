## Examples

mass=125
et_input=limit_area/LIMITS-130924-sm/hww-bg/et
mt_input=limit_area/LIMITS-130924-sm/hww-bg/mt_only
em_input=limit_area/LIMITS-130924-sm/hww-bg/em
tt_input=limit_area/LIMITS-130924-sm/hww-bg/tt
postfit="true"
ignore_corrs="false"
lumi_title_7tev="CMS Preliminary, #sqrt{s} = 7 TeV, L = 4.9 fb^{-1}"
lumi_title_8tev="CMS Preliminary, #sqrt{s} = 8 TeV, L = 19.8 fb^{-1}"
blind="--blind=true --x_blind_min=100 --x_blind_max=160"
cfg="scripts/new_plot_sm_2012.cfg"

###################################### mu-tau 8TeV
./bin/SOBPlot --cfg=scripts/new_plot_sm_2012.cfg \
 --datacard_path=$et_input/$mass      --datacard_regex="htt_.._._.TeV.txt" \
 --root_file_path=$et_input/common  	--root_file_regex="htt.*root" \
 --pulls_file=$et_input/125/out/mlfit.txt \
 --signal_mass=$mass --postfit=true --mssm=false --norm_bins=true \
 --title_left="CMS Preliminary, 4.9 fb^{-1} at 7TeV, 19.7 fb^{-1} at 8TeV" \
 --blind=false --x_blind_min=100 --x_blind_max=160 \
 --background_scheme=all --signal_scheme=sm_default --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --split_zll=true \
 --text1="e#tau_{h}" --text2="all categories" \
 --output="sob_et_all" --ignore_corrs=$ignore_corrs

./bin/SOBPlot --cfg=scripts/new_plot_sm_2012.cfg \
 --datacard_path=$mt_input/$mass      --datacard_regex="htt_.._._.TeV.txt" \
 --root_file_path=$mt_input/common  	--root_file_regex="htt.*root" \
 --pulls_file=$mt_input/125/out/mlfit.txt \
 --signal_mass=$mass --postfit=true --mssm=false --norm_bins=true \
 --title_left="CMS Preliminary, 4.9 fb^{-1} at 7TeV, 19.7 fb^{-1} at 8TeV" \
 --blind=false --x_blind_min=100 --x_blind_max=160 \
 --background_scheme=all --signal_scheme=sm_default --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --split_zll=true \
 --text1="#mu#tau_{h}" --text2="all categories" \
 --output="sob_mt_all" --ignore_corrs=$ignore_corrs

./bin/SOBPlot --cfg=scripts/new_plot_sm_2012.cfg \
 --datacard_path=$em_input/$mass      --datacard_regex="htt_.._._.TeV.txt" \
 --root_file_path=$em_input/common  	--root_file_regex="htt.*root" \
 --pulls_file=$em_input/125/out/mlfit.txt \
 --signal_mass=$mass --postfit=true --mssm=false --norm_bins=true \
 --title_left="CMS Preliminary, 4.9 fb^{-1} at 7TeV, 19.7 fb^{-1} at 8TeV" \
 --blind=false --x_blind_min=100 --x_blind_max=160 \
 --background_scheme=all --signal_scheme=sm_default --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --split_zll=false \
 --text1="e#mu" --text2="all categories" \
 --output="sob_em_all" --ignore_corrs=$ignore_corrs

./bin/SOBPlot --cfg=scripts/new_plot_sm_2012.cfg \
 --datacard_path=$tt_input/$mass      --datacard_regex="htt_.._._.TeV.txt" \
 --root_file_path=$tt_input/common  	--root_file_regex="htt.*root" \
 --pulls_file=$tt_input/125/out/mlfit.txt \
 --signal_mass=$mass --postfit=true --mssm=false --norm_bins=true \
 --title_left="CMS Preliminary, 19.7 fb^{-1} at 8TeV" \
 --blind=false --x_blind_min=100 --x_blind_max=160 \
 --background_schtte=all --signal_scheme=sm_default --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --split_zll=false \
 --text1="#tau_{h}#tau_{h}" --text2="all categories" \
 --output="sob_tt_all" --ignore_corrs=$ignore_corrs
