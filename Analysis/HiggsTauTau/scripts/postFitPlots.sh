## Examples

mass=125
et_input=Paper-LIMITS/post-fit-sm-v1/et
mt_input=Paper-LIMITS/post-fit-sm-v1/mt
em_input=Paper-LIMITS/post-fit-sm-v1/em
do_et=false
do_mt=true
do_em=false
postfit="false"
lumi_title="CMS Preliminary, #sqrt{s} = 8 TeV, L = 19.8 fb^{-1}"
blind="--blind=true --x_blind_min=100 --x_blind_max=160"
cfg="scripts/new_plot_sm_2012.cfg"


./bin/PostFitPlot2 --channel=mt --selection="0-Jet Low:0" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=mt_with_zmm --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=mt --selection="0-Jet Medium:1" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=mt_with_zmm --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=mt --selection="0-Jet High:2" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=mt_with_zmm --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=mt --selection="1-Jet Medium:3" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=mt_with_zmm --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=mt --selection="1-Jet High, Low p_{T}^{#tau#tau}:4" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=mt_with_zmm --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=mt --selection="1-Jet High, High p_{T}^{#tau#tau}:5" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=mt_with_zmm --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2  --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=mt --selection="VBF Loose:6" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=mt_with_zmm --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.6  --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=mt --selection="VBF Tight:7" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=mt_with_zmm --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=2.0  --draw_ratio=true --signal_scheme=sm_default